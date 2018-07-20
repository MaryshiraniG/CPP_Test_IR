/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.248
      Delta Created: 17/11/01 - 13:49:27
         Last Delta: app/src/common/InterfaceRecord/_us 11/1/17 - 13:49:27 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.MIR.cpp)
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us}[11/1/17-13:49:27 1.248] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.MIR.cpp (Jinsong Liu)\n" )
/*
  The contents of this file, its programs, all comments, code and information
               are the sole property of GLOBAL TRAVEL Inc.
     No duplication by any method, electronic or otherwise is permitted.
============================================================ END GLOBAL BANNER*/

/*
PURPOSE:
============================================================================
CHANGE HISTORY: ( PLEASE PUT MOST RECENT CHANGE ENTRY ALWAYS ON TOP )
============================================================================
DATE:       SCR#:   TASKID: INIT DESCRIPTION
-------     ------  ------- ---- ------------------------------------------------
27OCT2017   US499   v8617    MG  Including comm_rate from HS Remark
13OCT2017   US500   v8617    MG  Including Preferred & commissionable from A14 Segment
24MAY2017   US314   v8617    MG  Included the H- segment parsing logic under A167
23MAY2017   US250   v8617    MG  Modified the MIR.cpp to include RQ- seg under A16 
29OCT2014   519830  c1575    JL  fixing addIrconsumedetail to create costitin so that the line no is taken into account in addition to sup_id
29SEP2014   518873  c1575    JL  fixing processing MUL option for non ArcDoc (opposite), and comment below adjacent checking because business spec changes.
26SPE2014   518873  c1556    JL  processing MUL option for non ArcDoc 
24SPE2014   518873  c1556    JL  processing MUL option for non ArcDoc 
23SPE2014   518873  c1556    JL  processing multi line remark (Adjacent).
22SPE2014   518868  c1556    JL  don't create limo itin if unt_cn = 0.
30JUL2014   517902  v7966    JL  adding remark parsing for VFT/VFC. see also AIR/IUR change.
09MAY2014   516402  c1483    WW  new optional field(DDL:ddMMMYY) Airline(A04) start_date
30APR2014   516462  c1433    SL  data standardization p3, 516462                 
11APR2014   516742  c1413    JL  Foreign Hotel Booking: create Irremark for cur_id/local_rate override.
24MAR2014   516409  v7848    WW  Parse Limo Segment
11SEP2013   514016  c1195    JL  adding "MB" for mobile booking.
05SEP2013   514142  v7727    WW  Parse Rental.service_status for Car
17AUG2012	510686  v7467	  WW  Create SFM for specificed pax
22MAR2012   503909  v7388    WW  Only for US Matrix, add "per person" function
                                    for service fee PNR remark
============================================================================
*/

#include "gtypes.h"
#include "MIR_D.h"

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
#include "MirSvcFee.h"
#include "MIR.h"
#include "Log.h"
#include "pcre_api.h"

#define ENTER      LOG(logINFO) << "\n*** Enter>> " << __FUNCTION__
#define EXIT       LOG(logINFO) << "\n*** Exit << " << __FUNCTION__
#define A14_CONTINUE lt_segment = po_PNR.getNextSegment("A14"); continue;

//-----------------------------------------------------------------------------

void CMIR::printAll()
{
   LOG(logDEBUG) << "START GIRHEADER PRINT"  ;
   print_Irtripheader() ;
   LOG(logDEBUG) << "END GIRHEADER PRINT"  ;

   LOG(logDEBUG) << "START IRADDRESS PRINT"  ;
   printIraddress() ;
   LOG(logDEBUG) << "END IRADDRESS PRINT"  ;

   LOG(logDEBUG) << "START IRPAYMENT PRINT"  ;
   printIrpayment() ;
   LOG(logDEBUG) << "END IRPAYMENT PRINT"  ;

   LOG(logDEBUG) << "START GIRDOCUMENTTAX PRINT"  ;
   printIrtaxdetail() ;
   LOG(logDEBUG) << "END GIRDOCUMENTTAX PRINT"  ;

   LOG(logDEBUG) << "START GIRFREQUSER PRINT "  ;
   printFreqUser() ;
   LOG(logDEBUG) << "END GIRFREQUSER PRINT"  ;

   LOG(logDEBUG) << "START GIROTHER PRINT"  ;
   printOther() ;
   LOG(logDEBUG) << "END GIROTHER PRINT" ;

   LOG(logDEBUG) << "START IRPERSON PRINT"  ;
   printIrperson() ;
   LOG(logDEBUG) << "END IRPERSON PRINT"  ;

   LOG(logDEBUG) << "START IRTICKET PRINT"  ;
   printIrticket() ;
   LOG(logDEBUG) << "END IRTICKET PRINT"  ;

   LOG(logDEBUG) << "START IRTAX PRINT"  ;
   printIrtaxdetail() ;
   LOG(logDEBUG) << "END IRTAX PRINT"  ;

   LOG(logDEBUG) << "START GIRPERSONITIN PRINT"  ;
   printIrconsumdetail() ;
   LOG(logDEBUG) << "END GIRPERSONITIN PRINT"  ;

   LOG(logDEBUG) << "START GIRREMARK PRINT - TRANSPORT"  ;
   printIrremark() ;
   LOG(logDEBUG) << "END GIRREMARK PRINT - TRANSPORT"  ;

   LOG(logDEBUG) << "START GIRRENTAL PRINT"  ;
   print_Irrental() ;
   LOG(logDEBUG) << "END GIRRENTAL PRINT"  ;

   LOG(logDEBUG) << "START GIRTRANSPORT PRINT"  ;
   printIrtransport() ;
   LOG(logDEBUG) << "END GIRTRANSPORT PRINT"  ;

	LOG(logDEBUG) << "START GIRRECEIPT PRINT"  ;
	printIrreceipt() ;
	LOG(logDEBUG) << "END GIRRECEIPT PRINT"  ;

	LOG(logDEBUG) << "START GIREMAILADDRESS PRINT"  ;
	printIremailaddress() ;
	LOG(logDEBUG) << "END GIREMAILADDRESS PRINT"  ;

}

CMIR::CMIR () :mv_baggage(10),mv_service_status(10),mv_IrCarData(255,100),
					mv_manualTicketData(255,100),mv_IrHotelData(255,100),
					mv_exchTaxData(255,100),mv_a14CC(100),md_nonArcDoc(255,100),
					md_nonArcCost(255,100),mv_NonArcCC(25),
					mv_svcSF(100),mv_svcFEE(100)
{
   Log::ReportingLevel() = PARSER_LOG_LEVEL; // logDEBUG; PARSER_LOG_LEVEL;
   LOG(logDEBUG) << "CREATE MIR OBJECT"  ;
	mi_XDLine = 50; // Used to create Other Itinerary by service charge MCO ;
	mi_MCO_itin_line_number = -1;
	mi_SFC_line_number = 0;
}

//---------------------------------------------------------------------------

void CMIR::errorHandler(CPNR &po_PNR)
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

//------------------------------------------------------------------------------

void CMIR::createCMIR(CPNR &po_PNR)
{
   CDMatrix lo_Matrix;
   AB_Integer li_sqlcode;

   // LOG(logDEBUG) << "START MIR PROCESSING"  ;
	/* REMARK_PARSER: parse ALL remarks */
	RP_parseRemarks(po_PNR);

   fill_IrHeader(po_PNR) ;

	if ( mb_ignoreMir )
		return ;

   if (CMsgHandler::getMsgStatus())
      createIrItins(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
      fill_IrPasDocument(po_PNR); 

   if (CMsgHandler::getMsgStatus())
      processNonArcDocFromACCremark(); 

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

   if (CMsgHandler::getMsgStatus()) {
      fill_IrTASF(po_PNR);
	}

	// G9015 - Service Fee Charge
	if (CMsgHandler::getMsgStatus())
		processSFC() ;

   if (CMsgHandler::getMsgStatus()) {
      processEMD(po_PNR);
	}

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
   
	printAll();

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

   LOG(logDEBUG) << "*****WRITE GIR TABLES*****"  ;
   
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
		LOG(logERROR) << "*****S U C C E S S  -  CREATING REAL DATA ROWS*****"  ;
   }
	moveReceiptQueues( mv_rec_queType );
	lo_Matrix.dbCommit(li_sqlcode);

}

//------------------------------------------------------------------------------

void CMIR::fill_IrHeader(CPNR &po_PNR)
{
   // LOG(logDEBUG) << "START CMIR::fill_IrHeade"  ;

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
	mb_MCO		      = AB_false ;
	mb_MCO_TASF		   = AB_false ;
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
      CMsgHandler::setMsg(AB_Integer(2509), AB_Char(15," "), D_error_track("M92-PROCHDR"), AB_Integer(1), AB_Varchar(20,AB_null));
      return ;
   }
	else if ( lt_total_segment.At(248).For(1) == "6") 
	{
		// COUPON PRINT FOR VIRTUAL TICKER
		// Dont process this MIR
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

	// SCR 303096 START

   if (! CMsgHandler::getMsgStatus())
      return ;
 
	// SCR 303096 END

   populate_IrHeader_SectionA14(lv_long_address, po_PNR) ;

	fill_Iraddress(lv_long_address,po_PNR) ;

}

//------------------------------------------------------------------------------

void CMIR::populate_IrHeader_SectionHDR(AB_Text &pt_total_segment)
{
    LOG(logDEBUG) << "START CMIR::populate_IrHeader_SectionHDR"   ;

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
    * 12MAY2010 - spec change for switch
    *               on service bureau indicator.
	 *
	 * NOTE: The positions do not match the spec,
	 * but match what they give us in the PNR.
	 * According to Gord, the position(s) in the PNR
	 * overrides whats in the spec.
    ************************************/
	if ( pt_total_segment.At(82).For(4) != "    " )
   	md_IRT_booking_pcc = pt_total_segment.At(82).For(4);

	if (  pt_total_segment.At(118).For(2) != "  " ){
      if ( pt_total_segment.At(120).For(1) == "N" ){
   			md_IRT_booking_gds_agent = "N" + pt_total_segment.At(118).For(2);
		}
		else if ( pt_total_segment.At(120).For(1) == "Y" ){
            md_IRT_booking_gds_agent = "S" + pt_total_segment.At(118).For(2);
      }
		else{
				md_IRT_booking_gds_agent = pt_total_segment.At(118).For(2);
		}
   }

	if ( pt_total_segment.At(86).For(4) != "    " )
   	md_IRT_ticketing_pcc = pt_total_segment.At(86).For(4);

	if ( pt_total_segment.At(121).For(2) != "  " )
		md_IRT_ticketing_gds_agent = pt_total_segment.At(121).For(2);

   LOG(logDEBUG) << " -- md_IRT_booking_pcc         = |" << md_IRT_booking_pcc         << "|" ;
   LOG(logDEBUG) << " -- md_IRT_booking_gds_agent   = |" << md_IRT_booking_gds_agent   << "|" ;
   LOG(logDEBUG) << " -- md_IRT_ticketing_pcc       = |" << md_IRT_ticketing_pcc       << "|" ;
   LOG(logDEBUG) << " -- md_IRT_ticketing_gds_agent = |" << md_IRT_ticketing_gds_agent << "|" ;

   // ROW : RECORD CREATION DATE
	// Get 5 char Date
	md_IRT_record_creation_date = pt_total_segment.At(21).For(7);
	LOG(logDEBUG) << "AM====RCD_date=[" << md_IRT_record_creation_date << "]";
        // G9150, only this is set then we can get correct year
        mt_pnrDate = md_IRT_record_creation_date;

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

   if (pt_total_segment.For(4) == "T51V")
      md_IRT_crs_id = "APP" ;
   else if (pt_total_segment.For(4) == "T51C")
      md_IRT_crs_id = "APG" ;
   else if (pt_total_segment.For(4) == "T51G")
      md_IRT_crs_id = "GAL" ;

	/* 
	SCR 516802 - commenting out old TASF
	// if the record T50IN6 = 7, it is a MCO
	if (pt_total_segment.At(248).For(1) == "7") {
		mb_MCO = AB_true;
      md_e_ticket = "Y";
		if (pt_total_segment.At(33).For(2) == "XD") {
			mb_MCO_TASF = AB_true;
		}
	}
   SCR 516802 END 
	*/

   // LOG(logDEBUG) << "END CMIR::populate_IrHeader_SectionHDR"  ;
}

//------------------------------------------------------------------------------

void CMIR::storeFormOfPay(AB_Text &pt_total_segment)
{
   ENTER; 

	LOG(logDEBUG) << "FOP data:" << pt_total_segment ;

   md_form_of_pay = pt_total_segment.At(4).For(2) ; // Sets Form Of Pay
   md_A11FOP = md_form_of_pay;
	LOG(logDEBUG) << "md_A11FOP:"<< md_A11FOP  ;

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
		LOG(logDEBUG)  << "WE ARE IN GR" ;
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

	// SCR 303096 START
	if ( md_form_of_pay == "CC" )
	{
		md_cc_id.removeLeadAndTrailBlanks();
		md_cc_no.removeLeadAndTrailBlanks();
		md_cc_exp_date.removeLeadAndTrailBlanks();

		if ( md_cc_id.IsNull() ) 
		{
			CMsgHandler::setMsg(102, " ", "FOP-PARSE", 1, AB_Varchar(20,AB_null));
			return;
		}

		if ( md_cc_no.IsNull() )
		{
			CMsgHandler::setMsg(102, " ", "FOP-PARSE", 2, AB_Varchar(20,AB_null));
			return;
		}

		if ( md_cc_exp_date.NotIsNull() && ( ! md_cc_exp_date.isAllNumeric() ) )
		{
			CMsgHandler::setMsg(102, " ", "FOP-PARSE", 3, AB_Varchar(20,AB_null));
			return;
		}
	}
	// SCR 303096 END

   EXIT; 
}

//------------------------------------------------------------------------------

