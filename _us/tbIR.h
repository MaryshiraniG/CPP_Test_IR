/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.96
      Delta Created: 17/10/18 - 15:12:30
         Last Delta: app/src/common/InterfaceRecord/_us 10/18/17 - 15:12:30 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.tbIR.h) 

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

#ifndef IRCreateIraddress_h
#define IRCreateIraddress_h

typedef struct SCIRCreateIraddress {
	char c_session_id[8+1];
	char c_type1[1+1];
	CS_CHAR c_addr11[80+1];
	CS_CHAR c_addr21[80+1];
	CS_CHAR c_addr31[80+1];
	CS_CHAR c_addr41[80+1];
	CS_CHAR c_addr51[80+1];
	CS_CHAR c_addr61[80+1];
	CS_CHAR c_phone_acode1[5+1];
	CS_CHAR c_phone_no1[10+1];
	CS_CHAR c_bphone_acode1[5+1];
	CS_CHAR c_bphone_no1[10+1];
	CS_CHAR c_fax_acode1[5+1];
	CS_CHAR c_fax_no1[10+1];
	char c_type2[1+1];
	CS_CHAR c_addr12[80+1];
	CS_CHAR c_addr22[80+1];
	CS_CHAR c_addr32[80+1];
	CS_CHAR c_addr42[80+1];
	CS_CHAR c_addr52[80+1];
	CS_CHAR c_addr62[80+1];
	CS_CHAR c_phone_acode2[5+1];
	CS_CHAR c_phone_no2[10+1];
	CS_CHAR c_bphone_acode2[5+1];
	CS_CHAR c_bphone_no2[10+1];
	CS_CHAR c_fax_acode2[5+1];
	CS_CHAR c_fax_no2[10+1];
	CS_INT c_pi_sqlcode;
	CS_CHAR c_mphone_acode1[5+1];
	CS_CHAR c_mphone_no1[10+1];
	CS_CHAR c_mphone_acode2[5+1];
	CS_CHAR c_mphone_no2[10+1];
} SCIRCreateIraddress;

typedef struct SCIRCreateIraddressInd {
	short ind_session_id;
	short ind_type1;
	short ind_addr11;
	short ind_addr21;
	short ind_addr31;
	short ind_addr41;
	short ind_addr51;
	short ind_addr61;
	short ind_phone_acode1;
	short ind_phone_no1;
	short ind_bphone_acode1;
	short ind_bphone_no1;
	short ind_fax_acode1;
	short ind_fax_no1;
	short ind_type2;
	short ind_addr12;
	short ind_addr22;
	short ind_addr32;
	short ind_addr42;
	short ind_addr52;
	short ind_addr62;
	short ind_phone_acode2;
	short ind_phone_no2;
	short ind_bphone_acode2;
	short ind_bphone_no2;
	short ind_fax_acode2;
	short ind_fax_no2;
	short ind_pi_sqlcode;
	short ind_mphone_acode1;
	short ind_mphone_no1;
	short ind_mphone_acode2;
	short ind_mphone_no2;
} SCIRCreateIraddressInd;

#endif /* IRCreateIraddress_h */
/* ========================================================================= */

#ifndef IRCreateIrfrequentuser_h
#define IRCreateIrfrequentuser_h

