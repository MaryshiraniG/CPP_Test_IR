/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.94
      Delta Created: 17/10/18 - 15:12:29
         Last Delta: app/src/common/InterfaceRecord/_us 10/18/17 - 15:12:29 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.dbIR.h) 

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
16MAY2012   509845  c0572    WW  Process void ticket for CWT US M&G
============================================================================
*/

#ifndef dbIR_h
#define dbIR_h

#include "tbIR.h"

class CIRCreateIraddress {
public:
// Constructor
	CIRCreateIraddress();

	AB_Varchar session_id;
	AB_Varchar type1;
	AB_Varchar addr11;
	AB_Varchar addr21;
	AB_Varchar addr31;
	AB_Varchar addr41;
	AB_Varchar addr51;
	AB_Varchar addr61;
	AB_Varchar phone_acode1;
	AB_Varchar phone_no1;
	AB_Varchar bphone_acode1;
	AB_Varchar bphone_no1;
	AB_Varchar fax_acode1;
	AB_Varchar fax_no1;
	AB_Varchar type2;
	AB_Varchar addr12;
	AB_Varchar addr22;
	AB_Varchar addr32;
	AB_Varchar addr42;
	AB_Varchar addr52;
	AB_Varchar addr62;
	AB_Varchar phone_acode2;
	AB_Varchar phone_no2;
	AB_Varchar bphone_acode2;
	AB_Varchar bphone_no2;
	AB_Varchar fax_acode2;
	AB_Varchar fax_no2;
	AB_Varchar mphone_acode1;
	AB_Varchar mphone_no1;
	AB_Varchar mphone_acode2;
	AB_Varchar mphone_no2;
};

class CIRCreateIrfrequentuser {
public:
// Constructor
	CIRCreateIrfrequentuser();

	AB_Varchar session_id;
	AB_Smallint pax_id1;
	AB_Varchar sup_id1;
	AB_Varchar fup_type1;
	AB_Varchar fup1;
	AB_Varchar fup_status1;
	AB_Varchar srvc_sup_id1;
	AB_Smallint pax_id2;
	AB_Varchar sup_id2;
	AB_Varchar fup_type2;
	AB_Varchar fup2;
	AB_Varchar fup_status2;
	AB_Varchar srvc_sup_id2;
	AB_Smallint pax_id3;
	AB_Varchar sup_id3;
	AB_Varchar fup_type3;
	AB_Varchar fup3;
	AB_Varchar fup_status3;
	AB_Varchar srvc_sup_id3;
	AB_Smallint pax_id4;
	AB_Varchar sup_id4;
	AB_Varchar fup_type4;
	AB_Varchar fup4;
	AB_Varchar fup_status4;
	AB_Varchar srvc_sup_id4;
	AB_Smallint pax_id5;
	AB_Varchar sup_id5;
	AB_Varchar fup_type5;
	AB_Varchar fup5;
	AB_Varchar fup_status5;
	AB_Varchar srvc_sup_id5;
};

class CIRCreateIrpayment {
public:
// Constructor
	CIRCreateIrpayment();

	AB_Varchar session_id;
	AB_Smallint pax_id1;
	AB_Smallint line_number1;
	AB_Smallint pay_id1;
	AB_Varchar form_of_pay1;
	AB_Decimal amount1;
	AB_Varchar cc_id1;
	AB_Varchar cc_no1;
	AB_Varchar cc_exp_date1;
	AB_Varchar auth_no1;
	AB_Varchar auth_source1;
	AB_Smallint pax_id2;
	AB_Smallint line_number2;
	AB_Smallint pay_id2;
	AB_Varchar form_of_pay2;
	AB_Decimal amount2;
	AB_Varchar cc_id2;
	AB_Varchar cc_no2;
	AB_Varchar cc_exp_date2;
	AB_Varchar auth_no2;
	AB_Varchar auth_source2;
	AB_Smallint pax_id3;
	AB_Smallint line_number3;
	AB_Smallint pay_id3;
	AB_Varchar form_of_pay3;
	AB_Decimal amount3;
	AB_Varchar cc_id3;
	AB_Varchar cc_no3;
	AB_Varchar cc_exp_date3;
	AB_Varchar auth_no3;
	AB_Varchar auth_source3;
	AB_Smallint pax_id4;
	AB_Smallint line_number4;
	AB_Smallint pay_id4;
	AB_Varchar form_of_pay4;
	AB_Decimal amount4;
	AB_Varchar cc_id4;
	AB_Varchar cc_no4;
	AB_Varchar cc_exp_date4;
	AB_Varchar auth_no4;
	AB_Varchar auth_source4;
	AB_Smallint pax_id5;
	AB_Smallint line_number5;
	AB_Smallint pay_id5;
	AB_Varchar form_of_pay5;
	AB_Decimal amount5;
	AB_Varchar cc_id5;
	AB_Varchar cc_no5;
	AB_Varchar cc_exp_date5;
	AB_Varchar auth_no5;
	AB_Varchar auth_source5;
};

class CIRCreateIrperson {
public:
// Constructor
	CIRCreateIrperson();

	AB_Varchar session_id;
	AB_Smallint pax_id1;
	AB_Varchar title1;
	AB_Varchar last_name1;
	AB_Varchar first_name1;
	AB_Smallint new_pax_id1;
	AB_Smallint new_pax_line1;
	AB_Varchar sex1;
	AB_Varchar smoking1;
	AB_Varchar ctry_id1;
	AB_Varchar passport_no1;
	AB_Varchar passport_exp1;
	AB_Varchar reference1;
	AB_Varchar crs_invoice1;
	AB_Smallint traveller_id1;
	AB_Smallint pax_id2;
	AB_Varchar title2;
	AB_Varchar last_name2;
	AB_Varchar first_name2;
	AB_Smallint new_pax_id2;
	AB_Smallint new_pax_line2;
	AB_Varchar sex2;
	AB_Varchar smoking2;
	AB_Varchar ctry_id2;
	AB_Varchar passport_no2;
	AB_Varchar passport_exp2;
	AB_Varchar reference2;
	AB_Varchar crs_invoice2;
	AB_Smallint traveller_id2;
	AB_Smallint pax_id3;
	AB_Varchar title3;
	AB_Varchar last_name3;
	AB_Varchar first_name3;
	AB_Smallint new_pax_id3;
	AB_Smallint new_pax_line3;
	AB_Varchar sex3;
	AB_Varchar smoking3;
	AB_Varchar ctry_id3;
	AB_Varchar passport_no3;
	AB_Varchar passport_exp3;
	AB_Varchar reference3;
	AB_Varchar crs_invoice3;
	AB_Smallint traveller_id3;
	AB_Smallint pax_id4;
	AB_Varchar title4;
	AB_Varchar last_name4;
	AB_Varchar first_name4;
	AB_Smallint new_pax_id4;
	AB_Smallint new_pax_line4;
	AB_Varchar sex4;
	AB_Varchar smoking4;
	AB_Varchar ctry_id4;
	AB_Varchar passport_no4;
	AB_Varchar passport_exp4;
	AB_Varchar reference4;
	AB_Varchar crs_invoice4;
	AB_Smallint traveller_id4;
};

