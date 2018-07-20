/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999, 
															 2000, 2001
                SID: 1.152
      Delta Created: 14/11/04 - 14:29:53
         Last Delta: app/src/common/InterfaceRecord/_us 11/4/14 - 14:29:53 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TAIR.cpp) 
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us}[11/4/14-14:29:53 1.152] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TAIR.cpp (Jinsong Liu)\n" )
/*
  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
============================================================ END GLOBAL BANNER*/

#include "C.h"
#include "gtypes.h"
#include "TAIR_D.h"
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
#include "TAIR.h"

#define ENTER      LOG(logINFO) << "\n*** Enter>> " << __FUNCTION__
#define EXIT       LOG(logINFO) << "\n*** Exit << " << __FUNCTION__

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::printAll()
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

   LOG(logDEBUG) << "START IRPAYMENT PRINT"  ;
   printIrpayment() ;
   LOG(logDEBUG) << "END IRPAYMENT PRINT"  ;
}

CTAIR::CTAIR () :
					mv_IrHotelData(255,100),
					mv_IrCarData(255,100),
					md_tktData(255,100),
					mv_itinTransport(255),
					mva_airCommission(255,100),
					mva_tvlTKTSegments(500,100),
					mva_tvlOtherSegments(500,100),
					mva_airFareSectionWG(255,100),
					mva_airFareSectionWB(255,100),
					mva_exchSectionWC(255,100),
					mva_exchSectionWX(255,100),
					mv_baggage(10),
					mv_service_status(10),
					mv_fareBasis(15),
					mv_tktDesig(10),
					mv_segmentMile(5),
					mva_WS_section(255,100),
					mv_IrRentalData(255,100),
					mv_IrTransData(255,100),
      			mva_passengerWA (255,100),
					mva_airFareSectionWV(100),
					mva_paxFareByLeg(255,100),
					md_currentNameNumber(10)
{
	mi_IrHotelIndex  = 1 ;
	mi_IrCarIndex    = 1 ;
	mi_IrRentalIndex = 0 ;
	mi_IrTransIndex  = 0 ;
	mi_passengerWA   = 0;

	mi_tvlTKTIndex		= 0;
	mi_tvlOtherIndex	= 0;
	mi_tvlAIRIndex		= 0;

	// WS 17 START
		mi_XDLine = 50 ; // Used to create Other Itinerary by service charge MCO ;
		mi_MCO_itin_line_number = -1;
	// WS 17 END
}

//-----------------------------------------------------------------------------
void CTAIR::errorHandler(CPNRWS  &po_PNR)
{
   D_description ld_errorDescription ;
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

	LOG(logDEBUG) << "****** po_PNR.createGirRawTables ******"  ;
   po_PNR.createGirRawTables() ;

	LOG(logDEBUG) << "****** BEFORE createQueue ******"  ;
   createQueue(ld_errorDescription) ;
	LOG(logDEBUG) << "****** AFTER createQueue ******"  ;
}

