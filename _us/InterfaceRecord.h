/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.123
      Delta Created: 14/05/15 - 10:38:39
         Last Delta: app/src/common/InterfaceRecord/_us 5/15/14 - 10:38:39 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.InterfaceRecord.h)

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
17MAR2014   516409  V7848    WW  parsing limo segment
24OCT2013   514016  c1257    JL  retrofit version updated.
11SEP2013   514016  c1195    JL  adding MB code for mobile booking.
16MAY2012   509845  c0572    WW  Process void ticket for CWT US M&G
============================================================================
*/

#ifndef InterfaceRecord_H
#define InterfaceRecord_H


#include <string>
#include <set>

using namespace std;

#include "AirlineTicketingFee.h"
#include "RemarkParser.h"

class Text2Text
{
	public:
		Text2Text(const AB_Text& key, const AB_Text& value):_key(key),_value(value){}

		bool operator<( const Text2Text& other ) const
		{
		  //if(s1._key < s2._key )
		  if(strcmp(_key.to_string(),other._key.to_string()) < 0 )
				return true;
			else
				return false;
		}

		AB_Text   _key;
		AB_Text _value;
};

class Text2Decimal
{
	public:
		Text2Decimal(const AB_Text& key, const AB_Decimal& value):_key(key),_value(value){}
		Text2Decimal(const AB_Text& key):_key(key),_value(0.00){}

		bool operator<( const Text2Decimal& other ) const
		{
		  //if(s1._key < s2._key )
		  if(strcmp(_key.to_string(),other._key.to_string()) < 0 )
				return true;
			else
				return false;
		}

		void add(AB_Decimal value){_value = _value + value;}
		void add(AB_Varchar value){_value = _value + AB_Decimal(value);}

		static void add(set <Text2Decimal> &_set, const AB_Text key, const AB_Decimal value);
		//static void add(set <Text2Decimal> &_set, const AB_Text key, const AB_Text value);
		//static void add(set <Text2Decimal> &_set, const AB_Varchar key, const AB_Varchar value);
		static string toString(string title, set <Text2Decimal> &_set);

		AB_Text   _key;
		AB_Decimal _value;
};



class CInterfaceRecord
{
	friend class CAirlineTicketingFee;

 public:
	enum ItinType {
	  NO_TYPE,
	  HOTEL,
	  CAR,
	  TRANSPORT,
	  RAIL,
	  BUS,
	  SHIP,
	  TOUR,
	  LIMO,
	  INSURANCE,
	  LAND
	};

   private:

   protected :
	D_txn_identifier md_txn_id;       //1309-30SEP98-SC --added TranId to Ticket
   D_session_id     md_session_id;
   D_interface_type md_interface_type;
   D_update_date    md_creation_date;

	//Reject Company, Branch and con_id
   D_consultant     md_rejectCon;
   D_company        md_rejectCop;
   D_branch         md_rejectBra;

	// Client file After creation which will be used in TRM

   D_client_file_type     md_cfa_type;
   D_client_file_code     md_cfa_code;
   D_client_file_no       md_cfa_id;
   D_person_name          md_last_name;
   D_person_name          md_first_name;
   D_consultant           md_con_id;

	// G145 - Surfare Transport Creation
	V_AB_Varchar           mva_surfaceTransport ;
	int						  mi_surfaceTransport ;

	// Instead of BAD DATABASE CALL give a specific message whever possible
	AB_Integer             mi_syb_code;
	AB_Text                mv_syb_message ;

   // Irtaxdetail Info
   D_passenger_no md_ITD_pax_id[500];
   D_line_number  md_ITD_line_number[500];
   D_tax          md_ITD_tax_id[500];
   D_char_numeric md_ITD_tax_amount[500];
   D_city         md_ITD_city_id[500];
	AB_Text			md_ITD_tax_source[500];

   // Irpayment Info
   D_passenger_no         md_IRP_pax_id[500];
   D_line_number          md_IRP_line_number[500];
   D_line_number          md_IRP_pay_id[500];
   D_char_numeric         md_IRP_amount[500];
   D_form_of_payment      md_IRP_form_of_pay[500];
   D_credit_card_sup      md_IRP_cc_id[500];
   D_credit_card_no       md_IRP_cc_no[500];
   D_expiry_date          md_IRP_cc_exp_date[500];
   D_authorization        md_IRP_auth_no[500];
   D_authorization_source md_IRP_auth_source[500];


