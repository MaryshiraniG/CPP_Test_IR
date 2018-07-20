/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996

                SID: 1.5
      Delta Created: 14/11/04 - 14:29:54
         Last Delta: app/src/common/InterfaceRecord/_us/ 11/4/14 - 14:29:54 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TRMTAIR16.cpp)
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us/}[11/4/14-14:29:54 1.5] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TRMTAIR16.cpp (Ruma Sharma)\n" )
/*
  The contents of this file, its programs, all comments, code and information
               are the sole property of GLOBAL TRAVEL Inc.
     No duplication by any method, electronic or otherwise is permitted.
============================================================ END GLOBAL BANNER*/

#include "C.h"
#include "gtypes.h"
#include "TRMTAIR_D.h"
#include "Money.h"
#include "matsysT.h"
#include "MsgHandler.h"
#include "dbraw.h"
#include "PNRDetail.h"
#include "dbMatrix.h"
#include "PNRWS.h"
#include "dbIR.h"
#include "InterfaceRecord_D.h"
#include "InterfaceRecord.h"
#include "Address.h"
#include "TAIR16.h"
#include "TRMTAIR16.h"

CTRMTAIR16::CTRMTAIR16 ()
{
   // LOG(logDEBUG) << "CREATE TRMTRMTAIR OBJECT" << endl ;
}

void CTRMTAIR16::processRMA(D_session_id &pd_sessionId,
   						D_client_file_type  &pd_cfa_type,
   						D_client_file_code  &pd_cfa_code,
   						D_client_file_no    &pd_cfa_id,
   						D_person_name       &pd_last_name,
   						D_person_name       &pd_first_name,
   						D_consultant        &pd_con_id,
							D_company           &pd_cfaCop,
							D_branch            &pd_cfaBra) 
{
	CPNRWS             lo_PNR ;
	CPNRDetail         lo_PNRDetail ;
	CGirrawdetailRead  lo_GirrawRead ;	
	CDGirrawdetail     lo_DGirrawdetail ;	

	AB_Integer			 li_subsect_id = AB_Integer(0) ;
	AB_Integer			 li_line_number  = AB_Integer(0);
	AB_Integer			 li_counter  = AB_Integer(0);
	AB_Char   			 lc_more_data  = "Y";
	int					 li_index = 0 ;
	int					 li_tot_segment = 0 ;
	int                i = 0 ;

	lo_PNR.setSessionId(pd_sessionId) ;

	while ( lc_more_data == "Y" )
	{
		lo_GirrawRead.pc_session_id = pd_sessionId ;
		lo_GirrawRead.pi_subsect_id = li_subsect_id ;
		lo_GirrawRead.pi_line_number = li_line_number ;

		initGirrawdetail(lo_GirrawRead) ;
		
		lo_DGirrawdetail.read(lo_GirrawRead) ;

		lc_more_data = lo_GirrawRead.pc_more_data ;
		li_subsect_id = lo_GirrawRead.pi_key_subsect_id ;
		li_line_number = lo_GirrawRead.pi_key_line_number ;

		if (li_counter == AB_Integer(0))
		{
			D_update_date ld_creation_date ;
			D_interface_type ld_interface_type ;

			ld_creation_date = lo_GirrawRead.pd_interface_cdate ;
			ld_interface_type = lo_GirrawRead.pc_interface_type ;

			lo_PNR.setInterfaceType(ld_interface_type) ;
			lo_PNR.setCreationDate(ld_creation_date) ;
			lo_PNR.setSessionId(pd_sessionId) ;
			LOG(logDEBUG) << "SESSION ID SET " << pd_sessionId  << endl;
		}

		li_counter = lo_GirrawRead.pi_counter.to_long() ;
		li_tot_segment = 
		lo_GirrawRead.pi_counter.to_long() + li_tot_segment ;

		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec1,
								lo_GirrawRead.sub1,
								lo_GirrawRead.lno1,
								lo_GirrawRead.det1);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;

		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec2,
								lo_GirrawRead.sub2,
								lo_GirrawRead.lno2,
								lo_GirrawRead.det2);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec3,
								lo_GirrawRead.sub3,
								lo_GirrawRead.lno3,
								lo_GirrawRead.det3);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec4,
								lo_GirrawRead.sub4,
								lo_GirrawRead.lno4,
								lo_GirrawRead.det4);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;

		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec5,
								lo_GirrawRead.sub5,
								lo_GirrawRead.lno5,
								lo_GirrawRead.det5);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;

		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec6,
								lo_GirrawRead.sub6,
								lo_GirrawRead.lno6,
								lo_GirrawRead.det6);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec7,
								lo_GirrawRead.sub7,
								lo_GirrawRead.lno7,
								lo_GirrawRead.det7);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec8,
								lo_GirrawRead.sub8,
								lo_GirrawRead.lno8,
								lo_GirrawRead.det8);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec9,
								lo_GirrawRead.sub9,
								lo_GirrawRead.lno9,
								lo_GirrawRead.det9);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec10,
								lo_GirrawRead.sub10,
								lo_GirrawRead.lno10,
								lo_GirrawRead.det10);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec11,
								lo_GirrawRead.sub11,
								lo_GirrawRead.lno11,
								lo_GirrawRead.det11);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec12,
								lo_GirrawRead.sub12,
								lo_GirrawRead.lno12,
								lo_GirrawRead.det12);

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec13,
								lo_GirrawRead.sub13,
								lo_GirrawRead.lno13,
								lo_GirrawRead.det13);

		LOG(logDEBUG) << "DATA 13" << lo_GirrawRead.det13 << endl;

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec14,
								lo_GirrawRead.sub14,
								lo_GirrawRead.lno14,
								lo_GirrawRead.det14);
		LOG(logDEBUG) << "DATA 14" << lo_GirrawRead.det14 << endl;

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       
		lo_PNR.setPNRDetail(lo_PNRDetail,
								lo_GirrawRead.sec15,
								lo_GirrawRead.sub15,
								lo_GirrawRead.lno15,
								lo_GirrawRead.det15);
		LOG(logDEBUG) << "DATA 15" << lo_GirrawRead.det15 << endl;

		lo_PNR.getPNRDetail(lo_PNRDetail) ;
       

		if (lc_more_data == "N" )
			break ;
					
	}

	// printAllsegment() ;
	pd_cfa_type   = AB_null;
	pd_cfa_code   = AB_null;
	pd_cfa_id     = AB_null;
	pd_last_name  = AB_null;
	pd_first_name = AB_null;
	pd_con_id     = AB_null;

	createTRMTAIR(lo_PNR,pd_cfa_type,pd_cfa_code,pd_cfa_id,
					pd_last_name,pd_first_name,pd_con_id);

	LOG(logDEBUG) << "CFA TYPE" << pd_cfa_type << endl ;
	LOG(logDEBUG) << "CFA CODE" << pd_cfa_code << endl ;
	LOG(logDEBUG) << "CFA ID" << pd_cfa_id << endl ;
	LOG(logDEBUG) << "LAST NAME " << pd_last_name << endl ;
	LOG(logDEBUG) << "FIRST NAME " << pd_first_name << endl ;
	LOG(logDEBUG) << "CON ID " << pd_con_id << endl ;
	pd_cfaCop = md_rejectCop ;
	pd_cfaBra = md_rejectBra ;
	printAll() ;

}

