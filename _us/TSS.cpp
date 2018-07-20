/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999, 
															 2000, 2001
                SID: 1.116
      Delta Created: 14/11/04 - 14:29:54
         Last Delta: app/src/common/InterfaceRecord/_us 11/4/14 - 14:29:54 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TSS.cpp) 
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us}[11/4/14-14:29:54 1.116] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TSS.cpp (Jinsong Liu)\n" )
/*
  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
============================================================ END GLOBAL BANNER*/
#include "gtypes.h"
#include "TSS_D.h"

#include "C.h"
#include "Money.h"
#include "matsysT.h"
#include "MsgHandler.h"
#include "dbraw.h"
#include "PNRDetail.h"
#include "dbMatrix.h"
#include "PNR.h"
#include "dbIR.h"
#include "InterfaceRecord_D.h"
#include "InterfaceRecord.h"
#include "Address.h"
#include "TSS.h"
#include "MirSvcFee.h"

#define ENTER      LOG(logINFO) << "\n*** Enter>> " << __FUNCTION__
#define EXIT       LOG(logINFO) << "\n*** Exit << " << __FUNCTION__

//------------------------------------------------------------------------------

void CTSS::printAll()
{
   print_Irtripheader() ;

   printIraddress() ;

   printIrpayment() ;

   printIrtaxdetail() ;

   printFreqUser() ;

   printOther() ;

   printIrperson() ;

   printIrticket() ;

   printIrtaxdetail() ;

   printIrconsumdetail() ;

   printIrremark() ;

   print_Irrental() ;

   printIrtransport() ;

	printIremailaddress() ;

}
CTSS::CTSS () :mv_baggage(10),mv_service_status(10),mv_IrCarData(255,100),
					mv_manualTicketData(255,100),mv_IrHotelData(255,100),
					mv_exchTaxData(255,100),mv_a14CC(100),md_nonArcDoc(255,100),
					md_nonArcCost(255,100),mv_NonArcCC(25),
               mv_svcSF(100),mv_svcFEE(100)
{
	mi_SFC_line_number = 0;
}