//-----------------------------------------------------------------------------
void CTAIR::createCTAIR(CPNRWS  &po_PNR)
{
   CDMatrix lo_Matrix;
   AB_Integer li_sqlcode;

    LOG(logDEBUG) << "START TAIR PROCESSING"  ;

   if (CMsgHandler::getMsgStatus())
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
		processTVLSegment() ;

   if (CMsgHandler::getMsgStatus())
		processReceipt() ;

	//G145 - surface Segment
   if (CMsgHandler::getMsgStatus())
		createARNK() ;

	// US044 START
	if (CMsgHandler::getMsgStatus())
		IrTransportRemarks(po_PNR);
	// US044 END

   if (CMsgHandler::getMsgStatus())
      fillAirLocatorAndSeat(po_PNR) ;

printAll();

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

   // LOG(logDEBUG) << "*****WRITE GIR TABLES*****"  ;
   
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

//------------------------------------------------------------------------------

void CTAIR::fill_IrHeader(CPNRWS  &po_PNR)
{
   ENTER;

   AB_Text     lt_total_segment ;
   int         li_pos ; 
	V_AB_Varchar lv_arrayField(100,100) ;
	// WS 17 START *******
//	AB_Char    lc_pnrYear(2) ;
	AB_Char    lc_pnrDate(7) ;
	// WS 17 END *******

   md_session_id     = po_PNR.getSessionId() ;
   md_interface_type = po_PNR.getInterfaceType() ;
   md_creation_date  = po_PNR.getCreationDate() ;
   md_IRT_pnr_time    = AB_null ;

	// Commission index
	mi_commIndex       = 1 ;

	// Air Fare Info
   mi_airFareSectionWG   = 1 ;
   mi_airFareSectionWB   = 1 ;
   mi_airFareSectionWV   = 1 ;

	// Exchange Ticket Info
   mi_exchSectionWC   = 1 ;
   mi_exchSectionWX   = 1 ;
	mi_WS_count        = 1 ;

   po_PNR.resetSegment("1") ;
   lt_total_segment = po_PNR.getNextSegment("1")  ;

   if (lt_total_segment.NotIsNull())
	{
		mc_delimiter = lt_total_segment.At(2).For(1) ;

		// Get invoice no 

		li_pos  = lt_total_segment.findSubstring("IN-") ;

		if ( li_pos > 0)
			md_invoiceNo = lt_total_segment.At(li_pos + 3).For(6) ;
		else
			md_invoiceNo = AB_null ;

		li_pos  = lt_total_segment.findSubstring("FA-") ;

		if ( li_pos > 0)
		{
			md_IRT_reference = lt_total_segment.At(li_pos + 3).For(6) ;
			md_IRT_booking_no = md_IRT_reference;
		}
	}
   else 
      CMsgHandler::setMsg(AB_Integer(2509), AB_Char(15," "), D_error_track("TAIR-PROCHDR"), AB_Integer(1), AB_Varchar(20,AB_null));

   po_PNR.resetSegment("3") ;
   lt_total_segment = po_PNR.getNextSegment("3")  ;

   if (lt_total_segment.NotIsNull())
	{
		li_pos = lv_arrayField.parseStringMultDel(lt_total_segment,mc_delimiter.to_string()) ;

		if ( li_pos > 0 )
		{

			AB_Date    lt_dateToCompare = md_creation_date.get_start_of_day();

			li_pos  = lt_total_segment.findSubstring("NL-") ;

			if ( li_pos > 0)
			{
				md_IRT_iata_no = lt_total_segment.At(li_pos + 6).For(8) ;
			}
			else
			{
				li_pos  = lt_total_segment.findSubstring("TL-") ;
				md_IRT_iata_no  = lt_total_segment.At(li_pos + 6).For(8) ;
			}

			// WS 17 START

			li_pos  = lt_total_segment.findSubstring("TL-") ;
			lc_pnrDate = lt_total_segment.At(li_pos + 15).For(7);
			mt_lclPnrDate = lc_pnrDate;
			md_lclPnrTime = lt_total_segment.At(li_pos + 22).For(4);

			// WS 17 END

			// WS 17 START *******
			lc_pnrDate = lv_arrayField[2].At(16).For(7) ;

			adjustPNRDate( md_creation_date, 
								lc_pnrDate, 
								md_IRT_pnr_date, 
								mt_pnrDate ); 

			if (! CMsgHandler::getMsgStatus())
				return ;

			md_IRT_pnr_time = lv_arrayField[2].At(23).For(4) ;
			// WS 17 END *******
		}
		
	}

	md_IRT_crs_id = "WSP" ;
	md_IRT_start_date = "       " ;

   AB_Text  lt_W7_section ;

   po_PNR.resetSegment("7") ;
   lt_W7_section = po_PNR.getNextSegment("7");

	lt_W7_section = lt_W7_section.At(3);

	getFormOfPay(lt_W7_section,
					 md_formOfPay ,
					 md_cc_id, 
					 md_cc_no,
					 md_cc_exp_date,
					 md_auth_no,
					 md_auth_source ) ;

	// Get all the Remark field

	getRemarkFields(po_PNR) ;

   if (! CMsgHandler::getMsgStatus())
     	return ;

	// Fill Air commission from Section - 8

	fillCommission(po_PNR) ;

   if (! CMsgHandler::getMsgStatus())
     	return ;

	// Fill Air Fare - G

	fillAirFare(po_PNR) ;

	// Fill Horizontal Fare calculation - B

	fillFareCalculation(po_PNR) ;

	// Fill Trip Value for fare by leg - V

	fillTripValue(po_PNR) ;

	fillTVLSegments(po_PNR) ;

   if (! CMsgHandler::getMsgStatus())
     	return ;

	// Fill Air commission from Sections - C and X

	fillExchange(po_PNR) ;

   if (! CMsgHandler::getMsgStatus())
     	return ;

   EXIT;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillCommission(CPNRWS  &po_PNR)
{
	AB_Text		lt_W8_section ;

	
   po_PNR.resetSegment("8") ;
   lt_W8_section = po_PNR.getNextSegment("8")  ;

   while (lt_W8_section.NotIsNull()) 
	{
		
		// WS 17 START *******
      mva_airCommission[mi_commIndex++] = lt_W8_section.At(3) + mc_delimiter ;
		// WS 17 END *******
		
		lt_W8_section = po_PNR.getNextSegment("8")  ;
	}
	
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillTVLSegments(CPNRWS  &po_PNR)
{
	AB_Text      lt_WT_section ;
	V_AB_Varchar lv_arrayField(100,100) ;
	int          li_pos ;
	AB_Varchar  lv_fieldAC1(100);

   po_PNR.resetSegment("T") ;
   lt_WT_section = po_PNR.getNextSegment("T")  ;

   while (lt_WT_section.NotIsNull() && CMsgHandler::getMsgStatus()) 
	{
		lt_WT_section = lt_WT_section + mc_delimiter ;

      lv_fieldAC1 = lt_WT_section.getNthBracketedField(1,"AC1-", mc_delimiter.to_string());

		li_pos = lv_arrayField.parseStringMultDel(lt_WT_section, mc_delimiter.to_string()) ;

		if ( 	lv_arrayField[3].At(8).For(3) == "TKT" &&
				( lv_fieldAC1.IsNull() || lv_fieldAC1 != "FEE" ) )
		{
			mva_tvlTKTSegments[++mi_tvlTKTIndex] = lt_WT_section;
		} 
		else
		{
			mva_tvlOtherSegments[++mi_tvlOtherIndex] = lt_WT_section;
		}

   	lt_WT_section = po_PNR.getNextSegment("T")  ;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillAirFare(CPNRWS  &po_PNR)
{
	AB_Text		lt_WG_section ;

   po_PNR.resetSegment("G") ;
   lt_WG_section = po_PNR.getNextSegment("G")  ;

   while (lt_WG_section.NotIsNull()) 
	{
      mva_airFareSectionWG[mi_airFareSectionWG++] = lt_WG_section ;

		lt_WG_section = po_PNR.getNextSegment("G")  ;
		
	}

}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillFareCalculation(CPNRWS  &po_PNR)
{
	AB_Text		lt_WB_section ;

   po_PNR.resetSegment("B") ;
   lt_WB_section = po_PNR.getNextSegment("B")  ;

   while (lt_WB_section.NotIsNull()) 
	{
      mva_airFareSectionWB[mi_airFareSectionWB++] = lt_WB_section ;

		lt_WB_section = po_PNR.getNextSegment("B")  ;
		
	}

}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillTripValue(CPNRWS  &po_PNR)
{
	AB_Text		lt_WV_section ;

   po_PNR.resetSegment("V") ;
   lt_WV_section = po_PNR.getNextSegment("V")  ;

   while (lt_WV_section.NotIsNull()) 
	{
      mva_airFareSectionWV[mi_airFareSectionWV++] = lt_WV_section ;

		lt_WV_section = po_PNR.getNextSegment("V")  ;
		
	}

}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillExchange(CPNRWS  &po_PNR)
{
	AB_Text		lt_WC_section ;
	AB_Text		lt_WX_section ;

	
   po_PNR.resetSegment("C") ;
   lt_WC_section = po_PNR.getNextSegment("C")  ;

   while (lt_WC_section.NotIsNull()) 
	{
		
      mva_exchSectionWC[mi_exchSectionWC++] = lt_WC_section.At(2) ;

		lt_WC_section = po_PNR.getNextSegment("C")  ;
		
	}

	if ( mi_exchSectionWC > 1 )
	{
   	po_PNR.resetSegment("X") ;
   	lt_WX_section = po_PNR.getNextSegment("X")  ;

   	while (lt_WX_section.NotIsNull()) 
		{
			
      	mva_exchSectionWX[mi_exchSectionWX++] = lt_WX_section.At(2) ;
	
			lt_WX_section = po_PNR.getNextSegment("X")  ;
		
		}
	}
	
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillAirSeat(CPNRWS  &po_PNR)
{
	/*
	AB_Text		lt_WS_section ;
	
   po_PNR.resetSegment("S") ;
   lt_WS_section = po_PNR.getNextSegment("S")  ;

   while (lt_WS_section.NotIsNull()) 
	{
		
      mva_WS_section[mi_WS_count++] = lt_WS_section ;
		
		lt_WS_section = po_PNR.getNextSegment("S")  ;
	}
	*/
	
	// SCR 281836 START

	vector<AB_Text>	lvo_seat_data;
	AB_Text				lt_WM_section ;
	V_AB_Varchar 		lv_arrayField(100,100) ;
	int					li_index;

   po_PNR.resetSegment("M") ;
   lt_WM_section = po_PNR.getNextSegment("M")  ;

	while (lt_WM_section.NotIsNull()) {
		li_index = lv_arrayField.parseStringMultDel(lt_WM_section, "#");
		for (int i = 1; i <= li_index; i++) {
			int li_pos = lv_arrayField[i].findSubstring("SSRSEAT");
			if (li_pos > 0) {
				lvo_seat_data.push_back(lv_arrayField[i].At(li_pos));	
			}
		}

		lt_WM_section = po_PNR.getNextSegment("M");
	}

	for (unsigned i = 0; i < lvo_seat_data.size(); i++) {
		AB_Text lt_str = lvo_seat_data[i];
		D_supplier ld_sup_id = lt_str.At(8).For(2);
		D_name ld_orig_city_id = lt_str.At(13).For(3);
		D_name ld_dest_city_id = lt_str.At(16).For(3);

		D_service  ld_service = lt_str.At(19).For(4);
		ld_service.rightJustify('0') ;
		D_unit_type	ld_classofservice = lt_str.At(23).For(1);

		AB_Char lc_year(2) ;
		lc_year = mt_lclPnrDate.get_default_year(lt_str.At(24).For(5)) ;
		lc_year.rightJustify('0') ;
		D_char_date ld_start_date = lt_str.At(24).For(5) + lc_year;

		D_seat  ld_seat = lt_str.At(33).For(2) + lt_str.At(36).For(1);

		D_person_name ld_first_name;
		D_person_name ld_last_name = lt_str.At(38);
		ld_last_name.substitute(".", " ");

		li_index = ld_last_name.findSubstring("/");
		if (li_index == 0) {
			li_index = ld_last_name.findSubstring(" ");
		}

		if (li_index > 0) {
			ld_first_name = ld_last_name.At(li_index+1);
			ld_last_name  = ld_last_name.For(li_index-1);
			ld_first_name.removeLeadAndTrailBlanks();
			ld_last_name.removeLeadAndTrailBlanks();
		}


		bool lb_found = false;
		for (int j = 0; j < mi_IT_I; j++) {
			if (md_IT_sup_id[j] 				== ld_sup_id 			&&
				 md_IT_service[j]				== ld_service			&&
				 // md_IT_classofservice[j]	== ld_classofservice	&&
				 md_IT_start_date[j] 		== ld_start_date		&&
				 md_IT_orig_city_id[j]		== ld_orig_city_id	&&
				 md_IT_dest_city_id[j]		== ld_dest_city_id) {

				for (int k = 0; k < mi_ICD_I; k++) {
					if (md_ICD_itin_type[k] == md_IT_itin_type[j]	&&
						 md_ICD_itin_line_number[k] == md_IT_line_number[j]) {

						for (int l = 0; l < mi_IP_I; l++) {
							if (md_IP_pax_id[l] == md_ICD_pax_id[k]) {

								D_person_name ld_pax_fname = md_IP_first_name[l];
								D_person_name ld_pax_lname = md_IP_last_name[l];
								int li_fname_len = min(ld_first_name.length(),
															  ld_pax_fname.length());
								int li_lname_len = min(ld_last_name.length(),
															  ld_pax_lname.length());

								if (ld_first_name.For(li_fname_len) == ld_pax_fname.For(li_fname_len) &&
									 ld_last_name.For(li_lname_len) == ld_pax_lname.For(li_lname_len)) {
									md_ICD_seat[k] = ld_seat;
									lb_found = true;
									break;
								}
							}

						} // for (int l = 0; l < mi_IP_I; l++)

						if (lb_found) { break; }
					}
				} // for (int k = 0; k < mi_ICD_I; k++)

				if (lb_found) { break; }
			}
		} // for (int j = 0; j < mi_IT_I; j++)
	}

	// SCR 281836 END
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::populate_IrAddress(CPNRWS  &po_PNR)
{
	AB_Text		  lt_total_segment ;
	int		  	  i,li_pos ;
	AB_Varchar 	  lv_long_address(100);
	CAddress   	  lo_Address ;

	V_AB_Varchar lv_arrayField(100,100) ;

	mb_address = AB_false ;

   po_PNR.resetSegment("Q") ;
   lt_total_segment = po_PNR.getNextSegment("Q")  ;

	if ( lt_total_segment.IsNull())
	{
   	po_PNR.resetSegment("5") ;
   	lt_total_segment = po_PNR.getNextSegment("5")  ;
	}

   if (lt_total_segment.NotIsNull())
	{
		li_pos = lv_arrayField.parseStringMultDel(lt_total_segment.At(2),mc_delimiter.to_string()) ;
	}

	if (li_pos > 0 )
	{
		i = 1 ;

		while ( i <= 5 )
		{
			if ( lv_arrayField[i].NotIsNull() )
			{
				if ( lv_long_address.IsNull() )
					lv_long_address = lv_arrayField[i] ;
				else
					lv_long_address = lv_long_address + "/" + lv_arrayField[i] ;
			}
		i++ ;
		}

	}

   if (lv_long_address.NotIsNull())
	{
		li_pos = lv_arrayField.parseString(lv_long_address,"/") ;

      md_IA_addr1[mi_IA_I] = lv_arrayField[1] ;
		md_IA_addr2[mi_IA_I] = lv_arrayField[2] ;
		md_IA_addr3[mi_IA_I] = lv_arrayField[3] ;
		md_IA_addr4[mi_IA_I] = lv_arrayField[4] ;

		mb_address = AB_true ;

	}

	if ( mv_phoneField.NotIsNull() )
	{
		parsePhone(mv_phoneField) ;

		if (md_IA_phone_no[mi_IA_I].NotIsNull() || md_IA_bphone_no[mi_IA_I].NotIsNull() ||
				md_IA_fax_no[mi_IA_I].NotIsNull() || md_IA_mphone_no[mi_IA_I].NotIsNull() )
			mb_address = AB_true ;
	}

	overrideHomePhone();

	if (mb_address)
		md_IA_type[mi_IA_I++] = "P" ;

}

//----------------------------------------------------------------------------------------------------
void CTAIR::parsePhone(AB_Varchar &pv_phoneField) 
{
	AB_Varchar     lv_long_Rphone(100) ;
	AB_Varchar     lv_long_Bphone(100) ;
	AB_Varchar     lv_long_Mphone(100) ;
	AB_Varchar     lv_long_Fphone(100) ;

	lv_long_Rphone = AB_null ;
	lv_long_Bphone = AB_null ;
	lv_long_Mphone = AB_null ;
	lv_long_Fphone = AB_null ;

	pv_phoneField = pv_phoneField + "/" ;

	lv_long_Rphone = pv_phoneField.getNthBracketedField(1,"R/","/") ;

	if ( lv_long_Rphone.NotIsNull() )
	{

		editPhoneNumber(lv_long_Rphone);

		if ( lv_long_Rphone.length() > 7 )
		{
			md_IA_phone_acode[mi_IA_I] = lv_long_Rphone.For(3) ;
			md_IA_phone_no[mi_IA_I] = lv_long_Rphone.At(4) ;
		}
		else
		{
			md_IA_phone_no[mi_IA_I] = lv_long_Rphone ;
		}
	}

	lv_long_Bphone = pv_phoneField.getNthBracketedField(1,"B/","/") ;

	if ( lv_long_Bphone.NotIsNull() )
	{

		editPhoneNumber(lv_long_Bphone);

		if ( lv_long_Bphone.length() > 7 )
		{
			md_IA_bphone_acode[mi_IA_I] = lv_long_Bphone.For(3) ;
			md_IA_bphone_no[mi_IA_I] = lv_long_Bphone.At(4) ;
		}
		else
		{
			md_IA_bphone_no[mi_IA_I] = lv_long_Bphone ;
		}
	}

	lv_long_Mphone = pv_phoneField.getNthBracketedField(1, "M/", "/") ;

	if ( lv_long_Mphone.NotIsNull() )
	{
		if ( lv_long_Mphone.length() > 1)
		{
			lv_long_Mphone.removeTrailingBlanks() ;

			lv_long_Mphone.substitute("-","");
			lv_long_Mphone.substitute(" ","");

			if ( lv_long_Mphone.length() > 3 )
			{
				if ( !lv_long_Mphone.For(3).isAllNumeric())
				{
					lv_long_Mphone = lv_long_Mphone.At(4) ;
					lv_long_Mphone.removeTrailingBlanks() ;
				}
			}
			else
				lv_long_Mphone = AB_null ;

			editPhoneNumber(lv_long_Mphone);

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
	lv_long_Fphone = pv_phoneField.getNthBracketedField(1,"F/","/") ;

	if ( lv_long_Fphone.NotIsNull() )
	{

		editPhoneNumber(lv_long_Fphone);

		if ( lv_long_Fphone.length() > 7 )
		{
			md_IA_fax_acode[mi_IA_I] = lv_long_Fphone.For(3) ;
			md_IA_fax_no[mi_IA_I] = lv_long_Fphone.At(4) ;
		}
		else
		{
			md_IA_fax_no[mi_IA_I] = lv_long_Fphone ;
		}
	}
}


//---------------------------------------------------------------------------------------------------------------------------

void CTAIR::fill_IrPasDocument(CPNRWS  &po_PNR)
{
   ENTER;

   int                li_counter, li_pax_id, li_pos, li_taxCounter ;
   AB_Text            lt_WA_section ;
   AB_Text            lt_W9_section ; 		// Issuing Supplier
   int                li_WA_index ;
	V_AB_Varchar       lv_arrayField(100,100) ;
	AB_Boolean			 lb_exchange ;
	AB_Varchar         lv_field(50) ;
	AB_Integer         li_firstTkt, li_lastTkt, li_books ;
	D_amount           ld_totTax ;
	AB_Char            lc_docLine(2) ;
	AB_Boolean         lb_ticketFound ;
	AB_Text lv_paxName ;             //1606-27JAN99-v1103-SC 

	// WS 17 START
	AB_Text        	 lt_WO_section ;
	AB_Boolean			 lb_standAloneMCO;
	// WS 17 END
		

   li_pax_id = 0 ;
   li_taxCounter = 0 ;

	mi_tktCounter = 1 ;
   
   li_counter = 0 ;
   po_PNR.resetSegment("A") ;
   lt_WA_section = po_PNR.getNextSegment("A");

   while (lt_WA_section.NotIsNull()) 
   {
		lb_ticketFound = AB_false ;

		// WS 17 START
		lb_standAloneMCO	= AB_false;
		// WS 17 END

		lt_WA_section.removeTrailingBlanks() ;

		li_pos = lv_arrayField.parseStringMultDel(lt_WA_section,mc_delimiter.to_string()) ;

		lv_paxName = lv_arrayField[3] ;
      if (lv_paxName.For(2) !="C/" )        //1606-27JAN99-v1103-SC 
		{

		lb_exchange = AB_false ;

      li_pax_id++ ;
   
      md_IP_pax_id[mi_IP_I]      = li_pax_id ;

      md_ID_line_number[mi_ID_I] = mi_tktCounter++ ;
		lc_docLine                 = md_ID_line_number[mi_ID_I] ;
		lc_docLine.rightJustify('0') ;

      md_ID_pax_id[mi_ID_I]      = li_pax_id ;
      md_IP_crs_invoice[mi_IP_I] = md_invoiceNo ;


      md_IP_nameNumber[mi_IP_I]  = lv_arrayField[2] ;
		md_currentNameNumber			= lv_arrayField[2] ;

		if ( lv_arrayField[6].For(1) != "*" )
		{
			// Customer comments
			md_IP_reference[mi_IP_I]   = lv_arrayField[6] ;
		}

		if ( lv_arrayField[7].For(1) == "M" || lv_arrayField[7].For(1) == "P" )
		{
			lv_arrayField[7] = lv_arrayField[7].At(2)  ;
		}

		if ( lv_arrayField[7].isAllNumeric() )
		{
      	md_ID_form[mi_ID_I]          = lv_arrayField[7].For(4) ;
      	md_ID_ticket_no[mi_ID_I]     = lv_arrayField[7].At(5).For(6) ;
			lb_ticketFound = AB_true ;

			if ( lv_arrayField[7].length() > 10)
			{
				md_ID_ticket_no[mi_ID_I] = md_ID_ticket_no[mi_ID_I] + "-" + lv_arrayField[7].At(11).For(3) ;

				li_firstTkt = lv_arrayField[7].At(8).For(3) ;
				li_lastTkt  = lv_arrayField[7].At(11).For(3) ;

				li_books = li_lastTkt - li_firstTkt ;

				md_ID_no_of_books[mi_ID_I]   = li_books ;
				
			}
			else
			{
				md_ID_no_of_books[mi_ID_I]   = "1" ;
			}

			md_ID_doc_type[mi_ID_I]      = "BSP" ;
		}
		// WS 17 START
		else if ( lv_arrayField[7] == "O" ) // stand alone MCO
		{
			lb_standAloneMCO = AB_true;
			md_ID_form[mi_ID_I]        = AB_null ;
			md_ID_ticket_no[mi_ID_I]   = AB_null ;
		}
		// WS 17 END
		else
		{
			/**
			 * Non BSP Ticket
          **/   

			md_ID_form[mi_ID_I]        = AB_null ;
			md_ID_ticket_no[mi_ID_I]   = AB_null ;
			md_ID_no_of_books[mi_ID_I] = AB_null ;
			md_ID_e_ticket[mi_ID_I]    = "N" ;
		}


		// WS 17 START

		// Get optional invoice no

		if ( lb_ticketFound == AB_true || lb_standAloneMCO == AB_true )
		{
			if ( lv_arrayField[8].NotIsNull())
			{
				// SSI OR SI option is used
				if ( lv_arrayField[8].isAllNumeric() )
				{
					md_IP_crs_invoice[mi_IP_I] = lv_arrayField[8] ;
				}
				else if ( lv_arrayField[9].NotIsNull())
				{
					// SSI OR SI option is used
					if ( lv_arrayField[9].isAllNumeric() )
					{
						md_IP_crs_invoice[mi_IP_I] = lv_arrayField[9] ;
					}
				}
			}

			if ( lv_arrayField[8].NotIsNull() && lv_arrayField[8] == "E-" ||
			     lv_arrayField[9].NotIsNull() && lv_arrayField[9] == "E-" )
			{
				 /**
				  * It is an electronic ticket
				  **/

			    md_ID_e_ticket[mi_ID_I]  = "Y" ;
			}
			else
			{
			    md_ID_e_ticket[mi_ID_I]  = "N" ;
			}
		}
		// WS 17 END

		md_ID_reference[mi_ID_I]   = md_IRT_reference ;

		// Get Issuing supplier Information

		if ( md_supId.IsNull() )
		{
			po_PNR.resetSegment("9") ;
   		lt_W9_section = po_PNR.getNextSegment("9");

			if ( lt_W9_section.NotIsNull() )
			{
				lt_W9_section = lt_W9_section + mc_delimiter ;

				lv_field = lt_W9_section.getNthBracketedField
												(1, "V-", mc_delimiter.to_string()) ;
				if (lv_field.NotIsNull())
				{
					if ( lv_field.At(3).For(1) == "/" )
					{
						md_ID_sup_id[mi_ID_I] = lv_field.For(2) ;
					}
					else
					{
						md_ID_sup_id[mi_ID_I] = lv_field.For(3) ;
					}
					
					md_supId = md_ID_sup_id[mi_ID_I] ;
				}
			}
		}
		else
		{
			md_ID_sup_id[mi_ID_I] = md_supId ;
		}

		// Assign fare info
		
		md_ID_saving_code[mi_ID_I]     = md_savingCode ;
		md_ID_full_fare[mi_ID_I]       = md_fullFare ;
		md_ID_low_prac_fare[mi_ID_I]   = md_lowPracFare ;
		md_ID_rebate[mi_ID_I]          = md_rebate ;

		// Get CRS Tour Information

		if ( md_crsTour.IsNull() )
		{
			po_PNR.resetSegment("F") ;
   		AB_Text lt_WF_section = po_PNR.getNextSegment("F");

			if ( lt_WF_section.NotIsNull() )
				md_ID_crs_tour[mi_ID_I] = lt_WF_section.At(2).For(15) ;
			
			md_crsTour = md_ID_crs_tour[mi_ID_I] ;
		} else {
			md_ID_crs_tour[mi_ID_I] = md_crsTour;
		}

      md_ID_pic[mi_ID_I] = lv_arrayField[4] ;

      mva_passengerWA[++mi_passengerWA] = lv_arrayField[4]  ;

      li_counter = lv_paxName.findSubstring("*") ;

      if ( li_counter > 0 )
	  	{
	  		lv_paxName = lv_paxName.For( li_counter - 1) ;
	  	}

      processPassengerName(lv_paxName) ;
      
      if (! CMsgHandler::getMsgStatus())
         return ;

		// WS 17 START
		if ( lb_standAloneMCO == AB_false )
		{
			if ( lv_arrayField[7].For(1) != "*"  && lb_ticketFound)
			{
      		processFare(po_PNR,md_IP_pax_id[mi_IP_I],md_ID_pic[mi_ID_I],ld_totTax) ; 
			}

			lb_exchange = processExchange(md_IP_pax_id[mi_IP_I],md_ID_pic[mi_ID_I],ld_totTax,lc_docLine) ;


			if ( lb_exchange )
			{
				md_ID_exchange_code[mi_ID_I] = "E" ;
			}
		}

		// There's something wrong with the ticket and it is not a "dummy" ticket for Automated MCO
		if ( 	lb_standAloneMCO == AB_false &&
				md_ID_ticket_no[mi_ID_I].IsNull() && 
				md_ID_base_cost[mi_ID_I].IsNull() )
		{
			mi_tktCounter-- ;
		}
		// Normal ticket or a "dummy" ticket for Automated MCO
		else 
		{
			// If it is not a "dummy" ticket for Automated MCO, move to the next ticket
			if ( lb_standAloneMCO == AB_false )
			{
				mi_ID_I++ ;
			}

			// Try to locate an MCO information

   		po_PNR.resetSegment("O") ;

   		lt_WO_section = po_PNR.getNextSegment("O");
   		
			while (lt_WO_section.NotIsNull())
			{
				// If previous ticket is a normal ticket, MCO should create new ticket record
				// Otherwise MCO should add information to the last "dummy" ticket

				if ( lb_standAloneMCO == AB_false )
				{
					md_ID_pax_id[mi_ID_I]      	= md_ID_pax_id[mi_ID_I - 1] ;
					md_ID_pic[mi_ID_I] 				= md_ID_pic[mi_ID_I - 1] ;
					md_ID_e_ticket[mi_ID_I]		  	= md_ID_e_ticket[mi_ID_I - 1];
					md_ID_crs_tour[mi_ID_I] 		= md_ID_crs_tour[mi_ID_I - 1] ;

					md_ID_reference[mi_ID_I]   	= md_IRT_reference ;
					md_ID_saving_code[mi_ID_I]    = md_savingCode ;
					md_ID_full_fare[mi_ID_I]      = md_fullFare ;
					md_ID_low_prac_fare[mi_ID_I]  = md_lowPracFare ;
					md_ID_rebate[mi_ID_I]         = md_rebate ;
					md_ID_doc_type[mi_ID_I]      	= "BSP" ;
					md_ID_no_of_books[mi_ID_I]   	= "1" ;

					md_ID_line_number[mi_ID_I] 	= mi_tktCounter++ ;
				}

				// Try to populate current ticket with information from MCO record

				if ( fill_IrPasMCODocument(lt_WO_section) == AB_true )
				{
					// Ticket was populated. Next step is to create "dummy" Other Itinerary and populate
					// Consumer Details for it.
					createMCOConsumDetail();

					lb_standAloneMCO = AB_false; // Next MCO should create new ticket

					//Move to the next ticket.
					mi_ID_I++ ;
				}
				// MCO information was not found yet. So if it was not a "dummy" ticket,
				// decrement the ticket count incremented before
				else if ( lb_standAloneMCO == AB_false )
				{
					mi_tktCounter-- ;
				}

   			lt_WO_section = po_PNR.getNextSegment("O");
			}

			// If we have a "dummy" ticket but no MCO details for it,
			// decrement the ticket count incremented before

			if ( 	lb_standAloneMCO == AB_true 			&&
					md_ID_ticket_no[mi_ID_I].IsNull() 	&& 
					md_ID_base_cost[mi_ID_I].IsNull() )
			{
				mi_tktCounter-- ;
			}
		}

		// WS 17 END

      mi_IP_I++ ;

		}  

      lt_WA_section = po_PNR.getNextSegment("A");
   }

   EXIT;
}


//---------------------------------------------------------------------------------------------------------------------------

// WS 17 START
AB_Boolean CTAIR::fill_IrPasMCODocument(AB_Text  &pt_WO_section)
{
   ENTER;
	AB_Text			lt_WO_section;
	int				li_pos;
	AB_Varchar 		lv_nameNumber[10];			 
	V_AB_Varchar	lv_arrayField(100,100) ;
	AB_Varchar   	lv_data(100) ;
	AB_Boolean		lb_ticketFound;

   D_amount ld_commission ;
   D_amount ld_commRate ;
	D_amount	ld_baseCost;
	D_amount	ld_totalFare;

   D_form_of_payment      ld_formOfPay ;
   D_credit_card_sup      ld_cc_id;
   D_credit_card_no       ld_cc_no;
   D_expiry_date          ld_cc_exp_date;
   D_authorization        ld_auth_no;
   D_authorization_source ld_auth_source;

	ld_commission = D_amount(0);
	ld_commRate = D_amount(0);
	ld_baseCost = D_amount(0);
	ld_totalFare = D_amount(0);

	AB_Varchar lv_mco_type(2);
	AB_Varchar lv_unit_type(3);
	AB_Varchar lv_remark(255);

   lb_ticketFound = AB_false ;
	lt_WO_section = pt_WO_section + mc_delimiter;

   if (lt_WO_section.NotIsNull())
   {
      lt_WO_section.removeTrailingBlanks() ;
		li_pos = lv_arrayField.parseStringMultDel(lt_WO_section,mc_delimiter.to_string()) ;

		LOG(logDEBUG) << "*** DK - O SECTION : *** " << lt_WO_section  ;
		LOG(logDEBUG) << "*** DK - lv_arrayField[2] = " << lv_arrayField[2]  ;
		LOG(logDEBUG) << "*** DK - lv_arrayField[3] = " << lv_arrayField[3]  ;
		LOG(logDEBUG) << "*** DK - md_IP_nameNumber[mi_IP_I] = " << md_IP_nameNumber[mi_IP_I]  ;
		LOG(logDEBUG) << "*** DK - md_currentNameNumber = " << md_currentNameNumber  ;
		
		if ( lv_arrayField[2] == "SF" || lv_arrayField[2] == "RV" || lv_arrayField[2] == "OI")
		{
			lv_mco_type = lv_arrayField[2];
			if ( lv_arrayField[3] == md_currentNameNumber )
			{
				lv_data  = lt_WO_section.getNthBracketedField(1, "MCO-",  mc_delimiter.to_string()) ;

				if ( lv_data.NotIsNull() && lv_data.isAllNumeric())
				{

					md_ID_form[mi_ID_I] = lv_data.For(4);
   				md_ID_ticket_no[mi_ID_I] = lv_data.At(5).For(6);
			
					lv_data  = lt_WO_section.getNthBracketedField(1, "VC-",  mc_delimiter.to_string()) ;
					if ( lv_data.NotIsNull() )
					{
               	if ( lv_data.At(3).For(1) == "/" )
               	{
                  	md_ID_sup_id[mi_ID_I] = lv_data.For(2) ;
               	}
               	else
               	{
                  	md_ID_sup_id[mi_ID_I] = lv_data.For(3) ;
               	}
					}
			
					lv_data  = lt_WO_section.getNthBracketedField(1, "AMT-",  mc_delimiter.to_string()) ;
					if ( lv_data.NotIsNull() )
					{
						md_ID_cur_id[mi_ID_I] = lv_data.For(3);
						if ( md_ID_cur_id[mi_ID_I] == "   " )
							md_ID_cur_id[mi_ID_I] = "USD";

						ld_baseCost = lv_data.At(4).For(11);
						if (lv_mco_type == "RV") {
							ld_baseCost = D_amount(0);
						}
						md_ID_base_cost[mi_ID_I] = ld_baseCost;
					}
			
					lv_data  = lt_WO_section.getNthBracketedField(1, "TL-",  mc_delimiter.to_string()) ;
					if ( lv_data.NotIsNull() )
					{
						ld_totalFare = lv_data.At(4).For(11);
					}
			
					lv_data  = lt_WO_section.getNthBracketedField(1, "CP-",  mc_delimiter.to_string()) ;
					if ( lv_data.NotIsNull() )
					{
               	ld_commRate = D_amount(lv_data);
						md_ID_comm_rate[mi_ID_I]  = ld_commRate;
						md_ID_commission[mi_ID_I] = AB_null;
            	}

					lv_data  = lt_WO_section.getNthBracketedField(1, "CA-",  mc_delimiter.to_string()) ;
					if ( lv_data.NotIsNull() )
					{
               	ld_commission  = D_amount(lv_data);
						md_ID_commission[mi_ID_I] = ld_commission + 
											ld_commRate * D_amount(md_ID_base_cost[mi_ID_I]) / D_amount(100);
						md_ID_comm_rate[mi_ID_I]  = AB_null;
					}

					lv_data  = lt_WO_section.getNthBracketedField(1, "FP-",  mc_delimiter.to_string()) ;
					if ( lv_data.NotIsNull() )
					{
						getFormOfPay(	lv_data,
                         			ld_formOfPay ,
                         			ld_cc_id,
                         			ld_cc_no,
                         			ld_cc_exp_date,
                         			ld_auth_no,
                         			ld_auth_source );

						fill_Irpayment(ld_totalFare,
                           		ld_formOfPay,
                           		ld_cc_id,
                           		ld_cc_no,
                           		ld_cc_exp_date,
                           		ld_auth_no,
                           		ld_auth_source);

					}

					lv_data  = lt_WO_section.getNthBracketedField(1, "TS-",  mc_delimiter.to_string()) ;
					if (lv_data.NotIsNull()) {
						lv_remark = lv_data;
					}

					if (lv_mco_type == "SF") {
						lv_unit_type = "TAF";
   					lv_remark = "TRAVEL AGENCY SERVICE FEE";
					} else if (lv_mco_type == "RV") {
						lv_unit_type = md_ID_sup_id[mi_ID_I];
   					lv_remark = "RESIDUAL VALUE MCO";
					} else if (lv_mco_type == "OI") {
						lv_unit_type = md_ID_sup_id[mi_ID_I];
   					lv_remark = lv_remark;
						processMCOTax(lt_WO_section);
					}

					createMCOItin(lv_unit_type, lv_remark);

					lb_ticketFound = AB_true;
				}
			}
		}
	}

	EXIT;
	return lb_ticketFound;
}


void CTAIR::createMCOItin(const AB_Varchar& pv_unit_type,
								  const AB_Varchar& pv_remark)
{
   ENTER;

	if ( mi_MCO_itin_line_number >=  0 )
		return;

   AB_Date     ld_date ;

   md_IO_sup_id[mi_IO_I] = md_ID_sup_id[mi_ID_I];

   md_IO_itin_type[mi_IO_I] = "O" ;

   ld_date.getGMTNow() ;

   md_IO_start_date[mi_IO_I] = ld_date.to_stringFormat("DDMMMYY") ;
   md_IO_orig_city_id[mi_IO_I] = "ZZZ" ;
   md_IO_dest_city_id[mi_IO_I] = "ZZZ" ;
   md_IO_unit_ct[mi_IO_I] = 1 ;
   md_IO_unit_type[mi_IO_I] = pv_unit_type;
   md_IO_line_number[mi_IO_I] = mi_XDLine++ ;
   md_IO_end_date[mi_IO_I] = md_IO_start_date[mi_IO_I] ;
   md_IO_reference[mi_IO_I] = md_IRT_reference ;

   md_IRMK_remark[mi_IRMK_I]           = pv_remark;
   md_IRMK_itin_type[mi_IRMK_I]        = "O" ;
   md_IRMK_oth_line_number[mi_IRMK_I++]  = md_IO_line_number[mi_IO_I] ;

	mi_MCO_itin_line_number = mi_IO_I;

   mi_IO_I++ ;

   EXIT;
}

void CTAIR::createMCOConsumDetail()
{
   ENTER;

	// createMCOItin();

	if ( mi_MCO_itin_line_number < 0 )
		return;

   md_ICD_itin_type[mi_ICD_I]          = "O" ;
   md_ICD_itin_line_number[mi_ICD_I]   = md_IO_line_number[mi_MCO_itin_line_number] ;
   md_ICD_pax_id[mi_ICD_I]             = md_IP_pax_id[mi_IP_I];
   md_ICD_line_number[mi_ICD_I++]      = md_ID_line_number[mi_ID_I] ;

   EXIT;
}


// WS 17 END

//--------------------------------------------------------------------------------------------------------------
void CTAIR::processFare(CPNRWS       &po_PNR,
								AB_Integer   &li_pax_id,
								D_pic        &pc_Ptc,
								D_amount     &pd_totTax)
{
	ENTER;

	int				li_pos, li_index ;
	AB_Text		 	lt_WG_section ;
	V_AB_Varchar 	lv_arrayField(100,100) ;
	AB_Varchar		lv_taxString1(20) ;
	AB_Varchar		lv_taxString2(20) ;
	AB_Varchar		lv_taxString3(20) ;
	D_amount		   ld_baseCost ;
	D_amount       ld_totCalcFare ;
	D_amount       ld_totWGFare ;
	D_amount       ld_diffInTax ;

	li_index = 1 ;
	
   while ( li_index < mi_airFareSectionWG )
	{
		lt_WG_section = mva_airFareSectionWG[li_index] ;

		if ( lt_WG_section.At(2).For(3) == pc_Ptc )
		{
			lv_taxString1 = AB_null ;
			lv_taxString2 = AB_null ;
			lv_taxString3 = AB_null ;

			// Found Fare for the PTC

			li_pos = lv_arrayField.parseStringMultDel(lt_WG_section,mc_delimiter.to_string()) ;

			if ( li_pos > 0 )
			{
				//LOG(logDEBUG) << "MCO TICKET " << lv_arrayField[6]  ;

				if ( lv_arrayField[6].For(1) == "*" )
				{
					md_ID_cur_id[mi_ID_I]    = lv_arrayField[1].At(5).For(3) ;
				
					if ( md_ID_cur_id[mi_ID_I] == "   ")
						md_ID_cur_id[mi_ID_I] = "USD" ;

					ld_baseCost  = lv_arrayField[1].At(8).For(8) ;
					md_ID_base_cost[mi_ID_I]  = ld_baseCost ;
				}
				else
				{
					md_ID_cur_id[mi_ID_I]    = lv_arrayField[6].For(3) ;
				
					if ( md_ID_cur_id[mi_ID_I] == "   ")
						md_ID_cur_id[mi_ID_I] = "USD" ;

					ld_baseCost  = lv_arrayField[6].At(4).For(8) ;
					md_ID_base_cost[mi_ID_I]  = ld_baseCost ;
				}

				getCommission(pc_Ptc) ;

				if ( lv_arrayField[2].For(1) != "*" )
				{
					lv_taxString1 = lv_arrayField[2].At(2) ;
				}
				if ( lv_arrayField[3].For(1) != "*" )
				{
					lv_taxString2 = lv_arrayField[3].At(2) ;
				}
				if ( lv_arrayField[4].For(1) != "*" )
				{
					lv_taxString3 = lv_arrayField[4].At(2) ;
				}
				
				processTax(lv_taxString1,lv_taxString2,lv_taxString3,pd_totTax) ;

				if (md_ID_ticket_no[mi_ID_I].NotIsNull() && md_ID_base_cost[mi_ID_I].NotIsNull())
				{
					fill_Irpayment(lv_arrayField[5]);
				}

				ld_totCalcFare = pd_totTax + D_amount(md_ID_base_cost[mi_ID_I]) ;

				// get Tot fare from G section

				ld_totWGFare = lv_arrayField[5].At(4).For(8) ;

				if ( ld_totCalcFare != ld_totWGFare )
				{
					ld_diffInTax = ld_totWGFare - ld_totCalcFare ;

					if ( ld_diffInTax < D_amount(0) )
					{
					  CMsgHandler::setMsg(3461, " ", "TKT-PARSE", 1, AB_Varchar(20,AB_null));
					  return ;
					}
					else
					{
						// Add it as ZZ tax
						md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
						md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
						md_ITD_tax_id[mi_ITD_I]       = "ZZ" ;
						md_ITD_tax_amount[mi_ITD_I++] = ld_diffInTax ;

						pd_totTax = pd_totTax + ld_diffInTax ;
					}
				}
			}
			break ;	
		}	

		li_index++ ;
	}
   EXIT;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::processTax(AB_Varchar	&pv_taxString1,
							  AB_Varchar   &pv_taxString2,
							  AB_Varchar   &pv_taxString3,
							  D_amount     &pd_totTax)
{
   ENTER;	

	D_amount		ld_taxAmount ;
   D_amount    lm_XTtax;
   D_amount    lm_XFtax;
// SCR #117479 START
   D_amount    ld_totXFTaxes ;
// SCR #117479 END

	pd_totTax = D_amount(0) ;
	lm_XTtax = D_amount(0) ;
	lm_XFtax = D_amount(0) ;
	
	if ( pv_taxString1.NotIsNull() )
	{
   	if ( pv_taxString1.At(3).For(8) != "00000000" )
		{
			if (!pv_taxString1.At(6).For(6).isAllAlpha() )
			{	
				ld_taxAmount  = pv_taxString1.At(3).For(8) ;


				if ( pv_taxString1.For(2) == "XT" )
				{
					lm_XTtax = lm_XTtax + ld_taxAmount ;
				}
				else if ( pv_taxString1.For(2) == "XF" )
				{
					lm_XFtax = ld_taxAmount ;
				}
				else
				{
					md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
					md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
					md_ITD_tax_id[mi_ITD_I]       = pv_taxString1.For(2)  ;
	
					if ( md_ITD_tax_id[mi_ITD_I] == "  " )
						md_ITD_tax_id[mi_ITD_I] = "US" ;
	
					md_ITD_tax_amount[mi_ITD_I++] = ld_taxAmount ;

					pd_totTax = pd_totTax + ld_taxAmount ;
				}
			}
		}
	}
		
	if ( pv_taxString2.NotIsNull() )
	{
   	if ( pv_taxString2.At(3).For(8) != "00000000" )
		{
			if (!pv_taxString2.At(6).For(6).isAllAlpha() )
			{	
				ld_taxAmount                  = pv_taxString2.At(3).For(8) ;

				if ( pv_taxString2.For(2) == "XT" )
				{
					lm_XTtax = lm_XTtax + ld_taxAmount ;
				}
				else if ( pv_taxString2.For(2) == "XF" )
				{
					lm_XFtax = ld_taxAmount ;
				}
				else
				{
					md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
					md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
					md_ITD_tax_id[mi_ITD_I]       = pv_taxString2.For(2)  ;
	
					if ( md_ITD_tax_id[mi_ITD_I] == "  " )
						md_ITD_tax_id[mi_ITD_I] = "US" ;
	
					md_ITD_tax_amount[mi_ITD_I++] = ld_taxAmount ;

					pd_totTax = pd_totTax + ld_taxAmount ;
				}
			}
		}
	}

	if ( pv_taxString3.NotIsNull() )
	{
   	if ( pv_taxString3.At(3).For(8) != "00000000" )
		{
			if (!pv_taxString3.At(6).For(6).isAllAlpha() )
			{	
				ld_taxAmount                  = pv_taxString3.At(3).For(8) ;

				if ( pv_taxString3.For(2) == "XT" )
				{
					lm_XTtax = lm_XTtax + ld_taxAmount ;
				}
				else if ( pv_taxString3.For(2) == "XF" )
				{
					lm_XFtax = ld_taxAmount ;
				}
				else 
				{
					md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
					md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
					md_ITD_tax_id[mi_ITD_I]       = pv_taxString3.For(2)  ;
	
					if ( md_ITD_tax_id[mi_ITD_I] == "  " )
						md_ITD_tax_id[mi_ITD_I] = "US" ;
	
					md_ITD_tax_amount[mi_ITD_I++] = ld_taxAmount ;

					pd_totTax = pd_totTax + ld_taxAmount ;
				}
			}
		}
	}

   if ( lm_XFtax > D_amount(0)  || lm_XTtax > D_amount(0) )
   {
// SCR #117479 START

      getXFbreakdown(ld_totXFTaxes) ;

      pd_totTax = pd_totTax + ld_totXFTaxes ;

      if ( ld_totXFTaxes == D_amount(0) )
      {
         if ( lm_XFtax > D_amount(0) )
         {
            md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
            md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
            md_ITD_tax_id[mi_ITD_I]       = "XF" ;
            md_ITD_tax_amount[mi_ITD_I++]   = lm_XFtax ;

            pd_totTax = pd_totTax + lm_XFtax ;
         }
      }
      else
      {
         if ( lm_XTtax  > D_amount(0) )
         {
            if ( lm_XFtax == D_amount(0) )
            {
               lm_XTtax = lm_XTtax - ld_totXFTaxes ;

            }
         }
      }
// SCR #117479 END

      if ( lm_XTtax > D_amount(0) )
      {
         md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
         md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
         md_ITD_tax_id[mi_ITD_I]       = "XT" ;
         md_ITD_tax_amount[mi_ITD_I++]   = lm_XTtax ;

			pd_totTax = pd_totTax + lm_XTtax ;
      }
   }
   EXIT;
}


//---------------------------------------------------------------------------------------------------------------------------

void CTAIR::fill_Irpayment(AB_Text &pt_totalFare)
{
	ENTER;

	md_IRP_amount[mi_IRP_I]      = pt_totalFare.At(4).For(8) ;
	md_IRP_pax_id[mi_IRP_I]      = md_ID_pax_id[mi_ID_I] ;
	md_IRP_line_number[mi_IRP_I] = md_ID_line_number[mi_ID_I] ;
	md_IRP_pay_id[mi_IRP_I]      = AB_Integer(1) ;
	md_IRP_form_of_pay[mi_IRP_I] = md_formOfPay ;

	if ( md_formOfPay == "CC" )
	{
		md_IRP_cc_id[mi_IRP_I]       = md_cc_id ;
		md_IRP_cc_no[mi_IRP_I]       = md_cc_no ;
		md_IRP_cc_exp_date[mi_IRP_I] = md_cc_exp_date ;
		md_IRP_auth_no[mi_IRP_I]     = md_auth_no ;
		md_IRP_auth_source[mi_IRP_I] =  md_auth_source ;
	}

	mi_IRP_I++ ;
   EXIT;
}

//---------------------------------------------------------------------------------------------------------------------------
// WS 17 START
void CTAIR::fill_Irpayment(D_amount 					&pd_amount,
									D_form_of_payment 		&pd_formOfPay,
									D_credit_card_sup 		&pd_cc_id,
									D_credit_card_no 			&pd_cc_no,
									D_expiry_date 				&pd_cc_exp_date,
									D_authorization 			&pd_auth_no,
									D_authorization_source 	&pd_auth_source)
{
	ENTER;

	md_IRP_amount[mi_IRP_I]      = pd_amount;
	md_IRP_pax_id[mi_IRP_I]      = md_ID_pax_id[mi_ID_I] ;
	md_IRP_line_number[mi_IRP_I] = md_ID_line_number[mi_ID_I] ;
	md_IRP_pay_id[mi_IRP_I]      = AB_Integer(1) ;
	md_IRP_form_of_pay[mi_IRP_I] = pd_formOfPay ;

	if ( md_formOfPay == "CC" )
	{
		md_IRP_cc_id[mi_IRP_I]       = pd_cc_id ;
		md_IRP_cc_no[mi_IRP_I]       = pd_cc_no ;
		md_IRP_cc_exp_date[mi_IRP_I] = pd_cc_exp_date ;
		md_IRP_auth_no[mi_IRP_I]     = pd_auth_no ;
		md_IRP_auth_source[mi_IRP_I] =  pd_auth_source ;
	}

	mi_IRP_I++ ;
   EXIT;
}
//---------------------------------------------------------------------------------------------------------------------------
// WS 17 END
// WS 17 START *******
void CTAIR::getCommission(D_pic	&pd_ptcType)
{
	ENTER;

	int		li_index, li_pos, li_numOfFields, li_commIndex ;
	AB_Boolean lb_exchange;
	AB_Boolean lb_refund;
	AB_Text  lt_commission ;

	AB_Varchar	lv_commission(20) ;
	AB_Varchar  lv_commRate(20) ;

	D_amount	 ld_commission ;
	D_amount  ld_commRate ;

	V_AB_Varchar lv_arrayField(100, 100);
	AB_Varchar   lv_data(100) ;

	li_index = 1 ;
	ld_commission = D_amount(0);
	ld_commRate = D_amount(0);
	lb_exchange = AB_false;
	lb_refund = AB_false;
	md_ID_comm_penalty[mi_ID_I] = AB_null;
	md_ID_ref_commission[mi_ID_I] = AB_null;
	md_ID_ref_comm_rate[mi_ID_I] = AB_null;
	
   while (li_index < mi_commIndex )
	{
		li_numOfFields = lv_arrayField.parseStringMultDel(mva_airCommission[li_index], mc_delimiter.to_string());


		if ( lv_arrayField[1] == pd_ptcType )
		{
			lv_data = mva_airCommission[li_index].getNthBracketedField(1, "EXCH",  mc_delimiter.to_string()) ;
			if ( lv_data.NotIsNull() )
			{
				lb_exchange = AB_true;
			}

			lv_data = mva_airCommission[li_index].getNthBracketedField(1, "ACA-",  mc_delimiter.to_string()) ;
			lv_data.removeLeadingBlanks();
			lv_data.removeTrailingBlanks();

			if ( lv_data.NotIsNull() )
			{
				md_ID_comm_penalty[mi_ID_I] = lv_data.For(8);		
				if ( lb_exchange == AB_false )
					lb_refund = AB_true;
			}

			lv_data = mva_airCommission[li_index].getNthBracketedField(1, "OCA-",  mc_delimiter.to_string()) ;
			lv_data.removeLeadingBlanks();
			lv_data.removeTrailingBlanks();

			if ( lv_data.NotIsNull() )
			{
				md_ID_ref_commission[mi_ID_I] = lv_data.For(8);		
				md_ID_ref_comm_rate[mi_ID_I] = AB_null;
				if ( lb_exchange == AB_false )
					lb_refund = AB_true;
			}

			lv_data = mva_airCommission[li_index].getNthBracketedField(1, "NCA-",  mc_delimiter.to_string()) ;
			lv_data.removeLeadingBlanks();
			lv_data.removeTrailingBlanks();

			if ( lv_data.NotIsNull() )
			{
				md_ID_commission[mi_ID_I] = lv_data.For(8);		
				md_ID_comm_rate[mi_ID_I]  = AB_null ;
			}



			if ( lb_exchange == AB_true || lb_refund == AB_true)
			{
				lv_data = mva_airCommission[li_index].getNthBracketedField(1, "CP-",  mc_delimiter.to_string()) ;
				lv_data.removeLeadingBlanks();
				lv_data.removeTrailingBlanks();

				if ( 	lv_data.NotIsNull() 						&& 
						md_ID_commission[mi_ID_I].IsNull() 	&&
						md_ID_comm_rate[mi_ID_I].IsNull() )
				{
					md_ID_comm_rate[mi_ID_I]  = lv_data.For(5);
					md_ID_commission[mi_ID_I] = AB_null ;
				}

				lv_data = mva_airCommission[li_index].getNthBracketedField(1, "CA-",  mc_delimiter.to_string()) ;
				lv_data.removeLeadingBlanks();
				lv_data.removeTrailingBlanks();

				if ( 	lv_data.NotIsNull() )
				{
					if ( 	lv_data.For(1) == "P"					&&
							md_ID_commission[mi_ID_I].IsNull()  &&
							md_ID_comm_rate[mi_ID_I].IsNull() 	&&
							lv_data.At(2).NotIsNull() )
					{
						md_ID_commission[mi_ID_I] = lv_data.At(2).For(8) ;
						md_ID_comm_rate[mi_ID_I]  = AB_null ;
					}
					else if ( lv_data.For(1) == "N"   					&&
								md_ID_ref_commission[mi_ID_I].IsNull()	&&
								md_ID_ref_comm_rate[mi_ID_I].IsNull()	&&
								lv_data.At(2).NotIsNull() )
					{
						md_ID_ref_commission[mi_ID_I] = lv_data.At(2).For(8) ;
						md_ID_ref_comm_rate[mi_ID_I] = AB_null;
					}
					else if ( lv_data.For(1) == " "					&&
								lb_refund == AB_false					&&
								md_ID_commission[mi_ID_I].IsNull()  &&
								md_ID_comm_rate[mi_ID_I].IsNull() 	&&
								lv_data.At(2).NotIsNull() ) 
					{
						md_ID_commission[mi_ID_I] = lv_data.At(2).For(8) ; 
						md_ID_comm_rate[mi_ID_I] = AB_null;
					}
					else if ( lv_data.For(1) == " "              	&&
								lb_refund == AB_true							&&
								md_ID_ref_commission[mi_ID_I].IsNull() &&
								md_ID_ref_comm_rate[mi_ID_I].IsNull()  &&
								lv_data.At(2).NotIsNull() )
					{
						md_ID_ref_commission[mi_ID_I] = lv_data.At(2).For(8) ; 
						md_ID_ref_comm_rate[mi_ID_I] = AB_null;
					}
				}
			}
			else
			{
				li_commIndex = 2;

				while ( li_commIndex <= li_numOfFields )
				{
					if ( lv_arrayField[li_commIndex].For(3) == "CP-" )
					{
						lv_commRate = lv_arrayField[li_commIndex].At(4);
						lv_commRate.removeLeadingBlanks();
						lv_commRate.removeTrailingBlanks();

						if ( lv_commRate.NotIsNull() )
						{
							ld_commRate = ld_commRate + D_amount(lv_commRate);
						}
					}
					else if ( lv_arrayField[li_commIndex].For(3) == "CA-" )
					{
						lv_commission = lv_arrayField[li_commIndex].At(5);
						lv_commission.removeLeadingBlanks();
						lv_commission.removeTrailingBlanks();

						if ( lv_commission.NotIsNull() )
						{
							ld_commission  = ld_commission + D_amount(lv_commission);
						}	
					}
					li_commIndex++;
				}

				if ( ld_commission > D_amount(0) )
				{
					md_ID_commission[mi_ID_I] = ld_commission + ld_commRate * D_amount(md_ID_base_cost[mi_ID_I]) / D_amount(100);
					md_ID_comm_rate[mi_ID_I]  = AB_null;
				}
				else if ( ld_commRate > D_amount(0) )
				{
					md_ID_comm_rate[mi_ID_I]  = ld_commRate;
					md_ID_commission[mi_ID_I] = AB_null;
				}

			}
			break;
		}
		li_index++;
	}


   EXIT;  
}
// WS 17 END *******
	

//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CTAIR::processExchange(AB_Integer &pi_paxId,
											 D_pic      &pd_Ptc,
											 D_amount   &pd_totTax,
											 AB_Char    &pc_docLine)
{
   ENTER;

   // WS 17 START *******
   int           li_pos ;
   AB_Text       lt_exchange ;
   AB_Text       lt_exchangeFare ;
   AB_Boolean    lb_exchange ;
   D_amount      ld_baseCost ;
   D_amount      ld_oldTax ;
   AB_Varchar    lv_taxString1(20) ;
   AB_Varchar    lv_taxString2(20) ;
   AB_Varchar    lv_taxString3(20) ;
   D_amount      ld_totTax ;

   // C EXCHG CODE START
   D_amount      ld_newBaseCost ;
   D_amount      ld_penaltyFee ;
   D_amount      ld_newTotTax;
   // C EXCHG CODE END

   AB_Char      lc_baseCost(12) ;
   AB_Char      lc_commission(9) ;
   AB_Varchar   lv_commission(9) ;
   AB_Char      lc_tax(8) ;
   AB_Varchar   lv_remark(80) ;
   AB_Varchar   lv_data(100) ;

   D_amount     ld_commission ;
   AB_Boolean   lb_penalty ;
   AB_Boolean   lb_oldValuesSpecified ;

   lb_exchange = AB_false ;
   lb_penalty = AB_false ;
   lb_oldValuesSpecified = AB_false;
   ld_totTax = D_amount(0);
   ld_baseCost = D_amount(0);

   // C EXCHG CODE START
   ld_newBaseCost = D_amount(0);
   ld_penaltyFee  = D_amount(0);
   ld_newTotTax   = D_amount(0);
   // C EXCHG CODE END

   // WS 17 END *******

   lt_exchange = mva_exchSectionWC[pi_paxId.to_long()] ;

   lv_remark = AB_null ;


   if ( lt_exchange.NotIsNull() )
   {
      if ( lt_exchange.At(5).For(1) != "*" )
      {
         lb_exchange = AB_true ;

         md_ID_orig_ticket_no[mi_ID_I] = lt_exchange.At(6).For(13) ;

         md_ID_exchange_code[mi_ID_I] = lt_exchange.At(4).For(1) ;


         if ( md_ID_exchange_code[mi_ID_I] != "E" )
         {
            lv_taxString1 = AB_null ;
            lv_taxString2 = AB_null ;
            lv_taxString3 = AB_null ;

            // WS 17 START *******
            lt_exchangeFare = mva_exchSectionWX[pi_paxId.to_long()];

            if ( lt_exchangeFare.NotIsNull() )
            {

               lt_exchangeFare = lt_exchangeFare + mc_delimiter;

               md_ID_exchange_code[mi_ID_I] = lt_exchangeFare.At(5).For(1);

               if ( md_ID_exchange_code[mi_ID_I] == "N" )
                  md_ID_exchange_code[mi_ID_I] = "E" ;

               lv_data  = lt_exchangeFare.getNthBracketedField(1, "AP-",  mc_delimiter.to_string()) ;
               if ( lv_data.NotIsNull() )
                  md_ID_penalty_ch_fee[mi_ID_I] =  lv_data;

               if ( md_ID_exchange_code[mi_ID_I] != "E" )
               {
                  lv_data = lt_exchangeFare.getNthBracketedField(1, "OBF-",  mc_delimiter.to_string()) ;
                  if ( lv_data.NotIsNull() )
                  {
                     lb_oldValuesSpecified = AB_true;

                     ld_baseCost = lv_data.For(8);

                     lv_data = lt_exchangeFare.getNthBracketedField(1, "OT1-",  mc_delimiter.to_string()) ;
                     if ( lv_data.NotIsNull() )
                     {
                        ld_oldTax = lv_data.At(3).For(8);
                        ld_totTax = ld_totTax + ld_oldTax;
                     }

                     lv_data = lt_exchangeFare.getNthBracketedField(1, "OT2-",  mc_delimiter.to_string()) ;
                     if ( lv_data.NotIsNull() )
                     {
                        ld_oldTax = lv_data.At(3).For(8);
                        ld_totTax = ld_totTax + ld_oldTax;
                     }

                     lv_data = lt_exchangeFare.getNthBracketedField(1, "OT3-",  mc_delimiter.to_string()) ;
                     if ( lv_data.NotIsNull() )
                     {
                        ld_oldTax = lv_data.At(3).For(8);
                        ld_totTax = ld_totTax + ld_oldTax;
                     }
                  }

                  // C EXCHG CODE START

                  lv_data = lt_exchangeFare.getNthBracketedField(1, "#T1-",  mc_delimiter.to_string()) ;
                  if ( lv_data.NotIsNull() )
                  {
                     lv_taxString1 = lv_data.For(10);
                  }

                  lv_data = lt_exchangeFare.getNthBracketedField(1, "#T2-",  mc_delimiter.to_string()) ;
                  if ( lv_data.NotIsNull() )
                  {
                     lv_taxString2 = lv_data.For(10);
                  }

                  lv_data = lt_exchangeFare.getNthBracketedField(1, "#T3-",  mc_delimiter.to_string()) ;
                  if ( lv_data.NotIsNull() )
                  {
                     lv_taxString3 = lv_data.For(10);
                  }

                  processExchangeTax(lv_taxString1,lv_taxString2,lv_taxString3,ld_newTotTax) ;


                  lv_data  = lt_exchangeFare.getNthBracketedField(1, "#BF-",  mc_delimiter.to_string()) ;

                  if ( lv_data.NotIsNull() )
                  {
                     ld_newBaseCost = lv_data;
                  }
                  else
                  {
                     ld_newBaseCost = D_amount(0);
                  }

                  if ( lb_oldValuesSpecified != AB_true )
                  {
                     ld_baseCost    = ld_newBaseCost;
                     ld_totTax      = ld_newTotTax;
                  }

                  if ( md_ID_penalty_ch_fee[mi_ID_I].IsNull() )
                  {
                     md_ID_penalty_ch_fee[mi_ID_I] = "0" ;
                  }

                  ld_penaltyFee = D_amount( md_ID_penalty_ch_fee[mi_ID_I] );

                  if ( md_ID_exchange_code[mi_ID_I] == "C" )
                  {
                     if ( ( ld_newBaseCost +
                            ld_newTotTax ) == ld_penaltyFee )
                     {
                        md_ID_exchange_code[mi_ID_I] = "E";
                     }
                     else if ( ( ld_newBaseCost +
                                 ld_newTotTax ) < ld_penaltyFee )
                     {
                        md_ID_exchange_code[mi_ID_I] = "A";
                     }
                     else
                     {
                        md_ID_exchange_code[mi_ID_I] = "R";
                     }
                  }

                  // C EXCHG CODE END


            // WS 17 END *******

						// FIX FOR A -> E REPLACEMENT START

						if ( 	md_ID_exchange_code[mi_ID_I] == "A" 					&&
								D_amount(md_ID_base_cost[mi_ID_I]) < ld_baseCost 	&&
								lb_oldValuesSpecified != AB_true )
						{
							md_ID_exchange_code[mi_ID_I] = "E" ;
							lb_penalty = AB_true ;
						}

						// FIX FOR A -> E REPLACEMENT END

                  if ( md_ID_exchange_code[mi_ID_I] == "A" )      // Adcoll Exchange
                  {
                     // WS 17 START
                     if ( lb_oldValuesSpecified != AB_true )
                     {
                        ld_baseCost = D_amount(md_ID_base_cost[mi_ID_I]) - ld_baseCost ;

                        ld_totTax = pd_totTax - ld_totTax ;

                        if ( ld_totTax < D_amount(0) )
                        {
                           ld_baseCost = ld_baseCost - ld_totTax ;
                           lc_tax = " " ;
                        }
                        else
                        {
                           lc_tax = ld_totTax ;
                           lc_tax.rightJustify('0') ;
                        }

                     }
                     else
                     {
                        lc_tax = ld_totTax ;
                        lc_tax.rightJustify('0') ;
                     }

                     lc_baseCost = ld_baseCost ;
                     lc_baseCost.rightJustify('0') ;


                     if ( md_ID_ref_commission[mi_ID_I].NotIsNull() )
                     {
                        lc_commission = md_ID_ref_commission[mi_ID_I];
                     }
                     else if ( md_ID_ref_comm_rate[mi_ID_I].NotIsNull() )
                     {
                        ld_commission = md_ID_ref_comm_rate[mi_ID_I];
                        ld_commission = ld_commission * ld_baseCost / D_amount(100);
                        lc_commission = ld_commission ;
                        lc_commission.rightJustify('0') ;
                     }
                     else
                        lc_commission = "000000.00" ;

                     // WS 17 END

                     lv_remark =  pc_docLine + lc_baseCost + lc_commission + "T1:" + lc_tax + "XT" ;

                  }

                  else if ( md_ID_exchange_code[mi_ID_I] == "R" )       // Refund Exchange
                  {

                     // WS 17 START
                     if ( md_ID_ref_commission[mi_ID_I].NotIsNull())
                     {
                        ld_commission = md_ID_ref_commission[mi_ID_I] ;
                     }
                     else if ( md_ID_ref_comm_rate[mi_ID_I].NotIsNull() )
                     {
                        ld_commission = md_ID_ref_comm_rate[mi_ID_I] ;
                        ld_commission = ld_commission * ld_baseCost / D_amount(100);
                     }
                     else
                     {
                        ld_commission = D_amount(0);
                     }

                     lc_commission = ld_commission ;
                     lc_commission.rightJustify('0') ;

                     if ( lb_oldValuesSpecified != AB_true )
                     {
                        ld_baseCost = ld_baseCost + D_amount(md_ID_base_cost[mi_ID_I]) ;

                        ld_totTax = ld_totTax + pd_totTax ;

                        if ( ld_totTax < D_amount(0) )
                        {
                           ld_baseCost = ld_baseCost - ld_totTax ;
                           lc_tax = " " ;
                        }
                        else
                        {
                           lc_tax = ld_totTax ;
                           lc_tax.rightJustify('0') ;
                        }
                     }
                     else
                     {
                        lc_tax = ld_totTax ;
                        lc_tax.rightJustify('0') ;
                     }

                     lc_baseCost = ld_baseCost ;
                     lc_baseCost.rightJustify('0') ;

                     // WS 17 END

                     lv_remark =  pc_docLine + lc_baseCost + lc_commission + "T1:" + lc_tax + "XT" ;
                  }
               }
            }
         }

         if (  md_ID_exchange_code[mi_ID_I]  == "E" )    // Even Exchange
         {
            if ( lb_penalty )
            {
               // Find out if there is penalty fee

               if ( md_ID_penalty_ch_fee[mi_ID_I].IsNull() )
               {
                  md_ID_penalty_ch_fee[mi_ID_I] = "0" ;
               }
               md_ID_penalty_ch_fee[mi_ID_I] = D_amount(md_ID_penalty_ch_fee[mi_ID_I]) + ld_baseCost + ld_totTax ;

            }

            ld_baseCost = md_ID_base_cost[mi_ID_I] ;

            lc_baseCost = ld_baseCost ;
            lc_baseCost.rightJustify('0') ;

            // WS 17 START
            if ( md_ID_ref_commission[mi_ID_I].NotIsNull() )
            {
               lc_commission =  md_ID_ref_commission[mi_ID_I];
               lc_commission.rightJustify('0') ;
            }
            else if ( md_ID_ref_comm_rate[mi_ID_I].NotIsNull() )
            {
               lc_commission =  md_ID_ref_comm_rate[mi_ID_I] ;
               lc_commission.rightJustify('0') ;
               lc_commission = "P" + lc_commission.At(2) ;
            }
            else
            {
               lc_commission = "000000.00" ;
            }
            // WS 17 END

            lc_tax = pd_totTax ;
            lc_tax.rightJustify('0') ;

            lv_remark =  pc_docLine + lc_baseCost + lc_commission + "T1:" + lc_tax + "XT" ;
         }
      }
   }


   if ( lv_remark.NotIsNull() )
   {

      md_IRMK_type[mi_IRMK_I]           = "COS" ;
      md_IRMK_remark[mi_IRMK_I]         = lv_remark ;
      md_IRMK_pax_id[mi_IRMK_I++]       = md_ID_pax_id[mi_ID_I] ;
   }

   return(lb_exchange) ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::processExchangeTax(AB_Varchar	&pv_taxString1,
										AB_Varchar  &pv_taxString2,
										AB_Varchar  &pv_taxString3,
										D_amount    &pd_totTax)
{
	D_amount		ld_taxAmount ;

	pd_totTax = D_amount(0) ;
	
	if ( pv_taxString1.NotIsNull() )
	{
   	if ( pv_taxString1.At(3).For(8) != "00000000" )
		{
			if (!pv_taxString1.At(6).For(6).isAllAlpha() )
			{	
				ld_taxAmount                  = pv_taxString1.At(3).For(8) ;

				pd_totTax = pd_totTax + ld_taxAmount ;
			}
		}
	}
		
	if ( pv_taxString2.NotIsNull() )
	{
   	if ( pv_taxString2.At(3).For(8) != "00000000" )
		{
			if (!pv_taxString2.At(6).For(6).isAllAlpha() )
			{	
				ld_taxAmount                  = pv_taxString2.At(3).For(8) ;

				pd_totTax = pd_totTax + ld_taxAmount ;
			}
		}
	}

	if ( pv_taxString3.NotIsNull() )
	{
   	if ( pv_taxString3.At(3).For(8) != "00000000" )
		{
			if (!pv_taxString1.At(6).For(6).isAllAlpha() )
			{	
				ld_taxAmount                  = pv_taxString3.At(3).For(8) ;

				pd_totTax = pd_totTax + ld_taxAmount ;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------

void CTAIR::fillIrTransport(CPNRWS  &po_PNR)
{
   ENTER;

	AB_Text		  lt_W01_section ;
	AB_Text		  lt_W06_section ;
	V_AB_Varchar  lv_arrayField(100,100) ;
	// WS 17 START *******
	int			  li_pos, time_pos ;
	// WS 17 END *******
	D_char_date   ld_arrDate ; 
	AB_Date       ld_tempDate1 ; 
	AB_Date       ld_tempDate2 ; 
	AB_Integer    li_arrInd ;
	AB_Char       lc_itinLine(2) ;
	AB_Varchar	  lv_field(100) ;
	AB_Varchar	  lv_fareInfo(15) ;

	AB_Char       lc_year(2) ;

	// G145 - Change
	AB_Varchar   lv_airInfo(255) ;

	
   po_PNR.resetSegment("01") ;
   lt_W01_section = po_PNR.getNextSegment("01")  ;

   po_PNR.resetSegment("04") ;
   po_PNR.resetSegment("06") ;

   while (lt_W01_section.NotIsNull()) 
	{
		
		li_pos = lv_arrayField.parseStringMultDel(lt_W01_section,mc_delimiter.to_string());

		md_IT_itin_type[mi_IT_I] = "A" ;

		md_IT_line_number[mi_IT_I]    = lv_arrayField[2] ;

		md_IT_no_of_stops[mi_IT_I]    = lv_arrayField[3].For(1) ;
		if ( md_IT_no_of_stops[mi_IT_I] == "*" )
			md_IT_no_of_stops[mi_IT_I] = 0 ;

		if ( lv_arrayField[3].At(2).For(1) != " " )
		{
			// Get Shared Designator
			lt_W06_section =  po_PNR.getNextSegment("06")  ;
			md_IT_aff_carrier[mi_IT_I] = lt_W06_section.At(3).For(50) ;
		}

		md_IT_sup_id[mi_IT_I]         = lv_arrayField[3].At(3).For(2) ;
		md_IT_service[mi_IT_I]        = lv_arrayField[3].At(5).For(4) ;

		md_IT_classofservice[mi_IT_I] = lv_arrayField[4] ;

		md_IT_orig_city_id[mi_IT_I]   = lv_arrayField[5].For(3) ;

		// WS 17 START *******
		lc_year                       = mt_lclPnrDate.get_default_year(lv_arrayField[5].At(4).For(5)) ;
		lc_year.rightJustify('0') ;

		if ( lv_arrayField[5].At(4).For(1) != "*" )
		{
			md_IT_start_date[mi_IT_I]     = lv_arrayField[5].At(4).For(5) + lc_year ;
			time_pos = 9;
		}
		else
		{
			md_IT_start_date[mi_IT_I]     = mt_lclPnrDate.to_stringFormat("DDMMMYY") ;
			time_pos = 5;
		}

		if ( lv_arrayField[5].At(time_pos).For(1) != "*" )
		{
			md_IT_start_time[mi_IT_I]     = CheckTime(lv_arrayField[5].At(time_pos).For(5));
		}
		else
		{
			md_IT_start_time[mi_IT_I]     = md_lclPnrTime.changeDateFormat("hhmm", "hhmmP") ;
		}

		md_IT_dest_city_id[mi_IT_I]   = lv_arrayField[6].For(3) ;
		lc_year                       = mt_lclPnrDate.get_default_year(lv_arrayField[6].At(4).For(5)) ;
		lc_year.rightJustify('0') ;

      if ( lv_arrayField[6].At(4).For(1) != "*" )
      {
         ld_arrDate     = lv_arrayField[6].At(4).For(5) + lc_year ;
         time_pos = 9;
      }
      else
      {
         ld_arrDate     = mt_lclPnrDate.to_stringFormat("DDMMMYY") ;
         time_pos = 5;
      }

      if ( lv_arrayField[6].At(time_pos).For(1) != "*" )
      {
         md_IT_end_time[mi_IT_I]     = CheckTime(lv_arrayField[6].At(time_pos).For(5));
      }
      else
      {
         md_IT_end_time[mi_IT_I]     = md_lclPnrTime.changeDateFormat("hhmm", "hhmmP") ;
      }
      // WS 17 END *******

		ld_tempDate1                  = md_IT_start_date[mi_IT_I] ;
		ld_tempDate2                  = ld_arrDate ;

		// li_arrInd = ld_tempDate1.datediff(ld_tempDate2,"DD") ;
		li_arrInd = ld_tempDate2 - ld_tempDate1 ; 

		li_arrInd = li_arrInd / AB_Integer(86400) ;

		switch(li_arrInd.to_long())
		{
			case 0:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(0) ;
				break ;
			case 1:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(1) ;
				break ;
			case 2:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(2) ;
				break ;
			case 3:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(3) ;
				break ;
			case -1:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(4) ;
				break ;
			case -2:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(5) ;
				break ;
			default:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(0) ;
            break ;

		}

		
		if ( lv_arrayField[9] == "*" )
			mv_baggage = AB_null ;
		else
			mv_baggage = lv_arrayField[9] ;
		
		if ( lv_arrayField[10].For(1) != "*" )
		{
			mv_service_status = lv_arrayField[10].For(2) ;
		}
		else
		{
			mv_service_status = AB_null ;
		}

		// WS 17 START *******
		if ( lv_arrayField[10].At(lv_arrayField[10].length()).For(1) != "*" )
		{
			md_connect = lv_arrayField[10].At(lv_arrayField[10].length()).For(1);
		}
		else
		{
			md_connect = AB_null ;
		}

		lv_field = lv_arrayField[10].At(3).For(lv_arrayField[10].length() - 3);
		lv_field.removeLeadAndTrailBlanks();

		if ( lv_field.NotIsNull() && lv_field.For(1) != "*" )
		{
			md_IT_meal[mi_IT_I] = lv_field.For(6) ;
		}

		// WS 17 END *******

		if ( lv_arrayField[11].For(1) != "*" )
		{
			md_IT_carrier_type[mi_IT_I] = lv_arrayField[11] ;
		}

		if ( lv_arrayField[12].For(1) != "*" )
		{
			lv_fareInfo                 = lv_arrayField[12] ;
			li_pos         = lv_fareInfo.findSubstring("/") ;
			if ( li_pos > 0 )
			{
				mv_fareBasis                = lv_fareInfo.For(li_pos - 1) ;
				mv_tktDesig                 = lv_fareInfo.At(li_pos + 1) ;
			}
			else
			{
				mv_fareBasis                = lv_fareInfo ;
				mv_tktDesig                 = AB_null ;
			}
		}
		else
		{
			mv_fareBasis  = AB_null ;
			mv_tktDesig   = AB_null ;
		}

		if ( lv_arrayField[13].For(1) != "*" )
		{
			md_IT_actual_miles[mi_IT_I] = lv_arrayField[13] ;
		}
	
		md_IT_reference[mi_IT_I]    = md_IRT_reference ;

		lc_itinLine                 = lv_arrayField[2] ;
		lc_itinLine.rightJustify('0') ;

		// fill Irconsumdetail
		populateIrConsumDetail(po_PNR) ;

		lv_field = lc_itinLine + "A" + md_IT_sup_id[mi_IT_I] 
										+ lv_arrayField[5].At(4).For(5) ;

		mv_IrTransData[++mi_IrTransIndex] = lv_field ;

		//G145 - Change
		lv_airInfo = lc_itinLine + 
						 md_IT_orig_city_id[mi_IT_I] +
						 md_IT_dest_city_id[mi_IT_I] +
						 md_IT_start_date[mi_IT_I] +
						 md_IT_arrival_ind[mi_IT_I] ;

		mva_surfaceTransport[mi_surfaceTransport++]  = lv_airInfo ;
	
    	mi_IT_I++ ;
   	lt_W01_section = po_PNR.getNextSegment("01")  ;
	}
   EXIT;
}

//---------------------------------------------------------------------------------------------------------------------------

void CTAIR::fillIrHotel_H(CPNRWS  &po_PNR)
{
   ENTER;

   int          li_pos, li_count ,i ;
   AB_Varchar   lv_field(50) ;
	AB_Varchar   lv_identifier(5) ;
   AB_Date      ld_tempDate ;
   AB_Varchar   lv_varField(255) ;
	V_AB_Varchar lv_arrayField(100,100) ;

	AB_Varchar	 lv_phone(12) ;
   AB_Varchar   lv_fax(12) ;

	AB_Varchar   lv_SIAddress(100) ;
	AB_Varchar   lv_SIPhone(20) ;

	AB_Text		 lt_WH_section ;
	AB_Char      lc_itinLine(2) ;
	AB_Char      lc_status(2) ;
	AB_Char      lc_year(2) ;

   po_PNR.resetSegment("H") ;
   lt_WH_section = po_PNR.getNextSegment("H")  ;

   while (lt_WH_section.NotIsNull()) 
	{
		lv_SIAddress = AB_null ;

		i = lt_WH_section.length() ;

		if ( lt_WH_section.At(i).For(1) != mc_delimiter )
			lt_WH_section = lt_WH_section + mc_delimiter ;

		li_pos = lv_arrayField.parseStringMultDel(lt_WH_section,mc_delimiter.to_string());

   	md_IR_itin_type[mi_IR_I]      = "H" ;
   	md_IR_line_number[mi_IR_I]    = lv_arrayField[2] ;

		lc_itinLine                   = md_IR_line_number[mi_IR_I] ;
		lc_itinLine.rightJustify('0') ;

		lc_status                     = lv_arrayField[5].For(2) ;

		if ( lc_status == "MK" )
			 md_IR_source[mi_IR_I]	= "M";
		else
			md_IR_source[mi_IR_I]   = "A" ;

   	md_IR_service_status[mi_IR_I] = lv_arrayField[5].For(2) ;
   	md_IR_unit_ct[mi_IR_I]        = lv_arrayField[5].At(3).For(3) ;

		md_IR_origin_city_id[mi_IR_I] = lv_arrayField[6].For(3) ;

		// SCR 220449 START	

		// Get end and start date

		lc_year = mt_lclPnrDate.get_default_year(lv_arrayField[6].At(4).For(5)) ;
		lc_year.rightJustify('0') ;
   	md_IR_start_date[mi_IR_I]     = lv_arrayField[6].At(4).For(5) + lc_year ;

		AB_Date ld_start_date(md_IR_start_date[mi_IR_I]);

		lc_year = ld_start_date.get_default_year(lv_arrayField[6].At(9).For(5)) ;
		lc_year.rightJustify('0') ;

		md_IR_end_date[mi_IR_I]       =  lv_arrayField[6].At(9).For(5) + lc_year ;

		// SCR 220449 END	

		if ( lv_arrayField[3].For(2) == "  " )
			md_IR_sup_id[mi_IR_I]      = lv_arrayField[8].For(2) + "H" ;
		else
			md_IR_sup_id[mi_IR_I]      = lv_arrayField[3].For(2) + "H" ;

	   lv_field = lt_WH_section.getNthBracketedField(1, "CF-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_confirmation[mi_IR_I] = lv_field ;
		}

		md_IR_rate_duration[mi_IR_I]  = "D" ;


		md_IR_location_id[mi_IR_I]    = lv_arrayField[7] ; // property code
		// SCR408389 start
		lv_field = lt_WH_section.getNthBracketedField(1, "PID-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull()) {
			md_IR_location_id[mi_IR_I] = lv_field;
		}
		// SCR408389 end

		md_IR_site_name[mi_IR_I]      = lv_arrayField[8] ;  //Hotel Chain name
		if ( md_IR_site_name[mi_IR_I].At(3).For(1) == " " )
		{
			if ( md_IR_site_name[mi_IR_I].For(2) == md_IR_sup_id[mi_IR_I].For(2) )
			{
				md_IR_site_name[mi_IR_I] = md_IR_site_name[mi_IR_I].At(4) ;
			}
		}

		lv_identifier = mc_delimiter + "R-" ;
		lv_field = lt_WH_section.getNthBracketedField
				(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IR_unit_type[mi_IR_I]      = lv_field.For(4) ;
			md_IR_rate_type[mi_IR_I]      = lv_field.At(5).For(3) ;
		}

		lv_field = lt_WH_section.getNthBracketedField(1, "NP-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
			md_IR_number_of_pax[mi_IR_I] = lv_field ;
		else
			md_IR_number_of_pax[mi_IR_I] = 1 ;

		V_AB_Varchar     lv_arrayHotelAddress(255,4) ;
		int              li_hotelAddressCount ;

		li_hotelAddressCount = 0 ;

		// Get Information from  SI-

		lv_SIAddress = lt_WH_section.getNthBracketedField(1, "SI-/", mc_delimiter.to_string()) ;

		if ( lv_SIAddress.NotIsNull() )
		{
			li_pos  = lv_SIAddress.findSubstring("$PHONE") ;

			if ( li_pos == 0 )
			{
				li_pos  = lv_SIAddress.findSubstring("$FONE") ;
				
				if ( li_pos > 0 )
				{
					lv_SIPhone = lv_SIAddress.At(li_pos + 5 ) ;
					lv_SIPhone.leftJustify() ;
					lv_SIAddress = lv_SIAddress.For( li_pos - 1) ;
				}
				else
				{
					lv_SIAddress = lv_SIAddress ;
				}
			}
			else
			{
				lv_SIPhone = lv_SIAddress.At(li_pos + 6 ) ;
				lv_SIPhone.leftJustify() ;
				lv_SIAddress = lv_SIAddress.For( li_pos - 1) ;
			}

	     	if (lv_SIPhone.NotIsNull())
      	{
        		md_IR_phone_acode[mi_IR_I] = lv_SIPhone.For(5) ;
        		md_IR_phone_no[mi_IR_I]    = lv_SIPhone.At(6).For(7) ;
      	}

			li_hotelAddressCount = lv_arrayHotelAddress.parseStringMultDel(lv_SIAddress,"$");
      	li_count = 1 ;

      	while ( li_count  <= li_hotelAddressCount )
      	{
        		if ( li_count == 1 )
           		md_IR_addr1[mi_IR_I] = lv_arrayHotelAddress[li_count] ;
        		if ( li_count == 2 )
           		md_IR_addr2[mi_IR_I] = lv_arrayHotelAddress[li_count] ;
        		if ( li_count == 3 )
           		md_IR_addr3[mi_IR_I] = lv_arrayHotelAddress[li_count] ;
        		if ( li_count == 4 )
           		md_IR_addr4[mi_IR_I] = lv_arrayHotelAddress[li_count] ;

        		li_count++ ;

      	}

		}
			
		if ( li_hotelAddressCount == 0 )
		{
			lv_field = 
				lt_WH_section.getNthBracketedField(1, "HA1-", mc_delimiter.to_string()) ;
			if (lv_field.NotIsNull())
				lv_arrayHotelAddress[++li_hotelAddressCount] = lv_field ;

			lv_field = lt_WH_section.getNthBracketedField(1, "HA2-", mc_delimiter.to_string()) ;
			if (lv_field.NotIsNull())
				lv_arrayHotelAddress[++li_hotelAddressCount] = lv_field ;


			/*
			lv_field = lt_WH_section.getNthBracketedField(1, "SCC-", mc_delimiter.to_string()) ;
			if (lv_field.NotIsNull())
				lv_arrayHotelAddress[++li_hotelAddressCount] = lv_field ;

			lv_field = lt_WH_section.getNthBracketedField(1, "ZIP-", mc_delimiter.to_string()) ;
			if (lv_field.NotIsNull())
				lv_arrayHotelAddress[++li_hotelAddressCount] = lv_field ;
			*/

			li_count = 1 ;

			while ( li_count  <= li_hotelAddressCount )
			{
				if ( li_count == 1 )
					md_IR_addr1[mi_IR_I] = lv_arrayHotelAddress[li_count] ;
				if ( li_count == 2 )
					md_IR_addr2[mi_IR_I] = lv_arrayHotelAddress[li_count] ;
				if ( li_count == 3 )
					md_IR_addr3[mi_IR_I] = lv_arrayHotelAddress[li_count] ;
				if ( li_count == 4 )
					md_IR_addr4[mi_IR_I] = lv_arrayHotelAddress[li_count] ;
	
				li_count++ ;
				
			}
	
			lv_field = lt_WH_section.getNthBracketedField(1, "PH-", mc_delimiter.to_string()) ;
			if (lv_field.NotIsNull())
			{
				md_IR_phone_acode[mi_IR_I] = lv_field.For(5) ;
				md_IR_phone_no[mi_IR_I]    = lv_field.At(6) ;
			}
	
			lv_field = lt_WH_section.getNthBracketedField(1, "FAX-", mc_delimiter.to_string()) ;
			if (lv_field.NotIsNull())
			{
				md_IR_fax_acode[mi_IR_I] = lv_field.For(5) ;
				md_IR_fax_no[mi_IR_I]    = lv_field.At(6) ;
			}
		}

	   lv_field = lt_WH_section.getNthBracketedField(1, "FG-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_customer_id[mi_IR_I] = lv_field ;
		}
	
	
		lv_field = lt_WH_section.getNthBracketedField(1, "FT-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
      	md_IR_fup[mi_IR_I] = lv_field ;
		}

		lv_field = lt_WH_section.getNthBracketedField(1, "CD-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
      	md_IR_corporation_id[mi_IR_I] = lv_field ;
		}

		lv_identifier = mc_delimiter + "G-" ;
		lv_field = lt_WH_section.getNthBracketedField
				(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_guarantee[mi_IR_I] = lv_field ;
		}

		lv_identifier = mc_delimiter + "CI-" ;
		lv_field = lt_WH_section.getNthBracketedField
				(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			if ( lv_field.length() == 4 || lv_field.length() == 5 )
			{
				if ( lv_field.For(4).isAllNumeric() )
				{
					md_IR_start_time[mi_IR_I] = lv_field ;
				}			
			} else {
				md_IR_start_time[mi_IR_I] = CheckTime(lv_field);
			}
		}

		lv_identifier = mc_delimiter + "CO-" ;
		lv_field = lt_WH_section.getNthBracketedField
				(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			if ( lv_field.length() == 4 || lv_field.length() == 5 )
			{
				if ( lv_field.For(4).isAllNumeric() )
				{
					md_IR_end_time[mi_IR_I] = lv_field ;
				}
			} else {
				md_IR_end_time[mi_IR_I] = CheckTime(lv_field);
			}
		}

		IrHotelrates(lt_WH_section) ;
		
		IrHotelRemarks(lt_WH_section) ;

		lv_field = lc_itinLine + "H" + md_IR_sup_id[mi_IR_I] 
										+ lv_arrayField[6].At(4).For(5) ;

		mv_IrRentalData[++mi_IrRentalIndex] = lv_field ;

		fillIrconsumDetailOther(md_IR_itin_type[mi_IR_I],
										md_IR_line_number[mi_IR_I],
										lc_status) ;

		getOverrideRates(	md_IR_itin_type[mi_IR_I],
								md_IR_start_date[mi_IR_I],
								md_IR_origin_city_id[mi_IR_I],
								md_IR_saving_code[mi_IR_I],
								md_IR_local_rate[mi_IR_I],
								md_IR_regular_rate[mi_IR_I],
								md_IR_booked_rate[mi_IR_I],
								mv_IrHotelData,
								mi_IrHotelIndex) ;

   	mi_IR_I++ ;
		lt_WH_section = po_PNR.getNextSegment("H")  ;
	}
   EXIT;
}

//---------------------------------------------------------------------------------------------------------------------------

void CTAIR::IrHotelRemarks(AB_Text &pv_remarkField)
{
   ENTER;

   AB_Varchar lv_field(50), lv_remark(50) ;
	AB_Varchar lv_identifier(5) ;


	lv_identifier = mc_delimiter + "RA-" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "ROLLAWAY ADULT : " + lv_field ;
      CreateRentalRemarks(lv_remark) ;
   }

	lv_identifier = mc_delimiter + "RC-" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "ROLLAWAY CHILD : " + lv_field ;
      CreateRentalRemarks(lv_remark) ;
   }

	lv_identifier = mc_delimiter + "CR-" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "CRIB CHARGE : " + lv_field ;
      CreateRentalRemarks(lv_remark) ;
   }

	lv_identifier = mc_delimiter + "EX-" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "EXTRA ADULT : " + lv_field ;
      CreateRentalRemarks(lv_remark) ;
   }

	lv_identifier = mc_delimiter + "RL-" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      if (lv_field == "BF")
         lv_field == "BEACHFRONT" ;
      else if (lv_field == "MV")
         lv_field == "MOUTAIN VIEW" ;
      else if (lv_field == "OF")
         lv_field == "OCEAN FRONT" ;
      else if (lv_field == "PS")
         lv_field == "POOL SIDE" ;
      else if (lv_field == "SV")
         lv_field == "SEA VIEW" ;
      else if (lv_field == "BV")
         lv_field == "BAY VIEW" ;
      else if (lv_field == "GV")
         lv_field == "GARDEN VIEW" ;
      else if (lv_field == "LV")
         lv_field == "LAKE VIEW" ;
      else if (lv_field == "RV")
         lv_field == "RIVER VIEW" ;
      else if (lv_field == "CV")
         lv_field == "COURTYARD VIEW" ;
      else if (lv_field == "DO")
         lv_field == "DOWN AND OUT" ;
      else if (lv_field == "GC")
         lv_field == "GOLF COURSE VIEW" ;

      lv_remark = "TOUR ROOM LOCATION : " + lv_field ;
      CreateRentalRemarks(lv_remark) ;
   }

	lv_identifier = mc_delimiter + "M" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      if (lv_field.For(2) == "AP")
         lv_field = "AMERICAN PLAN" ;
      else if (lv_field.For(2) == "BP")
         lv_field = "BERMUDA PLAN" ;
      else if (lv_field.For(2) == "CB")
         lv_field = "CARIBBEAN PLAN" ;
      else if (lv_field.For(2) == "CP")
         lv_field = "CONTINENTAL PLAN" ;
      else if (lv_field.For(2) == "FP")
         lv_field = "FAMILY PLAN" ;
      else if (lv_field.For(2) == "MA")
         lv_field = "MODIFIED AMERICAN PLAN" ;
		else
			lv_field = AB_null ;

		if ( lv_field.NotIsNull() )
		{
      	lv_remark = "TOUR MEAL PLAN : " + lv_field ;
      	CreateRentalRemarks(lv_remark) ;
		}
   }

	lv_identifier = mc_delimiter + "EC-" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "EXTRA CHILD : " + lv_field ;
      CreateRentalRemarks(lv_remark) ;
   }

/*
	lv_identifier = mc_delimiter + "SI-" ;
   lv_field = pv_remarkField.getNthBracketedField(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   if (lv_field.NotIsNull())
   {
      lv_remark = "SPECIAL INFO : " + lv_field ;
      CreateRentalRemarks(lv_remark) ;
   }
*/

   EXIT;
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::CreateRentalRemarks(AB_Varchar &pv_field)
{
   ENTER;

   md_IRMK_itin_type[mi_IRMK_I]       = md_IR_itin_type[mi_IR_I] ;
   md_IRMK_remark[mi_IRMK_I]          = pv_field ;
   md_IRMK_ren_line_number[mi_IRMK_I] = md_IR_line_number[mi_IR_I] ;
   md_IRMK_pax_id[mi_IRMK_I++]        = AB_null;

   EXIT;
}


//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fillIrCar_I(CPNRWS  &po_PNR)
{
   ENTER;

   int          li_pos,i ;
   AB_Varchar   lv_field(50) ;
	// WS 17 START *******
   AB_Varchar   lv_fieldFM(50) ;
   AB_Varchar   lv_fieldMC(50) ;
	// WS 17 END *******
	V_AB_Varchar lv_arrayField(100,100) ;
	AB_Text		 lt_WI_section ;
	AB_Varchar   lv_identifier(5) ;
	AB_Varchar   lv_ratePlanCode(2) ;
	AB_Varchar   lv_mileageInfo(20) ;
	AB_Char      lc_status(2) ;

	AB_Char      lc_itinLine(2) ;
	D_amount		 ld_amount ;
	AB_Char      lc_year(2) ;


   po_PNR.resetSegment("I") ;
   lt_WI_section = po_PNR.getNextSegment("I")  ;

   while (lt_WI_section.NotIsNull()) 
	{
		i = lt_WI_section.length() ;

		if ( lt_WI_section.At(i).For(1) != mc_delimiter )
			lt_WI_section = lt_WI_section + mc_delimiter ;

		li_pos = lv_arrayField.parseStringMultDel(lt_WI_section,mc_delimiter.to_string());

   	md_IR_itin_type[mi_IR_I]      = "C" ;
   	md_IR_line_number[mi_IR_I]    = lv_arrayField[2] ;

		lc_itinLine                   = md_IR_line_number[mi_IR_I] ;
		lc_itinLine.rightJustify('0') ;


		md_IR_sup_id[mi_IR_I]         = lv_arrayField[4].For(2) + "C" ;
		
   	lc_status                     = lv_arrayField[4].At(3).For(2) ;

		if ( lc_status == "MK" )
			 md_IR_source[mi_IR_I]	= "M";
		else
			md_IR_source[mi_IR_I]   = "A" ;

   	md_IR_service_status[mi_IR_I] = lv_arrayField[4].At(3).For(2) ;

   	md_IR_unit_ct[mi_IR_I]        = lv_arrayField[4].At(5).For(3) ;

		md_IR_origin_city_id[mi_IR_I] = lv_arrayField[4].At(8).For(3) ;

		// Get start and end date

		// SCR 220449 START

		lc_year = mt_lclPnrDate.get_default_year(lv_arrayField[4].At(11).For(5)) ;
		lc_year.rightJustify('0') ;

   	md_IR_start_date[mi_IR_I]     = lv_arrayField[4].At(11).For(5) + lc_year ;

		AB_Date ld_start_date(md_IR_start_date[mi_IR_I]);

		lc_year = ld_start_date.get_default_year(lv_arrayField[4].At(16).For(5)) ;
		lc_year.rightJustify('0') ;
		md_IR_end_date[mi_IR_I]      =  lv_arrayField[4].At(16).For(5) + lc_year ;

		// SCR 220449 END

		lv_identifier = mc_delimiter + "DO-" ;
   	lv_field = lt_WI_section.getNthBracketedField
			(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;

   	if (lv_field.NotIsNull())
		{
			md_IR_dropoff_location[mi_IR_I] = lv_field;

			if ( lv_field.length() == 3 )
			{
      		md_IR_dest_city_id[mi_IR_I] = lv_field ;
			}
			else
			{
				md_IR_dest_city_id[mi_IR_I] = md_IR_origin_city_id[mi_IR_I] ;
			}
		}
   	else
		{
      	md_IR_dest_city_id[mi_IR_I] = md_IR_origin_city_id[mi_IR_I] ;
		}


		md_IR_unit_type[mi_IR_I]      = lv_arrayField[4].At(22).For(4) ;
		
		
		md_IR_pickup_location[mi_IR_I] = md_IR_origin_city_id[mi_IR_I].For(3);
	   lv_field = lt_WI_section.getNthBracketedField(1, "L-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_location[mi_IR_I] = lv_field ;	
			md_IR_pickup_location[mi_IR_I] = md_IR_origin_city_id[mi_IR_I].For(3)  + lv_field;
		}

		lv_identifier = mc_delimiter + "CI-" ;
		lv_field = lt_WI_section.getNthBracketedField
				(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			if ( lv_field.length() == 4 || lv_field.length() == 5 )
			{
				if ( lv_field.For(4).isAllNumeric() )
				{
					md_IR_start_time[mi_IR_I] = lv_field ;
				}
			} else {
				md_IR_start_time[mi_IR_I] = CheckTime(lv_field);
			}
		}

		lv_identifier = mc_delimiter + "CO-" ;
		lv_field = lt_WI_section.getNthBracketedField
				(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			if ( lv_field.length() == 4 || lv_field.length() == 5 )
			{
				if ( lv_field.For(4).isAllNumeric() )
				{
					md_IR_end_time[mi_IR_I] = lv_field ;
				}
			} else {
				md_IR_end_time[mi_IR_I] = CheckTime(lv_field);
			}
		}

	   lv_field = lt_WI_section.getNthBracketedField(1, "DT-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_end_time[mi_IR_I] = CheckTime(lv_field);
		}

	   lv_field = lt_WI_section.getNthBracketedField(1, "DC-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			// WS 17 START *******
			if ( lv_field.findFirstCharNotInList("0123456789.") <= 0 )	
				md_IR_dropoff_charge[mi_IR_I] = lv_field.For(15) ;
			else
			{
   			md_IRMK_remark[mi_IRMK_I]           	= lv_field;
   			md_IRMK_itin_type[mi_IRMK_I]        	= "C" ;
				/* SCR 148638 */
   			md_IRMK_ren_line_number[mi_IRMK_I++]  	= md_IR_line_number[mi_IR_I];
			}

			// WS 17 END *******
		}

	   lv_field = lt_WI_section.getNthBracketedField(1, "CD-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_corporation_id[mi_IR_I] = lv_field ;
		}

	   lv_field = lt_WI_section.getNthBracketedField(1, "FG-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_customer_id[mi_IR_I] = lv_field ;
		}

	   lv_field = lt_WI_section.getNthBracketedField(1, "FT-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_fup[mi_IR_I] = lv_field ;
		}

		lv_identifier = mc_delimiter + "G-" ;
		lv_field = lt_WI_section.getNthBracketedField
				(1, lv_identifier.to_string(), mc_delimiter.to_string()) ;
		
   	if (lv_field.NotIsNull())
		{
			md_IR_guarantee[mi_IR_I] = lv_field ;
		}

	   lv_field = lt_WI_section.getNthBracketedField(1, "CF-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
			md_IR_confirmation[mi_IR_I] = lv_field ;
		}
	
	   lv_field = lt_WI_section.getNthBracketedField(1, "RC-", mc_delimiter.to_string()) ;
   	if (lv_field.NotIsNull())
		{
      	md_IR_rate_code[mi_IR_I] = lv_field ;
		}

		// WS 17 START *******

		lv_field = lt_WI_section.getNthBracketedField(1, "FX-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			lv_field.substitute(" ","") ;
			md_IR_fax_acode[mi_IR_I] = lv_field.For(5) ;
			md_IR_fax_no[mi_IR_I]    = lv_field.At(6).For(10) ;
		}

		lv_field = lt_WI_section.getNthBracketedField(1, "PH-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			lv_field.substitute(" ","") ;
			md_IR_phone_acode[mi_IR_I] = lv_field.For(5) ;
			md_IR_phone_no[mi_IR_I]    = lv_field.At(6).For(7) ;
		}

		lv_field = lt_WI_section.getNthBracketedField(1, "XD-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			lv_field.substitute(" ","") ;

			li_pos = lv_field.findSubstring("FM-"); 		

			if ( li_pos == 0 )
				li_pos = lv_field.findSubstring("MC-");

			if ( li_pos != 0 )
			{
				md_IR_ex_day_charge[mi_IR_I] = lv_field.For(li_pos-1);
			}
			else
			{
				md_IR_ex_day_charge[mi_IR_I] = lv_field;
			}
		}

		// Rate Information

	   lv_field = lt_WI_section.getNthBracketedField(1, "R-", mc_delimiter.to_string()) ;

   	if (lv_field.NotIsNull())
		{
			if ( lv_field.For(1) == "T" || lv_field.For(1) == "G" || lv_field.For(1) == "Q" )
			{
				lv_field.substitute(" ","") ;
				if ( lv_field.At(5).For(3).isAllAlpha() )
				{
					md_IR_cur_id[mi_IR_I] = lv_field.At(5).For(3);
					li_pos = lv_field.At(8).findSubstring("/");
					ld_amount = lv_field.At(8).For(li_pos - 1) ;
					md_IR_booked_rate[mi_IR_I] = ld_amount ;
					md_IR_local_rate[mi_IR_I] = ld_amount ;
					md_IR_regular_rate[mi_IR_I] = ld_amount ;
					lv_ratePlanCode = lv_field.At(8 + li_pos).For(2) ;
				}
				else
				{
					md_IR_cur_id[mi_IR_I] = "USD";
					li_pos = lv_field.At(5).findSubstring("/");
					ld_amount = lv_field.At(5).For(li_pos - 1) ;
					md_IR_booked_rate[mi_IR_I] = ld_amount ;
					md_IR_local_rate[mi_IR_I] = ld_amount ;
					md_IR_regular_rate[mi_IR_I] = ld_amount ;
					lv_ratePlanCode = lv_field.At(5 + li_pos).For(2) ;
				}

				getRateDuration(lv_ratePlanCode) ;
			
	   		lv_fieldFM = lt_WI_section.getNthBracketedField(1, "FM-", mc_delimiter.to_string()) ;
				if ( lv_fieldFM.NotIsNull() )
				{
					lv_fieldFM.substitute(" ","") ;
				}

	   		lv_fieldMC = lt_WI_section.getNthBracketedField(1, "MC-", mc_delimiter.to_string()) ;
				if ( lv_fieldMC.NotIsNull() )
				{
					lv_fieldMC.substitute(" ","") ;
				}

				if ( lv_fieldFM.NotIsNull())
				{
					getMileage(lv_fieldFM, lv_fieldMC) ;
				}
			}
		}

		// WS 17 END *******

		lv_field = lc_itinLine + "C" + md_IR_sup_id[mi_IR_I] 
										+ lv_arrayField[4].At(11).For(5) ;

		mv_IrRentalData[++mi_IrRentalIndex] = lv_field ;

		fillIrconsumDetailOther(md_IR_itin_type[mi_IR_I],
										md_IR_line_number[mi_IR_I],
										lc_status) ;

		getOverrideRates(	md_IR_itin_type[mi_IR_I],
								md_IR_start_date[mi_IR_I],
								md_IR_origin_city_id[mi_IR_I],
								md_IR_saving_code[mi_IR_I],
								md_IR_local_rate[mi_IR_I],
								md_IR_regular_rate[mi_IR_I],
								md_IR_booked_rate[mi_IR_I],
								mv_IrCarData,
								mi_IrCarIndex) ;

   	mi_IR_I++ ;
		lt_WI_section = po_PNR.getNextSegment("I")  ;
	}
   EXIT;
}

//-------------------------------------------------------------------------------------------------
// WS 17 START *******
void CTAIR::getMileage(AB_Varchar &pv_fieldFM, AB_Varchar &pv_fieldMC ) 
{
	int	li_pos;
	AB_Integer li_freeMile ;
	AB_Boolean lb_unlimited;

	lb_unlimited = AB_false;

	li_pos  = pv_fieldFM.findSubstring("MI") ;

	if ( li_pos != 0 ) // FREE MILES INFO
	{
		md_IR_free_mile_code[mi_IR_I] = "M" ;
		li_freeMile =  pv_fieldFM.For(li_pos - 1);
		md_IR_free_mileage[mi_IR_I] = li_freeMile ;
	}
	else // FREE KILOMETERS INFO OR UNLIMITED OR NOTHING
	{
		li_pos  = pv_fieldFM.findSubstring("KM") ;

		if ( li_pos != 0 ) // FREE KILOMETERS INFO
		{
			md_IR_free_mile_code[mi_IR_I] = "K" ;
			li_freeMile =  pv_fieldFM.For(li_pos - 1);
			md_IR_free_mileage[mi_IR_I] = li_freeMile ;
		}
		else // UNLIMITED OR NOTHING
		{
			li_pos = pv_fieldFM.findSubstring("UNL") ;
			if ( li_pos != 0 ) // UNLIMITED
			{
				lb_unlimited = AB_true;
				md_IR_free_mile_code[mi_IR_I] = "M" ;
				md_IR_free_mileage[mi_IR_I] = "UNLIM" ;
			}
			else // NOTHING
			{
				return;
			}
		}
	}

	if ( lb_unlimited == AB_false && pv_fieldMC.NotIsNull() )
	{
		li_pos  = pv_fieldMC.findSubstring("MI") ;

		if ( li_pos == 0 )
			li_pos  = pv_fieldMC.findSubstring("KM") ;

		if ( li_pos != 0 )
		{
			md_IR_ex_mile_charge[mi_IR_I] = pv_fieldMC.For(li_pos - 1);
		}
		else
		{
			md_IR_ex_mile_charge[mi_IR_I] = "0.00" ;
		}
	}
	else
	{
		md_IR_ex_mile_charge[mi_IR_I] = "0.00" ;
	}

}
// WS 17 END *******

//--------------------------------------------------------------------------------------------------
void CTAIR::getRateDuration(AB_Varchar  &pv_field)
{
	if ( pv_field == "DY" )
	{
		md_IR_rate_duration[mi_IR_I] = "D" ;
	}
	else if ( pv_field == "MY" )
	{
		md_IR_rate_duration[mi_IR_I] = "M" ;
	}
	else if ( pv_field == "WD" )
	{
		md_IR_rate_duration[mi_IR_I] = "E" ;
	}
	else if ( pv_field == "WK" )
	{
		md_IR_rate_duration[mi_IR_I] = "W" ;
	}
	else 
	{
		// BR , FK, FM, HR, KM, MI, TL, **
		md_IR_rate_duration[mi_IR_I] = "X" ;
	}
}

//--------------------------------------------------------------------------
void CTAIR::IrHotelrates(AB_Text  &pv_section)
{
	AB_Varchar     lv_field(50);
	D_char_numeric ld_rate;

	// WS 17 START *******
	/**
	 * RS RT RQ RG RT1 RQ1 RG1 are mutually exclusive
	 * TTL is checked if all above rates are not present
	**/
	//WS 17 END *******

	lv_field = pv_section.getNthBracketedField(1, "#RT-", 
															 mc_delimiter.to_string()) ;
	if ( lv_field.IsNull() )
	{
	    lv_field = pv_section.getNthBracketedField(1, "#RG-", 
									  mc_delimiter.to_string()) ;

		 if ( lv_field.IsNull() )
		 {
		     lv_field = pv_section.getNthBracketedField(1, "#RQ-", 
										   mc_delimiter.to_string()) ;

		     if ( lv_field.IsNull() )
			  {
		         lv_field = pv_section.getNthBracketedField(1, "#RS-", 
												 mc_delimiter.to_string()) ;

					// WS 17 START *******
		     		if ( lv_field.IsNull() )
			  		{
						lv_field = pv_section.getNthBracketedField(1, "#RT1-", 
														mc_delimiter.to_string()) ;

						if ( lv_field.IsNull() )
						{
	    					lv_field = pv_section.getNthBracketedField(1, "#RG1-", 
									  					mc_delimiter.to_string()) ;

		 					if ( lv_field.IsNull() )
		 					{
		     					lv_field = pv_section.getNthBracketedField(1, "#RQ1-", 
										   					mc_delimiter.to_string()) ;

		 						if ( lv_field.IsNull() )
		 						{
		     						lv_field = pv_section.getNthBracketedField(1, "#TTL-", 
										   						mc_delimiter.to_string()) ;
									if ( lv_field.IsNull() )
									{
										md_IR_rate_duration[mi_IR_I]  = "X" ;
									}
								}
							}
						}
					}
					// WS 17 END *******
		     }
		 }
   }


   if ( lv_field.NotIsNull() )
	{
		 //lv_field.substitute(" ","") ;

		 if ( lv_field.For(3).isAllAlpha() )
		 {
			  /**
				* Currency code is available
				**/

			  md_IR_cur_id[mi_IR_I] = lv_field.For(3);

			  lv_field = lv_field.At(4);

			  int li_pos = lv_field.findSubstring(" ");

			  if ( li_pos == 0 ) 
			  {
		         lv_field.substitute(" ","") ;
			      ld_rate = lv_field.For(9);
			  }
			  else
			  {
				   ld_rate = lv_field.For(li_pos-1);
			  }
		 }
		 else
		 {
			  /**
				* Default to USD
				**/

			  md_IR_cur_id[mi_IR_I]  = "USD" ;

			  int li_pos = lv_field.findSubstring(" ");

			  if ( li_pos == 0 ) 
			  {
		         lv_field.substitute(" ","") ;
				   ld_rate = lv_field.For(9);
			  }
			  else
			  {
				   ld_rate = lv_field.For(li_pos-1);
			  }
			}
			md_IR_booked_rate[mi_IR_I]  = ld_rate;
		   md_IR_local_rate[mi_IR_I]   = ld_rate;
		   md_IR_regular_rate[mi_IR_I] = ld_rate;
    }
}

//-----------------------------------------------------------------------------

void CTAIR::getFormOfPay(AB_Text 			  	   &pv_fop,
								 D_form_of_payment      &pd_formOfPay ,
								 D_credit_card_sup      &pd_cc_id, 
								 D_credit_card_no       &pd_cc_no,
								 D_expiry_date          &pd_cc_exp_date,
								 D_authorization        &pd_auth_no,
								 D_authorization_source &pd_auth_source) 
{
	ENTER;

	AB_Text  lt_ccNo ;
	AB_Varchar lv_ccExpDate(4);

	if ( pv_fop.NotIsNull() )
	{
		if ( pv_fop.For(2) == "AR" )
		{
			pd_formOfPay = "IV" ;
		}
		else if ( pv_fop.For(2) == "CK" || pv_fop.For(5) == "CHECK" )
		{
			pd_formOfPay = "CK" ;
		}
		else if ( pv_fop.For(2) == "GR" )
		{
			pd_formOfPay     = "CC" ;
			pd_cc_id         = "GR" ;
			pd_cc_no         = pv_fop.At(3).For(18) ;
			pd_cc_exp_date   = AB_null ;
			pd_auth_no       = AB_null ;
			pd_auth_source   = "A" ;
		}
		else if ( pv_fop.For(3) == "MSC" )
		{
			pd_formOfPay     = "CC" ;
			pd_cc_id         = "ZZ" ;
			pd_cc_no         = "999999999999999" ;
			pd_cc_exp_date   = AB_null ;
			pd_auth_no       = AB_null ;
			pd_auth_source   = "M" ;
		}
		else if ( pv_fop.For(2) == "CC" )
		{
			pd_formOfPay     = "CC" ;
			pd_cc_id         = pv_fop.At(3).For(2) ;

			lt_ccNo   = pv_fop.At(5).For(306) ;
			lt_ccNo.substitute(" ","") ;

			// WS 17 START
			pd_cc_exp_date = AB_null ;
			// WS 17 END

			int li_pos       = lt_ccNo.findFirstCharNotInList("0123456789") ;

			if ( li_pos > 0 )
			{
				pd_cc_no  = lt_ccNo.For(li_pos - 1) ;

				// WS 17 START

				if (	lt_ccNo.At(li_pos).For(1) == "N" ||
						lt_ccNo.At(li_pos).For(1) == "P")
				{
			   	lv_ccExpDate = lt_ccNo.At(li_pos+1).For(4);

					if ( lv_ccExpDate.isAllNumeric() ) 
					{
						/**
					 	* Expiration date is available
					 	**/

			      	pd_cc_exp_date  = lv_ccExpDate;
			   	}			
				}
				// WS 17 END

			}
			else
			{
				pd_cc_no  = lt_ccNo ;
			}

			li_pos = pv_fop.findSubstring("/ M:") ;

			if ( li_pos > 0 )
			{
				pd_auth_no = pv_fop.At(li_pos + 4) ;
				li_pos = pd_auth_no.findSubstring(mc_delimiter.to_string()) ;
				if ( li_pos > 0 )
				{
					pd_auth_no = pd_auth_no.For( li_pos -1 ) ;
				}
				pd_auth_source   = " " ;
			}
			else
			{
				li_pos = pv_fop.findSubstring("/  :") ;
				if ( li_pos > 0 )
				{
					pd_auth_no = pv_fop.At(li_pos + 4) ;

					li_pos = pd_auth_no.findSubstring(mc_delimiter.to_string()) ;
					if ( li_pos > 0 )
					{
						pd_auth_no = pd_auth_no.For( li_pos -1 ) ;
					}
					pd_auth_source   = "A" ;
				}
				else
				{
					pd_auth_source   = "A" ;
			   }
			}

			// Extract credit card expiration date 

			// WS 17 START
			// pd_cc_exp_date = AB_null ;

			if ( pd_cc_exp_date.IsNull() )
			{
				li_pos = pv_fop.findSubstring("F1-EXP") ;

				if ( li_pos <= 0 ) 
				{

			    	li_pos = pv_fop.findSubstring("F2-EXP") ;

			    	if ( li_pos <= 0 ) 
					{

			        	li_pos = pv_fop.findSubstring("F3-EXP") ;

			        	if ( li_pos <= 0 ) 
						{

			            li_pos = pv_fop.findSubstring("F4-EXP") ;
                 	}
             	}
         	}

				if ( li_pos > 0 ) 
				{

			   	lv_ccExpDate = pv_fop.At(li_pos+6).For(4);

					if ( lv_ccExpDate.isAllNumeric() ) 
					{
						/**
					 	* Expiration date is available
					 	**/

			      	pd_cc_exp_date  = lv_ccExpDate;
			   	}			
         	}
			}
			// WS 17 END
		}
		else
		{
			pd_formOfPay = "CA" ;
		}
	}
	else
	{
		pd_formOfPay = "CA" ;
	}
} 

//-----------------------------------------------------------------------------

AB_Integer CTAIR::getSvcFop(AB_Text 			  &pv_fop,
							       D_form_of_payment  &pd_formOfPay ,
								    D_credit_card_sup  &pd_cc_id, 
      							 D_credit_card_no   &pd_cc_no,
	      						 D_expiry_date      &pd_cc_exp_date)
{
	AB_Text    lt_ccNo ;
   AB_Integer li_rval;

	li_rval = AB_Integer(0);
	pd_cc_no       = AB_Text(" ");
   pd_cc_id       = AB_Text(" ");
	pd_cc_exp_date = AB_Text(" ");

	if ( pv_fop.NotIsNull() )
	{
		if ( pv_fop.For(2) == "CK" )
		{
			pd_formOfPay = "CK" ;
		}
		else if ( pv_fop.For(2) == "CA" )
		{
			pd_formOfPay = "CA" ;
		}
		else if ( pv_fop.For(2) == "CC" )
		{
			pd_formOfPay  = "CC" ;
			pd_cc_id      = pv_fop.At(3).For(2) ;

			lt_ccNo   = pv_fop.At(5);

			int li_pos = lt_ccNo.findFirstCharNotInList("0123456789") ;

			if ( li_pos > 0 )
			{
				if ( (lt_ccNo.At(li_pos).For(1) == "P" ||
						lt_ccNo.At(li_pos).For(1) == "N" ) &&
					   lt_ccNo.For(li_pos-1).isAllNumeric() &&
					   lt_ccNo.At(li_pos+1).isAllNumeric() )
				{
				  // AX999.....9N1299
				  // AX999.....9P1299

				  pd_cc_no       = lt_ccNo.For(li_pos-1) ;

				  // SCR 206266 START

				  pd_cc_exp_date = lt_ccNo.At(li_pos+1);

				  // SCR 206266 END
				}
            else
				{
				  // Unexpected style

				  li_rval = 1;
				}
			}
			else
			{
				pd_cc_no  = lt_ccNo ;
			}
		}
		else
		{
			// Unexpected style

			li_rval = 1;
		}
	}
	else
	{
		// Unexpected style

		li_rval = 1;
	}
   return li_rval;
} 

//------------------------------------------------------------------------------
void CTAIR::populateIrConsumDetail(CPNRWS  &po_PNR)
{
	ENTER;

	D_pic          ld_prevPtc = AB_null ;
	V_AB_Varchar   lv_arrayField(100,100) ;
	int            li_pos ;
	AB_Text        lt_W4_section ;
	AB_Varchar		lv_fareInfo(15) ;

   int li_counter = 1 ;

   while (li_counter <= mi_passengerWA)
   {
		md_ICD_itin_line_number[mi_ICD_I] = md_IT_line_number[mi_IT_I] ;
		md_ICD_itin_type[mi_ICD_I]        = md_IT_itin_type[mi_IT_I] ;
		md_ICD_ftp_miles[mi_ICD_I]        = mv_segmentMile ;

		md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id [li_counter - 1] ;

		//md_ICD_pax_id[mi_ICD_I]           = mva_passengerWA [li_counter] ;

		if ( md_IT_itin_type[mi_IT_I] == "A" )
		{
			if ( ld_prevPtc.IsNull() )
			{
				ld_prevPtc = mva_passengerWA[li_counter] ;
				md_ICD_fare_basis[mi_ICD_I]       = mv_fareBasis ;
				md_ICD_tkt_desig[mi_ICD_I]        = mv_tktDesig ;
			}
			else
			{
				if ( ld_prevPtc == mva_passengerWA[li_counter] )
				{
					md_ICD_fare_basis[mi_ICD_I]       = mv_fareBasis ;
					md_ICD_tkt_desig[mi_ICD_I]        = mv_tktDesig ;
				}
				else
				{
					lt_W4_section = po_PNR.getNextSegment("04");
					li_pos = lv_arrayField.parseStringMultDel(lt_W4_section,mc_delimiter.to_string()) ;

					lv_fareInfo                 = lv_arrayField[2] ;
					if ( lv_fareInfo != "*" )
					{
						li_pos         = lv_fareInfo.findSubstring("/") ;
						if ( li_pos > 0 )
						{
							md_ICD_fare_basis[mi_ICD_I] = lv_fareInfo.For(li_pos - 1) ;
							md_ICD_tkt_desig[mi_ICD_I]  = lv_fareInfo.At(li_pos + 1) ;
						}
						else
						{
							md_ICD_fare_basis[mi_ICD_I]  = lv_fareInfo ;
							md_ICD_tkt_desig[mi_ICD_I]  = AB_null ;
						}
					}
					else
					{
						md_ICD_fare_basis[mi_ICD_I] = AB_null ;
						md_ICD_tkt_desig[mi_ICD_I]  = AB_null ;
					}
				}
			}
		}
		else
		{
			md_ICD_fare_basis[mi_ICD_I]       = mv_fareBasis ;
			md_ICD_tkt_desig[mi_ICD_I]        = mv_tktDesig ;
		}
		md_ICD_service_status[mi_ICD_I]   = mv_service_status ;
		md_ICD_baggage[mi_ICD_I]          = mv_baggage ;
		md_ICD_connection[mi_ICD_I]       = md_connect ;

		getSeatInfo(li_counter) ;

		mi_ICD_I++ ;
      li_counter++ ;
   }

   EXIT;
}

     
//---------------------------------------------------------------------------------------------------
void CTAIR::shiftConnectInd()
{
	 int j;

	  for (int li_counter =0; li_counter<mi_ICD_I; li_counter++)
	  {
		  if (md_ICD_itin_type[li_counter] =="A")
		  {
			  // consider different passengers
		     for (j=li_counter+1; j<=mi_ICD_I; j++)
		     {
			     if (md_ICD_itin_type[j]=="A")
			     {
			        if (md_ICD_pax_id[li_counter]==md_ICD_pax_id[j])
					  {
		              md_ICD_connection[li_counter] = md_ICD_connection[j];
						  break;          // break the j loop and for next li_count
					  }
			     }
			     else
			     {
		           md_ICD_connection[li_counter] = "O";
					  break;            // break the j loop and for next li_count
			     }
		     }
		  }
	  }
}

//---------------------------------------------------------------------------------------------------
void CTAIR::getSeatInfo(int   &pi_paxCount)
{
	int              li_pos, li_count = 1 ;
	AB_Varchar       lv_paxName(16) ;
	V_AB_Varchar     lv_arrayField(100,100) ;

	while ( li_count < mi_WS_count )
	{

		 li_pos = lv_arrayField.parseStringMultDel
				(mva_WS_section[li_count],mc_delimiter.to_string()) ;

		if ( md_IP_nameNumber[pi_paxCount] == lv_arrayField[3] &&
			  md_ICD_itin_type[mi_ICD_I] == lv_arrayField[2] )
		{
			md_ICD_seat[mi_ICD_I] = lv_arrayField[5].At(9).For(3) ;
		} 

		li_count++ ;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::processTVLSegment()
{
	ENTER;

	AB_Text		 	lt_WT_section ;
	V_AB_Varchar 	lv_arrayField(100,100) ;
	int          	li_pos ;
	int          	li_posDUE ;
	int          	li_posDU  ;
	AB_Boolean		lb_AIR_found;	
	AB_Boolean 		lb_document_exists;

	mb_svcFound = AB_false ;
	lb_AIR_found = AB_false;

	for ( int index = 1; index <= mi_tvlOtherIndex && CMsgHandler::getMsgStatus(); index++ )
	{
		lt_WT_section = mva_tvlOtherSegments[index];


		/**
		 * Check if the record is Service Fee record
		 **/

	   AB_Varchar  lv_fieldAC1(100);
      lv_fieldAC1 = lt_WT_section.getNthBracketedField(1,"AC1-", mc_delimiter.to_string());
	   if ( lv_fieldAC1 == "FEE" )
		{
			// Found Service Fee Record
				processSvcFee(lt_WT_section) ;
		}
		else
		{
			// Rules to pick up different Travel segment

			// 1. If No VC- or no DUE- Ignore the record
			// 2. If VC- but no DUE- reject it in RMA QUEUE
			// 3. If no VC- but DUE- reject it in RMA QUEUE
			// 4. If both VC- and DUE- process it

			li_posDUE  = lt_WT_section.findSubstring("DUE-") ;
			li_posDU   = lt_WT_section.findSubstring("DU-") ;
			lb_document_exists = (li_posDUE > 0 || li_posDU > 0);


	// SCR 286506 START


			if (CMsgHandler::getMsgStatus())
			{
				li_pos = lv_arrayField.parseStringMultDel(lt_WT_section,mc_delimiter.to_string()) ;
	

				if ( 	lv_arrayField[3].At(8).For(3) == "AIR"  	|| 
						lv_arrayField[3].At(8).For(3) == "PRP" )
				{

					// Process Transport  Air
					lb_AIR_found = processTVLTransport(lv_arrayField,lt_WT_section,lb_document_exists, lb_AIR_found) | 
										lb_AIR_found;
				} 
				else if ( li_posDUE > 0 || li_posDU > 0 ) 
				{

					if ( lv_arrayField[3].At(8).For(3) == "BUS" )
					{
						// Process Transport  Bus
						processTVLTransport(lv_arrayField,lt_WT_section,lb_document_exists, lb_AIR_found);
					} 
					else if ( lv_arrayField[3].At(8).For(3) == "TRN" )
					{
						// Process Transport  Rail
						processTVLTransport(lv_arrayField,lt_WT_section,lb_document_exists, lb_AIR_found);
					} 
					else if ( lv_arrayField[3].At(8).For(3) == "HTL" )
					{
						// Process Travel Hotel
						processTVLRental(lv_arrayField,lt_WT_section);
					} 
					else if ( lv_arrayField[3].At(8).For(3) == "CAR" )
					{
						// Process Travel Car
						processTVLRental(lv_arrayField,lt_WT_section);
					} 
					else
					{
						// Process Other, Insurance,Tour,Ship
						processTVLOther(lv_arrayField,lt_WT_section);
					}
				}
			} 

	// SCR 286506 END

		}
	}

	for ( int index = 1; index <= mi_tvlTKTIndex && CMsgHandler::getMsgStatus(); index++ )
	{
		lt_WT_section = mva_tvlTKTSegments[index];


		li_posDUE  = lt_WT_section.findSubstring("DUE-") ;
		li_posDU   = lt_WT_section.findSubstring("DU-") ;
		lb_document_exists = (li_posDUE > 0 || li_posDU > 0);

		if (CMsgHandler::getMsgStatus())
		{
			li_pos = lv_arrayField.parseStringMultDel(lt_WT_section,mc_delimiter.to_string());

			if ( 	lv_arrayField[3].At(8).For(3) == "TKT" )
			{
				// Process Transport  Air
				processTVLTransport(lv_arrayField,lt_WT_section,lb_document_exists, lb_AIR_found);
			} 
		}
	}

   EXIT;
}

//---------------------------------------------------------------------------------------------------

void CTAIR::getRemarkFields(CPNRWS   &po_PNR)
{
	AB_Text		lt_WN_section ;
	AB_Varchar  lv_data(90) ;
	int         li_dash_pos ;
	AB_Varchar  lv_udidNo(4) ;
	int         li_index = 1 ;
	AB_Text		lt_temp;
	AB_Boolean	lb_confFound = AB_false;
   AB_Char     lm_CONFsup(3) ;
   AB_Char     lm_testCONFsup(3) ;
   int         li_slashPos, li_counter;
	AB_Boolean lb_tvtFound = AB_false ;

	AB_Text     lt_segment ;
	V_AB_Varchar lv_arrayField(100,100) ;

	int			trans_count = 19;
	char*			orig_string[]={	
											"@A@",	/* 1 */
											"@AP@",	/* 2 */
											"@C@",	/* 3 */
											"@M@",	/* 4 */
											"@E@",	/* 5 */
											"@X@",	/* 6 */
											"@GT@",	/* 7 */
											"@LC@",	/* 8 */
											"@LB@",	/* 9 */
											"@RB@",	/* 10 */
											"@R@",	/* 11 */
											"@P@",	/* 12 */
											"@PL@",	/* 13 */
											"@H@",	/* 14 */
											"@Q@",	/* 15 */
											"@QO@",	/* 16 */
											"@SC@",	/* 17 */
											"@T@",	/* 18 */
											"@U@"		/* 19 */
										};
	
	char*			trans_string[]={	
											"&",	/* 1 */
											"'",	/* 2 */
											":",	/* 3 */
											",",	/* 4 */
											"=",	/* 5 */
											"!",	/* 6 */
											">",	/* 7 */
											"{",	/* 8 */
											"[",	/* 9 */
											"]",	/* 10 */
											"}",	/* 11 */
											"%",	/* 12 */
											"+",	/* 13 */
											"#",	/* 14 */
											"?",	/* 15 */
											"\"",	/* 16 */
											";",	/* 17 */
											"~",	/* 18 */
											"_"	/* 19 */
										};
	
   // TKX CC EXP DATE START
   AB_Char lc_ccExpDate(4);
   md_TKX_cc_exp_date = AB_null;
   // TKX CC EXP DATE END
	
	AB_Boolean 		lb_pr_found = AB_false;
	AB_Boolean 		lb_fop_found = AB_false;

   po_PNR.resetSegment("N") ;
	lt_segment = po_PNR.getNextSegment("N")  ;

	int li_pos        = lt_segment.findSubstring(".X /") ;

	if ( li_pos > 0 )
	{
		lt_WN_section = lt_segment.At(li_pos) + mc_delimiter ;
	}
	else
	{
		lt_WN_section = AB_null ;
	}

   if (lt_WN_section.NotIsNull())
   {
	   lv_data = lt_WN_section.getNthBracketedField(1, "CF/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
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

		/* 11JUL00 -CA- SCR 2333 replace EB with -EB */
	   lv_data = lt_WN_section.getNthBracketedField(1, "-EB/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
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

		int li = lt_WN_section.findSubstring(".X /-NUC/");

		if (li > 0)
		{

			md_IRMK_type[mi_IRMK_I]     = "NUC";
			md_IRMK_remark[mi_IRMK_I]   = "Y";
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}

	   lv_data = lt_WN_section.getNthBracketedField(1, "PR/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			 if ( !lb_pr_found )
			 {
				 md_IRMK_type[mi_IRMK_I]     = "PR" ;
				 md_IRMK_remark[mi_IRMK_I]   = lv_data;
				 md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
				 lb_pr_found = AB_true;
			 }
		}

	   lv_data = lt_WN_section.getNthBracketedField(1, "FOP/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull() && !lb_fop_found)
		{
			 lv_data.removeLeadAndTrailBlanks();

			 if ( lv_data == "AP" )
			 {
				 md_IRMK_type[mi_IRMK_I]     = "FOP" ;
				 md_IRMK_remark[mi_IRMK_I]   = lv_data;
				 md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
				 lb_fop_found = AB_true;
			 }
		}

	   lv_data = lt_WN_section.getNthBracketedField(1, "TV/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
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

	   lv_data = lt_WN_section.getNthBracketedField(1, "CN/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_IRT_con_id.IsNull() && lv_data.For(3) != "   " )
         	md_IRT_con_id = lv_data.For(3) ;
		}
		/*
	   lv_data = lt_WN_section.getNthBracketedField(1, "DE/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_IRT_city_id.IsNull() && lv_data.For(3) != "   " )
			{
         	md_IRT_city_id = lv_data.For(3) ;
			}
		}
		*/
	   lv_data = lt_WN_section.getNthBracketedField(1, "DP/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_IRT_cfa_dpt.IsNull() && lv_data.For(4) != "    " )
			{
         	md_IRT_cfa_dpt = lv_data.For(4) ;
			}
		}

	   lv_data = lt_WN_section.getNthBracketedField(1, "PH/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
       	mv_phoneField = lv_data ;
		}

	   lv_data = lt_WN_section.getNthBracketedField(1, "RC/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_IRT_iata_dest.IsNull() && lv_data.For(2) != "  " )
			{
         	md_IRT_iata_dest = lv_data.For(2) ;
			}
		}
	   lv_data = lt_WN_section.getNthBracketedField(1, "ML/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_IRT_mktg_list.IsNull() && lv_data.For(1) != " " )
			{
         	md_IRT_mktg_list = lv_data.For(1) ;
			}
		}
	   lv_data = lt_WN_section.getNthBracketedField(1, "FS/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_savingCode.IsNull() ) {
				lv_data.removeLeadAndTrailBlanks();
				md_savingCode = lv_data;
			}
		}
	   lv_data = lt_WN_section.getNthBracketedField(1, "FF/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_fullFare.IsNull() && lv_data.For(10) != "          " )
			{
         	md_fullFare      = lv_data.For(10) ;
			}
		}
	   lv_data = lt_WN_section.getNthBracketedField(1, "LP/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_lowPracFare.IsNull() && lv_data.For(10) != "          " )
			{
         	md_lowPracFare  = lv_data.For(10) ;
			}
		}
	   lv_data = lt_WN_section.getNthBracketedField(1, "RB/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_rebate.IsNull() && lv_data.For(10) != "          " )
			{
         	md_rebate    = lv_data.For(10) ;
			}
		}

		// Hotel Override
		li_index = 1 ;
		while ( li_index > 0 )
		{
	   	lv_data = lt_WN_section.getNthBracketedField(li_index, "-HS", mc_delimiter.to_string()) ;
   		if (lv_data.NotIsNull())
			{
				lt_temp=lv_data;
				lt_temp.removeLeadingBlanks();
				if(lt_temp.length()>0)
         	   mv_IrHotelData[mi_IrHotelIndex++] = lv_data ;
			}
			else
			{
				break ;
			}
			li_index++ ;
		}

		//Car Override
		li_index  = 1 ;
		while ( li_index > 0 )
		{
	   	lv_data = lt_WN_section.getNthBracketedField(li_index, "-CS", mc_delimiter.to_string()) ;
   		if (lv_data.NotIsNull())
			{
				lt_temp=lv_data;
				lt_temp.removeLeadingBlanks();
				if(lt_temp.length()>0)
         	   mv_IrCarData[mi_IrCarIndex++] = lv_data ;
			}
			else
			{
				break ;
			}

			li_index++ ;
		}

	   lv_data = lt_WN_section.getNthBracketedField(1, "BB/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( md_IRT_cop_id.IsNull() )
			{
				if(lv_data.For(2) != "  ")
	   			md_IRT_cop_id = lv_data.For(2) ;
				if(lv_data.At(3)  != "    ")
				   md_IRT_bra_id = lv_data.At(3) ;
			}
		}

	   lv_data = lt_WN_section.getNthBracketedField(1, "-AI/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
			if ( lv_data == "N" )
			{
				md_IRMK_type[mi_IRMK_I]            = "NAI" ;
				md_IRMK_remark[mi_IRMK_I]          = lv_data;
				md_IRMK_pax_id[mi_IRMK_I++]        = AB_null;
			}
		}

		int index = 1;
	   lv_data = lt_WN_section.getNthBracketedField(index++, "-EM/", mc_delimiter.to_string()) ;
		while (lv_data.NotIsNull())
		{
			li_slashPos  = lv_data.findSubstring("/");
			AB_Char email(60);
			AB_Char email_type(10);

			if ( li_slashPos > 0 )
			{
				email = lv_data.For(li_slashPos-1);
				email_type = lv_data.At(li_slashPos+1);
				if (email_type != "TO" && email_type != "CC" && email_type != "BCC") {
					email_type = "TO";
				}
			}
			else
			{
				email = lv_data;
				email_type = "TO";
			}

			for (int trans_index = 0; trans_index < trans_count; trans_index++)
			{
				email.substitute(orig_string[trans_index], trans_string[trans_index]);
			}

			if ( ! email.Like("_%@_%") ) 
			{
				CMsgHandler::setMsg(3951, " ", "EM-PARSE", 1, AB_Varchar(20,AB_null));
				return;
			}

			md_IRMK_type[mi_IRMK_I]            = "AEM" ;
			md_IRMK_remark[mi_IRMK_I]          = email + email_type;
			md_IRMK_pax_id[mi_IRMK_I++]        = AB_null;

	   	lv_data = lt_WN_section.getNthBracketedField(index++, "-EM/", mc_delimiter.to_string()) ;
		}

		lv_data = lt_WN_section.getNthBracketedField(1, "-PID/", mc_delimiter.to_string()) ;
		if (lv_data.NotIsNull())
		{
			lv_data.removeTrailingBlanks() ;
			if (lv_data .isAllNumeric())
			{
				md_IRMK_type[mi_IRMK_I]     = "PID" ;
				md_IRMK_remark[mi_IRMK_I]   = lv_data ;
				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}
		}

  		// TKX CC EXP DATE START

		lv_data = lt_WN_section.getNthBracketedField(1, "-EXP/", mc_delimiter.to_string()) ;
		if (lv_data.NotIsNull())
		{
      	if ( md_TKX_cc_exp_date.IsNull() )
      	{
      		lc_ccExpDate = lv_data.For(4);

      		if ( lc_ccExpDate.isAllNumeric() )
      		{
      			md_TKX_cc_exp_date = lc_ccExpDate;
      		}
      	}
		}

  		// TKX CC EXP DATE END

		// UDID Processing

		li_index = 1 ;
		while ( li_index > 0 )
		{
	   	lv_data = lt_WN_section.getNthBracketedField(li_index, "-U", mc_delimiter.to_string()) ;
   		if (lv_data.NotIsNull())
			{
				li_dash_pos = lv_data.findSubstring("-") ;

				if ( li_dash_pos == 2 || li_dash_pos == 3 )
				{
					lv_udidNo   = lv_data.For(li_dash_pos-1) ;

     				if (lv_udidNo.findFirstCharNotInList("0123456789") == 0)
					{
						if ( 	lv_data.At(li_dash_pos+1).For(1) != " " &&
								lv_udidNo != "0" && lv_udidNo != "00" )
						{
						    createUDIDifRange1to99(lv_udidNo, lv_data.At(li_dash_pos+1));
						}
					}
				}
			}
			else
			{
				break ;
			}
			li_index++ ;
		}

      lv_data = lt_WN_section.getNthBracketedField(1, "CONF/", mc_delimiter.to_string()) ;
      if (lv_data.NotIsNull())
      {
         lb_confFound = AB_false ;

         lv_data.removeLeadingBlanks();
         li_slashPos  = lv_data.findSubstring("/");

         if ( li_slashPos > 1 && li_slashPos < 5 )
         {
            lm_CONFsup   = lv_data.For(li_slashPos-1);

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

            if ( !lb_confFound )
            {

               lv_data  = lv_data.At(li_slashPos+1).For(20);

               lt_temp = lv_data;
               lt_temp.removeLeadingBlanks();
               lt_temp.removeTrailingBlanks();


               if(lt_temp.length()>0)
               {
                  md_IRMK_type[mi_IRMK_I]       = "CNF" ;
                  md_IRMK_pax_id[mi_IRMK_I]   = AB_null;
                  md_IRMK_remark[mi_IRMK_I++]  = "SUP:" + lm_CONFsup + "/" + lv_data ;
               }
            }
         }
      }

		// GAP 9090F Start: POS fee
		/*
		** skip fee with format: .Z /-FEE/.........................
		** skip fee with format: .Z 10NOV/FEE/.........................
		*/
		AB_Text lt_WN_section_fee;

		lt_WN_section_fee = lt_WN_section;

		int li_fee_position = lt_WN_section_fee.findSubstring("FEE/");

		while( li_fee_position > 0 ) {

			if( lt_WN_section_fee.At(li_fee_position-6).For(6) != ".Z  /-" &&
				lt_WN_section_fee.At(li_fee_position-5).For(5) != ".Z /-" &&
				lt_WN_section_fee.At(li_fee_position-4).For(4) != ".Z/-" &&

				lt_WN_section_fee.At(li_fee_position-9).For(3) != ".Z " &&
				lt_WN_section_fee.At(li_fee_position-1).For(4) != "/FEE/ "
				)
			{
				int index_fee = 1;
				lv_data = lt_WN_section_fee.getNthBracketedField(index_fee, "FEE/", mc_delimiter.to_string()) ;

				if ( lv_data.NotIsNull() ) {
					lv_data.removeTrailingBlanks() ;
					lv_data = lv_data + "/";

					AB_Varchar lv_cc_id(40);
      			lv_cc_id = lv_data.getNthBracketedField(1, "FOP-CC", "/") ;
					if (lv_cc_id.NotIsNull() && !validCreditCardNumber(lv_cc_id)) {
						lv_data = "BADCC/" + lv_data;
					}

					md_IRMK_type[mi_IRMK_I]     = "FEE" ;
					md_IRMK_remark[mi_IRMK_I]   = lv_data ;
					md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
				}
			}

			lt_WN_section_fee = lt_WN_section_fee.At(li_fee_position + 4);

			li_fee_position = lt_WN_section_fee.findSubstring("FEE/");
		}
/*
		int index_fee = 1;
      lv_data = lt_WN_section.getNthBracketedField(index_fee, "FEE/", mc_delimiter.to_string()) ;
		while (lv_data.NotIsNull())
		{
			lv_data.removeTrailingBlanks() ;
			lv_data = lv_data + "/";

			AB_Varchar lv_cc_id(40);
      	lv_cc_id = lv_data.getNthBracketedField(1, "FOP-CC", "/") ;
			if (lv_cc_id.NotIsNull() && !validCreditCardNumber(lv_cc_id)) {
				lv_data = "BADCC/" + lv_data;
			}

			md_IRMK_type[mi_IRMK_I]     = "FEE" ;
			md_IRMK_remark[mi_IRMK_I]   = lv_data ;
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null;

      	lv_data = lt_WN_section.getNthBracketedField(++index_fee, "FEE/", mc_delimiter.to_string()) ;
		}
*/
		// GAP 9090F End

		// GAP 9121 Start: AOF fee
      lv_data = lt_WN_section.getNthBracketedField(1, "AOF/", mc_delimiter.to_string()) ;
		if (lv_data.NotIsNull())
		{
			md_IRMK_type[mi_IRMK_I]     = "AOF" ;
		  	md_IRMK_remark[mi_IRMK_I]   = " ";
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9121 End

		// GAP 9090j Start
	   lv_data = lt_WN_section.getNthBracketedField(1, "IC/", mc_delimiter.to_string()) ;
   	if (lv_data.NotIsNull())
		{
		  	md_IRMK_type[mi_IRMK_I]     = "IC" ;
		  	md_IRMK_remark[mi_IRMK_I]   = " ";
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
		}
		// GAP 9090j End
		// GAP 9090K start
		int index_rp;
		index_rp = 1;
	   lv_data = lt_WN_section.getNthBracketedField(index_rp++, "RP/", mc_delimiter.to_string()) ;
		while(lv_data.NotIsNull())
		{
		  	md_IRMK_type[mi_IRMK_I]     = "NTE" ;
		  	md_IRMK_remark[mi_IRMK_I]   = lv_data.For(70);
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;

	   	lv_data = lt_WN_section.getNthBracketedField(index_rp++, "RP/", mc_delimiter.to_string()) ;
		}
		// GAP 9090K end

		// GAP 9090m Start
		int irp_cnt = 1;
      lv_data = lt_WN_section.getNthBracketedField(irp_cnt, "IRP", mc_delimiter.to_string()) ;
		while (lv_data.NotIsNull())
		{
			li_pos = lv_data.findSubstring("/");

			if (li_pos > 1 && lv_data.For(li_pos-1).isAllNumeric()) {
				lv_data.removeTrailingBlanks() ;

				md_IRMK_type[mi_IRMK_I]     = "IRP" ;
				md_IRMK_remark[mi_IRMK_I]   = lv_data ;
				md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
			}

			irp_cnt++;
      	lv_data = lt_WN_section.getNthBracketedField(irp_cnt, "IRP", mc_delimiter.to_string()) ;
		}
		// GAP 9090m End

		// GAP 9090O
		int index_receipt;
		index_receipt = 1;
		lv_data = lt_WN_section.getNthBracketedField(index_receipt++, "-R", mc_delimiter.to_string()) ;
		while (lv_data.NotIsNull())
		{
				int li_slash_pos;
				AB_Varchar lc_delimiter(2) ;
		   	AB_Boolean 	lb_found ;
				li_slash_pos = lv_data.findSubstring("/") ;

				if ( li_slash_pos == 2 || li_slash_pos == 3 )
				{
					lc_delimiter   = lv_data.For(li_slash_pos-1) ;
					lv_data			= lv_data.At(li_slash_pos + 1 );

					if (lc_delimiter.isAllNumeric() )
					{
		   			lb_found = AB_false;

						/* search for matching receipt number */
						for ( int li_ctr = 1; li_ctr <= mi_receiptIndex && !lb_found; li_ctr++ )
						{

							if ( lc_delimiter == gd_receiptType[li_ctr] )
							{

								gd_receiptData[li_ctr] = gd_receiptData[li_ctr]
														+ " " + lv_data;
								lb_found = AB_true;
							}
						}
					}
				}

				if ( !lb_found )
				{
					gd_receiptType[++mi_receiptIndex] = lc_delimiter;
					gd_receiptData[mi_receiptIndex]   = lv_data;
				}
			lv_data = lt_WN_section.getNthBracketedField(index_receipt++, "-R", mc_delimiter.to_string()) ;

		}

		// GAP 9106 start
		int index_fn;
		index_fn = 1;
	   lv_data = lt_WN_section.getNthBracketedField(index_fn++, "FN/", mc_delimiter.to_string()) ;
		while(lv_data.NotIsNull())
		{
		  	md_IRMK_type[mi_IRMK_I]     = "FN" ;
		  	md_IRMK_remark[mi_IRMK_I]   = lv_data.For(13);
		  	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;

	   	lv_data = lt_WN_section.getNthBracketedField(index_fn++, "FN/", mc_delimiter.to_string()) ;
		}
		// GAP 9106 end

	}

   // SCR 281836 START
   li_pos = lt_segment.findSubstring("#-") ;
   if (li_pos > 0) {
      lt_WN_section = lt_segment.At(li_pos);
      li_index = lv_arrayField.parseStringMultDel(lt_WN_section, "#");
      for (int i = 1; i <= li_index; i++) {
         if (lv_arrayField[i].For(1) == "-" &&
             lv_arrayField[i].findSubstring(" ACKN ") != 0) {
            mvo_n_sec_airline_locator.push_back(lv_arrayField[i].At(2));
         }
      }
   }
   // SCR 281836 END

   // SCR-437808-JJ-13OCT2009
   setCopIdByIata();
}

//---------------------------------------------------------------------------------------------------------------------------

void CTAIR::fillIrAmtrak(CPNRWS  &po_PNR)
{

   ENTER;

	AB_Text		  lt_W05_section ;
	V_AB_Varchar  lv_arrayField(100,100) ;
	int			  li_pos ;
	D_char_date   ld_arrDate ; 
	AB_Date       ld_tempDate1 ; 
	AB_Date       ld_tempDate2 ; 
	AB_Integer    li_arrInd ;
	AB_Char       lc_itinLine(2) ;
	AB_Char       lv_field(100) ;
	AB_Char       lc_year(2) ;

	
   po_PNR.resetSegment("05") ;
   lt_W05_section = po_PNR.getNextSegment("05")  ;

   while (lt_W05_section.NotIsNull()) 
	{
		
		li_pos = lv_arrayField.parseStringMultDel(lt_W05_section,mc_delimiter.to_string());

		md_IT_itin_type[mi_IT_I] = "R" ;

		md_IT_line_number[mi_IT_I]    = lv_arrayField[2] ;

		md_IT_no_of_stops[mi_IT_I]    = lv_arrayField[3].For(1) ;
		if ( md_IT_no_of_stops[mi_IT_I] == "*" )
			md_IT_no_of_stops[mi_IT_I] = 0 ;

		md_IT_sup_id[mi_IT_I]         = lv_arrayField[3].At(2).For(2) ;
		md_IT_service[mi_IT_I]        = lv_arrayField[3].At(4).For(4) ;

		md_IT_classofservice[mi_IT_I] = lv_arrayField[4] ;

		md_IT_orig_city_id[mi_IT_I]   = lv_arrayField[6].For(3) ;

		lc_year = mt_pnrDate.get_default_year(lv_arrayField[6].At(4).For(5)) ;
		lc_year.rightJustify('0') ;
		md_IT_start_date[mi_IT_I]     = lv_arrayField[6].At(4).For(5) + lc_year ;

		md_IT_start_time[mi_IT_I]     = CheckTime(lv_arrayField[6].At(9).For(5));

		md_IT_dest_city_id[mi_IT_I]   = lv_arrayField[7].For(3) ;

		lc_year = mt_pnrDate.get_default_year(lv_arrayField[7].At(4).For(5)) ;

		lc_year.rightJustify('0') ;
		ld_arrDate                    = lv_arrayField[7].At(4).For(5) + lc_year ;

		md_IT_end_time[mi_IT_I]       = CheckTime(lv_arrayField[7].At(9).For(5));

		ld_tempDate1                  = md_IT_start_date[mi_IT_I] ;
		ld_tempDate2                  = ld_arrDate ;

		// li_arrInd = ld_tempDate1.datediff(ld_tempDate2,"DD") ;
		li_arrInd = ld_tempDate2 - ld_tempDate1 ; 

		li_arrInd = li_arrInd / AB_Integer(86400) ;

		switch(li_arrInd.to_long())
		{
			case 0:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(0) ;
				break ;
			case 1:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(1) ;
				break ;
			case 2:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(2) ;
				break ;
			case 3:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(3) ;
				break ;
			case -1:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(4) ;
				break ;
			case -2:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(5) ;
				break ;
			default:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(0) ;
            break ;

		}

		
		if ( lv_arrayField[10] == "*" )
			mv_baggage = AB_null ;
		else
			mv_baggage = lv_arrayField[10] ;
		
		mv_service_status = lv_arrayField[11].For(2) ;

		md_IT_meal[mi_IT_I] = lv_arrayField[11].At(3).For(1) ;

		if ( lv_arrayField[13].NotIsNull() )
		{
         md_IRMK_remark[mi_IRMK_I]          = lv_arrayField[13] ;
         md_IRMK_itin_type[mi_IRMK_I]       = "R" ;
         md_IRMK_tra_line_number[mi_IRMK_I] = md_IT_line_number[mi_IT_I] ;
         md_IRMK_pax_id[mi_IRMK_I++]        = AB_null ;
		}

		mv_fareBasis                = AB_null ;
		mv_tktDesig                 = AB_null ;
	
		md_IT_reference[mi_IT_I]    = md_IRT_reference ;

		lc_itinLine                 = lv_arrayField[2] ;
		lc_itinLine.rightJustify('0') ;

		if ( lv_arrayField[11].At(4).For(1) != "*" )
      {
         md_connect = lv_arrayField[11].At(4).For(1) ;
      }
      else
      {
         md_connect = AB_null ;
      }


		// fill Irconsumdetail
		populateIrConsumDetail(po_PNR) ;


		lv_field = lc_itinLine + "R" + md_IT_sup_id[mi_IT_I] 
										+ lv_arrayField[6].At(4).For(5) ;

		mv_IrTransData[++mi_IrTransIndex] = lv_field ;

	
    	mi_IT_I++ ;
   	lt_W05_section = po_PNR.getNextSegment("05")  ;
	}
   EXIT;
}

//----------------------------------------------------------------------------------------------------------
void CTAIR::processTVLRental(V_AB_Varchar    &pv_arrayField,
								  	  AB_Text         &pv_WT_section )
{
	ENTER;

	D_char_date   ld_arrDate ; 
	AB_Date       ld_tempDate1 ; 
	AB_Date       ld_tempDate2 ; 
	AB_Integer    li_arrInd ;
	AB_Char       lc_itinLine(2) ;
	AB_Integer    li_itinLine ;
	AB_Varchar    lv_field(100) ;
	AB_Char       lc_dateStart(5) ;
	AB_Char       lc_dateEnd(5) ;
	AB_Char       lc_startYear(5) ;
	AB_Char       lc_endYear(5) ;
	AB_Varchar    lv_rental(60) ;
	AB_Boolean    lb_exist ;
	AB_Char       lc_status(2) ;
	AB_Char       lc_year(2) ;

	lc_startYear = AB_null ;
	lc_endYear   = AB_null ;
	lb_exist = AB_false ;
	lv_rental    = AB_null ;

	if ( pv_arrayField[3].At(8).For(3) == "HTL" )
	{
		md_IR_itin_type[mi_IR_I]      = "H" ;
		md_IR_sup_id[mi_IR_I]         = pv_arrayField[3].For(2) + "H" ;
	}
	else
	{
		md_IR_itin_type[mi_IR_I]      = "C" ;
		md_IR_sup_id[mi_IR_I]         = pv_arrayField[3].For(2) + "C" ;
	}


	md_IR_line_number[mi_IR_I]    = pv_arrayField[2] ;

	lc_dateStart                  = pv_arrayField[3].At(11).For(5) ;
	lc_itinLine = md_IR_line_number[mi_IR_I] ;
	lc_itinLine.rightJustify('0') ;

	lv_rental = lc_itinLine + md_IR_itin_type[mi_IR_I] + 
					md_IR_sup_id[mi_IR_I] + lc_dateStart ;

	lb_exist = checkHotelSegment(lv_rental,li_itinLine) ;

	if ( !lb_exist )
	{

		li_itinLine                   = md_IR_line_number[mi_IR_I] ;

		md_IR_unit_type[mi_IR_I]      =  pv_arrayField[3].At(8).For(3) ;

		lv_field = pv_WT_section.getNthBracketedField
					(1, "CC1", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IR_origin_city_id[mi_IR_I]   = lv_field.At(2) ;
		}
		else
		{
			md_IR_origin_city_id[mi_IR_I]   = "ZZZ" ;
		}

		lv_field = pv_WT_section.getNthBracketedField
					(1, "CC2", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IR_dest_city_id[mi_IR_I]   = lv_field.At(2) ;
	
			if (  md_IR_origin_city_id[mi_IR_I] == "ZZZ")
			{
		 		md_IR_origin_city_id[mi_IR_I]   = md_IR_dest_city_id[mi_IR_I] ;
			}
		}
		else
		{
			if ( md_IR_origin_city_id[mi_IR_I] != "ZZZ" )
			{
				md_IR_dest_city_id[mi_IR_I] = md_IR_origin_city_id[mi_IR_I] ;
			}
			else
			{
				md_IR_dest_city_id[mi_IR_I]   = "ZZZ" ;
			}
		}

		lc_dateEnd                    = pv_arrayField[3].At(16).For(5) ;

		if (lc_dateEnd == "     ")
		{
			lc_dateEnd = lc_dateStart ;
		}

		lv_field = pv_WT_section.getNthBracketedField
					(1, "Y1-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			lc_startYear   = lv_field ;
		}
		else
		{
			lc_startYear   = AB_null ;
		}
	
		lv_field = pv_WT_section.getNthBracketedField
					(1, "Y2-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			lc_endYear   = lv_field ;
		}
		else
		{
			lc_endYear   = AB_null ;
		}
	
		if ( lc_dateStart != "     " )
		{
			if ( lc_startYear.IsNull() )
			{
				lc_year   = mt_pnrDate.get_default_year(lc_dateStart) ;
				lc_year.rightJustify('0') ;

				md_IR_start_date[mi_IR_I]  = lc_dateStart + lc_year ;
			}
			else
			{

				md_IR_start_date[mi_IR_I]  = lc_dateStart + lc_startYear.At(3).For(2) ;
			}
		}
		else
		{
			md_IR_start_date[mi_IR_I]  = mt_pnrDate ;
		}

		if ( lc_dateEnd != "     " )
		{
			if ( lc_endYear.IsNull() )
			{
				lc_year   = mt_pnrDate.get_default_year(lc_dateEnd) ;
				lc_year.rightJustify('0') ;
				md_IR_end_date[mi_IR_I] = 	lc_dateEnd + lc_year ;
			}
			else
			{
				md_IR_end_date[mi_IR_I]  = lc_dateEnd + lc_endYear.At(3).For(2) ;
			}
		}
		else
		{
				md_IR_end_date[mi_IR_I] = 	mt_pnrDate ;
		}

		md_IR_service_status[mi_IR_I] = pv_arrayField[3].At(3).For(2) ;
		lc_status = md_IR_service_status[mi_IR_I] ;

		md_IR_reference[mi_IR_I]      = md_IRT_reference ;
   	md_IR_unit_ct[mi_IR_I]        = pv_arrayField[3].At(5).For(3) ;
		md_IR_source[mi_IR_I]         = "M" ;

      lv_field = pv_WT_section.getNthBracketedField
							(1, "CF-", mc_delimiter.to_string()) ;

      if (lv_field.NotIsNull())
      {
         md_IR_confirmation[mi_IR_I] = lv_field ;
      }

      md_IR_rate_duration[mi_IR_I]  = "D" ;


		fillIrconsumDetailOther(md_IR_itin_type[mi_IR_I],
									md_IR_line_number[mi_IR_I],
									lc_status) ;
  		mi_IR_I++ ;

	}

	processTVLDoc(pv_WT_section) ;

	updIrconsumdetailTVL(md_IR_itin_type[mi_IR_I-1],li_itinLine) ;
   EXIT;
}

//-----------------------------------------------------------------------------------------------

void CTAIR::processTVLDoc(AB_Text	&pv_WT_section)
{
	ENTER;

	int li_posDUE  = pv_WT_section.findSubstring("DUE-") ;
	int li_posDU   = pv_WT_section.findSubstring("DU-") ;

	if ( li_posDUE == 0 && li_posDU == 0 )
		return;

	AB_Varchar	lv_field(100), lv_amount(12) ; 
	D_amount    ld_cost, ld_total ;
	AB_Varchar  lv_fp(100) ;

	md_ID_line_number[mi_ID_I]   = mi_tktCounter++ ;

  	md_ID_pax_id[mi_ID_I]        = md_IP_pax_id[0] ;


   lv_field = pv_WT_section.getNthBracketedField
				(1, "VC-", mc_delimiter.to_string()) ;

	if (lv_field.NotIsNull())
	{
		md_ID_sup_id[mi_ID_I] = lv_field ;
	}

	lv_field = pv_WT_section.getNthBracketedField
            (1, "DOC-", mc_delimiter.to_string()) ;

	if (lv_field.NotIsNull())
	{
		md_ID_ticket_no[mi_ID_I] = lv_field ;
	}
	else
	{
		/* change as per scr#2788
      ** when ticket_no is already 999999999 then retain ticket_no
      ** but to identify non-arc tickets, pass 000000000
      */

		md_ID_ticket_no[mi_ID_I] = "0000000000" ;
	}

	md_ID_doc_type[mi_ID_I] = "TKT" ;

	lv_field = pv_WT_section.getNthBracketedField
            (1, "DUE-", mc_delimiter.to_string()) ;

	if (lv_field.IsNull())
   {
	    lv_field = pv_WT_section.getNthBracketedField
                  (1, "DU-", mc_delimiter.to_string()) ;
	}

	if (lv_field.NotIsNull())
	{
		md_ID_cur_id[mi_ID_I]      = lv_field.For(3) ;
		if ( md_ID_cur_id[mi_ID_I] == "   " )
		{
			md_ID_cur_id[mi_ID_I] = "USD" ;
		}

		lv_amount = lv_field.At(4).getNthBracketedField(1,"B","/") ;
		if ( lv_amount.NotIsNull() )	
		{
			ld_cost =  lv_amount ;
			md_ID_base_cost[mi_ID_I] = ld_cost ;
		}

		lv_amount = lv_field.At(4).getNthBracketedField(1,"X","/") ;

		if ( lv_amount.NotIsNull() )  
      {
         ld_cost =  lv_amount.At(3) ;

			md_ITD_line_number[mi_ITD_I]   = md_ID_line_number[mi_ID_I] ;
      	md_ITD_pax_id[mi_ITD_I]        = md_ID_pax_id[mi_ID_I] ;
      	md_ITD_tax_id[mi_ITD_I]        = lv_amount.For(2) ;
			if ( md_ITD_tax_id[mi_ITD_I] == "  " )
			{
				md_ITD_tax_id[mi_ITD_I] = "US" ;
			}
      	md_ITD_tax_amount[mi_ITD_I++]  = ld_cost ;
      }

		lv_amount = lv_field.At(4).getNthBracketedField(2,"X","/") ;

		if ( lv_amount.NotIsNull() )  
      {
         ld_cost =  lv_amount.At(3) ;

			md_ITD_line_number[mi_ITD_I]   = md_ID_line_number[mi_ID_I] ;
      	md_ITD_pax_id[mi_ITD_I]        = md_ID_pax_id[mi_ID_I] ;
      	md_ITD_tax_id[mi_ITD_I]        = lv_amount.For(2) ;
			if ( md_ITD_tax_id[mi_ITD_I] == "  " )
			{
				md_ITD_tax_id[mi_ITD_I] = "XT" ;
			}
      	md_ITD_tax_amount[mi_ITD_I++]  = ld_cost ;
      }

		lv_amount = lv_field.At(4).getNthBracketedField(1,"T","/") ;

		if ( lv_amount.NotIsNull() )  
      {
			ld_total = lv_amount ;
		}
		else
		{
			ld_total = md_ID_base_cost[mi_ID_I] ;
		}
	}

	md_ID_comm_rate[mi_ID_I]    = AB_null ;
	md_ID_commission[mi_ID_I]   = AB_null ;

	lv_field = pv_WT_section.getNthBracketedField
							(1,"CP-",mc_delimiter.to_string()) ;

	if ( lv_field.NotIsNull() )
	{
		md_ID_comm_rate[mi_ID_I]   = lv_field ;
	}
	else
	{
		lv_field = pv_WT_section.getNthBracketedField
							(1,"CM-",mc_delimiter.to_string()) ;

		if ( lv_field.NotIsNull() )
		{
			lv_field.removeTrailingBlanks() ;
			int li_pos = lv_field.findFirstCharNotInList("0123456789.") ;

			if ( li_pos > 0 )
				lv_field = lv_field.At(4) ;

			md_ID_commission[mi_ID_I]   = lv_field ;
		}
	}

	// Look For FP
	lv_fp = AB_null ;
	lv_field = pv_WT_section.getNthBracketedField
							(1,"FOP-",mc_delimiter.to_string()) ;
	if ( lv_field.NotIsNull() )
	{
		lv_fp = lv_field ;
	}

	fill_IrpaymentTVL(ld_total,lv_fp) ;

	md_ID_saving_code[mi_ID_I]     = md_savingCode ;
	md_ID_full_fare[mi_ID_I]       = md_fullFare ;
	md_ID_low_prac_fare[mi_ID_I]   = md_lowPracFare ;
	md_ID_rebate[mi_ID_I]          = md_rebate ;

	lv_field = pv_WT_section.getNthBracketedField(1,"VTC-",mc_delimiter.to_string()) ;
	if ( lv_field.NotIsNull() )
	{
		md_IRMK_type[mi_IRMK_I]            = "UDI" ;
		md_IRMK_udi_line_number[mi_IRMK_I] = "99";
		md_IRMK_remark[mi_IRMK_I]          = lv_field;
		md_IRMK_pax_id[mi_IRMK_I++]        = AB_null;
	}

	mi_ID_I++ ;

	EXIT;
}

//---------------------------------------------------------------------------------------------------
AB_Boolean CTAIR::checkHotelSegment(AB_Varchar	&pv_hotel,
												AB_Integer  &pi_itinLine) 
{
	int  li_index = 1 ;	

	while ( li_index <= mi_IrRentalIndex )
	{
		if ( pv_hotel.At(4).For(8) == mv_IrRentalData[li_index].At(4).For(8) )
		{
			pi_itinLine  = mv_IrRentalData[li_index].For(2) ;
			return(AB_true) ;
		}
	
		li_index++ ;
	}

	return (AB_false) ;
}

//---------------------------------------------------------------------------------------------------
void CTAIR::fillIrconsumDetailOther(AB_Char    &pc_itinType,
												AB_Integer &pi_itinLine,
												AB_Char    &pc_status) 
{
	md_ICD_itin_line_number[mi_ICD_I] = pi_itinLine ;
	md_ICD_itin_type[mi_ICD_I]        = pc_itinType ;
	md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id [0] ;
	md_ICD_service_status[mi_ICD_I++]   = pc_status ;
}
//---------------------------------------------------------------------------------------------------------------------------
void CTAIR::fill_IrpaymentTVL(D_amount    &pd_totalFare,
										AB_Text     &pv_FOP)
{
	md_IRP_amount[mi_IRP_I] = pd_totalFare ;
	md_IRP_pax_id[mi_IRP_I] = md_ID_pax_id[mi_ID_I] ;
	md_IRP_line_number[mi_IRP_I] = md_ID_line_number[mi_ID_I] ;
	md_IRP_pay_id[mi_IRP_I] = AB_Integer(1) ;

	if ( pv_FOP.IsNull() )
	{
		md_IRP_form_of_pay[mi_IRP_I] = md_formOfPay ;
		if ( md_formOfPay == "CC" )
		{
			md_IRP_cc_id[mi_IRP_I] = md_cc_id ;
			md_IRP_cc_no[mi_IRP_I] = md_cc_no ;
			md_IRP_cc_exp_date[mi_IRP_I] = md_cc_exp_date ;
			md_IRP_auth_no[mi_IRP_I] = md_auth_no ;
			md_IRP_auth_source[mi_IRP_I] =  md_auth_source ;
		}
	}
	else
	{
		getFormOfPay(pv_FOP,
						md_IRP_form_of_pay[mi_IRP_I],
						md_IRP_cc_id[mi_IRP_I],
						md_IRP_cc_no[mi_IRP_I],
						md_IRP_cc_exp_date[mi_IRP_I],
						md_IRP_auth_no[mi_IRP_I],
						md_IRP_auth_source[mi_IRP_I] ) ;
	}

  // TKX CC EXP DATE START

	if (md_IRP_form_of_pay[mi_IRP_I] == "CC")
  	{
  		if ( md_TKX_cc_exp_date.NotIsNull() )
  		{
  			md_IRP_cc_exp_date[mi_IRP_I] = md_TKX_cc_exp_date;
  		}
  	}

  // TKX CC EXP DATE END

	mi_IRP_I++ ;
}

//--------------------------------------------------------------------------------------------
void CTAIR::updIrconsumdetailTVL(AB_Char     &pc_itinType,
										AB_Integer  &pi_itinLine) 
{
	int       li_index = 0 ;
	AB_Char   lc_status(2) ;

	while( li_index < mi_ICD_I )
	{
		if (md_ICD_itin_line_number[li_index] == pi_itinLine)
		{
			// Found itin line in consumedetail
			if (md_ICD_line_number[li_index].IsNull())
			{
				// Found a free line to put the ticket no
				md_ICD_line_number[li_index]  = md_ID_line_number[mi_ID_I-1] ;
				lc_status  = md_ICD_service_status[li_index] ;

				return ;
			}

		}
		li_index++ ;
	}

	md_ICD_line_number[mi_ICD_I]      = md_ID_line_number[mi_ID_I-1] ;
	md_ICD_itin_line_number[mi_ICD_I] = pi_itinLine ;
	md_ICD_itin_type[mi_ICD_I]        = pc_itinType ;
	md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id [0] ;
	md_ICD_service_status[mi_ICD_I++] = lc_status ;

}

//----------------------------------------------------------------------------------------------------------

AB_Boolean CTAIR::processTVLTransport(	V_AB_Varchar  	&pv_arrayField,
													AB_Text        &pv_WT_section,
													AB_Boolean		pb_document_exists,
													AB_Boolean		pb_AIR_found)
{
	ENTER;
	D_char_date   ld_arrDate ; 
	AB_Date       ld_tempDate1 ; 
	AB_Date       ld_tempDate2 ; 
	AB_Integer    li_arrInd ;
	AB_Char       lc_itinLine(2) ;
	AB_Varchar    lv_field(100) ;
	AB_Char       lc_dateStart(5) ;
	AB_Char       lc_dateEnd(5) ;
	AB_Char       lc_startYear(5) ;
	AB_Char       lc_endYear(5) ;
	AB_Boolean	  lb_exist ;
	AB_Varchar	  lv_transport(100) ;
	AB_Char       lc_status(2) ;
	AB_Char		  lc_match ;
	AB_Char       lc_year(2) ;
	D_line_number ld_line_number;
	D_service_type ld_itin_type;
	AB_Text			lt_remark;
	int 				li_new_air_itin_index = -1;

	// G145 Change
	AB_Varchar    lv_airInfo(255) ;

	lc_startYear = AB_null ;
	lc_endYear   = AB_null ;
	lb_exist     = AB_false ;


	if ( 	pv_arrayField[3].At(8).For(3) == "AIR"  	|| 
			pv_arrayField[3].At(8).For(3) == "TKT" 	|| 
			pv_arrayField[3].At(8).For(3) == "PRP")
	{
		if (!pb_document_exists) {
			if (	AB_Text(pv_WT_section.getNthBracketedField(1, "CC1", mc_delimiter.to_string())).IsNull() ||
					AB_Text(pv_WT_section.getNthBracketedField(1, "CC2", mc_delimiter.to_string())).IsNull() ||
					AB_Text(pv_WT_section.getNthBracketedField(1, "CL-", mc_delimiter.to_string())).IsNull() ||
					AB_Text(pv_WT_section.getNthBracketedField(1, "TA-", mc_delimiter.to_string())).IsNull() ||
					AB_Text(pv_WT_section.getNthBracketedField(1, "TD-", mc_delimiter.to_string())).IsNull() ||
					AB_Text(pv_WT_section.getNthBracketedField(1, "FLT-", mc_delimiter.to_string())).IsNull() )
					return AB_false;

		}
		md_IT_itin_type[mi_IT_I]      = "A" ;
	}
	else if ( pv_arrayField[3].At(8).For(3) == "BUS" )
	{
		md_IT_itin_type[mi_IT_I]      = "B" ;
	}
	else if ( pv_arrayField[3].At(8).For(3) == "TRN" )
	{
		md_IT_itin_type[mi_IT_I]      = "R" ;
	}


	md_IT_line_number[mi_IT_I]    = pv_arrayField[2] ;

	ld_line_number						= md_IT_line_number[mi_IT_I];
	ld_itin_type						= md_IT_itin_type[mi_IT_I];

	lc_itinLine                 = pv_arrayField[2] ;
	lc_itinLine.rightJustify('0') ;

	md_IT_no_of_stops[mi_IT_I]    = 0 ;

	md_IT_sup_id[mi_IT_I]         = pv_arrayField[3].For(2) ;

	lc_dateStart                  = pv_arrayField[3].At(11).For(5) ;
	lc_dateEnd                    = pv_arrayField[3].At(16).For(5) ;

	lc_match = AB_null ;

	if ( pv_arrayField[3].At(8).For(3) == "TKT" )
	{
		lv_transport = lc_itinLine + md_IT_itin_type[mi_IT_I] + 
							pv_arrayField[3].For(2) + lc_dateStart ;
		lb_exist = checkTKTSegment(lv_transport,lc_match) | pb_AIR_found;
	}

	if ( !lb_exist )
	{

		lv_field = pv_WT_section.getNthBracketedField(1, "CC1", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IT_orig_city_id[mi_IT_I]   = lv_field.At(2) ;
		}
		else
		{
			md_IT_orig_city_id[mi_IT_I]   = "ZZZ" ;
		}

		lv_field = pv_WT_section.getNthBracketedField(1, "CC2", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IT_dest_city_id[mi_IT_I]   = lv_field.At(2) ;
			if (  md_IT_orig_city_id[mi_IT_I] == "ZZZ")
			{
			 	md_IT_orig_city_id[mi_IT_I]   = md_IT_dest_city_id[mi_IT_I] ;
			}
		}
		else
		{
			if ( md_IT_orig_city_id[mi_IT_I] != "ZZZ" )
			{
				md_IT_dest_city_id[mi_IT_I] = md_IT_orig_city_id[mi_IT_I] ;
			}
			else
			{
				md_IT_dest_city_id[mi_IT_I]   = "ZZZ" ;
			}
		}
	
		lv_field = pv_WT_section.getNthBracketedField(1, "Y1-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			lc_startYear   = lv_field ;
		}
		else
		{
			lc_startYear   = AB_null ;
		}

		lv_field = pv_WT_section.getNthBracketedField(1, "Y2-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			lc_endYear   = lv_field ;
		}
		else
		{
			lc_endYear   = AB_null ;
		}
	
		if ( lc_startYear.IsNull() )
		{
			lc_year   = mt_pnrDate.get_default_year(lc_dateStart) ;
			lc_year.rightJustify('0') ;

			md_IT_start_date[mi_IT_I]  = 
					lc_dateStart + lc_year ;
		}
		else
		{
			md_IT_start_date[mi_IT_I]  = lc_dateStart + lc_startYear.At(3).For(2) ;
		}
	
		if ( lc_endYear.IsNull() )
		{
			lc_year   = mt_pnrDate.get_default_year(lc_dateEnd) ;
			lc_year.rightJustify('0') ;

			ld_arrDate = lc_dateEnd + lc_year ;
		}
		else
		{
			ld_arrDate  = lc_dateEnd + lc_endYear.At(3).For(2) ;
		}
	
		ld_tempDate1                  = md_IT_start_date[mi_IT_I] ;
		ld_tempDate2                  = ld_arrDate ;
	
		li_arrInd = ld_tempDate2 - ld_tempDate1 ; 
	
		li_arrInd = li_arrInd / AB_Integer(86400) ;
	
		switch(li_arrInd.to_long())
		{
			case 0:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(0) ;
				break ;
			case 1:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(1) ;
				break ;
			case 2:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(2) ;
				break ;
			case 3:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(3) ;
				break ;
			case -1:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(4) ;
				break ;
			case -2:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(5) ;
				break ;
			default:
				md_IT_arrival_ind[mi_IT_I] = AB_Integer(0) ;
           	break ;
	
		}
	
		lv_field = pv_WT_section.getNthBracketedField(1, "TD-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IT_start_time[mi_IT_I]     = CheckTime(lv_field);
		}
		else
		{
			md_IT_start_time[mi_IT_I]     = md_lclPnrTime.changeDateFormat("hhmm", "hhmmP") ;
		}

		lv_field = pv_WT_section.getNthBracketedField(1, "TA-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IT_end_time[mi_IT_I]     = CheckTime(lv_field);
		}
		else
		{
			md_IT_end_time[mi_IT_I]     = md_lclPnrTime.changeDateFormat("hhmm", "hhmmP") ;
		}


		md_IT_reference[mi_IT_I]      = md_IRT_reference ;
	
		lv_field = pv_WT_section.getNthBracketedField(1, "FLT-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IT_service[mi_IT_I] = lv_field ;
		}
		else
		{
			md_IT_service[mi_IT_I] = "999" ;
		}
	
		lv_field = pv_WT_section.getNthBracketedField(1, "CL-", mc_delimiter.to_string()) ;
		if (lv_field.NotIsNull())
		{
			md_IT_classofservice[mi_IT_I] = lv_field;
		}
		else
		{
			md_IT_classofservice[mi_IT_I] = "T" ;
		}

		lc_status =  pv_arrayField[3].At(3).For(2) ;
	
	
		// fill Irconsumdetail
		fillIrconsumDetailOther(md_IT_itin_type[mi_IT_I],
									md_IT_line_number[mi_IT_I],
									lc_status) ;

		// G145 Chage

		if ( md_IT_itin_type[mi_IT_I] == "A" )
		{
			lv_airInfo = lc_itinLine + 	
							 md_IT_orig_city_id[mi_IT_I] +	
							 md_IT_dest_city_id[mi_IT_I] +
							 md_IT_start_date[mi_IT_I] +
							 md_IT_arrival_ind[mi_IT_I] ;

			mva_surfaceTransport[mi_surfaceTransport++]  = lv_airInfo ;
			li_new_air_itin_index = mi_IT_I;
		}

	
  		mi_IT_I++ ;

		if ( 	pv_arrayField[3].At(8).For(3) == "AIR" ||
				pv_arrayField[3].At(8).For(3) == "PRP" )
		{
			md_tvlAir_line_number[mi_tvlAIRIndex++] = ld_line_number;
			pb_AIR_found = AB_true;
		}

		if (ld_itin_type == "A")
		{
			lt_remark = pv_WT_section.getNthBracketedField(1, "FF1-", mc_delimiter.to_string()) ;
			if (lt_remark.NotIsNull())
			{
				createTransportAssociatedRemark( ld_line_number, lt_remark );
			}

			lt_remark = pv_WT_section.getNthBracketedField(1, "FF2-", mc_delimiter.to_string()) ;
			if (lt_remark.NotIsNull())
			{
				createTransportAssociatedRemark( ld_line_number, lt_remark );
			}

		}
	}

	if ( pb_document_exists )
	{

		processTVLDoc(pv_WT_section) ;

      // GAP9110: if it is PFS, we will create a non-bsp ticket using the first air itin in the pnr
      // we check if the given sup_id is a PFS sup_id

		if ( isPFSSupplier(md_IRT_cop_id, md_ID_sup_id[mi_ID_I-1]) ){
			md_ID_full_fare[mi_ID_I]       = AB_null ;
			md_ID_low_prac_fare[mi_ID_I]   = AB_null ;
			md_ID_saving_code[mi_ID_I]     = AB_null ;

			int li_index = 0 ;
			for (; li_index < mi_IT_I; li_index++) {
				if (md_IT_itin_type[li_index] == "A") {
					break;
				}
			}

			if (li_index != li_new_air_itin_index &&
				 li_new_air_itin_index != -1) {
				// clean up Irtransport and Irconsumdetail just created
				// note that li_new_air_itin_index = mi_IT_I-1
				initIrtransport(mi_IT_I-1);
				initIrconsumdetail(mi_ICD_I-1);
				mi_IT_I--;
				mi_ICD_I--;
			}

			updIrconsumdetailTVL(md_IT_itin_type[li_index], md_IT_line_number[li_index]) ;

		} else {

		// 	Either:
		//		1. Any NON TKT TVL segment.
		//		2. TKT segment w/o matching 01 and w/o any AIR/PRP TVL segments.
		//		We really created new Itinerary plus a Ticket. Now we need to link'em.

		if ( !lb_exist ) 
		{
			updIrconsumdetailTVL(md_IT_itin_type[mi_IT_I-1], md_IT_line_number[mi_IT_I-1]) ;
		}
		// 	We have:
		//		1. TKT segment with matching 01 and/or with any AIR/PRP TVL segments.
		//		We do NOT created any Itineraries, but created a Ticket and found 
		//		Itineraries created before those need to be linked to this Ticket.
		//		Now we will make these links.

		else
		{
			if ( lc_match.NotIsNull() ) // matching 01 AIR segment exists.
			{
				// Update Irconsumdetail for Itinerary which has same sup_id  or same start_date

				lv_field    = lv_transport.At(4).For(7) ; // sup_id + start_date

				updIrconsumdetailTKT(lv_field, lc_match) ;
			}

			if ( lc_match.IsNull() ) // TKT segment with matching 01 and/or with any AIR/PRP TVL segments.
			{
				for ( int index = 0; index < mi_tvlAIRIndex; index++ )
				{
					updIrconsumdetailTVL(ld_itin_type, md_tvlAir_line_number[index]);
				}
			}
		}
     }

	}

	EXIT;
	return pb_AIR_found;
}

//---------------------------------------------------------------------------------------------------
AB_Boolean CTAIR::checkTKTSegment(AB_Varchar	&pv_TKT,
											AB_Char     &pc_match)
{
	int  li_index = 1 ;	

	// Check 2 Char supplier ID first

	while ( li_index <= mi_IrTransIndex )
	{
		if ( pv_TKT.At(4).For(2) == mv_IrTransData[li_index].At(4).For(2) )
		{
			pc_match = "S" ; //Supplier match
			return(AB_true) ;
		}
	
		li_index++ ;
	}

	li_index = 1 ;

	// If no matching sup_id found check for start_date

	while ( li_index <= mi_IrTransIndex )
	{
		if ( pv_TKT.At(6).For(5) == mv_IrTransData[li_index].At(6).For(5) )
		{
			pc_match = "D" ; 	//Date match
			return(AB_true) ;
		}
	
		li_index++ ;
	}


	return (AB_false) ;
}

//--------------------------------------------------------------------------------------------

void CTAIR::updIrconsumdetailTKT(AB_Varchar   &pv_transport,
											AB_Char   	 &pc_match)
{
	int         li_indexI = 0 ;
	int         li_indexT = 1 ;
	AB_Char     lc_status(2) ;
	AB_Integer	li_itinLine ;
	AB_Boolean  lb_found ;

	if ( pc_match == "S" )			// Supplier match
	{
		while ( li_indexT <= mi_IrTransIndex )
		{
			lb_found = AB_false ;
			li_indexI = 0  ;
			lc_status = AB_null ;

			if ( mv_IrTransData[li_indexT].At(4).For(2)  == pv_transport.For(2))
			{
				li_itinLine = mv_IrTransData[li_indexT].For(2) ;

				while( li_indexI < mi_ICD_I )
				{
					if (md_ICD_itin_line_number[li_indexI] == li_itinLine)
					{
						lc_status  = md_ICD_service_status[li_indexI] ;
						// Found itin line in consumedetail
						if (md_ICD_line_number[li_indexI].IsNull() )
						{
							// Found a free line to put the ticket no
							if ( md_ICD_pax_id[li_indexI] == md_IP_pax_id [0] )
							{
								// This is for 1st Pax
								md_ICD_line_number[li_indexI]  = 
											md_ID_line_number[mi_ID_I-1] ;

								lb_found = AB_true ;
							}
						}
	
					}
					li_indexI++ ;

					if ( lb_found )
					{
						li_indexI = mi_ICD_I ;
					}
				}
				if (!lb_found)
				{
					// Did not Find any blank ICD
					md_ICD_line_number[mi_ICD_I]      = 
														md_ID_line_number[mi_ID_I-1] ;

					md_ICD_itin_line_number[mi_ICD_I] = li_itinLine ;

					md_ICD_itin_type[mi_ICD_I]        = 
									mv_IrTransData[li_indexT].At(3).For(1);

					md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id [0] ;
					md_ICD_service_status[mi_ICD_I++] = lc_status ;
				}
			}
	
			li_indexT++ ;
		}
	}

	if ( pc_match == "D" )				// Start date match
	{
		while ( li_indexT <= mi_IrTransIndex )
		{
			lb_found = AB_false ;
			li_indexI = 0  ;
			lc_status = AB_null ;

			if ( mv_IrTransData[li_indexT].At(6).For(5) == pv_transport.At(3).For(5))
			{
				li_itinLine = mv_IrTransData[li_indexT].For(2) ;

				while( li_indexI < mi_ICD_I )
				{
					if (md_ICD_itin_line_number[li_indexI] == li_itinLine)
					{
						lc_status  = md_ICD_service_status[li_indexI] ;
						// Found itin line in consumedetail
						if (md_ICD_line_number[li_indexI].IsNull() )
						{
							// Found a free line to put the ticket no
							if ( md_ICD_pax_id[li_indexI] == md_IP_pax_id [0] )
							{
								// This is for 1st Pax
								md_ICD_line_number[li_indexI]  = 
											md_ID_line_number[mi_ID_I-1] ;

								lb_found = AB_true ;
							}
						}
	
					}
					li_indexI++ ;

					if ( lb_found )
					{
						li_indexI = mi_ICD_I ;
					}
				}
				if (!lb_found)
				{
					// Did not Find any blank ICD
					md_ICD_line_number[mi_ICD_I]      = 
														md_ID_line_number[mi_ID_I-1] ;

					md_ICD_itin_line_number[mi_ICD_I] = li_itinLine ;

					md_ICD_itin_type[mi_ICD_I]        = 
									mv_IrTransData[li_indexT].At(3).For(1);

					md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id [0] ;
					md_ICD_service_status[mi_ICD_I++] = lc_status ;
				}
			}
	
			li_indexT++ ;
		}
	}


}

//----------------------------------------------------------------------------------------------------------

void CTAIR::processTVLOther(V_AB_Varchar    &pv_arrayField,
								  	AB_Text         &pv_WT_section )
{
	AB_Char       lc_itinLine(2) ;
	D_char_date   ld_arrDate ; 
	AB_Date       ld_tempDate1 ; 
	AB_Date       ld_tempDate2 ; 
	AB_Integer    li_itinLine ;
	AB_Varchar    lv_field(100) ;
	AB_Char       lc_dateStart(5) ;
	AB_Char       lc_dateEnd(5) ;
	AB_Char       lc_startYear(5) ;
	AB_Char       lc_endYear(5) ;
	AB_Char       lc_status(2) ;
	AB_Char       lc_year(2) ;

	if ( pv_arrayField[3].At(8).For(3) == "INS"  )
	{
		md_IO_itin_type[mi_IO_I]      = "I" ;
	}
	else if ( pv_arrayField[3].At(8).For(3) == "CRU" )
	{
		md_IO_itin_type[mi_IO_I]      = "S" ;
	}
	else if ( pv_arrayField[3].At(8).For(3) == "TUR" )
	{
		md_IO_itin_type[mi_IO_I]      = "T" ;
	}
	else
	{
		md_IO_itin_type[mi_IO_I]      = "O" ;
	}

	md_IO_line_number[mi_IO_I]    = pv_arrayField[2] ;

	lc_itinLine                 = pv_arrayField[2] ;
	lc_itinLine.rightJustify('0') ;

   // NULL SUP_ID FIX START
   md_IO_sup_id[mi_IO_I]   = "ZZ";
   // NULL SUP_ID FIX END

	lc_dateStart                  = pv_arrayField[3].At(11).For(5) ;

	li_itinLine                   = md_IO_line_number[mi_IO_I] ;

	md_IO_unit_type[mi_IO_I]      =  pv_arrayField[3].At(8).For(3) ;

	lv_field = pv_WT_section.getNthBracketedField
				(1, "CC1", mc_delimiter.to_string()) ;
	if (lv_field.NotIsNull())
	{
		md_IO_orig_city_id[mi_IO_I]   = lv_field.At(2) ;
	}
	else
	{
		md_IO_orig_city_id[mi_IO_I]   = "ZZZ" ;
	}

	lv_field = pv_WT_section.getNthBracketedField
				(1, "CC2", mc_delimiter.to_string()) ;
	if (lv_field.NotIsNull())
	{
		md_IO_dest_city_id[mi_IO_I]   = lv_field.At(2) ;

		if (  md_IO_orig_city_id[mi_IO_I] == "ZZZ")
		{
	 		md_IO_orig_city_id[mi_IO_I]   = md_IO_dest_city_id[mi_IO_I] ;
		}
	}
	else
	{
		if ( md_IO_orig_city_id[mi_IO_I] != "ZZZ" )
		{
			md_IO_dest_city_id[mi_IO_I] = md_IO_orig_city_id[mi_IO_I] ;
		}
		else
		{
			md_IO_dest_city_id[mi_IO_I]   = "ZZZ" ;
		}
	}

	lc_dateEnd                    = pv_arrayField[3].At(16).For(5) ;

	if (lc_dateEnd == "     ")
	{
		lc_dateEnd = lc_dateStart ;
	}

	lv_field = pv_WT_section.getNthBracketedField
				(1, "Y1-", mc_delimiter.to_string()) ;
	if (lv_field.NotIsNull())
	{
		lc_startYear   = lv_field ;
	}
	else
	{
		lc_startYear   = AB_null ;
	}

	lv_field = pv_WT_section.getNthBracketedField
				(1, "Y2-", mc_delimiter.to_string()) ;
	if (lv_field.NotIsNull())
	{
		lc_endYear   = lv_field ;
	}
	else
	{
		lc_endYear   = AB_null ;
	}

	if ( lc_dateStart != "     " )
	{
		if ( lc_startYear.IsNull() )
		{
			lc_year   = mt_pnrDate.get_default_year(lc_dateStart) ;
			lc_year.rightJustify('0') ;

			md_IO_start_date[mi_IO_I]  = lc_dateStart + lc_year ;
		}
		else
		{

			md_IO_start_date[mi_IO_I]  = lc_dateStart + lc_startYear.At(3).For(2) ;
		}
	}
	else
	{
		md_IO_start_date[mi_IO_I]  = mt_pnrDate ;
	}

	if ( lc_dateEnd != "     " )
	{
		if ( lc_endYear.IsNull() )
		{
			lc_year   = mt_pnrDate.get_default_year(lc_dateEnd) ;
			lc_year.rightJustify('0') ;

			md_IO_end_date[mi_IO_I] = 	lc_dateEnd + lc_year ;
		}
		else
		{
			md_IO_end_date[mi_IO_I]  = lc_dateEnd + lc_endYear.At(3).For(2) ;
		}
	}
	else
	{
			md_IO_end_date[mi_IO_I] = 	mt_pnrDate ;
	}

	lc_status = pv_arrayField[3].At(3).For(2) ;

  	md_IO_unit_ct[mi_IO_I]        = pv_arrayField[3].At(5).For(3) ;

	lv_field = pv_WT_section.getNthBracketedField
						(1, "CF-", mc_delimiter.to_string()) ;

	if (lv_field.NotIsNull())
	{
		md_IO_reference[mi_IO_I] = lv_field ;
	}

	processTVLDoc(pv_WT_section) ;

	md_ICD_itin_line_number[mi_ICD_I] = md_IO_line_number[mi_IO_I] ;
	md_ICD_itin_type[mi_ICD_I]        = md_IO_itin_type[mi_IO_I] ;
	md_ICD_pax_id[mi_ICD_I]           = md_IP_pax_id [0] ;
	md_ICD_line_number[mi_ICD_I]      = md_ID_line_number[mi_ID_I-1] ;
	md_ICD_service_status[mi_ICD_I++] = lc_status ;

  	mi_IO_I++ ;

}

//---------------------------------------------------------------------------------------------------------------------------

void CTAIR::getOverrideRates(AB_Char &pc_itin_type, 
									D_char_date &pt_start_date, 
									D_city      &pd_startCity, 
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

   if (pi_Index == li_counter)
      return ;

   //lv_data = pd_Data[li_counter] ;
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
      	if ((pd_Data[li_counter].For(5) == pt_start_date.For(5) ) &&
				 ( pd_Data[li_counter].At(6).For(3) == pd_startCity ) )
			{
         	lv_data = pd_Data[li_counter] ;
				break ;
			}
		}

      li_counter++ ;
   }

	if ( lv_data.IsNull() )
		return ;
   li_pos = lv_data.findSubstring("/SV") ;

   if (li_pos > 0)
   {
      if (lv_data.At(li_pos + 3).For(1) == "-")
         pd_saving_code = lv_data.At(li_pos + 4);
      else
         pd_saving_code = lv_data.At(li_pos + 3);

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

	LOG(logDEBUG)  << "DK---> pd_saving_code = " << pd_saving_code  ;
	EXIT;
}

//--------------------------------------------------------------------------------------------------------------

void CTAIR::testTripValue()
{
	int				li_pos, li_index ;
	AB_Text		 	lt_WV_section ;
	V_AB_Varchar 	lv_arrayField(100,500) ;
	AB_Boolean     lb_contParse ;
	D_passenger_no ld_paxId ;
	D_pic          lc_Ptc ;
	AB_Char        lc_paxId(2) ;
	AB_Varchar     lv_fareByLeg(12) ;

	int li_counter = 1 ;
	mi_paxFareByLeg = 0 ;

	while (li_counter <= mi_passengerWA)
	{
		lc_Ptc = mva_passengerWA[li_counter] ;
		lc_paxId = md_IP_pax_id [li_counter - 1] ;
		lc_paxId.rightJustify('0') ;

		li_index = 1 ;
		li_pos = 0 ;
   	while ( li_index < mi_airFareSectionWV )
		{
			lt_WV_section = mva_airFareSectionWV[li_index] ;

			if ( lt_WV_section.At(5).For(3) == lc_Ptc )
			{
				// found Trip Value for the PTC
				li_pos = lv_arrayField.parseStringMultDel(lt_WV_section,mc_delimiter.to_string()) ;

				break ;	
			}	

			li_index++ ;

		}

		if ( li_pos > 0 )
		{
			li_index = 5 ;
	
			lb_contParse = AB_true ;
	
			while ( li_index <= li_pos && lb_contParse )
			{
				LOG(logDEBUG)  << "DK---> li_index / li_pos :" << li_index << " / " << li_pos  ;
				LOG(logDEBUG) << "DK---> lv_arrayField[li_index] = " << lv_arrayField[li_index]  ;
				// Got all the element in the array
	
				lb_contParse = AB_false ;

				if ( lv_arrayField[li_index].For(5) == "/////")
				{
					LOG(logDEBUG)  << "DK---> /////"  ;
					// Means continuation
					lb_contParse = AB_true ;
				}
				else if ( lv_arrayField[li_index].For(3).isAllAlpha() )
				{
					// First Three are city code 
					LOG(logDEBUG)  << "DK---> isAllAlpha"  ;
	
					if ( lv_arrayField[li_index].At(4).For(2).isAllAlphaNumeric() )
					{
						// 2nd 2 Char are issuing airline ( could the alphanumeric)
						// Normal case
						// Type 1, 2, 3, 4 and couple other unnecessary
						LOG(logDEBUG)  << "DK---> isAllAlphaNumeric"  ;
	
						if ( (lv_arrayField[li_index].At(6).For(1) == "X" &&
								( lv_arrayField[li_index].At(7).For(1) == " " || 
							  	lv_arrayField[li_index].At(7).For(1).isAllNumeric())) ||
								lv_arrayField[li_index].At(6).For(1) == " " ||
								lv_arrayField[li_index].At(6).For(1).isAllNumeric() ) 
							
						{
							// Type 1,2,3,4
							LOG(logDEBUG)  << "DK---> lv_arrayField[li_index].At(6).For(1)==X"  ;
	
							lb_contParse = AB_true ;

							if ( lv_arrayField[li_index].At(11).For(1) == "M" )
							{
								lv_fareByLeg = "0.00" ;
							}
							else
							{
								lv_fareByLeg = lv_arrayField[li_index].At(6) ;
								lv_fareByLeg.leftJustify() ;

								if ( lv_fareByLeg.For(1) == "X" )
								{
									lv_fareByLeg = lv_fareByLeg.At(2) ;
									lv_fareByLeg.leftJustify() ;
								}

								if ( lv_fareByLeg.For(1) == " " )
								{
									lv_fareByLeg = "0.00" ;
								}
								else
								{
									lv_fareByLeg.removeTrailingBlanks() ;
									lv_fareByLeg = lv_fareByLeg.For(lv_fareByLeg.length()-2) + "." + lv_fareByLeg.At(lv_fareByLeg.length() - 1).For(2)  ;
								}
							}

							mva_paxFareByLeg[++mi_paxFareByLeg] = lc_paxId + lv_fareByLeg ;
						}
					}
					else if ( lv_arrayField[li_index].At(4).For(2) == "  " )
					{
						// type 13,20,24 
						LOG(logDEBUG)  << "DK---> lv_arrayField[li_index].At(4).For(2)=='  ' "  ;
						// But we have to ignore 13
						if ( lv_arrayField[li_index].For(3) == "XXX" ||
						  	lv_arrayField[li_index - 1].For(3) == "XXX" )
							
							// Waiting for example
							//||
						  	//lv_arrayField[li_index - 1].At(5).For(1) == "/" )
				  		{
							// Type 20,24
							//We should take care of Fare by leg here after G145
	
							lb_contParse = AB_true ;
				  		}
					}
					/*
					else if ( lv_arrayField[li_index].At(5).For(1) == "/" )
					{
						// Type 21
						LOG(logDEBUG)  << "DK---> lv_arrayField[li_index].At(5).For(1)=='/' "  ;
						lb_contParse = AB_true ;
						LOG(logDEBUG)  << "Trip Value Field"  ;
						LOG(logDEBUG) << lv_arrayField[li_index]  ;
					}
					*/
				}
	
				LOG(logDEBUG)  << "DK---> li_index++ "  ;
				li_index++ ;
			}
		}

		li_counter++ ;
	}
}

//--------------------------------------------------------------------------------------------------------------

void CTAIR::updateIcdWithFareByLeg()
{
	int            li_index = 1 ,li_counter = 0 ;
	D_passenger_no ld_paxId ;
	AB_Boolean     lb_arnk ;


	while ( li_index <= mi_paxFareByLeg )
	{
		li_counter = 0 ;
		ld_paxId = mva_paxFareByLeg[li_index].For(2) ;

		while ( li_counter < mi_ICD_I )
		{
			if ( 	md_ICD_itin_type[li_counter] != "O" &&
					md_ICD_pax_id[li_counter] == ld_paxId  &&
				  	md_ICD_fare_by_leg[li_counter].IsNull())
			{
				//Go and check if the itin is ARNK call the following procedure 
				// After implementation of G145
				// lb_arnk = checkARNK(li_counter) 

				lb_arnk = checkARNK(li_counter) ;

				md_ICD_fare_by_leg[li_counter] = mva_paxFareByLeg[li_index].At(3) ;
				break ;
			}

			li_counter++ ;
		}

		li_index++ ;	
	}
}

//-------------------------------------------------------------------------------------------------------------

AB_Boolean CTAIR::checkARNK(int    &pi_index)
{
	int  li_index = 0 ;

	while ( li_index < mi_IT_I )
	{
		if ( md_IT_service[li_index] == "ARNK" )
		{
			return (AB_true) ;
		}
		else
		{
			return(AB_false) ;
		}

		li_index++ ;
	}

	return(AB_false) ;
}

//------------------------------------------------------------------------------------------------------------

// CSR #117479 START

void CTAIR::getXFbreakdown(D_amount  &pd_totXFTaxes)
{
   int          li_index, li_pos ;
   AB_Text      lt_WV_section ;
   V_AB_Varchar lv_arrayField(100,500) ;
   AB_Boolean   lb_foundXF ;


   pd_totXFTaxes = D_amount(0) ;

   li_index = 1 ;
	li_pos   = 0 ;

   while ( li_index < mi_airFareSectionWV )
   {
      lt_WV_section = mva_airFareSectionWV[li_index] ;

      if ( lt_WV_section.At(5).For(3) == md_ID_pic[mi_ID_I] )
      {
         // found Trip Value for the PTC
         li_pos = lv_arrayField.parseStringMultDel(lt_WV_section,mc_delimiter.to_string()) ;

         break ;
      }

      li_index++ ;

   }

   pd_totXFTaxes = D_amount(0) ;
   lb_foundXF = AB_false ;
   if ( li_pos > 0 )
   {
      li_index = 5 ;

      while ( li_index <= li_pos )
      {
         // Got all the element in the array

         if ( !lb_foundXF )
         {
            if ( lv_arrayField[li_index].For(3) == " XF")
            {
               // Found Start of XF
               lb_foundXF = AB_true ;
               md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
               md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
               md_ITD_city_id[mi_ITD_I]      =
                                       lv_arrayField[li_index].At(8).For(3) ;
               md_ITD_tax_amount[mi_ITD_I] =
                                       lv_arrayField[li_index].At(11).For(1) ;

// SCR #117479 SPECIFIC START
               if ( md_ITD_tax_amount[mi_ITD_I] == "5" )
                  md_ITD_tax_amount[mi_ITD_I] = "4.5";
// SCR #117479 SPECIFIC END

               pd_totXFTaxes = pd_totXFTaxes + D_amount(md_ITD_tax_amount[mi_ITD_I]) ;
               md_ITD_tax_id[mi_ITD_I++]       = "XF" ;
            }
         }
         else
         {
            if ( lv_arrayField[li_index].For(3) != "ROE" &&
                 lv_arrayField[li_index].For(3) != "  $" &&
                 lv_arrayField[li_index].For(1) != "1" )
            {
               md_ITD_line_number[mi_ITD_I]  = md_ID_line_number[mi_ID_I] ;
               md_ITD_pax_id[mi_ITD_I]       = md_ID_pax_id[mi_ID_I] ;
               md_ITD_city_id[mi_ITD_I]      =
                                       lv_arrayField[li_index].At(8).For(3) ;
               md_ITD_tax_amount[mi_ITD_I] =
                                       lv_arrayField[li_index].At(11).For(1) ;

// SCR #117479 SPECIFIC START
               if ( md_ITD_tax_amount[mi_ITD_I] == "5" )
                  md_ITD_tax_amount[mi_ITD_I] = "4.5";
// SCR #117479 SPECIFIC END

               pd_totXFTaxes = pd_totXFTaxes + D_amount(md_ITD_tax_amount[mi_ITD_I]) ;
               md_ITD_tax_id[mi_ITD_I++]       = "XF" ;
            }
            else
            {
               break ;
            }
         }
         li_index++ ;
      }
   }
}

// CSR #117479 END

//------------------------------------------------------------------------------

void CTAIR::processSvcFee(AB_Text	&pv_WT_section)
{
	ENTER;

	AB_Varchar				  lv_field(100); 
	AB_Char                lc_svcFee(13) ; 
	AB_Varchar  		 	  lv_fp(100) ;
	D_form_of_payment      ld_fp ;
	D_credit_card_sup      ld_ccId; 
	D_credit_card_no       ld_ccNo ;
	D_expiry_date          ld_ccExpDate ;
	D_authorization        ld_authNo ;
	D_authorization_source ld_source ;
	AB_Char					  lc_svcSup(3) ;
	AB_Char                lv_svcFP(24) ;
	AB_Char                lc_svcDescription(20) ;
	AB_Integer             li_status;
	V_AB_Varchar 			  lv_arrayField(100,100) ;
	AB_Char       			  lc_accountLine(2) ;

	// SCR 230260 START
	AB_Char					  lc_fop(20);
	// SCR 230260 END
     
	lv_arrayField.parseStringMultDel(pv_WT_section,mc_delimiter.to_string()) ;
	lc_accountLine = lv_arrayField[2] ;
	lc_accountLine.rightJustify('0') ;

   li_status = AB_Integer(0);

   lv_field = pv_WT_section.getNthBracketedField(1,"VC-", 
																 mc_delimiter.to_string()) ;


	if (lv_field.NotIsNull())
	{
		lc_svcSup = lv_field ;

		lv_field = pv_WT_section.getNthBracketedField
            	(1, "DUE-", mc_delimiter.to_string()) ;

		if (lv_field.IsNull())
			lv_field = pv_WT_section.getNthBracketedField
            		(1, "DU-", mc_delimiter.to_string()) ;
			
		if (lv_field.NotIsNull())
		{
			lc_svcFee = lv_field.At(4).getNthBracketedField(1,"B","/") ;


			/**
			 * Resolve form of payment
			 *
			 * Try order
			 *  1. FOP -
			 *  2. AC2-
			 *  3. TAIR Record 7
			 *  4. Default to CA
          **/
		
			lv_fp = AB_null ;
			lv_field = pv_WT_section.getNthBracketedField
									(1,"FOP-",mc_delimiter.to_string()) ;

			if ( lv_field.NotIsNull() )
			{
				// FOP is available 

				lv_fp = lv_field ;

				li_status = getSvcFop(lv_fp,
											 ld_fp,
											 ld_ccId,
											 ld_ccNo,
								          ld_ccExpDate) ;

				if ( li_status == AB_Integer(0) )
				{
					// FOP data is used

				   if ( ld_fp == "CC" )
				   {
							// SCR 230260 START
							lc_fop	= ld_fp + ld_ccId + ld_ccNo;
				   		lv_svcFP = lc_fop + ld_ccExpDate ;
							// SCR 230260 END
				   }
			   	else
               {
			   		lv_svcFP = ld_fp ;
               }
				}
				else
				{
				  mi_rejectirId = AB_Integer(3616);

				  // FOP is not usable, try AC2-

				  lv_field = pv_WT_section.getNthBracketedField
				  					(1,"AC2-",mc_delimiter.to_string()) ;

				  if ( lv_field.NotIsNull() )
				  {
					 // Try AC2- 

				    lv_fp = lv_field ;

				    li_status = getSvcFop(lv_fp,
					      					  ld_fp,
						      				  ld_ccId,
							      			  ld_ccNo,
								              ld_ccExpDate) ;
		
      			 if ( li_status == AB_Integer(0) )
	      		 {
		      		// AC2- date is used

			      	if ( ld_fp == "CC" )
				      {
							// SCR 230260 START
						  lc_fop		= ld_fp + ld_ccId + ld_ccNo;
				   	  lv_svcFP 	= lc_fop + ld_ccExpDate ;
							// SCR 230260 END
				       }
			   	    else
                   {
			   		   lv_svcFP = ld_fp ;
                   }
			       }
				    else 
				    {
		      		 // AC2- date is not usable, 
					    // use TAIR record 7 

					    if ( md_formOfPay.NotIsNull() )
					    {
						   if ( md_formOfPay == "CC" ) 
						   {
                       if ( md_cc_exp_date.NotIsNull() )
                       {
								  // SCR 230260 START
                          lc_fop 	= md_formOfPay + md_cc_id + md_cc_no;
                          lv_svcFP 	= lc_fop + md_cc_exp_date ;
								  // SCR 230260 END
                       }
                       else 
                       {
                           lv_svcFP = md_formOfPay + md_cc_id + md_cc_no  ;
                       }
						   }
						   else
						   {
					        lv_svcFP = md_formOfPay ;
						   }
					    }
					    else
					    {
					      lv_svcFP = "CA";
					    }
				    }
				  } 
				  else 
				  {
					  // Neither FOP- & AC2- is available,  
					  // try to use TAIR record 7

					  if ( md_formOfPay.NotIsNull() )
					  {
						 if ( md_formOfPay == "CC" ) 
						 {
                     if ( md_cc_exp_date.NotIsNull() )
                     {
								// SCR 230260 START
                        lc_fop 	= md_formOfPay + md_cc_id + md_cc_no;
                        lv_svcFP = lc_fop + md_cc_exp_date ;
								// SCR 230260 END
                     }
                     else 
                     {
                         lv_svcFP = md_formOfPay + md_cc_id + md_cc_no  ;
                     }
						 }
						 else
						 {
					      lv_svcFP = md_formOfPay ;
						 }
					  }
					  else
					  {
					    lv_svcFP = "CA";
					  }
				  }
				}
			}
			else
			{
				// FOP- not available try AC2-

				lv_field = pv_WT_section.getNthBracketedField
				  					(1,"AC2-",mc_delimiter.to_string()) ;

				if ( lv_field.NotIsNull() )
				{
				  // Try AC2- 

				  lv_fp = lv_field ;

				  li_status = getSvcFop(lv_fp,
					      					ld_fp,
						      				ld_ccId,
							      			ld_ccNo,
								            ld_ccExpDate) ;
		
      		  if ( li_status == AB_Integer(0) )
	      	  {
		      	 // AC2- date is used

			       if ( ld_fp == "CC" )
				    {
						// SCR 230260 START
				   	lc_fop 	= ld_fp + ld_ccId + ld_ccNo;
				   	lv_svcFP = lc_fop + ld_ccExpDate ;
						// SCR 230260 END
				    }
			   	 else
                {
			   	   lv_svcFP = ld_fp ;
                }
			     }
				  else 
				  {
				    mi_rejectirId = AB_Integer(3616);

		      	 // AC2- date is not usable, 
					 // use TAIR record 7 

					 if ( md_formOfPay.NotIsNull() )
					 {
						if ( md_formOfPay == "CC" ) 
						{
                    if ( md_cc_exp_date.NotIsNull() )
                    {
							 // SCR 230260 START
                      lc_fop 		= md_formOfPay + md_cc_id + md_cc_no;
                      lv_svcFP 	= lc_fop + md_cc_exp_date ;
							 // SCR 230260 END
                    }
                    else 
                    {
                      lv_svcFP = md_formOfPay + md_cc_id + md_cc_no  ;
                    }
						}
						else
						{
					     lv_svcFP = md_formOfPay ;
					   }
					 }
					 else
					 {
					   lv_svcFP = "CA";
					 }
				  }
				} 
				else 
				{
				  // Neither FOP- & AC2- is available,  
				  // try to use  TAIR record 7

				  if ( md_formOfPay.NotIsNull() )
				  {
					 if ( md_formOfPay == "CC" ) 
					 {
                  if ( md_cc_exp_date.NotIsNull() )
                  {
						  // SCR 230260 START
                    lc_fop 	= md_formOfPay + md_cc_id + md_cc_no;
                    lv_svcFP 	= lc_fop + md_cc_exp_date ;
						  // SCR 230260 END
                  }
                  else 
                  {
                    lv_svcFP = md_formOfPay + md_cc_id + md_cc_no  ;
                  }
					 }
					 else
				    {
					   lv_svcFP = md_formOfPay ;
					 }
				  }
				  else
				  {
					 lv_svcFP = "CA";
				  }
				}
			}
		
			// SCR 412640 Start
			bool lb_cc_valid = true;
			if (md_formOfPay == "CC")  {
				AB_Varchar lv_cc_id(40);
				long       ll_error_code;
				lv_cc_id = md_cc_id + md_cc_no;
				if (!validCreditCard(lv_cc_id, md_cc_exp_date, ll_error_code)) {
					lb_cc_valid = false;
				}
			}
			// SCR 412640 End

			if ( mb_svcFound == AB_false )
				md_IRMK_type[mi_IRMK_I] = "SFC" ; // The first SFC for regular processing
			else
				md_IRMK_type[mi_IRMK_I] = "SFT"; // Multiple CFC's for subscribed companies
		
			lv_field = pv_WT_section.getNthBracketedField(1, "FF1-", mc_delimiter.to_string()) ;
			if ( lv_field.NotIsNull() )
			{
				lc_svcDescription = lv_field.For(20);
			}
			else
			{
				lc_svcDescription = " ";
			}

			AB_Char lc_crs = "WSP" ;

			md_IRMK_remark[mi_IRMK_I] = (lb_cc_valid? "" : "BADCC/") +
												 lc_crs    + 
			   								 lv_svcFP  + 
												 "  "      +
												 lc_svcDescription +
			   								 lc_svcFee +
												 lc_accountLine;
		
			md_IRMK_pax_id[mi_IRMK_I++] = AB_null ;                            

			mb_svcFound = AB_true ;
		}
      else
	   {
		  // Invalid service fee record 

	     mi_rejectirId = AB_Integer(3616);
	   }
	}
}

//------------------------------------------------------------------------------

void CTAIR::saveSvcIrregularity()
{
   D_description   ld_errorDescription ;
   AB_Integer      li_msgId ;

   md_queType = "SFC" ;

   li_msgId = mi_rejectirId;

   //CMsgHandler::getMsgDescWithSub(li_msgId, "E", 
   //                               mv_rejectirSub,
   //                               ld_errorDescription) ;

   CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;

   createQueue(ld_errorDescription) ;
}


// US044 START
void CTAIR::IrTransportRemarks(CPNRWS &po_PNR)
{
   AB_Text lt_segment, lt_remark ;
	V_AB_Varchar lv_arrayField(200,100) ;
	int li_pos, li_rem_number, li_offset;
	AB_Boolean lb_itin_found;
	D_line_number  ld_line_number;
   
   po_PNR.resetSegment("N") ;
   lt_segment = po_PNR.getNextSegment("N") ;

   while (lt_segment.NotIsNull())
   {
		lt_segment = lt_segment.At(2);
		lt_segment = mc_delimiter + lt_segment + mc_delimiter;

		li_rem_number= lv_arrayField.parseStringMultDel(lt_segment, mc_delimiter.to_string());

		for (int li_index = 1; li_index <= li_rem_number; li_index++ )
		{
			lb_itin_found = AB_false;

			// We process IR, LNS and PID remarks

			if ( 	lv_arrayField[li_index].For(3) == "IR-"	||
					lv_arrayField[li_index].For(4) == "LNS-"	||
					lv_arrayField[li_index].For(4) == "PID-" )
			{
				if ( lv_arrayField[li_index].For(3) == "IR-" )
					li_offset = 4;
				else
					li_offset = 5;

				// Looking for #SNN# or #SNN ....# where NN is itin_line_no

				if ( 	li_index <  li_rem_number	&&
						lv_arrayField[li_index+1].For(1)	== "S" )
				{
					li_pos	= lv_arrayField[li_index+1].findSubstring(" ");

					if ( 	li_pos > 2 		&& 
							lv_arrayField[li_index+1].At(2).For( li_pos - 2 ).isAllNumeric() )
					{
						lb_itin_found 	= AB_true;
						ld_line_number	= lv_arrayField[li_index+1].At(2).For( li_pos - 2 );
					}
					else if (li_pos == 0		&&
								lv_arrayField[li_index+1].At(2).isAllNumeric()  )
					{
						lb_itin_found = AB_true;
						ld_line_number	= lv_arrayField[li_index+1].At(2);
					}
				}


				lt_remark = lv_arrayField[li_index].At(li_offset);

				if ( lb_itin_found == AB_false )
				{
					createUnAssociatedRemark( lt_remark );
				}
				else
				{
            	if ( ! createTransportAssociatedRemark( ld_line_number, lt_remark ) )
               	if ( ! createRentalAssociatedRemark( ld_line_number, lt_remark ) )
                  	createOtherAssociatedRemark( ld_line_number, lt_remark );
				}
			}
		}

   	lt_segment = po_PNR.getNextSegment("N") ;
	}
}

// US044 END

// SCR 281836 START
void CTAIR::fillAirLocatorAndSeat(CPNRWS  &po_PNR) {
	for ( int li_index = 0; li_index < mi_IT_I; li_index++ ) {
		for (unsigned i = 0; i < mvo_n_sec_airline_locator.size(); i++) {
			AB_Text lt_str = mvo_n_sec_airline_locator[i];

			D_line_number	ld_line_number = lt_str.For(2);
			D_terminal		ld_locator = lt_str.At(40).For(6);

			if (md_IT_line_number[li_index] == ld_line_number) {
				md_IT_airline_locator[li_index] = ld_locator;
				break;
			}
		}
	}

	fillAirSeat(po_PNR);
}
// SCR 281836 END

// SCR 406029 START
void CTAIR::processMCOTax(AB_Text  &pt_WO_section)
{
   ENTER;	

   D_amount ld_totTaxes;

	AB_Varchar
		lv_taxString1(20),
		lv_taxString2(20),
		lv_taxString3(20),
		lv_taxString4(20),
		lv_taxString5(20),
		lv_taxString6(20),
		lv_taxString7(20),
		lv_taxString8(20),
		lv_taxString9(20);

	lv_taxString1 = pt_WO_section.getNthBracketedField(1, "TX1-", mc_delimiter.to_string());
	lv_taxString2 = pt_WO_section.getNthBracketedField(1, "TX2-", mc_delimiter.to_string());
	lv_taxString3 = pt_WO_section.getNthBracketedField(1, "TX3-", mc_delimiter.to_string());
	lv_taxString4 = pt_WO_section.getNthBracketedField(1, "TX4-", mc_delimiter.to_string());
	lv_taxString5 = pt_WO_section.getNthBracketedField(1, "TX5-", mc_delimiter.to_string());
	lv_taxString6 = pt_WO_section.getNthBracketedField(1, "TX6-", mc_delimiter.to_string());
	lv_taxString7 = pt_WO_section.getNthBracketedField(1, "TX7-", mc_delimiter.to_string());
	lv_taxString8 = pt_WO_section.getNthBracketedField(1, "TX8-", mc_delimiter.to_string());
	lv_taxString9 = pt_WO_section.getNthBracketedField(1, "TX9-", mc_delimiter.to_string());
			
	processTax(lv_taxString1, lv_taxString2, lv_taxString3, ld_totTaxes);
	processTax(lv_taxString4, lv_taxString5, lv_taxString6, ld_totTaxes);
	processTax(lv_taxString7, lv_taxString8, lv_taxString9, ld_totTaxes);
	
   EXIT;
}
// SCR 406029 END
