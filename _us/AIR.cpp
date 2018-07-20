/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.77
      Delta Created: 16/04/27 - 17:03:30
         Last Delta: app/src/common/InterfaceRecord/_us/ 4/27/16 - 17:03:30 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.AIR.cpp) 
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us/}[4/27/16-17:03:30 1.77] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.AIR.cpp (Daniel Wu)\n" )
/*
  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/
/*
PURPOSE:
============================================================================
CHANGE HISTORY: ( PLEASE PUT MOST RECENT CHANGE ENTRY ALWAYS ON TOP )
============================================================================
DATE:       SCR#: TASKID: INIT DESCIPTION
-------   ------  ------- ---- ---------------------------------------------
20APR2018 US664    -       KS  Record Issue date for ticket
20APR2018 US662    -       KS  Car rate not getting picked up, Parse RG and RQ
11APR2018 US682    -       MG  Domestic HTL Seg, Booked rate parsing
22MAR2018 US663    -       MG  Car/Hotel Status correction
20MAR2018 US661    -       MG  Booked Rate, Local rate correction
22AUG2014 518200  v7991    JL  TASF check sup_id XD
30JUL2014 517902  v7966    JL  Virtuoso remark, see also IUR.cpp, MIR.cpp
29JUL2014 518200  v7969    JL  implement TSAF, copied from _CA except the sup_id is XD
24FEB014  516355  c1395    JL  migrated changes from _ca (a9614 1.61, a9748 1.66)
23JAN2014 516353  v7825    JL  fixing two ^M (\015 char) issues for AIR parser loading. as in SCR: 516167
04AMR2013 510258  v7594    WW  Capture fare basis from Amadeus Interface Record
10JUL2012 510266  v7445    WW  Add conjunction ticket for exchange ticket
04JUL2012 509695  v7438    WW  Add pragma comment
29JUN2012 509695  v7438    WW  Ignore "hand ticket" data if it is exchange ticket 
============================================================================
*/

#include "gtypes.h"
#include "AIR_D.h"

#include "C.h"
#include "dbIR.h"
#include "Money.h"
#include "MsgHandler.h"
#include "matsysT.h"
#include "dbMatrix.h"
#include "List.h"
#include "DeleteList.h"
#include "airCommon.h"
#include "Supplier.h"
#include "dbSupplier.h"

#include "AIR.h"
#include "Log.h"
#include "pcre_api.h" //US662

#define ENTER      LOG(logINFO) << "\n*** Enter>> " << __FUNCTION__ ;
#define EXIT       LOG(logINFO) << "\n*** Exit << " << __FUNCTION__ ;
#define RETURN(a)  LOG(logINFO) << "\n*** Exit << " << __FUNCTION__ ;return(a);

CAIR::CAIR () : mva_IR_PSGRNBR(100,100), mva_IT_PNRSTAT(4,500), mva_IT_BAGALL(3,500)
{
   Log::ReportingLevel() = PARSER_LOG_LEVEL; // logDEBUG; PARSER_LOG_LEVEL;

	md_interface_type	= "AIR";
	md_IRT_crs_id		= "AMD";
	md_queType			= "RMA";

	for (unsigned i = 0; i < 500; i++)
	{
		mi_IT_SEGNBR[i] = 0;
		mi_IR_SEGNBR[i] = 0;
		mi_IO_SEGNBR[i] = 0;

      // SCR: 516355 initialize to prevent core dump.
		mi_TicketedSeg[i] = 0;

	}
}

void
CAIR::createAIR(const D_session_id &pd_session_id,
					 const AB_Date &pd_creation_date,
					 const AB_Char &pc_from)
{
	CairRecord		lo_a;
	CDMatrix       lo_Matrix;
	AB_Integer     li_sqlcde;

	md_session_id		= pd_session_id;
	md_creation_date	= pd_creation_date;


	if ( pc_from == "G")
	{
		lo_a.processAIR ("GIRTABLES",
							  pd_session_id,
							  pd_creation_date);
	}
	else
	{
		lo_a.processAIR ("MESSAGE",
							  pd_session_id,
							  pd_creation_date);
	}
						  
	/* lo_a is filled in by parsed data */

	/* REMARK_PARSER: parse ALL remarks */
	RP_parseRemarks(lo_a);

	/* populate Interface Arrays     */

	if (CMsgHandler::getMsgStatus())
	{
		populateIrArrays(lo_a);
	}

	md_queType = "RMA" ;

	if (CMsgHandler::getMsgStatus())
	{
		LOG(logDEBUG) << "\nMEL---> before createIrRowscreateIrRows \n" ;
		createIrRows() ;
		LOG(logDEBUG) << "\nMEL---> after createIrRowscreateIrRows \n" ;
	}

	/// TEST ERROR TO CREATE RMA REMOVE!!!!!!!!!!!!!!!!!!
	//CMsgHandler::setMsg(3957, " ", "TU-PARSE", 1, AB_Varchar(20,"TU"));

	if (! CMsgHandler::getMsgStatus())
	{
		lo_Matrix.dbRollback(li_sqlcde) ;
		errorHandler(lo_a);
		return;
	}
	else
	{
      if ( mi_rejectirId > AB_Integer(0) )
      {
          // Insert captured irregularities of service fee processing
          // into Rejectir

          saveSvcIrregularity();

          mi_rejectirId = AB_Integer(0);
      }

		lo_Matrix.dbCommit(li_sqlcde);
	}

	md_queType = "RMI" ;
	checkDomainEdits() ;

	LOG(logDEBUG) << "\nMK---> msg_id = " << CMsgHandler::getMsgId() << "\n" ;

	if (CMsgHandler::getMsgStatus())
	{
		createClientfile() ;
	}

	if (! CMsgHandler::getMsgStatus())
	{
		lo_Matrix.dbRollback(li_sqlcde);
		if (pc_from == "G") 
		{
		   TRM_errorHandler();
			return;
		}
		else
		{
			errorHandler(lo_a);
			return;
		}
	}

	LOG(logDEBUG) << "*****S U C C E S S  -  CREATING REAL DATA ROWS*****" << endl ;
	if ( isTRMBatchMode() )
	{
			TRM_Processor__updateQueueString("status", "C");
			TRM_Processor__updateQueueStringStringString("cop_id", md_rejectCop.to_string(), 
																	"bra_id", md_rejectBra.to_string(), 
																	"con_id", md_con_id.to_string());
			TRM_Processor__updateQueueString("cfa_type",          md_cfa_type.to_string());
			TRM_Processor__updateQueueString("cfa_code",          md_cfa_code.to_string());
			TRM_Processor__updateQueueInt   ("cfa_id",            md_cfa_id);
	}

	lo_Matrix.dbCommit(li_sqlcde);

}

void
CAIR::processRMA(D_session_id        &pd_session_id,
					  AB_Date             &pd_lcl_date,
					  D_client_file_type  &pd_cfa_type,
					  D_client_file_code  &pd_cfa_code,
					  D_client_file_no    &pd_cfa_id,
					  D_person_name       &pd_last_name,
					  D_person_name       &pd_first_name,
					  D_consultant        &pd_con_id,
					  D_company           &pd_cfaCop,
					  D_branch            &pd_cfaBra)
{
	createAIR(pd_session_id,pd_lcl_date,"G");

	pd_cfaCop 			= md_rejectCop ;
	pd_cfaBra 			= md_rejectBra ;

	if (CMsgHandler::getMsgStatus())
	{
		pd_cfa_type   	= md_cfa_type;
		pd_cfa_code   	= md_cfa_code;
		pd_cfa_id     	= md_cfa_id;
		pd_last_name  	= md_last_name;
		pd_first_name 	= md_first_name;
		pd_con_id     	= md_con_id;
	}
}

void CAIR::TRM_errorHandler()
{
   D_description lo_errorDescription ;
   CDMatrix lo_Matrix;
   AB_Integer li_sqlcode;

   CMsgHandler::getMsgDescription(CMsgHandler::getMsgId(), "E", lo_errorDescription) ;
   
   LOG(logERROR) << " ERROR CREATING GIR TABLES. Write Girrawheader, Girrawdetail, and Queue"  ;
   LOG(logERROR) << "ERROR Description: " << lo_errorDescription  ; 

	if ( isTRMBatchMode() )
	{
		TRM_Processor__updateQueueString("status", "E");
		TRM_Processor__updateQueueString("que_type", "RMI");
		TRM_Processor__updateQueueString("description", lo_errorDescription);
	}
	else
	{
		createQueue(lo_errorDescription) ;
	}

   lo_Matrix.dbCommit(li_sqlcode);
}

void
CAIR::errorHandler(CairRecord &po_a)
{
	D_description   ld_errorDescription;
   AB_Integer      li_msgId ;

   li_msgId = CMsgHandler::getMsgId() ;

   CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;

	ld_errorDescription.leftJustify() ;

	CMsgHandler::clearMsg();

	po_a.createGirTables(md_session_id,md_creation_date);

	if ( isTRMBatchMode() )
	{
		TRM_Processor__updateQueueString("status", "E");
		TRM_Processor__updateQueueString("que_type", "RMI");
		TRM_Processor__updateQueueString("description", ld_errorDescription);
	}
	else
	{
		createQueue(ld_errorDescription) ;
	}

}

void
CAIR::populateIrArrays(CairRecord &po_a)
{
	populateBaseVars(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrtripheader(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIraddress(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrperson(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrticket(po_a);

	if (CMsgHandler::getMsgStatus())
		processNonArcDoc(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrtransport(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrother(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrfrequentuser(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrrental(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrconsumdetail(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrreceipt(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrdistribution(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIremailaddress(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrremark(po_a);

	if (CMsgHandler::getMsgStatus())
		populateNetTicketInfo(po_a);

	if (CMsgHandler::getMsgStatus())
		populateMembership(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrtaxdetail(po_a);

	if (CMsgHandler::getMsgStatus())
		populateIrpayment(po_a);

	if (CMsgHandler::getMsgStatus())
		processSFC(po_a);

	if (CMsgHandler::getMsgStatus())
	  processSFM_SFO_SFA_SFN(po_a);

	if (CMsgHandler::getMsgStatus())
		processMCO(po_a);

	if (CMsgHandler::getMsgStatus())
		processEMD(po_a);

}

void
CAIR::processNonArcDoc(CairRecord &po_a) {
	ENTER

	int li_nbsp_cnt = 0;
	for (unsigned  i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM.size(); i++)
	{
		AB_Varchar	lv_remark(255);
		AB_Varchar  lv_association(70);
		lv_remark = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK;

		if (lv_remark.For(4) != "*MCA" &&
			 lv_remark.For(4) != "*MCC" &&
			 lv_remark.For(4) != "*MAR") {
			continue;
		}

		int li_pos1, li_pos2;
		li_pos1 = lv_remark.findSubstring("/S");

		if (li_pos1 > 0) {
			li_pos2 = lv_remark.findSubstring("/", li_pos1+2);

			if (li_pos2 == 0) {
				lv_association = lv_remark.At(li_pos1+1);
				lv_remark = lv_remark.For(li_pos1-1);
			} else {
				lv_association = lv_remark.At(li_pos1+1).For(li_pos2-li_pos1-1);
				lv_remark = lv_remark.For(li_pos1-1) + "/" + lv_remark.At(li_pos2+1);
			}
		}

		md_ID_line_number[mi_ID_I] = mi_ID_I + 1;
		md_ID_ticket_no[mi_ID_I]   = "0000000000" ;

		if (li_nbsp_cnt < mi_IP_I) {
			md_ID_pax_id[mi_ID_I] = md_IP_pax_id[li_nbsp_cnt];
		} else {
			md_ID_pax_id[mi_ID_I] = md_IP_pax_id[0];
		}

		if (processNonArcCost(po_a, lv_remark)) {
			li_nbsp_cnt++;

			md_ID_doc_type[mi_ID_I] 	= "TKT" ;
			md_ID_reference[mi_ID_I]	= po_a.mo_C4010AMD.RECLOC;
			md_ID_pic[mi_ID_I] 			= " ";

			md_ID_saving_code[mi_ID_I]		= retFromRM(po_a,"FS",0);
			md_ID_full_fare[mi_ID_I]		= retFromRM(po_a,"FF",0);
			md_ID_low_prac_fare[mi_ID_I]	= retFromRM(po_a,"LP",0);
	
			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[mi_ID_I].to_long()]->mo_CFTGrp.mo_C4120FT.size() > 0) {
				md_ID_crs_tour[mi_ID_I]	= po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[mi_ID_I].to_long()]->mo_CFTGrp.mo_C4120FT[0]->TOUR;
			}

			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[mi_ID_I].to_long()]->mo_C4098T_.TKTTYPE == "E") {
				md_ID_e_ticket[mi_ID_I]	= "Y";
			}

			// md_ID_personal_crs[mi_ID_I] 	= ;
			// md_ID_stp_home_city[mi_ID_I] 	= ;
			// md_ID_stp_host_city[mi_ID_I] 	= ;
			md_ID_rebate[mi_ID_I]				= retFromRM(po_a,"RB",0);

			md_ID_form[mi_ID_I]        = AB_null ;
			md_ID_no_of_books[mi_ID_I] = AB_Integer(1) ;
			md_ID_association[mi_ID_I]	= lv_association;

			mi_ID_I++;
		}
	}
	EXIT
}

AB_Boolean
CAIR::processNonArcCost(CairRecord &po_a, const AB_Varchar&  pv_remark)
{
	int					li_pos; 
	int					li_count; 
   AB_Text          	lt_segment ;
   V_AB_Varchar     	lva_arrayField(100,100) ;
	D_amount				ld_value;
	D_amount				ld_XT;
	AB_Char        	lc_tax_id(2);
	AB_Char        	lc_amount(13);
	AB_Varchar			lv_sup_id(10);
	int					li_ITD_I;
	AB_Boolean 			lb_result;
	AB_Boolean 			lb_TK_found;
	int					li_TK_index;
	int 					index;

	lb_result = AB_false;

	li_ITD_I = mi_ITD_I;

	ld_XT = D_amount(0);

   md_ID_cur_id[mi_ID_I] = AB_null;

   li_count = lva_arrayField.parseStringMultDel(pv_remark,"/") ;

	lv_sup_id = lva_arrayField[1].At(6).to_string();

	if ( 	lv_sup_id.IsNull()		||
			lv_sup_id.length() > 3 	|| 
			lv_sup_id.length() < 2 )
   	return AB_false;

   md_ID_sup_id[mi_ID_I] = lv_sup_id;

	lb_TK_found = AB_false;
	li_TK_index = li_count;

	for ( index = 2; index <= li_count; index++ )
	{
		lva_arrayField[index].leftJustify();
		lva_arrayField[index].removeLeadAndTrailBlanks();

		if ( 	lb_TK_found == AB_true	&&
				lva_arrayField[index].For(3) != "TK-" )
		{
			mi_ITD_I = li_ITD_I;
         return AB_false;
		}

		if ( 	lb_TK_found == AB_false	&&
				lva_arrayField[index].For(3) == "TK-" )
		{
			li_pos = lva_arrayField[index].At(4).findFirstCharNotInList("0123456789");

			if ( li_pos == 0 && lva_arrayField[index].At(4).NotIsNull() )
      		md_ID_ticket_no[mi_ID_I] = lva_arrayField[index].At(4);

			li_TK_index = index - 1;
			lb_TK_found = AB_true;
		}
	}

	li_count = li_TK_index;

	for ( index = 2; index <= li_count; index++ )
	{
		lb_result = AB_true;

		lva_arrayField[index].leftJustify();
		lva_arrayField[index].removeLeadAndTrailBlanks();

		if ( index == 2 )
		{
			lc_amount = lva_arrayField[index];

			ld_value = ld_value.input_conversion_trigger(lc_amount);

         if (! CMsgHandler::getMsgStatus())
         {
         	CMsgHandler::clearMsg();
				mi_ITD_I = li_ITD_I;
         	return AB_false;
         }
         
			if ( ld_value < D_amount(0) )
         {
				mi_ITD_I = li_ITD_I;
         	return AB_false;
         }

         md_ID_base_cost[mi_ID_I] = ld_value;
		}
		else
		{
			li_pos = lva_arrayField[index].findFirstCharNotInList("0123456789.");

			if ( li_pos == 0 || li_pos == 1 )
			{
				if ( index != li_count ||
					( li_pos == 1 && lva_arrayField[index].For(1) != "C" ) )
				{
					mi_ITD_I = li_ITD_I;
					return AB_false;
				}

				if ( li_pos == 0 )
					lc_amount = lva_arrayField[index];
				else
					lc_amount = lva_arrayField[index].At(2);

				ld_value = ld_value.input_conversion_trigger(lc_amount);

         	if (! CMsgHandler::getMsgStatus())
         	{
         		CMsgHandler::clearMsg();
					mi_ITD_I = li_ITD_I;
         		return AB_false;
         	}

				if ( ld_value < D_amount(0) )
         	{
					mi_ITD_I = li_ITD_I;
         		return AB_false;
         	}

   			if (li_pos == 1 )
   			{
      			md_ID_comm_rate[mi_ID_I]   = ld_value;
      			md_ID_commission[mi_ID_I]  = AB_null;
   			}
   			else
   			{
      			md_ID_comm_rate[mi_ID_I]   = AB_null;
      			md_ID_commission[mi_ID_I]  = ld_value;
   			}
			}
			else
			{
				lt_segment = lva_arrayField[index].to_string();

				if ( lt_segment.length() < 3 ||
					  lt_segment.At(li_pos).length() != 2 )
				{
					mi_ITD_I = li_ITD_I;
					return AB_false;
				}

				lc_amount = lva_arrayField[index].For(li_pos-1);

				ld_value = ld_value.input_conversion_trigger(lc_amount);

         	if (! CMsgHandler::getMsgStatus() )
         	{
         		CMsgHandler::clearMsg();
					mi_ITD_I = li_ITD_I;
         		return AB_false;
         	}

				if ( ld_value < D_amount(0) )
         	{
					mi_ITD_I = li_ITD_I;
         		return AB_false;
         	}

				lc_tax_id = lva_arrayField[index].At(li_pos).For(2);

				if ( lc_tax_id == "XT" && ld_value > D_amount(0) )
				{
					ld_XT = ld_XT + ld_value;
				}
				else
				{
					if ( ld_value > D_amount(0) )
         		{
         			md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
         			md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
         			md_ITD_tax_amount[mi_ITD_I]   = ld_value ;
						md_ITD_tax_id[mi_ITD_I] 		= lc_tax_id;

         			mi_ITD_I++;
         		}
				}
			}
		}
	}

	if ( lb_result == AB_false )
	{
		mi_ITD_I = li_ITD_I;
      return AB_false;
	}

	while ( 	( ( mi_ITD_I - li_ITD_I ) > 6	&& ld_XT == D_amount(0) )	||
				( ( mi_ITD_I - li_ITD_I ) > 5	&& ld_XT  > D_amount(0) ) )
	{
		mi_ITD_I--;

		ld_XT = ld_XT + D_amount(md_ITD_tax_amount[mi_ITD_I]);

		md_ITD_line_number[mi_ITD_I] 	= AB_null;
		md_ITD_pax_id[mi_ITD_I]			= AB_null;
		md_ITD_tax_amount[mi_ITD_I]	= AB_null;
		md_ITD_tax_id[mi_ITD_I]			= AB_null;

	}

	if ( ld_XT > D_amount(0) )
   {
   	md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
      md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
      md_ITD_tax_amount[mi_ITD_I]   = ld_XT ;
		md_ITD_tax_id[mi_ITD_I] 		= "XT";

      mi_ITD_I++;
   }

   // Find out Form of payment
	populateIrpayment(po_a, mi_ID_I, pv_remark.At(3).For(2));

   mi_IRP_I++ ;

	return lb_result;
}

AB_Text
CAIR::retSupPrimDoc(const AB_Varchar &pv_sup_id)
{
	CDSupplier     lo_Supplier;
	CSupplierRead  lo_SupplierRead;

	lo_SupplierRead.pc_sup_id = pv_sup_id;

	if ( CMsgHandler::getMsgStatus())
	{
		return lo_SupplierRead.pc_prim_doc;
	}

	return AB_null;
}

AB_Text
CAIR::retSupName(const AB_Varchar &pv_sup_id)
{
	CDSupplier 		lo_Supplier;
	CSupplierRead 	lo_SupplierRead;

	lo_SupplierRead.pc_sup_id = pv_sup_id;

	lo_Supplier.read(lo_SupplierRead);

	if ( CMsgHandler::getMsgStatus())
	{
		return lo_SupplierRead.pv_sup_name;
	}

	return AB_null;
}

AB_Text
CAIR::retSavingCodeFromRM(CairRecord &po_a,const AB_Text pt_what,const int &pax_id)
{
	for (unsigned  i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM.size(); i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.For(pt_what.length()+2) == "*" + pt_what + "/")
		{
		int li_pos = po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.findSubstring("SV-");
		LOG(logDEBUG)  << "DEBUG retSavingCode " << li_pos;
		if (li_pos > 0 && li_pos + 3 <=  po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.length()) {
			LOG(logDEBUG)  << "DEBUG savingCode " << po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.At(li_pos+3).For(1);
			return po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.At(li_pos+3).For(1);
			}
		}
	}
	return AB_null;
}

AB_Text
CAIR::retFromRM(CairRecord &po_a,const AB_Text pt_what,const int &pax_id)
{
	for (unsigned  i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM.size(); i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.For(pt_what.length()+2) == "*" + pt_what + "/")
		{
			return po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.At(pt_what.length()+3);
		}
	}
	return AB_null;
}

void 
CAIR::populateBaseVars(CairRecord &po_a)
{
	md_txn_id			= "RIRA";
	mt_lclPnrDate		= po_a.mo_C4015D_.PNRDTE.from_stringFormat("YYMMDD");
}

void 
CAIR::populateIrtripheader(CairRecord &po_a)
{
	AB_Varchar		lv_cfa_id(7);
	AB_Varchar		lv_cfa(11);

	lv_cfa					= retFromRM(po_a,"CF",0);

	LOG(logDEBUG) << "\nCAIR: lv_cfa= " << lv_cfa  ;

	int cfaTypePos =lv_cfa.At(4).findFirstCharNotInList("0123456789");

	LOG(logDEBUG) << "\nCAIR: cfaTypePos= " << cfaTypePos  ;

	//lv_cfa_id				= lv_cfa.At(4).For(lv_cfa.length() - 4);
	//cfaTypePos = 1 means that there is no numeric cfa_id
	//cfaTypePos = 0 means that lv_cfa is invalid and that there is no cfa_type
	//if (lv_cfa_id.IsNull())

	if ( cfaTypePos == 1 )
	{
		lv_cfa_id	= "0000000";
		LOG(logDEBUG) << "\nCAIR1: cfaTypePos= " << cfaTypePos  ;
	}
	else if ( cfaTypePos == 0 )
	{
		lv_cfa_id	= lv_cfa.At(4);
		LOG(logDEBUG) << "\nCAIR2: lv_cfa_id= " << lv_cfa_id  ;
	}
	else
	{
		lv_cfa_id				= lv_cfa.At(4).For(cfaTypePos -1);
		lv_cfa_id.padLeading( 7 - lv_cfa_id.length(), '0');
		LOG(logDEBUG) << "\nCAIR3: lv_cfa_id= " << lv_cfa_id  ;
	}

	md_IRT_cop_id			= retFromRM(po_a,"BB",0).For(2);
	md_IRT_bra_id			= retFromRM(po_a,"BB",0).At(3);

	md_IRT_iata_no			= po_a.mo_C4010AMD.CPNID; //TKTAGCY;

	md_IRT_crs_id			= "AMD";

	if ( cfaTypePos > 0 )
	{
		md_IRT_cfa_type		= lv_cfa.At(cfaTypePos + 3).For(1);
		LOG(logDEBUG) << "\nCAIR4: md_IRT_cfa_type= " << md_IRT_cfa_type  ;
	}
	else
		md_IRT_cfa_type		= "";

	md_IRT_cfa_code		= lv_cfa.For(3);
	md_IRT_cfa_id			= md_IRT_cfa_code + lv_cfa_id + md_IRT_cfa_type;

	md_IRT_con_id			= retFromRM(po_a,"CN",0).For(3);
	if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFSGrp.mo_C4119FS.size() > 0)
	{
		md_IRT_iata_dest		= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFSGrp.mo_C4119FS[0]->MISC;
	}
	md_IRT_city_id			= retFromRM(po_a,"DE",0).For(3);

	if ( retFromRM(po_a,"TU",0).isAllNumeric())
		md_IRT_ticketer_user_id			= retFromRM(po_a,"TU",0);

	//US664 - STARTS
	LOG(logDEBUG) << "md_IRT_pnr_date before="  << md_IRT_pnr_date  ;
	LOG(logDEBUG) << "md_IRT_record_creation_date before="  << md_IRT_record_creation_date  ;
	
	md_IRT_pnr_date		= po_a.mo_C4015D_.PNRDTE;
	//md_IRT_pnr_time		= ;
	md_IRT_start_date		= "       ";
	md_IRT_cfa_dpt			= retFromRM(po_a,"DP",0).For(4);
	
	md_IRT_record_creation_date = po_a.mo_C4015D_.AIRDTE;
	LOG(logDEBUG) << "md_IRT_pnr_date after="  << md_IRT_pnr_date  ;
	LOG(logDEBUG) << "md_IRT_record_creation_date after="  << md_IRT_record_creation_date  ;
	//US664 - ENDS
   // <<< as in SCR 516167
  if (md_IRT_cfa_dpt.At(4).For(1) == "\015") {
      md_IRT_cfa_dpt       = retFromRM(po_a,"DP",0).For(3);
      }

   // >>> as in SCR 516167

	md_IRT_mktg_list		= retFromRM(po_a,"ML",0).For(1);
	md_IRT_reference		= po_a.mo_C4010AMD.RECLOC;
	md_IRT_booking_no		= po_a.mo_C4010AMD.RECLOC;

	if (!retFromRM(po_a,"EB",0).IsNull()) {
		mv_actionCode = retFromRM(po_a,"EB",0).For(2);
		mv_source = retFromRM(po_a,"EB",0).At(3).For(1);
	}

	mv_phoneField			= retFromRM(po_a,"PH",0);
}

void 
CAIR::populateIraddress(CairRecord &po_a)
{

	AB_Boolean			lb_ABfound;
	AB_Boolean			lb_AMfound;
	AB_Boolean			lb_ABSfound;
	AB_Boolean			lb_AMSfound;
	AB_Boolean			lb_Numberfound;

	D_address_line 	ld_a[6];
	D_address_line 	ld_d[6];

	int					li_i;

	lb_ABfound		= AB_false;
	lb_AMfound		= AB_false;
	lb_ABSfound		= AB_false;
	lb_AMSfound		= AB_false;
	lb_Numberfound	= AB_false;

	li_i     = 0;

	getNumbersFromI_(po_a,
						  md_IA_phone_acode[mi_IA_I],
						  md_IA_phone_no[mi_IA_I],
						  md_IA_bphone_acode[mi_IA_I],
		    			  md_IA_bphone_no[mi_IA_I],
				  		  md_IA_fax_acode[mi_IA_I],
				        md_IA_fax_no[mi_IA_I]);

	overrideHomePhone();

	if (md_IA_phone_acode[mi_IA_I].NotIsNull() 	||
		 md_IA_phone_no[mi_IA_I].NotIsNull()		||
		 md_IA_bphone_acode[mi_IA_I].NotIsNull()	||
		 md_IA_bphone_no[mi_IA_I].NotIsNull()		||
		 md_IA_fax_acode[mi_IA_I].NotIsNull()		||	
		 md_IA_fax_no[mi_IA_I].NotIsNull() )
	{
		lb_Numberfound	= AB_true;
	}

	for (unsigned i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size() && !lb_ABfound; i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CABGrp.mo_C4138AB.size() > 0)
		{
			getAddressFromAB(po_a,i,
									ld_a[0],
									ld_a[1],
									ld_a[2],
									ld_a[3],
									ld_a[4],
									ld_a[5]
									);
			lb_ABfound	= AB_true;
		}
	}

	for (unsigned i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size() && !lb_AMfound; i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CAMGrp.mo_C4140AM.size() > 0)
		{
			getAddressFromAM(po_a,i,
									ld_d[0],
									ld_d[1],
									ld_d[2],
									ld_d[3],
									ld_d[4],
									ld_d[5]
									);
			lb_AMfound	= AB_true;
		}
	}

	for (unsigned i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size() && !lb_ABfound && !lb_ABSfound; i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CABSGrp.mo_C4139ABS.size() > 0)
		{
			getAddressFromABS(po_a,i,
									ld_a[0],
									ld_a[1],
									ld_a[2],
									ld_a[3],
									ld_a[4],
									ld_a[5]
									);
			lb_ABSfound	= AB_true;
		}
	}

	for (unsigned i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size() && !lb_AMfound && !lb_AMSfound; i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CAMSGrp.mo_C4141AMS.size() > 0)
		{
			getAddressFromAMS(po_a,i,
									ld_d[0],
									ld_d[1],
									ld_d[2],
									ld_d[3],
									ld_d[4],
									ld_d[5]
									);
			lb_AMSfound	= AB_true;
		}
	}

	/* condense Addresses */

	for ( unsigned i = 0; i < 6; i++)
	{
		int j;
		j = 1;

		while (ld_a[i].length() == 0 && j < 6)
		{
			for (unsigned k = i; k < 5; k++)
			{
				ld_a[k] 		= ld_a[k+1];
				//ld_a[k+1]	= "";
				ld_a[k+1]   = AB_null;
			}
			j++;
		}
	}

	for ( unsigned i = 0; i < 6; i++)
	{
		int j;
		j = 1;

		while (ld_d[i].length() == 0 && j < 6)
		{
			for (unsigned k = i; k < 5; k++)
			{
				ld_d[k] 		= ld_d[k+1];
				//ld_d[k+1]	= "";
				ld_d[k+1] 	= AB_null;
			}
			j++;
		}
	}

	if (lb_Numberfound)
	{
		md_IA_type[mi_IA_I] = "P";

		li_i		= 1;
	}

	if (lb_ABfound || lb_ABSfound)
	{
		md_IA_addr1[mi_IA_I]	= ld_a[0];
		md_IA_addr2[mi_IA_I]	= ld_a[1];
		md_IA_addr3[mi_IA_I]	= ld_a[2];
		md_IA_addr4[mi_IA_I]	= ld_a[3];
		md_IA_addr5[mi_IA_I]	= ld_a[4];
		md_IA_addr6[mi_IA_I]	= ld_a[5];

		md_IA_type[mi_IA_I] = "P";

		li_i     = 1;
	}

	mi_IA_I		= li_i;

	if (lb_AMfound || lb_AMSfound)
	{
		md_IA_addr1[mi_IA_I]	= ld_d[0];
		md_IA_addr2[mi_IA_I]	= ld_d[1];
		md_IA_addr3[mi_IA_I]	= ld_d[2];
		md_IA_addr4[mi_IA_I]	= ld_d[3];
		md_IA_addr5[mi_IA_I]	= ld_d[4];
		md_IA_addr6[mi_IA_I]	= ld_d[5];

		md_IA_type[mi_IA_I] = "D";

		mi_IA_I++;
	}
}

void 
CAIR::populateIrperson(CairRecord &po_a)
{
	AB_Varchar		lv_tmp(3);

	#define psgrnme\
	po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4088I_.PSGRNME

	for (unsigned i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size(); i++)
	{
		lv_tmp							= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4088I_.PSGRNBR;
		mi_IP_PSGRNBR[mi_IP_I]		= AB_Integer(lv_tmp).to_long();

		md_IP_pax_id[mi_IP_I]		= i + 1;
		//md_IP_title[mi_IP_I]			= ;
		md_IP_last_name[mi_IP_I]	= psgrnme.For(psgrnme.findSubstring("/")-1);
		md_IP_first_name[mi_IP_I]	= psgrnme.At(psgrnme.findSubstring("/")+1);
		//md_IP_new_pax_id[mi_IP_I]	= ;
		//md_IP_new_pax_line[mi_IP_I]= ;
		//md_IP_sex[mi_IP_I]			= ;
		//md_IP_smoking[mi_IP_I]		= ;
		//md_IP_ctry_id[mi_IP_I]		= ;
		//md_IP_passport_no[mi_IP_I]	= ;
		//md_IP_passport_exp[mi_IP_I]= ;
		md_IP_reference[mi_IP_I]		= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4088I_.IDNBR;
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4101V_.CINVNUM.NotIsNull())
		{
			md_IP_crs_invoice[mi_IP_I]	= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4101V_.INVNBR +
												  po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4101V_.CINVNUM;
		}
		else
		{
			md_IP_crs_invoice[mi_IP_I] = po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4101V_.INVNBR;
		}
		//md_IP_traveller_id[mi_IP_I]= ;
		//md_IP_nameNumber[mi_IP_I]	= ;

		mi_IP_I++;
	}
}

void 
CAIR::populateIrticket(CairRecord &po_a)
{
	ENTER

	int 	j;

	j = 0;
	bool	lb_exchange_ticket = false;
	int			firstCharPos;

	if (po_a.mo_C4013B_.ENTRY.findSubstring("/EXCH") > 0) {
		lb_exchange_ticket = true;
	}

	//md_TKX_cc_exp_date					= ;
	//mv_actionCode						= ;
	//mv_source								= ;
	for (unsigned i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size(); i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR.NotIsNull() &&
			 po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR != "")
		{
			md_ID_pax_id[mi_ID_I]        		= i + 1;
			md_ID_line_number[mi_ID_I] 		= mi_ID_I + 1;
			//md_ID_cost_line[mi_ID_I]			= ;
			md_ID_reference[mi_ID_I]			= po_a.mo_C4010AMD.RECLOC;
			md_ID_sup_id[mi_ID_I]				= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.NUMAIR;
			md_ID_cur_id[mi_ID_I]				= po_a.mo_C4066FareLine.mo_C4067K_.CURCODE;
			md_ID_form[mi_ID_I]					= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR.For(4);
			md_ID_ticket_no[mi_ID_I]			= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR.At(5);
			md_ID_pic[mi_ID_I]					= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4088I_.PSGRID;
			//md_ID_orig_ticket_no[mi_ID_I]	= ;

			retCommission(i,po_a,md_ID_comm_rate[mi_ID_I],md_ID_commission[mi_ID_I]);

			//md_ID_comm_rate[mi_ID_I]			= ;
			//md_ID_commission[mi_ID_I]		= ;

			AB_Varchar lv_digit(2);
			lv_digit = po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.DIGIT;
			if (!lv_digit.IsNull()) {
				lv_digit.removeLeadAndTrailBlanks();
			}

			if (!lv_digit.IsNull() && lv_digit != "")
			{
				if (AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.DIGIT) >
					 AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR.At(9)))
				{
					md_ID_no_of_books[mi_ID_I]		= AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.DIGIT) -
															  AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR.At(9)) + AB_Integer(1);
				}
				else
				{
					md_ID_no_of_books[mi_ID_I]    = AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR.At(8).For(1) +
																			 po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.DIGIT)
																			 + AB_Integer(100) -
															  AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTNBR.At(8)) + AB_Integer(1);
				}
			}
			else
			{
				md_ID_no_of_books[mi_ID_I]		= AB_Integer(1);
			}

			md_ID_base_cost[mi_ID_I]			= po_a.mo_C4066FareLine.mo_C4067K_.BASE ;

         if ( po_a.mo_C4066FareLine.mo_C4067K_.BASE.IsNull() )
         {
            if ( po_a.mo_C4066FareLine.mo_C4071KS_.EQVAMT.IsNull() )
               md_ID_base_cost[mi_ID_I]         = po_a.mo_C4066FareLine.mo_C4071KS_.BASE;
            else
               md_ID_base_cost[mi_ID_I]         = po_a.mo_C4066FareLine.mo_C4071KS_.EQVAMT;
         }

			md_ID_saving_code[mi_ID_I]			= retFromRM(po_a,"FS",0);
			md_ID_full_fare[mi_ID_I]			= retFromRM(po_a,"FF",0);
			md_ID_low_prac_fare[mi_ID_I]		= retFromRM(po_a,"LP",0);

			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFTGrp.mo_C4120FT.size() > 0)
			{
				md_ID_crs_tour[mi_ID_I]			= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFTGrp.mo_C4120FT[0]->TOUR;
			}

			md_ID_rebate[mi_ID_I]				= retFromRM(po_a,"RB",0);
			md_ID_doc_type[mi_ID_I]				= "BSP";

			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4098T_.TKTTYPE == "E")
			{
				md_ID_e_ticket[mi_ID_I]			= "Y";
			}

			if (lb_exchange_ticket) {
				AB_Varchar  	lv_remark(80);
				AB_Char     	lc_docLine(2);
				AB_Char			lc_base_cost(12);
				AB_Char			lc_commission(9);
				AB_Char			lc_XT_amount(8);
				int				li_pos;
				int            li_count;

				md_ID_exchange_code[mi_ID_I]	= "Z";
				//SCR#510266 add conjunction ticket for exchange ticket
				//md_ID_no_of_books[mi_ID_I]		= AB_Integer(1);

				if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFOGrp.mo_C4117FO.size() > 0) {
					/*
					 * ORIGEXCH looks like
					 * 001-3100000211YTO11AUG04/00792120/001-31000002114C1*B713.80/X100.08/C35.00
					 * org. tktNo	  |			|iataNo	|conj. tktNo  |    |b_cost|totTax |penalty
					 */
					V_AB_Varchar   lva(80,80);
					li_count = lva.parseStringMultDel(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFOGrp.mo_C4117FO[0]->ORIGEXCH,"/");

					lc_docLine = md_ID_line_number[mi_ID_I];
					lc_docLine.rightJustify('0');
					lv_remark = lc_docLine;

					for (int idx = 1; idx <= li_count; idx++) {
						if (idx == 1) {
							md_ID_orig_ticket_no[mi_ID_I]  = lva[idx].At(5);
							li_pos = md_ID_orig_ticket_no[mi_ID_I].findFirstCharNotInList("0123456789");
							if (li_pos > 0) {
								md_ID_orig_ticket_no[mi_ID_I] = md_ID_orig_ticket_no[mi_ID_I].For(li_pos - 1);
							}
						} else 

						if (lc_base_cost.IsNull() &&
							 (li_pos = lva[idx].findSubstring("*B")) > 0) {
							lc_base_cost = lva[idx].At(li_pos+2);
							if (lva[idx].At(li_pos+2).findFirstCharNotInList("0123456789.") > 0) {
								lc_base_cost = "0";	// invalid base cost, default to 0
							}
							lc_base_cost.rightJustify('0');
						} else

						if (lc_XT_amount.IsNull() && lva[idx].For(1) == "X") {
							lc_XT_amount = lva[idx].At(2);
							if (lva[idx].At(2).findFirstCharNotInList("0123456789.") > 0) {
								lc_XT_amount = "0";
							}
							lc_XT_amount.rightJustify('0');
						} else

						if (md_ID_penalty_ch_fee[mi_ID_I].IsNull() &&
							 lva[idx].For(1) == "C") {
							md_ID_penalty_ch_fee[mi_ID_I] = lva[idx].At(2);
							if (md_ID_penalty_ch_fee[mi_ID_I].findFirstCharNotInList("0123456789.") > 0) {
								md_ID_penalty_ch_fee[mi_ID_I] = "0";	// invalid amount, default to 0
							}
						}
					}
				}

				if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFMGrp.mo_C4114FM.size() > 0) {
					/*
					 * FARECOM looks like
					 * *M*18.00/XO/19.00/XP/5.00
					 * *M*8.00A/XO/9.00A/XP/5.00
					 * newComm |  |oComm|  |comm on penalty
					 */
					V_AB_Varchar   lva(80,80);
					li_count = lva.parseStringMultDel(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFMGrp.mo_C4114FM[0]->FARECOM,"/");

					for (int idx = 1; idx <= li_count; idx++) {
						if (lc_commission.IsNull()		&&
							 lva[idx] == "XO"				&&
							 idx + 1 <= li_count) {
							firstCharPos = lva[idx+1].findFirstCharNotInSet("N.");
							if (firstCharPos > 0)
							{
								if (lva[idx+1].At(firstCharPos).For(1) == "A") {
									lc_commission = lva[idx+1].For(firstCharPos-1);
								} else {
									lc_commission = "P" + lva[idx+1].For(firstCharPos-1);
								}
							}
							else 
							{
								lc_commission = "P" + lva[idx+1];
							}
							//if (lva[idx+1].At(lva[idx+1].length()).For(1) == "A") {
							//	lc_commission = lva[idx+1].For(lva[idx+1].length() - 1);
							//} else {
							//	lc_commission = "P" + lva[idx+1];
							//}
						} else

						if (md_ID_comm_penalty[mi_ID_I].IsNull() &&
							 lva[idx] == "XP"							  &&
							 idx + 1 <= li_count) {
							md_ID_comm_penalty[mi_ID_I] = lva[idx+1];
							if (md_ID_comm_penalty[mi_ID_I].findFirstCharNotInList("0123456789.") > 0) {
								md_ID_comm_penalty[mi_ID_I] = "0";	// invalid penalty commission, default to 0
							}
						}
					}
				}

				if (lc_base_cost.IsNull()) {
					/*
					 * this is even exchange tickets
					 */
					md_ID_exchange_code[mi_ID_I]	= "E";

					lc_base_cost = md_ID_base_cost[mi_ID_I];
					if (lc_commission.IsNull()) {
						if (!md_ID_comm_rate[mi_ID_I].IsNull()) {
							lc_commission = "P" + md_ID_comm_rate[mi_ID_I];
						} else {
							lc_commission = md_ID_commission[mi_ID_I];
						}
					}
					lv_remark = lc_docLine + lc_base_cost + lc_commission;
				} else {
					if (lc_commission.IsNull()) {
						lc_commission 	= "0";
					}

					if (lc_XT_amount.IsNull()) {
						lc_XT_amount	= "0";
					}
					lv_remark = lc_docLine + lc_base_cost + lc_commission + "T2:" + lc_XT_amount  + "XT";
				}

				createExchageCostRemark(lv_remark);
			}

			mi_ID_I++;
		}
	}
	//SCR#509695 Ignore "hand ticket" data if this is exchange ticket
	for (unsigned i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size() && !lb_exchange_ticket; i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH.size() > 0)
		{
			md_ID_pax_id[mi_ID_I]        		= i + 1;
			md_ID_line_number[mi_ID_I] 		= mi_ID_I + 1;
			//md_ID_cost_line[mi_ID_I]			= ;
			md_ID_reference[mi_ID_I]			= po_a.mo_C4010AMD.RECLOC;
			md_ID_sup_id[mi_ID_I]				= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.For(3);
			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFareLineGrp.mo_C4066FareLine.size() > 0) {
				md_ID_cur_id[mi_ID_I]				= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4067K_.CURCODE;
			} else {
				md_ID_cur_id[mi_ID_I]				= po_a.mo_C4066FareLine.mo_C4067K_.CURCODE;
			}
			md_ID_form[mi_ID_I]					= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(5).For(4);
			md_ID_ticket_no[mi_ID_I]			= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(9);
			md_ID_pic[mi_ID_I]					= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4088I_.PSGRID;
			//md_ID_orig_ticket_no[mi_ID_I]	= ;

			retCommission(i,po_a,md_ID_comm_rate[mi_ID_I],md_ID_commission[mi_ID_I]);

			//md_ID_comm_rate[mi_ID_I]			= ;
			//md_ID_commission[mi_ID_I]		= ;

			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(15).length() > 0)
			{
				if (AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(16)) >
					 AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(13).For(2)))
				{
					md_ID_no_of_books[mi_ID_I]		= AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(16)) -
															  AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(13).For(2)) + AB_Integer(1);
				}
				else
				{
					md_ID_no_of_books[mi_ID_I]    = AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(12).For(1) +
																			 po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(16))
																			 + AB_Integer(100) -
															  AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFHGrp.mo_C4112FH[0]->HANDTKT.At(12).For(3)) + AB_Integer(1);
				}
			}
			else
			{
				md_ID_no_of_books[mi_ID_I]		= AB_Integer(1);
			}

			//md_ID_exchange_code[mi_ID_I]	= ;
			//md_ID_penalty_ch_fee[mi_ID_I]	= ;
			//md_ID_comm_penalty[mi_ID_I]		= ;
			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFareLineGrp.mo_C4066FareLine.size() > 0) {
				md_ID_base_cost[mi_ID_I]			= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4067K_.BASE ;
			} else {
				md_ID_base_cost[mi_ID_I]			= po_a.mo_C4066FareLine.mo_C4067K_.BASE ;
			}
			md_ID_saving_code[mi_ID_I]			= retFromRM(po_a,"FS",0);
			md_ID_full_fare[mi_ID_I]			= retFromRM(po_a,"FF",0);
			md_ID_low_prac_fare[mi_ID_I]		= retFromRM(po_a,"LP",0);
			//md_ID_personal_crs[mi_ID_I]		= ;
			//md_ID_stp_home_city[mi_ID_I]	= ;
			//md_ID_stp_host_city[mi_ID_I]	= ;
			//md_ID_sel_access[mi_ID_I]		= ;

			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFTGrp.mo_C4120FT.size() > 0)
			{
				md_ID_crs_tour[mi_ID_I]			= po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFTGrp.mo_C4120FT[0]->TOUR;
			}

			md_ID_rebate[mi_ID_I]				= retFromRM(po_a,"RB",0);
			md_ID_doc_type[mi_ID_I]				= "BSP";

			mi_ID_I++;
		}
	}
	EXIT
}