   // TKX Expiry Date

   D_expiry_date          md_TKX_cc_exp_date;


	// Ebooking activity info
	AB_Varchar             mv_actionCode;
	AB_Varchar             mv_source;
	AB_Varchar             mv_reasonCode;

	// Irtripheader Info
   D_company              md_IRT_cop_id;
   D_branch               md_IRT_bra_id;
   D_iata_no              md_IRT_iata_no;
   D_crs                  md_IRT_crs_id;
   D_client_file_type     md_IRT_cfa_type;
   D_client_file_code     md_IRT_cfa_code;
   D_char_numeric         md_IRT_cfa_id;
   D_consultant           md_IRT_con_id;
   D_iata_destination     md_IRT_iata_dest;
   D_city                 md_IRT_city_id;
   D_char_date            md_IRT_pnr_date;
   D_char_time            md_IRT_pnr_time;
   D_char_date            md_IRT_start_date;
   D_reference            md_IRT_cfa_dpt;
   D_marketing_list       md_IRT_mktg_list;
   D_reference            md_IRT_reference;
   D_reference            md_IRT_booking_no;
	// GAP G9123 SCR 440488 START
	AB_Varchar				  md_IRT_booking_pcc;
	AB_Varchar				  md_IRT_booking_gds_agent;
	AB_Varchar				  md_IRT_ticketing_pcc;
	AB_Varchar				  md_IRT_ticketing_gds_agent;
	// GAP G9123 SCR 440488 END
   D_char_date            md_IRT_record_creation_date;
   D_char_time            md_IRT_record_creation_time;

	// GAP US107 START
	D_user_id				  md_IRT_ticketer_user_id;
	AB_Varchar				  mv_IRT_employee_number;
	// GAP US107 END

   // Iraddress
	/* Change
   D_address_type         md_IA_type;
	*/
   AB_Char                md_IA_type[500];
   D_address_line         md_IA_addr1[500];
   D_address_line         md_IA_addr2[500];
   D_address_line         md_IA_addr3[500];
   D_address_line         md_IA_addr4[500];
   D_address_line         md_IA_addr5[500];
   D_address_line         md_IA_addr6[500];
   D_area_code            md_IA_phone_acode[500];
   D_phone_number         md_IA_phone_no[500];
   D_area_code            md_IA_bphone_acode[500];
   D_phone_number         md_IA_bphone_no[500];
   D_area_code            md_IA_mphone_acode[500];
   D_phone_number         md_IA_mphone_no[500];
   D_area_code            md_IA_fax_acode[500];
   D_phone_number         md_IA_fax_no[500];

   // Irperson
   D_passenger_no         md_IP_pax_id[500];
   D_title                md_IP_title[500];
   D_person_name          md_IP_last_name[500];
   D_person_name          md_IP_first_name[500];
   D_passenger_no         md_IP_new_pax_id[500];
   D_passenger_no         md_IP_new_pax_line[500];
   D_sex                  md_IP_sex[500];
   D_yes_no               md_IP_smoking[500];
   D_country              md_IP_ctry_id[500];
   D_passport_no          md_IP_passport_no[500];
   D_char_date            md_IP_passport_exp[500];
   D_reference            md_IP_reference[500];
   D_service              md_IP_crs_invoice[500];
   D_line_number          md_IP_traveller_id[500];
   AB_Varchar             md_IP_nameNumber[500];