void CMIR::fill_Irpayment(AB_Text    &pt_total_segment,
								  AB_Integer &pi_paxId,
								  AB_Integer &pi_docLine)
{
	ENTER;
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

//------------------------------------------------------------------------------

void CMIR::fill_Iraddress(AB_Varchar &pv_long_address,
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

//------------------------------------------------------------------------------

void CMIR::populate_Iraddress_SectionA12(CPNR &po_PNR)
{
   // LOG(logDEBUG) << "START CMIR::populate_IrHeader_SectionA12"  ; 

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
 
		LOG(logDEBUG) << "lv_phone: " << lv_phone  ;

		parsePhone(lv_phone) ;

   LOG(logDEBUG) << "GOING TO GET NEXT A12 SEGMENT"  ;

      lv_phone = po_PNR.getNextSegment("A12") ;
   }

	overrideHomePhone();

	// Most of the time Fax comes as non numeric no.
	// and fax is used only in MML

   md_IA_fax_acode[mi_IA_I] = AB_null ;
   md_IA_fax_no[mi_IA_I]    = AB_null ;

	if (md_IA_phone_no[mi_IA_I].NotIsNull() || md_IA_bphone_no[mi_IA_I].NotIsNull() ||md_IA_fax_no[mi_IA_I].NotIsNull() || md_IA_mphone_no[mi_IA_I].NotIsNull() )
		mb_address = AB_true ;


	if (mb_address)
		md_IA_type[mi_IA_I++] = "P" ;
   LOG(logDEBUG) << "END CMIR::populate_IrHeader_SectionA12"  ;
}

//-----------------------------------------------------------------------------

void CMIR::populate_IrHeader_SectionA14(AB_Varchar &pv_long_address, 
													 CPNR &po_PNR)
{
   // LOG(logDEBUG) << "START CMIR::populate_IrHeader_SectionA14"  ;

   AB_Char    lc_delimiter(2) ;
   AB_Varchar lv_delimiterBeforeSlash(10) ;
	AB_Char    lc_CONFdelimiter(5) ;
   int        li_pos, 
              li_counter,
				  li_slashPos;
   AB_Varchar lv_data(80) ;
	AB_Text	  lt_data;
   AB_Varchar lv_dataAfterSlash(180) ;
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
	bool			lb_fo_saving_code_found = false;

	// GAP US107 START
	AB_Boolean lb_tu_found = AB_false;
	AB_Boolean lb_rf_found = AB_false;
	AB_Boolean lb_gf_found = AB_false;
	AB_Boolean lb_sr_found = AB_false;
	AB_Boolean lb_bf_found = AB_false;
	AB_Boolean lb_ri_found = AB_false;
	AB_Boolean lb_nm_found = AB_false;
	AB_Boolean lb_lt_found = AB_false;
	AB_Boolean lb_ms_found = AB_false;
	AB_Boolean lb_it_found = AB_false;
	AB_Boolean lb_rs_found = AB_false;
	AB_Boolean lb_if_found = AB_false;
	AB_Boolean lb_tn_found = AB_false;
	// GAP US107 END

	bool		  lb_cnf_tkt_found = false;

	AB_Integer	li_FEE_MAX(0);

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

	int			trans_count = 2;
	char*			orig_string[]={	
											"--",	/* 1 */
											"//"	/* 2 */
										};
	
	char*			trans_string[]={	
											"_",	/* 1 */
											"@"	/* 2 */
										};

   // TKX CC EXP DATE START
   AB_Char lc_ccExpDate(4);
   md_TKX_cc_exp_date = AB_null;
   // TKX CC EXP DATE END

   po_PNR.resetSegment("A14") ;
   lt_segment = po_PNR.getNextSegment("A14") ;

	/*
	// SCR: 518673
	AB_Varchar lv_acc_name(80);
	lv_acc_name = "";
	AB_Varchar lv_prv_acc(10);
	lv_prv_acc = "";
	AB_Boolean lb_acc_bad;
  	// SCR: 518673
	*/

	while (lt_segment.NotIsNull())
   {
	/*
	 // SCR: 518673
		if (lt_segment.At(6).For(3) == "ACC") {
				//LOG(logDEBUG)  << "DEBUG found ACC remark";
				int li_pos = lt_segment.At(10).findSubstring("/");
				if (li_pos > 0) {
					AB_Varchar tmp_acc_name(80);
					tmp_acc_name = lt_segment.At(10).For(li_pos-1);
					//LOG(logDEBUG)  << "DEBUG li_pos " << li_pos;
					//LOG(logDEBUG)  << "DEBUG remark name " << lv_acc_name;

					if (lv_prv_acc == "") {
						lb_acc_bad = AB_false;
						lv_acc_name = lv_acc_name + "/";
						lv_acc_name = lv_acc_name + tmp_acc_name;
						//LOG(logDEBUG)  << "DEBUG first remark";
							}
						else {
							if (lv_prv_acc == tmp_acc_name) {  // adjacent lines.
								lb_acc_bad = AB_false;
								//LOG(logDEBUG)  << "DEBUG adjacent remark";
										}
							else {
									AB_Text lt_text = lv_acc_name;
									int li_count;
									V_AB_Varchar      lva_arrayField(100,100) ;
									li_count = lva_arrayField.parseStringMultDel(lv_acc_name, "/");
									for (int li_tmp = 1; li_tmp <= li_count; li_tmp++) {
										if (lva_arrayField[li_tmp] == tmp_acc_name) {
												lb_acc_bad = AB_true;
												break;
												}
										}
									if (lb_acc_bad  == AB_false) {
											lv_acc_name = lv_acc_name + "/";
											lv_acc_name = lv_acc_name + tmp_acc_name;
											//LOG(logDEBUG)  << "DEBUG new remark";
										}
									}
						}
						lv_prv_acc = tmp_acc_name;	
						}
				//LOG(logDEBUG)  << "DEBUG: total accline " << lv_acc_name;
				if (lb_acc_bad == AB_true)   {
					//LOG(logDEBUG)  << "DEBUG discarding corrupted accline " << lt_segment;
					lb_acc_bad = AB_false;
					A14_CONTINUE;
					continue;
					}
				}
		else
			lv_prv_acc = "";
		// SCR: 518673
		*/


      lc_delimiter = lt_segment.At(6).For(2) ;
      lv_data      = lt_segment.At(9) ;
      lt_data      = lt_segment.At(9) ;

		// ***REMARK_PARSER***
		// moved to generic pRemarkParser->parse(lt_segment.At(6).to_string());

      li_slashPos   = lt_segment.At(6).findSubstring("/") ;
		if ( li_slashPos > 0 )
      {
			lv_delimiterBeforeSlash = lt_segment.At(6).For(li_slashPos-1) ;
			lv_dataAfterSlash       = lt_segment.At(6+li_slashPos) ;

			li_pos       = lv_dataAfterSlash.findSubstring("!") ;
			if (li_pos > 0)
				lv_dataAfterSlash = lv_dataAfterSlash.For(li_pos - 1) ;
		}
		else
		{
			lv_delimiterBeforeSlash = "";
			lv_dataAfterSlash       = "";
		}
		LOG(logDEBUG) << "AM======== lt_segment=[" << lt_segment << "]"  ;
		LOG(logDEBUG) << "AM======== lv_delimiterBeforeSlash=[" << lv_delimiterBeforeSlash << "]"  ;
		LOG(logDEBUG) << "AM======== lv_dataAfterSlash=[" << lv_dataAfterSlash << "]"  ;

		lc_CONFdelimiter = lt_segment.At(6).For(5) ;

      li_pos       = lv_data.findSubstring("!") ;
      if (li_pos > 0)
         lv_data = lv_data.For(li_pos - 1) ;

      li_pos       = lt_data.findSubstring("!") ;
      if (li_pos > 0)
         lt_data = lt_data.For(li_pos - 1) ;

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

      // GAP G9126/G9136 start
      else if (lt_segment.At(6).For(4) == "NFT/" && !lb_cnf_tkt_found) {
	parseRemarkCNF(lt_segment.At(10));
	lb_cnf_tkt_found = true;
      }
      // GAP G9126/G9136 end
		 
		// GAP US107 START
		else if ( lc_delimiter == "TU" && !lb_tu_found )
		{
			if ( !lt_data.isAllNumeric() )
			{
				// CMsgHandler::setMsg(3957, " ", "TU-PARSE", 1, AB_Varchar(20,"TU"));
				// return;
				A14_CONTINUE;
			}
			md_IRT_ticketer_user_id = lt_data;
			lb_tu_found = AB_true;
		}
		else if ( lc_delimiter == "RF" && !lb_rf_found )
		{
			md_ID_ticket_centre_reference = lt_data;
			lb_rf_found = AB_true;
		}
		else if ( lc_delimiter == "GF" && !lb_gf_found )
		{
			md_ID_grid_reference = lt_data;
			lb_gf_found = AB_true;
		}
		else if ( lc_delimiter == "SR" && !lb_sr_found )
		{
			md_ID_shop_reference = lt_data;
			lb_sr_found = AB_true;
		}
		else if ( lc_delimiter == "IF" && !lb_if_found )
		{
			AB_Char ld_commission_type;

			if ( lb_tn_found )
			{
				// CMsgHandler::setMsg(3957, " ", "RS-PARSE", 1, AB_Varchar(20,"IF"));
				// return;
				A14_CONTINUE;
			}

			editAmount( lt_data, D_amount(0.00), D_amount(9999999999.99), AB_true, AB_false );

			if (! CMsgHandler::getMsgStatus())
			{
				CMsgHandler::clearMsg();
				// CMsgHandler::setMsg(3957, " ", "RS-PARSE", 2, AB_Varchar(20,"IF"));
				// return;
				A14_CONTINUE;
			}

			if ( lt_data.For(1) == "P" )
			{
				ld_commission_type = "%";
				md_ID_tour_oper_commission = lt_data.At(2);
			}
			else
			{
				ld_commission_type = "$";
				md_ID_tour_oper_commission = lt_data;
			}

			if ( lb_rs_found )
			{
				if ( ld_commission_type != md_ID_commission_type )
				{
					// CMsgHandler::setMsg(3957, " ", "RS-PARSE", 3, AB_Varchar(20,"IF"));
					// return;
					A14_CONTINUE;
				}
			}

//			md_ID_publish_fare_agent_commission = D_amount(100.00) - D_amount(md_ID_publish_fare_tc_commission);

			md_ID_commission_type = ld_commission_type;
			lb_rs_found = AB_true;
		}
		else if ( lv_delimiterBeforeSlash == "RS" && !lb_rs_found )
		{
			AB_Char ld_commission_type;

			if ( lb_tn_found )
			{
				// CMsgHandler::setMsg(3957, " ", "RS-PARSE", 1, AB_Varchar(20,"RS"));
				// return;
				A14_CONTINUE;
			}

			editAmount( lt_data, D_amount(0.00), D_amount(9999999999.99), AB_true, AB_false );

			if (! CMsgHandler::getMsgStatus())
			{
				CMsgHandler::clearMsg();
				// CMsgHandler::setMsg(3957, " ", "RS-PARSE", 2, AB_Varchar(20,"RS"));
				// return;
				A14_CONTINUE;
			}

			if ( lt_data.For(1) == "P" )
			{
				ld_commission_type = "%";
				md_ID_publish_fare_tc_commission = lt_data.At(2);
			}
			else
			{
				ld_commission_type = "$";
				md_ID_publish_fare_tc_commission = lt_data;
			}

			if ( lb_if_found )
			{
				if ( ld_commission_type != md_ID_commission_type )
				{
					// CMsgHandler::setMsg(3957, " ", "RS-PARSE", 3, AB_Varchar(20,"RS"));
					// return;
					A14_CONTINUE;
				}
			}

//			md_ID_publish_fare_agent_commission = D_amount(100.00) - D_amount(md_ID_publish_fare_tc_commission);

			md_ID_commission_type = ld_commission_type;
			lb_rs_found = AB_true;
		}
		else if ( lc_delimiter == "TN" && !lb_tn_found )
		{
			if ( lb_rs_found )
			{
				// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 1, AB_Varchar(20,"TN"));
				// return;
				A14_CONTINUE;
			}

			V_AB_Varchar lv_arrayField(100,100) ;

			li_counter = 0;
			li_counter = lv_arrayField.parseStringMultDel(lt_data,"/") ;

			AB_Boolean 	lb_tn_nf_found 		= AB_false;
			AB_Boolean 	lb_tn_nfs_found 		= AB_false;
			AB_Boolean 	lb_tn_sp_found 		= AB_false;
			AB_Boolean 	lb_tn_cp_found 		= AB_false;
			AB_Boolean 	lb_tn_if_found 		= AB_false;
			AB_Boolean 	lb_percent_allowed 	= AB_false;
			AB_Varchar	lv_amount(20);


			for ( int li_index = 1; li_index <= li_counter; li_index++ )
			{
				LOG(logDEBUG)  << "DK TN = " << lv_arrayField[li_index]  ;
				lb_percent_allowed = AB_false;

				if (lv_arrayField[li_index].For(3) == "NF-")
				{
					if ( lb_tn_nfs_found )
					{
						// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 2, AB_Varchar(20,"TN"));
						// return;
						A14_CONTINUE;
					}

					md_ID_nett_fare_to_airline = lv_arrayField[li_index].At(4);
					lv_amount = md_ID_nett_fare_to_airline;
					md_ID_net_ticket_type = "R";
					lb_tn_nf_found = AB_true;
				}
				else if (lv_arrayField[li_index].For(4) == "NFS-")
				{
					if ( lb_tn_nf_found )
					{
						// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 3, AB_Varchar(20,"TN"));
						// return;
						A14_CONTINUE;
					}

					md_ID_nett_fare_to_airline = lv_arrayField[li_index].At(5);
					lv_amount = md_ID_nett_fare_to_airline;
					md_ID_net_ticket_type = "P";
					lb_tn_nfs_found = AB_true;
				}
				else if (lv_arrayField[li_index].For(3) == "SP-")
				{
					if ( lb_tn_sp_found )
					{
						// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 4, AB_Varchar(20,"TN"));
						// return;
						A14_CONTINUE;
					}

					md_ID_selling_price_to_shop = lv_arrayField[li_index].At(4);
					lv_amount = md_ID_selling_price_to_shop;
					lb_tn_sp_found = AB_true;
				}
				else if (lv_arrayField[li_index].For(3) == "CP-")
				{
					if ( lb_tn_cp_found )
					{
						// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 5, AB_Varchar(20,"TN"));
						// return;
						A14_CONTINUE;
					}

					lv_amount = lv_arrayField[li_index].At(4);
					lv_amount.removeLeadAndTrailBlanks();

					if ( lv_amount.NotIsNull() )
						md_ID_selling_price_to_client = lv_amount;
					else
						lv_amount = "0.00";

					lb_tn_cp_found = AB_true;
				}
				else if (lv_arrayField[li_index].For(3) == "IF-")
				{
					if ( lb_tn_if_found )
					{
						// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 6, AB_Varchar(20,"TN"));
						// return;
						A14_CONTINUE;
					}

					if ( lv_arrayField[li_index].At(4).For(1) == "P" )
						lv_amount = lv_arrayField[li_index].At(5);
					else
						lv_amount = lv_arrayField[li_index].At(4);

					lv_amount.removeLeadAndTrailBlanks();

					if ( lv_amount.NotIsNull() )
					{
						md_ID_tour_oper_commission = lv_amount;

						if ( lv_arrayField[li_index].At(4).For(1) == "P" )
							md_ID_commission_type = "%";
						else
							md_ID_commission_type = "$";
					}
					else
						lv_amount = "0.00";

					lb_percent_allowed = AB_true;

					lb_tn_if_found = AB_true;
				}
				else
				{
					// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 7, AB_Varchar(20,"TN"));
					// return;
					A14_CONTINUE;
				}

				editAmount( lv_amount, D_amount(0.00), D_amount(9999999999.99), lb_percent_allowed, AB_false );
				if (! CMsgHandler::getMsgStatus())
				{
					CMsgHandler::clearMsg();
					// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 8, AB_Varchar(20,"TN"));
					// return;
					A14_CONTINUE;
				}
			}

			if ( !lb_tn_nf_found || !lb_tn_sp_found )
			{
				if ( !lb_tn_nfs_found || !lb_tn_sp_found )
				{
					// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 9, AB_Varchar(20,"TN"));
					// return;
					A14_CONTINUE;
				}
			} 

			if ( lb_tn_nf_found || lb_tn_nfs_found )
			{
				D_amount ld_nett_fare_to_airline = D_amount(md_ID_nett_fare_to_airline);
				D_amount ld_selling_price_to_shop = D_amount(md_ID_selling_price_to_shop);

				if ( 	ld_selling_price_to_shop - ld_nett_fare_to_airline < D_amount(0.00) &&
						md_ID_commission_type != "$" )
				{
					// CMsgHandler::setMsg(3957, " ", "TN-PARSE", 10, AB_Varchar(20,"TN"));
					// return;
					A14_CONTINUE;
				}
				
			}

			md_ID_net_ticket_ind = "Y";
			lb_tn_found = AB_true;
		}
		// GAP US107 END

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

		else if (lt_segment.At(7).For(4) == "-NUC")
		{

			md_IRMK_type[mi_IRMK_I]     = "NUC";
			md_IRMK_remark[mi_IRMK_I]   = "Y";
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}

      else if (lc_delimiter == "PR")
		{
			 if ( !lb_pr_found )
			 {
				 md_IRMK_type[mi_IRMK_I]     = "PR" ;
				 md_IRMK_remark[mi_IRMK_I]   = lv_data ;
				 md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
				 lb_pr_found = AB_true;
			 }
		}
      else if (lt_segment.At(6).For(3) == "FOP" )
		{
		    if (!lb_fop_found) 
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
		}
      else if (lc_delimiter == "EN")
         mv_IRT_employee_number = lv_data.For(10) ;
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
      else if (lc_delimiter == "FO")
		{
			// override md_savingCode, see SCR 438696
			if (!lb_fo_saving_code_found) {
				lv_data.removeLeadAndTrailBlanks();
				md_savingCode = lv_data;
				lb_fo_saving_code_found = true;
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
		   
			if(lt_temp.length()>0) {
				// SCR: 514016
				md_IR_Itin_Hotel_Data[gi_IrHotelIndex] = lv_data;
            mv_IrHotelData[gi_IrHotelIndex++] = lv_data ;
				}

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
			if (mb_MCO && 
				 (lt_segment.At(6).For(3) == "MAR" ||
				  lt_segment.At(6).For(3) == "MCC")) {
				// do nothing to ignore this in case of mb_MCO
			} else {
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
		// GAP US111 START
		{
         setShortRemarks(lt_segment.At(5), AB_true) ;      
		}
      else if(lt_segment.For(4) == "A14S" && 
				  lt_segment.At(5).For(1).isAllNumeric() && 
				 (lt_segment.At(6).For(1).isAllNumeric() || lt_segment.At(6).For(1) == "-"))
		{
         setShortRemarks(lt_segment.At(5), AB_false) ;      
		}
		// GAP US111 END
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
      else if (lt_segment.At(4).For(3) == "SF-" && !mb_SfFound && !mb_MCO)
      {
			if ( mb_SfFound ) 
			{
			  // Multiple SF- found, excess ignored, prepare for Rejectir

           mi_rejectirId = AB_Integer(3613);
			}
			else
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

			if ( PCRE::pcre_match("/F(\\d+)/", lv_data.to_string() ) )
			{
				string fee_number;
				PCRE::pcre_get_group(1, fee_number);
				LOG(logDEBUG)  << "FEE remark has /Fn/ and n=[" << fee_number.data() << "]";
				AB_Integer currMax = AB_Integer(fee_number.data());
				if ( li_FEE_MAX < currMax )
				{
					li_FEE_MAX = currMax;
					LOG(logDEBUG)  << "Max Fn in FEE remarks is [" << li_FEE_MAX << "]";
				}
			}

		}
		// GAP 9090F End

		// GAP 9121 Start: AOF fee
      else if (lt_segment.At(6).For(4) == "AOF/" )
      {
		   parseAOFremark(lt_segment.At(10));
		  	//md_IRMK_type[mi_IRMK_I]     = "AOF" ;
		  	//md_IRMK_remark[mi_IRMK_I]   = " ";
		  	//md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9121 End

      else if (lt_segment.At(6).For(3) == "FEE" )
      {
			if ( mb_FeeFound ) {

			  // Multiple FEE- found, excess ignored, prepare for Rejectir

           mi_rejectirId = AB_Integer(3614);
			}
			else
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
		}
      else if (lc_delimiter == "AI")
		{
			if (lv_data.NotIsNull())
			{
				if ( lv_data == "N" )
				{
					md_IRMK_type[mi_IRMK_I]            = "NAI" ;
					md_IRMK_remark[mi_IRMK_I]          = lv_data;
					md_IRMK_pax_id[mi_IRMK_I++]        = AB_null;
				}
			}
		}
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
      else if (lc_delimiter == "EM")
		{
			if (lt_data.NotIsNull())
			{

				for (int trans_index = 0; trans_index < trans_count; trans_index++)
				{
					lt_data.substitute(orig_string[trans_index], trans_string[trans_index]);
				}

				li_slashPos  = lt_data.findSubstring("/");
				AB_Char email(60);
				AB_Char email_type(10);

				if ( li_slashPos > 0 )
				{
					email = lt_data.For(li_slashPos-1);
					email_type = lt_data.At(li_slashPos+1);
					if (email_type != "TO" && email_type != "CC" && email_type != "BCC") {
						email_type = "TO";
					}
				}
				else
				{
					email = lt_data;
					email_type = "TO";
				}

				if ( ! email.Like("_%@_%") ) 
				{
					CMsgHandler::setMsg(3951, " ", "EM-PARSE", 1, AB_Varchar(20,AB_null));
					return;
				}

				md_IRMK_type[mi_IRMK_I]            = "AEM" ;
				md_IRMK_remark[mi_IRMK_I]          = email + email_type;
				md_IRMK_pax_id[mi_IRMK_I++]        = AB_null;

			}

		}

		// TKX CC EXP DATE START

		else if ( lt_segment.At(6).For(3) == "EXP" )
		{
			lt_data      = lt_segment.At(10) ;
			li_pos       = lt_data.findSubstring("!") ;

			if (li_pos > 0)
				lt_data = lt_data.For(li_pos - 1) ;

			if ( lt_data.NotIsNull() && md_TKX_cc_exp_date.IsNull() )
			{
         	lc_ccExpDate = lt_data.For(4);

            if ( lc_ccExpDate.isAllNumeric() )
            {
            	md_TKX_cc_exp_date = lc_ccExpDate;
            }
			}
		}

		// TKX CC EXP DATE END

		else if (lt_segment.At(4).For(5) == "G*PID")
		{
			lt_data = lt_segment.At(10);
         li_pos 	= lt_data.findSubstring("!") ;

         if (li_pos > 0) {
            lt_data = lt_data.For(li_pos - 1) ;
			}

         lt_data.removeTrailingBlanks() ;
			if (lt_data .isAllNumeric())
			{
				md_IRMK_type[mi_IRMK_I]     = "PID" ;
				md_IRMK_remark[mi_IRMK_I]   = lt_data ;
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
			md_IRMK_type[mi_IRMK_I]     = "NTE";
			md_IRMK_remark[mi_IRMK_I]   = lt_data.For(70);
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

		// GAP 9106 start
		else if (lc_delimiter == "FN")
		{
			md_IRMK_type[mi_IRMK_I]     = "FN";
			md_IRMK_remark[mi_IRMK_I]   = lt_data.For(13);
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9106 end

		// GAP G9143 Start
		else if (lt_segment.At(6).For(5) == "FPC/Y") 
		{
		  	md_IRMK_type[mi_IRMK_I]     = "FPC" ;
		  	md_IRMK_remark[mi_IRMK_I]   = " ";
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP G9143 End

      else if(lv_delimiterBeforeSlash == "GOW")
		{
			createUniqueRemark("MEM", lv_dataAfterSlash);
		}

      else if(lv_delimiterBeforeSlash == "NE")
		{
		  //createRemarkRows(pRemarkParser->parse(lv_dataAfterSlash.to_string(), "NE"));
			// ***REMARK_PARSER***
			//(moved to storeParsedRemarks) createRemarkRows(pRemarkParser->parsed("NE"));
		}

		//SCR 503909 Start
		//lt_segment = A14G*SF/M-50.00/1.1/SERVICE FEE/CCAX3700000000028/EXP1204!

      if ( lt_segment.At(4).For(5) == "G*SF/" )
      {
			V_AB_Varchar 	lv_arrayField(100,100) ;
		   AB_Char        lc_amount(13);
  		 	AB_Char        lc_FM_pax(5);
		   AB_Char        lc_FM_exp_date(4);
   		AB_Char        lc_FM_description(20);
   		AB_Char        lc_FM_fop(2);
   		AB_Char        lc_FM_cc_id(2);
   		AB_Char        lc_FM_cc_no(30);
   		D_amount       ld_value;
   		AB_Boolean     lb_amount_ok;
         AB_Boolean	   lb_FR_found = AB_false;
         AB_Boolean  	lb_FO_found = AB_false;
         AB_Boolean  	lb_FA_found = AB_false;

         lt_data = lt_segment.At(9);
         li_pos   = lt_data.findSubstring("!") ;

         if (li_pos > 0)
            lt_data = lt_data.For(li_pos - 1) ;

         lt_data.removeLeadAndTrailBlanks();

         li_counter = lv_arrayField.parseStringMultDel(lt_data,"/") ;

         for ( int li_index = 1; li_index <= li_counter; li_index++ )
         {
            if ( lv_arrayField[li_index].For(2) != "M-" &&
                 lv_arrayField[li_index].For(2) != "O-" &&
                 lv_arrayField[li_index].For(2) != "A-" &&
                 lv_arrayField[li_index].For(2) != "R-" )
            {
               createSFNRemark(lb_FR_found);
               break;
            }
            else if (lv_arrayField[li_index].For(2) == "M-" )
            {
               if ( li_counter != 4 && li_counter != 5 )
                  break;

               // Pick up Amount (mandatory)

               if (  lv_arrayField[li_index].At(3).length() > 0 &&
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

                  if (  lb_amount_ok == AB_true              &&
                        ld_value     <= D_amount(999999.99)  &&
                        ld_value     >  D_amount(0.00) )
                  {

                     // Pick up Pax ID (mandatory)

                     li_index++;

                     if (  li_index < li_counter                &&
                           lv_arrayField[li_index].length() > 0 &&
                           lv_arrayField[li_index].length() < 6 &&
                         ( lv_arrayField[li_index].findFirstCharNotInList(".0123456789") == 0 || lv_arrayField[li_index] == "PP" ))
                     {
                        AB_Text  lt_pax_p1;
                        AB_Text  lt_pax_p2;
                        int      li_pax_dot;
                        int      li_pax_id;
                        int      li_pax_len = lv_arrayField[li_index].length();

                        lc_FM_pax = lv_arrayField[li_index];
                        li_pax_dot = lc_FM_pax.findSubstring(".");

                        LOG(logDEBUG)  << "WW===== SFM >> lc_FM_pax = " << lc_FM_pax  ;
                        if ( li_pax_dot > 0 )
                        {
                           lt_pax_p1 = lc_FM_pax.For(li_pax_dot - 1);
                           lt_pax_p2 = lc_FM_pax.At( li_pax_dot + 1).For(li_pax_len - li_pax_dot) ;

                           LOG(logDEBUG)  << "WW===== SFM >> lt_pax_p1 = [" << lt_pax_p1 << " ]"  ;
                           LOG(logDEBUG)  << "WW===== SFM >> lt_pax_p2 = [" << lt_pax_p2 << " ]"  ;

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

                        LOG(logDEBUG)  << "WW===== SFM >> lc_FM_pax = " << lc_FM_pax  ;

                        // Pick up Description (mandatory)

                        li_index++;

                        if (  li_index <= li_counter &&
                              lv_arrayField[li_index].length() > 0 )
                        {
                           lc_FM_description = lv_arrayField[li_index].For(20);

                           // Pick up the FOP information (mandatory)

                           li_index++;

                           lc_FM_cc_id    = " ";
                           lc_FM_cc_no    = " ";
                           lc_FM_exp_date = " ";

                           if ( li_index <= li_counter )
                           {
                              if (( lv_arrayField[li_index].length() == 2              &&
                                  (  lv_arrayField[li_index].For(2)   == "CK"          ||
                                     lv_arrayField[li_index].For(2)   == "CA" ) )      ||
                                  ( lv_arrayField[li_index].length() >  2              &&
                                    lv_arrayField[li_index].For(2)   == "CC"           &&
                                    lv_arrayField[li_index].At(3).For(2).isAllAlpha()  &&
                                    lv_arrayField[li_index].At(5).isAllNumeric() )  )
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
                                       if (  lv_arrayField[li_index].For(3)         == "EXP" &&
                                             lv_arrayField[li_index].At(4).length()  > 0     &&
                                             lv_arrayField[li_index].At(4).length() == 4     &&
                                             lv_arrayField[li_index].At(4).isAllNumeric() )
                                       {
                                          lc_FM_exp_date = lv_arrayField[li_index].At(4);
                                       }
                                    }
                                 }

                                 //SCR 412640 Start
                                 bool lb_cc_valid = true;
                                 if ( lc_FM_fop.For(2) == "CC" )
                                 {
                                    AB_Varchar lv_cc_id(40);
                                    long       ll_error_code;
                                    lv_cc_id = lc_FM_cc_id + lc_FM_cc_no;

                                    if ( !validCreditCard(lv_cc_id, lc_FM_exp_date, ll_error_code))
                                    {
                                       lb_cc_valid = false;
                                       LOG(logDEBUG)  << "WW===== SFM CC is not valid!!!!!!!!  !!!  "  ;

                                    }
                                 }
                                 //SCR 412640 End
                                 if (  lc_FM_fop != "CC" ||
										      (  lc_FM_fop == "CC" &&
										         lc_FM_exp_date != "    " ) )
                                 {
											   md_IRMK_type[mi_IRMK_I]    = "SFM" ;
											   md_IRMK_remark[mi_IRMK_I]  =  (lb_cc_valid? "" : "BADCC/") + 
																						lc_FM_pax         +
																						AB_Char(" ")      +
                                                                  AB_Char(13,lc_amount.to_string())+
																						lc_FM_description +
                                                                  lc_FM_fop         +
																						lc_FM_cc_id       +
                                                                  lc_FM_cc_no       +
                                                                  lc_FM_exp_date;

                                    //if ( li_pax_id > 0 && li_pax_id -1 <= mi_IP_I )
												if ( li_pax_id > 0 )
                                    {
                                       //md_IRMK_pax_id[mi_IRMK_I] = md_IP_pax_id[li_pax_id - 1];
													md_IRMK_pax_id[mi_IRMK_I] = AB_Integer(li_pax_id);
                                    }
                                    else
                                    {
                                       md_IRMK_pax_id[mi_IRMK_I] = AB_null;
                                    }

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
            else if ( "R-" == lv_arrayField[li_index].For(2) &&
                      AB_false == lb_FR_found )
            {
               if ( 1 == lv_arrayField[li_index].At(3).length() &&
                     "N" == lv_arrayField[li_index].At(3) )
               {
                  createSFNRemark(lb_FR_found);
                  break;
               }
            }
            else if ( "O-" == lv_arrayField[li_index].For(2)   &&
                        AB_false == lb_FR_found                &&
                        AB_false == lb_FO_found )
            {
               if ( lv_arrayField[li_index].At(3).length() <= 9   &&
                    lv_arrayField[li_index].At(3).For(1)   != "P" )
               {
                  if ( lv_arrayField[li_index].At(3).length() > 0 )
                  {
                     lc_amount   = lv_arrayField[li_index].At(3);
                     ld_value    = ld_value.input_conversion_trigger(lc_amount);

                     if ( !CMsgHandler::getMsgStatus() )
                     {
                        lb_amount_ok = AB_false;
                        CMsgHandler::clearMsg();
                     }
                     else
                     {
                        lb_amount_ok = AB_true;
                     }

                     LOG(logDEBUG)  << "SFO << lc_amount = " << lc_amount  ;
                     LOG(logDEBUG)  << "SFO << ld_value  = " << ld_value   ;

                     if ( AB_true == lb_amount_ok           &&
                          ld_value <= D_amount(999999.99)   &&
                          ld_value >= D_amount(0.00) )
                     {
                        md_IRMK_type[mi_IRMK_I]   = "SFO";
                        md_IRMK_remark[mi_IRMK_I] = AB_Char(" ") + AB_Char(13, lc_amount.to_string());
                        md_IRMK_pax_id[mi_IRMK_I] = AB_null;
                        mi_IRMK_I++;

                        lb_FO_found = AB_true;
                     }
                  }
                  else if ( lv_arrayField[li_index].At(4).length()  > 0       &&
                         lv_arrayField[li_index].At(4).length() <= 5       &&
                         lv_arrayField[li_index].At(3).For(1)   == "P" )
                  {
                     lc_amount = lv_arrayField[li_index].At(4);

                     ld_value  = ld_value.input_conversion_trigger(lc_amount);

                     if ( !CMsgHandler::getMsgStatus() )
                     {
                        lb_amount_ok = AB_false;
                        CMsgHandler::clearMsg();
                     }
                     else
                     {
                        lb_amount_ok = AB_true;
                     }

                     LOG(logDEBUG)  << "SFO << lc_amount = " << lc_amount  ;
                     LOG(logDEBUG)  << "SFO << ld_value  = " << ld_value   ;

                     if ( AB_true == lb_amount_ok           &&
                          ld_value <= D_amount(100.00)      &&
                          ld_value >= D_amount(0.00) )
                     {
                        md_IRMK_type[mi_IRMK_I] = "SFO";
								md_IRMK_remark[mi_IRMK_I] = AB_Char(" ") + AB_Char(13, lc_amount.to_string());
                        md_IRMK_pax_id[mi_IRMK_I] = AB_null;
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
                  createSFNRemark(lb_FR_found);
                  break;
               }
            }
            else if ( "A-" == lv_arrayField[li_index].For(2)      &&
                      lb_FR_found == AB_false                     &&
                      lb_FA_found == AB_false )
            {
               if ( lv_arrayField[li_index].At(3).length() > 0 )
               {
                  if ( lv_arrayField[li_index].At(4).length() > 0    &&
                       lv_arrayField[li_index].At(4).length() < 6    &&
                       lv_arrayField[li_index].At(3).For(1)  == "p" )
                  {
                     AB_Char  lc_FAS(1);
                     li_pos = lv_arrayField[li_index].length();

                     if ( lv_arrayField[li_index].At(li_pos) == "S" )
                     {
                        lc_FAS    = "S";
                        lc_amount = lv_arrayField[li_index].At(4).For(li_pos-4);
                     }
                     else
                     {
                        lc_FAS    = " ";
                        lc_amount = lv_arrayField[li_index].At(4);
                     }

                     ld_value = ld_value.input_conversion_trigger(lc_amount);

                     if ( ! CMsgHandler::getMsgStatus() )
                     {
                        lb_amount_ok = AB_false;
                        CMsgHandler::clearMsg();
                     }
                     else
                     {
                        lb_amount_ok = AB_true;
                     }

                     LOG(logDEBUG)  << "SFA >> lc_amount = " << lc_amount  ;
                     LOG(logDEBUG)  << "SFA >> ld_value = "  << ld_value   ;

                     if ( lb_amount_ok == AB_true           &&
                          ld_value     <  D_amount(100.00)  &&
                          ld_value     >= D_amount(0.00) )
                     {
                        if ( ld_value > D_amount(0.00) )
                        {
                           md_IRMK_type[mi_IRMK_I]       = "SFA";
                           md_IRMK_remark[mi_IRMK_I]     = lc_FAS + AB_Char(13, lc_amount.to_string() );
                           md_IRMK_pax_id[mi_IRMK_I]     = AB_null;
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
                  createSFNRemark(lb_FR_found);
                  break;
               }
            }
         }
      }
		//SCR 503909 End

      lt_segment = po_PNR.getNextSegment("A14") ;
   }

	if ( li_FEE_MAX > AB_Integer(0) )
	{
			LOG(logDEBUG)  << "Storing max Fn in FEE remark as =[" << li_FEE_MAX << "]";
		  	md_IRMK_type[mi_IRMK_I]     = "F_M" ;
		  	md_IRMK_remark[mi_IRMK_I]   = li_FEE_MAX;
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
	}



   // SCR-437808-JJ-13OCT2009
   setCopIdByIata();

   // LOG(logDEBUG) << "END CMIR::populate_IrHeader_SectionA14"  ;
}

//-----------------------------------------------------------------------------

void CMIR::populate_Iraddress_SectionA13(AB_Varchar &pv_long_address, 
														CPNR &po_PNR)
{
   // LOG(logDEBUG) << "START CMIR::populate_Iraddress_SectionA13"  ;
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

   // LOG(logDEBUG) << "END CMIR::populate_Iraddress_SectionA13"  ;
}

//-----------------------------------------------------------------------------

// GAP US111 START

void CMIR::setShortRemarks(AB_Text pt_remark, AB_Boolean pb_matrix_UDID)

// GAP US111 END
{
   // LOG(logDEBUG) << "START CMIR::setShortRemarks"  ;

	// SCR 159704 START
   AB_Char    lc_data(90) ;
	// SCR 159704 END
   AB_Varchar lv_num(2) ;
   int        li_charPos, 
              li_numPos ;

   lc_data    = pt_remark ;
   li_charPos = 0 ;
   li_numPos  = 0 ;

   li_charPos = lc_data.findSubstring("!") ;
   li_numPos  = lc_data.findSubstring("-") ;

   if (li_charPos > 0)
      lc_data = lc_data.For(li_charPos - 1) ;


   if ((li_numPos == 2) || (li_numPos == 3))  
   {
      lv_num = lc_data.For(li_numPos - 1) ;

      if (lv_num.findFirstCharNotInList("0123456789") == 0)
      {
         if (	lc_data.At(li_numPos + 1) != " " &&
					lv_num != "0" && lv_num != "00" )
         {
				// GAP US111 START

				createUDID(lv_num, lc_data.At(li_numPos + 1), pb_matrix_UDID);

				// GAP US111 END
         }
      }
   }

   // LOG(logDEBUG) << "START CMIR::setShortRemarks"  ;
}

//-----------------------------------------------------------------------------

void CMIR::fill_IrRentalandOther(CPNR &po_PNR) 
{
   ENTER;

   LOG(logDEBUG) << "START  CMIR::fill_IrRentalandOther"  ;

   int li_counter, li_A16_index ;
   AB_Text lt_section ;
   AB_Text lt_segment ;
   // SPNR *lm_A16_Section ;
   string label_TYP_Value;
   ItinType itin_type = NO_TYPE;
	string unparsedLabels, labels;

   
   li_counter = 0 ;

	if (!mb_MCO) {
		po_PNR.resetSegment("A16") ;
		lt_section = po_PNR.getNextSegment("A16") ;

                
		while (lt_section.NotIsNull())
		{
			 mb_reasonCodeFoundInRentalSegment = AB_false;
			 // G9150 - Data Standartization, parse part of segment as LABELS
		    itin_type = NO_TYPE;
		    label_TYP_Value = "";
		    pRemarkParser->resetRemark("LABELS");

			 unparsedLabels = lt_section.to_string();
			 LOG(logDEBUG) << "===unparsedLabels[" << unparsedLabels.data() << "]===" ;
			 while( 	PCRE::pcre_match("(OD-|FF:)([^!]+)", unparsedLabels) )
			 {
				PCRE::pcre_get_group(2, labels);
				PCRE::pcre_get_after_group(0, unparsedLabels);
				LOG(logDEBUG) << "-----labels[" << labels.data() << "]---" ;
			   pRemarkParser->parseByName("LABELS", labels);
			 }

			 pRemarkParser->getData("LABELS", "TYP", label_TYP_Value);

			 if ((lt_section.At(4).For(1) == "A") || (lt_section.At(4).For(1) == "7"))
          {
			    itin_type = HOTEL;
				 fill_IrRentalHotel(lt_section) ;
          }
			 else if ((lt_section.At(4).For(1) == "B") || (lt_section.At(4).For(1) == "8"))
          {
		       itin_type = CAR;
			    pRemarkParser->parseByName("LABELS", "TYP-CAR"); //TYP is mandatory, missing in spec
				 fill_IrRentalCar(lt_section) ;
          }
			 else if (lt_section.At(4).For(1) == "6") 
			 {
				 if ( lt_section.At(14).For(1) == "B" )
             {
		          itin_type = BUS;
					 fill_IrBus(lt_section) ;
             }
				 else if ( lt_section.At(14).For(1) == "R" )
             {
		          itin_type = RAIL;
					 fillIrRail(lt_section) ;
             }
             else if ( lt_section.At(14).For(1) == "L" )
             {
		          itin_type = LIMO;
				    fillIrLimo(lt_section) ;
             }
				 else
             {
				    if ( fill_IrOther(lt_section) )
					 {
				            if ( label_TYP_Value == "INS" ) itin_type = INSURANCE;
				       else if ( label_TYP_Value == "SEA" ) itin_type = SHIP;
				       else if ( label_TYP_Value == "TOR" ) itin_type = TOUR;
					 }
             }
		    }
                        

		    // G9150 - Data Standartization
		    populateRestOfLabels(itin_type);

			 lt_section = po_PNR.getNextSegment("A16") ;
		 }
   }

   // LOG(logDEBUG) << "END  CMIR::fill_IrRental"  ;
   EXIT; 
}

//-----------------------------------------------------------------------------

void CMIR::fill_IrRentalHotel(AB_Text &pt_section)
{
   // LOG(logDEBUG) << "START CMIR::fill_IrRentalHotel"  ;

   int        li_pos ;
   AB_Varchar lv_field(50) ;
   AB_Date    ld_tempDate ;
	AB_Char    lc_status(2) ;
	AB_Char    lc_year(2) ;

   // LOG(logDEBUG) << "mi_IR_I= " << mi_IR_I  ;

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

//***************************US314 - Chg start**************************************
      lv_field = pt_section.getNthBracketedField(1,"/H-","/") ;
      if (lv_field.NotIsNull())
      {
         md_IR_site_name[mi_IR_I] = lv_field;
      }
//****************************US314 - Chg End****************************************
//     	md_IR_site_name[mi_IR_I]      = pt_section.At(30).For(20) ;

		LOG(logDEBUG) << "\nMEL: md_IR_site_name1:  pt_section= " <<  pt_section  ;
		LOG(logDEBUG) << "\nMEL: md_IR_site_name:= " <<  md_IR_site_name[mi_IR_I]  ;


      md_IR_unit_ct[mi_IR_I]        = pt_section.At(50).For(1) ;

		AB_Boolean lb_itbExist = AB_false;

      lv_field = pt_section.getNthBracketedField(1, "ITB-", "/") ;
      if (lv_field.NotIsNull())
      {
         md_IR_unit_type[mi_IR_I]      = lv_field.For(3) ;
         md_IR_number_of_pax[mi_IR_I]  = lv_field.At(4).For(1) ; 
			lb_itbExist							= AB_true;
      }
		else
		{
         md_IR_unit_type[mi_IR_I]      = "SGLB" ;
         md_IR_number_of_pax[mi_IR_I]  = 1 ;

      	lv_field = pt_section.getNthBracketedField(1,"/H-","/") ;
      	if (lv_field.NotIsNull()) {
				//override the site name from earlier initialization
      		md_IR_site_name[mi_IR_I] = lv_field;
			}
		}

		LOG(logDEBUG) << "\nMEL: md_IR_site_name2:= " <<  md_IR_site_name[mi_IR_I]  ;

		if ( ! lb_itbExist )
		{
			lv_field = pt_section.getNthBracketedField(1,"/R-","!") ;
			if ( lv_field.NotIsNull() )
			{
				if ( lv_field.Length().to_long() > 4 )
				{
					md_IR_unit_type[mi_IR_I]      = lv_field.For(3);
					if (  lv_field.Length().to_long() > 6 )
						md_IR_rate_type[mi_IR_I] = lv_field.At(4).For(3);
					else
						md_IR_rate_type[mi_IR_I] = lv_field.At(4);
				}
				else
					md_IR_unit_type[mi_IR_I]      = lv_field;
			}

			//ignores /PC when ITB does not exist

			//must capture P-
			lv_field = pt_section.getNthBracketedField(1,"/P-","/") ;
			if (lv_field.NotIsNull()) {
				md_IR_location_id[mi_IR_I] = lv_field;
			}
			else
			{
				lv_field = pt_section.getNthBracketedField(1,"/P-","!") ;
				if (lv_field.NotIsNull()) {
					md_IR_location_id[mi_IR_I] = lv_field;
				}
			}

		}
		else
		{
			//must capture /PC when ITB exist
      	lv_field = pt_section.getNthBracketedField(1,"/PC","/") ;
      	if (lv_field.NotIsNull()) {
      		md_IR_location_id[mi_IR_I] = lv_field;
			}
		}

		LOG(logDEBUG) << "\nMEL: R-= " <<  lv_field  ;
		LOG(logDEBUG) << "\nMEL: R-lengtgh= " <<   lv_field.Length().to_long()  ;
		LOG(logDEBUG) << "\nMEL: unit_type-= " <<   md_IR_unit_type[mi_IR_I]  ;
		LOG(logDEBUG) << "\nMEL: rate_type-= " <<    md_IR_rate_type[mi_IR_I]  ;


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

		LOG(logDEBUG) << "\nMEL: location_id-= " <<    md_IR_location_id[mi_IR_I]  ;


   }

	mb_booked_rate_was_overriden_with_dot = AB_FALSE;

	AB_Varchar lv_rateCode(60);
	AB_Varchar lv_rateType(60);

   if ( md_IR_source[mi_IR_I] == "A" )
	{
   	getA16ARateData(pt_section, 
               md_IR_itin_type[mi_IR_I], 
               md_IR_start_date[mi_IR_I],
               md_IR_saving_code[mi_IR_I], 
               md_IR_cur_id[mi_IR_I],
               md_IR_local_rate[mi_IR_I], 
               md_IR_regular_rate[mi_IR_I],
               md_IR_booked_rate[mi_IR_I], 
					md_IR_commissionable[mi_IR_I],
					md_IR_preferred[mi_IR_I],
               mv_IrHotelData, gi_IrHotelIndex,
					lv_rateCode, lv_rateType,
					md_IR_com_rate[mi_IR_I]
					) ;
	}
	else
	{
   	getA167RateData(pt_section, 
               md_IR_itin_type[mi_IR_I], 
               md_IR_start_date[mi_IR_I],
               md_IR_saving_code[mi_IR_I], 
               md_IR_cur_id[mi_IR_I],
               md_IR_local_rate[mi_IR_I], 
               md_IR_regular_rate[mi_IR_I],
               md_IR_booked_rate[mi_IR_I], 
               md_IR_commissionable[mi_IR_I],
               md_IR_preferred[mi_IR_I],
               mv_IrHotelData, gi_IrHotelIndex,
					lv_rateCode, lv_rateType,
					md_IR_com_rate[mi_IR_I]
					) ;
	}

   if ( lv_rateCode.NotIsNull() )
       md_IR_rate_code[mi_IR_I] = lv_rateCode;

   if ( lv_rateType.NotIsNull() )
       md_IR_rate_type[mi_IR_I] = lv_rateType;

	LOG(logDEBUG) << "\nMEL: unit_type-= " <<   md_IR_unit_type[mi_IR_I]  ;
	LOG(logDEBUG) << "\nMEL: rate_type-= " <<    md_IR_rate_type[mi_IR_I]  ;
   LOG(logDEBUG) << "MEL: A167 " << mb_booked_rate_was_overriden_with_dot  ;
	if ( mb_booked_rate_was_overriden_with_dot == AB_TRUE )
	{
			// "-" is an idicator that "booked_rate" comes from Remarks and has "."
			// for Hotel Segment 
			// we don't want override it later
		md_IR_booked_rate[mi_IR_I] = "-" + md_IR_booked_rate[mi_IR_I];
		mb_booked_rate_was_overriden_with_dot = AB_FALSE;
	}

   LOG(logDEBUG) << "MEL: md_IR_booked_rate" << md_IR_booked_rate[mi_IR_I]  ;

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


	if ( !mb_reasonCodeFoundInRentalSegment ) // use global remark if exists
	{
	  if ( mv_actionCode.NotIsNull() )
		{
			md_IR_action_code[mi_IR_I] =  mv_actionCode;
		}
	  if ( mv_source.NotIsNull() )
		{
			md_IR_EB_source[mi_IR_I] =  mv_source;
		}
	  if ( mv_reasonCode.NotIsNull() )
		{
			md_IR_reason_code[mi_IR_I] =  mv_reasonCode;
		}

	}

   mi_IR_I++ ;

   // LOG(logDEBUG) << "END CMIR::fill_IrRentalHotel"  ;
}

//-----------------------------------------------------------------------------

void CMIR::IrHotelRemarks(AB_Varchar &pv_field)
{
   // LOG(logDEBUG) << "START CMIR::IrHotelRemarks"  ;

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

      // LOG(logDEBUG) << "END CMIR::IrHotelRemarks"  ;
}

//-----------------------------------------------------------------------------

void CMIR::CreateRentalRemarks(AB_Varchar &pv_field)
{
   // LOG(logDEBUG) << "START CMIR::CreateRentalRemarks"  ;

   md_IRMK_itin_type[mi_IRMK_I]   = md_IR_itin_type[mi_IR_I] ;
   md_IRMK_remark[mi_IRMK_I]         = pv_field ;
   md_IRMK_ren_line_number[mi_IRMK_I] = md_IR_line_number[mi_IR_I] ;
   md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;

   // LOG(logDEBUG) << "END CMIR::CreateRentalRemarks"  ;
}

//-----------------------------------------------------------------------------

void CMIR::getA16ARateData(AB_Text &pt_section, 
							  AB_Char &pc_itin_type, 
							  D_char_date &pt_start_date,
             			  D_saving_code &pd_saving_code, 
							  D_currency &pd_cur_id, 
							  D_char_numeric &pd_local_rate,
             			  D_char_numeric &pd_regular_rate, 
							  D_char_numeric &pd_booked_rate, 
                       AB_Char        &pd_commissionable,
                       AB_Char        &pd_preferred,
                       V_AB_Text &pd_Data, int pi_Index,
								AB_Varchar &pv_rateCode,
								AB_Varchar &pv_rateType,
								D_char_numeric &pd_comm_rate)
{
   LOG(logDEBUG) << "START CMIR::getA16ARateData"  ;

   D_amount lm_amount ;
   AB_Varchar lt_field(100) ;
	AB_Char    lc_delim(4) ;
	AB_Boolean lb_RT_Exists;
	AB_Boolean lb_RG_Exists;

   lm_amount = 0.00 ;
   pd_booked_rate = "0" ;
   pd_regular_rate = "0" ;
	lb_RT_Exists = AB_false;
	lb_RG_Exists = AB_false;

	lc_delim = "/RT-" ;
   parseRate(pt_section, lc_delim, pd_cur_id, lm_amount, lt_field) ;

   LOG(logDEBUG) << "MSG getRateData: for pc_itin_type: " << pc_itin_type << " pd_cur_id= " << pd_cur_id << " lm_amount= " << lm_amount  ;
	mb_booked_rate_was_overriden_with_dot = AB_FALSE;

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

		lb_RT_Exists = AB_true;

		LOG(logDEBUG) << "RT: Data: pd_cur_id= " << pd_cur_id  ;
		LOG(logDEBUG) << "lm_amount= " << lm_amount  ;
		LOG(logDEBUG) << "lt_field= " << lt_field  ;
		LOG(logDEBUG) << "pt_section= " << pt_section  ;
		LOG(logDEBUG) << "pd_booked_rate= " << pd_booked_rate  ;
		LOG(logDEBUG) << "pd_local_rate= " << pd_local_rate  ;
		LOG(logDEBUG) << "pd_regular_rate= " << pd_regular_rate  ;

   }

	if ( ! lb_RT_Exists )
	{
		lc_delim = "/RG-" ;
		parseRate(pt_section, lc_delim, pd_cur_id, lm_amount, lt_field) ;
		
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
			
		  if (pc_itin_type == "C")
				get_car_data(lt_field) ;

			lb_RG_Exists = AB_true;

			LOG(logDEBUG) << "RG: Data: pd_cur_id= " << pd_cur_id  ;
			LOG(logDEBUG) << "lm_amount= " << lm_amount  ;
			LOG(logDEBUG) << "lt_field= " << lt_field  ;
			LOG(logDEBUG) << "pt_section= " << pt_section  ;
			LOG(logDEBUG) << "pd_booked_rate= " << pd_booked_rate  ;
			LOG(logDEBUG) << "pd_local_rate= " << pd_local_rate  ;
			LOG(logDEBUG) << "pd_regular_rate= " << pd_regular_rate  ;

		}


		if ( ! lb_RG_Exists )
		{
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

			LOG(logDEBUG) << "RQ: Data: pd_cur_id= " << pd_cur_id  ;
			LOG(logDEBUG) << "lm_amount= " << lm_amount  ;
			LOG(logDEBUG) << "lt_field= " << lt_field  ;
			LOG(logDEBUG) << "pt_section= " << pt_section  ;
			LOG(logDEBUG) << "pd_booked_rate= " << pd_booked_rate  ;
			LOG(logDEBUG) << "pd_local_rate= " << pd_local_rate  ;
			LOG(logDEBUG) << "pd_regular_rate= " << pd_regular_rate  ;

		}

	}

//MEL - START
	int li_pos = 0;
   li_pos = pt_section.findSubstring("/CG-") ;

   AB_Varchar      lv_rateCode(60) ;
   if (li_pos > 0)
   {
      lv_rateCode = pt_section.At(li_pos + 4);

      LOG(logDEBUG) << "\ngetA16ARateData: CG-:= " << lv_rateCode  ;

      li_pos = lv_rateCode.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

      if ( li_pos > 0 )
      {
         lv_rateCode = lv_rateCode.For(li_pos - 1 ) ;

      	LOG(logDEBUG) << "\ngetA16ARateData: lv_rateCode: CG- is not the end of the line:= " << lv_rateCode  ;
      }
      else
      {
      	LOG(logDEBUG) << "\ngetA16ARateData: lv_rateCode: CG- is the end of the line:= " << lv_rateCode  ;
      }

		pv_rateType = lv_rateCode;

   }

   AB_Varchar      lv_rateType(60) ;
   li_pos = pt_section.findSubstring("/RAC-") ;
   if (li_pos > 0)
   {
      lv_rateType = pt_section.At(li_pos + 5);

      LOG(logDEBUG) << "\ngetA16ARateData: RAC-:= " << lv_rateType  ;

      lv_rateType.removeLeadAndTrailBlanks();

      li_pos = lv_rateType.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

      if ( li_pos > 0 )
      {
         lv_rateType = lv_rateType.For(li_pos - 1 ) ;
      	LOG(logDEBUG) << "\ngetA16ARateData: lv_rateType: RAC- is not the end of the line:= " << lv_rateType  ;
      }
      else
      {
      	LOG(logDEBUG) << "\ngetA16ARateData: lv_rateType: RAC- is the end of the line:= " << lv_rateType  ;
      }

		pv_rateType = lv_rateType;

   }
//MEL - END
//mel end

   D_char_numeric ldLocalRate;
   D_char_numeric ldRegularRate;
   D_char_numeric ldBookedRate;
	AB_Char lc_commissionable;
	AB_Char lc_preferred ;
	D_char_numeric lc_comm_rate;
	
   getOverrideRates(pc_itin_type, 
               pt_start_date, 
               pd_saving_code, 
               ldLocalRate, 
               ldRegularRate, 
               ldBookedRate, 
					lc_commissionable,
					lc_preferred,
               pd_Data, 
               pi_Index,
					lv_rateCode,
					lv_rateType,
					lc_comm_rate
					) ;

	//use the override if they exist
   if (lc_commissionable.NotIsNull() )
      pd_commissionable = lc_commissionable;

   if (lc_preferred.NotIsNull() )
      pd_preferred = lc_preferred;

   if (lc_comm_rate.NotIsNull() ) 
	  pd_comm_rate = lc_comm_rate;

	if ( lv_rateCode.NotIsNull() )
		pv_rateCode = lv_rateCode;

	//use the override if they exist
	if ( lv_rateType.NotIsNull() )
		pv_rateType = lv_rateType;

	if ( ldBookedRate.NotIsNull() )
	{
		pd_booked_rate = ldBookedRate;
		mb_booked_rate_was_overriden_with_dot = AB_TRUE;
	}

	if ( ldLocalRate.NotIsNull() )
	{
		pd_local_rate = "-" + ldLocalRate;
	}

	if ( pd_regular_rate.NotIsNull() )
	{
		pd_regular_rate = ldRegularRate;
	}

	LOG(logDEBUG) << "After the overrides: Data: pd_cur_id= " << pd_cur_id  ;
	LOG(logDEBUG) << "pd_booked_rate= " << pd_booked_rate  ;
	LOG(logDEBUG) << "pd_local_rate= " << pd_local_rate  ;
	LOG(logDEBUG) << "pd_regular_rate= " << pd_regular_rate  ;

   // LOG(logDEBUG) << "END CMIR::getRateData"  ;
}

void CMIR::getA167RateData(AB_Text &pt_section, 
							  AB_Char &pc_itin_type, 
							  D_char_date &pt_start_date,
             			  D_saving_code &pd_saving_code, 
							  D_currency &pd_cur_id, 
							  D_char_numeric &pd_local_rate,
             			  D_char_numeric &pd_regular_rate, 
							  D_char_numeric &pd_booked_rate, 
                       AB_Char        &pd_commissionable,
                       AB_Char        &pd_preferred,
                       V_AB_Text &pd_Data, int pi_Index,
								AB_Varchar &pv_rateCode,
								AB_Varchar &pv_rateType,
								D_char_numeric &pd_comm_rate)
{
   LOG(logDEBUG) << "START CMIR::getA167RateData"  ;

   D_amount lm_amount ;
   AB_Varchar lt_field(100) ;
	AB_Char    lc_delim(4) ;
	AB_Boolean lb_RT_Exists;
	AB_Boolean lb_RG_Exists;

   lm_amount = 0.00 ;
   pd_booked_rate = "0" ;
   pd_regular_rate = "0" ;
	lb_RT_Exists = AB_false;
	lb_RG_Exists = AB_false;

// ******************************US250 - Chg start*******************************
   if (pc_itin_type == "H")
   {
      lc_delim = "/RQ-" ;
      parseRate(pt_section, lc_delim, pd_cur_id, lm_amount, lt_field) ;
   }
   if (lm_amount == AB_Decimal(0.00))
   {
      lc_delim = "/RT-" ;
      parseRate(pt_section, lc_delim, pd_cur_id, lm_amount, lt_field) ;
   }
// *****************************US250 - Chg end************************************

   LOG(logDEBUG) << "MSG getRateData: for pc_itin_type: " << pc_itin_type << " pd_cur_id= " << pd_cur_id << " lm_amount= " << lm_amount  ;

	mb_booked_rate_was_overriden_with_dot = AB_TRUE;

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

		lb_RT_Exists = AB_true;

		LOG(logDEBUG) << "RT: Data: pd_cur_id= " << pd_cur_id  ;
		LOG(logDEBUG) << "lm_amount= " << lm_amount  ;
		LOG(logDEBUG) << "lt_field= " << lt_field  ;
		LOG(logDEBUG) << "pt_section= " << pt_section  ;
		LOG(logDEBUG) << "pd_booked_rate= " << pd_booked_rate  ;
		LOG(logDEBUG) << "pd_local_rate= " << pd_local_rate  ;
		LOG(logDEBUG) << "pd_regular_rate= " << pd_regular_rate  ;

   }

//MEL - START
   int li_pos = 0;
   li_pos = pt_section.findSubstring("/CG-") ;

   AB_Varchar      lv_rateCode(60) ;
   if (li_pos > 0)
   {
      lv_rateCode = pt_section.At(li_pos + 4);

      LOG(logDEBUG) << "\ngetA16ARateData: CG-:= " << lv_rateCode  ;

      li_pos = lv_rateCode.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

      if ( li_pos > 0 )
      {
         lv_rateCode = lv_rateCode.For(li_pos - 1 ) ;

         LOG(logDEBUG) << "\ngetA16ARateData: lv_rateCode: CG- is not the end of the line:= " << lv_rateCode  ;
      }
      else
      {
         LOG(logDEBUG) << "\ngetA16ARateData: lv_rateCode: CG- is the end of the line:= " << lv_rateCode  ;
      }

      pv_rateType = lv_rateCode;

   }

   AB_Varchar      lv_rateType(60) ;
   li_pos = pt_section.findSubstring("/RAC-") ;
   if (li_pos > 0)
   {
      lv_rateType = pt_section.At(li_pos + 5);

      LOG(logDEBUG) << "\ngetA16ARateData: RAC-:= " << lv_rateType  ;

      lv_rateType.removeLeadAndTrailBlanks();

      li_pos = lv_rateType.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

      if ( li_pos > 0 )
      {
         lv_rateType = lv_rateType.For(li_pos - 1 ) ;
         LOG(logDEBUG) << "\ngetA16ARateData: lv_rateType: RAC- is not the end of the line:= " << lv_rateType  ;
      }
      else
      {
         LOG(logDEBUG) << "\ngetA16ARateData: lv_rateType: RAC- is the end of the line:= " << lv_rateType  ;
      }

      pv_rateType = lv_rateType;

   }
//MEL - END

	/* For Manual A16 segments, there is no need to check for RT- and RQ- */

   D_char_numeric ldLocalRate;
   D_char_numeric ldRegularRate;
   D_char_numeric ldBookedRate;
	AB_Char lc_commissionable;
	AB_Char lc_preferred ;
	D_char_numeric lc_comm_rate;
	
   getOverrideRates(pc_itin_type, 
               pt_start_date, 
               pd_saving_code, 
               ldLocalRate, 
               ldRegularRate, 
               ldBookedRate, 
               lc_commissionable,
               lc_preferred,
               pd_Data, 
               pi_Index,
					lv_rateCode, lv_rateType, 
					lc_comm_rate) ;

   if (lc_commissionable.NotIsNull() )
      pd_commissionable = lc_commissionable;

   if (lc_preferred.NotIsNull() )
      pd_preferred = lc_preferred;

   if (lc_comm_rate.NotIsNull() )
		pd_comm_rate = lc_comm_rate;

	if ( lv_rateCode.NotIsNull() )
		pv_rateCode = lv_rateCode;

	if ( lv_rateType.NotIsNull() )
		pv_rateType = lv_rateType;

	if ( ldBookedRate.NotIsNull() )
	{
		pd_booked_rate = ldBookedRate;
	}
	else
	{
		//This means that the booked rate will not be adjusted
		//in the back end - SP_IR__createIrrental
		mb_booked_rate_was_overriden_with_dot = AB_TRUE;
	}

	if ( ldLocalRate.NotIsNull() )
	{
		pd_local_rate = ldLocalRate;
	}

	pd_local_rate = "-" + pd_local_rate;

	if ( pd_regular_rate.NotIsNull() )
	{
		pd_regular_rate = ldRegularRate;
	}

	LOG(logDEBUG) << "After the overrides: Data: pd_cur_id= " << pd_cur_id  ;
	LOG(logDEBUG) << "pd_booked_rate= " << pd_booked_rate  ;
	LOG(logDEBUG) << "pd_local_rate= " << pd_local_rate  ;
	LOG(logDEBUG) << "pd_regular_rate= " << pd_regular_rate  ;

   // LOG(logDEBUG) << "END CMIR::getRateData"  ;
}

//-----------------------------------------------------------------------------

void CMIR::parseRate(AB_Text &pt_section, 
							AB_Char &pc_start_delim, 
							D_currency &pd_cur_id, 
							D_amount &pd_amount, 
							AB_Varchar &pt_field)
{
   LOG(logDEBUG) << "START CMIR::parseRate"  ;

   int li_pos ;
   D_char_numeric lm_rate ;
   AB_Char lc_test ;
   AB_Money lm_amount ;
   AB_Varchar lv_rawAmount(15) ;
   AB_BOOL lb_ok;
   D_amount ld_test ;
   AB_Varchar lv_format(15) ;
	string curIdPre, curIdPost, amount;


   pt_field = pt_section.getNthBracketedField(1, pc_start_delim.to_string(), "!") ;
	pt_field.removeLeadAndTrailBlanks();
	LOG(logDEBUG) << "before parsing for rate: pt_field =[" << pt_field << "]";
   
	if ( PCRE::pcre_match("^([A-Z][A-Z][A-Z])?([\\d\\.]+)([A-Z][A-Z][A-Z])?" , pt_field.to_string() ))
	{
		LOG(logDEBUG) << "rate matched";
		PCRE::pcre_get_group(1, curIdPre);
		PCRE::pcre_get_group(2, amount);
		PCRE::pcre_get_group(3, curIdPost);
		if ( curIdPre.length() > 0 )
		{
         pd_cur_id = curIdPre.data() ;
		}
		else if ( curIdPost.length() > 0 )
		{
         pd_cur_id = curIdPost.data() ;
		}
		LOG(logDEBUG) << "cur=["  << pd_cur_id << "]";
		LOG(logDEBUG) << "amount=["  << amount.data() << "]";

      lv_rawAmount = amount.data() ;
      lv_rawAmount.removeLeadingBlanks() ;
      li_pos = lv_rawAmount.findSubstring(".") ;
		lv_format = "#########.##";
      if ((li_pos == 0) && (lv_rawAmount != 0) && (lv_rawAmount.NotIsNull()))
      {
			if ( md_IR_itin_type[mi_IR_I] == "H" && md_IR_source[mi_IR_I] == "A" )
			{
            if (lv_rawAmount.Length().to_long() > 13)
            {
					//set this to 0 if number of digits exceed 13
               lv_rawAmount = "0.00" ;
            }
            else
            {
				  lv_rawAmount = lv_rawAmount + ".00" ;
            }
			}
			else
			{
			  lv_rawAmount = lv_rawAmount + ".00" ;
			}

			//lv_rawAmount must end in .00 at this point

      }

		lm_amount = lv_rawAmount.to_money(lv_format.to_string(), lb_ok) ;
      
		if (! lb_ok )
				pd_amount = "0.00" ;
		else
				pd_amount = lm_amount.to_string() ;

   }

   LOG(logDEBUG) << "END CMIR::parseRate"  ;
}

//-----------------------------------------------------------------------------
void CMIR::getOverrideRates(AB_Char &pc_itin_type, 
									D_char_date &pt_start_date, 
									D_saving_code &pd_saving_code,
             					D_char_numeric &pd_local_rate, 
									D_char_numeric &pd_regular_rate,
             				   D_char_numeric &pd_booked_rate, 
                           AB_Char        &pd_commissionable,
                           AB_Char        &pd_preferred,    //US500 Chg
									V_AB_Text &pd_Data, 
									int pi_Index,
									AB_Varchar &pv_rateCode,
									AB_Varchar &pv_rateType,
									D_char_numeric &pd_comm_rate)
{
   // LOG(logDEBUG) << "START CMIR::getOverrideRates"  ;

   int li_counter, li_pos ;
   D_char_numeric  lm_amount ;
   AB_Varchar      lv_data(60) ;
	AB_Char         lc_delim(4) ;
	AB_Varchar      lv_rateData(60) ; 
	AB_Boolean      lb_RCFound ;
	D_char_numeric  ld_RTAmount ;
   AB_Varchar      lv_field(50) ;

//	mb_booked_rate_was_overriden_with_dot = AB_FALSE;


	if (pd_regular_rate == "0")
		pd_regular_rate = AB_null ;

   li_counter = 1 ;
   lb_RCFound = AB_false ;

	LOG(logDEBUG) << "HS INDEX = "<< pi_Index  ;

   if (pi_Index == li_counter)
      return ;

   lv_data = AB_null ;

    // SCR 516742
   AB_Boolean lb_hotel_overridden = AB_false;


   while (li_counter < pi_Index)
   {
	     // SCR: 516742
      lb_hotel_overridden = AB_false;

		if ( pc_itin_type == "H" )
		{
      	if (pd_Data[li_counter].For(5) == pt_start_date.For(5))
			{
            if (lv_data.IsNull())
            { 
         	lv_data = pd_Data[li_counter] ;
            }
            else
            {
             lv_data = AB_Text(lv_data) + pd_Data[li_counter] ;
            }
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

	setIrReasonCodeIfOverride(lv_data);  // set reason,action,source if EB- present

	LOG(logDEBUG) << "HS INDEX = "<< lv_data  ;

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


//MEL - START
   li_pos = lv_data.findSubstring("/CG-") ;

   AB_Varchar      lv_rateCode(60) ;
   if (li_pos > 0)
   {
		lv_rateCode = lv_data.At(li_pos + 4);

      LOG(logDEBUG) << "\ngetOverrideRates: CG-:= " << lv_rateCode  ;

		li_pos = lv_rateCode.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

		if ( li_pos > 0 )
		{
			lv_rateCode = lv_rateCode.For(li_pos - 1 ) ;

      	LOG(logDEBUG) << "\ngetOverrideRates: CG- is not the end of the line:= " << lv_rateCode  ;
		}
		else
		{
      	LOG(logDEBUG) << "\ngetOverrideRates: CG- is the end of the line:= " << lv_rateCode  ;
		}

		pv_rateCode = lv_rateCode;
   }

   AB_Varchar      lv_rateType(60) ;
   li_pos = lv_data.findSubstring("/RAC-") ;
   if (li_pos > 0)
   {
      lv_rateType = lv_data.At(li_pos + 5);

      LOG(logDEBUG) << "\ngetOverrideRates: RAC-:= " << lv_rateType  ;

		lv_rateType.removeLeadAndTrailBlanks();

      li_pos = lv_rateType.findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789");

      if ( li_pos > 0 )
      {
         lv_rateType = lv_rateType.For(li_pos - 1 ) ;

      	LOG(logDEBUG) << "\ngetOverrideRates: RAC- is not the end of the line:= " << lv_rateType  ;
      }
      else
      {
      	LOG(logDEBUG) << "\ngetOverrideRates: RAC- is the end of the line:= " << lv_rateType  ;
      }

		pv_rateType = lv_rateType;
   }
//MEL - END
//***********US500 Chg Start**********************

   li_pos = lv_data.findSubstring("NC-") ;

   if (li_pos > 0 && pd_commissionable.IsNull() &&
         ( lv_data.At(li_pos + 3).For(1) == "Y" ||
            lv_data.At(li_pos + 3).For(1) == "N") ) // pick up only the first code
   {
         LOG(logDEBUG) << "Mary===pd_commissionable" << pd_commissionable;
      pd_commissionable = lv_data.At(li_pos + 3).For(1) ;
      LOG(logDEBUG) << "Mary===pd_commissionable" << pd_commissionable;
   }

   li_pos = lv_data.findSubstring("BT-") ;

   if (li_pos > 0 && pd_preferred.IsNull() &&
         ( lv_data.At(li_pos + 3).For(1) == "P" ||
            lv_data.At(li_pos + 3).For(1) == "N") ) // pick up only the first code
   {
      LOG(logDEBUG) << "Mary===pd_preferred" << pd_preferred;
      pd_preferred = lv_data.At(li_pos + 3).For(1) ;
      LOG(logDEBUG) << "Mary===pd_preferred" << pd_preferred;
   }
//***********US500 Chg End**********************

//***********US499 Chg Start********************** 
   li_pos = lv_data.findSubstring("HP-") ;

   if (li_pos > 0 )
   {
      lv_field = lv_data.At(li_pos + 3);
      LOG(logDEBUG) << "lv_field: " << lv_field;

      li_pos = lv_field.findFirstCharNotInList("0123456789.") ;

      if ( li_pos > 0 )
            pd_comm_rate = lv_field.For(li_pos - 1) ;
         else
            pd_comm_rate = lv_field ;

    LOG(logDEBUG) << "pd_comm_rate : " << pd_comm_rate;
   }


//***********US499 Chg End************************

   li_pos = lv_data.findSubstring("RC-") ;

   if (li_pos > 0)
   {
		/* 
			if RC- exist, use the booked rate from RC-
			if RC- does not come with RT-, the local rate will not be set 
		*/

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

   }

   li_pos = lv_data.findSubstring("RT-") ;

   if (li_pos > 0)
   {
		if (lv_data.At(li_pos+3).For(3).isAllAlpha() )
		{
			md_IR_cur_id[mi_IR_I] = lv_data.At(li_pos+3).For(3) ;
         // SCR: 516742
         if (md_IR_itin_type[mi_IR_I] == 'H') {
               //LOG(logDEBUG)  << "Hotel CUR ID override " << md_IR_cur_id[mi_IR_I];
               lb_hotel_overridden = true; }



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

		/* 
			if RT- came with RC-, then use the local rate from the RT- and use the booked_rate
				from the RC- 
			if RT- came without RC-, then use the local and booked rate from from the RT-
		*/
      if ( lb_RCFound )
      {
         pd_local_rate = ld_RTAmount ;
      }
      else
      {
         pd_local_rate = ld_RTAmount ;
         pd_booked_rate = ld_RTAmount ;
      }
    
         // SCR: 516742: the local_rate is override.   
         if (md_IR_itin_type[mi_IR_I] == 'H') {
              //LOG(logDEBUG)  << "Hotel local_rate override " << md_IR_cur_id[mi_IR_I];
               lb_hotel_overridden = true; }

 

/*
		li_pos = pd_booked_rate.findSubstring(".") ;

		if (li_pos > 0)
		{
			mb_booked_rate_was_overriden_with_dot = AB_TRUE;
		}
*/

   }
   if (lb_hotel_overridden == AB_true) {
         //LOG(logDEBUG)  << "DEBUG Hotel override at line_no: " << md_IR_line_number[mi_IR_I];
      md_IRMK_type[mi_IRMK_I]  = "FHB";
      md_IRMK_itin_type[mi_IRMK_I]  = "H";
      md_IRMK_pax_id[mi_IRMK_I]  = AB_null;
      md_IRMK_ren_line_number[mi_IRMK_I] = md_IR_line_number[mi_IR_I];
      md_IRMK_remark[mi_IRMK_I++]   = "CUR_ID/LOCALRATE OVERRIDE";
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

   // LOG(logDEBUG) << "END CMIR::getOverrideRates"  ;
}

//-----------------------------------------------------------------------------

void CMIR::getAddressData(AB_Text &pt_section)
{
   // LOG(logDEBUG) << "START CMIR::getAddressData"  ;

   AB_Varchar  lv_field(100) ;
   V_AB_Varchar lv_arrayField(100,100) ;
   int li_pos ;


   lv_field = pt_section.getNthBracketedField(1, "W-:", "!") ;

	LOG(logDEBUG) << "\nMEL:ADDRESS:lv_field= "  << lv_field  ;

   if (lv_field.IsNull())
      return ;

   if (md_IR_source[mi_IR_I] == "A")
   {
      lv_field = lv_field.At(3) ;

      li_pos = lv_field.getReverseIndex("/") ;

      if (li_pos > 0)
      {
         if (lv_field.At(li_pos + 1) == " ")
            lv_field = lv_field.For(li_pos - 1) ;
      }
   }
   else
   {

		LOG(logDEBUG) << "\nMEL:ADDRESS:pt_section= "  << pt_section  ;

		AB_Boolean lb_itbExist = AB_false;
   	li_pos = pt_section.findSubstring("ITB-") ;
		if ( li_pos > 0 )
			lb_itbExist = AB_true;

/********************US314 Chg start*************************************************************************/
		if ( !lb_itbExist )
		{
			//remove the first 2 characters
			if ( lv_field.Length().to_long() > 2 )
				lv_field = lv_field.At(3);

			LOG(logDEBUG) << "\nMEL:HOTEL Address: first line should have been removed lv_field= " << lv_field  ;
		}

		LOG(logDEBUG) << "\nMEL:HOTEL Address: first line still there lv_field= " << lv_field  ;
/***************************US314 Chg End ********************************************************************/

      li_pos = lv_field.findSubstring("/RT-") ;

      if (li_pos > 1)
         lv_field = lv_field.For(li_pos - 1) ;

		int li_count ;
      li_count = lv_arrayField.parseStringMultDel(lv_field,"/") ;

		if ( lv_arrayField[li_count].For(1).isAllNumeric())
		{
			/**
			 * Extract phone code and phone number
			 **/

   		md_IR_phone_acode[mi_IR_I] = lv_arrayField[li_count].For(5);
   		md_IR_phone_no[mi_IR_I] = lv_arrayField[li_count].At(6);

			/**
          * Excluding phone field from address info
			 **/

			/*
			li_count = li_count -1 ;

			while ( li_count > 0 )
			{
				if (lv_field.IsNull())
					lv_field =  lv_arrayField[li_count] ;
				else
					lv_field = lv_arrayField[li_count] + "/" + lv_field ;
				li_count -- ;
			}
         */

         li_pos = lv_field.getReverseIndex("/") ;

         //li_pos = lv_field.length() - li_pos ;
         lv_field = lv_field.For(li_pos - 1) ;
		}

   }
   CAddress lo_address ;

	LOG(logDEBUG) << "\nMEL:HOTEL Address: lv_field= " << lv_field  ;

	lo_address.setFourLineAddr(lv_field);

   lo_address.getAddress(md_IR_addr1[mi_IR_I], 
               			 md_IR_addr2[mi_IR_I],
               			 md_IR_addr3[mi_IR_I], 
               			 md_IR_addr4[mi_IR_I]) ;

   // LOG(logDEBUG) << "END CMIR::getAddressData"  ;
}


//-----------------------------------------------------------------------------

void CMIR::fill_IrRentalCar(AB_Text &pt_section)
{
   // LOG(logDEBUG) << "START CMIR::fill_IrRentalCar"  ;

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
	//------ STS LABEL(G9150) ------------------
	RP_saveDataTo(	"LABELS", "STS",	md_IR_service_status[mi_IR_I]);
	lc_status = md_IR_service_status[mi_IR_I];
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
      md_IR_start_time[mi_IR_I] = Checktime(lv_field);
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
      md_IR_dest_city_id[mi_IR_I] = lv_field.For(3) ;
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

	AB_Varchar lv_rateCode(60);
	AB_Varchar lv_rateType(60);
	if ( md_IR_source[mi_IR_I] == "A" )
	{
   	getA16ARateData(pt_section, 
          md_IR_itin_type[mi_IR_I], 
          md_IR_start_date[mi_IR_I],
          md_IR_saving_code[mi_IR_I], 
          md_IR_cur_id[mi_IR_I],
          md_IR_local_rate[mi_IR_I], 
          md_IR_regular_rate[mi_IR_I],
          md_IR_booked_rate[mi_IR_I], 
          md_IR_commissionable[mi_IR_I],
          md_IR_preferred[mi_IR_I],
          mv_IrCarData, mi_IrCarIndex,
			 lv_rateCode,
			 lv_rateType, 
			 md_IR_com_rate[mi_IR_I]) ;
	}
	else
	{
   	getA167RateData(pt_section, 
          md_IR_itin_type[mi_IR_I], 
          md_IR_start_date[mi_IR_I],
          md_IR_saving_code[mi_IR_I], 
          md_IR_cur_id[mi_IR_I],
          md_IR_local_rate[mi_IR_I], 
          md_IR_regular_rate[mi_IR_I],
          md_IR_booked_rate[mi_IR_I], 
          md_IR_commissionable[mi_IR_I],
          md_IR_preferred[mi_IR_I],  
          mv_IrCarData, mi_IrCarIndex,
			 lv_rateCode,
			 lv_rateType, 
			 md_IR_com_rate[mi_IR_I]) ;
	}

	if ( lv_rateType.NotIsNull() )
		 md_IR_rate_type[mi_IR_I] = lv_rateType;

	if ( lv_rateCode.NotIsNull() )
		 md_IR_rate_code[mi_IR_I] = lv_rateCode;

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


	if ( !mb_reasonCodeFoundInRentalSegment ) // use global remark if exists
	{
	  if ( mv_actionCode.NotIsNull() )
		{
			md_IR_action_code[mi_IR_I] =  mv_actionCode;
		}
	  if ( mv_source.NotIsNull() )
		{
			md_IR_EB_source[mi_IR_I] =  mv_source;
		}
	  if ( mv_reasonCode.NotIsNull() )
		{
			md_IR_reason_code[mi_IR_I] =  mv_reasonCode;
		}

	}

   mi_IR_I++ ;

   // LOG(logDEBUG) << "END CMIR::fill_IrRentalCar"  ;
}

//-----------------------------------------------------------------------------

void CMIR::get_car_data(AB_Varchar &pv_field)
{
   // LOG(logDEBUG) << "START CMIR::get_car_data"  ;

   int li_pos, li_pos2 ;
   AB_Char lc_duration(2), lc_temp_free_milage(25) ; 
													 //1613-SC-13FEB99
   AB_Varchar lc_temp(255);             // 1706 - 15MAR99 - v1198 - SC -- 

   LOG(logDEBUG) << "MSG get_car_data: pc_field= " << pv_field  ;

   if (md_IR_free_mileage[mi_IR_I].IsNull())
   {
      li_pos = pv_field.findSubstring("UNL");

      LOG(logDEBUG) << "MSG get_car_data: li_pos= " << li_pos  ;

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
						md_IR_free_mileage[mi_IR_I] = lc_temp_free_milage.At(li_pos2+2) ;
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
         li_pos = lc_temp.findSubstring("XD") ;

		/* 1706 - 15MAR99 - v1198 - SC -- this is for xx ###.##XD instead of 
                                        XD###.## (see specs for A16)
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

   // LOG(logDEBUG) << "END CMIR::get_car_data"  ;
}

//-----------------------------------------------------------------------------

void CMIR::IrCarRemarks(AB_Varchar &pv_field)
{
   // LOG(logDEBUG) << "START CMIR::IrCarRemarks"  ;

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

   // LOG(logDEBUG) << "END CMIR::IrCarRemarks"  ;
}

//-----------------------------------------------------------------------------

/*
void CMIR::fill_IrOther(AB_Text &pt_section)
{
   // LOG(logDEBUG) << "START CMIR::fill_IrOther"  ;

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
      LOG(logDEBUG) << "Bmd_IO_end_date = NULL"  ;
      md_IO_end_date[mi_IO_I] = AB_null ;
   }
   else
   {
      AB_Date ld_tempDate ;

      ld_tempDate = pt_section.At(55).For(5) + md_IO_start_date[mi_IO_I].At(6);
      md_IO_end_date[mi_IO_I] = pt_section.At(35).For(5) + ld_tempDate.get_default_year(md_IO_start_date[mi_IO_I]) ;
      LOG(logDEBUG) << "A)md_IO_end_date= " << md_IO_end_date[mi_IO_I]  ;
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

   // LOG(logDEBUG) << "END CMIR::fill_IrOther"  ;
}
*/

//-----------------------------------------------------------------------------

void CMIR::processOtherRemark(AB_Text &pt_section)
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
      LOG(logDEBUG) << "li_start= "   << li_start    ;
      LOG(logDEBUG) << "li_end= "     << li_end      ;
      LOG(logDEBUG) << "pt_section= " << pt_section  ;

      if (lt_remark.IsNull())
      {
         lt_remark = pt_section.For(li_end - 1) ;
         lt_remark.removeTrailingBlanks() ;
         LOG(logDEBUG) << "1)lt_value.At(5)= " << pt_section.For(li_end - 1)  ;
         LOG(logDEBUG) << "1)lt_remark= " << lt_remark  ;
      }
      else
      {
         lt_remark = lt_remark + "/" + pt_section.For(li_end - 1) ;
         lt_remark.removeTrailingBlanks() ;
         LOG(logDEBUG) << "2)lt_value.At(5)= " << pt_section.For(li_end - 1)  ;
         LOG(logDEBUG) << "2)lt_remark= " << lt_remark  ;

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
      LOG(logDEBUG) << "3)lt_remark= " << lt_remark  ;

      md_IRMK_itin_type[mi_IRMK_I]   = "T" ;
      md_IRMK_oth_line_number[mi_IRMK_I] = md_IO_line_number[mi_IO_I] ;
      md_IRMK_remark[mi_IRMK_I]         = lt_remark;
      md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;
   }

}

//-----------------------------------------------------------------------------

void CMIR::updateLLData()
{
   LOG(logDEBUG) << "START CMIR::updateLLData"  ;

   int li_counter ;
   D_char_date lm_date ;
   D_city lm_city_id ;
   
   lm_date = AB_null ; 
   li_counter = 0 ;

   LOG(logDEBUG) << "mi_IO_I= " << mi_IO_I  ;
   LOG(logDEBUG) << "md_IO_end_date= " << md_IO_end_date[li_counter]  ;

   if (md_IT_itin_type[mi_IT_I - 1] == "A")
      lm_date = md_IT_start_date[mi_IT_I - 1] ;
 
   if (md_IT_itin_type[0] == "A")
      lm_city_id = md_IT_orig_city_id[0] ;

   LOG(logDEBUG) << "lm_date= " << lm_date  ;

   LOG(logDEBUG) << "li_counter= " << li_counter  ;
   LOG(logDEBUG) << "mi_IO_I= " << mi_IO_I  ;
   LOG(logDEBUG) << "NEW"  ;

   md_ID_form[mi_ID_I-1]        = AB_null;
   md_ID_ticket_no[mi_ID_I-1]   = AB_null;
   md_ID_no_of_books[mi_ID_I-1] = AB_null;

   while (li_counter < mi_IO_I)
   {
      LOG(logDEBUG) << "IN while loop GO"  ;

      if (lm_city_id.NotIsNull()) 
         md_IO_orig_city_id[li_counter] = lm_city_id ;

      if (md_IO_end_date[li_counter].IsNull())
      {
         LOG(logDEBUG) << "setting GO end date"  ;

         if (lm_date.IsNull())
         {
            md_IO_end_date[li_counter] = md_IO_start_date[li_counter] ;
            LOG(logDEBUG) << "1) md_IO_end_date[li_counter]= " << md_IO_end_date[li_counter] ; 
         }
         else
         {
            md_IO_end_date[li_counter] = lm_date ;
            LOG(logDEBUG) << "1) md_IO_end_date[li_counter]= " << md_IO_end_date[li_counter] ;
         }
      }
      li_counter++ ;
   }

   if (md_IO_reference[0].NotIsNull())
   {
      LOG(logDEBUG) << "START IT reference change"  ;
      li_counter = 0 ;
      while (li_counter < mi_IT_I)
      {
         md_IT_reference[li_counter++] = md_IO_reference[0] ;
         LOG(logDEBUG) << "md_IO_reference[li_counter - 1]= " << md_IO_reference[li_counter - 1]  ;
         LOG(logDEBUG) << "md_IO_reference[0]= " << md_IO_reference[0]  ;
      }
   }

   LOG(logDEBUG) << "END CMIR::updateLLData"  ;
}

//-----------------------------------------------------------------------------

void CMIR::fill_IrPasDocument(CPNR &po_PNR)
{
   // LOG(logDEBUG) << "START CMIR::fill_IrPasDocument"  ;

   int                li_counter, li_pax_id, li_pos, li_taxCounter ;
   //AB_Boolean       lb_foundLL ;
   AB_Text            lt_section ;
   int                li_A02_index ;
   AB_Varchar         lv_longTicket(255) ;
	V_AB_Varchar       lva_arrayField(100,100) ;

	D_passenger_no		 ld_paxId ;
   AB_Boolean         lb_pickTkt ;
   AB_Boolean         lb_commOverride ;
	AB_Varchar         lv_commOverride(10);
	AB_Char            lc_docLine(2) ;
	int					 li_pax_dot;
	int					 li_cur_pax_id;

   li_pax_id = 0 ;
   li_counter = 0 ;
   li_taxCounter = 0 ;

   li_counter = 0 ;
   //po_PNR.resetSegment("LL") ;
   //lt_section = po_PNR.getNextSegment("LL");

	mi_tktCounter = 1 ;
   
   //if (lt_section.IsNull())
      //lb_foundLL = AB_false ;
   //else
      //lb_foundLL = AB_true ;


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
					 		LOG(logDEBUG) << "NON ARC CREDIT CARD" << mv_NonArcCC  ;
		
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

		//if ( md_ID_doc_type[mi_ID_I]   != "BSP" )
			//md_ID_saving_code[mi_ID_I] = AB_null ;


		if (md_ID_ticket_no[mi_ID_I].IsNull() && md_ID_base_cost[mi_ID_I].IsNull())
			mi_tktCounter-- ;
		else {
			if (mb_MCO_TASF) {
				createMCOConsumDetail();
			}
			mi_ID_I++ ;
		}	

		AB_Text   lv_freq ;
		lv_freq = lt_section.At(4).For(33) ;
    
      processFrequentFlyer(po_PNR, lv_freq, li_pax_id) ;

      mi_IP_I++ ;

      lt_section = po_PNR.getNextSegment("A02");
   }

	if ( md_ITD_tax_id[mi_ITD_I].NotIsNull() )
		mi_ITD_I++ ;


	//SCR#510686, service fee not created for specificed pax
	for ( int li_index = 0; li_index < mi_IRMK_I; li_index++ )
	{
		if ( md_IRMK_remark[li_index].length() > 0  &&
			  md_IRMK_type[li_index] == "SFM" )
		{
	      li_pax_dot    = md_IRMK_remark[li_index].findSubstring(".");
         li_cur_pax_id = md_IRMK_pax_id[li_index].to_long();

         if ( li_pax_dot > 0 && li_cur_pax_id <= mi_IP_I )
         {
            md_IRMK_pax_id[li_index] = md_IP_pax_id[ li_cur_pax_id  - 1 ];
         }else{
            md_IRMK_pax_id[li_index] = md_IP_pax_id[0];
         }		

		}
	}


   // LOG(logDEBUG) << "END CMIR::fill_IrPasDocument"  ;
}

//-----------------------------------------------------------------------------

D_pic CMIR::getPicCode(AB_Text &pc_pic)
{
   // LOG(logDEBUG) << "START CMIR::getPicCode"  ;

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

   // LOG(logDEBUG) << "END CMIR::getPicCode"  ;
}

//------------------------------------------------------------------------------

AB_Boolean CMIR::processFare(	CPNR     &po_PNR, 
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

   if (lc_fare_num == "  ")
   {
	  lc_fare_num = "01";
	}

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
         processTax(lt_tax,ld_totTax, po_PNR) ; 
      }
      else
      {
			lt_tax = lt_section.At(51) ;
         processTax(lt_tax,ld_totTax, po_PNR) ;
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

		   if (md_ID_ticket_no[mi_ID_I].NotIsNull() && md_A11FOP == "EX" 
				|| lc_exch_type == "R" )
		   {
				 /**
				  * Since mi_IRP_I has be incremented fill_Irpayment
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
}

//-----------------------------------------------------------------------------

void CMIR::processTax(AB_Text   &pt_section,
							 D_amount  &pd_totTax,
							 CPNR       &po_PNR)
{
   ENTER;
   LOG(logDEBUG) << "\npt_section:" << pt_section  ;

   AB_Boolean   lb_finished, lb_additional_tax ;
   D_amount     lm_XT_tax, lm_value, li_additional_tax ;
   int          li_index, li_tempIndex ;
   AB_Money     lm_amount ;
   AB_Varchar   lv_rawAmount(15) ;
   AB_BOOL      lb_ok ;
	int          li_XFIndex ;
	AB_Varchar   lv_XFTaxes(32),lv_temp(32) ;
	AB_Decimal   ld_totXFTax(5,2) ,ld_XFTax(5,2) ;

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

					LOG(logDEBUG) << "We have  additional  XT tax : " << lm_value  ;
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
						getA09XFtaxes(po_PNR,lv_XFTaxes);

					   LOG(logDEBUG) << "We have  additional  XF tax : " << lm_value  ;
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
		LOG(logDEBUG) << "We do have additional tax XF XT! "  ;

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

							 getA09XFtaxes(po_PNR,lv_XFTaxes);

						}
						else if ( ld_tax == "XT" && lv_XFTaxes.IsNull())
						{
							/**
							 * We have XT tax , we might have XF: following
							 **/

							 getA09XFtaxes(po_PNR,lv_XFTaxes);

							 if ( lv_XFTaxes.NotIsNull() ) // we have XF details ; Get Total XF amount
							 {

								lv_temp = lv_XFTaxes ;

								while ( lv_temp.length() > 3 )
								{
									int li_ind, li_length;

									li_ind = lv_temp.At(4).findFirstCharNotInSet(".1234567890") ;

									li_length = li_ind ? li_ind - 1 : lv_temp.length() - 3;

									ld_XFTax = AB_Decimal(5,2,lv_temp.At(4).For(li_length) );

									ld_totXFTax = ld_totXFTax + D_amount(ld_XFTax) ;

									lv_temp = lv_temp.At(4 + li_length)  ;
								}

								/**
								 * Take away XF taxes
								 **/

								ld_taxAmt = ld_taxAmt - D_amount(ld_totXFTax); // New spot
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

			if ( lv_XFTaxes.NotIsNull() )
			{
				while ( lv_XFTaxes.length() > 3 )
				{
					int li_ind, li_length;

					li_ind = lv_XFTaxes.At(4).findFirstCharNotInSet(".1234567890") ;

					li_length = li_ind ? li_ind - 1 : lv_XFTaxes.length() - 3;

					ld_XFTax = AB_Decimal(5,2,lv_XFTaxes.At(4).For(li_length) );

   				md_ITD_line_number[mi_ITD_I]  = md_ID_pax_id[mi_ID_I] ;
            	md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            	md_ITD_tax_id[mi_ITD_I]       = "XF" ;
					md_ITD_city_id[mi_ITD_I]      = lv_XFTaxes.For(3) ;
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

		if ( lv_XFTaxes.NotIsNull() )
		{
			/**
			 * Create each XF taxes
			 **/

			while ( lv_XFTaxes.length() > 3 )
			{
				int li_ind, li_length;

				li_ind = lv_XFTaxes.At(4).findFirstCharNotInSet(".1234567890") ;

				li_length = li_ind ? li_ind - 1 : lv_XFTaxes.length() - 3;

				ld_XFTax = AB_Decimal(5,2,lv_XFTaxes.At(4).For(li_length) );

   			md_ITD_line_number[mi_ITD_I]  = md_ID_pax_id[mi_ID_I] ;
            md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            md_ITD_tax_id[mi_ITD_I]       = "XF" ;
				md_ITD_city_id[mi_ITD_I]      = lv_XFTaxes.For(3) ;
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

//------------------------------------------------------------------------------

void CMIR::fillIrDocumentTax(D_char_numeric &pd_taxAmount, D_tax &pd_taxId)
{
   // LOG(logDEBUG) << "START CMIR::fillIrDocumentTax"  ;

   
   md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
   md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
   md_ITD_tax_id[mi_ITD_I]       = pd_taxId ;
   md_ITD_tax_amount[mi_ITD_I++]   = pd_taxAmount ;

   // LOG(logDEBUG) << "END CMIR::fillIrDocumentTax"  ;
}


//------------------------------------------------------------------------------

void CMIR::processOrigFare(CPNR &po_PNR, 
									AB_Char &pc_exch_num, 
									AB_Char     &pc_docLine,
									AB_Char &pc_exch_type) 

{
   ENTER;

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

   md_ID_penalty_ch_fee[mi_ID_I] = lt_section.At(51).For(9) ;
   if (md_ID_penalty_ch_fee[mi_ID_I] == " ")
      md_ID_penalty_ch_fee[mi_ID_I] = AB_null ;

   md_ID_comm_penalty[mi_ID_I] = lt_section.At(60).For(9) ;
   if (md_ID_comm_penalty[mi_ID_I] == " ")
      md_ID_comm_penalty[mi_ID_I] = AB_null ;

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

			LOG(logDEBUG) << "md_orig_form_of_pay =["  << md_orig_form_of_pay << "]";

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
         else if ( md_orig_form_of_pay == "  " ) 
			{
               md_orig_form_of_pay = "CA";
               md_orig_cc_id = AB_null;
               md_orig_cc_no = AB_null;
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

   EXIT;
}

//------------------------------------------------------------------------------

void CMIR::processExchangeTaxes(	V_AB_Integer   &pia_exch_pax_id ,
            							V_AB_Varchar   &pva_exch_tax_id ,
            							V_AB_Varchar   &pva_exch_tax_amount,
            							int            &pi_exchangeTax_index)
{
   LOG(logDEBUG) << "START CMIR::processExchangeTaxes"  ;

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

   LOG(logDEBUG) << "END CMIR::processExchangeTaxes"  ;
}

//-----------------------------------------------------------------------------

void CMIR::calculateExchangeFare(D_char_numeric &pm_exchBaseCost, 
											D_char_numeric &pm_exchCommission)
{
   LOG(logDEBUG) << "START CMIR::calculateExchangeFare"  ;

   int li_exchangeTax_index ;
   D_amount li_orig_baseCost, li_exch_baseCost, li_orig_Commission, li_exch_Commission ;
   AB_BOOL lb_ok ;
   AB_Varchar lv_rawAmount(15) ;
   AB_Money lm_amount ;

   LOG(logDEBUG) << "pm_exchBaseCost= " << pm_exchBaseCost  ;
   LOG(logDEBUG) << "pm_exchCommission= " << pm_exchCommission  ;

   LOG(logDEBUG) << "start A"  ;
   lv_rawAmount = md_ID_base_cost[mi_ID_I];
   lm_amount = lv_rawAmount.to_money("#####.##-", lb_ok) ;
   li_orig_baseCost = lm_amount.to_string() ;
   LOG(logDEBUG) << "A: lv_rawAmount= " << lv_rawAmount  ;
   LOG(logDEBUG) << "A: lm_amount= " << lm_amount  ;
   LOG(logDEBUG) << "A:li_orig_baseCost= " << li_orig_baseCost  ;
   LOG(logDEBUG) << "end A"  ;

   if (lb_ok)
   {
      LOG(logDEBUG) << "start B"  ;
      lv_rawAmount = pm_exchBaseCost ;
      lm_amount = lv_rawAmount.to_money("##########.##-", lb_ok) ;
      li_exch_baseCost = lm_amount.to_string() ;
      LOG(logDEBUG) << "lv_rawAmount= " << lv_rawAmount  ;
      LOG(logDEBUG) << "lm_amount= " << lm_amount  ;
      LOG(logDEBUG) << "li_exch_baseCost= " << li_exch_baseCost  ;
      LOG(logDEBUG) << "end B" ;
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

//------------------------------------------------------------------------------

void CMIR::calculateExchangeTax(AB_Integer     &pi_paxId,
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
		//LOG(logDEBUG) << "EXCH" << pva_exchTaxId[li_exchCnt]  ;
		li_counter = 0 ;
		lb_found = AB_false ;
		while (li_counter < li_ITD_I)
		{
			//LOG(logDEBUG) << "ORIG" << md_ITD_tax_id[li_counter]  ;
			if ( md_ITD_pax_id[li_counter] == D_passenger_no(pi_paxId) )
			{
				if ( pva_exchTaxId[li_exchCnt] == md_ITD_tax_id[li_counter] )
				{
					//LOG(logDEBUG) << "ORIG" << md_ITD_tax_amount[li_counter]  ;
					ld_tax1 = md_ITD_tax_amount[li_counter] ;
					ld_tax2 = pva_exchTax[li_exchCnt] ;
					//LOG(logDEBUG) << "EXCH" << pva_exchTax[li_exchCnt]  ;
					ld_tax3 = ld_tax1 - ld_tax2 ;
					if ( ld_tax3 < AB_Decimal(0.00) )
						ld_tax3 = ld_tax3 * AB_Decimal(-1) ;
					md_ITD_tax_amount[li_counter] = ld_tax3 ;
					//LOG(logDEBUG) << "NEW" << md_ITD_tax_amount[li_counter]  ;
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

//------------------------------------------------------------------------------

void CMIR::processFrequentFlyer(CPNR &po_PNR, AB_Text &pm_name , int &pi_pax_id)
{
    ENTER;
   
   int li_counter, li_line_number, li_pos ;
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
            md_IRF_fup[mi_IRF_I] = lt_segment.At(28).For(20).to_string() ;
				li_pos = md_IRF_fup[mi_IRF_I].findSubstring("!") ;
				if (li_pos > 0) {
					md_IRF_fup[mi_IRF_I] = md_IRF_fup[mi_IRF_I].For(li_pos - 1) ;
				}

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
            	md_IRF_fup[mi_IRF_I]= lt_segment.At(28).For(20).to_string() ;
					li_pos = md_IRF_fup[mi_IRF_I].findSubstring("!") ;
					if (li_pos > 0) {
						md_IRF_fup[mi_IRF_I] = md_IRF_fup[mi_IRF_I].For(li_pos - 1) ;
					}

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

//------------------------------------------------------------------------------

AB_Boolean CMIR::LLUpdateIrperson(CPNR &po_PNR) 
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
      LOG(logDEBUG) << "END NO DATA CMIR::LLUpdateIrperson"  ;
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
   
   LOG(logDEBUG) << "START CMIR::LLUpdateIrperson"  ;
   return AB_true ;
}

//-----------------------------------------------------------------------------

void CMIR::createIrItins(CPNR &po_PNR)
{
   ENTER;

	if (!mb_MCO_TASF) {
   	fillIrconsumdetail(po_PNR) ;
   	populateIrTransportSectionA04(po_PNR) ;
	}
	
	if (mb_MCO && !mb_MCO_TASF && mi_IT_I == 0) {
		// in case of MCO but not TASF, we must have an air segment
   	CMsgHandler::setMsg(3464, " ", "AIR-SEG", 1, AB_Varchar(20,AB_null));
	}

   //populateIrTransportSectionA05(po_PNR) ;

   EXIT;
}

//------------------------------------------------------------------------------

void CMIR::populateIrTransportSectionA04(CPNR &po_PNR)
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
		if (!validA04(lt_segment)) {
			// ignore invalid A04. In case of MCO, some empty A04 may exist in PNR
      	lt_segment = po_PNR.getNextSegment("A04");
			continue;
		}

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

      //SCR516402
      li_pos = lt_segment.findSubstring("DDL:") ;
      if ( li_pos > 0 )
      {
         if ( PCRE::pcre_match("[0-9]{2}[A-Z]{3}[0-9]{2}", lt_segment.At(li_pos + 4).For(7).to_string() ) )
         {
            md_IT_start_date[mi_IT_I] = lt_segment.At(li_pos + 4).For(7) ;
         }
      }


      mi_IT_I++ ;
      lt_segment = po_PNR.getNextSegment("A04");
   }

   EXIT;
}

//------------------------------------------------------------------------------

void CMIR::populateIrTransportSectionA05(CPNR &po_PNR)
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

//------------------------------------------------------------------------------

void CMIR::commonIrTransportFields(AB_Text &pt_segment)
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

//-----------------------------------------------------------------------------
// US044 START
void CMIR::IrTransportRemarks(CPNR &po_PNR)
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

//------------------------------------------------------------------------------

void CMIR::fillIrconsumdetail(CPNR &po_PNR)
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

//------------------------------------------------------------------------------

void CMIR::populateIrconsumdetailSectionA06(CPNR &po_PNR)
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

               LOG(logDEBUG) << "li_counter= " << li_counter  ;
               LOG(logDEBUG) << "li_pos= " << li_pos  ;
               LOG(logDEBUG) << "li_pax_pos= " << li_pax_pos  ;
               LOG(logDEBUG) << "md_ICD_pax_id[li_counter]= " << md_ICD_pax_id[li_counter]  ;
               LOG(logDEBUG) << "lt_segment.At(li_pax_pos).For(3)= " << lt_segment.At(li_pax_pos).For(3)  ;
               LOG(logDEBUG) << "lt_segment.At(li_pax_pos.to_long() + 3).For(1)= " << lt_segment.At(li_pax_pos.to_long() + 3).For(1)  ;
               LOG(logDEBUG) << "li_value= " << li_value  ;
            }
         }
         li_counter++ ;
      }
      lt_segment = po_PNR.getNextSegment("A06") ;
   }

   EXIT; 
}

//------------------------------------------------------------------------------

void CMIR::populateIrconsumdetailSectionA04(CPNR &po_PNR)
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

//------------------------------------------------------------------------------

void CMIR::updateConsumdetail()
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

//------------------------------------------------------------------------------

void CMIR::processNonArcDoc()
{
	ENTER;

	AB_Text         lt_section ;
	AB_Char         lc_docLine(2) ;
	AB_Char         lc_docPax(2) ;
	int             li_index ;
	AB_Varchar		 lv_ticketNo(10);

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
				break ;
			}
			md_ID_pax_id[mi_ID_I]      = md_IP_pax_id[li_index - 1] ;

      	md_ID_ticket_no[mi_ID_I]   = md_nonArcDoc[li_index].For(10) ;
         md_ID_form[mi_ID_I]        = AB_null ;
      	md_ID_no_of_books[mi_ID_I] = AB_Integer(1) ;
		}
		else
		{
			/*** change as per scr#2788
			** when ticket_no is already 999999999 then retain ticket_no
			** but to identify non-arc tickets, pass 000000000
			**/

			md_ID_ticket_no[mi_ID_I]   =  "0000000000" ;
      	md_ID_pax_id[mi_ID_I]      = md_IP_pax_id[0] ;
         md_ID_form[mi_ID_I]        = AB_null ;
      	md_ID_no_of_books[mi_ID_I] = AB_Integer(1) ;
			LOG(logDEBUG) << "\nmd_ID_ticket_no " << md_ID_ticket_no[mi_ID_I]  ;
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

		if (lb_isPFS) {
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
   EXIT;
}

//------------------------------------------------------------------------------

void CMIR::processNonArcCost(AB_Text  &pt_section)
{
	ENTER;

	V_AB_Varchar       lva_arrayField(100,100) ;
	D_amount           ld_cost, ld_tax ;
	AB_Text            lt_segment ;
	AB_Boolean 	   lb_TK_found;
	int 		   index;
	int		   li_count; 

	md_ID_sup_id[mi_ID_I]     = pt_section.At(5).For(3) ;
	md_ID_cur_id[mi_ID_I]      = "USD" ;

	int li_pos       = pt_section.findSubstring("!") ;

	if (li_pos > 0)
		pt_section = pt_section.For(li_pos -1) ;

	li_count = lva_arrayField.parseStringMultDel(pt_section,"/") ;

	lb_TK_found = AB_false;

	for ( index = 2; index <= li_count; index++ )
	{
		lva_arrayField[index].leftJustify();
		lva_arrayField[index].removeLeadAndTrailBlanks();

		if ( 	lb_TK_found == AB_false	&&
				lva_arrayField[index].For(3) == "TK-" )
		{
			li_pos = lva_arrayField[index].At(4).findFirstCharNotInList("0123456789");

			if ( li_pos == 0 && lva_arrayField[index].At(4).NotIsNull() )
      		md_ID_ticket_no[mi_ID_I] = lva_arrayField[index].At(4);

			lb_TK_found = AB_true;
		}
	}

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

	/************************************************************************************
	if ( lva_arrayField[5].For(1) == "N" && lva_arrayField[5].At(2).For(1).isAllNumeric())
		lt_segment = lva_arrayField[6] ;
	else
		lt_segment = lva_arrayField[5] ;
		

	if ( md_IRP_form_of_pay[mi_IRP_I] == "CC" )
		fillTktFP(lt_segment) ;
	************************************************************************************/

   if ( md_IRP_form_of_pay[mi_IRP_I] == "CC" )
	{
		lt_segment = AB_null;
      fillTktFP(lt_segment) ;
	}

	mi_IRP_I++ ;
   EXIT;
}

//------------------------------------------------------------------------------

bool CMIR::fill_IrOther(AB_Text &pt_section)
{
   ENTER;

	AB_Char       lc_productType(1);
	AB_Varchar    lv_field(80) ;
	AB_Char       lc_year(2) ;


   LOG(logDEBUG) << "START  CMIR::IrOther"  ;

   string itinType, itinTypeForACC;
	pRemarkParser->getData("LABELS", "TYP", itinType);

		  if ( itinType == "SEA" ) itinTypeForACC = "SEA";
	else if ( itinType == "TOR" ) itinTypeForACC = "TUR";
	else                          itinTypeForACC = "OTH";

	int li_pos = pt_section.findSubstring("DP:") ;
	int li_pos2 = pt_section.findSubstring("OD-") ;

   if (li_pos==0 && !pRemarkParser->isRemarkParsed("LABELS") && li_pos2==0 )
		return false;

   LOG(logDEBUG) << "START  CMIR::IrOther 1111111111"  ;

   if (li_pos>0)
		md_IO_sup_id[mi_IO_I] = pt_section.At(li_pos + 6).For(3) ;
	else
   {
	  string remarkId = "ACC/";  //remark like: [ACC/BUS02/]
	  remarkId += itinTypeForACC;
	  remarkId += pt_section.At(5).For(2).to_string();
	  remarkId += "/";
	  if (!RP_saveDataTo(remarkId, 
								"SUP", 
								md_IO_sup_id[mi_IO_I])
			)
		 return false;
	}

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
      LOG(logDEBUG) << "Bmd_IO_end_date = NULL"  ;
		md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;
   }
   else
   {
      AB_Date ld_tempDate ;

      ld_tempDate = md_IO_start_date[mi_IO_I];

      lc_year = ld_tempDate.get_default_year(pt_section.At(55).For(5)) ;
		lc_year.rightJustify('0') ;

      md_IO_end_date[mi_IO_I] = pt_section.At(55).For(5) + lc_year ;
      LOG(logDEBUG) << "A)md_IO_end_date= " << md_IO_end_date[mi_IO_I]  ;
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

	// add remarks as per SCR518984
   V_AB_Text      lv_arrayField(100);
	int li_pos3 = pt_section.findSubstring("TYP-") ;
   int li_count;
	string unparsedLabels, labels, oneLine = "";

   li_count= lv_arrayField.parseStringMultDel(pt_section, "/");

   if (!pRemarkParser->isRemarkParsed("LABELS") // no LABELS
		 && li_pos3 != 0  // there is unknown TYP
		 && li_pos2 !=0   // there is OD-
		 && li_count > 1 )// many slashes
	{
	  // make one line from multi lines, then break for multiple remarks
		unparsedLabels = pt_section.to_string();
		LOG(logDEBUG) << "===unparsedLabels[" << unparsedLabels.data() << "]===" ;
		while( 	PCRE::pcre_match("(OD-|FF:)([^!]+)", unparsedLabels) )
		{
		  PCRE::pcre_get_group(2, labels);
		  PCRE::pcre_get_after_group(0, unparsedLabels);
		  LOG(logDEBUG) << "-----labels[" << labels.data() << "]---" ;
		  LOG(logDEBUG) << "===unparsedLabels[" << unparsedLabels.data() << "]===" ;
		  oneLine += labels;
		  //pRemarkParser->parseByName("LABELS", labels);
		}
		LOG(logDEBUG) << "===oneLine[" << oneLine.data() << "]===" ;


		AB_Text        lt_data;
		AB_Text        lt_remark;
		int            li_pos;
		lt_data = oneLine.data();
      while ( lt_data.NotIsNull() && lt_data.length() > 0 )
      {
         lt_remark = lt_data.For(70);
         li_pos = 70;

         if ( lt_data.length() > 70 )
         {
            li_pos = lt_remark.getReverseIndex(" ");

            if ( li_pos > 60 )
               lt_remark = lt_remark.For(li_pos);
            else
               li_pos = 70;
         }

         md_IRMK_remark[mi_IRMK_I]  = lt_remark.For(70);
		   LOG(logDEBUG) << "md_IRMK_remark[" << md_IRMK_remark[mi_IRMK_I] << "]===" ;

         if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
         {
			  //md_IRMK_type[mi_IRMK_I]             = getRemarkType(md_IO_itin_type[mi_IO_I].to_string()).data();
            md_IRMK_type[mi_IRMK_I]             = "PN" + md_IO_itin_type[mi_IO_I];
            md_IRMK_oth_line_number[mi_IRMK_I]  = md_IO_line_number[mi_IO_I];
            md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[mi_IO_I];
            md_IRMK_pax_id[mi_IRMK_I]           = AB_null;
            mi_IRMK_I++;
         }

         lt_data = lt_data.At(li_pos+1);
      }



	}


	addIrconsumdetail(md_IO_sup_id[mi_IO_I],
							md_IO_itin_type[mi_IO_I],
							md_IO_line_number[mi_IO_I]) ;

   mi_IO_I++ ;

   EXIT; 

	return true;
}



//-----------------------------------------------------------------------------

void CMIR::getItinType(AB_Char  &pc_productType,
							  AB_Char  &pc_itinType,
							  AB_Char  &pc_unitType)
{
	if (pc_productType == "C")
	{
		pc_itinType = "S" ;
		pc_unitType = "SEA" ;
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
		pc_itinType = "C" ;
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
		pc_unitType = "OTH" ;
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

//------------------------------------------------------------------------------

void CMIR::addIrconsumdetail(D_supplier    &pd_supId,
						           AB_Char        &pc_itinType,
						           AB_Integer     &pi_itinLine)
{
	int 	li_index = 1 ;
   while (li_index <= mi_nonArcDoc)
   {  
		// SCR: 519830
		AB_Varchar lv_line_id(2);
		lv_line_id = md_nonArcCost[li_index].For(2);
		AB_Integer li_itinLine = AB_Integer(lv_line_id);

		if ((li_itinLine == pi_itinLine) && (md_nonArcCost[li_index].At(5).For(3) == pd_supId ))
		{
			md_ICD_pax_id[mi_ICD_I] = md_nonArcCost[li_index].At(3).For(2) ;
			md_ICD_itin_type[mi_ICD_I] = pc_itinType ;
			md_ICD_line_number[mi_ICD_I] = md_nonArcCost[li_index].For(2) ;
			md_ICD_itin_line_number[mi_ICD_I++] = pi_itinLine ;
		}
		 li_index++ ;
	}

}

//------------------------------------------------------------------------------

void CMIR::fill_IrBus(AB_Text &pt_section)
{
	AB_Char       lc_productType(1);
	AB_Varchar     lv_data(500);
	AB_Char      lc_year(2) ;

	int li_pos = pt_section.findSubstring("DP:") ;

   if (li_pos==0 && !pRemarkParser->isRemarkParsed("LABELS"))
		return ;

   if (li_pos>0)
		md_IT_sup_id[mi_IT_I] = pt_section.At(li_pos + 6).For(3) ;
	else
   {
	  //remark like:r1-ACC/BUS/ or r2-ACC/BUS00/ or r3-ACC/BUS02
	  string remarkACCbase = "ACC/BUS";  //remark like:ACC/BUS/ or ACC/BUS00/
	  string r1 = remarkACCbase + "/";
	  string r2 = remarkACCbase + "00/";
	  string r3 = remarkACCbase + pt_section.At(5).For(2).to_string() + "/";
	  if (   !RP_saveDataTo(r1, "SUP", md_IT_sup_id[mi_IT_I])
			&& !RP_saveDataTo(r2, "SUP", md_IT_sup_id[mi_IT_I])
			&& !RP_saveDataTo(r3, "SUP", md_IT_sup_id[mi_IT_I])
			)
		 return;
	}


	md_IT_line_number[mi_IT_I]    = pt_section.At(5).For(2) ;

	md_IT_itin_type[mi_IT_I]      = "B" ;

   md_IT_start_date[mi_IT_I] = pt_section.At(7).For(7) ;

   md_IT_orig_city_id[mi_IT_I] = pt_section.At(49).For(3) ;
   md_IT_dest_city_id[mi_IT_I] = pt_section.At(49).For(3) ;
	md_IT_arrival_ind[mi_IT_I]    = 0 ;

	md_IT_service[mi_IT_I] = "9999" ;

        // G9150
	if (pRemarkParser->isRemarkParsed("LABELS"))
        {
	  //------ STS ------------------
	  if (RP_saveDataTo("LABELS", "STS", md_ICD_service_status[mi_ICD_I]))
          {
   	     md_ICD_pax_id[mi_ICD_I] = "1" ;
   	     md_ICD_itin_type[mi_ICD_I] = md_IT_itin_type[mi_IT_I] ;
   	     md_ICD_line_number[mi_ICD_I] = "1" ;
   	     md_ICD_itin_line_number[mi_ICD_I] = md_IT_line_number[mi_IT_I] ;
          }
          
	  //ignore SUC for BUS RP_saveDataTo("LABELS", "SUC", md_IT_sup_id[mi_IT_I]);

	  //------ CF ------------------
          li_pos = pt_section.findSubstring("CF:") ;
          if ( li_pos > 0 )
          {
             lv_data = pt_section.At( li_pos ) ;
             li_pos = lv_data.findSubstring("!") ;
             if ( li_pos > 0 )
             {
                lv_data = lv_data.For( li_pos - 1 ) ;
             }
             md_IT_airline_locator[mi_IT_I] = lv_data.At(4);
          }
        }

	addIrconsumdetail(md_IT_sup_id[mi_IT_I],
							md_IT_itin_type[mi_IT_I],
							md_IT_line_number[mi_IT_I]) ;


   mi_IT_I++ ;

}

//------------------------------------------------------------------------------

void CMIR::fillIrRail(AB_Text &pt_segment)
{
	AB_Char       lc_productType(1);
	AB_Varchar     lv_data(500);
	AB_Char      lc_year(2) ;

	int li_pos = pt_segment.findSubstring("DP:") ;

   if (li_pos==0 && !pRemarkParser->isRemarkParsed("LABELS"))
		return;

   if (li_pos>0)
		md_IT_sup_id[mi_IT_I] = pt_segment.At(li_pos + 6).For(3) ;
	else
   {
	  //remark like:r1-ACC/RAL/ or r2-ACC/RAL00/ or r3-ACC/RAL02
	  string remarkACCbase = "ACC/RAL";  
	  string r1 = remarkACCbase + "/";
	  string r2 = remarkACCbase + "00/";
	  string r3 = remarkACCbase + pt_segment.At(5).For(2).to_string() + "/";
	  if (   !RP_saveDataTo(r1, "SUP", md_IT_sup_id[mi_IT_I])
			&& !RP_saveDataTo(r2, "SUP", md_IT_sup_id[mi_IT_I])
			&& !RP_saveDataTo(r3, "SUP", md_IT_sup_id[mi_IT_I])
			)
		 return;
	}

        md_IT_line_number[mi_IT_I]    = pt_segment.At(5).For(2) ;

        md_IT_itin_type[mi_IT_I]     = "R" ;

        md_IT_start_date[mi_IT_I] = pt_segment.At(7).For(7) ;

        md_IT_orig_city_id[mi_IT_I] = pt_segment.At(49).For(3) ;
        md_IT_dest_city_id[mi_IT_I] = pt_segment.At(49).For(3) ;

        md_IT_arrival_ind[mi_IT_I]    = 0 ;
        md_IT_service[mi_IT_I] = "9999" ;

        // G9150
        if (pRemarkParser->isRemarkParsed("LABELS"))
        {

	  //------ STS ------------------
	  if (RP_saveDataTo("LABELS", "STS", md_ICD_service_status[mi_ICD_I]))
          {
   	     md_ICD_pax_id[mi_ICD_I] = "1" ;
   	     md_ICD_itin_type[mi_ICD_I] = md_IT_itin_type[mi_IT_I] ;
   	     md_ICD_line_number[mi_ICD_I] = "1" ;
   	     md_ICD_itin_line_number[mi_ICD_I] = md_IT_line_number[mi_IT_I] ;
          }
          
	  //ignore SUC for RAIL RP_saveDataTo("LABELS", "SUC", md_IT_sup_id[mi_IT_I]);
          
	  //------ CF ------------------
          li_pos = pt_segment.findSubstring("CF:") ;
          if ( li_pos > 0 )
          {
             lv_data = pt_segment.At( li_pos ) ;
             li_pos = lv_data.findSubstring("!") ;
             if ( li_pos > 0 )
             {
                lv_data = lv_data.For( li_pos - 1 ) ;
             }
             md_IT_airline_locator[mi_IT_I] = lv_data.At(4);
          }
       
        }

	addIrconsumdetail(md_IT_sup_id[mi_IT_I],
							md_IT_itin_type[mi_IT_I],
							md_IT_line_number[mi_IT_I]) ;


   mi_IT_I++ ;
}

//------------------------------------------------------------------------------

void CMIR::fillTktFP(AB_Text &pv_CCInfo) 
{
	ENTER;
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

	if (md_TKX_cc_exp_date.NotIsNull()) {
		md_IRP_cc_exp_date[mi_IRP_I]  = md_TKX_cc_exp_date;
	}

	// TKX CC EXP DATE END
}

//------------------------------------------------------------------------------

void CMIR::updateTaxdetail()
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

//------------------------------------------------------------------------------

void CMIR::fillIrconsumDetailRental(AB_Char    &pc_itinType,
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

//------------------------------------------------------------------------------

void CMIR::checkRental()
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

//------------------------------------------------------------------------------

void CMIR::processExtraTicketLine()
{
	ENTER;
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
	
//------------------------------------------------------------------------------

void CMIR::processTaxExtraTicketLine(int &pi_tktLine) 
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

//------------------------------------------------------------------------------

void CMIR::parsePhone(AB_Varchar    &pv_phone)
{
   int li_pos ;
	AB_Varchar		lv_long_Rphone(100) ;
	AB_Varchar		lv_long_Hphone(100) ;   // 516462
	AB_Varchar		lv_long_Bphone(100) ;
	AB_Varchar		lv_long_Fphone(100) ;
	AB_Varchar		lv_long_Mphone(100) ;
	int li_init = 1 ;
	int li_testPos ;

	AB_Varchar     lv_email(255);
	AB_Varchar     lv_email_before(255);
	AB_Varchar     lv_email_after(255);

	lv_long_Rphone = AB_null ;
	lv_long_Bphone = AB_null ;
	lv_long_Fphone = AB_null ;
	lv_long_Mphone = AB_null ;

	pv_phone.removeTrailingBlanks() ;

	lv_email = pv_phone;

	pv_phone = pv_phone + "/" ;
	lv_long_Rphone = pv_phone.getNthBracketedField(1, "R/", "/") ;
	if ( lv_long_Rphone.NotIsNull() )
        {
	  editPhoneNumber(lv_long_Rphone, md_IA_phone_acode[mi_IA_I], md_IA_phone_no[mi_IA_I]);
        }

        // 516462, treat H same as R
	lv_long_Hphone = pv_phone.getNthBracketedField(1, "H/", "/") ;
	if ( lv_long_Hphone.NotIsNull() )
        {
	  editPhoneNumber(lv_long_Hphone, md_IA_phone_acode[mi_IA_I], md_IA_phone_no[mi_IA_I]);
        }

	lv_long_Bphone = pv_phone.getNthBracketedField(1, "B/", "/") ;
	if ( lv_long_Bphone.NotIsNull() )
        {
	  editPhoneNumber(lv_long_Bphone, md_IA_bphone_acode[mi_IA_I], md_IA_bphone_no[mi_IA_I]);
        }

	lv_long_Mphone = pv_phone.getNthBracketedField(1, "M/", "/") ;
	if ( lv_long_Mphone.NotIsNull() )
        {
	  editPhoneNumber(lv_long_Mphone, md_IA_mphone_acode[mi_IA_I], md_IA_mphone_no[mi_IA_I]);
        }

	// parse cell if mobile is empty
	if ( md_IA_mphone_no[mi_IA_I].IsNull() )
	{
		lv_long_Mphone = pv_phone.getNthBracketedField(1, "C/", "/") ;
	        if ( lv_long_Mphone.NotIsNull() )
                {
		  editPhoneNumber(lv_long_Mphone, md_IA_mphone_acode[mi_IA_I], md_IA_mphone_no[mi_IA_I]);
                }
	}

	lv_long_Fphone = pv_phone.getNthBracketedField(1, "F/", "/") ;
	if ( lv_long_Fphone.NotIsNull() )
        {
	  editPhoneNumber(lv_long_Fphone, md_IA_fax_acode[mi_IA_I], md_IA_fax_no[mi_IA_I]);
        }

	LOG(logDEBUG)  << "EMAIL>> pv_phone = " << pv_phone  ;

	li_pos = lv_email.findSubstring( "E/", 1 );

	if ( li_pos == 7 )
	{
		lv_email = lv_email.At(9);

		if ( lv_email.NotIsNull() )
		{
			lv_email.substitute( "--", "_" );

			LOG(logDEBUG)  << "EMAIL>> lv_email = " << lv_email  ;

			li_pos = lv_email.findSubstring( "//", 1 );

			if ( li_pos > 0 )
			{
                               // 516462, remove trailing /xxx after //
                               li_testPos = lv_email.findSubstring("/", li_pos+2);
                               if ( li_testPos > 1 ) 
                               {
                                  lv_email = lv_email.For(li_testPos - 1) ;
                               }

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

//------------------------------------------------------------------------------

void CMIR::condense_IrDocumentTax(D_passenger_no  &pd_pax_id, 
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
   
	int li_itd_i = mi_ITD_I;

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
		md_ITD_city_id[mi_ITD_I] = AB_null ;
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
		md_ITD_city_id[mi_ITD_I] = AB_null ;
      md_ITD_tax_id[mi_ITD_I++] = "XT" ;
   }

	for (int i = mi_ITD_I; i < li_itd_i; i++) {
      md_ITD_pax_id[i] = AB_null;
      md_ITD_line_number[i] = AB_null;
      md_ITD_tax_amount[i] = AB_null;
		md_ITD_city_id[i] = AB_null ;
		md_ITD_tax_id[i] = AB_null ;
	}

   EXIT; 
}

void
CMIR::fill_IrTASF(CPNR &po_PNR) 
{
	LOG(logDEBUG) << "START CMIR::parseTASF" ;

	AB_Varchar lm_data(60);
	int 		  li_tktCounter;
	int		  li_pos;
	string     sup_id, form, ticket;

	li_tktCounter = mi_ID_I + 1;

	po_PNR.resetSegment("A02");
	po_PNR.resetSegment("A14");
	//po_PNR.resetSegment("A16");
	AB_Text lt_segmentA02 = po_PNR.getNextSegment("A02");
	AB_Text lt_segmentA14 = po_PNR.getNextSegment("A14");
	//AB_Text lt_segmentA16;
	AB_Text lt_segmentA14MS;

   AB_Boolean  lb_A14MS_found  = AB_false;

	if (lt_segmentA02.NotIsNull()) 
	{
		/* 
		 * follow the rule in fill_IrPasDocument():
		 * tickets can only be created if there are passangers
		 */
		while (lt_segmentA14.NotIsNull()) 
		{
			/*
			 * TASF (Travel Agent Service Fee)
			 * 	for each TASF, we expect
			 * 		A14MS 
			 * 		A14MSX
			 * 		A166.........T (or O)
			 */

			if (!lb_A14MS_found && lt_segmentA14.For(6) == "A14MS/") 
			{
			  /* SCR 516802 We will create A16 on the fly...
				lt_segmentA16 = po_PNR.getNextSegment("A16");

				while (lt_segmentA16.NotIsNull()) 
				{
					if (lt_segmentA16.At(4).For(1) 	== "6" &&
					 	(lt_segmentA16.At(14).For(1) 	== "T" ||
					  	 lt_segmentA16.At(14).For(1) 	== "O")) 
					{
						break;
					}

					lt_segmentA16 = po_PNR.getNextSegment("A16");
				}

				if ( lt_segmentA16.IsNull() )
					return;
			  */

				lt_segmentA14MS 	= lt_segmentA14;

				LOG(logDEBUG) << "A14MS == [" << lt_segmentA14MS.to_string() << "]";
				if ( PCRE::pcre_match("/TK(...?)(....)(......)[/!]", lt_segmentA14MS.to_string()))
				{
					PCRE::pcre_get_group(1, sup_id);
					PCRE::pcre_get_group(2, form);
					PCRE::pcre_get_group(3, ticket);

					LOG(logDEBUG) << "A14MS== form   [" << form.data() << "]";
					LOG(logDEBUG) << "A14MS== sup_id [" << sup_id.data() << "]";
					LOG(logDEBUG) << "A14MS== ticket [" << ticket.data() << "]";

					gm_sup_id                  	= sup_id.data(); //lm_data.For(3);
					md_ID_sup_id[mi_ID_I]			= gm_sup_id;
					md_ID_form[mi_ID_I]        	= form.data(); //lm_data.At(4).For(4);
					md_ID_ticket_no[mi_ID_I]   	= ticket.data(); //lm_data.At(8).For(6);
					md_ID_doc_type[mi_ID_I]    	= "BSP";
					md_ID_line_number[mi_ID_I] 	= li_tktCounter++;
					md_ID_pax_id[mi_ID_I]      	= md_IP_pax_id[0];

					// SCR 210495 START

					//md_ID_cur_id[mi_ID_I]      	= "CAD";

					md_ID_cur_id[mi_ID_I]      	= AB_null;

					// SCR 210495 END

					md_ID_reference[mi_ID_I]   	= md_IRT_reference ;
					md_ID_sel_access[mi_ID_I]  	= md_selAccess ;
					md_ID_personal_crs[mi_ID_I] 	= md_personalCrs ;
					md_ID_crs_tour[mi_ID_I]     	= md_crsTour ;
					md_ID_e_ticket[mi_ID_I]     	= "N";
					md_ID_stp_home_city[mi_ID_I] 	= md_stpHomeCity ;
					md_ID_stp_host_city[mi_ID_I] 	= md_stpHostCity ;
					md_ID_saving_code[mi_ID_I] 	= md_savingCode ;
					md_ID_full_fare[mi_ID_I] 		= md_full_fare ;
					md_ID_low_prac_fare[mi_ID_I] 	= md_low_prac_fare ;
					md_ID_rebate[mi_ID_I] 			= md_rebate;
					md_ID_pic[mi_ID_I] 				= " ";
					md_ID_no_of_books[mi_ID_I] 	= "01";
					lb_A14MS_found 					= AB_true;
				}

				lm_data = lt_segmentA14MS.getNthBracketedField(1, "/S", "/");

				if (lm_data.IsNull()) 
				{
					lm_data = lt_segmentA14MS.getNthBracketedField(1, "/S", "!");
				}

				if (lm_data.NotIsNull()) 
				{
					md_ID_base_cost[mi_ID_I] = lm_data;
				}

				lm_data = lt_segmentA14MS.getNthBracketedField(1, "/C", "!");

				if (lm_data.NotIsNull()) 
				{
					md_ID_commission[mi_ID_I] = lm_data;
				}

			} 
			else if (lb_A14MS_found && lt_segmentA14.For(7) == "A14MSX/") 
			{

				// get taxes from MS remark only if we have all MS, MSX and A16 in place

				lm_data = lt_segmentA14MS.getNthBracketedField(1, "/TX", "/");

				if (lm_data.IsNull()) 
				{
					lm_data = lt_segmentA14MS.getNthBracketedField(1, "/TX", "!");
				}

				while (lm_data.NotIsNull() && 
						(li_pos = lm_data.findFirstCharNotInList("0123456789.")) != 0) 
				{
					D_char_numeric	ld_taxAmt = lm_data.For(li_pos-1);
					D_tax 			ld_tax    = lm_data.At(li_pos).For(2);

					fillIrDocumentTax(ld_taxAmt, ld_tax);

					lm_data = lm_data.At(li_pos+2);
				}


				lm_data = lt_segmentA14.getNthBracketedField(1, "/CCN", "/");

				if (lm_data.IsNull()) 
				{
					lm_data = lt_segmentA14.getNthBracketedField(1, "/CCN", "!");
				}

				int li_old_mi_IRMK_I = mi_IRMK_I;

				fill_IrOther4TASF(sup_id);

				createTASFIrpayment(lm_data);
				createTASFConsumdetail();

				/*
				 * if no Irremark is created by calling fill_IrOther()
				 * create it here
				 */

				if (li_old_mi_IRMK_I == mi_IRMK_I) 
				{
					createTASFRemark();
				}

				mi_ID_I++;
				lb_A14MS_found = AB_false;
			}

			lt_segmentA14 = po_PNR.getNextSegment("A14");
		}
	}

	LOG(logDEBUG) << "END CMIR::parseTASF" ;
}

void
CMIR::createTASFIrpayment(const AB_Varchar &pv_data) {
	LOG(logDEBUG) << "START CMIR::createTASFIrpayment" ;

	if (pv_data.NotIsNull()) {
		int	li_pos;

		/*
		 * We do not have amount and
		 * Irpayment.amount is used nowhere, so put an
		 * arbitrary value here
		 */
		md_IRP_amount[mi_IRP_I] 		= "123.45";
		md_IRP_pax_id[mi_IRP_I]       = md_ID_pax_id[mi_ID_I];
		md_IRP_line_number[mi_IRP_I]  = md_ID_line_number[mi_ID_I];
		md_IRP_form_of_pay[mi_IRP_I]  = "CC";
		md_IRP_pay_id[mi_IRP_I] 		= AB_Integer(1);

		md_IRP_cc_id[mi_IRP_I] = pv_data.For(2);
		if ((li_pos = pv_data.findSubstring("EXP")) != 0) {
			md_IRP_cc_no[mi_IRP_I]        = pv_data.At(3).For(li_pos - 3);
			md_IRP_cc_exp_date[mi_IRP_I]  = pv_data.At(li_pos + 3).For(4);
		} else {
			md_IRP_cc_no[mi_IRP_I]        = pv_data.At(3);  
		}
		mi_IRP_I++;
	}

	LOG(logDEBUG) << "END CMIR::createTASFIrpayment" ;
}

void
CMIR::createTASFConsumdetail() {
	LOG(logDEBUG) << "START CMIR::createTASFConsumdetail" ;

	md_ICD_itin_type[mi_ICD_I]          = md_IO_itin_type[mi_IO_I - 1];
   md_ICD_itin_line_number[mi_ICD_I]   = md_IO_line_number[mi_IO_I - 1];
   md_ICD_pax_id[mi_ICD_I]             = md_ID_pax_id[mi_ID_I];
   md_ICD_line_number[mi_ICD_I++]      = md_ID_line_number[mi_ID_I] ;

	LOG(logDEBUG) << "END CMIR::createTASFConsumdetail" ;
}

void
CMIR::createTASFRemark() {
	LOG(logDEBUG) << "START CMIR::createTASFRemark" ;

   md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[mi_IO_I - 1];
   md_IRMK_oth_line_number[mi_IRMK_I]	= md_IO_line_number[mi_IO_I - 1] ;
	md_IRMK_remark[mi_IRMK_I++]         = "MANAGEMENT FEE";

	LOG(logDEBUG) << "END CMIR::createTASFRemark" ;
}

void CMIR::fill_IrOther4TASF(string& ps_sup_id)
{
   ENTER;

   md_IO_sup_id[mi_IO_I] = ps_sup_id.data() ;

	md_IO_line_number[mi_IO_I]    = mi_XDLine++ ;

	md_IO_itin_type[mi_IO_I] = "O";
	md_IO_unit_type[mi_IO_I] = "TAF";

   md_IO_start_date[mi_IO_I] = md_creation_date.to_stringFormat("DDMMMYY") ;

   md_IO_orig_city_id[mi_IO_I] = "ZZZ";
   md_IO_dest_city_id[mi_IO_I] = "ZZZ";


	md_IO_end_date[mi_IO_I] = md_creation_date.to_stringFormat("DDMMMYY") ;

   md_IO_reference[mi_IO_I] = md_IRT_booking_no;

   md_IO_unit_ct[mi_IO_I] = 1 ;

   mi_IO_I++ ;

   EXIT; 
}

//------------------------------------------------------------------------------

void CMIR::processSFC()
{
	ENTER;

	AB_Char lc_svcDescription(20) ;

	lc_svcDescription = " ";

	/*
	mb_SfFound   = AB_true;
   mb_FeeFound  = AB_true;
   mv_svcFEE = AB_Text("FEE-MR2/FP:CCAX370000000000028/D1200");
   mv_svcSF  = AB_Text("123.45/CC");

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

	if ( mb_SfFound && ! mb_FeeFound )
	{
		 // SF- without FEE- 

		 mi_rejectirId = 3611;
	}
	else if ( ! mb_SfFound && mb_FeeFound )
	{
		 // FEE- without SF- 

		 mi_rejectirId = 3612;
   }
	else if ( mb_SfFound && mb_FeeFound )
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
       AB_Integer li_irregularId;

		 //lo_MirSvcFee.print();

		 lo_MirSvcFee.getDetail(lc_amount,
										lc_fop,
										lc_supId,
										lc_cc,
										lc_expDate,
										li_irregularId);

		 if ( li_irregularId == AB_Integer(0) ) 
		 {
				// SCR 303096 START

				if ( md_form_of_pay == "CC" )
				{
					AB_Varchar lv_temp(20);

					lv_temp = lc_cc.For(2);

					if ( lv_temp.IsNull() ) 
					{
						CMsgHandler::setMsg(102, " ", "FEE-PARSE", 1, AB_Varchar(20,AB_null));
						return;
					}

					lv_temp = lc_cc.At(3);
					lv_temp.removeLeadAndTrailBlanks();

					if ( lv_temp.IsNull() )
					{
						CMsgHandler::setMsg(102, " ", "FEE-PARSE", 2, AB_Varchar(20,AB_null));
						return;
					}

					lv_temp = lc_expDate;
					lv_temp.removeLeadAndTrailBlanks();

					if ( lv_temp.NotIsNull() && ( ! lv_temp.isAllNumeric() ) )
					{
						CMsgHandler::setMsg(102, " ", "FEE-PARSE", 3, AB_Varchar(20,AB_null));
						return;
					}
				}

				// SCR 303096 END

				// SCR 412640 Start
				bool lb_cc_valid = true;
				if (lc_fop == "CC")  {
					AB_Varchar lv_cc_id(40);
					long       ll_error_code;
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

			  if (lc_expDate.IsNull())
			  {
					lc_expDate = "    ";
			  }

		     md_IRMK_remark[mi_IRMK_I] = (lb_cc_valid? "" : "BADCC/") +
			  										AB_Text("APP") + /* CRS             */
                                       lc_fop         + /* form of payment */
                                       lc_cc          +
													lc_expDate     +
                                       AB_Text("  ")  + /* PER ALL pax_id  */
												 	lc_svcDescription +
													lc_amount      + /* amount          */
													lc_accountLine;

		     md_IRMK_pax_id[mi_IRMK_I++] = AB_null ;
		 }
		 else
		 {
           mi_rejectirId = li_irregularId;
		 }
	}
   EXIT; 
}

//------------------------------------------------------------------------------

void CMIR::saveSvcIrregularity()
{
   D_description   ld_errorDescription ;
	AB_Integer      li_msgId ;

   md_queType = "SFC" ;

	li_msgId = mi_rejectirId;


   //CMsgHandler::getMsgDescWithSub(li_msgId, "E", 
   //                              mv_rejectirSub,
   //                             ld_errorDescription) ;

   CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;

   createQueue(ld_errorDescription) ;
}


void CMIR::getA09XFtaxes(CPNR &po_PNR, AB_Varchar &pv_XFTaxes)
{
	AB_Text	lt_total_segment ;
	int		li_XFIndex ;

	po_PNR.resetSegment("A09") ;
	lt_total_segment = po_PNR.getNextSegment("A09")  ;

	while (lt_total_segment.NotIsNull())
	{

		li_XFIndex = lt_total_segment.findSubstring("XF") ;

		if ( li_XFIndex > 0 )
		{
			if (
				lt_total_segment.At(li_XFIndex+2).For(3).isAllAlpha() &&
				lt_total_segment.At(li_XFIndex+5).For(1).isAllNumeric() )
			{

				pv_XFTaxes = lt_total_segment.At(li_XFIndex+2) ;

				li_XFIndex = pv_XFTaxes.findFirstCharNotInSet("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.") ;

				if ( li_XFIndex > 0 )
				{
					pv_XFTaxes = pv_XFTaxes.For(li_XFIndex-1) ;
				}
				lt_total_segment = AB_null ; // Only one XF valid section in A09
														// exit the wile loop
			}
			else /* an XF appears as random string in A09 ; contiue the search */
				lt_total_segment = lt_total_segment.At(2);
		}
		else /* No XF string in A09 */
		{
			pv_XFTaxes = AB_null;
			lt_total_segment = AB_null ; // exit the wile loop
		}
	}
}

void CMIR::createMCOItin()
{
   ENTER;

	if ( mi_MCO_itin_line_number >=  0 )
		return;

   AB_Date     ld_date ;

   md_IO_sup_id[mi_IO_I] = "XD" ;

   md_IO_itin_type[mi_IO_I] = "O" ;

   ld_date.getGMTNow() ;

   md_IO_start_date[mi_IO_I] = ld_date.to_stringFormat("DDMMMYY") ;
   md_IO_orig_city_id[mi_IO_I] = "ZZZ" ;
   md_IO_dest_city_id[mi_IO_I] = "ZZZ" ;
   md_IO_unit_ct[mi_IO_I] = 1 ;
   md_IO_unit_type[mi_IO_I] = "TAF" ;
   md_IO_line_number[mi_IO_I] = mi_XDLine++ ;
   md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;
   md_IO_reference[mi_IO_I] = md_IRT_reference ;

   md_IRMK_remark[mi_IRMK_I]           = "TRAVEL AGENCY SERVICE FEE" ;
   md_IRMK_itin_type[mi_IRMK_I]        = "O" ;
   md_IRMK_oth_line_number[mi_IRMK_I++]  = md_IO_line_number[mi_IO_I] ;

	mi_MCO_itin_line_number = mi_IO_I;

   mi_IO_I++ ;

   EXIT;
}

void CMIR::createMCOConsumDetail()
{
   ENTER;

	createMCOItin();

	if ( mi_MCO_itin_line_number < 0 )
		return;

   md_ICD_itin_type[mi_ICD_I]          = "O" ;
   md_ICD_itin_line_number[mi_ICD_I]   = md_IO_line_number[mi_MCO_itin_line_number] ;
   md_ICD_pax_id[mi_ICD_I]             = md_IP_pax_id[mi_IP_I];
   md_ICD_line_number[mi_ICD_I++]      = md_ID_line_number[mi_ID_I] ;

   EXIT;
}

void CMIR::createSFNRemark(AB_Boolean &pb_FR_found)
{
   md_IRMK_type[mi_IRMK_I]    = "SFN" ;
   md_IRMK_remark[mi_IRMK_I]  = "N" ;
   md_IRMK_pax_id[mi_IRMK_I]  = AB_null;
   mi_IRMK_I++;

   pb_FR_found = AB_true;
}

void CMIR::RP_parseRemarks(CPNR &po_PNR) 
{
   ENTER;

   AB_Text  lt_segment ;
   po_PNR.resetSegment("A14") ;
   lt_segment = po_PNR.getNextSegment("A14") ;
   while (lt_segment.NotIsNull()) 
	{

      // SCR 517902: Virtuoso Remark Parser for VFT/VFC
      AB_Text lt_remark_text = lt_segment.At(6);
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


		pRemarkParser->parse(lt_segment.At(6).to_string());

      lt_segment = po_PNR.getNextSegment("A14") ;
   }
   EXIT;
}

void CMIR::fillIrLimo(AB_Text &pt_segment)
{
   ENTER;
	// SCR: 518868 don't create limo when unit_cnt = 0
	if (pt_segment.At(63).For(1) == "O") {
			//LOG(logDEBUG)  << "DEBUG: don't create LIMO segment when unit_cnt is 0";
			EXIT;
			return;
			}
	

   AB_Text        lt_segment ;
   AB_Varchar     lv_remark(50) ;
   AB_Varchar     lv_field(50) ;
   AB_Char        lc_year(2) ;
   AB_BOOL        lb_ok ;
   AB_Money       lm_amount ;
   int            li_pos ;
	string         labelValue;

   md_IR_itin_type[mi_IR_I]      = "C" ;
   md_IR_free_mileage[mi_IR_I]   = AB_null ;

   md_IR_line_number[mi_IR_I]    = pt_segment.At(5).For(2) ;
   md_IR_start_date[mi_IR_I]     = pt_segment.At(7).For(7) ;
   md_IR_end_date[mi_IR_I]       = md_IR_start_date[mi_IR_I] ;
   md_IR_origin_city_id[mi_IR_I] = pt_segment.At(49).For(3) ;
   md_IR_dest_city_id[mi_IR_I]   = md_IR_origin_city_id[mi_IR_I] ;

   md_IR_service_status[mi_IR_I] = pt_segment.At(53).For(2) ;
	//------ STS LABEL(G9150) START------------------
	if ( pRemarkParser->getData("LABELS", "STS", labelValue))
	{
	   mv_service_status             = labelValue.data() ;
		md_IR_service_status[mi_IR_I] = labelValue.data() ;
	}
	//------ STS LABEL(G9150) END------------------


   md_IR_unit_ct[mi_IR_I]        = pt_segment.At(63).For(1) ;
	md_IR_unit_type[mi_IR_I]		= "LIMO" ;
	md_IR_sup_id[mi_IR_I]			= "LBC" ;

   li_pos = pt_segment.findSubstring("OD-") ;
   if ( li_pos > 0 )
   {
      lt_segment = pt_segment.At( li_pos ) ;
   }

   li_pos = lt_segment.findSubstring("!") ;
   if ( li_pos > 0 )
   {
      lt_segment = lt_segment.For( li_pos - 1 ) ;
   }
   lt_segment = lt_segment + "/" ;

   lv_field = lt_segment.getNthBracketedField(1, "/RATE-", "/") ;
   if ( lv_field.NotIsNull() )
   {
     	li_pos = lv_field.findSubstring("$") ; 
		if ( li_pos > 0)
			lv_field = lv_field.At( li_pos + 1 ) ;

		li_pos = lv_field.findFirstCharNotInList(".0123456789");
		if (li_pos > 0)
		{
			md_IR_cur_id[mi_IR_I] = lv_field.At( li_pos ) ;
			lv_field 	 			 = lv_field.For( li_pos-1 ) ;
		}

		ParseMoney(lv_field, md_IR_local_rate[mi_IR_I]);
      md_IR_booked_rate[mi_IR_I] = md_IR_local_rate[mi_IR_I] ;
   }

   lv_field = lt_segment.getNthBracketedField(1, "/CONF-", "/") ;
   if ( lv_field.NotIsNull() )
   {
      md_IR_confirmation[mi_IR_I] = lv_field ;
   }

   lv_field = lt_segment.getNthBracketedField(1, "/CANCEL-", "/") ;
   if ( lv_field.NotIsNull() )
   {
      lv_remark = "/CANCEL-" + lv_field ;
      IrLimoRemark(lv_remark) ;
   }

   if ( md_IR_service_status[mi_IR_I] == "HK" )
   {
      md_IR_sup_id[mi_IR_I] = pt_segment.At(15).For(2) + "C" ;

      lv_field = lt_segment.getNthBracketedField(1, "/PICKUP-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         lv_remark = lv_field ;
         li_pos = lv_field.findSubstring(" AT ") ;
         if ( li_pos > 0 )
         {
            lv_remark = "/PICKUP-" + lv_field.For(li_pos - 1) ;
            lv_field  = lv_field.At(li_pos + 4) ;
            md_IR_start_time[mi_IR_I] = CheckTime(lv_field);

         }
			else
			{
				lv_remark = "/PICKUP-" + lv_remark ;
			}
         IrLimoRemark(lv_remark) ;
      }

      lv_field = lt_segment.getNthBracketedField(1, "/DROPOFF-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         lv_remark = lv_field ;
         li_pos = lv_field.findSubstring(" AT ") ;
         if ( li_pos > 0 )
         {
            lv_remark = "/DROPOFF-" + lv_field.For(li_pos - 1) ;
            lv_field  = lv_field.At(li_pos + 4) ;
            md_IR_end_time[mi_IR_I] = CheckTime(lv_field);

         }
			else
			{
				lv_remark = "/DROPOFF-" + lv_remark ;
			}

         IrLimoRemark(lv_remark) ;
      }

   }
   else
   {
      lv_field = lt_segment.getNthBracketedField(1, "/VENDOR-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         md_IR_site_name[mi_IR_I] = lv_field ;
         lv_remark                = "/VENDOR-" + lv_field ;
         IrLimoRemark(lv_remark) ;
      }

      lv_field = lt_segment.getNthBracketedField(1, "/PHONE-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         editPhoneNumber(lv_field, md_IR_phone_acode[mi_IR_I], md_IR_phone_no[mi_IR_I], true);
      }

      lv_field = lt_segment.getNthBracketedField(1, "/PICKUP-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         lv_remark = "/PICKUP-" + lv_field ;
         IrLimoRemark(lv_remark) ;
      }

      lv_field = lt_segment.getNthBracketedField(1, "/PICKUP TIME-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         md_IR_start_time[mi_IR_I] = ParseTime(lv_field) ;
      }

      lv_field = lt_segment.getNthBracketedField(1, "/DROPOFF-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         lv_remark = "/DROPOFF-" + lv_field ;
         IrLimoRemark(lv_remark) ;
      }

      lv_field = lt_segment.getNthBracketedField(1, "/DROPOFF TIME-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         md_IR_end_time[mi_IR_I] = ParseTime(lv_field) ;
      }

      lv_field = lt_segment.getNthBracketedField(1, "/SUC-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         md_IR_sup_id[mi_IR_I] = lv_field.For(2) + "C";
      }

      lv_field = lt_segment.getNthBracketedField(1, "/CM-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         md_IR_commissionable[mi_IR_I] = lv_field.For(1).Upper() ;
      }

      lv_field = lt_segment.getNthBracketedField(1, "/CP-", "/") ;
      if ( lv_field.NotIsNull() )
      {
         ParseMoney( lv_field, md_IR_com_rate[mi_IR_I] ) ;
      }
   } // service_code is not HK

   lv_field = pt_segment.getNthBracketedField(1, "CF:", "!") ;
   if (lv_field.Length().to_long() > 2)
      md_IR_confirmation[mi_IR_I] = lv_field.At(3) ;

   //Set Limo indicator in Remark
   md_IRMK_type[mi_IRMK_I]             = "LMI"; //LIMO inditictor
   md_IRMK_itin_type[mi_IRMK_I]        = "C" ;
   md_IRMK_ren_line_number[mi_IRMK_I]  = md_IR_line_number[mi_IR_I] ;
   md_IRMK_remark[mi_IRMK_I]           = "Y";
   md_IRMK_pax_id[mi_IRMK_I++]         = AB_null;
/*
   AB_Varchar lv_rateCode(60);
   AB_Varchar lv_rateType(60);

   getA16ARateData(pt_segment,
          md_IR_itin_type[mi_IR_I],
          md_IR_start_date[mi_IR_I],
          md_IR_saving_code[mi_IR_I],
          md_IR_cur_id[mi_IR_I],
          md_IR_local_rate[mi_IR_I],
          md_IR_regular_rate[mi_IR_I],
          md_IR_booked_rate[mi_IR_I],
          md_IR_commissionable[mi_IR_I],
          md_IR_preferred[mi_IR_I],
          mv_IrCarData, mi_IrCarIndex,
          lv_rateCode,
          lv_rateType, 
			 md_IR_com_rate[mi_IR_I]) ;

   if ( lv_rateType.NotIsNull() )
       md_IR_rate_type[mi_IR_I] = lv_rateType;

   if ( lv_rateCode.NotIsNull() )
       md_IR_rate_code[mi_IR_I] = lv_rateCode;
*/
   AB_Varchar  lv_duePaid(3) ;
   AB_Char     lc_status(2) ;

   lv_duePaid = AB_null ;
   lc_status  = md_IR_service_status[mi_IR_I] ;
   fillIrconsumDetailRental(md_IR_itin_type[mi_IR_I],
                           md_IR_line_number[mi_IR_I],
                           lc_status,
                           lv_duePaid) ;


   mi_IR_I++;

   EXIT;
}

void CMIR::IrLimoRemark( AB_Varchar &pv_field )
{
   ENTER;

   md_IRMK_type[mi_IRMK_I]             = "PNC" ;
   md_IRMK_itin_type[mi_IRMK_I]        = "C" ;
   md_IRMK_ren_line_number[mi_IRMK_I]  = md_IR_line_number[mi_IR_I] ;
   md_IRMK_remark[mi_IRMK_I]           = pv_field ;
   md_IRMK_pax_id[mi_IRMK_I++]         = AB_null ;

   EXIT;
}

void CMIR::processNonArcDocFromACCremark()
{
   ENTER;
	// LOOP ON ALL 
	int i = 0;
	string remarkName;
	for( pRemarkParser->it_remarks_begin(); !pRemarkParser->it_remarks_end(); pRemarkParser->it_next_remark() )
	{


		remarkName = pRemarkParser->it_remark().getRemarkName();
		LOG(logINFO) << remarkName.data();
		if ( PCRE::pcre_match("ACC_TEMPLATE", remarkName ))
		{

        // SCR: 518873
         int multi;
			multi = 1;
         string mul;
         pRemarkParser->it_remark().getValueForParameter("MUL", mul);
         if (mul == "PER") {
               multi = mi_IP_I; // the # of passenger
               }
			// generating old MCC, MAR or MCA remark
			//     1   2   3    4      5    6
			//A14G*MCC-FPC/0.00/0.00  /0.00/TK-7263465015
			//A14G*MCC-ACJ/0.00/0.00RC/P0  /TK-2104300512
			//A14G*MAR-HC9/0.00/0.00  /0.00/TK-7890101617
			string newRemark;

			// 1. remark name, derived from FOP 
			newRemark = "M";
			string fop = "CA";
			pRemarkParser->it_remark().getValueForParameter("FP", fop);
			if ( fop == "CK" ) newRemark += "AR";
			else  newRemark += fop;

			newRemark += "-";

			//2. sup_id
			string sup_id;
			pRemarkParser->it_remark().getValueForParameter("SUP", sup_id);
			newRemark += sup_id;
			newRemark += "/";

			//3.fare
			string fare = "0.00";
			pRemarkParser->it_remark().getValueForParameterGroup("FAR", "AMOUNT", fare);
			
			//4.taxes , in CWT US only TX
			string tax = "0.00";;
			pRemarkParser->it_remark().getValueForParameterGroup("TAX", "AMOUNT", tax);

			// add taxes to fare
			D_amount fare_plus_taxes = D_amount(fare.data());
			fare_plus_taxes = fare_plus_taxes + D_amount(tax.data());

			 // SCR 518873
         D_amount total(0);
         for (int li = 0; li < multi; li++) {
            total = total + fare_plus_taxes;
         }
         fare_plus_taxes = total;
         //LOG(logDEBUG)  << "DEBUG fair plus taxes " << total;


			// finish 3.
			newRemark += fare_plus_taxes.to_string();
			newRemark += "/";

			//4.taxes , in CWT US only TX
			//string tax = "0.00";;
			//pRemarkParser->it_remark().getValueForParameterGroup("TAX", "AMOUNT", tax);
			//newRemark += tax;

        	D_amount tax_amount =  D_amount(tax.data());
        	D_amount total_tax(0);
         	for (int li = 0; li < multi; li++) {
            	total_tax = total_tax + tax_amount;
            	}
        	newRemark += total_tax.to_string();

			newRemark += "/";

			//5.commission
			string commission = "0.00";;
			pRemarkParser->it_remark().getValueForParameterGroup("COM", "AMOUNT_OR_PERCENT", commission);
        // SCR: 518873
        
			if (commission.at(0) == 'P') {   // percent 
				newRemark += commission;
				}
			else {
         	D_amount com_amount =  D_amount(commission.data());
         	D_amount total_com(0);
         	for (int li = 0; li < multi; li++) {
            	total_com = total_com + com_amount;
            	}
         	newRemark += total_com.to_string();
       	} 
			newRemark += "/";


			//6.ticket no, optional 
			string ticketNo;
			if (pRemarkParser->it_remark().getValueForParameterGroup("TKT", "TKT_NO", ticketNo) )
		   {
				newRemark += "TK-";
				newRemark += ticketNo;
				newRemark += "/";
			}

			if ((mul == "ALL") || (mul == "PER")) {
					newRemark += "MUL-";
					newRemark += mul;
					newRemark += "/";
					}
			LOG(logINFO) << "GENERATED REMARK [" << newRemark.data() << "]";

			// ADD NEW REMARK TO POOL
			md_nonArcDoc[mi_nonArcDoc++] = newRemark.data() ;
		}

	}

   EXIT;
}


void
CMIR::processEMD(CPNR &po_PNR)
{
	LOG(logDEBUG)  << "MIR::processEMD starts......." ;

   AB_Text		lt_section29,  lt_section30;
	long long	currentTicketNo, startTicketNo, endTicketNo;
	AB_Char		lc_year(2);
	int			firstAirTransportInd = 0;
	bool			airFound = false;

	for ( int ind = 0; ind < mi_IT_I && !airFound; ind++)
	{
		if ( md_IT_itin_type[ind] == "A" )
		{
			airFound = true;
			firstAirTransportInd = ind;
		}
	}

   po_PNR.resetSegment("A29") ;
   lt_section29 = po_PNR.getNextSegment("A29") ;

   while (lt_section29.NotIsNull())
   {

		LOG(logDEBUG) << "A29=[" << lt_section29 << "]" ;


		AB_Varchar lv_base_cost(20);
		AB_Varchar lv_cur_id(3);
		AB_Varchar lv_sup_id(3);
		AB_Varchar lv_ticket_no_start(10);
		AB_Varchar lv_ticket_no_end(10);
		AB_Varchar lv_A29_fop(2);
		AB_Varchar lv_base_amount(12);
		AB_Varchar lv_base_amount_cur_id(3);
		AB_Varchar lv_A30_date(5);

		//============== A29 =========================
		// A29VCR
		lv_sup_id = lt_section29.At(96).For(2);
		LOG(logDEBUG) << "A29.A29VCR.sup_id[" << lv_sup_id << "]" ;

		// A29EMD
		lv_ticket_no_start = lt_section29.At(73).For(10);
		LOG(logDEBUG) << "A29.A29EMD.ticket_no_start [" << lv_ticket_no_start << "]" ;

		// A29CNJ
		lv_ticket_no_end = lt_section29.At(83).For(10);
		LOG(logDEBUG) << "A29.A29CNJ.ticket_no_end[" << lv_ticket_no_end << "]" ;
		if ( lv_ticket_no_end == "          " )lv_ticket_no_end = lv_ticket_no_start;

		// A29TYP
		lv_A29_fop = lt_section29.At(171).For(2);
		LOG(logDEBUG) << "A29.A29TYP.fop[" << lv_A29_fop << "]" ;

		// A29CBA A29BAM or A29CEA A29EAM
		regularOrEquivalentCurAndAmount(	lt_section29, 98, 101, 113, 116,
													lv_base_amount_cur_id, lv_base_amount);

		// A30  loop start
		set <Text2Decimal> TaxId2Amount;
		bool foundDate = false;
		po_PNR.resetSegment("A30") ;
		lt_section30 = po_PNR.getNextSegment("A30") ;

		while (lt_section30.NotIsNull())
		{
			LOG(logDEBUG) << "A30=[" << lt_section30 << "]" ;

			//============== A30 ==============================
			AB_Varchar lv_current_ticket_no(10);
			AB_Varchar lv_tax1_amount(8);
			AB_Varchar lv_tax1_id(2);
			AB_Varchar lv_tax2_amount(8);
			AB_Varchar lv_tax2_id(2);
			AB_Varchar lv_tax3_amount(8);
			AB_Varchar lv_tax3_id(2);

			lv_current_ticket_no = lt_section30.At(12).For(10);
			LOG(logDEBUG) << "A30.A30EMD.current_ticket_no [" << lv_current_ticket_no << "]" ;

			sscanf(lv_current_ticket_no.to_string(), "%lld", &currentTicketNo);
			sscanf(lv_ticket_no_start.to_string(),   "%lld", &startTicketNo);
			sscanf(lv_ticket_no_end.to_string(),     "%lld", &endTicketNo);
			
			if(   currentTicketNo >= startTicketNo && currentTicketNo <= endTicketNo)
			{
				if ( !foundDate)
				{
					lv_A30_date = lt_section30.At(27).For(5);
					LOG(logDEBUG) << "A30.A30DAT.date [" << lv_A30_date << "]" ;
					if ( lv_A30_date != "     " )
					{
						foundDate = true;
					}

				}

				lv_tax1_amount = lt_section30.At(119).For(8);
				LOG(logDEBUG) << "A30.A30DTX1.tax1 [" << lv_tax1_amount << "]" ;
				lv_tax1_id = lt_section30.At(127).For(2);
				LOG(logDEBUG) << "A30.A30DTC1.tax1-id [" << lv_tax1_id << "]" ;

				if ( lv_tax1_id != "  " )
				{
				  Text2Decimal::add(TaxId2Amount, lv_tax1_id, AB_Decimal(lv_tax1_amount.to_string()));
				}

				lv_tax2_amount = lt_section30.At(129).For(8);
				LOG(logDEBUG) << "A30.A30DTX2.tax2 [" << lv_tax2_amount << "]" ;
				lv_tax2_id = lt_section30.At(137).For(2);
				LOG(logDEBUG) << "A30.A30DTC2.tax2_id [" << lv_tax2_id << "]" ;

				if ( lv_tax2_id != "  " )
				{
				  Text2Decimal::add(TaxId2Amount, lv_tax2_id, AB_Decimal(lv_tax2_amount.to_string()));
				}

				lv_tax3_amount = lt_section30.At(139).For(8);
				LOG(logDEBUG) << "A30.A30DTX3.tax3 [" << lv_tax3_amount << "]" ;
				lv_tax3_id = lt_section30.At(147).For(2);
				LOG(logDEBUG) << "A30.A30DTC3.tax3_id [" << lv_tax3_id << "]" ;

				if ( lv_tax3_id != "  " )
				{
				  Text2Decimal::add(TaxId2Amount, lv_tax3_id, AB_Decimal(lv_tax3_amount.to_string()));
				}
			} // end of applicable A30

			  LOG(logDEBUG) << Text2Decimal::toString("A30 TAXES", TaxId2Amount).data();
			lt_section30 = po_PNR.getNextSegment("A30") ;
		} // end of A30 loop





		lv_cur_id = lv_base_amount_cur_id;
		lv_base_cost = lv_base_amount;

		lv_base_cost.removeLeadAndTrailBlanks();

		// create ticket
		md_ID_pax_id[mi_ID_I] 		= md_IP_pax_id[0];
		md_ID_line_number[mi_ID_I] = mi_ID_I + 1;
		md_ID_reference[mi_ID_I]	= md_IRT_reference;
		md_ID_sup_id[mi_ID_I]		= lv_sup_id;  
		md_ID_base_cost[mi_ID_I]	= lv_base_cost; 
		md_ID_cur_id[mi_ID_I]		= lv_cur_id; 
		md_ID_form[mi_ID_I]			= lv_ticket_no_start.For(4); 
		md_ID_ticket_no[mi_ID_I]	= lv_ticket_no_start.At(5).For(6); 
		md_ID_no_of_books[mi_ID_I]	= AB_Integer((int)(endTicketNo - startTicketNo) + 1);
		md_ID_doc_type[mi_ID_I]		= "BSP";


		AB_Date ld_tempDate ;
		ld_tempDate = md_IRT_pnr_date;
		lc_year = ld_tempDate.get_default_year(lv_A30_date) ;
		lc_year.rightJustify('0') ;

		// create Irother
		AB_Date     ld_date;
		ld_date.getGMTNow();

		md_IO_sup_id[mi_IO_I] = lv_sup_id; //"XB";
		md_IO_itin_type[mi_IO_I] = "O";
		if ( airFound )
		{
			md_IO_start_date[mi_IO_I] = md_IT_start_date[firstAirTransportInd];
			md_IO_orig_city_id[mi_IO_I] = md_IT_orig_city_id[firstAirTransportInd];
			md_IO_dest_city_id[mi_IO_I] = md_IT_orig_city_id[firstAirTransportInd];
		}
		else
		{
			md_IO_start_date[mi_IO_I] = lv_A30_date + lc_year; //ld_date.to_stringFormat("DDMMMYY");
			md_IO_orig_city_id[mi_IO_I] = "ZZZ";
			md_IO_dest_city_id[mi_IO_I] = "ZZZ";
		}
		md_IO_unit_ct[mi_IO_I] = 1;
		md_IO_unit_type[mi_IO_I] = "EMD" ;

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


		// create Irconsumdetail
		md_ICD_itin_type[mi_ICD_I]          = md_IO_itin_type[mi_IO_I];
		md_ICD_itin_line_number[mi_ICD_I]	= md_IO_line_number[mi_IO_I];
		md_ICD_pax_id[mi_ICD_I]             = md_IP_pax_id[0];
		md_ICD_line_number[mi_ICD_I]      	= md_ID_line_number[mi_ID_I];


		// create Irpayment 
		md_IRP_amount[mi_IRP_I]       =  md_ID_base_cost[mi_ID_I]; 
		md_IRP_pax_id[mi_IRP_I]       = md_IP_pax_id[0];
		md_IRP_line_number[mi_IRP_I]  = md_ID_line_number[mi_ID_I];
		md_IRP_pay_id[mi_IRP_I]       = AB_Integer(1);
		md_IRP_form_of_pay[mi_IRP_I]  = "CA"; //lv_fop; //"CC";

		AB_Varchar lv_fop(20);

		if (lv_A29_fop.For(2) == "CC") {
			lv_fop = "CC";
      	md_IRP_cc_exp_date[mi_IRP_I]  = md_cc_exp_date ;
      	md_IRP_auth_no[mi_IRP_I]      = md_auth_no ;
      	md_IRP_auth_source[mi_IRP_I]  = md_auth_source ;

      	if (md_IRP_auth_source[mi_IRP_I] == " "   || md_IRP_auth_source[mi_IRP_I] == "S")
         	md_IRP_auth_source[mi_IRP_I] = " " ;
      	else
         	md_IRP_auth_source[mi_IRP_I] = "A" ;

      	md_IRP_cc_id[mi_IRP_I] = md_cc_id ;
      	md_IRP_cc_no[mi_IRP_I] = md_cc_no ;
		}
		else if (lv_A29_fop.For(2) == "CA" || lv_A29_fop.For(2) == "CK") {
			lv_fop = lv_A29_fop.For(2);
		}
		else { // other fops default to CASH
			lv_fop = "CA";
		}

		md_IRP_form_of_pay[mi_IRP_I]  = lv_fop; //"CC";

		// create taxes
		set <Text2Decimal>::iterator it;
		for( it = TaxId2Amount.begin(); it != TaxId2Amount.end(); it++ )
		{
			md_ITD_pax_id[mi_ITD_I]       = md_IP_pax_id[0];
			md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
			md_ITD_tax_id[mi_ITD_I]       = it->_key; //"XG"; //ld_tax_id;
			md_ITD_tax_amount[mi_ITD_I] 	= it->_value; //D_amount(123.45); //ld_taxAmount;
			mi_ITD_I++;
		}

		mi_ID_I++; // irticket
		mi_IO_I++; //orother
		mi_ICD_I++; //irconsumerdetail
		mi_IRP_I++;  //irpayment

		lt_section29 = po_PNR.getNextSegment("A29") ;
   } // end of loop for A29

}

void
CMIR::regularOrEquivalentCurAndAmount(AB_Text pt_section, int regCurInd, int regAmountInd, int eqvCurInd, int eqvAmountInd, AB_Varchar& pv_cur, AB_Varchar& pv_amount)
{
	LOG(logDEBUG)  << "regularOrEquivalentCurAndAmount starts.....";
   if ( pt_section.At(eqvCurInd).For(3) == " ")
   {
		pv_amount = pt_section.At(regAmountInd).For(12) ;
      pv_cur    = pt_section.At(regCurInd).For(3) ;
   }
   else
   {
		pv_amount = pt_section.At(eqvAmountInd).For(12) ;
      pv_cur    = pt_section.At(eqvCurInd).For(3) ;
   }
	LOG(logDEBUG)  << "cur_id =[" << pv_cur << "]";
	LOG(logDEBUG)  << "Amount =[" << pv_amount << "]";
	LOG(logDEBUG)  << "regularOrEquivalentCurAndAmount END";
}