void 
CAIR::populateIrtransport(CairRecord &po_a)
{
	AB_Varchar		lv_tmp(3);

	/* H- Ticketed Segments */

	for (unsigned i = 0; i < po_a.mo_C4011ticketedGrp.mo_CTicketedSeg.size(); i++)
	{
		if (po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4017H_.MSGTAG.NotIsNull())
		{
			/* AIR_H_AIR */

			#define a4017H_(att)\
			po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4017H_.att

			lv_tmp								= a4017H_(SEGNBR);
			mi_IT_SEGNBR[mi_IT_I]			= AB_Integer(lv_tmp).to_long();
			mi_IT_TYPE[mi_IT_I]				= AIR_H_AIR;
			mi_TicketedSeg[mi_IT_I]			= i;

			mva_IT_PNRSTAT[mi_IT_I + 1]	= a4017H_(PNRSTAT);
			mva_IT_BAGALL[mi_IT_I + 1]		= a4017H_(BAGALL);
			md_IT_itin_type[mi_IT_I]		= "A";
			md_IT_line_number[mi_IT_I]		= mi_IT_I + 1;
			md_IT_start_date[mi_IT_I]		= deriveDte(a4017H_(DEPDTE));
			md_IT_start_time[mi_IT_I]		= a4017H_(DEPTIM);
			md_IT_orig_city_id[mi_IT_I]	= a4017H_(ORIGAIR);
			md_IT_orig_cityname[mi_IT_I]	= a4017H_(ORIGCTY);
			//md_IT_chg_of_gauge[mi_IT_I]	= ;
			//md_IT_cog_dep_city[mi_IT_I]	= ;
			//md_IT_cog_dep_date[mi_IT_I]	= ;
			//md_IT_cog_dep_time[mi_IT_I]	= ;
			//md_IT_cog_dep_term[mi_IT_I]	= ;
			//md_IT_cog_dep_gate[mi_IT_I]	= ;
			//md_IT_cog_of_miles[mi_IT_I]	= ;
			md_IT_end_time[mi_IT_I]			= a4017H_(ARRTIM);
			md_IT_dest_city_id[mi_IT_I]	= a4017H_(DESTAIR);
			md_IT_dest_cityname[mi_IT_I]	= a4017H_(DESTCTY);
			md_IT_classofservice[mi_IT_I]	= a4017H_(CLSBKG);
			md_IT_service[mi_IT_I]			= a4017H_(FLTNBR);
			md_IT_sup_id[mi_IT_I]			= a4017H_(AIRCDE);

			AB_Varchar lv_sup_id(3);
			lv_sup_id = md_IT_sup_id[mi_IT_I].to_string();

			md_IT_reference[mi_IT_I]		= po_a.mo_C4010AMD.RECLOC;
			md_IT_no_of_stops[mi_IT_I]		= a4017H_(NBRSTOP);
			//md_IT_dep_terminal[mi_IT_I]	= ;
			//md_IT_dep_gate[mi_IT_I]		= ;
			//md_IT_report_time[mi_IT_I]	= ;
			//md_IT_arr_gate[mi_IT_I]		= ;
			//md_IT_arr_terminal[mi_IT_I]	= ;
			md_IT_carrier_type[mi_IT_I]	= a4017H_(EQUIP);
			md_IT_meal[mi_IT_I]				= a4017H_(MEAL);
			md_IT_aff_carrier[mi_IT_I]		= a4017H_(SHRCOMM);
			md_IT_actual_miles[mi_IT_I]	= a4017H_(MLTPM);
			md_IT_arrival_ind[mi_IT_I]		= deriveArrInd(
													  a4017H_(DEPDTE),
													  a4017H_(ARRDTE));
			md_IT_elapsed_time[mi_IT_I]	= a4017H_(FLTDUR);

			// SCR: 514895
			md_IT_airline_locator[mi_IT_I]		=	po_a.mo_C4010AMD.getAirlineLocator(lv_sup_id);

			if (po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4017H_.mo_CXYGrp.mo_C4021X_.size() > 0)
			{
            //start of SCR 504189
            //don't override md_IT_sup_id; so comment out the code

            //md_IT_sup_id[mi_IT_I] = po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4017H_.mo_CXYGrp.mo_C4021X_[0]->AIRCDE;

            AB_Varchar  lv_tmp(3);

            //lv_tmp = md_IT_sup_id[mi_IT_I];
            //USE THE OPERATED BY associated remark

            lv_tmp = po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4017H_.mo_CXYGrp.mo_C4021X_[0]->AIRCDE;
            createITRemark(lv_tmp);
			}

			mi_IT_I++ ;
		}

		if (po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4018H_.MSGTAG.NotIsNull())
		{
			/* AIR_H_SURFACE */

			#define a4018H_(att)\
			po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4018H_.att

			AB_Date ld;
			lv_tmp								= a4018H_(SEGNBR);
			mi_IT_SEGNBR[mi_IT_I]			= AB_Integer(lv_tmp).to_long();
			mi_IT_TYPE[mi_IT_I]				= AIR_H_SURFACE;
			mi_TicketedSeg[mi_IT_I]			= i;
			md_IT_itin_type[mi_IT_I]		= "A";
			md_IT_line_number[mi_IT_I]    = mi_IT_I + 1;
			md_IT_start_date[mi_IT_I]     = AB_now.to_matrixStr();
			md_IT_orig_city_id[mi_IT_I]	= a4018H_(ORIGAIR);
			md_IT_orig_cityname[mi_IT_I]	= a4018H_(ORIGCTY);
			md_IT_dest_city_id[mi_IT_I]	= a4018H_(DESTAIR);
			md_IT_dest_cityname[mi_IT_I]	= a4018H_(DESTCTY);
			md_IT_sup_id[mi_IT_I]			= "ZZZ";
			md_IT_service[mi_IT_I]        = "VOID";

			mi_IT_I++ ;
		}

		if (po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4019H_.MSGTAG.NotIsNull())
		{
			/* AIR_H_OPEN */

			#define a4019H_(att)\
			po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4019H_.att

			lv_tmp								= a4019H_(SEGNBR);
			mi_IT_SEGNBR[mi_IT_I]			= AB_Integer(lv_tmp).to_long();
			mi_IT_TYPE[mi_IT_I]				= AIR_H_OPEN;
			mi_TicketedSeg[mi_IT_I]			= i;

			mva_IT_PNRSTAT[mi_IT_I + 1]	= a4019H_(STATUS);
			mva_IT_BAGALL[mi_IT_I + 1]		= a4019H_(BAGALL);
			md_IT_itin_type[mi_IT_I]		= "A";
			md_IT_line_number[mi_IT_I]		= mi_IT_I + 1;
			md_IT_start_date[mi_IT_I]		= deriveDte(a4019H_(DEPDTE));
			md_IT_orig_city_id[mi_IT_I]	= a4019H_(ORIGAIR);
			md_IT_orig_cityname[mi_IT_I]	= a4019H_(ORIGCTY);
			md_IT_dest_city_id[mi_IT_I]	= a4019H_(DESTAIR);
			md_IT_sup_id[mi_IT_I]			= a4019H_(AIRCDE);
			md_IT_classofservice[mi_IT_I]	= a4019H_(CLSBKG);
			md_IT_service[mi_IT_I]			= a4019H_(CLSSVC);
			md_IT_actual_miles[mi_IT_I]	= a4019H_(MLTPM);

			mi_IT_I++ ;
		}
		if (po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4020H_.MSGTAG.NotIsNull() &&
				  ((po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4020H_.SEGTYPE == "TRN") ||
					(po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4020H_.SEGTYPE == "BUS")))
		{
			/* AIR_H_RAIL */

			#define a4020H_(att)\
			po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4020H_.att

			lv_tmp								= a4020H_(SEGNBR);
			mi_IT_SEGNBR[mi_IT_I]			= AB_Integer(lv_tmp).to_long();
			mi_IT_TYPE[mi_IT_I]				= AIR_H_RAIL;
			mi_TicketedSeg[mi_IT_I]			= i;

			mva_IT_PNRSTAT[mi_IT_I + 1]	= a4020H_(PNRSTAT);

			if (po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4020H_.SEGTYPE == "TRN")
			{
				md_IT_itin_type[mi_IT_I]		= "R";
			}
			else
			{
				md_IT_itin_type[mi_IT_I]		= "B";
			}

			md_IT_line_number[mi_IT_I]		= mi_IT_I + 1;
			md_IT_start_date[mi_IT_I]		= deriveDte(a4020H_(DEPDTE));
			md_IT_start_time[mi_IT_I]		= a4020H_(DEPTIM);
			md_IT_orig_city_id[mi_IT_I]	= a4020H_(ORIGCDE);
			md_IT_orig_cityname[mi_IT_I]	= a4020H_(ORIGCTY);
			md_IT_end_time[mi_IT_I]       = deriveDte(a4020H_(ARRDTE));
			md_IT_end_time[mi_IT_I]			= a4020H_(ARRTIM);
			md_IT_dest_city_id[mi_IT_I]	= a4020H_(DESTCDE);
			md_IT_dest_cityname[mi_IT_I]	= a4020H_(DESTCTY);
			md_IT_classofservice[mi_IT_I]	= a4020H_(CLSSVC);
			md_IT_service[mi_IT_I]			= a4020H_(PRODNBR);
			md_IT_sup_id[mi_IT_I]			= a4020H_(PRODCDE);
			md_IT_reference[mi_IT_I]		= po_a.mo_C4010AMD.RECLOC;

			mi_IT_I++ ;
		}
	}

	/* U- unticketed segments */

	for (unsigned i = 0; i < po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg.size(); i++)
	{
		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4027U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_AIR */

			#define a4027U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4027U_.att

			lv_tmp								= a4027U_(SEGNBR);
			mi_IT_SEGNBR[mi_IT_I]			= AB_Integer(lv_tmp).to_long();
			mi_IT_TYPE[mi_IT_I]				= AIR_U_AIR;
			mi_UnicketedSeg[mi_IT_I]		= i;

			mva_IT_PNRSTAT[mi_IT_I + 1]   = a4027U_(PNRSTAT);
			mva_IT_BAGALL[mi_IT_I + 1]		= a4027U_(BAGALL);
			md_IT_itin_type[mi_IT_I]		= "A";
			md_IT_line_number[mi_IT_I]		= mi_IT_I + 1;
			md_IT_start_date[mi_IT_I]		= deriveDte(a4027U_(DEPDTE));
			md_IT_start_time[mi_IT_I]		= a4027U_(DEPTIM);
			md_IT_orig_city_id[mi_IT_I]	= a4027U_(ORIGAIR);
			md_IT_orig_cityname[mi_IT_I]	= a4027U_(ORIGCTY);
			//md_IT_chg_of_gauge[mi_IT_I]	= ;
			//md_IT_cog_dep_city[mi_IT_I]	= ;
			//md_IT_cog_dep_date[mi_IT_I]	= ;
			//md_IT_cog_dep_time[mi_IT_I]	= ;
			//md_IT_cog_dep_term[mi_IT_I]	= ;
			//md_IT_cog_dep_gate[mi_IT_I]	= ;
			//md_IT_cog_of_miles[mi_IT_I]	= ;
			md_IT_end_time[mi_IT_I]			= a4027U_(ARRTIM);
			md_IT_dest_city_id[mi_IT_I]	= a4027U_(DESTAIR);
			md_IT_dest_cityname[mi_IT_I]	= a4027U_(DESTCTY);
			md_IT_classofservice[mi_IT_I]	= a4027U_(CLSBKG);
			md_IT_service[mi_IT_I]			= a4027U_(FLTNBR);
			md_IT_sup_id[mi_IT_I]			= a4027U_(AIRCDE);
			md_IT_reference[mi_IT_I]		= po_a.mo_C4010AMD.RECLOC;
			md_IT_no_of_stops[mi_IT_I]		= a4027U_(NBRSTOP);
			//md_IT_dep_terminal[mi_IT_I]	= ;
			//md_IT_dep_gate[mi_IT_I]		= ;
			//md_IT_report_time[mi_IT_I]	= ;
			//md_IT_arr_gate[mi_IT_I]		= ;
			//md_IT_arr_terminal[mi_IT_I]	= ;
			md_IT_carrier_type[mi_IT_I]	= a4027U_(EQUIP);
			md_IT_meal[mi_IT_I]				= a4027U_(MEAL);
			md_IT_aff_carrier[mi_IT_I]		= a4027U_(SHRCOMM);
			md_IT_actual_miles[mi_IT_I]	= a4027U_(MLTPM);
			md_IT_arrival_ind[mi_IT_I]		= deriveArrInd(
													  a4027U_(DEPDTE),
													  a4027U_(ARRDTE));
			md_IT_elapsed_time[mi_IT_I]	= a4027U_(FLTDUR);
			//md_IT_airline_locator[mi_IT_I]= ;

			if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4027U_.mo_CXYGrp.mo_C4021X_.size() > 0)
			{
				md_IT_sup_id[mi_IT_I] = po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4027U_.mo_CXYGrp.mo_C4021X_[0]->AIRCDE;

				AB_Varchar	lv_tmp(3);

				lv_tmp = md_IT_sup_id[mi_IT_I];
				createITRemark(lv_tmp);
			}
			mi_IT_I++ ;
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4029U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_OPEN */

			#define a4029U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4029U_.att

			lv_tmp								= a4029U_(SEGNBR);
			mi_IT_SEGNBR[mi_IT_I]			= AB_Integer(lv_tmp).to_long();
			mi_IT_TYPE[mi_IT_I]				= AIR_U_OPEN;
			mi_UnicketedSeg[mi_IT_I]		= i;

			mva_IT_PNRSTAT[mi_IT_I + 1]	= a4029U_(STATUS);
			mva_IT_BAGALL[mi_IT_I + 1]		= a4029U_(BAGALL);
			md_IT_itin_type[mi_IT_I]		= "A";
			md_IT_line_number[mi_IT_I]		= mi_IT_I + 1;
			md_IT_start_date[mi_IT_I]		= deriveDte(a4029U_(DEPDTE));
			md_IT_orig_city_id[mi_IT_I]	= a4029U_(ORIGAIR);
			md_IT_orig_cityname[mi_IT_I]	= a4029U_(ORIGCTY);
			md_IT_dest_city_id[mi_IT_I]	= a4029U_(DESTAIR);
			md_IT_sup_id[mi_IT_I]			= a4029U_(AIRCDE);
			md_IT_classofservice[mi_IT_I]	= a4029U_(CLSBKG);
			md_IT_service[mi_IT_I]			= a4029U_(CLSSVC);
			md_IT_actual_miles[mi_IT_I]	= a4029U_(MLTPM);

			mi_IT_I++ ;
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4030U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_RAIL */

			#define a4030U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4030U_.att

			if (a4030U_(SEGTYPE) == "TRN" ||
				 a4030U_(SEGTYPE) == "BUS") {
				lv_tmp								= a4030U_(SEGNBR);
				mi_IT_SEGNBR[mi_IT_I]			= AB_Integer(lv_tmp).to_long();
				mi_IT_TYPE[mi_IT_I]				= AIR_U_RAIL;
				mi_UnicketedSeg[mi_IT_I]		= i;
				mva_IT_PNRSTAT[mi_IT_I + 1]	= a4030U_(PNRSTAT);

				if (a4030U_(SEGTYPE) == "TRN") {
					md_IT_itin_type[mi_IT_I] = "R";
				} else {
					md_IT_itin_type[mi_IT_I] = "B";
				}

				md_IT_line_number[mi_IT_I]		= mi_IT_I + 1;
				md_IT_orig_city_id[mi_IT_I] 	= a4030U_(ORIGCDE);
				md_IT_dest_city_id[mi_IT_I] 	= a4030U_(DESTCDE);
				md_IT_sup_id[mi_IT_I]		 	= a4030U_(PRODCDE);
				md_IT_service[mi_IT_I]		 	= a4030U_(PRODNBR);
				md_IT_classofservice[mi_IT_I]	= a4030U_(CLSSVC);
				md_IT_start_date[mi_IT_I]     = deriveDte(a4030U_(DEPDTE));
				md_IT_start_time[mi_IT_I]     = a4030U_(DEPTIM);
				// md_IT_end_date[mi_IT_I]     	= deriveDte(a4030U_(ARRDTE));
				md_IT_end_time[mi_IT_I]     	= a4030U_(ARRTIM);

				mi_IT_I++ ;
			}
		}
	}
}