//------------------------------------------------------------------------------
void CTSS::errorHandler(CPNR &po_PNR)
{
   D_description   ld_errorDescription ;
	AB_Integer      li_msgId ;

	li_msgId = CMsgHandler::getMsgId() ;

	if ( li_msgId == AB_Integer(9887) )
	{
		CMsgHandler::getSybMsgInfo(mi_syb_code,mv_syb_message) ;
		ld_errorDescription = AB_null ;
	}
   else
	{
      //ld_errorDescription = CMsgHandler::getMsgWithSub(AB_Text("E"),
		//															    AB_Boolean(AB_true)) ; 
		
      CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;
	}

   CMsgHandler::clearMsg();

   po_PNR.createGirRawTables() ;

   createQueue(ld_errorDescription) ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::createCTSS(CPNR &po_PNR)
{
   CDMatrix lo_Matrix;
   AB_Integer li_sqlcode;

   // LOG(logDEBUG) << "START TSS PROCESSING" << endl ;
   fill_IrHeader(po_PNR) ;

	if ( mb_ignoreMir )
		return ;

   if (CMsgHandler::getMsgStatus())
      createIrItins(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
      fill_IrPasDocument(po_PNR); 

   if (CMsgHandler::getMsgStatus())
      processNonArcDoc(); 

   if (CMsgHandler::getMsgStatus())
      fill_IrRentalandOther(po_PNR) ;

	if (CMsgHandler::getMsgStatus())
		checkRental() ;

	if (CMsgHandler::getMsgStatus())
		processExtraTicketLine() ;

	// G145 - Surface Segment
	if (CMsgHandler::getMsgStatus())
		createARNK() ;

	// G9015 - Service Fee Charge
   if (CMsgHandler::getMsgStatus())
      processSFC() ; 

	if (CMsgHandler::getMsgStatus())
		processReceipt();

	// US044 START
	if (CMsgHandler::getMsgStatus())
		IrTransportRemarks(po_PNR);
	// US044 END

   md_queType = "RMA" ;
   if (! CMsgHandler::getMsgStatus())
   {
      //{ Set info for communication group, point of return 3

      AB_Varchar lv_msgId(10);
      lv_msgId = "3," + AB_Text( CMsgHandler::getMsgId() );

      po_PNR.MTX_yield(AB_Varchar(30,"Csssncntx1"),
                        AB_Varchar(30,"ferror_fld"),
                        lv_msgId );
      //}

      errorHandler(po_PNR);
      return ;
   }

   createIrRows() ;
   
   if (! CMsgHandler::getMsgStatus())
   {
      //{ Set info for communication group, point of return 4

      AB_Varchar lv_msgId(10);
      lv_msgId = "4," + AB_Text( CMsgHandler::getMsgId() );

      po_PNR.MTX_yield(AB_Varchar(30,"Csssncntx1"),
                        AB_Varchar(30,"ferror_fld"),
                        lv_msgId );
      //}

      lo_Matrix.dbRollback(li_sqlcode) ;
      errorHandler(po_PNR) ;
      return ;
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
      lo_Matrix.dbCommit(li_sqlcode);
   }

   checkDomainEdits() ;

   if (CMsgHandler::getMsgStatus())  
      createClientfile() ;

   if (! CMsgHandler::getMsgStatus())
   {
      //{ Set info for communication group, point of return 5

      AB_Varchar lv_msgId(10);
      lv_msgId = "5," + AB_Text( CMsgHandler::getMsgId() );

      po_PNR.MTX_yield(AB_Varchar(30,"Csssncntx1"),
                        AB_Varchar(30,"ferror_fld"),
                        lv_msgId );
      //}

      md_queType = "RMI" ;
      lo_Matrix.dbRollback(li_sqlcode);
      errorHandler(po_PNR) ;
   } 
   else
   {		 
      if ( mi_rejectirId > AB_Integer(0) )
      {
          // Insert captured complains
          // into Rejectir

          saveSvcIrregularity();
      }
   }
	moveReceiptQueues( mv_rec_queType );
	lo_Matrix.dbCommit(li_sqlcode);


   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_IrHeader(CPNR &po_PNR)
{
   ENTER; 

   AB_Text     lt_total_segment ;
   AB_Varchar  lv_long_address(100);
   int         li_pos, 
               li_index, 
               li_counter ;

   li_counter        = 0 ;
   md_session_id     = po_PNR.getSessionId() ;
   md_interface_type = po_PNR.getInterfaceType() ;
   md_creation_date  = po_PNR.getCreationDate() ;
   md_IRT_pnr_time   = AB_null ;
	mb_ignoreMir      = AB_false ;
	mb_foundFare      = AB_false ;

   po_PNR.resetSegment("A00") ;
   lt_total_segment = po_PNR.getNextSegment("A00")  ;

   if (lt_total_segment.NotIsNull())
      md_selAccess = lt_total_segment.At(4).For(43) ;
   else 
      md_selAccess = AB_null ;

   po_PNR.resetSegment("HDR") ;
   lt_total_segment = po_PNR.getNextSegment("HDR") ;
   
   if (lt_total_segment.IsNull())
   {
      CMsgHandler::setMsg(AB_Integer(2509), AB_Char(15," "), D_error_track("TSS-PROCHDR"), AB_Integer(1), AB_Varchar(20,AB_null));
      return ;
   }
	else if ( lt_total_segment.At(248).For(1) == "6") 
	{
		// COUPON PRINT FOR VIRTUAL TICKER
		// Dont process this TSS
		mb_ignoreMir = AB_true ;
		return ;
	}
   else
      populate_IrHeader_SectionHDR(lt_total_segment) ;

   if (! CMsgHandler::getMsgStatus())
      return ;
 
   po_PNR.resetSegment("A11") ; // For of payment
   lt_total_segment   = po_PNR.getNextSegment("A11") ;
	storeFormOfPay(lt_total_segment);

   populate_IrHeader_SectionA14(lv_long_address, po_PNR) ;

	fill_Iraddress(lv_long_address,po_PNR) ;

   EXIT;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populate_IrHeader_SectionHDR(AB_Text &pt_total_segment)
{
   ENTER; 

   md_IRT_iata_no     = pt_total_segment.At(90).For(8) ;
   md_supId           = pt_total_segment.At(33).For(2) ;
   md_IRT_pnr_date    = pt_total_segment.At(125).For(7) ;
   md_IRT_start_date  = pt_total_segment.At(62).For(7) ;
   md_personalCrs     = pt_total_segment.At(258).For(1) ;
   md_crsTour         = pt_total_segment.At(227).For(15) ;
   md_IRT_reference   = pt_total_segment.At(99).For(6) ;
	md_IRT_reference.leftJustify();
	md_IRT_booking_no  = md_IRT_reference;

   /************************************
    * 22APR2010 - GAP G9123 SCR 440488 *
    ************************************
    * This was in the scope of the GAP at first
	 * then they took it out at the last minute.
	 * I'll leave it here in comment, since I bet
	 * they change their minds again.
    ************************************
			if ( pt_total_segment.At(82).For(4) != "    " )
			   md_IRT_booking_pcc = pt_total_segment.At(82).For(4)  ;
			if ( pt_total_segment.At(86).For(4) != "    " )
			   md_IRT_ticketing_pcc = pt_total_segment.At(86).For(4) ;

		   LOG(logDEBUG) << " -- md_IRT_booking_pcc         = |" << md_IRT_booking_pcc << "|"        << endl;
		   LOG(logDEBUG) << " -- md_IRT_ticketing_pcc       = |" << md_IRT_ticketing_pcc   << "|"    << endl;
    ************************************
	 */

   if (pt_total_segment.At(284).For(2) == "S:")
   {
      md_stpHomeCity = pt_total_segment.At(290).For(4) ;
      md_stpHostCity = pt_total_segment.At(286).For(4) ;
   }

   if (pt_total_segment.At(244).For(1) == "N")
      md_crsPriced = "Y" ;
   else
      md_crsPriced = "N" ;

  /**
	* T50IN6 ATB Indicator
	**/

	if (pt_total_segment.At(248).For(1) == "5")
		 md_e_ticket = "Y" ;
	else
	    md_e_ticket = "N" ;

   if (pt_total_segment.At(3).For(2) == "1V")
      md_IRT_crs_id = "APP" ;
   else if (pt_total_segment.At(3).For(2) == "SA")
      md_IRT_crs_id = "SAB" ;
   else if (pt_total_segment.At(3).For(2) == "WS")
      md_IRT_crs_id = "WSP" ;
   // NAVITAIRE START
   else if (pt_total_segment.At(3).For(2) == "1N")
      md_IRT_crs_id = "DIR" ;
   // NAVITAIRE END


   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::storeFormOfPay(AB_Text &pt_total_segment)
{
   ENTER; 

   md_form_of_pay = pt_total_segment.At(4).For(2) ; // Sets Form Of Pay
   md_A11FOP = md_form_of_pay;

   if (md_form_of_pay == "CC") // Sets value for Credit Card
   {
      md_cc_id       = pt_total_segment.At(19).For(2) ;
      md_cc_no       = pt_total_segment.At(21).For(20) ;
      md_cc_exp_date = pt_total_segment.At(41).For(4) ;
      md_auth_no     = pt_total_segment.At(47).For(6) ;

		// Set CC Info For Passenger

		md_IRMK_type[mi_IRMK_I]   = "CCI" ;
		md_IRMK_remark[mi_IRMK_I] = pt_total_segment.At(19).For(22) +
											 pt_total_segment.At(41).For(4) ;
		md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;

      if (md_auth_no.For(1) != " ")
      {
         if (pt_total_segment.At(46).For(1) == " ")
            md_auth_source = "A" ;
         else
            md_auth_source = " " ;
      }
   }
   else if ((md_form_of_pay == "X ") && (pt_total_segment.At(23).For(2).isAllNumeric()))
   {
      // Sets value for Freeform Credit Card
      md_form_of_pay = "CC" ;
      md_cc_id       = pt_total_segment.At(21).For(2) ;
      md_cc_no       = pt_total_segment.At(23).For(18) ;

		// Set CC Info For Passenger

		md_IRMK_type[mi_IRMK_I]   = "CCI" ;
		md_IRMK_remark[mi_IRMK_I] = pt_total_segment.At(19).For(20) ;
		md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;
   }
	/*
   else if ((md_form_of_pay == "X ") && (pt_total_segment.At(19).For(2) == "GR" ))
   {
		LOG(logDEBUG) << endl << "WE ARE IN GR" ;
      // Government Request Form of payment
      md_form_of_pay = "CC" ;
      md_cc_id       = "GR" ;
      md_cc_no       = pt_total_segment.At(23).For(18) ;
   }
	*/
   else if ((md_form_of_pay == "X ") && 
       (md_form_of_pay.At(21).For(3) == "INV") && 
       (md_form_of_pay.At(21).For(2) == "IV"))
   {
      md_form_of_pay = "IV" ; // Sets value for Freeform Invoice
		md_auth_source = AB_null ;
   }
   else if ((md_form_of_pay == "X ") || 
       (md_form_of_pay == "S") ||
       ((md_form_of_pay != "CA") && 
        (md_form_of_pay != "IV") && 
        (md_form_of_pay != "CK")))
   {
      md_form_of_pay = "CA"; // Sets all other Types of Form Of Payment to Cash
		md_auth_source = AB_null ;
   }

   if ((md_form_of_pay == "CC") && (md_cc_id == "90"))
   {
      // Sets Government Warrants
      md_cc_no = md_cc_id.For(2) +  md_cc_no ;
      md_cc_id = "GO" ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_Irpayment(AB_Text    &pt_total_segment,
								  AB_Integer &pi_paxId,
								  AB_Integer &pi_docLine)
{
	AB_Varchar		lc_fp(30) ;

	md_IRP_amount[mi_IRP_I] = pt_total_segment.At(24).For(12) ; // From A07 Section
	md_IRP_pax_id[mi_IRP_I] = pi_paxId ;
	md_IRP_line_number[mi_IRP_I] = pi_docLine ;
	md_IRP_pay_id[mi_IRP_I] = AB_Integer(1) ;

	if ( md_IRP_form_of_pay[mi_IRP_I] == "CC" )
		fillTktFP(mv_NonArcCC) ;
	else
		md_IRP_auth_source[mi_IRP_I] = AB_null ;

	mi_IRP_I++ ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_Iraddress(AB_Varchar &pv_long_address,
								  CPNR       &po_PNR)
{
	int           li_pos ;
	V_AB_Varchar  lv_arrayField(100,100) ;

	mb_address = AB_false ;

	if ( pv_long_address.IsNull())
	{
		// If A14 address is Null Use A13 delivery address
     	populate_Iraddress_SectionA13(pv_long_address, po_PNR) ;  
	}

   if (pv_long_address.NotIsNull())
	{
		li_pos = lv_arrayField.parseString(pv_long_address,"/") ;

      md_IA_addr1[mi_IA_I] = lv_arrayField[1] ;
		md_IA_addr2[mi_IA_I] = lv_arrayField[2] ;
		md_IA_addr3[mi_IA_I] = lv_arrayField[3] ;
		md_IA_addr4[mi_IA_I] = lv_arrayField[4] ;

		mb_address = AB_true ;
	 }

	// Get Phone field
   populate_Iraddress_SectionA12(po_PNR) ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populate_Iraddress_SectionA12(CPNR &po_PNR)
{
   ENTER; 

   int        li_counter, 
              li_pos ;
   AB_Varchar lv_phone(100) ;
   CAddress   lo_Address ;

   li_counter = 0;
   
   po_PNR.resetSegment("A12") ;
   lv_phone = po_PNR.getNextSegment("A12") ; 

   while (lv_phone.NotIsNull())
   {
      li_pos = lv_phone.findSubstring("!") ;

      if (li_pos == 0)
         li_pos = 65 ;

      lv_phone = lv_phone.For(li_pos - 1) ;
 
		LOG(logDEBUG) << "lv_phone: " << lv_phone << endl ;

		parsePhone(lv_phone) ;

      lv_phone = po_PNR.getNextSegment("A12") ;
   }

	overrideHomePhone();

	// Most of the time Fax comes as non numeric no.
	// and fax is used only in MML

   md_IA_fax_acode[mi_IA_I] = AB_null ;
   md_IA_fax_no[mi_IA_I]    = AB_null ;

	if (md_IA_phone_no[mi_IA_I].NotIsNull() || md_IA_bphone_no[mi_IA_I].NotIsNull() ||md_IA_fax_no[mi_IA_I].NotIsNull() ||  md_IA_mphone_no[mi_IA_I].NotIsNull() )
		mb_address = AB_true ;


	if (mb_address)
		md_IA_type[mi_IA_I++] = "P" ;
   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populate_IrHeader_SectionA14(AB_Varchar &pv_long_address, 
													 CPNR &po_PNR)
{
   ENTER; 

   AB_Char    lc_delimiter(2) ;
	AB_Char    lc_CONFdelimiter(8) ;
   int        li_pos, 
              li_counter,
				  li_slashPos;
   AB_Varchar lv_data(60) ;
   AB_Varchar lt_data(70) ;	//it is same as lv_data,just more characters
	AB_Varchar lm_CONFdata(60) ;
	AB_Char 	  lm_CONFsup(3) ;
	AB_Char    lm_testCONFsup(3) ;
   AB_Text    lt_segment ;
   AB_Text    lt_temp ;

	AB_Boolean lb_mlsFound = AB_false ;
	AB_Boolean lb_confFound = AB_false ;
	AB_Boolean lb_tvtFound = AB_false ;

	AB_Boolean		lb_pr_found = AB_false;
	AB_Boolean		lb_fop_found = AB_false;

   pv_long_address      = AB_null ;
   li_counter           = 0 ;
   mi_manualTicketIndex = 1 ;
   mi_exchTaxIndex      = 1 ;
   mi_IrCarIndex       = 1 ;
   gi_IrHotelIndex     = 1 ;
	mi_nonArcDoc        = 1 ;

   // Service Fee variable Initialization

   mb_SfFound      = AB_false;
   mb_FeeFound     = AB_false;
   mv_svcSF        = AB_null;
   mv_svcFEE       = AB_null;


   // TKX CC EXP DATE START
   AB_Char lc_ccExpDate(4);
   md_TKX_cc_exp_date = AB_null;
   // TKX CC EXP DATE END

   po_PNR.resetSegment("A14") ;
   lt_segment = po_PNR.getNextSegment("A14") ;

	mt_UDID = "";

   while (lt_segment.NotIsNull())
   {
      lc_delimiter = lt_segment.At(6).For(2) ;
      lv_data      = lt_segment.At(9) ;
      lt_data      = lt_segment.At(9) ;	// will be used by RP segment
      li_pos       = lv_data.findSubstring("!") ;

		lc_CONFdelimiter = lt_segment.At(6).For(5) ;

      if (li_pos > 0)
         lv_data = lv_data.For(li_pos - 1) ;

      if (lc_CONFdelimiter == "CONF/")
      {
         lb_confFound = AB_false ;

         lm_CONFdata  = lt_segment.At(11) ;

         lm_CONFdata.removeLeadingBlanks();
         li_slashPos  = lm_CONFdata.findSubstring("/");

         if ( li_slashPos > 1 && li_slashPos < 5 )
         {
            lm_CONFsup   = lm_CONFdata.For(li_slashPos-1);

            li_counter = 0;

            while ( li_counter < mi_IRMK_I )
            {
               if ( md_IRMK_type[li_counter] == "CNF" )
               {
                  lm_testCONFsup =  md_IRMK_type[li_counter].At(5).For(3);
                  if ( lm_CONFsup == lm_testCONFsup)
                  {
                     lb_confFound = AB_true;
                     break;
                  }
               }
					li_counter++;
            }

            if ( lb_confFound == AB_false )
            {

               lm_CONFdata  = lm_CONFdata.At(li_slashPos+1).For(20);
               li_pos       = lm_CONFdata.findSubstring("!") ;

               if ( li_pos > 0 )
                  lm_CONFdata = lm_CONFdata.For(li_pos - 1) ;

               lt_temp = lm_CONFdata;
               lt_temp.removeLeadingBlanks();
               lt_temp.removeTrailingBlanks();

               if(lt_temp.length()>0)
               {
                  md_IRMK_type[mi_IRMK_I]      = "CNF" ;
                  md_IRMK_pax_id[mi_IRMK_I]    = AB_null;
                  md_IRMK_remark[mi_IRMK_I++]  = "SUP:" + lm_CONFsup + "/" + lm_CONFdata ;
               }
            }
         }
      }
		// GAP 9042 START
		else if ( lc_delimiter == "TV" )
		{
			if ( lb_tvtFound == AB_false )
			{
				lb_tvtFound = AB_true;
         	lv_data.removeLeadingBlanks();
				if ( lv_data.IsNull() )
					lv_data = "X";
				md_IRMK_type[mi_IRMK_I]      = "TVT";
				md_IRMK_pax_id[mi_IRMK_I]    = AB_null;
				md_IRMK_remark[mi_IRMK_I++]  = lv_data.For(1);
			}
		}
		// GAP 9042 END
      else if (lc_delimiter == "PH")
      {
			mv_phoneField = lv_data;
		}
      else if (lc_delimiter == "CF")
      {
			if ( md_IRT_cfa_code.IsNull() && lv_data.For(3) != "   " )
			{
				// CFA EXP START

         	int li_length = (lv_data.length() > 11) ? 11:lv_data.length();
         	AB_Varchar lv_cfa_id(7);

         	lv_cfa_id = lv_data.At(4).For(li_length - 4 );

         	if ( lv_cfa_id.IsNull() )
            	lv_cfa_id = "0000000";
         	else
            	lv_cfa_id.padLeading( 7 - lv_cfa_id.length(), '0');

         	md_IRT_cfa_code = lv_data.For(3) ;
         	md_IRT_cfa_type = lv_data.At(li_length) ;
         	md_IRT_cfa_id   = md_IRT_cfa_code + lv_cfa_id + md_IRT_cfa_type ;

         	// CFA EXP END
      	}
		}

		else if (lt_segment.At(7).For(4) == "-NUC")
      {

         md_IRMK_type[mi_IRMK_I]     = "NUC";
         md_IRMK_remark[mi_IRMK_I]   = "Y";
         md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
      }

      else if (lc_delimiter == "EB")
      {
         /**
          * Electronic booking activity
          **/

         if ( mv_actionCode.IsNull() )
         {
            mv_actionCode = lv_data.For(2);
            mv_source     = lv_data.At(3).For(1) ;
            if ( lv_data.At(4).For(1) == "/" ) 
            {
               mv_reasonCode = lv_data.At(5).For(10);
            }
		   }
      }
      else if (lc_delimiter == "PR")
		{
			 if ( !lb_pr_found )
			 {
				 md_IRMK_type[mi_IRMK_I]     = "PR" ;
				 md_IRMK_remark[mi_IRMK_I]   = lv_data;
				 md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
				 lb_pr_found = AB_true;
			 }
		}
      else if (lt_segment.At(6).For(3) == "FOP" && !lb_fop_found)
		{
			 lv_data=lv_data.At(2);

			 if ( lv_data.NotIsNull())
			 {
				 lv_data.removeLeadAndTrailBlanks();

				 if ( lv_data == "AP" )
				 {
					 md_IRMK_type[mi_IRMK_I]     = "FOP" ;
					 md_IRMK_remark[mi_IRMK_I]   = lv_data ;
					 md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
					 lb_fop_found = AB_true;
				 }
			 }
		}
      else if (lc_delimiter == "CN")
		{
			if ( md_IRT_con_id.IsNull() && lv_data.For(3) != "   " )
         	md_IRT_con_id = lv_data.For(3) ;
		}
		/*
      else if (lc_delimiter == "DE")
		{
			if ( md_IRT_city_id.IsNull() && lv_data.For(3) != "   " )
         	md_IRT_city_id = lv_data.For(3) ;
		}
		*/
      else if (lc_delimiter == "DP")
		{
			if ( md_IRT_cfa_dpt.IsNull() && lv_data.For(4) != "    " )
         	md_IRT_cfa_dpt = lv_data.For(4) ;
		}
      else if (lc_delimiter == "RC")
		{
			if ( md_IRT_iata_dest.IsNull() && lv_data.For(2) != "  " )
         	md_IRT_iata_dest = lv_data.For(2) ;
		}
      else if (lc_delimiter == "ML")
		{
			if ( md_IRT_mktg_list.IsNull() && lv_data.For(1) != " " )
         	md_IRT_mktg_list = lv_data.For(1) ;
		}
      else if (lc_delimiter == "FS")
		{
			if ( md_savingCode.IsNull() ) {
				lv_data.removeLeadAndTrailBlanks();
				md_savingCode = lv_data;
			}
		}
      else if (lc_delimiter == "FF")
		{
			if ( md_full_fare.IsNull() && lv_data.For(10) != "          " )
         	md_full_fare      = lv_data.For(10) ;
		}
      else if (lc_delimiter == "LP")
		{
			if ( md_low_prac_fare.IsNull() && lv_data.For(10) != "          " )
         	md_low_prac_fare  = lv_data.For(10) ;
		}
      else if (lc_delimiter == "RB")
		{
			if ( md_rebate.IsNull() && lv_data.For(10) != "          " )
         	md_rebate    = lv_data.For(10) ;
		}
      else if (lc_delimiter == "HS")
		{
         //mv_IrHotelData[gi_IrHotelIndex++] = lv_data ;
         lv_data = lt_segment.At(8) ;
         li_pos  = lv_data.findSubstring("!") ;

         if (li_pos > 0)
            lv_data = lv_data.For(li_pos - 1) ;

         lt_temp=lv_data;
         lt_temp.removeLeadingBlanks();
		   if(lt_temp.length()>0)
            mv_IrHotelData[gi_IrHotelIndex++] = lv_data ;

		}
      else if (lc_delimiter == "CS")
		{
         //mv_IrCarData[mi_IrCarIndex++] = lv_data ;
         lv_data = lt_segment.At(8) ;
         li_pos  = lv_data.findSubstring("!") ;

         if (li_pos > 0)
            lv_data = lv_data.For(li_pos - 1) ;

         lt_temp=lv_data;
         lt_temp.removeLeadingBlanks();
		   if(lt_temp.length()>0)
            mv_IrCarData[mi_IrCarIndex++] = lv_data ;

		}
      else if (lt_segment.At(6).For(3) == "VEN")
		{
			if ( gm_sup_id.IsNull() && lv_data.At(2).For(3) != "   " )
         	gm_sup_id = lv_data.At(2).For(3) ;
		}
      else if (lc_delimiter == "TX")
		{
         lt_temp=lv_data;
         lt_temp.removeLeadingBlanks();
			if(lt_temp.length()>0)
            mv_exchTaxData[mi_exchTaxIndex++] = lv_data ;
	   }
      else if (lt_segment.At(4).For(3) == "TK-")
      {
         lv_data = lt_segment.At(7) ;
         li_pos = lv_data.findSubstring("!") ;

         if (li_pos > 0)
            lv_data = lv_data.For(li_pos - 1) ;

         lt_temp=lv_data;
         lt_temp.removeLeadingBlanks();
			if(lt_temp.length()>0)
            mv_manualTicketData[mi_manualTicketIndex++] = lv_data ;

      }
      else if (lc_delimiter == "MC" || lc_delimiter == "MA")
		{
			// Non Arc Document
			lv_data = lt_segment.At(6) ;
			li_pos = lv_data.findSubstring("!") ;
			if (li_pos > 0)
				lv_data = lv_data.For(li_pos - 1) ;
         lt_temp=lv_data;
         lt_temp.removeLeadingBlanks();
			if(lt_temp.length()>0)
			   md_nonArcDoc[mi_nonArcDoc++] = lv_data ;
		}
      else if (lc_delimiter == "CC" )
		{
			// Non Arc Document Credit Card No
			lv_data = lt_segment.At(9) ;
			li_pos = lv_data.findSubstring("!") ;
			if (li_pos > 0)
				lv_data = lv_data.For(li_pos - 1) ;

         lt_temp=lv_data;
         lt_temp.removeLeadingBlanks();
			if(lt_temp.length()>0)
			   mv_a14CC = lv_data ;
		}
      else if (lc_delimiter == "AD")
      {
         if (pv_long_address.IsNull())
            pv_long_address = lv_data + "/" ;
         else
            pv_long_address = pv_long_address + lv_data + "/" ;
      }
      else if(lc_delimiter == "SC") 
		{
			if ( !lb_mlsFound )
			{
				lb_mlsFound = AB_true ;
				md_IRMK_type[mi_IRMK_I]     = "MLS" ;
            lt_temp=lv_data;
            lt_temp.removeLeadingBlanks();
			   if(lt_temp.length()>0)
               md_IRMK_remark[mi_IRMK_I] = lv_data ;
				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}
		}
      else if(lt_segment.For(4) == "A14U")
         setShortRemarks(lt_segment.At(5)) ;      
		else if (lc_delimiter == "BB")
		{
			if ( md_IRT_cop_id.IsNull() )
			{
			   if(lv_data.For(2) != "  ")
			   	md_IRT_cop_id = lv_data.For(2) ;
			   if(lv_data.At(3)  != "    ")
			   	md_IRT_bra_id = lv_data.At(3) ;
			}
		}
      else if (lt_segment.At(4).For(3) == "SF-" && !mb_SfFound )
      {
         // Pick up service fee amount

         lv_data = lt_segment.At(7) ;

         // Remove ! field if any

         li_pos  = lv_data.findSubstring("!") ;
         if (li_pos > 0)
            lv_data = lv_data.For(li_pos - 1) ;
         lv_data.removeTrailingBlanks() ;

         mv_svcSF   = lv_data;
         mb_SfFound = AB_true ;
      }

		// GAP 9090F Start: POS fee
      else if (lt_segment.At(6).For(4) == "FEE/" )
      {
			lv_data = lt_segment.At(10) ;
			li_pos  = lv_data.findSubstring("!") ;
			if (li_pos > 0) {
				lv_data = lv_data.For(li_pos - 1) ;
			}
			lv_data.removeTrailingBlanks() ;
			lv_data = lv_data + "/";

			AB_Varchar lv_cc_id(40);
      	lv_cc_id = lv_data.getNthBracketedField(1, "FOP-CC", "/") ;
			if (lv_cc_id.NotIsNull() && !validCreditCardNumber(lv_cc_id)) {
				lv_data = "BADCC/" + lv_data;
			}

		  	md_IRMK_type[mi_IRMK_I]     = "FEE" ;
		  	md_IRMK_remark[mi_IRMK_I]   = lv_data;
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9090F End

      else if (lt_segment.At(6).For(3) == "FEE" && !mb_FeeFound )
      {
         // Pick up supplier and form of pay info

         lv_data = lt_segment.At(6) ;

         // Remove ! field if any

         li_pos  = lv_data.findSubstring("!") ;
         if (li_pos > 0)
            lv_data = lv_data.For(li_pos - 1) ;

         lv_data.removeTrailingBlanks() ;

         mv_svcFEE = lv_data;
         mb_FeeFound = AB_true ;
      }

      // TKX CC EXP DATE START

      else if ( lt_segment.At(6).For(3) == "EXP" )
      {
         lv_data      = lt_segment.At(10) ;
         li_pos       = lv_data.findSubstring("!") ;

         if (li_pos > 0)
            lv_data = lv_data.For(li_pos - 1) ;

         if ( lv_data.NotIsNull() && md_TKX_cc_exp_date.IsNull() )
         {
            lc_ccExpDate = lv_data.For(4);

            if ( lc_ccExpDate.isAllNumeric() )
            {
               md_TKX_cc_exp_date = lc_ccExpDate;
            }
         }
      }

      // TKX CC EXP DATE END

		else if (lt_segment.At(4).For(5) == "G*PID")
		{
			lv_data = lt_segment.At(10);
         li_pos 	= lv_data.findSubstring("!") ;

         if (li_pos > 0) {
            lv_data = lv_data.For(li_pos - 1) ;
			}

         lv_data.removeTrailingBlanks() ;
			if (lv_data .isAllNumeric())
			{
				md_IRMK_type[mi_IRMK_I]     = "PID" ;
				md_IRMK_remark[mi_IRMK_I]   = lv_data ;
				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}
		}

		// GAP 9090j Start
		else if (lc_delimiter == "IC")
		{
		  	md_IRMK_type[mi_IRMK_I]     = "IC" ;
		  	md_IRMK_remark[mi_IRMK_I]   = " ";
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9090j End
		// GAP 9090K start
		else if (lc_delimiter == "RP")
		{
		  	md_IRMK_type[mi_IRMK_I]     = "NTE" ;
		  	md_IRMK_remark[mi_IRMK_I]   = lt_data;
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9090K end

		// GAP 9090m Start
      else if (lt_segment.At(6).For(3) == "IRP") {
			lv_data = lt_segment.At(9) ;
			li_pos = lv_data.findSubstring("/");
			if (li_pos > 1 && lv_data.For(li_pos-1).isAllNumeric()) {
				li_pos  = lv_data.findSubstring("!") ;
				if (li_pos > 0) {
					lv_data = lv_data.For(li_pos - 1) ;
				}
				lv_data.removeTrailingBlanks() ;

				md_IRMK_type[mi_IRMK_I]     = "IRP" ;
				md_IRMK_remark[mi_IRMK_I]   = lv_data;
				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}
		}
		// GAP 9090m End
		// GAP 9090O Begin
		else if (lc_delimiter.For(1) == "R" && lc_delimiter.At(2).isAllNumeric() )
		{	/* R - receipt remark */
		   AB_Boolean lb_found = AB_false;

				/* search for matching receipt number */
				for ( int li_ctr = 1; li_ctr <= mi_receiptIndex && !lb_found; 
						li_ctr++ )
				{

					if ( lc_delimiter == gd_receiptType[li_ctr] )
					{

						gd_receiptData[li_ctr] = gd_receiptData[li_ctr]
														+ " " + lv_data;
						lb_found = AB_true;
					}
				}

				if ( !lb_found )
				{
					gd_receiptType[++mi_receiptIndex] = lc_delimiter;
					gd_receiptData[mi_receiptIndex]   = lv_data;

				}
		}	/* R - receipt remark */
		// GAP 9090O End

		// GAP 9106 start
		else if (lc_delimiter == "FN")
		{
		  	md_IRMK_type[mi_IRMK_I]     = "FN" ;
		  	md_IRMK_remark[mi_IRMK_I]   = lv_data;
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9106 end

      lt_segment = po_PNR.getNextSegment("A14") ;
   }

	parseXmlUdid();

   setCopIdByIata();

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populate_Iraddress_SectionA13(AB_Varchar &pv_long_address, 
														CPNR &po_PNR)
{
   ENTER;

   int counter, li_pos ;
   AB_Text lt_segment ;

   counter = 0 ;
   
   po_PNR.resetSegment("A13") ;
   lt_segment = po_PNR.getNextSegment("A13") ;

   while (lt_segment.NotIsNull())
   {
      if (lt_segment.At(4).For(2) == "D-")
      {
         pv_long_address = lt_segment.At(6) ;

         li_pos = pv_long_address.findSubstring("!") ;

         if (li_pos > 1)
            pv_long_address = pv_long_address.For(li_pos-1) ;

         return ;
      }
      lt_segment = po_PNR.getNextSegment("A13") ;
   }
   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::setShortRemarks(AB_Text pt_remark)
{
   ENTER; 

   AB_Varchar lv_data(90) ;
   AB_Varchar lv_num(2) ;
   int        li_charPos,
              li_numPos ;

   lv_data    = pt_remark ;
   li_charPos = 0 ;
   li_numPos  = 0 ;

   li_charPos = lv_data.findSubstring("!") ;
   li_numPos  = lv_data.findSubstring("-") ;

   if (li_charPos > 0)
      lv_data = lv_data.For(li_charPos - 1) ;


   if ((li_numPos == 2) || (li_numPos == 3))
   {
      lv_num = lv_data.For(li_numPos - 1) ;

      if (lv_num.findFirstCharNotInList("0123456789") == 0)
      {
         if (  lv_data.At(li_numPos + 1) != " " 		&&
					lv_data.At(li_numPos + 1).NotIsNull()	&&
               lv_num != "0" && lv_num != "00" )
         {
	     if ( createUDIDifRange1to99(lv_num, lv_data.At(li_numPos + 1)) )
	     {
		  mt_UDID = mt_UDID + lv_data.At(li_numPos + 1).Upper();
	     }
         }
      }
   }

   EXIT; 
}

void CTSS::parseXmlUdid() 
{
	AB_Char lc_tag(40);
	AB_Text lt_tag;
	AB_Char lc_content(40);
	int li_end_pos;
	int li_start_pos;
	int li_tag_number;
	int li_IRMK_I;

	li_IRMK_I = mi_IRMK_I;

	li_tag_number = 1;

	LOG(logDEBUG) << endl << "XML >>> mt_UDID = '" << mt_UDID << "'" << endl << flush;

	lt_tag = mt_UDID.getNthBracketedField(li_tag_number, "<", ">");

	while (lt_tag.NotIsNull())
	{
		li_start_pos 	= 0;
		li_end_pos		= 0;

		li_start_pos 	= mt_UDID.findSubstring(("<" + lt_tag + ">").to_string());
		li_end_pos 		= mt_UDID.findSubstring(("</" + lt_tag + ">").to_string());

		LOG(logDEBUG) << endl << "XML >>> lt_tag = '" << lt_tag << "'" << endl << flush;

		if (li_start_pos != 0 && li_end_pos != 0 && li_start_pos < li_end_pos)
		{
			li_start_pos 	= li_start_pos + lt_tag.length() + 2;
			lc_tag 			= lt_tag;

			lc_content 		= mt_UDID.At(li_start_pos).For(li_end_pos - li_start_pos);

			LOG(logDEBUG) << endl << "XML >>> lc_tag = '" << lc_tag << "'" << endl << flush;
			LOG(logDEBUG) << endl << "XML >>> lc_content = '" << lc_content << "'" << endl << flush;

			md_IRMK_type[mi_IRMK_I]   				= "UDX" ;
			md_IRMK_udi_line_number[mi_IRMK_I] 	= AB_null ;
			md_IRMK_remark[mi_IRMK_I]         	= lc_tag + lc_content;
			md_IRMK_pax_id[mi_IRMK_I++]       	= AB_null;
		} 
		else 
		{
			mi_IRMK_I = li_IRMK_I;

			md_IRMK_type[mi_IRMK_I]   				= "UDE" ;
			md_IRMK_udi_line_number[mi_IRMK_I] 	= AB_null ;
			md_IRMK_remark[mi_IRMK_I]         	= "PARSING ERROR";
			md_IRMK_pax_id[mi_IRMK_I++]       	= AB_null;
			break;
		}

		li_tag_number+=2;

		lt_tag = mt_UDID.getNthBracketedField(li_tag_number, "<", ">");

	}
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_IrRentalandOther(CPNR &po_PNR) 
{
    ENTER; 

   int li_counter, li_A16_index ;
   AB_Text lt_section ;
   // SPNR *lm_A16_Section ;
   
   li_counter = 0 ;

   po_PNR.resetSegment("A16") ;
   lt_section = po_PNR.getNextSegment("A16") ;

   while (lt_section.NotIsNull())
   {
      if ((lt_section.At(4).For(1) == "A") || (lt_section.At(4).For(1) == "7"))
         fill_IrRentalHotel(lt_section) ;
      else if ((lt_section.At(4).For(1) == "B") || (lt_section.At(4).For(1) == "8"))
         fill_IrRentalCar(lt_section) ;
      else if (lt_section.At(4).For(1) == "6") 
		{
			if ( lt_section.At(14).For(1) == "B" )
				fill_IrBus(lt_section) ;
			else if ( lt_section.At(14).For(1) == "R" )
				fillIrRail(lt_section) ;
			else
         	fill_IrOther(lt_section) ;
		}

      lt_section = po_PNR.getNextSegment("A16") ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_IrRentalHotel(AB_Text &pt_section)
{
   ENTER; 

   int        li_pos ;
   AB_Varchar lv_field(50) ;
   AB_Date    ld_tempDate ;
	AB_Char    lc_status(2) ;
	AB_Char    lc_year(2) ;

   md_IR_itin_type[mi_IR_I]      = "H" ;
   md_IR_line_number[mi_IR_I]    = pt_section.At(5).For(2) ;
   md_IR_start_date[mi_IR_I]     = pt_section.At(7).For(7) ;

   lv_field = pt_section.getNthBracketedField(1, "/CO-", "!") ;
   if (lv_field.NotIsNull())
      md_IR_com_rate[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/CD-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_corporation_id[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/FG-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_customer_id[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/FT-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_fup[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/G-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_guarantee[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "CF:", "!") ;
   if (lv_field.Length().to_long() > 2)
      md_IR_confirmation[mi_IR_I] = lv_field.At(3) ;

   if (md_IR_guarantee[mi_IR_I].IsNull())
   {
      lv_field = pt_section.getNthBracketedField(1, "/ADV", "/") ;
      if (lv_field.NotIsNull())
         md_IR_guarantee[mi_IR_I] = lv_field ;
   }

   li_pos = pt_section.findSubstring("W-:") ;
   if (li_pos > 0)
      lv_field = pt_section.For(li_pos) ;
   else
      lv_field = pt_section ;

   IrHotelRemarks(lv_field)  ;

   if (pt_section.At(4).For(1) == "A")
   {
      md_IR_source[mi_IR_I]         = "A" ;
      md_IR_location_id[mi_IR_I]    = pt_section.At(14).For(6) ;
      md_IR_sup_id[mi_IR_I]         = pt_section.At(20).For(2) + "H" ;
      md_IR_origin_city_id[mi_IR_I] = pt_section.At(22).For(3) ;
      lc_status                     = pt_section.At(27).For(2) ;
      md_IR_site_name[mi_IR_I]      = pt_section.At(37).For(20) ;
      md_IR_phone_acode[mi_IR_I]    = pt_section.At(57).For(7) ;
      md_IR_phone_no[mi_IR_I]       = pt_section.At(64).For(10) ;
      md_IR_fax_acode[mi_IR_I]      = pt_section.At(74).For(7) ;
      md_IR_fax_no[mi_IR_I]         = pt_section.At(81).For(10) ;
      md_IR_rate_duration[mi_IR_I]  = pt_section.At(91).For(1) ;
		if (md_IR_rate_duration[mi_IR_I] == " ")
			md_IR_rate_duration[mi_IR_I] = "D" ;
      md_IR_unit_ct[mi_IR_I]        = pt_section.At(92).For(1) ;
      md_IR_unit_type[mi_IR_I]      = pt_section.At(93).For(3) ;
      md_IR_rate_type[mi_IR_I]      = pt_section.At(96).For(6) ;
      md_IR_number_of_pax[mi_IR_I]  = pt_section.At(99).For(1) ;
      
		ld_tempDate = md_IR_start_date[mi_IR_I] ;

		if ( pt_section.At(29).For(5) == "     " )
			md_IR_end_date[mi_IR_I] = md_IR_start_date[mi_IR_I] ;
		else
		{
			lc_year = ld_tempDate.get_default_year(pt_section.At(29).For(5)) ;
			lc_year.rightJustify('0') ;

			md_IR_end_date[mi_IR_I] = pt_section.At(29).For(5) + lc_year ;
		}

   }
   else
   {
      md_IR_source[mi_IR_I]         = "M" ;
      md_IR_sup_id[mi_IR_I]         = pt_section.At(14).For(2) + "H" ;
      md_IR_origin_city_id[mi_IR_I] = pt_section.At(16).For(3) ;
      lc_status                     = pt_section.At(20).For(2) ;

		ld_tempDate = md_IR_start_date[mi_IR_I] ;
		if ( pt_section.At(22).For(5) == "     " )
			md_IR_end_date[mi_IR_I] = md_IR_start_date[mi_IR_I] ;
		else
		{
			lc_year =  ld_tempDate.get_default_year(pt_section.At(22).For(5)) ;
			lc_year.rightJustify('0') ;

			md_IR_end_date[mi_IR_I] = pt_section.At(22).For(5) + lc_year ;
		}

      md_IR_site_name[mi_IR_I]      = pt_section.At(30).For(20) ;
      md_IR_unit_ct[mi_IR_I]        = pt_section.At(50).For(1) ;

      lv_field = pt_section.getNthBracketedField(1, "/ITB-", "!") ;
      if (lv_field.NotIsNull())
      {
         md_IR_unit_type[mi_IR_I]      = lv_field.For(3) ;
         md_IR_number_of_pax[mi_IR_I]  = lv_field.At(4).For(1) ; 
      }
		else
		{
         md_IR_unit_type[mi_IR_I]      = "SGLB" ;
         md_IR_number_of_pax[mi_IR_I]  = 1 ;
		}

		// SCR424772 (do the same as in scr 408389) start
      lv_field = pt_section.getNthBracketedField(1,"/PC","/") ;
      if (lv_field.NotIsNull()) {
      	md_IR_location_id[mi_IR_I] = lv_field;
		}
		// SCR424772 (do the same as in scr 408389) end

      lv_field = pt_section.getNthBracketedField(1,"/RR-","/") ;
      if (lv_field.NotIsNull())
      {
         if (lv_field.At(5).For(6) == "WEEKLY")
            md_IR_rate_duration[mi_IR_I] = "W" ;
         else if (lv_field.At(5).For(7) == "WEEKEND")
            md_IR_rate_duration[mi_IR_I] = "X" ;
         else if (lv_field.At(5).For(7) == "SPECIAL")
            md_IR_rate_duration[mi_IR_I] = "S" ;
         else if (lv_field.At(5).For(4) == "DAYS")
            md_IR_rate_duration[mi_IR_I] = "D" ;
      }
		else
			md_IR_rate_duration[mi_IR_I] = "D" ;
   }

   getRateData(pt_section, 
               md_IR_itin_type[mi_IR_I], 
               md_IR_start_date[mi_IR_I],
               md_IR_saving_code[mi_IR_I], 
               md_IR_cur_id[mi_IR_I],
               md_IR_local_rate[mi_IR_I], 
               md_IR_regular_rate[mi_IR_I],
               md_IR_booked_rate[mi_IR_I], 
               mv_IrHotelData, gi_IrHotelIndex) ;

   getAddressData(pt_section) ;

	AB_Varchar       lv_duePaid(3) ;

	li_pos = pt_section.findSubstring("DP:") ;

	if ( li_pos > 0 )
	{
		lv_duePaid = pt_section.At(li_pos + 6).For(3) ;
	}
	else
	{
		lv_duePaid = AB_null ;
	}

   fillIrconsumDetailRental(md_IR_itin_type[mi_IR_I],
                           md_IR_line_number[mi_IR_I],
                           lc_status,
									lv_duePaid) ;


   mi_IR_I++ ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::IrHotelRemarks(AB_Varchar &pv_field)
{
   ENTER; 

   AB_Varchar lv_field(50), lv_remark(50) ;


   lv_field = pv_field.getNthBracketedField(1, "/RA-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "ROLLAWAY ADULT : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/RC-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "ROLLAWAY CHILD : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/CR-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "CRIB CHARGE : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/EX-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "EXTRA ADULT : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/L", "/") ;
   if (lv_field.NotIsNull())
   {
      if (lv_field == "BF")
         lv_field = "BEACHFRONT" ;
      else if (lv_field == "MV")
         lv_field = "MOUTAIN VIEW" ;
      else if (lv_field == "OF")
         lv_field = "OCEAN FRONT" ;
      else if (lv_field == "PS")
         lv_field = "POOL SIDE" ;
      else if (lv_field == "SV")
         lv_field = "SEA VIEW" ;
      else if (lv_field == "BV")
         lv_field = "BAY VIEW" ;
      else if (lv_field == "GV")
         lv_field = "GARDEN VIEW" ;
      else if (lv_field == "LV")
         lv_field = "LAKE VIEW" ;
      else if (lv_field == "RV")
         lv_field = "RIVER VIEW" ;
      else if (lv_field == "CV")
         lv_field = "COURTYARD VIEW" ;
      else if (lv_field == "DO")
         lv_field = "DOWN AND OUT" ;
      else if (lv_field == "GC")
         lv_field = "GOLF COURSE VIEW" ;

      lv_remark = "TOUR ROOM LOCATION : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/M", "/") ;
   if (lv_field.NotIsNull())
   {
      if (lv_field == "AP")
         lv_field = "AMERICAN PLAN" ;
      else if (lv_field == "BP")
         lv_field = "BERMUDA PLAN" ;
      else if (lv_field == "CB")
         lv_field = "CARIBBEAN PLAN" ;
      else if (lv_field == "CP")
         lv_field = "CONTINENTAL PLAN" ;
      else if (lv_field == "FP")
         lv_field = "FAMILY PLAN" ;
      else if (lv_field == "MA")
         lv_field = "MODIFIED AMERICAN PLAN" ;

      lv_remark = "TOUR MEAL PLAN : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/EC-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "EXTRA CHILD : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/SI-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "SPECIAL INFO : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

      // LOG(logDEBUG) << "END CTSS::IrHotelRemarks" << endl ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::CreateRentalRemarks(AB_Varchar &pv_field)
{
   ENTER; 

   md_IRMK_itin_type[mi_IRMK_I]   = md_IR_itin_type[mi_IR_I] ;
   md_IRMK_remark[mi_IRMK_I]         = pv_field ;
   md_IRMK_ren_line_number[mi_IRMK_I] = md_IR_line_number[mi_IR_I] ;
   md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;

   EXIT; 
}
//---------------------------------------------------------------------------------------------------------------------------
void CTSS::getRateData(AB_Text &pt_section, 
							  AB_Char &pc_itin_type, 
							  D_char_date &pt_start_date,
             			  D_saving_code &pd_saving_code, 
							  D_currency &pd_cur_id, 
							  D_char_numeric &pd_local_rate,
             			  D_char_numeric &pd_regular_rate, 
							  D_char_numeric &pd_booked_rate, 
                       V_AB_Text &pd_Data, int pi_Index)
{
   ENTER; 

   D_amount lm_amount ;
   AB_Varchar lt_field(100) ;
	AB_Char    lc_delim(4) ;

   lm_amount = 0.00 ;
   pd_booked_rate = "0" ;
   pd_regular_rate = "0" ;

	lc_delim = "/RT-" ;
   parseRate(pt_section, lc_delim, pd_cur_id, lm_amount, lt_field) ;

   LOG(logDEBUG) << "MSG getRateData: for pc_itin_type: " << pc_itin_type << " pd_cur_id= " << pd_cur_id << " lm_amount= " << lm_amount << endl ;

   if (lt_field.NotIsNull())
   {
      if (lm_amount != AB_Decimal(0.00))
      {
         pd_booked_rate = lm_amount ;
         pd_local_rate = lm_amount ;
         pd_regular_rate = lm_amount ;
      }

      if (pc_itin_type == "C")
         get_car_data(lt_field) ;
   }

	lc_delim = "/RQ-" ;
   parseRate(pt_section, lc_delim, pd_cur_id, lm_amount, lt_field) ;

   if (lt_field.NotIsNull())
   {
      if (lm_amount != AB_Decimal(0.00))
      {
         if ((pd_booked_rate != 0) && (lm_amount > AB_Decimal(pd_booked_rate)))
            pd_regular_rate = lm_amount ;
         else if (pd_booked_rate == 0)
         {
            pd_booked_rate = lm_amount ;
            pd_local_rate = lm_amount ;
         }
      }

      if (pc_itin_type == "C")
         get_car_data(lt_field) ;
   }

	lc_delim = "/RG-" ;
   parseRate(pt_section, lc_delim, pd_cur_id, lm_amount, lt_field) ;
   
   //LOG(logDEBUG) << "pd_cur_id= " << pd_cur_id << endl ;
   //LOG(logDEBUG) << "lm_amount= " << lm_amount << endl ;
   //LOG(logDEBUG) << "lt_field= " << lt_field << endl ;
   //LOG(logDEBUG) << "pt_section= " << pt_section << endl ;

   if (lt_field.NotIsNull())
   {
      if (lm_amount != AB_Decimal(0.00))
      {
         if (pd_booked_rate != 0)
         {
            if (lm_amount > AB_Decimal(pd_regular_rate))
               pd_regular_rate = lm_amount ;
         }
         else
         {
            pd_booked_rate = lm_amount ;
            pd_local_rate = lm_amount ;
         }
      }
      
      //LOG(logDEBUG) << "pd_booked_rate= " << pd_booked_rate << endl ;
      //LOG(logDEBUG) << "pd_local_rate= " << pd_local_rate << endl ;

     if (pc_itin_type == "C")
         get_car_data(lt_field) ;
   }

   getOverrideRates(pc_itin_type, 
               pt_start_date, 
               pd_saving_code, 
               pd_local_rate, 
               pd_regular_rate, 
               pd_booked_rate, 
               pd_Data, 
               pi_Index) ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::parseRate(AB_Text &pt_section, 
							AB_Char &pc_start_delim, 
							D_currency &pd_cur_id, 
							D_amount &pd_amount, 
							AB_Varchar &pt_field)
{
   ENTER; 

   int li_pos ;
   D_char_numeric lm_rate ;
   AB_Char lc_test ;
   AB_Money lm_amount ;
   AB_Varchar lv_rawAmount(15) ;
   AB_BOOL lb_ok ;
   D_amount ld_test ;

   pt_field = pt_section.getNthBracketedField(1, pc_start_delim.to_string(), "!") ;
   
   if (pt_field.NotIsNull())
   {
      lc_test = pt_field.At(1).For(1) ;
      if (lc_test.isAllAlpha())
      {
         pd_cur_id = pt_field.For(3) ;
         lm_rate = pt_field.At(4) ;
      }
      else
         lm_rate = pt_field ;

      li_pos = lm_rate.findFirstCharNotInList("0123456789.") ;

      if (li_pos > 0)
         lm_rate = lm_rate.For(li_pos - 1) ;

      lv_rawAmount = lm_rate ;
      lv_rawAmount.removeLeadingBlanks() ;
      li_pos = lv_rawAmount.findSubstring(".") ;
      if ((li_pos == 0) && (lv_rawAmount != 0) && (lv_rawAmount.NotIsNull()))
      {
			if ( md_IR_itin_type[mi_IR_I] == "H" && md_IR_source[mi_IR_I] == "A" )
			{
         		ld_test = AB_Decimal(lv_rawAmount/AB_Decimal(100.00)) ;
         		lv_rawAmount = ld_test ;
			}
			else
			{
				lv_rawAmount = lv_rawAmount + ".00" ;
			}
      }

      lm_amount = lv_rawAmount.to_money("#########.##", lb_ok) ;
      
      if (!lb_ok)
         pd_amount = "0.00" ;
      else
         pd_amount = lm_amount.to_string() ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::getOverrideRates(AB_Char &pc_itin_type, 
									D_char_date &pt_start_date, 
									D_saving_code &pd_saving_code,
             					D_char_numeric &pd_local_rate, 
									D_char_numeric &pd_regular_rate,
             				   D_char_numeric &pd_booked_rate, 
									V_AB_Text &pd_Data, int pi_Index)
{
   ENTER; 

   int li_counter, li_pos ;
   D_char_numeric  lm_amount ;
   AB_Varchar      lv_data(60) ;
	AB_Char         lc_delim(4) ;
	AB_Varchar      lv_rateData(60) ; 
	AB_Boolean      lb_RCFound ;
	D_char_numeric  ld_RTAmount ;
	D_char_numeric  ld_RCAmount ;

	if (pd_regular_rate == "0")
		pd_regular_rate = AB_null ;

   li_counter = 1 ;
   lb_RCFound = AB_false ;

	LOG(logDEBUG) << "HS INDEX = "<< pi_Index << endl ;

   if (pi_Index == li_counter)
      return ;

   lv_data = AB_null ;

   while (li_counter < pi_Index)
   {
		if ( pc_itin_type == "H" )
		{
      	if (pd_Data[li_counter].For(5) == pt_start_date.For(5))
			{
         	lv_data = pd_Data[li_counter] ;
				break ;
			}
		}
		else
		{
      	if ((pd_Data[li_counter].For(5) == pt_start_date.For(5)) &&
			(pd_Data[li_counter].At(6).For(3) == md_IR_origin_city_id[mi_IR_I] ))
			{
         	lv_data = pd_Data[li_counter] ;
				break ;
			}
		}

      li_counter++ ;
   }

   li_pos = lv_data.findSubstring("/SV") ;

   if (li_pos > 0)
   {
		li_pos++;
      if (lv_data.At(li_pos + 2).For(1) == "-")
         pd_saving_code = lv_data.At(li_pos + 3);
      else
         pd_saving_code = lv_data.At(li_pos + 2);

		li_pos = pd_saving_code.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") ;
		if (li_pos > 0) {
			pd_saving_code = pd_saving_code.For(li_pos-1);
		}
   }

   li_pos = lv_data.findSubstring("RC-") ;

   if (li_pos > 0)
   {
		lb_RCFound = AB_true ;

		if (lv_data.At(li_pos+3).For(3).isAllAlpha() )
		{
			lv_rateData = lv_data.At(li_pos+6) ;

         li_pos = lv_rateData.findFirstCharNotInList("0123456789.") ;

			if ( li_pos > 0 )
				pd_booked_rate = lv_rateData.For(li_pos - 1) ;
			else
				pd_booked_rate = lv_rateData ;

		}
		else
		{
			lv_rateData = lv_data.At( li_pos +3 ) ;
         li_pos = lv_rateData.findFirstCharNotInList("0123456789.") ;

			if ( li_pos > 0 )
				pd_booked_rate = lv_rateData.For(li_pos - 1) ;
			else
				pd_booked_rate = lv_rateData ;

		}

		ld_RCAmount = pd_booked_rate ;
   }

   li_pos = lv_data.findSubstring("RT-") ;

   if (li_pos > 0)
   {
		if (lv_data.At(li_pos+3).For(3).isAllAlpha() )
		{
			md_IR_cur_id[mi_IR_I] = lv_data.At(li_pos+3).For(3) ;

			lv_rateData = lv_data.At(li_pos+6) ;

         li_pos = lv_rateData.findFirstCharNotInList("0123456789.") ;

			if ( li_pos > 0 )
				ld_RTAmount = lv_rateData.For(li_pos - 1) ;
			else
				ld_RTAmount = lv_rateData ;
						
		}
		else
		{
			lv_rateData = lv_data.At( li_pos +3 ) ;
         li_pos = lv_rateData.findFirstCharNotInList("0123456789.") ;

			if ( li_pos > 0 )
				ld_RTAmount = lv_rateData.For(li_pos - 1) ;
			else
				ld_RTAmount = lv_rateData ;

		}

      if ( lb_RCFound )
      {
         pd_local_rate = ld_RTAmount ;
      }
      else
      {
         pd_booked_rate = ld_RTAmount ;
      }

   }
		
	if ( md_IR_cur_id[mi_IR_I].IsNull() )
		md_IR_cur_id[mi_IR_I] = "USD" ;

   li_pos = lv_data.findSubstring("DC-") ;
	if (li_pos > 0) {
		md_IR_discount_code[mi_IR_I] = lv_data.At(li_pos+3).For(1);
	}

   li_pos = lv_data.findSubstring("/RD-") ;
	if (li_pos > 0) {
		lv_rateData = lv_data.At(li_pos+4).For(1);
		if (lv_rateData == "D" ||
			 lv_rateData == "W" ||
			 lv_rateData == "M" ||
			 lv_rateData == "E" ||
			 lv_rateData == "X") {
			md_IR_rate_duration[mi_IR_I]  = lv_rateData;
		}
	}

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::getRateOverride(AB_Varchar &pv_data, 
									int   pi_pos, 
									D_char_numeric &pd_amount)
{
   ENTER; 

   int li_pos ;
   D_amount lm_amount ;
   AB_Money lm_value ;
   AB_Varchar lv_rawAmount(15) ;
   AB_BOOL lb_ok ;
	AB_Varchar lv_data(25) ;

   // li_pos = pv_data.findSubstring("RT");

   if (pi_pos > 0)
   {
      if (pv_data.At(pi_pos + 2).For(1) == "-")
         pi_pos = pi_pos + 3 ;
      else
         pi_pos = pi_pos + 2 ;

      lv_data = pv_data.At(pi_pos) ;

      li_pos = lv_data.findFirstCharNotInList("0123456789.") ;

      if (li_pos > 0)
         lv_data = lv_data.For(li_pos - 1) ;

      lv_rawAmount = lv_data ;
      lv_rawAmount.removeLeadingBlanks() ;
      lm_value = lv_rawAmount.to_money("#######.##", lb_ok) ;
      lm_amount = lm_value.to_string() ;

      pd_amount = D_char_numeric(lm_amount) ;
		 
   }

   EXIT; 
}
//---------------------------------------------------------------------------------------------------------------------------
void CTSS::getAddressData(AB_Text &pt_section)
{
   ENTER; 

   AB_Varchar  lv_field(100) ;
   V_AB_Varchar lv_arrayField(100,100) ;
   int li_pos ;

   lv_field = pt_section.getNthBracketedField(1, "W-:", "!") ;

   if (lv_field.IsNull())
      return ;

   if (md_IR_source[mi_IR_I] == "A")
   {
      lv_field = lv_field.At(3) ;

      li_pos = lv_field.getReverseIndex("/") ;

      if (li_pos > 0)
      {
		// 1706 - 15MAR99 - v1198 - SC --
      //  li_pos = lv_field.length() - li_pos ;

         if (lv_field.At(li_pos + 1) == " ")
            lv_field = lv_field.For(li_pos - 1) ;
      }
   }
   else
   {
      li_pos = lv_field.findSubstring("/") ;

      if (li_pos > 0)
         lv_field = lv_field.At(li_pos + 1) ;

      li_pos = lv_field.findSubstring("/RT-") ;

      if (li_pos > 1)
         lv_field = lv_field.For(li_pos - 1) ;

		int li_count ;
      li_count = lv_arrayField.parseStringMultDel(lv_field,"/") ;

		if ( lv_arrayField[li_count].For(1).isAllNumeric())
		{
   		md_IR_phone_acode[mi_IR_I] = lv_arrayField[li_count].For(5);
   		md_IR_phone_no[mi_IR_I] = lv_arrayField[li_count].At(6);
			li_count = li_count -1 ;
			while ( li_count < 0 )
			{
				if (lv_field.IsNull())
					lv_field =  lv_arrayField[li_count] ;
				else
					lv_field = lv_arrayField[li_count] + "/" + lv_field ;
				li_count -- ;
			}
		}

		/*
      if (li_pos > 0)
      {
         li_pos = lv_field.getReverseIndex("/") ;

         li_pos = lv_field.length() - li_pos ;
         lv_field = lv_field.For(li_pos - 1) ;

      }
		*/
   }
   CAddress lo_address ;

	lo_address.setFourLineAddr(lv_field);
   lo_address.getAddress(md_IR_addr1[mi_IR_I], 
               			 md_IR_addr2[mi_IR_I],
               			 md_IR_addr3[mi_IR_I], 
               			 md_IR_addr4[mi_IR_I]) ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_IrRentalCar(AB_Text &pt_section)
{
   ENTER;

   int 			li_pos ;
   AB_Varchar 	lv_field(50) ;
   D_amount 	lm_amount ;
   D_currency 	lm_dummy ;
   AB_Date 		ld_tempDate ;
	AB_Char		lc_status(2) ;
	AB_Char		lc_year(2) ;

   md_IR_itin_type[mi_IR_I]      = "C" ;
   md_IR_free_mileage[mi_IR_I] = AB_null ;

   if (pt_section.At(4).For(1) == "B")
      md_IR_source[mi_IR_I] = "A" ;
   else
      md_IR_source[mi_IR_I] = "M" ;

   md_IR_line_number[mi_IR_I] = pt_section.At(5).For(2) ;
   md_IR_start_date[mi_IR_I] = pt_section.At(7).For(7) ;

	ld_tempDate = md_IR_start_date[mi_IR_I] ;

	if ( pt_section.At(28).For(5) == "     " )
		md_IR_end_date[mi_IR_I] = md_IR_start_date[mi_IR_I] ;
	else
	{
		lc_year =  ld_tempDate.get_default_year((pt_section.At(28).For(5))) ;
		lc_year.rightJustify('0') ;
		md_IR_end_date[mi_IR_I] = pt_section.At(28).For(5) + lc_year ;
	}

   md_IR_service_status[mi_IR_I] = pt_section.At(26).For(2) ;
   md_IR_origin_city_id[mi_IR_I] = pt_section.At(36).For(4) ;
   md_IR_sup_id[mi_IR_I] = pt_section.At(40).For(2) + "C" ;
   md_IR_unit_type[mi_IR_I] = pt_section.At(42).For(4) ;
   md_IR_unit_ct[mi_IR_I] = pt_section.At(46).For(1) ;
   md_IR_location[mi_IR_I] = pt_section.At(47).For(26) ;
   md_IR_dest_city_id[mi_IR_I] = pt_section.At(73).For(3) ;
   md_IR_phone_acode[mi_IR_I] = pt_section.At(99).For(5) ;
   md_IR_phone_no[mi_IR_I] = pt_section.At(104).For(15) ;

   if (md_IR_dest_city_id[mi_IR_I] == " ")
      md_IR_dest_city_id[mi_IR_I] = AB_null ;

	/*
		Time format not good
   lv_field = pt_section.getNthBracketedField(1, "ARR-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_start_time[mi_IR_I] = CheckTime(lv_field);
	*/

   lv_field = pt_section.getNthBracketedField(1, "/G-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_guarantee[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/CD-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_corporation_id[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/ID-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_customer_id[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/FT-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_fup[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/DO-", "/") ;
   if (lv_field.NotIsNull()) {
		md_IR_dropoff_location[mi_IR_I] = lv_field;
      md_IR_dest_city_id[mi_IR_I] = lv_field ;
	}

   lv_field = pt_section.getNthBracketedField(1, "PUP-", "/") ;
   if (lv_field.NotIsNull()) {
      md_IR_pickup_location[mi_IR_I] = lv_field;
	}

	/*
		Time format not good
   lv_field = pt_section.getNthBracketedField(1, "/DT-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_end_time[mi_IR_I] = CheckTime(lv_field) ;
	*/

   lv_field = pt_section.getNthBracketedField(1, "CF:", "!") ;
   if (lv_field.Length().to_long() > 2)
      md_IR_confirmation[mi_IR_I] = lv_field.At(3) ;

   lv_field = pt_section.getNthBracketedField(1, "/RC-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_rate_code[mi_IR_I] = lv_field ;

	AB_Char lv_delim(4) ;
	lv_delim = "/DC-" ;
   parseRate(pt_section, lv_delim, lm_dummy, lm_amount, lv_field) ;
   md_IR_dropoff_charge[mi_IR_I] = D_char_numeric(lm_amount) ;

   IrCarRemarks(lv_field) ;

   getAddressData(pt_section) ;

   getRateData(pt_section, 
          md_IR_itin_type[mi_IR_I], 
          md_IR_start_date[mi_IR_I],
          md_IR_saving_code[mi_IR_I], 
          md_IR_cur_id[mi_IR_I],
          md_IR_local_rate[mi_IR_I], 
          md_IR_regular_rate[mi_IR_I],
          md_IR_booked_rate[mi_IR_I], 
          mv_IrCarData, mi_IrCarIndex) ;

	AB_Varchar	lv_duePaid(3) ;

	li_pos = pt_section.findSubstring("DP:") ;

	if ( li_pos > 0 )
	{
		lv_duePaid = pt_section.At(li_pos + 6).For(3) ;
	}
	else
	{
		lv_duePaid = AB_null ;
	}

   fillIrconsumDetailRental(md_IR_itin_type[mi_IR_I],
                           md_IR_line_number[mi_IR_I],
                           lc_status,
									lv_duePaid) ;


   mi_IR_I++ ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::get_car_data(AB_Varchar &pv_field)
{
   ENTER; 

   int li_pos, li_pos2 ;
   AB_Char lc_duration(2), lc_temp_free_milage(25) ;
                                        //1613-SC-13FEB99
	AB_Varchar lc_temp(255);             // 1706 - 15MAR99 - v1198 - SC --


   if (md_IR_free_mileage[mi_IR_I].IsNull())
   {
      li_pos = pv_field.findSubstring("UNL");

      LOG(logDEBUG) << "MSG get_car_data: li_pos= " << li_pos << endl ;

      if (li_pos > 0)
         md_IR_free_mileage[mi_IR_I] = "UNLIM" ;
   }

   li_pos = pv_field.findFirstCharNotInList("0123456789.") ;

   if (li_pos == 1)
   {
      lc_temp = pv_field.At(4) ;
      li_pos = lc_temp.findFirstCharNotInList("0123456789.") ;
   }
   else
      return ;

   if (li_pos > 0)
   {
      if (md_IR_rate_duration[mi_IR_I].IsNull())
      {
         lc_duration = lc_temp.At(li_pos).For(2) ;

         if ((lc_duration == "WK") || (lc_duration == "WY"))
         {
            md_IR_rate_duration[mi_IR_I] = "W" ;
            lc_temp = lc_temp.At(li_pos + 3) ;
         }
         else if ((lc_duration == "WE") || (lc_duration == "WD"))
         {
            md_IR_rate_duration[mi_IR_I] = "E" ;
            lc_temp = lc_temp.At(li_pos + 3) ;
         }
         else if ((lc_duration == "MO") || (lc_duration == "MY"))
         {
            md_IR_rate_duration[mi_IR_I] = "M" ;
            lc_temp = lc_temp.At(li_pos + 3) ;
         }
         else if (lc_duration.For(1) == "D")
         {
            md_IR_rate_duration[mi_IR_I] = "D" ;
            lc_temp = lc_temp.At(li_pos + 3) ;
         }
			else
			{
				md_IR_rate_duration[mi_IR_I] = "X" ;
				lc_temp = lc_temp.At(li_pos + 3) ;
			}
      }
   }

   li_pos = lc_temp.findSubstring("KM") ;

   if (li_pos > 0)
      md_IR_free_mile_code[mi_IR_I] = "K" ;
   else
   {
      li_pos = lc_temp.findSubstring("MI") ;
      if (li_pos > 0)
         md_IR_free_mile_code[mi_IR_I] = "M" ;
   }

   if (li_pos > 0)
   {
      if (md_IR_free_mileage[mi_IR_I].IsNull())
      {
         // 1706 - 19MAR99 - v1198 - SC --
			li_pos2 = lc_temp.For(li_pos-1).findFirstCharNotInList("0123456789. ");

         if(li_pos2 ==0)
         {
            // .30 MI 150 FM
            md_IR_ex_mile_charge[mi_IR_I] = lc_temp.For(li_pos - 1) ;
         }
         else
         {
            // 150 FM .30 MI
            if (lc_temp.At(li_pos2+2).For(li_pos-li_pos2-2).findFirstCharNotInList("0123456789. ") == 0 )
               md_IR_ex_mile_charge[mi_IR_I] = lc_temp.At(li_pos2+2).For(li_pos - li_pos2-2) ;
         }


         if (md_IR_free_mile_code[mi_IR_I] == "K")
         {
            li_pos = lc_temp.findSubstring("FK") ;
            if (li_pos > 0)
				{
					lc_temp_free_milage = lc_temp.For(li_pos - 1) ;
					li_pos2 = lc_temp_free_milage.findSubstring("KM") ;
					if ( li_pos2 > 0 )
						md_IR_free_mileage[mi_IR_I] = lc_temp_free_milage.At(li_pos2+2) ;
               else
						md_IR_free_mileage[mi_IR_I] = lc_temp_free_milage ;
				}
         }
         else if (md_IR_free_mile_code[mi_IR_I] == "M")
         {
            li_pos = lc_temp.findSubstring("FM") ;
            if (li_pos > 0)
				{
               lc_temp_free_milage = lc_temp.For(li_pos - 1) ;
               li_pos2 = lc_temp_free_milage.findSubstring("MI") ;
               if ( li_pos2 > 0 )
                  md_IR_free_mileage[mi_IR_I] = lc_temp_free_milage.At(li_pos2+2
) ;
               else
                  md_IR_free_mileage[mi_IR_I] = lc_temp_free_milage ;
				}
         }

			/*
			if ( !li_pos > 0)
			{
         	md_IR_ex_mile_charge[mi_IR_I] = lc_temp.For(li_pos - 1) ;
         	lc_temp = lc_temp.At(li_pos + 2) ;
			}
			*/
      }

      if (md_IR_ex_day_charge[mi_IR_I].IsNull())
      {
         li_pos = lc_temp.findSubstring("XD") ;
         if (li_pos > 0)
         {
            lc_temp = lc_temp.At(li_pos + 2) ;
            li_pos = lc_temp.findFirstCharNotInList("0123456789.") ;
            md_IR_ex_day_charge[mi_IR_I] = lc_temp.For(li_pos - 1) ;
         }
      }
   }
   else
   {
      li_pos = lc_temp.findSubstring("FK") ;
      if (li_pos == 0)
         li_pos = lc_temp.findSubstring("FM") ;

      if (li_pos > 0)
      {
         md_IR_free_mile_code[mi_IR_I] = lc_temp.At(li_pos + 1).For(1) ;

         if ( md_IR_free_mileage[mi_IR_I].IsNull())
         {
            li_pos2 = lc_temp.For(li_pos - 1).getReverseIndex(" ") ;
            if (li_pos2 > 0)
               md_IR_free_mileage[mi_IR_I] = lc_temp.At(li_pos2 + 1).For(li_pos - (li_pos2 + 1)) ;
         }
      }

      if (md_IR_ex_day_charge[mi_IR_I].IsNull())
      {
		/* 1706 - 15MAR99 - v1198 - SC -- this is for xx ###.##XD instead of
															 XD###.## (see specs for A16)
         li_pos = lc_temp.findSubstring("XD") ;
         if (li_pos > 0)
         {
            li_pos2 = lc_temp.For(li_pos - 1).getReverseIndex(" ") ;
            if (li_pos2 > 0)
               md_IR_ex_day_charge[mi_IR_I] = lc_temp.At(li_pos2 + 1).For(li_pos - (li_pos2 + 1)) ;
         }
       */
         if (li_pos > 0)
         {
            lc_temp = lc_temp.At(li_pos + 2) ;
            li_pos = lc_temp.findFirstCharNotInList("0123456789.") ;
            md_IR_ex_day_charge[mi_IR_I] = lc_temp.For(li_pos - 1) ;
         }
      }
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::IrCarRemarks(AB_Varchar &pv_field)
{
   ENTER; 

   AB_Varchar lv_field(50), lv_remark(50) ;

   lv_field = pv_field.getNthBracketedField(1, "/PR-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "PREPAYMENT INFO : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/SQ-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "SPECIAL EQUIPMENT : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   lv_field = pv_field.getNthBracketedField(1, "/SI-", "/") ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "SPECIAL INFO : " + lv_field ;
      CreateRentalRemarks(lv_field) ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
/*
void CTSS::fill_IrOther(AB_Text &pt_section)
{
   ENTER; 

   // int li_field_length ;
   AB_Varchar lv_field(50) ;

   if (gm_sup_id.IsNull())
      gm_sup_id = "T4R" ;

   md_IO_itin_type[mi_IO_I] = "T" ;
   md_IO_start_date[mi_IO_I] = pt_section.At(7).For(7) ;
   md_IO_dest_city_id[mi_IO_I] = pt_section.At(49).For(4) ;
   md_IO_unit_type[mi_IO_I] = "TOUR" ;
   md_IO_unit_ct[mi_IO_I] = pt_section.At(63).For(1) ;
   md_IO_line_number[mi_IO_I] =  mi_IO_I + 1 ;
   md_IO_sup_id[mi_IO_I] = gm_sup_id ;
   md_IO_orig_city_id[mi_IO_I] = md_IO_dest_city_id[mi_IO_I];
   
   if (pt_section.At(55).For(5) == "     ")
   {
      LOG(logDEBUG) << "Bmd_IO_end_date = NULL" << endl ;
      md_IO_end_date[mi_IO_I] = AB_null ;
   }
   else
   {
      AB_Date ld_tempDate ;

      ld_tempDate = pt_section.At(55).For(5) + md_IO_start_date[mi_IO_I].At(6);
      md_IO_end_date[mi_IO_I] = pt_section.At(35).For(5) + ld_tempDate.get_default_year(md_IO_start_date[mi_IO_I]) ;
      LOG(logDEBUG) << "A)md_IO_end_date= " << md_IO_end_date[mi_IO_I] << endl ;
   }

   lv_field = pt_section.getNthBracketedField(1, "CF:", "!");

   if (lv_field.Length().to_long() > 2)
      md_IO_reference[mi_IO_I] = lv_field.At(3) ;
   else
      md_IO_reference[mi_IO_I] = AB_null ;

   if (md_IO_unit_ct[mi_IO_I] == " ")
      md_IO_unit_ct[mi_IO_I] = 1 ;

   processOtherRemark(pt_section) ;

   mi_IO_I++ ;

   EXIT; 
}
*/

//-------------------------------------------------------------------------------------------------------------------
void CTSS::processOtherRemark(AB_Text &pt_section)
{
   AB_Text lt_remark;
   int li_rmk_counter, li_start, li_end ;
   

   lt_remark = AB_null ;
   li_rmk_counter = 0 ;

   li_start = pt_section.findSubstring("FF:") ;
   pt_section = pt_section.At(li_start+6) ; 
   li_end   = pt_section.findSubstring("!");

   while (li_start > 0 && li_end > 0)
   {
      if (lt_remark.IsNull())
      {
         lt_remark = pt_section.For(li_end - 1) ;
         lt_remark.removeTrailingBlanks() ;
      }
      else
      {
         lt_remark = lt_remark + "/" + pt_section.For(li_end - 1) ;
         lt_remark.removeTrailingBlanks() ;

         md_IRMK_itin_type[mi_IRMK_I]   = "T" ;
         md_IRMK_oth_line_number[mi_IRMK_I] = md_IO_line_number[mi_IO_I] ;
         md_IRMK_remark[mi_IRMK_I]         = lt_remark;
         md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;
         lt_remark = AB_null ;
      }

      pt_section = pt_section.At(li_end + 1) ;

      li_start = pt_section.findSubstring("FF:") ;
      pt_section = pt_section.At(li_start+6) ;
      li_end   = pt_section.findSubstring("!") ;
 
   }

   if (lt_remark.NotIsNull())
   {
      md_IRMK_itin_type[mi_IRMK_I]   = "T" ;
      md_IRMK_oth_line_number[mi_IRMK_I] = md_IO_line_number[mi_IO_I] ;
      md_IRMK_remark[mi_IRMK_I]         = lt_remark;
      md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::updateLLData()
{
   ENTER; 

   int li_counter ;
   D_char_date lm_date ;
   D_city lm_city_id ;
   
   lm_date = AB_null ; 
   li_counter = 0 ;

   if (md_IT_itin_type[mi_IT_I - 1] == "A")
      lm_date = md_IT_start_date[mi_IT_I - 1] ;
 
   if (md_IT_itin_type[0] == "A")
      lm_city_id = md_IT_orig_city_id[0] ;

   md_ID_form[mi_ID_I-1]        = AB_null;
   md_ID_ticket_no[mi_ID_I-1]   = AB_null;
   md_ID_no_of_books[mi_ID_I-1] = AB_null;

   while (li_counter < mi_IO_I)
   {
      if (lm_city_id.NotIsNull()) 
         md_IO_orig_city_id[li_counter] = lm_city_id ;

      if (md_IO_end_date[li_counter].IsNull())
      {
         if (lm_date.IsNull())
         {
            md_IO_end_date[li_counter] = md_IO_start_date[li_counter] ;
         }
         else
         {
            md_IO_end_date[li_counter] = lm_date ;
         }
      }
      li_counter++ ;
   }

   if (md_IO_reference[0].NotIsNull())
   {
      li_counter = 0 ;
      while (li_counter < mi_IT_I)
      {
         md_IT_reference[li_counter++] = md_IO_reference[0] ;
      }
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_IrPasDocument(CPNR &po_PNR)
{
   ENTER; 

   int                li_counter, li_pax_id, li_pos, li_taxCounter ;
   AB_Text            lt_section ;
   int                li_A02_index ;
   AB_Varchar         lv_longTicket(255) ;
	V_AB_Varchar       lva_arrayField(100,100) ;

	D_passenger_no		 ld_paxId ;
   AB_Boolean         lb_pickTkt ;
   AB_Boolean         lb_commOverride ;
	AB_Varchar         lv_commOverride(10);
	AB_Char            lc_docLine(2) ;

   li_pax_id = 0 ;
   li_counter = 0 ;
   li_taxCounter = 0 ;

   li_counter = 0 ;

	mi_tktCounter = 1 ;
   
   li_counter = 0 ;
   po_PNR.resetSegment("A02") ;
   lt_section = po_PNR.getNextSegment("A02");

   while (lt_section.NotIsNull()) 
   {
      li_pax_id++ ;
		lb_pickTkt      = AB_true ;
		lb_commOverride = AB_false ;
		mv_NonArcCC     = AB_null ;
   
      md_IP_pax_id[mi_IP_I]      = li_pax_id ;
      md_ID_line_number[mi_ID_I] = mi_tktCounter++ ;

		lc_docLine =  md_ID_line_number[mi_ID_I] ;
		lc_docLine.rightJustify('0');

      md_ID_pax_id[mi_ID_I]      = li_pax_id ;

		if ( lt_section.At(49).For(10) != "          " )
		{
      	md_ID_form[mi_ID_I]        = lt_section.At(49).For(4) ;
      	md_ID_ticket_no[mi_ID_I]   = lt_section.At(53).For(6) ;
      	md_ID_no_of_books[mi_ID_I] = lt_section.At(59).For(2) ;
			md_ID_doc_type[mi_ID_I] = "BSP" ;
			md_IRP_form_of_pay[mi_IRP_I] = md_form_of_pay ;
		}
		else
		{
      	md_ID_form[mi_ID_I]        = AB_null ;
      	md_ID_ticket_no[mi_ID_I]   = AB_null ;
      	md_ID_no_of_books[mi_ID_I] = AB_null ;
		}

      md_IP_crs_invoice[mi_IP_I] = lt_section.At(61).For(9) ;
		md_ID_reference[mi_ID_I]   = md_IRT_reference ;
		md_ID_sup_id[mi_ID_I]      = md_supId ;
		md_ID_sel_access[mi_ID_I]  = md_selAccess ;
		md_ID_personal_crs[mi_ID_I] = md_personalCrs ;
		md_ID_crs_tour[mi_ID_I]     = md_crsTour ;
		md_ID_e_ticket[mi_ID_I]     = md_e_ticket ;
		md_ID_stp_home_city[mi_ID_I] = md_stpHomeCity ;
		md_ID_stp_host_city[mi_ID_I] = md_stpHostCity ;
		md_ID_saving_code[mi_ID_I] = md_savingCode ;
		md_ID_full_fare[mi_ID_I] = md_full_fare ;
		md_ID_low_prac_fare[mi_ID_I] = md_low_prac_fare ;
		md_ID_rebate[mi_ID_I] = md_rebate;

      li_pos = lt_section.findSubstring("NR:") ;
      if (li_pos > 2)
         md_IP_reference[mi_IP_I] = lt_section.At(li_pos + 3).For(33) ;
      
      li_pos = lt_section.findSubstring("G:") ;
      if (li_pos > 2)
         md_IP_sex[mi_IP_I] = lt_section.At(li_pos + 2).For(1) ;

      li_pos = lt_section.findSubstring("S:") ;
      if (li_pos > 2)
         md_IP_smoking[mi_IP_I] = lt_section.At(li_pos + 2).For(1) ;

      li_pos = lt_section.findSubstring("C:") ;
      if (li_pos > 2)
         md_IP_ctry_id[mi_IP_I] = lt_section.At(li_pos + 2).For(2) ;

      li_pos = lt_section.findSubstring("P:") ;
      if (li_pos > 2)
         md_IP_passport_no[mi_IP_I] = lt_section.At(li_pos + 2).For(33) ;

      li_pos = lt_section.findSubstring("D:") ;
      if (li_pos > 2)
         md_IP_passport_exp[mi_IP_I] = lt_section.At(li_pos + 2).For(7) ;

		AB_Text  lv_pic ;
		lv_pic = lt_section.At(70).For(6) ;
      md_ID_pic[mi_ID_I] = getPicCode(lv_pic) ;
		AB_Text lv_paxName ;
		lv_paxName = lt_section.At(4).For(33) ;

		li_pos = lv_paxName.findSubstring("/");
		if (li_pos>0)
		{
		   int li_temp;
	      li_temp=lv_paxName.At(li_pos+1).findSubstring("/");
			if (li_temp>0)
		      lv_paxName = lv_paxName.For(li_pos+li_temp-1);
      }
				
      processPassengerName(lv_paxName) ;
      
      if (! CMsgHandler::getMsgStatus())
         return ;

      lv_longTicket = AB_null ;
		lb_commOverride = AB_false ;

		if ( processFare(po_PNR, lt_section, li_pax_id,lc_docLine) )
		{
			mb_foundFare = AB_true ;

			if ( lt_section.At(49).For(10) == "          " )
			{
      		if (li_pax_id < mi_manualTicketIndex)
      		{
         		lv_longTicket = mv_manualTicketData[li_pax_id] ;
	
         		if (lv_longTicket.NotIsNull())
					{
						li_pos = lv_longTicket.findSubstring("/M") ;
		
						if ( li_pos == 0 )
						{
            			md_ID_form[mi_ID_I]        = lv_longTicket.For(4) ;
      					md_ID_ticket_no[mi_ID_I]   = lv_longTicket.At(5) ;
							md_ID_doc_type[mi_ID_I]    = "BSP" ;

                     //Following if not working if variable is null !!!
                     //if ( md_ID_exchange_code[mi_ID_I] != "E" )
                     //{
                       md_IRP_form_of_pay[mi_IRP_I] = md_form_of_pay ;
                     //}

      					md_ID_no_of_books[mi_ID_I] = AB_Integer(1) ;
							fill_Irpayment(mt_A07_segment,
						  						md_ID_pax_id[mi_ID_I],
						  						md_ID_line_number[mi_ID_I]);

                     if ( md_ID_exchange_code[mi_ID_I] == "E" &&
								  md_A11FOP == "EX" )
                     {

                       /**
                        * Override form of payment by A10FOP when ticket is
								* even exchange and A11FOP is EX
                        **/

                       md_IRP_form_of_pay[mi_IRP_I-1] = md_orig_form_of_pay;
                       md_IRP_cc_id[mi_IRP_I-1]       = md_orig_cc_id;
                       md_IRP_cc_no[mi_IRP_I-1]       = md_orig_cc_no;
                       md_IRP_cc_exp_date[mi_IRP_I-1] = AB_null;
                       md_IRP_auth_no[mi_IRP_I-1]     = AB_null;
                       md_IRP_auth_source[mi_IRP_I-1] = AB_null;
                     }
						}
						else
						{
							// Parse string
		
							int i = lva_arrayField.parseStringMultDel(lv_longTicket,"/") ;
	
							if (i > 2 )
							{
								if ( lva_arrayField[3].For(1) == "N" )
								{
									lb_pickTkt = AB_true ;
									mv_NonArcCC = lva_arrayField[4] ;
									lb_commOverride = AB_false ;
		
								}
								else
								{
									if ( lva_arrayField[3].For(1) == "P" )
										lb_commOverride = AB_true ;
									else if (lva_arrayField[3].For(1).isAllNumeric() )
										lb_commOverride = AB_true ;
									
		
									lb_pickTkt = AB_true ;
		
	
									if ( lva_arrayField[4].For(1) == "N" && lva_arrayField[4].At(2).For(1).isAllNumeric())
									{
										mv_NonArcCC = lva_arrayField[5] ;
									}
									else
										mv_NonArcCC = lva_arrayField[4] ;
								}
							}
					 		LOG(logDEBUG) << "NON ARC CREDIT CARD" << mv_NonArcCC << endl ;
		
							if ( lb_pickTkt )
							{
								// Non arc ticket segment
		
								md_ID_sup_id[mi_ID_I]  = lva_arrayField[2].At(5).For(3) ;
								md_ID_doc_type[mi_ID_I]   = "TKT" ;
            				md_ID_form[mi_ID_I]        = AB_null ;
      						md_ID_ticket_no[mi_ID_I]   = lva_arrayField[1].For(10) ;
      						md_ID_no_of_books[mi_ID_I] = AB_Integer(1) ;
		
								if ( lva_arrayField[2].At(2).For(2) == "CA" )
									md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
								else if ( lva_arrayField[2].At(2).For(2) == "AR" )
									md_IRP_form_of_pay[mi_IRP_I] = "CK" ;
								else if ( lva_arrayField[2].At(2).For(2) != "CC")
									md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
								else
									md_IRP_form_of_pay[mi_IRP_I] = "CC" ;

								fill_Irpayment(mt_A07_segment,
						  						md_ID_pax_id[mi_ID_I],
						  						md_ID_line_number[mi_ID_I]);
		
							}
		
						}
					}
      		}
			}

		}


		// Override commission for Non Arc

		if ( lb_commOverride )
		{
			lva_arrayField[3].leftJustify() ;

			md_ID_commission[mi_ID_I]  = lva_arrayField[3] ;

			li_pos       = md_ID_commission[mi_ID_I].findSubstring("P") ;

			if (li_pos > 0)
			{
				md_ID_comm_rate[mi_ID_I]   = 
									md_ID_commission[mi_ID_I].At(li_pos + 1 ) ;
				md_ID_commission[mi_ID_I]  = AB_null ;
			}
			else
			{
				md_ID_commission[mi_ID_I].leftJustify() ;
				md_ID_comm_rate[mi_ID_I] = AB_null ;
			}
		}


		if (md_ID_ticket_no[mi_ID_I].IsNull() && md_ID_base_cost[mi_ID_I].IsNull())
			mi_tktCounter-- ;
		else
			mi_ID_I++ ;
			

		AB_Text   lv_freq ;
		lv_freq = lt_section.At(4).For(33) ;
    
      processFrequentFlyer(po_PNR, lv_freq, li_pax_id) ;

      mi_IP_I++ ;

      lt_section = po_PNR.getNextSegment("A02");
   }

	if ( md_ITD_tax_id[mi_ITD_I].NotIsNull() )
		mi_ITD_I++ ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
D_pic CTSS::getPicCode(AB_Text &pc_pic)
{
   ENTER; 

   if (pc_pic.For(2)=="DF")
      return "DFD";
   else if (pc_pic.For(2)=="DP")
      return "DFP";
   else if (pc_pic.For(2)=="YS")
      return "YSB";
   else if (pc_pic.For(2)=="YC")
      return "YTH";
   else if (pc_pic.For(2)=="SC")
      return "SCR";
   else if (pc_pic.For(2)=="YM")
      return "MIL";
   else if (pc_pic.For(2)=="AC")
      return "ADC";
   else if (pc_pic.For(1)=="I")
      return "INF";
   else if (pc_pic.For(3)=="GOV")
      return "GTR";
   else if ((pc_pic.For(2)=="SD")&&(pc_pic.At(5).For(1)=="C"))
      return "SCD";
   else if (pc_pic.For(2)=="SD")
      return "SCR";
   else if ((pc_pic.For(2)=="CD")&&(pc_pic.At(5).For(1)=="C"))
      return "SCD";
   else if (pc_pic.For(2)=="CD")
      return "SCR";
   else if (pc_pic.For(1)=="C")
      return "CHD";
   else if ((pc_pic.For(5)=="FPC01")||(pc_pic.For(5)=="FPC02"))
      return "F02";
   else if (pc_pic.For(3)=="FPC")
      return "F"+pc_pic.At(4).For(2);
   else if (pc_pic.For(2)=="FP")
   {
      if (pc_pic.At(3).For(1).isAllNumeric()) 
         return "ACC";
      else
         return "FDT";
   }
   else
      return pc_pic.For(3);

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CTSS::processFare(	CPNR     &po_PNR, 
								      AB_Text  &pt_section, 
								      int      &pi_pax_id,
								      AB_Char  &pc_docLine) 
{
   ENTER; 

   AB_Text        lt_section ;
   int            li_counter ;
   AB_Char        lc_fare_num(2), lc_exch_num(2), lc_exch_type(2) ;
   V_AB_Varchar   lva_exchTax(15,100) ;
   V_AB_Varchar   lva_exchTaxId(2,100) ;
   AB_Char        lm_exchange_code ;
   D_char_numeric lm_commission, lm_comm_rate, 
						lm_exchBaseCost, lm_exchCommission ;
   D_amount       li_tempCalculation ;
	AB_Text        lt_tax ;
   D_amount       ld_totTax ;
	D_amount       ld_totCalcFare ;
	D_amount       ld_totA07Fare ;
	D_amount       ld_diffInTax ;


   lc_fare_num = pt_section.At(76).For(2) ;
   lc_exch_num = pt_section.At(78).For(2) ;

   if (lc_fare_num != "  ")
   {
      li_counter = 0 ;
      po_PNR.resetSegment("HDR") ;
      lt_section = po_PNR.getNextSegment("HDR") ;

      li_tempCalculation = lt_section.At(215).For(8) ;
      lm_exchange_code = lt_section.At(249).For(1) ;

      if (li_tempCalculation.to_double() != 0.00)
         lm_commission = D_char_numeric(li_tempCalculation.to_double()/100.00) ;
      else 
         //lm_commission = lt_section.At(215).For(8) ;
           lm_commission = li_tempCalculation ;
      
      li_tempCalculation =  lt_section.At(223).For(4) ;

      if (li_tempCalculation.to_double() != 0.00)
         lm_comm_rate = D_char_numeric(li_tempCalculation.to_double()/100.00) ;
      else
         //lm_comm_rate = lt_section.At(223).For(4) ;
         lm_comm_rate = li_tempCalculation ;
      
      lt_section = AB_null ;
      lt_section = po_PNR.getTotalSection("A07", lc_fare_num) ;
      
      if (lt_section.length() == 0 )
			lt_section = AB_null ;

      if (lt_section.IsNull())
         return(AB_false) ;

		mt_A07_segment = lt_section ;
		if (md_ID_ticket_no[mi_ID_I].NotIsNull())
		{
			fill_Irpayment(lt_section,
						  md_ID_pax_id[mi_ID_I],
						  md_ID_line_number[mi_ID_I]);
	  }
 
      md_ID_comm_rate[mi_ID_I]  = lm_comm_rate ;
      md_ID_commission[mi_ID_I] = lm_commission ;

      if (lt_section.At(36).For(3) == " ")
      {
         md_ID_base_cost[mi_ID_I] = lt_section.At(9).For(12) ;
         md_ID_cur_id[mi_ID_I]    = lt_section.At(6).For(3) ;
      }
      else
      {
         md_ID_base_cost[mi_ID_I] = lt_section.At(39).For(12) ;
         md_ID_cur_id[mi_ID_I]    = lt_section.At(36).For(3) ;
      }
      if (lt_section.At(51).For(3) == "NR:")
      {
			lt_tax = lt_section.At(62) ;
         processTax(lt_tax,ld_totTax) ; 
      }
      else
      {
			lt_tax = lt_section.At(51) ;
         processTax(lt_tax,ld_totTax) ;
      }   

		// Calc tot fare for APO - Matrix Calculated
		ld_totCalcFare = ld_totTax + D_amount(md_ID_base_cost[mi_ID_I]) ;

		// Get Interface tot Fare

		ld_totA07Fare = D_amount(lt_section.At(24).For(12)) ;

		if ( ld_totCalcFare != ld_totA07Fare )
		{
			ld_diffInTax = ld_totA07Fare - ld_totCalcFare ;

			if ( ld_diffInTax < D_amount(0) )
			{
				CMsgHandler::setMsg(3461, " ", "TKT-PARSE", 1, AB_Varchar(20,AB_null));
				return(AB_false) ;
			}
			else
			{
				// Add it as ZZ tax
				md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
				md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
				md_ITD_tax_id[mi_ITD_I]       = "ZZ" ;
				md_ITD_tax_amount[mi_ITD_I++]   = ld_diffInTax ;
				ld_totTax = ld_totTax + ld_diffInTax ;
			}

		}

      if (lc_exch_num != " ")
      {
			// Populate A10 segment
			processOrigFare(po_PNR, 
								lc_exch_num, 
								pc_docLine,
								lc_exch_type) ;

			md_ID_exchange_code[mi_ID_I] = "E" ;

         /**
          * Override form of payment by A10FOP for even exchange
          **/

         if ( md_ID_ticket_no[mi_ID_I].NotIsNull() && md_A11FOP == "EX" )
         {
            /**
              * Override form of payment by A10FOP when ticket is
				  * even exchange and A11FOP is EX
              **/

            md_IRP_form_of_pay[mi_IRP_I-1] = md_orig_form_of_pay;
            md_IRP_cc_id[mi_IRP_I-1]       = md_orig_cc_id;
            md_IRP_cc_no[mi_IRP_I-1]       = md_orig_cc_no;
            md_IRP_cc_exp_date[mi_IRP_I-1] = AB_null;
            md_IRP_auth_no[mi_IRP_I-1]     = AB_null;
            md_IRP_auth_source[mi_IRP_I-1] = AB_null;
         }
         //populateExchangeFare(po_PNR) ;
      }

		return(AB_true) ;
   }

	return(AB_false) ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::processTax(AB_Text   &pt_section,
							 D_amount  &pd_totTax)
{
   ENTER;
   LOG(logDEBUG) << "\npt_section:" << pt_section << endl ;

   AB_Boolean   lb_finished, lb_additional_tax ;
   D_amount     lm_XT_tax, lm_value, li_additional_tax ;
   int          li_index, li_tempIndex ;
   AB_Money     lm_amount ;
   AB_Varchar   lv_rawAmount(15) ;
   AB_BOOL      lb_ok ;
	int          li_XFIndex ;
	AB_Varchar   lv_XFTaxes(32),lv_temp(32) ;
	AB_Decimal   ld_totXFTax(5,2) ,ld_XFTax(5,2)  ;

	int          li_taxCounter,li_noofTax = 0 ;

	pd_totTax = D_amount(0) ;
   li_additional_tax = D_amount(0); // NEW
	lv_XFTaxes = AB_null ;
	ld_totXFTax = AB_Decimal(0.00) ;

   if ((pt_section.At(4).For(1) != "T") && (pt_section.At(6).For(1) != ":"))
      return ;

   lb_finished = AB_false ;
   lb_additional_tax = AB_false ;
   lm_XT_tax = AB_Decimal(0.00) ;
   lm_value = AB_Decimal(0.00) ;
   li_index = 4 ;

	li_taxCounter = mi_ITD_I ;

   while ( (li_index < 66) && !lb_finished )
   {
      if ( (pt_section.At(li_index).For(1) == "!") || 
			  (pt_section.At(li_index + 2).For(1) != ":")) 
      {
			/**
			 * No further processing, why not break ???
			 **/

         lb_finished = AB_true ;
		}
      else 
      {
			/**
			 * Extract the tax amount
			 **/

         lv_rawAmount = pt_section.At(li_index + 3).For(8) ;
         lv_rawAmount.removeLeadingBlanks() ;
         lm_amount = lv_rawAmount.to_money("#####.##", lb_ok) ;
         lm_value = lm_amount.to_string() ; 

			/* double chek that there is a valid amount */

			lb_ok = lv_rawAmount.findFirstCharNotInList("0123456789.") ? 0 : 1 ;

         if (lb_ok) 
         {
				/**
				 * Amount looks OK
				 **/

            if (pt_section.At(li_index + 11).For(2) == "XT")
            {
					/**
					 * We have an XT tax
					 **/

               lb_additional_tax = AB_true ;

               lm_XT_tax = lm_XT_tax + lm_value ;

					LOG(logDEBUG) << "We have  additional  XT tax : " << lm_value << endl << flush;
            }
            else
				{
					D_char_numeric   ld_taxAmt;
					D_tax            ld_tax ;
					ld_taxAmt = pt_section.At(li_index + 3).For(8) ;
					ld_tax = pt_section.At(li_index + 11).For(2) ; // Used to be For(3)

					if ( ld_tax == "XF" )
					{
						/**
						 * We have an XF tax
						 **/ 

               	lb_additional_tax = AB_true ;

						li_XFIndex = pt_section.findSubstring("XF:") ;

						if ( li_XFIndex > 0 )
						{
							lv_XFTaxes = pt_section.At(li_XFIndex+3) ;

							li_XFIndex = lv_XFTaxes.findSubstring("!") ;

							if ( li_XFIndex > 0 )
							{
								lv_XFTaxes = lv_XFTaxes.For(li_XFIndex-1) ;
							}
						}
						else
						{
							lv_XFTaxes = AB_Varchar(2,ld_tax) + "   " ; // Overwritten repeattedly ???
						}
					   LOG(logDEBUG) << "We have  additional  XF tax : " << lm_value << endl << flush;
					}
					else
					{
						/**
						 * We have an tax which is not XT,XF
						 **/ 

               	fillIrDocumentTax(ld_taxAmt, ld_tax) ;

						pd_totTax = pd_totTax + D_amount(ld_taxAmt) ;
						li_noofTax++ ;
					}
				}
         }
         else 
			{
				/**
				 * ???
				 **/

            lb_additional_tax = AB_true ;
			}
      }
      li_index = li_index + 13 ;
   }

   if ( lb_additional_tax )
   {
		LOG(logDEBUG) << "We do have additional tax XF XT! " << endl << flush;

      li_index = pt_section.findSubstring("IT:") ;

      li_tempIndex = mi_ITD_I ;
  
      if (li_index != 0)
      {
			/**
			 * We have IT: , needs further processing
			 **/

         lb_finished = AB_false ;
         li_index = li_index + 3 ;
    
         while ( !lb_finished && (li_index < pt_section.length()) )
         {
            if ((pt_section.At(li_index).For(8) != " ") && (pt_section.At(li_index).For(1) != "!"))
            {
					/**
					 * Extract tax amount from IT: 
					 **/

               lv_rawAmount = pt_section.At(li_index).For(8) ;
               lv_rawAmount.removeLeadingBlanks() ;
               lm_amount = lv_rawAmount.to_money("#####.##", lb_ok) ;
               lm_value = lm_amount.to_string() ;

               if (lb_ok)
               {
						/**
						 * Adds up the additional tax
						 **/

                  li_additional_tax = li_additional_tax + lm_value ;

						D_char_numeric   ld_taxAmt;
						D_tax            ld_tax ;

						ld_taxAmt = pt_section.At(li_index).For(8) ;
						ld_tax = pt_section.At(li_index + 8).For(2) ;

						if ( ld_tax == "XF" )
						{
							/**
							 * We have XF tax
							 **/

							li_XFIndex = pt_section.findSubstring("XF:") ;

							if ( li_XFIndex > 0 )
							{
								lv_XFTaxes = pt_section.At(li_XFIndex+3) ;
								li_XFIndex = lv_XFTaxes.findSubstring("!") ;
								if ( li_XFIndex > 0 )
								{
									lv_XFTaxes = lv_XFTaxes.For(li_XFIndex-1) ;
								}
							}
							else
							{
								lv_XFTaxes = AB_Varchar(2,ld_tax) + "   " ;
							}
						}
						else if ( ld_tax == "XT" && lv_XFTaxes.IsNull())
						{
							/**
							 * We have XT tax , we might have XF: following
							 **/

							li_XFIndex = pt_section.findSubstring("XF:") ;

							if ( li_XFIndex > 0 )
							{
								lv_XFTaxes = pt_section.At(li_XFIndex+3) ;
								li_XFIndex = lv_XFTaxes.findSubstring("!") ;

								if ( li_XFIndex > 0 )
								{
									lv_XFTaxes = lv_XFTaxes.For(li_XFIndex-1) ;
								}

								// Get Total XF amount

								lv_temp = lv_XFTaxes ;

								while ( lv_temp.length() > 3  && lv_XFTaxes != "XF   " )
								{
									int li_ind, li_length;

									li_ind = lv_temp.findFirstCharNotInSet(".1234567890") ;
									li_length = li_ind ? li_ind - 1 : lv_temp.length() - 3;

									ld_XFTax = AB_Decimal(5,2,lv_temp.For(li_length) );

									ld_totXFTax = ld_totXFTax + D_amount(ld_XFTax) ;

									lv_temp = lv_temp.At(4 + li_length)  ;
			
								}

								/**
								 * Take away XF taxes
								 **/

								ld_taxAmt = ld_taxAmt - D_amount(ld_totXFTax); // New spot
							}
							else
							{
								lv_XFTaxes = AB_null ;
							}
						}

						if ( ld_tax != "XF" )
						{
               		fillIrDocumentTax(ld_taxAmt, ld_tax) ;

							pd_totTax = pd_totTax + D_amount(ld_taxAmt) ;

							li_noofTax++ ;
						}
               }
            }
            else
               lb_finished = AB_false ;

            li_index = li_index + 10 ;
         }

			if ( lv_XFTaxes.NotIsNull() && lv_XFTaxes != "XF   " )
			{
				while ( lv_XFTaxes.length() > 3 )
				{
					int li_ind, li_length;

					li_ind = lv_XFTaxes.findFirstCharNotInSet(".1234567890") ;

					li_length = li_ind ? li_ind - 1 : lv_XFTaxes.length();

					ld_XFTax = AB_Decimal(5,2,lv_XFTaxes.For(li_length) );

   				md_ITD_line_number[mi_ITD_I]  = md_ID_pax_id[mi_ID_I] ;
            	md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            	md_ITD_tax_id[mi_ITD_I]       = "XF" ;
					md_ITD_city_id[mi_ITD_I]      = lv_XFTaxes.At(li_ind).For(3) ;
            	md_ITD_tax_amount[mi_ITD_I++] = ld_XFTax ;

					pd_totTax = pd_totTax + D_amount(ld_XFTax) ;

					li_noofTax++ ;

					lv_XFTaxes = lv_XFTaxes.At(4 + li_length) ;
				}
			}

         if ( li_additional_tax != lm_XT_tax ) 
         {
				/**
				 * Create XT tax
				 **/

            mi_ITD_I = li_tempIndex ;

   			md_ITD_line_number[mi_ITD_I]  = md_ID_pax_id[mi_ID_I] ;
            md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            md_ITD_tax_id[mi_ITD_I]       = "XT" ;
            md_ITD_tax_amount[mi_ITD_I++] = lm_XT_tax ;

				pd_totTax = pd_totTax + lm_XT_tax ;

				li_noofTax++ ;
         }
      }
/* {NEW */
      else
		{
			/**
			 * There is no IT: but we have XT, create XT
			 **/

			if ( lm_XT_tax.NotIsNull() ) {

            mi_ITD_I = li_tempIndex ;

   			md_ITD_line_number[mi_ITD_I]  = md_ID_pax_id[mi_ID_I] ;
            md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            md_ITD_tax_id[mi_ITD_I]       = "XT" ;
            md_ITD_tax_amount[mi_ITD_I++] = lm_XT_tax ;

				pd_totTax = pd_totTax + lm_XT_tax ;

				li_noofTax++ ;
		   }	
      }
/*}NEW */

		if ( lv_XFTaxes.NotIsNull() && lv_XFTaxes != "XF   " )
		{
			/**
			 * Create each XF taxes
			 **/

			while ( lv_XFTaxes.length() > 3 )
			{
				int li_ind, li_length;

				li_ind = lv_XFTaxes.findFirstCharNotInSet(".1234567890") ;

				li_length = li_ind ? li_ind - 1 : lv_XFTaxes.length();

				ld_XFTax = AB_Decimal(5,2,lv_XFTaxes.For(li_length) );

   			md_ITD_line_number[mi_ITD_I]  = md_ID_pax_id[mi_ID_I] ;
            md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            md_ITD_tax_id[mi_ITD_I]       = "XF" ;
				md_ITD_city_id[mi_ITD_I]      = lv_XFTaxes.At(li_ind).For(3) ;
            md_ITD_tax_amount[mi_ITD_I++] = ld_XFTax ;

				pd_totTax = pd_totTax + D_amount(ld_XFTax) ;

				li_noofTax++ ;

				lv_XFTaxes = lv_XFTaxes.At(4 + li_length) ;
 
			}
		}
   }

	if ( li_noofTax > 10 )
	{
		/**
		 * If more than 10 Taxes then condense Tax except US and XF
		 **/

		condense_IrDocumentTax(md_ID_pax_id[mi_ID_I],
									  md_ID_line_number[mi_ID_I],
									  li_taxCounter) ;

	}

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fillIrDocumentTax(D_char_numeric &pd_taxAmount, D_tax &pd_taxId)
{
   ENTER;
   
   md_ITD_line_number[mi_ITD_I]  = md_ID_pax_id[mi_ID_I] ;
   md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
   md_ITD_tax_id[mi_ITD_I]       = pd_taxId ;
   md_ITD_tax_amount[mi_ITD_I++]   = pd_taxAmount ;

   EXIT;
}


//---------------------------------------------------------------------------------------------------------------------------
void CTSS::processOrigFare(CPNR &po_PNR, 
									AB_Char &pc_exch_num, 
									AB_Char     &pc_docLine,
									AB_Char &pc_exch_type) 

{

   AB_Text      lt_section ;   
   int          li_count, li_pos ;
   AB_Varchar   lv_commission1(20),
                lv_commission2(20) ;
   V_AB_Varchar lva_origValues(100, 100) ;

	AB_Varchar	 lv_exch_taxes(255) ; 

	AB_Varchar   lv_remark(80) ;
	AB_Char      lc_tax_code(2) ;


	lv_exch_taxes = AB_null  ;
	lv_remark     = AB_null ;

   lt_section   = po_PNR.getTotalSection("A10", pc_exch_num) ;
   li_count     = 0 ;

   if (lt_section.IsNull())
      return ;

   pc_exch_type = lt_section.At(35).For(1) ;
   md_ID_orig_ticket_no[mi_ID_I] = lt_section.At(6).For(13) ;
   md_ID_penalty_ch_fee[mi_ID_I] = lt_section.At(51).For(8) ;

   
   if (md_ID_penalty_ch_fee[mi_ID_I] == " ")
      md_ID_penalty_ch_fee[mi_ID_I] = AB_null ;

	lv_remark = pc_docLine + lt_section.At(39).For(12) ;
      
	li_pos = lt_section.findSubstring("!") ;
	li_count = 0 ;
	while (li_pos > 0)
	{
		li_count++ ;

		if (li_count == 3) 
      {
			lv_commission1 = lt_section.At(13).For(9) ;

         md_orig_form_of_pay = lt_section.At(35).For(2) ;

         if ( md_orig_form_of_pay == "S " )
         {
				/**
				 * Cash payment
				 **/

            md_orig_form_of_pay = "CA";
            md_orig_cc_id = AB_null;
            md_orig_cc_no = AB_null;
			}
         else if ( md_orig_form_of_pay == "CK" ) 
         {
				/**
				 * Check payment
				 **/

            md_orig_form_of_pay = "CK";
            md_orig_cc_id = AB_null;
            md_orig_cc_no = AB_null;
         } 
         else if ( md_orig_form_of_pay == "CC" ) 
			{
				/**
				 * Check payment
				 **/

            md_orig_form_of_pay = "CC";
            md_orig_cc_id = lt_section.At(37).For(2);
            md_orig_cc_no = lt_section.At(39).For(15);
            md_orig_cc_no.removeTrailingBlanks();

				if ( ! md_orig_cc_no.isAllNumeric() ) 
				{
               md_orig_form_of_pay = "CA";
               md_orig_cc_id = AB_null;
               md_orig_cc_no = AB_null;
				}
			}
         else 
         {
			   /**
			 	 * Might be Credit card directly 
				 * eg. AX1234567890
				 **/

            md_orig_form_of_pay = "CC";
            md_orig_cc_id = lt_section.At(35).For(2);
            md_orig_cc_no = lt_section.At(37).For(17);

            md_orig_cc_no.removeTrailingBlanks();

				if ( ! md_orig_cc_no.isAllNumeric() ) 
				{
               md_orig_form_of_pay = "CA";
               md_orig_cc_id = AB_null;
               md_orig_cc_no = AB_null;
				}
         }
      }

		if (li_count == 2)
		{
			lv_exch_taxes = lt_section ;
			lv_commission2 = lt_section.At(13).For(9) ; 
		}
        
		lt_section = lt_section.At(li_pos + 1) ;
		li_pos = lt_section.findSubstring("!") ;

		if ( li_count == 2 )
		if ( lv_exch_taxes.NotIsNull() )
		{
			lv_exch_taxes = lv_exch_taxes.For(li_pos) ;
		}

	}

	if (li_count >= 3)
	{
		lv_remark = lv_remark + lv_commission1 ;
	}
	else
	{
		// No Exchange Taxes were recorded 

		lv_remark = lv_remark + lv_commission2 ;
		lv_exch_taxes = AB_null ;
	}

	lc_tax_code = AB_null ;
	if ( lv_exch_taxes.NotIsNull() )
	{
		li_pos = lv_exch_taxes.findSubstring("T1:") ;

		if ( lv_exch_taxes.At(li_pos+11).For(2) == "  " )
		{
			lc_tax_code = "US" ;
		}
		else 
		{
			lc_tax_code = lv_exch_taxes.At(li_pos+11).For(2) ;
		}

		lv_remark = lv_remark + "T1:" + lv_exch_taxes.At(li_pos+3).For(8) + lc_tax_code ;

		li_pos = lv_exch_taxes.findSubstring("T2:") ;
		if ( li_pos > 0 )
		{
			if ( lv_exch_taxes.At(li_pos+11).For(2) == "  " )
			{
				lc_tax_code = "XT" ;
			}
			else 
			{
				lc_tax_code = lv_exch_taxes.At(li_pos+11).For(2) ;
			}

			lv_remark = lv_remark + "T2:" + lv_exch_taxes.At(li_pos+3).For(8) + lc_tax_code ;
		}

	}

	if ( lv_remark.NotIsNull() )
	{

		md_IRMK_type[mi_IRMK_I]           = "COS" ;
		md_IRMK_remark[mi_IRMK_I]         = lv_remark ;
		md_IRMK_pax_id[mi_IRMK_I++]       = md_ID_pax_id[mi_ID_I] ;
	}

}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::processExchangeTaxes(	V_AB_Integer   &pia_exch_pax_id ,
            							V_AB_Varchar   &pva_exch_tax_id ,
            							V_AB_Varchar   &pva_exch_tax_amount,
            							int            &pi_exchangeTax_index)
{
   LOG(logDEBUG) << "START CTSS::processExchangeTaxes" << endl ;

   AB_Varchar     lv_taxData(100) ;
   AB_Integer     li_tempPax ;  
   int            li_count, 
                  li_index, 
                  li_pos, 
                  li_pax ;
   V_AB_Varchar lv_taxFormatData(30,30) ;

   li_count = 0 ;
   pi_exchangeTax_index = 1 ;
   li_index = 1 ;
   li_pos = 0 ;
   li_tempPax = md_ID_pax_id[mi_ID_I ] ;
   li_pax = li_tempPax.to_long() ;

   if (mi_exchTaxIndex < li_pax)
      CMsgHandler::setMsg(2495, " ", "RET_XTAX", 1, AB_Varchar(20,AB_null));
   else
   {
      lv_taxData =  mv_exchTaxData[li_pax] ;
      li_count = lv_taxFormatData.parseString(lv_taxData, "/") ;
      
      while (li_index <= li_count)
      {
         li_pos = lv_taxFormatData[li_index].findFirstCharNotInList("0123456789.") ;
         if (li_pos > 1)
         {
            if (lv_taxFormatData[li_index].length() == li_pos + 1) 
            {
               pva_exch_tax_id[pi_exchangeTax_index]     = lv_taxFormatData[li_index].At(li_pos) ; 
               pva_exch_tax_amount[pi_exchangeTax_index] = lv_taxFormatData[li_index].For(li_pos - 1) ;
               pia_exch_pax_id[pi_exchangeTax_index++]     = li_pax ;
            }
            else
            {
               CMsgHandler::setMsg(2495, " ", "RET_XTAX", 2, AB_Varchar(20,AB_null));
               return ;
            }
         }
         else 
         {
            CMsgHandler::setMsg(2495, " ", "RET_XTAX", 3, AB_Varchar(20,AB_null));
            return ;
         }
     
         li_index++ ;
      }
   
   }

   LOG(logDEBUG) << "END CTSS::processExchangeTaxes" << endl ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::calculateExchangeFare(D_char_numeric &pm_exchBaseCost, 
											D_char_numeric &pm_exchCommission)
{
   ENTER; 

   int li_exchangeTax_index ;
   D_amount li_orig_baseCost, li_exch_baseCost, li_orig_Commission, li_exch_Commission ;
   AB_BOOL lb_ok ;
   AB_Varchar lv_rawAmount(15) ;
   AB_Money lm_amount ;

   lv_rawAmount = md_ID_base_cost[mi_ID_I];
   lm_amount = lv_rawAmount.to_money("#####.##-", lb_ok) ;
   li_orig_baseCost = lm_amount.to_string() ;

   if (lb_ok)
   {
      lv_rawAmount = pm_exchBaseCost ;
      lm_amount = lv_rawAmount.to_money("##########.##-", lb_ok) ;
      li_exch_baseCost = lm_amount.to_string() ;

      if (lb_ok)
      {
         md_ID_base_cost[mi_ID_I] = li_orig_baseCost - li_exch_baseCost ;
    
         if (md_ID_base_cost[mi_ID_I] > 0)
            md_ID_exchange_code[mi_ID_I] = "A" ;
         else if (md_ID_base_cost[mi_ID_I] < 0 ) 
         {
            md_ID_exchange_code[mi_ID_I] = "R" ;
            md_ID_base_cost[mi_ID_I] = D_amount((li_orig_baseCost - li_exch_baseCost) * D_amount(-1.00)) ;
         }
         else 
            md_ID_exchange_code[mi_ID_I] = "E" ;
      }
      else 
         md_ID_base_cost[mi_ID_I] = "ERR" ;
   }
   else 
      md_ID_base_cost[mi_ID_I] = "ERR" ;

   lv_rawAmount = md_ID_commission[mi_ID_I] ;

   lm_amount = lv_rawAmount.to_money("######.##-", lb_ok) ;

   li_orig_Commission = lm_amount.to_string() ;
   
   if (lb_ok)
   {
      lv_rawAmount = pm_exchCommission ;
      lm_amount = lv_rawAmount.to_money("######.##-", lb_ok) ;
      li_exch_Commission = lm_amount.to_string() ;

      if (lb_ok)
      {
         if (li_orig_Commission == AB_Decimal(0.00))
         {
            if ((md_ID_comm_rate[mi_ID_I] != 0) && (li_orig_baseCost != AB_Decimal(0.00)))
            {
               if (li_orig_baseCost > AB_Decimal(0.00))
                  li_orig_Commission = ((li_orig_baseCost * md_ID_comm_rate[mi_ID_I]) / AB_Decimal(100)) + AB_Decimal(.005) ;
               else
                  li_orig_Commission = ((li_orig_baseCost * md_ID_comm_rate[mi_ID_I]) / AB_Decimal(100)) - AB_Decimal(.005) ;
            }
         }

         md_ID_commission[mi_ID_I] = li_orig_Commission - li_exch_Commission ;

         if (md_ID_commission[mi_ID_I] > 0)
            md_ID_exchange_code[mi_ID_I] = "A" ;
         else if (md_ID_commission[mi_ID_I] < 0 ) 
         {
            md_ID_exchange_code[mi_ID_I] = "R" ;
            md_ID_commission[mi_ID_I] = D_amount((li_orig_Commission - li_exch_Commission) * D_amount(-1.00)) ;
         }
         else 
            md_ID_exchange_code[mi_ID_I] = "E" ;
      }
      else 
         md_ID_commission[mi_ID_I] = "ERR" ;
   }
   else 
      md_ID_commission[mi_ID_I] = "ERR" ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::calculateExchangeTax(AB_Integer     &pi_paxId,
										  V_AB_Varchar   &pva_exchTaxId,
										  V_AB_Varchar   &pva_exchTax)
{
   ENTER; 

   int              li_taxIndex ;
	int              li_counter, li_ITD_I, li_exchCnt ;
	AB_Boolean       lb_found ;
	D_amount         ld_tax1,ld_tax2,ld_tax3 ;

	li_counter = 0 ;

	li_ITD_I = mi_ITD_I ;

	li_exchCnt = 1 ;

	while ( li_exchCnt < 3 )
	{
		LOG(logDEBUG) << "EXCH" << pva_exchTaxId[li_exchCnt] << endl ;
		li_counter = 0 ;
		lb_found = AB_false ;
		while (li_counter < li_ITD_I)
		{
			LOG(logDEBUG) << "ORIG" << md_ITD_tax_id[li_counter] << endl ;
			if ( md_ITD_pax_id[li_counter] == D_passenger_no(pi_paxId) )
			{
				if ( pva_exchTaxId[li_exchCnt] == md_ITD_tax_id[li_counter] )
				{
					ld_tax1 = md_ITD_tax_amount[li_counter] ;
					ld_tax2 = pva_exchTax[li_exchCnt] ;
					ld_tax3 = ld_tax1 - ld_tax2 ;
					if ( ld_tax3 < AB_Decimal(0.00) )
						ld_tax3 = ld_tax3 * AB_Decimal(-1) ;
					md_ITD_tax_amount[li_counter] = ld_tax3 ;
					lb_found = AB_true ;
				}

				if ( lb_found )
					break ;
			}

			li_counter++ ;
		}

		if ( !lb_found )
		{
			md_ITD_line_number[mi_ITD_I] = md_ITD_line_number[li_counter] ;
			md_ITD_tax_id[mi_ITD_I] = pva_exchTaxId[li_exchCnt] ;
			md_ITD_pax_id[mi_ITD_I] = md_ITD_pax_id[li_counter] ;
			md_ITD_tax_amount[mi_ITD_I++] = pva_exchTax[li_exchCnt] ;
		}

		li_exchCnt++ ;
	}

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::processFrequentFlyer(CPNR &po_PNR, AB_Text &pm_name , int &pi_pax_id)
{
   ENTER; 
   
   int li_counter, li_line_number ;
   AB_Text lt_segment ;
	AB_Boolean lb_found ;
   
   li_counter = 1 ;
   li_line_number = 0 ;
	lb_found = AB_false ;

   po_PNR.resetSegment("A03") ;
   lt_segment = po_PNR.getNextSegment("A03") ;

   while (lt_segment.NotIsNull())
   {
      if (li_counter > li_line_number)
      {
         if (lt_segment.At(4).For(21) == pm_name)
         {
				lb_found = AB_true ;
            li_line_number = li_counter;
            md_IRF_pax_id[mi_IRF_I] = pi_pax_id ;
            md_IRF_sup_id[mi_IRF_I] = lt_segment.At(25).For(2) ;
            md_IRF_fup_type[mi_IRF_I] = AB_null ;
            md_IRF_fup[mi_IRF_I] = lt_segment.At(28).For(11).to_string() ;
            md_IRF_fup[mi_IRF_I++].removeLeadingBlanks() ;
         }
         else if (lt_segment.At(4).For(21) == " ")
         {
				if ( lb_found )
				{
            	li_line_number = li_counter ;
            	md_IRF_pax_id[mi_IRF_I]= pi_pax_id ;
            	md_IRF_sup_id[mi_IRF_I]= lt_segment.At(25).For(2) ;
            	md_IRF_fup_type[mi_IRF_I]= AB_null ;
            	md_IRF_fup[mi_IRF_I]= lt_segment.At(28).For(11).to_string() ;
            	md_IRF_fup[mi_IRF_I++].removeLeadingBlanks() ;
				}
         }
			else
			{
				if ( lb_found )
				{
					break ;
				}
			}
      }
      li_counter++ ;
      lt_segment = po_PNR.getNextSegment("A03") ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------
AB_Boolean CTSS::LLUpdateIrperson(CPNR &po_PNR) 
{
   ENTER; 

   int li_indexLL, li_counter, li_pos, li_paxDataIndex,li_name, li_pax_id ;
   V_AB_Varchar lav_paxData(60,30) ;
   AB_Text lt_segment ;
   AB_Varchar lv_fullName(255) ;

   po_PNR.resetSegment("LL") ;
   lt_segment = po_PNR.getNextSegment("LL");

   if (lt_segment.IsNull())
   {
      return AB_false ;
   }
   
   li_counter = 0 ;
   li_pax_id = 2 ;

   while (lt_segment.NotIsNull()) 
   {
      lv_fullName = lt_segment.At(10) ;
      li_pos = lv_fullName.findSubstring("!") ;

      if (li_pos > 0)
         lv_fullName = lv_fullName.For(li_pos - 1) ;
      
      li_paxDataIndex = lav_paxData.parseStringMultDel(lv_fullName, "/") ;
      li_name = 1 ;
      
      while (li_name < li_paxDataIndex)
      {
         md_IP_last_name[mi_IP_I] = lav_paxData[1] ;
         md_IP_first_name[mi_IP_I] = lav_paxData[++li_name] ;
         md_IP_pax_id[mi_IP_I++] = li_pax_id++ ;
      }
      lt_segment = po_PNR.getNextSegment("LL");
   }
   return AB_true ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::createIrItins(CPNR &po_PNR)
{
   ENTER; 

   fillIrconsumdetail(po_PNR) ;

   populateIrTransportSectionA04(po_PNR) ;
   populateIrTransportSectionA05(po_PNR) ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populateIrTransportSectionA04(CPNR &po_PNR)
{
   ENTER; 

   // REMOVED CHECK FOR ROW BEING SPLIT

   int li_counter, li_pos ;
   AB_Text lt_segment ;


   li_counter = 0 ;
   po_PNR.resetSegment("A04") ;
   lt_segment = po_PNR.getNextSegment("A04");

   while (lt_segment.NotIsNull())
   {
      //md_IT_baggage[mi_IT_I]    = lt_segment.At(87).For(3) ;
      mv_baggage                  = lt_segment.At(87).For(3) ;

      commonIrTransportFields(lt_segment) ;

      md_IT_reference[mi_IT_I]    = md_IRT_reference ;
      md_IT_no_of_stops[mi_IT_I]  = lt_segment.At(86).For(1) ;
      md_IT_carrier_type[mi_IT_I] = lt_segment.At(90).For(4) ;
      md_IT_actual_miles[mi_IT_I] = lt_segment.At(97).For(5) ;

      md_IT_carrier_type[mi_IT_I].removeTrailingBlanks() ;

		// US044 START
      //Get Remarks
      //IrTransportRemarks(po_PNR) ;
		// US044 END

      if (lt_segment.At(81).For(4) == "    ")
         md_IT_meal[mi_IT_I] = AB_null ;
      else
         md_IT_meal[mi_IT_I] = lt_segment.At(81).For(4) ;

         //md_IT_ftp_miles[mi_IT_I] = lt_segment.At(li_pos + 3).For(5) ;
         //md_IT_ftp_miles[mi_IT_I] = AB_null ;
      li_pos = lt_segment.findSubstring("FF:") ;
      if (li_pos > 3)
			mi_ftp_miles = lt_segment.At(li_pos + 3).For(5) ;
      else
			mi_ftp_miles = AB_null ;
     
      li_pos = lt_segment.findSubstring("AC:") ;
      if (li_pos > 3)
         md_IT_aff_carrier[mi_IT_I] = lt_segment.At(li_pos + 3).For(12) ;
      else
         md_IT_aff_carrier[mi_IT_I] = AB_null ;

		//G145 Chage

      mi_IT_I++ ;
      lt_segment = po_PNR.getNextSegment("A04");
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populateIrTransportSectionA05(CPNR &po_PNR)
{
   ENTER; 

   int li_counter ;
   AB_Text lt_segment ;

   li_counter = 0 ;
   lt_segment = po_PNR.getNextSegment("A05");

   while (lt_segment.NotIsNull())
   {
      commonIrTransportFields(lt_segment) ;

      md_IT_reference[mi_IT_I] = md_IRT_reference ;
      md_IT_no_of_stops[mi_IT_I] = AB_null ;
      //md_IT_baggage[mi_IT_I] = AB_null ;
		mv_baggage = AB_null ;
      md_IT_carrier_type[mi_IT_I] = AB_null ;
      md_IT_actual_miles[mi_IT_I] = AB_null ;
      //md_IT_ftp_miles[mi_IT_I] = AB_null ;
      md_IT_aff_carrier[mi_IT_I] = AB_null ;

		// US044 START
      //Get Remarks
      //IrTransportRemarks(po_PNR) ;
		// US044 END

      if (lt_segment.At(79).For(4) == "    ")
         md_IT_meal[mi_IT_I] = AB_null ;
      else
         md_IT_meal[mi_IT_I] = lt_segment.At(79).For(4) ;

		//G145 Chage

      mi_IT_I++ ;
      lt_segment = po_PNR.getNextSegment("A05");
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::commonIrTransportFields(AB_Text &pt_segment)
{
   ENTER; 

   AB_Date  ld_tempDate ;
	AB_Char  lc_itinLine(2) ;
	D_city   lv_oCity,lv_dCity ;
	AB_Char  lc_year(2) ;

	AB_Varchar   lv_airInfo(255);

   md_IT_line_number[mi_IT_I]    = pt_segment.At(4).For(2) ;
	lc_itinLine                   = pt_segment.At(4).For(2) ;
   md_IT_sup_id[mi_IT_I]         = pt_segment.At(6).For(2) ;
   md_IT_service[mi_IT_I]        = pt_segment.At(23).For(4) ;
   md_IT_classofservice[mi_IT_I] = pt_segment.At(27).For(2) ;
   //md_IT_service_status[mi_IT_I] = pt_segment.At(29).For(2) ;
	mv_service_status             = pt_segment.At(29).For(2) ;
   md_IT_orig_city_id[mi_IT_I]   = pt_segment.At(47).For(3) ;
	lv_oCity = pt_segment.At(47).For(3) ;
   md_IT_orig_cityname[mi_IT_I]  = pt_segment.At(50).For(13) ;
   md_IT_dest_city_id[mi_IT_I]   = pt_segment.At(63).For(3) ;
	lv_dCity = pt_segment.At(63).For(3) ;
   md_IT_dest_cityname[mi_IT_I]  = pt_segment.At(66).For(13) ;

   if ((md_IT_sup_id[mi_IT_I] == "A6") ||
       (md_IT_sup_id[mi_IT_I] == "3Y") ||
       (md_IT_sup_id[mi_IT_I] == "2V") ||
       (md_IT_sup_id[mi_IT_I] == "2R"))
      md_IT_itin_type[mi_IT_I] = "R" ;
   else
      md_IT_itin_type[mi_IT_I] = "A" ;

   if (pt_segment.At(46).For(1) == "1")
      md_IT_arrival_ind[mi_IT_I] = "4" ;
   else if (pt_segment.At(46).For(1) == "3")
      md_IT_arrival_ind[mi_IT_I] = "1" ;
   else if (pt_segment.At(46).For(1) == "4")
      md_IT_arrival_ind[mi_IT_I] = "2" ;
   else
      md_IT_arrival_ind[mi_IT_I] = "0" ;

   if (pt_segment.At(31).For(5) == "     ")
      md_IT_start_date[mi_IT_I] = md_IRT_pnr_date ;
   else
   {
		AB_Char    lc_depDate(5) ;

		lc_depDate = pt_segment.At(31).For(5) ;
		mt_pnrDate = md_IRT_pnr_date ;

		lc_year =  mt_pnrDate.get_default_year(lc_depDate) ;
		lc_year.rightJustify('0') ;

		md_IT_start_date[mi_IT_I] = lc_depDate + lc_year ;

   }

   if (pt_segment.At(36).For(5) == "     ")
      md_IT_start_time[mi_IT_I] = AB_null ;
   else
      md_IT_start_time[mi_IT_I] = CheckTime(pt_segment.At(36).For(5));

   if (pt_segment.At(41).For(5) == "     ")
      md_IT_end_time[mi_IT_I] = AB_null ;
   else
      md_IT_end_time[mi_IT_I] = CheckTime(pt_segment.At(41).For(5));

//Common NULL Fields
   md_IT_cog_dep_city[mi_IT_I]  = AB_null ;
   md_IT_cog_dep_date[mi_IT_I]  = AB_null ;
   md_IT_cog_dep_time[mi_IT_I]  = AB_null ;
   md_IT_cog_dep_term[mi_IT_I]  = AB_null ;
   md_IT_cog_dep_gate[mi_IT_I]  = AB_null ;
   md_IT_cog_of_miles[mi_IT_I]  = AB_null ;
   md_IT_dep_terminal[mi_IT_I]  = AB_null ;
   md_IT_dep_gate[mi_IT_I]      = AB_null ;
   md_IT_report_time[mi_IT_I]   = AB_null ;
   md_IT_arr_gate[mi_IT_I]      = AB_null ;
   md_IT_arr_terminal[mi_IT_I]  = AB_null ;
   //md_IT_boarding_pass[mi_IT_I] = AB_null ;

//Common Edits
   editCity(md_IT_orig_city_id[mi_IT_I]) ;
   editCity(md_IT_dest_city_id[mi_IT_I]) ;
   editSupplier(md_IT_sup_id[mi_IT_I]) ;
   editUnitType(md_IT_classofservice[mi_IT_I]) ;


//Fill itin_type field for Irconsumdetail
   updateConsumdetail() ;

		//G145 Chage

		lv_airInfo = lc_itinLine + 
						 lv_oCity + 
						 lv_dCity +
					 	 md_IT_start_date[mi_IT_I] +
					 	 md_IT_arrival_ind[mi_IT_I];

		mva_surfaceTransport[mi_surfaceTransport++]  = lv_airInfo ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
// US044 START
void CTSS::IrTransportRemarks(CPNR &po_PNR)
{
   ENTER;

   AB_Text 			lt_segment ;
   AB_Text 			lt_remark ;
	D_line_number  ld_line_number;
   
   po_PNR.resetSegment("A15") ;
   lt_segment = po_PNR.getNextSegment("A15") ;

   while (lt_segment.NotIsNull())
   {
      ld_line_number = lt_segment.At(4).For(2) ;
		lt_remark = lt_segment.At(6).For(70);

		if ( lt_segment.At(4).For(2) == "00" )
		{
			createUnAssociatedRemark( lt_remark );
		}
		else
		{
      	if ( ! createTransportAssociatedRemark( ld_line_number, lt_remark ) )
         	if ( ! createRentalAssociatedRemark( ld_line_number, lt_remark ) )
            	createOtherAssociatedRemark( ld_line_number, lt_remark );
		}
      lt_segment = po_PNR.getNextSegment("A15") ;
   }
   EXIT;
}
// US044 END


//-----------------------------------------------------------------------------------------------------------
void CTSS::fillIrconsumdetail(CPNR &po_PNR)
{
   ENTER; 

   int li_pax_id ;
   AB_Text lt_A02Data, lt_A08Data ;

   li_pax_id = 0 ;
   po_PNR.resetSegment("A02") ;
   lt_A02Data = po_PNR.getNextSegment("A02") ;

   while (lt_A02Data.NotIsNull())
   {
      po_PNR.resetSegment("A08") ;
      lt_A08Data = po_PNR.getNextSegment("A08") ;

      while (lt_A08Data.NotIsNull())
      {
         if (lt_A02Data.At(76).For(2) == lt_A08Data.At(4).For(2)) 
         {
            LOG(logDEBUG) << "XXXXX" << endl;
            LOG(logDEBUG) << "fillIrPersonItin: lt_A02Data.At(76).For(2)= " << lt_A02Data.At(76).For(2) << endl ;
            LOG(logDEBUG) << "fillIrPersonItin: lt_A08Data.At(4).For(2)= " << lt_A08Data.At(4).For(2) << endl ;
            LOG(logDEBUG) << "fillIrPersonItin: lt_A02Data= " << lt_A02Data << endl ;
            LOG(logDEBUG) << "fillIrPersonItin: lt_A08Data= " << lt_A08Data << endl ;

            md_ICD_pax_id[mi_ICD_I]      = li_pax_id  + 1 ;
            md_ICD_itin_line_number[mi_ICD_I] = lt_A08Data.At(6).For(2) ;
            md_ICD_fare_basis[mi_ICD_I]  = lt_A08Data.At(8).For(8)  ;
				md_ICD_fare_basis[mi_ICD_I].removeTrailingBlanks() ;


				md_ICD_tkt_desig[mi_ICD_I] = lt_A08Data.At(38).For(6) ;
				md_ICD_tkt_desig[mi_ICD_I].leftJustify() ;

            md_ICD_fare_by_leg[mi_ICD_I] = lt_A08Data.At(16).For(6) + "." +
														 lt_A08Data.At(22).For(2);
            md_ICD_connection[mi_ICD_I]  = "O" ;
            md_ICD_cog_seat[mi_ICD_I++]  = AB_null ;
         }
         lt_A08Data = po_PNR.getNextSegment("A08") ;
      }
      li_pax_id++ ;
      lt_A02Data = po_PNR.getNextSegment("A02") ;
   }
   populateIrconsumdetailSectionA06(po_PNR) ;
   populateIrconsumdetailSectionA04(po_PNR) ;

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populateIrconsumdetailSectionA06(CPNR &po_PNR)
{
   ENTER; 

   int li_counter ;
   AB_Text lt_segment ;
   AB_Integer li_value , li_pax_pos, li_pos ;

   po_PNR.resetSegment("A06") ;
   lt_segment = po_PNR.getNextSegment("A06") ;

   while (lt_segment.NotIsNull())
   {
      li_counter = 0 ;
      li_value = lt_segment.At(4).For(2) ;

      while (li_counter < mi_ICD_I)
      {
         if (md_ICD_itin_line_number[li_counter] == li_value)
         {
            li_pos = lt_segment.findSubstring("!") ;
            li_pax_pos = (6 + ((md_ICD_pax_id[li_counter].to_long() - 1) * 4)) ;

            if ((li_pax_pos.to_long() + 3) < li_pos.to_long())
            {
               md_ICD_seat[li_counter]    = lt_segment.At(li_pax_pos).For(3) ;
               md_ICD_smoking[li_counter] = lt_segment.At(li_pax_pos.to_long() + 3).For(1) ;

               LOG(logDEBUG) << "li_counter= " << li_counter << endl ;
               LOG(logDEBUG) << "li_pos= " << li_pos << endl ;
               LOG(logDEBUG) << "li_pax_pos= " << li_pax_pos << endl ;
               LOG(logDEBUG) << "md_ICD_pax_id[li_counter]= " << md_ICD_pax_id[li_counter] << endl ;
               LOG(logDEBUG) << "lt_segment.At(li_pax_pos).For(3)= " << lt_segment.At(li_pax_pos).For(3) << endl ;
               LOG(logDEBUG) << "lt_segment.At(li_pax_pos.to_long() + 3).For(1)= " << lt_segment.At(li_pax_pos.to_long() + 3).For(1) << endl ;
               LOG(logDEBUG) << "li_value= " << li_value << endl ;
            }
         }
         li_counter++ ;
      }
      lt_segment = po_PNR.getNextSegment("A06") ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::populateIrconsumdetailSectionA04(CPNR &po_PNR)
{
   ENTER; 

   int li_counter, li_A04_counter ;
   AB_Text lt_segment ;
   AB_Integer li_value ;

   po_PNR.resetSegment("A04") ; 

   lt_segment = po_PNR.getNextSegment("A04") ;

   while (lt_segment.NotIsNull())
   {
      li_counter = 0 ;
      li_value = lt_segment.At(4).For(2) ;

      while (li_counter < mi_ICD_I)
      {
         if ((md_ICD_itin_line_number[li_counter].to_long() + 1) == li_value.to_long())
            md_ICD_connection[li_counter] = lt_segment.At(85).For(1) ;

         li_counter++ ;
      }
      lt_segment = po_PNR.getNextSegment("A04") ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::updateConsumdetail()
{
   ENTER; 

   int li_counter ;

   li_counter = 0 ;

   while (li_counter < mi_ICD_I)
   {
      if (md_ICD_itin_line_number[li_counter] == md_IT_line_number[mi_IT_I])
		{
         md_ICD_itin_type[li_counter] = md_IT_itin_type[mi_IT_I] ;
			md_ICD_baggage[li_counter] = mv_baggage ;
			md_ICD_service_status[li_counter] = mv_service_status ;
		}

      li_counter++ ;
   }

   EXIT; 
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::processNonArcDoc()
{
	AB_Text         lt_section ;
	AB_Char         lc_docLine(2) ;
	AB_Char         lc_docPax(2) ;
	AB_Varchar		 lv_ticketNo(10);
	int             li_index ;

	li_index = 1 ;

   while (li_index < mi_nonArcDoc )
   {
		bool lb_isPFS = false;

      md_ID_line_number[mi_ID_I] = mi_tktCounter++ ;
		md_ID_doc_type[mi_ID_I] = "TKT" ;

		// Process all non arc for 1st passenger

		md_ID_reference[mi_ID_I]   = md_IRT_reference ;
		md_ID_personal_crs[mi_ID_I] = md_personalCrs ;
		md_ID_crs_tour[mi_ID_I]     = md_crsTour ;
		md_ID_e_ticket[mi_ID_I]     = md_e_ticket ;
		md_ID_stp_home_city[mi_ID_I] = md_stpHomeCity ;
		md_ID_stp_host_city[mi_ID_I] = md_stpHostCity ;

		if ( md_nonArcDoc[li_index].For(3) ==  "TK-" )
		{
			if ( md_IP_pax_id[li_index - 1].IsNull() )
			{
				LOG(logDEBUG) << "WE ARE INSIDE" << endl ;
				break ;
			}
			md_ID_pax_id[mi_ID_I]      = md_IP_pax_id[li_index - 1] ;
      	md_ID_ticket_no[mi_ID_I]   = md_nonArcDoc[li_index].For(10) ;
         md_ID_form[mi_ID_I]        = AB_null ;
      	md_ID_no_of_books[mi_ID_I] = AB_Integer(1) ;
		}
		else
		{
		/* change as per scr#2788
		** when ticket_no is already 999999999 then retain ticket_no
		** but to identify non-arc tickets, pass 000000000
		*/

			md_ID_ticket_no[mi_ID_I]   = "0000000000" ;
      	md_ID_pax_id[mi_ID_I]      = md_IP_pax_id[0] ;
         md_ID_form[mi_ID_I]        = AB_null ;
      	md_ID_no_of_books[mi_ID_I] = AB_Integer(1) ;
		}

		int li_pos = md_nonArcDoc[li_index].findSubstring("M") ; 

		lt_section = md_nonArcDoc[li_index].At(li_pos) ;

      // GAP9110: if it is PFS, we will create a non-bsp ticket using the first air itin in the pnr
      // we check if the given sup_id is a PFS sup_id

      AB_Varchar        lv_sup_id(10);
      lv_sup_id = lt_section.At(5).For(3) ;
      if ( isPFSSupplier(md_IRT_cop_id, lv_sup_id) ){
         lb_isPFS = true;
         bool lb_ok = createPFSConsumdetail( md_ID_pax_id[mi_ID_I], md_ID_line_number[mi_ID_I] );
         if ( ! lb_ok ){
            initTicket( mi_ID_I );
			   mi_tktCounter-- ;
            li_index++;
            continue;
         }
      }

      processNonArcCost(lt_section) ;

      md_ID_saving_code[mi_ID_I] = md_savingCode ;
      md_ID_full_fare[mi_ID_I] = md_full_fare ;
      md_ID_low_prac_fare[mi_ID_I] = md_low_prac_fare ;
      md_ID_rebate[mi_ID_I] = md_rebate;

		lc_docLine = md_ID_line_number[mi_ID_I] ;
		lc_docLine.rightJustify('0');

		lc_docPax = md_ID_pax_id[mi_ID_I] ;
		lc_docPax.rightJustify('0');

		if ( lb_isPFS ) {
            md_ID_full_fare[mi_ID_I]     = AB_null ;
            md_ID_low_prac_fare[mi_ID_I] = AB_null ;
				md_ID_saving_code[mi_ID_I]   = AB_null ;
      }
      else{
			md_nonArcCost[li_index] = lc_docLine +lc_docPax + md_ID_sup_id[mi_ID_I] ;
		}

		li_index ++ ;
      mi_ID_I++ ;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::processNonArcCost(AB_Text  &pt_section)
{
	V_AB_Varchar       lva_arrayField(100,100) ;
	D_amount           ld_cost, ld_tax ;
	AB_Text            lt_segment ;

	md_ID_sup_id[mi_ID_I]     = pt_section.At(5).For(3) ;
	md_ID_cur_id[mi_ID_I]      = "USD" ;

	int li_pos       = pt_section.findSubstring("!") ;

	if (li_pos > 0)
		pt_section = pt_section.For(li_pos -1) ;

	int i = lva_arrayField.parseStringMultDel(pt_section,"/") ;

	lva_arrayField[4].leftJustify() ;

	md_ID_commission[mi_ID_I]  = lva_arrayField[4] ;

	li_pos       = md_ID_commission[mi_ID_I].findSubstring("P") ;

	if (li_pos > 0)
	{
		md_ID_comm_rate[mi_ID_I]   = md_ID_commission[mi_ID_I].At(li_pos + 1 ) ;
		md_ID_commission[mi_ID_I]  = AB_null ;
	}
	else
		md_ID_commission[mi_ID_I].leftJustify() ;

	lva_arrayField[2].leftJustify() ;

	ld_cost = lva_arrayField[2] ;

	lva_arrayField[3].leftJustify() ;

	ld_tax = lva_arrayField[3] ;

	if ( ld_tax > D_amount(0.00) )
	{
		ld_cost = ld_cost - ld_tax ;

		md_ITD_line_number[mi_ITD_I]   = md_ID_line_number[mi_ID_I] ;
		md_ITD_pax_id[mi_ITD_I]        = md_ID_pax_id[mi_ID_I] ;
		md_ITD_tax_id[mi_ITD_I]        = "XT" ;
		md_ITD_tax_amount[mi_ITD_I++]  = ld_tax ;
	}

	md_ID_base_cost[mi_ID_I] = ld_cost ;

	// Find out Form of payment

	md_IRP_amount[mi_IRP_I] = md_ID_base_cost[mi_ID_I] ;
	md_IRP_pax_id[mi_IRP_I] = md_ID_pax_id[mi_ID_I] ;
	md_IRP_line_number[mi_IRP_I]  = md_ID_line_number[mi_ID_I] ;
	md_IRP_pay_id[mi_IRP_I] = AB_Integer(1) ;

	if ( pt_section.At(2).For(2) == "CA" )
		md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
	else if ( pt_section.At(2).For(2) == "AR" )
		md_IRP_form_of_pay[mi_IRP_I] = "CK" ;
	else if ( pt_section.At(2).For(2) != "CC")
		md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
	else
		md_IRP_form_of_pay[mi_IRP_I] = "CC" ;

	if ( lva_arrayField[5].For(1) == "N" && lva_arrayField[5].At(2).For(1).isAllNumeric())
		lt_segment = lva_arrayField[6] ;
	else
		lt_segment = lva_arrayField[5] ;
		

	if ( md_IRP_form_of_pay[mi_IRP_I] == "CC" )
		fillTktFP(lt_segment) ;

	mi_IRP_I++ ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_IrOther(AB_Text &pt_section)
{
   ENTER; 

	AB_Char       lc_productType(1);
	AB_Varchar    lv_field(80) ;
	AB_Char       lc_year(2) ;


	int li_pos = pt_section.findSubstring("DP:") ;

	if ( li_pos == 0)
		return ;

   md_IO_sup_id[mi_IO_I] = pt_section.At(li_pos + 6).For(3) ;

	md_IO_line_number[mi_IO_I]    = pt_section.At(5).For(2) ;

	lc_productType = pt_section.At(14).For(1) ;

	getItinType(lc_productType,
					md_IO_itin_type[mi_IO_I],
					md_IO_unit_type[mi_IO_I]) ;

   md_IO_start_date[mi_IO_I] = pt_section.At(7).For(7) ;

   md_IO_orig_city_id[mi_IO_I] = pt_section.At(49).For(3) ;
   md_IO_dest_city_id[mi_IO_I] = pt_section.At(49).For(3) ;


   if (pt_section.At(55).For(5) == "     ")
   {
      LOG(logDEBUG) << "Bmd_IO_end_date = NULL" << endl ;
		md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;
   }
   else
   {
      AB_Date ld_tempDate ;

      ld_tempDate = pt_section.At(55).For(5) + md_IO_start_date[mi_IO_I].At(6);

      lc_year = ld_tempDate.get_default_year(md_IO_start_date[mi_IO_I]) ;
		lc_year.rightJustify('0') ;

      md_IO_end_date[mi_IO_I] = pt_section.At(35).For(5) + lc_year ;
      LOG(logDEBUG) << "A)md_IO_end_date= " << md_IO_end_date[mi_IO_I] << endl ;
   }

   lv_field = pt_section.getNthBracketedField(1, "CF:", "!");

   if (lv_field.Length().to_long() > 2)
      md_IO_reference[mi_IO_I] = lv_field.At(3) ;
   else
      md_IO_reference[mi_IO_I] = AB_null ;

   if (pt_section.At(63).For(1) == " ")
      md_IO_unit_ct[mi_IO_I] = 1 ;
	else
   	md_IO_unit_ct[mi_IO_I] = pt_section.At(63).For(1) ;

	addIrconsumdetail(md_IO_sup_id[mi_IO_I],
							md_IO_itin_type[mi_IO_I],
							md_IO_line_number[mi_IO_I]) ;

   mi_IO_I++ ;

   EXIT; 
}


//---------------------------------------------------------------------------------------------------------------------------
void CTSS::getItinType(AB_Char  &pc_productType,
							  AB_Char  &pc_itinType,
							  AB_Char  &pc_unitType)
{
	if (pc_productType == "C")
	{
		pc_itinType = "S" ;
		pc_unitType = "CRUISE" ;
		return ;
	}
	if (pc_productType == "D")
	{
		pc_itinType = "O" ;
		pc_unitType = "LAND" ;
		return ;
	}
	if (pc_productType == "F")
	{
		pc_itinType = "O" ;
		pc_unitType = "FORMS" ;
		return ;
	}
	if (pc_productType == "G")
	{
		pc_itinType = "O" ;
		pc_unitType = "GROUP" ;
		return ;
	}
	if (pc_productType == "I")
	{
		pc_itinType = "I" ;
		pc_unitType = "INS" ;
		return ;
	}
	if (pc_productType == "K")
	{
		pc_itinType = "O" ;
		pc_unitType = "TICKET" ;
		return ;
	}
	if (pc_productType == "L")
	{
		pc_itinType = "O" ;
		pc_unitType = "LIMO" ;
		return ;
	}
	if (pc_productType == "N")
	{
		pc_itinType = "O" ;
		pc_unitType = "INCLUDE" ;
		return ;
	}
	if (pc_productType == "O")
	{
		pc_itinType = "O" ;
		pc_unitType = "OTHER" ;
		return ;
	}
	if (pc_productType == "P")
	{
		pc_itinType = "O" ;
		pc_unitType = "PACKAGE" ;
		return ;
	}
	if (pc_productType == "S")
	{
		pc_itinType = "O" ;
		pc_unitType = "SERVICE" ;
		return ;
	}
	if (pc_productType == "T")
	{
		pc_itinType = "T" ;
		pc_unitType = "TOUR" ;
		return ;
	}
	if (pc_productType == "U")
	{
		pc_itinType = "O" ;
		pc_unitType = "SUPPLY" ;
		return ;
	}
	if (pc_productType == "V")
	{
		pc_itinType = "O" ;
		pc_unitType = "LEISURE" ;
		return ;
	}
	if (pc_productType == "W")
	{
		pc_itinType = "O" ;
		pc_unitType = "SOFTWARE" ;
		return ;
	}
}

//-----------------------------------------------------------------------------------------------------------------
void CTSS::addIrconsumdetail(D_supplier    &pd_supId,
						           AB_Char        &pc_itinType,
						           AB_Integer     &pi_itinLine)
{
	int 	li_index = 1 ;

   while (li_index <= mi_nonArcDoc)
   {
		if (md_nonArcCost[li_index].At(5).For(3) == pd_supId )
		{
			md_ICD_pax_id[mi_ICD_I] = md_nonArcCost[li_index].At(3).For(2) ;
			md_ICD_itin_type[mi_ICD_I] = pc_itinType ;
			md_ICD_line_number[mi_ICD_I] = md_nonArcCost[li_index].For(2) ;
			md_ICD_itin_line_number[mi_ICD_I++] = pi_itinLine ;
		}
		 li_index++ ;
	}

}

//---------------------------------------------------------------------------------------------------------------------------
void CTSS::fill_IrBus(AB_Text &pt_section)
{
	AB_Char       lc_productType(1);

	int li_pos = pt_section.findSubstring("DP:") ;

	if ( li_pos == 0)
		return ;

   md_IT_sup_id[mi_IT_I] = pt_section.At(li_pos + 6).For(3) ;

	md_IT_line_number[mi_IT_I]    = pt_section.At(5).For(2) ;

	md_IT_itin_type[mi_IT_I]      = "B" ;

   md_IT_start_date[mi_IT_I] = pt_section.At(7).For(7) ;

   md_IT_orig_city_id[mi_IT_I] = pt_section.At(49).For(3) ;
   md_IT_dest_city_id[mi_IT_I] = pt_section.At(49).For(3) ;
	md_IT_arrival_ind[mi_IT_I]    = 0 ;

	addIrconsumdetail(md_IT_sup_id[mi_IT_I],
							md_IT_itin_type[mi_IT_I],
							md_IT_line_number[mi_IT_I]) ;

	md_IT_service[mi_IT_I] = "9999" ;

   mi_IT_I++ ;
}

//---------------------------------------------------------------------------------------------------------------------------

void CTSS::fillIrRail(AB_Text &pt_segment)
{
	AB_Char       lc_productType(1);

	int li_pos = pt_segment.findSubstring("DP:") ;

	if ( li_pos == 0)
		return ;

   md_IT_sup_id[mi_IT_I] = pt_segment.At(li_pos + 6).For(3) ;

	md_IT_line_number[mi_IT_I]    = pt_segment.At(5).For(2) ;

	md_IT_itin_type[mi_IT_I]     = "R" ;

   md_IT_start_date[mi_IT_I] = pt_segment.At(7).For(7) ;

   md_IT_orig_city_id[mi_IT_I] = pt_segment.At(49).For(3) ;
   md_IT_dest_city_id[mi_IT_I] = pt_segment.At(49).For(3) ;

	md_IT_arrival_ind[mi_IT_I]    = 0 ;

	addIrconsumdetail(md_IT_sup_id[mi_IT_I],
							md_IT_itin_type[mi_IT_I],
							md_IT_line_number[mi_IT_I]) ;

	md_IT_service[mi_IT_I] = "9999" ;

   mi_IT_I++ ;
}

//-----------------------------------------------------------------------------------------------------
void CTSS::fillTktFP(AB_Text &pv_CCInfo) 
{
	// Find out Form of payment

	if ( pv_CCInfo.NotIsNull())
	{
		md_IRP_cc_id[mi_IRP_I] = pv_CCInfo.For(2) ;
		md_IRP_cc_no[mi_IRP_I] = pv_CCInfo.At(3) ;
		md_IRP_cc_exp_date[mi_IRP_I] = AB_null ;
		md_IRP_auth_no[mi_IRP_I] = AB_null ;
		md_IRP_auth_source[mi_IRP_I] =  AB_null ;
	}
	else
	{
		if ( mv_a14CC.NotIsNull())
		{
			md_IRP_cc_id[mi_IRP_I] = mv_a14CC.For(2) ;
			md_IRP_cc_no[mi_IRP_I] = mv_a14CC.At(3) ;
			md_IRP_cc_exp_date[mi_IRP_I] = AB_null ;
			md_IRP_auth_no[mi_IRP_I] = AB_null ;
			md_IRP_auth_source[mi_IRP_I] =  AB_null ;
		}
		else
		{
			md_IRP_cc_id[mi_IRP_I] = md_cc_id ;
			md_IRP_cc_no[mi_IRP_I] = md_cc_no ;
			md_IRP_cc_exp_date[mi_IRP_I] = md_cc_exp_date ;
			md_IRP_auth_no[mi_IRP_I] = md_auth_no ;
			if (md_auth_source == " " || md_auth_source == "S" )
				md_IRP_auth_source[mi_IRP_I] =  " " ;
			else
				md_IRP_auth_source[mi_IRP_I] = "A" ;
		}

	}

   // TKX CC EXP DATE START

	if ( md_TKX_cc_exp_date.NotIsNull() )
	{
		md_IRP_cc_exp_date[mi_IRP_I]  = md_TKX_cc_exp_date;
	}

   // TKX CC EXP DATE END

}

//-----------------------------------------------------------------------------------------------------
void CTSS::updateTaxdetail()
{
	int li_count = 0 ;

	while ( li_count < mi_ITD_I )
	{
		if ( md_ITD_pax_id[li_count] == md_IP_pax_id[mi_IP_I] &&
			  md_ITD_line_number[li_count] == md_ID_line_number[mi_ID_I] )
		{
			md_ITD_tax_amount[li_count] = "0.00" ;
		}
		li_count ++ ;

	}
}

//---------------------------------------------------------------------------------------------------
void CTSS::fillIrconsumDetailRental(AB_Char    &pc_itinType,
                                    AB_Integer &pi_itinLine,
                                    AB_Char    &pc_status,
												AB_Varchar &pv_duePaid)
{
	if ( pv_duePaid.NotIsNull() )
	{
   	md_ICD_text[mi_ICD_I]           = "RENTAL:" + pv_duePaid ;
	}
   md_ICD_itin_line_number[mi_ICD_I] = pi_itinLine ;
   md_ICD_itin_type[mi_ICD_I]        = pc_itinType ;
   md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id [0] ;
   md_ICD_service_status[mi_ICD_I++]   = pc_status ;

}

//---------------------------------------------------------------------------------------------------
void CTSS::checkRental()
{
	int   li_count, li_index = 1 ;

   while (li_index <= mi_nonArcDoc)
   {
		li_count = 0 ;
		while ( li_count < mi_ICD_I )
		{
			if ( md_ICD_text[li_count].For(7) == "RENTAL:" && md_ICD_line_number[li_count].IsNull())
			{
				if (md_nonArcCost[li_index].At(5).For(3) == md_ICD_text[li_count].At(8).For(3) )
				{
					md_ICD_line_number[li_count] = md_nonArcCost[li_index].For(2) ;

				}
			}
			li_count++ ;
		}
		li_index++ ;
	}
}

//-----------------------------------------------------------------------------------------------
void CTSS::processExtraTicketLine()
{
	int          li_no_pax ;
	int          li_index ;
	int          li_pos,i ;
	AB_Varchar   lv_longTicket(40);
	AB_Varchar   lv_FP(5) ;
	V_AB_Varchar lva_arrayField(100,100) ;


	li_no_pax = mi_IP_I ;

	li_index = mi_manualTicketIndex - li_no_pax ;

	if ( li_index > 0 )
	{
		while (li_index > 0 )
		{
			lv_longTicket = mv_manualTicketData[li_no_pax + 1] ;
	

			if (lv_longTicket.NotIsNull())
			{
      		md_ID_line_number[mi_ID_I] = mi_tktCounter++ ;
				md_ID_pax_id[mi_ID_I]      = md_ID_pax_id[0] ;
	
				li_pos = lv_longTicket.findSubstring("/M") ;
		
				if ( li_pos == 0 )
				{
					md_ID_form[mi_ID_I]         = lv_longTicket.For(4) ;
					md_ID_ticket_no[mi_ID_I]    = lv_longTicket.At(5) ;
					md_ID_doc_type[mi_ID_I]     = "BSP" ;
					md_ID_sup_id[mi_ID_I]       = md_supId ;
					md_ID_sel_access[mi_ID_I]   = md_selAccess ;
					md_ID_personal_crs[mi_ID_I]  = md_personalCrs ;
					md_ID_crs_tour[mi_ID_I]      = md_crsTour ;
					md_ID_e_ticket[mi_ID_I]      = md_e_ticket ;
					md_ID_stp_home_city[mi_ID_I] = md_stpHomeCity ;
					md_ID_stp_host_city[mi_ID_I] = md_stpHostCity ;
					md_ID_pic[mi_ID_I]           = md_ID_pic[0] ;

				}
				else
				{
					// Parse string

					lv_FP = lv_longTicket.At(li_pos+1).For(3) ;
	
					int i = lva_arrayField.parseStringMultDel(lv_longTicket,"/") ;
		
					// Non arc ticket segment
	
					md_ID_sup_id[mi_ID_I]  = lva_arrayField[2].At(5).For(3) ;
					md_ID_doc_type[mi_ID_I]   = "TKT" ;
           		md_ID_form[mi_ID_I]        = AB_null ;
      			md_ID_ticket_no[mi_ID_I]   = lva_arrayField[1].For(10) ;
				}
	
				md_ID_no_of_books[mi_ID_I]   = AB_Integer(1) ;
				md_ID_saving_code[mi_ID_I]   = md_savingCode ;
				md_ID_full_fare[mi_ID_I]     = md_full_fare ;
				md_ID_low_prac_fare[mi_ID_I] = md_low_prac_fare ;
				md_ID_rebate[mi_ID_I]        = md_rebate;
	
				/* Assign Cost Info */
				md_ID_comm_rate[mi_ID_I]  = md_ID_comm_rate[0] ;
				md_ID_commission[mi_ID_I]  = md_ID_commission[0] ;
				md_ID_base_cost[mi_ID_I]  = md_ID_base_cost[0] ;
	
				processTaxExtraTicketLine(mi_ID_I) ;

				/* Assign Form of payment */
				md_IRP_amount[mi_IRP_I] = md_ID_base_cost[mi_ID_I] ;
				md_IRP_pax_id[mi_IRP_I] = md_ID_pax_id[mi_ID_I] ;
				md_IRP_line_number[mi_IRP_I]  = md_ID_line_number[mi_ID_I] ;
				md_IRP_pay_id[mi_IRP_I] = AB_Integer(1) ;

				if ( lv_FP.NotIsNull() )
				{
   				if ( lv_FP.At(2).For(2) == "CA" )
      				md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
   				else if ( lv_FP.At(2).For(2) == "AR" )
      				md_IRP_form_of_pay[mi_IRP_I] = "CK" ;
   				else if ( lv_FP.At(2).For(2) != "CC")
      				md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
   				else
      				md_IRP_form_of_pay[mi_IRP_I] = "CC" ;
				}
				else
				{
      				md_IRP_form_of_pay[mi_IRP_I] = md_form_of_pay ;
				}

				AB_Varchar lv_segment ;
				lv_segment = AB_null ;

				if ( md_IRP_form_of_pay[mi_IRP_I] == "CC" )
					fillTktFP(lv_segment) ;

				mi_IRP_I++ ;

				md_ID_reference[mi_ID_I++] = md_ID_reference[0] ;
	
			}
	
			li_no_pax++ ;
			li_index-- ;
		}
	}
}
	
//-----------------------------------------------------------------------------------------------
void CTSS::processTaxExtraTicketLine(int &pi_tktLine) 
{

	int li_index ;


	li_index = 1 ;

	while ( li_index < mi_ITD_I )
	{

		if ( md_ITD_line_number[li_index] > AB_Integer(1) )
		{
			break ;
		}
		else
		{
			md_ITD_line_number[mi_ITD_I]   = md_ID_line_number[pi_tktLine] ;
			md_ITD_pax_id[mi_ITD_I]        = md_ID_pax_id[0] ;
			md_ITD_tax_id[mi_ITD_I]        = md_ITD_tax_id[li_index] ;
			md_ITD_city_id[mi_ITD_I]       = md_ITD_city_id[li_index] ;
			md_ITD_tax_amount[mi_ITD_I++]  = md_ITD_tax_amount[li_index] ;
		}

		li_index++ ;
	}
}
//-----------------------------------------------------------------------------------------------
void CTSS::parsePhone(AB_Varchar    &pv_phone)
{
   int li_pos ;
	AB_Varchar		lv_long_Rphone(100) ;
	AB_Varchar		lv_long_Bphone(100) ;
	AB_Varchar		lv_long_Mphone(100) ;
	AB_Varchar		lv_long_Fphone(100) ;
	int li_init = 1 ;
	int li_testPos ;

	AB_Varchar     lv_email(255);
	AB_Varchar     lv_email_before(255);
	AB_Varchar     lv_email_after(255);

	lv_long_Rphone = AB_null ;
	lv_long_Bphone = AB_null ;
	lv_long_Mphone = AB_null ;
	lv_long_Fphone = AB_null ;

	pv_phone.removeTrailingBlanks() ;

	lv_email = pv_phone;

	pv_phone = pv_phone + "/" ;
	lv_long_Rphone = pv_phone.getNthBracketedField(1, "R/", "/") ;


	if ( lv_long_Rphone.NotIsNull() )
	{
		if ( lv_long_Rphone.length() > 1)
		{
			editPhoneNumber(lv_long_Rphone);

			if ( lv_long_Rphone.length() > 3 )
			{
				if ( !lv_long_Rphone.For(3).isAllNumeric())
				{
					lv_long_Rphone = lv_long_Rphone.At(4) ;
					lv_long_Rphone.removeTrailingBlanks() ;
				}
			}
			else
				lv_long_Rphone = AB_null ;

			if ( lv_long_Rphone.NotIsNull() )
			{
				if ( lv_long_Rphone.length() > 9 )
				{
					if ( lv_long_Rphone.At(1).For(10).isAllNumeric() )
					{
						md_IA_phone_acode[mi_IA_I] = lv_long_Rphone.At(1).For(3) ;
						md_IA_phone_no[mi_IA_I]   = lv_long_Rphone.At(4).For(7) ;
					}
				}
				else if ( lv_long_Rphone.length() > 6 )
				{
					if ( lv_long_Rphone.At(1).For(7).isAllNumeric())
						md_IA_phone_no[mi_IA_I]   = lv_long_Rphone.For(7) ;

				}
			}
		}
	}

	lv_long_Bphone = pv_phone.getNthBracketedField(1, "B/", "/") ;


	if ( lv_long_Bphone.NotIsNull() )
	{
		if ( lv_long_Bphone.length() > 1)
		{
			editPhoneNumber(lv_long_Bphone);

			if ( lv_long_Bphone.length() > 3 )
			{
				if ( !lv_long_Bphone.For(3).isAllNumeric())
				{
					lv_long_Bphone = lv_long_Bphone.At(4) ;
					lv_long_Bphone.removeTrailingBlanks() ;
				}
			}
			else
				lv_long_Bphone = AB_null ;

			if ( lv_long_Bphone.NotIsNull() )
			{
				if ( lv_long_Bphone.length() > 9 )
				{
					if ( lv_long_Bphone.At(1).For(10).isAllNumeric() )
					{
						md_IA_bphone_acode[mi_IA_I] = lv_long_Bphone.At(1).For(3) ;
						md_IA_bphone_no[mi_IA_I]   = lv_long_Bphone.At(4).For(7) ;
					}
				}
				else if ( lv_long_Bphone.length() > 6 )
				{
					if ( lv_long_Bphone.At(1).For(7).isAllNumeric())
						md_IA_bphone_no[mi_IA_I]   = lv_long_Bphone.For(7) ;

				}
			}
		}
	}

	lv_long_Mphone = pv_phone.getNthBracketedField(1, "M/", "/") ;

	if ( lv_long_Mphone.NotIsNull() )
	{
		if ( lv_long_Mphone.length() > 1)
		{
			editPhoneNumber(lv_long_Mphone);

			if ( lv_long_Mphone.length() > 3 )
			{
				if ( !lv_long_Mphone.For(3).isAllNumeric())
				{
					lv_long_Mphone = lv_long_Bphone.At(4) ;
					lv_long_Mphone.removeTrailingBlanks() ;
				}
			}
			else
				lv_long_Mphone = AB_null ;

			if ( lv_long_Mphone.NotIsNull() )
			{
				if ( lv_long_Mphone.length() > 9 )
				{
					if ( lv_long_Mphone.At(1).For(10).isAllNumeric() )
					{
						md_IA_mphone_acode[mi_IA_I] = lv_long_Mphone.At(1).For(3) ;
						md_IA_mphone_no[mi_IA_I]   = lv_long_Mphone.At(4).For(7) ;
					}
				}
				else if ( lv_long_Mphone.length() > 6 )
				{
					if ( lv_long_Mphone.At(1).For(7).isAllNumeric())
						md_IA_mphone_no[mi_IA_I]   = lv_long_Mphone.For(7) ;

				}
			}
		}
	}



	lv_long_Fphone = pv_phone.getNthBracketedField(1, "F/", "/") ;

	if ( lv_long_Fphone.NotIsNull() )
	{
		if ( lv_long_Fphone.length() > 1)
		{
			editPhoneNumber(lv_long_Fphone);

			if ( lv_long_Fphone.length() > 3 )
			{
				if ( !lv_long_Fphone.For(3).isAllNumeric())
				{
					lv_long_Fphone = lv_long_Fphone.At(4) ;
				}
			}
			else
				lv_long_Fphone = AB_null ;

			if ( lv_long_Fphone.NotIsNull() )
			{
				if ( lv_long_Fphone.length() > 9 )
				{
					if ( lv_long_Fphone.At(1).For(10).isAllNumeric() )
					{
						md_IA_fax_acode[mi_IA_I] = lv_long_Fphone.At(1).For(3) ;
						md_IA_fax_no[mi_IA_I]   = lv_long_Fphone.At(4).For(7) ;
					}
				}
				else if ( lv_long_Fphone.length() > 6 )
				{
					if ( lv_long_Fphone.At(1).For(7).isAllNumeric())
						md_IA_fax_no[mi_IA_I]   = lv_long_Fphone.For(7) ;

				}
			}
		}
	}

	LOG(logDEBUG) << endl << "EMAIL>> pv_phone = " << pv_phone << endl << flush;

	li_pos = lv_email.findSubstring( "E/", 1 );

	if ( li_pos == 7 )
	{
		lv_email = lv_email.At(9);

		if ( lv_email.NotIsNull() )
		{
			lv_email.substitute( "--", "_" );

			LOG(logDEBUG) << endl << "EMAIL>> lv_email = " << lv_email << endl << flush;

			li_pos = lv_email.findSubstring( "//", 1 );

			if ( li_pos > 0 )
			{
				lv_email_before   = lv_email.For( li_pos - 1 );
				lv_email_after    = lv_email.At( li_pos + 2 );

				if ( lv_email_before.NotIsNull() && lv_email_after.NotIsNull() )
				{
					lv_email =  lv_email_before + "@" + lv_email_after;
				}
			}

			if ( editEmailaddress( lv_email ) )
			{
				mc_EM_email_type[mi_EM_I]	= "C";
				mt_EM_email_id[mi_EM_I] 	= lv_email;
				mi_EM_I++;
			}
		}
	}
}
//------------------------------------------------------------------------------------------------------
void CTSS::condense_IrDocumentTax(D_passenger_no  &pd_pax_id, 
											 D_line_number   &pd_doc_line,
											 int &pi_taxCounter)
{
	ENTER;

   D_amount lm_US, lm_XF1, lm_XF2, lm_XF3, lm_XF4, lm_XT, lm_oth, lm_temp ;
   D_city   ld_cityXF1, ld_cityXF2, ld_cityXF3, ld_cityXF4 ;
   D_tax    lm_oth_id ;
   int      li_counter,li_XFCount ;

   li_counter = pi_taxCounter ;
   lm_US = AB_Decimal(0.00) ;
   lm_XF1 = AB_null ;
   lm_XF2 = AB_null ;
   lm_XF3 = AB_null ;
   lm_XF4 = AB_null ;
   lm_XT = AB_Decimal(0.00) ;
   lm_oth = AB_Decimal(0.00) ;
	li_XFCount = 1 ;
   

   while (li_counter < mi_ITD_I)
   {

		if ( md_ITD_pax_id[li_counter] == pd_pax_id && 
			  md_ITD_line_number[li_counter] == pd_doc_line )
		{

			lm_temp = md_ITD_tax_amount[li_counter] ;

			if (md_ITD_tax_id[li_counter] == "US")
				lm_US = lm_US + lm_temp ;
			else if (md_ITD_tax_id[li_counter] == "XF")
			{
				if ( li_XFCount == 1 )
				{
					lm_XF1 = lm_temp ;
					ld_cityXF1 = md_ITD_city_id[li_counter] ;
				}
				else if ( li_XFCount == 2 )
				{
					lm_XF2 = lm_temp ;
					ld_cityXF2 = md_ITD_city_id[li_counter] ;
				}
				else if ( li_XFCount == 3 )
				{
					lm_XF3 = lm_temp ;
					ld_cityXF3 = md_ITD_city_id[li_counter] ;
				}
				else if ( li_XFCount == 4 )
				{
					lm_XF4 = lm_temp ;
					ld_cityXF4 = md_ITD_city_id[li_counter] ;
				}

				li_XFCount++ ;
			}
			else
			{
				lm_oth = lm_oth + lm_temp ;
				lm_oth_id = "XT" ;
			}
		}

		li_counter++ ;
	}

   mi_ITD_I = pi_taxCounter ;

   if (lm_US != AB_Decimal(0.00))
   {
      md_ITD_pax_id[mi_ITD_I] = pd_pax_id ;
      md_ITD_line_number[mi_ITD_I] = pd_doc_line ;
      md_ITD_tax_amount[mi_ITD_I] = lm_US ;
      md_ITD_tax_id[mi_ITD_I++] = "US" ;
   }

   if (lm_XF1.NotIsNull() )
   {
      md_ITD_pax_id[mi_ITD_I] = pd_pax_id ;
      md_ITD_line_number[mi_ITD_I] = pd_doc_line ;
      md_ITD_tax_amount[mi_ITD_I] = lm_XF1 ;
		md_ITD_city_id[mi_ITD_I] = ld_cityXF1 ;
      md_ITD_tax_id[mi_ITD_I++] = "XF" ;
   }

   if (lm_XF2.NotIsNull() )
   {
      md_ITD_pax_id[mi_ITD_I] = pd_pax_id ;
      md_ITD_line_number[mi_ITD_I] = pd_doc_line ;
      md_ITD_tax_amount[mi_ITD_I] = lm_XF2 ;
		md_ITD_city_id[mi_ITD_I] = ld_cityXF2 ;
      md_ITD_tax_id[mi_ITD_I++] = "XF" ;
   }

   if (lm_XF3.NotIsNull() )
   {
      md_ITD_pax_id[mi_ITD_I] = pd_pax_id ;
      md_ITD_line_number[mi_ITD_I] = pd_doc_line ;
      md_ITD_tax_amount[mi_ITD_I] = lm_XF3 ;
		md_ITD_city_id[mi_ITD_I] = ld_cityXF3 ;
      md_ITD_tax_id[mi_ITD_I++] = "XF" ;
   }

   if (lm_XF4.NotIsNull() )
   {
      md_ITD_pax_id[mi_ITD_I] = pd_pax_id ;
      md_ITD_line_number[mi_ITD_I] = pd_doc_line ;
      md_ITD_tax_amount[mi_ITD_I] = lm_XF4 ;
		md_ITD_city_id[mi_ITD_I] = ld_cityXF4 ;
      md_ITD_tax_id[mi_ITD_I++] = "XF" ;
   }

   if (lm_oth != AB_Decimal(0.00))
   {
      md_ITD_pax_id[mi_ITD_I] = pd_pax_id ;
      md_ITD_line_number[mi_ITD_I] = pd_doc_line ;
      md_ITD_tax_amount[mi_ITD_I] = lm_oth ;
      md_ITD_tax_id[mi_ITD_I++] = "XT" ;
   }

	md_ITD_tax_id[mi_ITD_I] = AB_null ;

   EXIT; 
}

//------------------------------------------------------------------------------
void CTSS::processSFC()
{
	ENTER;

	AB_Char lc_svcDescription(20) ;

	lc_svcDescription = " ";

   /*
	mb_SfFound   = AB_true;
   mb_FeeFound  = AB_true;

   //mv_svcFEE = AB_Text("FEE-MR2/FP:CCAX370000000000028/D1200");
   //mv_svcFEE = AB_Text("FEE-MR2/FP:CA");
   //mv_svcFEE = AB_Text("FEE-MR2/FP:CK");
   //mv_svcFEE = AB_Text("FEE-MR2/CC");
   //mv_svcFEE = AB_Text("FEE-MR2/");
   //mv_svcFEE = AB_Text("FEE-MR2");
   //mv_svcFEE = AB_Text("FEE-");
   mv_svcFEE = AB_Text("FEE");

   mv_svcSF  = AB_Text("123.45/CC");
   */

	/**
	 * If both SF- and FEE- available, process service fee
	 **/

	if ( mb_SfFound && mb_FeeFound )
	{
		 // md_form_of_pay holds form of pay from A11

		 AB_Varchar lv_fopFromA11(2);
		 AB_Varchar lv_ccIdFromA11(18);
		 AB_Varchar lv_expDateFromA11(4);

		 lv_fopFromA11     = md_form_of_pay;
       lv_ccIdFromA11    = md_cc_id + md_cc_no.For(16);
		 lv_expDateFromA11 = md_cc_exp_date;

		 CMirSvcFee lo_MirSvcFee(mv_svcSF,
										 mv_svcFEE,
										 lv_fopFromA11,
										 lv_ccIdFromA11,
										 lv_expDateFromA11);

		 AB_Char    lc_amount(13);
		 AB_Char    lc_fop(2);
		 AB_Char    lc_supId(3);
		 AB_Char    lc_cc(18);
		 AB_Char    lc_expDate(4);
       AB_Integer li_status;

		 lo_MirSvcFee.print();

		 lo_MirSvcFee.getDetail(lc_amount,
										lc_fop,
										lc_supId,
										lc_cc,
										lc_expDate,
										li_status);

		 if ( li_status == AB_Integer(0) ) 
		 {
				// SCR 412640 Start
				bool lb_cc_valid = true;
				if (lc_fop == "CC")  {
					AB_Varchar lv_cc_id(40);
					long		  ll_error_code;
					lv_cc_id = lc_cc;
					if (!validCreditCard(lv_cc_id, lc_expDate, ll_error_code)) {
						lb_cc_valid = false;
					}
				}
				// SCR 412640 End

			  AB_Char  lc_accountLine(2) ;
			  mi_SFC_line_number++;
			  lc_accountLine = AB_Integer(mi_SFC_line_number);

		     md_IRMK_type[mi_IRMK_I] = "SFC" ;

		     md_IRMK_remark[mi_IRMK_I] = (lb_cc_valid? "" : "BADCC/") +
			  										AB_Text("APP") + /* CRS             */
                                       lc_fop       +  /* form of payment */
                                       lc_cc        +
													lc_expDate   +
                                       AB_Text("  ")+  /* PER ALL pax_id  */
												 	lc_svcDescription +
													lc_amount    +  /* amount          */
													lc_accountLine;

		     md_IRMK_pax_id[mi_IRMK_I++] = AB_null ;
		 }
	}
   EXIT; 
}

//------------------------------------------------------------------------------

void CTSS::saveSvcIrregularity()
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