   // Irticket
   D_passenger_no         md_ID_pax_id[500];
   D_line_number          md_ID_line_number[500];
   D_line_number          md_ID_cost_line[500];
   D_reference            md_ID_reference[500];
   D_supplier             md_ID_sup_id[500];
   D_currency             md_ID_cur_id[500];
   D_ticket_form          md_ID_form[500];
   D_ticket_reference     md_ID_ticket_no[500];
   D_pic                  md_ID_pic[500];
   D_ticket_reference     md_ID_orig_ticket_no[500];
   D_char_numeric         md_ID_comm_rate[500];
   D_char_numeric         md_ID_commission[500];
   D_char_numeric         md_ID_no_of_books[500];
   D_exchange_code        md_ID_exchange_code[500];
   D_char_numeric         md_ID_penalty_ch_fee[500];
   D_char_numeric         md_ID_comm_penalty[500];
   D_char_numeric         md_ID_base_cost[500];
   D_saving_code          md_ID_saving_code[500];
   D_char_numeric         md_ID_full_fare[500];
   D_char_numeric         md_ID_low_prac_fare[500];
   D_yes_no               md_ID_personal_crs[500];
   D_city                 md_ID_stp_home_city[500];
   D_city                 md_ID_stp_host_city[500];
   D_ticket_reference     md_ID_sel_access[500];
   D_short_name           md_ID_crs_tour[500];
   D_char_numeric         md_ID_rebate[500];
   D_document_type        md_ID_doc_type[500];
   D_yes_no               md_ID_e_ticket[500];
   D_remark               md_ID_association[500];
	// QREX START
	D_yes_no						md_ID_refund[500];
	// QREX END

	// GAP US107 START
	AB_Char 	md_ID_commission_type;
	D_yes_no md_ID_net_ticket_ind;
	AB_Char 	md_ID_net_ticket_type;

	D_char_numeric md_ID_nett_fare_to_airline;
	D_char_numeric md_ID_selling_price_to_shop;
	D_char_numeric md_ID_selling_price_to_client;
	D_char_numeric md_ID_tour_oper_commission;

	D_char_numeric md_ID_publish_fare_tc_commission;
	D_char_numeric md_ID_publish_fare_agent_commission;

	D_reference md_ID_ticket_centre_reference;
	D_reference md_ID_grid_reference;
	D_reference md_ID_shop_reference;
	// GAP US107 END

   // Irremark
   D_line_number  md_IRMK_line_number[500];
   D_passenger_no md_IRMK_pax_id[500];
   D_segment_type md_IRMK_type[500];
   D_remark       md_IRMK_remark[500];
   D_service_type md_IRMK_itin_type[500];
   D_line_number  md_IRMK_tra_line_number[500];
   D_line_number  md_IRMK_ren_line_number[500];
   D_line_number  md_IRMK_oth_line_number[500];
   D_line_number  md_IRMK_udi_line_number[500];

   //Irtransport
   D_service_type   md_IT_itin_type[500];
   D_line_number    md_IT_line_number[500];
   D_char_date      md_IT_start_date[500];
   D_char_time      md_IT_start_time[500];
   D_city           md_IT_orig_city_id[500];
   D_name           md_IT_orig_cityname[500];
   D_service        md_IT_chg_of_gauge[500];
   D_city           md_IT_cog_dep_city[500];
   D_char_date      md_IT_cog_dep_date[500];
   D_char_time      md_IT_cog_dep_time[500];
   D_terminal       md_IT_cog_dep_term[500];
   D_gate           md_IT_cog_dep_gate[500];
   D_free_mileage   md_IT_cog_of_miles[500];
   D_char_date      md_IT_end_date[500];
   D_char_time      md_IT_end_time[500];
   D_city           md_IT_dest_city_id[500];
   D_name           md_IT_dest_cityname[500];
   D_unit_type      md_IT_classofservice[500];
   D_service        md_IT_service[500];
   D_supplier       md_IT_sup_id[500];
   D_reference      md_IT_reference[500];
   D_char_numeric   md_IT_no_of_stops[500];
   D_terminal       md_IT_dep_terminal[500];
   D_gate           md_IT_dep_gate[500];
   D_time           md_IT_report_time[500];
   D_gate           md_IT_arr_gate[500];
   D_terminal       md_IT_arr_terminal[500];
   D_carrier_type   md_IT_carrier_type[500];
   D_meal_code      md_IT_meal[500];
   D_name           md_IT_aff_carrier[500];
   D_counter        md_IT_actual_miles[500];
   D_arrival_ind    md_IT_arrival_ind[500];
   D_time		     md_IT_elapsed_time[500];
   D_terminal       md_IT_airline_locator[500];

