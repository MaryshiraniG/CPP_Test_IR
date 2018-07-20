/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.30
      Delta Created: 14/07/11 - 13:48:50
         Last Delta: app/src/common/InterfaceRecord/_us 7/11/14 - 13:48:50 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TAIR.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/

#include <vector>
using namespace std;

class CTAIR : public CInterfaceRecord
{
   public:
      void createCTAIR(CPNRWS &po_PNR)  ;
      void printAll() ;
		CTAIR () ; 
      ~CTAIR() {cout << "OBJECT CTAIR DESTROYED" << endl ;}

      int mi_IrHotelIndex ;
      V_AB_Varchar mv_IrHotelData ;

      int mi_IrCarIndex ;
      V_AB_Varchar mv_IrCarData ;

      int mi_tktIndex ;
      V_AB_Char md_tktData ;

		int    mi_tktCounter ;

		AB_Varchar        mv_itinTransport ;    // Assigned for PNRS having no pax and itin relationship

   protected:
		AB_Char           mc_delimiter ;

      D_supplier             md_supId ; // Goes for Irticket
      D_short_name           md_crsTour ; // Goes for Irticket
		D_form_of_payment      md_formOfPay ;
		D_credit_card_sup      md_cc_id; 
		D_credit_card_no       md_cc_no;
		D_expiry_date          md_cc_exp_date;
		D_authorization        md_auth_no;
		D_authorization_source md_auth_source;
		// WS 17 START *******
		AB_Date					  mt_lclPnrDate;
		// WS 17 END *******
		// WS 17 START
		D_char_time             md_lclPnrTime;
		int               		mi_XDLine ;
		int       					mi_MCO_itin_line_number;
		AB_Varchar					md_currentNameNumber;
		D_char_numeric         	md_ID_ref_comm_rate[500];
		D_char_numeric         	md_ID_ref_commission[500];

		// WS 17 END

		D_amount				 md_commRate ;
		D_amount				 md_commission ;

		// PASSENGER INFO
      V_AB_Varchar       mva_passengerWA ;
      int                mi_passengerWA ;

		// TVL Segments
      V_AB_Varchar       mva_tvlTKTSegments ;
      int                mi_tvlTKTIndex ;

      V_AB_Varchar       mva_tvlOtherSegments ;
      int                mi_tvlOtherIndex ;

		D_line_number    	 md_tvlAir_line_number[500];
      int                mi_tvlAIRIndex ;

		// STORE COMMISSION
      V_AB_Varchar       mva_airCommission ;
      int                mi_commIndex ;

		// STRORE AIR FARE INFO
      V_AB_Varchar       mva_airFareSectionWG ;
      int                mi_airFareSectionWG ;

		// HORIZONTAL FARE CALCULATION
      V_AB_Varchar       mva_airFareSectionWB ;
      int                mi_airFareSectionWB ;

		// TRIP VALUE SECTION
      V_AB_Text          mva_airFareSectionWV ;
      int                mi_airFareSectionWV ;

		// STRORE ISSUE EXCHANGE FOR INFO
      V_AB_Varchar       mva_exchSectionWC ;
      int                mi_exchSectionWC ;
	
		// EXCHANGE FARE INFO
      V_AB_Varchar       mva_exchSectionWX ;
      int                mi_exchSectionWX ;

		// STORE AIR SEAT INFO
      int                mi_WS_count ;
      V_AB_Varchar       mva_WS_section ;

		// FARE BY LEG BY PAX ID TYPE
      V_AB_Varchar       mva_paxFareByLeg ;
      int                mi_paxFareByLeg ;

		AB_Boolean			mb_exchange ;
		AB_Varchar        mv_baggage ;
		AB_Varchar			mv_service_status ;
		AB_Varchar			mv_fareBasis ;
		AB_Varchar			mv_tktDesig ;
		AB_Varchar			mv_segmentMile ;
		D_connection      md_connect ;

		D_document_number md_invoiceNo ;