//---------------------------------------------------------------------------------------------------------------------------
void CTRMTAIR16::createTRMTAIR(CPNRWS				  &po_PNR,
   								D_client_file_type  &pd_cfa_type,
   								D_client_file_code  &pd_cfa_code,
   								D_client_file_no    &pd_cfa_id,
   								D_person_name       &pd_last_name,
   								D_person_name       &pd_first_name,
   								D_consultant        &pd_con_id)
{
   CDMatrix lo_Matrix;
   AB_Integer li_sqlcode;

   // LOG(logDEBUG) << "START TAIR PROCESSING" << endl ;

   if(CMsgHandler::getMsgStatus())
   	fill_IrHeader(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		populate_IrAddress(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		fill_IrPasDocument(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		fillIrTransport(po_PNR) ;

	if (CMsgHandler::getMsgStatus())
		testTripValue() ;

	if (CMsgHandler::getMsgStatus())
		updateIcdWithFareByLeg() ;

   if (CMsgHandler::getMsgStatus())
		fillIrAmtrak(po_PNR) ;

   // 10APR99 - SC - shift connection ticket segment stopover codes
   //                which are signed in fillIrTransport and fillIrAmtrak.
   if (CMsgHandler::getMsgStatus())
      shiftConnectInd() ;

   if (CMsgHandler::getMsgStatus())
		fillIrHotel_H(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		fillIrCar_I(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		processTVLSegment(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		createARNK() ;

   // US044 START
   if (CMsgHandler::getMsgStatus())
      IrTransportRemarks(po_PNR);
   // US044 END

	md_queType = "RMA" ;
	
   if (CMsgHandler::getMsgStatus())
   	createIrRows() ;
   
   if (! CMsgHandler::getMsgStatus())
   {
      lo_Matrix.dbRollback(li_sqlcode) ;
      errorHandler() ;
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

   // LOG(logDEBUG) << "*****WRITE GIR TABLES*****" << endl ;
   
	md_queType = "RMI" ;
   checkDomainEdits() ;

   if (! CMsgHandler::getMsgStatus())
	{
      errorHandler() ;
      return ;
	}

  	createClientfile() ;

   if (! CMsgHandler::getMsgStatus())
   {
      // LOG(logDEBUG) << "NEW V1.0" << endl ;
      lo_Matrix.dbRollback(li_sqlcode);
      errorHandler() ;
   }
   else
   {		 
      if ( mi_rejectirId > AB_Integer(0) )
      {
          // Insert captured complains
          // into Rejectir

          saveSvcIrregularity();
      }
		pd_cfa_type   = md_cfa_type;
		pd_cfa_code   = md_cfa_code;
		pd_cfa_id     = md_cfa_id;
		pd_last_name  = md_last_name;
		pd_first_name = md_first_name;
		pd_con_id     = md_con_id;

   	LOG(logDEBUG) << "*****S U C C E S S  -  CREATING REAL DATA ROWS*****" << endl ;
   }
	moveReceiptQueues( mv_rec_queType );
   lo_Matrix.dbCommit(li_sqlcode);
}

//-----------------------------------------------------------------------------

void CTRMTAIR16::errorHandler()
{
   D_description   ld_errorDescription ;
   CDMatrix        lo_Matrix;
   AB_Integer      li_sqlcode;
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
	   //																 AB_Boolean(AB_true)) ; 
	 
      CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;
	}

	if ( md_queType == "RMA" )
	{
		if ( li_msgId == AB_Integer(9887) )
		{
			CMsgHandler::clearMsg();
			setMsg(ld_errorDescription) ;
		}

		return ;
	}

   createQueue(ld_errorDescription) ;

   lo_Matrix.dbCommit(li_sqlcode);
}

//--------------------------------------------------------------------------------------------
void CTRMTAIR16::initGirrawdetail(CGirrawdetailRead  &po_GirrawRead) 
{
	
		po_GirrawRead.sec1 = AB_null ;
		po_GirrawRead.sub1 = AB_null ;
		po_GirrawRead.lno1 = AB_null ;
		po_GirrawRead.det1 = AB_null ;
		po_GirrawRead.sec2 = AB_null ;
		po_GirrawRead.sub2 = AB_null ;
		po_GirrawRead.lno2 = AB_null ;
		po_GirrawRead.det2 = AB_null ;
		po_GirrawRead.sec3 = AB_null ;
		po_GirrawRead.sub3 = AB_null ;
		po_GirrawRead.lno3 = AB_null ;
		po_GirrawRead.det3 = AB_null ;
		po_GirrawRead.sec4 = AB_null ;
		po_GirrawRead.sub4 = AB_null ;
		po_GirrawRead.lno4 = AB_null ;
		po_GirrawRead.det4 = AB_null ;
		po_GirrawRead.sec5 = AB_null ;
		po_GirrawRead.sub5 = AB_null ;
		po_GirrawRead.lno5 = AB_null ;
		po_GirrawRead.det5 = AB_null ;
		po_GirrawRead.sec6 = AB_null ;
		po_GirrawRead.sub6 = AB_null ;
		po_GirrawRead.lno6 = AB_null ;
		po_GirrawRead.det6 = AB_null ;
		po_GirrawRead.sec7 = AB_null ;
		po_GirrawRead.sub7 = AB_null ;
		po_GirrawRead.lno7 = AB_null ;
		po_GirrawRead.det7 = AB_null ;
		po_GirrawRead.sec8 = AB_null ;
		po_GirrawRead.sub8 = AB_null ;
		po_GirrawRead.lno8 = AB_null ;
		po_GirrawRead.det8 = AB_null ;
		po_GirrawRead.sec9 = AB_null ;
		po_GirrawRead.sub9 = AB_null ;
		po_GirrawRead.lno9 = AB_null ;
		po_GirrawRead.det9 = AB_null ;
		po_GirrawRead.sec10 = AB_null ;
		po_GirrawRead.sub10 = AB_null ;
		po_GirrawRead.lno10 = AB_null ;
		po_GirrawRead.det10 = AB_null ;
		po_GirrawRead.sec11 = AB_null ;
		po_GirrawRead.sub11 = AB_null ;
		po_GirrawRead.lno11 = AB_null ;
		po_GirrawRead.det11 = AB_null ;
		po_GirrawRead.sec12 = AB_null ;
		po_GirrawRead.sub12 = AB_null ;
		po_GirrawRead.lno12 = AB_null ;
		po_GirrawRead.det12 = AB_null ;
		po_GirrawRead.sec13 = AB_null ;
		po_GirrawRead.sub13 = AB_null ;
		po_GirrawRead.lno13 = AB_null ;
		po_GirrawRead.det13 = AB_null ;
		po_GirrawRead.sec14 = AB_null ;
		po_GirrawRead.sub14 = AB_null ;
		po_GirrawRead.lno14 = AB_null ;
		po_GirrawRead.det14 = AB_null ;
		po_GirrawRead.sec15 = AB_null ;
		po_GirrawRead.sub15 = AB_null ;
		po_GirrawRead.lno15 = AB_null ;
		po_GirrawRead.det15 = AB_null ;
}

//------------------------------------------------------------------------------

void CTRMTAIR16::saveSvcIrregularity()
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