class CIRCreateIrremark {
public:
// Constructor
	CIRCreateIrremark();

	AB_Varchar session_id;
	AB_Smallint line_number1;
	AB_Smallint pax_id1;
	AB_Varchar type1;
	AB_Varchar remark1;
	AB_Varchar itin_type1;
	AB_Smallint tra_line_number1;
	AB_Smallint ren_line_number1;
	AB_Smallint oth_line_number1;
	AB_Smallint udi_line_number1;
	AB_Smallint line_number2;
	AB_Smallint pax_id2;
	AB_Varchar type2;
	AB_Varchar remark2;
	AB_Varchar itin_type2;
	AB_Smallint tra_line_number2;
	AB_Smallint ren_line_number2;
	AB_Smallint oth_line_number2;
	AB_Smallint udi_line_number2;
	AB_Smallint line_number3;
	AB_Smallint pax_id3;
	AB_Varchar type3;
	AB_Varchar remark3;
	AB_Varchar itin_type3;
	AB_Smallint tra_line_number3;
	AB_Smallint ren_line_number3;
	AB_Smallint oth_line_number3;
	AB_Smallint udi_line_number3;
	AB_Smallint line_number4;
	AB_Smallint pax_id4;
	AB_Varchar type4;
	AB_Varchar remark4;
	AB_Varchar itin_type4;
	AB_Smallint tra_line_number4;
	AB_Smallint ren_line_number4;
	AB_Smallint oth_line_number4;
	AB_Smallint udi_line_number4;
	AB_Smallint line_number5;
	AB_Smallint pax_id5;
	AB_Varchar type5;
	AB_Varchar remark5;
	AB_Varchar itin_type5;
	AB_Smallint tra_line_number5;
	AB_Smallint ren_line_number5;
	AB_Smallint oth_line_number5;
	AB_Smallint udi_line_number5;
	AB_Smallint line_number6;
	AB_Smallint pax_id6;
	AB_Varchar type6;
	AB_Varchar remark6;
	AB_Varchar itin_type6;
	AB_Smallint tra_line_number6;
	AB_Smallint ren_line_number6;
	AB_Smallint oth_line_number6;
	AB_Smallint udi_line_number6;
	AB_Smallint line_number7;
	AB_Smallint pax_id7;
	AB_Varchar type7;
	AB_Varchar remark7;
	AB_Varchar itin_type7;
	AB_Smallint tra_line_number7;
	AB_Smallint ren_line_number7;
	AB_Smallint oth_line_number7;
	AB_Smallint udi_line_number7;
	AB_Smallint line_number8;
	AB_Smallint pax_id8;
	AB_Varchar type8;
	AB_Varchar remark8;
	AB_Varchar itin_type8;
	AB_Smallint tra_line_number8;
	AB_Smallint ren_line_number8;
	AB_Smallint oth_line_number8;
	AB_Smallint udi_line_number8;
	AB_Smallint line_number9;
	AB_Smallint pax_id9;
	AB_Varchar type9;
	AB_Varchar remark9;
	AB_Varchar itin_type9;
	AB_Smallint tra_line_number9;
	AB_Smallint ren_line_number9;
	AB_Smallint oth_line_number9;
	AB_Smallint udi_line_number9;
	AB_Smallint line_number0;
	AB_Smallint pax_id0;
	AB_Varchar type0;
	AB_Varchar remark0;
	AB_Varchar itin_type0;
	AB_Smallint tra_line_number0;
	AB_Smallint ren_line_number0;
	AB_Smallint oth_line_number0;
	AB_Smallint udi_line_number0;
};

class CIRCreateIrtransport {
public:
// Constructor
	CIRCreateIrtransport();

	AB_Varchar session_id;
	AB_Varchar itin_type;
	AB_Smallint line_number;
	AB_Varchar start_date;
	AB_Varchar start_time;
	AB_Varchar orig_city_id;
	AB_Varchar orig_cityname;
	AB_Varchar cog_dep_city;
	AB_Varchar cog_dep_date;
	AB_Varchar cog_dep_time;
	AB_Varchar cog_dep_term;
	AB_Varchar cog_dep_gate;
	AB_Varchar cog_of_miles;
	AB_Varchar end_date;
	AB_Varchar end_time;
	AB_Varchar dest_city_id;
	AB_Varchar dest_cityname;
	AB_Varchar classofservice;
	AB_Varchar service;
	AB_Varchar sup_id;
	AB_Varchar reference;
	AB_Varchar chg_of_gauge;
	AB_Varchar no_of_stops;
	AB_Varchar dep_terminal;
	AB_Varchar dep_gate;
	AB_Varchar report_time;
	AB_Varchar arr_gate;
	AB_Varchar arr_terminal;
	AB_Varchar airline_locator;
	AB_Varchar carrier_type;
	AB_Varchar meal;
	AB_Varchar aff_carrier;
	AB_Integer actual_miles;
	AB_Varchar arrival_ind;
	AB_Varchar elapsed_time;
	AB_Varchar itin_type1;
	AB_Smallint line_number1;
	AB_Varchar start_date1;
	AB_Varchar start_time1;
	AB_Varchar orig_city_id1;
	AB_Varchar orig_cityname1;
	AB_Varchar cog_dep_city1;
	AB_Varchar cog_dep_date1;
	AB_Varchar cog_dep_time1;
	AB_Varchar cog_dep_term1;
	AB_Varchar cog_dep_gate1;
	AB_Varchar cog_of_miles1;
	AB_Varchar end_date1;
	AB_Varchar end_time1;
	AB_Varchar dest_city_id1;
	AB_Varchar dest_cityname1;
	AB_Varchar classofservice1;
	AB_Varchar service1;
	AB_Varchar sup_id1;
	AB_Varchar reference1;
	AB_Varchar chg_of_gauge1;
	AB_Varchar no_of_stops1;
	AB_Varchar dep_terminal1;
	AB_Varchar dep_gate1;
	AB_Varchar report_time1;
	AB_Varchar arr_gate1;
	AB_Varchar arr_terminal1;
	AB_Varchar airline_locator1;
	AB_Varchar carrier_type1;
	AB_Varchar meal1;
	AB_Varchar aff_carrier1;
	AB_Integer actual_miles1;
	AB_Varchar arrival_ind1;
	AB_Varchar elapsed_time1;
};

class CIRCreateIrconsumdetail {
public:
// Constructor
	CIRCreateIrconsumdetail();