   //IRfrequestuser
   D_passenger_no md_IRF_pax_id[500];
   D_supplier     md_IRF_sup_id[500];
   D_plan_type    md_IRF_fup_type[500];
   D_reference    md_IRF_fup[500];
   D_service_status	md_IRF_fup_status[500];
   D_supplier     md_IRF_srvc_sup_id[500];

   //Irother
   AB_Text        md_IO_prod_code[500];
   D_service_type md_IO_itin_type[500];
   D_line_number  md_IO_line_number[500];
   D_char_date    md_IO_start_date[500];
   D_char_time    md_IO_start_time[500];
   D_city         md_IO_orig_city_id[500];
   D_char_date    md_IO_end_date[500];
   D_char_time    md_IO_end_time[500];
   D_city         md_IO_dest_city_id[500];
   D_unit_type    md_IO_unit_type[500];
   D_counter      md_IO_unit_ct[500];
   D_supplier     md_IO_sup_id[500];
   D_reference    md_IO_reference[500];
   D_saving_code  md_IO_saving_code[500];
   // GAP US089 START
   D_reference    md_IO_confirmation[500];
   // GAP US089 END
	// GAP US092 START
	D_name			md_IO_departure_point[500];
	D_name			md_IO_arrival_point[500];
	D_name			md_IO_svc_description[500];
	D_name			md_IO_cabin_no[500];
	D_name			md_IO_tour_no[500];
	// GAP US092 END

   //Irconsumdetail
   D_passenger_no      md_ICD_pax_id[500];
   D_service_type      md_ICD_itin_type[500];
   D_line_number       md_ICD_line_number[500];
   D_line_number       md_ICD_itin_line_number[500];
   D_seat              md_ICD_seat[500];
   D_connection        md_ICD_connection[500];
   D_yes_no            md_ICD_smoking[500];
   D_char_numeric      md_ICD_fare_by_leg[500];
   D_fare_basis        md_ICD_fare_basis[500];
   AB_Text          	  md_ICD_tkt_desig[500];
   D_yes_no            md_ICD_cog_seat[500];
   D_baggage_code      md_ICD_baggage[500];
   D_yes_no            md_ICD_crs_priced[500];
   D_service_status    md_ICD_service_status[500];
   D_char_numeric      md_ICD_ftp_miles[500];
   D_yes_no            md_ICD_boarding_pass[500];
   D_meal_code         md_ICD_special_meal[500];
   D_meal_code         md_ICD_special_need[500];
   AB_Text             md_ICD_text[500];
   D_currency          md_ICD_cur_id[500];

   //Irrental
   // SCR: 514016
	AB_Text             md_IR_Itin_Hotel_Data[500];


