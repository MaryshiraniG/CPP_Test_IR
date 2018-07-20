/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.79
      Delta Created: 17/10/18 - 15:12:28
         Last Delta: app/src/common/InterfaceRecord/_us 10/18/17 - 15:12:28 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.IUR.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/

/*
PURPOSE:
============================================================================
CHANGE HISTORY: ( PLEASE PUT MOST RECENT CHANGE ENTRY ALWAYS ON TOP )
============================================================================
DATE:       SCR#:   TASKID: INIT DESCRIPTION
-------     ------  ------- ---- ------------------------------------------------
25APR2018   US446   v8617    FG  Capture cur_id correctly for values in Personitin.fare_by_leg
13OCT2017   US498   v8619    FG  Capture commission rate for a hotel segment from Sabre interface record
16MAY2012   509845  c0572    WW  Process void ticket for CWT US M&G
============================================================================
*/

class CIUR : public CInterfaceRecord
{
   public:
      void createCIUR(CPNR &po_PNR) ;
      void printAll() ;
		CIUR () ; 
      ~CIUR() {cout << "OBJECT CIUR DESTROYED" << endl ;}

      int gi_IrHotelIndex ;
      V_AB_Varchar gv_IrHotelData ;

      int mi_IrCarIndex ;
      V_AB_Varchar gv_IrCarData ;

      int mi_TransportIndex ;
      V_AB_Varchar gd_TransportData ;

      int mi_tktIndex ;
      V_AB_Char md_tktData ;

		int mi_entitleIndex ;
		V_AB_Varchar gd_entitleData ;
		AB_Varchar			mv_ticketM5 ;

		int mi_accountIndex ;
		V_AB_Varchar md_accountingData ;

		int mi_NonArcItin ;
		V_AB_Varchar md_NonArcItin ;

		int mi_IgnoredItin ;
		V_AB_Varchar md_IgnoredItin ;

		int    mi_tktCounter ;

		AB_Varchar mv_itinTransport ; // Assigned for PNRS having no pax and itin relationship

      AB_Varchar mv_paxLst;
		AB_Char    mc_itemType;

	   D_form_of_payment      md_orig_form_of_pay; 
		D_credit_card_sup      md_orig_cc_id;
		D_credit_card_no       md_orig_cc_no;
      D_expiry_date          md_orig_cc_exp_date;
      D_authorization        md_orig_auth_no;
      D_authorization_source md_orig_auth_source;

   protected:

		AB_Boolean        mb_address ;
		D_document_number md_invoiceNo ;
		AB_Char				mc_iurType ;
		AB_Boolean			mb_exchange ;
		AB_Boolean			mb_buildCfa ;

		AB_Integer			mi_noofPax ;
		AB_Integer			mi_noofentitle ;

		D_saving_code		md_savingCode ;
		D_char_numeric		md_fullFare;
		D_char_numeric		md_lowPracFare;
		D_char_numeric		md_rebate ;
		AB_Char           mv_service_status;

		V_AB_Varchar		mva_M5AccSection ;
		int               mi_M5AccIndex ;
		int               mi_XDLine ;

		V_AB_Varchar		mva_M6FareSection ;
		int               mi_M6FareIndex ;

      // GAP US089 START
      V_AB_Varchar      mva_MERemarkSection;
      int               mi_MERemarkIndex;

      AB_Varchar        mv_lastIOName;
      // GAP US089 END

		//GAP US092 START
		AB_Varchar        mv_lastTourCity;
		//GAP US092 END

		AB_Varchar        mv_lastShipCity;
		AB_Varchar        mv_lastLandCity;

		// Indicator to say if it is an automates Mco/PTA/Tour order
		AB_Boolean		   mb_MPT ; 	
								
		// Indicator to say if it is net fare ticket 
		AB_Boolean		   mb_netFareTicket ; 	