	AB_Varchar session_id;
	AB_Smallint pax_id1;
	AB_Varchar itin_type1;
	AB_Smallint itin_line_number1;
	AB_Smallint line_number1;
	AB_Varchar seat1;
	AB_Varchar connection1;
	AB_Varchar smoking1;
	AB_Decimal fare_by_leg1;
	AB_Varchar fare_basis1;
	AB_Varchar tkt_desig1;
	AB_Varchar cog_seat1;
	AB_Varchar service_status1;
	AB_Integer ftp_miles1;
	AB_Varchar boarding_pass1;
	AB_Varchar special_meal1;
	AB_Varchar special_need1;
	AB_Varchar baggage1;
	AB_Varchar crs_priced1;
	AB_Varchar cur_id1;
	AB_Smallint pax_id2;
	AB_Varchar itin_type2;
	AB_Smallint itin_line_number2;
	AB_Smallint line_number2;
	AB_Varchar seat2;
	AB_Varchar connection2;
	AB_Varchar smoking2;
	AB_Decimal fare_by_leg2;
	AB_Varchar fare_basis2;
	AB_Varchar tkt_desig2;
	AB_Varchar cog_seat2;
	AB_Varchar service_status2;
	AB_Integer ftp_miles2;
	AB_Varchar boarding_pass2;
	AB_Varchar special_meal2;
	AB_Varchar special_need2;
	AB_Varchar baggage2;
	AB_Varchar crs_priced2;
	AB_Varchar cur_id2;
	AB_Smallint pax_id3;
	AB_Varchar itin_type3;
	AB_Smallint itin_line_number3;
	AB_Smallint line_number3;
	AB_Varchar seat3;
	AB_Varchar connection3;
	AB_Varchar smoking3;
	AB_Decimal fare_by_leg3;
	AB_Varchar fare_basis3;
	AB_Varchar tkt_desig3;
	AB_Varchar cog_seat3;
	AB_Varchar service_status3;
	AB_Integer ftp_miles3;
	AB_Varchar boarding_pass3;
	AB_Varchar special_meal3;
	AB_Varchar special_need3;
	AB_Varchar baggage3;
	AB_Varchar crs_priced3;
	AB_Varchar cur_id3;
	AB_Smallint pax_id4;
	AB_Varchar itin_type4;
	AB_Smallint itin_line_number4;
	AB_Smallint line_number4;
	AB_Varchar seat4;
	AB_Varchar connection4;
	AB_Varchar smoking4;
	AB_Decimal fare_by_leg4;
	AB_Varchar fare_basis4;
	AB_Varchar tkt_desig4;
	AB_Varchar cog_seat4;
	AB_Varchar service_status4;
	AB_Integer ftp_miles4;
	AB_Varchar boarding_pass4;
	AB_Varchar special_meal4;
	AB_Varchar special_need4;
	AB_Varchar baggage4;
	AB_Varchar crs_priced4;
	AB_Varchar cur_id4;
};

class CIRCreateIrtaxdetail {
public:
// Constructor
	CIRCreateIrtaxdetail();

	AB_Varchar session_id;
	AB_Smallint pax_id1;
	AB_Smallint line_number1;
	AB_Varchar tax_id1;
	AB_Decimal tax_amount1;
	AB_Varchar tax_source1;
	AB_Varchar city_id1;
	AB_Smallint pax_id2;
	AB_Smallint line_number2;
	AB_Varchar tax_id2;
	AB_Varchar tax_source2;
	AB_Varchar city_id2;
	AB_Decimal tax_amount2;
	AB_Smallint pax_id3;
	AB_Smallint line_number3;
	AB_Varchar tax_id3;
	AB_Varchar tax_source3;
	AB_Varchar city_id3;
	AB_Decimal tax_amount3;
	AB_Smallint pax_id4;
	AB_Smallint line_number4;
	AB_Varchar tax_id4;
	AB_Varchar tax_source4;
	AB_Varchar city_id4;
	AB_Decimal tax_amount4;
	AB_Smallint pax_id5;
	AB_Smallint line_number5;
	AB_Varchar tax_id5;
	AB_Varchar tax_source5;
	AB_Varchar city_id5;
	AB_Decimal tax_amount5;
	AB_Smallint pax_id6;
	AB_Smallint line_number6;
	AB_Varchar tax_id6;
	AB_Varchar tax_source6;
	AB_Varchar city_id6;
	AB_Decimal tax_amount6;
	AB_Smallint pax_id7;
	AB_Smallint line_number7;
	AB_Varchar tax_id7;
	AB_Varchar tax_source7;
	AB_Varchar city_id7;
	AB_Decimal tax_amount7;
	AB_Smallint pax_id8;
	AB_Smallint line_number8;
	AB_Varchar tax_id8;
	AB_Varchar tax_source8;
	AB_Varchar city_id8;
	AB_Decimal tax_amount8;
	AB_Smallint pax_id9;
	AB_Smallint line_number9;
	AB_Varchar tax_id9;
	AB_Varchar tax_source9;
	AB_Varchar city_id9;
	AB_Decimal tax_amount9;
	AB_Smallint pax_id0;
	AB_Smallint line_number0;
	AB_Varchar tax_id0;
	AB_Varchar tax_source0;
	AB_Varchar city_id0;
	AB_Decimal tax_amount0;
};

class CIRLoadClientFile {
public:
// Constructor
	CIRLoadClientFile();

	AB_Varchar pc_session_id;
	AB_Varchar pc_tran_id;
	AB_Varchar pc_con_id;
	AB_Varchar reference;
	AB_Varchar cfa_id;
	AB_Varchar mktg_list;
	AB_Date gmt_date_time;
	AB_Varchar pc_PNR_date;
	AB_Varchar iata_no;
	AB_Varchar pc_company;
	AB_Varchar pc_branch;
	AB_Integer pi_cfa_no;
	AB_Varchar pc_cfa_code;
	AB_Varchar pc_cfa_type;
	AB_Varchar pc_cfa_con;
	AB_Varchar pc_fname;
	AB_Varchar pc_lname;
	AB_Varchar pc_rejectCon;
	AB_Varchar pc_rejectCop;
	AB_Varchar pc_rejectBra;
	AB_Varchar pc_city;
	AB_Integer pi_rejectirId;
	AB_Varchar pv_rejectirSub;
};

class CIRReject {
public:
// Constructor
	CIRReject();

	AB_Varchar pc_session_id;
	AB_Varchar pc_rejectCop;
	AB_Varchar pc_rejectBra;
	AB_Varchar pc_rejectCon;
	AB_Varchar reference;
	AB_Date gmt_date_time;
	AB_Varchar iata_no;
	AB_Varchar description;
	AB_Varchar queue_type;
};

class CIRGetLocalDate {
public:
// Constructor
	CIRGetLocalDate();