   AB_Text        	  md_IR_prod_code[500];
   D_service_type      md_IR_itin_type[500];
   D_line_number       md_IR_line_number[500];
   D_char_date         md_IR_start_date[500];
   D_char_time         md_IR_start_time[500];
   D_city              md_IR_origin_city_id[500];
   D_char_date         md_IR_end_date[500];
   D_char_time         md_IR_end_time[500];
   D_city              md_IR_dest_city_id[500];
   D_unit_type         md_IR_unit_type[500];
   D_char_numeric      md_IR_unit_ct[500];
   D_service_status    md_IR_service_status[500];
   D_supplier          md_IR_sup_id[500];
   D_product_source    md_IR_source[500];
   D_counter           md_IR_number_of_pax[500];
   D_char_numeric      md_IR_ex_day_charge[500];
   D_char_numeric      md_IR_ex_hr_charge[500];
   D_char_numeric      md_IR_ex_mile_charge[500];
   D_free_mileage_code md_IR_free_mile_code[500];
   D_free_mileage      md_IR_free_mileage[500];
   D_char_numeric      md_IR_dropoff_charge[500];
   D_char_numeric      md_IR_regular_rate[500];
   D_char_numeric      md_IR_booked_rate[500];
   D_char_numeric      md_IR_harp_htlkey[500];
   AB_Char     		  md_IR_commissionable[500];
	AB_Char             md_IR_preferred[500];
   D_char_numeric      md_IR_local_rate[500];
   D_currency          md_IR_cur_id[500];
   D_rate_type         md_IR_rate_type[500];
   D_rate_code         md_IR_rate_code[500];
   D_rate_duration     md_IR_rate_duration[500];
   D_char_numeric      md_IR_rate_multip[500];
   D_char_numeric      md_IR_com_rate[500];
   D_multi_level_rate  md_IR_multi_lvl_rate[500];
   D_saving_code       md_IR_saving_code[500];
   D_reference         md_IR_location[500];
   D_location_id       md_IR_location_id[500];
   D_name              md_IR_site_name[500];
   D_address_line      md_IR_addr1[500];
   D_address_line      md_IR_addr2[500];
   D_address_line      md_IR_addr3[500];
   D_address_line      md_IR_addr4[500];
   D_phone_number      md_IR_phone_acode[500];
   D_phone_number      md_IR_phone_no[500];
   D_phone_number      md_IR_fax_acode[500];
   D_phone_number      md_IR_fax_no[500];
   D_reference         md_IR_fup[500];
   D_reference         md_IR_customer_id[500];
   D_reference         md_IR_guarantee[500];
   D_reference         md_IR_confirmation[500];
   D_reference         md_IR_reference[500];
   D_reference         md_IR_corporate_disc[500];
   D_reference         md_IR_corporation_id[500];
   D_reference         md_IR_cancel_policy[500];
   AB_Text        	  md_IR_pickup_location[500];
   AB_Text        	  md_IR_dropoff_location[500];
   AB_Text        	  md_IR_discount_code[500];
   AB_Text             md_IR_action_code[500];
   AB_Text				  md_IR_EB_source[500];
   AB_Text             md_IR_reason_code[500];

   AB_Text             md_IR_virtualCard_cc[500];
   AB_Text             md_IR_virtualCard_exp_date[500];
   AB_Text             md_IR_virtualCard_deployment[500];
   AB_Text             md_IR_virtualCard_card_usage_id[500];
   AB_Text             md_IR_virtualCard_gross_value_ind[500];
   AB_Text             md_IR_pcc_cur_id[500];

	int mi_receiptIndex ;
	V_AB_Varchar gd_receiptType ;
	V_AB_Varchar gd_receiptData ;

   //Irreceipt
   D_company           md_RC_cop_id;
   D_branch            md_RC_bra_id;
   D_lang_id           md_RC_lang_id;
   D_person_name       md_RC_name[500];
   D_bank_account      md_RC_bank_acc[500];
   D_char_numeric      md_RC_total_amt[500];
   D_gl_account        md_RC_gl_acc1[500];
   D_char_numeric      md_RC_gl_amt1[500];
   D_gl_account        md_RC_gl_acc2[500];
   D_char_numeric      md_RC_gl_amt2[500];
   D_gl_account        md_RC_gl_acc3[500];
   D_char_numeric      md_RC_gl_amt3[500];
   D_form_of_payment   md_RC_form_of_pay[500];
   D_credit_card_sup   md_RC_cc_id[500];
   D_credit_card_no    md_RC_cc_no[500];
   D_expiry_date       md_RC_cc_exp_date[500];
   D_authorization     md_RC_auth_no[500];
   AB_Text             md_RC_link_to[500];
   D_yes_no            md_RC_valid[500];
   AB_Text             md_RC_payment_reference[500];

   //Irdistribution
   D_line_number       md_DI_rec_line[500];
   D_gl_account        md_DI_gl_acc[500];
   D_char_numeric      md_DI_gl_amt[500];

	//Iremailaddress
	AB_Text			  	  mt_EM_email_id[500];
	AB_Char				  mc_EM_email_type[500];

	//Irstopovercity
	D_service_type			md_ISOC_itin_type[500];
   D_line_number			md_ISOC_line_number[500];
   D_line_number			md_ISOC_order_number[500];
	D_city					md_ISOC_stop_over_city[500];

   int mi_IO_I,
       mi_IR_I,
       mi_IA_I,
       mi_ICD_I,
       mi_IT_I,
       mi_ITD_I,
       mi_IRF_I,
       mi_IP_I,
       mi_ID_I,
       mi_IRP_I,
       mi_IRMK_I ;

