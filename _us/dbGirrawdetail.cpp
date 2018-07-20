/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996

      SID: 1.11
      Delta Created: 04/04/15 - 12:19:44
    Last Delta: app/src/common/InterfaceRecord/_us 4/15/04 - 12:19:44 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.dbGirrawdetail.cpp) 
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us}[4/15/04-12:19:44 1.11] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.dbGirrawdetail.cpp (Jinsong Liu)\n" )
/*
  The contents of this file, its programs, all comments, code and information 
          are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
============================================================ END GLOBAL BANNER*/

#include "gtypes.h"
#include "dbGirrawdetail.h"

extern "C" {
#include "RGtimer.h"
extern int
dbGirrawdetailRead(SCGirrawdetailRead *ps_GirrawdetailRead,
          SCGirrawdetailReadInd *ps_GirrawdetailReadInd);
}

//Constructor
CGirrawdetailRead::CGirrawdetailRead() :
	pc_session_id(8),
	pc_interface_type(3),
	sec1(3),
	det1(255),
	sec2(3),
	det2(255),
	sec3(3),
	det3(255),
	sec4(3),
	det4(255),
	sec5(3),
	det5(255),
	sec6(3),
	det6(255),
	sec7(3),
	det7(255),
	sec8(3),
	det8(255),
	sec9(3),
	det9(255),
	sec10(3),
	det10(255),
	sec11(3),
	det11(255),
	sec12(3),
	det12(255),
	sec13(3),
	det13(255),
	sec14(3),
	det14(255),
	sec15(3),
	det15(255),
	pc_more_data(1)
{
}

void
CDGirrawdetail::read(CGirrawdetailRead &po_GirrawdetailRead_cpp)
{
	SCGirrawdetailRead ls_GirrawdetailRead;
	SCGirrawdetailReadInd ls_GirrawdetailReadInd;

	memset(&ls_GirrawdetailRead, 0, sizeof(SCGirrawdetailRead));
	memset(&ls_GirrawdetailReadInd, -1, sizeof(SCGirrawdetailReadInd));

	read_appToDb(ls_GirrawdetailRead, ls_GirrawdetailReadInd,
			po_GirrawdetailRead_cpp);

	logTimer("B:GirrawdetailRead", NULL);
	if(::dbGirrawdetailRead(&ls_GirrawdetailRead, &ls_GirrawdetailReadInd) != 0) {
		logTimer("A:GirrawdetailRead (FAILED)", NULL);
		return;
	}
	logTimer("A:GirrawdetailRead", NULL);

	read_dbToApp(ls_GirrawdetailRead, ls_GirrawdetailReadInd,
			po_GirrawdetailRead_cpp);
}