void 
CAIR::populateIrother(CairRecord &po_a)
{
	/*
	md_IO_itin_type[mi_IO_I]		= ;
	md_IO_line_number[mi_IO_I]		= ;
	md_IO_start_date[mi_IO_I]		= ;
	md_IO_start_time[mi_IO_I]		= ;
	md_IO_orig_city_id[mi_IO_I]	= ;
	md_IO_end_date[mi_IO_I]			= ;
	md_IO_end_time[mi_IO_I]			= ;
	md_IO_dest_city_id[mi_IO_I]	= ;
	md_IO_unit_type[mi_IO_I]		= ;
	md_IO_unit_ct[mi_IO_I]			= ;
	md_IO_sup_id[mi_IO_I]			= ;
	md_IO_reference[mi_IO_I]		= po_a.mo_C4010AMD.RECLOC;
	md_IO_saving_code[mi_IO_I]		= ;
	md_IO_confirmation[mi_IO_I]	= ;
	md_IO_departure_point[mi_IO_I]= ;
	md_IO_arrival_point[mi_IO_I]	= ;
	md_IO_svc_description[mi_IO_I]= ;
	md_IO_cabin_no[mi_IO_I]			= ;
	md_IO_tour_no[mi_IO_I]			= ;
	*/

	AB_Varchar	lv_tmp(3);

	for (unsigned i = 0; i < po_a.mo_C4011ticketedGrp.mo_CTicketedSeg.size(); i++)
	{
		if (po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4020H_.MSGTAG.NotIsNull() &&
			 po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[i]->mo_C4020H_.SEGTYPE == "SHP")
		{

			/* Other for SHP */

			/* AIR_H_RAIL */

			lv_tmp								= a4020H_(SEGNBR);
			mi_IO_SEGNBR[mi_IO_I]			= AB_Integer(lv_tmp).to_long();
			mi_IO_TYPE[mi_IO_I]				= AIR_H_RAIL;

			md_IO_itin_type[mi_IO_I]		= "S";

			md_IO_line_number[mi_IO_I]		= mi_IO_I + 1;
			md_IO_unit_type[mi_IO_I]		= md_IO_itin_type[mi_IO_I];
			md_IO_start_date[mi_IO_I]		= deriveDte(a4020H_(DEPDTE));
			md_IO_start_time[mi_IO_I]		= a4020H_(DEPTIM);
			md_IO_orig_city_id[mi_IO_I]	= a4020H_(ORIGCDE);
			md_IO_end_time[mi_IO_I]       = deriveDte(a4020H_(ARRDTE));
			md_IO_end_time[mi_IO_I]			= a4020H_(ARRTIM);
			md_IO_dest_city_id[mi_IO_I]	= a4020H_(DESTCDE);
			md_IO_sup_id[mi_IO_I]			= a4020H_(PRDTRNS);
			md_IO_reference[mi_IO_I]		= po_a.mo_C4010AMD.RECLOC;

			//md_IO_unit_type[mi_IO_I]		= ;
			//md_IO_unit_ct[mi_IO_I]			= ;
			//md_IO_sup_id[mi_IO_I]			= ;
			//md_IO_saving_code[mi_IO_I]		= ;
			//md_IO_confirmation[mi_IO_I]	= ;
			//md_IO_departure_point[mi_IO_I]= ;
			//md_IO_arrival_point[mi_IO_I]	= ;
			//md_IO_svc_description[mi_IO_I]= ;
			//md_IO_cabin_no[mi_IO_I]			= ;
			//md_IO_tour_no[mi_IO_I]			= ;

			mi_IO_I++ ;
		}
	}

	for (unsigned i = 0; i < po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg.size(); i++)
	{
		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4030U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_RAIL */

			#define a4030U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4030U_.att

			if (a4030U_(SEGTYPE) == "SHP") {
				lv_tmp								= a4030U_(SEGNBR);
				mi_IO_SEGNBR[mi_IO_I]			= AB_Integer(lv_tmp).to_long();
				mi_IO_TYPE[mi_IO_I]				= AIR_U_RAIL;
				mi_UnicketedSeg[mi_IO_I]		= i;

				md_IO_itin_type[mi_IO_I] 		= "S";
				md_IO_line_number[mi_IO_I]		= mi_IO_I + 1;
				md_IO_unit_type[mi_IO_I]		= md_IO_itin_type[mi_IO_I];
				md_IO_orig_city_id[mi_IO_I] 	= a4030U_(ORIGCDE);
				md_IO_dest_city_id[mi_IO_I] 	= a4030U_(DESTCDE);
				md_IO_sup_id[mi_IO_I]		 	= a4030U_(PRODCDE);
				md_IO_start_date[mi_IO_I]     = deriveDte(a4030U_(DEPDTE));
				md_IO_start_time[mi_IO_I]     = a4030U_(DEPTIM);
				md_IO_end_date[mi_IO_I]     	= deriveDte(a4030U_(ARRDTE));
				md_IO_end_time[mi_IO_I]     	= a4030U_(ARRTIM);

				mi_IO_I++ ;
			}
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4037U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_MANTOUR */

			#define a4037U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4037U_.att

			lv_tmp                        	= a4037U_(SEGNBR);
			mi_IO_SEGNBR[mi_IO_I]				= AB_Integer(lv_tmp).to_long();
			mi_IO_TYPE[mi_IO_I]					= AIR_U_MANTOUR;

			md_IO_itin_type[mi_IO_I]			= "T";
			md_IO_line_number[mi_IO_I]			= mi_IO_I + 1;
			md_IO_start_date[mi_IO_I]			= deriveDte(a4037U_(DEPDTE));
			// md_IO_start_time[mi_IO_I]		= ;
			md_IO_orig_city_id[mi_IO_I]		= a4037U_(AIRPRT);
			// md_IO_end_date[mi_IO_I]			= ;
			// md_IO_end_time[mi_IO_I]			= ;
			// md_IO_dest_city_id[mi_IO_I]	= ;
			md_IO_unit_type[mi_IO_I]			= "TUR";
			md_IO_unit_ct[mi_IO_I]				= 1;
			md_IO_sup_id[mi_IO_I]				= a4037U_(AIRCDE);
			md_IO_reference[mi_IO_I]			= po_a.mo_C4010AMD.RECLOC;
			// md_IO_saving_code[mi_IO_I]		= ;
			// md_IO_confirmation[mi_IO_I]	= ;
			// md_IO_departure_point[mi_IO_I]= ;
			// md_IO_arrival_point[mi_IO_I]	= ;
			// md_IO_svc_description[mi_IO_I]= ;
			// md_IO_cabin_no[mi_IO_I]			= ;
			// md_IO_tour_no[mi_IO_I]			= ;

			if (a4037U_(FREE).NotIsNull() && 
				 a4037U_(FREE).length() > 0)
			{
				createIORemark(a4037U_(FREE),"T");
			}

			mi_IO_I++ ;
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4038U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_AUTTOUR */

			#define a4038U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4038U_.att

			lv_tmp                        	= a4038U_(SEGNBR);
			mi_IO_SEGNBR[mi_IO_I]				= AB_Integer(lv_tmp).to_long();
			mi_IO_TYPE[mi_IO_I]					= AIR_U_AUTTOUR;

			md_IO_itin_type[mi_IO_I]			= "T";
			md_IO_line_number[mi_IO_I]			= mi_IO_I + 1;
			md_IO_start_date[mi_IO_I]			= deriveDte(a4038U_(DEPDTE));
			// md_IO_start_time[mi_IO_I]		= ;
			md_IO_orig_city_id[mi_IO_I]		= a4038U_(AIRPRT);
			md_IO_end_date[mi_IO_I]				= deriveDte(a4038U_(RTNDTE));
			// md_IO_end_time[mi_IO_I]			= ;
			// md_IO_dest_city_id[mi_IO_I]	= ;
			md_IO_unit_type[mi_IO_I]			= "TOUR";
			md_IO_unit_ct[mi_IO_I]				= 1;
			md_IO_sup_id[mi_IO_I]				= a4038U_(AIRCDE);
			md_IO_reference[mi_IO_I]			= po_a.mo_C4010AMD.RECLOC;
			// md_IO_saving_code[mi_IO_I]		= ;
			md_IO_confirmation[mi_IO_I]		= a4038U_(CONFNBR);
			// md_IO_departure_point[mi_IO_I]= ;
			// md_IO_arrival_point[mi_IO_I]	= ;
			md_IO_svc_description[mi_IO_I]	= a4038U_(TOURNME);
			// md_IO_cabin_no[mi_IO_I]			= ;
			// md_IO_tour_no[mi_IO_I]			= ;

			mi_IO_I++ ;
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4050U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_MISC */

			#define a4050U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4050U_.att

			lv_tmp                        	= a4050U_(SEGNBR);
			mi_IO_SEGNBR[mi_IO_I]				= AB_Integer(lv_tmp).to_long();
			mi_IO_TYPE[mi_IO_I]					= AIR_U_MISC;

			md_IO_itin_type[mi_IO_I]			= "O";
			md_IO_line_number[mi_IO_I]			= mi_IO_I + 1;
			md_IO_start_date[mi_IO_I]			= deriveDte(a4050U_(DATE));
			// md_IO_start_time[mi_IO_I]		= ;
			md_IO_orig_city_id[mi_IO_I]		= a4050U_(CTYCDE);
			// md_IO_end_date[mi_IO_I]			= ;
			// md_IO_end_time[mi_IO_I]			= ;
			// md_IO_dest_city_id[mi_IO_I]	= ;
			md_IO_unit_type[mi_IO_I]			= "OTH";
			md_IO_unit_ct[mi_IO_I]				= 1;
			md_IO_sup_id[mi_IO_I]				= a4050U_(SUPPCDE);
			md_IO_reference[mi_IO_I]			= po_a.mo_C4010AMD.RECLOC;
			// md_IO_saving_code[mi_IO_I]		= ;
			// md_IO_confirmation[mi_IO_I]	= ;
			// md_IO_departure_point[mi_IO_I]= ;
			// md_IO_arrival_point[mi_IO_I]	= ;
			// md_IO_svc_description[mi_IO_I]= ;
			// md_IO_cabin_no[mi_IO_I]			= ;
			// md_IO_tour_no[mi_IO_I]			= ;

			if (a4050U_(FREE).NotIsNull() && 
				 a4050U_(FREE).length() > 0)
			{
				createIORemark(a4050U_(FREE),"O");
			}

			mi_IO_I++ ;
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4052U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_AUTCRUISE */

			#define a4052U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4052U_.att

			lv_tmp                        	= a4052U_(ELMNBR);
			mi_IO_SEGNBR[mi_IO_I]				= AB_Integer(lv_tmp).to_long();
			mi_IO_TYPE[mi_IO_I]					= AIR_U_AUTCRUISE;

			md_IO_itin_type[mi_IO_I]		= "S";
			md_IO_line_number[mi_IO_I]		= mi_IO_I + 1;
			md_IO_start_date[mi_IO_I]		= a4052U_(DEPDTE).For(5) + a4052U_(DEPDTE).At(8);
			// md_IO_start_time[mi_IO_I]		= ;
			md_IO_orig_city_id[mi_IO_I]	= a4052U_(CITYDEP);
			// md_IO_end_date[mi_IO_I]			= ;
			// md_IO_end_time[mi_IO_I]			= ;
			md_IO_dest_city_id[mi_IO_I]	= a4052U_(CITYARR);
			md_IO_unit_type[mi_IO_I]		= md_IO_itin_type[mi_IO_I];
			md_IO_unit_ct[mi_IO_I]			= a4052U_(PNRSTAT).At(3);
			md_IO_sup_id[mi_IO_I]			= a4052U_(AIRCDE);
			md_IO_reference[mi_IO_I]		= po_a.mo_C4010AMD.RECLOC;
			// md_IO_saving_code[mi_IO_I]		= ;
			md_IO_confirmation[mi_IO_I]	= a4052U_(CFNUM);
			// md_IO_departure_point[mi_IO_I]= ;
			// md_IO_arrival_point[mi_IO_I]	= ;
			md_IO_svc_description[mi_IO_I]= a4052U_(SHPNAM);
			// md_IO_cabin_no[mi_IO_I]			= ;
			// md_IO_tour_no[mi_IO_I]			= ;

			mi_IO_I++ ;
		}
	}
}

void 
CAIR::populateIrfrequentuser(CairRecord &po_a)
{
	
	#define a4090FQV(att)\
	po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFQVGrp.mo_C4090FQV[j]->att

	for (unsigned i = 0; i < mi_IP_I; i++)
	{
		for (unsigned j = 0; j < po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CFQVGrp.mo_C4090FQV.size(); j++)
		{

			set <Text2Text>::iterator it;
			it = FriquentFlyer_SupId2PaxId.find(Text2Text(a4090FQV(FQTVNBR).For(2), md_IP_pax_id[i]));
			if ( it == FriquentFlyer_SupId2PaxId.end() )
			{
				LOG(logDEBUG) << "NOT FOUND in FriquentFlyer_SupId2PaxId:" << a4090FQV(FQTVNBR).For(2) << "\t" << md_IP_pax_id[i] ;

				md_IRF_pax_id[mi_IRF_I]		 = md_IP_pax_id[i] ;
				md_IRF_sup_id[mi_IRF_I]		 = a4090FQV(FQTVNBR).For(2);
				//md_IRF_fup_type[mi_IRF_I]	 = ;
				md_IRF_fup[mi_IRF_I]			 = a4090FQV(FQTVNBR).At(3);
				md_IRF_fup_status[mi_IRF_I] = "HK";
				md_IRF_srvc_sup_id[mi_IRF_I]= a4090FQV(AIRCDE);

				FriquentFlyer_SupId2PaxId.insert(Text2Text(a4090FQV(FQTVNBR).For(2), md_IP_pax_id[i]));

				mi_IRF_I++;
			}
		}
	}
}

void 
CAIR::populateIrconsumdetail(CairRecord &po_a)
{
	AB_Decimal		ld_coeff(13,2,1);
	int				li_i;
	int				li_k;
	V_AB_Varchar 	lva_farebase(40,40);

	if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine.size() > 0)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4079N_.CURRTYP == "NUC")
		{
			ld_coeff	= AB_Decimal(13,2,po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4067K_.BASE)/
						  po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4079N_.TOTAL;
		}
	}
	else
	{
		if (po_a.mo_C4066FareLine.mo_C4079N_.CURRTYP == "NUC")
		{
			ld_coeff	= AB_Decimal(13,2,po_a.mo_C4066FareLine.mo_C4067K_.BASE)/
						  po_a.mo_C4066FareLine.mo_C4079N_.TOTAL;
		}
	}

	li_i	= 1;
	li_k	= 1;

	for (unsigned i = 0; i < mi_IT_I; i++)
	{
		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine.size() > 0)
		{
			if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4079N_.BASE[i + 1] == "VOID")
			{
				lva_farebase[i + 1]  = "VOID";
				li_i                 = li_i - 1;
			}
			else
			{
				if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4078M_.FAREB[li_i].IsNull())
				{
					lva_farebase[i + 1] = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4078M_.PRIMCDE[li_i];
				}
				else
				{
					lva_farebase[i + 1] = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4078M_.PRIMCDE[li_i] +
											   po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4078M_.FAREB[li_i];
				}
			}
		}
		else
		{
			if (po_a.mo_C4066FareLine.mo_C4079N_.BASE[i + 1] == "VOID")
			{
				lva_farebase[i + 1]  = "VOID";
				li_i                 = li_i - 1;
			}
			else
			{
				if (po_a.mo_C4066FareLine.mo_C4078M_.FAREB[li_i].IsNull())
				{
					lva_farebase[i + 1]   = po_a.mo_C4066FareLine.mo_C4078M_.PRIMCDE[li_i]; 
				}
				else
				{
					lva_farebase[i + 1]   = po_a.mo_C4066FareLine.mo_C4078M_.PRIMCDE[li_i] +
													po_a.mo_C4066FareLine.mo_C4078M_.FAREB[li_i];
				}
			}
		}
		li_i	= li_i + 1;
	}

	for (unsigned i = 0; i < mi_IT_I; i++)
	{
		/* Transport */

		for (unsigned j = 0; j < mi_IP_I; j++)
		{
			/* all people - it looks like every person has the same set of transport itineraries */

			md_ICD_pax_id[mi_ICD_I]					= j + 1;
			md_ICD_itin_type[mi_ICD_I]				= md_IT_itin_type[i];
			md_ICD_itin_line_number[mi_ICD_I]	= i + 1;

			if (mi_IT_TYPE[i] == AIR_H_AIR     ||
				 mi_IT_TYPE[i] == AIR_H_SURFACE ||
			 	 mi_IT_TYPE[i] == AIR_H_OPEN    ||
			 	 mi_IT_TYPE[i] == AIR_H_RAIL)
			{
				/* find the line_number from the Irticket */
				
				for (unsigned idx = 0; idx < mi_ID_I; idx++)
				{
					if (md_ID_pax_id[idx] == md_IP_pax_id[j])
					{
						if (md_ID_doc_type[idx] == "BSP") {
							md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
						} else if (md_ID_association[idx].IsNull()) {
							md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
						}  else {
							AB_Integer		li_cnt;
							V_AB_Integer	lvi_list(100);

							retListNumbers(md_ID_association[idx],li_cnt,lvi_list);

							for (int lidx = 1; lidx <= li_cnt.to_long(); lidx++) {
								if (mi_IT_SEGNBR[i] == lvi_list[lidx].to_long()) {
									md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
									break;
								}
							}
						}
					}
				}

				if (mi_IT_TYPE[i] == AIR_H_AIR)
				{
					md_ICD_connection[mi_ICD_I]	= po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[mi_TicketedSeg[i]]->mo_C4017H_.STOP;
				}
				else if (mi_IT_TYPE[i] == AIR_H_SURFACE)
				{
					md_ICD_connection[mi_ICD_I]	= po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[mi_TicketedSeg[i]]->mo_C4018H_.STOP;
				}
				else if (mi_IT_TYPE[i] == AIR_H_OPEN)
				{
					md_ICD_connection[mi_ICD_I]	= po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[mi_TicketedSeg[i]]->mo_C4019H_.STOP;
				}
				else if (mi_IT_TYPE[i] == AIR_H_RAIL)
				{
					md_ICD_connection[mi_ICD_I]	= po_a.mo_C4011ticketedGrp.mo_CTicketedSeg[mi_TicketedSeg[i]]->mo_C4020H_.STOP;
				}

				md_ICD_seat[mi_ICD_I]				= po_a.mo_CPaxSegGrp.mo_CPaxSeg[j]->mo_C4096S_.SEATNBR[i + 1];

				if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine.size() > 0)
				{
					if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4079N_.BASE[i + 1] != "VOID")
					{
						md_ICD_fare_by_leg[mi_ICD_I]		= AB_Decimal(13,2,po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4079N_.BASE[i + 1]) * ld_coeff;
						//md_ICD_fare_basis[mi_ICD_I]		= lva_farebase[i + 1];
					}
				}
				else
				{
					if (po_a.mo_C4066FareLine.mo_C4079N_.BASE[i + 1] != "VOID")
					{
						md_ICD_fare_by_leg[mi_ICD_I]		= AB_Decimal(13,2,po_a.mo_C4066FareLine.mo_C4079N_.BASE[i + 1]) * ld_coeff;
						//md_ICD_fare_basis[mi_ICD_I]		= lva_farebase[i + 1];
					}
				}

				md_ICD_fare_basis[mi_ICD_I]    = lva_farebase[i + 1];
				if (  md_ICD_fare_by_leg[mi_ICD_I].IsNull() ) {
					if ( !  md_ICD_fare_basis[mi_ICD_I].IsNull() )
							md_ICD_fare_by_leg[mi_ICD_I] = AB_Decimal(13,2,0.00);
				}
			}

			if (mi_IT_TYPE[i] == AIR_H_AIR     ||
				 mi_IT_TYPE[i] == AIR_H_RAIL	  ||
			 	 mi_IT_TYPE[i] == AIR_U_AIR	  ||
				 mi_IT_TYPE[i] == AIR_U_RAIL)
			{
				md_ICD_service_status[mi_ICD_I]	= mva_IT_PNRSTAT[i + 1];
				md_ICD_baggage[mi_ICD_I]			= mva_IT_BAGALL[i + 1] ;
			}

			md_ICD_tkt_desig[mi_ICD_I]  = po_a.mo_C4066FareLine.mo_C4078M_.TKTDSG[i+1];
			//md_ICD_connection[mi_ICD_I]		= ;
			//md_ICD_smoking[mi_ICD_I]			= ;
			//md_ICD_fare_by_leg[mi_ICD_I]		= ;
			//md_ICD_fare_basis[mi_ICD_I]		= ;
			//md_ICD_cog_seat[mi_ICD_I]			= ;
			//md_ICD_crs_priced[mi_ICD_I]		= ;
			//md_ICD_service_status[mi_ICD_I]	= ;
			//md_ICD_ftp_miles[mi_ICD_I]			= ;
			//md_ICD_boarding_pass[mi_ICD_I]	= ;
			//md_ICD_special_meal[mi_ICD_I]		= ;
			//md_ICD_special_need[mi_ICD_I]		= ;
			//md_ICD_text[mi_ICD_I]				= ;

			mi_ICD_I++;
		}
	}

/* Looks like IR creates associassion to all passengers by itself 
	Confirmed with Gord/Cindy we will leave it as it is           */

	for (unsigned i = 0; i < mi_IR_I; i++)
	{
		for (unsigned j = 0; j < mi_IP_I; j++)
		{

			md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id[j];
			md_ICD_itin_type[mi_ICD_I]        = md_IR_itin_type[i];
			md_ICD_itin_line_number[mi_ICD_I] = md_IR_line_number[i];
			md_ICD_service_status[mi_ICD_I]   = md_IR_service_status[i];

			for (unsigned idx = 0; idx < mi_ID_I; idx++) {

				if (md_ID_pax_id[idx] == md_IP_pax_id[j] &&
					 md_ID_doc_type[idx] != "BSP") {	// for BSP, no rentals in itin list
					if (md_ID_association[idx].IsNull()) {
						md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
					} else {
						AB_Integer		li_cnt;
						V_AB_Integer	lvi_list(100);

						retListNumbers(md_ID_association[idx],li_cnt,lvi_list);

						for (int lidx = 1; lidx <= li_cnt.to_long(); lidx++) {
							if (mi_IR_SEGNBR[i] == lvi_list[lidx].to_long()) {
								md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
								break;
							}
						}
					}
				}
			}

			mi_ICD_I++;
		}
	}

	for (unsigned i = 0; i < mi_IO_I; i++)
	{
		/* Other */

		bool lb_association_done = false;

		if (mi_IO_TYPE[i] == AIR_U_MISC ||
			 mi_IO_TYPE[i] == AIR_U_MANTOUR) {
			for (unsigned j = 0; j < po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg.size(); j++) {

				AB_Varchar MSGTAG(2);
				AB_Varchar PSGNBR(100);
				AB_Varchar lv_tmp(3);

				if (mi_IO_TYPE[i] == AIR_U_MISC) {
					C4050U_& lo_C4050U_ = po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[j]->mo_C4050U_;
					MSGTAG = lo_C4050U_.MSGTAG;
					if (MSGTAG.NotIsNull()) {
						lv_tmp = lo_C4050U_.SEGNBR;
						PSGNBR = lo_C4050U_.PSGNBR;
					}

				} else {
					C4037U_& lo_C4037U_ = po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[j]->mo_C4037U_;
					MSGTAG = lo_C4037U_.MSGTAG;
					if (MSGTAG.NotIsNull()) {
						lv_tmp = lo_C4037U_.SEGNBR;
						PSGNBR = lo_C4037U_.PSGNBR;
					}
				}


				if (MSGTAG.NotIsNull()) {
					if (mi_IO_SEGNBR[i] == AB_Integer(lv_tmp).to_long()) {

						if (PSGNBR.NotIsNull() && PSGNBR.length() > 0)
						{
							AB_Integer		li_psgcnt;
							V_AB_Integer	lvi_psglist(100);

							retListNumbers(PSGNBR,li_psgcnt,lvi_psglist);

							for (int k= 1; k <= li_psgcnt.to_long(); k++) {
								md_ICD_pax_id[mi_ICD_I]				 = lvi_psglist[k];
								md_ICD_itin_type[mi_ICD_I]			 = md_IO_itin_type[i];
								md_ICD_itin_line_number[mi_ICD_I] = md_IO_line_number[i];

								for (unsigned idx = 0; idx < mi_ID_I; idx++) {
									if (md_ID_pax_id[idx] == lvi_psglist[k] &&
										 md_ID_doc_type[idx] != "BSP") { // for BSP, no other in itin list
										if (md_ID_association[idx].IsNull()) {
											md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
										} else {
											AB_Integer		li_cnt;
											V_AB_Integer	lvi_list(100);

											retListNumbers(md_ID_association[idx],li_cnt,lvi_list);

											for (int lidx = 1; lidx <= li_cnt.to_long(); lidx++) {
												if (mi_IO_SEGNBR[i] == lvi_list[lidx].to_long()) {
													md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
													break;
												}
											}
										}
									}
								}

								mi_ICD_I++;
							}

							lb_association_done = true;
							break;
						}
					}
				}
			}
		}
			 
		if (!lb_association_done) {
			for (unsigned j = 0; j < mi_IP_I; j++)
			{
				md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id[j];
				md_ICD_itin_type[mi_ICD_I]        = md_IO_itin_type[i];
				md_ICD_itin_line_number[mi_ICD_I] = md_IO_line_number[i];

				for (unsigned idx = 0; idx < mi_ID_I; idx++) {
					if (md_ID_pax_id[idx] == md_IP_pax_id[j] &&
						 md_ID_doc_type[idx] != "BSP") {	// for BSP, no other in itin list
						if (md_ID_association[idx].IsNull()) {
							md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
						} else {
							AB_Integer		li_cnt;
							V_AB_Integer	lvi_list(100);

							retListNumbers(md_ID_association[idx],li_cnt,lvi_list);

							for (int lidx = 1; lidx <= li_cnt.to_long(); lidx++) {
								if (mi_IO_SEGNBR[i] == lvi_list[lidx].to_long()) {
									md_ICD_line_number[mi_ICD_I]  = md_ID_line_number[idx];
									break;
								}
							}
						}
					}
				}

				mi_ICD_I++;
			}
		}
	}
}