	AB_Varchar pc_copId;
	AB_Varchar pc_braId;
	AB_Varchar pc_iataNo;
	AB_Varchar pc_cityId;
	AB_Date pd_gmtDateTime;
	AB_Date pd_lclDateTime;
};

class CIRCreateIrother {
public:
// Constructor
	CIRCreateIrother();

	AB_Varchar session_id;
	AB_Varchar itin_type1;
	AB_Smallint line_number1;
	AB_Varchar start_date1;
	AB_Varchar start_time1;
	AB_Varchar orig_city_id1;
	AB_Varchar end_date1;
	AB_Varchar end_time1;
	AB_Varchar dest_city_id1;
	AB_Varchar unit_type1;
	AB_Integer unit_ct1;
	AB_Varchar sup_id1;
	AB_Varchar reference1;
	AB_Varchar confirmation1;
	AB_Varchar saving_code1;
	AB_Varchar departure_point1;
	AB_Varchar arrival_point1;
	AB_Varchar svc_description1;
	AB_Varchar cabin_no1;
	AB_Varchar tour_no1;
	AB_Varchar itin_type2;
	AB_Smallint line_number2;
	AB_Varchar start_date2;
	AB_Varchar start_time2;
	AB_Varchar orig_city_id2;
	AB_Varchar end_date2;
	AB_Varchar end_time2;
	AB_Varchar dest_city_id2;
	AB_Varchar unit_type2;
	AB_Integer unit_ct2;
	AB_Varchar sup_id2;
	AB_Varchar reference2;
	AB_Varchar confirmation2;
	AB_Varchar saving_code2;
	AB_Varchar departure_point2;
	AB_Varchar arrival_point2;
	AB_Varchar svc_description2;
	AB_Varchar cabin_no2;
	AB_Varchar tour_no2;
	AB_Varchar itin_type3;
	AB_Smallint line_number3;
	AB_Varchar start_date3;
	AB_Varchar start_time3;
	AB_Varchar orig_city_id3;
	AB_Varchar end_date3;
	AB_Varchar end_time3;
	AB_Varchar dest_city_id3;
	AB_Varchar unit_type3;
	AB_Integer unit_ct3;
	AB_Varchar sup_id3;
	AB_Varchar reference3;
	AB_Varchar confirmation3;
	AB_Varchar saving_code3;
	AB_Varchar departure_point3;
	AB_Varchar arrival_point3;
	AB_Varchar svc_description3;
	AB_Varchar cabin_no3;
	AB_Varchar tour_no3;
};

class CIRCleanupIrtransport {
public:
// Constructor
	CIRCleanupIrtransport();

	AB_Varchar session_id;
};

class CIRCreateIrrental {
public:
// Constructor
	CIRCreateIrrental();

	AB_Varchar session_id;
	AB_Varchar cop_id;
	AB_Varchar itin_type;
	AB_Smallint line_number;
	AB_Varchar start_date;
	AB_Varchar start_time;
	AB_Varchar origin_city_id;
	AB_Varchar end_date;
	AB_Varchar end_time;
	AB_Varchar dest_city_id;
	AB_Varchar unit_type;
	AB_Varchar unit_ct;
	AB_Varchar sup_id;
	AB_Varchar source;
	AB_Varchar action_code;
	AB_Varchar reason_code;
	AB_Varchar crs_id;
	AB_Integer number_of_pax;
	AB_Varchar ex_day_charge;
	AB_Varchar ex_hr_charge;
	AB_Varchar ex_mile_charge;
	AB_Varchar free_mile_code;
	AB_Varchar free_mileage;
	AB_Varchar dropoff_charge;
	AB_Varchar regular_rate;
	AB_Varchar booked_rate;
	AB_Varchar local_rate;
	AB_Varchar cur_id;
	AB_Varchar rate_type;
	AB_Varchar rate_code;
	AB_Varchar rate_duration;
	AB_Varchar rate_multip;
	AB_Varchar comm_rate;
	AB_Varchar multi_lvl_rate;
	AB_Varchar saving_code;
	AB_Varchar location;
	AB_Varchar location_id;
	AB_Varchar site_name;
	AB_Varchar addr1;
	AB_Varchar addr2;
	AB_Varchar addr3;
	AB_Varchar addr4;
	AB_Varchar phone_acode;
	AB_Varchar phone_no;
	AB_Varchar fax_acode;
	AB_Varchar fax_no;
	AB_Varchar guarantee;
	AB_Varchar confirmation;
	AB_Varchar corporate_disc;
	AB_Varchar corporation_id;
	AB_Varchar cancel_policy;
	AB_Varchar reference;
	AB_Varchar fup;
	AB_Varchar customer_id;
	AB_Integer harp_htlkey;
	AB_Varchar man_creation;
	AB_Varchar pickup_location;
	AB_Varchar dropoff_location;
	AB_Varchar discount_code;
	AB_Varchar commissionable;
	AB_Varchar preferred;
	AB_Varchar service_status;
	AB_Varchar virtualCard_cc;
	AB_Varchar virtualCard_exp_date;
	AB_Varchar virtualCard_deployment;
	AB_Varchar virtualCard_card_usage_id;
	AB_Varchar virtualCard_gross_value_ind;
	AB_Varchar pcc_cur_id;
};

class CIRCreateIrticket {
public:
// Constructor
	CIRCreateIrticket();