	int mi_RC_I,
		 mi_DI_I;

	int mi_EM_I;

	int mi_ISOC_I;

	// Queue type (RMI,RMA)
	D_que_type        md_queType ;

   AB_Integer mi_rejectirId;  // msg id for Rejectir
   AB_Varchar mv_rejectirSub; // msg sub for Rejectir
	AB_Varchar mv_phoneField;

	AB_Boolean 				  mb_motli_branch;
   D_company              md_motli_cop_id;
   D_branch               md_motli_bra_id;
   D_client_file_code     md_motli_cfa_code;
   D_reference            md_motli_cfa_dpt;
	AB_Boolean				  mb_ticket4aofRecorded;

	set <Text2Text> M02_TktNo2ExpDate;
	set <Text2Text> FriquentFlyer_SupId2PaxId;
	set <string> singleRemarkFlags;

	// QREX START
	AB_Boolean 					mb_refundPNR;
	// QREX END

	RemarkParser* pRemarkParser;
	AB_Date           mt_pnrDate ;

    D_credit_card_sup   md_CCRFP_cc_id;
    D_credit_card_no    md_CCRFP_cc_no;
    D_expiry_date       md_CCRFP_cc_exp_date;

	AB_Boolean mb_reasonCodeFoundInRentalSegment;

	AB_Boolean mb_itIsBatchTRM;

   public :
		CInterfaceRecord()  ;
		~CInterfaceRecord()  ;
		void createQueue(D_description po_errorDescription) ;
		void createClientfile() ;
		void initIraddress(CIRCreateIraddress &po_createIraddress) ;
		void initIrpayment(CIRCreateIrpayment &po_createIrpayment) ;
		void initIrtaxdetail(CIRCreateIrtaxdetail &po_createIrtaxdetail) ;
		void initIrfrequentuser(CIRCreateIrfrequentuser &po_createIrfrequentuser) ;
		void initIrother(CIRCreateIrother &po_createIrother) ;
		void initIrperson(CIRCreateIrperson &po_createIrperson) ;
		void initIrticket(CIRCreateIrticket &po_createIrticket) ;
		void initIrconsumdetail(CIRCreateIrconsumdetail &po_createIrconsumdetail) ;
		void initIrremark(CIRCreateIrremark &po_createIrremark) ;
		void initIrtransport(CIRCreateIrtransport &po_createIrtransport) ;
		void initIrreceipt(CIRCreateIrreceipt &po_createIrreceipt) ;
		void initIrdistribution(CIRCreateIrdistribution &po_createIrdistribution) ;
		void initIremailaddress(CIRCreateIremailaddress &po_createIremailaddress) ;
		void initIrstopovercity(CIRCreateIrstopovercity &po_createIrstopovercity);

		void cleanupIrtransport();
		void createIrtransport() ;
		void createIrremark() ;
		void createIrconsumdetail() ;
		void createIrperson() ;
		void createIrticket() ;
		void createIrother() ;
		void createIrfrequentuser() ;
		void createIrtaxdetail() ;
		void createIraddress() ;
		void createIrpayment() ;
      void createIrreceipt() ;
      void createIrdistribution() ;
		void createIremailaddress() ;
		void createIrstopovercity() ;
		void updateIrRows() ;