void 
CAIR::populateIrrental(CairRecord &po_a)
{

	AB_Varchar     lv_tmp(3);

	LOG(logDEBUG) << "populateIrrental STARTS..... mi_IR_I="  << mi_IR_I  ;
	for (unsigned i = 0; i < po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg.size(); i++)
	{
		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4031U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_MANHTL */
			LOG(logDEBUG) << "populateIrrental 1. AIR_U_MANHTL"  ;

			#define a4031U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4031U_.att

			mi_IR_TYPE[mi_IR_I]					= AIR_U_AUTHTL;
			mva_IR_PSGRNBR[mi_IR_I + 1]		= a4031U_(PSGNBR);

			md_IR_itin_type[mi_IR_I]			= "H";
			md_IR_line_number[mi_IR_I]			= mi_IR_I + 1;
			md_IR_start_date[mi_IR_I]			= deriveDte(a4031U_(INDTE));
			//	md_IR_start_time[mi_IR_I]		= ;
			md_IR_origin_city_id[mi_IR_I]		= a4031U_(AIRPT);
			md_IR_end_date[mi_IR_I]				= deriveDte(a4031U_(OUTDTE));
			//	md_IR_end_time[mi_IR_I]			= ;
			//	md_IR_dest_city_id[mi_IR_I]	= ;
			md_IR_unit_type[mi_IR_I]			= "ROOM";
			md_IR_unit_ct[mi_IR_I]				= a4031U_(NBRRMS);
			md_IR_service_status[mi_IR_I]		= a4031U_(STATUS);
			md_IR_sup_id[mi_IR_I]				= a4031U_(AIRCDE).For(2) + "H";
			md_IR_source[mi_IR_I]				= "M";

			// md_IR_number_of_pax[mi_IR_I]	= ;
			//	md_IR_ex_day_charge[mi_IR_I]	= ;
			//	md_IR_ex_hr_charge[mi_IR_I]	= ;
			//	md_IR_ex_mile_charge[mi_IR_I]	= ;
			//	md_IR_free_mile_code[mi_IR_I]	= ;
			//	md_IR_free_mileage[mi_IR_I]	= ;
			// md_IR_dropoff_charge[mi_IR_I]	= ;
			//	md_IR_regular_rate[mi_IR_I]	= ;
			// md_IR_booked_rate[mi_IR_I]		= ;
			// md_IR_local_rate[mi_IR_I]	   = ;
			// md_IR_cur_id[mi_IR_I]			= ;
			//	md_IR_rate_type[mi_IR_I]		= ;
			// md_IR_rate_code[mi_IR_I]		= ;
			md_IR_rate_duration[mi_IR_I]		= "D";
			//	md_IR_rate_multip[mi_IR_I]		= ;
			//	md_IR_com_rate[mi_IR_I]			= ;
			//	md_IR_multi_lvl_rate[mi_IR_I]	= ;
			//md_IR_saving_code[mi_IR_I]			= retFromRM(po_a,"CS",0);
			// SCR: 514859
			md_IR_saving_code[mi_IR_I]			= retSavingCodeFromRM(po_a,"HS",0);
			//	md_IR_location[mi_IR_I]			= ;
			//	md_IR_location_id[mi_IR_I]		= ;
			// md_IR_site_name[mi_IR_I]		= ;
			// md_IR_addr1[mi_IR_I]				= ;

			if (a4031U_(FREE).IsNull()) {
				continue;
			}

			V_AB_Varchar	lv_arrayField(100, 100);
			int  li_count = lv_arrayField.parseStringMultDel(a4031U_(FREE), ",");
			bool lb_name_found = false;
			bool lb_addr_found = false;
			bool lb_phone_no_found = false;

			for (int i = 1; i <= li_count; i++) {
				if (lv_arrayField[i].For(6) == "PHONE-") {
					if (i == li_count && !lb_phone_no_found) {
						int li_pos;
						if ((li_pos = lv_arrayField[i].findSubstring("/CF-")) > 0) {
							md_IR_confirmation[mi_IR_I] = lv_arrayField[i].At(li_pos+4);
							lv_arrayField[i] = lv_arrayField[i].For(li_pos-1);
						}

						AB_Varchar		lv_str(50);
						lv_str = lv_arrayField[i].At(7);
						lv_str.removeLeadAndTrailBlanks();
						md_IR_phone_acode[mi_IR_I] = lv_str.For(3);
						lv_str = lv_str.At(4);
						lv_str.removeLeadAndTrailBlanks();
						md_IR_phone_no[mi_IR_I] = lv_str;
						lb_phone_no_found = true;
					} else {
						// wrong format, we are going to ignore this hotel segment
						lb_phone_no_found = false;
						break;
					}
				} else if (i == 1) {
					md_IR_site_name[mi_IR_I] = lv_arrayField[i];
					lb_name_found = true;
				} else if (i == 2) {
					md_IR_addr1[mi_IR_I] = lv_arrayField[i];
					lb_addr_found = true;
				} else {
					if (i == 3) {
						md_IR_addr2[mi_IR_I] = lv_arrayField[i];
					} else if (i == 4) {
						md_IR_addr3[mi_IR_I] = lv_arrayField[i];
					} else if (i == 5) {
						md_IR_addr4[mi_IR_I] = lv_arrayField[i];
					}
				}
			}

			if (lb_name_found && lb_addr_found && lb_phone_no_found) {
				lv_tmp                        	= a4031U_(SEGNBR);
				mi_IR_SEGNBR[mi_IR_I]				= AB_Integer(lv_tmp).to_long();
				mi_IR_I++ ;
			}
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4032U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_AUTHTL */
			LOG(logDEBUG) << "populateIrrental 2. AIR_U_AUTHTL"  ;


			#define a4032U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4032U_.att

			lv_tmp                        	= a4032U_(SEGNBR);
			mi_IR_SEGNBR[mi_IR_I]				= AB_Integer(lv_tmp).to_long();
			mi_IR_TYPE[mi_IR_I]					= AIR_U_AUTHTL;
			mva_IR_PSGRNBR[mi_IR_I + 1]		= a4032U_(PSGNBR);

			md_IR_itin_type[mi_IR_I]			= "H";
			md_IR_line_number[mi_IR_I]			= mi_IR_I + 1;
			md_IR_start_date[mi_IR_I]			= deriveDte(a4032U_(INDTE));
			//	md_IR_start_time[mi_IR_I]		= ;
			md_IR_origin_city_id[mi_IR_I]		= a4032U_(AIRPT);
			md_IR_end_date[mi_IR_I]				= deriveDte(a4032U_(OUTDTE));
			//	md_IR_end_time[mi_IR_I]			= ;
			//	md_IR_dest_city_id[mi_IR_I]	= ;
			md_IR_unit_type[mi_IR_I]			= a4032U_(SIPPRM.At(2).For(3));
			md_IR_unit_ct[mi_IR_I]				= a4032U_(NBRRMS);
			md_IR_service_status[mi_IR_I]		= a4032U_(PNRSTAT);    //US663 Chg
			md_IR_sup_id[mi_IR_I]				= a4032U_(AIRCDE).For(2) + "H";
			md_IR_source[mi_IR_I]				= "A";
			lv_tmp									= a4032U_(SIPPRM.For(1));
			md_IR_number_of_pax[mi_IR_I]		= AB_Integer(lv_tmp).to_long();;
			//	md_IR_ex_day_charge[mi_IR_I]	= ;
			//	md_IR_ex_hr_charge[mi_IR_I]	= ;
			//	md_IR_ex_mile_charge[mi_IR_I]	= ;
			//	md_IR_free_mile_code[mi_IR_I]	= ;
			//	md_IR_free_mileage[mi_IR_I]	= ;
			//md_IR_dropoff_charge[mi_IR_I]	= ;
			//	md_IR_regular_rate[mi_IR_I]	= ;
			//****************US661- Chg Start*********************************************************
			md_IR_booked_rate[mi_IR_I]       = a4032U_(RATELOC[1]);
			md_IR_local_rate[mi_IR_I]        = a4032U_(ROOMRTE[1]);
			md_IR_cur_id[mi_IR_I]			   = a4032U_(CURRCDE);
			//***********************US682 - Chg Start*************************************************
			if (md_IR_cur_id[mi_IR_I] == "USD")
			{
		   	md_IR_booked_rate[mi_IR_I] = md_IR_local_rate[mi_IR_I];
         }
			//***********************US682 - Chg End **************************************************
			//*********************US661 Chg End*******************************************************
			//	md_IR_rate_type[mi_IR_I]		= ;
			md_IR_rate_code[mi_IR_I]			= a4032U_(SIPPRM.At(5));
			md_IR_rate_duration[mi_IR_I]		= "D";
			//	md_IR_rate_multip[mi_IR_I]		= ;
			//	md_IR_com_rate[mi_IR_I]			= ;
			//	md_IR_multi_lvl_rate[mi_IR_I]	= ;
			//md_IR_saving_code[mi_IR_I]			= retFromRM(po_a,"CS",0);
			// SCR: 514859
			md_IR_saving_code[mi_IR_I]			= retSavingCodeFromRM(po_a,"HS",0);

			//	md_IR_location[mi_IR_I]			= ;
			//	md_IR_location_id[mi_IR_I]		= ;
			md_IR_site_name[mi_IR_I]			= a4032U_(HTLNME);
			md_IR_addr1[mi_IR_I]					= a4032U_(HFLA1);

			if (a4032U_(HFLA2).NotIsNull())
			{
				md_IR_addr2[mi_IR_I]				= a4032U_(HFLA2);
			}
			else
			{
				if (a4032U_(HFLCT).NotIsNull())
				{
					if (a4032U_(HFLST).NotIsNull())
					{
						md_IR_addr2[mi_IR_I]		= a4032U_(HFLCT) + " " + a4032U_(HFLST);
					}
					else
					{
						md_IR_addr2[mi_IR_I]    = a4032U_(HFLCT);
					}
				}
				else
				{
					if (a4032U_(HFLCY).NotIsNull())
					{
						if (a4032U_(HFLZP).NotIsNull())
						{
							md_IR_addr2[mi_IR_I]    = a4032U_(HFLCY) + " " + a4032U_(HFLZP);							
						}
						else
						{
							md_IR_addr2[mi_IR_I]    = a4032U_(HFLCY);
						}
					}
				}
			}

			if (md_IR_addr2[mi_IR_I] == a4032U_(HFLCY) 			||
				 md_IR_addr2[mi_IR_I] == a4032U_(HFLCY) + " " + a4032U_(HFLZP))
			{
				// we are done
			}
			else if (md_IR_addr2[mi_IR_I] == a4032U_(HFLCT))
			{
				if (a4032U_(HFLST).NotIsNull())
				{
					if (a4032U_(HFLCY).NotIsNull())
					{
						md_IR_addr3[mi_IR_I]         = a4032U_(HFLST) + " " + a4032U_(HFLCY);
					}
					else
					{
						if (a4032U_(HFLZP).NotIsNull())
						{
							md_IR_addr3[mi_IR_I]         = a4032U_(HFLCY) + " " + a4032U_(HFLZP);
						}
						else
						{
							md_IR_addr3[mi_IR_I]         = a4032U_(HFLCY);
						}
					}
				}
				else
				{
					if (a4032U_(HFLCY).NotIsNull())
					{
						if (a4032U_(HFLZP).NotIsNull())
						{
							md_IR_addr3[mi_IR_I]         = a4032U_(HFLCY) + " " + a4032U_(HFLZP);
						}
						else
						{
							md_IR_addr3[mi_IR_I]         = a4032U_(HFLCY);
						}
					}
					else
					{
						md_IR_addr3[mi_IR_I]         = a4032U_(HFLZP);
					}
				}
			}
			else if (md_IR_addr2[mi_IR_I] == a4032U_(HFLCT) + " " + a4032U_(HFLST))
			{
				if (a4032U_(HFLCY).NotIsNull())
				{
					if (a4032U_(HFLZP).NotIsNull())
					{
						md_IR_addr3[mi_IR_I]         = a4032U_(HFLCY) + " " + a4032U_(HFLZP);
					}
					else
					{
						md_IR_addr3[mi_IR_I]         = a4032U_(HFLCY);
					}
				}
				else
				{
					md_IR_addr3[mi_IR_I]         = a4032U_(HFLZP);
				}
			}

			md_IR_phone_acode[mi_IR_I]			= a4032U_(HFLPH).For(10);
			md_IR_phone_no[mi_IR_I]				= a4032U_(HFLPH).At(11);
			md_IR_fax_acode[mi_IR_I]			= a4032U_(HFLFX).For(10);
			md_IR_fax_no[mi_IR_I]				= a4032U_(HFLFX).At(11);
			md_IR_fup[mi_IR_I]					= a4032U_(PAXFFT);
			md_IR_customer_id[mi_IR_I]			= a4032U_(IDOPT);
			md_IR_guarantee[mi_IR_I]			= a4032U_(GUOPT);
			md_IR_confirmation[mi_IR_I]		= a4032U_(CONFNBR);
			md_IR_reference[mi_IR_I]			= po_a.mo_C4010AMD.RECLOC;
			md_IR_corporate_disc[mi_IR_I]		= a4032U_(CDOPT);
			//md_IR_cancel_policy[mi_IR_I]		= a4032U_(HOLD) + " " + a4032U_(CANCEL1);
			md_IR_cancel_policy[mi_IR_I]		= a4032U_(CANCEL1);

			if (a4032U_(SIOPT).NotIsNull() &&
				 a4032U_(SIOPT).length() > 0)
			{
				createIRRemark(a4032U_(SIOPT),"H");
			}

			mi_IR_I++ ;
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4033U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_DAHTL */
			LOG(logDEBUG) << "populateIrrental 3. AIR_U_DAHTL"  ;

			#define a4033U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4033U_.att

			lv_tmp                        	= a4033U_(SEGNBR);
			mi_IR_SEGNBR[mi_IR_I]				= AB_Integer(lv_tmp).to_long();
			mi_IR_TYPE[mi_IR_I]					= AIR_U_DAHTL;
			mva_IR_PSGRNBR[mi_IR_I + 1]		= a4033U_(PSGNBR);

			md_IR_itin_type[mi_IR_I]			= "H";
			md_IR_line_number[mi_IR_I]			= mi_IR_I + 1;
			md_IR_start_date[mi_IR_I]			= deriveDte(a4033U_(INDTE));
			//	md_IR_start_time[mi_IR_I]		= ;
			md_IR_origin_city_id[mi_IR_I]		= a4033U_(AIRPT);
			md_IR_end_date[mi_IR_I]				= deriveDte(a4033U_(OUTDTE));
			// md_IR_end_time[mi_IR_I]			= ;
			//	md_IR_dest_city_id[mi_IR_I]	= ;
			md_IR_unit_type[mi_IR_I]			= a4033U_(ROOMTP);
			md_IR_unit_ct[mi_IR_I]				= a4033U_(NBRRMS);
			md_IR_service_status[mi_IR_I]		= a4033U_(STATUS);
			md_IR_sup_id[mi_IR_I]				= a4033U_(AIRCDE).For(2) + "H";
			md_IR_source[mi_IR_I]				= "A";		// ??
			//	md_IR_number_of_pax[mi_IR_I]	= ;
			//	md_IR_ex_day_charge[mi_IR_I]	= ;
			//	md_IR_ex_hr_charge[mi_IR_I]	= ;
			//	md_IR_ex_mile_charge[mi_IR_I]	= ;
			//	md_IR_free_mile_code[mi_IR_I]	= ;
			//	md_IR_free_mileage[mi_IR_I]	= ;
			// md_IR_dropoff_charge[mi_IR_I]	= ;
			//	md_IR_regular_rate[mi_IR_I]	= ;
			md_IR_booked_rate[mi_IR_I]			= a4033U_(ROOMRTE);
			//	md_IR_local_rate[mi_IR_I]		= ;
			md_IR_cur_id[mi_IR_I]				= a4033U_(CURRCDE);
			//	md_IR_rate_type[mi_IR_I]		= ;
			md_IR_rate_code[mi_IR_I]			= md_IR_unit_type[mi_IR_I];
			md_IR_rate_duration[mi_IR_I]		= "D";
			//	md_IR_rate_multip[mi_IR_I]		= ;
			//	md_IR_com_rate[mi_IR_I]			= ;
			//	md_IR_multi_lvl_rate[mi_IR_I]	= ;
			//md_IR_saving_code[mi_IR_I]			= retFromRM(po_a,"CS",0);
			// SCR: 514859
			//md_IR_saving_code[mi_IR_I]			= retFromRM(po_a,"CS",0);
			md_IR_saving_code[mi_IR_I]			= retSavingCodeFromRM(po_a,"HS",0);
			//	md_IR_location[mi_IR_I]			= ;
			//	md_IR_location_id[mi_IR_I]		= ;
			md_IR_site_name[mi_IR_I]			= a4033U_(HTLNME);
			//	md_IR_addr1[mi_IR_I]				= ;
			//	md_IR_addr2[mi_IR_I]				= ;
			//	md_IR_addr3[mi_IR_I]				= ;
			//	md_IR_addr4[mi_IR_I]				= ;
			//	md_IR_phone_acode[mi_IR_I]		= ;
			//	md_IR_phone_no[mi_IR_I]			= ;
			//	md_IR_fax_acode[mi_IR_I]		= ;
			//	md_IR_fax_no[mi_IR_I]			= ;
			// md_IR_fup[mi_IR_I]				= ;
			// md_IR_customer_id[mi_IR_I]		= ;
			//	md_IR_guarantee[mi_IR_I]		= ;
			md_IR_confirmation[mi_IR_I]		= a4033U_(CONFNBR);
			md_IR_reference[mi_IR_I]			= po_a.mo_C4010AMD.RECLOC;
			//	md_IR_corporate_disc[mi_IR_I]	= ;
			//	md_IR_cancel_policy[mi_IR_I]	= ;

			mi_IR_I++ ;
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4034U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_MANCAR */
			LOG(logDEBUG) << "populateIrrental 4. AIR_U_MANCAR"  ;

			#define a4034U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4034U_.att

			mi_IR_TYPE[mi_IR_I]					= AIR_U_MANCAR;
			mva_IR_PSGRNBR[mi_IR_I + 1]		= a4034U_(PSGNBR);

			md_IR_itin_type[mi_IR_I]			= "C";
			md_IR_line_number[mi_IR_I]			= mi_IR_I + 1;
			md_IR_start_date[mi_IR_I]			= deriveDte(a4034U_(PICKUP));
			//	md_IR_start_time[mi_IR_I]		= ;
			md_IR_origin_city_id[mi_IR_I]		= a4034U_(AIRPT);
			md_IR_end_date[mi_IR_I]				= deriveDte(a4034U_(DROP));
			// md_IR_end_time[mi_IR_I]			= ;
			//	md_IR_dest_city_id[mi_IR_I]	= ;
			md_IR_unit_type[mi_IR_I]			= a4034U_(CAR);
			md_IR_unit_ct[mi_IR_I]				= a4034U_(NBRCARS);
			md_IR_service_status[mi_IR_I]		= a4034U_(STATUS);
			md_IR_sup_id[mi_IR_I]				= a4034U_(AIRCDE).For(2) + "C";
			md_IR_source[mi_IR_I]				= "M";		// ??
			//	md_IR_number_of_pax[mi_IR_I]	= ;
			//	md_IR_ex_day_charge[mi_IR_I]	= ;
			//	md_IR_ex_hr_charge[mi_IR_I]	= ;
			//	md_IR_ex_mile_charge[mi_IR_I]	= ;
			//	md_IR_free_mile_code[mi_IR_I]	= ;
			//	md_IR_free_mileage[mi_IR_I]	= ;
			// md_IR_dropoff_charge[mi_IR_I]	= ;
			//	md_IR_regular_rate[mi_IR_I]	= ;
			// md_IR_booked_rate[mi_IR_I]		= ;
			//	md_IR_local_rate[mi_IR_I]		= ;
			//	md_IR_cur_id[mi_IR_I]			= ;
			//	md_IR_rate_type[mi_IR_I]		= ;
			// md_IR_rate_code[mi_IR_I]		= ;
			md_IR_rate_duration[mi_IR_I]		= "D";
			//	md_IR_rate_multip[mi_IR_I]		= ;
			//	md_IR_com_rate[mi_IR_I]			= ;
			//	md_IR_multi_lvl_rate[mi_IR_I]	= ;
			// SCR: 514859
			//md_IR_saving_code[mi_IR_I]			= retFromRM(po_a,"CS",0);
			md_IR_saving_code[mi_IR_I]			= retSavingCodeFromRM(po_a,"CS",0);
			//	md_IR_location[mi_IR_I]			= ;
			//	md_IR_location_id[mi_IR_I]		= ;
			//	md_IR_site_name[mi_IR_I]		= ;
			//	md_IR_addr1[mi_IR_I]				= ;
			//	md_IR_addr2[mi_IR_I]				= ;
			//	md_IR_addr3[mi_IR_I]				= ;
			//	md_IR_addr4[mi_IR_I]				= ;
			//	md_IR_phone_acode[mi_IR_I]		= ;
			//	md_IR_phone_no[mi_IR_I]			= ;
			//	md_IR_fax_acode[mi_IR_I]		= ;
			//	md_IR_fax_no[mi_IR_I]			= ;
			// md_IR_fup[mi_IR_I]				= ;
			// md_IR_customer_id[mi_IR_I]		= ;
			//	md_IR_guarantee[mi_IR_I]		= ;
			// md_IR_confirmation[mi_IR_I]	= ;
			// md_IR_reference[mi_IR_I]		= ;
			//	md_IR_corporate_disc[mi_IR_I]	= ;
			//	md_IR_cancel_policy[mi_IR_I]	= ;

			if (a4034U_(FREE).IsNull()) {
				continue;
			}

			V_AB_Varchar	lv_arrayField(100, 100);
			int 	li_count = lv_arrayField.parseStringMultDel(a4034U_(FREE), ",");
			AB_Varchar	lv_remark1(80), lv_remark2(80);
			bool			lb_ignore = false;

			for (int i = 1; i <= li_count; i++) {
				int 	li_pos;
				if ((li_pos = lv_arrayField[i].findSubstring("PICKUP TIME-")) > 0) {
					if (i == 1) {
						md_IR_start_time[mi_IR_I] = lv_arrayField[i].At(li_pos+12).For(4);
						if (md_IR_start_time[mi_IR_I].findFirstCharNotInList("0123456789") != 0) {
							md_IR_start_time[mi_IR_I] = AB_null;
						}
					} else {	// invalid format
						lb_ignore = true;
						break;
					}
				} else 

				if ((li_pos = lv_arrayField[i].findSubstring("DROPOFF TIME-")) > 0) {
					if (i == 1 || lv_arrayField[i-1].findSubstring("PICKUP TIME-") > 0) {
						md_IR_end_time[mi_IR_I] = lv_arrayField[i].At(li_pos+13).For(4);
						if (md_IR_end_time[mi_IR_I].findFirstCharNotInList("0123456789") != 0) {
							 md_IR_end_time[mi_IR_I] = AB_null;
						}
					} else {	// invalid format
						lb_ignore = true;
						break;
					}
				} else {
					if ((li_pos = lv_arrayField[i].findSubstring("/CF-")) > 0) {
						if (i != li_count) {	// invalid format
							lb_ignore = true;
							break;
						}

						md_IR_confirmation[mi_IR_I] = lv_arrayField[i].At(li_pos+4);
						lv_arrayField[i] = lv_arrayField[i].For(li_pos-1);
					}

					if (lv_remark1.IsNull()) {
						lv_remark1 = lv_arrayField[i];
					} else if (i != li_count) {	// invalid format
						lb_ignore = true;
						break;
					}

					lv_remark2 = lv_arrayField[i];
				}
			}

			if (!lb_ignore) {
				if (!lv_remark1.IsNull()) {
					createIRRemark(lv_remark1, "C");
				}

				if (!lv_remark2.IsNull()) {
					createIRRemark(lv_remark2, "C");
				}

				lv_tmp                        	= a4034U_(SEGNBR);
				mi_IR_SEGNBR[mi_IR_I]				= AB_Integer(lv_tmp).to_long();
				mi_IR_I++ ;
			}
		}

		if (po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4035U_.MSGTAG.NotIsNull())
		{
			/* AIR_U_AUTCAR */
			LOG(logDEBUG) << "populateIrrental 5. AIR_U_AUTCAR(ADDRESS)"  ;

			#define a4035U_(att)\
			po_a.mo_C4026unticketedGrp.mo_CUnticketedSeg[i]->mo_C4035U_.att

			lv_tmp                        	= a4035U_(SEGNBR);
			mi_IR_SEGNBR[mi_IR_I]				= AB_Integer(lv_tmp).to_long();
			mi_IR_TYPE[mi_IR_I]					= AIR_U_AUTCAR;
			mva_IR_PSGRNBR[mi_IR_I + 1]		= a4035U_(PSGNBR);

			md_IR_itin_type[mi_IR_I]			= "C";
			md_IR_line_number[mi_IR_I]			= mi_IR_I + 1;
			md_IR_start_date[mi_IR_I]			= deriveDte(a4035U_(PICKUP));
			md_IR_start_time[mi_IR_I]			= a4035U_(ARROPT).At(a4035U_(ARROPT).length()-3);
			if (md_IR_start_time[mi_IR_I].findFirstCharNotInList("0123456789") != 0) {
				md_IR_start_time[mi_IR_I] = AB_null;
			}

			md_IR_origin_city_id[mi_IR_I]		= a4035U_(AIRPT);
			md_IR_end_date[mi_IR_I]				= deriveDte(a4035U_(DROP));
			md_IR_end_time[mi_IR_I]				= a4035U_(RTTME);
			if (md_IR_end_time[mi_IR_I].findFirstCharNotInList("0123456789") != 0) {
				md_IR_end_time[mi_IR_I] = AB_null;
			}

			//	md_IR_dest_city_id[mi_IR_I]	= ;
			md_IR_unit_type[mi_IR_I]			= a4035U_(CAR);
			md_IR_unit_ct[mi_IR_I]				= a4035U_(NBRCARS);
			md_IR_service_status[mi_IR_I]		= a4035U_(PNRSTAT);   //US663 Chg
			md_IR_sup_id[mi_IR_I]				= a4035U_(AIRCDE).For(2) + "C";
			md_IR_source[mi_IR_I]				= "A";
			//	md_IR_number_of_pax[mi_IR_I]	= ;
			md_IR_dropoff_charge[mi_IR_I]		= a4035U_(DCOPT);

			//	md_IR_cur_id[mi_IR_I]			= ;
			//	md_IR_local_rate[mi_IR_I]		= ;
			//	md_IR_regular_rate[mi_IR_I]	= ;
			// md_IR_booked_rate[mi_IR_I]			= a4035U_(RBOPT); // ?? or rgopt or rqopt
			//	md_IR_free_mileage[mi_IR_I]	= ;
			//	md_IR_rate_duration[mi_IR_I]	= ;
			//	md_IR_free_mile_code[mi_IR_I]	= ;
			//	md_IR_ex_day_charge[mi_IR_I]	= ;
			//	md_IR_ex_hr_charge[mi_IR_I]	= ;
			//	md_IR_ex_mile_charge[mi_IR_I]	= ;

			AB_Varchar	lv_rbgq_opt(59);
			// US662 - STARTS // Consider RG first and RQ second
			if (a4035U_(RGOPT).NotIsNull() && a4035U_(RGOPT) != "") {
				lv_rbgq_opt = a4035U_(RGOPT);
			} else if (a4035U_(RQOPT).NotIsNull() && a4035U_(RQOPT) != "") {
				lv_rbgq_opt = a4035U_(RQOPT);
			} else {
				lv_rbgq_opt = a4035U_(RBOPT);
			}
			// US662 - ENDS
		// SCR: 514859: retrieve car address
      int li;
      for (li = 1; li <= a4035U_(PUPOPT).length(); li++) {
         if (a4035U_(PUPOPT).At(li).For(1) == "*") {
            li++; // get ride of '*'
            break; }
      	}
      if (li <= a4035U_(PUPOPT).length()) {
         md_IR_addr1[mi_IR_I]          = a4035U_(PUPOPT).At(li);
         }
		// SCR: 514859: retrieve car address


			retCarRateData(lv_rbgq_opt,
							md_IR_cur_id[mi_IR_I],
							md_IR_local_rate[mi_IR_I],
							md_IR_booked_rate[mi_IR_I],
							md_IR_regular_rate[mi_IR_I],
							md_IR_free_mileage[mi_IR_I],
							md_IR_rate_duration[mi_IR_I],
							md_IR_free_mile_code[mi_IR_I],
							md_IR_ex_day_charge[mi_IR_I],
							md_IR_ex_hr_charge[mi_IR_I],
							md_IR_ex_mile_charge[mi_IR_I]);

			//	md_IR_rate_type[mi_IR_I]		= ;
			md_IR_rate_code[mi_IR_I]			= a4035U_(RCOPT);
			//	md_IR_rate_multip[mi_IR_I]		= ;
			//	md_IR_com_rate[mi_IR_I]			= ;
			//	md_IR_multi_lvl_rate[mi_IR_I]	= ;
			// 514859
			//md_IR_saving_code[mi_IR_I]			= retFromRM(po_a,"CS",0);
			md_IR_saving_code[mi_IR_I]			= retSavingCodeFromRM(po_a,"CS",0);
			//	md_IR_location[mi_IR_I]			= ;
			//	md_IR_location_id[mi_IR_I]		= ;
			//	md_IR_site_name[mi_IR_I]		= ;
			//	md_IR_addr1[mi_IR_I]				= ;
			//	md_IR_addr2[mi_IR_I]				= ;
			//	md_IR_addr3[mi_IR_I]				= ;
			//	md_IR_addr4[mi_IR_I]				= ;
			//	md_IR_phone_acode[mi_IR_I]		= ;
			//	md_IR_phone_no[mi_IR_I]			= ;
			//	md_IR_fax_acode[mi_IR_I]		= ;
			//	md_IR_fax_no[mi_IR_I]			= ;
			md_IR_fup[mi_IR_I]					= a4035U_(FTOPT);
			md_IR_customer_id[mi_IR_I]			= a4035U_(IDOPT);
			//	md_IR_guarantee[mi_IR_I]		= ;
			md_IR_confirmation[mi_IR_I]		= a4035U_(CONFNBR);
			md_IR_reference[mi_IR_I]			= po_a.mo_C4010AMD.RECLOC;
			md_IR_corporate_disc[mi_IR_I]		= a4035U_(CDOPT);	
			//	md_IR_cancel_policy[mi_IR_I]	= ;

			if (a4035U_(SIOPT).NotIsNull() && 
				 a4035U_(SIOPT).length() > 0)
			{
				createIRRemark(a4035U_(SIOPT),"C");
			}

			mi_IR_I++ ;
		}
	}
	LOG(logDEBUG) << "populateIrrental EXIT mi_IR_I="  << mi_IR_I;
	LOG(logDEBUG) << "populateIrrental EXIT md_IR_addr1=[" << md_IR_addr1[mi_IR_I-1] << "]";
}