		AB_Boolean        mb_address ;
		AB_Char				mc_iurType ;
		AB_Boolean			mb_buildCfa ;

		AB_Integer			mi_noofPax ;
		AB_Integer			mi_noofentitle ;

		D_saving_code		md_savingCode ;
		D_char_numeric		md_fullFare;
		D_char_numeric		md_lowPracFare;
		D_char_numeric		md_rebate ;

		// STORE RENTAL INFO TO CHECK WITH TVL SEGMENT

      int mi_IrRentalIndex ;
      V_AB_Varchar mv_IrRentalData ;

		// STORE RENTAL INFO TO CHECK WITH TVL SEGMENT

      int mi_IrTransIndex ;
      V_AB_Varchar mv_IrTransData ;

		// Service Fee Variable
		AB_Boolean   mb_svcFound ;

		vector<AB_Text>	mvo_n_sec_airline_locator;
								
		void errorHandler(CPNRWS &po_PNR);

		void fill_IrHeader(CPNRWS &po_PNR);
		void fillCommission(CPNRWS &po_PNR) ;
		void fillTVLSegments(CPNRWS &po_PNR) ;
		void populate_IrAddress(CPNRWS &po_PNR);

		void fill_IrPasDocument(CPNRWS &po_PNR);
		// WS 17 START 
		AB_Boolean fill_IrPasMCODocument(AB_Text  &pt_WO_section);
		void createMCOItin(const AB_Varchar& pv_unit_type, const AB_Varchar& pv_remark);
		void createMCOConsumDetail();
		// WS 17 END

		//void processFare(CPNRWS &po_PNR,AB_Integer &li_pax_id,D_pic &pc_Ptc) ;
		void processFare(CPNRWS       &po_PNR,
								AB_Integer   &li_pax_id,
								D_pic        &pc_Ptc,
								D_amount     &pd_totTax) ;
		//void processTax(AB_Varchar	&pv_taxString1,
							//AB_Varchar   &pv_taxString2,
							//AB_Varchar   &pv_taxString3) ;
		void processTax(AB_Varchar	&pv_taxString1,
								AB_Varchar   &pv_taxString2,
								AB_Varchar   &pv_taxString3,
								D_amount     &pd_totTax) ;
		void fill_Irpayment(AB_Text    &pt_totalFare) ;
		// WS 17 START
		void fill_Irpayment(	D_amount                &pd_amount,
                           D_form_of_payment       &pd_formOfPay,
                           D_credit_card_sup       &pd_cc_id,
                           D_credit_card_no        &pd_cc_no,
                           D_expiry_date           &pd_cc_exp_date,
                           D_authorization         &pd_auth_no,
                           D_authorization_source  &pd_auth_source);

		// WS 17 END
		void getCommission(D_pic	&pd_ptcType) ;

		void fillExchange(CPNRWS &po_PNR) ;
		//AB_Boolean getExchange(AB_Integer &pi_paxId,D_pic &pd_Ptc) ;
		AB_Boolean processExchange(AB_Integer &pi_paxId,
											D_pic      &pd_Ptc,
											D_amount   &pd_totTax,
											AB_Char     &pc_docLine) ;
		void processExchangeTax(AB_Varchar	&pv_taxString1,
										AB_Varchar   &pv_taxString2,
										AB_Varchar   &pv_taxString3,
										D_amount     &pd_totTax) ;

		void fillIrTransport(CPNRWS &po_PNR) ;

		void fillIrHotel_H(CPNRWS &po_PNR) ;
		void CreateRentalRemarks(AB_Varchar &pv_field) ;
		void IrHotelRemarks(AB_Text &pv_remarkField) ;

		void fillIrCar_I(CPNRWS  &po_PNR) ;
		void getRateDuration(AB_Varchar  &pv_field) ;
		// WS 17 START *******
		void getMileage(AB_Varchar &pv_fieldFM, AB_Varchar &pv_fieldMC)  ;
		// WS 17 END *******
		void IrHotelrates(AB_Text  &pv_section) ;

