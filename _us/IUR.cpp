/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999, 
															 2000, 2001
                SID: 1.383
      Delta Created: 17/10/18 - 15:12:27
         Last Delta: app/src/common/InterfaceRecord/_us 10/18/17 - 15:12:27 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.IUR.cpp) 
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us}[10/18/17-15:12:27 1.383] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.IUR.cpp (MA)\n" )
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
25APR2018   US446   v8617    FG  Capture cur_id correctly for values in Personitin.fare_by_leg
11OCT2017   US500   v8617    MG  Icluding preferred value from HS Remark.
13OCT2017   US498   v8619    FG  Capture commission rate for a hotel segment from Sabre interface record
28SEP2017   US497     -      KS  Stop capturing category code only for RC
04AUG2017   US285    -       MG  Included the logic for IUR type "B"
24SEP2015   517735  v8358    WW  Capture IU2PEN as penalty from M2
09FEB2015   519581  c1694    SX  add OB Fee Tax and  for multiple cost lines
18SEP2014   486684  c1566    JL  ignore 'LIM' segments with product code 'M'
30JUL2014   517902  v7966    JL  adding remark parsing for VFT/VFC.
30APR2014   516462  c1433    SL  data standardization p3, 516462
29APR2014   517781  c1429    JL  Hotel servic status in Irconsumdetail
15APR2014   516122  c1421    WW  G9146B - CWT Value Rates
10APR2014   516742  c1413    JL  foreign hotel booking, hotel remark override.
09APR2014   486684  v7901    SL  ignore 'OTH' segments with product code 'M'
08APR2014   515207  c1412    JL  process 7/8 char IU3NRM data element.
12AMR2014   516409  v7848    WW  Parsing Limo Segment in M3
10JAN2014   513807  c1350    WW  Set Car segment status by IU3AAC
09JAN2014   516217  c1349    WW  Capture departure year for AIR M3
04NOV2013   515485  c1265    JL  restoring based on 1.317 + the fix in 1.318.
30OCT2013   515485  c1265    JL  retrofit from ver 1.315 (restoring).
28OCT2013   515485  c1262    JL  fix the EB issue in PNR level.
24OCT2013   514016  c1257    JL  updated retrofit version.
19SEP2013   514550  c1213    WW  CAT35 based on new logic
11SEP2013	514016  c1195    JL  adding "MB" for mobile booking.
25JUL2013	514549  c1158    WW  inactive CAT35
10JUL2013   514023  c1141    WW  CAT35, BSP/ARC Net Ticket Sales -- phase 1
04JUL2013   513310  c1128    WW  Allow auth_no include alpha
15APR2013	513196  c1026	  JL  Allow multiple service fees
17AUG2012	510686  v7467	  WW  Create SFM for specified pax
16MAY2012	509845  c0572	  WW  Process void ticket for CWT US M&G
30MAR2012   504805  c0560    WW  Catch auth_no/auth_source information
22MAR2012   503909  v7388    WW  Only for US Matrix, add "per person" function
                                    for service fee PNR remark
============================================================================
*/



#include "C.h"
#include "gtypes.h"
#include "IUR_D.h"
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
#include "IUR.h"
#include "BraComp.h"
#include "Log.h"
#include "pcre_api.h"

#define ENTER      LOG(logINFO) << "\n*** Enter>> " << __FUNCTION__
#define EXIT       LOG(logINFO) << "\n*** Exit << " << __FUNCTION__

//-------------------------------------------------------------------------

void CIUR::printAll()
{
   LOG(logDEBUG) << "START GIRHEADER PRINT"  ;
   print_Irtripheader() ;
   LOG(logDEBUG) << "END GIRHEADER PRINT"  ;

   LOG(logDEBUG) << "START IRADDRESS PRINT"  ;
   printIraddress() ;
   LOG(logDEBUG) << "END IRADDRESS PRINT"  ;

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

   LOG(logDEBUG) << "START IRPAYMENT PRINT"  ;
   printIrpayment() ;
   LOG(logDEBUG) << "END IRPAYMENT PRINT"  ;

   LOG(logDEBUG) << "START IRSTOPOVERCITY PRINT"  ;
   printIrstopovercity() ;
   LOG(logDEBUG) << "END STOPOVERCITY PRINT"  ;
}

CIUR::CIUR () :mv_ticketM5(255) ,
					gv_IrHotelData(255,100),
					gv_IrCarData(255,100),
					gd_TransportData(255,100),
					md_tktData(255,100),
					gd_entitleData(255,100),
					md_accountingData(255,100),
					md_NonArcItin(255,100),
					md_IgnoredItin(255,100),
					mv_itinTransport(255),
					mv_service_status(2),
					mv_paxLst(255),
      			mva_M5AccSection (255,100),
      			mva_M6FareSection (511,100),
               // GAP US089 START
               mva_MERemarkSection (255,100),
               mv_lastIOName(80),
               // GAP US089 END

					// GAP US092 START
					mv_lastTourCity(80),
					// GAP US092 END
					mv_lastShipCity(80)
{
    Log::ReportingLevel() = PARSER_LOG_LEVEL; // logDEBUG; PARSER_LOG_LEVEL;

	gi_IrHotelIndex = 1 ;
	mi_IrCarIndex = 1;

	mi_M5AccIndex = 1  ;
	mi_M6FareIndex = 1  ;

   // GAP US089 START
   mi_MERemarkIndex  = 1;
   mv_lastIOName     = AB_null;
   // GAP US089 END

	// GAP US092 START
   mv_lastTourCity   = AB_null;
	// GAP US092 END

   mv_lastShipCity   = AB_null;

	mb_MPT = AB_false ;

	mi_XDLine = 50 ; // Used to create Other Itinerary by service charge MCO ;

	mb_netFareTicket = AB_false;
}

//--------------------------------------------------------------------------

void CIUR::errorHandler(CPNR &po_PNR)
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
	   //																    AB_Boolean(AB_true)) ;
      CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;
	}

   CMsgHandler::clearMsg();

   po_PNR.createGirRawTables() ;

   createQueue(ld_errorDescription) ;
}

//---------------------------------------------------------------------------

void CIUR::createCIUR(CPNR &po_PNR)
{
	ENTER;

   CDMatrix lo_Matrix;
   AB_Integer li_sqlcode;

	/* REMARK_PARSER: parse ALL remarks */
	RP_parseRemarks(po_PNR);

   fill_IrHeader(po_PNR) ;

	if (!mb_buildCfa )
      return ;

	storeEntitleData(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		storeAccountingData(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
		fill_IrItinerary(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
   	fill_IrPerson(po_PNR) ;

   if (CMsgHandler::getMsgStatus())
   	processTicket(po_PNR) ;

	// QREX START
	if ( ! mb_refundPNR )
	{
	// QREX END

		// GAP US089 START
		if (CMsgHandler::getMsgStatus())
			createM8Remarks(po_PNR) ;
		// GAP US089 END

		if (CMsgHandler::getMsgStatus())
			processAirlineTicketingFee(po_PNR);

		if (CMsgHandler::getMsgStatus())
			processReceipt();

		if (CMsgHandler::getMsgStatus())
			updIcdConnectInd() ;

		if (CMsgHandler::getMsgStatus())
			updFareInfo() ;

		//G145 - surface Segment
		if (CMsgHandler::getMsgStatus())
			createARNK() ;

		// US044 START
		if (CMsgHandler::getMsgStatus())
			IrTransportRemarks(po_PNR);
		// US044 END

		if (CMsgHandler::getMsgStatus())
			processEmailaddress(po_PNR);

	// QREX START
	}
	// QREX END

	md_queType = "RMA" ;

   if (! CMsgHandler::getMsgStatus())
   {
		LOG(logERROR) << "EXIT 000000000000 - RMA"  ;
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
		LOG(logERROR) << "\nERRORS in createIrRows !!!!!!!!!!!!!!!!!!!!!!"  ;
	}

	printAll();

   if (! CMsgHandler::getMsgStatus())
   {
		LOG(logERROR) << "ERRORS in createIrRows or printAll !!!!!!!!!!!!!!!!!!!!!!"  ;
	}

   if (! CMsgHandler::getMsgStatus())
   {
		LOG(logERROR) << "EXIT - RMA, createIrRows error..."  ;
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
	{
		LOG(logINFO) << "DOMAIN EDIT is OK, calling createClientfile..."  ;
   	createClientfile() ;
	}

   if (! CMsgHandler::getMsgStatus())
   {
		LOG(logERROR) << "ERROR - RMI (DOMAIN error or createClientfile error...)"  ;
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

		// not an error, just to see final result in logfile
		LOG(logERROR) << "*****S U C C E S S  -  CREATING REAL DATA ROWS*****"  ;
   }
	moveReceiptQueues( mv_rec_queType );
	lo_Matrix.dbCommit(li_sqlcode);

}

//--------------------------------------------------------------------------

void CIUR::fillM5Acc(CPNR &po_PNR)
{
	AB_Text		lt_M5_section ;
	AB_Varchar	lv_tktString(255);
	int			li_pos,
					li_tax1Ind,
					li_tax2Ind,
					li_tax3Ind,
					li_OPAind;

   ENTER;
   po_PNR.resetSegment("M5") ;
   lt_M5_section = po_PNR.getNextSegment("M5");

   while (lt_M5_section.NotIsNull())
	{
		// QREX START
		V_AB_Varchar lv_arrayField(100,100) ;

		lv_tktString  = lt_M5_section.At(12) ;
		lv_arrayField.parseStringMultDel(lv_tktString,"/") ;

		li_tax1Ind = 4;
		if (scanTaxesAndOPAfields(lv_arrayField, li_tax1Ind, li_tax2Ind, li_tax3Ind, li_OPAind) == AB_true )
		{
			if ( 	lt_M5_section.At(8).For(1) 	== "R" 	&&
					lt_M5_section.At(7).For(1) 	== " " 	)
			{
			  if ( lv_arrayField[li_OPAind+4].For(1)	== "F"
				 || lv_arrayField[li_OPAind+4].For(1)	== "P")
			  {
				mb_refundPNR    = AB_true;
				LOG(logDEBUG) << "AM===QREX= THIS IS FULL/PARTUAL REFUND PNR[" << lv_arrayField[li_OPAind+4].For(1) << "]"  ;
			  }
			}
		}
		// QREX END

      mva_M5AccSection[mi_M5AccIndex++] = lt_M5_section ;

		lt_M5_section = po_PNR.getNextSegment("M5")  ;
	}
   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::fillM6FareCalc(CPNR &po_PNR)
{
	AB_Text		lt_M6_section ;

   po_PNR.resetSegment("M6") ;
   lt_M6_section = po_PNR.getNextSegment("M6");

   while (lt_M6_section.NotIsNull())
	{
      mva_M6FareSection[mi_M6FareIndex++] = lt_M6_section ;

		lt_M6_section = po_PNR.getNextSegment("M6")  ;
	}
}

//---------------------------------------------------------------------------
// GAP US089 START
void CIUR::fillMERemark(CPNR &po_PNR)
{
   ENTER;
   AB_Text        lt_segment, lt_remark;
   int            li_pos;
   int            li_rem_number, li_element_number;
   V_AB_Text      lv_arrayField(100) ;
   V_AB_Text      lv_remarkField(100) ;
   D_line_number  ld_line_number;
   AB_Char  		lc_line_number(2);

   po_PNR.resetSegment("ME") ;
   lt_segment = po_PNR.getNextSegment("ME");

   while (lt_segment.NotIsNull())
   {
      li_rem_number= lv_arrayField.parseStringMultDel(lt_segment, "!");

      for ( int li_index=1; li_index <= li_rem_number; li_index++ )
      {
         if ( lv_arrayField[li_index].At(3).NotIsNull() )
         {
            ld_line_number = lv_arrayField[li_index].At(3).For(2);

            li_element_number = lv_remarkField.parseStringMultDel( lv_arrayField[li_index].At(5), "@" );

            for ( int li_elem_index=1; li_elem_index <= li_element_number; li_elem_index++ )
            {
               lv_remarkField[li_elem_index].removeLeadingBlanks() ;

               if ( lv_remarkField[li_elem_index].For(3) == "CS$" )
                  lt_remark      = lv_remarkField[li_elem_index].At(4);
               else
                  lt_remark      = lv_remarkField[li_elem_index];

					lc_line_number = ld_line_number;
               mva_MERemarkSection[mi_MERemarkIndex++] = lc_line_number + lt_remark;
            }

         }
      }

      lt_segment = po_PNR.getNextSegment("ME");
   }

   EXIT;
}
// GAP US089 END
//---------------------------------------------------------------------------

void CIUR::fill_IrHeader(CPNR &po_PNR)
{
   ENTER;

   AB_Text     lt_total_segment ;
   int         li_pos,
               li_index,
               li_counter ;
	AB_Boolean  lb_buildCfa ;

   li_counter        = 0 ;
   md_session_id     = po_PNR.getSessionId() ;
   md_interface_type = po_PNR.getInterfaceType() ;
   md_creation_date  = po_PNR.getCreationDate() ;
   md_IRT_pnr_time    = AB_null ;

   po_PNR.resetSegment("M0") ;
   lt_total_segment = po_PNR.getNextSegment("M0")  ;

   if (lt_total_segment.NotIsNull())
	{
		// Get invoice no and iur type

      md_invoiceNo = lt_total_segment.At(37).For(7) ;
      mc_iurType   = lt_total_segment.At(14).For(1) ;

      if ( mc_iurType == 5)
      {
         processVoidTicket(po_PNR);
         EXIT;
         return;
      }

	}
   else
      CMsgHandler::setMsg(AB_Integer(2509),
								  AB_Char(15," "),
								  D_error_track("IUR-PROCHDR"),
								  AB_Integer(1),
								  AB_Varchar(20,AB_null));

	mb_buildCfa = chectNT_getCFA(po_PNR) ;

   AB_Text  lt_segment ;
   po_PNR.resetSegment("M9") ;
   lt_segment = po_PNR.getNextSegment("M9") ;
   while (lt_segment.NotIsNull()) {
      if (lt_segment.At(5).For(3) == "PH/") {
			mv_phoneField = lt_segment.At(8);
	  	}
      lt_segment = po_PNR.getNextSegment("M9") ;
   }

	if (mb_buildCfa )
	{
		populate_IrHeader_SectionM0(lt_total_segment) ;

   	if (! CMsgHandler::getMsgStatus())
      	return ;

		populate_IrHeader_SectionM9(po_PNR) ;

   	if (! CMsgHandler::getMsgStatus())
      	return ;

		if ( mb_motli_branch == AB_true )
		{
			populate_IrHeader_SectionM8(po_PNR) ;

   		if (! CMsgHandler::getMsgStatus())
      		return ;
		}
	}

	if ( mc_iurType < 3 || mc_iurType == "B")
	{
		fillM5Acc(po_PNR) ;
	}

	fillM6FareCalc(po_PNR) ;

   // GAP US089 START
   fillMERemark(po_PNR);
   // GAP US089 END

   // SCR-437808-JJ-13OCT2009
   setCopIdByIata();

   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::populate_IrHeader_SectionM0(AB_Text &pt_total_segment)
{
	ENTER;

	AB_Char    lc_pnrDate(5) ;
	AB_Char    lc_recordCreationDate(5) ;
	AB_Char    lc_recordCreationTime(5) ;
	AB_Char    lc_depDate(5) ;
	AB_Char    lc_pnrYear(2) ;
	AB_Integer testi ;
	AB_Date	  lt_dateToCompare ;

   AB_Char    lc_InterfaceDate(5) ;     // 20NOV98 - 1234 - SC
   AB_Char    lc_InterfaceYear(2) ;     // 20NOV98 - 1234 - SC
	AB_Date    ld_InterfaceDate    ;     // 08APR99 - 1727 - SC
   AB_Char    lc_year(2) ;

   md_IRT_iata_no      = pt_total_segment.At(44).For(2)  +
								pt_total_segment.At(47).For(5) +
								pt_total_segment.At(53).For(1) ;

	/* SCR: 429632 check if this iata belongs to MOTLI branch */
	CBraComp lc_braComp;
	D_system_id ld_sys="MOTL";

	lc_braComp.get_bracompByIata(ld_sys , md_IRT_iata_no);

   if ( ! CMsgHandler::getMsgStatus())
	{
		 CMsgHandler::clearMsg();
	}
	else
	{
		if ( lc_braComp.getStartDate() <=  AB_Date::now() )
		{
			mb_motli_branch = AB_true;
			md_motli_cop_id = lc_braComp.getCopId();
			md_motli_bra_id = lc_braComp.getSysParm3();
			md_motli_cfa_code = lc_braComp.getSysParm1();
			md_motli_cfa_dpt = lc_braComp.getSysParm2();
		}

	}
   md_IRT_reference    = pt_total_segment.At(54).For(8) ;
   md_IRT_reference.leftJustify();
	md_IRT_booking_no = md_IRT_reference;

	if ( pt_total_segment.At(79).For(1) == "1" ||
		  pt_total_segment.At(80).For(1) == "1" ||
		  pt_total_segment.At(81).For(1) == "1" )
	{
		// Found Automated MCO or PTA or TOUR ORDER
		mb_MPT = AB_true ;
	}

	md_IRT_crs_id = "SAB" ;

	/************************************
	 * 22APR2010 - GAP G9123 SCR 440488 *
	 ************************************/
	if ( pt_total_segment.At(89).For(5) != "     " )
	   md_IRT_booking_pcc         = pt_total_segment.At(89).For(5)  ;
	if ( pt_total_segment.At(96).For(3) != "   " )
	   md_IRT_booking_gds_agent   = pt_total_segment.At(96).For(3)  ;
	if ( pt_total_segment.At(127).For(5) != "     " )
	   md_IRT_ticketing_pcc       = pt_total_segment.At(127).For(5) ;
	if ( pt_total_segment.At(134).For(3) != "   " )
		md_IRT_ticketing_gds_agent = pt_total_segment.At(134).For(3) ;

	LOG(logDEBUG) << " -- md_IRT_booking_pcc         = |" << md_IRT_booking_pcc << "|"        ;
	LOG(logDEBUG) << " -- md_IRT_booking_gds_agent   = |" << md_IRT_booking_gds_agent << "|"  ;
	LOG(logDEBUG) << " -- md_IRT_ticketing_pcc       = |" << md_IRT_ticketing_pcc   << "|"    ;
	LOG(logDEBUG) << " -- md_IRT_ticketing_gds_agent = |" << md_IRT_ticketing_gds_agent << "|" ;

   /**************************************************************************
   08APR99 - 1727 - SC - PNR Creation Date DDMMM M0 117(5) <=Interface Date
   **************************************************************************/

	// Get 5 char Date
   lc_pnrDate         = pt_total_segment.At(117).For(5) ;

	// Get the year

	adjustPNRDate( md_creation_date,
						lc_pnrDate,
						md_IRT_pnr_date,
						mt_pnrDate);

	if (! CMsgHandler::getMsgStatus())
		return ;

	md_IRT_pnr_time = pt_total_segment.At(122).For(5) ;



   // ROW : RECORD CREATION DATE
	// Get 5 char Date
	AB_Date           dummyDate ;
   lc_recordCreationDate       = pt_total_segment.At(226).For(5) ;
   //lc_recordCreationTime       = pt_total_segment.At(221).For(5) ;
	md_IRT_record_creation_time = pt_total_segment.At(221).For(5) ;

	LOG(logDEBUG) << "AM====RCD_date=[" << lc_recordCreationDate << "]"  ;
	LOG(logDEBUG) << "AM====RCD_time=[" << md_IRT_record_creation_time << "]"  ;

	// Get the year

	adjustRecordCreationDate( md_creation_date,
						lc_recordCreationDate,
						md_IRT_record_creation_time,
						md_IRT_record_creation_date);

	if (! CMsgHandler::getMsgStatus())
		return ;





	lc_depDate = pt_total_segment.At(142).For(5) ;

	if ( lc_depDate != "     " )
	{
   	lc_year = mt_pnrDate.get_default_year(lc_depDate) ;
		lc_year.rightJustify('0') ;

   	md_IRT_start_date = lc_depDate + lc_year ;
	}
	else
		md_IRT_start_date = "       " ;

	if (mc_iurType == 1 || mc_iurType == "B")
	{
		mi_noofPax = pt_total_segment.At(190).For(3) ;
		if ( mb_MPT)
		{
			LOG(logDEBUG) << "I AM IN MC" ;
			mi_noofPax = pt_total_segment.At(187).For(3) ;
		}
	}
	else
		mi_noofPax = pt_total_segment.At(187).For(3) ;


	if (pt_total_segment.At(196).For(3) == "   ")
		mi_noofentitle = AB_Integer(0) ;
	else
		mi_noofentitle = pt_total_segment.At(196).For(3) ;

	// Get Address and populate

	populate_IrAddress(pt_total_segment) ;

   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::populate_IrAddress(AB_Text &pt_total_segment)
{
	ENTER;

	int		  i,li_pos ;
	AB_Varchar lv_long_address(100);
	AB_Varchar lv_long_phone(20);
	AB_Varchar lv_addr(100);
	CAddress   lo_Address ;
	AB_Text	lt_test ;
	AB_Varchar lv_modAddress(512);
	D_address_line ld_modAddressLine;
	V_AB_Varchar lv_arrayFieldMod(100,100) ;

	lt_test = pt_total_segment.At(232) ;
	LOG(logDEBUG) << "ADDREES BEFORE ALT/HOME: [" << lt_test << "]";

	/*
	 * Sabre currently only sends 5 lines of address
	 */
   D_address_line lv_address[6] ;
	V_AB_Varchar lv_arrayField(100,100) ;
	mb_address = AB_false ;

	li_pos = lv_arrayField.parseStringMultDel(lt_test,"!") ;

	//(G9150 START)
	//remove "ALT-*" and "HOME-*" lines (out of 7-11 lines) and reparse
	lv_modAddress = "";
	for ( i = 7; i <= 11; i++)
	{
		ld_modAddressLine = lv_arrayField[i];
		LOG(logDEBUG) << "------------[" << ld_modAddressLine << "]";
		if ( ld_modAddressLine.IsNull() ) continue;

		ld_modAddressLine.removeLeadingBlanks();

		// SCR 521568 ; remove 2V from address
		string no2Vaddress;
		if ( PCRE::pcre_match("^2V (.*)$" , ld_modAddressLine.to_string() ))
		{
				PCRE::pcre_get_group(1, no2Vaddress);
				LOG(logDEBUG) << "2V found, stripped["  << no2Vaddress.data() << "]";
				ld_modAddressLine = no2Vaddress.data();
		}
		// SCR 521568 END ; remove 2V from address


		if ( ld_modAddressLine.For(4) != "ALT-" && ld_modAddressLine.For(5) != "HOME-")
			lv_modAddress = lv_modAddress + ld_modAddressLine + "!";
		LOG(logDEBUG) << "[" << lv_modAddress << "]";
	}

	lv_modAddress = lv_modAddress +  "!!!!!";
	LOG(logDEBUG) << "ADDREES BEFORE ALT/HOME: [" << lv_modAddress << "]";
	lv_arrayFieldMod.parseStringMultDel(lv_modAddress,"!") ;

	md_IA_addr1[mi_IA_I] = lv_arrayFieldMod[1] ;
	md_IA_addr2[mi_IA_I] = lv_arrayFieldMod[2] ;
	md_IA_addr3[mi_IA_I] = lv_arrayFieldMod[3] ;
	md_IA_addr4[mi_IA_I] = lv_arrayFieldMod[4] ;
	md_IA_addr5[mi_IA_I] = lv_arrayFieldMod[5] ;
	//(G9150 END)

	i = 12 ;

	while ( i <= 15 )
	{
		lv_long_phone = lv_arrayField[i] ;
		parsePhone(lv_long_phone) ;

		i++ ;
	}

	overrideHomePhone();

   if (md_IA_addr1[mi_IA_I].NotIsNull() ||
		 md_IA_addr2[mi_IA_I].NotIsNull() ||
		 md_IA_addr3[mi_IA_I].NotIsNull() ||
		 md_IA_addr4[mi_IA_I].NotIsNull() ||
		 md_IA_addr5[mi_IA_I].NotIsNull())
	{
      //lo_Address.setFourLineAddr(lv_long_address);
     	//lo_Address.getAddress(md_IA_addr1[mi_IA_I],
      					//md_IA_addr2[mi_IA_I],
      					//md_IA_addr3[mi_IA_I],
      					//md_IA_addr4[mi_IA_I]);

		mb_address = AB_true ;

	}

	if (mb_address)
		md_IA_type[mi_IA_I++] = "P" ;

	EXIT;
}

//----------------------------------------------------------------------------

void CIUR::parsePhone(AB_Varchar  &pv_long_phone)
{
	AB_Varchar lv_long_phone(20);

	int  li_pos       = pv_long_phone.findSubstring("-H") ;


	if ( li_pos == 0 )
	{
		li_pos       = pv_long_phone.findSubstring("-B") ;

		if ( li_pos <= 0 )  // 516462
                {
                  li_pos = pv_long_phone.findSubstring("-W") ;
                }

		if ( li_pos > 0 )
		{
			lv_long_phone = pv_long_phone.For(li_pos - 1) ;
			// found business phone
			mb_address = AB_true ;

			editPhoneNumber(lv_long_phone, md_IA_bphone_acode[mi_IA_I], md_IA_bphone_no[mi_IA_I]);
		}
		else
		{
			li_pos       = pv_long_phone.findSubstring("-F") ;
			if ( li_pos > 0 )
			{
				lv_long_phone = pv_long_phone.For(li_pos - 1) ;
				mb_address = AB_true ;

				// found FAX phone

				editPhoneNumber(lv_long_phone, md_IA_fax_acode[mi_IA_I], md_IA_fax_no[mi_IA_I]);

			}
			else
			{
				li_pos       = pv_long_phone.findSubstring("-M") ;
				if ( li_pos > 0 )
				{
					lv_long_phone = pv_long_phone.For(li_pos - 1) ;
					mb_address = AB_true ;

					// found MOBILE phone

					editPhoneNumber(lv_long_phone, md_IA_mphone_acode[mi_IA_I], md_IA_mphone_no[mi_IA_I]);

				}
				else
					return;
			}
		}
	}
	else
	{
		// found home phone
			lv_long_phone = pv_long_phone.For(li_pos - 1) ;

			mb_address = AB_true ;


			editPhoneNumber(lv_long_phone, md_IA_phone_acode[mi_IA_I], md_IA_phone_no[mi_IA_I]);

	}
}

//------------------------------------------------------------------------------

void CIUR::populate_IrHeader_SectionM9(CPNR &po_PNR)
{
   ENTER;

	/* compatibility with Receipt code start */

	AB_Varchar lv_delimiter(10) ;
	AB_Varchar lv_data(80) ;

	/* compatibility with Receipt code end		 */

	AB_Varchar lv_delimiterBeforeSlash(10) ;
	AB_Varchar lv_dataAfterSlash(80) ;
   AB_Char    lc_delimiter(2) ;
   AB_Char    lc_CONFdelimiter(5) ;
   int        li_pos,
              li_counter,
				  li_addr_cnt = 1,
				  li_slashPos;
   AB_Varchar lm_data(512) ;
   AB_Varchar lm_CONFdata(512) ;
   AB_Char 	  lm_CONFsup(3) ;
   AB_Char 	  lm_testCONFsup(3) ;
   AB_Text    lt_segment ;
   AB_Text    lt_temp;
	// GAP US092 START
   AB_Text 			lt_data;
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
	// GAP US092 END

	bool				lb_cnf_tkt_found = false;

	AB_Boolean lb_mlsFound 	= AB_false ;
	AB_Boolean lb_confFound = AB_false ;
	AB_Boolean lb_fop_found = AB_false ;
	AB_Boolean lb_delivery_addr_found = AB_false ;
	AB_Boolean lb_tvtFound = AB_false ;


	AB_Boolean lb_pr_found = AB_false ;

	// TKX CC EXP DATE START
	AB_Char lc_ccExpDate(4);
	md_TKX_cc_exp_date = AB_null;
	// TKX CC EXP DATE END

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

   po_PNR.resetSegment("M9") ;
   lt_segment = po_PNR.getNextSegment("M9") ;

   while (lt_segment.NotIsNull())
   {

		/* compatibility with Receipt code start */

		li_slashPos   = lt_segment.At(5).findSubstring("/") ;

		lv_delimiterBeforeSlash = lt_segment.At(5).For(li_slashPos-1) ;
		lv_dataAfterSlash       = lt_segment.At(5+li_slashPos+1);

		if ( li_slashPos >= 3 )
		{

			lv_delimiter  = lt_segment.At(5).For(li_slashPos-1) ;
			lv_data       = lt_segment.At(5+li_slashPos) ;
		}

		li_pos = lv_data.findSubstring("!") ;


		if (li_pos > 0)
		{

			lv_data = lv_data.For(li_pos - 1) ;
		}

		/* compatibility with Receipt code end   */

      lc_delimiter = lt_segment.At(5).For(2) ;

      
      cout << "FG: IUR.cpp: WHILE LOOP: lc_delimiter= " << lc_delimiter << flush << endl;

      cout << "FG: IUR.cpp: WHILE LOOP: lt_segment= " << lt_segment << flush << endl;

		// GAP US092 START
      lt_data      = lt_segment.At(8).to_string() ;
		// GAP US092 END
      lm_data      = lt_segment.At(8) ;
      li_pos       = lm_data.findSubstring("!") ;

      lc_CONFdelimiter = lt_segment.At(5).For(5) ;

      if (li_pos > 0)
		{
         lm_data = lm_data.For(li_pos - 1) ;
		}

		li_pos       = lt_data.findSubstring("!") ;

		if (li_pos > 0)
		{
			// GAP US092 START
         lt_data = lt_data.For(li_pos - 1) ;
			// GAP US092 END
		}

		if (lc_CONFdelimiter == "CONF/")
		{
			lb_confFound = AB_false ;

      	lm_CONFdata  = lt_segment.At(10) ;

			lm_CONFdata.removeLeadingBlanks();
			li_slashPos	 = lm_CONFdata.findSubstring("/");

			if ( li_slashPos > 1 && li_slashPos < 5 )
			{
				lm_CONFsup	 = lm_CONFdata.For(li_slashPos-1);

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
					lm_CONFdata	 = lm_CONFdata.At(li_slashPos+1).For(20);
      			li_pos       = lm_CONFdata.findSubstring("!") ;

      			if ( li_pos > 0 )
         			lm_CONFdata = lm_CONFdata.For(li_pos - 1) ;

           		lt_temp = lm_CONFdata;
           		lt_temp.removeLeadingBlanks();
           		lt_temp.removeTrailingBlanks();

			  		if(lt_temp.length() > 0)
					{
   					md_IRMK_type[mi_IRMK_I] 		= "CNF" ;
 						md_IRMK_pax_id[mi_IRMK_I]	= AB_null;
   		  			md_IRMK_remark[mi_IRMK_I++] 	= "SUP:" + lm_CONFsup + "/" + lm_CONFdata ;
					}
				}
			}
		}

		// GAP G9126/G9136 start
		else if (lt_segment.At(5).For(4) == "NFT/" && !lb_cnf_tkt_found) {
		  parseRemarkCNF(lt_segment.At(9));
		  lb_cnf_tkt_found = true;
	   }
		// GAP G9126/G9136 end

      else if (lc_delimiter == "CN"  && mb_motli_branch == AB_false )
		{
			if ( md_IRT_con_id.IsNull() && lm_data.For(3) != "   " )
         	md_IRT_con_id = lm_data.For(3) ;
		}
      else if (lc_delimiter == "EB")
		{

			cout << "FG: IUR.cpp: FG: IN ELSE EB " << flush << endl;
         /**
          * Electronic booking activity
          **/

         if ( mv_actionCode.IsNull() )
         {

				cout << "FG: IUR.cpp: FG: IN ELSE EB: IN IF actionCode IsNull " << flush << endl;

				// SCR 515485, undo the change.
            mv_actionCode = lm_data.For(2);
            mv_source     = lm_data.At(3).For(1) ;
            //mv_actionCode = "EB";
            //mv_source     = lm_data.At(1).For(1) ;

            cout << "FG: IUR.cpp: FG: IN ELSE EB: IN IF actionCode IsNull: lm_data= " << lm_data << " " << flush << endl;
            cout << "FG: IUR.cpp: FG: IN ELSE EB: IN IF actionCode IsNull:mv_actionCode= " << mv_actionCode << " " << flush << endl;
            cout << "FG: IUR.cpp: FG: IN ELSE EB: IN IF actionCode IsNull:mv_source= " << mv_source << " " << flush << endl;

				if ( lm_data.At(4).For(1) == "/" )
				{
					mv_reasonCode = lm_data.At(5).For(10);
				}
		   }
		}
	  else if (lc_delimiter == "PR")
	  {
		 if ( !lb_pr_found )
		 {
			 md_IRMK_type[mi_IRMK_I]     = "PR" ;
			 md_IRMK_remark[mi_IRMK_I]   = lm_data ;
			 md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			 lb_pr_found = AB_true;
		 }
	  }
	  else if (lv_delimiter == "FOP" && !lb_fop_found && lv_data.NotIsNull())
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
		/*
      else if (lc_delimiter == "DE")
		{
			if ( md_IRT_city_id.IsNull() && lm_data.For(3) != "   " )
         	md_IRT_city_id = lm_data.For(3) ;
		}
		*/
      else if (lc_delimiter == "TV")
		{
         if ( lb_tvtFound == AB_false )
         {
            lb_tvtFound = AB_true;
            lm_data.removeLeadingBlanks();
            if ( lm_data.IsNull() )
               lm_data = "X";
            md_IRMK_type[mi_IRMK_I]      = "TVT";
            md_IRMK_pax_id[mi_IRMK_I]    = AB_null;
            md_IRMK_remark[mi_IRMK_I++]  = lm_data.For(1);
         }
		}
      else if (lc_delimiter == "DP" && mb_motli_branch == AB_false )
		{
			if ( md_IRT_cfa_dpt.IsNull() && lm_data.For(4) != "    " )
         	md_IRT_cfa_dpt = lm_data.For(4) ;
		}
      else if (lc_delimiter == "RC")
		{
			if ( md_IRT_iata_dest.IsNull() && lm_data.For(2) != "  " )
         	md_IRT_iata_dest = lm_data.For(2) ;
		}
      else if (lc_delimiter == "ML")
		{
			if ( md_IRT_mktg_list.IsNull() && lm_data.For(1) != " " )
         	md_IRT_mktg_list = lm_data.For(1) ;
		}
		else if (lc_delimiter == "FS")
		{
			if ( md_savingCode.IsNull() ) {
				lm_data.removeLeadAndTrailBlanks();
				md_savingCode = lm_data;
				LOG(logDEBUG) << "-- md_savingCode = " << md_savingCode ;
			}
		}
      else if (lc_delimiter == "FF")
		{
			if ( md_fullFare.IsNull() && lm_data.For(10) != "          " )
         	md_fullFare = lm_data.For(10) ;
		}
      else if (lc_delimiter == "LP")
		{
			if ( md_lowPracFare.IsNull() && lm_data.For(10) != "          " )
         	md_lowPracFare = lm_data.For(10) ;
		}
      else if (lc_delimiter == "RB")
		{
			if ( md_rebate.IsNull() && lm_data.For(10) != "          " )
         	md_rebate = lm_data.For(10) ;
		}
		else if (lc_delimiter == "HS")
		{
      	lm_data      = lt_segment.At(7) ;
      	li_pos       = lm_data.findSubstring("!") ;

      	if (li_pos > 0)
         	lm_data = lm_data.For(li_pos - 1) ;

         lt_temp = lm_data;
         lt_temp.removeLeadingBlanks();

			//LOG(logDEBUG)  << "DEBUG " << lt_temp << " #" ;
			if(lt_temp.length()>0)  {
			   // SCR: 514016
			   md_IR_Itin_Hotel_Data[gi_IrHotelIndex] = lv_data;

    			gv_IrHotelData[gi_IrHotelIndex++] = lm_data ;
				}
		}
		else if (lc_delimiter == "CS")
		{

      	lm_data      = lt_segment.At(7) ;
      	li_pos       = lm_data.findSubstring("!") ;

      	if (li_pos > 0)
         	lm_data = lm_data.For(li_pos - 1) ;

         lt_temp = lm_data;
         lt_temp.removeLeadingBlanks();
			if(lt_temp.length()>0)
			   gv_IrCarData[mi_IrCarIndex++] = lm_data ;
		}
      else if (lc_delimiter == "SC")
		{
			if ( !lb_mlsFound)
			{
				lb_mlsFound = AB_true ;
   			md_IRMK_type[mi_IRMK_I]     = "MLS" ;
            lt_temp = lm_data;
            lt_temp.removeLeadingBlanks();
			   if(lt_temp.length()>0)
   		   	md_IRMK_remark[mi_IRMK_I]   = lm_data ;
 				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}
		}
      else if (lc_delimiter == "BB" && mb_motli_branch == AB_false )
		{
			if ( md_IRT_cop_id.IsNull() )
			{
				LOG(logDEBUG) << "BOOKING COMOPANY"  ;
				LOG(logDEBUG) << md_IRT_cop_id ;
				if(lm_data.For(2) != "  ")
   				md_IRT_cop_id = lm_data.For(2) ;
				if(lm_data.At(3)  != "    ")
   				md_IRT_bra_id = lm_data.At(3) ;
			}
		} else if (lt_segment.At(5).For(3) == "DL/") {
			/*
			 * get address from M9 as delivery address.
			 * Note that this overrides the code change we did
			 * for Protravel in the version 1.91
			 */
			lm_data      = lt_segment.At(8);
			li_pos       = lm_data.findSubstring("!");
			if (li_pos > 0) {
				lm_data = lm_data.For(li_pos - 1);

				switch(li_addr_cnt) {
					case 1:	md_IA_addr1[mi_IA_I] = lm_data; break;
					case 2:	md_IA_addr2[mi_IA_I] = lm_data; break;
					case 3:	md_IA_addr3[mi_IA_I] = lm_data; break;
					case 4:	md_IA_addr4[mi_IA_I] = lm_data; break;
					case 5:	md_IA_addr5[mi_IA_I] = lm_data; break;
					case 6:	md_IA_addr6[mi_IA_I] = lm_data; break;
				}

				if (!lb_delivery_addr_found) {
					lb_delivery_addr_found = AB_true;
				}

				li_addr_cnt ++;
			}
		}
		// GAP US092 START
      else if (lt_segment.At(5).For(3) == "SF/")
		{
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

						LOG(logDEBUG)  << "SFM >> lc_amount = " << lc_amount  ;
						LOG(logDEBUG)  << "SFM >> ld_value = " << ld_value  ;

						if ( lb_amount_ok == AB_true 					&&
							  ld_value 		<= D_amount(999999.99) 	&&
							  ld_value 		>  D_amount(0.00) )
						{

							// Pick up Pax ID (mandatory)

							li_index++;

							if ( li_index < li_counter 					&&
								  lv_arrayField[li_index].length() > 0 &&
								  lv_arrayField[li_index].length() < 6 &&
								  ( lv_arrayField[li_index].findFirstCharNotInList(".0123456789") == 0 || lv_arrayField[li_index] == "PP" ) )
							{
                        AB_Text  lt_pax_p1;
                        AB_Text  lt_pax_p2;
                        int      li_pax_dot;
                        int      li_pax_id;
                        int      li_pax_len = lv_arrayField[li_index].length();

								lc_FM_pax = lv_arrayField[li_index];
                        li_pax_dot = lc_FM_pax.findSubstring(".");

                        if ( li_pax_dot > 0 )
                        {
                           lt_pax_p1 = lc_FM_pax.For(li_pax_dot - 1);
                           lt_pax_p2 = lc_FM_pax.At( li_pax_dot + 1).For(li_pax_len - li_pax_dot);

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
										    	lv_arrayField[li_index].For(2) 	== "CK" )	 		||
											 ( lv_arrayField[li_index].length() == 2					&&
												lv_arrayField[li_index].For(2)   == "CA"	) 			||
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
                                       				LOG(logDEBUG)  << "WW===== SFM CC is not valid!!!!!!!!  !!! "  ;
												}
											}
											// SCR 412640 End
											if (  lc_FM_fop != "CC" ||
												(  lc_FM_fop == "CC" &&
													lc_FM_exp_date != "    " ) )
											{
												md_IRMK_type[mi_IRMK_I]    = "SFM" ;
   		   								md_IRMK_remark[mi_IRMK_I] 	= 	(lb_cc_valid? "" : "BADCC/") +
																						lc_FM_pax         +
																						AB_Char(" ") 		+
																						AB_Char(13,lc_amount.to_string())+
																						lc_FM_description +
																						lc_FM_fop			+
																						lc_FM_cc_id			+
																						lc_FM_cc_no			+
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

												LOG(logDEBUG)  << "SFM >>> md_IRMK_remark = " << md_IRMK_remark[mi_IRMK_I]  ;
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
		// GAP US092 END

	  	// TKX CC EXP DATE START
      else if (lt_segment.At(5).For(4) == "EXP/")
	  	{
			if ( lt_data.NotIsNull() && md_TKX_cc_exp_date.IsNull() )
			{
				lc_ccExpDate = lt_data.At(2).For(4);

				if ( lc_ccExpDate.isAllNumeric() )
				{
					md_TKX_cc_exp_date = lc_ccExpDate;
				}
			}
	  	}
		// TKX CC EXP DATE END

		// scr# 292421 start
		else if (lt_segment.At(5).For(4) == "NUC/") {
			// do not update existing cfa
		  	md_IRMK_type[mi_IRMK_I]     = "NUC" ;
		  	md_IRMK_remark[mi_IRMK_I]   = "Y";
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
	   }
		// scr# 292421 end

		else if (lt_segment.At(5).For(4) == "PID/") {
			lm_data      = lt_segment.At(9);
			li_pos       = lm_data.findSubstring("!");
			if (li_pos > 0) {
				lm_data = lm_data.For(li_pos - 1);
			}

			lm_data.removeTrailingBlanks() ;
			if (lm_data .isAllNumeric())
			{
				md_IRMK_type[mi_IRMK_I]     = "PID" ;
				md_IRMK_remark[mi_IRMK_I]   = lm_data ;
				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}
	   }
		else if (lv_delimiter.For(1) == "R" && lv_delimiter.At(2).isAllNumeric() && mb_motli_branch == AB_false )
		{	/* R - receipt remark */
		   AB_Boolean lb_found = AB_false;

				/* search for matching receipt number */
				for ( int li_ctr = 1; li_ctr <= mi_receiptIndex && !lb_found;
						li_ctr++ )
				{

					if ( lv_delimiter == gd_receiptType[li_ctr] )
					{
						gd_receiptData[li_ctr] = gd_receiptData[li_ctr]
														+ " " + lv_data;
						lb_found = AB_true;
					}
				}

				if ( !lb_found )
				{
					gd_receiptType[++mi_receiptIndex] = lv_delimiter;
					gd_receiptData[mi_receiptIndex]   = lv_data;
				}
		  }	/* R - receipt remark */


			// GAP US107 START
			else if ( lc_delimiter == "TU" && !lb_tu_found )
			{
				if ( !lt_data.isAllNumeric() )
				{
					CMsgHandler::setMsg(3957, " ", "TU-PARSE", 1, AB_Varchar(20,"TU"));
					return;
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
					CMsgHandler::setMsg(3957, " ", "RS-PARSE", 1, AB_Varchar(20,"IF"));
					return;
				}

				editAmount( lt_data, D_amount(0.00), D_amount(9999999999.99), AB_true, AB_false );

				if (! CMsgHandler::getMsgStatus())
				{
					CMsgHandler::clearMsg();
					CMsgHandler::setMsg(3957, " ", "RS-PARSE", 2, AB_Varchar(20,"IF"));
					return;
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
						CMsgHandler::setMsg(3957, " ", "RS-PARSE", 3, AB_Varchar(20,"IF"));
						return;
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
					CMsgHandler::setMsg(3957, " ", "RS-PARSE", 1, AB_Varchar(20,"RS"));
					return;
				}

				editAmount( lt_data, D_amount(0.00), D_amount(9999999999.99), AB_true, AB_false );

				if (! CMsgHandler::getMsgStatus())
				{
					CMsgHandler::clearMsg();
					CMsgHandler::setMsg(3957, " ", "RS-PARSE", 2, AB_Varchar(20,"RS"));
					return;
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
						CMsgHandler::setMsg(3957, " ", "RS-PARSE", 3, AB_Varchar(20,"RS"));
						return;
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
					CMsgHandler::setMsg(3957, " ", "TN-PARSE", 1, AB_Varchar(20,"TN"));
					return;
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
							CMsgHandler::setMsg(3957, " ", "TN-PARSE", 2, AB_Varchar(20,"TN"));
							return;
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
							CMsgHandler::setMsg(3957, " ", "TN-PARSE", 3, AB_Varchar(20,"TN"));
							return;
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
							CMsgHandler::setMsg(3957, " ", "TN-PARSE", 4, AB_Varchar(20,"TN"));
							return;
						}

						md_ID_selling_price_to_shop = lv_arrayField[li_index].At(4);
						lv_amount = md_ID_selling_price_to_shop;
						lb_tn_sp_found = AB_true;
					}
					else if (lv_arrayField[li_index].For(3) == "CP-")
					{
						if ( lb_tn_cp_found )
						{
							CMsgHandler::setMsg(3957, " ", "TN-PARSE", 5, AB_Varchar(20,"TN"));
							return;
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
							CMsgHandler::setMsg(3957, " ", "TN-PARSE", 6, AB_Varchar(20,"TN"));
							return;
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
						CMsgHandler::setMsg(3957, " ", "TN-PARSE", 7, AB_Varchar(20,"TN"));
						return;
					}

					editAmount( lv_amount, D_amount(0.00), D_amount(9999999999.99), lb_percent_allowed, AB_false );
					if (! CMsgHandler::getMsgStatus())
					{
						CMsgHandler::clearMsg();
						CMsgHandler::setMsg(3957, " ", "TN-PARSE", 8, AB_Varchar(20,"TN"));
						return;
					}
				}

				if ( !lb_tn_nf_found || !lb_tn_sp_found )
				{
					if ( !lb_tn_nfs_found || !lb_tn_sp_found )
					{
						CMsgHandler::setMsg(3957, " ", "TN-PARSE", 9, AB_Varchar(20,"TN"));
						return;
					}
				}

				if ( lb_tn_nf_found || lb_tn_nfs_found )
				{
					D_amount ld_nett_fare_to_airline = D_amount(md_ID_nett_fare_to_airline);
					D_amount ld_selling_price_to_shop = D_amount(md_ID_selling_price_to_shop);

					if ( 	ld_selling_price_to_shop - ld_nett_fare_to_airline < D_amount(0.00) &&
							md_ID_commission_type != "$" )
					{
						CMsgHandler::setMsg(3957, " ", "TN-PARSE", 10, AB_Varchar(20,"TN"));
						return;
					}

				}

				md_ID_net_ticket_ind = "Y";
				lb_tn_found = AB_true;
			}
			// GAP US107 END

      	else if (lc_delimiter == "EN") {
         	mv_IRT_employee_number = lv_data.For(10) ;
			}

		// GAP 9090F Start: POS fee
		else if (lt_segment.At(5).For(4) == "FEE/") {
			lm_data      = lt_segment.At(9);
			li_pos       = lm_data.findSubstring("!");
			if (li_pos > 0) {
				lm_data = lm_data.For(li_pos - 1);
			}
			lm_data.removeTrailingBlanks() ;
			lm_data = lm_data + "/";

			AB_Varchar lv_cc_id(40);
      	lv_cc_id = lm_data.getNthBracketedField(1, "FOP-CC", "/") ;
			if (lv_cc_id.NotIsNull() && !validCreditCardNumber(lv_cc_id)) {
				lm_data = "BADCC/" + lm_data;
			}

		  	md_IRMK_type[mi_IRMK_I]     = "FEE" ;
		  	md_IRMK_remark[mi_IRMK_I]   = lm_data;
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
	   }
		// GAP 9090F End

		// GAP 9121 Start: AOF fee
		else if (lt_segment.At(5).For(4) == "AOF/") {
		   parseAOFremark(lt_segment.At(9));
		  	//md_IRMK_type[mi_IRMK_I]     = "AOF" ;
		  	//md_IRMK_remark[mi_IRMK_I]   = " ";
		  	//md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
	   }
		// GAP 9121 End

		// GAP 9090j Start
      else if (lc_delimiter == "IC")
		{
		  	md_IRMK_type[mi_IRMK_I]     = "IC" ;
		  	md_IRMK_remark[mi_IRMK_I]   = " ";
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9090j End

      else if (lv_delimiter == "GOW")
		{
			createUniqueRemark("MEM", lv_data);
		}

      else if (lv_delimiter == "NE")
		{
		  //createRemarkRows(pRemarkParser->parsed("NE"));
		}

		// GAP 9090K start
		else if ( lc_delimiter == "RP")
		{
			md_IRMK_type[mi_IRMK_I]     = "NTE" ;
			md_IRMK_remark[mi_IRMK_I]   = lt_data.For(70);
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9090K end

		// GAP 9090m Start
		else if (lt_segment.At(5).For(3) == "IRP" &&  mb_motli_branch == AB_false ) {
			lm_data      = lt_segment.At(8);
			li_pos       = lm_data.findSubstring("/");
			if (li_pos > 1 && lm_data.For(li_pos-1).isAllNumeric()) {
				li_pos       = lm_data.findSubstring("!");
				if (li_pos > 0) {
					lm_data = lm_data.For(li_pos - 1);
				}
				lm_data.removeTrailingBlanks() ;

				md_IRMK_type[mi_IRMK_I]     = "IRP" ;
				md_IRMK_remark[mi_IRMK_I]   = lm_data;
				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}
		}
		// GAP 9090m End

		// GAP 9106 start
		else if ( lc_delimiter == "FN")
		{
			md_IRMK_type[mi_IRMK_I]     = "FN" ;
			md_IRMK_remark[mi_IRMK_I]   = lt_data.For(13);
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9106 end

		// GAP G9143 Start
		else if (lt_segment.At(5).For(5) == "FPC/Y")
		{
		  	md_IRMK_type[mi_IRMK_I]     = "FPC" ;
		  	md_IRMK_remark[mi_IRMK_I]   = " ";
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP G9143 End

      lt_segment = po_PNR.getNextSegment("M9") ;
	}

   // SCR 207781 START
   parseM8SavingCodeFullFare(po_PNR);
   // SCR 207781 END

	if (md_savingCode == "") {
		md_savingCode = AB_null;
	}

	if (md_fullFare == "") {
		md_fullFare = AB_null;
	}

	if (lb_delivery_addr_found) {
		md_IA_type[mi_IA_I++] = "D";
	}

   EXIT;
}

void CIUR::populate_IrHeader_SectionM8(CPNR &po_PNR)
{
   ENTER;

	/* compatibility with Receipt code start */

	AB_Varchar lv_delimiter(10) ;
	AB_Varchar lv_data(80) ;

	/* compatibility with Receipt code end		 */

   AB_Varchar lc_delimiter(3) ;
   int        li_pos,
				  li_slashPos;
   AB_Varchar lm_data(60) ;
   AB_Text    lt_segment ;
	V_AB_Varchar 	lv_arrayField(100,100) ;

   po_PNR.resetSegment("M8") ;
   lt_segment = po_PNR.getNextSegment("M8") ;

	if ( mb_motli_branch == AB_true )
	{
		md_IRT_cfa_code = AB_null;
		md_IRT_cfa_type = AB_null;
		md_IRT_cfa_id   = AB_null;
	}

   while (lt_segment.NotIsNull()) {
		if (lt_segment.At(5).For(4) == "M*/-" ) {

			li_slashPos   = lt_segment.At(8).findSubstring("/") ;

			if ( li_slashPos >= 3 )
			{

				lv_delimiter  = lt_segment.At(8).For(li_slashPos-1) ;
				lv_data       = lt_segment.At(8+li_slashPos) ;
				lm_data       = lt_segment.At(8+li_slashPos) ;
			}

			li_pos = lv_data.findSubstring("!") ;

			if (li_pos > 0)
			{

				lv_data = lv_data.For(li_pos - 1) ;
				lm_data = lm_data.For(li_pos - 1) ;
			}

			lc_delimiter = lv_delimiter.At(2);


			if (lc_delimiter == "CN"  && mb_motli_branch == AB_true )
			{
				if ( md_IRT_con_id.IsNull() && lm_data.For(3) != "   " )
					md_IRT_con_id = lm_data.For(3) ;
			}
			else if (lc_delimiter == "BB" && mb_motli_branch == AB_true )
			{
				if ( md_IRT_cop_id.IsNull() )
				{
					LOG(logDEBUG) << "BOOKING COMOPANY"  ;
					LOG(logDEBUG) << md_IRT_cop_id ;
					if(lm_data.For(2) != "  ")
						md_IRT_cop_id = lm_data.For(2) ;
					if(lm_data.At(3)  != "    ")
						md_IRT_bra_id = lm_data.At(3) ;
				}
			}

			else if (lc_delimiter == "DP" && mb_motli_branch == AB_true )
			{
				if ( md_IRT_cfa_dpt.IsNull() && lm_data.For(4) != "    " )
					md_IRT_cfa_dpt = lm_data.For(4) ;
			}
			if (lc_delimiter == "CF" && mb_motli_branch == AB_true )
			{

				int li_length = (lm_data.length() > 11) ? 11:lm_data.length();
				AB_Varchar lv_cfa_id(7);

				lv_cfa_id = lm_data.At(4).For(li_length - 4 );

				if ( lv_cfa_id.IsNull() )
					lv_cfa_id = "0000000";
				else
					lv_cfa_id.padLeading( 7 - lv_cfa_id.length(), '0');

				// CFA EXP END

				if ( md_IRT_cfa_code.IsNull() )
				{
					md_IRT_cfa_code = lm_data.For(3) ;
					md_IRT_cfa_type = lm_data.At(li_length) ;
					md_IRT_cfa_id   = md_IRT_cfa_code + lv_cfa_id + md_IRT_cfa_type ;
				}
			}

			else if ( mb_motli_branch == AB_true && lc_delimiter.For(1) == "R" && lc_delimiter.At(2).isAllNumeric() )
			{	/* R - receipt remark */
				AB_Boolean lb_found = AB_false;


					/* search for matching receipt number */
					for ( int li_ctr = 1; li_ctr <= mi_receiptIndex && !lb_found;
							li_ctr++ )
					{

						if ( lc_delimiter == gd_receiptType[li_ctr] )
						{
							gd_receiptData[li_ctr] = gd_receiptData[li_ctr]
															+ " " + lm_data;
							lb_found = AB_true;
						}
					}

					if ( !lb_found )
					{
						gd_receiptType[++mi_receiptIndex] = lc_delimiter;
						gd_receiptData[mi_receiptIndex]   = lm_data;
					}
			  }	/* R - receipt remark */

			else if (lt_segment.At(9).For(3) == "IRP" &&  mb_motli_branch == AB_true )
			{
				lm_data = lt_segment.At(12);
				li_pos       = lm_data.findSubstring("/");
				if (li_pos > 1 && lm_data.For(li_pos-1).isAllNumeric()) {

					li_pos       = lm_data.findSubstring("!");
					if (li_pos > 0) {
						lm_data = lm_data.For(li_pos - 1);
					}
					lm_data.removeTrailingBlanks() ;

					md_IRMK_type[mi_IRMK_I]     = "IRP" ;
					md_IRMK_remark[mi_IRMK_I]   = lm_data;
					md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
				}
			}
		}

      lt_segment = po_PNR.getNextSegment("M8") ;
	}

	if ( md_IRT_cop_id.IsNull() )
		md_IRT_cop_id = md_motli_cop_id;
	if ( md_IRT_bra_id.IsNull() )
		md_IRT_bra_id = md_motli_bra_id;
	if ( md_IRT_cfa_code.IsNull() )
		md_IRT_cfa_code = md_motli_cfa_code;
	if ( md_IRT_cfa_dpt.IsNull() )
		md_IRT_cfa_dpt = md_motli_cfa_dpt;
	if ( md_IRT_cfa_type.IsNull() )
		md_IRT_cfa_type = "C";
	if ( md_IRT_cfa_id.IsNull() )
		md_IRT_cfa_id = md_IRT_cfa_code + "0000000" + md_IRT_cfa_type;
   EXIT;
}

void CIUR::create_SFN_Remark(AB_Boolean &pb_FR_found)
{
   md_IRMK_type[mi_IRMK_I]  	= "SFN" ;
   md_IRMK_remark[mi_IRMK_I] 	= "N" ;
 	md_IRMK_pax_id[mi_IRMK_I] 	= AB_null;
	mi_IRMK_I++;

	pb_FR_found = AB_true;
}

//------------------------------------------------------------------------------

AB_Boolean  CIUR::chectNT_getCFA(CPNR &po_PNR)
{
   ENTER;

   AB_Char    lc_delimiter(2) ;
   int        li_pos;
   AB_Varchar lv_data(60) ;
   AB_Varchar lv_NTdata(60) ;
   AB_Text    lt_segment ;
   AB_Char    lc_NTData ;
	AB_Boolean lb_NTfound ;
	AB_Boolean lb_CFfound ;


	lb_NTfound = AB_false ;
	lb_CFfound = AB_false ;

   po_PNR.resetSegment("M9") ;
   lt_segment = po_PNR.getNextSegment("M9") ;

   while (lt_segment.NotIsNull())
   {
      lc_delimiter = lt_segment.At(5).For(2) ;
      lv_data      = lt_segment.At(8) ;
      li_pos       = lv_data.findSubstring("!") ;

      if (li_pos > 0)
         lv_data = lv_data.For(li_pos - 1) ;

      if (lc_delimiter == "CF" )
		{
			lb_CFfound = AB_true ;
			// CFA EXP START

			int li_length = (lv_data.length() > 11) ? 11:lv_data.length();
			AB_Varchar lv_cfa_id(7);

			lv_cfa_id = lv_data.At(4).For(li_length - 4 );

			if ( lv_cfa_id.IsNull() )
				lv_cfa_id = "0000000";
			else
				lv_cfa_id.padLeading( 7 - lv_cfa_id.length(), '0');

			// CFA EXP END
			if ( md_IRT_cfa_code.IsNull() )
			{
				md_IRT_cfa_code = lv_data.For(3) ;
				md_IRT_cfa_type = lv_data.At(li_length) ;
				md_IRT_cfa_id   = md_IRT_cfa_code + lv_cfa_id + md_IRT_cfa_type ;

				if ( mc_iurType == "1" || mc_iurType == "B" || mc_iurType == "2")
					break ;
			}
		}
      else if (lc_delimiter == "NT")
      {
			lb_NTfound = AB_true ;
			lv_NTdata = lv_data ;
			if ( lv_NTdata.For(1) == "N" )
				lb_NTfound = AB_false ;
      }

		if (lb_NTfound && lb_CFfound )
			break ;

   	lt_segment = po_PNR.getNextSegment("M9") ;
   }

	if ( !lb_NTfound)
	{
		if ( mc_iurType == "3" )
			return(AB_false) ;
		else
			return(AB_true) ;
	}
	else
		return(AB_true) ;

   EXIT;
}


//------------------------------------------------------------------------------

void CIUR::fill_IrItinerary(CPNR &po_PNR)
{
   ENTER;

   int li_counter, li_M3_index ;
   AB_Text lt_section ;
	ItinType itin_type = NO_TYPE;
	string label_TYP_Value;

   li_counter = 0 ;
	mv_itinTransport = AB_null ;


   po_PNR.resetSegment("M3") ;
   lt_section = po_PNR.getNextSegment("M3") ;

	mi_NonArcItin = 0 ;
	mi_IgnoredItin = 0 ;

   while (lt_section.NotIsNull())
   {

		cout << "MEL_JIAN: lt_section= " << lt_section << endl << flush;
		cout << "MEL_JIAN: lt_section= " << lt_section << endl << flush;

		cout << "MEL_JIAN: lt_section.At(5).For(1)= " << lt_section.At(5).For(1)  << endl << flush;
		cout << "MEL_JIAN: lt_section.At(15).For(3)= " << lt_section.At(15).For(3)  << endl << flush;

		// G9150 - Data Standartization, INIT
		itin_type = NO_TYPE;
		label_TYP_Value = "";
		pRemarkParser->resetRemark("LABELS");

		// G9090c start
      if ((lt_section.At(15).For(3) == "CNA")) {
			AB_Char lc_M5link(1);
			lc_M5link = lt_section.At(6).For(1);
		   md_IgnoredItin[++ mi_IgnoredItin] = lt_section.At(3).For(2) + lc_M5link  ;
      	lt_section = po_PNR.getNextSegment("M3") ;
			continue;
		}

		// LABELS applicable only to third form of M3
      if ( lt_section.At(5).For(1) == "1"   // first form (AIR)
			  ||  (lt_section.At(5).For(1) == "3" && lt_section.At(15).For(3) == "HHL")  // second form
			  )
		{
		  // LABELS are not applicable to these forms of M3
		}
		else
		{
			// G9150 - Data Standartization, parse part of segment as LABELS
			pRemarkParser->parseByName("LABELS", lt_section.At(36).to_string());
			pRemarkParser->getData("LABELS", "TYP", label_TYP_Value);
		}

   	md_IO_prod_code[mi_IO_I] = lt_section.At(15).For(3);
   	md_IR_prod_code[mi_IR_I] = lt_section.At(15).For(3);
		// G9090c end

      if ((lt_section.At(15).For(3) == "HHL") ||
			 (lt_section.At(15).For(3) == "HTL") ||
			 (lt_section.At(15).For(3) == "HHT") ||
			 (lt_section.At(15).For(3) == "BOQ") ||
			 (lt_section.At(15).For(3) == "BEQ") ||
			 (lt_section.At(15).For(3) == "CBQ"))
		{
         fill_IrRentalHotel(lt_section) ;
		}
      else if (lt_section.At(15).For(3) == "CAR")
		{
			LOG(logDEBUG) << "GOING FOR CAR"   ;
		   itin_type = CAR;
			pRemarkParser->parseByName("LABELS", "TYP-CAR"); //TYP is mandatory, missing in spec
         fill_IrRentalCar(lt_section) ;
			LOG(logDEBUG) << "CAME OUT OF CAR"   ;
		}
		else if ( lt_section.At(5).For(1) == "1" )
		{
			fillIrTransportM3(lt_section,po_PNR) ;
		}
		else if ( lt_section.At(5).For(1) == "6" )
		{
		   itin_type = RAIL;
			fillIrRailM3(lt_section,po_PNR) ;
		}
		else if ( lt_section.At(5).For(1) == "5" )
		{
		   itin_type = BUS;
			fill_IrBusM3(lt_section) ;
		}

		// GAP US092 START

		else if ( lt_section.At(5).For(1) == "4" 	||
					 lt_section.At(5).For(1) == "D"	||
					 lt_section.At(5).For(1) == "F"	||
					 lt_section.At(5).For(1) == "H" )
		{
		   itin_type = SHIP;
			fill_IrShipM3(lt_section) ;
		}
		else if ( lt_section.At(5).For(1) == "2" 	||
					 lt_section.At(5).For(1) == "G"	||
					 lt_section.At(5).For(1) == "J" )
		{
		   itin_type = TOUR;
			fill_IrTourM3(lt_section) ;
		}

		// GAP US092 END

      // GAP G9139 START --LIMO SEGMENT
      else if ( lt_section.At(5).For(1) == "8"     &&
                lt_section.At(15).For(3) == "OTH"  &&
						(    lt_section.At(39).For(9) == "/TRANSFER"
						  || lt_section.At(39).For(5) == "/LIMO"
						  || label_TYP_Value == "LIM"
						)
					)
      {
		   itin_type = LIMO;
         fill_IrRentalLimo(lt_section) ;
      }

		else if ( lt_section.At(5).For(1) == "C" 	)
		{
		   itin_type = LAND;
			fill_IrLandM3(lt_section) ;
		}

      // ignore 'OTH' with product code of 'M'
      else if ( ( lt_section.At(5).For(1) == "M"     ||
       			 lt_section.At(5).For(2) == "8 " )    &&
                ((lt_section.At(15).For(3) == "OTH")  ||
                (lt_section.At(15).For(3) == "LIM")))
      {
	LOG(logDEBUG) << "CIUR::fill_IrItinerary: got OTH/LIM seg with M, ignore it"  ;
      }

      // GAP G9139 END

		else
		{
			if ( fill_IrOtherM3(lt_section) )
			{
				if ( label_TYP_Value == "INS" ) itin_type = INSURANCE;
			}
		}

		// G9150 - Data Standartization
		populateRestOfLabels(itin_type);

      lt_section = po_PNR.getNextSegment("M3") ;
   }

	validateItin();

   EXIT;
}

//-----------------------------------------------------------------------------

void CIUR::fillIrTransportM3(AB_Text &pt_segment, CPNR &po_PNR)
{
   ENTER;

   int        li_counter,
			     li_pos ;
   AB_Char    lc_date(5) ;
   AB_Char    lc_service(5) ;
   AB_Char    lc_itinLine(2) ;
   AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_seatData ;
	AB_Text 	  lt_ftpMiles ;
	AB_Varchar lv_airInfo(255) ;
	AB_Varchar lv_str(255) ;
   AB_Char    lc_year(2) ;

   mi_TransportIndex  = 0;

	md_IT_itin_type[mi_IT_I] = "A" ;

	if (pt_segment.For(1) == "!" )
		pt_segment = pt_segment.At(2) ;

	md_IT_line_number[mi_IT_I]    = pt_segment.At(3).For(2) ;

	lc_date = pt_segment.At(10).For(5) ;

	if (lc_date != "     " )
	{
		//begin of SCR516217, capture departure year for AIR M3
		AB_Char lc_departure_year(4);
		lc_departure_year = pt_segment.At(236).For(4) ;
		if ( lc_departure_year != "    " )
		{
			lc_year = lc_departure_year.At(3).For(2);
		}else
		{
   		lc_year = mt_pnrDate.get_default_year(lc_date) ;
		}
		//end of SCR516217

		lc_year.rightJustify('0') ;

		md_IT_start_date[mi_IT_I] = lc_date + lc_year ;
	}
	else
		md_IT_start_date[mi_IT_I] = md_IRT_pnr_date ;

	mv_service_status = pt_segment.At(8).For(2) ;
	// md_IT_boarding_pass[mi_IT_I]  = pt_segment.At(18).For(1) ;

	md_IT_orig_city_id[mi_IT_I]   = pt_segment.At(19).For(3) ;
	md_IT_orig_cityname[mi_IT_I]  = pt_segment.At(22).For(17) ;
	md_IT_dest_city_id[mi_IT_I]   = pt_segment.At(39).For(3) ;
	md_IT_dest_cityname[mi_IT_I]  = pt_segment.At(42).For(17) ;

	md_IT_sup_id[mi_IT_I]         = pt_segment.At(59).For(2) ;

	lc_service = pt_segment.At(61).For(5) ;

	if (lc_service == "     ")
		md_IT_service[mi_IT_I]     = "OPEN" ;
	else
	{
		lc_service.leftJustify() ;
		md_IT_service[mi_IT_I]     = lc_service;
	}

	md_IT_classofservice[mi_IT_I] = pt_segment.At(66).For(1) ;

	if (pt_segment.At(68).For(5) == "     ") {
		md_IT_start_time[mi_IT_I] = AB_null;
	} else {
		md_IT_start_time[mi_IT_I] = CheckTime(pt_segment.At(68).For(5));
	}

	if (pt_segment.At(73).For(5) == "     ") {
		md_IT_end_time[mi_IT_I] = AB_null ;
	} else {
		md_IT_end_time[mi_IT_I] = CheckTime(pt_segment.At(73).For(5));
	}

	// get elapsed time
	lv_str = pt_segment.At(78).For(8) ;
	lv_str.removeLeadAndTrailBlanks();
	if (lv_str == "") {
		md_IT_elapsed_time[mi_IT_I] = AB_null;
	} else {
		if ((li_pos = lv_str.findSubstring(".")) == 0) {
			lv_str = "    " + lv_str;
			li_pos = lv_str.length() - 3;
			md_IT_elapsed_time[mi_IT_I] =  lv_str.At(li_pos);
		} else {
			lv_str = "00" + lv_str + "00";
			li_pos = lv_str.findSubstring(".");
			md_IT_elapsed_time[mi_IT_I] = lv_str.At(li_pos-2).For(2) +
													lv_str.At(li_pos+1).For(2);
		}
	}

	md_IT_meal[mi_IT_I]           = pt_segment.At(86).For(4) ;
	if (md_IT_meal[mi_IT_I] == "   ")
		md_IT_meal[mi_IT_I] = AB_null ;
	md_IT_arrival_ind[mi_IT_I]    = pt_segment.At(91).For(1) ;

	md_IT_no_of_stops[mi_IT_I]    = pt_segment.At(92).For(1) ;
	// If no of stops is @ - For VIA RAIL ticket they sometimes send it
	if (md_IT_no_of_stops[mi_IT_I] == "@")
		md_IT_no_of_stops[mi_IT_I] = "0" ;

	md_IT_carrier_type[mi_IT_I]   = pt_segment.At(113).For(3) ;
	md_IT_actual_miles[mi_IT_I]   = pt_segment.At(116).For(6) ;
	md_IT_dep_terminal[mi_IT_I]   = pt_segment.At(130).For(26) ;
	md_IT_dep_gate[mi_IT_I]       = pt_segment.At(156).For(4) ;
	md_IT_arr_terminal[mi_IT_I]   = pt_segment.At(160).For(26) ;
	md_IT_arr_gate[mi_IT_I]       = pt_segment.At(186).For(4) ;
	md_IT_report_time[mi_IT_I]    = pt_segment.At(190).For(5) ;
	md_IT_aff_carrier[mi_IT_I]    = pt_segment.At(196).For(37) ;
	md_IT_airline_locator[mi_IT_I]= pt_segment.At(240).For(8) ;
	md_IT_reference[mi_IT_I]      = md_IRT_reference ;

	lc_itinLine                   = pt_segment.At(3).For(2) ;

   if (mv_itinTransport.IsNull())
      mv_itinTransport = lc_itinLine ;
   else
      mv_itinTransport = mv_itinTransport + lc_itinLine ;

	// fill Personitin

	lc_noofSeats = pt_segment.At(128).For(2) ;
	lt_seatData  = pt_segment.At(249) ;

	lt_ftpMiles = pt_segment.At(122).For(6) ;
	fillConsumDetail(lc_noofSeats,
					 md_IT_itin_type[mi_IT_I],
				 	 lc_itinLine,
					 lt_seatData,
					 lt_ftpMiles) ; /* ftp_miles */

	//G145 - Change
	lv_airInfo = lc_itinLine +
					 md_IT_orig_city_id[mi_IT_I] +
					 md_IT_dest_city_id[mi_IT_I] +
					 md_IT_start_date[mi_IT_I] +
					 md_IT_arrival_ind[mi_IT_I];


	LOG(logDEBUG) << "IN IUR : " << lv_airInfo  << " COUNT:" << mi_surfaceTransport << ":"  ;
	mva_surfaceTransport[mi_surfaceTransport++]  = lv_airInfo ;

	// fill stop over city
	AB_Text cities = pt_segment.At(93).For(18);
	fillIrStopovercity(
		md_IT_itin_type[mi_IT_I],
		md_IT_line_number[mi_IT_I],
		cities);

    mi_IT_I++ ;

   // LOG(logDEBUG) << "END CIUR::populateIrTransportSectionA04"  ;
   EXIT;
}

void CIUR::fillIrStopovercity(
			D_service_type	&pd_itinType,
			D_line_number	&pd_itinLine,
			AB_Text	&pc_cities )
{
	AB_Text	cities;
	cities = pc_cities;
	cities.removeTrailingBlanks();
	cities.removeLeadingBlanks();
	int orderNumber;

	orderNumber = 0;

	while( cities.NotIsNull() && cities.length() > 0 )
	{
		orderNumber++;
		md_ISOC_itin_type[mi_ISOC_I] = pd_itinType;
		md_ISOC_line_number[mi_ISOC_I] = pd_itinLine;
		md_ISOC_stop_over_city[mi_ISOC_I] = cities.At(1).For(3);
		md_ISOC_order_number[mi_ISOC_I] = orderNumber;

		cities = cities.At(4);
		cities.removeTrailingBlanks();
		cities.removeLeadingBlanks();
		mi_ISOC_I++;
	}
}

//------------------------------------------------------------------------------

void CIUR::fillConsumDetail(AB_Char &pc_noofSeats,
						 		AB_Char     &pc_itinType,
						 		AB_Char     &pc_itinLine,
						 		AB_Text     &pt_varyAirData,
								AB_Text		&pc_ftpMiles)
{
   AB_Text     lt_text ;
   int         li_noofICD ;

   // Store seat info and meal info along with itin type and itin line in
   // array gd_TransportData. So that later on when we are going to create
   // Irperson we can compare and fill the line

   lt_text = pc_itinType + pc_itinLine + pc_noofSeats + pt_varyAirData ;

   int li_index = 0 ;

   /*
   if ( mi_noofPax  < mi_noofentitle )
      li_noofICD = mi_noofentitle.to_long() ;
   else
      */
   li_noofICD = mi_noofPax.to_long() ;

   // SCR 244859 START

   AB_Text        lt_entitleData;
   AB_Varchar     lv_fareinfo(20);
   int            li_pos;
   D_fare_basis   ld_fare_basis  = AB_null;
   D_char_numeric ld_fare_by_leg = AB_null;
   D_currency     ld_cur_id      = AB_null;

   for ( int li_M4_index = 1; li_M4_index <= mi_entitleIndex && pc_itinType == "A"; li_M4_index++ )
   {
      lt_entitleData = gd_entitleData[li_M4_index];

      if ( lt_entitleData.At(3).For(2) == pc_itinLine )
      {
         lv_fareinfo = lt_entitleData.At(25).For(13);

         li_pos = lv_fareinfo.findSubstring("/");

         if ( li_pos > 0 )
         {
            ld_fare_basis = lv_fareinfo.For(li_pos -1);
         }
         else
         {
            ld_fare_basis = lv_fareinfo ;
         }

         ld_fare_basis.leftJustify() ;
         ld_fare_by_leg = lt_entitleData.At(40).For(8) ;

			// ROW
		   ld_cur_id = lt_entitleData.At(71).For(3) ;

         break;
      }
   }

   // SCR 244859 END


   while ( li_index < li_noofICD )
   {
      md_ICD_itin_type[mi_ICD_I]          = pc_itinType ;
      md_ICD_itin_line_number[mi_ICD_I]   = pc_itinLine ;
      md_ICD_ftp_miles[mi_ICD_I]          = pc_ftpMiles ;
      md_ICD_text[mi_ICD_I]               = lt_text ;

      // SCR 244859 START

      md_ICD_fare_basis[mi_ICD_I]         = ld_fare_basis ;
      md_ICD_fare_by_leg[mi_ICD_I]        = ld_fare_by_leg ;

      // SCR 244859 END

		// ROW
		md_ICD_cur_id[mi_ICD_I] = ld_cur_id ;

		if (pc_itinType == "A" || pc_itinType == "C" || pc_itinType == "H" ) {
			 if (md_ICD_service_status[mi_ICD_I].IsNull()) {
			md_ICD_service_status[mi_ICD_I]  = mv_service_status;
				}
		}

      mi_ICD_I++ ;
      li_index++ ;
   }
}


//------------------------------------------------------------------------------

void CIUR::fill_IrPerson(CPNR &po_PNR)
{
   ENTER;

   AB_Text    lt_section ;
   AB_Text    lt_itin ;
   int	     li_noItin ;
	int		  li_pax_dot;
	int		  li_cur_pax_id;
	AB_Varchar lv_name(50) ;

   po_PNR.resetSegment("M1") ;
   lt_section = po_PNR.getNextSegment("M1");

   while (lt_section.NotIsNull())
   {
		if (( lt_section.At(127).For(1) == "1" && (mc_iurType == "1" || mc_iurType == "B")) ||
				(mc_iurType == "2" ) ||
				(mc_iurType == "3" ) || (mc_iurType == "A" ) || mb_MPT)
		{
      	md_IP_pax_id[mi_IP_I]        = lt_section.At(3).For(2) ;

			if ( mc_iurType == "1" || mc_iurType == "B" || mc_iurType == "2" )
      		md_IP_crs_invoice[mi_IP_I] = md_invoiceNo ;
			else
      		md_IP_crs_invoice[mi_IP_I] = AB_null ;

			md_IP_reference[mi_IP_I] = lt_section.At(69).For(30) ;

			md_IP_reference[mi_IP_I].leftJustify() ;

			if ( md_IP_reference[mi_IP_I] == " " )
				md_IP_reference[mi_IP_I] = AB_null ;

      	lv_name = lt_section.At(5).For(33) ;
      	processPassengerName(lv_name);

         // Process Frequent Flyer from M9 Comparing Name

         processFrequentF(po_PNR,lv_name) ;

         lt_itin = lt_section.getNthField(1,"!") ;
         lt_itin.leftJustify() ;

         if ( lt_itin == " " )
            lt_itin = mv_itinTransport ;

			AB_Char	lc_pax(2) ;
			lc_pax = lt_section.At(3).For(2) ;
			updConsumDetailSeat(lc_pax,lt_itin );


      	mi_IP_I++ ;
		}

      lt_section = po_PNR.getNextSegment("M1");
   }

	// GAP US086 START

	for ( int li_index = 0; li_index < mi_IRMK_I; li_index++ )
	{
		if ( 	md_IRMK_remark[li_index].length() > 0 	&&
				md_IRMK_type[li_index] == "SFM" )
		{
			if ( md_IRMK_remark[li_index].For(5) != "     " )
			{
				li_pax_dot = md_IRMK_remark[li_index].findSubstring(".");
				li_cur_pax_id = md_IRMK_pax_id[li_index].to_long();

				if ( li_pax_dot > 0 && li_cur_pax_id <= mi_IP_I )
				{
					md_IRMK_pax_id[li_index] = md_IP_pax_id[ li_cur_pax_id  - 1 ];
				}else{
					md_IRMK_pax_id[li_index] = md_IP_pax_id[0];
				}
			}

		}
	}
	// GAP US086 END

   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::updConsumDetailSeat(AB_Char  &pc_paxId,
						 		       AB_Text  &pt_itinPax)
{
	int          li_index, li_count;
	AB_Char		 lc_itinLine(2) ;
	AB_Integer	 li_pos ;
	AB_Integer	 li_counter ;
	AB_Integer   li_noofSeats ;
	AB_Text      lt_seatData ;
	AB_Char   	 lc_seatPax(2) ;
	AB_Text      lt_mealData ;
	AB_Char		 lc_prev_itinLine(2) ;
	AB_Integer   li_seatPax ;
	AB_Integer   li_paxId ;
	AB_Integer   li_temp ;
	V_AB_Varchar lv_arrayField(100,100) ;

	li_index = 0 ;
	lc_prev_itinLine = "00" ;
	lc_itinLine = AB_null ;

	while ( li_index < mi_ICD_I )
	{
		li_count = lv_arrayField.parseStringMultDel(md_ICD_text[li_index].At(6),"!") ;

		if (md_ICD_pax_id[li_index].IsNull())
		{
			// Get itin line number
			lc_itinLine = md_ICD_itin_line_number[li_index];
			lc_itinLine.rightJustify('0');

			if (lc_prev_itinLine !=lc_itinLine)
			{
				li_pos = pt_itinPax.findSubstring(lc_itinLine.to_string()) ;
				lc_prev_itinLine = lc_itinLine ;

				// Itinerary found for passenger

				if (li_pos > AB_Integer(0))
				{
					md_ICD_pax_id[li_index] =  pc_paxId ;

					// Get no of seats from  md_ICD_text

					li_noofSeats = md_ICD_text[li_index].At(4).For(2) ;

					li_counter = AB_Integer(1) ;

					// li_temp = md_ICD_pax_id[li_index] ;
					// lt_seatData = lv_arrayField[li_temp] ;

					li_temp = md_ICD_pax_id[li_index] + li_noofSeats + AB_Integer(1) ;
   				lt_mealData = md_ICD_text[li_index].At(6).getNthField( li_temp.to_long(), "!") ;

					while ( li_counter <= li_noofSeats && li_counter.to_long() <= li_count)
					{
						lt_seatData = lv_arrayField[li_counter] ;
						if (lt_seatData.NotIsNull())
						{
							lc_seatPax = lt_seatData.For(2) ;

							li_paxId = pc_paxId ;
							li_seatPax = lc_seatPax ;
							if ( li_seatPax == li_paxId )
							{
								if ( lt_seatData.At(3).For(1) == " ")
									md_ICD_seat[li_index] = lt_seatData.At(4).For(3) ;
								break ;
							}
						}
						else
							md_ICD_seat[li_index] = AB_null ;

						li_counter = li_counter + AB_Integer(1) ;
					}
				}
			}
		}
		li_index++ ;
	}
}

//------------------------------------------------------------------------------

void CIUR::processTicket(CPNR &po_PNR)
{
	if ( mc_iurType != "3" )
	{
		// QREX START
		if ( mb_refundPNR )
		{
			fillRefundFormM5(po_PNR);
		}
		else
		{
		// QREX END

			mi_tktCounter = 1 ;

			fillTicketFormM2(po_PNR) ;
			fillTicketFormM5(po_PNR) ;

		// QREX START
		}
		//QREX END
	}

	// SCR 231339 START

	updateTransportSupplier();

	// SCR 231339 END
}

//---------------------------------------------------------------------------
// QREX START

void CIUR::fillRefundFormM5(CPNR &po_PNR) {
	ENTER;

   AB_Text          	lt_section ;
   AB_Integer        li_arraySize;
	AB_Varchar			lv_tktString(255) ;
	V_AB_Varchar   	lv_arrayField(100,100) ;
	int					li_tax1Ind,
							li_tax2Ind,
							li_tax3Ind,
							li_OPAind;

   po_PNR.resetSegment("M5") ;
   lt_section = po_PNR.getNextSegment("M5");

	while ( lt_section.NotIsNull() )
	{
		// SCR 398530 start: process sfc for refund ticket
		lv_tktString                 = lt_section.At(12) ;

		li_arraySize = lv_arrayField.parseStringMultDel(lv_tktString,"/") ;

		li_tax1Ind = 4;
		if (scanTaxesAndOPAfields(lv_arrayField, li_tax1Ind, li_tax2Ind, li_tax3Ind, li_OPAind) == AB_true )
		{
			if ( 	lt_section.At(8).For(1) 	== "R" 	&&
					lt_section.At(7).For(1) 	== " " 	&&
			      (lv_arrayField[li_OPAind+4].For(1)	== "F"
					 || lv_arrayField[li_OPAind+4].For(1)	== "P"))
			{
				md_ID_doc_type[mi_ID_I]    = "BSP" ;

				if ( lt_section.At(5).For(2).isAllNumeric())
				{
					md_ID_pax_id[mi_ID_I] = lt_section.At(5).For(2) ;
				}
				else
				{
					md_ID_pax_id[mi_ID_I] = md_IP_pax_id[0] ;
				}

				md_ID_line_number[mi_ID_I] = mi_ID_I + 1;
				md_ID_reference[mi_ID_I]   = md_IRT_reference ;
				md_ID_sup_id[mi_ID_I]      = lt_section.At(9).For(2) ;

				md_ID_saving_code[mi_ID_I]     = md_savingCode ;
				md_ID_full_fare[mi_ID_I]       = md_fullFare ;
				md_ID_low_prac_fare[mi_ID_I]   = md_lowPracFare ;
				md_ID_rebate[mi_ID_I]          = md_rebate ;

				parseM5ticket(	lv_tktString,
									lv_arrayField,
									li_arraySize,
									md_ID_pax_id[mi_ID_I],
									md_ID_line_number[mi_ID_I]) ;

				md_ID_form[mi_ID_I]        = lv_arrayField[1].For(4) ;
				md_ID_ticket_no[mi_ID_I]   = lv_arrayField[1].At(5).For(6) ;
				md_ID_refund[mi_ID_I]		= lv_arrayField[li_OPAind+4].For(1); // "F" or "P"
				mi_ID_I++ ;
			}

		}

   	lt_section = po_PNR.getNextSegment("M5");
	}
}
// QREX END
//---------------------------------------------------------------------------


void CIUR::fillTicketFormM2(CPNR &po_PNR)
{
	ENTER;
   int                li_counter,
							 li_pax_id,
							 li_pos,
							 li_taxCounter ;
   AB_Text            lt_section ;
   int                li_M1_index ;
	char					 lc_sign ;
	AB_Varchar			 lv_taxString(30) ;
	AB_Integer			 li_noofM4  ;
	AB_Text       		 lt_entitleNos  ;
	AB_Text       		 lt_fareCalcNos  ;
	AB_Char       		 lc_firstFareCalcNo(2) ;
	AB_Char        	 lc_pax(2)  ;
	AB_Char        	 lc_docId(2)  ;
	AB_Char        	 lc_docPic(3) ;
	D_amount        	 ld_totTax ;
	int					 li_tktCounter = 1 ;
	AB_Varchar			 lv_exchange_info(20) ;
	D_amount           ld_amount ;
	D_amount           ld_commission ;
	AB_Integer         li_num_books ;
	AB_Boolean			 lb_found ;
	D_amount           ld_totCalcFare ;
	D_amount           ld_totM2Fare ;
	D_amount           ld_diffInTax ;
	AB_Char         	 lc_IU2APZ(1) ;
	D_amount        	 ld_IU2PEN;
   AB_Varchar         lc_cur_id(3); //FG - US446

   po_PNR.resetSegment("M2") ;
   lt_section = po_PNR.getNextSegment("M2");
	mi_tktIndex  = 0;

	while ( lt_section.NotIsNull() )
	{
		AB_Char lc_IU2IND;

		lc_IU2IND = lt_section.At(21).For(1);

		/**
		 * IU2IND Ticket indicator
		 *
		 * Blank = Normal Ticket
       * "1"   = AMTRAK Ticket
       * "2"   = Electronic Ticket
       * "3"   = Non-ARC Electronic Ticket
		 **/

		AB_Text tktNo = lt_section.At(8).For(10);
		AB_Text expDate = lt_section.At(222).For(4);
		if ( expDate.isAllNumeric() )
		{
				M02_TktNo2ExpDate.insert(Text2Text(tktNo, expDate));
		}

		set <Text2Text>::iterator it;
		for( it = M02_TktNo2ExpDate.begin(); it != M02_TktNo2ExpDate.end(); it++ )
		{
			  LOG(logDEBUG) << it->_key << "\t" << it->_value ;
		}

		if (lc_IU2IND == "3" )
		{
			/*
			it = M02_TktNo2ExpDate.find(Text2Text("12345", ""));
			if ( it != M02_TktNo2ExpDate.end() )
			LOG(logDEBUG) << "FOUND:" << it->_key << "\t" << it->_value ;
			else
			LOG(logDEBUG) << "NOT FOUND:"  ;
			*/

		}
		else  //if (lc_IU2IND != "3" )
		{
         if ( lc_IU2IND == "2" )
			{
				 /**
				  * It is an electronic ticket
				  **/

     		    md_ID_e_ticket[mi_ID_I] = "Y";
			}
			else
			{
     		    md_ID_e_ticket[mi_ID_I] = "N";
			}

			lv_exchange_info      = AB_null ;
     		md_ID_pax_id[mi_ID_I] = lt_section.At(3).For(2) ;

			AB_Boolean  lb_tktPaxId;
			lb_tktPaxId = AB_false;

			int i;

			i = 0;

			while ( i <= mi_IP_I && !lb_tktPaxId )
		   {
			     if ( md_ID_pax_id[mi_ID_I]==md_IP_pax_id[i] )
				       lb_tktPaxId = AB_true;

				  i++;
			}

			if( !lb_tktPaxId )
			{
			    CMsgHandler::setMsg(3487, " ", "TKT-PARSE",
											1, AB_Varchar(20,AB_null));
      	    md_rejectCop = "10" ;
      	    md_rejectBra = "1101" ;
      	    md_rejectCon = AB_null ;

			    return;
			}

     		md_ID_line_number[mi_ID_I] = mi_tktCounter++ ;
     		md_ID_pic[mi_ID_I]         = lt_section.At(5).For(3) ;
     		md_ID_reference[mi_ID_I]   = md_IRT_reference ;
     		md_ID_doc_type[mi_ID_I]    = "BSP" ;

			lv_exchange_info = lt_section.getNthField(5,"!")  ;

			if (lv_exchange_info == " ")
				lv_exchange_info = AB_null ;

			LOG(logDEBUG)  << "DK >>> lv_exchange_info = '" << lv_exchange_info << "'"  ;

			if (lv_exchange_info.NotIsNull())
			{
				md_ID_orig_ticket_no[mi_ID_I] = lv_exchange_info ;

				li_pos = md_ID_orig_ticket_no[mi_ID_I].findSubstring("/") ;

				if (li_pos > 0)
					md_ID_orig_ticket_no[mi_ID_I] = md_ID_orig_ticket_no[mi_ID_I].For(li_pos - 1) ;

			}

			md_ID_crs_tour[mi_ID_I]		= lt_section.At(165).For(15) ;
			md_ID_sup_id[mi_ID_I]      = lt_section.At(232).For(2) ;

			if (lt_section.At(234).For(10) == "          " )
			{
				CMsgHandler::setMsg(3009,
										  " ",
										  "TKT-PARSE",
										  1,
										  AB_Varchar(20,AB_null));
      		md_rejectCop = "10" ;
      		md_rejectBra = "1101" ;
      		md_rejectCon = AB_null ;

				return;
			}

			md_ID_form[mi_ID_I]        = lt_section.At(234).For(4) ;
  			md_ID_ticket_no[mi_ID_I]   = lt_section.At(238).For(6) ;
  			md_ID_no_of_books[mi_ID_I] = lt_section.At(244).For(1) ;
			li_num_books               = md_ID_no_of_books[mi_ID_I] ;

			li_num_books = li_num_books + AB_Integer(1) ;
			md_ID_no_of_books[mi_ID_I] = li_num_books ;

			lc_docId = md_ID_line_number[mi_ID_I] ;
			lc_docId.rightJustify('0') ;
			lc_pax = lt_section.At(3).For(2) ;

			// Look for base cost

			if (lt_section.At(117).For(1) == " ")
			{
				// crs currency ticket

				md_ID_cur_id[mi_ID_I]    = lt_section.At(35).For(3) ;

				if (lt_section.At(34).For(1) == " " )
					md_ID_base_cost[mi_ID_I]  = lt_section.At(38).For(8) ;
				else
					md_ID_base_cost[mi_ID_I]  = lt_section.At(34).For(1) +
													 	 lt_section.At(38).For(8) ;

				// infant ticket currency
            if (lt_section.At(5).For(3) == "INF" )
            {
               if (lt_section.At(77).For(3) != lt_section.At(35).For(3))
                  md_ID_cur_id[mi_ID_I]  = lt_section.At(77).For(3) ;
            }

            lc_cur_id = md_ID_cur_id[mi_ID_I]; //FG - US446
			}
			else
			{
				// local currency ticket

            //FG - US446 - Start
            if (lt_section.At(117).For(3) != lt_section.At(35).For(3))
            {
               lc_cur_id = lt_section.At(35).For(3) ;
            }
            else
            {
               lc_cur_id = lt_section.At(117).For(3) ;
            }
            //FG - US446 - End

				md_ID_cur_id[mi_ID_I]    = lt_section.At(117).For(3) ;

				if (lt_section.At(116).For(1) == " " )
					md_ID_base_cost[mi_ID_I]  = lt_section.At(120).For(8) ;
				else
					md_ID_base_cost[mi_ID_I]  = lt_section.At(116).For(1) +
														 lt_section.At(120).For(8) ;
			}

			// SCR 525408
			LOG(logDEBUG) << "md_base_cost= "      << md_ID_base_cost[mi_ID_I]     ;
			if ( !PCRE::pcre_match("^\\s*[\\d\\.]+\\s*$" , md_ID_base_cost[mi_ID_I].to_string() ))
				md_ID_base_cost[mi_ID_I] = "0.00";
			LOG(logDEBUG) << "md_base_cost= "      << md_ID_base_cost[mi_ID_I]     ;

			if (md_ID_cur_id[mi_ID_I] == "   ")
				md_ID_cur_id[mi_ID_I] = "USD" ;

			md_ID_comm_rate[mi_ID_I]  = lt_section.At(128).For(8) ;
			md_ID_comm_rate[mi_ID_I].leftJustify() ;
			ld_commission = md_ID_comm_rate[mi_ID_I] ;
			md_ID_comm_rate[mi_ID_I] = ld_commission ;


         lc_IU2APZ = lt_section.At(33).For(1);

         if ( lc_IU2APZ == "B" || lc_IU2APZ == "I" )
         {
				 mb_netFareTicket	= AB_true;
         }

			li_pos       = md_ID_comm_rate[mi_ID_I].findSubstring(" ") ;

			if (lt_section.At(136).For(1) == " " )
         {
				md_ID_commission[mi_ID_I]  = lt_section.At(137).For(8) ;
         }
			else
         {
				md_ID_commission[mi_ID_I]  = lt_section.At(136).For(1) +
													  lt_section.At(137).For(8) ;
         }

			// SCR 512224: if comm_amount populated, ignore comm_rate(see SABRE spec)
			if ( md_ID_commission[mi_ID_I] != "        " )
			{
				md_ID_comm_rate[mi_ID_I]  = "        " ;
			}

			ld_commission = md_ID_commission[mi_ID_I] ;
			md_ID_commission[mi_ID_I] = ld_commission ;

			/**
			 * Check if matching M5 exists
			 **/

      	lb_found = existMatchingM5(lc_pax,lc_docId) ;

			if ( lb_found )
			{
			   /**
			    * Get form of pay from M2
             **/

			   fillFOPM2( md_ID_pax_id[mi_ID_I],
						     md_ID_line_number[mi_ID_I],
						     lt_section) ;

				// Needed for XF Tax Calculation

				lt_fareCalcNos = lt_section.getNthField(2,"!") ;
				lc_firstFareCalcNo = lt_fareCalcNos.For(2) ;

				lv_taxString   = lt_section.At(46).For(30) ;

				processTax(po_PNR,lv_taxString,ld_totTax,lc_firstFareCalcNo) ;

				ld_totCalcFare = ld_totTax + D_amount(md_ID_base_cost[mi_ID_I]) ;

				// 76 - sign 77,78,79 - Currency, 80-87 - amount

				ld_totM2Fare = D_amount(lt_section.At(80).For(8) ) ;

				LOG(logDEBUG) << "AM===ROW=== ld_totM2Fare =[" << ld_totM2Fare << "]"  ;
				if ( ld_totCalcFare != ld_totM2Fare && ! mb_netFareTicket )
            {
               ld_diffInTax = ld_totM2Fare - ld_totCalcFare ;

					LOG(logDEBUG) << "AM===ROW=== ld_diffInTax =[" << ld_diffInTax << "]"  ;
               if ( ld_diffInTax < D_amount(0) )
               {
                  CMsgHandler::setMsg(3461, " ",
												  "TKT-PARSE", 1,
												  AB_Varchar(20,AB_null));
                  return ;
               }
               else
               {
                  // Add it as ZZ tax

                  md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I];
                  md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I];
                  md_ITD_tax_id[mi_ITD_I]       = "ZZ";
                  md_ITD_tax_source[mi_ITD_I]   = "M2";
                  md_ITD_tax_amount[mi_ITD_I++] = ld_diffInTax;

                  ld_totTax = ld_totTax + ld_diffInTax;
               }
            }

				if (lv_exchange_info.NotIsNull())
				{
					// Create Irremark for Exchange Cost line.

					ld_IU2PEN = D_amount(lt_section.At(91).For(8) ) ;

					processOrigFare(lc_pax,lc_docId,po_PNR,ld_totTax, ld_IU2PEN) ;

               /* to be removed

					if ( md_ID_exchange_code[mi_ID_I] == "E" )
					{
					    **
						 * If it is even exchange, we should use original
					    * FOP from M2 SCR# 2468
						 **

					   getOrigFop(lt_section);

					    **
					 	 * Replace by original form of payment
						 **

	               md_IRP_form_of_pay[mi_IRP_I-1] = md_orig_form_of_pay;
	               md_IRP_cc_id[mi_IRP_I-1]       = md_orig_cc_id;
	               md_IRP_cc_no[mi_IRP_I-1]       = md_orig_cc_no;
		            md_IRP_cc_exp_date[mi_IRP_I-1] = md_orig_cc_exp_date;
		            md_IRP_auth_no[mi_IRP_I-1]     = md_orig_auth_no;
		            md_IRP_auth_source[mi_IRP_I-1] = md_orig_auth_source;
					}
					*/
				}

				// Assign Fare code to all the tickets

				md_ID_saving_code[mi_ID_I]     = md_savingCode ;
				md_ID_full_fare[mi_ID_I]       = md_fullFare ;
				md_ID_low_prac_fare[mi_ID_I]   = md_lowPracFare ;
				md_ID_rebate[mi_ID_I]          = md_rebate ;

				li_noofM4 = lt_section.At(220).For(2) ;
				lt_entitleNos = lt_section.getNthField(1,"!") ;
				lc_docPic = lt_section.At(5).For(3) ;

				updIrconsumdetailTkt(li_noofM4,
										lt_entitleNos,
										lc_pax ,
										lc_docId,
										lc_docPic,
                              lc_cur_id) ; //FG - US446

				md_ID_form[mi_ID_I].removeTrailingBlanks() ;

				md_tktData[++ mi_tktIndex] =
				md_ID_form[mi_ID_I] + md_ID_ticket_no[mi_ID_I] ;

				mi_ID_I++ ;
			}
			else
			{
				initTicket(mi_ID_I) ;
				mi_tktCounter-- ;
			}
		}
   	lt_section = po_PNR.getNextSegment("M2");
	}
   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::processOrigFare(AB_Char   	&pc_paxId,
								   AB_Char     &pc_docLine,
								   CPNR        &po_PNR,
								   D_amount    &pd_totTax,
								   D_amount    &pd_IU2PEN)

{
	ENTER;

	AB_Text      lt_section ;
	AB_Char      lc_baseCost(12) ;
	D_amount     ld_baseCost ;

	AB_Char      lc_commission(9) ;
	D_amount     ld_commission ;
	D_amount     ld_commRate ;

	AB_Varchar   lv_remark(80) ;
	D_amount     ld_totTax ;
	AB_Char      lc_tax(8) ;
	AB_Char      lc_M5Form(4) ;
	AB_Char      lc_M5Tkt(6) ;

	AB_Varchar   lv_taxString(10) ;
	int          i,li_pos,li_index ;

	AB_Char      lc_exchangeType ;
	AB_Boolean   lb_penalty ;

	lv_remark = AB_null ;
	li_index  = 1;
	lb_penalty = AB_false ;

   while ( li_index < mi_M5AccIndex )
	{
		lt_section = mva_M5AccSection[li_index] ;
		if (lt_section.At(5).For(2) == pc_paxId )
		{
			V_AB_Varchar lv_arrayField(100,100) ;

			i = lv_arrayField.parseStringMultDel(lt_section.At(12),"/") ;

			lc_M5Form        = lv_arrayField[1].For(4) ;
			lc_M5Tkt         = lv_arrayField[1].At(5).For(6) ;

			if ( lc_M5Form == md_ID_form[mi_ID_I].For(4) &&
				  lc_M5Tkt == md_ID_ticket_no[mi_ID_I].For(6) )
			{

				ld_commRate = AB_null ;
				ld_commission = AB_null ;

				lv_arrayField[2].leftJustify() ;
				lc_commission  = lv_arrayField[2] ;

				li_pos       = lc_commission.findSubstring("P") ;

				if (li_pos > 0)
				{
					ld_commRate   = lc_commission.At(li_pos + 1 ) ;
				}
				else
				{
					ld_commission   = lc_commission ;
				}

				lc_baseCost = lv_arrayField[3] ;

				if ( lc_baseCost.For(1).isAllAlpha())
				{
					// Remove Currency

					lc_baseCost = lc_baseCost.At(4) ;
				}
				ld_baseCost = lc_baseCost ;

				// Determine exchange type E - Even, A - Adcoll, R - Refund

				lc_exchangeType = lt_section.At(8).For(1) ;

				if ( lc_exchangeType == " " )
					lc_exchangeType = "A" ;

				// If M2 Adcol exchange ticket comes and M2 base fare is less than
				//	M5 base fare then make it an even exchange and M5 amount put as
				//	penalty and comm_penalty

				if ( lc_exchangeType == "A" &&
					  D_amount(md_ID_base_cost[mi_ID_I]) < ld_baseCost )
				{
					lc_exchangeType = "E" ;
					lb_penalty = AB_true ;
				}

				if ( lc_exchangeType == "E" )
				{
					if ( lb_penalty )  //M2 amount is less than M5 and it is a adcol
					{
						if ( ld_commRate.NotIsNull() )
						{
							D_amount     ld_comm_penalty ;

							ld_comm_penalty =
									(ld_baseCost * ld_commRate ) / D_amount(100) ;
							md_ID_comm_penalty[mi_ID_I] = ld_comm_penalty ;
						}
						else
						{
							md_ID_comm_penalty[mi_ID_I] = ld_commission ;
						}

						// Process Taxes

						getTotM5Taxes(lv_arrayField,ld_totTax ) ;

						ld_baseCost = ld_baseCost + ld_totTax ;
						md_ID_penalty_ch_fee[mi_ID_I] = ld_baseCost ;

					}

					// Now process refund cost line for even exchange

					md_ID_exchange_code[mi_ID_I] = "E" ;
					ld_baseCost = md_ID_base_cost[mi_ID_I] ;

					lc_baseCost = ld_baseCost ;
					lc_baseCost.rightJustify('0') ;

					if (md_ID_commission[mi_ID_I] == "        " )
					{
						lc_commission =  md_ID_comm_rate[mi_ID_I] ;
						if ( lc_commission.NotIsNull() )
							lc_commission.rightJustify(' ') ;
						else
							lc_commission = "000000.00" ;

						lc_commission = "P" + lc_commission.At(2) ;
					}
					else
					{
						lc_commission =  md_ID_commission[mi_ID_I] ;
						if ( lc_commission.NotIsNull() )
							lc_commission.rightJustify(' ') ;
						else
							lc_commission = "000000.00" ;
					}

					lc_tax = pd_totTax ;
					lc_tax.rightJustify('0') ;

					lv_remark =  pc_docLine + lc_baseCost + lc_commission +
									 "T1:" + lc_tax + "XT" ;
					if ( lb_penalty &&  pd_IU2PEN.NotIsNull() )
					{
						lv_remark = lv_remark + "M2PENALTY:" + pd_IU2PEN.to_string() ;
					}
				}
				else if ( lc_exchangeType == "A" )
				{
					md_ID_exchange_code[mi_ID_I] = "A" ;

      			if (md_ID_commission[mi_ID_I] == "        " )
      			{
						if ( ld_commission == AB_null )
						{
							ld_commission = (ld_commRate * D_amount(100) ) / ld_baseCost  ;

						}

						D_amount    ld_newComm ;
						ld_newComm  = md_ID_comm_rate[mi_ID_I] ;
						ld_newComm = ( ld_newComm * D_amount(100) ) / D_amount(md_ID_base_cost[mi_ID_I]) ;
						ld_commission = ld_newComm - ld_commission ;

         			lc_commission =  ld_commission  ;

						// commission could be negative, so don't rightjustify it with '0'
						if ( lc_commission.NotIsNull() )
							lc_commission.rightJustify(' ') ;
						else
							lc_commission = "000000.00" ;
      			}
      			else
      			{
         			if ( ld_commission == AB_null )
         			{
            			ld_commission = (ld_commRate * D_amount(100) ) / ld_baseCost  ;

         			}

						ld_commission = D_amount(md_ID_commission[mi_ID_I]) - ld_commission  ;

         			lc_commission =  ld_commission ;
						if ( lc_commission.NotIsNull() )
							lc_commission.rightJustify(' ') ;
						else
							lc_commission = "000000.00" ;
      			}

					ld_baseCost = D_amount(md_ID_base_cost[mi_ID_I]) - ld_baseCost ;
					lc_baseCost = ld_baseCost ;
					lc_baseCost.rightJustify('0') ;

					// Process Taxes
					getTotM5Taxes(lv_arrayField,ld_totTax ) ;

					ld_totTax = pd_totTax - ld_totTax ;

					if ( ld_totTax < D_amount(0) )
					{
						ld_baseCost = ld_baseCost - ld_totTax ;
						lc_baseCost = ld_baseCost ;
						lc_baseCost.rightJustify('0') ;
						lc_tax = " " ;
					}
					else
					{
						lc_tax = ld_totTax ;
						lc_tax.rightJustify('0') ;
					}

					lv_remark = pc_docLine + lc_baseCost + lc_commission + "T1:" + lc_tax + "XT" ;
				}
				else if ( lc_exchangeType == "G" )
				{
					md_ID_exchange_code[mi_ID_I] = "G" ;

      			if (md_ID_commission[mi_ID_I] == "        " )
      			{
						if ( ld_commission == AB_null )
						{
							ld_commission = (ld_commRate * D_amount(100) ) / ld_baseCost  ;

						}

						D_amount    ld_newComm ;
						ld_newComm  = md_ID_comm_rate[mi_ID_I] ;
						ld_newComm = ( ld_newComm * D_amount(100) ) / D_amount(md_ID_base_cost[mi_ID_I]) ;
						ld_commission = ld_newComm + ld_commission ;

         			lc_commission =  ld_commission  ;
						if ( lc_commission.NotIsNull() )
							lc_commission.rightJustify(' ') ;
						else
							lc_commission = "000000.00" ;
      			}
      			else
      			{
         			if ( ld_commission == AB_null )
         			{
            			ld_commission = (ld_commRate * D_amount(100) ) / ld_baseCost  ;

         			}

						ld_commission = D_amount(md_ID_commission[mi_ID_I]) + ld_commission  ;

         			lc_commission =  ld_commission ;
						if ( lc_commission.NotIsNull() )
							lc_commission.rightJustify(' ') ;
						else
							lc_commission = "000000.00" ;
      			}

					ld_baseCost = D_amount(md_ID_base_cost[mi_ID_I]) + ld_baseCost ;
					lc_baseCost = ld_baseCost ;
					lc_baseCost.rightJustify('0') ;

					// Process Taxes
					getTotM5Taxes(lv_arrayField,ld_totTax ) ;

					ld_totTax = pd_totTax + ld_totTax ;

					lc_tax = ld_totTax ;
					lc_tax.rightJustify('0') ;

					lv_remark = pc_docLine + lc_baseCost + lc_commission + "T1:" + lc_tax + "XT" ;
				}
				break ;
			}
		}
		li_index++ ;
	}

   if ( lv_remark.NotIsNull() )
   {
      md_IRMK_type[mi_IRMK_I]           = "COS" ;
      md_IRMK_remark[mi_IRMK_I]         = lv_remark ;
      md_IRMK_pax_id[mi_IRMK_I++]       = md_ID_pax_id[mi_ID_I] ;

		if ( pd_IU2PEN.NotIsNull() && md_ID_penalty_ch_fee[mi_ID_I] .IsNull() )
		{
			md_ID_penalty_ch_fee[mi_ID_I] = pd_IU2PEN;
			createRemark("CHP", pd_IU2PEN.to_string() );
		}
   }
   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::processTax(CPNR       &po_PNR ,
							AB_Varchar	&pv_taxString,
							D_amount    &pd_totTax,
							AB_Char     &pc_fareCalcNo)
{
	ENTER;

	int	      li_ctr,
				   li_pos,
					li_firstDigitPos,
				   li_index;
	AB_Char	   lc_taxCode(2) ;
	AB_Boolean	lb_dup ;
	AB_Decimal  lm_tax1(13,2);
	AB_Decimal  lm_tax2(13,2);
	AB_Decimal  lm_XTtax(13,2);
	AB_Decimal  lm_XFtax(13,2);
	AB_Varchar  lv_rawAmount(12);
	AB_Varchar  lv_amount(12);

// SCR #117474 START
   D_amount    ld_totXFTaxes ;
// SCR #117474 END


	li_ctr = 1 ;
	li_pos = 1 ;
	lb_dup = AB_false ;
	pd_totTax = D_amount(0) ;
	lm_XTtax = D_amount(0) ;
	lm_XFtax = D_amount(0) ;

	while ( li_ctr < 4 )
	{
		lc_taxCode = pv_taxString.At(li_pos + 8).For(2) ;
		lv_rawAmount = pv_taxString.At(li_pos).For(8) ;
		li_firstDigitPos = lv_rawAmount.findFirstCharNotInList(" ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		// in ROW, files taxes could start with P or PD, so skip it
		lv_amount = lv_rawAmount.At(li_firstDigitPos);

		LOG(logDEBUG) << "AM===ROW=== li_firstDigitPos =[" << li_firstDigitPos << "]"  ;
		LOG(logDEBUG) << "AM===ROW=== lv_rawAmount =[" << lv_rawAmount << "] lv_amount=[" << lv_amount << "]"  ;


		if ( lc_taxCode != "  " )
		{
			li_index = 1 ;
			if (li_ctr > 1 )
			{
				lb_dup = AB_false ;
				while ( li_index < li_ctr)
				{
					if ( lc_taxCode ==  md_ITD_tax_id[mi_ITD_I - li_index] )
					{
						if ( md_ID_line_number[mi_ID_I] == md_ITD_line_number[mi_ITD_I - li_index] )
						{
							lm_tax1 = md_ITD_tax_amount[mi_ITD_I - li_index] ;

							lm_tax2 = lv_amount;
							pd_totTax = pd_totTax + D_amount(lm_tax2) ;

                     md_ITD_tax_source[mi_ITD_I - li_index]   = "M2";
							md_ITD_tax_amount[mi_ITD_I - li_index] = lm_tax1 + lm_tax2 ;
							lb_dup = AB_true ;
						}
					}
					li_index++ ;
				}
			}
			if ( !lb_dup )
			{
				if ( lc_taxCode == "XF" )
				{
				   lm_tax1 = lv_amount; //pv_taxString.At(li_pos).For(8) ;
					lm_XFtax = lm_XFtax +  lm_tax1 ;
				}
				else if ( lc_taxCode == "XT" )
				{
					lm_tax1 = lv_amount; //pv_taxString.At(li_pos).For(8) ;
					lm_XTtax = lm_XTtax +  lm_tax1 ;

				}
				else
				{
					md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
					md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
					md_ITD_tax_id[mi_ITD_I]       = lc_taxCode ;

					lm_tax1 = lv_amount;
               md_ITD_tax_source[mi_ITD_I]   = "M2";
					md_ITD_tax_amount[mi_ITD_I]   = lv_amount; //pv_taxString.At(li_pos).For(8);


					md_ITD_tax_amount[mi_ITD_I++].removeLeadAndTrailBlanks();
					pd_totTax = pd_totTax + D_amount(lv_amount); //pv_taxString.At(li_pos).For(8)) ;
				}

			}
			else
			{
				LOG(logDEBUG) << "Got A duplicate TAx"  ;
				//break ;
			}

		}

		li_pos = li_pos + 10 ;
		li_ctr++ ;

	// condense_IrDocumentTax(md_ID_pax_id[mi_ID_I], li_ctr) ;
	}


	if ( lm_XFtax > D_amount(0)  || lm_XTtax > D_amount(0) )
	{
		/**
	    *	We have XF or XT in M2, let's attempt to breakdown XF
		 *
		 * If XF not in M2, it might be available in M6
		 **/

// SCR #117474 START
      D_amount ld_totalXFXTInM2;

      ld_totalXFXTInM2 = lm_XFtax + lm_XTtax;

      getXFbreakdown(po_PNR,ld_totalXFXTInM2,ld_totXFTaxes,pc_fareCalcNo) ;

      pd_totTax = pd_totTax + ld_totXFTaxes ;

      if ( ld_totXFTaxes == D_amount(0) )
      {
         /**
          * No break down happening
          **/

         if ( lm_XFtax > D_amount(0) )
         {
            /**
             * Use available XF in M2
             **/

            md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
            md_ITD_tax_id[mi_ITD_I]       = "XF" ;
            md_ITD_tax_source[mi_ITD_I]   = "M2";
            md_ITD_tax_amount[mi_ITD_I++] = lm_XFtax ;
            pd_totTax = pd_totTax + lm_XFtax ;
         }
      }
      else
      {
         /**
          * XF is broken down,
          * multiple XF Irtaxdetail already created
          **/

         if ( lm_XTtax  > D_amount(0) && lm_XFtax == D_amount(0) )
         {
            /**
             * ld_totXFTaxes is the XF hidden in XT
             *
             * XT in M2 should be adjusted by taking away XF part
             **/

            lm_XTtax = lm_XTtax - ld_totXFTaxes ;
         }
      }
// SCR #117474 END

		if ( lm_XTtax > D_amount(0) )
		{
			/**
			 * Create XT Tax
			 **/

			md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
			md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
			md_ITD_tax_id[mi_ITD_I]       = "XT" ;
         md_ITD_tax_source[mi_ITD_I]   = "M2";
			md_ITD_tax_amount[mi_ITD_I++] = lm_XTtax ;
			pd_totTax                     = pd_totTax + lm_XTtax ;
		}
	}
   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::getXFbreakdown( CPNR        &po_PNR,           // Input
// SCR #117474 START
                           D_amount    &pd_totalXFXTInM2, // Input
                           D_amount    &pd_totXFTaxes,    // Output
// SCR #117474 END
									AB_Char     &pc_fareCalcNo)    // Input
{
	ENTER;

// SCR #117474 START

   AB_Text      lt_section ;
   AB_Varchar   lv_XFTaxes(32);
   int          li_XFIndex,li_index, li_XFTaxCount, li_endInd, li_currCodeCounter, li_backwardInd, li_currInd ;
   D_amount     ld_XFTax ;
   AB_Integer   i ;
   AB_Char      lc_M6No(2) ;
	AB_Boolean   lb_currCodeFound;

   li_index = 1 ;
   lv_XFTaxes = AB_null ;
   li_XFTaxCount = 0;
	li_currCodeCounter = 0;
	lb_currCodeFound = AB_FALSE;
	li_currInd = 0;
   //po_PNR.resetSegment("M6") ;
   //lt_section = po_PNR.getNextSegment("M6") ;


   pd_totXFTaxes = D_amount(0) ;

// SCR #117474 END

	i = AB_Integer(1) ;

   while ( li_index < mi_M6FareIndex )
	{
		lt_section = mva_M6FareSection[li_index] ;

		// ROW-G9142 START, ignore this M6 if currency in this M6 is different from M2
		li_endInd = lt_section.findSubstring("END" ) ;
		// search backwards to find 3 char curr code
		for ( li_backwardInd = li_endInd-1; li_backwardInd > 0 && lb_currCodeFound == AB_false; li_backwardInd--)
		{
		  if ( lt_section.At(li_backwardInd).For(1).isAllAlpha() )
		  {
			 li_currCodeCounter++;
		  }
		  if ( li_currCodeCounter == 3 )
		  {
			 lb_currCodeFound = AB_true;
			 li_currInd = li_backwardInd;
		  }
		}

		LOG(logDEBUG) << "AM==== ROW ==== curr in M6 =[" << lt_section.At(li_currInd).For(3) << "]"  ;
		LOG(logDEBUG) << "AM==== ROW ==== curr in M2 =[" << md_ID_cur_id[mi_ID_I] << "]"  ;
		// if no curr or it is not ticket curr skip this M6
		if ( lb_currCodeFound == AB_false || lt_section.At(li_currInd).For(3) != md_ID_cur_id[mi_ID_I])
		{
			LOG(logDEBUG) << "AM==== ROW ==== SKIPPING THIS M6 because of currency"  ;
			li_index++ ;
			continue;
		}
		// ROW-G9142 END


		lc_M6No = i ;
		lc_M6No.rightJustify('0') ;

		if ( lc_M6No == pc_fareCalcNo )
		{
			li_XFIndex = lt_section.findSubstring("XF" ) ;

			//**************************************************************
			//1400- 22OCT98 - SC -- for more than 1 XF, pick up the last one
			//**************************************************************
			int li_temp = li_XFIndex;
			int li_tempIndex = li_temp;

			while ( li_tempIndex > 0 )
			{
			   li_tempIndex = lt_section.At(li_temp+2).findSubstring("XF" ) ;
				if( li_tempIndex > 0 )
				{
			     li_XFIndex = li_tempIndex + li_temp + 1 ;
			     li_temp = li_XFIndex;
				}
         }
			//**************************************************************

			if ( li_XFIndex > 0 )
			{
				lv_XFTaxes = lt_section.At(li_XFIndex + 2) ;

				li_XFIndex = lv_XFTaxes.findSubstring(" " ) ;
				if ( li_XFIndex > 0 )
				{
					lv_XFTaxes = lv_XFTaxes.For(li_XFIndex - 1) ;
				}

				li_XFIndex = lv_XFTaxes.findSubstring("!" ) ;
				if ( li_XFIndex > 0 )
				{
					lv_XFTaxes = lv_XFTaxes.For(li_XFIndex - 1) ;
				}
	 		}

			LOG(logDEBUG) << "TOTAL XF TAXES IN M6" << lv_XFTaxes ;

         if ( lv_XFTaxes.NotIsNull() )
         {
			   /**
			    * We have XF break down info, before the actual XF creation,
				 * we must make sure that the break down total is less than
				 * XF XT total in M2
			    **/

// SCR #117474 START

            D_amount  ld_totXFTaxes(0);
            AB_Varchar  lv_listOfXF(32);
            AB_Varchar  lv_amount(10);
            AB_Integer  li_nextCityCode   =  1;
            AB_Boolean  lb_dataOK         =  AB_true;

            lv_listOfXF = lv_XFTaxes;

            while (  li_nextCityCode > AB_Integer(0)  &&
                     lv_listOfXF.length() > 3         &&
                     li_XFTaxCount        < 4 )
            {
               lv_listOfXF = lv_listOfXF.At(4);

               li_nextCityCode = lv_listOfXF.findFirstCharNotInSet("N.");

               if ( li_nextCityCode > AB_Integer(1) )
               {
                  lv_amount = lv_listOfXF.For(li_nextCityCode - AB_Integer(1));
               }
               else
               {
                  if ( li_nextCityCode <= AB_Integer(0) && lv_listOfXF.length() > 0 )
                  {
                     lv_amount = lv_listOfXF;
                     li_nextCityCode = lv_listOfXF.length() + 1;
                  }
                  else
                  {
                     lb_dataOK = AB_false;
                     break;
                  }
               }

               if ( lv_amount.NotIsNull() && lv_amount.findFirstCharNotInSet("N.") == 0 )
               {
                  ld_XFTax = lv_amount;
               }
               else
               {
                  lb_dataOK = AB_false;
                  break;
               }

               ld_totXFTaxes = ld_totXFTaxes + ld_XFTax ;
               lv_listOfXF = lv_listOfXF.At(li_nextCityCode) ;
               li_XFTaxCount++;
            }

            if ( lv_listOfXF.length() > 0 && lv_listOfXF.length() <= 3 )
            {
               lb_dataOK = AB_false;
            }

            li_XFTaxCount = 0;

            LOG(logDEBUG) << "pd_totalXFXTInM2:"<<pd_totalXFXTInM2;
            LOG(logDEBUG) << "ld_totXFTaxes:"<<ld_totXFTaxes;
// SCR #117474 END

				if ( lb_dataOK == AB_false )
				{
					/**
					 * XF tax breakdown not possible, XF data is not good
					 * prepare a msg for Note
					 **/

	           AB_Varchar   lv_remark(80) ;

              lv_remark = AB_Text("XF TAX BREAKDOWN NOT POSSIBLE,") +
								  AB_Text("XF INFO IN M6 IS INVALID");
              md_IRMK_type[mi_IRMK_I]           = "NTE" ;
              md_IRMK_remark[mi_IRMK_I]         = lv_remark ;
              md_IRMK_pax_id[mi_IRMK_I++]       = md_ID_pax_id[mi_ID_I] ;

				}
// SCR #117474 START
            else if ( ld_totXFTaxes <= pd_totalXFXTInM2 )
            {
               /**
                 * Break down is OK, go break XF down
                 **/

                li_nextCityCode = 1;

                while ( li_nextCityCode > AB_Integer(0)  &&
                        lv_XFTaxes.length()  > 3         &&
                        li_XFTaxCount        < 4 )
                {
                   li_nextCityCode = lv_XFTaxes.At(4).findFirstCharNotInSet("N.");

                  if ( li_nextCityCode > AB_Integer(1) )
                  {
                     ld_XFTax = lv_XFTaxes.At(4).For(li_nextCityCode - AB_Integer(1));
                  }
                  else
                  {
                     if ( li_nextCityCode <= AB_Integer(0) && lv_XFTaxes.At(4).length() > 0 )
                     {
                        ld_XFTax = lv_XFTaxes.At(4);
                        li_nextCityCode = lv_XFTaxes.At(4).length() + 1;
                     }
                     else
                     {
                        ld_XFTax = D_amount(0);
                     }
                  }

                  if ( ld_XFTax != D_amount(0) )
                  {
                     md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
                     md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
                     md_ITD_tax_id[mi_ITD_I]       = "XF" ;
                     md_ITD_city_id[mi_ITD_I]      = lv_XFTaxes.For(3) ;
                     md_ITD_tax_source[mi_ITD_I]   = "M2";
                     md_ITD_tax_amount[mi_ITD_I++] = ld_XFTax ;

                     pd_totXFTaxes = pd_totXFTaxes + ld_XFTax ;
                  }

                   lv_XFTaxes = lv_XFTaxes.At(li_nextCityCode+AB_Integer(3)) ;
                   li_XFTaxCount++;
                }

            }
            else if ( ld_totXFTaxes > pd_totalXFXTInM2 )
// SCR #117474 END
				{
					/**
					 * XF tax breakdown not possible, discrepancy with M6/M2
					 * prepare a msg for Note
					 **/

	           AB_Varchar   lv_remark(80) ;

              lv_remark = AB_Text("XF TAX BREAKDOWN NOT POSSIBLE,") +
								  AB_Text("DISCREPANCY WITH M6/M2");
              md_IRMK_type[mi_IRMK_I]     = "NTE" ;
              md_IRMK_remark[mi_IRMK_I]   = lv_remark ;
              md_IRMK_pax_id[mi_IRMK_I++] = md_ID_pax_id[mi_ID_I] ;
				}
         }
			break ;
		}
		i = i + AB_Integer(1) ;
		li_index++ ;
   }
   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::fillFOPM2(AB_Integer &pi_paxId,
							AB_Integer &pi_docLine,
							AB_Text    &pt_total_segment)
{
	ENTER;

	AB_Varchar	lv_fp(100) ;
	// SCR 210529 START
	AB_Varchar	lv_exp_date(4);
	// SCR 210529 END
	AB_Varchar	lv_firstPart(512) ;

	md_IRP_amount[mi_IRP_I]      = md_ID_base_cost[mi_ID_I] ;
	md_IRP_pax_id[mi_IRP_I]      = pi_paxId ;
	md_IRP_line_number[mi_IRP_I] = pi_docLine ;
	md_IRP_pay_id[mi_IRP_I]      = AB_Integer(1) ;

	V_AB_Varchar lv_arrayField(512,100) ;
	int li_pos ;
	int li_dollarPos;

	li_pos = lv_arrayField.parseStringMultDel(pt_total_segment,"!") ;
	lv_fp = lv_arrayField[4] ;
	lv_firstPart = lv_arrayField[1] ;

	/**
	 * Preprocessing
	 **/

	LOG(logDEBUG) << "\nM2 FOP : lv_fp:"<<lv_fp;
	int li_slashPos;

	//lv_fp.removeLeadingBlanks();

   while ( lv_fp.NotIsNull() && ! lv_fp.For(1).isAllAlphaNumeric())
	{
		lv_fp = lv_fp.At(2);
	}

	if ( lv_fp.For(2) == "A/" ) {
		lv_fp = lv_fp.At(3);
	}

	if ( lv_fp.For(2) == "EF"           &&
	     lv_fp.At(3).For(1).isAllAlpha() )
   {
		lv_fp = lv_fp.At(3);
	}

	if ( lv_fp.For(3) == "CCC"           &&
	     lv_fp.At(4).For(2).isAllAlpha() &&
	     lv_fp.At(6).For(2).isAllNumeric() )
   {
		lv_fp = lv_fp.At(2);
	}
	else if ( lv_fp.For(3) == "CCK" )
   {
		lv_fp = lv_fp.At(2);
	}
	else if ( lv_fp.For(6) == "CCHECK" )
   {
		lv_fp = lv_fp.At(2);
	}
	else if ( lv_fp.For(5) == "CCASH" )
	{
		lv_fp = lv_fp.At(2);
	}
	else if ( lv_fp.For(2) == "GR")
   {
		// Case of GR$K123456/####.##/##.##
		//         GR K123456

		lv_fp= lv_fp.At(3);

		// Remove possible /#####

	   li_slashPos = lv_fp.findSubstring("/");

		if ( li_slashPos > 0 ) {
         lv_fp = lv_fp.For(li_slashPos-1);
		}

		int li_length = lv_fp.length();

	   AB_Varchar lv_fpStr(100) ;
      lv_fpStr = AB_null;

		for ( int ii = 1; ii <= li_length; ii++) {

          if ( lv_fp.At(ii).For(1).isAllAlphaNumeric() )
			 {
				  if ( lv_fpStr.IsNull() )
				  {
				     lv_fpStr = lv_fp.At(ii).For(1);
				  }
				  else
				  {
				     lv_fpStr = lv_fpStr + lv_fp.At(ii).For(1);
				  }
			 }
		}

		li_dollarPos = lv_fp.findSubstring("$");

		if ( li_dollarPos == 0 )
		{
			lv_fp = "CA";
		}
		else
		{
      	if ( lv_fpStr.IsNull())
			{
          	lv_fp = "CCGR";
			}
			else
			{
          	lv_fp = "CCGR" + lv_fpStr;
			}
		}
	}

	li_slashPos = lv_fp.findSubstring("/");

	li_dollarPos = lv_fp.findSubstring("$");

	if ( lv_fp.IsNull() )
	{
		md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
	}
	else if ( li_slashPos  > 8                               		&&
				 li_slashPos  == (li_dollarPos+3)               		&&
				 lv_fp.At(1).For(2).isAllAlpha()                		&&
				 lv_fp.At(3).For(li_dollarPos-3).isAllNumeric() 		&&

				 // SCR 210529 START
				 (lv_fp.At(li_dollarPos+1).For(2).isAllNumeric() 	||
				  (lv_fp.At(li_dollarPos+1).For(1) == " " 			&&
					lv_fp.At(li_dollarPos+2).For(1).isAllNumeric() ))	&&
				 // SCR 210529 END

				 lv_fp.At(li_dollarPos+4).For(2).isAllNumeric()  )
	{
	    /**                 123                123456
	     *                  1234567890123456789012345
	     * Expected pattern VI4788250000410978$04/03
		  *                  VVNNNNNNNNNNNNNNNN DD YY
        **/

		 LOG(logDEBUG) << "\nSTANDARD FOP A:" << lv_fp;

		 // SCR 210529 START
			if ( lv_fp.At(li_dollarPos+1).For(1) == " " )
			{
				lv_exp_date = AB_Varchar("0") +
								  lv_fp.At(li_dollarPos+2).For(1) +
								  lv_fp.At(li_dollarPos+4).For(2) ;
			}
			else
			{
				lv_exp_date = 	lv_fp.At(li_dollarPos+1).For(2) +
									lv_fp.At(li_dollarPos+4).For(2) ;
			}
		 // SCR 210529 END

	    md_IRP_form_of_pay[mi_IRP_I] = "CC";
	    md_IRP_cc_id[mi_IRP_I]       = lv_fp.At(1).For(2);
	    md_IRP_cc_no[mi_IRP_I]       = lv_fp.At(3).For(li_dollarPos-3);
		 // SCR 210529 START
		 md_IRP_cc_exp_date[mi_IRP_I] = lv_exp_date;
		 // SCR 210529 END
	}
	else if ( li_slashPos > 7                                		&&
				 li_slashPos  == (li_dollarPos-6)               		&&
				 lv_fp.For(2) == "CC"                           		&&
				 lv_fp.At(3).For(2).isAllAlpha()                		&&
				 lv_fp.At(5).For(li_slashPos-5).isAllNumeric()  		&&

				 // SCR 210529 START
				 (lv_fp.At(li_dollarPos+1).For(2).isAllNumeric() 	||
				  (lv_fp.At(li_dollarPos+1).For(1) == " " 			&&
					lv_fp.At(li_dollarPos+2).For(1).isAllNumeric() ) ) &&
				 // SCR 210529 START

				 lv_fp.At(li_dollarPos+3).For(1) == "/"         		&&
				 lv_fp.At(li_dollarPos+4).For(2).isAllNumeric()   )
	{
	    /**
	     *                  12345678901234567890123456789012
	     * Expected pattern CCVI1234567890123456/0403/$04/03
		  *                    VVNNNNNNNNNNNNNNNN       DD YY
        **/

		 LOG(logDEBUG) << "\nSTANDARD FOP B:"<<lv_fp;

		 // SCR 210529 START
			if ( lv_fp.At(li_dollarPos+1).For(1) == " " )
			{
				lv_exp_date = AB_Varchar("0") +
								  lv_fp.At(li_dollarPos+2).For(1) +
								  lv_fp.At(li_dollarPos+4).For(2) ;
			}
			else
			{
				lv_exp_date = 	lv_fp.At(li_dollarPos+1).For(2) +
									lv_fp.At(li_dollarPos+4).For(2) ;
			}
		 // SCR 210529 END

	    md_IRP_form_of_pay[mi_IRP_I] = "CC";
	    md_IRP_cc_id[mi_IRP_I]       = lv_fp.At(3).For(2);
	    md_IRP_cc_no[mi_IRP_I]       = lv_fp.At(5).For(li_slashPos-5);
		 // SCR 210529 START
		 md_IRP_cc_exp_date[mi_IRP_I] = lv_exp_date;
		 // SCR 210529 END
	}
	else
	{
	   AB_Char lc_secType(2) ;
	   lc_secType = "M2" ;

		LOG(logDEBUG)<<"\nNON STANDARD:"<<lv_fp;

		  //SCR  499241
		md_IRP_cc_exp_date[mi_IRP_I] = lv_firstPart.At(222).For(4);
		if ( !md_IRP_cc_exp_date[mi_IRP_I].isAllNumeric() )
		{
			md_IRP_cc_exp_date[mi_IRP_I] = AB_null ;
		}

      //SCR 504805
      md_IRP_auth_no[mi_IRP_I] = lv_firstPart.At(208).For(9);
      md_IRP_auth_no[mi_IRP_I].removeTrailingBlanks();
      md_IRP_auth_no[mi_IRP_I].removeLeadingBlanks();

      //SCR513310  autho no could be number and alpha
		//if( !md_IRP_auth_no[mi_IRP_I].isAllNumeric() )
      //{
         //md_IRP_auth_no[mi_IRP_I] = AB_null;
      //}


      md_IRP_auth_source[mi_IRP_I] = lv_firstPart.At(186).For(1);

      // end of SCR 504805


		translate_fp(lv_fp,lc_secType) ;
   }

	if (md_IRP_form_of_pay[mi_IRP_I] != "CC")
	{
		md_IRP_cc_exp_date[mi_IRP_I] = AB_null ;
		md_IRP_auth_no[mi_IRP_I]     = AB_null ;
		md_IRP_auth_source[mi_IRP_I] =  AB_null ;
	}

	LOG(logDEBUG) << "\nmd_IRP_form_of_pay["<<mi_IRP_I<<"]:"
		  << md_IRP_form_of_pay[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_cc_exp_date["<<mi_IRP_I<<"]:"
		  << md_IRP_cc_exp_date[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_cc_id["<<mi_IRP_I<<"]:"
		  << md_IRP_cc_id[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_cc_no["<<mi_IRP_I<<"]:"
		  << md_IRP_cc_no[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_pax_id["<<mi_IRP_I<<"]:"
		  << md_IRP_pax_id[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_pay_id["<<mi_IRP_I<<"]:"
		  << md_IRP_pay_id[mi_IRP_I]  ;

	mi_IRP_I++ ;
   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::translate_fp(AB_Varchar &pc_fp, AB_Char &pc_secType)
{
	ENTER;

	LOG(logDEBUG) << "\npc_fp:"<<pc_fp;

	int	      cc_pos ;
	int	      non_num_pos ;
	AB_Varchar  lc_fp ;
	AB_Boolean  lb_foundCC ;

	if (pc_fp.At(1).For(1) == "*")
		pc_fp = pc_fp.At(2) ;

	cc_pos = pc_fp.findSubstring("CC") ;

	if ( pc_fp.For(2) == "CC" )
	{
		lb_foundCC = AB_true ;
		pc_fp = pc_fp.At(3) ;
	}
	else
		lb_foundCC = AB_false ;

	// Determine form of payment

	if ( pc_fp.For(6) == "CHEQUE" ||
		  pc_fp.For(5) == "CHECK"  ||
		  pc_fp.For(3) == "CHQ"    ||
		  pc_fp.For(2) == "CK" )
   {
		md_IRP_form_of_pay[mi_IRP_I] = "CK" ;
	}
	else if ( pc_fp.For(2) == "CX")
	{
		pc_fp = pc_fp.At(3) ;
		md_IRP_form_of_pay[mi_IRP_I] = "CC" ;
	}
	else if ( pc_fp.For(14) == "NONREF INVOICE" || pc_fp.For(3) == "INV" )
   {
		md_IRP_form_of_pay[mi_IRP_I] = "IV" ;
	}
	else
	{
		non_num_pos = pc_fp.At(3).For(1).findFirstCharNotInList("0123456789") ;

		if ( non_num_pos == 0 || pc_fp.For(2) == "TP" )
		{
			md_IRP_form_of_pay[mi_IRP_I] = "CC" ;
		}
		else
		{
			/*
			if ( pc_secType == "M2" )
			{
				md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
			}
			else
			{
				if ( pc_fp.At(3).For(2) == "GR" )
				{
					md_IRP_form_of_pay[mi_IRP_I] = "CC" ;
					pc_fp = pc_fp.At(3) ;
				}
				else
				{
					md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
				}
			}
			*/
			if ( lb_foundCC )
			{
				md_IRP_form_of_pay[mi_IRP_I] = "CC" ;
			}
			else
			{
				md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
			}
		}
	}

	if ( md_IRP_form_of_pay[mi_IRP_I] == "CC")
	{
		md_IRP_cc_id[mi_IRP_I] = pc_fp.For(2) ;

		AB_Char lc_cc(18) ;
		lc_cc = pc_fp.At(3).For(18) ;

		if ( pc_secType == "M2" )
			lc_cc.substitute(" ","") ;

		if ( pc_fp.For(2) != "GR" )
		{
			non_num_pos = lc_cc.findFirstCharNotInList("0123456789") ;

			if ( non_num_pos != 0 )
				md_IRP_cc_no[mi_IRP_I] = lc_cc.For(non_num_pos-1) ;
			else
				md_IRP_cc_no[mi_IRP_I] =  lc_cc ;

			// Look For Government warrents

			if (md_IRP_cc_id[mi_IRP_I] == "90" )
			{
				md_IRP_cc_no[mi_IRP_I] = "90" + md_IRP_cc_no[mi_IRP_I] ;
				md_IRP_cc_id[mi_IRP_I] = "GO" ;
			}
		}
		else
		{
			// For government request

			if ( pc_secType == "M2" )
			{
				non_num_pos = lc_cc.findSubstring("!") ;
			}
			else
			{
				non_num_pos = lc_cc.findSubstring(".") ;
				non_num_pos = non_num_pos - 1 ;
			}

			if ( non_num_pos > 0 )
			{
				md_IRP_cc_no[mi_IRP_I] = lc_cc.For(non_num_pos-1) ;
			}
			else
			{
				md_IRP_cc_no[mi_IRP_I] = lc_cc ;
			}
		}

	}
	else
	{
		md_IRP_cc_id[mi_IRP_I] = NULL ;
		md_IRP_cc_no[mi_IRP_I] = NULL ;
	}
   EXIT;
}

//-----------------------------------------------------------------------------

void CIUR::updIrconsumdetailTkt(AB_Integer	&pi_noofM4,
								        AB_Text		&pt_entitleNos,
								        AB_Char		&pc_paxId,
								        AB_Char		&pc_docId,
								        AB_Char		&pc_docPic,
                                AB_Varchar   &pc_cur_id) //FG - US446
{
   ENTER;

	AB_Integer	 li_entitle ;
	AB_Integer	 li_counter ;
	AB_Text      lt_seatData ;
	AB_Char      lc_tktPax(2) ;
	AB_Text      lt_entitleData ;
	int		    li_simIndex,li_pos ;
	AB_Boolean   lb_foundICD ;
	AB_Varchar	 lv_fareinfo(20) ;

	AB_Char   lc_tktEntitle(2) ;
	AB_Char   lc_entitleNo(2) ;
	AB_Integer	li_itinLine ;

	AB_Char      lc_ICDPax(2) ;

	li_entitle = 1 ;
	li_simIndex = -1;

	while ( li_entitle <= AB_Integer(mi_entitleIndex) )
	{
		lc_tktPax = pt_entitleNos.For(2) ;

		// Get the entotle ment line number from Ticket ;
		lc_tktEntitle = pt_entitleNos.For(2) ;

		lc_entitleNo = li_entitle ;
		lc_entitleNo.rightJustify('0') ;

		// Get the stored data from array

		lt_entitleData = gd_entitleData[li_entitle] ;

		if (lc_entitleNo == lc_tktEntitle )
		{
			if ( pt_entitleNos.Length() > AB_Integer(2) )
				pt_entitleNos = pt_entitleNos.At(3) ;
			else
				pt_entitleNos = AB_null ;
			// Found entitlement line for the ticket

			int li_index = 0 ;
			while( li_index < mi_ICD_I )
			{
				lb_foundICD = AB_false ;
				li_itinLine = lt_entitleData.At(3).For(2) ;

				if (md_ICD_itin_line_number[li_index] == li_itinLine)
				{
					li_simIndex = li_index ;

					// Found itin line in consumedetail

					if (md_ICD_line_number[li_index].IsNull())
					{
						lc_ICDPax = md_ICD_pax_id[li_index] ;
						lc_ICDPax.rightJustify('0') ;

						if ( lc_ICDPax == pc_paxId )
						{
							// Found a free line to put the ticket no

							md_ICD_line_number[li_index]  = pc_docId ;
							lb_foundICD = AB_true ;
							li_simIndex = li_index ;
							break ;
						}
					}

				}
				li_index++ ;
			}

			if ( !lb_foundICD )
			{
				if ( li_simIndex < 0 )
				{
					// error 'MISMATCH FARE LEG SEGMENT
					CMsgHandler::setMsg(AB_Integer(3456),
											  AB_Char(15," "),
											  D_error_track("UPD-IRCONS-TKT"),
											  AB_Integer(1),
											  AB_Varchar(20,AB_null));

					return;
				}
				// Added a line for more than one ticket per pax for same itin

				md_ICD_pax_id[mi_ICD_I] = pc_paxId ;
				md_ICD_itin_type[mi_ICD_I] = md_ICD_itin_type[li_simIndex] ;
				md_ICD_itin_line_number[mi_ICD_I] = md_ICD_itin_line_number[li_simIndex] ;
				md_ICD_seat[mi_ICD_I] = md_ICD_seat[li_simIndex] ;
				md_ICD_cog_seat[mi_ICD_I] = md_ICD_cog_seat[li_simIndex] ;
				md_ICD_ftp_miles[mi_ICD_I] = md_ICD_ftp_miles[li_simIndex] ;
				md_ICD_special_meal[mi_ICD_I] = md_ICD_special_meal[li_simIndex] ;
				md_ICD_special_need[mi_ICD_I] = md_ICD_special_need[li_simIndex] ;
				md_ICD_line_number[mi_ICD_I] = pc_docId ;
				li_simIndex = mi_ICD_I ;
			}

			// Get the connection Indicator

			/*
				md_ICD_connection[li_simIndex] = getConnectionInd(
														  pc_docPic,
														  lt_entitleData.At(3).For(2)) ;
															  */
			//if (( li_simIndex == 0 && lb_foundICD ) || li_simIndex > 0 )
			md_ICD_connection[li_simIndex] = "O" ;

			lv_fareinfo = lt_entitleData.At(25).For(13) ;

			li_pos = lv_fareinfo.findSubstring("/") ;
			if ( li_pos > 0 )
			{
				md_ICD_fare_basis[li_simIndex] = lv_fareinfo.For(li_pos -1) ;
				md_ICD_tkt_desig[li_simIndex]  = lv_fareinfo.At(li_pos + 1) ;
			}
			else
			{
				md_ICD_fare_basis[li_simIndex] = lv_fareinfo ;
			}

			md_ICD_fare_basis[li_simIndex].leftJustify() ;
			md_ICD_tkt_desig[li_simIndex].leftJustify() ;

			md_ICD_fare_by_leg[li_simIndex] = lt_entitleData.At(40).For(8) ;

			/* SCR 516055 -ignore status from M4
			if (lt_entitleData.At(20).For(2) != "  ") {
				md_ICD_service_status[li_simIndex] = lt_entitleData.At(20).For(2) ;
			}
			*/

			md_ICD_baggage[li_simIndex] = lt_entitleData.At(22).For(3) ;

			md_ICD_cur_id[li_simIndex] = pc_cur_id; //FG - US446 - lt_entitleData.At(71).For(3) ;

			if ( !lb_foundICD )
				mi_ICD_I++ ;

		}
		li_entitle = li_entitle + AB_Integer(1) ;
	}
   EXIT;
}


//------------------------------------------------------------------------------

void CIUR::fill_IrRentalHotel(AB_Text &pt_section)
{
   ENTER;

	if (pt_section.At(15).For(3) == "HHL")
		fill_IrRentalHotelAuto(pt_section) ;
	else
		fill_IrRentalHotelManual(pt_section) ;

   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::fill_IrRentalHotelAuto(AB_Text &pt_section)
{
   ENTER;

   int          li_pos,
					 i,
                li_pos_pct ;

   AB_Varchar   lv_field(50) ;
   AB_Varchar   lv_com_rate_full(50) ;
   AB_Varchar   lv_com_rate(50) ;
   
   AB_Date      ld_tempDate ;
   AB_Varchar   lv_hotel(32) ;
   AB_Varchar   lv_varField(255) ;
	V_AB_Varchar lv_arrayField(100,100) ;
   AB_Char      lc_year(2) ;

  // SCR: 515207 migrated from _au
  AB_Varchar   lv_IU3NRM_segment(20) ;
  lv_IU3NRM_segment = pt_section.At(93).For(8) ;
  lv_IU3NRM_segment.removeLeadAndTrailBlanks();
  LOG(logDEBUG) << "\n*** lv_IU3NRM_segment Length Check: " << lv_IU3NRM_segment.length() ;
  LOG(logDEBUG) << "\n*** lv_IU3NRM_segment DATA   Check: " << lv_IU3NRM_segment ;
  // SCR: 515207

   md_IR_itin_type[mi_IR_I]      = "H" ;
   md_IR_line_number[mi_IR_I]    = pt_section.At(3).For(2) ;
	md_IR_source[mi_IR_I]         = "A" ;

	mv_service_status             = pt_section.At(8).For(2) ;
   md_IR_service_status[mi_IR_I] = pt_section.At(8).For(2) ;

	// Get start date

  	lc_year = mt_pnrDate.get_default_year(pt_section.At(10).For(5)) ;
	lc_year.rightJustify('0') ;

   md_IR_start_date[mi_IR_I]     = pt_section.At(10).For(5) + lc_year ;

   md_IR_unit_ct[mi_IR_I]        = pt_section.At(93).For(1) ;

   md_IR_confirmation[mi_IR_I]   = pt_section.At(21).For(15) ;

	md_IR_origin_city_id[mi_IR_I] = pt_section.At(36).For(3) ;

	md_IR_sup_id[mi_IR_I]         = pt_section.At(39).For(2) + "H" ;

	md_IR_rate_duration[mi_IR_I]  = "D" ;

	md_IR_end_date[mi_IR_I]       =  pt_section.At(45).For(5) ;

	if (md_IR_end_date[mi_IR_I] == "     " )
		md_IR_end_date[mi_IR_I]    =  md_IR_start_date[mi_IR_I] ;
	else
	{
   	//28JAN98 - 1607 - SC - Change mt_pnrDate as md_IR_start_date.

      ld_tempDate = md_IR_start_date[mi_IR_I];

   	lc_year = ld_tempDate.get_default_year(pt_section.At(45).For(5)) ;
		lc_year.rightJustify('0') ;

		md_IR_end_date[mi_IR_I]    =  pt_section.At(45).For(5) + lc_year ;
	}

	md_IR_location_id[mi_IR_I]    =  pt_section.At(55).For(6) ;
	li_pos = pt_section.findSubstring("/7P-");
	if (li_pos > 0) {
		AB_Varchar lv_propery_id(6);
		lv_propery_id = pt_section.At(li_pos+5).For(6);
		if (lv_propery_id.NotIsNull() && lv_propery_id != "      ") {
			md_IR_location_id[mi_IR_I] = lv_propery_id;
		}
	}

	lv_hotel                      = pt_section.At(61).For(32) ;

	lv_hotel.leftJustify() ;

	if ( lv_hotel.At(3).For(1) == " ")
	{
		if ( lv_hotel.For(2) == md_IR_sup_id[mi_IR_I].For(2) )
			lv_hotel = lv_hotel.At(4) ;
	}

	lv_hotel.leftJustify() ;

	md_IR_site_name[mi_IR_I]      =  lv_hotel ;

	// SCR: 515207
  if ( lv_IU3NRM_segment.length() == 8 )
  {
     md_IR_unit_type[mi_IR_I]      =  pt_section.At(94).For(4) ;
     md_IR_rate_type[mi_IR_I]      =  pt_section.At(98).For(3) ;
  }
  else
  {
     md_IR_unit_type[mi_IR_I]      =  pt_section.At(94).For(3) ;
     md_IR_rate_type[mi_IR_I]      =  pt_section.At(97).For(3) ;
  }

	//G9146B - CWT Value Rates
	lv_field = pt_section.getNthBracketedField(1, "/RC-", "/") ;
	if (lv_field.NotIsNull())
	{
		//only /RC-XXX-YYY-Z/ will be parsed
		if ( PCRE::pcre_match("^...-...-.$", lv_field.to_string()) )
		{
			if ( PCRE::pcre_match("^[a-zA-Z0-9]{3}$", lv_field.For(3).to_string() ) )
				md_IR_corporate_disc[mi_IR_I] = lv_field.For(3) ;

			//li_pos = lv_field.At(5).For(3).Upper().findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-") ;
			if ( PCRE::pcre_match("^[a-zA-Z0-9]{3}$", lv_field.At(5).For(3).to_string()  ) )
				md_IR_rate_type[mi_IR_I] 		= lv_field.At(5).For(3) ;

         //US497-Stop capturing category code only for RC- data element - STARTS
			//if ( PCRE::pcre_match("^[a-zA-Z0-9]$", lv_field.At(9).For(1).to_string() ) )
				//md_IR_rate_code[mi_IR_I] 		= lv_field.At(9).For(1) ;
			LOG(logDEBUG) << "md_IR_corporate_disc=====>" << md_IR_corporate_disc[mi_IR_I] << "<"  ;
			LOG(logDEBUG) << "md_IR_rate_type=====>" << md_IR_rate_type[mi_IR_I] << "<"  ;
			LOG(logDEBUG) << "md_IR_rate_code=====>" << md_IR_rate_code[mi_IR_I] << "<"  ;
			//US497-Stop capturing category code only for RC- data element - ENDS
		}
	}

	LOG(logDEBUG) << "AM====== RATE_TYPE(M03) is [" << md_IR_rate_type[mi_IR_I] << "]"  ;


	lv_varField                   =  pt_section.At(101) ;

	li_pos = lv_arrayField.parseStringMultDel(lv_varField,"/") ;

	string local_rate,  local_curr;
	string booked_rate, booked_curr;
	if ( li_pos > 0 )
	{
		lv_arrayField[1].removeLeadAndTrailBlanks();
		if ( PCRE::pcre_match("^([\\d\\.]+)([A-Z][A-Z][A-Z])" , lv_arrayField[1].to_string() ))
		{
			PCRE::pcre_get_group(1, local_rate);
			PCRE::pcre_get_group(2, local_curr);
			LOG(logDEBUG) << "local rate match - YES "  << local_rate.data() << " " << local_curr.data();
			md_IR_local_rate[mi_IR_I] = local_rate.data() ;
			md_IR_cur_id[mi_IR_I]     = local_curr.data();
		}
	}

	if ( li_pos > 1 )
	{
		lv_arrayField[2].removeLeadAndTrailBlanks();
		if ( PCRE::pcre_match("^([\\d\\.]+)([A-Z][A-Z][A-Z])" , lv_arrayField[2].to_string() ))
		{
			PCRE::pcre_get_group(1, booked_rate);
			PCRE::pcre_get_group(2, booked_curr);
			LOG(logDEBUG) << "booked rate match - YES "  << booked_rate.data() << " " << booked_curr.data();
			md_IR_booked_rate[mi_IR_I] = booked_rate.data() ;
			md_IR_pcc_cur_id[mi_IR_I] = booked_curr.data() ;
		}
		//i = lv_arrayField[2].findFirstCharNotInList("0123456789.") ;
		//if ( i > 0 )
		//	md_IR_booked_rate[mi_IR_I] = lv_arrayField[2].For(i-1) ;
	}

	if ( md_IR_booked_rate[mi_IR_I].IsNull() )
		md_IR_booked_rate[mi_IR_I] = md_IR_local_rate[mi_IR_I] ;

	// get cancellation policy
   if (lv_arrayField.parseStringMultDel(pt_section, "!") == 3) {
		if (lv_arrayField[3].length() == 35) {
			lv_arrayField[3].removeLeadAndTrailBlanks();
			md_IR_cancel_policy[mi_IR_I] = lv_arrayField[3];
		}
	}

   lv_field = pt_section.getNthBracketedField(1, "/CMN-", "/") ;
   if (lv_field.NotIsNull())
	{
	  if      ( lv_field == "C" )  md_IR_commissionable[mi_IR_I] = "Y";
	  else if ( lv_field == "NC" ) md_IR_commissionable[mi_IR_I] = "N";
	}

	processCommonHotel(pt_section) ;

	// For Non arc ticketing

	AB_Char    lc_noofSeats(2) ;
	AB_Text    lt_seatData ;
	AB_Text    lt_ftpMiles ;
	AB_Char    lc_M5link(1) ;
	AB_Char    lc_itinLine(2) ;

	lc_M5link = pt_section.At(6).For(1) ;

	lc_itinLine                   = pt_section.At(3).For(2) ;

   if (mv_itinTransport.IsNull())
      mv_itinTransport = lc_itinLine ;
   else
      mv_itinTransport = mv_itinTransport + lc_itinLine ;

	// fill Personitin

	lc_noofSeats = "00" ;
	lt_seatData  = " " ;

	lt_ftpMiles = AB_null ;


	fillConsumDetail(lc_noofSeats,
					 md_IR_itin_type[mi_IR_I],
				 	 lc_itinLine,
					 lt_seatData,
					 lt_ftpMiles) ; /* ftp_miles */

	md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;

   //US498 - FG - Start
   if (md_IR_com_rate[mi_IR_I].IsNull())
   {
      lv_com_rate_full = pt_section.getNthBracketedField(1, "/TAC-", "/") ;
   
      li_pos_pct = 0;
      li_pos_pct = lv_com_rate_full.findSubstring("FIXED");

      if (li_pos_pct == 0 )
      {
         li_pos_pct = lv_com_rate_full.findSubstring("PCT");
      }
   
      if (li_pos_pct - 1  > 0 )
      {
         lv_com_rate = lv_com_rate_full.At(1).For(li_pos_pct-1);
   
         md_IR_com_rate[mi_IR_I] = lv_com_rate;
   
      }
   }
   //US498 - FG - End

   mi_IR_I++ ;

   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::processCommonHotel(AB_Text &pt_section)
{
	ENTER;

	AB_Char	itin_type = "H" ;
	AB_Char lc_commissionable;
   AB_Char lc_comm_rate(50);
   //***********US500 Chg Start**********************
	AB_Char lc_preferred ;
	//***********US500 Chg End ***********************

	md_IR_number_of_pax[mi_IR_I] = 1 ;
   getAddressData(pt_section) ;

	getSpecialData(pt_section) ;

	if ( mv_actionCode.NotIsNull() )
	{

         cout << "FG: processCommonHotel: mv_actionCode= " << mv_actionCode << flush << endl;

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

   getOverrideRates(itin_type,
               	  md_IR_start_date[mi_IR_I],
               	  md_IR_saving_code[mi_IR_I],
               	  md_IR_local_rate[mi_IR_I],
               	  md_IR_regular_rate[mi_IR_I],
               	  md_IR_booked_rate[mi_IR_I],
          			  lc_commissionable, //md_IR_commissionable[mi_IR_I],
                       lc_preferred,   //US500 Chg
               	  gv_IrHotelData, gi_IrHotelIndex,
                    lc_comm_rate) ; //US498 - FG

   if ( lc_comm_rate.NotIsNull())
      md_IR_com_rate[mi_IR_I] = lc_comm_rate;
   
	if ( lc_commissionable.NotIsNull())
		md_IR_commissionable[mi_IR_I] = lc_commissionable;

//***********US500 Chg Start**********************
    if ( lc_preferred.NotIsNull())
		md_IR_preferred[mi_IR_I] = lc_preferred;
//***********US500 Chg End**********************
   EXIT;
}

//-----------------------------------------------------------------------------

void CIUR::getAddressData(AB_Text &pt_section)
{
   ENTER;

   AB_Varchar   lv_longAddress(120) ;
   AB_Varchar   lv_longPhone(120) ;
   AB_Varchar   lv_longFax(120) ;
	AB_Varchar 	 lv_field(255) ;
   int          li_pos ;
	CAddress     lo_Address ;
	AB_Text	    lt_section ;

	lt_section = pt_section ;

	lv_field = lt_section.getNthBracketedField(1, "/RC-", "/") ;
	if (lv_field.NotIsNull())
   {
      //Skip /RC-XXX-YYY-N/
		lv_field = "/RC-"+lv_field+"/" ;
		li_pos = lt_section.findSubstring("/RC-") ;
		lt_section = lt_section.For(li_pos) + lt_section.At(li_pos + lv_field.length() ) ;
   }

	lv_field = lt_section.getNthBracketedField(1, "/RQ-", "/") ;
	if (lv_field.NotIsNull())
   {
      //Skip /RQ-XXX/
		lv_field = "/RQ-"+lv_field+"/" ;
		li_pos = lt_section.findSubstring("/RQ-") ;
		lt_section = lt_section.For(li_pos) + lt_section.At(li_pos + lv_field.length() ) ;
   }

   li_pos = lt_section.findSubstring("@") ;

	if ( li_pos > 0 )
	{
		lv_longAddress = lt_section.At(li_pos + 1) ;

		// Look For Phone

		li_pos = lv_longAddress.findSubstring("$FONE") ;

		if (li_pos > 0)
		{
			lv_longPhone   = lv_longAddress.At(li_pos + 5) ;
			lv_longAddress = lv_longAddress.For(li_pos - 1) ;
		}

		lv_longAddress.substitute("$","/");

		lo_Address.setFourLineAddr(lv_longAddress);
      lo_Address.getAddress(md_IR_addr1[mi_IR_I],
               				md_IR_addr2[mi_IR_I],
               				md_IR_addr3[mi_IR_I],
               				md_IR_addr4[mi_IR_I]) ;

		if ( lv_longPhone.NotIsNull() )
		{
			lv_longPhone.removeLeadAndTrailBlanks();
			li_pos = lv_longPhone.findSubstring("$FAX") ;

			if ( li_pos > 0 )
			{
				lv_longFax   = lv_longPhone.At(li_pos + 4) ;
				lv_longPhone = lv_longPhone.For(li_pos - 1) ;
				li_pos       = lv_longFax.findSubstring("@") ;
				lv_longFax   = lv_longFax.For(li_pos - 1) ;

			}
			else
			{
				li_pos = lv_longPhone.findSubstring("@") ;
				// G9090c start
				if (li_pos == 0) {
					li_pos = lv_longPhone.findSubstring("!") ;
				}
				// G9090c end
				lv_longPhone = lv_longPhone.For(li_pos - 1) ;
			}

			// assign phone and fax if exists

			editPhoneNumber(lv_longPhone, md_IR_phone_acode[mi_IR_I], md_IR_phone_no[mi_IR_I], true);

			if ( lv_longFax.NotIsNull() )
			{
			  editPhoneNumber(lv_longFax, md_IR_fax_acode[mi_IR_I], md_IR_fax_no[mi_IR_I], true);
			}
		}
	}
   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::getSpecialData(AB_Text &pt_section)
{
   ENTER;

   int          li_pos ;
   AB_Varchar   lv_remark(80) ;

	li_pos = pt_section.findSubstring("/SI-") ;

	if ( li_pos > 0 )
	{
		lv_remark = pt_section.At(li_pos + 4);


		li_pos = lv_remark.findSubstring("-CF") ;

		if (li_pos > 1)
			lv_remark = lv_remark.For(li_pos - 1) ;
		else
			lv_remark = AB_null ;

		if (lv_remark.length() > 0 )
		{
			md_IRMK_remark[mi_IRMK_I]         = lv_remark ;
			md_IRMK_itin_type[mi_IRMK_I]   = "H" ;
			md_IRMK_ren_line_number[mi_IRMK_I] = md_IR_line_number[mi_IR_I] ;
			md_IRMK_pax_id[mi_IRMK_I++]       = AB_null ;
		}
	}

	AB_Varchar   lv_field(255);
   lv_field = pt_section.getNthBracketedField(1, "ARR-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_start_time[mi_IR_I] = CheckTime(lv_field);

   lv_field = pt_section.getNthBracketedField(1, "DEP-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_end_time[mi_IR_I] = CheckTime(lv_field);


	// get corp id
   lv_field = pt_section.getNthBracketedField(1, "ID-", "/") ;
   if (lv_field.NotIsNull()) {
		lv_field.removeLeadAndTrailBlanks();
		if ((li_pos = lv_field.findSubstring(" ")) > 0) {
			lv_field = lv_field.For(li_pos-1);
		}

		if ((li_pos = lv_field.findSubstring(",")) > 0) {
			lv_field = lv_field.For(li_pos-1);
		}

      md_IR_corporation_id[mi_IR_I] = lv_field;
	}

	if (pt_section.findSubstring("/G/") == 0) {
		lv_field = pt_section.getNthBracketedField(1, "/G", "/") ;
		if (lv_field.IsNull()) {
			lv_field = pt_section.getNthBracketedField(1, "/G", "!") ;
		}
		if (lv_field.NotIsNull())
			md_IR_guarantee[mi_IR_I] = lv_field ;
	}

   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::getOverrideRates(AB_Char        &pc_itin_type,
									 D_char_date    &pd_start_date,
									 D_saving_code  &pd_saving_code,
             					 D_char_numeric &pd_local_rate,
									 D_char_numeric &pd_regular_rate,
             					 D_char_numeric &pd_booked_rate,
									 AB_Char			 &pd_commissionable,
                                     AB_Char        &pd_preferred,    //US500 Chg
									 V_AB_Text      &pd_Data,
									 int            pi_Index,
                            AB_Char        &pc_comm_rate) //US498 - FG
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
   AB_Varchar      lv_field(50) ; //US498 - FG

	if (pd_regular_rate == "0")
		pd_regular_rate = AB_null ;

   li_counter = 1 ;
	lb_RCFound = AB_false ;

	LOG(logDEBUG) << "AM===== HS/CS pi_Index=" << pi_Index << " pd_start_date= " << pd_start_date  ;

   if (pi_Index == li_counter)
      return ;

   //lv_data = pd_Data[li_counter] ;
	lv_data = AB_null ;

    // SCR 516742
	AB_Boolean lb_hotel_overridden = AB_false;

   while (li_counter < pi_Index)
   {
      // SCR: 516742
      lb_hotel_overridden = AB_false;

	  LOG(logDEBUG) << "AM===== HS/CS loop[" << li_counter << "] data=" << pd_Data[li_counter]  ;
		if ( pc_itin_type == "H" )
		{
			AB_Text lt_data = pd_Data[li_counter];
			if ( lt_data.findSubstring("/") == 5 )
			{
				lt_data = "0" + lt_data;
			}

     		if (lt_data.For(5) == pd_start_date.For(5))
			{
        		if (lv_data.IsNull()) 
				{
				lv_data = lt_data.At(6) ;
				}
				else
				{
        		lv_data = AB_Text(lv_data) + lt_data.At(6) ;
				}
				//break ;
			}
		}
		else
		{
			if ((pd_Data[li_counter].For(5) == pd_start_date.For(5) ) &&
				(pd_Data[li_counter].At(6).For(3) == md_IR_origin_city_id[mi_IR_I] ) )
			{
				lv_data = pd_Data[li_counter].At(9) ;
				break ;
			}

		}
      li_counter++ ;
   }

	LOG(logDEBUG) << "AM===== FOUND=[" << lv_data <<  "]"  ;

	if ( lv_data.IsNull() )
		return ;
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

   li_pos = lv_data.findSubstring("NC-") ;

   if (li_pos > 0 && pd_commissionable.IsNull() &&
			( lv_data.At(li_pos + 3).For(1) == "Y" ||
				lv_data.At(li_pos + 3).For(1) == "N") ) // pick up only the first code
   {
      pd_commissionable = lv_data.At(li_pos + 3).For(1) ;
   }
//***********US500 Chg Start**********************
   li_pos = lv_data.findSubstring("BT-") ;

   if (li_pos > 0 && pd_preferred.IsNull() &&
         ( lv_data.At(li_pos + 3).For(1) == "P" ||
            lv_data.At(li_pos + 3).For(1) == "N") ) // pick up only the first code
   {
		pd_preferred = lv_data.At(li_pos + 3).For(1) ;
   }
//***********US500 Chg End**********************


   //US498 - FG - Start
   li_pos = lv_data.findSubstring("HP-") ;

   if (li_pos > 0 )
   {
      lv_field = lv_data.At(li_pos + 3);

      li_pos = lv_field.findFirstCharNotInList("0123456789.") ;

      if ( li_pos > 0 )
            pc_comm_rate = lv_field.For(li_pos - 1) ;
         else
            pc_comm_rate = lv_field ;
   }
   //US498 - FG - End

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

		if ( lb_RCFound )
		{
			pd_local_rate = ld_RTAmount ;

			 // SCR: 516742
         if (md_IR_itin_type[mi_IR_I] == 'H') {
               //LOG(logDEBUG)  << "Hotel local_rate override " << md_IR_cur_id[mi_IR_I];
               lb_hotel_overridden = true; }

		}
		else
		{
			pd_booked_rate = ld_RTAmount ;
		}
   }

   if (lb_hotel_overridden == AB_true) {
         //LOG(logDEBUG)  << "DEBUG Hotel override at line_no: " << md_IR_line_number[mi_IR_I];
    	md_IRMK_type[mi_IRMK_I]  = "FHB";
    	md_IRMK_itin_type[mi_IRMK_I]  = "H";
    	md_IRMK_pax_id[mi_IRMK_I]  = AB_null;
    	md_IRMK_ren_line_number[mi_IRMK_I] = md_IR_line_number[mi_IR_I];
    	md_IRMK_remark[mi_IRMK_I++]   = "CUR_ID/LOCALRATE OVERRIDE";
      }


	if ( md_IR_cur_id[mi_IR_I].IsNull() || md_IR_cur_id[mi_IR_I]== "   " )
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

	setIrReasonCodeIfOverride(lv_data);  // set reason,action,source if EB- present

	/*-----------------------------------------AM
	li_pos = lv_data.findSubstring("/RAC-") ;
	if (li_pos > 0)
	{
		md_IR_rate_type[mi_IR_I]      =  lv_data.At(li_pos+5).For(3) ;
		li_pos = md_IR_rate_type[mi_IR_I].findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") ;
		if (li_pos > 0)
		{
			md_IR_rate_type[mi_IR_I] = md_IR_rate_type[mi_IR_I].For(li_pos-1);
		}
		LOG(logDEBUG) << "AM====== RATE_TYPE(RAC) is [" << md_IR_rate_type[mi_IR_I] << "]"  ;
	}

	li_pos = lv_data.findSubstring("/CG-") ;
	if (li_pos > 0)
	{
		md_IR_rate_code[mi_IR_I]      =  lv_data.At(li_pos+4).For(10) ;
		li_pos = md_IR_rate_code[mi_IR_I].findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789") ;
		if (li_pos > 0)
		{
			md_IR_rate_code[mi_IR_I] = md_IR_rate_code[mi_IR_I].For(li_pos-1);
		}
		LOG(logDEBUG) << "AM====== RATE_CODE(CG) is [" << md_IR_rate_code[mi_IR_I] << "]"  ;
	}
	------------------- AM */

   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::getRateOverride(AB_Varchar &pv_data, AB_Varchar &lc_delim, D_char_numeric &pm_amount)
{
   ENTER;

   int li_pos ;
   D_amount lm_amount ;
   AB_Money lm_value ;
   AB_Varchar lv_rawAmount(15) ;
   AB_BOOL lb_ok ;


   li_pos = pv_data.findSubstring(lc_delim.to_string()) ;

   if (li_pos > 0)
   {
      if (pv_data.At(li_pos + 2).For(1) == "-")
         li_pos = li_pos + 3 ;
      else
         li_pos = li_pos + 2 ;

      pv_data = pv_data.At(li_pos) ;

      li_pos = pv_data.findFirstCharNotInList("0123456789.") ;

      if (li_pos > 0)
         pv_data = pv_data.For(li_pos - 1) ;

      lv_rawAmount = pv_data ;
      lv_rawAmount.removeLeadingBlanks() ;
      lm_value = lv_rawAmount.to_money("#######.##", lb_ok) ;
      lm_amount = lm_value.to_string() ;

      pm_amount = D_char_numeric(lm_amount) ;
   }

   EXIT;
}

//-----------------------------------------------------------------------------

void CIUR::fill_IrRentalHotelManual(AB_Text &pt_section)
{
   ENTER;

   int          li_pos,
                li_pos_SP,
					 li_rateInd,
					 i ;
   AB_Varchar   lv_field(50) ;
   AB_Date      ld_tempDate ;
   AB_Varchar   lv_hotel(255) ;
   AB_Varchar   lv_varField(255) ;
	V_AB_Varchar lv_arrayField(100,100) ;
	AB_Char      lc_year(2) ;
   AB_Text      pt_section_SP ;

   /**************************************************************************
   19DEC07 - 382703 - FG - Parse for SP- tag and populate Rental.location_id
   **************************************************************************/

   li_pos = pt_section.findSubstring("SP-") ;

   if (li_pos > 0)
   {

      pt_section_SP = pt_section.At(li_pos + 3 ) ;
      li_pos_SP = pt_section_SP.findSubstring("/");
      md_IR_location_id[mi_IR_I]    =  pt_section_SP.For(li_pos_SP - 1) ;

      LOG(logDEBUG)  << "SP value >> location_id = " <<  md_IR_location_id[mi_IR_I]  ;
   }

	// SCR408389 start
	lv_field = pt_section.getNthBracketedField(1,"/PC","/") ;
	if (lv_field.NotIsNull()) {
		md_IR_location_id[mi_IR_I] = lv_field;
	}
	// SCR408389 end

	// SCR518882 start
	lv_field = pt_section.getNthBracketedField(1,"/P-","/") ;
	if (lv_field.IsNull()) {
		lv_field = pt_section.getNthBracketedField(1, "/P-", "!") ;
	}
	if (lv_field.NotIsNull()) {
		md_IR_location_id[mi_IR_I] = lv_field;
	}

	if (pt_section.findSubstring("/G/") != 0) {
			md_IR_guarantee[mi_IR_I] = "GUARANTEED" ;
	}
	// SCR518882 end

	li_pos = pt_section.findSubstring("SI-") ;

	if (md_IR_prod_code[mi_IR_I] == "BOQ" ||
		 md_IR_prod_code[mi_IR_I] == "BEQ" ||
		 md_IR_prod_code[mi_IR_I] == "CBQ") {
   	li_pos = pt_section.findSubstring("SI-@") ;
	}


	if ( li_pos > 0 )
	{
   	md_IR_itin_type[mi_IR_I]      = "H" ;
   	md_IR_line_number[mi_IR_I]    = pt_section.At(3).For(2) ;
		md_IR_source[mi_IR_I]         = "M" ;

   	md_IR_service_status[mi_IR_I] = pt_section.At(8).For(2) ;
		// SCR: 517781
		mv_service_status  = md_IR_service_status[mi_IR_I];

		md_IR_rate_duration[mi_IR_I]  = "D" ;

		// Get start date

   	lc_year = mt_pnrDate.get_default_year(pt_section.At(10).For(5)) ;
		lc_year.rightJustify('0') ;

   	md_IR_start_date[mi_IR_I]     = pt_section.At(10).For(5) + lc_year ;

   	md_IR_unit_ct[mi_IR_I]        = pt_section.At(19).For(2) ;

   	md_IR_confirmation[mi_IR_I]   = pt_section.At(21).For(15) ;

		lv_varField                   =  pt_section.At(36) ;

		li_pos = lv_arrayField.parseStringMultDel(lv_varField,"/") ;

		md_IR_origin_city_id[mi_IR_I] = lv_arrayField[1] ;

		 //28JAN98 - 1607 - SC - Change mt_pnrDate as md_IR_start_date.

      ld_tempDate = md_IR_start_date[mi_IR_I] ;

   	lc_year = ld_tempDate.get_default_year(lv_arrayField[2].At(4).For(5)) ;
		lc_year.rightJustify('0') ;

		md_IR_end_date[mi_IR_I]       = lv_arrayField[2].At(4).For(5) + lc_year ;

		if ( md_IR_end_date[mi_IR_I] == "    " )
			md_IR_end_date[mi_IR_I]    = md_IR_start_date[mi_IR_I] ;

		lv_arrayField[3].leftJustify() ;

		md_IR_sup_id[mi_IR_I]         = lv_arrayField[3].For(2) + "H" ;

		if (lv_arrayField[3].At(3).For(1) == " " )
			md_IR_site_name[mi_IR_I]   =  lv_arrayField[3].At(4) ;
		else
			md_IR_site_name[mi_IR_I]   =  lv_arrayField[3] ;

		// UNIT TYPE START----------------------
		int li_unitTypeInd = 4;
		int li_rateInd = 5;
		if ( lv_arrayField[4].For(3) == "RT-" )
		{
			li_unitTypeInd = 5;
			li_rateInd = 4;
		}
		md_IR_unit_type[mi_IR_I]      = lv_arrayField[li_unitTypeInd] ;
		md_IR_unit_type[mi_IR_I].leftJustify() ;

		if (md_IR_prod_code[mi_IR_I] == "BOQ" ||
			 md_IR_prod_code[mi_IR_I] == "BEQ" ||
			 md_IR_prod_code[mi_IR_I] == "CBQ" ||
			 (md_IR_prod_code[mi_IR_I] == "HHT" && md_IR_sup_id[mi_IR_I].For(2) == "NL")) {
			md_IR_site_name[mi_IR_I].removeLeadAndTrailBlanks();
      	md_IR_location_id[mi_IR_I] =  md_IR_sup_id[mi_IR_I].For(2) +
				md_IR_site_name[mi_IR_I].At(md_IR_site_name[mi_IR_I].length()-2);
		}
		// UNIT TYPE END----------------------

		// RATE START----------------------
		if ( lv_arrayField[li_rateInd].For(2) == "PC" ) li_rateInd++;

		if (lv_arrayField[li_rateInd].NotIsNull() )
		{
			lv_arrayField[li_rateInd].leftJustify() ;

			if  (lv_arrayField[li_rateInd].length() > 0 )
			{

			    //SCR469519, skip any leading key RG-
			  li_pos = lv_arrayField[li_rateInd].findSubstring("-");
			  if ( li_pos > 0 && li_pos < lv_arrayField[li_rateInd].length() )
			  {
			    lv_arrayField[li_rateInd] = lv_arrayField[li_rateInd].At(li_pos+1);
			  }

				i = lv_arrayField[li_rateInd].findFirstCharNotInList("0123456789.") ;

				if (i == 1) {
					i = lv_arrayField[li_rateInd].findFirstCharNotInList("ABCDEFGHIJKLMNOPQRSTUVWXYZ") ;
					if (i > 0) {
						md_IR_cur_id[mi_IR_I] = lv_arrayField[li_rateInd].For(i-1);
						md_IR_local_rate[mi_IR_I] = lv_arrayField[li_rateInd].At(i);
					}
				}

				else if (i > 1)
				{
					md_IR_cur_id[mi_IR_I]     = lv_arrayField[li_rateInd].At(i) ;
					md_IR_local_rate[mi_IR_I] = lv_arrayField[li_rateInd].For(i-1) ;
				}
				else
				{
					md_IR_cur_id[mi_IR_I]     = AB_null ;
					md_IR_local_rate[mi_IR_I] = lv_arrayField[li_rateInd] ;
				}

				md_IR_booked_rate[mi_IR_I]   = md_IR_local_rate[mi_IR_I] ;
			}
		}
		// RATE END----------------------

		processCommonHotel(pt_section) ;

		// For Non arc ticketing

		AB_Char    lc_noofSeats(2) ;
		AB_Text    lt_seatData ;
		AB_Text    lt_ftpMiles ;
		AB_Char    lc_M5link(1) ;
		AB_Char    lc_itinLine(2) ;

		lc_M5link = pt_section.At(6).For(1) ;

		lc_itinLine                   = pt_section.At(3).For(2) ;

   	if (mv_itinTransport.IsNull())
      	mv_itinTransport = lc_itinLine ;
   	else
      	mv_itinTransport = mv_itinTransport + lc_itinLine ;

		// fill Personitin

		lc_noofSeats = "00" ;
		lt_seatData  = " " ;

		lt_ftpMiles = AB_null ;


		fillConsumDetail(lc_noofSeats,
					 md_IR_itin_type[mi_IR_I],
				 	 lc_itinLine,
					 lt_seatData,
					 lt_ftpMiles) ; /* ftp_miles */

		md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;

		if (md_IR_unit_type[mi_IR_I].IsNull() )
			md_IR_unit_type[mi_IR_I] = "SGLB" ;

   	mi_IR_I++ ;
	}

   EXIT;
}


//-----------------------------------------------------------------------------

void CIUR::CreateRentalRemarks(AB_Varchar &pv_field)
{
   ENTER;

   md_IRMK_itin_type[mi_IRMK_I]   = md_IR_itin_type[mi_IR_I] ;
   md_IRMK_ren_line_number[mi_IRMK_I] = md_IR_line_number[mi_IR_I] ;
   md_IRMK_remark[mi_IRMK_I]         = pv_field ;
   md_IRMK_pax_id[mi_IRMK_I++]       = AB_null;

   EXIT;
}


//------------------------------------------------------------------------------

void CIUR::fill_IrRentalCar(AB_Text &pt_section)
{
   ENTER;

   int 			 li_pos,
		 			 li_count ;
   AB_Varchar   lv_field(255) ;
   D_amount     lm_amount ;
   D_currency   lm_dummy ;
   AB_Date 		 ld_tempDate ;
	AB_Varchar   lv_varField(255) ;
	V_AB_Varchar lv_arrayField(100,100) ;
	AB_Varchar   lv_tempField(255) ;
	AB_Varchar   lv_delim(4) ;
	AB_Char	    itin_type = "C" ;
	AB_Char      lc_year(2) ;
	string       labelValue;
   AB_Char        lc_comm_rate;

   md_IR_itin_type[mi_IR_I]      = "C" ;
   md_IR_free_mileage[mi_IR_I] = AB_null ;

   md_IR_source[mi_IR_I] = "M" ;

   md_IR_line_number[mi_IR_I] = pt_section.At(3).For(2) ;

	mv_service_status             = pt_section.At(8).For(2) ;
   md_IR_service_status[mi_IR_I] = pt_section.At(8).For(2) ;
	//------ STS LABEL(G9150) START------------------
	if ( pRemarkParser->getData("LABELS", "STS", labelValue))
	{
	   mv_service_status             = labelValue.data() ;
		md_IR_service_status[mi_IR_I] = labelValue.data() ;
	}
	//------ STS LABEL(G9150) END------------------

	if ( md_IR_service_status[mi_IR_I] != "GK" && md_IR_service_status[mi_IR_I] != "BK" )
		md_IR_source[mi_IR_I] = "A" ;

   lc_year = mt_pnrDate.get_default_year(pt_section.At(10).For(5)) ;
	lc_year.rightJustify('0') ;
   md_IR_start_date[mi_IR_I] = pt_section.At(10).For(5) +  lc_year ;

   md_IR_unit_ct[mi_IR_I] = pt_section.At(18).For(1) ;

   md_IR_sup_id[mi_IR_I] = pt_section.At(19).For(2) + "C" ;

  	md_IR_confirmation[mi_IR_I] = pt_section.At(21).For(15) ;

	lv_varField = pt_section.At(36) ;

	li_pos = lv_arrayField.parseStringMultDel(lv_varField,"/") ;

	md_IR_origin_city_id[mi_IR_I] = lv_arrayField[1] ;
	//------ SC LABEL(G9150) ------------------
	RP_saveDataTo(	"LABELS", "SC",	md_IR_origin_city_id[mi_IR_I]);

	lv_tempField = lv_arrayField[2] ;

	if (lv_tempField.For(5).NotIsNull() )
	{
		li_count = 1 ;

      //25FEB99 - 1676 - v1158 - SC - Change mt_pnrDate as md_IR_start_date.

//   	lc_year = mt_pnrDate.get_default_year(lv_tempField.For(5)) ;
      ld_tempDate = md_IR_start_date[mi_IR_I] ;
   	lc_year = ld_tempDate.get_default_year(lv_tempField.For(5)) ;
		lc_year.rightJustify('0') ;
		md_IR_end_date[mi_IR_I] = lv_tempField.For(5) + lc_year ;
	}
	else
	{
		 md_IR_end_date[mi_IR_I] =  md_IR_start_date[mi_IR_I]  ;
		 li_count = 0 ;
	}

   md_IR_rate_duration[mi_IR_I] = "D" ;

   md_IR_unit_type[mi_IR_I] = lv_arrayField[li_count + 2] ;

   lv_field = pt_section.getNthBracketedField(1, "/DO-", "/") ;

   if (lv_field.NotIsNull()) {
		md_IR_dropoff_location[mi_IR_I] = lv_field;
      md_IR_dest_city_id[mi_IR_I] = lv_field ;
	} else {
		md_IR_dest_city_id[mi_IR_I] = md_IR_origin_city_id[mi_IR_I] ;
	}

   lv_field = pt_section.getNthBracketedField(1, "PUP-", "/") ;
   if (lv_field.NotIsNull()) {
      md_IR_pickup_location[mi_IR_I] = lv_field;
	}

   lv_field = pt_section.getNthBracketedField(1, "ARR-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_start_time[mi_IR_I] = CheckTime(lv_field);

   lv_field = pt_section.getNthBracketedField(1, "RET-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_end_time[mi_IR_I] = CheckTime(lv_field);



   lv_field = pt_section.getNthBracketedField(1, "/CD-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_corporate_disc[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/FT-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_fup[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/ID-", "/") ;
	if (lv_field.NotIsNull()) {
      lv_field.removeLeadAndTrailBlanks();
      if ((li_pos = lv_field.findSubstring(" ")) > 0) {
         lv_field = lv_field.For(li_pos-1);
      }

      if ((li_pos = lv_field.findSubstring(",")) > 0) {
         lv_field = lv_field.For(li_pos-1);
      }

      md_IR_corporation_id[mi_IR_I] = lv_field;
   }

   lv_field = pt_section.getNthBracketedField(1, "/LC-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_location[mi_IR_I] = lv_field ;

   lv_field = pt_section.getNthBracketedField(1, "/PH-", "/") ;
   if (lv_field.NotIsNull())
	{
	  editPhoneNumber(lv_field, md_IR_phone_acode[mi_IR_I], md_IR_phone_no[mi_IR_I], true);
	}


	if (pt_section.findSubstring("/G/") == 0) {
		lv_field = pt_section.getNthBracketedField(1, "/G", "/") ;
		if (lv_field.IsNull()) {
			lv_field = pt_section.getNthBracketedField(1, "/G", "!") ;
		}
		if (lv_field.NotIsNull())
			md_IR_guarantee[mi_IR_I] = lv_field ;
	}

   lv_field = pt_section.getNthBracketedField(1, "/RC-", "/") ;
   if (lv_field.NotIsNull())
      md_IR_rate_code[mi_IR_I] = lv_field ;

	lv_delim = "/DC-" ;
   parseRate(pt_section, lv_delim, lm_dummy, lm_amount, lv_field) ;
   md_IR_dropoff_charge[mi_IR_I] = D_char_numeric(lm_amount) ;

	lv_delim = "/RQ-" ;
	parseRate(pt_section, lv_delim, md_IR_cur_id[mi_IR_I], lm_amount, lv_field) ;
	md_IR_local_rate[mi_IR_I] = lm_amount ;

	if (lv_field.NotIsNull() )
	{
		// Get Other Car rate info
		getOtherCarRateInfo(lv_field) ;
	}
	else
	{
		lv_delim = "/RG-" ;
		parseRate(pt_section, lv_delim, md_IR_cur_id[mi_IR_I], lm_amount, lv_field) ;
		md_IR_local_rate[mi_IR_I] = lm_amount ;

		if (lv_field.NotIsNull() )
		{
			// Get Other Car rate info
			getOtherCarRateInfo(lv_field) ;
		}
		// SCR 219958 START
		else
		{
			lv_delim = "/RT-" ;
			parseRate(pt_section, lv_delim, md_IR_cur_id[mi_IR_I], lm_amount, lv_field) ;
			md_IR_local_rate[mi_IR_I] = lm_amount ;

			if (lv_field.NotIsNull() )
			{
				// Get Other Car rate info
				getOtherCarRateInfo(lv_field) ;
			}
		}
		// SCR 219958 END
	}

	if (md_IR_local_rate[mi_IR_I].NotIsNull())
		md_IR_booked_rate[mi_IR_I] = md_IR_local_rate[mi_IR_I] ;

   IrCarRemarks(lv_field) ;

   // getAddressData(pt_section) ;

	if ( mv_actionCode.NotIsNull() )
	{
         cout << "FG: fill_IrRentalCar: mv_actionCode= " << mv_actionCode << flush << endl ;
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

   getOverrideRates(itin_type,
          			  md_IR_start_date[mi_IR_I],
          			  md_IR_saving_code[mi_IR_I],
          			  md_IR_local_rate[mi_IR_I],
          			  md_IR_regular_rate[mi_IR_I],
          			  md_IR_booked_rate[mi_IR_I],
          			  md_IR_commissionable[mi_IR_I],
                      md_IR_preferred[mi_IR_I],
          			  gv_IrCarData, mi_IrCarIndex,
                    lc_comm_rate) ; //US498 - FG

	// For Non arc ticketing

	AB_Char    lc_noofSeats(2) ;
	AB_Text    lt_seatData ;
	AB_Text    lt_ftpMiles ;
	AB_Char    lc_M5link(1) ;
	AB_Char    lc_itinLine(2) ;

	lc_M5link = pt_section.At(6).For(1) ;

	lc_itinLine                   = pt_section.At(3).For(2) ;

   if (mv_itinTransport.IsNull())
      mv_itinTransport = lc_itinLine ;
   else
      mv_itinTransport = mv_itinTransport + lc_itinLine ;

	// fill Personitin

	lc_noofSeats = "00" ;
	lt_seatData  = " " ;

	lt_ftpMiles = AB_null ;


	fillConsumDetail(lc_noofSeats,
					 md_IR_itin_type[mi_IR_I],
				 	 lc_itinLine,
					 lt_seatData,
					 lt_ftpMiles) ; /* ftp_miles */

	md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;


   mi_IR_I++ ;

   EXIT;
}


//-----------------------------------------------------------------------------

void CIUR::IrCarRemarks(AB_Varchar &pv_field)
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



//------------------------------------------------------------------------------

// GAP US089 START
void CIUR::createM8Remarks(CPNR &po_PNR)
// GAP US089 END
{
   ENTER;

   AB_Text     lt_segment ;
   AB_Text     lt_remark ;
	int         li_dash_pos ;
	int         li_CR_pos ;
	AB_Varchar lv_udidNo(4) ;

   // GAP US089 START
   AB_Boolean  lb_udidFound;
   // GAP US089 END

   po_PNR.resetSegment("M8") ;
   lt_segment = po_PNR.getNextSegment("M8");

   while (lt_segment.NotIsNull())
   {
      // GAP US089 START
      lb_udidFound   = AB_false;
      // GAP US089 END

		if ( lt_segment.At(5).For(1) == "U" )
		{
			lt_remark = lt_segment.At(6) ;

			li_dash_pos = lt_remark.findSubstring("-") ;
			li_CR_pos   = lt_remark.findSubstring("!") ;

			if ( li_CR_pos > 0 )
				lt_remark = lt_remark.For(li_CR_pos-1);

			if ( li_dash_pos <= 4 )
			{
				lv_udidNo = lt_remark.For(li_dash_pos-1) ;

				if ( lv_udidNo.For(1) == "*" )
				{
					lv_udidNo = lv_udidNo.At(2) ;
				}

      		if (lv_udidNo.findFirstCharNotInList("0123456789") == 0)
				{
					if ( lt_remark.At(li_dash_pos+1).For(1) != " " &&
							lv_udidNo != "0" && lv_udidNo != "00" )
					{
						// GAP US111 START
						createUDID(lv_udidNo, lt_remark.At(li_dash_pos+1), AB_false);
						// GAP US111 END

                  // GAP US089 START
                  lb_udidFound   = AB_true;
                  // GAP US089 END
					}
				}
			}
		}

      lt_segment = po_PNR.getNextSegment("M8");
   }

	// GAP US111 START
	processNonMatrixUDID(po_PNR);
	// GAP US111 END

   EXIT;
}

//-----------------------------------------------------------------------------

// GAP US111 START
AB_Boolean CIUR::processNonMatrixUDID(CPNR &po_PNR)
{
   ENTER;

   AB_Text     lt_segment ;
   AB_Text     lt_remark ;
	int         li_slash_pos ;
	int         li_CR_pos ;
	AB_Varchar lv_udidNo(4) ;

   AB_Boolean  lb_udidFound;

   po_PNR.resetSegment("M8") ;
   lt_segment = po_PNR.getNextSegment("M8");

   while (lt_segment.NotIsNull())
   {
      lb_udidFound   = AB_false;

		if ( lt_segment.At(5).For(3) == "S*U" )
		{
			lt_remark = lt_segment.At(8) ;

			li_slash_pos = lt_remark.findSubstring("/") ;
			li_CR_pos   = lt_remark.findSubstring("!") ;

			if ( li_CR_pos > 0 )
				lt_remark = lt_remark.For(li_CR_pos-1);

			if ( li_slash_pos <= 3 )
			{
				lv_udidNo = lt_remark.For(li_slash_pos-1) ;

      		if (lv_udidNo.findFirstCharNotInList("0123456789") == 0)
				{
					if ( lt_remark.At(li_slash_pos+1).For(1) != " " &&
							lv_udidNo != "0" && lv_udidNo != "00" )
					{
						createUDID(lv_udidNo, lt_remark.At(li_slash_pos+1), AB_false);

                  lb_udidFound   = AB_true;
					}
				}
			}
		}

      lt_segment = po_PNR.getNextSegment("M8");
   }

   return lb_udidFound;

}
// GAP US111 END

//-----------------------------------------------------------------------------

void CIUR::parseM8SavingCodeFullFare(CPNR &po_PNR) {
   ENTER;

   AB_Text     lt_segment ;
   int         li_pos ;

   po_PNR.resetSegment("M8") ;
   lt_segment = po_PNR.getNextSegment("M8");

   while (lt_segment.NotIsNull()) {
      if (lt_segment.findSubstring("/") == 0 &&
			 lt_segment.At(5).For(2) == "S*") {

         if (md_savingCode.IsNull()) {

            // SCR 207781 START
            // set md_savingCode != AB_null to avoid picking this
            // up from M9 later
            // md_savingCode = "";
            // SCR 207781 END

            lt_segment = lt_segment.At(7);

            if (lt_segment.For(1).isAllAlpha()) {
               md_savingCode = lt_segment.For(1);
               lt_segment = lt_segment.At(2);
            }
         }

         if (md_fullFare.IsNull()) {

            // SCR 207781 START
            // set md_fullFare != AB_null to avoid picking this
            // up from M9 later
            // md_fullFare = "";
            // SCR 207781 END

            if (lt_segment.For(1).isAllNumeric()) {
               lt_segment = lt_segment + "X";
               li_pos = lt_segment.findFirstCharNotInList("0123456789.");
               lt_segment = lt_segment.For(li_pos-1);
               if ((li_pos = lt_segment.findSubstring(".")) != 0) {
                  lt_segment = lt_segment + "00";
                  lt_segment = lt_segment.For(li_pos+2);
               }
               md_fullFare = lt_segment;
            }
         }

         break;   // only check the first M8..S*
      }

      lt_segment = po_PNR.getNextSegment("M8");
   }

   EXIT;
}


//-----------------------------------------------------------------------------

void CIUR::storeEntitleData(CPNR &po_PNR)
{
   ENTER;

   AB_Text     lt_segment ;

   po_PNR.resetSegment("M4") ;
   lt_segment = po_PNR.getNextSegment("M4");
	mi_entitleIndex = 0 ;

   while (lt_segment.NotIsNull())
   {
		gd_entitleData[++ mi_entitleIndex] = lt_segment ;

      lt_segment = po_PNR.getNextSegment("M4");
   }
   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::parseRate(AB_Text &pt_section, AB_Varchar &pm_start_delim, D_currency &pm_cur_id, D_amount &pm_amount, AB_Varchar &pt_field)
{
   ENTER;

   int li_pos ;
   D_char_numeric lm_rate ;
   AB_Char lc_test ;
   AB_Money lm_amount ;
   AB_Varchar lv_rawAmount(15) ;
   AB_BOOL lb_ok ;
   D_amount ld_test ;

   pt_field = pt_section.getNthBracketedField(1, pm_start_delim.to_string(), "/") ;

   if (pt_field.NotIsNull())
   {
      lc_test = pt_field.At(1).For(1) ;
      if (lc_test.isAllAlpha())
      {
         pm_cur_id = pt_field.For(3) ;
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
         ld_test = AB_Decimal(lv_rawAmount/AB_Decimal(100.00)) ;
         lv_rawAmount = ld_test ;
      }

      lm_amount = lv_rawAmount.to_money("#######.##", lb_ok) ;

      if (!lb_ok)
         pm_amount = "0.00" ;
      else
         pm_amount = lm_amount.to_string() ;
   }
}

//------------------------------------------------------------------------------

void CIUR::getOtherCarRateInfo(AB_Varchar &pv_field)
{
   ENTER;

   int li_pos, li_pos2 ;
   AB_Char lc_temp(255), lc_duration(2) ;
	AB_Varchar lv_temp(255);

	li_pos = 0 ;

	// pv_field can be:
	// CAD194.56 433F BR .19KM
	// CAD258.00 UNL WY

	// EUR694.50 700F WY XD115.75 100F XH115.75 100F .30KM
	// USD263.99 UNL DY XH131.75 UNL
	// RG-USD502.00 UNL WY

   li_pos = pv_field.findFirstCharNotInList("0123456789.") ;

   if (li_pos == 1)
   {
      lv_temp = pv_field.At(4) ;
      li_pos = lv_temp.findFirstCharNotInList("0123456789.") ;
   }
   else
      return ;

   if (li_pos > 0)
   {
		 lv_temp = lv_temp.At(li_pos);

   	if (md_IR_free_mileage[mi_IR_I].IsNull()) {
         if ( lv_temp.For(4) == " UNL" )
         {
				 md_IR_free_mileage[mi_IR_I] = "UNLIM";
             lv_temp = lv_temp.At(5);
         } else {
				li_pos = lv_temp.findFirstCharNotInList(" ");
				if (li_pos > 0) {
					lv_temp = lv_temp.At(li_pos);
					li_pos = lv_temp.findFirstCharNotInList("0123456789.");
					if (lv_temp.At(li_pos).For(1) == "F") {
						md_IR_free_mileage[mi_IR_I] = lv_temp.For(li_pos - 1);
						lv_temp = lv_temp.At(li_pos+1);
					}
				}
			}
		}

		// Search for rate duraion

		if ((li_pos = pv_field.findSubstring(" WK ")) > 0 ||
			 pv_field.At(pv_field.length()-2) == " WK"	  ||
			 (li_pos = pv_field.findSubstring(" WY ")) > 0 ||
			 pv_field.At(pv_field.length()-2) == " WY")
		{
			md_IR_rate_duration[mi_IR_I] = "W" ;
		}
		else if ((li_pos = pv_field.findSubstring(" WE ")) > 0 ||
					pv_field.At(pv_field.length()-2) == " WE"		 ||
					(li_pos = pv_field.findSubstring(" WD ")) > 0 ||
					pv_field.At(pv_field.length()-2) == " WD")
		{
			md_IR_rate_duration[mi_IR_I] = "E" ;
		}
		else if ((li_pos = pv_field.findSubstring(" MO ")) > 0 ||
					pv_field.At(pv_field.length()-2) == " MO"		 ||
					(li_pos = pv_field.findSubstring(" MY ")) > 0 ||
					pv_field.At(pv_field.length()-2) == " MY")
		{
			md_IR_rate_duration[mi_IR_I] = "M" ;
		}
		else if ((li_pos = pv_field.findSubstring(" BR ")) > 0 ||
					pv_field.At(pv_field.length()-2) == " BR")
		{
			md_IR_rate_duration[mi_IR_I] = "X" ;
		}
		else if ((li_pos = pv_field.findSubstring(" D ")) > 0 ||
					pv_field.At(pv_field.length()-1) == " D"		 ||
					(li_pos = pv_field.findSubstring(" DY ")) > 0 ||
					pv_field.At(pv_field.length()-2) == " DY")
		{
			md_IR_rate_duration[mi_IR_I] = "D" ;
		}
   }

	li_pos2 = li_pos;

   li_pos = lv_temp.findSubstring("KM") ;

   if (li_pos > 0)
      md_IR_free_mile_code[mi_IR_I] = "K" ;
   else
   {
      li_pos = lv_temp.findSubstring("MI") ;
      if (li_pos > 0)
         md_IR_free_mile_code[mi_IR_I] = "M" ;
   }

	if (md_IR_cur_id[mi_IR_I] == "USD")
		md_IR_free_mile_code[mi_IR_I] = "M" ;
	else
		md_IR_free_mile_code[mi_IR_I] = "K" ;

	lv_temp = pv_field;
	if (md_IR_ex_day_charge[mi_IR_I].IsNull())
	{
		li_pos = lv_temp.findSubstring("XD") ;
		if (li_pos > 0)
		{
			lv_temp = lv_temp.At(li_pos + 2) ;
			li_pos = lv_temp.findFirstCharNotInList("0123456789.") ;
			md_IR_ex_day_charge[mi_IR_I] = lv_temp.For(li_pos - 1) ;
		}
	}


	lv_temp = pv_field;
	if (md_IR_ex_hr_charge[mi_IR_I].IsNull())
	{
		li_pos = lv_temp.findSubstring("XH") ;
		if (li_pos > 0)
		{
			lv_temp = lv_temp.At(li_pos + 2) ;
			li_pos = lv_temp.findFirstCharNotInList("0123456789.") ;
			md_IR_ex_hr_charge[mi_IR_I] = lv_temp.For(li_pos - 1) ;
		}
	}

	lv_temp = pv_field.At(li_pos2);
	lv_temp.removeLeadAndTrailBlanks();

	if (lv_temp.findSubstring("UNL") > 0) {
		// keep md_IR_ex_mile_charge[mi_IR_I] as NULL
		// md_IR_ex_mile_charge[mi_IR_I] = "UNLIM";
	} else {
		if ((li_pos = lv_temp.findSubstring("KM")) > 0 ||
		 	 (li_pos = lv_temp.findSubstring("MI")) > 0) {

			li_pos = lv_temp.getReverseIndex(" ", li_pos);
			lv_temp = lv_temp.At(li_pos+2);

			li_pos = lv_temp.findFirstCharNotInList("0123456789.");
			lv_temp = lv_temp.For(li_pos - 1);

			if (lv_temp.For(1) == ".") {
				lv_temp = "0" + lv_temp;
			}

			md_IR_ex_mile_charge[mi_IR_I] = lv_temp;
		}
	}

   EXIT;
}

//--------------------------------------------------------------------------

void CIUR::parseM5ticket(AB_Varchar   &pv_tktString,
								 V_AB_Varchar &pva_arrayField,
								 AB_Integer   &pi_arraySize,
								 AB_Integer   &pi_paxId,
								 AB_Integer   &pi_docId)
{
	ENTER;

   int                li_pos,
							 li_taxCounter ,
							 i ,li_noOfPax ;
   AB_Text            lt_section ;
   AB_Varchar         lt_formOfPay(50) ;
	AB_Varchar			 lv_taxString(30) ;
	D_amount           ld_comm ;
	AB_Text				 tkt;
	md_ID_form[mi_ID_I]        = pva_arrayField[1].For(4) ;
  	md_ID_ticket_no[mi_ID_I]   = pva_arrayField[1].At(5).For(6) ;

	if ( pva_arrayField[5] == "PER" )
	{
		// Multiply base cost Tax, commission doller amount by no of Pax

		li_noOfPax = mi_IP_I ;

	}
	else  if ( pva_arrayField[6] == "PER" )
	{
		// Multiply base cost Tax, commission doller amount by no of Pax

		li_noOfPax = mi_IP_I  ;
	}

	else  if ( pva_arrayField[7] == "PER" )
	{
		// Multiply base cost Tax, commission doller amount by no of Pax

		li_noOfPax = mi_IP_I  ;
	}
	else
	{
		li_noOfPax = 1 ;
	}

	md_ID_cur_id[mi_ID_I]      = "   " ;

	//md_ID_amounts_source[mi_ID_I] = "M5" ;

	pva_arrayField[2].leftJustify() ;
	md_ID_commission[mi_ID_I]  = pva_arrayField[2] ;

	li_pos       = md_ID_commission[mi_ID_I].findSubstring("P") ;
	if (li_pos > 0)
	{
		md_ID_comm_rate[mi_ID_I]   = md_ID_commission[mi_ID_I].At(li_pos + 1 ) ;
		ld_comm = md_ID_comm_rate[mi_ID_I] ;
		md_ID_comm_rate[mi_ID_I]  = ld_comm;
		md_ID_commission[mi_ID_I]  = " " ;
	}
	else
	{
		md_ID_commission[mi_ID_I].leftJustify() ;
		ld_comm = md_ID_commission[mi_ID_I] ;
		ld_comm = ld_comm * AB_Integer(li_noOfPax) ;
		md_ID_commission[mi_ID_I] = ld_comm ;
	}

   if ( pva_arrayField[3].For(3).isAllAlpha() ){
			 md_ID_cur_id[mi_ID_I] = pva_arrayField[3].For(3);
          md_ID_base_cost[mi_ID_I] = pva_arrayField[3].At(4);
			 md_ID_base_cost[mi_ID_I].leftJustify();
	}
	else{
			md_ID_cur_id[mi_ID_I] = "USD" ;
	      md_ID_base_cost[mi_ID_I] = pva_arrayField[3] ;
	      md_ID_base_cost[mi_ID_I].leftJustify();
	}

	D_amount ld_baseCost ;
	ld_baseCost = md_ID_base_cost[mi_ID_I] ;
	ld_baseCost = ld_baseCost * AB_Integer(li_noOfPax) ;
	md_ID_base_cost[mi_ID_I] = ld_baseCost ;

/*
	if ( md_ID_base_cost[mi_ID_I].For(1).isAllAlpha())
		md_ID_cur_id[mi_ID_I] = md_ID_base_cost[mi_ID_I].For(3) ;
	else
		md_ID_cur_id[mi_ID_I] = "USD" ;
*/

	LOG(logDEBUG) << md_ID_base_cost[mi_ID_I]  ;

	// Process Taxes

	li_taxCounter = 0 ;

	lv_taxString = pva_arrayField[4] ;

	lv_taxString.leftJustify() ;

	if (lv_taxString != "0.00" )
	{
		li_taxCounter++ ;
		md_ITD_pax_id[mi_ITD_I]       = pi_paxId ;
		md_ITD_line_number[mi_ITD_I]  = pi_docId ;
		md_ITD_tax_id[mi_ITD_I]       = "XT" ;
      md_ITD_tax_source[mi_ITD_I]   = "M5";
		md_ITD_tax_amount[mi_ITD_I] = lv_taxString ;

		D_amount ld_tax1 ;
		ld_tax1 = md_ITD_tax_amount[mi_ITD_I] ;
		ld_tax1 = ld_tax1 * AB_Integer(li_noOfPax) ;
		md_ITD_tax_amount[mi_ITD_I] = ld_tax1 ;

		mi_ITD_I++ ;
	}

	lv_taxString = pva_arrayField[5] ;

	lv_taxString.leftJustify() ;

	LOG(logDEBUG) << "AM======= lv_taxString = ["  << lv_taxString << "]" ;

	if (lv_taxString != "ONE" && lv_taxString != "PER" && lv_taxString != "ALL")
	{

		if (lv_taxString.At(2) != "0.00" )
		{
			// For US If there is a 2nd Tax it goes as XT (from ROW project on -> XG)

			li_taxCounter++ ;
			md_ITD_line_number[mi_ITD_I]  = pi_docId ;
			md_ITD_pax_id[mi_ITD_I]       = pi_paxId ;
			md_ITD_tax_id[mi_ITD_I]       = "XG" ;
         md_ITD_tax_source[mi_ITD_I]   = "M5";
			md_ITD_tax_amount[mi_ITD_I] = lv_taxString.At(2) ;
			md_ITD_tax_amount[mi_ITD_I].leftJustify() ;

			D_amount ld_tax2 ;
			ld_tax2 = md_ITD_tax_amount[mi_ITD_I] ;
			ld_tax2 = ld_tax2 * AB_Integer(li_noOfPax) ;
			md_ITD_tax_amount[mi_ITD_I] = ld_tax2 ;

			mi_ITD_I++ ;
		}
	}

	if ( lv_taxString == "ONE" ||
		  lv_taxString == "PER" ||
		  lv_taxString == "ALL" )
		li_pos = 6 ;
	else
	{
		if ( pva_arrayField[7] == "ONE" ||
			  pva_arrayField[7] == "PER" ||
			  pva_arrayField[7] == "ALL")
			li_pos = 8 ;
		else
			li_pos = 7 ;
	}

	LOG(logDEBUG) << "AM======= li_pos = ["  << li_pos << "]" ;
	// Get Form of payment

	lt_formOfPay = pva_arrayField[li_pos] ;

	// Get optional cc exp date

	AB_Char    lc_ccExpDate(4);
   int        li_expPos;

	li_expPos = pv_tktString.findSubstring("-EXP$");

	if ( li_expPos > 0 &&
		  pv_tktString.At(li_expPos+5).For(2).isAllNumeric()  &&
		  pv_tktString.At(li_expPos+7).For(1) == AB_Text("/") &&
		  pv_tktString.At(li_expPos+8).For(2).isAllNumeric()    )
   {
      lc_ccExpDate = pv_tktString.At(li_expPos+5).For(2) +
                     pv_tktString.At(li_expPos+8).For(2) ;
	}
	else
	{
		set <Text2Text>::iterator it;
		tkt = pva_arrayField[1].For(10);
		it = M02_TktNo2ExpDate.find(Text2Text(tkt, ""));
		if ( it != M02_TktNo2ExpDate.end() )
		{
				LOG(logDEBUG) << "tkt:[" << tkt << "]" ;
				LOG(logDEBUG) << "FOUND in MAP:" << it->_key << "\t" << it->_value ;
				lc_ccExpDate = it->_value;
		}
		else
		{
				LOG(logDEBUG) << "tkt:[" << tkt << "]" ;
				LOG(logDEBUG) << "NOT FOUND in MAP:"  ;
				lc_ccExpDate = AB_null;
		}
	}

	processMiscellaneousFee(lt_formOfPay, pi_paxId, lc_ccExpDate);

	fillFOPM5(pi_paxId,
				 pi_docId,
				 lt_formOfPay,
				 lc_ccExpDate) ;

	li_pos = li_pos + 1 ;

	LOG(logDEBUG) << "AM======= li_pos = ["  << li_pos << "]" ;
	LOG(logDEBUG) << "AM======= pva_arrayField[li_pos].For(1) = ["  << pva_arrayField[li_pos].For(1) << "]" ;
  	md_ID_no_of_books[mi_ID_I] = pva_arrayField[li_pos].For(1) ;

	//  next field could be coupons numbers
	li_pos = li_pos + 1 ;
	if ( PCRE::pcre_match("[1234A]", pva_arrayField[li_pos].For(1).to_string()))
	{
	  LOG(logDEBUG) << "AM=== coupons found in M5 [" << pva_arrayField[li_pos] << "]";
	  parseCouponList(pva_arrayField[li_pos], tkt, md_ID_no_of_books[mi_ID_I]);
	}

	EXIT;
}

//---------------------------------------------------------------------------
// GAP US092 START
void CIUR::processMiscellaneousFee(AB_Text &pt_data, AB_Integer &pi_paxId, AB_Char &pc_ccExpDate)
{
	ENTER;

   AB_Text           lt_data ;
	AB_Char				lc_M5_pax(5);
	AB_Char				lc_FM_pax(5);
	int					li_pax_pos;
	int					li_name_pos;
	AB_Char				lc_ccExpDate(4);
	AB_Boolean			lb_processed;

	lb_processed = AB_false;

	LOG(logDEBUG)  << "SFM >>> processMiscellaneousFee START = '" << pt_data << "'"  ;

	lt_data = pt_data.to_string();

	LOG(logDEBUG)  << "SFM >>> / is found"  ;

	li_pax_pos = lt_data.findSubstring(" ");

	if ( li_pax_pos > 0 )
	{
		LOG(logDEBUG)  << "SFM >>> ' ' is found"  ;

		lt_data = lt_data.At(li_pax_pos+1);

		if ( lt_data.length() > 0 )
		{
			LOG(logDEBUG)  << "SFM >>> pax is found"  ;

			li_name_pos = lt_data.findFirstCharNotInList(".0123456789");
			if ( li_name_pos > 0 &&
				  li_name_pos <= 6 )
			{
				LOG(logDEBUG)  << "SFM >>> name is found"  ;

				lc_M5_pax = lt_data.For(li_name_pos-1);

				lb_processed = AB_true;

				for ( int li_index = 0; li_index < mi_IRMK_I; li_index++ )
				{
					if ( 	md_IRMK_remark[li_index].length() > 0 	&&
							md_IRMK_type[li_index] == "SFM" )
					{
						lc_FM_pax = md_IRMK_remark[li_index].For(5);

						if ( 	lc_FM_pax != "     " )
						{
							if ( lc_FM_pax == lc_M5_pax )
							{
								md_IRMK_remark[li_index] = AB_Char("     ") + md_IRMK_remark[li_index].At(6);
 								md_IRMK_pax_id[li_index] = pi_paxId;
							}
/*							else
							{
								md_IRMK_pax_id[li_index] = md_IP_pax_id[0];
							}
							*/
						}

					}
				}
			}
		}
	}

	if ( lb_processed == AB_false )
	{
		for ( int li_index = 0; li_index < mi_IRMK_I; li_index++ )
		{
			if ( 	md_IRMK_remark[li_index].length() > 0 	&&
					md_IRMK_type[li_index] == "SFM" )
			{
				lc_FM_pax = md_IRMK_remark[li_index].For(5);
/*
				if ( lc_FM_pax != "     " )
				{
					md_IRMK_pax_id[li_index] = md_IP_pax_id[0];
				}

				*/
			}
		}
	}
	EXIT;
}
// GAP US092 END
//---------------------------------------------------------------------------

void CIUR::fillFOPM5(AB_Integer &pi_paxId,
							AB_Integer &pi_docLine,
							AB_Varchar &pt_formOfPay,
							AB_Char    &pc_ccExpDate)
{
	ENTER;

	AB_Varchar		lc_fp(30) ;
	AB_Char        lc_secType(2) ;

	lc_secType = "M5" ;

	md_IRP_amount[mi_IRP_I] = md_ID_base_cost[mi_ID_I] ;
	md_IRP_pax_id[mi_IRP_I] = pi_paxId ;
	md_IRP_line_number[mi_IRP_I] = pi_docLine ;
	md_IRP_pay_id[mi_IRP_I] = AB_Integer(1) ;

	if ( pt_formOfPay.IsNull() )
		md_IRP_form_of_pay[mi_IRP_I] = "CA" ;
	else
		translate_fp(pt_formOfPay,lc_secType) ;

	if (md_IRP_form_of_pay[mi_IRP_I] != "CC")
	{
		md_IRP_cc_exp_date[mi_IRP_I] = AB_null ;
		md_IRP_auth_no[mi_IRP_I] = AB_null ;
		md_IRP_auth_source[mi_IRP_I] =  AB_null ;
	}
	else
	{
		md_IRP_cc_exp_date[mi_IRP_I] = pc_ccExpDate;

		// TKX CC EXP DATE START
		if (md_TKX_cc_exp_date.NotIsNull()) {
			md_IRP_cc_exp_date[mi_IRP_I] = md_TKX_cc_exp_date;
		}
		// TKX CC EXP DATE END
	}

	// DEBUG
	LOG(logDEBUG) << "\nmd_IRP_form_of_pay["<<mi_IRP_I<<"]:"
		  << md_IRP_form_of_pay[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_cc_exp_date["<<mi_IRP_I<<"]:"
		  << md_IRP_cc_exp_date[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_cc_id["<<mi_IRP_I<<"]:"
		  << md_IRP_cc_id[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_cc_no["<<mi_IRP_I<<"]:"
		  << md_IRP_cc_no[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_pax_id["<<mi_IRP_I<<"]:"
		  << md_IRP_pax_id[mi_IRP_I]  ;
	LOG(logDEBUG) << "md_IRP_pay_id["<<mi_IRP_I<<"]:"
		  << md_IRP_pay_id[mi_IRP_I]  ;

	mi_IRP_I++ ;
   EXIT;
}

//-------------------------------------------------------------------------

void CIUR::updIcdConnectInd()
{
   ENTER;

	AB_Integer	 li_entitle ;
	int       	 li_counter ;
	AB_Text      lt_entitleData ;
	AB_Integer   li_value ;

	li_entitle = 1 ;

	while ( li_entitle <= AB_Integer(mi_entitleIndex) )
	{
		li_counter = 0 ;
		lt_entitleData = gd_entitleData[li_entitle] ;
		li_value = lt_entitleData.At(3).For(2) ;

		while (li_counter < mi_ICD_I)
		{
			if ((md_ICD_itin_line_number[li_counter].to_long() + 1) == li_value.to_long())
				md_ICD_connection[li_counter] = lt_entitleData.At(8).For(1) ;

			li_counter++ ;
		}

		li_entitle = li_entitle + AB_Integer(1) ;
	}
   EXIT;
}

//-----------------------------------------------------------------------------

void CIUR::fillIrRailM3(AB_Text &pt_segment, CPNR &po_PNR)
{
   ENTER;

   int        li_counter,
			     li_pos ;
   AB_Char    lc_date(5) ;
   AB_Char    lc_service(5) ;
   AB_Char    lc_itinLine(2) ;
   AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_remark ;
	AB_Text 	  lt_ftpMiles ;
	AB_Text 	  lt_seatData ;
	AB_Char    lc_M5link(1) ;
	AB_Char    lc_year(2) ;
	AB_Varchar lv_field(50);
	int		  li_len;

	lc_M5link =  pt_segment.At(6).For(1) ;

	md_IT_itin_type[mi_IT_I] = "R" ;

	if (pt_segment.For(1) == "!" )
		pt_segment = pt_segment.At(2) ;

	if ( lc_M5link != "1" && lc_M5link != " " ) 			// Not an Amtrack Rail
	{

		md_NonArcItin[++ mi_NonArcItin] = pt_segment.At(3).For(2) + lc_M5link ;
	}

	//md_IT_sup_id[mi_IT_I]      = pt_segment.At(21).For(2) ;
	md_IT_sup_id[mi_IT_I]      = "ZZ" ;
	if(PCRE::pcre_match("/SI-SID", pt_segment.to_string()) )
	{
		md_IT_sup_id[mi_IT_I]      = "2V" ;
	}

	if (md_IT_sup_id[mi_IT_I].IsNull())
	{
		md_IT_itin_type[mi_IT_I] = AB_null ;
		return ;
	}

	md_IT_line_number[mi_IT_I]    = pt_segment.At(3).For(2) ;

	lc_date = pt_segment.At(10).For(5) ;

	if (lc_date != "     " )
	{
   	lc_year = mt_pnrDate.get_default_year(lc_date) ;
		lc_year.rightJustify('0') ;
		md_IT_start_date[mi_IT_I] = lc_date + lc_year ;
	}
	else
		md_IT_start_date[mi_IT_I] = md_IRT_pnr_date ;

	md_IT_reference[mi_IT_I]      = md_IRT_reference ;

	//md_IT_end_date[mi_IT_I]       = md_IT_start_date[mi_IT_I] ;

	//md_IT_classofservice[mi_IT_I] = pt_segment.At(8).For(2) ;

	// Log 28010
	md_IT_classofservice[mi_IT_I] = "C" ;

	li_pos = pt_segment.findSubstring("NA-TRAIN") ;
	if ( li_pos > 0)
	{
		md_IT_service[mi_IT_I]        = pt_segment.At(li_pos+9) ;
		li_pos = md_IT_service[mi_IT_I].findSubstring("/") ;
		if ( li_pos > 1)
			md_IT_service[mi_IT_I]     = md_IT_service[mi_IT_I].For(li_pos - 1) ;
		else
			md_IT_service[mi_IT_I]     = "9999" ;
	}
	else
		md_IT_service[mi_IT_I]        = "9999" ;

	md_IT_start_time[mi_IT_I]     = AB_null ;
	md_IT_end_time[mi_IT_I]       = AB_null ;

	lv_field = pt_segment.getNthBracketedField(1, "LV-", "/") ;
	if (lv_field.NotIsNull()) {
		md_IT_start_time[mi_IT_I] = CheckTime(lv_field);
	}

	lv_field = pt_segment.getNthBracketedField(1, "AR-", "/") ;
	if (lv_field.NotIsNull()) {
		md_IT_end_time[mi_IT_I] = CheckTime(lv_field);
	}

	md_IT_meal[mi_IT_I] = AB_null ;
	md_IT_arrival_ind[mi_IT_I]    = 0 ;
	md_IT_no_of_stops[mi_IT_I] = "0" ;

	md_IT_orig_city_id[mi_IT_I]   = "ZZZ" ;
	md_IT_dest_city_id[mi_IT_I]   = "ZZZ" ;
	LOG(logDEBUG) << "looking for 6chars follwed by / in [" << pt_segment.At(36).For(7) << "]---" ;
	if(PCRE::pcre_match("[A-Z]{6}/", pt_segment.At(36).For(7).to_string()) )
	{
		LOG(logDEBUG) << "MATCH FOUND!!!!!!!!" ;
		md_IT_orig_city_id[mi_IT_I]   = pt_segment.At(36).For(3) ;
		md_IT_dest_city_id[mi_IT_I]   = pt_segment.At(39).For(3) ;
	}

	// SCR 521568 ; use new location of orig_city, dest_city
	if(PCRE::pcre_match("/FR-[^-]+-([A-Z]{3})/", pt_segment.to_string()) )
	{
		string orig_city_id;
		PCRE::pcre_get_group(1, orig_city_id);
		LOG(logDEBUG) << "-----orig_city_id[" << orig_city_id.data() << "]---" ;
		md_IT_orig_city_id[mi_IT_I]   = orig_city_id.data() ;
	}
	if(PCRE::pcre_match("/AT-[^-]+-([A-Z]{3})/", pt_segment.to_string()) )
	{
		string dest_city_id;
		PCRE::pcre_get_group(1, dest_city_id);
		LOG(logDEBUG) << "-----dest_city_id[" << dest_city_id.data() << "]---" ;
		md_IT_dest_city_id[mi_IT_I]   = dest_city_id.data() ;
	}
	// SCR 521568 END; use new location of orig_city, dest_city

	li_pos = pt_segment.findSubstring("SI-") ;

	if ( li_pos > 0 )
	{
		lt_remark = pt_segment.At(li_pos+3) ;

		li_pos = lt_remark.findSubstring("CF") ;

		if ( li_pos > 0 )
			md_IT_aff_carrier[mi_IT_I]  = lt_remark.For(li_pos - 1) ;
		else
			md_IT_aff_carrier[mi_IT_I]  = lt_remark ;
	}

	lc_itinLine                   = pt_segment.At(3).For(2) ;

   if (mv_itinTransport.IsNull())
      mv_itinTransport = lc_itinLine ;
   else
      mv_itinTransport = mv_itinTransport + lc_itinLine ;


        // G9150 start
	if (pRemarkParser->isRemarkParsed("LABELS"))
        {
	  //------ STS ------------------
	  RP_saveDataTo(	"LABELS", "STS", md_ICD_service_status[mi_ICD_I]);

        }
        // G9150 end

	// fill Personitin

	lc_noofSeats = "00" ;
	lt_seatData  = " " ;

	lt_ftpMiles = AB_null ;

	fillConsumDetail(lc_noofSeats,
					 md_IT_itin_type[mi_IT_I],
				 	 lc_itinLine,
					 lt_seatData,
					 lt_ftpMiles) ; /* ftp_miles */


    mi_IT_I++ ;

    EXIT;
}

//------------------------------------------------------------------------------

void CIUR::fill_IrBusM3(AB_Text &pt_section)
{
   ENTER;

   // int li_field_length ;
   AB_Varchar lv_field(50) ;
	AB_Char    lc_productType(1) ;
	AB_Char    lc_M5link(1) ;
   AB_Char    lc_date(5) ;
   AB_Char    lc_itinLine(2) ;
   AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_seatData ;
	AB_Text 	  lt_ftpMiles ;
	AB_Char    lc_year(2) ;

	md_IT_itin_type[mi_IT_I] = "B" ;

	lc_M5link = pt_section.At(6).For(1) ;

	// SCR 231339 START

	if ( lc_M5link  != " " )
	{
		getSupId(lc_M5link,md_IT_sup_id[mi_IT_I]) ;
	}

//	if (md_IT_sup_id[mi_IT_I].IsNull())
//	{
//		md_IT_itin_type[mi_IT_I] = AB_null ;
//		return ;
//	}

	// SCR 231339 END

   lc_date = pt_section.At(10).For(5) ;

	if (lc_date != "     " )
	{
   	lc_year = mt_pnrDate.get_default_year(lc_date) ;
		lc_year.rightJustify('0') ;
		md_IT_start_date[mi_IT_I] = lc_date + lc_year ;
	}
	else
		md_IT_start_date[mi_IT_I] = md_IRT_pnr_date ;

   md_IT_orig_city_id[mi_IT_I] = pt_section.At(36).For(3) ;
	md_IT_dest_city_id[mi_IT_I] = pt_section.At(39).For(3) ;

	/* G9130: ignore what was done in SCR 244977
   // SCR 244977 START

   V_AB_Text   lv_arrayField(100);

   lv_arrayField.parseStringMultDel(pt_section.At(36), "/");
   md_IT_orig_cityname[mi_IT_I] = lv_arrayField[1];

   // SCR 244977 END
	*/

	 md_IT_service[mi_IT_I]        = "9999" ;

   md_IT_line_number[mi_IT_I] =  pt_section.At(3).For(2) ;

   // md_IT_end_date[mi_IT_I] = md_IT_start_date[mi_IT_I] ;
	md_IT_arrival_ind[mi_IT_I]    = 0 ;

   md_IT_reference[mi_IT_I] = pt_section.At(21).For(15) ;

	lc_itinLine                   = pt_section.At(3).For(2) ;

   if (mv_itinTransport.IsNull())
      mv_itinTransport = lc_itinLine ;
   else
      mv_itinTransport = mv_itinTransport + lc_itinLine ;

	// fill Personitin

	lc_noofSeats = "00" ;
	lt_seatData  = " " ;

	lt_ftpMiles = AB_null ;

   // Gap G9130 START
   V_AB_Text   lv_arrayField2(100);
   AB_Text     lt_key;
   AB_Text     lt_data;
   int li_count = lv_arrayField2.parseStringMultDel(pt_section, "/");

	for ( int li_index = 2; li_index <= li_count; li_index++ ) {
		lt_key = lv_arrayField2[li_index].For(3);
		lt_data = lv_arrayField2[li_index].At(4).to_string();
		lt_data.removeLeadAndTrailBlanks();

		if (lt_key == "LV-") {
			if ( lt_data.NotIsNull() && lt_data.length() > 0 ) {
				md_IT_start_time[mi_IT_I] = CheckTime(lt_data.For(6));
			}
		}
		else if (lt_key == "DT-") {
			if ( lt_data.NotIsNull() && lt_data.length() > 0 ) {
   			if (lt_data != "     " ) {
      			lc_year = mt_pnrDate.get_default_year(lt_data.For(5)) ;
      			lc_year.rightJustify('0') ;
      			md_IT_end_date[mi_IT_I] = lt_data.For(5) + lc_year ;
   			}
			}
		}
		else if (lt_key == "AR-") {
			if ( lt_data.NotIsNull() && lt_data.length() > 0 ) {
				md_IT_end_time[mi_IT_I] = CheckTime(lt_data.For(6));
			}
		}
		else if (lt_key == "FR-") {
			if ( lt_data.NotIsNull() && lt_data.length() > 0 ) {
				md_IT_orig_cityname[mi_IT_I] = lt_data;
			}
		}
		else if (lt_key == "AT-") {
			if ( lt_data.NotIsNull() && lt_data.length() > 0 ) {
				md_IT_dest_cityname[mi_IT_I] = lt_data;
			}
		}
	}

	if (md_IT_orig_cityname[mi_IT_I].IsNull() && md_IT_dest_cityname[mi_IT_I].NotIsNull()) {
		md_IT_orig_cityname[mi_IT_I] = md_IT_dest_cityname[mi_IT_I];
	}
	else if (md_IT_orig_cityname[mi_IT_I].NotIsNull() && md_IT_dest_cityname[mi_IT_I].NotIsNull()) {
		md_IT_orig_cityname[mi_IT_I] = md_IT_orig_cityname[mi_IT_I] + "-" + md_IT_dest_cityname[mi_IT_I];
	}

   // Gap G9130 END

        // G9150
	if (pRemarkParser->isRemarkParsed("LABELS"))
        {
	  //------ STS ------------------
	  RP_saveDataTo(	"LABELS", "STS", md_ICD_service_status[mi_ICD_I]);

        }

	fillConsumDetail(lc_noofSeats,
					 md_IT_itin_type[mi_IT_I],
				 	 lc_itinLine,
					 lt_seatData,
					 lt_ftpMiles) ; /* ftp_miles */

	md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;

   mi_IT_I++ ;

   EXIT;
}

//------------------------------------------------------------------------------

// GAP US092 START
void CIUR::fill_IrShipM3(AB_Text &pt_section)
{
   ENTER;

   // int li_field_length ;

   AB_Varchar lv_field(50) ;
   AB_Char    lc_M5link(1) ;
   AB_Char    lc_itinLine(2) ;
   AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_seatData ;
   AB_Text    lt_ftpMiles ;

   AB_Boolean  lb_dupFound;

   lb_dupFound = AB_false;

   lc_M5link = pt_section.At(6).For(1) ;

   if ( lc_M5link  == " " || mc_iurType == "3")
   {
      md_IO_sup_id[mi_IO_I] = AB_null;
   }
   else
   {
      getSupId(lc_M5link,md_IO_sup_id[mi_IO_I]) ;

      if (md_IO_sup_id[mi_IO_I].IsNull())
      {
         md_IO_itin_type[mi_IO_I] = AB_null ;
         return ;
      }
   }

   parseShipSegment( pt_section, lb_dupFound );

	if ( lb_dupFound == AB_false )
	{
      lc_itinLine = pt_section.At(3).For(2) ;

      if (mv_itinTransport.IsNull())
         mv_itinTransport = lc_itinLine ;
      else
         mv_itinTransport = mv_itinTransport + lc_itinLine ;

      // fill Personitin

      lc_noofSeats = "00" ;
      lt_seatData  = " " ;

      lt_ftpMiles = AB_null ;

      fillConsumDetail( lc_noofSeats,
                        md_IO_itin_type[mi_IO_I],
                        lc_itinLine,
                        lt_seatData,
                        lt_ftpMiles) ; /* ftp_miles */

		if ( lc_M5link  != " " )
      	md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;

      mi_IO_I++ ;
	}

   EXIT;
}

void CIUR::parseShipSegment(  AB_Text     &pt_section,
                              AB_Boolean  &pb_dupFound)
{
   int            li_pos;
   int            li_startPos;
   int            li_CFPos;
   int            li_index;
   int            li_count;
   int            li_eff_IO_index;
   AB_Text        lt_section;
   AB_Text        lt_data;
   AB_Text        lt_remark;
   V_AB_Text      lv_arrayField(100);
   AB_Char    		lc_date(5) ;
   AB_Char    		lc_year(2) ;
   AB_Char        lc_productType(1) ;

   lc_productType = pt_section.At(5).For(1) ;

   getItinType(lc_productType,
               md_IO_itin_type[mi_IO_I],
               md_IO_unit_type[mi_IO_I]) ;

   md_IO_line_number[mi_IO_I]    = pt_section.At(3).For(2) ;
   md_IO_reference[mi_IO_I]      = md_IRT_reference;
   md_IO_unit_ct[mi_IO_I]        = AB_null;

   lc_date = pt_section.At(10).For(5) ;

   if (lc_date != "     " )
   {
      lc_year = mt_pnrDate.get_default_year(lc_date) ;
      lc_year.rightJustify('0') ;
      md_IO_start_date[mi_IO_I] = lc_date + lc_year ;
   }
   else
      md_IO_start_date[mi_IO_I] = md_IRT_pnr_date ;

   lt_section = pt_section.At(36);

   li_pos       = lt_section.findSubstring("!") ;

   if (li_pos > 0)
   	lt_section = lt_section.For(li_pos - 1) ;

   li_count= lv_arrayField.parseStringMultDel(lt_section, "/");

   pb_dupFound       = AB_false;
   li_eff_IO_index   = mi_IO_I;

   if ( md_IO_sup_id[mi_IO_I].IsNull() )
   {
      for ( li_index = mi_IO_I-1; li_index >= 0; li_index--)
      {
         if (  md_IO_itin_type[li_index] != "S" )
         {
            break;
         }

         if (  md_IO_itin_type[li_index] == "S" )
         {
            if ( lv_arrayField[1] 				== mv_lastShipCity &&
                 md_IO_start_date[li_index]  == md_IO_start_date[mi_IO_I]  )
            {
               pb_dupFound       = AB_true;
               li_eff_IO_index   = li_index;
               break;
            }
            else
            {
               break;
            }
         }
      }
   }

	if ( pb_dupFound == AB_false )
	{
		mv_lastShipCity = lv_arrayField[1];

		if ( lv_arrayField[1].length() == 6 )
		{
			md_IO_orig_city_id[mi_IO_I] = lt_section.For(3);
			md_IO_dest_city_id[mi_IO_I] = lt_section.At(4).For(3);
		}
		else
		{
			md_IO_orig_city_id[mi_IO_I] = "XXX";
			md_IO_dest_city_id[mi_IO_I] = "XXX";
		}
	}

	// to separate G9150 (LABELS/DATA STANDARTIZATION) from old format
	string tmp;
	bool isLabels = pRemarkParser->getData("LABELS", "TYP", tmp);

	for ( li_index = 2; li_index <= li_count; li_index++ )
	{
	  if (     isLabels  // LABELS will be populated in populateRestOfLabels
			  || lv_arrayField[li_index].IsNull()
			  || lv_arrayField[li_index].length() == 0)
		{
			continue;
		}

		lt_data = lv_arrayField[li_index].At(4).to_string();

		if ( lv_arrayField[li_index].For(3) == "LV-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_start_time[li_eff_IO_index].IsNull() )
			{
				md_IO_start_time[li_eff_IO_index] = CheckTime(lt_data.For(6));
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "FR-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_departure_point[li_eff_IO_index].IsNull() )
			{
				md_IO_departure_point[li_eff_IO_index] = lt_data.For(14);
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "DT-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_end_date[li_eff_IO_index].IsNull() )
			{
   			if (lt_data != "     " )
   			{
      			lc_year = mt_pnrDate.get_default_year(lt_data.For(5)) ;
      			lc_year.rightJustify('0') ;
      			md_IO_end_date[mi_IO_I] = lt_data.For(5) + lc_year ;
   			}
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "AR-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_end_time[li_eff_IO_index].IsNull() )
			{
				md_IO_end_time[li_eff_IO_index] = CheckTime(lt_data.For(6));
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "AT-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_arrival_point[li_eff_IO_index].IsNull() )
			{
				 md_IO_arrival_point[li_eff_IO_index] = lt_data.For(14);
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "NA-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_svc_description[li_eff_IO_index].IsNull() )
			{
				md_IO_svc_description[li_eff_IO_index] = lt_data.For(15);
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "SP-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_cabin_no[li_eff_IO_index].IsNull() )
			{
				md_IO_cabin_no[li_eff_IO_index] = lt_data.For(15);
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "CF-" && pb_dupFound == AB_false )
		{
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_confirmation[li_eff_IO_index].IsNull() )
			{
         	md_IO_confirmation[li_eff_IO_index] = lt_data.For(15);
         	md_IO_confirmation[li_eff_IO_index].removeLeadAndTrailBlanks();
			}
		}
		else if ( lv_arrayField[li_index].For(3) == "SI-" )
		{
			if ( lt_data.NotIsNull() )
			{
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

         		if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
         		{
            		md_IRMK_type[mi_IRMK_I]             = "PN" + md_IO_itin_type[li_eff_IO_index];
            		md_IRMK_oth_line_number[mi_IRMK_I]  = md_IO_line_number[li_eff_IO_index];
            		md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[li_eff_IO_index];
            		md_IRMK_pax_id[mi_IRMK_I]           = AB_null;
            		mi_IRMK_I++;
         		}

         		lt_data = lt_data.At(li_pos+1);
      		}
			}
		}
	}


	if ( md_IO_confirmation[li_eff_IO_index].IsNull() && pb_dupFound == AB_false )
   {
		lt_data = pt_section.At(21).For(15);
		lt_data.removeLeadAndTrailBlanks();

		if ( lt_data.NotIsNull() && lt_data.length() > 0 )
		{
     		md_IO_confirmation[li_eff_IO_index] = lt_data;
		}
   }

	processOtherAssociatedRemarks(li_eff_IO_index);
}

//------------------------------------------------------------------------------

void CIUR::fill_IrTourM3(AB_Text &pt_section)
{
   ENTER;

   AB_Varchar lv_field(50) ;
   AB_Char    lc_M5link(1) ;
   AB_Char    lc_itinLine(2) ;
   AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_seatData ;
   AB_Text    lt_ftpMiles ;

   AB_Boolean  lb_dupFound;

   lb_dupFound = AB_false;

   lc_M5link = pt_section.At(6).For(1) ;

   if ( lc_M5link  == " " || mc_iurType == "3" )
   {
      md_IO_sup_id[mi_IO_I]         = AB_null;
   }
   else
   {
      getSupId(lc_M5link,md_IO_sup_id[mi_IO_I]) ;

      if (md_IO_sup_id[mi_IO_I].IsNull())
      {
         md_IO_itin_type[mi_IO_I] = AB_null ;
         return ;
      }
   }

	LOG(logDEBUG)  << "DK >>> JUST BEFORE parseTourSegment"  ;
   parseTourSegment( pt_section, lb_dupFound );
	LOG(logDEBUG)  << "DK >>> JUST AFTER parseTourSegment"  ;

   if ( lb_dupFound == AB_false )
   {
   	lc_itinLine = pt_section.At(3).For(2) ;

      if (mv_itinTransport.IsNull())
      	mv_itinTransport = lc_itinLine ;
      else
      	mv_itinTransport = mv_itinTransport + lc_itinLine ;

		// fill Personitin

      lc_noofSeats   = "00";
      lt_seatData    = " ";
      lt_ftpMiles    = AB_null;

      fillConsumDetail( lc_noofSeats,
                        md_IO_itin_type[mi_IO_I],
                        lc_itinLine,
                        lt_seatData,
                        lt_ftpMiles) ; /* ftp_miles */

		if ( lc_M5link  != " " )
			md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;

      mi_IO_I++ ;
	}

   EXIT;
}

void CIUR::parseTourSegment(  AB_Text     &pt_section,
                              AB_Boolean  &pb_dupFound)
{
   int            li_pos;
   int            li_startPos;
   int            li_CFPos;
   int            li_index;
   int            li_count;
   int            li_eff_IO_index;
   AB_Text        lt_section;
   AB_Text        lt_data;
   AB_Text        lt_remark;
   V_AB_Text      lv_arrayField(100);
   AB_Char    		lc_date(5) ;
   AB_Char    		lc_year(2) ;
   AB_Char        lc_productType(1) ;

   lc_productType = pt_section.At(5).For(1) ;

   getItinType(lc_productType,
               md_IO_itin_type[mi_IO_I],
               md_IO_unit_type[mi_IO_I]) ;

   md_IO_line_number[mi_IO_I]    		= pt_section.At(3).For(2) ;
   md_IO_reference[mi_IO_I]      		= md_IRT_reference;

   md_IO_unit_ct[mi_IO_I]     = AB_null;
	md_IO_dest_city_id[mi_IO_I]= AB_null;

	md_IO_start_time[mi_IO_I] 	= AB_null;
	md_IO_end_date[mi_IO_I] 	= AB_null;
	md_IO_end_time[mi_IO_I] 	= AB_null;


   lc_date = pt_section.At(10).For(5) ;

   if (lc_date != "     " )
   {
      lc_year = mt_pnrDate.get_default_year(lc_date) ;
      lc_year.rightJustify('0') ;
      md_IO_start_date[mi_IO_I] = lc_date + lc_year ;
   }
   else
      md_IO_start_date[mi_IO_I] = md_IRT_pnr_date ;

   lt_section = pt_section.At(36);

   li_pos       = lt_section.findSubstring("!") ;

   if (li_pos > 0)
   	lt_section = lt_section.For(li_pos - 1) ;


   li_count= lv_arrayField.parseStringMultDel(lt_section, "/");

   pb_dupFound       = AB_false;
   li_eff_IO_index   = mi_IO_I;

   if ( md_IO_sup_id[mi_IO_I].IsNull() )
   {
      for ( li_index = mi_IO_I-1; li_index >= 0; li_index--)
      {
         if (  md_IO_itin_type[li_index] != "T" )
         {
            break;
         }

         if (  md_IO_itin_type[li_index] == "T" )
         {
            if ( lv_arrayField[1] 				== mv_lastTourCity &&
                 md_IO_start_date[li_index]  == md_IO_start_date[mi_IO_I]  )
            {
               pb_dupFound       = AB_true;
               li_eff_IO_index   = li_index;
               break;
            }
            else
            {
               break;
            }
         }
      }
   }

	if ( pb_dupFound == AB_false )
	{
		mv_lastTourCity = lv_arrayField[1];

		if ( lv_arrayField[1].length() == 3 )
			md_IO_orig_city_id[mi_IO_I] = lv_arrayField[1];
		else
			md_IO_orig_city_id[mi_IO_I] = "XXX";
	}

	// G9150 START: to separate  (LABELS/DATA STANDARTIZATION) from old format
	string tmp;
	bool isLabels = pRemarkParser->getData("LABELS", "TYP", tmp);
	// G9150 END

	for ( li_index = 2; li_index <= li_count; li_index++ )
	{
	  if (     isLabels  // LABELS will be populated in populateRestOfLabels
			  || lv_arrayField[li_index].IsNull()
			  || lv_arrayField[li_index].length() == 0)
		{
			continue;
		}

		if ( li_index == li_count 							&&
			  lv_arrayField[li_index].For(3) == "CF-" &&
			  pb_dupFound == AB_false )
		{
			lt_data = lv_arrayField[li_index].At(4).to_string();
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0 	&&
				  md_IO_confirmation[li_eff_IO_index].IsNull() )
			{
         	md_IO_confirmation[li_eff_IO_index] = lt_data.For(15);
         	md_IO_confirmation[li_eff_IO_index].removeLeadAndTrailBlanks();
			}
		}
		else if ( li_index == 2 && pb_dupFound == AB_false )
		{
			lt_data = lv_arrayField[li_index].to_string();
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_svc_description[li_eff_IO_index].IsNull() )
			{
				md_IO_svc_description[li_eff_IO_index] = lt_data.For(40);
			}
		}
		else if ( li_index == 3 && pb_dupFound == AB_false )
		{
			lt_data = lv_arrayField[li_index].to_string();
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_tour_no[li_eff_IO_index].IsNull() )
			{
				md_IO_tour_no[li_eff_IO_index] = lt_data.For(15);
			}
		}
		else if ( li_index == 4 && pb_dupFound == AB_false )
		{
			lt_data = lv_arrayField[li_index].to_string();
			lt_data.removeLeadAndTrailBlanks();

			if ( lt_data.NotIsNull() 	&&
				  lt_data.length() > 0  &&
				  md_IO_unit_type[li_eff_IO_index].IsNull() )
			{
				md_IO_unit_type[li_eff_IO_index] = lt_data.For(4);
			}
		}
		else if ( li_index == 5 && lv_arrayField[li_index].For(3) == "SI-" )
		{
			lt_data = lv_arrayField[li_index].At(4).to_string();

			if ( lt_data.NotIsNull() )
			{
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

         		if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
         		{
            		md_IRMK_type[mi_IRMK_I]             = "PN" + md_IO_itin_type[li_eff_IO_index];
            		md_IRMK_oth_line_number[mi_IRMK_I]  = md_IO_line_number[li_eff_IO_index];
            		md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[li_eff_IO_index];
            		md_IRMK_pax_id[mi_IRMK_I]           = AB_null;
            		mi_IRMK_I++;
         		}

         		lt_data = lt_data.At(li_pos+1);
      		}
			}
		}
	}


   if ( md_IO_confirmation[li_eff_IO_index].IsNull() && pb_dupFound == AB_false )
   {
		lt_data = pt_section.At(21).For(15);
		lt_data.removeLeadAndTrailBlanks();

		if ( lt_data.NotIsNull() && lt_data.length() > 0 )
		{
     		md_IO_confirmation[li_eff_IO_index] = lt_data;
		}
   }

	processOtherAssociatedRemarks(li_eff_IO_index);
}
// GAP US092 END

//------------------------------------------------------------------------------

// GAP US089 START
bool CIUR::fill_IrOtherM3(AB_Text &pt_section)
{
   ENTER;

   // int li_field_length ;

   AB_Varchar lv_field(50) ;
   AB_Char    lc_productType(1) ;
   AB_Char    lc_M5link(1) ;
   AB_Char    lc_date(5) ;
   AB_Char    lc_itinLine(2) ;
   AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_seatData ;
   AB_Text    lt_ftpMiles ;
   AB_Char    lc_year(2) ;

   AB_Boolean  lb_dupFound;

   lb_dupFound = AB_false;

   lc_productType = pt_section.At(5).For(1) ;

   getItinType(lc_productType,
               md_IO_itin_type[mi_IO_I],
               md_IO_unit_type[mi_IO_I]) ;

   lc_M5link = pt_section.At(6).For(1) ;

   if ( lc_M5link  == " " || mc_iurType == "3")
   {
      if ( md_IO_itin_type[mi_IO_I] != "O" )
      {
         md_IO_itin_type[mi_IO_I] = AB_null ;
         return false;
      }

      md_IO_line_number[mi_IO_I]    = mi_IO_I + 1 ; //pt_section.At(3).For(2);
      md_IO_reference[mi_IO_I]      = md_IRT_reference;
      md_IO_sup_id[mi_IO_I]         = AB_null;
      md_IO_unit_ct[mi_IO_I]        = AB_null;
      md_IO_dest_city_id[mi_IO_I]   = AB_null;
      md_IO_end_date[mi_IO_I]       = AB_null;

      lc_date = pt_section.At(10).For(5) ;

      if (lc_date != "     " )
      {
         lc_year = mt_pnrDate.get_default_year(lc_date) ;
         lc_year.rightJustify('0') ;
         md_IO_start_date[mi_IO_I] = lc_date + lc_year ;
      }
      else
         md_IO_start_date[mi_IO_I] = md_IRT_pnr_date ;

      parseOtherSegment( pt_section, lb_dupFound );

      if ( lb_dupFound == AB_false )
      {
         lc_itinLine = pt_section.At(3).For(2) ;

         if (mv_itinTransport.IsNull())
            mv_itinTransport = lc_itinLine ;
         else
            mv_itinTransport = mv_itinTransport + lc_itinLine ;

         // fill Personitin

         lc_noofSeats   = "00";
         lt_seatData    = " ";
         lt_ftpMiles    = AB_null;

         fillConsumDetail( lc_noofSeats,
                           md_IO_itin_type[mi_IO_I],
                           lc_itinLine,
                           lt_seatData,
                           lt_ftpMiles) ; /* ftp_miles */

         mi_IO_I++ ;
      }
   }
   else
   {
      getSupId(lc_M5link,md_IO_sup_id[mi_IO_I]) ;

      if (md_IO_sup_id[mi_IO_I].IsNull())
      {
         md_IO_itin_type[mi_IO_I] = AB_null ;
         return false;
      }

      md_IO_unit_ct[mi_IO_I]        = 1 ;
      md_IO_line_number[mi_IO_I]    = pt_section.At(3).For(2) ;

      lc_date = pt_section.At(10).For(5) ;

      if (lc_date != "     " )
      {
         lc_year = mt_pnrDate.get_default_year(lc_date) ;
         lc_year.rightJustify('0') ;
         md_IO_start_date[mi_IO_I] = lc_date + lc_year ;
      }
      else
         md_IO_start_date[mi_IO_I] = md_IRT_pnr_date ;

      md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;

      if ( md_IO_itin_type[mi_IO_I] == "O" )
      {
         parseOtherSegment( pt_section, lb_dupFound );
         md_IO_reference[mi_IO_I] = md_IRT_reference;
      }
      else if ( md_IO_itin_type[mi_IO_I] == "I" )
      {
         md_IO_orig_city_id[mi_IO_I]   = pt_section.At(36).For(3) ;
         //SCR518984:ignore ref md_IO_reference[mi_IO_I] = pt_section.At(21).For(15) ;

			processOtherAssociatedRemarks(mi_IO_I);
      }
      else
      {
         md_IO_orig_city_id[mi_IO_I]   = pt_section.At(36).For(3) ;
         md_IO_reference[mi_IO_I] = pt_section.At(21).For(15) ;

			processOtherAssociatedRemarks(mi_IO_I);
      }

      if ( lb_dupFound == AB_false )
      {
			md_IO_dest_city_id[mi_IO_I]   = md_IO_orig_city_id[mi_IO_I] ;

			lc_itinLine = pt_section.At(3).For(2) ;

			if (mv_itinTransport.IsNull())
				mv_itinTransport = lc_itinLine ;
			else
				mv_itinTransport = mv_itinTransport + lc_itinLine ;

			// fill Personitin

			lc_noofSeats = "00" ;
			lt_seatData  = " " ;

			lt_ftpMiles = AB_null ;

			fillConsumDetail( lc_noofSeats,
                        md_IO_itin_type[mi_IO_I],
                        lc_itinLine,
                        lt_seatData,
                        lt_ftpMiles) ; /* ftp_miles */

			md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;

			mi_IO_I++ ;
		}
   }

   EXIT;

   return true;
}

void CIUR::parseOtherSegment( AB_Text     &pt_section,
                              AB_Boolean  &pb_dupFound)
{
   int            li_pos;
   int            li_startPos;
   int            li_CFPos;
   int            li_index;
   int            li_count;
   int            li_eff_IO_index;
   AB_Text        lt_section;
   AB_Text        lt_data;
   AB_Text        lt_remark;
   V_AB_Text      lv_arrayField(100);

   lt_section = pt_section.At(36);

   li_pos       = lt_section.findSubstring("!") ;

   if (li_pos > 0)
   	lt_section = lt_section.For(li_pos - 1) ;

   li_count= lv_arrayField.parseStringMultDel(lt_section, "/");

   pb_dupFound       = AB_false;
   li_eff_IO_index   = mi_IO_I;

   if ( md_IO_sup_id[mi_IO_I].IsNull() )
   {
      for ( li_index = mi_IO_I-1; li_index >= 0; li_index--)
      {
         if (  md_IO_itin_type[li_index] != "O" ||
             ( md_IO_itin_type[li_index] == "O" &&
               md_IO_sup_id[li_index].NotIsNull() ) )
         {
            break;
         }

         if (  md_IO_itin_type[li_index] == "O" &&
               md_IO_sup_id[li_index].IsNull() )
         {
            if ( lv_arrayField[1]            == mv_lastIOName  &&
                 md_IO_start_date[li_index]  == md_IO_start_date[mi_IO_I]  )
            {
               pb_dupFound       = AB_true;
               li_eff_IO_index   = li_index;
               break;
            }
            else
            {
               break;
            }
         }
      }
   }

   if ( pb_dupFound == AB_false )
   {
      if ( md_IO_sup_id[mi_IO_I].IsNull() )
      {
         mv_lastIOName = lv_arrayField[1];
      }

      if ( lv_arrayField[1].length() == 3 )
      {
         md_IO_orig_city_id[mi_IO_I] = lv_arrayField[1];
      }
      else
      {
         md_IO_orig_city_id[mi_IO_I] = "XXX";
      }
   }

   if ( li_count > 1 )
   {
		if ( lv_arrayField[2].NotIsNull() )
		{
      	lt_data = lv_arrayField[2].to_string();

			if ( 	md_IO_sup_id[mi_IO_I].IsNull() )
			{
				lt_data.removeLeadingBlanks();

				if ( lt_data.For(13) == "THANK YOU FOR" )
				{
					pb_dupFound = AB_true;
					return;
				}
			}
		}

		li_startPos = lt_section.findSubstring("/");
		li_CFPos 	= lt_section.findSubstring("/CF-");

		if ( li_startPos > 0 )
		{
			if ( li_CFPos > 0 )
				lt_data = lt_section.At(li_startPos+1).For(li_CFPos-li_startPos-1);
			else
				lt_data = lt_section.At(li_startPos+1);
		}

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

         if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
         {
            md_IRMK_type[mi_IRMK_I]             = "PN" + md_IO_itin_type[li_eff_IO_index];
            md_IRMK_oth_line_number[mi_IRMK_I]  = md_IO_line_number[li_eff_IO_index];
            md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[li_eff_IO_index];
            md_IRMK_pax_id[mi_IRMK_I]           = AB_null;
            mi_IRMK_I++;
         }

         lt_data = lt_data.At(li_pos+1);
      }
   }

   if ( li_CFPos > 0 )
   {
      if ( md_IO_confirmation[li_eff_IO_index].IsNull() )
      {
         md_IO_confirmation[li_eff_IO_index] = lt_section.At(li_CFPos+4).For(20);
         md_IO_confirmation[li_eff_IO_index].removeLeadAndTrailBlanks();
      }
   }

   if ( md_IO_confirmation[li_eff_IO_index].IsNull() )
   {
      md_IO_confirmation[li_eff_IO_index] = pt_section.At(21).For(15);
      md_IO_confirmation[li_eff_IO_index].removeLeadAndTrailBlanks();
   }

	processOtherAssociatedRemarks(li_eff_IO_index);
}

void CIUR::processOtherAssociatedRemarks(int pi_eff_IO_index)
{
   AB_Text        lt_remark;
   D_line_number  ld_line_number;

   for ( int li_index=1; li_index < mi_MERemarkIndex; li_index++ )
   {
      ld_line_number = mva_MERemarkSection[li_index].For(2);
      lt_remark      = mva_MERemarkSection[li_index].At(3);

      if ( ld_line_number == md_IO_line_number[mi_IO_I] )
      {
         md_IRMK_remark[mi_IRMK_I]  = lt_remark.For(70);

         if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
         {
            md_IRMK_type[mi_IRMK_I]             = "PN" + md_IO_itin_type[pi_eff_IO_index];
            md_IRMK_oth_line_number[mi_IRMK_I]  = md_IO_line_number[pi_eff_IO_index];
            md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[pi_eff_IO_index];
            md_IRMK_pax_id[mi_IRMK_I]           = AB_null;
            mi_IRMK_I++;
         }
      }
   }
}

// GAP US089 END

//------------------------------------------------------------------------------

void CIUR::storeAccountingData(CPNR &po_PNR)
{
   ENTER;

   AB_Text     lt_segment ;

   po_PNR.resetSegment("M5") ;
   lt_segment = po_PNR.getNextSegment("M5");
	mi_accountIndex = 0 ;

   while (lt_segment.NotIsNull())
   {
		md_accountingData[++ mi_accountIndex] = lt_segment ;

      lt_segment = po_PNR.getNextSegment("M5");
   }
   EXIT;
}

//------------------------------------------------------------------------------

void CIUR::getSupId(AB_Char  &pc_M5link,
						  D_supplier &pd_supId)
{
   ENTER;

   int          li_counter, li_pos ;
	V_AB_Varchar lv_arrayField(100,100) ;

   li_counter = 1 ;
	pd_supId = AB_null ;

	LOG(logDEBUG)  << "DK >>> mi_accountIndex = " << mi_accountIndex  ;
   while (li_counter <= mi_accountIndex)
   {
      if (pc_M5link == md_accountingData[li_counter].At(7).For(1))
		{
			li_pos = lv_arrayField.
				parseStringMultDel(md_accountingData[li_counter].At(9),"/") ;

			pd_supId = lv_arrayField[1].At(4).For(3) ;

			if (lv_arrayField[1].length() < 4) {
				pd_supId = lv_arrayField[1];
			}

			LOG(logDEBUG)  << "DK >>> JUST BEFORE RETURN FROM getSupId " << pd_supId  ;
			return ;

		}

      li_counter++ ;
   }
   EXIT;
}

//-----------------------------------------------------------------------------

void CIUR::getItinType(AB_Char  &pc_productType,
							  AB_Char  &pc_itinType,
							  AB_Char  &pc_unitType)
{
	ENTER;

	if (pc_productType == "2")
	{
		pc_itinType = "T" ;
		pc_unitType = "TOUR" ;
		return ;
	}
	if (pc_productType == "4")
	{
		pc_itinType = "S" ;
		pc_unitType = "SEA" ;
		return ;
	}
	if (pc_productType == "7")
	{
		pc_itinType = "I" ;
		pc_unitType = "INS" ;
		return ;
	}
	if (pc_productType == "8")
	{
		pc_itinType = "O" ;
		pc_unitType = "ATX" ;
		return ;
	}
	if (pc_productType == "9")
	{
		pc_itinType = "O" ;
		pc_unitType = "MCO" ;
		return ;
	}
	if (pc_productType == "B")
	{
		pc_itinType = "O" ;
		pc_unitType = "PTA" ;
		return ;
	}
	if (pc_productType == "C")
	{
		pc_itinType = "O" ;
		pc_unitType = "LAN" ;
		return ;
	}
	if (pc_productType == "D")
	{
		pc_itinType = "S" ;
		pc_unitType = "SHIP" ;
		return ;
	}
	if (pc_productType == "F")
	{
		pc_itinType = "S" ;
		pc_unitType = "SEA" ;
		return ;
	}
	if (pc_productType == "G")
	{
		pc_itinType = "T" ;
		pc_unitType = "TOUR" ;
		return ;
	}
	if (pc_productType == "H")
	{
		pc_itinType = "S" ;
		pc_unitType = "SHIP" ;
		return ;
	}
	if (pc_productType == "I")
	{
		pc_itinType = "O" ;
		pc_unitType = "LLN" ;
		return ;
	}
	if (pc_productType == "J")
	{
		pc_itinType = "T" ;
		pc_unitType = "TOUR" ;
		return ;
	}
	if (pc_productType == "E")
	{
		pc_itinType = "O" ;
		pc_unitType = "OTH" ;
		return ;
	}
   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::fillTicketFormM5(CPNR &po_PNR)

{
	ENTER;

   int                li_pos ;
   AB_Integer         li_arraySize;
   AB_Text            lt_section ;
	AB_Varchar			 lv_tktString(255) ;
	AB_Char   			 lc_M5tkt(10) ;
	AB_Char            lc_match(1) ;
	AB_Char            lc_IurLnk(1) ;
	V_AB_Varchar       lv_arrayField(100,100) ;
	AB_Char            lc_paxId(2) ;
	AB_Varchar         lt_aofTicketNo(255);

   po_PNR.resetSegment("M5") ;
   lt_section = po_PNR.getNextSegment("M5");

	while ( lt_section.NotIsNull() )
	{
		bool	lb_ok = true;
		// reset exchange code

		md_ID_exchange_code[mi_ID_I] = AB_null;

		// Check existence of service fee

		AB_Text lt_svcFee;

		lt_svcFee = lt_section.getNthField(7,"/");
		lc_IurLnk = lt_section.At(7).For(1) ;

		lt_aofTicketNo = lt_section.getNthField(1,"/");

		if ( lt_svcFee.At(2).For(4) == AB_Text("-FEE") )
		{
          // M5 record is a service fee record

			 processM5SvcFee(lt_section);
		}
		else if (isLinkedItinIgnored(lc_IurLnk)) {
			// skip this M5
		}
		else if ( lt_section.At(8).For(1) != "R" )
		{
			if ( lt_section.At(5).For(2).isAllNumeric())
			{
				lc_paxId                   = lt_section.At(5).For(2) ;
     			md_ID_pax_id[mi_ID_I]      = lt_section.At(5).For(2) ;
			}
			else
			{
				md_ID_pax_id[mi_ID_I]      = md_IP_pax_id[0] ;
				lc_paxId                   = md_IP_pax_id[0] ;
				lc_paxId.rightJustify('0') ;
			}
     		md_ID_line_number[mi_ID_I] = mi_tktCounter++ ;
     		md_ID_reference[mi_ID_I]   = md_IRT_reference ;

			LOG(logDEBUG) << " AM==============BSP_OR_NOT=== lt_section =[" << lt_section << "]"  ;

			lc_IurLnk = lt_section.At(7).For(1) ;
			LOG(logDEBUG) << " AM==============BSP_OR_NOT=== lc_IurLnk =[" << lc_IurLnk << "]"  ;

			if ( lc_IurLnk != " ")
			{
				li_pos       = lt_section.findSubstring("/") ;

				if ( lt_section.At(li_pos+1).For(3).isAllNumeric() )
					md_ID_doc_type[mi_ID_I]    = "BSP" ; 		// MCO ARE BSP
				else
				{
					if ( lt_section.At(11).For(1) == "-" )
						md_ID_doc_type[mi_ID_I]    = "BSP" ;      // AMTRAK BSP
					else
						md_ID_doc_type[mi_ID_I]    = "TKT" ;
				}
			}
			else
     			md_ID_doc_type[mi_ID_I]    = "BSP" ;

			LOG(logDEBUG) << " AM==============BSP_OR_NOT=== md_ID_doc_type =[" << md_ID_doc_type[mi_ID_I] << "]"  ;

			lc_match = "N" ;

			if ( md_ID_doc_type[mi_ID_I]    == "BSP" )
			{
				if ( lt_section.At(8).For(1) == "G" )
				{
					md_ID_exchange_code[mi_ID_I] = "R" ;

				}
				else if ( lt_section.At(8).For(1) != " " )
				{
					md_ID_exchange_code[mi_ID_I] = "A" ;
				}
				else
				{
					md_ID_exchange_code[mi_ID_I] = AB_null ;
				}
				if ( md_ID_exchange_code[mi_ID_I].NotIsNull() )
				{
					li_pos       = lt_section.findSubstring("@") ;

					if ( li_pos > 0 )
					{

						md_ID_orig_ticket_no[mi_ID_I] =
										lt_section.At(li_pos + 1).For(14) ;

						li_pos = md_ID_orig_ticket_no[mi_ID_I].findSubstring("/") ;

						if (li_pos > 0)
							md_ID_orig_ticket_no[mi_ID_I] =
										md_ID_orig_ticket_no[mi_ID_I].For(li_pos - 1) ;
					}
				}

				lv_tktString                 = lt_section.At(12) ;

				li_arraySize = lv_arrayField.parseStringMultDel(lv_tktString,"/") ;

				if ( mc_iurType == "1" || mc_iurType == "B")
				{
					lc_M5tkt  = lv_arrayField[1].For(10) ;
					lc_match = checkM2tkt(lc_M5tkt) ;
				}

				if ( lc_match == "N" )
				{
					md_ID_sup_id[mi_ID_I]      = lt_section.At(9).For(2) ;

					if ( lt_section.At(9).For(2) == "XD")
					{
						// Service Charge MCO (TASF)
						createXDItin() ;
					}

				   if ( PCRE::pcre_match("/EMD", lt_section.to_string()))
					{
						createEMDItin() ;
					}

					// Assign Fare code to all the tickets
					md_ID_saving_code[mi_ID_I]     = md_savingCode ;
					md_ID_full_fare[mi_ID_I]       = md_fullFare ;
					md_ID_low_prac_fare[mi_ID_I]   = md_lowPracFare ;
					md_ID_rebate[mi_ID_I]          = md_rebate ;

					parseM5ticket(lv_tktString,
									  lv_arrayField,
									  li_arraySize,
                       	     md_ID_pax_id[mi_ID_I],
								     md_ID_line_number[mi_ID_I]) ;

					li_pos = lt_section.At(9).findSubstring("/V-") ;

					if ( li_pos > 0 )
					{
						md_ID_sup_id[mi_ID_I] = lt_section.At(9+li_pos+5).For(3) ;
						md_ID_doc_type[mi_ID_I]    = "TKT" ;
  						md_ID_ticket_no[mi_ID_I]   = lv_arrayField[1].For(10) ;
						md_ID_form[mi_ID_I]        = AB_null ;
					}
					else
					{
					   li_pos       = lt_section.At(9).findSubstring("/E-V-") ;
					   if ( li_pos > 0 )
					   {
					   	md_ID_sup_id[mi_ID_I] = lt_section.At(9+li_pos+7).For(3) ;
					   	md_ID_doc_type[mi_ID_I]    = "TKT" ;
  					   	md_ID_ticket_no[mi_ID_I]   = lv_arrayField[1].For(10) ;
					   	md_ID_form[mi_ID_I]        = AB_null ;
						}
					}

					if ( lc_IurLnk != " ")
					{
						AB_Char        	 lc_docId(2)  ;

						lc_docId = md_ID_line_number[mi_ID_I] ;

						updIrconsdetM5Tkt(lc_IurLnk,
												lc_paxId,
												lc_docId ) ;
					}
				}
				else
				{
					// GAP US092 START

   				AB_Varchar lt_formOfPay(50) ;
					AB_Char    lc_ccExpDate(4);

					lc_ccExpDate = AB_null;

					lv_arrayField[5].leftJustify() ;

					if ( lv_arrayField[5] == "ONE" ||
		  				  lv_arrayField[5] == "PER" ||
		  				  lv_arrayField[5] == "ALL" )
						li_pos = 6 ;
					else
					{
						if ( lv_arrayField[7] == "ONE" ||
			  				  lv_arrayField[7] == "PER" ||
			  				  lv_arrayField[7] == "ALL")
							li_pos = 8 ;
						else
							li_pos = 7 ;
					}

					// Get Form of payment

					lt_formOfPay = lv_arrayField[li_pos] ;

					processMiscellaneousFee(lt_formOfPay, md_ID_pax_id[mi_ID_I], lc_ccExpDate);

					// GAP US092 END

					mi_tktCounter-- ;
				}
			}
			else
			{
				// NON ARC PROCESSING

				LOG(logDEBUG) << " AM===BSP__NOT=== "  ;
				lv_tktString = lt_section.At(9) ;

				li_arraySize = lv_arrayField.parseStringMultDel(lv_tktString,"/") ;

				parseM5NonArcTicket(lv_tktString,   // ticket string
										  lv_arrayField,  // array
										  li_arraySize,   // size of array
										  md_ID_pax_id[mi_ID_I],
									     md_ID_line_number[mi_ID_I]) ;

				// Assign Fare code to all the tickets

				// GAP9110: if it is PFS, we will create a non-bsp ticket using the first air itin in the pnr
				// we check if the given sup_id is a PFS sup_id

            if ( isPFSSupplier(md_IRT_cop_id, md_ID_sup_id[mi_ID_I]) ){
					md_ID_rebate[mi_ID_I]          = md_rebate ;
					lb_ok = createPFSConsumdetail(md_ID_pax_id[mi_ID_I], md_ID_line_number[mi_ID_I]);
					if (!lb_ok) {
						// if no air itin can be found, we just ignore this non-bsp ticket
						// and its Irpayment
						if (mi_IRP_I > 0 &&
							md_IRP_line_number[mi_IRP_I-1] == md_ID_line_number[mi_ID_I]) {
							initIrpayment(mi_IRP_I-1);
							mi_IRP_I--;
						}
						initTicket(mi_ID_I) ;
						mi_tktCounter-- ;
					}
					else{
						md_ID_saving_code[mi_ID_I]   = AB_null;
						md_ID_full_fare[mi_ID_I]     = AB_null;
						md_ID_low_prac_fare[mi_ID_I] = AB_null;
						md_ID_rebate[mi_ID_I]        = AB_null;
					}
			}
			else if ( md_ID_sup_id[mi_ID_I] == "RFP" )
			{
				if ( md_CCRFP_cc_id.IsNull() )
				{
					LOG(logDEBUG) << " AM==============md_IRP_cc_no =[" << md_IRP_cc_no[mi_IRP_I-1] << "]"  ;
					md_CCRFP_cc_id = md_IRP_cc_id[mi_IRP_I-1];
					md_CCRFP_cc_no = md_IRP_cc_no[mi_IRP_I-1];
					md_CCRFP_cc_exp_date = md_IRP_cc_exp_date[mi_IRP_I-1];
				}

				// we ignore this non-bsp ticket
				// and its Irpayment
				if (mi_IRP_I > 0 &&
					md_IRP_line_number[mi_IRP_I-1] == md_ID_line_number[mi_ID_I]) {
					initIrpayment(mi_IRP_I-1);
					mi_IRP_I--;
				}
				initTicket(mi_ID_I) ;
				mi_tktCounter-- ;
				lb_ok = false;
			}
            else {
					md_ID_saving_code[mi_ID_I]     = md_savingCode ;
					md_ID_full_fare[mi_ID_I]       = md_fullFare ;
					md_ID_low_prac_fare[mi_ID_I]   = md_lowPracFare ;
					md_ID_rebate[mi_ID_I]          = md_rebate ;

					AB_Char        	 lc_docId(2)  ;

					lc_docId = md_ID_line_number[mi_ID_I] ;

					updIrconsdetM5Tkt(lc_IurLnk,
											lc_paxId,
											lc_docId ) ;
            }

			}

			if ( lb_ok && lc_match == "N" )
			{

				if ( md_ID_doc_type[mi_ID_I] != "BSP" )
				{
				   // look for FOPAP after mandatory ONE|ALL|PER/CC
				  if ( PCRE::pcre_match("(ONE|ALL|PER)/CC.+FOPAP", lv_tktString.to_string()))
					{
						LOG(logDEBUG) << "AM=== FOPAP found in M5" ;
						md_IRP_form_of_pay[mi_IRP_I-1] = "AP" ;
					}
				}
			}

			if ( lb_ok && lc_match == "N" )
				mi_ID_I++ ;
		}

  		lt_section = po_PNR.getNextSegment("M5");
	}
   EXIT;
}

//---------------------------------------------------------------------------

AB_Char CIUR::checkM2tkt(AB_Char  &pc_M5tkt)
{
   ENTER;

   int          li_counter ;
	AB_Char      lc_match(1) ;

   li_counter = 1 ;
	lc_match = "N" ;

   while (li_counter <= mi_tktIndex)
   {
      if (pc_M5tkt == md_tktData[li_counter])
		{
			lc_match = "Y" ;
			break ;
		}
      li_counter++ ;
   }
	return(lc_match) ;
}

AB_Boolean CIUR::scanTaxesAndOPAfields(V_AB_Varchar &pva_arrayField, int tax1Ind, int& tax2Ind, int& tax3Ind, int& OPAind)
{
   int			li_nextField;
	AB_Boolean	lb_taxesEnded,
					lb_retValue;
	AB_Varchar	lv_nextField(30);

	lb_taxesEnded = AB_false;
	lb_retValue   = AB_false;
	tax2Ind = -1;
	tax3Ind = -1;
	OPAind = -1;

	// TAX1
	li_nextField = tax1Ind;

	// other TAXES,
	// TAX2 - optional
	li_nextField++;
	lv_nextField = pva_arrayField[li_nextField] ;
	LOG(logDEBUG) << "AM====SCAN T2? [" << lv_nextField << "]"  ;
	if ( lv_nextField == "ONE" || lv_nextField == "PER" || lv_nextField == "ALL" )
	{
		lb_taxesEnded = AB_true;
		li_nextField--;
	}
	else
	{
	   tax2Ind = li_nextField;
	}

	// TAX3 - optional
	if ( lb_taxesEnded != AB_true )
	{
		li_nextField++;
		lv_nextField = pva_arrayField[li_nextField] ;
		LOG(logDEBUG) << "AM====SCAN T3? [" << lv_nextField << "]"  ;
		if ( lv_nextField == "ONE" || lv_nextField == "PER" || lv_nextField == "ALL" )
		{
			lb_taxesEnded = AB_true;
			li_nextField--;
		}
		else
		{
			tax3Ind = li_nextField;
		}
	}

	// ONE/PER/ALL field -mandatory
	li_nextField++;
	lv_nextField = pva_arrayField[li_nextField] ;
	LOG(logDEBUG) << "AM====SCAN OPA!!! [" << lv_nextField << "]"  ;
	if ( lv_nextField == "ONE" || lv_nextField == "PER" || lv_nextField == "ALL" )
	{
		lb_retValue = AB_true;
		OPAind = li_nextField;
	}

	LOG(logDEBUG) << "AM====SCAN RETURNS(" << lb_retValue << ") TAX2=" << tax2Ind << " TAX3=" << tax3Ind << " OPA=" << OPAind  ;
	return lb_retValue;
}

//--------------------------------------------------------------------------

void CIUR::parseM5NonArcTicket(AB_Varchar   &pv_tktString,
										 V_AB_Varchar &pva_arrayField,
							  	       AB_Integer   &pi_arraySize,
							  	       AB_Integer   &pi_paxId,
								       AB_Integer   &pi_docId)
{
	ENTER;

   int                li_pos,
							 li_taxCounter ,
							 i, li_noOfPax ,
							 li_nextField,
							 li_tax1Ind,
							 li_tax2Ind,
							 li_tax3Ind,
							 li_OPAind,
							 li_len;
   AB_Text            lt_section ;
   AB_Varchar         lt_formOfPay(50) ;
	AB_Varchar			 lv_taxString(30) ,
							 lv_nextField(30),
							 lv_ticketNo(10);
	AB_Integer         li_tktLen ;
	AB_Boolean			 lb_statOK;

	LOG(logDEBUG) << "AM====22222222222=== pva_arrayField[1] = ["  << pva_arrayField[1] << "]" ;
		md_ID_sup_id[mi_ID_I]      = pva_arrayField[1].At(4).For(3) ;

		//md_ID_amounts_source[mi_ID_I] = "M5" ;

		li_tktLen = pva_arrayField[2].Length() ;

		if ( li_tktLen > AB_Integer(3) )
		{
			li_tktLen = li_tktLen - AB_Integer(3);

			li_len = li_tktLen.to_long() ;
			if ( li_len > 10 )
				li_len = 10 ;

  			md_ID_ticket_no[mi_ID_I]   = pva_arrayField[2].At(4).For(li_len) ;
		}
		else
		{
			/* change as per scr#2788
			** when ticket_no is already 999999999 then retain ticket_no
			** but to identify non-arc tickets that do not have
			** specifi ticket_no pass 0000000000
			*/

			lv_ticketNo = pva_arrayField[2].At(4).For(10) ;

			lv_ticketNo.removeLeadAndTrailBlanks() ;


			if (lv_ticketNo.IsNull() )
				md_ID_ticket_no[mi_ID_I] = "0000000000" ;
			else
				md_ID_ticket_no[mi_ID_I] = pva_arrayField[2].At(4).For(10) ;

		}

		li_tax1Ind = 5;
		if (scanTaxesAndOPAfields(pva_arrayField, li_tax1Ind, li_tax2Ind, li_tax3Ind, li_OPAind) != AB_true )
			return;

		if ( pva_arrayField[li_OPAind] == "PER" )
		{
			// Multiply base cost Tax, commission doller amount by no of Pax

			li_noOfPax = mi_IP_I  ;
		}
		else
		{
			li_noOfPax = 1 ;
		}

		md_ID_cur_id[mi_ID_I]      = "USD" ;

		pva_arrayField[3].leftJustify() ;
		md_ID_commission[mi_ID_I]  = pva_arrayField[3] ;

		li_pos       = md_ID_commission[mi_ID_I].findSubstring("P") ;
		if (li_pos > 0)
		{
			md_ID_comm_rate[mi_ID_I]  = md_ID_commission[mi_ID_I].At(li_pos+1);
			md_ID_commission[mi_ID_I] = AB_null ;
		}
		else
		{
			md_ID_commission[mi_ID_I].leftJustify() ;
			D_amount ld_comm ;

			ld_comm = md_ID_commission[mi_ID_I] ;
			ld_comm = ld_comm * AB_Integer(li_noOfPax) ;

			md_ID_commission[mi_ID_I] = ld_comm ;
		}

		/*
		md_ID_base_cost[mi_ID_I] = pva_arrayField[4] ;
		md_ID_base_cost[mi_ID_I].leftJustify();
		*/


   	if ( pva_arrayField[4].For(3).isAllAlpha() ){
			 md_ID_cur_id[mi_ID_I] = pva_arrayField[4].For(3);
          md_ID_base_cost[mi_ID_I] = pva_arrayField[4].At(4);
			 md_ID_base_cost[mi_ID_I].leftJustify();
		}
		else{
			md_ID_cur_id[mi_ID_I] = "USD" ;
	      md_ID_base_cost[mi_ID_I] = pva_arrayField[4] ;
	      md_ID_base_cost[mi_ID_I].leftJustify();
		}

	D_amount ld_baseCost ;
	ld_baseCost = md_ID_base_cost[mi_ID_I] ;
	ld_baseCost = ld_baseCost * AB_Integer(li_noOfPax) ;
	md_ID_base_cost[mi_ID_I] = ld_baseCost ;


		D_amount lb_baseCost ;
		lb_baseCost = md_ID_base_cost[mi_ID_I] ;
		lb_baseCost = lb_baseCost * AB_Integer(li_noOfPax) ;
		md_ID_base_cost[mi_ID_I] = lb_baseCost ;

		li_taxCounter = 0 ;

		// Process Taxes
		// TAX 1
		lv_taxString = pva_arrayField[li_tax1Ind] ;

		lv_taxString.leftJustify() ;

		if (lv_taxString != "0.00" )
		{
			li_taxCounter++ ;
			md_ITD_line_number[mi_ITD_I]  = pi_docId ;
			md_ITD_pax_id[mi_ITD_I]       = pi_paxId ;
			md_ITD_tax_id[mi_ITD_I]       = "XT" ;
         md_ITD_tax_source[mi_ITD_I]   = "M5";
			md_ITD_tax_amount[mi_ITD_I] = lv_taxString.At(1) ;
			md_ITD_tax_amount[mi_ITD_I].leftJustify() ;

			D_amount ld_tax ;
			ld_tax = md_ITD_tax_amount[mi_ITD_I] ;
			ld_tax = ld_tax * AB_Integer(li_noOfPax) ;
			md_ITD_tax_amount[mi_ITD_I] = ld_tax ;

			mi_ITD_I++ ;
		}

		// Form of payment FIELD
		lt_formOfPay = pva_arrayField[li_OPAind+1] ;

	   // Get optional cc exp date

	   AB_Char    lc_ccExpDate(4);
      int        li_expPos;

	   li_expPos = pv_tktString.findSubstring("-EXP$");

	   if ( li_expPos > 0 &&
		     pv_tktString.At(li_expPos+5).For(2).isAllNumeric()  &&
		     pv_tktString.At(li_expPos+7).For(1) == AB_Text("/") &&
		     pv_tktString.At(li_expPos+8).For(2).isAllNumeric()    )
      {
         lc_ccExpDate = pv_tktString.At(li_expPos+5).For(2) +
                        pv_tktString.At(li_expPos+8).For(2) ;
	   }
	   else
   	{
			set <Text2Text>::iterator it;
			it = M02_TktNo2ExpDate.find(Text2Text(md_ID_ticket_no[mi_ID_I], ""));
			if ( it != M02_TktNo2ExpDate.end() )
			{
				LOG(logDEBUG) << "FOUND in MAP:" << it->_key << "\t" << it->_value ;
				lc_ccExpDate = it->_value;
			}
			else
			{
				LOG(logDEBUG) << "tkt:[" << md_ID_ticket_no[mi_ID_I] << "]" ;
				LOG(logDEBUG) << "NOT FOUND in MAP:"  ;
				lc_ccExpDate = AB_null;
			}
	   }

		processMiscellaneousFee(lt_formOfPay, pi_paxId, lc_ccExpDate);

		fillFOPM5(pi_paxId,
					 pi_docId,
					 lt_formOfPay,
					 lc_ccExpDate) ;

	LOG(logDEBUG) << "AM====22222222222=== pva_arrayField[li_OPAind+2] = ["  << pva_arrayField[li_OPAind+2] << "]" ;
      md_ID_no_of_books[mi_ID_I] = pva_arrayField[li_OPAind+2].For(1) ;

		if ( md_ID_ticket_no[mi_ID_I].IsNull() )
		{
			md_ID_no_of_books[mi_ID_I] = "1" ;
		}
      EXIT;
}

//---------------------------------------------------------------------------

void CIUR::updIrconsdetM5Tkt(AB_Char      &pc_M5link,
									  AB_Char		&pc_paxId,
									  AB_Char		&pc_docId)
{
   ENTER;

	AB_Integer	 li_OthItin ;
	int		    li_simIndex ;
	AB_Boolean   lb_foundICD ;
	AB_Char      lc_itinLink(1) ;

	AB_Integer	li_itinLine ;

	li_OthItin = 1 ;

	while ( li_OthItin <= AB_Integer(mi_NonArcItin) )
	{
		lc_itinLink = md_NonArcItin[li_OthItin].At(3).For(1) ;

		li_itinLine = md_NonArcItin[li_OthItin].For(2) ;

		if (pc_M5link == lc_itinLink )
		{
			int li_index = 0 ;
			while( li_index < mi_ICD_I )
			{
				lb_foundICD = AB_false ;

				if (md_ICD_itin_line_number[li_index] == li_itinLine)
				{
					li_simIndex = li_index ;
					// Found itin line in consumedetail
					if (md_ICD_line_number[li_index].IsNull())
					{
						// Found a free line to put the ticket no
						md_ICD_line_number[li_index]  = pc_docId ;
						md_ICD_pax_id[li_index] = pc_paxId ;
						lb_foundICD = AB_true ;
						li_simIndex = li_index ;
						break ;
					}

				}
				li_index++ ;
			}

			if ( !lb_foundICD )
			{
				// Added a line for more than one ticket per pax for same itin

				md_ICD_pax_id[mi_ICD_I] = pc_paxId ;
				md_ICD_itin_type[mi_ICD_I] = md_ICD_itin_type[li_simIndex] ;
				md_ICD_itin_line_number[mi_ICD_I] = md_ICD_itin_line_number[li_simIndex] ;
				md_ICD_line_number[mi_ICD_I] = pc_docId ;
				li_simIndex = mi_ICD_I ;
			}

			if ( !lb_foundICD )
				mi_ICD_I++ ;

		}

		li_OthItin = li_OthItin + AB_Integer(1) ;
	}
   EXIT;
}

//----------------------------------------------------------------------------

void CIUR::updFareInfo()
{
   ENTER;

	if ( mi_ID_I < 2)
	{
		if (md_ID_doc_type[0] == "BSP" )
		{
			md_ID_saving_code[0]     = md_savingCode ;
			md_ID_full_fare[0]       = md_fullFare ;
			md_ID_low_prac_fare[0]   = md_lowPracFare ;
			md_ID_rebate[0]          = md_rebate ;
		}
	}
   EXIT;
}

//--------------------------------------------------------------------------

AB_Boolean CIUR::existMatchingM5(AB_Char  &pc_paxId,
								         AB_Char  &pc_docLine)
{
	ENTER;

	/**
	 * New method cloned from getM5Fop just to
	 * detect existence of matching M5
	 **/

	AB_Text      lt_section ;
	AB_Text      lt_ticketInfo ;
	AB_Char      lc_M5Form(4) ;
	AB_Char      lc_M5Tkt(6) ;
	AB_Integer	 li_paxId, li_docId;
	AB_Varchar   lv_formOfPay(50) ;
	V_AB_Varchar lv_arrayField(100,100) ;

	int          i;
	int          li_index ;
	AB_Boolean   lb_found ;

	li_index  = 1;

   while ( li_index < mi_M5AccIndex )
	{
		lb_found = AB_false ;
		lt_section = mva_M5AccSection[li_index] ;

		if ( lt_section.At(5).For(2) == pc_paxId  ||
			  lt_section.At(5).For(2) == "AA"      ||
			  lt_section.At(5).For(2) == "PP")
		{
			lt_ticketInfo   = lt_section.At(12) ;

			i = lv_arrayField.parseStringMultDel(lt_ticketInfo,"/") ;

			lc_M5Form        = lv_arrayField[1].For(4) ;
			lc_M5Tkt         = lv_arrayField[1].At(5).For(6) ;

			if ( lc_M5Form == md_ID_form[mi_ID_I].For(4) &&
				  lc_M5Tkt == md_ID_ticket_no[mi_ID_I].For(6) )
			{
				lb_found = AB_true ;

				if ( mb_netFareTicket == AB_true )
				{
					AB_Varchar		lv_base_cost(20);

					if ( lv_arrayField[3].For(3).isAllAlpha() ){
						 lv_base_cost = lv_arrayField[3].At(4);
					}
					else{
						 lv_base_cost = lv_arrayField[3] ;
					}

					lv_base_cost.leftJustify();

					if ( lv_base_cost > md_ID_base_cost[mi_ID_I] )
					{
						// Add NFT/NP- remark virtually
						md_IRMK_type[mi_IRMK_I]     = "NFT" ;
						md_IRMK_remark[mi_IRMK_I]   = "C" ;
						md_IRMK_pax_id[mi_IRMK_I++] = AB_null;

						md_IRMK_type[mi_IRMK_I]     = "NPC" ; // for NFT/NP-xxx.xx
						md_IRMK_remark[mi_IRMK_I]   = lv_base_cost;
						md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
					}
				}

				break ;
			}
		}
		li_index++ ;
	}
	return (lb_found) ;
}

//--------------------------------------------------------------------------

void CIUR::getOrigFop(AB_Text &pt_M2)
{
   ENTER;

	// to be removed

	V_AB_Varchar lv_arrayField(200,200) ;
	AB_Varchar   lv_fp(200);
	int          li_size ;

	li_size = lv_arrayField.parseStringMultDel(pt_M2,"!") ;
	lv_fp   = lv_arrayField[4] ;

	int li_pos;

	//LOG(logDEBUG) << "lv_fp:"<<lv_fp;

	li_pos = lv_fp.findSubstring("*EF");

	if ( li_pos > 0 ) {

		 lv_fp = lv_fp.At(li_pos+3);
       lv_fp.removeLeadAndTrailBlanks();

		 if ( lv_fp.For(2) == "CC" )
		 {
			  lv_fp = lv_fp.At(3);
		 }

       if ( lv_fp.For(2).isAllAlpha() &&
				lv_fp.At(3).isAllNumeric() )
       {
           /**
				* Credit card payment VI12345678
				**/

            md_orig_form_of_pay = "CC";
            md_orig_cc_id = lv_fp.For(2);
            md_orig_cc_no = lv_fp.At(3);

	         md_orig_cc_exp_date = AB_null;
	         md_orig_auth_no     = AB_null;
	         md_orig_auth_source = AB_null;
       }
		 else if ( lv_fp == "CHECK" || lv_fp == "CHQ" || lv_fp == "CK" )
		 {
            md_orig_form_of_pay = "CK";
            md_orig_cc_id       = AB_null;
            md_orig_cc_no       = AB_null;

	         md_orig_cc_exp_date = AB_null;
	         md_orig_auth_no     = AB_null;
	         md_orig_auth_source = AB_null;
       }
		 else
		 {
         	/**
		       * Default to cash
		       **/

				md_orig_form_of_pay = "CA";
            md_orig_cc_id       = AB_null;
            md_orig_cc_no       = AB_null;

	         md_orig_cc_exp_date = AB_null;
	         md_orig_auth_no     = AB_null;
	         md_orig_auth_source = AB_null;
		 }
	}
	else
	{
		 /**
		  * Default to cash
		  **/

       md_orig_form_of_pay = "CA";
       md_orig_cc_id       = AB_null;
       md_orig_cc_no       = AB_null;
       md_orig_cc_exp_date = AB_null;
	    md_orig_auth_no     = AB_null;
	    md_orig_auth_source = AB_null;
	}

	//LOG(logDEBUG) << "md_orig_form_of_pay:"<<md_orig_form_of_pay;
	//LOG(logDEBUG) << "md_orig_cc_id      :"<<md_orig_cc_id;
	//LOG(logDEBUG) << "md_orig_cc_no      :"<<md_orig_cc_no;

   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::createXDItin()
{
	ENTER;

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
	md_ICD_itin_type[mi_ICD_I]      		= "O" ;
	md_ICD_itin_line_number[mi_ICD_I]   = md_IO_line_number[mi_IO_I] ;
	/* SCR 334463 BEGIN
	md_ICD_pax_id[mi_ICD_I]					= md_IP_pax_id[0] ;
	SCR 334463 END */

	//md_ICD_pax_id[mi_ICD_I]             = ( mi_ID_I > 0 ) ? md_ID_pax_id[mi_ID_I] : md_IP_pax_id[0] ;
	md_ICD_pax_id[mi_ICD_I]             = md_ID_pax_id[mi_ID_I]  ;
	if(md_ICD_pax_id[mi_ICD_I].IsNull() )
		md_ICD_pax_id[mi_ICD_I]             = md_IP_pax_id[0] ;

	md_ICD_line_number[mi_ICD_I++]		= md_ID_line_number[mi_ID_I] ;

	md_IRMK_remark[mi_IRMK_I]           = "TRAVEL AGENCY SERVICE FEE" ;
	md_IRMK_itin_type[mi_IRMK_I]        = "O" ;
	md_IRMK_oth_line_number[mi_IRMK_I++]  = md_IO_line_number[mi_IO_I] ;

   mi_IO_I++ ;
   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::createEMDItin()
{
	ENTER;

	AB_Date     ld_date ;
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


	md_IO_sup_id[mi_IO_I] = md_ID_sup_id[mi_ID_I] ;

	md_IO_itin_type[mi_IO_I] = "O" ;

	ld_date.getGMTNow() ;

	if ( airFound )
	{
		md_IO_start_date[mi_IO_I] = md_IT_start_date[firstAirTransportInd];
		md_IO_orig_city_id[mi_IO_I] = md_IT_orig_city_id[firstAirTransportInd];
		md_IO_dest_city_id[mi_IO_I] = md_IT_orig_city_id[firstAirTransportInd];
	}
	else
	{
		md_IO_start_date[mi_IO_I] = ld_date.to_stringFormat("DDMMMYY") ;
		md_IO_orig_city_id[mi_IO_I] = "ZZZ";
		md_IO_dest_city_id[mi_IO_I] = "ZZZ";
	}
   md_IO_unit_ct[mi_IO_I] = 1 ;
   md_IO_unit_type[mi_IO_I] = "EMD" ;
	md_IO_line_number[mi_IO_I] = mi_IO_I + 1 ;
   md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;
   md_IO_reference[mi_IO_I] = md_IRT_reference ;
	md_ICD_itin_type[mi_ICD_I]      		= "O" ;
	md_ICD_itin_line_number[mi_ICD_I]   = md_IO_line_number[mi_IO_I] ;
	//md_ICD_pax_id[mi_ICD_I]             = ( mi_ID_I > 0 ) ? md_ID_pax_id[mi_ID_I] : md_IP_pax_id[0] ;
	md_ICD_pax_id[mi_ICD_I]             = md_ID_pax_id[mi_ID_I]  ;
	if(md_ICD_pax_id[mi_ICD_I].IsNull() )
		md_ICD_pax_id[mi_ICD_I]             = md_IP_pax_id[0] ;

	md_ICD_line_number[mi_ICD_I++]		= md_ID_line_number[mi_ID_I] ;

   mi_IO_I++ ;
   EXIT;
}

//--------------------------------------------------------------------------

void CIUR::getTotM5Taxes(V_AB_Varchar  &pva_arrayField,
								 D_amount      &pd_totTax )
{
	ENTER;

	AB_Varchar   lv_taxString(10) ;

	// Add up taxes in M5

	lv_taxString = pva_arrayField[4] ;

	pd_totTax = D_amount(0) ;

	lv_taxString.leftJustify() ;

	if (lv_taxString != "0.00" )
	{
		pd_totTax  = pd_totTax + D_amount(lv_taxString) ;
	}

	lv_taxString = pva_arrayField[5] ;

	lv_taxString.leftJustify() ;

	if (lv_taxString != "ONE" && lv_taxString != "PER" && lv_taxString != "ALL")
	{

		if (lv_taxString.At(2) != "0.00" )
		{
			// For US If there is a 2nd Tax it goes as XT

			pd_totTax  = pd_totTax + D_amount(lv_taxString) ;

		}
	}
   EXIT;
}

//---------------------------------------------------------------------------

void CIUR::processM5SvcFee(AB_Text  &pt_section)
{
	ENTER;

	AB_Varchar    lv_supIdField(16) ;
	V_AB_Varchar  lva_arrayField(100,100) ;
	AB_Varchar    lv_arrayfield(100) ;
	AB_Char       lc_fp(20) ;
	AB_Char       lc_supId(3) ;
	AB_Char       lc_svcFee(13) ;
	AB_Char       lc_accountLine(2) ;
	AB_Char       lc_paxId(2) ;
	AB_Char       lc_section(2) ;
	int           li_ctr,li_length ;
	AB_Char       lc_svcDescription(20) ;
	AB_Varchar    lv_ticketNoField(16) ;

	lc_svcDescription = " ";

	lc_section = "M5" ;

	li_ctr = lva_arrayField.parseStringMultDel(pt_section,"/") ;

	// 1. Pick up item no, possibly 01-99 PP(PER) AA(ALL)
	//    Three types are mutually exclusive.
	//
   //    ## can show multiple times, can not duplicate

	lc_accountLine = pt_section.At(3).For(2) ;

	lc_paxId = pt_section.At(5).For(2) ;

   lc_paxId.rightJustify('0');

	/**
	 *  ## "S"  (Single pax id)
	 *  AA "A"  (ALL)
	 *  PP "P"  (PER)
	 **/

	if ( mc_itemType.IsNull() )
	{
		// First record of service fee

	   if ( lc_paxId.isAllNumeric() )
		{
			// Single pax

			mc_itemType = "S";
			mv_paxLst = lc_paxId + "#";
      }
		else if ( lc_paxId == AB_Text("AA") )
		{
			// All documents

			mc_itemType = "A";
      }
		else if ( lc_paxId == AB_Text("PP") )
		{
			// Per pax

			mc_itemType = "P";
      }
		else
		{
			// Invalid record

			mi_rejectirId = AB_Integer(3618);
			return;
		}
	}
   else if ( mc_itemType == AB_Text("A") ||
             mc_itemType == AB_Text("P") )
	{
      // Already set to ALL or PER,
		// can not set to anything else

		mi_rejectirId = AB_Integer(3618);
		return;
   }
   else if ( mc_itemType == AB_Text("S") )
	{
		// Already set to type S, so take
		// the one that has not been picked up;

		if ( lc_paxId.isAllNumeric() ) {

			// Check if it is duplicate

			int li_index;
		   li_index = mv_paxLst.findSubstring(lc_paxId.to_string());

			if ( li_index > 0 )
			{
	   	   // Can not set to anything else

	   	   //mi_rejectirId = AB_Integer(3618);
	   	   //return;
			}
			else
			{
			  // It is not duplicate, create service fee

			  mv_paxLst = mv_paxLst + lc_paxId + "#";
			}
		}
		else
      {
	   	// Can not set to anything else

	   	mi_rejectirId = AB_Integer(3618);
	   	return;
      }
	}

	// 2. Pick up supplier id

	/*
	lv_supIdField = lva_arrayField[1] ;

	lv_supIdField.removeTrailingBlanks() ;

	li_length = lv_supIdField.Length().to_long() ;

	lc_supId  = lv_supIdField.At(li_length - 2).For(3) ;
   */

	// 2a. G9140 - pick up ticket number for AOF: ...../FPT1020308907/......
	saveTicketNo4AOF(lva_arrayField[2]);


	// 3. Pick up service fee amount
	// Might have leading 3 character currency code, how to filter that???

	lc_svcFee = lva_arrayField[4] ;
	lc_svcFee.leftJustify();

	AB_Char lc_copyFee(13) ;
	lc_copyFee = lva_arrayField[4];

   D_amount ld_amount;

	// use lc_copyFee in input_conversion_trigger because of a possible
	// bug in input_conversion_trigger()
	ld_amount.input_conversion_trigger(lc_copyFee);

   if (! CMsgHandler::getMsgStatus())
	{
		LOG(logDEBUG) << "-- CMsgHandler::getMsgId() = " << CMsgHandler::getMsgId() ;
		CMsgHandler::clearMsg();

	   lc_svcFee = lc_svcFee.At(4);

	   ld_amount.input_conversion_trigger(lc_svcFee);

      if (! CMsgHandler::getMsgStatus())
      {
		   CMsgHandler::clearMsg();
			mi_rejectirId = AB_Integer(3618);
      }
	}

	// Pick up form of payment

	lc_fp = lva_arrayField[7].For(20);

	/* treat "CX" like "CC" SCR# 294105 */

	if (lc_fp.For(2) == "CX")
	{
		lc_fp = "CC" + lc_fp.At(3);
	}

   int  li_index;

   li_index = lc_fp.findSubstring(" ");

	if (li_index > 0 )
	{
     lc_fp = lc_fp.For(li_index-1);
	}

	// Pick up cc expiration date

	AB_Varchar lv_ccExpDateStrA(30);
	AB_Varchar lv_ccExpDateStrB(30);
   AB_Char    lc_ccExpDate(4);

	lc_ccExpDate = AB_null;

   if ( lc_fp.For(2) == "CC" || lc_fp.For(2) == "CX")
	{
		/**
		 * Resolve cc expiration date  1-FEE-EXP$1201
		 *                             1-FEE-EXP$12/01
		 **/

	   lv_ccExpDateStrA = lva_arrayField[8];  // 1-FEE-EXP$12
	   lv_ccExpDateStrB = lva_arrayField[9];  // 01

      lv_ccExpDateStrA.removeLeadAndTrailBlanks();
      lv_ccExpDateStrB.removeLeadAndTrailBlanks();

	   int li_expPos;

	   li_expPos = lv_ccExpDateStrA.findSubstring("-EXP$");

      if ( li_expPos > 0  &&
           lv_ccExpDateStrA.At(li_expPos+5).For(4).isAllNumeric() )
      {
	   	/**
	   	 * CC expiration date info is present
	   	 **/

	      lc_ccExpDate = lv_ccExpDateStrA.At(li_expPos+5);
      }
		else if ( li_expPos > 0                               &&
                lv_ccExpDateStrA.At(li_expPos+5).isAllNumeric()  &&
                lv_ccExpDateStrA.At(li_expPos+5).length() == 2   &&
                lv_ccExpDateStrB.For(2).isAllNumeric() )
      {
	   	/**
	   	 * CC expiration date info is present
	   	 **/

	      lc_ccExpDate = lv_ccExpDateStrA.At(li_expPos+5) +
		   					lv_ccExpDateStrB.For(2) ;
      } else {

	      lc_ccExpDate = " ";
		}
	}

	if ( lc_ccExpDate.IsNull() )
		lc_ccExpDate = " ";

	// SCR 412640 Start
	bool lb_cc_valid = true;
   if (lc_fp.For(2) == "CC")  {
		AB_Varchar lv_cc_id(40);
		long		  ll_error_code;
		lv_cc_id = lc_fp.At(3);
		if (!validCreditCard(lv_cc_id, lc_ccExpDate, ll_error_code)) {
			lb_cc_valid = false;
		}
	}
	// SCR 412640 End

	md_IRMK_type[mi_IRMK_I] = "SFC" ;

	md_IRMK_remark[mi_IRMK_I] = (lb_cc_valid? "" : "BADCC/") +
										 AB_Text("SAB") +
										 lc_fp          +
										 lc_ccExpDate   +
										 lc_paxId       +
										 lc_svcDescription +
										 lc_svcFee +
										 lc_accountLine;

	md_IRMK_pax_id[mi_IRMK_I++] = AB_null ;

	EXIT;
}

//--------------------------------------------------------------------------

void CIUR::saveSvcIrregularity()
{
	ENTER;

   D_description   ld_errorDescription ;
   AB_Integer      li_msgId ;

   md_queType = "SFC" ;

   li_msgId = mi_rejectirId;

   //CMsgHandler::getMsgDescWithSub(li_msgId, "E",
	//										 mv_rejectirSub,
	//										 ld_errorDescription) ;

   CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;

   createQueue(ld_errorDescription) ;

	EXIT;
}

//------------------------------------------------------------------------------
void CIUR::processFrequentF(CPNR &po_PNR,AB_Varchar &pc_full_name)
{
   AB_Text  lt_segment ;
   int      li_length ;
   int      li_start ;
   AB_Char  lc_sup(2) ;
   AB_Char  lc_srvc_sup(2) ;
	AB_Char  lc_fup_status(2) ;
   AB_Varchar  lvc_fup(26);

   pc_full_name.removeTrailingBlanks();

   li_length = pc_full_name.Length().to_long() ;

   po_PNR.resetSegment("M9") ;
   lt_segment = po_PNR.getNextSegment("M9") ;
   li_start = mi_IRF_I ;

   while (lt_segment.NotIsNull())
   {
      if (lt_segment.At(55).For(li_length) == pc_full_name.At(1).For(li_length))
      {
         lc_sup = lt_segment.At(6).For(2) ;
         lc_srvc_sup = lt_segment.At(37).For(2) ;
			lc_fup_status = lt_segment.At(34).For(2) ;
			lvc_fup = lt_segment.At(8).For(26);
			lvc_fup.leftJustify();
         if (checkForDuplicateFF(li_start,lc_sup, lc_srvc_sup, lvc_fup)  && lc_fup_status != "NO" )
         {
            md_IRF_fup[mi_IRF_I]         = lt_segment.At(8).For(26) ;
            md_IRF_fup[mi_IRF_I].leftJustify() ;
            md_IRF_pax_id[mi_IRF_I]      = md_IP_pax_id[mi_IP_I] ;
            md_IRF_sup_id[mi_IRF_I]      = lt_segment.At(6).For(2) ;
            md_IRF_fup_status[mi_IRF_I]  = lt_segment.At(34).For(2) ;
            md_IRF_srvc_sup_id[mi_IRF_I] = lt_segment.At(37).For(2) ;

				/**
				 * Extract fup_type in between two *
				 **/

			   int li_first, li_second;

				li_first = lt_segment.At(55+li_length).findSubstring("*");

				if ( li_first > 0 ) {

				  li_second =
				  lt_segment.At(55+li_length+li_first).findSubstring("*");

				  if ( li_second > 0 ) {

					  /**
						* Frequent user type
						**/

					  md_IRF_fup_type[mi_IRF_I] =
						 lt_segment.At(55+li_length+li_first).For(li_second-1);
					  md_IRF_fup_type[mi_IRF_I].leftJustify();
				  }
				}
            mi_IRF_I++;
         }
      }
      lt_segment = po_PNR.getNextSegment("M9") ;
   }
}

//-----------------------------------------------------------------------------

AB_Boolean CIUR::checkForDuplicateFF(int  pi_start,
												 const AB_Char& pc_sup,
												 const AB_Char& pc_srvc_sup,
												 const AB_Varchar& pvc_fup) const
{
   while ( pi_start <  mi_IRF_I )
   {
      if (md_IRF_pax_id[pi_start] == md_IP_pax_id[mi_IP_I]
         && md_IRF_sup_id[pi_start] == pc_sup
			&& md_IRF_srvc_sup_id[pi_start] == pc_srvc_sup )
			//&& md_IRF_fup[pi_start] == pvc_fup)

         return(AB_false) ;

      pi_start++ ;
   }
   return(AB_true) ;
}

//-----------------------------------------------------------------------------

// US044 START
// GAP US089 START
void CIUR::IrTransportRemarks(CPNR &po_PNR)
{
   ENTER;
   AB_Text        lt_segment, lt_remark;
   int            li_pos;
   D_line_number  ld_line_number;

   int li_index;

   po_PNR.resetSegment("M7") ;
   lt_segment = po_PNR.getNextSegment("M7");

   while (lt_segment.NotIsNull())
   {
      li_pos = lt_segment.findSubstring("!") ;

      if ( li_pos > 0 )
         lt_segment = lt_segment.For(li_pos-1) ;

      lt_remark = lt_segment.At(5);

      createUnAssociatedRemark( lt_remark );

      lt_segment = po_PNR.getNextSegment("M7");
   }


   for ( li_index=1; li_index < mi_MERemarkIndex; li_index++ )
   {
      ld_line_number = mva_MERemarkSection[li_index].For(2);
      lt_remark      = mva_MERemarkSection[li_index].At(3);

      if ( ! createTransportAssociatedRemark( ld_line_number, lt_remark ) )
         createRentalAssociatedRemark( ld_line_number, lt_remark );
   }

   EXIT;
}
// GAP US089 END

//-----------------------------------------------------------------------------

// US044 END

// SCR 231339 START

void CIUR::updateTransportSupplier()
{
	D_supplier ld_sup_id;

	if ( mi_ID_I > 0 )
		ld_sup_id = md_ID_sup_id[0];
	else
		ld_sup_id = "BUS";

	for ( int li_index=0; li_index < mi_IT_I; li_index++ )
	{
		if ( 	md_IT_itin_type[li_index] == "B" &&
				md_IT_sup_id[li_index].IsNull() )
		{
			md_IT_sup_id[li_index] = ld_sup_id;
		}
	}
}

// SCR 231339 END

// G9090c start
void CIUR::validateItin() {
	ENTER;
	for (int i = 0; i < mi_IR_I; i++) {
		if (md_IR_prod_code[i] == "HHT" &&
			 md_IR_sup_id[i].For(2) == "NL") {

			for (int j = 0; j < mi_IO_I; j++) {
				if (md_IO_prod_code[j] == "OTH") {
					if (md_IR_origin_city_id[i] == md_IO_dest_city_id[j]) {
						md_IO_prod_code[j] = "IGNORE OTH";
					}
				}
			}
		}
	}
   EXIT;
}

bool CIUR::isLinkedItinIgnored(const AB_Char& pc_M5link) {
	for (int i = 1; i <= mi_IgnoredItin; i++) {
		if (pc_M5link == md_IgnoredItin[i].At(3).For(1)) {
			return true;
		}
	}
	return false;
}
// G9090c end


// SCR 333749

void CIUR::processEmailaddress( CPNR &po_PNR)
{
   AB_Text     lt_MF_section;
   AB_Varchar  lv_email(65);
   AB_Varchar  lv_email_before(65);
   AB_Varchar  lv_email_after(65);
	int			li_pos1;
	int			li_pos2;
	int			li_pos;

   po_PNR.resetSegment("MF") ;
   lt_MF_section = po_PNR.getNextSegment("MF");

   while (lt_MF_section.NotIsNull())
   {
		li_pos1 = lt_MF_section.findSubstring( "$", 1 );
		li_pos2 = lt_MF_section.findSubstring( "$", li_pos1 + 1 );

		if ( li_pos1 > 0 && li_pos2 > 0 )
		{
			lv_email = lt_MF_section.At( li_pos1 + 1 ).For( li_pos2 - li_pos1 - 1 );

			if ( lv_email.NotIsNull() )
			{
				li_pos = lv_email.findSubstring( "==", 1 );

				if ( li_pos > 0 )
				{
					lv_email_before 	= lv_email.For( li_pos - 1 );
					lv_email_after		= lv_email.At( li_pos + 2 );

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

      lt_MF_section = po_PNR.getNextSegment("MF")  ;
   }
}

void CIUR::processVoidTicket( CPNR &po_PNR)
{
   ENTER;

   AB_Text           lt_segment;
   AB_Char           lc_iurType;

   po_PNR.resetSegment("M0");
   lt_segment = po_PNR.getNextSegment("M0");
   lc_iurType = lt_segment.At(14).For(1);
   if ( lc_iurType == 5 )
   {
      AB_Text     lt_tktForm  = lt_segment.At(30).For(4);
      AB_Text     lt_tktNo    = lt_segment.At(34).For(6);
      AB_Text  	lt_numOfTkt = lt_segment.At(41).For(2);
      AB_Text     lt_tktRef   = lt_segment.At(43).For(8);

		//for debug
		LOG(logDEBUG)<<"VOID IUR="<<lt_segment;
      lt_tktRef.leftJustify();

      if ( lt_numOfTkt == AB_Text("02")
            || lt_numOfTkt == AB_Text("03")
            || lt_numOfTkt == AB_Text("04") )
      {
         lt_tktNo = lt_tktNo+AB_Text("-");
         lt_tktNo = lt_tktNo+lt_numOfTkt;
      }

      md_ID_form[mi_ID_I]     = lt_tktForm;
      md_ID_ticket_no[mi_ID_I]= lt_tktNo;
      md_ID_reference[mi_ID_I]= lt_tktRef;

      mi_ID_I++;
      processIRVoidTicket();

      mb_buildCfa = AB_false;

   }// end of lc_iurType == 5

   EXIT;
}

void CIUR::processAirlineTicketingFee(CPNR &po_PNR) {
   ENTER;

   AB_Text     lt_segment;
	AB_Varchar	lv_tkt_form(20);
	AB_Varchar	lv_tkt_no(20);
	AB_Varchar	lv_fee_type(2);
	AB_Char		lc_fee_amt(20);
	AB_Char		lc_ticket_number(20);

	CAirlineTicketingFee 	lo_AirlineTicketingFee;

   po_PNR.resetSegment("MA");
   lt_segment = po_PNR.getNextSegment("MA");

   while (lt_segment.NotIsNull()) {
	LOG(logDEBUG) << "AM======MA seg: [" << lt_segment << "]"  ;

		lv_tkt_form = lt_segment.At(11).For(4);
		lv_tkt_no   = lt_segment.At(15).For(6);
		lv_fee_type = lt_segment.At(24).For(2);
		lc_fee_amt  = lt_segment.At(66).For(11);

		if (lv_fee_type == "OB") {
			D_amount		ld_value;
			bool 			lb_amount_ok = true;

			ld_value = ld_value.input_conversion_trigger(lc_fee_amt);
			if (!CMsgHandler::getMsgStatus()) {
				lb_amount_ok = false;
				CMsgHandler::clearMsg();
			}

			if (lb_amount_ok) {
				for (int i = 0; i < mi_ID_I; i++) {
					if (md_ID_form[i]      == lv_tkt_form &&
						 md_ID_ticket_no[i] == lv_tkt_no) {
						CAirlineTicketingFee::CTicketingFee
							lo_fee(md_ID_pax_id[i],
									 md_ID_line_number[i],
									 ld_value,
									 lv_tkt_form + lv_tkt_no);

						lo_AirlineTicketingFee.addFee(lo_fee);
						
						//SCR514300, Parsing MA for OB GST //519611 added
						if ( lt_segment.length() > 90 )
						{
							md_IRMK_type[mi_IRMK_I]     = "OBT" ;
							md_IRMK_remark[mi_IRMK_I]   = lt_segment.At(81).For(14) + ";" +lv_tkt_no;	//519611 Added ; lv_tkt_no
							md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
						}
						
						break;
					}
				}
			}

		}

      lt_segment = po_PNR.getNextSegment("MA");
	}

	lo_AirlineTicketingFee.dump();
	lo_AirlineTicketingFee.createOBTaxes(*this);

   EXIT;
}

void CIUR::RP_parseRemarks(CPNR &po_PNR)
{
   ENTER;

   AB_Text  lt_segment ;
   po_PNR.resetSegment("M9") ;
   lt_segment = po_PNR.getNextSegment("M9") ;
   while (lt_segment.NotIsNull())
	{
      // SCR 517902: Virtuoso Remark Parser for VFT/VFC
      AB_Text lt_remark_text = lt_segment.At(5);
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
                        } }
				else {
						if (lt_remark_text.For(4) == "VFT/")
								createRemark("__Q", "FFI/FEE FORMAT INVALID - FEE WAS NOT CREATED");
						if (lt_remark_text.For(4) == "VFC/")
								createRemark("__Q", "FFI/FEE FORMAT INVALID - FEE WAS NOT CREATED");

				}
            }

		pRemarkParser->parse(lt_segment.At(5).to_string());

      lt_segment = po_PNR.getNextSegment("M9") ;
   }

	LOG(logINFO) << pRemarkParser->parsedDataToString().data() ;

   EXIT;
}

void CIUR::parseCouponList(AB_Varchar &list, AB_Text &tkt, D_char_numeric &no_of_books)
{
   ENTER;

	V_AB_Varchar   	lv_arrayField(100,100) ;
   AB_Integer        noCoupons;
	AB_Integer			counter;
	AB_Integer			noBooks;
	string            remark;
	long long			currentTicketNo;

	noBooks         = AB_Integer(no_of_books);
	sscanf(tkt.to_string(), "%lld", &currentTicketNo);

	noCoupons = lv_arrayField.parseStringMultDel(list,",") ;
	LOG(logDEBUG) << "starting input tkt:[" << tkt << "]";
	LOG(logDEBUG) << "AM=== coupons list [" << list << "] has " << noBooks << "coupons";
	LOG(logDEBUG) << "starting currentTicketNo:[" << currentTicketNo << "]";

	for ( counter = AB_Integer(1); counter <= noBooks && counter <= noCoupons; counter= counter + AB_Integer(1))
	{
		LOG(logDEBUG) << "AM=== coupons# " << counter << " = [" << lv_arrayField[counter] << "]";

		// build remark "CPN"-> "ticketNo/couponNo"

      ostringstream convert;
		convert << currentTicketNo;
		remark = convert.str();

		remark += "/"; // + lv_arrayField[counter];

		remark += lv_arrayField[counter].to_string();
		//       type in remark table      data for remark table
		createRemark("CPN",            remark.data() );

		currentTicketNo++;
	}

   EXIT;
}

void CIUR::fill_IrRentalLimo(AB_Text &pt_section )
{
   ENTER;

   AB_Text     	lt_segment ;
   AB_Varchar     lv_remark(255) ;
   AB_Varchar     lv_field(255) ;
   AB_Char        lc_date(5) ;
   AB_Char        lc_year(2) ;
	AB_Char      	lc_itin_type = "C" ;
   int            li_pos ;
   int            li_count ;
   int            li_index ;
	string         labelValue;

   md_IR_itin_type[mi_IR_I]      = lc_itin_type ;
   md_IR_sup_id[mi_IR_I]         = "LBC" ;
   md_IR_line_number[mi_IR_I]    = pt_section.At(3).For(2) ;
   md_IR_service_status[mi_IR_I] = pt_section.At(8).For(2) ;
	md_IR_prod_code[mi_IR_I] 		= "CAR" ;

	//------ STS LABEL(G9150) START------------------
	if ( pRemarkParser->getData("LABELS", "STS", labelValue))
	{
	   mv_service_status             = labelValue.data() ;
		md_IR_service_status[mi_IR_I] = labelValue.data() ;
	}
	//------ STS LABEL(G9150) END------------------

   lc_date = pt_section.At(10).For(5) ;
   if( lc_date != "     " )
   {
      lc_year = mt_pnrDate.get_default_year(pt_section.At(10).For(5));
      lc_year.rightJustify('0') ;
      md_IR_start_date[mi_IR_I] = pt_section.At(10).For(5) +  lc_year ;
   }
   else
      md_IR_start_date[mi_IR_I] = md_IRT_pnr_date ;

   md_IR_end_date[mi_IR_I]  = md_IR_start_date[mi_IR_I] ;

   if ( pt_section.At(19).For(1) == "0" )
		md_IR_unit_ct[mi_IR_I]   = pt_section.At(20).For(1) ;
   else
		md_IR_unit_ct[mi_IR_I]   = pt_section.At(19).For(2) ;

   md_IR_unit_type[mi_IR_I] = "LIMO" ;

   lt_segment = pt_section.At(36) ;
   li_pos     = lt_segment.findSubstring("!") ;
   if ( li_pos > 0 )
      lt_segment = lt_segment.For(li_pos - 1) ;

	lt_segment = lt_segment + "/" ;

   md_IR_origin_city_id[mi_IR_I] = lt_segment.For(3) ;
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
         lv_field              = lv_field.For( li_pos-1 ) ;
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

	//Set Limo indicator in Remark
	md_IRMK_type[mi_IRMK_I] 				= "LMI";
	md_IRMK_itin_type[mi_IRMK_I]        = "C" ;
	md_IRMK_ren_line_number[mi_IRMK_I] 	= md_IR_line_number[mi_IR_I] ;
	md_IRMK_remark[mi_IRMK_I] 				= "Y";
	md_IRMK_pax_id[mi_IRMK_I++]			= AB_null;
/*
   getOverrideRates(lc_itin_type,
                    md_IR_start_date[mi_IR_I],
                    md_IR_saving_code[mi_IR_I],
                    md_IR_local_rate[mi_IR_I],
                    md_IR_regular_rate[mi_IR_I],
                    md_IR_booked_rate[mi_IR_I],
                    md_IR_commissionable[mi_IR_I],
                    gv_IrCarData, mi_IrCarIndex) ;
*/
	//For Non arc ticketing
	AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_seatData ;
   AB_Text    lt_ftpMiles ;
   AB_Char    lc_itinLine(2) ;

   lc_itinLine = pt_section.At(3).For(2) ;

   if (mv_itinTransport.IsNull())
      mv_itinTransport = lc_itinLine ;
   else
      mv_itinTransport = mv_itinTransport + lc_itinLine ;

   // fill Personitin

   lc_noofSeats = "00" ;
   lt_seatData  = " " ;

   lt_ftpMiles = AB_null ;


   fillConsumDetail(lc_noofSeats,
                md_IR_itin_type[mi_IR_I],
                lc_itinLine,
                lt_seatData,
                lt_ftpMiles) ;

   md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2)  ;

	mi_IR_I++;

   EXIT;
}

void CIUR::IrLimoRemark( AB_Varchar	&pv_field )
{
	ENTER;

	md_IRMK_type[mi_IRMK_I]   				= "PNC" ;
	md_IRMK_itin_type[mi_IRMK_I]   		= "C" ;
	md_IRMK_ren_line_number[mi_IRMK_I] 	= md_IR_line_number[mi_IR_I] ;
	md_IRMK_remark[mi_IRMK_I]         	= pv_field ;
	md_IRMK_pax_id[mi_IRMK_I++]       	= AB_null;

	EXIT;
}


//------------------------------------------------------------------------------
void CIUR::fill_IrLandM3(AB_Text &pt_section)
{
   ENTER;

   AB_Varchar lv_field(50) ;
   AB_Char    lc_M5link(1) ;
   AB_Char    lc_itinLine(2) ;
   AB_Char    lc_noofSeats(2) ;
   AB_Text    lt_seatData ;
   AB_Text    lt_ftpMiles ;

   AB_Boolean  lb_dupFound;

   lb_dupFound = AB_false;

   lc_M5link = pt_section.At(6).For(1) ;

   if ( lc_M5link  == " " || mc_iurType == "3" )
   {
      md_IO_sup_id[mi_IO_I]         = AB_null;
   }
   else
   {
      getSupId(lc_M5link,md_IO_sup_id[mi_IO_I]) ;

      //if (md_IO_sup_id[mi_IO_I].IsNull())
      //{
      //   md_IO_itin_type[mi_IO_I] = AB_null ;
      //   return ;
      //}
   }

	md_IO_unit_type[mi_IO_I] = "LAND"; //G9150 v1.1, p.13
   md_IO_unit_ct[mi_IO_I]   = pt_section.At(20).For(1) ;

	LOG(logDEBUG)  << "DK >>> JUST BEFORE parseTourSegment"  ;
   parseLandSegment( pt_section, lb_dupFound );
	LOG(logDEBUG)  << "DK >>> JUST AFTER parseTourSegment"  ;

   if ( lb_dupFound == AB_false )
   {
   	lc_itinLine = pt_section.At(3).For(2) ;

      if (mv_itinTransport.IsNull())
      	mv_itinTransport = lc_itinLine ;
      else
      	mv_itinTransport = mv_itinTransport + lc_itinLine ;

		// fill Personitin

      lc_noofSeats   = "00";
      lt_seatData    = " ";
      lt_ftpMiles    = AB_null;

      fillConsumDetail( lc_noofSeats,
                        md_IO_itin_type[mi_IO_I],
                        lc_itinLine,
                        lt_seatData,
                        lt_ftpMiles) ; /* ftp_miles */

		if ( lc_M5link  != " " )
			md_NonArcItin[++ mi_NonArcItin] = pt_section.At(3).For(2) + lc_M5link  ;

      mi_IO_I++ ;
	}

   EXIT;
}

void CIUR::parseLandSegment(  AB_Text     &pt_section,
                              AB_Boolean  &pb_dupFound)
{
   int            li_pos;
   int            li_startPos;
   int            li_CFPos;
   int            li_index;
   int            li_count;
   int            li_eff_IO_index;
   AB_Text        lt_section;
   AB_Text        lt_data;
   AB_Text        lt_remark;
   V_AB_Text      lv_arrayField(100);
   AB_Char    		lc_date(5) ;
   AB_Char    		lc_year(2) ;

	LOG(logDEBUG)  << "DK >>> CHECK POINT 1"  ;
	md_IO_itin_type[mi_IO_I] 				= "O";
   md_IO_line_number[mi_IO_I]    		= pt_section.At(3).For(2) ;
   md_IO_reference[mi_IO_I]      		= md_IRT_reference;

	LOG(logDEBUG)  << "DK >>> CHECK POINT 2"  ;
	md_IO_dest_city_id[mi_IO_I]= AB_null;

	md_IO_start_time[mi_IO_I] 	= AB_null;
	md_IO_end_date[mi_IO_I] 	= AB_null;
	md_IO_end_time[mi_IO_I] 	= AB_null;


	LOG(logDEBUG)  << "DK >>> CHECK POINT 3"  ;
   lc_date = pt_section.At(10).For(5) ;

   if (lc_date != "     " )
   {
      lc_year = mt_pnrDate.get_default_year(lc_date) ;
      lc_year.rightJustify('0') ;
      md_IO_start_date[mi_IO_I] = lc_date + lc_year ;
      md_IO_end_date  [mi_IO_I] = lc_date + lc_year ;
   }
   else
	{
      md_IO_start_date[mi_IO_I] = md_IRT_pnr_date ;
      md_IO_end_date  [mi_IO_I] = md_IRT_pnr_date ;
	}

	LOG(logDEBUG)  << "DK >>> CHECK POINT 4"  ;
   lt_section = pt_section.At(36);

   li_pos       = lt_section.findSubstring("!") ;

   if (li_pos > 0)
   	lt_section = lt_section.For(li_pos - 1) ;


	LOG(logDEBUG)  << "DK >>> CHECK POINT 5"  ;
   li_count= lv_arrayField.parseStringMultDel(lt_section, "/");

   pb_dupFound       = AB_false;
   li_eff_IO_index   = mi_IO_I;

	LOG(logDEBUG)  << "DK >>> JUST BEFORE LOOP 1"  ;
   if ( md_IO_sup_id[mi_IO_I].IsNull() )
   {
      for ( li_index = mi_IO_I-1; li_index >= 0; li_index--)
      {
         if (  md_IO_itin_type[li_index] != "L" )
         {
            break;
         }

         if (  md_IO_itin_type[li_index] == "L" )
         {
            if ( lv_arrayField[1] 				== mv_lastLandCity &&
                 md_IO_start_date[li_index]  == md_IO_start_date[mi_IO_I]  )
            {
               pb_dupFound       = AB_true;
               li_eff_IO_index   = li_index;
               break;
            }
            else
            {
               break;
            }
         }
      }
   }

	LOG(logDEBUG)  << "DK >>> JUST AFTER LOOP 1"  ;
	if ( pb_dupFound == AB_false )
	{
		mv_lastLandCity = lv_arrayField[1];

		if ( lv_arrayField[1].length() == 3 )
			md_IO_orig_city_id[mi_IO_I] = lv_arrayField[1];
		else if ( lv_arrayField[1].length() == 6 )
		{
		  md_IO_orig_city_id[mi_IO_I] = lv_arrayField[1].For(3);
		  md_IO_dest_city_id[mi_IO_I] = lv_arrayField[1].At(4);
		}
		else
			md_IO_orig_city_id[mi_IO_I] = "XXX";
	}

   if ( md_IO_confirmation[li_eff_IO_index].IsNull() && pb_dupFound == AB_false )
   {
		lt_data = pt_section.At(21).For(15);
		lt_data.removeLeadAndTrailBlanks();

		if ( lt_data.NotIsNull() && lt_data.length() > 0 )
		{
     		md_IO_confirmation[li_eff_IO_index] = lt_data;
		}
   }

	processOtherAssociatedRemarks(li_eff_IO_index);
}