	AB_Varchar session_id;
	AB_Varchar cop_id;
	AB_Varchar action_code;
	AB_Varchar source;
	AB_Varchar reason_code;
	AB_Varchar crs_id;
	AB_Varchar commission_type;
	AB_Varchar net_ticket_ind;
	AB_Varchar net_ticket_type;
	AB_Decimal nett_fare_to_airline;
	AB_Decimal selling_price_to_shop;
	AB_Decimal selling_price_to_client;
	AB_Decimal tour_oper_commission;
	AB_Varchar ticket_centre_reference;
	AB_Varchar grid_reference;
	AB_Varchar shop_reference;
	AB_Decimal publish_fare_tc_commission;
	AB_Decimal publish_fare_agent_commission;
	AB_Smallint pax_id1;
	AB_Smallint line_number1;
	AB_Smallint cost_line1;
	AB_Varchar reference1;
	AB_Varchar sup_id1;
	AB_Varchar cur_id1;
	AB_Varchar form1;
	AB_Varchar ticket_no1;
	AB_Varchar pic1;
	AB_Varchar orig_ticket_no1;
	AB_Decimal comm_rate1;
	AB_Decimal commission1;
	AB_Varchar no_of_books1;
	AB_Varchar exchange_code1;
	AB_Decimal penalty_ch_fee1;
	AB_Decimal comm_penalty1;
	AB_Decimal base_cost1;
	AB_Varchar saving_code1;
	AB_Decimal full_fare1;
	AB_Decimal low_prac_fare1;
	AB_Varchar personal_crs1;
	AB_Varchar stp_home_city1;
	AB_Varchar stp_host_city1;
	AB_Varchar sel_access1;
	AB_Varchar crs_tour1;
	AB_Decimal rebate1;
	AB_Varchar doc_type1;
	AB_Varchar e_ticket1;
	AB_Varchar refund1;
	AB_Smallint pax_id2;
	AB_Smallint line_number2;
	AB_Smallint cost_line2;
	AB_Varchar reference2;
	AB_Varchar sup_id2;
	AB_Varchar cur_id2;
	AB_Varchar form2;
	AB_Varchar ticket_no2;
	AB_Varchar pic2;
	AB_Varchar orig_ticket_no2;
	AB_Decimal comm_rate2;
	AB_Decimal commission2;
	AB_Varchar no_of_books2;
	AB_Varchar exchange_code2;
	AB_Decimal penalty_ch_fee2;
	AB_Decimal comm_penalty2;
	AB_Decimal base_cost2;
	AB_Varchar saving_code2;
	AB_Decimal full_fare2;
	AB_Decimal low_prac_fare2;
	AB_Varchar personal_crs2;
	AB_Varchar stp_home_city2;
	AB_Varchar stp_host_city2;
	AB_Varchar sel_access2;
	AB_Varchar crs_tour2;
	AB_Decimal rebate2;
	AB_Varchar doc_type2;
	AB_Varchar e_ticket2;
	AB_Varchar refund2;
	AB_Smallint pax_id3;
	AB_Smallint line_number3;
	AB_Smallint cost_line3;
	AB_Varchar reference3;
	AB_Varchar sup_id3;
	AB_Varchar cur_id3;
	AB_Varchar form3;
	AB_Varchar ticket_no3;
	AB_Varchar pic3;
	AB_Varchar orig_ticket_no3;
	AB_Decimal comm_rate3;
	AB_Decimal commission3;
	AB_Varchar no_of_books3;
	AB_Varchar exchange_code3;
	AB_Decimal penalty_ch_fee3;
	AB_Decimal comm_penalty3;
	AB_Decimal base_cost3;
	AB_Varchar saving_code3;
	AB_Decimal full_fare3;
	AB_Decimal low_prac_fare3;
	AB_Varchar personal_crs3;
	AB_Varchar stp_home_city3;
	AB_Varchar stp_host_city3;
	AB_Varchar sel_access3;
	AB_Varchar crs_tour3;
	AB_Decimal rebate3;
	AB_Varchar doc_type3;
	AB_Varchar e_ticket3;
	AB_Varchar refund3;
};

class CIRCreateIrtripheader {
public:
// Constructor
	CIRCreateIrtripheader();

	AB_Varchar session_id;
	AB_Varchar interface_type;
	AB_Date creation_date;
	AB_Varchar iata_no;
	AB_Varchar cop_id;
	AB_Varchar bra_id;
	AB_Varchar crs_id;
	AB_Varchar cfa_type;
	AB_Varchar cfa_code;
	AB_Varchar cfa_id;
	AB_Varchar con_id;
	AB_Varchar iata_dest;
	AB_Varchar city_id;
	AB_Varchar pnr_date;
	AB_Varchar pnr_time;
	AB_Varchar start_date;
	AB_Varchar cfa_dpt;
	AB_Varchar mktg_list;
	AB_Varchar reference;
	AB_Varchar booking_no;
	AB_Integer ticketer_user_id;
	AB_Varchar employee_number;
	AB_Varchar booking_pcc;
	AB_Varchar booking_gds_agent;
	AB_Varchar ticketing_pcc;
	AB_Varchar ticketing_gds_agent;
	AB_Varchar record_creation_date;
	AB_Varchar record_creation_time;
};

class CIRGetSupServiceType {
public:
// Constructor
	CIRGetSupServiceType();

	AB_Varchar pc_sup_id;
	AB_Varchar pc_service_type;
};

class CIRCreateIrdistribution {
public:
// Constructor
	CIRCreateIrdistribution();

	AB_Varchar session_id;
	AB_Integer line_number_1;
	AB_Varchar acc_id_1;
	AB_Decimal amt_1;
	AB_Integer line_number_2;
	AB_Varchar acc_id_2;
	AB_Decimal amt_2;
	AB_Integer line_number_3;
	AB_Varchar acc_id_3;
	AB_Decimal amt_3;
	AB_Integer line_number_4;
	AB_Varchar acc_id_4;
	AB_Decimal amt_4;
	AB_Integer line_number_5;
	AB_Varchar acc_id_5;
	AB_Decimal amt_5;
	AB_Integer line_number_6;
	AB_Varchar acc_id_6;
	AB_Decimal amt_6;
};

class CIRValidateSetFixed {
public:
// Constructor
	CIRValidateSetFixed();

	AB_Varchar pc_session_id;
	AB_Varchar pc_iata;
	AB_Varchar pc_company;
	AB_Varchar pc_branch;
	AB_Varchar pc_consultant;
	AB_Varchar pc_mml_ind;
	AB_Varchar pc_cfa_type;
	AB_Date gmt_date_time;
	AB_Varchar pv_process_name;
	AB_Varchar pc_braMktList;
	AB_Varchar pc_tktBranch;
	AB_Varchar pc_city;
	AB_Date lcl_date_time;
	AB_Varchar pc_rejectCon;
	AB_Varchar pc_rejectCop;
	AB_Varchar pc_rejectBra;
	AB_Integer pi_rejectirId;
	AB_Varchar pv_rejectirSub;
};

class CIRCreateIrreceipt {
public:
// Constructor
	CIRCreateIrreceipt();

	AB_Varchar session_id;
	AB_Integer line_number;
	AB_Varchar cop_id;
	AB_Varchar bra_id;
	AB_Varchar name;
	AB_Decimal total_amt;
	AB_Varchar bank_id;
	AB_Varchar form_of_pay;
	AB_Varchar cc_id;
	AB_Varchar cc_no;
	AB_Varchar cc_exp_date;
	AB_Varchar cc_auth_no;
	AB_Varchar acc_id1;
	AB_Decimal amt1;
	AB_Varchar acc_id2;
	AB_Decimal amt2;
	AB_Varchar acc_id3;
	AB_Decimal amt3;
	AB_Varchar rec_valid;
	AB_Varchar lang_id;
	AB_Varchar link_to;
	AB_Varchar payment_reference;
};

class CIRMoveQueue {
public:
// Constructor
	CIRMoveQueue();

	AB_Varchar pc_session_id;
	AB_Varchar pc_queue_type;
	AB_Varchar pc_rejectCop;
	AB_Varchar pc_rejectBra;
	AB_Varchar pc_rejectCon;
};