		void createIrRows() ;
		// WS 17 START *******
		void getLocalDate(AB_Date &pd_gmtDateTime, AB_Date &pd_lclDateTime);
		// WS 17 END *******
		void editDomains() ;
		AB_Boolean editFormOfPayment() ;
		AB_Boolean editAddressDomain() ;
		AB_Boolean editDocumentDomain() ;
		AB_Boolean editPhoneDomain() ;
		void editCity(D_city &pm_city) ;
		void editUnitType(D_unit_type &pm_unitType) ;
		void editSupplier(D_supplier &pm_supId) ;
		AB_Boolean editNameDomain(D_person_name pd_name) ;
		AB_Boolean editAmountDomain(D_char_numeric pd_amount, const char* lv_format, D_amount &pd_value) ;
		AB_Boolean editReceiptAmount(D_char_numeric pd_amount, const char* lv_format, D_amount &pd_value) ;
      AB_Boolean editReceiptDomains();
      AB_Boolean editReceipt();
      void editReceiptCert( AB_Text &pv_cert, AB_Text &pc_ccId,
         AB_Text &pc_bnkId, AB_Boolean &pb_valid );
		AB_Boolean editEmailaddress(AB_Varchar& pv_email) ;
		void print_Irtripheader() ;
		void printIrremark() ;
		void printIraddress() ;
		void printIrtaxdetail() ;
		void printIrtransport() ;
		void printIrconsumdetail() ;
		void print_Irrental() ;
		void printIrperson() ;
		void printIrticket() ;
		void printFreqUser() ;
		void printOther() ;
		void printIrpayment() ;
		void printIrreceipt() ;
		void printIremailaddress() ;
		void printIrstopovercity();
		void checkDomainEdits()  ;
		void createARNK() ;
		void checkTicket( AB_Integer   &pi_firstAirLine,
														AB_Integer   &pi_secAirLine,
														V_AB_Integer &pia_tktLine,
														V_AB_Integer &pia_paxLine);
		void createAllArnkICD();
		void createSpecArnkICD( V_AB_Integer  &pia_tktLine,
																V_AB_Integer  &pia_paxLine) ;
		void processPassengerName(AB_Text &pc_full_name)  ;

	   void setTranId(D_txn_identifier &md_txnTranId){md_txn_id=md_txnTranId;}        //1309-30SEP98-SC --added TranId to Ticket

		void setMsg(D_description &pd_errorDescription) ;

		void initTicket(int  &pi_index) ;
		void initIrtransport(const int& index);
		void initIrconsumdetail(const int& index);
		void initIrpayment(int index);

      void setCopIdByIata();   // SCR-437808-JJ-13OCT2009
	  AB_Boolean isCCExists4copid(AB_Varchar cc_id);
      AB_Boolean isPFSSupplier( const D_company&, const D_supplier& );

	void setTRMBatchMode(){mb_itIsBatchTRM = AB_true;}
	AB_Boolean isTRMBatchMode(){return mb_itIsBatchTRM;}

protected:
      void adjustPNRDate(
                           const AB_Date &pt_creation_date,
                           const AB_Text &pv_pnrDate,
                                 AB_Text &pv_IRT_pnr_date,
                                 AB_Date &pt_pnrDate);

		void adjustRecordCreationDate( const  AB_Date &pt_creation_date,
												  const AB_Text &pv_RecordCreationDate,
												  const AB_Text &pv_RecordCreationTime,
								 		 				  AB_Text &pv_IRT_RecordCreation_date);

// US 044 START
		AB_Boolean createTransportAssociatedRemark( string label, string remark = "LABELS" );  // G9150
		AB_Boolean createTransportAssociatedRemark( int IT_ind, string& remarkStr ); // G9150
		AB_Boolean createTransportAssociatedRemark( D_line_number &pd_line_number, AB_Text &pd_remark );
		AB_Boolean createRentalAssociatedRemark( D_line_number &pd_line_number, AB_Text &pd_remark );
		AB_Boolean createRentalAssociatedRemark( int IR_ind, string remark );
		AB_Boolean createOtherAssociatedRemark( D_line_number &pd_line_number, AB_Text &pd_remark );
		AB_Boolean createOtherAssociatedRemark( int IR_ind, string remark );
		void createUnAssociatedRemark( AB_Text &pd_remark );
// US 044 END
      // GAP US089 START
      void createUnAssociatedInvoiceRemark( AB_Text &pd_remark );
      // GAP US089 END

		AB_Text CheckTime(const AB_Text &lv_time) const;
		void overrideHomePhone();
		void editAddress();

      void assignIrdistribution( CIRCreateIrreceipt &po_createIrreceipt,
            int pi_rec_line );