typedef struct SCIRCreateIrfrequentuser {
	char c_session_id[8+1];
	CS_SMALLINT c_pax_id1;
	char c_sup_id1[3+1];
	CS_CHAR c_fup_type1[10+1];
	CS_CHAR c_fup1[40+1];
	char c_fup_status1[2+1];
	char c_srvc_sup_id1[3+1];
	CS_SMALLINT c_pax_id2;
	char c_sup_id2[3+1];
	CS_CHAR c_fup_type2[10+1];
	CS_CHAR c_fup2[40+1];
	char c_fup_status2[2+1];
	char c_srvc_sup_id2[3+1];
	CS_SMALLINT c_pax_id3;
	char c_sup_id3[3+1];
	CS_CHAR c_fup_type3[10+1];
	CS_CHAR c_fup3[40+1];
	char c_fup_status3[2+1];
	char c_srvc_sup_id3[3+1];
	CS_SMALLINT c_pax_id4;
	char c_sup_id4[3+1];
	CS_CHAR c_fup_type4[10+1];
	CS_CHAR c_fup4[40+1];
	char c_fup_status4[2+1];
	char c_srvc_sup_id4[3+1];
	CS_SMALLINT c_pax_id5;
	char c_sup_id5[3+1];
	CS_CHAR c_fup_type5[10+1];
	CS_CHAR c_fup5[40+1];
	char c_fup_status5[2+1];
	char c_srvc_sup_id5[3+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrfrequentuser;

typedef struct SCIRCreateIrfrequentuserInd {
	short ind_session_id;
	short ind_pax_id1;
	short ind_sup_id1;
	short ind_fup_type1;
	short ind_fup1;
	short ind_fup_status1;
	short ind_srvc_sup_id1;
	short ind_pax_id2;
	short ind_sup_id2;
	short ind_fup_type2;
	short ind_fup2;
	short ind_fup_status2;
	short ind_srvc_sup_id2;
	short ind_pax_id3;
	short ind_sup_id3;
	short ind_fup_type3;
	short ind_fup3;
	short ind_fup_status3;
	short ind_srvc_sup_id3;
	short ind_pax_id4;
	short ind_sup_id4;
	short ind_fup_type4;
	short ind_fup4;
	short ind_fup_status4;
	short ind_srvc_sup_id4;
	short ind_pax_id5;
	short ind_sup_id5;
	short ind_fup_type5;
	short ind_fup5;
	short ind_fup_status5;
	short ind_srvc_sup_id5;
	short ind_pi_sqlcode;
} SCIRCreateIrfrequentuserInd;

#endif /* IRCreateIrfrequentuser_h */
/* ========================================================================= */

#ifndef IRCreateIrpayment_h
#define IRCreateIrpayment_h

typedef struct SCIRCreateIrpayment {
	char c_session_id[8+1];
	CS_SMALLINT c_pax_id1;
	CS_SMALLINT c_line_number1;
	CS_SMALLINT c_pay_id1;
	char c_form_of_pay1[2+1];
	CS_FLOAT c_amount1;
	char c_cc_id1[2+1];
	CS_CHAR c_cc_no1[30+1];
	char c_cc_exp_date1[4+1];
	CS_CHAR c_auth_no1[20+1];
	char c_auth_source1[1+1];
	CS_SMALLINT c_pax_id2;
	CS_SMALLINT c_line_number2;
	CS_SMALLINT c_pay_id2;
	char c_form_of_pay2[2+1];
	CS_FLOAT c_amount2;
	char c_cc_id2[2+1];
	CS_CHAR c_cc_no2[30+1];
	char c_cc_exp_date2[4+1];
	CS_CHAR c_auth_no2[20+1];
	char c_auth_source2[1+1];
	CS_SMALLINT c_pax_id3;
	CS_SMALLINT c_line_number3;
	CS_SMALLINT c_pay_id3;
	char c_form_of_pay3[2+1];
	CS_FLOAT c_amount3;
	char c_cc_id3[2+1];
	CS_CHAR c_cc_no3[30+1];
	char c_cc_exp_date3[4+1];
	CS_CHAR c_auth_no3[20+1];
	char c_auth_source3[1+1];
	CS_SMALLINT c_pax_id4;
	CS_SMALLINT c_line_number4;
	CS_SMALLINT c_pay_id4;
	char c_form_of_pay4[2+1];
	CS_FLOAT c_amount4;
	char c_cc_id4[2+1];
	CS_CHAR c_cc_no4[30+1];
	char c_cc_exp_date4[4+1];
	CS_CHAR c_auth_no4[20+1];
	char c_auth_source4[1+1];
	CS_SMALLINT c_pax_id5;
	CS_SMALLINT c_line_number5;
	CS_SMALLINT c_pay_id5;
	char c_form_of_pay5[2+1];
	CS_FLOAT c_amount5;
	char c_cc_id5[2+1];
	CS_CHAR c_cc_no5[30+1];
	char c_cc_exp_date5[4+1];
	CS_CHAR c_auth_no5[20+1];
	char c_auth_source5[1+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrpayment;

typedef struct SCIRCreateIrpaymentInd {
	short ind_session_id;
	short ind_pax_id1;
	short ind_line_number1;
	short ind_pay_id1;
	short ind_form_of_pay1;
	short ind_amount1;
	short ind_cc_id1;
	short ind_cc_no1;
	short ind_cc_exp_date1;
	short ind_auth_no1;
	short ind_auth_source1;
	short ind_pax_id2;
	short ind_line_number2;
	short ind_pay_id2;
	short ind_form_of_pay2;
	short ind_amount2;
	short ind_cc_id2;
	short ind_cc_no2;
	short ind_cc_exp_date2;
	short ind_auth_no2;
	short ind_auth_source2;
	short ind_pax_id3;
	short ind_line_number3;
	short ind_pay_id3;
	short ind_form_of_pay3;
	short ind_amount3;
	short ind_cc_id3;
	short ind_cc_no3;
	short ind_cc_exp_date3;
	short ind_auth_no3;
	short ind_auth_source3;
	short ind_pax_id4;
	short ind_line_number4;
	short ind_pay_id4;
	short ind_form_of_pay4;
	short ind_amount4;
	short ind_cc_id4;
	short ind_cc_no4;
	short ind_cc_exp_date4;
	short ind_auth_no4;
	short ind_auth_source4;
	short ind_pax_id5;
	short ind_line_number5;
	short ind_pay_id5;
	short ind_form_of_pay5;
	short ind_amount5;
	short ind_cc_id5;
	short ind_cc_no5;
	short ind_cc_exp_date5;
	short ind_auth_no5;
	short ind_auth_source5;
	short ind_pi_sqlcode;
} SCIRCreateIrpaymentInd;

#endif /* IRCreateIrpayment_h */
/* ========================================================================= */

#ifndef IRCreateIrperson_h
#define IRCreateIrperson_h

typedef struct SCIRCreateIrperson {
	char c_session_id[8+1];
	CS_SMALLINT c_pax_id1;
	CS_CHAR c_title1[40+1];
	CS_CHAR c_last_name1[40+1];
	CS_CHAR c_first_name1[40+1];
	CS_SMALLINT c_new_pax_id1;
	CS_SMALLINT c_new_pax_line1;
	char c_sex1[1+1];
	char c_smoking1[1+1];
	char c_ctry_id1[2+1];
	CS_CHAR c_passport_no1[40+1];
	char c_passport_exp1[7+1];
	CS_CHAR c_reference1[40+1];
	CS_CHAR c_crs_invoice1[10+1];
	CS_SMALLINT c_traveller_id1;
	CS_SMALLINT c_pax_id2;
	CS_CHAR c_title2[40+1];
	CS_CHAR c_last_name2[40+1];
	CS_CHAR c_first_name2[40+1];
	CS_SMALLINT c_new_pax_id2;
	CS_SMALLINT c_new_pax_line2;
	char c_sex2[1+1];
	char c_smoking2[1+1];
	char c_ctry_id2[2+1];
	CS_CHAR c_passport_no2[40+1];
	char c_passport_exp2[7+1];
	CS_CHAR c_reference2[40+1];
	CS_CHAR c_crs_invoice2[10+1];
	CS_SMALLINT c_traveller_id2;
	CS_SMALLINT c_pax_id3;
	CS_CHAR c_title3[40+1];
	CS_CHAR c_last_name3[40+1];
	CS_CHAR c_first_name3[40+1];
	CS_SMALLINT c_new_pax_id3;
	CS_SMALLINT c_new_pax_line3;
	char c_sex3[1+1];
	char c_smoking3[1+1];
	char c_ctry_id3[2+1];
	CS_CHAR c_passport_no3[40+1];
	char c_passport_exp3[7+1];
	CS_CHAR c_reference3[40+1];
	CS_CHAR c_crs_invoice3[10+1];
	CS_SMALLINT c_traveller_id3;
	CS_SMALLINT c_pax_id4;
	CS_CHAR c_title4[40+1];
	CS_CHAR c_last_name4[40+1];
	CS_CHAR c_first_name4[40+1];
	CS_SMALLINT c_new_pax_id4;
	CS_SMALLINT c_new_pax_line4;
	char c_sex4[1+1];
	char c_smoking4[1+1];
	char c_ctry_id4[2+1];
	CS_CHAR c_passport_no4[40+1];
	char c_passport_exp4[7+1];
	CS_CHAR c_reference4[40+1];
	CS_CHAR c_crs_invoice4[10+1];
	CS_SMALLINT c_traveller_id4;
	CS_INT c_pi_sqlcode;
} SCIRCreateIrperson;

typedef struct SCIRCreateIrpersonInd {
	short ind_session_id;
	short ind_pax_id1;
	short ind_title1;
	short ind_last_name1;
	short ind_first_name1;
	short ind_new_pax_id1;
	short ind_new_pax_line1;
	short ind_sex1;
	short ind_smoking1;
	short ind_ctry_id1;
	short ind_passport_no1;
	short ind_passport_exp1;
	short ind_reference1;
	short ind_crs_invoice1;
	short ind_traveller_id1;
	short ind_pax_id2;
	short ind_title2;
	short ind_last_name2;
	short ind_first_name2;
	short ind_new_pax_id2;
	short ind_new_pax_line2;
	short ind_sex2;
	short ind_smoking2;
	short ind_ctry_id2;
	short ind_passport_no2;
	short ind_passport_exp2;
	short ind_reference2;
	short ind_crs_invoice2;
	short ind_traveller_id2;
	short ind_pax_id3;
	short ind_title3;
	short ind_last_name3;
	short ind_first_name3;
	short ind_new_pax_id3;
	short ind_new_pax_line3;
	short ind_sex3;
	short ind_smoking3;
	short ind_ctry_id3;
	short ind_passport_no3;
	short ind_passport_exp3;
	short ind_reference3;
	short ind_crs_invoice3;
	short ind_traveller_id3;
	short ind_pax_id4;
	short ind_title4;
	short ind_last_name4;
	short ind_first_name4;
	short ind_new_pax_id4;
	short ind_new_pax_line4;
	short ind_sex4;
	short ind_smoking4;
	short ind_ctry_id4;
	short ind_passport_no4;
	short ind_passport_exp4;
	short ind_reference4;
	short ind_crs_invoice4;
	short ind_traveller_id4;
	short ind_pi_sqlcode;
} SCIRCreateIrpersonInd;

#endif /* IRCreateIrperson_h */
/* ========================================================================= */

#ifndef IRCreateIrremark_h
#define IRCreateIrremark_h

typedef struct SCIRCreateIrremark {
	char c_session_id[8+1];
	CS_SMALLINT c_line_number1;
	CS_SMALLINT c_pax_id1;
	char c_type1[3+1];
	CS_CHAR c_remark1[255+1];
	char c_itin_type1[1+1];
	CS_SMALLINT c_tra_line_number1;
	CS_SMALLINT c_ren_line_number1;
	CS_SMALLINT c_oth_line_number1;
	CS_SMALLINT c_udi_line_number1;
	CS_SMALLINT c_line_number2;
	CS_SMALLINT c_pax_id2;
	char c_type2[3+1];
	CS_CHAR c_remark2[255+1];
	char c_itin_type2[1+1];
	CS_SMALLINT c_tra_line_number2;
	CS_SMALLINT c_ren_line_number2;
	CS_SMALLINT c_oth_line_number2;
	CS_SMALLINT c_udi_line_number2;
	CS_SMALLINT c_line_number3;
	CS_SMALLINT c_pax_id3;
	char c_type3[3+1];
	CS_CHAR c_remark3[255+1];
	char c_itin_type3[1+1];
	CS_SMALLINT c_tra_line_number3;
	CS_SMALLINT c_ren_line_number3;
	CS_SMALLINT c_oth_line_number3;
	CS_SMALLINT c_udi_line_number3;
	CS_SMALLINT c_line_number4;
	CS_SMALLINT c_pax_id4;
	char c_type4[3+1];
	CS_CHAR c_remark4[255+1];
	char c_itin_type4[1+1];
	CS_SMALLINT c_tra_line_number4;
	CS_SMALLINT c_ren_line_number4;
	CS_SMALLINT c_oth_line_number4;
	CS_SMALLINT c_udi_line_number4;
	CS_SMALLINT c_line_number5;
	CS_SMALLINT c_pax_id5;
	char c_type5[3+1];
	CS_CHAR c_remark5[255+1];
	char c_itin_type5[1+1];
	CS_SMALLINT c_tra_line_number5;
	CS_SMALLINT c_ren_line_number5;
	CS_SMALLINT c_oth_line_number5;
	CS_SMALLINT c_udi_line_number5;
	CS_SMALLINT c_line_number6;
	CS_SMALLINT c_pax_id6;
	char c_type6[3+1];
	CS_CHAR c_remark6[255+1];
	char c_itin_type6[1+1];
	CS_SMALLINT c_tra_line_number6;
	CS_SMALLINT c_ren_line_number6;
	CS_SMALLINT c_oth_line_number6;
	CS_SMALLINT c_udi_line_number6;
	CS_SMALLINT c_line_number7;
	CS_SMALLINT c_pax_id7;
	char c_type7[3+1];
	CS_CHAR c_remark7[255+1];
	char c_itin_type7[1+1];
	CS_SMALLINT c_tra_line_number7;
	CS_SMALLINT c_ren_line_number7;
	CS_SMALLINT c_oth_line_number7;
	CS_SMALLINT c_udi_line_number7;
	CS_SMALLINT c_line_number8;
	CS_SMALLINT c_pax_id8;
	char c_type8[3+1];
	CS_CHAR c_remark8[255+1];
	char c_itin_type8[1+1];
	CS_SMALLINT c_tra_line_number8;
	CS_SMALLINT c_ren_line_number8;
	CS_SMALLINT c_oth_line_number8;
	CS_SMALLINT c_udi_line_number8;
	CS_SMALLINT c_line_number9;
	CS_SMALLINT c_pax_id9;
	char c_type9[3+1];
	CS_CHAR c_remark9[255+1];
	char c_itin_type9[1+1];
	CS_SMALLINT c_tra_line_number9;
	CS_SMALLINT c_ren_line_number9;
	CS_SMALLINT c_oth_line_number9;
	CS_SMALLINT c_udi_line_number9;
	CS_SMALLINT c_line_number0;
	CS_SMALLINT c_pax_id0;
	char c_type0[3+1];
	CS_CHAR c_remark0[255+1];
	char c_itin_type0[1+1];
	CS_SMALLINT c_tra_line_number0;
	CS_SMALLINT c_ren_line_number0;
	CS_SMALLINT c_oth_line_number0;
	CS_SMALLINT c_udi_line_number0;
	CS_INT c_pi_sqlcode;
} SCIRCreateIrremark;

typedef struct SCIRCreateIrremarkInd {
	short ind_session_id;
	short ind_line_number1;
	short ind_pax_id1;
	short ind_type1;
	short ind_remark1;
	short ind_itin_type1;
	short ind_tra_line_number1;
	short ind_ren_line_number1;
	short ind_oth_line_number1;
	short ind_udi_line_number1;
	short ind_line_number2;
	short ind_pax_id2;
	short ind_type2;
	short ind_remark2;
	short ind_itin_type2;
	short ind_tra_line_number2;
	short ind_ren_line_number2;
	short ind_oth_line_number2;
	short ind_udi_line_number2;
	short ind_line_number3;
	short ind_pax_id3;
	short ind_type3;
	short ind_remark3;
	short ind_itin_type3;
	short ind_tra_line_number3;
	short ind_ren_line_number3;
	short ind_oth_line_number3;
	short ind_udi_line_number3;
	short ind_line_number4;
	short ind_pax_id4;
	short ind_type4;
	short ind_remark4;
	short ind_itin_type4;
	short ind_tra_line_number4;
	short ind_ren_line_number4;
	short ind_oth_line_number4;
	short ind_udi_line_number4;
	short ind_line_number5;
	short ind_pax_id5;
	short ind_type5;
	short ind_remark5;
	short ind_itin_type5;
	short ind_tra_line_number5;
	short ind_ren_line_number5;
	short ind_oth_line_number5;
	short ind_udi_line_number5;
	short ind_line_number6;
	short ind_pax_id6;
	short ind_type6;
	short ind_remark6;
	short ind_itin_type6;
	short ind_tra_line_number6;
	short ind_ren_line_number6;
	short ind_oth_line_number6;
	short ind_udi_line_number6;
	short ind_line_number7;
	short ind_pax_id7;
	short ind_type7;
	short ind_remark7;
	short ind_itin_type7;
	short ind_tra_line_number7;
	short ind_ren_line_number7;
	short ind_oth_line_number7;
	short ind_udi_line_number7;
	short ind_line_number8;
	short ind_pax_id8;
	short ind_type8;
	short ind_remark8;
	short ind_itin_type8;
	short ind_tra_line_number8;
	short ind_ren_line_number8;
	short ind_oth_line_number8;
	short ind_udi_line_number8;
	short ind_line_number9;
	short ind_pax_id9;
	short ind_type9;
	short ind_remark9;
	short ind_itin_type9;
	short ind_tra_line_number9;
	short ind_ren_line_number9;
	short ind_oth_line_number9;
	short ind_udi_line_number9;
	short ind_line_number0;
	short ind_pax_id0;
	short ind_type0;
	short ind_remark0;
	short ind_itin_type0;
	short ind_tra_line_number0;
	short ind_ren_line_number0;
	short ind_oth_line_number0;
	short ind_udi_line_number0;
	short ind_pi_sqlcode;
} SCIRCreateIrremarkInd;

#endif /* IRCreateIrremark_h */
/* ========================================================================= */

#ifndef IRCreateIrtransport_h
#define IRCreateIrtransport_h

typedef struct SCIRCreateIrtransport {
	char c_session_id[8+1];
	char c_itin_type[1+1];
	CS_SMALLINT c_line_number;
	char c_start_date[7+1];
	CS_CHAR c_start_time[8+1];
	char c_orig_city_id[4+1];
	CS_CHAR c_orig_cityname[80+1];
	char c_cog_dep_city[4+1];
	char c_cog_dep_date[7+1];
	CS_CHAR c_cog_dep_time[8+1];
	CS_CHAR c_cog_dep_term[10+1];
	char c_cog_dep_gate[4+1];
	CS_CHAR c_cog_of_miles[10+1];
	char c_end_date[7+1];
	CS_CHAR c_end_time[8+1];
	char c_dest_city_id[4+1];
	CS_CHAR c_dest_cityname[80+1];
	char c_classofservice[4+1];
	CS_CHAR c_service[10+1];
	char c_sup_id[3+1];
	CS_CHAR c_reference[40+1];
	CS_CHAR c_chg_of_gauge[10+1];
	CS_CHAR c_no_of_stops[15+1];
	CS_CHAR c_dep_terminal[26+1];
	char c_dep_gate[4+1];
	char c_report_time[4+1];
	char c_arr_gate[4+1];
	CS_CHAR c_arr_terminal[26+1];
	CS_CHAR c_airline_locator[8+1];
	CS_CHAR c_carrier_type[10+1];
	CS_CHAR c_meal[6+1];
	CS_CHAR c_aff_carrier[80+1];
	CS_INT c_actual_miles;
	char c_arrival_ind[2+1];
	char c_elapsed_time[4+1];
	char c_itin_type1[1+1];
	CS_SMALLINT c_line_number1;
	char c_start_date1[7+1];
	CS_CHAR c_start_time1[8+1];
	char c_orig_city_id1[4+1];
	CS_CHAR c_orig_cityname1[80+1];
	char c_cog_dep_city1[4+1];
	char c_cog_dep_date1[7+1];
	CS_CHAR c_cog_dep_time1[8+1];
	CS_CHAR c_cog_dep_term1[10+1];
	char c_cog_dep_gate1[4+1];
	CS_CHAR c_cog_of_miles1[10+1];
	char c_end_date1[7+1];
	CS_CHAR c_end_time1[8+1];
	char c_dest_city_id1[4+1];
	CS_CHAR c_dest_cityname1[80+1];
	char c_classofservice1[4+1];
	CS_CHAR c_service1[10+1];
	char c_sup_id1[3+1];
	CS_CHAR c_reference1[40+1];
	CS_CHAR c_chg_of_gauge1[10+1];
	CS_CHAR c_no_of_stops1[15+1];
	CS_CHAR c_dep_terminal1[26+1];
	char c_dep_gate1[4+1];
	char c_report_time1[4+1];
	char c_arr_gate1[4+1];
	CS_CHAR c_arr_terminal1[26+1];
	CS_CHAR c_airline_locator1[8+1];
	CS_CHAR c_carrier_type1[10+1];
	CS_CHAR c_meal1[6+1];
	CS_CHAR c_aff_carrier1[80+1];
	CS_INT c_actual_miles1;
	char c_arrival_ind1[2+1];
	char c_elapsed_time1[4+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrtransport;

typedef struct SCIRCreateIrtransportInd {
	short ind_session_id;
	short ind_itin_type;
	short ind_line_number;
	short ind_start_date;
	short ind_start_time;
	short ind_orig_city_id;
	short ind_orig_cityname;
	short ind_cog_dep_city;
	short ind_cog_dep_date;
	short ind_cog_dep_time;
	short ind_cog_dep_term;
	short ind_cog_dep_gate;
	short ind_cog_of_miles;
	short ind_end_date;
	short ind_end_time;
	short ind_dest_city_id;
	short ind_dest_cityname;
	short ind_classofservice;
	short ind_service;
	short ind_sup_id;
	short ind_reference;
	short ind_chg_of_gauge;
	short ind_no_of_stops;
	short ind_dep_terminal;
	short ind_dep_gate;
	short ind_report_time;
	short ind_arr_gate;
	short ind_arr_terminal;
	short ind_airline_locator;
	short ind_carrier_type;
	short ind_meal;
	short ind_aff_carrier;
	short ind_actual_miles;
	short ind_arrival_ind;
	short ind_elapsed_time;
	short ind_itin_type1;
	short ind_line_number1;
	short ind_start_date1;
	short ind_start_time1;
	short ind_orig_city_id1;
	short ind_orig_cityname1;
	short ind_cog_dep_city1;
	short ind_cog_dep_date1;
	short ind_cog_dep_time1;
	short ind_cog_dep_term1;
	short ind_cog_dep_gate1;
	short ind_cog_of_miles1;
	short ind_end_date1;
	short ind_end_time1;
	short ind_dest_city_id1;
	short ind_dest_cityname1;
	short ind_classofservice1;
	short ind_service1;
	short ind_sup_id1;
	short ind_reference1;
	short ind_chg_of_gauge1;
	short ind_no_of_stops1;
	short ind_dep_terminal1;
	short ind_dep_gate1;
	short ind_report_time1;
	short ind_arr_gate1;
	short ind_arr_terminal1;
	short ind_airline_locator1;
	short ind_carrier_type1;
	short ind_meal1;
	short ind_aff_carrier1;
	short ind_actual_miles1;
	short ind_arrival_ind1;
	short ind_elapsed_time1;
	short ind_pi_sqlcode;
} SCIRCreateIrtransportInd;

#endif /* IRCreateIrtransport_h */
/* ========================================================================= */

#ifndef IRCreateIrconsumdetail_h
#define IRCreateIrconsumdetail_h

typedef struct SCIRCreateIrconsumdetail {
	char c_session_id[8+1];
	CS_SMALLINT c_pax_id1;
	char c_itin_type1[1+1];
	CS_SMALLINT c_itin_line_number1;
	CS_SMALLINT c_line_number1;
	CS_CHAR c_seat1[4+1];
	char c_connection1[1+1];
	char c_smoking1[1+1];
	CS_FLOAT c_fare_by_leg1;
	CS_CHAR c_fare_basis1[20+1];
	CS_CHAR c_tkt_desig1[20+1];
	char c_cog_seat1[1+1];
	char c_service_status1[2+1];
	CS_INT c_ftp_miles1;
	char c_boarding_pass1[1+1];
	CS_CHAR c_special_meal1[6+1];
	CS_CHAR c_special_need1[80+1];
	char c_baggage1[3+1];
	char c_crs_priced1[1+1];
	char c_cur_id1[3+1];
	CS_SMALLINT c_pax_id2;
	char c_itin_type2[1+1];
	CS_SMALLINT c_itin_line_number2;
	CS_SMALLINT c_line_number2;
	CS_CHAR c_seat2[4+1];
	char c_connection2[1+1];
	char c_smoking2[1+1];
	CS_FLOAT c_fare_by_leg2;
	CS_CHAR c_fare_basis2[20+1];
	CS_CHAR c_tkt_desig2[20+1];
	char c_cog_seat2[1+1];
	char c_service_status2[2+1];
	CS_INT c_ftp_miles2;
	char c_boarding_pass2[1+1];
	CS_CHAR c_special_meal2[6+1];
	CS_CHAR c_special_need2[80+1];
	char c_baggage2[3+1];
	char c_crs_priced2[1+1];
	char c_cur_id2[3+1];
	CS_SMALLINT c_pax_id3;
	char c_itin_type3[1+1];
	CS_SMALLINT c_itin_line_number3;
	CS_SMALLINT c_line_number3;
	CS_CHAR c_seat3[4+1];
	char c_connection3[1+1];
	char c_smoking3[1+1];
	CS_FLOAT c_fare_by_leg3;
	CS_CHAR c_fare_basis3[20+1];
	CS_CHAR c_tkt_desig3[20+1];
	char c_cog_seat3[1+1];
	char c_service_status3[2+1];
	CS_INT c_ftp_miles3;
	char c_boarding_pass3[1+1];
	CS_CHAR c_special_meal3[6+1];
	CS_CHAR c_special_need3[80+1];
	char c_baggage3[3+1];
	char c_crs_priced3[1+1];
	char c_cur_id3[3+1];
	CS_SMALLINT c_pax_id4;
	char c_itin_type4[1+1];
	CS_SMALLINT c_itin_line_number4;
	CS_SMALLINT c_line_number4;
	CS_CHAR c_seat4[4+1];
	char c_connection4[1+1];
	char c_smoking4[1+1];
	CS_FLOAT c_fare_by_leg4;
	CS_CHAR c_fare_basis4[20+1];
	CS_CHAR c_tkt_desig4[20+1];
	char c_cog_seat4[1+1];
	char c_service_status4[2+1];
	CS_INT c_ftp_miles4;
	char c_boarding_pass4[1+1];
	CS_CHAR c_special_meal4[6+1];
	CS_CHAR c_special_need4[80+1];
	char c_baggage4[3+1];
	char c_crs_priced4[1+1];
	char c_cur_id4[3+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrconsumdetail;

typedef struct SCIRCreateIrconsumdetailInd {
	short ind_session_id;
	short ind_pax_id1;
	short ind_itin_type1;
	short ind_itin_line_number1;
	short ind_line_number1;
	short ind_seat1;
	short ind_connection1;
	short ind_smoking1;
	short ind_fare_by_leg1;
	short ind_fare_basis1;
	short ind_tkt_desig1;
	short ind_cog_seat1;
	short ind_service_status1;
	short ind_ftp_miles1;
	short ind_boarding_pass1;
	short ind_special_meal1;
	short ind_special_need1;
	short ind_baggage1;
	short ind_crs_priced1;
	short ind_cur_id1;
	short ind_pax_id2;
	short ind_itin_type2;
	short ind_itin_line_number2;
	short ind_line_number2;
	short ind_seat2;
	short ind_connection2;
	short ind_smoking2;
	short ind_fare_by_leg2;
	short ind_fare_basis2;
	short ind_tkt_desig2;
	short ind_cog_seat2;
	short ind_service_status2;
	short ind_ftp_miles2;
	short ind_boarding_pass2;
	short ind_special_meal2;
	short ind_special_need2;
	short ind_baggage2;
	short ind_crs_priced2;
	short ind_cur_id2;
	short ind_pax_id3;
	short ind_itin_type3;
	short ind_itin_line_number3;
	short ind_line_number3;
	short ind_seat3;
	short ind_connection3;
	short ind_smoking3;
	short ind_fare_by_leg3;
	short ind_fare_basis3;
	short ind_tkt_desig3;
	short ind_cog_seat3;
	short ind_service_status3;
	short ind_ftp_miles3;
	short ind_boarding_pass3;
	short ind_special_meal3;
	short ind_special_need3;
	short ind_baggage3;
	short ind_crs_priced3;
	short ind_cur_id3;
	short ind_pax_id4;
	short ind_itin_type4;
	short ind_itin_line_number4;
	short ind_line_number4;
	short ind_seat4;
	short ind_connection4;
	short ind_smoking4;
	short ind_fare_by_leg4;
	short ind_fare_basis4;
	short ind_tkt_desig4;
	short ind_cog_seat4;
	short ind_service_status4;
	short ind_ftp_miles4;
	short ind_boarding_pass4;
	short ind_special_meal4;
	short ind_special_need4;
	short ind_baggage4;
	short ind_crs_priced4;
	short ind_cur_id4;
	short ind_pi_sqlcode;
} SCIRCreateIrconsumdetailInd;

#endif /* IRCreateIrconsumdetail_h */
/* ========================================================================= */

#ifndef IRCreateIrtaxdetail_h
#define IRCreateIrtaxdetail_h

typedef struct SCIRCreateIrtaxdetail {
	char c_session_id[8+1];
	CS_SMALLINT c_pax_id1;
	CS_SMALLINT c_line_number1;
	char c_tax_id1[2+1];
	CS_FLOAT c_tax_amount1;
	char c_tax_source1[10+1];
	char c_city_id1[3+1];
	CS_SMALLINT c_pax_id2;
	CS_SMALLINT c_line_number2;
	char c_tax_id2[2+1];
	char c_tax_source2[10+1];
	char c_city_id2[3+1];
	CS_FLOAT c_tax_amount2;
	CS_SMALLINT c_pax_id3;
	CS_SMALLINT c_line_number3;
	char c_tax_id3[2+1];
	char c_tax_source3[10+1];
	char c_city_id3[3+1];
	CS_FLOAT c_tax_amount3;
	CS_SMALLINT c_pax_id4;
	CS_SMALLINT c_line_number4;
	char c_tax_id4[2+1];
	char c_tax_source4[10+1];
	char c_city_id4[3+1];
	CS_FLOAT c_tax_amount4;
	CS_SMALLINT c_pax_id5;
	CS_SMALLINT c_line_number5;
	char c_tax_id5[2+1];
	char c_tax_source5[10+1];
	char c_city_id5[3+1];
	CS_FLOAT c_tax_amount5;
	CS_SMALLINT c_pax_id6;
	CS_SMALLINT c_line_number6;
	char c_tax_id6[2+1];
	char c_tax_source6[10+1];
	char c_city_id6[3+1];
	CS_FLOAT c_tax_amount6;
	CS_SMALLINT c_pax_id7;
	CS_SMALLINT c_line_number7;
	char c_tax_id7[2+1];
	char c_tax_source7[10+1];
	char c_city_id7[3+1];
	CS_FLOAT c_tax_amount7;
	CS_SMALLINT c_pax_id8;
	CS_SMALLINT c_line_number8;
	char c_tax_id8[2+1];
	char c_tax_source8[10+1];
	char c_city_id8[3+1];
	CS_FLOAT c_tax_amount8;
	CS_SMALLINT c_pax_id9;
	CS_SMALLINT c_line_number9;
	char c_tax_id9[2+1];
	char c_tax_source9[10+1];
	char c_city_id9[3+1];
	CS_FLOAT c_tax_amount9;
	CS_SMALLINT c_pax_id0;
	CS_SMALLINT c_line_number0;
	char c_tax_id0[2+1];
	char c_tax_source0[10+1];
	char c_city_id0[3+1];
	CS_FLOAT c_tax_amount0;
	CS_INT c_pi_sqlcode;
} SCIRCreateIrtaxdetail;

typedef struct SCIRCreateIrtaxdetailInd {
	short ind_session_id;
	short ind_pax_id1;
	short ind_line_number1;
	short ind_tax_id1;
	short ind_tax_amount1;
	short ind_tax_source1;
	short ind_city_id1;
	short ind_pax_id2;
	short ind_line_number2;
	short ind_tax_id2;
	short ind_tax_source2;
	short ind_city_id2;
	short ind_tax_amount2;
	short ind_pax_id3;
	short ind_line_number3;
	short ind_tax_id3;
	short ind_tax_source3;
	short ind_city_id3;
	short ind_tax_amount3;
	short ind_pax_id4;
	short ind_line_number4;
	short ind_tax_id4;
	short ind_tax_source4;
	short ind_city_id4;
	short ind_tax_amount4;
	short ind_pax_id5;
	short ind_line_number5;
	short ind_tax_id5;
	short ind_tax_source5;
	short ind_city_id5;
	short ind_tax_amount5;
	short ind_pax_id6;
	short ind_line_number6;
	short ind_tax_id6;
	short ind_tax_source6;
	short ind_city_id6;
	short ind_tax_amount6;
	short ind_pax_id7;
	short ind_line_number7;
	short ind_tax_id7;
	short ind_tax_source7;
	short ind_city_id7;
	short ind_tax_amount7;
	short ind_pax_id8;
	short ind_line_number8;
	short ind_tax_id8;
	short ind_tax_source8;
	short ind_city_id8;
	short ind_tax_amount8;
	short ind_pax_id9;
	short ind_line_number9;
	short ind_tax_id9;
	short ind_tax_source9;
	short ind_city_id9;
	short ind_tax_amount9;
	short ind_pax_id0;
	short ind_line_number0;
	short ind_tax_id0;
	short ind_tax_source0;
	short ind_city_id0;
	short ind_tax_amount0;
	short ind_pi_sqlcode;
} SCIRCreateIrtaxdetailInd;

#endif /* IRCreateIrtaxdetail_h */
/* ========================================================================= */

#ifndef IRLoadClientFile_h
#define IRLoadClientFile_h

typedef struct SCIRLoadClientFile {
	char c_pc_session_id[8+1];
	char c_pc_tran_id[6+1];
	char c_pc_con_id[3+1];
	CS_CHAR c_reference[40+1];
	CS_CHAR c_cfa_id[15+1];
	char c_mktg_list[1+1];
	CS_DATETIME c_gmt_date_time;
	char c_pc_PNR_date[7+1];
	char c_iata_no[8+1];
	char c_pc_company[2+1];
	char c_pc_branch[4+1];
	CS_INT c_pi_cfa_no;
	char c_pc_cfa_code[3+1];
	char c_pc_cfa_type[1+1];
	char c_pc_cfa_con[3+1];
	CS_CHAR c_pc_fname[30+1];
	CS_CHAR c_pc_lname[30+1];
	char c_pc_rejectCon[3+1];
	char c_pc_rejectCop[2+1];
	char c_pc_rejectBra[4+1];
	char c_pc_city[3+1];
	CS_INT c_pi_rejectirId;
	CS_CHAR c_pv_rejectirSub[10+1];
	CS_INT c_pi_sqlcode;
} SCIRLoadClientFile;

typedef struct SCIRLoadClientFileInd {
	short ind_pc_session_id;
	short ind_pc_tran_id;
	short ind_pc_con_id;
	short ind_reference;
	short ind_cfa_id;
	short ind_mktg_list;
	short ind_gmt_date_time;
	short ind_pc_PNR_date;
	short ind_iata_no;
	short ind_pc_company;
	short ind_pc_branch;
	short ind_pi_cfa_no;
	short ind_pc_cfa_code;
	short ind_pc_cfa_type;
	short ind_pc_cfa_con;
	short ind_pc_fname;
	short ind_pc_lname;
	short ind_pc_rejectCon;
	short ind_pc_rejectCop;
	short ind_pc_rejectBra;
	short ind_pc_city;
	short ind_pi_rejectirId;
	short ind_pv_rejectirSub;
	short ind_pi_sqlcode;
} SCIRLoadClientFileInd;

#endif /* IRLoadClientFile_h */
/* ========================================================================= */

#ifndef IRReject_h
#define IRReject_h

typedef struct SCIRReject {
	char c_pc_session_id[8+1];
	char c_pc_rejectCop[2+1];
	char c_pc_rejectBra[4+1];
	char c_pc_rejectCon[3+1];
	CS_CHAR c_reference[40+1];
	CS_DATETIME c_gmt_date_time;
	char c_iata_no[8+1];
	CS_CHAR c_description[255+1];
	char c_queue_type[3+1];
	CS_INT c_pi_sqlcode;
} SCIRReject;

typedef struct SCIRRejectInd {
	short ind_pc_session_id;
	short ind_pc_rejectCop;
	short ind_pc_rejectBra;
	short ind_pc_rejectCon;
	short ind_reference;
	short ind_gmt_date_time;
	short ind_iata_no;
	short ind_description;
	short ind_queue_type;
	short ind_pi_sqlcode;
} SCIRRejectInd;

#endif /* IRReject_h */
/* ========================================================================= */

#ifndef IRGetLocalDate_h
#define IRGetLocalDate_h

typedef struct SCIRGetLocalDate {
	char c_pc_copId[2+1];
	char c_pc_braId[4+1];
	char c_pc_iataNo[8+1];
	char c_pc_cityId[3+1];
	CS_DATETIME c_pd_gmtDateTime;
	CS_DATETIME c_pd_lclDateTime;
} SCIRGetLocalDate;

typedef struct SCIRGetLocalDateInd {
	short ind_pc_copId;
	short ind_pc_braId;
	short ind_pc_iataNo;
	short ind_pc_cityId;
	short ind_pd_gmtDateTime;
	short ind_pd_lclDateTime;
} SCIRGetLocalDateInd;

#endif /* IRGetLocalDate_h */
/* ========================================================================= */

#ifndef IRCreateIrother_h
#define IRCreateIrother_h

typedef struct SCIRCreateIrother {
	char c_session_id[8+1];
	char c_itin_type1[1+1];
	CS_SMALLINT c_line_number1;
	char c_start_date1[7+1];
	CS_CHAR c_start_time1[8+1];
	char c_orig_city_id1[4+1];
	char c_end_date1[7+1];
	CS_CHAR c_end_time1[8+1];
	char c_dest_city_id1[4+1];
	char c_unit_type1[4+1];
	CS_INT c_unit_ct1;
	char c_sup_id1[3+1];
	CS_CHAR c_reference1[40+1];
	CS_CHAR c_confirmation1[40+1];
	CS_CHAR c_saving_code1[10+1];
	CS_CHAR c_departure_point1[14+1];
	CS_CHAR c_arrival_point1[14+1];
	CS_CHAR c_svc_description1[40+1];
	CS_CHAR c_cabin_no1[15+1];
	CS_CHAR c_tour_no1[15+1];
	char c_itin_type2[1+1];
	CS_SMALLINT c_line_number2;
	char c_start_date2[7+1];
	CS_CHAR c_start_time2[8+1];
	char c_orig_city_id2[4+1];
	char c_end_date2[7+1];
	CS_CHAR c_end_time2[8+1];
	char c_dest_city_id2[4+1];
	char c_unit_type2[4+1];
	CS_INT c_unit_ct2;
	char c_sup_id2[3+1];
	CS_CHAR c_reference2[40+1];
	CS_CHAR c_confirmation2[40+1];
	CS_CHAR c_saving_code2[10+1];
	CS_CHAR c_departure_point2[14+1];
	CS_CHAR c_arrival_point2[14+1];
	CS_CHAR c_svc_description2[40+1];
	CS_CHAR c_cabin_no2[15+1];
	CS_CHAR c_tour_no2[15+1];
	char c_itin_type3[1+1];
	CS_SMALLINT c_line_number3;
	char c_start_date3[7+1];
	CS_CHAR c_start_time3[8+1];
	char c_orig_city_id3[4+1];
	char c_end_date3[7+1];
	CS_CHAR c_end_time3[8+1];
	char c_dest_city_id3[4+1];
	char c_unit_type3[4+1];
	CS_INT c_unit_ct3;
	char c_sup_id3[3+1];
	CS_CHAR c_reference3[40+1];
	CS_CHAR c_confirmation3[40+1];
	CS_CHAR c_saving_code3[10+1];
	CS_CHAR c_departure_point3[14+1];
	CS_CHAR c_arrival_point3[14+1];
	CS_CHAR c_svc_description3[40+1];
	CS_CHAR c_cabin_no3[15+1];
	CS_CHAR c_tour_no3[15+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrother;

typedef struct SCIRCreateIrotherInd {
	short ind_session_id;
	short ind_itin_type1;
	short ind_line_number1;
	short ind_start_date1;
	short ind_start_time1;
	short ind_orig_city_id1;
	short ind_end_date1;
	short ind_end_time1;
	short ind_dest_city_id1;
	short ind_unit_type1;
	short ind_unit_ct1;
	short ind_sup_id1;
	short ind_reference1;
	short ind_confirmation1;
	short ind_saving_code1;
	short ind_departure_point1;
	short ind_arrival_point1;
	short ind_svc_description1;
	short ind_cabin_no1;
	short ind_tour_no1;
	short ind_itin_type2;
	short ind_line_number2;
	short ind_start_date2;
	short ind_start_time2;
	short ind_orig_city_id2;
	short ind_end_date2;
	short ind_end_time2;
	short ind_dest_city_id2;
	short ind_unit_type2;
	short ind_unit_ct2;
	short ind_sup_id2;
	short ind_reference2;
	short ind_confirmation2;
	short ind_saving_code2;
	short ind_departure_point2;
	short ind_arrival_point2;
	short ind_svc_description2;
	short ind_cabin_no2;
	short ind_tour_no2;
	short ind_itin_type3;
	short ind_line_number3;
	short ind_start_date3;
	short ind_start_time3;
	short ind_orig_city_id3;
	short ind_end_date3;
	short ind_end_time3;
	short ind_dest_city_id3;
	short ind_unit_type3;
	short ind_unit_ct3;
	short ind_sup_id3;
	short ind_reference3;
	short ind_confirmation3;
	short ind_saving_code3;
	short ind_departure_point3;
	short ind_arrival_point3;
	short ind_svc_description3;
	short ind_cabin_no3;
	short ind_tour_no3;
	short ind_pi_sqlcode;
} SCIRCreateIrotherInd;

#endif /* IRCreateIrother_h */
/* ========================================================================= */

#ifndef IRCleanupIrtransport_h
#define IRCleanupIrtransport_h

typedef struct SCIRCleanupIrtransport {
	char c_session_id[8+1];
} SCIRCleanupIrtransport;

typedef struct SCIRCleanupIrtransportInd {
	short ind_session_id;
} SCIRCleanupIrtransportInd;

#endif /* IRCleanupIrtransport_h */
/* ========================================================================= */

#ifndef IRCreateIrrental_h
#define IRCreateIrrental_h

typedef struct SCIRCreateIrrental {
	char c_session_id[8+1];
	char c_cop_id[2+1];
	char c_itin_type[1+1];
	CS_SMALLINT c_line_number;
	char c_start_date[7+1];
	CS_CHAR c_start_time[8+1];
	char c_origin_city_id[4+1];
	char c_end_date[7+1];
	CS_CHAR c_end_time[8+1];
	char c_dest_city_id[4+1];
	char c_unit_type[4+1];
	CS_CHAR c_unit_ct[15+1];
	char c_sup_id[3+1];
	char c_source[1+1];
	char c_action_code[2+1];
	CS_CHAR c_reason_code[10+1];
	char c_crs_id[3+1];
	CS_INT c_number_of_pax;
	CS_CHAR c_ex_day_charge[15+1];
	CS_CHAR c_ex_hr_charge[15+1];
	CS_CHAR c_ex_mile_charge[15+1];
	char c_free_mile_code[1+1];
	CS_CHAR c_free_mileage[10+1];
	CS_CHAR c_dropoff_charge[15+1];
	CS_CHAR c_regular_rate[15+1];
	CS_CHAR c_booked_rate[15+1];
	CS_CHAR c_local_rate[15+1];
	char c_cur_id[3+1];
	char c_rate_type[3+1];
	CS_CHAR c_rate_code[10+1];
	char c_rate_duration[1+1];
	CS_CHAR c_rate_multip[15+1];
	CS_CHAR c_comm_rate[15+1];
	char c_multi_lvl_rate[1+1];
	CS_CHAR c_saving_code[10+1];
	CS_CHAR c_location[40+1];
	char c_location_id[6+1];
	CS_CHAR c_site_name[80+1];
	CS_CHAR c_addr1[80+1];
	CS_CHAR c_addr2[80+1];
	CS_CHAR c_addr3[80+1];
	CS_CHAR c_addr4[80+1];
	CS_CHAR c_phone_acode[10+1];
	CS_CHAR c_phone_no[10+1];
	CS_CHAR c_fax_acode[10+1];
	CS_CHAR c_fax_no[10+1];
	CS_CHAR c_guarantee[40+1];
	CS_CHAR c_confirmation[40+1];
	CS_CHAR c_corporate_disc[40+1];
	CS_CHAR c_corporation_id[40+1];
	CS_CHAR c_cancel_policy[40+1];
	CS_CHAR c_reference[40+1];
	CS_CHAR c_fup[40+1];
	CS_CHAR c_customer_id[40+1];
	CS_INT c_harp_htlkey;
	char c_man_creation[1+1];
	CS_INT c_pi_sqlcode;
	CS_CHAR c_pickup_location[10+1];
	CS_CHAR c_dropoff_location[10+1];
	char c_discount_code[1+1];
	char c_commissionable[1+1];
	char c_preferred[1+1];
	CS_CHAR c_service_status[2+1];
	CS_CHAR c_virtualCard_cc[2+1];
	CS_CHAR c_virtualCard_exp_date[4+1];
	CS_CHAR c_virtualCard_deployment[40+1];
	CS_CHAR c_virtualCard_card_usage_id[20+1];
	CS_CHAR c_virtualCard_gross_value_ind[1+1];
	CS_CHAR c_pcc_cur_id[1+1];
} SCIRCreateIrrental;

typedef struct SCIRCreateIrrentalInd {
	short ind_session_id;
	short ind_cop_id;
	short ind_itin_type;
	short ind_line_number;
	short ind_start_date;
	short ind_start_time;
	short ind_origin_city_id;
	short ind_end_date;
	short ind_end_time;
	short ind_dest_city_id;
	short ind_unit_type;
	short ind_unit_ct;
	short ind_sup_id;
	short ind_source;
	short ind_action_code;
	short ind_reason_code;
	short ind_crs_id;
	short ind_number_of_pax;
	short ind_ex_day_charge;
	short ind_ex_hr_charge;
	short ind_ex_mile_charge;
	short ind_free_mile_code;
	short ind_free_mileage;
	short ind_dropoff_charge;
	short ind_regular_rate;
	short ind_booked_rate;
	short ind_local_rate;
	short ind_cur_id;
	short ind_rate_type;
	short ind_rate_code;
	short ind_rate_duration;
	short ind_rate_multip;
	short ind_comm_rate;
	short ind_multi_lvl_rate;
	short ind_saving_code;
	short ind_location;
	short ind_location_id;
	short ind_site_name;
	short ind_addr1;
	short ind_addr2;
	short ind_addr3;
	short ind_addr4;
	short ind_phone_acode;
	short ind_phone_no;
	short ind_fax_acode;
	short ind_fax_no;
	short ind_guarantee;
	short ind_confirmation;
	short ind_corporate_disc;
	short ind_corporation_id;
	short ind_cancel_policy;
	short ind_reference;
	short ind_fup;
	short ind_customer_id;
	short ind_harp_htlkey;
	short ind_man_creation;
	short ind_pi_sqlcode;
	short ind_pickup_location;
	short ind_dropoff_location;
	short ind_discount_code;
	short ind_commissionable;
	short ind_preferred;
	short ind_service_status;
	short ind_virtualCard_cc;
	short ind_virtualCard_exp_date;
	short ind_virtualCard_deployment;
	short ind_virtualCard_card_usage_id;
	short ind_virtualCard_gross_value_ind;
	short ind_pcc_cur_id;
} SCIRCreateIrrentalInd;

#endif /* IRCreateIrrental_h */
/* ========================================================================= */

#ifndef IRCreateIrticket_h
#define IRCreateIrticket_h

typedef struct SCIRCreateIrticket {
	char c_session_id[8+1];
	char c_cop_id[2+1];
	char c_action_code[2+1];
	char c_source[1+1];
	CS_CHAR c_reason_code[10+1];
	char c_crs_id[3+1];
	char c_commission_type[1+1];
	char c_net_ticket_ind[1+1];
	char c_net_ticket_type[1+1];
	CS_FLOAT c_nett_fare_to_airline;
	CS_FLOAT c_selling_price_to_shop;
	CS_FLOAT c_selling_price_to_client;
	CS_FLOAT c_tour_oper_commission;
	CS_CHAR c_ticket_centre_reference[30+1];
	CS_CHAR c_grid_reference[30+1];
	CS_CHAR c_shop_reference[30+1];
	CS_FLOAT c_publish_fare_tc_commission;
	CS_FLOAT c_publish_fare_agent_commission;
	CS_SMALLINT c_pax_id1;
	CS_SMALLINT c_line_number1;
	CS_SMALLINT c_cost_line1;
	CS_CHAR c_reference1[40+1];
	char c_sup_id1[3+1];
	char c_cur_id1[3+1];
	CS_CHAR c_form1[6+1];
	CS_CHAR c_ticket_no1[30+1];
	CS_CHAR c_pic1[6+1];
	CS_CHAR c_orig_ticket_no1[30+1];
	CS_FLOAT c_comm_rate1;
	CS_FLOAT c_commission1;
	CS_CHAR c_no_of_books1[15+1];
	char c_exchange_code1[1+1];
	CS_FLOAT c_penalty_ch_fee1;
	CS_FLOAT c_comm_penalty1;
	CS_FLOAT c_base_cost1;
	CS_CHAR c_saving_code1[10+1];
	CS_FLOAT c_full_fare1;
	CS_FLOAT c_low_prac_fare1;
	char c_personal_crs1[1+1];
	char c_stp_home_city1[4+1];
	char c_stp_host_city1[4+1];
	CS_CHAR c_sel_access1[30+1];
	CS_CHAR c_crs_tour1[20+1];
	CS_FLOAT c_rebate1;
	char c_doc_type1[3+1];
	char c_e_ticket1[1+1];
	char c_refund1[1+1];
	CS_SMALLINT c_pax_id2;
	CS_SMALLINT c_line_number2;
	CS_SMALLINT c_cost_line2;
	CS_CHAR c_reference2[40+1];
	char c_sup_id2[3+1];
	char c_cur_id2[3+1];
	CS_CHAR c_form2[6+1];
	CS_CHAR c_ticket_no2[30+1];
	CS_CHAR c_pic2[6+1];
	CS_CHAR c_orig_ticket_no2[30+1];
	CS_FLOAT c_comm_rate2;
	CS_FLOAT c_commission2;
	CS_CHAR c_no_of_books2[15+1];
	char c_exchange_code2[1+1];
	CS_FLOAT c_penalty_ch_fee2;
	CS_FLOAT c_comm_penalty2;
	CS_FLOAT c_base_cost2;
	CS_CHAR c_saving_code2[10+1];
	CS_FLOAT c_full_fare2;
	CS_FLOAT c_low_prac_fare2;
	char c_personal_crs2[1+1];
	char c_stp_home_city2[4+1];
	char c_stp_host_city2[4+1];
	CS_CHAR c_sel_access2[30+1];
	CS_CHAR c_crs_tour2[20+1];
	CS_FLOAT c_rebate2;
	char c_doc_type2[3+1];
	char c_e_ticket2[1+1];
	char c_refund2[1+1];
	CS_SMALLINT c_pax_id3;
	CS_SMALLINT c_line_number3;
	CS_SMALLINT c_cost_line3;
	CS_CHAR c_reference3[40+1];
	char c_sup_id3[3+1];
	char c_cur_id3[3+1];
	CS_CHAR c_form3[6+1];
	CS_CHAR c_ticket_no3[30+1];
	CS_CHAR c_pic3[6+1];
	CS_CHAR c_orig_ticket_no3[30+1];
	CS_FLOAT c_comm_rate3;
	CS_FLOAT c_commission3;
	CS_CHAR c_no_of_books3[15+1];
	char c_exchange_code3[1+1];
	CS_FLOAT c_penalty_ch_fee3;
	CS_FLOAT c_comm_penalty3;
	CS_FLOAT c_base_cost3;
	CS_CHAR c_saving_code3[10+1];
	CS_FLOAT c_full_fare3;
	CS_FLOAT c_low_prac_fare3;
	char c_personal_crs3[1+1];
	char c_stp_home_city3[4+1];
	char c_stp_host_city3[4+1];
	CS_CHAR c_sel_access3[30+1];
	CS_CHAR c_crs_tour3[20+1];
	CS_FLOAT c_rebate3;
	char c_doc_type3[3+1];
	char c_e_ticket3[1+1];
	char c_refund3[1+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrticket;

typedef struct SCIRCreateIrticketInd {
	short ind_session_id;
	short ind_cop_id;
	short ind_action_code;
	short ind_source;
	short ind_reason_code;
	short ind_crs_id;
	short ind_commission_type;
	short ind_net_ticket_ind;
	short ind_net_ticket_type;
	short ind_nett_fare_to_airline;
	short ind_selling_price_to_shop;
	short ind_selling_price_to_client;
	short ind_tour_oper_commission;
	short ind_ticket_centre_reference;
	short ind_grid_reference;
	short ind_shop_reference;
	short ind_publish_fare_tc_commission;
	short ind_publish_fare_agent_commission;
	short ind_pax_id1;
	short ind_line_number1;
	short ind_cost_line1;
	short ind_reference1;
	short ind_sup_id1;
	short ind_cur_id1;
	short ind_form1;
	short ind_ticket_no1;
	short ind_pic1;
	short ind_orig_ticket_no1;
	short ind_comm_rate1;
	short ind_commission1;
	short ind_no_of_books1;
	short ind_exchange_code1;
	short ind_penalty_ch_fee1;
	short ind_comm_penalty1;
	short ind_base_cost1;
	short ind_saving_code1;
	short ind_full_fare1;
	short ind_low_prac_fare1;
	short ind_personal_crs1;
	short ind_stp_home_city1;
	short ind_stp_host_city1;
	short ind_sel_access1;
	short ind_crs_tour1;
	short ind_rebate1;
	short ind_doc_type1;
	short ind_e_ticket1;
	short ind_refund1;
	short ind_pax_id2;
	short ind_line_number2;
	short ind_cost_line2;
	short ind_reference2;
	short ind_sup_id2;
	short ind_cur_id2;
	short ind_form2;
	short ind_ticket_no2;
	short ind_pic2;
	short ind_orig_ticket_no2;
	short ind_comm_rate2;
	short ind_commission2;
	short ind_no_of_books2;
	short ind_exchange_code2;
	short ind_penalty_ch_fee2;
	short ind_comm_penalty2;
	short ind_base_cost2;
	short ind_saving_code2;
	short ind_full_fare2;
	short ind_low_prac_fare2;
	short ind_personal_crs2;
	short ind_stp_home_city2;
	short ind_stp_host_city2;
	short ind_sel_access2;
	short ind_crs_tour2;
	short ind_rebate2;
	short ind_doc_type2;
	short ind_e_ticket2;
	short ind_refund2;
	short ind_pax_id3;
	short ind_line_number3;
	short ind_cost_line3;
	short ind_reference3;
	short ind_sup_id3;
	short ind_cur_id3;
	short ind_form3;
	short ind_ticket_no3;
	short ind_pic3;
	short ind_orig_ticket_no3;
	short ind_comm_rate3;
	short ind_commission3;
	short ind_no_of_books3;
	short ind_exchange_code3;
	short ind_penalty_ch_fee3;
	short ind_comm_penalty3;
	short ind_base_cost3;
	short ind_saving_code3;
	short ind_full_fare3;
	short ind_low_prac_fare3;
	short ind_personal_crs3;
	short ind_stp_home_city3;
	short ind_stp_host_city3;
	short ind_sel_access3;
	short ind_crs_tour3;
	short ind_rebate3;
	short ind_doc_type3;
	short ind_e_ticket3;
	short ind_refund3;
	short ind_pi_sqlcode;
} SCIRCreateIrticketInd;

#endif /* IRCreateIrticket_h */
/* ========================================================================= */

#ifndef IRCreateIrtripheader_h
#define IRCreateIrtripheader_h

typedef struct SCIRCreateIrtripheader {
	char c_session_id[8+1];
	char c_interface_type[3+1];
	CS_DATETIME c_creation_date;
	char c_iata_no[8+1];
	char c_cop_id[2+1];
	char c_bra_id[4+1];
	char c_crs_id[3+1];
	char c_cfa_type[1+1];
	char c_cfa_code[3+1];
	CS_CHAR c_cfa_id[15+1];
	char c_con_id[3+1];
	char c_iata_dest[2+1];
	char c_city_id[4+1];
	char c_pnr_date[7+1];
	CS_CHAR c_pnr_time[8+1];
	char c_start_date[7+1];
	CS_CHAR c_cfa_dpt[40+1];
	char c_mktg_list[1+1];
	CS_CHAR c_reference[40+1];
	CS_CHAR c_booking_no[40+1];
	CS_INT c_ticketer_user_id;
	CS_CHAR c_employee_number[10+1];
	CS_CHAR c_booking_pcc[10+1];
	CS_CHAR c_booking_gds_agent[10+1];
	CS_CHAR c_ticketing_pcc[10+1];
	CS_CHAR c_ticketing_gds_agent[10+1];
	char c_record_creation_date[7+1];
	CS_CHAR c_record_creation_time[8+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrtripheader;

typedef struct SCIRCreateIrtripheaderInd {
	short ind_session_id;
	short ind_interface_type;
	short ind_creation_date;
	short ind_iata_no;
	short ind_cop_id;
	short ind_bra_id;
	short ind_crs_id;
	short ind_cfa_type;
	short ind_cfa_code;
	short ind_cfa_id;
	short ind_con_id;
	short ind_iata_dest;
	short ind_city_id;
	short ind_pnr_date;
	short ind_pnr_time;
	short ind_start_date;
	short ind_cfa_dpt;
	short ind_mktg_list;
	short ind_reference;
	short ind_booking_no;
	short ind_ticketer_user_id;
	short ind_employee_number;
	short ind_booking_pcc;
	short ind_booking_gds_agent;
	short ind_ticketing_pcc;
	short ind_ticketing_gds_agent;
	short ind_record_creation_date;
	short ind_record_creation_time;
	short ind_pi_sqlcode;
} SCIRCreateIrtripheaderInd;

#endif /* IRCreateIrtripheader_h */
/* ========================================================================= */

#ifndef IRGetSupServiceType_h
#define IRGetSupServiceType_h

typedef struct SCIRGetSupServiceType {
	char c_pc_sup_id[3+1];
	char c_pc_service_type[1+1];
} SCIRGetSupServiceType;

typedef struct SCIRGetSupServiceTypeInd {
	short ind_pc_sup_id;
	short ind_pc_service_type;
} SCIRGetSupServiceTypeInd;

#endif /* IRGetSupServiceType_h */
/* ========================================================================= */

#ifndef IRCreateIrdistribution_h
#define IRCreateIrdistribution_h

typedef struct SCIRCreateIrdistribution {
	char c_session_id[8+1];
	CS_INT c_line_number_1;
	char c_acc_id_1[6+1];
	CS_FLOAT c_amt_1;
	CS_INT c_line_number_2;
	char c_acc_id_2[6+1];
	CS_FLOAT c_amt_2;
	CS_INT c_line_number_3;
	char c_acc_id_3[6+1];
	CS_FLOAT c_amt_3;
	CS_INT c_line_number_4;
	char c_acc_id_4[6+1];
	CS_FLOAT c_amt_4;
	CS_INT c_line_number_5;
	char c_acc_id_5[6+1];
	CS_FLOAT c_amt_5;
	CS_INT c_line_number_6;
	char c_acc_id_6[6+1];
	CS_FLOAT c_amt_6;
	CS_INT c_pi_sqlcode;
} SCIRCreateIrdistribution;

typedef struct SCIRCreateIrdistributionInd {
	short ind_session_id;
	short ind_line_number_1;
	short ind_acc_id_1;
	short ind_amt_1;
	short ind_line_number_2;
	short ind_acc_id_2;
	short ind_amt_2;
	short ind_line_number_3;
	short ind_acc_id_3;
	short ind_amt_3;
	short ind_line_number_4;
	short ind_acc_id_4;
	short ind_amt_4;
	short ind_line_number_5;
	short ind_acc_id_5;
	short ind_amt_5;
	short ind_line_number_6;
	short ind_acc_id_6;
	short ind_amt_6;
	short ind_pi_sqlcode;
} SCIRCreateIrdistributionInd;

#endif /* IRCreateIrdistribution_h */
/* ========================================================================= */

#ifndef IRValidateSetFixed_h
#define IRValidateSetFixed_h

typedef struct SCIRValidateSetFixed {
	char c_pc_session_id[8+1];
	char c_pc_iata[8+1];
	char c_pc_company[2+1];
	char c_pc_branch[4+1];
	char c_pc_consultant[3+1];
	char c_pc_mml_ind[1+1];
	char c_pc_cfa_type[1+1];
	CS_DATETIME c_gmt_date_time;
	CS_CHAR c_pv_process_name[3+1];
	char c_pc_braMktList[1+1];
	char c_pc_tktBranch[4+1];
	char c_pc_city[4+1];
	CS_DATETIME c_lcl_date_time;
	char c_pc_rejectCon[3+1];
	char c_pc_rejectCop[2+1];
	char c_pc_rejectBra[4+1];
	CS_INT c_pi_rejectirId;
	CS_CHAR c_pv_rejectirSub[3+1];
} SCIRValidateSetFixed;

typedef struct SCIRValidateSetFixedInd {
	short ind_pc_session_id;
	short ind_pc_iata;
	short ind_pc_company;
	short ind_pc_branch;
	short ind_pc_consultant;
	short ind_pc_mml_ind;
	short ind_pc_cfa_type;
	short ind_gmt_date_time;
	short ind_pv_process_name;
	short ind_pc_braMktList;
	short ind_pc_tktBranch;
	short ind_pc_city;
	short ind_lcl_date_time;
	short ind_pc_rejectCon;
	short ind_pc_rejectCop;
	short ind_pc_rejectBra;
	short ind_pi_rejectirId;
	short ind_pv_rejectirSub;
} SCIRValidateSetFixedInd;

#endif /* IRValidateSetFixed_h */
/* ========================================================================= */

#ifndef IRCreateIrreceipt_h
#define IRCreateIrreceipt_h

typedef struct SCIRCreateIrreceipt {
	char c_session_id[8+1];
	CS_INT c_line_number;
	char c_cop_id[2+1];
	char c_bra_id[4+1];
	CS_CHAR c_name[60+1];
	CS_FLOAT c_total_amt;
	char c_bank_id[6+1];
	char c_form_of_pay[2+1];
	char c_cc_id[2+1];
	CS_CHAR c_cc_no[30+1];
	char c_cc_exp_date[4+1];
	CS_CHAR c_cc_auth_no[20+1];
	char c_acc_id1[6+1];
	CS_FLOAT c_amt1;
	char c_acc_id2[6+1];
	CS_FLOAT c_amt2;
	char c_acc_id3[6+1];
	CS_FLOAT c_amt3;
	char c_link_to[1+1];
	char c_rec_valid[1+1];
	char c_lang_id[1+1];
   char c_payment_reference[30+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrreceipt;

typedef struct SCIRCreateIrreceiptInd {
	short ind_session_id;
	short ind_line_number;
	short ind_cop_id;
	short ind_bra_id;
	short ind_name;
	short ind_total_amt;
	short ind_bank_id;
	short ind_form_of_pay;
	short ind_cc_id;
	short ind_cc_no;
	short ind_cc_exp_date;
	short ind_cc_auth_no;
	short ind_acc_id1;
	short ind_amt1;
	short ind_acc_id2;
	short ind_amt2;
	short ind_acc_id3;
	short ind_amt3;
	short ind_link_to;
	short ind_rec_valid;
	short ind_lang_id;
   short ind_payment_reference;
	short ind_pi_sqlcode;
} SCIRCreateIrreceiptInd;

#endif /* IRCreateIrreceipt_h */
/* ========================================================================= */

#ifndef IRMoveQueue_h
#define IRMoveQueue_h

typedef struct SCIRMoveQueue {
	char c_pc_session_id[8+1];
	char c_pc_queue_type[3+1];
	char c_pc_rejectCop[2+1];
	char c_pc_rejectBra[4+1];
	char c_pc_rejectCon[3+1];
	CS_INT c_pi_sqlcode;
} SCIRMoveQueue;

typedef struct SCIRMoveQueueInd {
	short ind_pc_session_id;
	short ind_pc_queue_type;
	short ind_pc_rejectCop;
	short ind_pc_rejectBra;
	short ind_pc_rejectCon;
	short ind_pi_sqlcode;
} SCIRMoveQueueInd;

#endif /* IRMoveQueue_h */
/* ========================================================================= */

#ifndef IRCreateIremailaddress_h
#define IRCreateIremailaddress_h

typedef struct SCIRCreateIremailaddress {
	char c_session_id[8+1];
	CS_CHAR c_email_id1[255+1];
	char c_email_type1[1+1];
	CS_CHAR c_email_id2[255+1];
	char c_email_type2[1+1];
	CS_CHAR c_email_id3[255+1];
	char c_email_type3[1+1];
	CS_CHAR c_email_id4[255+1];
	char c_email_type4[1+1];
	CS_CHAR c_email_id5[255+1];
	char c_email_type5[1+1];
	CS_CHAR c_email_id6[255+1];
	char c_email_type6[1+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIremailaddress;

typedef struct SCIRCreateIremailaddressInd {
	short ind_session_id;
	short ind_email_id1;
	short ind_email_type1;
	short ind_email_id2;
	short ind_email_type2;
	short ind_email_id3;
	short ind_email_type3;
	short ind_email_id4;
	short ind_email_type4;
	short ind_email_id5;
	short ind_email_type5;
	short ind_email_id6;
	short ind_email_type6;
	short ind_pi_sqlcode;
} SCIRCreateIremailaddressInd;

#endif /* IRCreateIremailaddress_h */
/* ========================================================================= */

#ifndef IRIsPFSSupplier_h
#define IRIsPFSSupplier_h

typedef struct SCIRIsPFSSupplier {
	char c_cop_id[2+1];
	char c_sup_id[3+1];
	CS_DATETIME c_pd_lcl_date;
	char c_pb_is_pfs[1+1];
} SCIRIsPFSSupplier;

typedef struct SCIRIsPFSSupplierInd {
	short ind_cop_id;
	short ind_sup_id;
	short ind_pd_lcl_date;
	short ind_pb_is_pfs;
} SCIRIsPFSSupplierInd;

#endif /* IRIsPFSSupplier_h */
/* ========================================================================= */

#ifndef IRGetCopIdByIata_h
#define IRGetCopIdByIata_h

typedef struct SCIRGetCopIdByIata {
	char c_pc_iata_no[8+1];
	char c_pc_cop_id[2+1];
} SCIRGetCopIdByIata;

typedef struct SCIRGetCopIdByIataInd {
	short ind_pc_iata_no;
	short ind_pc_cop_id;
} SCIRGetCopIdByIataInd;

#endif /* IRGetCopIdByIata_h */
/* ========================================================================= */

#ifndef IRUpdateIrRows_h
#define IRUpdateIrRows_h

typedef struct SCIRUpdateIrRows {
	char c_pc_session_id[8+1];
} SCIRUpdateIrRows;

typedef struct SCIRUpdateIrRowsInd {
	short ind_pc_session_id;
} SCIRUpdateIrRowsInd;

#endif /* IRUpdateIrRows_h */
/* ========================================================================= */

#ifndef IRCreateIrstopovercity_h
#define IRCreateIrstopovercity_h

typedef struct SCIRCreateIrstopovercity {
	char c_session_id[8+1];
	char c_itin_type1[1+1];
	CS_SMALLINT c_line_number1;
	CS_SMALLINT c_order_number1;
	char c_city_id1[4+1];
	char c_itin_type2[1+1];
	CS_SMALLINT c_line_number2;
	CS_SMALLINT c_order_number2;
	char c_city_id2[4+1];
	char c_itin_type3[1+1];
	CS_SMALLINT c_line_number3;
	CS_SMALLINT c_order_number3;
	char c_city_id3[4+1];
	char c_itin_type4[1+1];
	CS_SMALLINT c_line_number4;
	CS_SMALLINT c_order_number4;
	char c_city_id4[4+1];
	char c_itin_type5[1+1];
	CS_SMALLINT c_line_number5;
	CS_SMALLINT c_order_number5;
	char c_city_id5[4+1];
	char c_itin_type6[1+1];
	CS_SMALLINT c_line_number6;
	CS_SMALLINT c_order_number6;
	char c_city_id6[4+1];
	CS_INT c_pi_sqlcode;
} SCIRCreateIrstopovercity;

typedef struct SCIRCreateIrstopovercityInd {
	short ind_session_id;
	short ind_itin_type1;
	short ind_line_number1;
	short ind_order_number1;
	short ind_city_id1;
	short ind_itin_type2;
	short ind_line_number2;
	short ind_order_number2;
	short ind_city_id2;
	short ind_itin_type3;
	short ind_line_number3;
	short ind_order_number3;
	short ind_city_id3;
	short ind_itin_type4;
	short ind_line_number4;
	short ind_order_number4;
	short ind_city_id4;
	short ind_itin_type5;
	short ind_line_number5;
	short ind_order_number5;
	short ind_city_id5;
	short ind_itin_type6;
	short ind_line_number6;
	short ind_order_number6;
	short ind_city_id6;
	short ind_pi_sqlcode;
} SCIRCreateIrstopovercityInd;

#endif /* IRCreateIrstopovercity_h */
/* ========================================================================= */

#ifndef IRProcessVoidTicket_h
#define IRProcessVoidTicket_h

typedef struct SCIRProcessVoidTicket {
   CS_CHAR c_pc_form[6+1];
   CS_CHAR c_pc_ticket_no[30+1];
   CS_CHAR c_pc_reference[40+1];
	CS_INT  c_pi_sqlcode;
} SCIRProcessVoidTicket;

typedef struct SCIRProcessVoidTicketInd {
   short ind_pc_form;
   short ind_pc_ticket_no;
   short ind_pc_reference;
	short ind_pi_sqlcode;
} SCIRProcessVoidTicketInd;

#endif /* IRProcessVoidTicket_h */


/* ========================================================================= */

#ifndef IRValReasonCode_h
#define IRValReasonCode_h

typedef struct SCIRValReasonCode {
	char c_cop_id[2+1];
	CS_CHAR c_reason_code[10+1];
	char c_rc_valid[1+1];
} SCIRValReasonCode;

typedef struct SCIRValReasonCodeInd {
	short ind_cop_id;
	short ind_reason_code;
	short ind_rc_valid;
} SCIRValReasonCodeInd;

#endif /* IRValReasonCode_h */