class CIRCreateIremailaddress {
public:
// Constructor
	CIRCreateIremailaddress();

	AB_Varchar session_id;
	AB_Varchar email_id1;
	AB_Varchar email_type1;
	AB_Varchar email_id2;
	AB_Varchar email_type2;
	AB_Varchar email_id3;
	AB_Varchar email_type3;
	AB_Varchar email_id4;
	AB_Varchar email_type4;
	AB_Varchar email_id5;
	AB_Varchar email_type5;
	AB_Varchar email_id6;
	AB_Varchar email_type6;
};

class CIRIsPFSSupplier {
public:
// Constructor
	CIRIsPFSSupplier();

	AB_Varchar cop_id;
	AB_Varchar sup_id;
	AB_Date pd_lcl_date;
	AB_Varchar pb_is_pfs;
};

class CIRGetCopIdByIata {
public:
// Constructor
	CIRGetCopIdByIata();

	AB_Varchar pc_iata_no;
	AB_Varchar pc_cop_id;
};

class CIRUpdateIrRows {
public:
// Constructor
	CIRUpdateIrRows();

	AB_Varchar pc_session_id;
};

class CIRCreateIrstopovercity {
public:
// Constructor
	CIRCreateIrstopovercity();

	AB_Varchar session_id;
	AB_Varchar itin_type1;
	AB_Smallint line_number1;
	AB_Smallint order_number1;
	AB_Varchar city_id1;
	AB_Varchar itin_type2;
	AB_Smallint line_number2;
	AB_Smallint order_number2;
	AB_Varchar city_id2;
	AB_Varchar itin_type3;
	AB_Smallint line_number3;
	AB_Smallint order_number3;
	AB_Varchar city_id3;
	AB_Varchar itin_type4;
	AB_Smallint line_number4;
	AB_Smallint order_number4;
	AB_Varchar city_id4;
	AB_Varchar itin_type5;
	AB_Smallint line_number5;
	AB_Smallint order_number5;
	AB_Varchar city_id5;
	AB_Varchar itin_type6;
	AB_Smallint line_number6;
	AB_Smallint order_number6;
	AB_Varchar city_id6;
};

class CIRProcessVoidTicket {
public:
// Constructor
   CIRProcessVoidTicket();

   AB_Varchar pc_form;
   AB_Varchar pc_ticket_no;
   AB_Varchar pc_reference;
};

class CIRValReasonCode {
public:
// Constructor
	CIRValReasonCode();

	AB_Varchar cop_id;
	AB_Varchar reason_code;
	AB_Varchar rc_valid;
};