// C to C++ assignments:
void
CDGirrawdetail::read_dbToApp(SCGirrawdetailRead &ps_GirrawdetailRead,
		SCGirrawdetailReadInd &ps_GirrawdetailReadInd,
		CGirrawdetailRead &po_GirrawdetailRead_cpp)
{
	int ii;

	if(ps_GirrawdetailReadInd.ind_pc_interface_type
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.pc_interface_type = ps_GirrawdetailRead.c_pc_interface_type;
	}

	if(ps_GirrawdetailReadInd.ind_pd_interface_cdate
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.pd_interface_cdate.fromCSDateTime(ps_GirrawdetailRead.c_pd_interface_cdate);
	}

	if(ps_GirrawdetailReadInd.ind_sec1
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec1 = ps_GirrawdetailRead.c_sec1;
	}

	if(ps_GirrawdetailReadInd.ind_sub1
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub1 = ps_GirrawdetailRead.c_sub1;
	}

	if(ps_GirrawdetailReadInd.ind_lno1
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno1 = ps_GirrawdetailRead.c_lno1;
	}

	if(ps_GirrawdetailReadInd.ind_det1
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det1 = ps_GirrawdetailRead.c_det1;
	}

	if(ps_GirrawdetailReadInd.ind_sec2
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec2 = ps_GirrawdetailRead.c_sec2;
	}

	if(ps_GirrawdetailReadInd.ind_sub2
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub2 = ps_GirrawdetailRead.c_sub2;
	}

	if(ps_GirrawdetailReadInd.ind_lno2
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno2 = ps_GirrawdetailRead.c_lno2;
	}

	if(ps_GirrawdetailReadInd.ind_det2
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det2 = ps_GirrawdetailRead.c_det2;
	}

	if(ps_GirrawdetailReadInd.ind_sec3
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec3 = ps_GirrawdetailRead.c_sec3;
	}

	if(ps_GirrawdetailReadInd.ind_sub3
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub3 = ps_GirrawdetailRead.c_sub3;
	}

	if(ps_GirrawdetailReadInd.ind_lno3
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno3 = ps_GirrawdetailRead.c_lno3;
	}

	if(ps_GirrawdetailReadInd.ind_det3
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det3 = ps_GirrawdetailRead.c_det3;
	}

	if(ps_GirrawdetailReadInd.ind_sec4
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec4 = ps_GirrawdetailRead.c_sec4;
	}

	if(ps_GirrawdetailReadInd.ind_sub4
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub4 = ps_GirrawdetailRead.c_sub4;
	}

	if(ps_GirrawdetailReadInd.ind_lno4
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno4 = ps_GirrawdetailRead.c_lno4;
	}

	if(ps_GirrawdetailReadInd.ind_det4
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det4 = ps_GirrawdetailRead.c_det4;
	}

	if(ps_GirrawdetailReadInd.ind_sec5
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec5 = ps_GirrawdetailRead.c_sec5;
	}

	if(ps_GirrawdetailReadInd.ind_sub5
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub5 = ps_GirrawdetailRead.c_sub5;
	}

	if(ps_GirrawdetailReadInd.ind_lno5
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno5 = ps_GirrawdetailRead.c_lno5;
	}

	if(ps_GirrawdetailReadInd.ind_det5
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det5 = ps_GirrawdetailRead.c_det5;
	}

	if(ps_GirrawdetailReadInd.ind_sec6
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec6 = ps_GirrawdetailRead.c_sec6;
	}

	if(ps_GirrawdetailReadInd.ind_sub6
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub6 = ps_GirrawdetailRead.c_sub6;
	}

	if(ps_GirrawdetailReadInd.ind_lno6
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno6 = ps_GirrawdetailRead.c_lno6;
	}

	if(ps_GirrawdetailReadInd.ind_det6
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det6 = ps_GirrawdetailRead.c_det6;
	}

	if(ps_GirrawdetailReadInd.ind_sec7
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec7 = ps_GirrawdetailRead.c_sec7;
	}

	if(ps_GirrawdetailReadInd.ind_sub7
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub7 = ps_GirrawdetailRead.c_sub7;
	}

	if(ps_GirrawdetailReadInd.ind_lno7
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno7 = ps_GirrawdetailRead.c_lno7;
	}

	if(ps_GirrawdetailReadInd.ind_det7
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det7 = ps_GirrawdetailRead.c_det7;
	}

	if(ps_GirrawdetailReadInd.ind_sec8
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec8 = ps_GirrawdetailRead.c_sec8;
	}

	if(ps_GirrawdetailReadInd.ind_sub8
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub8 = ps_GirrawdetailRead.c_sub8;
	}

	if(ps_GirrawdetailReadInd.ind_lno8
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno8 = ps_GirrawdetailRead.c_lno8;
	}

	if(ps_GirrawdetailReadInd.ind_det8
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det8 = ps_GirrawdetailRead.c_det8;
	}

	if(ps_GirrawdetailReadInd.ind_sec9
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec9 = ps_GirrawdetailRead.c_sec9;
	}

	if(ps_GirrawdetailReadInd.ind_sub9
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub9 = ps_GirrawdetailRead.c_sub9;
	}

	if(ps_GirrawdetailReadInd.ind_lno9
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno9 = ps_GirrawdetailRead.c_lno9;
	}

	if(ps_GirrawdetailReadInd.ind_det9
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det9 = ps_GirrawdetailRead.c_det9;
	}

	if(ps_GirrawdetailReadInd.ind_sec10
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec10 = ps_GirrawdetailRead.c_sec10;
	}

	if(ps_GirrawdetailReadInd.ind_sub10
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub10 = ps_GirrawdetailRead.c_sub10;
	}

	if(ps_GirrawdetailReadInd.ind_lno10
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno10 = ps_GirrawdetailRead.c_lno10;
	}

	if(ps_GirrawdetailReadInd.ind_det10
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det10 = ps_GirrawdetailRead.c_det10;
	}

	if(ps_GirrawdetailReadInd.ind_sec11
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec11 = ps_GirrawdetailRead.c_sec11;
	}

	if(ps_GirrawdetailReadInd.ind_sub11
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub11 = ps_GirrawdetailRead.c_sub11;
	}

	if(ps_GirrawdetailReadInd.ind_lno11
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno11 = ps_GirrawdetailRead.c_lno11;
	}

	if(ps_GirrawdetailReadInd.ind_det11
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det11 = ps_GirrawdetailRead.c_det11;
	}

	if(ps_GirrawdetailReadInd.ind_sec12
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec12 = ps_GirrawdetailRead.c_sec12;
	}

	if(ps_GirrawdetailReadInd.ind_sub12
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub12 = ps_GirrawdetailRead.c_sub12;
	}

	if(ps_GirrawdetailReadInd.ind_lno12
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno12 = ps_GirrawdetailRead.c_lno12;
	}

	if(ps_GirrawdetailReadInd.ind_det12
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det12 = ps_GirrawdetailRead.c_det12;
	}

	if(ps_GirrawdetailReadInd.ind_sec13
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec13 = ps_GirrawdetailRead.c_sec13;
	}

	if(ps_GirrawdetailReadInd.ind_sub13
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub13 = ps_GirrawdetailRead.c_sub13;
	}

	if(ps_GirrawdetailReadInd.ind_lno13
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno13 = ps_GirrawdetailRead.c_lno13;
	}

	if(ps_GirrawdetailReadInd.ind_det13
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det13 = ps_GirrawdetailRead.c_det13;
	}

	if(ps_GirrawdetailReadInd.ind_sec14
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec14 = ps_GirrawdetailRead.c_sec14;
	}

	if(ps_GirrawdetailReadInd.ind_sub14
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub14 = ps_GirrawdetailRead.c_sub14;
	}

	if(ps_GirrawdetailReadInd.ind_lno14
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno14 = ps_GirrawdetailRead.c_lno14;
	}

	if(ps_GirrawdetailReadInd.ind_det14
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det14 = ps_GirrawdetailRead.c_det14;
	}

	if(ps_GirrawdetailReadInd.ind_sec15
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sec15 = ps_GirrawdetailRead.c_sec15;
	}

	if(ps_GirrawdetailReadInd.ind_sub15
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.sub15 = ps_GirrawdetailRead.c_sub15;
	}

	if(ps_GirrawdetailReadInd.ind_lno15
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.lno15 = ps_GirrawdetailRead.c_lno15;
	}

	if(ps_GirrawdetailReadInd.ind_det15
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.det15 = ps_GirrawdetailRead.c_det15;
	}

	if(ps_GirrawdetailReadInd.ind_pi_counter
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.pi_counter = ps_GirrawdetailRead.c_pi_counter;
	}

	if(ps_GirrawdetailReadInd.ind_pc_more_data
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.pc_more_data = ps_GirrawdetailRead.c_pc_more_data;
	}

	if(ps_GirrawdetailReadInd.ind_pi_key_subsect_id
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.pi_key_subsect_id = ps_GirrawdetailRead.c_pi_key_subsect_id;
	}

	if(ps_GirrawdetailReadInd.ind_pi_key_line_number
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_GirrawdetailRead_cpp.pi_key_line_number = ps_GirrawdetailRead.c_pi_key_line_number;
	}

}

