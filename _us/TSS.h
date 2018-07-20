/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.21
      Delta Created: 14/07/11 - 13:48:51
         Last Delta: app/src/common/InterfaceRecord/_us 7/11/14 - 13:48:51 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TSS.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/


class CTSS : public CInterfaceRecord
{
   public:
      void createCTSS(CPNR &po_PNR)  ;
      void printAll() ;
      CTSS() ;
      ~CTSS() {cout << "OBJECT CTSS DESTROYED" << endl ;}

      int gi_IrHotelIndex ;
      V_AB_Varchar mv_IrHotelData ;

      int mi_IrCarIndex ;
      V_AB_Varchar mv_IrCarData ;

      int mi_manualTicketIndex ;
      V_AB_Varchar mv_manualTicketData ;

      int mi_exchTaxIndex ;
      V_AB_Varchar mv_exchTaxData ;

      int          mi_nonArcDoc ;
      V_AB_Varchar md_nonArcDoc ;
      V_AB_Varchar md_nonArcCost ;
		AB_Varchar   mv_a14CC ;

		int          mi_tktCounter ;
		int			 mi_SFC_line_number;

	   D_form_of_payment      md_orig_form_of_pay; // Got from A10
		D_credit_card_sup      md_orig_cc_id;
		D_credit_card_no       md_orig_cc_no;

      // Service FEE Variable

      AB_Boolean       mb_SfFound ;    // Check to see if SF-  field is present
      AB_Boolean       mb_FeeFound ;   // Check to see if FEE- field is present
      AB_Varchar       mv_svcSF  ;     // Holding SF-  data
      AB_Varchar       mv_svcFEE ;     // Holding FEE- data

	protected:

		AB_Boolean        mb_ignoreMir ;
      D_supplier        gm_sup_id ;

		D_ticket_reference md_selAccess ;   // Goes for Irticket
		D_supplier			 md_supId ;       // Goes for Irticket
		D_yes_no  			 md_personalCrs ; // Goes for Irticket
		D_short_name		 md_crsTour ;     // Goes for Irticket
		D_yes_no		       md_e_ticket ;    // Goes for Irticket
		D_city		       md_stpHomeCity ; // Goes for Irticket
		D_city		       md_stpHostCity ; // Goes for Irticket
		D_yes_no	          md_crsPriced ;   // Goes For Irconsumdetail


		D_credit_card_sup      md_cc_id; // Got from A11
		D_credit_card_no       md_cc_no;
		D_expiry_date          md_cc_exp_date;
		D_authorization        md_auth_no;
		D_authorization_source md_auth_source;
		D_form_of_payment      md_form_of_pay;
		D_form_of_payment      md_A11FOP;
		AB_Boolean             mb_address;
		D_saving_code          md_savingCode ; // For Irticket
		D_char_numeric         md_full_fare;
		D_char_numeric         md_low_prac_fare;
		D_char_numeric         md_rebate;
		AB_Varchar             mv_baggage ;
		AB_Integer             mi_ftp_miles ;
		AB_Varchar             mv_service_status ;
		AB_Text                mv_NonArcCC ;

		AB_Boolean             mb_foundFare ;
		AB_Text					  mt_A07_segment ;
		
		AB_Text					  mt_UDID;