void 
CAIR::populateIrreceipt(CairRecord &po_a)
{
	/*
	md_RC_cop_id					= ;
	md_RC_bra_id					= ;
	md_RC_lang_id					= ;
	
	md_RC_name[mi_RC_I]			= ;
	md_RC_bank_acc[mi_RC_I]		= ;
	md_RC_total_amt[mi_RC_I]	= ;
	md_RC_gl_acc1[mi_RC_I]		= ;
	md_RC_gl_amt1[mi_RC_I]		= ;
	md_RC_gl_acc2[mi_RC_I]		= ;
	md_RC_gl_amt2[mi_RC_I]		= ;
	md_RC_gl_acc3[mi_RC_I]		= ;
	md_RC_gl_amt3[mi_RC_I]		= ;
	md_RC_form_of_pay[mi_RC_I]	= ;
	md_RC_cc_id[mi_RC_I]			= ;
	md_RC_cc_no[mi_RC_I]			= ;
	md_RC_cc_exp_date[mi_RC_I]	= ;
	md_RC_auth_no[mi_RC_I]		= ;
	md_RC_valid[mi_RC_I]			= ;
	*/
}

void 
CAIR::populateIrdistribution(CairRecord &po_a)
{
	/*
	md_DI_rec_line[mi_DI_I]	= ;
	md_DI_gl_acc[mi_DI_I]	= ;
	md_DI_gl_amt[mi_DI_I]	= ;
	*/
}

void 
CAIR::populateIremailaddress(CairRecord &po_a)
{
	for (unsigned  i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size(); i++)
	{
		int            li_count;
		V_AB_Varchar   lva(255,255);
		li_count = lva.parseStringMultDel(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_C4088I_.APINFO,"//");

		for (unsigned i = 1; i <= li_count; i++)
		{
			if (lva[i].NotIsNull())
			{
				if (lva[i].For(2) == "E-")
				{
					AB_Varchar lv_tmp(255);

					lv_tmp	= lva[i].At(3);

					if (editEmailaddress(lv_tmp))
					{
						mc_EM_email_type[mi_EM_I]  = "C";
						mt_EM_email_id[mi_EM_I]		= lva[i].At(3);

						mi_EM_I++;
					}
				}
			}
		}
	}
}

void 
CAIR::populateIrremark(CairRecord &po_a)
{
	ENTER;

	if (!retFromRM(po_a,"SC",0).IsNull()) {
		md_IRMK_type[mi_IRMK_I] 	= "MLS";
		md_IRMK_remark[mi_IRMK_I]	= retFromRM(po_a,"SC",0);
		md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;
		mi_IRMK_I++;
	}

	if (!retFromRM(po_a,"PR",0).IsNull()) {

		AB_Text lt_pr = retFromRM(po_a,"PR",0);

		md_IRMK_type[mi_IRMK_I] 	= "PR";
		md_IRMK_remark[mi_IRMK_I]	= lt_pr;
		md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;
		mi_IRMK_I++;
	}

   for (unsigned  i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM.size(); i++)
   {
      AB_Varchar  lv_remark(255);
      AB_Varchar  lv_udidNo(10);

      lv_remark = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK;

      if (lv_remark.For(2) == "*U") {
         lv_remark = lv_remark.At(3);
         int li_pos = lv_remark.findSubstring("-");
         lv_udidNo = lv_remark.For(li_pos - 1);

         if (lv_udidNo.findFirstCharNotInList("0123456789") == 0 )
         {
             createUDIDifRange1to99(lv_udidNo, lv_remark.At(li_pos + 1));
         }

      }
   }

	for (unsigned i = 0; i < po_a.mo_CRIIGrp.mo_C4148RII.size(); i++)
	{
		/* Invoice/Itinerary Remark */
		/* Only Segment association will be considered */

		if (po_a.mo_CRIIGrp.mo_C4148RII[i]->SEGNBR.NotIsNull())
		{
			/* Associated Remark PNx where x = itin_type */

			int				j;
			AB_Integer		li_cnt;
			V_AB_Integer	lvi_list(100);
			AB_Varchar		lv_itin_type(1);

			retListNumbers(po_a.mo_CRIIGrp.mo_C4148RII[i]->SEGNBR,li_cnt,lvi_list);
		
			j = 1;

			while ( j <= li_cnt.to_long() )
			{
				retSegAttributes(lvi_list[j].to_long(),
									  lv_itin_type,
									  md_IRMK_tra_line_number[mi_IRMK_I],
									  md_IRMK_ren_line_number[mi_IRMK_I],
									  md_IRMK_oth_line_number[mi_IRMK_I]);
											
				md_IRMK_type[mi_IRMK_I]					= "PN" + lv_itin_type;
				md_IRMK_remark[mi_IRMK_I]				= po_a.mo_CRIIGrp.mo_C4148RII[i]->INVINTRM;
				md_IRMK_itin_type[mi_IRMK_I]			= lv_itin_type;

				j++;

				mi_IRMK_I++;
			}
		}
		else
		{
			/* Unassociated Remark */

			createUnAssociatedRemark(po_a.mo_CRIIGrp.mo_C4148RII[i]->INVINTRM);
		}
	}

	for (unsigned i = 0; i < po_a.mo_CRIRGrp.mo_C4149RIR.size(); i++)
	{
		/* Itinerary Remark */
		/* Only Segment association will be considered */

		if (po_a.mo_CRIRGrp.mo_C4149RIR[i]->SEGNBR.NotIsNull())
		{
			/* Associated Remark PNx where x = itin_type */

			int				j;
			AB_Integer		li_cnt;
			V_AB_Integer	lvi_list(100);
			AB_Varchar		lv_itin_type(1);

			retListNumbers(po_a.mo_CRIRGrp.mo_C4149RIR[i]->SEGNBR,li_cnt,lvi_list);
		
			j = 1;

			while ( j <= li_cnt.to_long() )
			{

				retSegAttributes(lvi_list[j].to_long(),
									  lv_itin_type,
									  md_IRMK_tra_line_number[mi_IRMK_I],
									  md_IRMK_ren_line_number[mi_IRMK_I],
									  md_IRMK_oth_line_number[mi_IRMK_I]);
											
				md_IRMK_type[mi_IRMK_I]					= "PN" + lv_itin_type;
				md_IRMK_remark[mi_IRMK_I]				= po_a.mo_CRIRGrp.mo_C4149RIR[i]->INTRM;
				md_IRMK_itin_type[mi_IRMK_I]			= lv_itin_type;

				j++;

				mi_IRMK_I++;
			}
		}
		else
		{
			/* Unassociated Remark */

			createUnAssociatedRemark(po_a.mo_CRIRGrp.mo_C4149RIR[i]->INTRM);
		}
	}

	for (unsigned i = 0; i < po_a.mo_CRIFGrp.mo_C4150RIF.size(); i++)
	{
		/* Itinerary Remark */
		/* Only Segment association will be considered */

		if (po_a.mo_CRIFGrp.mo_C4150RIF[i]->SEGNBR.NotIsNull())
		{
			/* Associated Remark PNx where x = itin_type */

			int				j;
			AB_Integer		li_cnt;
			V_AB_Integer	lvi_list(100);
			AB_Varchar		lv_itin_type(1);

			retListNumbers(po_a.mo_CRIFGrp.mo_C4150RIF[i]->SEGNBR,li_cnt,lvi_list);
		
			j = 1;

			while ( j <= li_cnt.to_long() )
			{

				retSegAttributes(lvi_list[j].to_long(),
									  lv_itin_type,
									  md_IRMK_tra_line_number[mi_IRMK_I],
									  md_IRMK_ren_line_number[mi_IRMK_I],
									  md_IRMK_oth_line_number[mi_IRMK_I]);
											
				md_IRMK_type[mi_IRMK_I]					= "PN" + lv_itin_type;
				md_IRMK_remark[mi_IRMK_I]				= po_a.mo_CRIFGrp.mo_C4150RIF[i]->INVRM;
				md_IRMK_itin_type[mi_IRMK_I]			= lv_itin_type;

				j++;

				mi_IRMK_I++;
			}
		}
		else
		{
			/* Unassociated Remark */

			createUnAssociatedRemark(po_a.mo_CRIFGrp.mo_C4150RIF[i]->INVRM);
		}
	}

	/*
	md_IRMK_line_number[mi_IRMK_I]		= ;
	md_IRMK_pax_id[mi_IRMK_I]				= ;
	md_IRMK_type[mi_IRMK_I]					= ;
	md_IRMK_remark[mi_IRMK_I]				= ;
	md_IRMK_itin_type[mi_IRMK_I]			= ;
	md_IRMK_tra_line_number[mi_IRMK_I]	= ;
	md_IRMK_ren_line_number[mi_IRMK_I]	= ;
	md_IRMK_oth_line_number[mi_IRMK_I]	= ;
	md_IRMK_udi_line_number[mi_IRMK_I]	= ;
	*/

	if (!retFromRM(po_a,"GOW",0).IsNull()) {
	  createUniqueRemark("MEM", retFromRM(po_a,"GOW",0));
	}

	if (!retFromRM(po_a,"NUC",0).IsNull()) {
	  createUniqueRemark("NUC", "Y");
	}

	if (!retFromRM(po_a,"NE",0).IsNull()) {
	  //createRemarkRows(pRemarkParser->parse(retFromRM(po_a,"NE",0).to_string(), "NE"));
			// ***REMARK_PARSER***
	  //moved to generic pRemarkParser->parse(retFromRM(po_a,"NE",0).to_string());
			//moved to generic createRemarkRows(pRemarkParser->parsed("NE"));
	}

   EXIT;
}

void 
CAIR::populateNetTicketInfo(CairRecord &po_a)
{
	/*
	md_NETT_pax_id[mi_NETT_I]		= ;
	md_NETT_ticket_no[mi_NETT_I]	= ;
	md_NETT_base_cost[mi_NETT_I]	= ;
	md_NETT_commission[mi_NETT_I]	= ;
	*/
}

void 
CAIR::populateMembership(CairRecord &po_a)
{
	/*
	md_MEM_pax_id[mi_MEM_I]	= ;
	md_MEM_number[mi_MEM_I]	= ;
	*/
}

void 
CAIR::populateIrtaxdetail(CairRecord &po_a)
{
LOG(logDEBUG) << "\npopulateIrtaxdetail: mi_ITD_I= " << mi_ITD_I  ;
	for (unsigned i = 0; i < mi_ID_I; i++)
	{
		int 	li_idx;

		if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[i].to_long()-1]->mo_CFareLineGrp.mo_C4066FareLine.size() > 0)
		{
			int   j;

			j = 1;

			while (po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[i].to_long()-1]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4068KFT.TCURR[j].NotIsNull())
			{

				if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[i].to_long()-1]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4068KFT.TAXA[j].findFirstCharNotInList(
						"0123456789.") == 0) {
					D_amount ld_taxAmount = po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[i].to_long()-1]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4068KFT.TAXA[j];
					D_tax		ld_tax_id = po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[i].to_long()-1]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4068KFT.TAXC[j];

					if (ld_tax_id == "XF" &&
						 po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[i].to_long()-1]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4081Q_.MSGTAG.NotIsNull()) {
						getXFbreakdown(ld_taxAmount,
											md_ID_pax_id[i],
											md_ID_line_number[i],
											po_a.mo_CPaxSegGrp.mo_CPaxSeg[md_ID_pax_id[i].to_long()-1]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4081Q_.FARE);	
					} else {
						/*
						 * we should have taxes CA,RC,XG,XQ only once for each. 
						 * Should they appear multiple times, we add them up
						 */
						if ((ld_tax_id == "CA" ||
							  ld_tax_id == "RC" ||
							  ld_tax_id == "XG" ||
							  ld_tax_id == "XQ")	&&
							 (li_idx = findTax(ld_tax_id, md_ID_pax_id[i], md_ID_line_number[i])) >= 0) {

							md_ITD_tax_amount[li_idx] = D_amount(md_ITD_tax_amount[li_idx]) + ld_taxAmount;

						} else {
							md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[i];
							md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[i];
							md_ITD_tax_id[mi_ITD_I]			= ld_tax_id;
							//md_ITD_city_id[mi_ITD_I]    = ;

							md_ITD_tax_amount[mi_ITD_I++]	= ld_taxAmount;
						}
					}
				}

				j++;

			}
		}
		else if (po_a.mo_C4066FareLine.mo_C4068KFT.MSGTAG.NotIsNull())
		{
			int   j;

			j = 1;

			while (po_a.mo_C4066FareLine.mo_C4068KFT.TCURR[j].NotIsNull())
			{
				if (po_a.mo_C4066FareLine.mo_C4068KFT.TAXA[j].findFirstCharNotInList(
						"0123456789.") == 0) {
					D_amount ld_taxAmount = po_a.mo_C4066FareLine.mo_C4068KFT.TAXA[j];
					D_tax		ld_tax_id = po_a.mo_C4066FareLine.mo_C4068KFT.TAXC[j];


					if (ld_tax_id == "XF" &&
						 po_a.mo_C4066FareLine.mo_C4081Q_.MSGTAG.NotIsNull()) {

						getXFbreakdown(ld_taxAmount,
											md_ID_pax_id[i],
											md_ID_line_number[i],
											po_a.mo_C4066FareLine.mo_C4081Q_.FARE);	
					} else {
						if ((ld_tax_id == "CA" ||
							  ld_tax_id == "RC" ||
							  ld_tax_id == "XG" ||
							  ld_tax_id == "XQ")	&&
							 (li_idx = findTax(ld_tax_id, md_ID_pax_id[i], md_ID_line_number[i])) >= 0) {

							md_ITD_tax_amount[li_idx] = D_amount(md_ITD_tax_amount[li_idx]) + ld_taxAmount;
						} else {
							md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[i];
							md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[i];
							md_ITD_tax_id[mi_ITD_I]			= ld_tax_id;
							//md_ITD_city_id[mi_ITD_I]    = ;
							md_ITD_tax_amount[mi_ITD_I++]	= ld_taxAmount;
						}
					}
				}

				j++;
			}
		}
      else if (po_a.mo_C4066FareLine.mo_C4074TAX.MSGTAG.NotIsNull())
      {
         int   j;

         j = 1;

		   //LOG(logDEBUG) << "MEL: po_a.mo_C4066FareLine.mo_C4074TAX.TCURR.Length() " << po_a.mo_C4066FareLine.mo_C4074TAX.TCURR.Length();

         while (j <= po_a.mo_C4066FareLine.mo_C4074TAX.TCURR.Length().to_long() && po_a.mo_C4066FareLine.mo_C4074TAX.TCURR[j].NotIsNull())
         {
            if (po_a.mo_C4066FareLine.mo_C4074TAX.TAXA[j].findFirstCharNotInList(
                  "0123456789.") == 0) {

               D_amount ld_taxAmount = po_a.mo_C4066FareLine.mo_C4074TAX.TAXA[j];
               D_tax    ld_tax_id = po_a.mo_C4066FareLine.mo_C4074TAX.TAXC[j];


               if (ld_tax_id == "XF" &&
                   po_a.mo_C4066FareLine.mo_C4081Q_.MSGTAG.NotIsNull()) {

                  getXFbreakdown(ld_taxAmount,
                                 md_ID_pax_id[i],
                                 md_ID_line_number[i],
                                 po_a.mo_C4066FareLine.mo_C4081Q_.FARE);

               } else {

                  if ((ld_tax_id == "CA" ||
                       ld_tax_id == "RC" ||
                       ld_tax_id == "XG" ||
                       ld_tax_id == "XQ") &&
                      (li_idx = findTax(ld_tax_id, md_ID_pax_id[i], md_ID_line_number[i])) >= 0) {

                     md_ITD_tax_amount[li_idx] = D_amount(md_ITD_tax_amount[li_idx]) + ld_taxAmount;

                  } else {

                     md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[i];
                     md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[i];
                     md_ITD_tax_id[mi_ITD_I]       = ld_tax_id;
                     md_ITD_tax_amount[mi_ITD_I++] = ld_taxAmount;
                  }
               }
            }

            j++;

         }
      }

	}


	/*
	 * remarks for even exchange tickets still missing tax information.
	 * We repair them here after processing taxes
	 */
	for (unsigned i = 0; i < mi_IRMK_I; i++) {
		if (md_IRMK_type[i] == "COS" && md_IRMK_remark[i].findSubstring("XT") == 0) {
			// found even exchange remark line
			AB_Integer li_doc_line = md_IRMK_remark[i].For(2);
			AB_Char	  lc_XT_amount(8);

			D_amount    ld_XT_amount;
			ld_XT_amount = D_amount(0);

			for (unsigned j = 0; j < mi_ITD_I; j++) {
				if (md_ITD_line_number[j] == li_doc_line) {
					ld_XT_amount = ld_XT_amount + D_amount(md_ITD_tax_amount[j]);
				}
			}

			lc_XT_amount = ld_XT_amount;

			md_IRMK_remark[i] = md_IRMK_remark[i] + "T2:" + lc_XT_amount + "XT";
		}
	}

	/*reset the non arc taxes that were not supposed to be charged*/
	if ( md_ITD_tax_id[0].NotIsNull() && md_ITD_line_number[0].to_long() > 1)
	{
		D_line_number tmpLineNumber ;
		D_passenger_no tmpPaxId;

		tmpLineNumber = md_ITD_line_number[0] ;
		tmpPaxId = md_ITD_pax_id[0] ;

		for (int ii = 1; ii < 501; ii++)
		{
			if ( md_ITD_line_number[ii].NotIsNull() )
			{
				if ( (md_ITD_line_number[ii] == tmpLineNumber ) &&
					 	( md_ITD_pax_id[ii] == tmpPaxId) )
				{
     					md_ITD_line_number[ii]  = AB_null;
      				md_ITD_pax_id[ii]       = AB_null;
      				md_ITD_tax_amount[ii]   = AB_null;
      				md_ITD_tax_id[ii]       = AB_null;
      				md_ITD_city_id[ii]       = AB_null;
				}
				else continue;
			}
			else break; 
		} /* end of for loop */

	}

}

void 
CAIR::populateIrpayment(CairRecord &po_a)
{
   LOG(logDEBUG) << "Enter======== populateIrpayment(CairRecord &po_a)]"  ;
	if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFPGrp.mo_C4118FP.size() > 0)
	{
		
		AB_Varchar		lv_form_of_pay(2);
		AB_Varchar		lv_cc_id(2);
		AB_Varchar		lv_cc_no(20);
		AB_Varchar		lv_cc_exp_date(4);
		AB_Varchar		lv_auth_no(10);

		getFormOfPay(po_a,
						 lv_form_of_pay,
						 lv_cc_id,
						 lv_cc_no,
						 lv_cc_exp_date,
						 lv_auth_no);

		LOG(logDEBUG) << "AM======== populateIrpayment(CairRecord &po_a)] mi_ID_I=[" << mi_ID_I << "]"  ;
		for (unsigned i = 0; i < mi_ID_I; i++)
		{
			/*
			 * FOP of NBSP is not in mo_C4118FP[0]->PAYMNT. 
			 * We will handle this separately
			 */

		   LOG(logDEBUG) << "MEL======== populateIrpayment(CairRecord &po_a)] md_ID_doc_type[i]=[" << md_ID_doc_type[i] << "]"  ;
			if (md_ID_doc_type[i] == "BSP") {
			
         	LOG(logDEBUG) << "MEL======== 1mo_C4067K_.TOTAMT=[]"  ;

				md_IRP_pax_id[mi_IRP_I]			= md_ID_pax_id[i];

         	LOG(logDEBUG) << "MEL======== 2mo_C4067K_.TOTAMT=[]"  ;

				md_IRP_line_number[mi_IRP_I]	= md_ID_line_number[i];

         	LOG(logDEBUG) << "MEL======== 3mo_C4067K_.TOTAMT=[]"  ;

				md_IRP_pay_id[mi_IRP_I]			= 1;

         	LOG(logDEBUG) << "MEL======== 4mo_C4067K_.TOTAMT=[]"  ;

				//start of SCR 503470

				if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine.size() > 0)
				{
         		LOG(logDEBUG) << "MEL======== 5mo_C4067K_.TOTAMT=[]"  ;
					md_IRP_amount[mi_IRP_I]		= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFareLineGrp.mo_C4066FareLine[0]->mo_C4067K_.TOTAMT;
				}
				else
				{
         		LOG(logDEBUG) << "MEL======== 6mo_C4067K_.TOTAMT=[]"  ;
					md_IRP_amount[mi_IRP_I]    = po_a.mo_C4066FareLine.mo_C4067K_.TOTAMT;
         		LOG(logDEBUG) << "MEL======== 61mo_C4067K_.TOTAMT=[]"  ;
				}

         	LOG(logDEBUG) << "MEL======== 62mo_C4067K_.TOTAMT=[]"  ;
				if ( po_a.mo_C4066FareLine.mo_C4067K_.TOTAMT.IsNull() && md_IRP_amount[mi_IRP_I].IsNull() )
         	{
         		LOG(logDEBUG) << "MEL======== 7mo_C4067K_.TOTAMT=[]"  ;
            	md_IRP_amount[mi_IRP_I]         = po_a.mo_C4066FareLine.mo_C4071KS_.TOTAMT;
         	}

         LOG(logDEBUG) << "MEL======== 8mo_C4067K_.TOTAMT=[]"  ;


			md_IRP_auth_no[mi_IRP_I]		= lv_auth_no;
			md_IRP_form_of_pay[mi_IRP_I]	= lv_form_of_pay;
			md_IRP_cc_id[mi_IRP_I]			= lv_cc_id;
			md_IRP_cc_no[mi_IRP_I]			= lv_cc_no;
			md_IRP_cc_exp_date[mi_IRP_I]	= lv_cc_exp_date;
			md_IRP_auth_no[mi_IRP_I]		= lv_auth_no;
			//md_IRP_auth_source[mi_IRP_I]	= ;

				mi_IRP_I++;
			}
		}
	}
}


void 
CAIR::populateIrpayment(CairRecord& 		po_a,
								int 					pi_id, 
								const AB_Text& 	pt_fop)
{
   md_IRP_amount[mi_IRP_I] 		= md_ID_base_cost[pi_id] ;
   md_IRP_pax_id[mi_IRP_I] 		= md_ID_pax_id[pi_id] ;
   md_IRP_line_number[mi_IRP_I]  = md_ID_line_number[pi_id] ;
   md_IRP_pay_id[mi_IRP_I] 		= AB_Integer(1) ;

   if ( pt_fop == "CA" )
      md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
   else if ( pt_fop == "AR" )
      md_IRP_form_of_pay[mi_IRP_I] = "CK" ;
   else if ( pt_fop != "CC")
      md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
   else
      md_IRP_form_of_pay[mi_IRP_I] = "CC" ;


   if ( md_IRP_form_of_pay[mi_IRP_I] == "CC" )
	{
		AB_Varchar  lv_payment(500);
		lv_payment = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFPGrp.mo_C4118FP[0]->PAYMNT;

		if (lv_payment.For(2) == "CC") {

			md_IRP_cc_id[mi_IRP_I] = lv_payment.At(3).For(2);

			int i = lv_payment.findSubstring("/");

			if (i > 0)
			{
				md_IRP_cc_no[mi_IRP_I] 			= lv_payment.At(5).For(i - 5);
				md_IRP_cc_exp_date[mi_IRP_I] 	= lv_payment.At(i + 1);
			}
		}
	}
}

void
CAIR::processSFC(CairRecord &po_a)
{

	AB_Varchar lv_svcSF(100), lv_svcFEE(100);

	lv_svcSF  = retFromRM(po_a, "SF", 0);
	lv_svcFEE = retFromRM(po_a, "FEE", 0);

	if (lv_svcSF.IsNull() && lv_svcFEE.IsNull()) {
		return;
	}

	if (!lv_svcSF.IsNull() && lv_svcFEE.IsNull()) {
		mi_rejectirId = 3611;
		return;
	}

	if (lv_svcSF.IsNull() && !lv_svcFEE.IsNull()) {
		mi_rejectirId = 3612;
		return;
	}

	lv_svcSF.removeLeadAndTrailBlanks();
	lv_svcFEE.removeLeadAndTrailBlanks();

	/*
	 * confirmed by Cindy:
	 * we use Canadian SFC format in the US system.
	 * This will be different from what in US:MIR()
	 */

   /*
   mb_SfFound   = AB_true;
   mb_FeeFound  = AB_true;
   lv_svcFEE = AB_Text("FEE/MR2/FP:CCAX370000000000028/D1200");
   lv_svcSF  = AB_Text("123.45/CC");

   //lv_svcFEE = AB_Text("FEE/MR2/FP:CCAX370000000000028/D1200");
   //lv_svcFEE = AB_Text("FEE/MR2/FP:CA");
   //lv_svcFEE = AB_Text("FEE/MR2/FP:CK");
   //lv_svcFEE = AB_Text("FEE/MR2/CC");
   //lv_svcFEE = AB_Text("FEE/MR2/");
   //lv_svcFEE = AB_Text("FEE/MR2");
   //lv_svcFEE = AB_Text("FEE/");
   lv_svcFEE = AB_Text("FEE");

   lv_svcSF  = AB_Text("100.00/XG7.00/XQ0.00/CC");
   */

   {
		AB_Char		lc_amount(13);
		AB_Char		lc_tax1Code(2);
		AB_Char		lc_tax1Amount(10);
		AB_Char		lc_tax2Code(2);
		AB_Char		lc_tax2Amount(10);
		AB_Char		lc_tax3Code(2);
		AB_Char		lc_tax3Amount(10);
		AB_Char    	lc_fop(2);
		AB_Char    	lc_supId(3);
		AB_Char    	lc_cc(18);
		AB_Char    	lc_expDate(4);
		AB_Integer 	li_irregularId;

		getServiceFeeDetail(	po_a,
									lv_svcSF,
									lv_svcFEE,
									lc_amount,
                       		lc_tax1Code,
                       		lc_tax1Amount,
                       		lc_tax2Code,
                       		lc_tax2Amount,
                       		lc_tax3Code,
                       		lc_tax3Amount,
                       		lc_fop,
                       		lc_supId,
                       		lc_cc,
                       		lc_expDate,
                       		li_irregularId); 

       if ( li_irregularId == AB_Integer(0) )
       {
           md_IRMK_type[mi_IRMK_I] = "SFC" ;

           md_IRMK_remark[mi_IRMK_I] = AB_Text("AMD") + /* CRS             */
                                       lc_fop         + /* form of payment */
                                       lc_cc          +
                                       lc_expDate     +
                                       AB_Text("  ")  + /* PER ALL pax_id  */
                                       lc_amount      + /* amount          */
                                       lc_tax1Code    + /* tax1Code        */
                                       lc_tax1Amount  + /* tax1amount      */
                                       lc_tax2Code    + /* tax2Code        */
                                       lc_tax2Amount  + /* tax2amount      */
                                       lc_tax3Code    + /* tax3Code        */
                                       lc_tax3Amount  ; /* tax3amount      */

           md_IRMK_pax_id[mi_IRMK_I++] = AB_null ;
       }
       else
       {
           mi_rejectirId = li_irregularId;
       }
   }
}

void CAIR::create_SFN_Remark(AB_Boolean &pb_FR_found)
{
   md_IRMK_type[mi_IRMK_I]  	= "SFN" ;
   md_IRMK_remark[mi_IRMK_I] 	= "N" ;
 	md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;
	mi_IRMK_I++;

	pb_FR_found = AB_true;
}