		// GAP US107 START
		void editAmount(	AB_Text lt_amount_string,
								D_amount pd_min_amount,
								D_amount pd_max_amount,
								AB_Boolean pb_percent_allowed,
								AB_Boolean pb_percent_restricted);
		// GAP US107 END
		void editPhoneNumber( AB_Text& pt_phone_number, bool oldRules = false);
		void editPhoneNumber( AB_Text& pt_phone_number, AB_Varchar& acore, AB_Varchar& phoneNum, bool oldRules = false);
		void adjustTransportItinType();
      AB_Boolean
      validateCoupon(const D_bank_account&   pd_bnk_id,
                     const AB_Text&          pt_coupon_id1,
                     const AB_Text&          pt_coupon_id2,
                     const D_char_numeric&   pd_total_amt);

		// GAP US111 START
		void createUDID(D_line_number pd_UDID_number, AB_Text pt_remark, AB_Boolean pb_matrix_UDID);
		// GAP US111 END

		AB_Varchar mv_rec_queType;
		void processReceipt();
		void getReceiptQueueType();
		void createInfoQueue(D_que_type pd_que_type, AB_Integer pi_msg_id);
		void moveReceiptQueues( AB_Varchar pd_que_type);
		bool validCreditCardNumber(const AB_Varchar& pv_cc_id);
		bool validCreditCard(const AB_Varchar& pv_cc_id, const AB_Char& pc_exp_date, long& pi_error_code);
		bool validCreditCardBIN(const AB_Varchar& pv_cc_id);
		bool createPFSConsumdetail(const D_passenger_no& pc_paxId, const D_line_number& pc_docId) ;
   virtual void parseRemarkCNF(AB_Text lv_data);
		bool createUDIDifRange1to99(D_line_number pd_UDID_number, AB_Text pt_remark);

	void saveTicketNo4AOF(AB_Varchar xxxTicketNo);
	void parseAOFremark(AB_Text remark);
	void cleanUpAOFRemarksIfNeeded();
	void validateCC_BINsInFeeRemarks();
	void createUniqueRemark( AB_Char pc_type, AB_Text pd_remark );
	void processIRVoidTicket();

	void createRemark( const char* pc_type, const char* pt_remark );
	void createRemarkRows(Remark&);

	void RP_storeParsedRemarksInMemory();
	void RP_storeParsedRemarksInDatabase();

	bool RP_saveDataTo(const string& remarkName, const string& paramName, AB_Char& where);
	bool RP_saveDataTo(const string& remarkName, const string& paramName, AB_Varchar& where);
	bool RP_saveDataTo(const string& remarkName, const string& paramName, AB_Text& where);
	bool RP_saveDataTo(const string& remarkName, const string& paramName, const string& groupName, AB_Char& where);
	bool RP_saveDataTo(const string& remarkName, const string& paramName, const string& groupName, AB_Varchar& where);
	bool RP_saveDataTo(const string& remarkName, const string& paramName, const string& groupName, AB_Text& where);

	void RP_populateIritineraryudid(	D_service_type	itin_type,
												D_line_number	line_number,
												D_char_date		start_date,
												D_city			city_id);
	AB_Text ParseTime(const AB_Text &lv_time) const;
	void ParseMoney(AB_Varchar &pv_inString, AB_Varchar &pv_outMoney) ;

	void CommonTransportLabels() ;
	virtual void RailLabels() ;
	virtual void BusLabels() ;

	void populateRestOfLabels(ItinType itin_type) ;

	void CommonRentalLabels() ;
	virtual void CarLabels() ;
	virtual void LimoLabels() ;

	void CommonOtherLabels() ;
	virtual void TourLabels() ;
	virtual void ShipLabels() ;
	virtual void LandLabels() ;

	virtual void setIrReasonCodeIfOverride(AB_Varchar &lv_data);

	virtual void TRM_Processor__updateQueueString(AB_Varchar lv_columnName, AB_Varchar lv_columnValue);
	virtual void TRM_Processor__updateQueueStringStringString(	AB_Varchar lv_columnName, AB_Varchar lv_columnStringValue, 	AB_Varchar lv_columnName2, AB_Varchar lv_columnStringValue2, 	AB_Varchar lv_columnName3, AB_Varchar lv_columnStringValue3);
	virtual void TRM_Processor__updateQueueInt(AB_Varchar lv_columnName, AB_Integer lv_columnValue);

} ;
#endif