// C++ to C assignments:
void
CDGirrawdetail::read_appToDb(SCGirrawdetailRead &ps_GirrawdetailRead,
		SCGirrawdetailReadInd &ps_GirrawdetailReadInd,
		CGirrawdetailRead &po_GirrawdetailRead_cpp)
{
	int ii;

	if(po_GirrawdetailRead_cpp.pc_session_id.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pc_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_pc_session_id, po_GirrawdetailRead_cpp.pc_session_id.to_string());
	}

	if(po_GirrawdetailRead_cpp.pi_subsect_id.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pi_subsect_id = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_pi_subsect_id = po_GirrawdetailRead_cpp.pi_subsect_id.to_long();
	}

	if(po_GirrawdetailRead_cpp.pi_line_number.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pi_line_number = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_pi_line_number = po_GirrawdetailRead_cpp.pi_line_number.to_long();
	}

	if(po_GirrawdetailRead_cpp.pc_interface_type.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pc_interface_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_pc_interface_type, po_GirrawdetailRead_cpp.pc_interface_type.to_string());
	}

	if(po_GirrawdetailRead_cpp.pd_interface_cdate.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pd_interface_cdate = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_pd_interface_cdate = po_GirrawdetailRead_cpp.pd_interface_cdate.toCSDateTime();
	}

	if(po_GirrawdetailRead_cpp.sec1.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec1, po_GirrawdetailRead_cpp.sec1.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub1.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub1 = po_GirrawdetailRead_cpp.sub1.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno1.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno1 = po_GirrawdetailRead_cpp.lno1.to_long();
	}

	if(po_GirrawdetailRead_cpp.det1.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det1, po_GirrawdetailRead_cpp.det1.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec2.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec2, po_GirrawdetailRead_cpp.sec2.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub2.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub2 = po_GirrawdetailRead_cpp.sub2.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno2.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno2 = po_GirrawdetailRead_cpp.lno2.to_long();
	}

	if(po_GirrawdetailRead_cpp.det2.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det2, po_GirrawdetailRead_cpp.det2.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec3.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec3, po_GirrawdetailRead_cpp.sec3.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub3.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub3 = po_GirrawdetailRead_cpp.sub3.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno3.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno3 = po_GirrawdetailRead_cpp.lno3.to_long();
	}

	if(po_GirrawdetailRead_cpp.det3.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det3, po_GirrawdetailRead_cpp.det3.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec4.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec4, po_GirrawdetailRead_cpp.sec4.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub4.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub4 = po_GirrawdetailRead_cpp.sub4.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno4.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno4 = po_GirrawdetailRead_cpp.lno4.to_long();
	}

	if(po_GirrawdetailRead_cpp.det4.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det4, po_GirrawdetailRead_cpp.det4.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec5.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec5, po_GirrawdetailRead_cpp.sec5.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub5.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub5 = po_GirrawdetailRead_cpp.sub5.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno5.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno5 = po_GirrawdetailRead_cpp.lno5.to_long();
	}

	if(po_GirrawdetailRead_cpp.det5.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det5, po_GirrawdetailRead_cpp.det5.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec6.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec6, po_GirrawdetailRead_cpp.sec6.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub6.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub6 = po_GirrawdetailRead_cpp.sub6.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno6.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno6 = po_GirrawdetailRead_cpp.lno6.to_long();
	}

	if(po_GirrawdetailRead_cpp.det6.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det6, po_GirrawdetailRead_cpp.det6.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec7.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec7, po_GirrawdetailRead_cpp.sec7.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub7.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub7 = po_GirrawdetailRead_cpp.sub7.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno7.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno7 = po_GirrawdetailRead_cpp.lno7.to_long();
	}

	if(po_GirrawdetailRead_cpp.det7.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det7, po_GirrawdetailRead_cpp.det7.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec8.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec8, po_GirrawdetailRead_cpp.sec8.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub8.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub8 = po_GirrawdetailRead_cpp.sub8.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno8.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno8 = po_GirrawdetailRead_cpp.lno8.to_long();
	}

	if(po_GirrawdetailRead_cpp.det8.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det8, po_GirrawdetailRead_cpp.det8.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec9.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec9, po_GirrawdetailRead_cpp.sec9.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub9.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub9 = po_GirrawdetailRead_cpp.sub9.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno9.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno9 = po_GirrawdetailRead_cpp.lno9.to_long();
	}

	if(po_GirrawdetailRead_cpp.det9.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det9, po_GirrawdetailRead_cpp.det9.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec10.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec10 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec10, po_GirrawdetailRead_cpp.sec10.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub10.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub10 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub10 = po_GirrawdetailRead_cpp.sub10.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno10.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno10 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno10 = po_GirrawdetailRead_cpp.lno10.to_long();
	}

	if(po_GirrawdetailRead_cpp.det10.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det10 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det10, po_GirrawdetailRead_cpp.det10.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec11.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec11 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec11, po_GirrawdetailRead_cpp.sec11.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub11.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub11 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub11 = po_GirrawdetailRead_cpp.sub11.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno11.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno11 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno11 = po_GirrawdetailRead_cpp.lno11.to_long();
	}

	if(po_GirrawdetailRead_cpp.det11.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det11 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det11, po_GirrawdetailRead_cpp.det11.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec12.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec12 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec12, po_GirrawdetailRead_cpp.sec12.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub12.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub12 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub12 = po_GirrawdetailRead_cpp.sub12.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno12.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno12 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno12 = po_GirrawdetailRead_cpp.lno12.to_long();
	}

	if(po_GirrawdetailRead_cpp.det12.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det12 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det12, po_GirrawdetailRead_cpp.det12.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec13.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec13 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec13, po_GirrawdetailRead_cpp.sec13.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub13.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub13 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub13 = po_GirrawdetailRead_cpp.sub13.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno13.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno13 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno13 = po_GirrawdetailRead_cpp.lno13.to_long();
	}

	if(po_GirrawdetailRead_cpp.det13.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det13 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det13, po_GirrawdetailRead_cpp.det13.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec14.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec14 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec14, po_GirrawdetailRead_cpp.sec14.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub14.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub14 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub14 = po_GirrawdetailRead_cpp.sub14.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno14.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno14 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno14 = po_GirrawdetailRead_cpp.lno14.to_long();
	}

	if(po_GirrawdetailRead_cpp.det14.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det14 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det14, po_GirrawdetailRead_cpp.det14.to_string());
	}

	if(po_GirrawdetailRead_cpp.sec15.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sec15 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_sec15, po_GirrawdetailRead_cpp.sec15.to_string());
	}

	if(po_GirrawdetailRead_cpp.sub15.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_sub15 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_sub15 = po_GirrawdetailRead_cpp.sub15.to_long();
	}

	if(po_GirrawdetailRead_cpp.lno15.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_lno15 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_lno15 = po_GirrawdetailRead_cpp.lno15.to_long();
	}

	if(po_GirrawdetailRead_cpp.det15.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_det15 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_det15, po_GirrawdetailRead_cpp.det15.to_string());
	}

	if(po_GirrawdetailRead_cpp.pi_counter.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pi_counter = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_pi_counter = po_GirrawdetailRead_cpp.pi_counter.to_long();
	}

	if(po_GirrawdetailRead_cpp.pc_more_data.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pc_more_data = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_GirrawdetailRead.c_pc_more_data, po_GirrawdetailRead_cpp.pc_more_data.to_string());
	}

	if(po_GirrawdetailRead_cpp.pi_key_subsect_id.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pi_key_subsect_id = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_pi_key_subsect_id = po_GirrawdetailRead_cpp.pi_key_subsect_id.to_long();
	}

	if(po_GirrawdetailRead_cpp.pi_key_line_number.NotIsNull()) {
		ps_GirrawdetailReadInd.ind_pi_key_line_number = C_SYBASE_INDICATOR_NOT_NULL;
		ps_GirrawdetailRead.c_pi_key_line_number = po_GirrawdetailRead_cpp.pi_key_line_number.to_long();
	}

}