void
CAIR::processSFM_SFO_SFA_SFN(CairRecord &po_a)
{
   int				li_pos, 
						li_pax_len,
						li_pax_dot,
						li_pax_id,
						li_total_pax_in_PNR,
						li_counter,
						li_addr_cnt = 1,
						li_slashPos;

   AB_Text 			lt_remark;
   AB_Text 			lt_data;
   AB_Text 			lt_pax_p1;
   AB_Text 			lt_pax_p2;
	V_AB_Varchar 	lv_arrayField(100,100) ;
   D_amount       ld_value;
	AB_Boolean		lb_amount_ok;
	AB_Char			lc_amount(13);
	AB_Char			lc_FAS(1);
	AB_Char			lc_FM_pax(5);
	AB_Char			lc_FM_exp_date(4);
	AB_Char			lc_FM_description(20);
	AB_Char			lc_FM_fop(2);
	AB_Char			lc_FM_cc_id(2);
	AB_Char			lc_FM_cc_no(30);
	AB_Boolean		lb_FR_found = AB_false;
	AB_Boolean		lb_FO_found = AB_false;
	AB_Boolean		lb_FA_found = AB_false;
	AB_Char			lc_lastChar(1);

	//lt_data  = retFromRM(po_a, "SF", 0);
	li_total_pax_in_PNR = po_a.mo_CPaxSegGrp.mo_CPaxSeg.size();
	LOG(logDEBUG) << "AM===== SF PAX_IND_MAX=[" << li_total_pax_in_PNR << "]"  ;

	//for (unsigned pax_ind = 0; pax_ind < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size(); pax_ind++)
	//{

   for (unsigned  i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM.size(); i++)
   {
      AB_Varchar  lv_remark(255);
      AB_Varchar  lv_udidNo(10);

      lt_remark = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK;

      if (lt_remark.For(4) == "*SF/") 
		{
         lt_data = lt_remark.At(5);
			LOG(logDEBUG) << "AM===== SF=[" << lt_data << "]"  ;
			lc_lastChar = lt_data.At(lt_data.length()).For(1);
			LOG(logDEBUG) << "AM===== LAST CHAR=[" << lc_lastChar << "]"  ;
			if ( strcmp(lc_lastChar.to_string(), "\015") == 0 ) 
			{
				lt_data = lt_data.For(lt_data.length() - 1);
				LOG(logDEBUG) << "AM===== SF=[" << lt_data << "]"  ;
			}


			lt_data.removeLeadAndTrailBlanks();

			li_counter 			= lv_arrayField.parseStringMultDel(lt_data,"/") ;

			for ( int li_index = 1; li_index <= li_counter; li_index++ )
			{
				if ( 	lv_arrayField[li_index].For(2) != "M-"	&&
						lv_arrayField[li_index].For(2) != "O-"	&&
						lv_arrayField[li_index].For(2) != "A-"	&&
						lv_arrayField[li_index].For(2) != "R-" )
				{
					create_SFN_Remark(lb_FR_found);
					break;
				}
				else if (lv_arrayField[li_index].For(2) == "M-" )
				{
					if ( li_counter != 4 && li_counter != 5 )
						break;

					// Pick up Amount (mandatory)

					if ( 	lv_arrayField[li_index].At(3).length() > 0 &&
							lv_arrayField[li_index].At(3).length()  <= 9 )
					{
						lc_amount = lv_arrayField[li_index].At(3);

						ld_value = ld_value.input_conversion_trigger(lc_amount);

   					if (! CMsgHandler::getMsgStatus())
						{
							lb_amount_ok = AB_false;
							CMsgHandler::clearMsg();
						}
						else
						{
							lb_amount_ok = AB_true;
						}

						if ( lb_amount_ok == AB_true 					&&
							  ld_value 		<= D_amount(999999.99) 	&&
							  ld_value 		>  D_amount(0.00) ) 
						{

							// Pick up Pax ID (mandatory)

							li_index++;

							if ( li_index < li_counter 					&&
								  lv_arrayField[li_index].length() > 0 &&
								  lv_arrayField[li_index].length() < 6 &&
								( lv_arrayField[li_index].findFirstCharNotInList(".0123456789") == 0  ||
								  lv_arrayField[li_index] == "PP" ))
							{
							   li_pax_len = lv_arrayField[li_index].length();

								lc_FM_pax = lv_arrayField[li_index];
								LOG(logDEBUG)  << "AM===== SFM >> lc_FM_pax = " << lc_FM_pax  ;
								li_pax_dot = lc_FM_pax.findSubstring(".");
								if (li_pax_dot > 0 ) 
								{
								   lt_pax_p1 = lc_FM_pax.For(li_pax_dot-1);
								   lt_pax_p2 = lc_FM_pax.At(li_pax_dot + 1).For(li_pax_len-li_pax_dot);
									LOG(logDEBUG)  << "AM===== SFM >> lt_pax_p1 = [" << lt_pax_p1 << "]"  ;
									LOG(logDEBUG)  << "AM===== SFM >> lt_pax_p2 = [" << lt_pax_p2 << "]" ;
									li_pax_id = AB_Integer(lt_pax_p2).to_long();
								   if ( lt_pax_p1 != "1" )
								   {
									   lc_FM_pax = "1." + lt_pax_p1;
										li_pax_id = AB_Integer(lt_pax_p1).to_long();
								   }
								}
								else if ( lv_arrayField[li_index] == "PP" )
								{
								   lc_FM_pax = "PP";
									li_pax_id = 1;
								}
								else
								{
								   lc_FM_pax = "1.1";
									li_pax_id = 1;
								}
								LOG(logDEBUG)  << "AM===== SFM >> lc_FM_pax = " << lc_FM_pax  ;

								// Pick up Description (mandatory)

								li_index++;

								if ( li_index <= li_counter &&
									  lv_arrayField[li_index].length() > 0 )
								{
									lc_FM_description = lv_arrayField[li_index].For(20);

									// Pick up the FOP information (mandatory)

									li_index++;

									lc_FM_cc_id = " ";
									lc_FM_cc_no = " ";
									lc_FM_exp_date = " ";

									if ( li_index <= li_counter )
									{
										if (( lv_arrayField[li_index].length() == 2 					&&
												lv_arrayField[li_index].For(2) 	== "CK"	)			||
											 ( lv_arrayField[li_index].length() == 2 					&&
												lv_arrayField[li_index].For(2) 	== "CA"	)			||
											 (	lv_arrayField[li_index].length() > 	2  				&&
												lv_arrayField[li_index].For(2) 	== "CC" 				&&
												lv_arrayField[li_index].At(3).For(2).isAllAlpha()	&&
												lv_arrayField[li_index].At(5).isAllNumeric() )	)
										{
											lc_FM_fop = lv_arrayField[li_index].For(2);

											if ( lc_FM_fop == "CC" )
											{
												lc_FM_cc_id = lv_arrayField[li_index].At(3).For(2);
												lc_FM_cc_no = lv_arrayField[li_index].At(5).For(30);
											}

											if ( li_index < li_counter && lc_FM_fop == "CC" )
											{
												// Pick up EXP Date (mandatory)

												li_index++;

												if ( li_index <= li_counter )
												{
													if ( 	lv_arrayField[li_index].For(3)			== "EXP" &&
											  				lv_arrayField[li_index].At(4).length()  > 0 		&&
											  				lv_arrayField[li_index].At(4).length() == 4 		&&
											  				lv_arrayField[li_index].At(4).isAllNumeric() )
													{
														lc_FM_exp_date = lv_arrayField[li_index].At(4);
														LOG(logDEBUG) << "AM===== SF lc_FM_exp_date 3=[" << lc_FM_exp_date << "]"  ;
													}
												}
											}

											// SCR 412640 Start
											bool lb_cc_valid = true;
											if (lc_FM_fop.For(2) == "CC")  {
												AB_Varchar lv_cc_id(40);
												long		  ll_error_code;
												lv_cc_id = lc_FM_cc_id + lc_FM_cc_no;
												if (!validCreditCard(lv_cc_id, lc_FM_exp_date, ll_error_code)) {
													lb_cc_valid = false;
													LOG(logDEBUG)  << "AM===== SFM CC is not valid!!!!!!!!!!! "  ;
												}
											}
											// SCR 412640 End
											LOG(logDEBUG)  << "AM===== SFM >> lc_FM_fop = " << lc_FM_fop  ;
											LOG(logDEBUG)  << "AM===== SFM >> lc_FM_exp_date = " << lc_FM_exp_date  ;

											if ( 	lc_FM_fop != "CC"	||
												(	lc_FM_fop == "CC"	&&
													lc_FM_exp_date != "    " ) )
											{
   											md_IRMK_type[mi_IRMK_I]  	= "SFM" ;
   		   								md_IRMK_remark[mi_IRMK_I] 	= 	(lb_cc_valid? "" : "BADCC/") +
																						lc_FM_pax			+
																						AB_Char(" ") 		+
																						AB_Char(13,lc_amount.to_string())+
																						lc_FM_description +
																						lc_FM_fop			+
																						lc_FM_cc_id			+
																						lc_FM_cc_no			+
																						lc_FM_exp_date;

												if ( li_pax_id > 0 && li_pax_id - 1 <= li_total_pax_in_PNR )
													md_IRMK_pax_id[mi_IRMK_I] 	= md_IP_pax_id[li_pax_id - 1]; //0]; //AB_null;
												else
													md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;

												mi_IRMK_I++;

												break;
											}
										}
									}
								}
							}
						}
					}

					break;
				}
				else if (lv_arrayField[li_index].For(2) == "R-" && 
							lb_FR_found == AB_false )
				{
					if ( 	lv_arrayField[li_index].At(3).length() == 1 &&
							lv_arrayField[li_index].At(3)	== "N" )
					{
						create_SFN_Remark(lb_FR_found);
						break;
					}
				}
				else if (lv_arrayField[li_index].For(2) == "O-" && 
							lb_FR_found == AB_false 					&&
							lb_FO_found == AB_false )
				{
					if ( 	lv_arrayField[li_index].At(3).length() > 0 )
					{
						if ( lv_arrayField[li_index].At(3).length()  <= 9 &&
							  lv_arrayField[li_index].At(3).For(1) 	!= "P" )
						{
							lc_amount = lv_arrayField[li_index].At(3);

							ld_value = ld_value.input_conversion_trigger(lc_amount);

   						if (! CMsgHandler::getMsgStatus())
							{
								lb_amount_ok = AB_false;
								CMsgHandler::clearMsg();
							}
							else
							{
								lb_amount_ok = AB_true;
							}

							LOG(logDEBUG)  << "SFO >> lc_amount = " << lc_amount  ;
							LOG(logDEBUG)  << "SFO >> ld_value = " << ld_value  ;

							if ( lb_amount_ok	== AB_true					&&
								  ld_value 		<= D_amount(999999.99) 	&&
								  ld_value		>= D_amount(0.00) )
							{
   							md_IRMK_type[mi_IRMK_I]  	= "SFO" ;
   		   				md_IRMK_remark[mi_IRMK_I] 	= AB_Char(" ") + AB_Char(13,lc_amount.to_string());
 								md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;
								mi_IRMK_I++;

								lb_FO_found = AB_true;
							}
						}
						else if ( lv_arrayField[li_index].At(4).length()	 > 0	&&
								    lv_arrayField[li_index].At(4).length() 	<= 5 	&&
									 lv_arrayField[li_index].At(3).For(1)     == "P" )
						{
							lc_amount = lv_arrayField[li_index].At(4);

							ld_value = ld_value.input_conversion_trigger(lc_amount);

   						if (! CMsgHandler::getMsgStatus())
							{
								lb_amount_ok = AB_false;
								CMsgHandler::clearMsg();
							}
							else
							{
								lb_amount_ok = AB_true;
							}

							LOG(logDEBUG)  << "SFO >> lc_amount = " << lc_amount  ;
							LOG(logDEBUG)  << "SFO >> ld_value = " << ld_value  ;

							if ( lb_amount_ok	== AB_true				&&
								  ld_value 		< 	D_amount(100.00) 	&&
								  ld_value		>=	D_amount(0.00) )
							{
   							md_IRMK_type[mi_IRMK_I]  	= "SFO" ;
   		   				md_IRMK_remark[mi_IRMK_I] 	= AB_Char("P") + AB_Char(13,lc_amount.to_string());
 								md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;
								mi_IRMK_I++;

								lb_FO_found = AB_true;
							}
						}
					}
					else
					{
						lb_FO_found = AB_true;
					}

					if ( lb_FO_found == AB_false )
					{
						create_SFN_Remark(lb_FR_found);
						break;
					}
				}
				else if (lv_arrayField[li_index].For(2) == "A-" && 
							lb_FR_found == AB_false 					&&
							lb_FA_found == AB_false )
				{
					if ( lv_arrayField[li_index].At(3).length()   > 0 )
					{
						if ( 	lv_arrayField[li_index].At(4).length()   > 0		&&
								lv_arrayField[li_index].At(4).length()  <= 6 	&&
								lv_arrayField[li_index].At(3).For(1)    == "P" )
						{
							li_pos = lv_arrayField[li_index].length();

							if ( lv_arrayField[li_index].At(li_pos) == "S" )
							{
								lc_FAS = "S";
								lc_amount = lv_arrayField[li_index].At(4).For(li_pos-4);
							}
							else
							{
								lc_FAS = " ";
								lc_amount = lv_arrayField[li_index].At(4);
							}

							ld_value = ld_value.input_conversion_trigger(lc_amount);

   						if (! CMsgHandler::getMsgStatus())
							{
								lb_amount_ok = AB_false;
								CMsgHandler::clearMsg();
							}
							else
							{
								lb_amount_ok = AB_true;
							}

							LOG(logDEBUG)  << "SFA >> lc_amount = " << lc_amount  ;
							LOG(logDEBUG)  << "SFA >> ld_value = " << ld_value  ;

							if ( lb_amount_ok	==	AB_true				&&
								  ld_value 		< 	D_amount(100.00) 	&&
								  ld_value		>=	D_amount(0.00) )
							{
								if ( ld_value > D_amount(0.00) )
								{
   								md_IRMK_type[mi_IRMK_I]  	= "SFA" ;
   		   					md_IRMK_remark[mi_IRMK_I] 	=  lc_FAS + AB_Char(13,lc_amount.to_string());
 									md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;
									mi_IRMK_I++;
								}

								lb_FA_found = AB_true;
							}
						}
					}
					else
					{
						lb_FA_found = AB_true;
					}

					if ( lb_FA_found == AB_false )
					{
						create_SFN_Remark(lb_FR_found);
						break;
					}
				}
			}
		}
	}
	//}

}

void
CAIR::getServiceFeeDetail(	CairRecord&			po_a,
									const AB_Varchar& pv_svcSF,
									const AB_Varchar& pv_svcFEE,
									AB_Char    & pc_amount,
                       		AB_Char    & pc_tax1Code,
                       		AB_Char    & pc_tax1Amount,
                       		AB_Char    & pc_tax2Code,
                       		AB_Char    & pc_tax2Amount,
                       		AB_Char    & pc_tax3Code,
                       		AB_Char    & pc_tax3Amount,
                       		AB_Char    & pc_formOfPay,
                       		AB_Char    & pc_supplier,
                       		AB_Char    & pc_creditCardId,
                       		AB_Char    & pc_expirationDate,
                       		AB_Integer & pi_status) 
{
	AB_Char        lc_delimiter;
	V_AB_Varchar   lv_arrayField(100,100) ;
	int            li_pos ;
	int     			li_index;
	int 				li_tax_count;

	D_char_numeric	ld_amount;
	D_amount			ld_value;
	
	AB_Varchar 		lv_fopData(100);
	AB_Varchar 		lv_ccData(30);
		

	lc_delimiter 	= "/";
	li_tax_count	= 0;

	pc_amount				= " ";
	pc_tax1Code				= " ";
	pc_tax1Amount			= " ";
	pc_tax2Code				= " ";
	pc_tax2Amount			= " ";
	pc_tax3Code				= " ";
	pc_tax3Amount			= " ";
	pc_formOfPay			= " ";
	pc_supplier				= " ";
	pc_creditCardId		= "               ";
	pc_expirationDate		= "    ";
	pi_status				= AB_Integer(0);

	AB_Varchar		lv_form_of_pay(2);
	AB_Varchar		lv_cc_id(2);
	AB_Varchar		lv_cc_no(20);
	AB_Varchar		lv_cc_exp_date(4);
	AB_Varchar		lv_auth_no(10);

	getFormOfPay(po_a,
					 lv_form_of_pay,
					 lv_cc_id,
					 lv_cc_no,
					 lv_cc_exp_date,
					 lv_auth_no);

	{
		li_pos = lv_arrayField.parseStringMultDel(pv_svcSF, lc_delimiter.to_string()) ;

		if ( li_pos > 0 )
		{
			for ( li_index=1; li_index <= li_pos; li_index++ )
				lv_arrayField[li_index].removeLeadAndTrailBlanks();

			if ( lv_arrayField[1].NotIsNull() )
			{
				ld_amount = lv_arrayField[1];

				if ( ! editAmountDomain(ld_amount, "#########.##", ld_value) )
				{
					pi_status = AB_Integer(3610);

					return;
				}
				else
				{
					pc_amount = ld_amount;
				}
			}
			else
			{
				pi_status = AB_Integer(3610);

				return;
			}

			for ( li_index=2; li_index <= li_pos && li_tax_count < 3; li_index++ )
			{
				if ( lv_arrayField[li_index].NotIsNull() && 
					  lv_arrayField[li_index].length() > 2 )
				{
					ld_amount = lv_arrayField[li_index].At(3).For(8);

					if ( ! editAmountDomain(ld_amount, "#######.##", ld_value) ||
						  ( lv_arrayField[li_index].For(2) != "RC"				&&
							 lv_arrayField[li_index].For(2) != "XG"				&&
							 lv_arrayField[li_index].For(2) != "XQ" ) )
					{
						pc_tax1Code				= " ";
						pc_tax1Amount			= " ";
						pc_tax2Code				= " ";
						pc_tax2Amount			= " ";
						pc_tax3Code				= " ";
						pc_tax3Amount			= " ";

						break;
					}

					li_tax_count++;	

					if ( li_tax_count == 1 )
					{
						pc_tax1Code		= lv_arrayField[li_index].For(2);
						pc_tax1Amount	= lv_arrayField[li_index].At(3).For(10);
					}
					else if ( li_tax_count == 2 )
					{
						pc_tax2Code    = lv_arrayField[li_index].For(2);
						pc_tax2Amount  = lv_arrayField[li_index].At(3).For(10);
					}
					else if ( li_tax_count == 3 )
					{
						pc_tax3Code    = lv_arrayField[li_index].For(2);
						pc_tax3Amount  = lv_arrayField[li_index].At(3).For(10);
					}
				}
			}

			// Pick up the FOP

			lv_fopData = pv_svcFEE;

			if ( 	 lv_fopData.length()	== 3 													||
				  ( lv_fopData.length()	== 4 && lv_fopData.At(4) 			== "/" ) ||
				  ( lv_fopData.length()	== 6 && lv_fopData.At(4).For(3) 	== "/CC" ) )
			{
				pc_supplier 	= lv_fopData.For(3);
				pc_formOfPay	= lv_form_of_pay;

				if ( pc_formOfPay == "CC" )
				{
					pc_creditCardId	= lv_cc_id + lv_cc_no.For(16);
					pc_expirationDate	= lv_cc_exp_date;
				}
			}
			else if ( 	lv_fopData.length() == 9 &&
						 (	lv_fopData.At(4).For(6) == "/FP:CK"	||
							lv_fopData.At(4).For(6) == "/FP:CA" ) )
			{
				pc_supplier    = lv_fopData.For(3);
				pc_formOfPay   = lv_fopData.At(8).For(2);
			}
			else if ( lv_fopData.At(4).For(6) == "/FP:CC" )
			{
				pc_supplier    	= lv_fopData.For(3);
				pc_formOfPay   	= "CC";

				lv_ccData = lv_fopData.At(10);
				lv_ccData.removeLeadAndTrailBlanks();

				li_index = lv_ccData.findSubstring("/D");

				if ( li_index > 0 )
				{
					pc_creditCardId	= lv_ccData.For(li_index-1);
					pc_expirationDate	= lv_ccData.At(li_index+2).For(4);
				}
				else
				{
					pc_creditCardId	= lv_ccData.For(18);
				}

			}
			else
			{
				pi_status = AB_Integer(3609);

				return;
			}
		}
		else
		{
			pi_status = AB_Integer(3610);

			return;
		}
	}
}

void
CAIR::processMCO(CairRecord &po_a) {
   for (unsigned i = 0; i < po_a.mo_CMCOGrp.mo_C4059MCO.size(); i++) {
      #define a4059MCO(att)\
      po_a.mo_CMCOGrp.mo_C4059MCO[i]->att


      if (a4059MCO(AIRCDE) == "XD" && !a4059MCO(PSGNBR).IsNull() && a4059MCO(PSGNBR) != "") {
         AB_Integer     li_psgcnt;
         V_AB_Integer   lvi_psglist(100);
         int            next_available_line_number;
			AB_Integer		paxNoInMCO_Int, paxNo_Int;
			int				paxNoInMCO, paxNo;

         retListNumbers(a4059MCO(PSGNBR), li_psgcnt, lvi_psglist);

         LOG(logDEBUG)  << "DEBUG TASF MCO LINEID: " << a4059MCO(ELMNBR);
         LOG(logDEBUG)  << "DEBUG TASF MCO PSGNBR: " << a4059MCO(PSGNBR);
			paxNoInMCO_Int = a4059MCO(PSGNBR).At(2);
			paxNoInMCO = paxNoInMCO_Int.to_long();
         LOG(logDEBUG)  << "DEBUG TASF MCO PSGNBR No: " << paxNoInMCO;
         LOG(logDEBUG)  << "DEBUG TASF MCO psgcnt:" << li_psgcnt.to_string()<< " " <<  lvi_psglist[1].to_string() << " " << mi_IP_I;


         /*
          * CAIR::populateIrperson()
          * Note that the index of the array md_IP_pax_id[] is the same
          * of the vector po_a.mo_CPaxSegGrp.mo_CPaxSeg[], see function
          * CAIR::populateIrperson()
          */
         int li_pax_idx = -1;
         if (li_psgcnt.to_long() > 0 && lvi_psglist[1].to_long() <= mi_IP_I) {
            li_pax_idx = lvi_psglist[1].to_long() - 1;
         }

         // SCR: TASF

         if (li_pax_idx < 0)  {
               LOG(logERROR)  << "WARNING: no valid pax_id, exiting later";
               LOG(logDEBUG)  << "WARNING: loop on pax_ids=" << po_a.mo_CPaxSegGrp.mo_CPaxSeg.size();

				for (int indPax = 0; indPax < po_a.mo_CPaxSegGrp.mo_CPaxSeg.size() && li_pax_idx < 0; indPax++)
				{
               LOG(logDEBUG)  << "pax_id string=" << po_a.mo_CPaxSegGrp.mo_CPaxSeg[indPax]->mo_C4088I_.PSGRNBR;
					paxNo_Int = po_a.mo_CPaxSegGrp.mo_CPaxSeg[indPax]->mo_C4088I_.PSGRNBR;
					paxNo = paxNo_Int.to_long();
               LOG(logDEBUG)  << "pax_id No=" << paxNo;
					if ( paxNo == paxNoInMCO )
				   {
						LOG(logERROR)  << "WARNING: found valid pax_id, no exiting...";
						li_pax_idx = indPax;
					}
				}

				if (li_pax_idx < 0)  {
               LOG(logERROR)  << "WARNING: still no valid pax_id, exiting NOW";
               return;
				}

         }

         int li_mfm_idx = -1;
            for (int indMFM = 0; indMFM < po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFMGrp.mo_C4128MFM.size(); indMFM++) {
               LOG(logDEBUG) << "DEBUG MFM line ID @ " << indMFM  << " is " << po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFMGrp.mo_C4128MFM[indMFM]->LINEID;
               int mco_line = AB_Integer(a4059MCO(ELMNBR)).to_long();
               LOG(logDEBUG) << "DEBUG mco_line= " << mco_line;
               int mfm_line = AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFMGrp.mo_C4128MFM[indMFM]->LINEID).to_long();
               LOG(logDEBUG) << "DEBUG mfm_line= " << mfm_line;
               if (mco_line == mfm_line)
                        li_mfm_idx = indMFM;
                  }
         if (li_mfm_idx < 0)  {
               LOG(logERROR)  << "WARNING: no valid mfm line exiting";
               return;
               }


         int li_mtc_idx = -1;
            for (int indTMC = 0; indTMC < po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC.size(); indTMC++) {
               LOG(logDEBUG) << "DEBUG TMC line ID @ " << indTMC  << " is " << po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC[indTMC]->LINEID;
               int mco_line = AB_Integer(a4059MCO(ELMNBR)).to_long();
               int mtc_line = AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC[indTMC]->LINEID).to_long();
               if (mco_line == mtc_line)
                        li_mtc_idx = indTMC;
                  }
         if (li_mtc_idx < 0)  {
               LOG(logERROR)  << "WARNING: no valid mtc line exiting";
               return;
               }


         int li_mfp_idx = -1;
            for (int indMFP = 0; indMFP < po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP.size(); indMFP++) {
               LOG(logDEBUG) << "DEBUG MFP line ID @ " << indMFP  << " is " << po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP[indMFP]->LINEID;
               int mco_line = AB_Integer(a4059MCO(ELMNBR)).to_long();
               int mfp_line = AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP[indMFP]->LINEID).to_long();
               if (mco_line == mfp_line)
                        li_mfp_idx = indMFP;
                  }
         if (li_mfp_idx < 0)  {
               LOG(logERROR)  << "WARNING: no valid mfp line exiting";
               return;
               }

         if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP[0]->PAYMNT.At(4).For(2) != "CC") {
               LOG(logERROR)  << "WARNING: no valid payment for mfp line exiting: " << po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP[0]->PAYMNT.At(4).For(2);
               return;
               }

         // We have found a valid associated pax
         /*
         if (li_pax_idx >= 0 &&
             po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC.size() > 0 &&
             po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP.size() > 0 &&
             po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP[0]->PAYMNT.At(4).For(2) == "CC")
         */
             {

            // create ticket
            md_ID_pax_id[mi_ID_I]      = md_IP_pax_id[li_pax_idx];
            md_ID_line_number[mi_ID_I] = mi_ID_I + 1;
            md_ID_reference[mi_ID_I]   = po_a.mo_C4010AMD.RECLOC;
            //md_ID_sup_id[mi_ID_I]    = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC[0]->NUMAIR;
            md_ID_sup_id[mi_ID_I]      = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC[li_mtc_idx]->NUMAIR;
            md_ID_base_cost[mi_ID_I]   = a4059MCO(BASE);
            md_ID_cur_id[mi_ID_I]      = a4059MCO(CURCODE);
            md_ID_form[mi_ID_I]        = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC[li_mtc_idx]->TKTNBR.For(4);
            md_ID_ticket_no[mi_ID_I]   = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CTMCGrp.mo_C4100TMC[li_mtc_idx]->TKTNBR.At(5);
            md_ID_pic[mi_ID_I]         = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_C4088I_.PSGRID;
            md_ID_no_of_books[mi_ID_I] = AB_Integer(1);
            md_ID_rebate[mi_ID_I]      = retFromRM(po_a,"RB",0);
            md_ID_doc_type[mi_ID_I]    = "BSP";
            md_ID_saving_code[mi_ID_I] = retFromRM(po_a,"FS",0);
            md_ID_full_fare[mi_ID_I]   = retFromRM(po_a,"FF",0);
            md_ID_low_prac_fare[mi_ID_I]  = retFromRM(po_a,"LP",0);

            //SCR: TASF
            md_ID_commission[mi_ID_I] =  po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFMGrp.mo_C4128MFM[li_mfm_idx]->FARECOM;

            if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CFTGrp.mo_C4120FT.size() > 0) {
               md_ID_crs_tour[mi_ID_I] = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CFTGrp.mo_C4120FT[0]->TOUR;
            }

            md_ID_e_ticket[mi_ID_I] = "Y";


            // create Irother
            AB_Date     ld_date;
            ld_date.getGMTNow();

            md_IO_sup_id[mi_IO_I] = "XD";
            md_IO_itin_type[mi_IO_I] = "O";
            md_IO_start_date[mi_IO_I] = ld_date.to_stringFormat("DDMMMYY");
            md_IO_orig_city_id[mi_IO_I] = "ZZZ";
            md_IO_dest_city_id[mi_IO_I] = "ZZZ";
            md_IO_unit_ct[mi_IO_I] = 1;
            md_IO_unit_type[mi_IO_I] = "TAF" ;


            //SCR 495569 md_IO_line_number[mi_IO_I] = mi_IO_I + 1;
            if (  mi_IO_I == 0 )
            {
               md_IO_line_number[mi_IO_I] =   1;
            }
            else
            {
               next_available_line_number = md_IO_line_number[mi_IO_I - 1].to_long();
               md_IO_line_number[mi_IO_I] = ++next_available_line_number;
            }

            md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;
            md_IO_reference[mi_IO_I] = po_a.mo_C4010AMD.RECLOC;


            // create Irremark
            md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[mi_IO_I];
            md_IRMK_oth_line_number[mi_IRMK_I]  = md_IO_line_number[mi_IO_I];
            md_IRMK_remark[mi_IRMK_I]           = "MANAGEMENT FEE";


            // create Irconsumdetail
            md_ICD_itin_type[mi_ICD_I]          = md_IO_itin_type[mi_IO_I];
            md_ICD_itin_line_number[mi_ICD_I]   = md_IO_line_number[mi_IO_I];
            md_ICD_pax_id[mi_ICD_I]             = md_IP_pax_id[li_pax_idx];
            md_ICD_line_number[mi_ICD_I]        = md_ID_line_number[mi_ID_I];


            // create Irpayment
            //md_IRP_amount[mi_IRP_I]       = "123.45";  // dummy amount
            md_IRP_amount[mi_IRP_I]       =  md_ID_commission[mi_ID_I];
            md_IRP_pax_id[mi_IRP_I]       = md_IP_pax_id[li_pax_idx];
            md_IRP_line_number[mi_IRP_I]  = md_ID_line_number[mi_ID_I];
            md_IRP_form_of_pay[mi_IRP_I]  = "CC";
            md_IRP_pay_id[mi_IRP_I]       = AB_Integer(1);

            AB_Varchar lv_cc_data(248);
            int        li_pos;
            //lv_cc_data = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP[0]->PAYMNT.At(6);
            lv_cc_data = po_a.mo_CPaxSegGrp.mo_CPaxSeg[li_pax_idx]->mo_CMFPGrp.mo_C4131MFP[li_mfp_idx]->PAYMNT.At(6);

            md_IRP_cc_id[mi_IRP_I]        = lv_cc_data.For(2);

            if ((li_pos = lv_cc_data.findSubstring("/")) != 0) {
               md_IRP_cc_no[mi_IRP_I]        = lv_cc_data.At(3).For(li_pos - 3);
               md_IRP_cc_exp_date[mi_IRP_I]  = lv_cc_data.At(li_pos + 1).For(4);
            } else {
               md_IRP_cc_no[mi_IRP_I]        = lv_cc_data.At(3);
            }


            // create taxes
            int li_idx;
            for (int j = 1; j <= 30; j++) {
               if (a4059MCO(TCURR)[j].NotIsNull() &&
                   a4059MCO(TAXA)[j].NotIsNull() &&
                   a4059MCO(TAXA)[j].findFirstCharNotInList("0123456789.") == 0) {
                  D_amount ld_taxAmount = a4059MCO(TAXA)[j];
                  D_tax    ld_tax_id = a4059MCO(TAXC)[j];

                  if ((ld_tax_id == "CA" ||
                       ld_tax_id == "RC" ||
                       ld_tax_id == "XG" ||
                       ld_tax_id == "XQ") &&
                      (li_idx = findTax(ld_tax_id,
                                        md_IP_pax_id[li_pax_idx],
                                        md_ID_line_number[mi_ID_I])) >= 0) {
                     md_ITD_tax_amount[li_idx] = D_amount(md_ITD_tax_amount[li_idx]) + ld_taxAmount;
                  } else {
                     md_ITD_pax_id[mi_ITD_I]       = md_IP_pax_id[li_pax_idx];
                     md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
                     md_ITD_tax_id[mi_ITD_I]       = ld_tax_id;
                     md_ITD_tax_amount[mi_ITD_I]   = ld_taxAmount;
                     mi_ITD_I++;
                  }
               }
            }

            mi_ID_I++;
            mi_IO_I++;
            mi_IRMK_I++;
            mi_ICD_I++;
            mi_IRP_I++;
         }
      }
   }
}