		void parsePhone(AB_Varchar &pv_phoneField)  ;

		void getFormOfPay(AB_Text 			  	  	  &pv_fop,
								D_form_of_payment      &pd_formOfPay ,
								D_credit_card_sup      &pd_cc_id, 
								D_credit_card_no       &pd_cc_no,
								D_expiry_date          &pd_cc_exp_date,
								D_authorization        &pd_auth_no,
								D_authorization_source &pd_auth_source)  ;

		AB_Integer getSvcFop(AB_Text 			  	&pv_fop,
					      		D_form_of_payment &pd_formOfPay ,
								   D_credit_card_sup &pd_cc_id, 
						      	D_credit_card_no  &pd_cc_no,
				      			D_expiry_date     &pd_cc_exp_date); 

		void fillAirSeat(CPNRWS  &po_PNR) ;
		void getSeatInfo(int   &pi_paxCount) ;
		void populateIrConsumDetail(CPNRWS  &po_PNR) ;

		void processTVLSegment() ;
		AB_Boolean processTVLTransport(	V_AB_Varchar   	&pv_arrayField,
											AB_Text         	&pv_WT_section,
											AB_Boolean			pb_document_exists,
											AB_Boolean			pb_AIR_found) ;
		void getRemarkFields(CPNRWS   &po_PNR) ;

		void fillIrAmtrak(CPNRWS  &po_PNR) ;

		void fillAirFare(CPNRWS  &po_PNR) ;
		void processTVLDoc(AB_Text	&pv_WT_section) ;
		void processTVLRental(V_AB_Varchar   &pv_arrayField,
								  	AB_Text         &pv_WT_section) ;
		AB_Boolean checkHotelSegment(AB_Varchar	&pv_hotel,
												AB_Integer  &pi_itinLine)  ;

		void fillIrconsumDetailOther(AB_Char    &pc_itinType,
											AB_Integer &pi_itinLine,
											AB_Char    &pc_status) ;
		void fill_IrpaymentTVL(D_amount    &pd_totalFare,
									AB_Text     &pv_FOP) ;
		void updIrconsumdetailTVL(AB_Char     &pc_itinType,
									AB_Integer  &pi_itinLine)  ;
		void updIrconsumdetailTKT(AB_Varchar   &pv_transport,
											AB_Char   	 &pc_match) ;
		AB_Boolean checkTKTSegment(AB_Varchar	&pv_TKT,
											AB_Char     &pc_match) ;
		void processTVLOther(V_AB_Varchar    &pv_arrayField,
								  	AB_Text         &pv_WT_section) ;
		void getOverrideRates(AB_Char &pc_itin_type, 
									D_char_date &pt_start_date, 
									D_city      &pd_startCity, 
									D_saving_code &pd_saving_code,
             					D_char_numeric &pd_local_rate, 
									D_char_numeric &pd_regular_rate,
             				   D_char_numeric &pd_booked_rate, 
									V_AB_Text &pd_Data, int pi_Index) ;
		void fillFareCalculation(CPNRWS  &po_PNR) ;
// SCR #117479 START
      void getXFbreakdown(D_amount  &pd_totXFTaxes)  ;
// SCR #117479 END

		void testTripValue() ;
		void fillTripValue(CPNRWS  &po_PNR) ;
		void updateIcdWithFareByLeg() ;
		AB_Boolean checkARNK(int    &pi_index) ;

		void shiftConnectInd();   //10APR99-SC

		void processSvcFee(AB_Text	&pv_WT_section) ;
		void saveSvcIrregularity() ;

		// US044 START
		void IrTransportRemarks(CPNRWS &po_PNR);
		// US044 END

		// SCR 281836 START
		void fillAirLocatorAndSeat(CPNRWS  &po_PNR);
		// SCR 281836 END

		// SCR 406029 START
		void processMCOTax(AB_Text  &pt_WO_section);
		// SCR 406029 END
} ;