		void errorHandler(CPNR &po_PNR);
		void fill_IrHeader(CPNR &po_PNR);
		void populate_IrHeader_SectionM0(AB_Text &pt_total_segment);
		void populate_IrAddress(AB_Text &pt_total_segment);
		void parsePhone(AB_Varchar  &pv_long_phone) ;
		void populate_IrHeader_SectionM9(CPNR &po_PNR);
		void populate_IrHeader_SectionM8(CPNR &po_PNR);
		AB_Boolean  chectNT_getCFA(CPNR &po_PNR);
		void fill_IrItinerary(CPNR &po_PNR) ;
		void fillIrTransportM3(AB_Text &pt_segment, CPNR &po_PNR);
		void fillConsumDetail( AB_Char &pc_noofSeats,
								 		AB_Char     &pc_itinType,
								 		AB_Char     &pc_itinLine,
								 		AB_Text     &pt_varyAirData,
										AB_Text		&pc_ftpMiles) ;
		void fill_IrPerson(CPNR &po_PNR);
		void updConsumDetailSeat(AB_Char  &pc_paxId,
							 		AB_Text     &pt_itinPax) ;
		void processTicket(CPNR &po_PNR);
		void fillTicketFormM2(CPNR &po_PNR);
		/* void fillTicketFormM2andM5(AB_Char &pc_pax,CPNR &po_PNR); */
		/*
		void fillExchangeTkt(AB_Integer	&pi_paxId,
								AB_Integer	   &pi_docLine,
								CPNR &po_PNR) ;
								*/

// SCR #117474 START
      void getXFbreakdown( CPNR &po_PNR,
                           D_amount    &pd_totalXFXTInM2,
                           D_amount    &pd_totXFTaxes,
                           AB_Char     &pc_fareCalcNo);
// SCR #117474 END
		void processOrigFare(AB_Char   	&pc_paxId,
								  AB_Char      &pc_docLine,
								  CPNR         &po_PNR,
								  D_amount     &pd_totTax,
								  D_amount     &pd_IU2PEN) ;
		void fillTicketFormM5(CPNR &po_PNR);
		void parseM5ticket(AB_Varchar   &pv_tktString,
								 V_AB_Varchar &pva_arrayField,
								 AB_Integer   &pi_arraySize,
								 AB_Integer   &pi_paxId,
								 AB_Integer   &pi_docId) ;
		void processTax(CPNR       &po_PNR ,
							AB_Varchar	&pv_taxString,
							D_amount    &pd_totTax,
							AB_Char     &pc_fareCalcNo) ;
		void fillFOPM2(AB_Integer &pi_paxId,
									AB_Integer &pi_docLine,
									AB_Text    &pt_total_segment) ;
		void fillFOPM5(AB_Integer &pi_paxId,
							AB_Integer &pi_docLine,
							AB_Varchar &pt_formOfPay,
							AB_Char    &pc_ccExpDate);
		void translate_fp(AB_Varchar  &pc_fp,AB_Char  &pc_secType);
		void updIrconsumdetailTkt(AB_Integer	&pi_noofM4,
										AB_Text		&pt_entitleNos,
										AB_Char		&pc_paxId,
										AB_Char		&pc_docId,
										AB_Char		&pc_docPic,
                              AB_Varchar  &pc_cur_id) ; //FG - US446
		void fill_IrRentalHotel(AB_Text &pt_section);
		void fill_IrRentalHotelAuto(AB_Text &pt_section);
		void processCommonHotel(AB_Text &pt_section);
		void getAddressData(AB_Text &pt_section);
		void getSpecialData(AB_Text &pt_section);
		void getOverrideRates(AB_Char &pc_itin_type, 
											D_char_date		&pd_start_date, 
											D_saving_code	&pd_saving_code,
		             					D_char_numeric &pd_local_rate, 
											D_char_numeric &pd_regular_rate,
		             					D_char_numeric &pd_booked_rate, 
											AB_Char			&pd_commissionable,
											AB_Char        &pd_preferred,
											V_AB_Text		&pd_Data, 
											int				pi_Index,
                                 AB_Char        &pc_comm_rate) ;
		void getRateOverride(AB_Varchar &pv_data, AB_Varchar &lc_delim, D_char_numeric &pm_amount);
		void fill_IrRentalHotelManual(AB_Text &pt_section);
		void CreateRentalRemarks(AB_Varchar &pv_field);
		void fill_IrRentalCar(AB_Text &pt_section);
		void IrCarRemarks(AB_Varchar &pv_field);

      // GAP US089 START
      void createM8Remarks(CPNR &po_PNR);
      void parseOtherSegment( AB_Text     &pt_section,
                              AB_Boolean  &pb_dupFound);
		void processOtherAssociatedRemarks(int pi_eff_IO_index);
      // GAP US089 END

		// GAP US111 START
		AB_Boolean processNonMatrixUDID(CPNR &po_PNR);
		// GAP US111 END

		void storeEntitleData(CPNR &po_PNR);
		void parseRate(AB_Text &pt_section, AB_Varchar &pm_start_delim, D_currency &pm_cur_id, D_amount &pm_amount, AB_Varchar &pt_field) ;
		void getOtherCarRateInfo(AB_Varchar &pv_field) ;