void
CAIR::processEMD(CairRecord &po_a)
{
	LOG(logDEBUG)  << "processEMD starts....... has #EMDs:" << po_a.mo_CEMDGrp.mo_CEMD.size();

	for (unsigned i = 0; i < po_a.mo_CEMDGrp.mo_CEMD.size(); i++) {
		#define EMD(att)\
		po_a.mo_CEMDGrp.mo_CEMD[i]->att

		LOG(logDEBUG)  << "current EMDs" << EMD(TSDID_TSDNBR).At(2);

		int li_mtc_idx = -1;
		for (int i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC.size(); i++) {
					LOG(logDEBUG) << "DEBUG TMC line ID @ " << i  << " is " << po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC[i]->LINEID;
					int mco_line = AB_Integer(EMD(TSDID_TSDNBR).At(2)).to_long();
					int mtc_line = AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC[i]->LINEID).to_long();
					if (mco_line == mtc_line)
								li_mtc_idx = i;
		}
		if (li_mtc_idx < 0)  {
					LOG(logDEBUG)  << "DEBUG: no valid mtc line, exiting";
					return;
		}


		int li_mfp_idx = -1;
		for (int i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CMFPGrp.mo_C4131MFP.size(); i++) {
					LOG(logDEBUG) << "DEBUG MFP line ID @ " << i  << " is " << po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CMFPGrp.mo_C4131MFP[i]->EMD_LINEID;
					int mco_line = AB_Integer(EMD(TSDID_TSDNBR).At(2)).to_long();
					int mfp_line = AB_Integer(po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CMFPGrp.mo_C4131MFP[i]->EMD_LINEID).to_long();
					if (mco_line == mfp_line)
								li_mfp_idx = i;
		}
		if (li_mfp_idx < 0)  {
					LOG(logDEBUG)  << "DEBUG: no valid mfp line exiting";
					return;
		}

		AB_Varchar lv_base_cost(20);
		AB_Varchar lv_cur_id(3);

		if ( EMD(CURCODE_EQVAMNT).length() > 0 )
		{
			lv_cur_id = EMD(CURCODE_EQVAMNT).For(3);
			lv_base_cost = EMD(CURCODE_EQVAMNT).At(4);
		}
		else
		{
			lv_cur_id = EMD(CURCODE_BASE).For(3);
			lv_base_cost = EMD(CURCODE_BASE).At(4);
		}

		lv_base_cost.removeLeadAndTrailBlanks();

		// create ticket
		md_ID_pax_id[mi_ID_I] 		= md_IP_pax_id[0];
		md_ID_line_number[mi_ID_I] = mi_ID_I + 1;
		md_ID_reference[mi_ID_I]	= po_a.mo_C4010AMD.RECLOC;
		//md_ID_sup_id[mi_ID_I]		= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC[0]->NUMAIR;
		md_ID_sup_id[mi_ID_I]		= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC[li_mtc_idx]->NUMAIR;
		md_ID_base_cost[mi_ID_I]	= lv_base_cost; //EMD(BASE);
		md_ID_cur_id[mi_ID_I]		= lv_cur_id; //EMD(CURCODE);
		md_ID_form[mi_ID_I]			= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC[li_mtc_idx]->TKTNBR.For(4);
		md_ID_ticket_no[mi_ID_I]	= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC[li_mtc_idx]->TKTNBR.At(5);
		md_ID_pic[mi_ID_I]			= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_C4088I_.PSGRID;
		md_ID_no_of_books[mi_ID_I]	= AB_Integer(1);
		//md_ID_rebate[mi_ID_I]		= retFromRM(po_a,"RB",0);
		md_ID_doc_type[mi_ID_I]		= "BSP";
		//md_ID_saving_code[mi_ID_I]	= retFromRM(po_a,"FS",0);
		//md_ID_full_fare[mi_ID_I]	= retFromRM(po_a,"FF",0);
		//md_ID_low_prac_fare[mi_ID_I]	= retFromRM(po_a,"LP",0);

		//SCR: TASF
		//md_ID_commission[mi_ID_I] =  po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CMFMGrp.mo_C4128MFM[li_mfm_idx]->FARECOM;

		//md_ID_crs_tour[mi_ID_I]	= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFTGrp.mo_C4120FT[0]->TOUR;

		//md_ID_e_ticket[mi_ID_I]	= "Y";


		// create Irother
		AB_Date     ld_date;
		ld_date.getGMTNow();

		md_IO_sup_id[mi_IO_I] = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CTMCGrp.mo_C4100TMC[li_mtc_idx]->NUMAIR; //"XB";
		md_IO_itin_type[mi_IO_I] = "O";
		md_IO_start_date[mi_IO_I] = ld_date.to_stringFormat("DDMMMYY");
		md_IO_orig_city_id[mi_IO_I] = "ZZZ";
		md_IO_dest_city_id[mi_IO_I] = "ZZZ";
		md_IO_unit_ct[mi_IO_I] = 1;
		md_IO_unit_type[mi_IO_I] = "EMD" ;

		//SCR 495569 md_IO_line_number[mi_IO_I] = mi_IO_I + 1;
		if (  mi_IO_I == 0 )
		{
			md_IO_line_number[mi_IO_I] =   1;
		}
		else
		{
			int next_available_line_number = md_IO_line_number[mi_IO_I - 1].to_long();
			md_IO_line_number[mi_IO_I] = ++next_available_line_number;
		}

		md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;
		//md_IO_reference[mi_IO_I] = po_a.mo_C4010AMD.RECLOC;


		// create Irconsumdetail
		md_ICD_itin_type[mi_ICD_I]          = md_IO_itin_type[mi_IO_I];
		md_ICD_itin_line_number[mi_ICD_I]	= md_IO_line_number[mi_IO_I];
		md_ICD_pax_id[mi_ICD_I]             = md_IP_pax_id[0];
		md_ICD_line_number[mi_ICD_I]      	= md_ID_line_number[mi_ID_I];


		// create Irpayment
		md_IRP_amount[mi_IRP_I]       =  md_ID_base_cost[mi_ID_I]; //md_ID_commission[mi_ID_I];
		md_IRP_pax_id[mi_IRP_I]       = md_IP_pax_id[0];
		md_IRP_line_number[mi_IRP_I]  = md_ID_line_number[mi_ID_I];
		// set it later md_IRP_form_of_pay[mi_IRP_I]  = "CC";
		md_IRP_pay_id[mi_IRP_I]       = AB_Integer(1);


		AB_Varchar lv_fop_data(5);
		AB_Varchar lv_cc_data(248);
		AB_Varchar lv_fop(20);
		AB_Varchar lv_cc_id(20);
		AB_Varchar lv_cc_no(20);
		AB_Varchar lv_cc_exp_date(20);
		int 		  li_pos;

		lv_fop_data = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CMFPGrp.mo_C4131MFP[0]->PAYMNT.At(4).For(5);
		LOG(logDEBUG) << "FOP in MFP segment"  << lv_fop_data;

		if (lv_fop_data.For(2) == "CC") {
			lv_fop = "CC";
			lv_cc_data 	= po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CMFPGrp.mo_C4131MFP[li_mfp_idx]->PAYMNT.At(6);
			lv_cc_id 	= lv_cc_data.For(2);
			if ((li_pos = lv_cc_data.findSubstring("/")) != 0) {
				md_IRP_cc_no[mi_IRP_I]        = lv_cc_data.At(3).For(li_pos - 3);
				md_IRP_cc_exp_date[mi_IRP_I]  = lv_cc_data.At(li_pos + 1).For(4);
			} else {
				md_IRP_cc_no[mi_IRP_I]        = lv_cc_data.At(3);
			}
			md_IRP_cc_id[mi_IRP_I] 			= lv_cc_id;
		}
		else if (lv_fop_data.For(5) == "CHECK") {
			lv_fop = "CK";
		}
		else { // CASH and other fops default to CASH
			lv_fop = "CA";
		}

		md_IRP_form_of_pay[mi_IRP_I]  = lv_fop; //"CC";


		// create taxes
		int li_idx;
		for (int j = 1; j <= 30; j++) {
			if (EMD(TCURR)[j].NotIsNull() &&
				 EMD(TAXA)[j].NotIsNull() &&
				 EMD(TAXA)[j].findFirstCharNotInList("0123456789.") == 0) {
				D_amount ld_taxAmount = EMD(TAXA)[j];
				D_tax		ld_tax_id = EMD(TAXC)[j];

				if ((ld_tax_id == "CA" ||
					  ld_tax_id == "RC" ||
					  ld_tax_id == "XG" ||
					  ld_tax_id == "XQ")	&&
					 (li_idx = findTax(ld_tax_id,
					 						 md_IP_pax_id[0],
											 md_ID_line_number[mi_ID_I])) >= 0) {
					md_ITD_tax_amount[li_idx] = D_amount(md_ITD_tax_amount[li_idx]) + ld_taxAmount;
				} else {
					md_ITD_pax_id[mi_ITD_I]       = md_IP_pax_id[0];
					md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
					md_ITD_tax_id[mi_ITD_I]       = ld_tax_id;
					md_ITD_tax_amount[mi_ITD_I] 	= ld_taxAmount;
					mi_ITD_I++;
				}
			}
		}

		mi_ID_I++; // irticket
		mi_IO_I++; //orother
		mi_ICD_I++; //irconsumerdetail
		mi_IRP_I++;  //irpayment
	}
}

void
CAIR::createIRRemark(const AB_Varchar &pv_rmk, const AB_Varchar &pv_itin_type)
{
	md_IRMK_remark[mi_IRMK_I]           = pv_rmk;
	md_IRMK_itin_type[mi_IRMK_I]        = pv_itin_type;
	md_IRMK_ren_line_number[mi_IRMK_I]  = mi_IR_I + 1;

	mi_IRMK_I++;
}

void
CAIR::createITRemark(const AB_Varchar &pv_sup_id)
{
	AB_Varchar	lv_tmp(80);

	lv_tmp										= retSupName(pv_sup_id);

	md_IRMK_remark[mi_IRMK_I]				= "OPERATED BY " + lv_tmp;
	md_IRMK_itin_type[mi_IRMK_I]			= "A";
	md_IRMK_tra_line_number[mi_IRMK_I]	= mi_IT_I + 1;

	mi_IRMK_I++;
}

void
CAIR::createIORemark(const AB_Varchar &pv_rmk, const AB_Varchar &pv_itin_type)
{
	md_IRMK_remark[mi_IRMK_I]           = pv_rmk;
	md_IRMK_itin_type[mi_IRMK_I]        = pv_itin_type;
	md_IRMK_oth_line_number[mi_IRMK_I]  = mi_IO_I + 1;

	mi_IRMK_I++;
}

void
CAIR::createExchageCostRemark(const AB_Varchar &pv_remark)
{
	md_IRMK_type[mi_IRMK_I]			= "COS" ;
	md_IRMK_remark[mi_IRMK_I]     = pv_remark;
	md_IRMK_pax_id[mi_IRMK_I]     = md_ID_pax_id[mi_ID_I];

	mi_IRMK_I++;
}

void
CAIR::retCommission(const int &pax_id, CairRecord &po_a, AB_Varchar &comm_rate, AB_Varchar &commission)
{
	if(po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CFMGrp.mo_C4114FM.size() > 0)
	{
		AB_Varchar 	lv_tmp(127);	
		/*
		int			i;
		
		lv_tmp 	= po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CFMGrp.mo_C4114FM[0]->FARECOM;

		int            li_count;
		V_AB_Varchar   lva(80,80);
		li_count = lva.parseStringMultDel(lv_tmp, "/");
		lv_tmp = lva[1];

		i = lv_tmp.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ*");

		if (i > 0)
		{
			if (lv_tmp.At(lv_tmp.length()).For(1) == "A")
			{
				commission = lv_tmp.At(i);
				commission = commission.For(commission.length() - 1);
			}
			else
			{
				comm_rate  = lv_tmp.At(i);	
			}
		}
		*/

		int			firstCharPos;
		
		lv_tmp 	= po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CFMGrp.mo_C4114FM[0]->FARECOM;

		int            li_count;
		V_AB_Varchar   lva(80,80);
		li_count = lva.parseStringMultDel(lv_tmp, "/");
		lv_tmp = lva[1];

		firstCharPos = lv_tmp.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ*");

		//LOG(logDEBUG) << "MELA lv= " << lv_tmp  ;
		//LOG(logDEBUG) << "MELA positionOfFirstCharNotInList= " << firstCharPos  ;
		//LOG(logDEBUG) << "MELA lv_tmpLenght= " << lv_tmp.length()  ;

		if (firstCharPos > 0)
			lv_tmp = lv_tmp.At(firstCharPos);

		firstCharPos = lv_tmp.findFirstCharNotInSet("N.");

		//LOG(logDEBUG) << "MELA lv= " << lv_tmp  ;
		//LOG(logDEBUG) << "MELA findFirstCharNotInSet= " << firstCharPos  ;
		//LOG(logDEBUG) << "MELA lv_tmpLenght= " << lv_tmp.length()  ;

		if (firstCharPos > 0)
			lv_tmp = lv_tmp.For(firstCharPos);
		else 
			if (lv_tmp.length() > 0 )
				comm_rate  = lv_tmp;

		//LOG(logDEBUG) << "MELA lv= " << lv_tmp  ;

		if (firstCharPos > 0)
		{
			//if (lv_tmp.At(lv_tmp.length()).For(1) == "A")

			if (lv_tmp.At(firstCharPos).For(1) == "A")
			{
				//commission = lv_tmp.At(i);

				commission = lv_tmp.For(firstCharPos - 1);

				//commission = commission.For(commission.length() - 1);
			}
			else
			{
				comm_rate  = lv_tmp.For(firstCharPos - 1);
			}
		}


	}
}

void
CAIR::retSegAttributes(const int &j, AB_Varchar &pc_itin_type,
							  D_line_number &pd_tra_line_number,
							  D_line_number &pd_ren_line_number,
							  D_line_number &pd_oth_line_number)
{

	int				type;
	AB_Boolean		lb_found;

	lb_found = AB_false;

	for (unsigned i = 0; i < 500 ; i++)
	{

		if (mi_IT_SEGNBR[i] == j)
		{

			pc_itin_type		 = md_IT_itin_type[i];
			pd_tra_line_number = md_IT_line_number[i];

			lb_found = AB_true;
		}

		if (lb_found) 
		{
			return;
		}

		if (mi_IT_SEGNBR[i] == 0)
		{
			break;
		}

	}

	for (unsigned i = 0; i < 500 ; i++)
	{

		if (mi_IR_SEGNBR[i] == j)
		{

			pc_itin_type		 = md_IR_itin_type[i];
			pd_ren_line_number = md_IR_line_number[i];

			lb_found = AB_true;
		}

		if (lb_found) 
		{
			return;
		}

		if (mi_IR_SEGNBR[i] == 0)
		{
			break;
		}
	}

	for (unsigned i = 0; i < 500 ; i++)
	{

		if (mi_IO_SEGNBR[i] == j)
		{

			pc_itin_type		 = md_IO_itin_type[i];
			// SCR: 516455, migrating from _ca v1.66
			//pd_tra_line_number = md_IO_line_number[i];
		   pd_oth_line_number = md_IO_line_number[i];

			lb_found = AB_true;
		}

		if (lb_found) 
		{
			return;
		}

		if (mi_IO_SEGNBR[i] == 0)
		{
			break;
		}
	}
}

void
CAIR::retListNumbers(const AB_Varchar &pd_delete_list,
									AB_Integer   &pi_count,
									V_AB_Integer &pvi_list)
{
	AB_Char              lc_pc_activeOnly(1);
	AB_Varchar           lv_whatToDelete(3);
	AB_Varchar           lv_deleteList(255);
	AB_Varchar           lv_deleteList1(255);

	V_AB_Varchar			lva_tmp(3,256);
	AB_Integer				li_tmp;

   // <<< as in SCR 516167
   AB_Varchar pd_delete_listNew(1024);
   //LOG(logDEBUG)  << "DEBUG retListNumbers AAA " << pd_delete_list ;
   //LOG(logDEBUG)  << "DEBUG length " << pd_delete_list.length() ;
   if (pd_delete_list.At(pd_delete_list.length()).For(1) == "\015") {
      pd_delete_listNew = pd_delete_list.For(pd_delete_list.length()-1);
      //LOG(logDEBUG)  << "DEBUG retListNumbers restore AAA " << pd_delete_listNew ;
      }
   else
      pd_delete_listNew = pd_delete_list;

   //CDeleteList          lo_CDeleteList( pd_delete_list.At(2),
   CDeleteList          lo_CDeleteList( pd_delete_listNew.At(2),
                                        "A");
   // >>> as in SCR 516167

	if ( ! CMsgHandler::getMsgStatus())
		return ;

	if ( ! lo_CDeleteList.getExpandedList( lv_deleteList,
														lv_deleteList1 ))
		return ;

	li_tmp 	= lva_tmp.parseStringMultDel(lv_deleteList,",");

	pi_count = AB_Integer(0);

	while ( pi_count < li_tmp - AB_Integer(3) )
	{
		pi_count = pi_count + AB_Integer(1);

		pvi_list[pi_count] = lva_tmp[pi_count + AB_Integer(3)];
	}
}

void
CAIR::retCarRateData(const AB_Varchar&		pv_rbgq_opt,
							D_currency&				pd_cur_id,
							D_char_numeric&		pd_local_rate,
							D_char_numeric&		pd_booked_rate,
							D_char_numeric& 		pd_regular_rate,
							D_free_mileage& 		pd_free_mileage,
							D_rate_duration& 		pd_rate_duration,
							D_free_mileage_code& pd_free_mile_code,
							D_char_numeric& 		pd_ex_day_charge,
							D_char_numeric& 		pd_ex_hr_charge,
							D_char_numeric& 		pd_ex_mile_charge) const {
	/*
	 * pv_rbgq_opt looks like:
	 * RG-USD 139.99- UNL WY 25.99- UNL XD 14.99- UNL XH
	 * RG-USD 124.12- UNL WY 28.99- UNL XD 10.00- UNL XH
	 * RG-USD 200.00- UNL WY 40.00- UNL XD 20.00- UNL XH
	 * RG-USD 14.99- UNL DY 5.00- UNL XH
	 * RG-EUR 119.00- UNL WY 17.00- UNL XD
	 * RG-GBP 31.00- UNL DY
	 */
	LOG(logDEBUG)  << "RG >>> pv_rbgq_opt = " << pv_rbgq_opt  ;
	AB_Varchar 		lv_str(59);
	int 				li_pos, idx;
	AB_Varchar 		lv_rawAmount(15);
	AB_BOOL 			lb_ok;
	V_AB_Varchar	lv_arrayField(100, 100);

	
	lv_str = pv_rbgq_opt;
	int li_count = lv_arrayField.parseStringMultDel(lv_str, " ");
	
	idx = 1;
	
	//Code for parsing RG, RQ- - US662 STARTS
	string cur_id, booked_rate, local_rate, rate_duration, free_mileage, free_mile_code;
	if(lv_str.NotIsNull())
	{
		// Regex to match USD31.00DY-UNL MI in RG and ignore anything after MI
		// Add ^ at front and $ at the end to force it to compare whole string
		//CAD52.88DY-0.38 KM 0FK
		//USD32.95DY-UNL MI
		//USD31.00DY-UNL MI 10.23XH-UNL MI
		//USD155.00WY-UNL MI 31.00XD-UNL MI
		if ( PCRE::pcre_match("((?:[A-Z]+))([0-9]*\\.[0-9]+)((?:[A-Z]+))([-])((?:[0-9A-Z.]+))([[:space:]])((?:[A-Z]+))", lv_str.to_string())) {			
			PCRE::pcre_get_group(1, cur_id); //USD
			PCRE::pcre_get_group(2, booked_rate); //31.00
			PCRE::pcre_get_group(2, local_rate); //31.00
			PCRE::pcre_get_group(3, rate_duration); //DY
			PCRE::pcre_get_group(5, free_mileage); //UNL 
			PCRE::pcre_get_group(7, free_mile_code); //MI
			
			string a, b, c, d, e, f, g, h, i, j;
			PCRE::pcre_get_group(1, a);
			PCRE::pcre_get_group(2, b);
			PCRE::pcre_get_group(3, c);
			PCRE::pcre_get_group(4, d);
			PCRE::pcre_get_group(5, e);
			PCRE::pcre_get_group(6, f);
			PCRE::pcre_get_group(7, g);

			LOG(logDEBUG) << "-----lv_str.to_string()[" << lv_str.to_string() << "]---" ;
			LOG(logDEBUG) << "-----a[" << a.data() << "]---" ;
			LOG(logDEBUG) << "-----b[" << b.data() << "]---" ;
			LOG(logDEBUG) << "-----c[" << c.data() << "]---" ;
			LOG(logDEBUG) << "-----d[" << d.data() << "]---" ;
			LOG(logDEBUG) << "-----e[" << e.data() << "]---" ;
			LOG(logDEBUG) << "-----f[" << f.data() << "]---" ;
			LOG(logDEBUG) << "-----g[" << g.data() << "]---" ;

			
			
			/*
			Text=RG-USD31.00DY-UNL MI 34.00XD-UNL MI
			Full match	0-17	`USD31.00DY-UNL MI`
			Group 1.	0-3	`USD`
			Group 2.	3-8	`31.00`
			Group 3.	8-10	`DY`
			Group 4.	10-11	`-`
			Group 5.	11-14	`UNL`
			Group 6.	14-15	` `
			Group 7.	15-17	`MI`
			Anything after MI will be ignored
			*/
			
			pd_cur_id = cur_id.data() ;
			pd_booked_rate = booked_rate.data() ;
			pd_local_rate = local_rate.data() ;
			pd_free_mileage = free_mileage.data() ;
			LOG(logDEBUG)  << "Inside 1st RG-Condition";
		} else
	
		// Regex to match *ZE*USD33.00- .00 UNL DY 25.00- UNL XH in RQ and ignore anything after DY
		// Add ^ at front and $ at the end to force it to compare whole string
		if ( PCRE::pcre_match("(.{4}\\K)((?:[A-Z]+))([0-9]*\\.[0-9]+)(.{2}\\K)([0-9]*\\.[0-9]+)([[:space:]])((?:[0-9A-Z]+))([[:space:]])((?:[A-Z]+))", lv_str.to_string())) {
			
			string a, b, c, d, e, f, g, h, i, j;
			PCRE::pcre_get_group(1, a);
			PCRE::pcre_get_group(2, b);
			PCRE::pcre_get_group(3, c);
			PCRE::pcre_get_group(4, d);
			PCRE::pcre_get_group(5, e);
			PCRE::pcre_get_group(6, f);
			PCRE::pcre_get_group(7, g);
			PCRE::pcre_get_group(8, h);
			PCRE::pcre_get_group(9, i);
			//PCRE::pcre_get_group(10, j);
			
			LOG(logDEBUG) << "-----lv_str.to_string()[" << lv_str.to_string() << "]---" ;
			LOG(logDEBUG) << "-----a[" << a.data() << "]---" ;
			LOG(logDEBUG) << "-----b[" << b.data() << "]---" ;
			LOG(logDEBUG) << "-----c[" << c.data() << "]---" ;
			LOG(logDEBUG) << "-----d[" << d.data() << "]---" ;
			LOG(logDEBUG) << "-----e[" << e.data() << "]---" ;
			LOG(logDEBUG) << "-----f[" << f.data() << "]---" ;
			LOG(logDEBUG) << "-----g[" << g.data() << "]---" ;
			LOG(logDEBUG) << "-----h[" << h.data() << "]---" ;
			LOG(logDEBUG) << "-----i[" << i.data() << "]---" ;
			//LOG(logDEBUG) << "-----j[" << j.data() << "]---" ;
			
			PCRE::pcre_get_group(2, cur_id); //USD
			PCRE::pcre_get_group(3, booked_rate); //33.00
			PCRE::pcre_get_group(3, local_rate); //33.00
			PCRE::pcre_get_group(9, rate_duration); //DY
			PCRE::pcre_get_group(7, free_mileage); //UNL 
			free_mile_code = "M" ; //Defaulting to Miles as there is no data
			
			//If it free_mileage has other value than UNL, like 180K, then consider K as Kilometer and 180 in free_milegae
			if ( PCRE::pcre_match("((?:[0-9]+))((?:[A-Z]+))", free_mileage)) {
				PCRE::pcre_get_group(1, free_mileage);
				PCRE::pcre_get_group(2, free_mile_code);
			}
			
			/*
			Text = *ZE*USD33.00- .00 UNL DY 25.00- UNL XH
			Group 1.	0-4	`*ZE*`
			Group 2.	4-7	`USD`
			Group 3.	7-12	`33.00`
			Group 4.	12-14	`- `
			Group 5.	14-17	`.00`
			Group 6.	17-18	` `
			Group 7.	18-21	`UNL`
			Group 8.	21-22	` `
			Group 9.	22-24	`DY`
		
			Anything after DY will be ignored
			*/
			pd_cur_id = cur_id.data() ;
			pd_booked_rate = booked_rate.data() ;
			pd_local_rate = local_rate.data() ;
			pd_free_mileage = free_mileage.data() ;
			
			LOG(logDEBUG)  << "Inside 2nd RQ-Condition";
		} else
		
		// Regex to match *ZE*USD33.00- .00MI UNL DY 25.00- UNL XH in RQ and ignore anything after DY
		// Add ^ at front and $ at the end to force it to compare whole string
		if ( PCRE::pcre_match("(.{4}\\K)((?:[A-Z]+))([0-9]*\\.[0-9]+)(.{5}\\K)((?:[A-Z]+))([[:space:]])((?:[0-9A-Z]+))([[:space:]])((?:[A-Z]+))", lv_str.to_string())) {
			string a, b, c, d, e, f, g, h, i, j, k;
			PCRE::pcre_get_group(1, a);
			PCRE::pcre_get_group(2, b);
			PCRE::pcre_get_group(3, c);
			PCRE::pcre_get_group(4, d);
			PCRE::pcre_get_group(5, e);
			PCRE::pcre_get_group(6, f);
			PCRE::pcre_get_group(7, g);
			PCRE::pcre_get_group(8, h);
			PCRE::pcre_get_group(9, i);
			PCRE::pcre_get_group(10, j);
			//PCRE::pcre_get_group(11, k);
			
			LOG(logDEBUG) << "-----lv_str.to_string()[" << lv_str.to_string() << "]---" ;
			LOG(logDEBUG) << "-----a[" << a.data() << "]---" ;
			LOG(logDEBUG) << "-----b[" << b.data() << "]---" ;
			LOG(logDEBUG) << "-----c[" << c.data() << "]---" ;
			LOG(logDEBUG) << "-----d[" << d.data() << "]---" ;
			LOG(logDEBUG) << "-----e[" << e.data() << "]---" ;
			LOG(logDEBUG) << "-----f[" << f.data() << "]---" ;
			LOG(logDEBUG) << "-----g[" << g.data() << "]---" ;
			LOG(logDEBUG) << "-----h[" << h.data() << "]---" ;
			LOG(logDEBUG) << "-----i[" << i.data() << "]---" ;
			LOG(logDEBUG) << "-----j[" << j.data() << "]---" ;
			//LOG(logDEBUG) << "-----k[" << k.data() << "]---" ;
			
			
			PCRE::pcre_get_group(2, cur_id); //USD
			PCRE::pcre_get_group(3, booked_rate); //33.00
			PCRE::pcre_get_group(3, local_rate); //33.00
			PCRE::pcre_get_group(9, rate_duration); //DY
			PCRE::pcre_get_group(7, free_mileage); //UNL 
			PCRE::pcre_get_group(5, free_mile_code); //MI
			pd_cur_id = cur_id.data() ;
			pd_booked_rate = booked_rate.data() ;
			pd_local_rate = local_rate.data() ;
			pd_free_mileage = free_mileage.data() ;
			
			
			
			
			
			/*
			Text = *ZE*USD33.00- .00MI UNL DY 25.00- UNL XH
			Group 1.	0-4	`*ZE*`
			Group 2.	4-7	`USD`
			Group 3.	7-12	`32.00`
			Group 4.	12-17	`- .00`
			Group 5.	17-19	`MI`
			Group 6.	19-20	` `
			Group 7.	20-23	`UNL`
			Group 8.	23-24	` `
			Group 9.	24-26	`DY`


			Anything after DY will be ignored
			*/
			
			LOG(logDEBUG)  << "Inside 3rd RQ-Condition";
		}
		
	}
	
	
	
	//if (rate_duration.NotIsNull()) {
	if (rate_duration == "WK" || rate_duration == "WY") {
		pd_rate_duration = "W" ;
	} else

	if (rate_duration == "WE" || rate_duration == "WD") {
		pd_rate_duration = "E" ;
	} else

	if (rate_duration == "MY" || rate_duration == "MO") {
		pd_rate_duration = "M" ;
	} else

	if (rate_duration == "DY" || rate_duration == "DL" || rate_duration == "D") {
		pd_rate_duration = "D" ;
	}
	else {
		pd_rate_duration = "X" ;
	} 
	//}
	if(free_mile_code == "MI" || free_mile_code == "ML" || free_mile_code == "M")
	{
		pd_free_mile_code = "M";
	}
	else if(free_mile_code == "KM" || free_mile_code == "K")
	{
		pd_free_mile_code = "K";
	}
	
	//Code for parsing RG, RQ- - US662 ENDS
	
	/* //Commneting existing code for parsing RG- - US662 STARTS
	idx = 1;
	if (li_count >= idx) {
		pd_cur_id = lv_arrayField[idx];
		pd_free_mile_code = "K";
		if (pd_cur_id == "USD") {
			pd_free_mile_code = "M";
		}
	}
	idx++;

	if (li_count >= idx) {
		li_pos = lv_arrayField[idx].findSubstring("-");
		if (li_pos > 0) {
			lv_rawAmount = lv_arrayField[idx].For(li_pos-1);
   		AB_Money lm_amount = lv_rawAmount.to_money("#######.##", lb_ok);
			pd_local_rate = lv_rawAmount;
			if (!lb_ok) {
				pd_local_rate = "0.00";
			}

			pd_booked_rate = pd_local_rate;
			pd_regular_rate = pd_local_rate;
		}
	}
	idx++;

	if (li_count >= idx) {
		if (lv_arrayField[idx] == "UNL") {
			pd_free_mileage = "UNLIM";
		} else {
			li_pos = lv_str.findFirstCharNotInList("0123456789.");
			if (li_pos > 0) {
				pd_free_mileage = lv_arrayField[idx].For(li_pos-1);
			}
		}
	}
	idx++;

	pd_rate_duration = "D";
	if (li_count >= idx) {
		if (lv_arrayField[idx] == "WK" || lv_arrayField[idx] == "WY") {
			pd_rate_duration = "W" ;
		} else

		if (lv_arrayField[idx] == "WE" || lv_arrayField[idx] == "WD") {
			pd_rate_duration = "E" ;
		} else

		if (lv_arrayField[idx] == "MO" || lv_arrayField[idx] == "MY") {
			pd_rate_duration = "M" ;
		} else

		if (lv_arrayField[idx] == "BR") {
			pd_rate_duration = "X" ;
		} else

		if (lv_arrayField[idx] == "DY" || lv_arrayField[idx] == "D") {
			pd_rate_duration = "D" ;
		}
	}
	idx++;


	if (li_count >= idx) {
		li_pos = lv_arrayField[idx].findSubstring("-");
		if (li_pos > 0) {
			lv_rawAmount = lv_arrayField[idx].For(li_pos-1);
		}
	}
	idx++;

	if (li_count >= idx) {
		// we do not parse this free mileage
		if (lv_arrayField[idx] == "UNL") {
		} else {
		}
	}
	idx++;

	if (li_count >= idx) {
		if (lv_arrayField[idx] == "XD") {
			pd_ex_day_charge = lv_rawAmount;
		} else 

		if (lv_arrayField[idx] == "XH") {
			pd_ex_hr_charge = lv_rawAmount;
		}
	}
	idx++;


	if (li_count >= idx) {
		li_pos = lv_arrayField[idx].findSubstring("-");
		if (li_pos > 0) {
			lv_rawAmount = lv_arrayField[idx].For(li_pos-1);
		}
	}
	idx++;

	if (li_count >= idx) {
		// we do not parse this free mileage
		if (lv_arrayField[idx] == "UNL") {
		} else {
		}
	}
	idx++;

	if (li_count >= idx) {
		if (lv_arrayField[idx] == "XH") {
			pd_ex_hr_charge = lv_rawAmount;
		}
	}
	*/ //Commneting existing code for parsing RG- - US662 ENDS
}

