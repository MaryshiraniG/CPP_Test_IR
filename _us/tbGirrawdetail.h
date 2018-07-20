/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.11
      Delta Created: 04/04/15 - 12:19:45
         Last Delta: app/src/common/InterfaceRecord/_us 4/15/04 - 12:19:45 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.tbGirrawdetail.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/

#ifndef GirrawdetailRead_h
#define GirrawdetailRead_h

typedef struct SCGirrawdetailRead {
	char c_pc_session_id[8+1];
	CS_SMALLINT c_pi_subsect_id;
	CS_SMALLINT c_pi_line_number;
	char c_pc_interface_type[3+1];
	CS_DATETIME c_pd_interface_cdate;
	char c_sec1[3+1];
	CS_SMALLINT c_sub1;
	CS_SMALLINT c_lno1;
	CS_CHAR c_det1[255+1];
	char c_sec2[3+1];
	CS_SMALLINT c_sub2;
	CS_SMALLINT c_lno2;
	CS_CHAR c_det2[255+1];
	char c_sec3[3+1];
	CS_SMALLINT c_sub3;
	CS_SMALLINT c_lno3;
	CS_CHAR c_det3[255+1];
	char c_sec4[3+1];
	CS_SMALLINT c_sub4;
	CS_SMALLINT c_lno4;
	CS_CHAR c_det4[255+1];
	char c_sec5[3+1];
	CS_SMALLINT c_sub5;
	CS_SMALLINT c_lno5;
	CS_CHAR c_det5[255+1];
	char c_sec6[3+1];
	CS_SMALLINT c_sub6;
	CS_SMALLINT c_lno6;
	CS_CHAR c_det6[255+1];
	char c_sec7[3+1];
	CS_SMALLINT c_sub7;
	CS_SMALLINT c_lno7;
	CS_CHAR c_det7[255+1];
	char c_sec8[3+1];
	CS_SMALLINT c_sub8;
	CS_SMALLINT c_lno8;
	CS_CHAR c_det8[255+1];
	char c_sec9[3+1];
	CS_SMALLINT c_sub9;
	CS_SMALLINT c_lno9;
	CS_CHAR c_det9[255+1];
	char c_sec10[3+1];
	CS_SMALLINT c_sub10;
	CS_SMALLINT c_lno10;
	CS_CHAR c_det10[255+1];
	char c_sec11[3+1];
	CS_SMALLINT c_sub11;
	CS_SMALLINT c_lno11;
	CS_CHAR c_det11[255+1];
	char c_sec12[3+1];
	CS_SMALLINT c_sub12;
	CS_SMALLINT c_lno12;
	CS_CHAR c_det12[255+1];
	char c_sec13[3+1];
	CS_SMALLINT c_sub13;
	CS_SMALLINT c_lno13;
	CS_CHAR c_det13[255+1];
	char c_sec14[3+1];
	CS_SMALLINT c_sub14;
	CS_SMALLINT c_lno14;
	CS_CHAR c_det14[255+1];
	char c_sec15[3+1];
	CS_SMALLINT c_sub15;
	CS_SMALLINT c_lno15;
	CS_CHAR c_det15[255+1];
	CS_INT c_pi_counter;
	char c_pc_more_data[1+1];
	CS_SMALLINT c_pi_key_subsect_id;
	CS_SMALLINT c_pi_key_line_number;
} SCGirrawdetailRead;

typedef struct SCGirrawdetailReadInd {
	short ind_pc_session_id;
	short ind_pi_subsect_id;
	short ind_pi_line_number;
	short ind_pc_interface_type;
	short ind_pd_interface_cdate;
	short ind_sec1;
	short ind_sub1;
	short ind_lno1;
	short ind_det1;
	short ind_sec2;
	short ind_sub2;
	short ind_lno2;
	short ind_det2;
	short ind_sec3;
	short ind_sub3;
	short ind_lno3;
	short ind_det3;
	short ind_sec4;
	short ind_sub4;
	short ind_lno4;
	short ind_det4;
	short ind_sec5;
	short ind_sub5;
	short ind_lno5;
	short ind_det5;
	short ind_sec6;
	short ind_sub6;
	short ind_lno6;
	short ind_det6;
	short ind_sec7;
	short ind_sub7;
	short ind_lno7;
	short ind_det7;
	short ind_sec8;
	short ind_sub8;
	short ind_lno8;
	short ind_det8;
	short ind_sec9;
	short ind_sub9;
	short ind_lno9;
	short ind_det9;
	short ind_sec10;
	short ind_sub10;
	short ind_lno10;
	short ind_det10;
	short ind_sec11;
	short ind_sub11;
	short ind_lno11;
	short ind_det11;
	short ind_sec12;
	short ind_sub12;
	short ind_lno12;
	short ind_det12;
	short ind_sec13;
	short ind_sub13;
	short ind_lno13;
	short ind_det13;
	short ind_sec14;
	short ind_sub14;
	short ind_lno14;
	short ind_det14;
	short ind_sec15;
	short ind_sub15;
	short ind_lno15;
	short ind_det15;
	short ind_pi_counter;
	short ind_pc_more_data;
	short ind_pi_key_subsect_id;
	short ind_pi_key_line_number;
} SCGirrawdetailReadInd;

#endif /* GirrawdetailRead_h */