		void errorHandler(CPNR &po_PNR);
		void fill_IrHeader(CPNR &po_PNR);
		void populate_IrHeader_SectionHDR(AB_Text &pt_total_segment);
		void storeFormOfPay(AB_Text &pt_total_segment);
		void fill_Irpayment(AB_Text    &pt_total_segment,
										  AB_Integer &pi_paxId,
										  AB_Integer &pi_docLine);
		void fill_Iraddress(AB_Varchar &pv_long_address,
										  CPNR       &po_PNR);
		void populate_Iraddress_SectionA12(CPNR &po_PNR);
		void populate_IrHeader_SectionA14(AB_Varchar &pv_long_address, 
															 CPNR &po_PNR);
		void populate_Iraddress_SectionA13(AB_Varchar &pv_long_address, 
																CPNR &po_PNR);
		void setShortRemarks(AB_Text pt_remark);
		void fill_IrRentalandOther(CPNR &po_PNR) ;
		void fill_IrRentalHotel(AB_Text &pt_section);
		void IrHotelRemarks(AB_Varchar &pv_field);
		void CreateRentalRemarks(AB_Varchar &pv_field);
		void getRateData(AB_Text &pt_section, 
									  AB_Char &pc_itin_type, 
									  D_char_date &pt_start_date,
		             			  D_saving_code &pd_saving_code, 
									  D_currency &pd_cur_id, 
									  D_char_numeric &pd_local_rate,
		             			  D_char_numeric &pd_regular_rate, 
									  D_char_numeric &pd_booked_rate, 
		                       V_AB_Text &pd_Data, int pi_Index);
		void parseRate(AB_Text &pt_section, 
									AB_Char &pc_start_delim, 
									D_currency &pd_cur_id, 
									D_amount &pd_amount, 
									AB_Varchar &pt_field);
		void getOverrideRates(AB_Char &pc_itin_type, 
											D_char_date &pt_start_date, 
											D_saving_code &pd_saving_code,
		             					D_char_numeric &pd_local_rate, 
											D_char_numeric &pd_regular_rate,
		             				   D_char_numeric &pd_booked_rate, 
											V_AB_Text &pd_Data, int pi_Index);
		void getRateOverride(AB_Varchar &pv_data, 
											int     pi_pos, 
											D_char_numeric &pd_amount);
		void getAddressData(AB_Text &pt_section);
		void fill_IrRentalCar(AB_Text &pt_section);
		void get_car_data(AB_Varchar &pv_field);
		void IrCarRemarks(AB_Varchar &pv_field);
		void fill_IrOther(AB_Text &pt_section);
		void processOtherRemark(AB_Text &pt_section);
		void updateLLData();
		void fill_IrPasDocument(CPNR &po_PNR) ;
		D_pic getPicCode(AB_Text &pc_pic);
		AB_Boolean processFare(	CPNR &po_PNR, 
								AB_Text &pt_section, 
								int &pi_pax_id ,
								AB_Char  &pc_docLine)  ;
		void processTax(AB_Text   &pt_section,
							 D_amount  &pd_totTax) ;
		void fillIrDocumentTax(D_char_numeric &pd_taxAmount, D_tax &pd_taxId);
		void processOrigFare(CPNR        &po_PNR, 
									AB_Char     &pc_exch_num, 
									AB_Char     &pc_docLine,
									AB_Char     &pc_exch_type) ;
		void processExchangeTaxes(	V_AB_Integer &pia_exch_pax_id ,
		            							V_AB_Varchar   &pva_exch_tax_id ,
		            							V_AB_Varchar   &pva_exch_tax_amount,
		            							int            &pi_exchangeTax_index) ;
		void calculateExchangeFare(D_char_numeric &pm_exchBaseCost, 
													D_char_numeric &pm_exchCommission);
		void calculateExchangeTax(AB_Integer     &pi_paxId,
										  V_AB_Varchar   &pva_exchTaxId,
										  V_AB_Varchar   &pva_exchTax) ;
		void processFrequentFlyer(CPNR &po_PNR, AB_Text &pm_name , int &pi_pax_id);
		AB_Boolean LLUpdateIrperson(CPNR &po_PNR) ;
		void createIrItins(CPNR &po_PNR);
		void populateIrTransportSectionA04(CPNR &po_PNR);
		void populateIrTransportSectionA05(CPNR &po_PNR);
		void commonIrTransportFields(AB_Text &pt_segment);
		void IrTransportRemarks(CPNR &po_PNR);
		void fillIrconsumdetail(CPNR &po_PNR);
		void populateIrconsumdetailSectionA06(CPNR &po_PNR);
		void populateIrconsumdetailSectionA04(CPNR &po_PNR);
		void updateConsumdetail();

		void processNonArcDoc();
		void processNonArcCost(AB_Text  &pt_section);
		void getItinType(AB_Char  &pc_productType,
							  		AB_Char  &pc_itinType,
							  		AB_Char  &pc_unitType) ;
		void addIrconsumdetail( D_supplier     &pd_supId,
												AB_Char        &pc_itinType,
												AB_Integer     &pi_itinLine) ;
		void fill_IrBus(AB_Text &pt_section) ;
		void fillIrRail(AB_Text &pt_segment) ;

		void fillTktFP(AB_Text &pv_CCInfo)  ;
		void parsePhone(AB_Varchar  &pv_long_phone) ;
		void updateTaxdetail() ;
		void fillIrconsumDetailRental(AB_Char    &pc_itinType,
                                    AB_Integer &pi_itinLine,
                                    AB_Char    &pc_status,
												AB_Varchar &pv_duePaid) ;
		void checkRental() ;
		void processTaxExtraTicketLine(int &pi_tktLine) ;
		void processExtraTicketLine() ;
		void condense_IrDocumentTax(D_passenger_no  &pd_pax_id, 
											 D_line_number   &pd_doc_line,
											 int &pi_taxCounter) ;
      void processSFC() ;
      void saveSvcIrregularity() ;
		void parseXmlUdid();
} ;