void
CAIR::getXFbreakdown(const D_amount& 			pd_tot_amt,
							const D_passenger_no&	pd_pax_id,
							const D_line_number&		pd_line_number,
							const AB_Varchar&			pv_fare) {
	/*
	 * pv_fare looks like XF LGA3EWR3CLE3DEN3
	 * Maximum break down to 4 XF city tax, rest of them
	 * consolidate into the 4th one
	 */

	AB_Varchar   lv_xf_str(255);
	lv_xf_str = pv_fare;

	int li_pos = lv_xf_str.findSubstring("XF ") ;

	if ( li_pos > 0 )
	{
		lv_xf_str = lv_xf_str.At(li_pos+3) ;
		lv_xf_str.removeLeadAndTrailBlanks();
		lv_xf_str.substitute("#","");
	} else {
		lv_xf_str = AB_null;
	}

	if ( lv_xf_str.NotIsNull() ) 
	{
		D_city   lav_city[5];
		D_amount lav_amt[5];
		D_amount ld_totalXF;
      int li_xfTotal = 0;
		ld_totalXF     = D_amount(0);

      while ( lv_xf_str.For(3).isAllAlpha() &&
              lv_xf_str.At(4).For(1).isAllNumeric() )
      {
		   if ( li_xfTotal == 4 ) 
			{
			   // Add into the fourth city

				lav_amt[4] = lav_amt[4] + 
								 D_amount(lv_xf_str.At(4).For(1));
			} 
			else 
			{
			   // Create a new XF city

				li_xfTotal++;
				lav_city[li_xfTotal] = lv_xf_str.For(3);
				lav_amt[li_xfTotal]  = lv_xf_str.At(4).For(1);
			}

			ld_totalXF = ld_totalXF + D_amount(lv_xf_str.At(4).For(1));
		   lv_xf_str = lv_xf_str.At(5);
	  }

	  if ( li_xfTotal > 0 ) {

        /** 
		   * Breakdown XF into multiple cities
			**/

        for ( int ii = 1; ii <= li_xfTotal; ii++ )
		  {
			  // Convert  back into original curency

			  if ( ii == li_xfTotal ) 
           {
				  // To avoid rounding problem

			     lav_amt[ii] = pd_tot_amt;

              for ( int jj = 1; jj < li_xfTotal; jj++ )
              {
			        lav_amt[ii] = lav_amt[ii] - lav_amt[jj];
				  }
			  }
			  else 
			  {
			     //lav_amt[ii] = lav_amt[ii]*(pd_tot_amt/ld_totalXF); not working !!
			     lav_amt[ii] = lav_amt[ii]*pd_tot_amt;
			     lav_amt[ii] = lav_amt[ii]/ld_totalXF;
			  }

			  md_ITD_pax_id[mi_ITD_I]       = pd_pax_id;
			  md_ITD_line_number[mi_ITD_I]  = pd_line_number;
			  md_ITD_tax_id[mi_ITD_I]       = "XF";

				if ( lav_city[ii].IsNull() )
					lav_city[ii] = D_city("ZZZ");
							
			  md_ITD_city_id[mi_ITD_I]      = lav_city[ii];
			  md_ITD_tax_amount[mi_ITD_I++] = lav_amt[ii];
	 	  }
      } else {
        /** 
			* XF is not broken down
			**/

			md_ITD_pax_id[mi_ITD_I]       = pd_pax_id;
			md_ITD_line_number[mi_ITD_I]  = pd_line_number;
			md_ITD_tax_id[mi_ITD_I]       = "XF";
			md_ITD_city_id[mi_ITD_I]      = D_city("ZZZ");
			md_ITD_tax_amount[mi_ITD_I++] = pd_tot_amt;
	   }
	} else {
	  /** 
		* XF is not broken down
		**/

		md_ITD_pax_id[mi_ITD_I]       = pd_pax_id;
		md_ITD_line_number[mi_ITD_I]  = pd_line_number;
		md_ITD_tax_id[mi_ITD_I]       = "XF";
		md_ITD_city_id[mi_ITD_I]      = D_city("ZZZ");
		md_ITD_tax_amount[mi_ITD_I++] = pd_tot_amt;
	}
}


AB_Char
CAIR::deriveDte(const AB_Varchar & pd_dte)
{
	AB_Char      lc_dte(7);
	AB_Char      lc_year(2);

	if (pd_dte == "     ")
	{
		return "       ";
	}

	lc_year       = mt_lclPnrDate.get_default_year(pd_dte);

	lc_year.rightJustify('0');
	lc_dte     = pd_dte + lc_year;
	
	return lc_dte;
}

AB_Integer
CAIR::deriveArrInd(const AB_Varchar &pd_depdte,
						 const AB_Varchar &pd_arrdte)
{
	AB_Date       ld_tempDate1;
	AB_Date       ld_tempDate2;
	AB_Integer    li_arrInd;

	if (pd_arrdte == "     ")
	{
		return 0;
	}

	ld_tempDate1  = deriveDte(pd_depdte);
	ld_tempDate2  = deriveDte(pd_arrdte);

   li_arrInd = ld_tempDate2 - ld_tempDate1 ;

   li_arrInd = li_arrInd / AB_Integer(86400) ;

   switch(li_arrInd.to_long())
   {
      case 0:
         return AB_Integer(0) ;
      case 1:
         return AB_Integer(1) ;
      case 2:
         return AB_Integer(2) ;
      case 3:
         return AB_Integer(3) ;
      case -1:
         return AB_Integer(4) ;
      case -2:
         return AB_Integer(5) ;
      default:
         return AB_Integer(0) ;
   }
}

void
CAIR::getAcodePhone(const AB_Varchar &token,
						  D_area_code &acode,
						  D_phone_number &phone)
{
	AB_Varchar	lv_tmp(255);

	lv_tmp = "";

	for (unsigned i = 1; i <= token.length(); i++)
	{
		if (token.At(i).For(1).isAllNumeric())
		{
			lv_tmp = lv_tmp + token.At(i).For(1);
		}
	}

	acode	= lv_tmp.For(3);
	phone	= lv_tmp.At(4);
}

void
CAIR::getNumbersFromI_(CairRecord &po_a,
							  D_area_code &pd_IA_phone_acode,
				  			  D_phone_number &pd_IA_phone_no,
				  			  D_area_code &pd_IA_bphone_acode,
				  			  D_phone_number &pd_IA_bphone_no,
				  			  D_area_code &pd_IA_fax_acode,
				  			  D_phone_number &pd_IA_fax_no)
{
	AB_Boolean		lb_Hprocessed;
	AB_Boolean		lb_Bprocessed;
	AB_Boolean		lb_Fprocessed;
	int 				li_count;
	V_AB_Varchar 	lva(255,255);
	li_count = lva.parseStringMultDel(po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_C4088I_.APINFO,"//");

	lb_Hprocessed	= AB_false;
	lb_Bprocessed	= AB_false;
	lb_Fprocessed	= AB_false;

	for (unsigned i = 1; i <= li_count; i++)
	{
		if (lva[i].NotIsNull())
		{
			if ((lva[i].At(lva[i].length()-1).For(2) == "-H" ||
				  lva[i].For(2) == "H-" ) &&
				 !lb_Hprocessed)
			{
				getAcodePhone(lva[i],pd_IA_phone_acode,pd_IA_phone_no);

				lb_Hprocessed	= AB_true;
			}

			if ((lva[i].At(lva[i].length()-1).For(2) == "-B" ||
				  lva[i].For(2) == "B-" ) &&
				 !lb_Bprocessed)
			{
				getAcodePhone(lva[i],pd_IA_bphone_acode,pd_IA_bphone_no);

				lb_Bprocessed	= AB_true;
			}

			if ((lva[i].At(lva[i].length()-1).For(2) == "-F" ||
				  lva[i].For(2) == "F-" ) &&
				 !lb_Fprocessed)
			{
				getAcodePhone(lva[i],pd_IA_fax_acode,pd_IA_fax_no);

				lb_Fprocessed	= AB_true;
			}
		}
	}
}

void
CAIR::getAddressFromAB(CairRecord &po_a,const int &i,
								D_address_line &pd_IA_addr1,
								D_address_line &pd_IA_addr2,
								D_address_line &pd_IA_addr3,
								D_address_line &pd_IA_addr4,
								D_address_line &pd_IA_addr5,
								D_address_line &pd_IA_addr6)
{
	V_AB_Varchar	lva(255,4);
	int 				li_count;

	li_count = lva.parseStringMultDel(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CABGrp.mo_C4138AB[0]->BILLADDR,",");

	lva[1].removeLeadingBlanks();lva[1].removeTrailingBlanks();
	lva[2].removeLeadingBlanks();lva[2].removeTrailingBlanks();
	lva[3].removeLeadingBlanks();lva[3].removeTrailingBlanks();
	lva[4].removeLeadingBlanks();lva[4].removeTrailingBlanks();

	pd_IA_addr1	= lva[1].For(29);
	pd_IA_addr2 = lva[2].For(30);
	pd_IA_addr3	= lva[3].For(29);
	pd_IA_addr4	= lva[4].For(30);
}

void
CAIR::getAddressFromAM(CairRecord &po_a,const int &i,
								D_address_line &pd_IA_addr1,
								D_address_line &pd_IA_addr2,
								D_address_line &pd_IA_addr3,
								D_address_line &pd_IA_addr4,
								D_address_line &pd_IA_addr5,
								D_address_line &pd_IA_addr6)
{
	V_AB_Varchar	lva(255,4);
	int 				li_count;

	li_count = lva.parseStringMultDel(po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CAMGrp.mo_C4140AM[0]->MAIL,",");

	lva[1].removeLeadingBlanks();lva[1].removeTrailingBlanks();
	lva[2].removeLeadingBlanks();lva[2].removeTrailingBlanks();
	lva[3].removeLeadingBlanks();lva[3].removeTrailingBlanks();
	lva[4].removeLeadingBlanks();lva[4].removeTrailingBlanks();

	pd_IA_addr1	= lva[1].For(29);
	pd_IA_addr2 = lva[2].For(30);
	pd_IA_addr3	= lva[3].For(29);
	pd_IA_addr4	= lva[4].For(30);
}

void
CAIR::getAddressFromABS(CairRecord &po_a,const int &i,
								D_address_line &pd_IA_addr1,
								D_address_line &pd_IA_addr2,
								D_address_line &pd_IA_addr3,
								D_address_line &pd_IA_addr4,
								D_address_line &pd_IA_addr5,
								D_address_line &pd_IA_addr6)
{

	V_AB_Varchar	lva_a(255,9);
	int				j;
	int				k;
	AB_Boolean		lb_ok;

	lb_ok	= AB_true;

	#define a4139ABS(att)\
	po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CABSGrp.mo_C4139ABS[0]->att

	lva_a[1]		= a4139ABS(COMPANY);
	lva_a[2]		= a4139ABS(NAME);
	lva_a[3]		= a4139ABS(ADRS1);
	lva_a[4]		= a4139ABS(ADRS1_1);

	if (a4139ABS(CITY).length() != 0)
	{
		if (a4139ABS(STATE).length() != 0)
		{
			lva_a[5] = a4139ABS(CITY) + " " + a4139ABS(STATE);
		}
		else
		{
			lva_a[5] = a4139ABS(CITY);
		}
	}
	else
	{
		if (a4139ABS(STATE).length() != 0)
		{
			lva_a[5] = a4139ABS(STATE);
		}
	}

	if (lva_a[5].length() > 0)
	{
		if (a4139ABS(COUNTRY).length() != 0)
		{
			lva_a[5]	= lva_a[5] + " " + a4139ABS(COUNTRY);
		}
	}
	else
	{
		lva_a[5]	= a4139ABS(COUNTRY);
	}

	lva_a[6]		= a4139ABS(ZP);

	pd_IA_addr1	= lva_a[1];
	pd_IA_addr2	= lva_a[2];
	pd_IA_addr3	= lva_a[3];
	pd_IA_addr4	= lva_a[4];
	pd_IA_addr5	= lva_a[5];
	pd_IA_addr6	= lva_a[6];
}

void
CAIR::getAddressFromAMS(CairRecord &po_a,const int &i,
								D_address_line &pd_IA_addr1,
								D_address_line &pd_IA_addr2,
								D_address_line &pd_IA_addr3,
								D_address_line &pd_IA_addr4,
								D_address_line &pd_IA_addr5,
								D_address_line &pd_IA_addr6)
{

	V_AB_Varchar	lva_a(255,9);
	int				j;
	int				k;
	AB_Boolean		lb_ok;

	lb_ok	= AB_true;

	#define a4141AMS(att)\
	po_a.mo_CPaxSegGrp.mo_CPaxSeg[i]->mo_CAMSGrp.mo_C4141AMS[0]->att

	lva_a[1]		= a4141AMS(COMPANY);
	lva_a[2]		= a4141AMS(NAME);
	lva_a[3]		= a4141AMS(ADRS1);
	lva_a[4]		= a4141AMS(ADRS1_1);

	if (a4141AMS(CITY).length() != 0)
	{
		if (a4141AMS(STATE).length() != 0)
		{
			lva_a[5] = a4141AMS(CITY) + " " + a4141AMS(STATE);
		}
		else
		{
			lva_a[5] = a4141AMS(CITY);
		}
	}
	else
	{
		if (a4141AMS(STATE).length() != 0)
		{
			lva_a[5] = a4141AMS(STATE);
		}
	}

	if (lva_a[5].length() > 0)
	{
		if (a4141AMS(COUNTRY).length() != 0)
		{
			lva_a[5]	= lva_a[5] + " " + a4141AMS(COUNTRY);
		}
	}
	else
	{
		lva_a[5]	= a4141AMS(COUNTRY);
	}

	lva_a[6]		= a4141AMS(ZP);

	pd_IA_addr1	= lva_a[1];
	pd_IA_addr2	= lva_a[2];
	pd_IA_addr3	= lva_a[3];
	pd_IA_addr4	= lva_a[4];
	pd_IA_addr5	= lva_a[5];
	pd_IA_addr6	= lva_a[6];
}

void 
CAIR::getFormOfPay(CairRecord& 	po_a,
						 AB_Varchar&   pv_form_of_pay,
						 AB_Varchar&	pv_cc_id,
						 AB_Varchar&	pv_cc_no,
						 AB_Varchar&	pv_cc_exp_date,
						 AB_Varchar&	pv_auth_no
						 )
{
	if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFPGrp.mo_C4118FP.size() > 0)
	{
		
		int				i;
		int				j;
		int				k;
		AB_Varchar		lv_tmp(80);
		AB_Varchar		paymnt(80);

		//
		// all tickets in the PNR use the same form of payment, i.e.,
		// the first form of payment of first passanger
		// PAYMNT can look like
		// CASH
		// CHECK
		// CCVI4444333322221111/1104
		// CCAX3714000000000000/1202/NTEST
		// O/CASH
		// O/*CHECK
		// O/CASH+/CASH
		// O/CHECK+/CHECK
		// O/CCVI+/CCVI4444333322221111/1105
		//

		paymnt = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CFPGrp.mo_C4118FP[0]->PAYMNT;

		if (paymnt.For(2) == "O/") {
			// exchange ticket form of paymnt
			V_AB_Varchar   lva(80,80);

			paymnt = paymnt.At(3);
			i = paymnt.findSubstring("+/");
			if (i > 0) {
				paymnt = paymnt.At(i+2);
			}
		}

		if (paymnt.For(5) == "*CASH" 		||
			 paymnt.For(4) == "CASH"  		||
			 paymnt.For(7) == "VOUCHER" 	||
			 paymnt.For(8) == "*VOUCHER")
		{
			pv_form_of_pay = "CA";
		}
		else if (paymnt.For(6) == "*CHECK" ||
					paymnt.For(5) == "CHECK")
		{
			pv_form_of_pay = "CK";
		}
		else
		{
			if (paymnt.For(2) == "CC")
			{
				lv_tmp				= paymnt;
			}
			else if (paymnt.For(3) == "*CC")
			{
				lv_tmp				= paymnt.At(2);
			}
			else if (paymnt.For(2) == "GR")
			{
				lv_tmp            = paymnt;
			}
			else if (paymnt.For(3) == "SGR")
			{
				lv_tmp            = paymnt.At(2);
			}
			else if (paymnt.For(3) == "*GR")
			{
				lv_tmp            = paymnt.At(2);
			}
			else if (paymnt.For(4) == "*SGR")
			{
				lv_tmp            = paymnt.At(3);
			}	

			pv_form_of_pay 	= "CC";

			if (lv_tmp.For(2) == "GR")
			{

				pv_cc_id			= lv_tmp.For(2);
				pv_cc_no			= lv_tmp.At(3);
			}
			else
			{
				pv_cc_id				= lv_tmp.At(3).For(2);

				i = lv_tmp.findSubstring("/");
				j = lv_tmp.findSubstring("/",i + 1);
				k = lv_tmp.findSubstring("E");

				if (j > i)
				{
					/* lv_tmp like CCVI4444333322221111/1105/N123456 */

					pv_cc_no       = lv_tmp.At(5).For(i - 5);
					pv_cc_exp_date = lv_tmp.At(i + 1).For(4);
					pv_auth_no		= lv_tmp.At(j + 2);
				}
				else if (k > 0 && k < i)
				{
					/* lv_tmp like CCAX370000000000028E06/1105 */

					pv_cc_no       = lv_tmp.At(5).For(k - 5);
					pv_cc_exp_date = lv_tmp.At(i + 1).For(4);
				}
				else if (i > 0 && k == 0 && j == 0)
				{
					pv_cc_no 		= lv_tmp.At(5).For(i - 5);
					pv_cc_exp_date = lv_tmp.At(i + 1);
				}
				else
				{
					if ( i > 1 )
						pv_cc_no       = lv_tmp.At(5).For(i - 5);
					else
						pv_cc_no       = lv_tmp.At(5);
				}
			}
		}
	}
}

void CAIR::saveSvcIrregularity()
{
   D_description   ld_errorDescription ;
   AB_Integer      li_msgId ;

   md_queType = "SFC" ;

   li_msgId = mi_rejectirId;

   //CMsgHandler::getMsgDescWithSub(li_msgId, "E", 
	//										 mv_rejectirSub,
	//										 ld_errorDescription) ;

   CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;

   createQueue(ld_errorDescription) ;
}

int
CAIR::findTax(const D_tax&					pd_tax_id,
				  const D_passenger_no&		pd_pax_id,
				  const D_line_number&		pd_line_number) const {
	for (int i = 0; i < mi_ITD_I; i++) {
		if (md_ITD_tax_id[i] 		== pd_tax_id &&
			 md_ITD_pax_id[i] 		== pd_pax_id &&
			 md_ITD_line_number[i]	== pd_line_number) {
			return i;
		}
	}
	return -1;
}

void 
CAIR::printAll()
{
	int		li;

	LOG(logDEBUG) << "\n";

	LOG(logDEBUG) << "AMADEUS AIR PRINT" ;
	LOG(logDEBUG) << "***********************************************************" ;

   LOG(logDEBUG) << "\n";

   LOG(logDEBUG) << "START HEADER PRINT"  ;
   print_Irtripheader() ;
   LOG(logDEBUG) << "END HEADER PRINT"  ;

   LOG(logDEBUG) << "START IRADDRESS PRINT"  ;
   printIraddress() ;
   LOG(logDEBUG) << "END IRADDRESS PRINT"  ;

   LOG(logDEBUG) << "START IRPAYMENT PRINT"  ;
   printIrpayment() ;
   LOG(logDEBUG) << "END IRPAYMENT PRINT"  ;

   LOG(logDEBUG) << "START DOCUMENTTAX PRINT"  ;
   printIrtaxdetail() ;
   LOG(logDEBUG) << "END DOCUMENTTAX PRINT"  ;

   LOG(logDEBUG) << "START FREQUSER PRINT "  ;
   printFreqUser() ;
   LOG(logDEBUG) << "END FREQUSER PRINT"  ;

   LOG(logDEBUG) << "START OTHER PRINT"  ;
   printOther() ;
   LOG(logDEBUG) << "END OTHER PRINT" ;

   LOG(logDEBUG) << "START IRPERSON PRINT"  ;
   printIrperson() ;
   LOG(logDEBUG) << "END IRPERSON PRINT"  ;

   LOG(logDEBUG) << "START IRTICKET PRINT"  ;
   printIrticket() ;
   LOG(logDEBUG) << "END IRTICKET PRINT"  ;

   LOG(logDEBUG) << "START IRTAX PRINT"  ;
   printIrtaxdetail() ;
   LOG(logDEBUG) << "END IRTAX PRINT"  ;

   LOG(logDEBUG) << "START PERSONITIN PRINT"  ;
   printIrconsumdetail() ;
   LOG(logDEBUG) << "END PERSONITIN PRINT"  ;

   LOG(logDEBUG) << "START REMARK PRINT - TRANSPORT"  ;
   printIrremark() ;
   LOG(logDEBUG) << "END REMARK PRINT - TRANSPORT"  ;

   LOG(logDEBUG) << "START RENTAL PRINT"  ;
   print_Irrental() ;
   LOG(logDEBUG) << "END RENTAL PRINT"  ;

   LOG(logDEBUG) << "START TRANSPORT PRINT"  ;
   printIrtransport() ;
   LOG(logDEBUG) << "END TRANSPORT PRINT"  ;
}

void CAIR::RP_parseRemarks(CairRecord &po_a)
{
	for (unsigned  i = 0; i < po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM.size(); i++)
	{
	     // SCR 517902: Virtuoso Remark Parser for VFT/VFC
      AB_Text lt_remark_text = po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.At(2);
      if ((lt_remark_text.For(4) == "VFT/") || (lt_remark_text.For(4) == "VFC/"))  {
            V_AB_Varchar      lv_arrayField(100,100) ;
            lv_arrayField.parseStringMultDel(lt_remark_text,"/") ;
            AB_Boolean lb_valid;
            lb_valid= AB_true;
            for (int li = 1; li <= lv_arrayField[2].length(); li++) {
               if (((lv_arrayField[2].At(li).For(1) < "0") || (lv_arrayField[2].At(li).For(1) > "9")) && (lv_arrayField[2].At(li).For(1) != ".")) {
                  lb_valid = AB_false;
               }}
            if (lb_valid == AB_true) {
                     if (lt_remark_text.For(4) == "VFT/") {
                        md_IRMK_type[mi_IRMK_I]             = "VTA";
                        md_IRMK_remark[mi_IRMK_I++]            = lv_arrayField[2];
                        md_IRMK_type[mi_IRMK_I]             = "VTD";
                        md_IRMK_remark[mi_IRMK_I++]            = lv_arrayField[3];
                        }
                     if (lt_remark_text.For(4) == "VFC/") {
                        md_IRMK_type[mi_IRMK_I]             = "VCA";
                        md_IRMK_remark[mi_IRMK_I++]            = lv_arrayField[2];
                        md_IRMK_type[mi_IRMK_I]             = "VCD";
                        md_IRMK_remark[mi_IRMK_I++]            = lv_arrayField[3];
                        }
					}
					else {
                  if (lt_remark_text.For(4) == "VFT/")
                        createRemark("__Q", "FFI/FEE FORMAT INVALID - FEE WAS NOT CREATED");
                  if (lt_remark_text.For(4) == "VFC/")
                        createRemark("__Q", "FFI/FEE FORMAT INVALID - FEE WAS NOT CREATED");
						}
          }

	
		pRemarkParser->parse(po_a.mo_CPaxSegGrp.mo_CPaxSeg[0]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.At(2).to_string());
		//if (po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.For(pt_what.length()+2) == "*" + pt_what + "/")
		//{
		//	return po_a.mo_CPaxSegGrp.mo_CPaxSeg[pax_id]->mo_CRMGrp.mo_C4145RM[i]->PNRRMK.At(pt_what.length()+3);
		//}
	}
}