		void updIcdConnectInd() ;

		void fillIrRailM3(AB_Text &pt_segment, CPNR &po_PNR) ;
		void fill_IrBusM3(AB_Text &pt_section) ;
		bool fill_IrOtherM3(AB_Text &pt_section) ;

		// GAP US092 START
		void fill_IrShipM3(AB_Text &pt_section);
		void parseShipSegment(  AB_Text     &pt_section, AB_Boolean  &pb_dupFound);
		void fill_IrTourM3(AB_Text &pt_section);
		void parseTourSegment(  AB_Text     &pt_section, AB_Boolean  &pb_dupFound);
		void processMiscellaneousFee(AB_Text &pt_data, AB_Integer &pi_paxId, AB_Char &pc_ccExpDate);
		// GAP US092 END

		//GAP US086 START
		void create_SFN_Remark(AB_Boolean &pb_FR_found);
		// GAP US086 END

		void storeAccountingData(CPNR &po_PNR) ;
		void getSupId(AB_Char  &pc_M5link,
						  D_supplier &pd_supId) ;
		void getItinType(AB_Char  &pc_productType,
							  AB_Char  &pc_itinType,
							  AB_Char  &pc_unitType) ;
		AB_Char checkM2tkt(AB_Char  &pc_M5tkt) ;
		AB_Boolean scanTaxesAndOPAfields(V_AB_Varchar &pva_arrayField, 
													int tax1Ind, 
													int& tax2Ind, 
													int& tax3Ind, 
													int& OPAind);
		void parseM5NonArcTicket(AB_Varchar   &pv_tktString,
										 V_AB_Varchar &pva_arrayField,
								       AB_Integer   &pi_arraySize,
								       AB_Integer   &pi_paxId,
								       AB_Integer   &pi_docId) ;
		void updIrconsdetM5Tkt(AB_Char      &pc_M5link,
									  AB_Char		&pc_paxId,
									  AB_Char		&pc_docId) ;

		void updFareInfo() ;

		void fillM5Acc(CPNR &po_PNR) ;

		void fillM6FareCalc(CPNR &po_PNR) ;
      void processFrequentF(CPNR &po_PNR,AB_Varchar &pc_full_name)  ;
		AB_Boolean checkForDuplicateFF(int  pi_start,
												 const AB_Char  &pc_sup, 
												 const AB_Char&,
												 const AB_Varchar&) const;

		AB_Boolean existMatchingM5(AB_Char &pc_paxId,
						  		         AB_Char &pc_docLine) ;

		void createXDItin() ;
		void createEMDItin() ;

      void getTotM5Taxes(V_AB_Varchar  &pva_arrayField,
								D_amount      &pd_totTax ) ;

      D_supplier gm_sup_id ;
		void processM5SvcFee(AB_Text  &pt_section) ;
		void getOrigFop(AB_Text &pt_M2);
		void saveSvcIrregularity();
		// US044 START
      void IrTransportRemarks(CPNR &po_PNR) ;
		// US044 END

		void parseM8SavingCodeFullFare(CPNR &po_PNR);

      // GAP US089 START
      void fillMERemark(CPNR &po_PNR);
      // GAP US089 END

		// SCR 231339 START
		void updateTransportSupplier();
		// SCR 231339 END

		// G9090c START
		void validateItin();
		bool isLinkedItinIgnored(const AB_Char&);
		// G9090c END
		/* SCR 333749 */
		void processEmailaddress( CPNR &);

		void fillIrStopovercity(
			D_service_type	&pc_itinType,
			D_line_number	&pc_itinLine,
			AB_Text			&pc_cities );

		// QREX START
		void fillRefundFormM5(CPNR &po_PNR);
		// QREX END

	   void processVoidTicket(CPNR &po_PNR);	

		  // parsing MA sections for OB fees
		void processAirlineTicketingFee(CPNR &po_PNR);

	void RP_parseRemarks(CPNR &po_PNR);

	void parseCouponList(AB_Varchar &list, AB_Text &tkt, D_char_numeric &no_of_books);
 
	//Gap G9139 Limo Segment
   void fill_IrRentalLimo(AB_Text &pt_section );
	void IrLimoRemark( AB_Varchar &pv_field );

	void fill_IrLandM3(AB_Text &pt_section);
	void parseLandSegment(AB_Text &pt_section, AB_Boolean  &pb_dupFound);

} ;