class CDIR {
public:
	void createIraddress(CIRCreateIraddress &po_IRCreateIraddress_cpp);
	void createIrfrequentuser(CIRCreateIrfrequentuser &po_IRCreateIrfrequentuser_cpp);
	void createIrpayment(CIRCreateIrpayment &po_IRCreateIrpayment_cpp);
	void createIrperson(CIRCreateIrperson &po_IRCreateIrperson_cpp);
	void createIrremark(CIRCreateIrremark &po_IRCreateIrremark_cpp);
	void createIrtransport(CIRCreateIrtransport &po_IRCreateIrtransport_cpp);
	void createIrconsumdetail(CIRCreateIrconsumdetail &po_IRCreateIrconsumdetail_cpp);
	void createIrtaxdetail(CIRCreateIrtaxdetail &po_IRCreateIrtaxdetail_cpp);
	void loadClientFile(CIRLoadClientFile &po_IRLoadClientFile_cpp);
	void reject(CIRReject &po_IRReject_cpp);
	void getLocalDate(CIRGetLocalDate &po_IRGetLocalDate_cpp);
	void createIrother(CIRCreateIrother &po_IRCreateIrother_cpp);
	void cleanupIrtransport(CIRCleanupIrtransport &po_IRCleanupIrtransport_cpp);
	void createIrrental(CIRCreateIrrental &po_IRCreateIrrental_cpp);
	void createIrticket(CIRCreateIrticket &po_IRCreateIrticket_cpp);
	void createIrtripheader(CIRCreateIrtripheader &po_IRCreateIrtripheader_cpp);
	void getSupServiceType(CIRGetSupServiceType &po_IRGetSupServiceType_cpp);
	void createIrdistribution(CIRCreateIrdistribution &po_IRCreateIrdistribution_cpp);
	void validateSetFixed(CIRValidateSetFixed &po_IRValidateSetFixed_cpp);
	void createIrreceipt(CIRCreateIrreceipt &po_IRCreateIrreceipt_cpp);
	void moveQueue(CIRMoveQueue &po_IRMoveQueue_cpp);
	void createIremailaddress(CIRCreateIremailaddress &po_IRCreateIremailaddress_cpp);
	void isPFSSupplier(CIRIsPFSSupplier &po_IRIsPFSSupplier_cpp);
	void getCopIdByIata(CIRGetCopIdByIata &po_IRGetCopIdByIata_cpp);
	void updateIrRows(CIRUpdateIrRows &po_IRUpdateIrRows_cpp);
	void createIrstopovercity(CIRCreateIrstopovercity &po_IRCreateIrstopovercity_cpp);
	void processVoidTicket(CIRProcessVoidTicket &po_IRProcessVoidTicket_cpp);
	void valReasonCode(CIRValReasonCode &po_IRValReasonCode_cpp);

private:
	void createIraddress_dbToApp(SCIRCreateIraddress &ps_IRCreateIraddress,
			SCIRCreateIraddressInd &ps_IRCreateIraddressInd,
			CIRCreateIraddress &po_IRCreateIraddress_cpp);
	void createIraddress_appToDb(SCIRCreateIraddress &ps_IRCreateIraddress,
			SCIRCreateIraddressInd &ps_IRCreateIraddressInd,
			CIRCreateIraddress &po_IRCreateIraddress_cpp);
	void createIrfrequentuser_dbToApp(SCIRCreateIrfrequentuser &ps_IRCreateIrfrequentuser,
			SCIRCreateIrfrequentuserInd &ps_IRCreateIrfrequentuserInd,
			CIRCreateIrfrequentuser &po_IRCreateIrfrequentuser_cpp);
	void createIrfrequentuser_appToDb(SCIRCreateIrfrequentuser &ps_IRCreateIrfrequentuser,
			SCIRCreateIrfrequentuserInd &ps_IRCreateIrfrequentuserInd,
			CIRCreateIrfrequentuser &po_IRCreateIrfrequentuser_cpp);
	void createIrpayment_dbToApp(SCIRCreateIrpayment &ps_IRCreateIrpayment,
			SCIRCreateIrpaymentInd &ps_IRCreateIrpaymentInd,
			CIRCreateIrpayment &po_IRCreateIrpayment_cpp);
	void createIrpayment_appToDb(SCIRCreateIrpayment &ps_IRCreateIrpayment,
			SCIRCreateIrpaymentInd &ps_IRCreateIrpaymentInd,
			CIRCreateIrpayment &po_IRCreateIrpayment_cpp);
	void createIrperson_dbToApp(SCIRCreateIrperson &ps_IRCreateIrperson,
			SCIRCreateIrpersonInd &ps_IRCreateIrpersonInd,
			CIRCreateIrperson &po_IRCreateIrperson_cpp);
	void createIrperson_appToDb(SCIRCreateIrperson &ps_IRCreateIrperson,
			SCIRCreateIrpersonInd &ps_IRCreateIrpersonInd,
			CIRCreateIrperson &po_IRCreateIrperson_cpp);
	void createIrremark_dbToApp(SCIRCreateIrremark &ps_IRCreateIrremark,
			SCIRCreateIrremarkInd &ps_IRCreateIrremarkInd,
			CIRCreateIrremark &po_IRCreateIrremark_cpp);
	void createIrremark_appToDb(SCIRCreateIrremark &ps_IRCreateIrremark,
			SCIRCreateIrremarkInd &ps_IRCreateIrremarkInd,
			CIRCreateIrremark &po_IRCreateIrremark_cpp);
	void createIrtransport_dbToApp(SCIRCreateIrtransport &ps_IRCreateIrtransport,
			SCIRCreateIrtransportInd &ps_IRCreateIrtransportInd,
			CIRCreateIrtransport &po_IRCreateIrtransport_cpp);
	void createIrtransport_appToDb(SCIRCreateIrtransport &ps_IRCreateIrtransport,
			SCIRCreateIrtransportInd &ps_IRCreateIrtransportInd,
			CIRCreateIrtransport &po_IRCreateIrtransport_cpp);
	void createIrconsumdetail_dbToApp(SCIRCreateIrconsumdetail &ps_IRCreateIrconsumdetail,
			SCIRCreateIrconsumdetailInd &ps_IRCreateIrconsumdetailInd,
			CIRCreateIrconsumdetail &po_IRCreateIrconsumdetail_cpp);
	void createIrconsumdetail_appToDb(SCIRCreateIrconsumdetail &ps_IRCreateIrconsumdetail,
			SCIRCreateIrconsumdetailInd &ps_IRCreateIrconsumdetailInd,
			CIRCreateIrconsumdetail &po_IRCreateIrconsumdetail_cpp);
	void createIrtaxdetail_dbToApp(SCIRCreateIrtaxdetail &ps_IRCreateIrtaxdetail,
			SCIRCreateIrtaxdetailInd &ps_IRCreateIrtaxdetailInd,
			CIRCreateIrtaxdetail &po_IRCreateIrtaxdetail_cpp);
	void createIrtaxdetail_appToDb(SCIRCreateIrtaxdetail &ps_IRCreateIrtaxdetail,
			SCIRCreateIrtaxdetailInd &ps_IRCreateIrtaxdetailInd,
			CIRCreateIrtaxdetail &po_IRCreateIrtaxdetail_cpp);
	void loadClientFile_dbToApp(SCIRLoadClientFile &ps_IRLoadClientFile,
			SCIRLoadClientFileInd &ps_IRLoadClientFileInd,
			CIRLoadClientFile &po_IRLoadClientFile_cpp);
	void loadClientFile_appToDb(SCIRLoadClientFile &ps_IRLoadClientFile,
			SCIRLoadClientFileInd &ps_IRLoadClientFileInd,
			CIRLoadClientFile &po_IRLoadClientFile_cpp);
	void reject_dbToApp(SCIRReject &ps_IRReject,
			SCIRRejectInd &ps_IRRejectInd,
			CIRReject &po_IRReject_cpp);
	void reject_appToDb(SCIRReject &ps_IRReject,
			SCIRRejectInd &ps_IRRejectInd,
			CIRReject &po_IRReject_cpp);
	void getLocalDate_dbToApp(SCIRGetLocalDate &ps_IRGetLocalDate,
			SCIRGetLocalDateInd &ps_IRGetLocalDateInd,
			CIRGetLocalDate &po_IRGetLocalDate_cpp);
	void getLocalDate_appToDb(SCIRGetLocalDate &ps_IRGetLocalDate,
			SCIRGetLocalDateInd &ps_IRGetLocalDateInd,
			CIRGetLocalDate &po_IRGetLocalDate_cpp);
	void createIrother_dbToApp(SCIRCreateIrother &ps_IRCreateIrother,
			SCIRCreateIrotherInd &ps_IRCreateIrotherInd,
			CIRCreateIrother &po_IRCreateIrother_cpp);
	void createIrother_appToDb(SCIRCreateIrother &ps_IRCreateIrother,
			SCIRCreateIrotherInd &ps_IRCreateIrotherInd,
			CIRCreateIrother &po_IRCreateIrother_cpp);
	void cleanupIrtransport_dbToApp(SCIRCleanupIrtransport &ps_IRCleanupIrtransport,
			SCIRCleanupIrtransportInd &ps_IRCleanupIrtransportInd,
			CIRCleanupIrtransport &po_IRCleanupIrtransport_cpp);
	void cleanupIrtransport_appToDb(SCIRCleanupIrtransport &ps_IRCleanupIrtransport,
			SCIRCleanupIrtransportInd &ps_IRCleanupIrtransportInd,
			CIRCleanupIrtransport &po_IRCleanupIrtransport_cpp);
	void createIrrental_dbToApp(SCIRCreateIrrental &ps_IRCreateIrrental,
			SCIRCreateIrrentalInd &ps_IRCreateIrrentalInd,
			CIRCreateIrrental &po_IRCreateIrrental_cpp);
	void createIrrental_appToDb(SCIRCreateIrrental &ps_IRCreateIrrental,
			SCIRCreateIrrentalInd &ps_IRCreateIrrentalInd,
			CIRCreateIrrental &po_IRCreateIrrental_cpp);
	void createIrticket_dbToApp(SCIRCreateIrticket &ps_IRCreateIrticket,
			SCIRCreateIrticketInd &ps_IRCreateIrticketInd,
			CIRCreateIrticket &po_IRCreateIrticket_cpp);
	void createIrticket_appToDb(SCIRCreateIrticket &ps_IRCreateIrticket,
			SCIRCreateIrticketInd &ps_IRCreateIrticketInd,
			CIRCreateIrticket &po_IRCreateIrticket_cpp);
	void createIrtripheader_dbToApp(SCIRCreateIrtripheader &ps_IRCreateIrtripheader,
			SCIRCreateIrtripheaderInd &ps_IRCreateIrtripheaderInd,
			CIRCreateIrtripheader &po_IRCreateIrtripheader_cpp);
	void createIrtripheader_appToDb(SCIRCreateIrtripheader &ps_IRCreateIrtripheader,
			SCIRCreateIrtripheaderInd &ps_IRCreateIrtripheaderInd,
			CIRCreateIrtripheader &po_IRCreateIrtripheader_cpp);
	void getSupServiceType_dbToApp(SCIRGetSupServiceType &ps_IRGetSupServiceType,
			SCIRGetSupServiceTypeInd &ps_IRGetSupServiceTypeInd,
			CIRGetSupServiceType &po_IRGetSupServiceType_cpp);
	void getSupServiceType_appToDb(SCIRGetSupServiceType &ps_IRGetSupServiceType,
			SCIRGetSupServiceTypeInd &ps_IRGetSupServiceTypeInd,
			CIRGetSupServiceType &po_IRGetSupServiceType_cpp);
	void createIrdistribution_dbToApp(SCIRCreateIrdistribution &ps_IRCreateIrdistribution,
			SCIRCreateIrdistributionInd &ps_IRCreateIrdistributionInd,
			CIRCreateIrdistribution &po_IRCreateIrdistribution_cpp);
	void createIrdistribution_appToDb(SCIRCreateIrdistribution &ps_IRCreateIrdistribution,
			SCIRCreateIrdistributionInd &ps_IRCreateIrdistributionInd,
			CIRCreateIrdistribution &po_IRCreateIrdistribution_cpp);
	void validateSetFixed_dbToApp(SCIRValidateSetFixed &ps_IRValidateSetFixed,
			SCIRValidateSetFixedInd &ps_IRValidateSetFixedInd,
			CIRValidateSetFixed &po_IRValidateSetFixed_cpp);
	void validateSetFixed_appToDb(SCIRValidateSetFixed &ps_IRValidateSetFixed,
			SCIRValidateSetFixedInd &ps_IRValidateSetFixedInd,
			CIRValidateSetFixed &po_IRValidateSetFixed_cpp);
	void createIrreceipt_dbToApp(SCIRCreateIrreceipt &ps_IRCreateIrreceipt,
			SCIRCreateIrreceiptInd &ps_IRCreateIrreceiptInd,
			CIRCreateIrreceipt &po_IRCreateIrreceipt_cpp);
	void createIrreceipt_appToDb(SCIRCreateIrreceipt &ps_IRCreateIrreceipt,
			SCIRCreateIrreceiptInd &ps_IRCreateIrreceiptInd,
			CIRCreateIrreceipt &po_IRCreateIrreceipt_cpp);
	void moveQueue_dbToApp(SCIRMoveQueue &ps_IRMoveQueue,
			SCIRMoveQueueInd &ps_IRMoveQueueInd,
			CIRMoveQueue &po_IRMoveQueue_cpp);
	void moveQueue_appToDb(SCIRMoveQueue &ps_IRMoveQueue,
			SCIRMoveQueueInd &ps_IRMoveQueueInd,
			CIRMoveQueue &po_IRMoveQueue_cpp);
	void createIremailaddress_dbToApp(SCIRCreateIremailaddress &ps_IRCreateIremailaddress,
			SCIRCreateIremailaddressInd &ps_IRCreateIremailaddressInd,
			CIRCreateIremailaddress &po_IRCreateIremailaddress_cpp);
	void createIremailaddress_appToDb(SCIRCreateIremailaddress &ps_IRCreateIremailaddress,
			SCIRCreateIremailaddressInd &ps_IRCreateIremailaddressInd,
			CIRCreateIremailaddress &po_IRCreateIremailaddress_cpp);
	void isPFSSupplier_dbToApp(SCIRIsPFSSupplier &ps_IRIsPFSSupplier,
			SCIRIsPFSSupplierInd &ps_IRIsPFSSupplierInd,
			CIRIsPFSSupplier &po_IRIsPFSSupplier_cpp);
	void isPFSSupplier_appToDb(SCIRIsPFSSupplier &ps_IRIsPFSSupplier,
			SCIRIsPFSSupplierInd &ps_IRIsPFSSupplierInd,
			CIRIsPFSSupplier &po_IRIsPFSSupplier_cpp);
	void getCopIdByIata_dbToApp(SCIRGetCopIdByIata &ps_IRGetCopIdByIata,
			SCIRGetCopIdByIataInd &ps_IRGetCopIdByIataInd,
			CIRGetCopIdByIata &po_IRGetCopIdByIata_cpp);
	void getCopIdByIata_appToDb(SCIRGetCopIdByIata &ps_IRGetCopIdByIata,
			SCIRGetCopIdByIataInd &ps_IRGetCopIdByIataInd,
			CIRGetCopIdByIata &po_IRGetCopIdByIata_cpp);
	void updateIrRows_dbToApp(SCIRUpdateIrRows &ps_IRUpdateIrRows,
			SCIRUpdateIrRowsInd &ps_IRUpdateIrRowsInd,
			CIRUpdateIrRows &po_IRUpdateIrRows_cpp);
	void updateIrRows_appToDb(SCIRUpdateIrRows &ps_IRUpdateIrRows,
			SCIRUpdateIrRowsInd &ps_IRUpdateIrRowsInd,
			CIRUpdateIrRows &po_IRUpdateIrRows_cpp);
	void createIrstopovercity_dbToApp(SCIRCreateIrstopovercity &ps_IRCreateIrstopovercity,
			SCIRCreateIrstopovercityInd &ps_IRCreateIrstopovercityInd,
			CIRCreateIrstopovercity &po_IRCreateIrstopovercity_cpp);
	void createIrstopovercity_appToDb(SCIRCreateIrstopovercity &ps_IRCreateIrstopovercity,
			SCIRCreateIrstopovercityInd &ps_IRCreateIrstopovercityInd,
			CIRCreateIrstopovercity &po_IRCreateIrstopovercity_cpp);
   void processVoidTicket_dbToApp(SCIRProcessVoidTicket &ps_IRProcessVoidTicket,
         SCIRProcessVoidTicketInd &ps_IRProcessVoidTicketInd,
         CIRProcessVoidTicket &po_IRProcessVoidTicket_cpp);
   void processVoidTicket_appToDb(SCIRProcessVoidTicket &ps_IRProcessVoidTicket,
         SCIRProcessVoidTicketInd &ps_IRProcessVoidTicketInd,
         CIRProcessVoidTicket &po_IRProcessVoidTicket_cpp);
	void valReasonCode_dbToApp(SCIRValReasonCode &ps_IRValReasonCode,
			SCIRValReasonCodeInd &ps_IRValReasonCodeInd,
			CIRValReasonCode &po_IRValReasonCode_cpp);
	void valReasonCode_appToDb(SCIRValReasonCode &ps_IRValReasonCode,
			SCIRValReasonCodeInd &ps_IRValReasonCodeInd,
			CIRValReasonCode &po_IRValReasonCode_cpp);
};

#endif /* dbIR_h */
