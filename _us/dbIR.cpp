/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999, 
															 2000, 2001
                SID: 1.99
      Delta Created: 17/10/18 - 15:12:29
         Last Delta: app/src/common/InterfaceRecord/_us 10/18/17 - 15:12:29 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.dbIR.cpp)
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us}[10/18/17-15:12:29 1.99] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.dbIR.cpp (ENTER AUTHOR'S NAME HERE)\n" )

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
16MAY2012   509845  c0572    WW  Process void ticket for CWT US M&G
============================================================================
*/

#include "gtypes.h"
#include "dbIR.h"

extern "C" {
#include "RGtimer.h"
extern int
dbIRCreateIraddress(SCIRCreateIraddress *ps_IRCreateIraddress,
          SCIRCreateIraddressInd *ps_IRCreateIraddressInd);
}

//Constructor
CIRCreateIraddress::CIRCreateIraddress() :
	session_id(8),
	type1(1),
	addr11(80),
	addr21(80),
	addr31(80),
	addr41(80),
	addr51(80),
	addr61(80),
	phone_acode1(5),
	phone_no1(10),
	bphone_acode1(5),
	bphone_no1(10),
	fax_acode1(5),
	fax_no1(10),
	type2(1),
	addr12(80),
	addr22(80),
	addr32(80),
	addr42(80),
	addr52(80),
	addr62(80),
	phone_acode2(5),
	phone_no2(10),
	bphone_acode2(5),
	bphone_no2(10),
	fax_acode2(5),
	fax_no2(10),
	mphone_acode1(5),
	mphone_no1(10),
	mphone_acode2(5),
	mphone_no2(10)
{
}

void
CDIR::createIraddress(CIRCreateIraddress &po_IRCreateIraddress_cpp)
{
	SCIRCreateIraddress ls_IRCreateIraddress;
	SCIRCreateIraddressInd ls_IRCreateIraddressInd;

	memset(&ls_IRCreateIraddress, 0, sizeof(SCIRCreateIraddress));
	memset(&ls_IRCreateIraddressInd, -1, sizeof(SCIRCreateIraddressInd));

	createIraddress_appToDb(ls_IRCreateIraddress, ls_IRCreateIraddressInd,
			po_IRCreateIraddress_cpp);

	logTimer("B:IRCreateIraddress", NULL);
	if(::dbIRCreateIraddress(&ls_IRCreateIraddress, &ls_IRCreateIraddressInd) != 0) {
		logTimer("A:IRCreateIraddress (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIraddress", NULL);

	createIraddress_dbToApp(ls_IRCreateIraddress, ls_IRCreateIraddressInd,
			po_IRCreateIraddress_cpp);
}

// C to C++ assignments:
void
CDIR::createIraddress_dbToApp(SCIRCreateIraddress &ps_IRCreateIraddress,
		SCIRCreateIraddressInd &ps_IRCreateIraddressInd,
		CIRCreateIraddress &po_IRCreateIraddress_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIraddress_appToDb(SCIRCreateIraddress &ps_IRCreateIraddress,
		SCIRCreateIraddressInd &ps_IRCreateIraddressInd,
		CIRCreateIraddress &po_IRCreateIraddress_cpp)
{
	int ii;

	if(po_IRCreateIraddress_cpp.session_id.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_session_id, po_IRCreateIraddress_cpp.session_id.to_string());
	}

	if(po_IRCreateIraddress_cpp.type1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_type1, po_IRCreateIraddress_cpp.type1.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr11.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr11 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr11, po_IRCreateIraddress_cpp.addr11.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr21.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr21 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr21, po_IRCreateIraddress_cpp.addr21.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr31.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr31 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr31, po_IRCreateIraddress_cpp.addr31.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr41.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr41 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr41, po_IRCreateIraddress_cpp.addr41.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr51.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr51 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr51, po_IRCreateIraddress_cpp.addr51.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr61.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr61 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr61, po_IRCreateIraddress_cpp.addr61.to_string());
	}

	if(po_IRCreateIraddress_cpp.phone_acode1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_phone_acode1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_phone_acode1, po_IRCreateIraddress_cpp.phone_acode1.to_string());
	}

	if(po_IRCreateIraddress_cpp.phone_no1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_phone_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_phone_no1, po_IRCreateIraddress_cpp.phone_no1.to_string());
	}

	if(po_IRCreateIraddress_cpp.bphone_acode1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_bphone_acode1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_bphone_acode1, po_IRCreateIraddress_cpp.bphone_acode1.to_string());
	}

	if(po_IRCreateIraddress_cpp.bphone_no1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_bphone_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_bphone_no1, po_IRCreateIraddress_cpp.bphone_no1.to_string());
	}

	if(po_IRCreateIraddress_cpp.fax_acode1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_fax_acode1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_fax_acode1, po_IRCreateIraddress_cpp.fax_acode1.to_string());
	}

	if(po_IRCreateIraddress_cpp.fax_no1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_fax_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_fax_no1, po_IRCreateIraddress_cpp.fax_no1.to_string());
	}

	if(po_IRCreateIraddress_cpp.type2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_type2, po_IRCreateIraddress_cpp.type2.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr12.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr12 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr12, po_IRCreateIraddress_cpp.addr12.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr22.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr22 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr22, po_IRCreateIraddress_cpp.addr22.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr32.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr32 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr32, po_IRCreateIraddress_cpp.addr32.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr42.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr42 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr42, po_IRCreateIraddress_cpp.addr42.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr52.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr52 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr52, po_IRCreateIraddress_cpp.addr52.to_string());
	}

	if(po_IRCreateIraddress_cpp.addr62.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_addr62 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_addr62, po_IRCreateIraddress_cpp.addr62.to_string());
	}

	if(po_IRCreateIraddress_cpp.phone_acode2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_phone_acode2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_phone_acode2, po_IRCreateIraddress_cpp.phone_acode2.to_string());
	}

	if(po_IRCreateIraddress_cpp.phone_no2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_phone_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_phone_no2, po_IRCreateIraddress_cpp.phone_no2.to_string());
	}

	if(po_IRCreateIraddress_cpp.bphone_acode2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_bphone_acode2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_bphone_acode2, po_IRCreateIraddress_cpp.bphone_acode2.to_string());
	}

	if(po_IRCreateIraddress_cpp.bphone_no2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_bphone_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_bphone_no2, po_IRCreateIraddress_cpp.bphone_no2.to_string());
	}

	if(po_IRCreateIraddress_cpp.fax_acode2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_fax_acode2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_fax_acode2, po_IRCreateIraddress_cpp.fax_acode2.to_string());
	}

	if(po_IRCreateIraddress_cpp.fax_no2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_fax_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_fax_no2, po_IRCreateIraddress_cpp.fax_no2.to_string());
	}

	if(po_IRCreateIraddress_cpp.mphone_acode1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_mphone_acode1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_mphone_acode1, po_IRCreateIraddress_cpp.mphone_acode1.to_string());
	}

	if(po_IRCreateIraddress_cpp.mphone_no1.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_mphone_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_mphone_no1, po_IRCreateIraddress_cpp.mphone_no1.to_string());
	}

	if(po_IRCreateIraddress_cpp.mphone_acode2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_mphone_acode2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_mphone_acode2, po_IRCreateIraddress_cpp.mphone_acode2.to_string());
	}

	if(po_IRCreateIraddress_cpp.mphone_no2.NotIsNull()) {
		ps_IRCreateIraddressInd.ind_mphone_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIraddress.c_mphone_no2, po_IRCreateIraddress_cpp.mphone_no2.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrfrequentuser(SCIRCreateIrfrequentuser *ps_IRCreateIrfrequentuser,
          SCIRCreateIrfrequentuserInd *ps_IRCreateIrfrequentuserInd);
}

//Constructor
CIRCreateIrfrequentuser::CIRCreateIrfrequentuser() :
	session_id(8),
	sup_id1(3),
	fup_type1(10),
	fup1(40),
	fup_status1(2),
	srvc_sup_id1(3),
	sup_id2(3),
	fup_type2(10),
	fup2(40),
	fup_status2(2),
	srvc_sup_id2(3),
	sup_id3(3),
	fup_type3(10),
	fup3(40),
	fup_status3(2),
	srvc_sup_id3(3),
	sup_id4(3),
	fup_type4(10),
	fup4(40),
	fup_status4(2),
	srvc_sup_id4(3),
	sup_id5(3),
	fup_type5(10),
	fup5(40),
	fup_status5(2),
	srvc_sup_id5(3)
{
}

void
CDIR::createIrfrequentuser(CIRCreateIrfrequentuser &po_IRCreateIrfrequentuser_cpp)
{
	SCIRCreateIrfrequentuser ls_IRCreateIrfrequentuser;
	SCIRCreateIrfrequentuserInd ls_IRCreateIrfrequentuserInd;

	memset(&ls_IRCreateIrfrequentuser, 0, sizeof(SCIRCreateIrfrequentuser));
	memset(&ls_IRCreateIrfrequentuserInd, -1, sizeof(SCIRCreateIrfrequentuserInd));

	createIrfrequentuser_appToDb(ls_IRCreateIrfrequentuser, ls_IRCreateIrfrequentuserInd,
			po_IRCreateIrfrequentuser_cpp);

	logTimer("B:IRCreateIrfrequentuser", NULL);
	if(::dbIRCreateIrfrequentuser(&ls_IRCreateIrfrequentuser, &ls_IRCreateIrfrequentuserInd) != 0) {
		logTimer("A:IRCreateIrfrequentuser (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrfrequentuser", NULL);

	createIrfrequentuser_dbToApp(ls_IRCreateIrfrequentuser, ls_IRCreateIrfrequentuserInd,
			po_IRCreateIrfrequentuser_cpp);
}

// C to C++ assignments:
void
CDIR::createIrfrequentuser_dbToApp(SCIRCreateIrfrequentuser &ps_IRCreateIrfrequentuser,
		SCIRCreateIrfrequentuserInd &ps_IRCreateIrfrequentuserInd,
		CIRCreateIrfrequentuser &po_IRCreateIrfrequentuser_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrfrequentuser_appToDb(SCIRCreateIrfrequentuser &ps_IRCreateIrfrequentuser,
		SCIRCreateIrfrequentuserInd &ps_IRCreateIrfrequentuserInd,
		CIRCreateIrfrequentuser &po_IRCreateIrfrequentuser_cpp)
{
	int ii;

	if(po_IRCreateIrfrequentuser_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_session_id, po_IRCreateIrfrequentuser_cpp.session_id.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.pax_id1.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrfrequentuser.c_pax_id1 = po_IRCreateIrfrequentuser_cpp.pax_id1.to_long();
	}

	if(po_IRCreateIrfrequentuser_cpp.sup_id1.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_sup_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_sup_id1, po_IRCreateIrfrequentuser_cpp.sup_id1.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_type1.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_type1, po_IRCreateIrfrequentuser_cpp.fup_type1.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup1.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup1, po_IRCreateIrfrequentuser_cpp.fup1.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_status1.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_status1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_status1, po_IRCreateIrfrequentuser_cpp.fup_status1.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.srvc_sup_id1.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_srvc_sup_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_srvc_sup_id1, po_IRCreateIrfrequentuser_cpp.srvc_sup_id1.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.pax_id2.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrfrequentuser.c_pax_id2 = po_IRCreateIrfrequentuser_cpp.pax_id2.to_long();
	}

	if(po_IRCreateIrfrequentuser_cpp.sup_id2.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_sup_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_sup_id2, po_IRCreateIrfrequentuser_cpp.sup_id2.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_type2.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_type2, po_IRCreateIrfrequentuser_cpp.fup_type2.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup2.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup2, po_IRCreateIrfrequentuser_cpp.fup2.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_status2.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_status2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_status2, po_IRCreateIrfrequentuser_cpp.fup_status2.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.srvc_sup_id2.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_srvc_sup_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_srvc_sup_id2, po_IRCreateIrfrequentuser_cpp.srvc_sup_id2.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.pax_id3.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrfrequentuser.c_pax_id3 = po_IRCreateIrfrequentuser_cpp.pax_id3.to_long();
	}

	if(po_IRCreateIrfrequentuser_cpp.sup_id3.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_sup_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_sup_id3, po_IRCreateIrfrequentuser_cpp.sup_id3.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_type3.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_type3, po_IRCreateIrfrequentuser_cpp.fup_type3.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup3.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup3, po_IRCreateIrfrequentuser_cpp.fup3.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_status3.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_status3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_status3, po_IRCreateIrfrequentuser_cpp.fup_status3.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.srvc_sup_id3.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_srvc_sup_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_srvc_sup_id3, po_IRCreateIrfrequentuser_cpp.srvc_sup_id3.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.pax_id4.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_pax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrfrequentuser.c_pax_id4 = po_IRCreateIrfrequentuser_cpp.pax_id4.to_long();
	}

	if(po_IRCreateIrfrequentuser_cpp.sup_id4.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_sup_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_sup_id4, po_IRCreateIrfrequentuser_cpp.sup_id4.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_type4.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_type4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_type4, po_IRCreateIrfrequentuser_cpp.fup_type4.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup4.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup4, po_IRCreateIrfrequentuser_cpp.fup4.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_status4.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_status4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_status4, po_IRCreateIrfrequentuser_cpp.fup_status4.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.srvc_sup_id4.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_srvc_sup_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_srvc_sup_id4, po_IRCreateIrfrequentuser_cpp.srvc_sup_id4.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.pax_id5.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_pax_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrfrequentuser.c_pax_id5 = po_IRCreateIrfrequentuser_cpp.pax_id5.to_long();
	}

	if(po_IRCreateIrfrequentuser_cpp.sup_id5.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_sup_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_sup_id5, po_IRCreateIrfrequentuser_cpp.sup_id5.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_type5.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_type5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_type5, po_IRCreateIrfrequentuser_cpp.fup_type5.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup5.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup5, po_IRCreateIrfrequentuser_cpp.fup5.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.fup_status5.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_fup_status5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_fup_status5, po_IRCreateIrfrequentuser_cpp.fup_status5.to_string());
	}

	if(po_IRCreateIrfrequentuser_cpp.srvc_sup_id5.NotIsNull()) {
		ps_IRCreateIrfrequentuserInd.ind_srvc_sup_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrfrequentuser.c_srvc_sup_id5, po_IRCreateIrfrequentuser_cpp.srvc_sup_id5.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrpayment(SCIRCreateIrpayment *ps_IRCreateIrpayment,
          SCIRCreateIrpaymentInd *ps_IRCreateIrpaymentInd);
}

//Constructor
CIRCreateIrpayment::CIRCreateIrpayment() :
	session_id(8),
	form_of_pay1(2),
	amount1(13, 2),
	cc_id1(2),
	cc_no1(30),
	cc_exp_date1(4),
	auth_no1(20),
	auth_source1(1),
	form_of_pay2(2),
	amount2(13, 2),
	cc_id2(2),
	cc_no2(30),
	cc_exp_date2(4),
	auth_no2(20),
	auth_source2(1),
	form_of_pay3(2),
	amount3(13, 2),
	cc_id3(2),
	cc_no3(30),
	cc_exp_date3(4),
	auth_no3(20),
	auth_source3(1),
	form_of_pay4(2),
	amount4(13, 2),
	cc_id4(2),
	cc_no4(30),
	cc_exp_date4(4),
	auth_no4(20),
	auth_source4(1),
	form_of_pay5(2),
	amount5(13, 2),
	cc_id5(2),
	cc_no5(30),
	cc_exp_date5(4),
	auth_no5(20),
	auth_source5(1)
{
}

void
CDIR::createIrpayment(CIRCreateIrpayment &po_IRCreateIrpayment_cpp)
{
	SCIRCreateIrpayment ls_IRCreateIrpayment;
	SCIRCreateIrpaymentInd ls_IRCreateIrpaymentInd;

	memset(&ls_IRCreateIrpayment, 0, sizeof(SCIRCreateIrpayment));
	memset(&ls_IRCreateIrpaymentInd, -1, sizeof(SCIRCreateIrpaymentInd));

	createIrpayment_appToDb(ls_IRCreateIrpayment, ls_IRCreateIrpaymentInd,
			po_IRCreateIrpayment_cpp);

	logTimer("B:IRCreateIrpayment", NULL);
	if(::dbIRCreateIrpayment(&ls_IRCreateIrpayment, &ls_IRCreateIrpaymentInd) != 0) {
		logTimer("A:IRCreateIrpayment (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrpayment", NULL);

	createIrpayment_dbToApp(ls_IRCreateIrpayment, ls_IRCreateIrpaymentInd,
			po_IRCreateIrpayment_cpp);
}

// C to C++ assignments:
void
CDIR::createIrpayment_dbToApp(SCIRCreateIrpayment &ps_IRCreateIrpayment,
		SCIRCreateIrpaymentInd &ps_IRCreateIrpaymentInd,
		CIRCreateIrpayment &po_IRCreateIrpayment_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrpayment_appToDb(SCIRCreateIrpayment &ps_IRCreateIrpayment,
		SCIRCreateIrpaymentInd &ps_IRCreateIrpaymentInd,
		CIRCreateIrpayment &po_IRCreateIrpayment_cpp)
{
	int ii;

	if(po_IRCreateIrpayment_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_session_id, po_IRCreateIrpayment_cpp.session_id.to_string());
	}

	if(po_IRCreateIrpayment_cpp.pax_id1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pax_id1 = po_IRCreateIrpayment_cpp.pax_id1.to_long();
	}

	if(po_IRCreateIrpayment_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_line_number1 = po_IRCreateIrpayment_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrpayment_cpp.pay_id1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pay_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pay_id1 = po_IRCreateIrpayment_cpp.pay_id1.to_long();
	}

	if(po_IRCreateIrpayment_cpp.form_of_pay1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_form_of_pay1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_form_of_pay1, po_IRCreateIrpayment_cpp.form_of_pay1.to_string());
	}

	if(po_IRCreateIrpayment_cpp.amount1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_amount1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_amount1 = po_IRCreateIrpayment_cpp.amount1.to_double();
	}

	if(po_IRCreateIrpayment_cpp.cc_id1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_id1, po_IRCreateIrpayment_cpp.cc_id1.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_no1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_no1, po_IRCreateIrpayment_cpp.cc_no1.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_exp_date1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_exp_date1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_exp_date1, po_IRCreateIrpayment_cpp.cc_exp_date1.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_no1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_no1, po_IRCreateIrpayment_cpp.auth_no1.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_source1.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_source1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_source1, po_IRCreateIrpayment_cpp.auth_source1.to_string());
	}

	if(po_IRCreateIrpayment_cpp.pax_id2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pax_id2 = po_IRCreateIrpayment_cpp.pax_id2.to_long();
	}

	if(po_IRCreateIrpayment_cpp.line_number2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_line_number2 = po_IRCreateIrpayment_cpp.line_number2.to_long();
	}

	if(po_IRCreateIrpayment_cpp.pay_id2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pay_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pay_id2 = po_IRCreateIrpayment_cpp.pay_id2.to_long();
	}

	if(po_IRCreateIrpayment_cpp.form_of_pay2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_form_of_pay2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_form_of_pay2, po_IRCreateIrpayment_cpp.form_of_pay2.to_string());
	}

	if(po_IRCreateIrpayment_cpp.amount2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_amount2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_amount2 = po_IRCreateIrpayment_cpp.amount2.to_double();
	}

	if(po_IRCreateIrpayment_cpp.cc_id2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_id2, po_IRCreateIrpayment_cpp.cc_id2.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_no2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_no2, po_IRCreateIrpayment_cpp.cc_no2.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_exp_date2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_exp_date2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_exp_date2, po_IRCreateIrpayment_cpp.cc_exp_date2.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_no2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_no2, po_IRCreateIrpayment_cpp.auth_no2.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_source2.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_source2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_source2, po_IRCreateIrpayment_cpp.auth_source2.to_string());
	}

	if(po_IRCreateIrpayment_cpp.pax_id3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pax_id3 = po_IRCreateIrpayment_cpp.pax_id3.to_long();
	}

	if(po_IRCreateIrpayment_cpp.line_number3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_line_number3 = po_IRCreateIrpayment_cpp.line_number3.to_long();
	}

	if(po_IRCreateIrpayment_cpp.pay_id3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pay_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pay_id3 = po_IRCreateIrpayment_cpp.pay_id3.to_long();
	}

	if(po_IRCreateIrpayment_cpp.form_of_pay3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_form_of_pay3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_form_of_pay3, po_IRCreateIrpayment_cpp.form_of_pay3.to_string());
	}

	if(po_IRCreateIrpayment_cpp.amount3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_amount3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_amount3 = po_IRCreateIrpayment_cpp.amount3.to_double();
	}

	if(po_IRCreateIrpayment_cpp.cc_id3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_id3, po_IRCreateIrpayment_cpp.cc_id3.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_no3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_no3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_no3, po_IRCreateIrpayment_cpp.cc_no3.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_exp_date3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_exp_date3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_exp_date3, po_IRCreateIrpayment_cpp.cc_exp_date3.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_no3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_no3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_no3, po_IRCreateIrpayment_cpp.auth_no3.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_source3.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_source3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_source3, po_IRCreateIrpayment_cpp.auth_source3.to_string());
	}

	if(po_IRCreateIrpayment_cpp.pax_id4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pax_id4 = po_IRCreateIrpayment_cpp.pax_id4.to_long();
	}

	if(po_IRCreateIrpayment_cpp.line_number4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_line_number4 = po_IRCreateIrpayment_cpp.line_number4.to_long();
	}

	if(po_IRCreateIrpayment_cpp.pay_id4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pay_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pay_id4 = po_IRCreateIrpayment_cpp.pay_id4.to_long();
	}

	if(po_IRCreateIrpayment_cpp.form_of_pay4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_form_of_pay4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_form_of_pay4, po_IRCreateIrpayment_cpp.form_of_pay4.to_string());
	}

	if(po_IRCreateIrpayment_cpp.amount4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_amount4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_amount4 = po_IRCreateIrpayment_cpp.amount4.to_double();
	}

	if(po_IRCreateIrpayment_cpp.cc_id4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_id4, po_IRCreateIrpayment_cpp.cc_id4.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_no4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_no4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_no4, po_IRCreateIrpayment_cpp.cc_no4.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_exp_date4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_exp_date4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_exp_date4, po_IRCreateIrpayment_cpp.cc_exp_date4.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_no4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_no4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_no4, po_IRCreateIrpayment_cpp.auth_no4.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_source4.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_source4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_source4, po_IRCreateIrpayment_cpp.auth_source4.to_string());
	}

	if(po_IRCreateIrpayment_cpp.pax_id5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pax_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pax_id5 = po_IRCreateIrpayment_cpp.pax_id5.to_long();
	}

	if(po_IRCreateIrpayment_cpp.line_number5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_line_number5 = po_IRCreateIrpayment_cpp.line_number5.to_long();
	}

	if(po_IRCreateIrpayment_cpp.pay_id5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_pay_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_pay_id5 = po_IRCreateIrpayment_cpp.pay_id5.to_long();
	}

	if(po_IRCreateIrpayment_cpp.form_of_pay5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_form_of_pay5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_form_of_pay5, po_IRCreateIrpayment_cpp.form_of_pay5.to_string());
	}

	if(po_IRCreateIrpayment_cpp.amount5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_amount5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrpayment.c_amount5 = po_IRCreateIrpayment_cpp.amount5.to_double();
	}

	if(po_IRCreateIrpayment_cpp.cc_id5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_id5, po_IRCreateIrpayment_cpp.cc_id5.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_no5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_no5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_no5, po_IRCreateIrpayment_cpp.cc_no5.to_string());
	}

	if(po_IRCreateIrpayment_cpp.cc_exp_date5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_cc_exp_date5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_cc_exp_date5, po_IRCreateIrpayment_cpp.cc_exp_date5.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_no5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_no5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_no5, po_IRCreateIrpayment_cpp.auth_no5.to_string());
	}

	if(po_IRCreateIrpayment_cpp.auth_source5.NotIsNull()) {
		ps_IRCreateIrpaymentInd.ind_auth_source5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrpayment.c_auth_source5, po_IRCreateIrpayment_cpp.auth_source5.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrperson(SCIRCreateIrperson *ps_IRCreateIrperson,
          SCIRCreateIrpersonInd *ps_IRCreateIrpersonInd);
}

//Constructor
CIRCreateIrperson::CIRCreateIrperson() :
	session_id(8),
	title1(40),
	last_name1(40),
	first_name1(40),
	sex1(1),
	smoking1(1),
	ctry_id1(2),
	passport_no1(40),
	passport_exp1(7),
	reference1(40),
	crs_invoice1(10),
	title2(40),
	last_name2(40),
	first_name2(40),
	sex2(1),
	smoking2(1),
	ctry_id2(2),
	passport_no2(40),
	passport_exp2(7),
	reference2(40),
	crs_invoice2(10),
	title3(40),
	last_name3(40),
	first_name3(40),
	sex3(1),
	smoking3(1),
	ctry_id3(2),
	passport_no3(40),
	passport_exp3(7),
	reference3(40),
	crs_invoice3(10),
	title4(40),
	last_name4(40),
	first_name4(40),
	sex4(1),
	smoking4(1),
	ctry_id4(2),
	passport_no4(40),
	passport_exp4(7),
	reference4(40),
	crs_invoice4(10)
{
}

void
CDIR::createIrperson(CIRCreateIrperson &po_IRCreateIrperson_cpp)
{
	SCIRCreateIrperson ls_IRCreateIrperson;
	SCIRCreateIrpersonInd ls_IRCreateIrpersonInd;

	memset(&ls_IRCreateIrperson, 0, sizeof(SCIRCreateIrperson));
	memset(&ls_IRCreateIrpersonInd, -1, sizeof(SCIRCreateIrpersonInd));

	createIrperson_appToDb(ls_IRCreateIrperson, ls_IRCreateIrpersonInd,
			po_IRCreateIrperson_cpp);

	logTimer("B:IRCreateIrperson", NULL);
	if(::dbIRCreateIrperson(&ls_IRCreateIrperson, &ls_IRCreateIrpersonInd) != 0) {
		logTimer("A:IRCreateIrperson (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrperson", NULL);

	createIrperson_dbToApp(ls_IRCreateIrperson, ls_IRCreateIrpersonInd,
			po_IRCreateIrperson_cpp);
}

// C to C++ assignments:
void
CDIR::createIrperson_dbToApp(SCIRCreateIrperson &ps_IRCreateIrperson,
		SCIRCreateIrpersonInd &ps_IRCreateIrpersonInd,
		CIRCreateIrperson &po_IRCreateIrperson_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrperson_appToDb(SCIRCreateIrperson &ps_IRCreateIrperson,
		SCIRCreateIrpersonInd &ps_IRCreateIrpersonInd,
		CIRCreateIrperson &po_IRCreateIrperson_cpp)
{
	int ii;

	if(po_IRCreateIrperson_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_session_id, po_IRCreateIrperson_cpp.session_id.to_string());
	}

	if(po_IRCreateIrperson_cpp.pax_id1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_pax_id1 = po_IRCreateIrperson_cpp.pax_id1.to_long();
	}

	if(po_IRCreateIrperson_cpp.title1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_title1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_title1, po_IRCreateIrperson_cpp.title1.to_string());
	}

	if(po_IRCreateIrperson_cpp.last_name1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_last_name1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_last_name1, po_IRCreateIrperson_cpp.last_name1.to_string());
	}

	if(po_IRCreateIrperson_cpp.first_name1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_first_name1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_first_name1, po_IRCreateIrperson_cpp.first_name1.to_string());
	}

	if(po_IRCreateIrperson_cpp.new_pax_id1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_id1 = po_IRCreateIrperson_cpp.new_pax_id1.to_long();
	}

	if(po_IRCreateIrperson_cpp.new_pax_line1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_line1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_line1 = po_IRCreateIrperson_cpp.new_pax_line1.to_long();
	}

	if(po_IRCreateIrperson_cpp.sex1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_sex1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_sex1, po_IRCreateIrperson_cpp.sex1.to_string());
	}

	if(po_IRCreateIrperson_cpp.smoking1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_smoking1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_smoking1, po_IRCreateIrperson_cpp.smoking1.to_string());
	}

	if(po_IRCreateIrperson_cpp.ctry_id1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_ctry_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_ctry_id1, po_IRCreateIrperson_cpp.ctry_id1.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_no1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_no1, po_IRCreateIrperson_cpp.passport_no1.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_exp1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_exp1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_exp1, po_IRCreateIrperson_cpp.passport_exp1.to_string());
	}

	if(po_IRCreateIrperson_cpp.reference1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_reference1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_reference1, po_IRCreateIrperson_cpp.reference1.to_string());
	}

	if(po_IRCreateIrperson_cpp.crs_invoice1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_crs_invoice1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_crs_invoice1, po_IRCreateIrperson_cpp.crs_invoice1.to_string());
	}

	if(po_IRCreateIrperson_cpp.traveller_id1.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_traveller_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_traveller_id1 = po_IRCreateIrperson_cpp.traveller_id1.to_long();
	}

	if(po_IRCreateIrperson_cpp.pax_id2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_pax_id2 = po_IRCreateIrperson_cpp.pax_id2.to_long();
	}

	if(po_IRCreateIrperson_cpp.title2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_title2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_title2, po_IRCreateIrperson_cpp.title2.to_string());
	}

	if(po_IRCreateIrperson_cpp.last_name2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_last_name2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_last_name2, po_IRCreateIrperson_cpp.last_name2.to_string());
	}

	if(po_IRCreateIrperson_cpp.first_name2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_first_name2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_first_name2, po_IRCreateIrperson_cpp.first_name2.to_string());
	}

	if(po_IRCreateIrperson_cpp.new_pax_id2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_id2 = po_IRCreateIrperson_cpp.new_pax_id2.to_long();
	}

	if(po_IRCreateIrperson_cpp.new_pax_line2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_line2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_line2 = po_IRCreateIrperson_cpp.new_pax_line2.to_long();
	}

	if(po_IRCreateIrperson_cpp.sex2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_sex2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_sex2, po_IRCreateIrperson_cpp.sex2.to_string());
	}

	if(po_IRCreateIrperson_cpp.smoking2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_smoking2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_smoking2, po_IRCreateIrperson_cpp.smoking2.to_string());
	}

	if(po_IRCreateIrperson_cpp.ctry_id2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_ctry_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_ctry_id2, po_IRCreateIrperson_cpp.ctry_id2.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_no2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_no2, po_IRCreateIrperson_cpp.passport_no2.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_exp2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_exp2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_exp2, po_IRCreateIrperson_cpp.passport_exp2.to_string());
	}

	if(po_IRCreateIrperson_cpp.reference2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_reference2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_reference2, po_IRCreateIrperson_cpp.reference2.to_string());
	}

	if(po_IRCreateIrperson_cpp.crs_invoice2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_crs_invoice2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_crs_invoice2, po_IRCreateIrperson_cpp.crs_invoice2.to_string());
	}

	if(po_IRCreateIrperson_cpp.traveller_id2.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_traveller_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_traveller_id2 = po_IRCreateIrperson_cpp.traveller_id2.to_long();
	}

	if(po_IRCreateIrperson_cpp.pax_id3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_pax_id3 = po_IRCreateIrperson_cpp.pax_id3.to_long();
	}

	if(po_IRCreateIrperson_cpp.title3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_title3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_title3, po_IRCreateIrperson_cpp.title3.to_string());
	}

	if(po_IRCreateIrperson_cpp.last_name3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_last_name3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_last_name3, po_IRCreateIrperson_cpp.last_name3.to_string());
	}

	if(po_IRCreateIrperson_cpp.first_name3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_first_name3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_first_name3, po_IRCreateIrperson_cpp.first_name3.to_string());
	}

	if(po_IRCreateIrperson_cpp.new_pax_id3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_id3 = po_IRCreateIrperson_cpp.new_pax_id3.to_long();
	}

	if(po_IRCreateIrperson_cpp.new_pax_line3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_line3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_line3 = po_IRCreateIrperson_cpp.new_pax_line3.to_long();
	}

	if(po_IRCreateIrperson_cpp.sex3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_sex3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_sex3, po_IRCreateIrperson_cpp.sex3.to_string());
	}

	if(po_IRCreateIrperson_cpp.smoking3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_smoking3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_smoking3, po_IRCreateIrperson_cpp.smoking3.to_string());
	}

	if(po_IRCreateIrperson_cpp.ctry_id3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_ctry_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_ctry_id3, po_IRCreateIrperson_cpp.ctry_id3.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_no3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_no3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_no3, po_IRCreateIrperson_cpp.passport_no3.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_exp3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_exp3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_exp3, po_IRCreateIrperson_cpp.passport_exp3.to_string());
	}

	if(po_IRCreateIrperson_cpp.reference3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_reference3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_reference3, po_IRCreateIrperson_cpp.reference3.to_string());
	}

	if(po_IRCreateIrperson_cpp.crs_invoice3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_crs_invoice3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_crs_invoice3, po_IRCreateIrperson_cpp.crs_invoice3.to_string());
	}

	if(po_IRCreateIrperson_cpp.traveller_id3.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_traveller_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_traveller_id3 = po_IRCreateIrperson_cpp.traveller_id3.to_long();
	}

	if(po_IRCreateIrperson_cpp.pax_id4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_pax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_pax_id4 = po_IRCreateIrperson_cpp.pax_id4.to_long();
	}

	if(po_IRCreateIrperson_cpp.title4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_title4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_title4, po_IRCreateIrperson_cpp.title4.to_string());
	}

	if(po_IRCreateIrperson_cpp.last_name4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_last_name4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_last_name4, po_IRCreateIrperson_cpp.last_name4.to_string());
	}

	if(po_IRCreateIrperson_cpp.first_name4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_first_name4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_first_name4, po_IRCreateIrperson_cpp.first_name4.to_string());
	}

	if(po_IRCreateIrperson_cpp.new_pax_id4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_id4 = po_IRCreateIrperson_cpp.new_pax_id4.to_long();
	}

	if(po_IRCreateIrperson_cpp.new_pax_line4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_new_pax_line4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_new_pax_line4 = po_IRCreateIrperson_cpp.new_pax_line4.to_long();
	}

	if(po_IRCreateIrperson_cpp.sex4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_sex4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_sex4, po_IRCreateIrperson_cpp.sex4.to_string());
	}

	if(po_IRCreateIrperson_cpp.smoking4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_smoking4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_smoking4, po_IRCreateIrperson_cpp.smoking4.to_string());
	}

	if(po_IRCreateIrperson_cpp.ctry_id4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_ctry_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_ctry_id4, po_IRCreateIrperson_cpp.ctry_id4.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_no4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_no4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_no4, po_IRCreateIrperson_cpp.passport_no4.to_string());
	}

	if(po_IRCreateIrperson_cpp.passport_exp4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_passport_exp4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_passport_exp4, po_IRCreateIrperson_cpp.passport_exp4.to_string());
	}

	if(po_IRCreateIrperson_cpp.reference4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_reference4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_reference4, po_IRCreateIrperson_cpp.reference4.to_string());
	}

	if(po_IRCreateIrperson_cpp.crs_invoice4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_crs_invoice4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrperson.c_crs_invoice4, po_IRCreateIrperson_cpp.crs_invoice4.to_string());
	}

	if(po_IRCreateIrperson_cpp.traveller_id4.NotIsNull()) {
		ps_IRCreateIrpersonInd.ind_traveller_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrperson.c_traveller_id4 = po_IRCreateIrperson_cpp.traveller_id4.to_long();
	}

}

extern "C" {
extern int
dbIRCreateIrremark(SCIRCreateIrremark *ps_IRCreateIrremark,
          SCIRCreateIrremarkInd *ps_IRCreateIrremarkInd);
}

//Constructor
CIRCreateIrremark::CIRCreateIrremark() :
	session_id(8),
	type1(3),
	remark1(255),
	itin_type1(1),
	type2(3),
	remark2(255),
	itin_type2(1),
	type3(3),
	remark3(255),
	itin_type3(1),
	type4(3),
	remark4(255),
	itin_type4(1),
	type5(3),
	remark5(255),
	itin_type5(1),
	type6(3),
	remark6(255),
	itin_type6(1),
	type7(3),
	remark7(255),
	itin_type7(1),
	type8(3),
	remark8(255),
	itin_type8(1),
	type9(3),
	remark9(255),
	itin_type9(1),
	type0(3),
	remark0(255),
	itin_type0(1)
{
}

void
CDIR::createIrremark(CIRCreateIrremark &po_IRCreateIrremark_cpp)
{
	SCIRCreateIrremark ls_IRCreateIrremark;
	SCIRCreateIrremarkInd ls_IRCreateIrremarkInd;

	memset(&ls_IRCreateIrremark, 0, sizeof(SCIRCreateIrremark));
	memset(&ls_IRCreateIrremarkInd, -1, sizeof(SCIRCreateIrremarkInd));

	createIrremark_appToDb(ls_IRCreateIrremark, ls_IRCreateIrremarkInd,
			po_IRCreateIrremark_cpp);

	logTimer("B:IRCreateIrremark", NULL);
	if(::dbIRCreateIrremark(&ls_IRCreateIrremark, &ls_IRCreateIrremarkInd) != 0) {
		logTimer("A:IRCreateIrremark (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrremark", NULL);

	createIrremark_dbToApp(ls_IRCreateIrremark, ls_IRCreateIrremarkInd,
			po_IRCreateIrremark_cpp);
}

// C to C++ assignments:
void
CDIR::createIrremark_dbToApp(SCIRCreateIrremark &ps_IRCreateIrremark,
		SCIRCreateIrremarkInd &ps_IRCreateIrremarkInd,
		CIRCreateIrremark &po_IRCreateIrremark_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrremark_appToDb(SCIRCreateIrremark &ps_IRCreateIrremark,
		SCIRCreateIrremarkInd &ps_IRCreateIrremarkInd,
		CIRCreateIrremark &po_IRCreateIrremark_cpp)
{
	int ii;

	if(po_IRCreateIrremark_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_session_id, po_IRCreateIrremark_cpp.session_id.to_string());
	}

	if(po_IRCreateIrremark_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number1 = po_IRCreateIrremark_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id1 = po_IRCreateIrremark_cpp.pax_id1.to_long();
	}

	if(po_IRCreateIrremark_cpp.type1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type1, po_IRCreateIrremark_cpp.type1.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark1, po_IRCreateIrremark_cpp.remark1.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type1, po_IRCreateIrremark_cpp.itin_type1.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number1 = po_IRCreateIrremark_cpp.tra_line_number1.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number1 = po_IRCreateIrremark_cpp.ren_line_number1.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number1 = po_IRCreateIrremark_cpp.oth_line_number1.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number1.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number1 = po_IRCreateIrremark_cpp.udi_line_number1.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number2 = po_IRCreateIrremark_cpp.line_number2.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id2 = po_IRCreateIrremark_cpp.pax_id2.to_long();
	}

	if(po_IRCreateIrremark_cpp.type2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type2, po_IRCreateIrremark_cpp.type2.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark2, po_IRCreateIrremark_cpp.remark2.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type2, po_IRCreateIrremark_cpp.itin_type2.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number2 = po_IRCreateIrremark_cpp.tra_line_number2.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number2 = po_IRCreateIrremark_cpp.ren_line_number2.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number2 = po_IRCreateIrremark_cpp.oth_line_number2.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number2.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number2 = po_IRCreateIrremark_cpp.udi_line_number2.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number3 = po_IRCreateIrremark_cpp.line_number3.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id3 = po_IRCreateIrremark_cpp.pax_id3.to_long();
	}

	if(po_IRCreateIrremark_cpp.type3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type3, po_IRCreateIrremark_cpp.type3.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark3, po_IRCreateIrremark_cpp.remark3.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type3, po_IRCreateIrremark_cpp.itin_type3.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number3 = po_IRCreateIrremark_cpp.tra_line_number3.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number3 = po_IRCreateIrremark_cpp.ren_line_number3.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number3 = po_IRCreateIrremark_cpp.oth_line_number3.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number3.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number3 = po_IRCreateIrremark_cpp.udi_line_number3.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number4 = po_IRCreateIrremark_cpp.line_number4.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id4 = po_IRCreateIrremark_cpp.pax_id4.to_long();
	}

	if(po_IRCreateIrremark_cpp.type4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type4, po_IRCreateIrremark_cpp.type4.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark4, po_IRCreateIrremark_cpp.remark4.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type4, po_IRCreateIrremark_cpp.itin_type4.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number4 = po_IRCreateIrremark_cpp.tra_line_number4.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number4 = po_IRCreateIrremark_cpp.ren_line_number4.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number4 = po_IRCreateIrremark_cpp.oth_line_number4.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number4.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number4 = po_IRCreateIrremark_cpp.udi_line_number4.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number5 = po_IRCreateIrremark_cpp.line_number5.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id5 = po_IRCreateIrremark_cpp.pax_id5.to_long();
	}

	if(po_IRCreateIrremark_cpp.type5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type5, po_IRCreateIrremark_cpp.type5.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark5, po_IRCreateIrremark_cpp.remark5.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type5, po_IRCreateIrremark_cpp.itin_type5.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number5 = po_IRCreateIrremark_cpp.tra_line_number5.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number5 = po_IRCreateIrremark_cpp.ren_line_number5.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number5 = po_IRCreateIrremark_cpp.oth_line_number5.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number5.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number5 = po_IRCreateIrremark_cpp.udi_line_number5.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number6 = po_IRCreateIrremark_cpp.line_number6.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id6 = po_IRCreateIrremark_cpp.pax_id6.to_long();
	}

	if(po_IRCreateIrremark_cpp.type6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type6, po_IRCreateIrremark_cpp.type6.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark6, po_IRCreateIrremark_cpp.remark6.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type6, po_IRCreateIrremark_cpp.itin_type6.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number6 = po_IRCreateIrremark_cpp.tra_line_number6.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number6 = po_IRCreateIrremark_cpp.ren_line_number6.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number6 = po_IRCreateIrremark_cpp.oth_line_number6.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number6.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number6 = po_IRCreateIrremark_cpp.udi_line_number6.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number7 = po_IRCreateIrremark_cpp.line_number7.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id7 = po_IRCreateIrremark_cpp.pax_id7.to_long();
	}

	if(po_IRCreateIrremark_cpp.type7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type7, po_IRCreateIrremark_cpp.type7.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark7, po_IRCreateIrremark_cpp.remark7.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type7, po_IRCreateIrremark_cpp.itin_type7.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number7 = po_IRCreateIrremark_cpp.tra_line_number7.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number7 = po_IRCreateIrremark_cpp.ren_line_number7.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number7 = po_IRCreateIrremark_cpp.oth_line_number7.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number7.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number7 = po_IRCreateIrremark_cpp.udi_line_number7.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number8 = po_IRCreateIrremark_cpp.line_number8.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id8 = po_IRCreateIrremark_cpp.pax_id8.to_long();
	}

	if(po_IRCreateIrremark_cpp.type8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type8, po_IRCreateIrremark_cpp.type8.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark8, po_IRCreateIrremark_cpp.remark8.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type8, po_IRCreateIrremark_cpp.itin_type8.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number8 = po_IRCreateIrremark_cpp.tra_line_number8.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number8 = po_IRCreateIrremark_cpp.ren_line_number8.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number8 = po_IRCreateIrremark_cpp.oth_line_number8.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number8.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number8 = po_IRCreateIrremark_cpp.udi_line_number8.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number9 = po_IRCreateIrremark_cpp.line_number9.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id9 = po_IRCreateIrremark_cpp.pax_id9.to_long();
	}

	if(po_IRCreateIrremark_cpp.type9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type9, po_IRCreateIrremark_cpp.type9.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark9, po_IRCreateIrremark_cpp.remark9.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type9, po_IRCreateIrremark_cpp.itin_type9.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number9 = po_IRCreateIrremark_cpp.tra_line_number9.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number9 = po_IRCreateIrremark_cpp.ren_line_number9.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number9 = po_IRCreateIrremark_cpp.oth_line_number9.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number9.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number9 = po_IRCreateIrremark_cpp.udi_line_number9.to_long();
	}

	if(po_IRCreateIrremark_cpp.line_number0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_line_number0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_line_number0 = po_IRCreateIrremark_cpp.line_number0.to_long();
	}

	if(po_IRCreateIrremark_cpp.pax_id0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_pax_id0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_pax_id0 = po_IRCreateIrremark_cpp.pax_id0.to_long();
	}

	if(po_IRCreateIrremark_cpp.type0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_type0 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_type0, po_IRCreateIrremark_cpp.type0.to_string());
	}

	if(po_IRCreateIrremark_cpp.remark0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_remark0 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_remark0, po_IRCreateIrremark_cpp.remark0.to_string());
	}

	if(po_IRCreateIrremark_cpp.itin_type0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_itin_type0 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrremark.c_itin_type0, po_IRCreateIrremark_cpp.itin_type0.to_string());
	}

	if(po_IRCreateIrremark_cpp.tra_line_number0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_tra_line_number0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_tra_line_number0 = po_IRCreateIrremark_cpp.tra_line_number0.to_long();
	}

	if(po_IRCreateIrremark_cpp.ren_line_number0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_ren_line_number0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_ren_line_number0 = po_IRCreateIrremark_cpp.ren_line_number0.to_long();
	}

	if(po_IRCreateIrremark_cpp.oth_line_number0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_oth_line_number0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_oth_line_number0 = po_IRCreateIrremark_cpp.oth_line_number0.to_long();
	}

	if(po_IRCreateIrremark_cpp.udi_line_number0.NotIsNull()) {
		ps_IRCreateIrremarkInd.ind_udi_line_number0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrremark.c_udi_line_number0 = po_IRCreateIrremark_cpp.udi_line_number0.to_long();
	}

}

extern "C" {
extern int
dbIRCreateIrtransport(SCIRCreateIrtransport *ps_IRCreateIrtransport,
          SCIRCreateIrtransportInd *ps_IRCreateIrtransportInd);
}

//Constructor
CIRCreateIrtransport::CIRCreateIrtransport() :
	session_id(8),
	itin_type(1),
	start_date(7),
	start_time(8),
	orig_city_id(4),
	orig_cityname(80),
	cog_dep_city(4),
	cog_dep_date(7),
	cog_dep_time(8),
	cog_dep_term(10),
	cog_dep_gate(4),
	cog_of_miles(10),
	end_date(7),
	end_time(8),
	dest_city_id(4),
	dest_cityname(80),
	classofservice(4),
	service(10),
	sup_id(3),
	reference(40),
	chg_of_gauge(10),
	no_of_stops(15),
	dep_terminal(26),
	dep_gate(4),
	report_time(4),
	arr_gate(4),
	arr_terminal(26),
	airline_locator(8),
	carrier_type(10),
	meal(6),
	aff_carrier(80),
	arrival_ind(2),
	elapsed_time(4),
	itin_type1(1),
	start_date1(7),
	start_time1(8),
	orig_city_id1(4),
	orig_cityname1(80),
	cog_dep_city1(4),
	cog_dep_date1(7),
	cog_dep_time1(8),
	cog_dep_term1(10),
	cog_dep_gate1(4),
	cog_of_miles1(10),
	end_date1(7),
	end_time1(8),
	dest_city_id1(4),
	dest_cityname1(80),
	classofservice1(4),
	service1(10),
	sup_id1(3),
	reference1(40),
	chg_of_gauge1(10),
	no_of_stops1(15),
	dep_terminal1(26),
	dep_gate1(4),
	report_time1(4),
	arr_gate1(4),
	arr_terminal1(26),
	airline_locator1(8),
	carrier_type1(10),
	meal1(6),
	aff_carrier1(80),
	arrival_ind1(2),
	elapsed_time1(4)
{
}

void
CDIR::createIrtransport(CIRCreateIrtransport &po_IRCreateIrtransport_cpp)
{
	SCIRCreateIrtransport ls_IRCreateIrtransport;
	SCIRCreateIrtransportInd ls_IRCreateIrtransportInd;

	memset(&ls_IRCreateIrtransport, 0, sizeof(SCIRCreateIrtransport));
	memset(&ls_IRCreateIrtransportInd, -1, sizeof(SCIRCreateIrtransportInd));

	createIrtransport_appToDb(ls_IRCreateIrtransport, ls_IRCreateIrtransportInd,
			po_IRCreateIrtransport_cpp);

	logTimer("B:IRCreateIrtransport", NULL);
	if(::dbIRCreateIrtransport(&ls_IRCreateIrtransport, &ls_IRCreateIrtransportInd) != 0) {
		logTimer("A:IRCreateIrtransport (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrtransport", NULL);

	createIrtransport_dbToApp(ls_IRCreateIrtransport, ls_IRCreateIrtransportInd,
			po_IRCreateIrtransport_cpp);
}

// C to C++ assignments:
void
CDIR::createIrtransport_dbToApp(SCIRCreateIrtransport &ps_IRCreateIrtransport,
		SCIRCreateIrtransportInd &ps_IRCreateIrtransportInd,
		CIRCreateIrtransport &po_IRCreateIrtransport_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrtransport_appToDb(SCIRCreateIrtransport &ps_IRCreateIrtransport,
		SCIRCreateIrtransportInd &ps_IRCreateIrtransportInd,
		CIRCreateIrtransport &po_IRCreateIrtransport_cpp)
{
	int ii;

	if(po_IRCreateIrtransport_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_session_id, po_IRCreateIrtransport_cpp.session_id.to_string());
	}

	if(po_IRCreateIrtransport_cpp.itin_type.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_itin_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_itin_type, po_IRCreateIrtransport_cpp.itin_type.to_string());
	}

	if(po_IRCreateIrtransport_cpp.line_number.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_line_number = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtransport.c_line_number = po_IRCreateIrtransport_cpp.line_number.to_long();
	}

	if(po_IRCreateIrtransport_cpp.start_date.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_start_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_start_date, po_IRCreateIrtransport_cpp.start_date.to_string());
	}

	if(po_IRCreateIrtransport_cpp.start_time.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_start_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_start_time, po_IRCreateIrtransport_cpp.start_time.to_string());
	}

	if(po_IRCreateIrtransport_cpp.orig_city_id.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_orig_city_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_orig_city_id, po_IRCreateIrtransport_cpp.orig_city_id.to_string());
	}

	if(po_IRCreateIrtransport_cpp.orig_cityname.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_orig_cityname = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_orig_cityname, po_IRCreateIrtransport_cpp.orig_cityname.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_city.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_city = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_city, po_IRCreateIrtransport_cpp.cog_dep_city.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_date.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_date, po_IRCreateIrtransport_cpp.cog_dep_date.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_time.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_time, po_IRCreateIrtransport_cpp.cog_dep_time.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_term.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_term = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_term, po_IRCreateIrtransport_cpp.cog_dep_term.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_gate.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_gate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_gate, po_IRCreateIrtransport_cpp.cog_dep_gate.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_of_miles.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_of_miles = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_of_miles, po_IRCreateIrtransport_cpp.cog_of_miles.to_string());
	}

	if(po_IRCreateIrtransport_cpp.end_date.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_end_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_end_date, po_IRCreateIrtransport_cpp.end_date.to_string());
	}

	if(po_IRCreateIrtransport_cpp.end_time.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_end_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_end_time, po_IRCreateIrtransport_cpp.end_time.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dest_city_id.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dest_city_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dest_city_id, po_IRCreateIrtransport_cpp.dest_city_id.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dest_cityname.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dest_cityname = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dest_cityname, po_IRCreateIrtransport_cpp.dest_cityname.to_string());
	}

	if(po_IRCreateIrtransport_cpp.classofservice.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_classofservice = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_classofservice, po_IRCreateIrtransport_cpp.classofservice.to_string());
	}

	if(po_IRCreateIrtransport_cpp.service.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_service = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_service, po_IRCreateIrtransport_cpp.service.to_string());
	}

	if(po_IRCreateIrtransport_cpp.sup_id.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_sup_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_sup_id, po_IRCreateIrtransport_cpp.sup_id.to_string());
	}

	if(po_IRCreateIrtransport_cpp.reference.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_reference, po_IRCreateIrtransport_cpp.reference.to_string());
	}

	if(po_IRCreateIrtransport_cpp.chg_of_gauge.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_chg_of_gauge = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_chg_of_gauge, po_IRCreateIrtransport_cpp.chg_of_gauge.to_string());
	}

	if(po_IRCreateIrtransport_cpp.no_of_stops.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_no_of_stops = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_no_of_stops, po_IRCreateIrtransport_cpp.no_of_stops.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dep_terminal.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dep_terminal = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dep_terminal, po_IRCreateIrtransport_cpp.dep_terminal.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dep_gate.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dep_gate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dep_gate, po_IRCreateIrtransport_cpp.dep_gate.to_string());
	}

	if(po_IRCreateIrtransport_cpp.report_time.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_report_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_report_time, po_IRCreateIrtransport_cpp.report_time.to_string());
	}

	if(po_IRCreateIrtransport_cpp.arr_gate.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_arr_gate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_arr_gate, po_IRCreateIrtransport_cpp.arr_gate.to_string());
	}

	if(po_IRCreateIrtransport_cpp.arr_terminal.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_arr_terminal = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_arr_terminal, po_IRCreateIrtransport_cpp.arr_terminal.to_string());
	}

	if(po_IRCreateIrtransport_cpp.airline_locator.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_airline_locator = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_airline_locator, po_IRCreateIrtransport_cpp.airline_locator.to_string());
	}

	if(po_IRCreateIrtransport_cpp.carrier_type.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_carrier_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_carrier_type, po_IRCreateIrtransport_cpp.carrier_type.to_string());
	}

	if(po_IRCreateIrtransport_cpp.meal.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_meal = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_meal, po_IRCreateIrtransport_cpp.meal.to_string());
	}

	if(po_IRCreateIrtransport_cpp.aff_carrier.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_aff_carrier = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_aff_carrier, po_IRCreateIrtransport_cpp.aff_carrier.to_string());
	}

	if(po_IRCreateIrtransport_cpp.actual_miles.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_actual_miles = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtransport.c_actual_miles = po_IRCreateIrtransport_cpp.actual_miles.to_long();
	}

	if(po_IRCreateIrtransport_cpp.arrival_ind.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_arrival_ind = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_arrival_ind, po_IRCreateIrtransport_cpp.arrival_ind.to_string());
	}

	if(po_IRCreateIrtransport_cpp.elapsed_time.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_elapsed_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_elapsed_time, po_IRCreateIrtransport_cpp.elapsed_time.to_string());
	}

	if(po_IRCreateIrtransport_cpp.itin_type1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_itin_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_itin_type1, po_IRCreateIrtransport_cpp.itin_type1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtransport.c_line_number1 = po_IRCreateIrtransport_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrtransport_cpp.start_date1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_start_date1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_start_date1, po_IRCreateIrtransport_cpp.start_date1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.start_time1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_start_time1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_start_time1, po_IRCreateIrtransport_cpp.start_time1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.orig_city_id1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_orig_city_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_orig_city_id1, po_IRCreateIrtransport_cpp.orig_city_id1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.orig_cityname1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_orig_cityname1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_orig_cityname1, po_IRCreateIrtransport_cpp.orig_cityname1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_city1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_city1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_city1, po_IRCreateIrtransport_cpp.cog_dep_city1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_date1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_date1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_date1, po_IRCreateIrtransport_cpp.cog_dep_date1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_time1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_time1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_time1, po_IRCreateIrtransport_cpp.cog_dep_time1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_term1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_term1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_term1, po_IRCreateIrtransport_cpp.cog_dep_term1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_dep_gate1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_dep_gate1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_dep_gate1, po_IRCreateIrtransport_cpp.cog_dep_gate1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.cog_of_miles1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_cog_of_miles1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_cog_of_miles1, po_IRCreateIrtransport_cpp.cog_of_miles1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.end_date1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_end_date1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_end_date1, po_IRCreateIrtransport_cpp.end_date1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.end_time1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_end_time1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_end_time1, po_IRCreateIrtransport_cpp.end_time1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dest_city_id1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dest_city_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dest_city_id1, po_IRCreateIrtransport_cpp.dest_city_id1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dest_cityname1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dest_cityname1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dest_cityname1, po_IRCreateIrtransport_cpp.dest_cityname1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.classofservice1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_classofservice1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_classofservice1, po_IRCreateIrtransport_cpp.classofservice1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.service1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_service1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_service1, po_IRCreateIrtransport_cpp.service1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.sup_id1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_sup_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_sup_id1, po_IRCreateIrtransport_cpp.sup_id1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.reference1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_reference1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_reference1, po_IRCreateIrtransport_cpp.reference1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.chg_of_gauge1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_chg_of_gauge1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_chg_of_gauge1, po_IRCreateIrtransport_cpp.chg_of_gauge1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.no_of_stops1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_no_of_stops1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_no_of_stops1, po_IRCreateIrtransport_cpp.no_of_stops1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dep_terminal1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dep_terminal1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dep_terminal1, po_IRCreateIrtransport_cpp.dep_terminal1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.dep_gate1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_dep_gate1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_dep_gate1, po_IRCreateIrtransport_cpp.dep_gate1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.report_time1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_report_time1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_report_time1, po_IRCreateIrtransport_cpp.report_time1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.arr_gate1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_arr_gate1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_arr_gate1, po_IRCreateIrtransport_cpp.arr_gate1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.arr_terminal1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_arr_terminal1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_arr_terminal1, po_IRCreateIrtransport_cpp.arr_terminal1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.airline_locator1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_airline_locator1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_airline_locator1, po_IRCreateIrtransport_cpp.airline_locator1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.carrier_type1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_carrier_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_carrier_type1, po_IRCreateIrtransport_cpp.carrier_type1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.meal1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_meal1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_meal1, po_IRCreateIrtransport_cpp.meal1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.aff_carrier1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_aff_carrier1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_aff_carrier1, po_IRCreateIrtransport_cpp.aff_carrier1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.actual_miles1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_actual_miles1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtransport.c_actual_miles1 = po_IRCreateIrtransport_cpp.actual_miles1.to_long();
	}

	if(po_IRCreateIrtransport_cpp.arrival_ind1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_arrival_ind1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_arrival_ind1, po_IRCreateIrtransport_cpp.arrival_ind1.to_string());
	}

	if(po_IRCreateIrtransport_cpp.elapsed_time1.NotIsNull()) {
		ps_IRCreateIrtransportInd.ind_elapsed_time1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtransport.c_elapsed_time1, po_IRCreateIrtransport_cpp.elapsed_time1.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrconsumdetail(SCIRCreateIrconsumdetail *ps_IRCreateIrconsumdetail,
          SCIRCreateIrconsumdetailInd *ps_IRCreateIrconsumdetailInd);
}

//Constructor
CIRCreateIrconsumdetail::CIRCreateIrconsumdetail() :
	session_id(8),
	itin_type1(1),
	seat1(4),
	connection1(1),
	smoking1(1),
	fare_by_leg1(13, 2),
	fare_basis1(20),
	tkt_desig1(20),
	cog_seat1(1),
	service_status1(2),
	boarding_pass1(1),
	special_meal1(6),
	special_need1(80),
	baggage1(3),
	crs_priced1(1),
	cur_id1(3),
	itin_type2(1),
	seat2(4),
	connection2(1),
	smoking2(1),
	fare_by_leg2(13, 2),
	fare_basis2(20),
	tkt_desig2(20),
	cog_seat2(1),
	service_status2(2),
	boarding_pass2(1),
	special_meal2(6),
	special_need2(80),
	baggage2(3),
	crs_priced2(1),
	cur_id2(3),
	itin_type3(1),
	seat3(4),
	connection3(1),
	smoking3(1),
	fare_by_leg3(13, 2),
	fare_basis3(20),
	tkt_desig3(20),
	cog_seat3(1),
	service_status3(2),
	boarding_pass3(1),
	special_meal3(6),
	special_need3(80),
	baggage3(3),
	crs_priced3(1),
	cur_id3(3),
	itin_type4(1),
	seat4(4),
	connection4(1),
	smoking4(1),
	fare_by_leg4(13, 2),
	fare_basis4(20),
	tkt_desig4(20),
	cog_seat4(1),
	service_status4(2),
	boarding_pass4(1),
	special_meal4(6),
	special_need4(80),
	baggage4(3),
	crs_priced4(1),
	cur_id4(3)
{
}

void
CDIR::createIrconsumdetail(CIRCreateIrconsumdetail &po_IRCreateIrconsumdetail_cpp)
{
	SCIRCreateIrconsumdetail ls_IRCreateIrconsumdetail;
	SCIRCreateIrconsumdetailInd ls_IRCreateIrconsumdetailInd;

	memset(&ls_IRCreateIrconsumdetail, 0, sizeof(SCIRCreateIrconsumdetail));
	memset(&ls_IRCreateIrconsumdetailInd, -1, sizeof(SCIRCreateIrconsumdetailInd));

	createIrconsumdetail_appToDb(ls_IRCreateIrconsumdetail, ls_IRCreateIrconsumdetailInd,
			po_IRCreateIrconsumdetail_cpp);

	logTimer("B:IRCreateIrconsumdetail", NULL);
	if(::dbIRCreateIrconsumdetail(&ls_IRCreateIrconsumdetail, &ls_IRCreateIrconsumdetailInd) != 0) {
		logTimer("A:IRCreateIrconsumdetail (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrconsumdetail", NULL);

	createIrconsumdetail_dbToApp(ls_IRCreateIrconsumdetail, ls_IRCreateIrconsumdetailInd,
			po_IRCreateIrconsumdetail_cpp);
}

// C to C++ assignments:
void
CDIR::createIrconsumdetail_dbToApp(SCIRCreateIrconsumdetail &ps_IRCreateIrconsumdetail,
		SCIRCreateIrconsumdetailInd &ps_IRCreateIrconsumdetailInd,
		CIRCreateIrconsumdetail &po_IRCreateIrconsumdetail_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrconsumdetail_appToDb(SCIRCreateIrconsumdetail &ps_IRCreateIrconsumdetail,
		SCIRCreateIrconsumdetailInd &ps_IRCreateIrconsumdetailInd,
		CIRCreateIrconsumdetail &po_IRCreateIrconsumdetail_cpp)
{
	int ii;

	if(po_IRCreateIrconsumdetail_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_session_id, po_IRCreateIrconsumdetail_cpp.session_id.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.pax_id1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_pax_id1 = po_IRCreateIrconsumdetail_cpp.pax_id1.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_type1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_itin_type1, po_IRCreateIrconsumdetail_cpp.itin_type1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_line_number1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_itin_line_number1 = po_IRCreateIrconsumdetail_cpp.itin_line_number1.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_line_number1 = po_IRCreateIrconsumdetail_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.seat1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_seat1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_seat1, po_IRCreateIrconsumdetail_cpp.seat1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.connection1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_connection1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_connection1, po_IRCreateIrconsumdetail_cpp.connection1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.smoking1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_smoking1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_smoking1, po_IRCreateIrconsumdetail_cpp.smoking1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_by_leg1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_by_leg1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_fare_by_leg1 = po_IRCreateIrconsumdetail_cpp.fare_by_leg1.to_double();
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_basis1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_basis1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_fare_basis1, po_IRCreateIrconsumdetail_cpp.fare_basis1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.tkt_desig1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_tkt_desig1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_tkt_desig1, po_IRCreateIrconsumdetail_cpp.tkt_desig1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cog_seat1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cog_seat1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cog_seat1, po_IRCreateIrconsumdetail_cpp.cog_seat1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.service_status1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_service_status1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_service_status1, po_IRCreateIrconsumdetail_cpp.service_status1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.ftp_miles1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_ftp_miles1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_ftp_miles1 = po_IRCreateIrconsumdetail_cpp.ftp_miles1.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.boarding_pass1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_boarding_pass1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_boarding_pass1, po_IRCreateIrconsumdetail_cpp.boarding_pass1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_meal1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_meal1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_meal1, po_IRCreateIrconsumdetail_cpp.special_meal1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_need1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_need1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_need1, po_IRCreateIrconsumdetail_cpp.special_need1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.baggage1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_baggage1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_baggage1, po_IRCreateIrconsumdetail_cpp.baggage1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.crs_priced1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_crs_priced1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_crs_priced1, po_IRCreateIrconsumdetail_cpp.crs_priced1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cur_id1.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cur_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cur_id1, po_IRCreateIrconsumdetail_cpp.cur_id1.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.pax_id2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_pax_id2 = po_IRCreateIrconsumdetail_cpp.pax_id2.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_type2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_itin_type2, po_IRCreateIrconsumdetail_cpp.itin_type2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_line_number2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_itin_line_number2 = po_IRCreateIrconsumdetail_cpp.itin_line_number2.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.line_number2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_line_number2 = po_IRCreateIrconsumdetail_cpp.line_number2.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.seat2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_seat2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_seat2, po_IRCreateIrconsumdetail_cpp.seat2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.connection2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_connection2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_connection2, po_IRCreateIrconsumdetail_cpp.connection2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.smoking2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_smoking2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_smoking2, po_IRCreateIrconsumdetail_cpp.smoking2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_by_leg2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_by_leg2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_fare_by_leg2 = po_IRCreateIrconsumdetail_cpp.fare_by_leg2.to_double();
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_basis2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_basis2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_fare_basis2, po_IRCreateIrconsumdetail_cpp.fare_basis2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.tkt_desig2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_tkt_desig2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_tkt_desig2, po_IRCreateIrconsumdetail_cpp.tkt_desig2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cog_seat2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cog_seat2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cog_seat2, po_IRCreateIrconsumdetail_cpp.cog_seat2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.service_status2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_service_status2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_service_status2, po_IRCreateIrconsumdetail_cpp.service_status2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.ftp_miles2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_ftp_miles2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_ftp_miles2 = po_IRCreateIrconsumdetail_cpp.ftp_miles2.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.boarding_pass2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_boarding_pass2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_boarding_pass2, po_IRCreateIrconsumdetail_cpp.boarding_pass2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_meal2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_meal2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_meal2, po_IRCreateIrconsumdetail_cpp.special_meal2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_need2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_need2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_need2, po_IRCreateIrconsumdetail_cpp.special_need2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.baggage2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_baggage2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_baggage2, po_IRCreateIrconsumdetail_cpp.baggage2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.crs_priced2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_crs_priced2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_crs_priced2, po_IRCreateIrconsumdetail_cpp.crs_priced2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cur_id2.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cur_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cur_id2, po_IRCreateIrconsumdetail_cpp.cur_id2.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.pax_id3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_pax_id3 = po_IRCreateIrconsumdetail_cpp.pax_id3.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_type3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_itin_type3, po_IRCreateIrconsumdetail_cpp.itin_type3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_line_number3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_itin_line_number3 = po_IRCreateIrconsumdetail_cpp.itin_line_number3.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.line_number3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_line_number3 = po_IRCreateIrconsumdetail_cpp.line_number3.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.seat3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_seat3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_seat3, po_IRCreateIrconsumdetail_cpp.seat3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.connection3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_connection3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_connection3, po_IRCreateIrconsumdetail_cpp.connection3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.smoking3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_smoking3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_smoking3, po_IRCreateIrconsumdetail_cpp.smoking3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_by_leg3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_by_leg3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_fare_by_leg3 = po_IRCreateIrconsumdetail_cpp.fare_by_leg3.to_double();
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_basis3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_basis3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_fare_basis3, po_IRCreateIrconsumdetail_cpp.fare_basis3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.tkt_desig3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_tkt_desig3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_tkt_desig3, po_IRCreateIrconsumdetail_cpp.tkt_desig3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cog_seat3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cog_seat3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cog_seat3, po_IRCreateIrconsumdetail_cpp.cog_seat3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.service_status3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_service_status3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_service_status3, po_IRCreateIrconsumdetail_cpp.service_status3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.ftp_miles3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_ftp_miles3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_ftp_miles3 = po_IRCreateIrconsumdetail_cpp.ftp_miles3.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.boarding_pass3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_boarding_pass3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_boarding_pass3, po_IRCreateIrconsumdetail_cpp.boarding_pass3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_meal3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_meal3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_meal3, po_IRCreateIrconsumdetail_cpp.special_meal3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_need3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_need3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_need3, po_IRCreateIrconsumdetail_cpp.special_need3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.baggage3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_baggage3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_baggage3, po_IRCreateIrconsumdetail_cpp.baggage3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.crs_priced3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_crs_priced3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_crs_priced3, po_IRCreateIrconsumdetail_cpp.crs_priced3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cur_id3.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cur_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cur_id3, po_IRCreateIrconsumdetail_cpp.cur_id3.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.pax_id4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_pax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_pax_id4 = po_IRCreateIrconsumdetail_cpp.pax_id4.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_type4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_type4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_itin_type4, po_IRCreateIrconsumdetail_cpp.itin_type4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.itin_line_number4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_itin_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_itin_line_number4 = po_IRCreateIrconsumdetail_cpp.itin_line_number4.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.line_number4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_line_number4 = po_IRCreateIrconsumdetail_cpp.line_number4.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.seat4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_seat4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_seat4, po_IRCreateIrconsumdetail_cpp.seat4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.connection4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_connection4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_connection4, po_IRCreateIrconsumdetail_cpp.connection4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.smoking4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_smoking4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_smoking4, po_IRCreateIrconsumdetail_cpp.smoking4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_by_leg4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_by_leg4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_fare_by_leg4 = po_IRCreateIrconsumdetail_cpp.fare_by_leg4.to_double();
	}

	if(po_IRCreateIrconsumdetail_cpp.fare_basis4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_fare_basis4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_fare_basis4, po_IRCreateIrconsumdetail_cpp.fare_basis4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.tkt_desig4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_tkt_desig4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_tkt_desig4, po_IRCreateIrconsumdetail_cpp.tkt_desig4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cog_seat4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cog_seat4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cog_seat4, po_IRCreateIrconsumdetail_cpp.cog_seat4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.service_status4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_service_status4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_service_status4, po_IRCreateIrconsumdetail_cpp.service_status4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.ftp_miles4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_ftp_miles4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrconsumdetail.c_ftp_miles4 = po_IRCreateIrconsumdetail_cpp.ftp_miles4.to_long();
	}

	if(po_IRCreateIrconsumdetail_cpp.boarding_pass4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_boarding_pass4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_boarding_pass4, po_IRCreateIrconsumdetail_cpp.boarding_pass4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_meal4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_meal4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_meal4, po_IRCreateIrconsumdetail_cpp.special_meal4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.special_need4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_special_need4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_special_need4, po_IRCreateIrconsumdetail_cpp.special_need4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.baggage4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_baggage4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_baggage4, po_IRCreateIrconsumdetail_cpp.baggage4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.crs_priced4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_crs_priced4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_crs_priced4, po_IRCreateIrconsumdetail_cpp.crs_priced4.to_string());
	}

	if(po_IRCreateIrconsumdetail_cpp.cur_id4.NotIsNull()) {
		ps_IRCreateIrconsumdetailInd.ind_cur_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrconsumdetail.c_cur_id4, po_IRCreateIrconsumdetail_cpp.cur_id4.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrtaxdetail(SCIRCreateIrtaxdetail *ps_IRCreateIrtaxdetail,
          SCIRCreateIrtaxdetailInd *ps_IRCreateIrtaxdetailInd);
}

//Constructor
CIRCreateIrtaxdetail::CIRCreateIrtaxdetail() :
	session_id(8),
	tax_id1(2),
	tax_amount1(13, 2),
	tax_source1(10),
	city_id1(3),
	tax_id2(2),
	tax_source2(10),
	city_id2(3),
	tax_amount2(13, 2),
	tax_id3(2),
	tax_source3(10),
	city_id3(3),
	tax_amount3(13, 2),
	tax_id4(2),
	tax_source4(10),
	city_id4(3),
	tax_amount4(13, 2),
	tax_id5(2),
	tax_source5(10),
	city_id5(3),
	tax_amount5(13, 2),
	tax_id6(2),
	tax_source6(10),
	city_id6(3),
	tax_amount6(13, 2),
	tax_id7(2),
	tax_source7(10),
	city_id7(3),
	tax_amount7(13, 2),
	tax_id8(2),
	tax_source8(10),
	city_id8(3),
	tax_amount8(13, 2),
	tax_id9(2),
	tax_source9(10),
	city_id9(3),
	tax_amount9(13, 2),
	tax_id0(2),
	tax_source0(10),
	city_id0(3),
	tax_amount0(13, 2)
{
}

void
CDIR::createIrtaxdetail(CIRCreateIrtaxdetail &po_IRCreateIrtaxdetail_cpp)
{
	SCIRCreateIrtaxdetail ls_IRCreateIrtaxdetail;
	SCIRCreateIrtaxdetailInd ls_IRCreateIrtaxdetailInd;

	memset(&ls_IRCreateIrtaxdetail, 0, sizeof(SCIRCreateIrtaxdetail));
	memset(&ls_IRCreateIrtaxdetailInd, -1, sizeof(SCIRCreateIrtaxdetailInd));

	createIrtaxdetail_appToDb(ls_IRCreateIrtaxdetail, ls_IRCreateIrtaxdetailInd,
			po_IRCreateIrtaxdetail_cpp);

	logTimer("B:IRCreateIrtaxdetail", NULL);
	if(::dbIRCreateIrtaxdetail(&ls_IRCreateIrtaxdetail, &ls_IRCreateIrtaxdetailInd) != 0) {
		logTimer("A:IRCreateIrtaxdetail (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrtaxdetail", NULL);

	createIrtaxdetail_dbToApp(ls_IRCreateIrtaxdetail, ls_IRCreateIrtaxdetailInd,
			po_IRCreateIrtaxdetail_cpp);
}

// C to C++ assignments:
void
CDIR::createIrtaxdetail_dbToApp(SCIRCreateIrtaxdetail &ps_IRCreateIrtaxdetail,
		SCIRCreateIrtaxdetailInd &ps_IRCreateIrtaxdetailInd,
		CIRCreateIrtaxdetail &po_IRCreateIrtaxdetail_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrtaxdetail_appToDb(SCIRCreateIrtaxdetail &ps_IRCreateIrtaxdetail,
		SCIRCreateIrtaxdetailInd &ps_IRCreateIrtaxdetailInd,
		CIRCreateIrtaxdetail &po_IRCreateIrtaxdetail_cpp)
{
	int ii;

	if(po_IRCreateIrtaxdetail_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_session_id, po_IRCreateIrtaxdetail_cpp.session_id.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id1.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id1 = po_IRCreateIrtaxdetail_cpp.pax_id1.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number1 = po_IRCreateIrtaxdetail_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id1.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id1, po_IRCreateIrtaxdetail_cpp.tax_id1.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount1.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount1 = po_IRCreateIrtaxdetail_cpp.tax_amount1.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source1.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source1, po_IRCreateIrtaxdetail_cpp.tax_source1.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id1.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id1, po_IRCreateIrtaxdetail_cpp.city_id1.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id2.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id2 = po_IRCreateIrtaxdetail_cpp.pax_id2.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number2.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number2 = po_IRCreateIrtaxdetail_cpp.line_number2.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id2.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id2, po_IRCreateIrtaxdetail_cpp.tax_id2.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source2.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source2, po_IRCreateIrtaxdetail_cpp.tax_source2.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id2.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id2, po_IRCreateIrtaxdetail_cpp.city_id2.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount2.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount2 = po_IRCreateIrtaxdetail_cpp.tax_amount2.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id3.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id3 = po_IRCreateIrtaxdetail_cpp.pax_id3.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number3.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number3 = po_IRCreateIrtaxdetail_cpp.line_number3.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id3.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id3, po_IRCreateIrtaxdetail_cpp.tax_id3.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source3.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source3, po_IRCreateIrtaxdetail_cpp.tax_source3.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id3.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id3, po_IRCreateIrtaxdetail_cpp.city_id3.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount3.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount3 = po_IRCreateIrtaxdetail_cpp.tax_amount3.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id4.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id4 = po_IRCreateIrtaxdetail_cpp.pax_id4.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number4.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number4 = po_IRCreateIrtaxdetail_cpp.line_number4.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id4.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id4, po_IRCreateIrtaxdetail_cpp.tax_id4.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source4.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source4, po_IRCreateIrtaxdetail_cpp.tax_source4.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id4.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id4, po_IRCreateIrtaxdetail_cpp.city_id4.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount4.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount4 = po_IRCreateIrtaxdetail_cpp.tax_amount4.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id5.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id5 = po_IRCreateIrtaxdetail_cpp.pax_id5.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number5.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number5 = po_IRCreateIrtaxdetail_cpp.line_number5.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id5.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id5, po_IRCreateIrtaxdetail_cpp.tax_id5.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source5.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source5, po_IRCreateIrtaxdetail_cpp.tax_source5.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id5.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id5, po_IRCreateIrtaxdetail_cpp.city_id5.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount5.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount5 = po_IRCreateIrtaxdetail_cpp.tax_amount5.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id6.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id6 = po_IRCreateIrtaxdetail_cpp.pax_id6.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number6.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number6 = po_IRCreateIrtaxdetail_cpp.line_number6.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id6.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id6, po_IRCreateIrtaxdetail_cpp.tax_id6.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source6.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source6, po_IRCreateIrtaxdetail_cpp.tax_source6.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id6.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id6, po_IRCreateIrtaxdetail_cpp.city_id6.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount6.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount6 = po_IRCreateIrtaxdetail_cpp.tax_amount6.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id7.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id7 = po_IRCreateIrtaxdetail_cpp.pax_id7.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number7.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number7 = po_IRCreateIrtaxdetail_cpp.line_number7.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id7.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id7, po_IRCreateIrtaxdetail_cpp.tax_id7.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source7.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source7, po_IRCreateIrtaxdetail_cpp.tax_source7.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id7.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id7 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id7, po_IRCreateIrtaxdetail_cpp.city_id7.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount7.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount7 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount7 = po_IRCreateIrtaxdetail_cpp.tax_amount7.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id8.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id8 = po_IRCreateIrtaxdetail_cpp.pax_id8.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number8.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number8 = po_IRCreateIrtaxdetail_cpp.line_number8.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id8.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id8, po_IRCreateIrtaxdetail_cpp.tax_id8.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source8.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source8, po_IRCreateIrtaxdetail_cpp.tax_source8.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id8.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id8 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id8, po_IRCreateIrtaxdetail_cpp.city_id8.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount8.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount8 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount8 = po_IRCreateIrtaxdetail_cpp.tax_amount8.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id9.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id9 = po_IRCreateIrtaxdetail_cpp.pax_id9.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number9.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number9 = po_IRCreateIrtaxdetail_cpp.line_number9.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id9.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id9, po_IRCreateIrtaxdetail_cpp.tax_id9.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source9.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source9, po_IRCreateIrtaxdetail_cpp.tax_source9.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id9.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id9 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id9, po_IRCreateIrtaxdetail_cpp.city_id9.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount9.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount9 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount9 = po_IRCreateIrtaxdetail_cpp.tax_amount9.to_double();
	}

	if(po_IRCreateIrtaxdetail_cpp.pax_id0.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_pax_id0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_pax_id0 = po_IRCreateIrtaxdetail_cpp.pax_id0.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.line_number0.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_line_number0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_line_number0 = po_IRCreateIrtaxdetail_cpp.line_number0.to_long();
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_id0.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_id0 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_id0, po_IRCreateIrtaxdetail_cpp.tax_id0.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_source0.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_source0 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_tax_source0, po_IRCreateIrtaxdetail_cpp.tax_source0.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.city_id0.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_city_id0 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtaxdetail.c_city_id0, po_IRCreateIrtaxdetail_cpp.city_id0.to_string());
	}

	if(po_IRCreateIrtaxdetail_cpp.tax_amount0.NotIsNull()) {
		ps_IRCreateIrtaxdetailInd.ind_tax_amount0 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtaxdetail.c_tax_amount0 = po_IRCreateIrtaxdetail_cpp.tax_amount0.to_double();
	}

}

extern "C" {
extern int
dbIRLoadClientFile(SCIRLoadClientFile *ps_IRLoadClientFile,
          SCIRLoadClientFileInd *ps_IRLoadClientFileInd);
}

//Constructor
CIRLoadClientFile::CIRLoadClientFile() :
	pc_session_id(8),
	pc_tran_id(6),
	pc_con_id(3),
	reference(40),
	cfa_id(15),
	mktg_list(1),
	pc_PNR_date(7),
	iata_no(8),
	pc_company(2),
	pc_branch(4),
	pc_cfa_code(3),
	pc_cfa_type(1),
	pc_cfa_con(3),
	pc_fname(30),
	pc_lname(30),
	pc_rejectCon(3),
	pc_rejectCop(2),
	pc_rejectBra(4),
	pc_city(3),
	pv_rejectirSub(10)
{
}

void
CDIR::loadClientFile(CIRLoadClientFile &po_IRLoadClientFile_cpp)
{
	SCIRLoadClientFile ls_IRLoadClientFile;
	SCIRLoadClientFileInd ls_IRLoadClientFileInd;

	memset(&ls_IRLoadClientFile, 0, sizeof(SCIRLoadClientFile));
	memset(&ls_IRLoadClientFileInd, -1, sizeof(SCIRLoadClientFileInd));

	loadClientFile_appToDb(ls_IRLoadClientFile, ls_IRLoadClientFileInd,
			po_IRLoadClientFile_cpp);

	logTimer("B:IRLoadClientFile", NULL);
	if(::dbIRLoadClientFile(&ls_IRLoadClientFile, &ls_IRLoadClientFileInd) != 0) {
		logTimer("A:IRLoadClientFile (FAILED)", NULL);
		return;
	}
	logTimer("A:IRLoadClientFile", NULL);

	loadClientFile_dbToApp(ls_IRLoadClientFile, ls_IRLoadClientFileInd,
			po_IRLoadClientFile_cpp);
}

// C to C++ assignments:
void
CDIR::loadClientFile_dbToApp(SCIRLoadClientFile &ps_IRLoadClientFile,
		SCIRLoadClientFileInd &ps_IRLoadClientFileInd,
		CIRLoadClientFile &po_IRLoadClientFile_cpp)
{
	int ii;

	if(ps_IRLoadClientFileInd.ind_pi_cfa_no
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pi_cfa_no = ps_IRLoadClientFile.c_pi_cfa_no;
	}

	if(ps_IRLoadClientFileInd.ind_pc_cfa_code
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_cfa_code = ps_IRLoadClientFile.c_pc_cfa_code;
	}

	if(ps_IRLoadClientFileInd.ind_pc_cfa_type
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_cfa_type = ps_IRLoadClientFile.c_pc_cfa_type;
	}

	if(ps_IRLoadClientFileInd.ind_pc_cfa_con
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_cfa_con = ps_IRLoadClientFile.c_pc_cfa_con;
	}

	if(ps_IRLoadClientFileInd.ind_pc_fname
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_fname = ps_IRLoadClientFile.c_pc_fname;
	}

	if(ps_IRLoadClientFileInd.ind_pc_lname
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_lname = ps_IRLoadClientFile.c_pc_lname;
	}

	if(ps_IRLoadClientFileInd.ind_pc_rejectCon
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_rejectCon = ps_IRLoadClientFile.c_pc_rejectCon;
	}

	if(ps_IRLoadClientFileInd.ind_pc_rejectCop
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_rejectCop = ps_IRLoadClientFile.c_pc_rejectCop;
	}

	if(ps_IRLoadClientFileInd.ind_pc_rejectBra
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_rejectBra = ps_IRLoadClientFile.c_pc_rejectBra;
	}

	if(ps_IRLoadClientFileInd.ind_pc_city
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pc_city = ps_IRLoadClientFile.c_pc_city;
	}

	if(ps_IRLoadClientFileInd.ind_pi_rejectirId
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pi_rejectirId = ps_IRLoadClientFile.c_pi_rejectirId;
	}

	if(ps_IRLoadClientFileInd.ind_pv_rejectirSub
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRLoadClientFile_cpp.pv_rejectirSub = ps_IRLoadClientFile.c_pv_rejectirSub;
	}

}

// C++ to C assignments:
void
CDIR::loadClientFile_appToDb(SCIRLoadClientFile &ps_IRLoadClientFile,
		SCIRLoadClientFileInd &ps_IRLoadClientFileInd,
		CIRLoadClientFile &po_IRLoadClientFile_cpp)
{
	int ii;

	if(po_IRLoadClientFile_cpp.pc_session_id.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_session_id, po_IRLoadClientFile_cpp.pc_session_id.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_tran_id.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_tran_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_tran_id, po_IRLoadClientFile_cpp.pc_tran_id.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_con_id.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_con_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_con_id, po_IRLoadClientFile_cpp.pc_con_id.to_string());
	}

	if(po_IRLoadClientFile_cpp.reference.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_reference, po_IRLoadClientFile_cpp.reference.to_string());
	}

	if(po_IRLoadClientFile_cpp.cfa_id.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_cfa_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_cfa_id, po_IRLoadClientFile_cpp.cfa_id.to_string());
	}

	if(po_IRLoadClientFile_cpp.mktg_list.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_mktg_list = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_mktg_list, po_IRLoadClientFile_cpp.mktg_list.to_string());
	}

	if(po_IRLoadClientFile_cpp.gmt_date_time.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_gmt_date_time = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRLoadClientFile.c_gmt_date_time = po_IRLoadClientFile_cpp.gmt_date_time.toCSDateTime();
	}

	if(po_IRLoadClientFile_cpp.pc_PNR_date.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_PNR_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_PNR_date, po_IRLoadClientFile_cpp.pc_PNR_date.to_string());
	}

	if(po_IRLoadClientFile_cpp.iata_no.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_iata_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_iata_no, po_IRLoadClientFile_cpp.iata_no.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_company.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_company = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_company, po_IRLoadClientFile_cpp.pc_company.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_branch.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_branch = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_branch, po_IRLoadClientFile_cpp.pc_branch.to_string());
	}

	if(po_IRLoadClientFile_cpp.pi_cfa_no.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pi_cfa_no = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRLoadClientFile.c_pi_cfa_no = po_IRLoadClientFile_cpp.pi_cfa_no.to_long();
	}

	if(po_IRLoadClientFile_cpp.pc_cfa_code.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_cfa_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_cfa_code, po_IRLoadClientFile_cpp.pc_cfa_code.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_cfa_type.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_cfa_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_cfa_type, po_IRLoadClientFile_cpp.pc_cfa_type.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_cfa_con.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_cfa_con = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_cfa_con, po_IRLoadClientFile_cpp.pc_cfa_con.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_fname.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_fname = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_fname, po_IRLoadClientFile_cpp.pc_fname.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_lname.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_lname = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_lname, po_IRLoadClientFile_cpp.pc_lname.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_rejectCon.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_rejectCon = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_rejectCon, po_IRLoadClientFile_cpp.pc_rejectCon.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_rejectCop.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_rejectCop = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_rejectCop, po_IRLoadClientFile_cpp.pc_rejectCop.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_rejectBra.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_rejectBra = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_rejectBra, po_IRLoadClientFile_cpp.pc_rejectBra.to_string());
	}

	if(po_IRLoadClientFile_cpp.pc_city.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pc_city = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pc_city, po_IRLoadClientFile_cpp.pc_city.to_string());
	}

	if(po_IRLoadClientFile_cpp.pi_rejectirId.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pi_rejectirId = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRLoadClientFile.c_pi_rejectirId = po_IRLoadClientFile_cpp.pi_rejectirId.to_long();
	}

	if(po_IRLoadClientFile_cpp.pv_rejectirSub.NotIsNull()) {
		ps_IRLoadClientFileInd.ind_pv_rejectirSub = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRLoadClientFile.c_pv_rejectirSub, po_IRLoadClientFile_cpp.pv_rejectirSub.to_string());
	}

}

extern "C" {
extern int
dbIRReject(SCIRReject *ps_IRReject,
          SCIRRejectInd *ps_IRRejectInd);
}

//Constructor
CIRReject::CIRReject() :
	pc_session_id(8),
	pc_rejectCop(2),
	pc_rejectBra(4),
	pc_rejectCon(3),
	reference(40),
	iata_no(8),
	description(255),
	queue_type(3)
{
}

void
CDIR::reject(CIRReject &po_IRReject_cpp)
{
	SCIRReject ls_IRReject;
	SCIRRejectInd ls_IRRejectInd;

	memset(&ls_IRReject, 0, sizeof(SCIRReject));
	memset(&ls_IRRejectInd, -1, sizeof(SCIRRejectInd));

	reject_appToDb(ls_IRReject, ls_IRRejectInd,
			po_IRReject_cpp);

	logTimer("B:IRReject", NULL);
	if(::dbIRReject(&ls_IRReject, &ls_IRRejectInd) != 0) {
		logTimer("A:IRReject (FAILED)", NULL);
		return;
	}
	logTimer("A:IRReject", NULL);

	reject_dbToApp(ls_IRReject, ls_IRRejectInd,
			po_IRReject_cpp);
}

// C to C++ assignments:
void
CDIR::reject_dbToApp(SCIRReject &ps_IRReject,
		SCIRRejectInd &ps_IRRejectInd,
		CIRReject &po_IRReject_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::reject_appToDb(SCIRReject &ps_IRReject,
		SCIRRejectInd &ps_IRRejectInd,
		CIRReject &po_IRReject_cpp)
{
	int ii;

	if(po_IRReject_cpp.pc_session_id.NotIsNull()) {
		ps_IRRejectInd.ind_pc_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_pc_session_id, po_IRReject_cpp.pc_session_id.to_string());
	}

	if(po_IRReject_cpp.pc_rejectCop.NotIsNull()) {
		ps_IRRejectInd.ind_pc_rejectCop = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_pc_rejectCop, po_IRReject_cpp.pc_rejectCop.to_string());
	}

	if(po_IRReject_cpp.pc_rejectBra.NotIsNull()) {
		ps_IRRejectInd.ind_pc_rejectBra = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_pc_rejectBra, po_IRReject_cpp.pc_rejectBra.to_string());
	}

	if(po_IRReject_cpp.pc_rejectCon.NotIsNull()) {
		ps_IRRejectInd.ind_pc_rejectCon = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_pc_rejectCon, po_IRReject_cpp.pc_rejectCon.to_string());
	}

	if(po_IRReject_cpp.reference.NotIsNull()) {
		ps_IRRejectInd.ind_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_reference, po_IRReject_cpp.reference.to_string());
	}

	if(po_IRReject_cpp.gmt_date_time.NotIsNull()) {
		ps_IRRejectInd.ind_gmt_date_time = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRReject.c_gmt_date_time = po_IRReject_cpp.gmt_date_time.toCSDateTime();
	}

	if(po_IRReject_cpp.iata_no.NotIsNull()) {
		ps_IRRejectInd.ind_iata_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_iata_no, po_IRReject_cpp.iata_no.to_string());
	}

	if(po_IRReject_cpp.description.NotIsNull()) {
		ps_IRRejectInd.ind_description = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_description, po_IRReject_cpp.description.to_string());
	}

	if(po_IRReject_cpp.queue_type.NotIsNull()) {
		ps_IRRejectInd.ind_queue_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRReject.c_queue_type, po_IRReject_cpp.queue_type.to_string());
	}

}

extern "C" {
extern int
dbIRGetLocalDate(SCIRGetLocalDate *ps_IRGetLocalDate,
          SCIRGetLocalDateInd *ps_IRGetLocalDateInd);
}

//Constructor
CIRGetLocalDate::CIRGetLocalDate() :
	pc_copId(2),
	pc_braId(4),
	pc_iataNo(8),
	pc_cityId(3)
{
}

void
CDIR::getLocalDate(CIRGetLocalDate &po_IRGetLocalDate_cpp)
{
	SCIRGetLocalDate ls_IRGetLocalDate;
	SCIRGetLocalDateInd ls_IRGetLocalDateInd;

	memset(&ls_IRGetLocalDate, 0, sizeof(SCIRGetLocalDate));
	memset(&ls_IRGetLocalDateInd, -1, sizeof(SCIRGetLocalDateInd));

	getLocalDate_appToDb(ls_IRGetLocalDate, ls_IRGetLocalDateInd,
			po_IRGetLocalDate_cpp);

	logTimer("B:IRGetLocalDate", NULL);
	if(::dbIRGetLocalDate(&ls_IRGetLocalDate, &ls_IRGetLocalDateInd) != 0) {
		logTimer("A:IRGetLocalDate (FAILED)", NULL);
		return;
	}
	logTimer("A:IRGetLocalDate", NULL);

	getLocalDate_dbToApp(ls_IRGetLocalDate, ls_IRGetLocalDateInd,
			po_IRGetLocalDate_cpp);
}

// C to C++ assignments:
void
CDIR::getLocalDate_dbToApp(SCIRGetLocalDate &ps_IRGetLocalDate,
		SCIRGetLocalDateInd &ps_IRGetLocalDateInd,
		CIRGetLocalDate &po_IRGetLocalDate_cpp)
{
	int ii;

	if(ps_IRGetLocalDateInd.ind_pd_gmtDateTime
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRGetLocalDate_cpp.pd_gmtDateTime.fromCSDateTime(ps_IRGetLocalDate.c_pd_gmtDateTime);
	}

	if(ps_IRGetLocalDateInd.ind_pd_lclDateTime
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRGetLocalDate_cpp.pd_lclDateTime.fromCSDateTime(ps_IRGetLocalDate.c_pd_lclDateTime);
	}

}

// C++ to C assignments:
void
CDIR::getLocalDate_appToDb(SCIRGetLocalDate &ps_IRGetLocalDate,
		SCIRGetLocalDateInd &ps_IRGetLocalDateInd,
		CIRGetLocalDate &po_IRGetLocalDate_cpp)
{
	int ii;

	if(po_IRGetLocalDate_cpp.pc_copId.NotIsNull()) {
		ps_IRGetLocalDateInd.ind_pc_copId = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetLocalDate.c_pc_copId, po_IRGetLocalDate_cpp.pc_copId.to_string());
	}

	if(po_IRGetLocalDate_cpp.pc_braId.NotIsNull()) {
		ps_IRGetLocalDateInd.ind_pc_braId = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetLocalDate.c_pc_braId, po_IRGetLocalDate_cpp.pc_braId.to_string());
	}

	if(po_IRGetLocalDate_cpp.pc_iataNo.NotIsNull()) {
		ps_IRGetLocalDateInd.ind_pc_iataNo = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetLocalDate.c_pc_iataNo, po_IRGetLocalDate_cpp.pc_iataNo.to_string());
	}

	if(po_IRGetLocalDate_cpp.pc_cityId.NotIsNull()) {
		ps_IRGetLocalDateInd.ind_pc_cityId = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetLocalDate.c_pc_cityId, po_IRGetLocalDate_cpp.pc_cityId.to_string());
	}

	if(po_IRGetLocalDate_cpp.pd_gmtDateTime.NotIsNull()) {
		ps_IRGetLocalDateInd.ind_pd_gmtDateTime = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRGetLocalDate.c_pd_gmtDateTime = po_IRGetLocalDate_cpp.pd_gmtDateTime.toCSDateTime();
	}

	if(po_IRGetLocalDate_cpp.pd_lclDateTime.NotIsNull()) {
		ps_IRGetLocalDateInd.ind_pd_lclDateTime = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRGetLocalDate.c_pd_lclDateTime = po_IRGetLocalDate_cpp.pd_lclDateTime.toCSDateTime();
	}

}

extern "C" {
extern int
dbIRCreateIrother(SCIRCreateIrother *ps_IRCreateIrother,
          SCIRCreateIrotherInd *ps_IRCreateIrotherInd);
}

//Constructor
CIRCreateIrother::CIRCreateIrother() :
	session_id(8),
	itin_type1(1),
	start_date1(7),
	start_time1(8),
	orig_city_id1(4),
	end_date1(7),
	end_time1(8),
	dest_city_id1(4),
	unit_type1(4),
	sup_id1(3),
	reference1(40),
	confirmation1(40),
	saving_code1(10),
	departure_point1(14),
	arrival_point1(14),
	svc_description1(40),
	cabin_no1(15),
	tour_no1(15),
	itin_type2(1),
	start_date2(7),
	start_time2(8),
	orig_city_id2(4),
	end_date2(7),
	end_time2(8),
	dest_city_id2(4),
	unit_type2(4),
	sup_id2(3),
	reference2(40),
	confirmation2(40),
	saving_code2(10),
	departure_point2(14),
	arrival_point2(14),
	svc_description2(40),
	cabin_no2(15),
	tour_no2(15),
	itin_type3(1),
	start_date3(7),
	start_time3(8),
	orig_city_id3(4),
	end_date3(7),
	end_time3(8),
	dest_city_id3(4),
	unit_type3(4),
	sup_id3(3),
	reference3(40),
	confirmation3(40),
	saving_code3(10),
	departure_point3(14),
	arrival_point3(14),
	svc_description3(40),
	cabin_no3(15),
	tour_no3(15)
{
}

void
CDIR::createIrother(CIRCreateIrother &po_IRCreateIrother_cpp)
{
	SCIRCreateIrother ls_IRCreateIrother;
	SCIRCreateIrotherInd ls_IRCreateIrotherInd;

	memset(&ls_IRCreateIrother, 0, sizeof(SCIRCreateIrother));
	memset(&ls_IRCreateIrotherInd, -1, sizeof(SCIRCreateIrotherInd));

	createIrother_appToDb(ls_IRCreateIrother, ls_IRCreateIrotherInd,
			po_IRCreateIrother_cpp);

	logTimer("B:IRCreateIrother", NULL);
	if(::dbIRCreateIrother(&ls_IRCreateIrother, &ls_IRCreateIrotherInd) != 0) {
		logTimer("A:IRCreateIrother (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrother", NULL);

	createIrother_dbToApp(ls_IRCreateIrother, ls_IRCreateIrotherInd,
			po_IRCreateIrother_cpp);
}

// C to C++ assignments:
void
CDIR::createIrother_dbToApp(SCIRCreateIrother &ps_IRCreateIrother,
		SCIRCreateIrotherInd &ps_IRCreateIrotherInd,
		CIRCreateIrother &po_IRCreateIrother_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrother_appToDb(SCIRCreateIrother &ps_IRCreateIrother,
		SCIRCreateIrotherInd &ps_IRCreateIrotherInd,
		CIRCreateIrother &po_IRCreateIrother_cpp)
{
	int ii;

	if(po_IRCreateIrother_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_session_id, po_IRCreateIrother_cpp.session_id.to_string());
	}

	if(po_IRCreateIrother_cpp.itin_type1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_itin_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_itin_type1, po_IRCreateIrother_cpp.itin_type1.to_string());
	}

	if(po_IRCreateIrother_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrother.c_line_number1 = po_IRCreateIrother_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrother_cpp.start_date1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_start_date1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_start_date1, po_IRCreateIrother_cpp.start_date1.to_string());
	}

	if(po_IRCreateIrother_cpp.start_time1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_start_time1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_start_time1, po_IRCreateIrother_cpp.start_time1.to_string());
	}

	if(po_IRCreateIrother_cpp.orig_city_id1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_orig_city_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_orig_city_id1, po_IRCreateIrother_cpp.orig_city_id1.to_string());
	}

	if(po_IRCreateIrother_cpp.end_date1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_end_date1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_end_date1, po_IRCreateIrother_cpp.end_date1.to_string());
	}

	if(po_IRCreateIrother_cpp.end_time1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_end_time1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_end_time1, po_IRCreateIrother_cpp.end_time1.to_string());
	}

	if(po_IRCreateIrother_cpp.dest_city_id1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_dest_city_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_dest_city_id1, po_IRCreateIrother_cpp.dest_city_id1.to_string());
	}

	if(po_IRCreateIrother_cpp.unit_type1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_unit_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_unit_type1, po_IRCreateIrother_cpp.unit_type1.to_string());
	}

	if(po_IRCreateIrother_cpp.unit_ct1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_unit_ct1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrother.c_unit_ct1 = po_IRCreateIrother_cpp.unit_ct1.to_long();
	}

	if(po_IRCreateIrother_cpp.sup_id1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_sup_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_sup_id1, po_IRCreateIrother_cpp.sup_id1.to_string());
	}

	if(po_IRCreateIrother_cpp.reference1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_reference1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_reference1, po_IRCreateIrother_cpp.reference1.to_string());
	}

	if(po_IRCreateIrother_cpp.confirmation1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_confirmation1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_confirmation1, po_IRCreateIrother_cpp.confirmation1.to_string());
	}

	if(po_IRCreateIrother_cpp.saving_code1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_saving_code1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_saving_code1, po_IRCreateIrother_cpp.saving_code1.to_string());
	}

	if(po_IRCreateIrother_cpp.departure_point1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_departure_point1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_departure_point1, po_IRCreateIrother_cpp.departure_point1.to_string());
	}

	if(po_IRCreateIrother_cpp.arrival_point1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_arrival_point1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_arrival_point1, po_IRCreateIrother_cpp.arrival_point1.to_string());
	}

	if(po_IRCreateIrother_cpp.svc_description1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_svc_description1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_svc_description1, po_IRCreateIrother_cpp.svc_description1.to_string());
	}

	if(po_IRCreateIrother_cpp.cabin_no1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_cabin_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_cabin_no1, po_IRCreateIrother_cpp.cabin_no1.to_string());
	}

	if(po_IRCreateIrother_cpp.tour_no1.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_tour_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_tour_no1, po_IRCreateIrother_cpp.tour_no1.to_string());
	}

	if(po_IRCreateIrother_cpp.itin_type2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_itin_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_itin_type2, po_IRCreateIrother_cpp.itin_type2.to_string());
	}

	if(po_IRCreateIrother_cpp.line_number2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrother.c_line_number2 = po_IRCreateIrother_cpp.line_number2.to_long();
	}

	if(po_IRCreateIrother_cpp.start_date2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_start_date2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_start_date2, po_IRCreateIrother_cpp.start_date2.to_string());
	}

	if(po_IRCreateIrother_cpp.start_time2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_start_time2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_start_time2, po_IRCreateIrother_cpp.start_time2.to_string());
	}

	if(po_IRCreateIrother_cpp.orig_city_id2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_orig_city_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_orig_city_id2, po_IRCreateIrother_cpp.orig_city_id2.to_string());
	}

	if(po_IRCreateIrother_cpp.end_date2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_end_date2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_end_date2, po_IRCreateIrother_cpp.end_date2.to_string());
	}

	if(po_IRCreateIrother_cpp.end_time2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_end_time2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_end_time2, po_IRCreateIrother_cpp.end_time2.to_string());
	}

	if(po_IRCreateIrother_cpp.dest_city_id2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_dest_city_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_dest_city_id2, po_IRCreateIrother_cpp.dest_city_id2.to_string());
	}

	if(po_IRCreateIrother_cpp.unit_type2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_unit_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_unit_type2, po_IRCreateIrother_cpp.unit_type2.to_string());
	}

	if(po_IRCreateIrother_cpp.unit_ct2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_unit_ct2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrother.c_unit_ct2 = po_IRCreateIrother_cpp.unit_ct2.to_long();
	}

	if(po_IRCreateIrother_cpp.sup_id2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_sup_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_sup_id2, po_IRCreateIrother_cpp.sup_id2.to_string());
	}

	if(po_IRCreateIrother_cpp.reference2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_reference2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_reference2, po_IRCreateIrother_cpp.reference2.to_string());
	}

	if(po_IRCreateIrother_cpp.confirmation2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_confirmation2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_confirmation2, po_IRCreateIrother_cpp.confirmation2.to_string());
	}

	if(po_IRCreateIrother_cpp.saving_code2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_saving_code2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_saving_code2, po_IRCreateIrother_cpp.saving_code2.to_string());
	}

	if(po_IRCreateIrother_cpp.departure_point2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_departure_point2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_departure_point2, po_IRCreateIrother_cpp.departure_point2.to_string());
	}

	if(po_IRCreateIrother_cpp.arrival_point2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_arrival_point2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_arrival_point2, po_IRCreateIrother_cpp.arrival_point2.to_string());
	}

	if(po_IRCreateIrother_cpp.svc_description2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_svc_description2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_svc_description2, po_IRCreateIrother_cpp.svc_description2.to_string());
	}

	if(po_IRCreateIrother_cpp.cabin_no2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_cabin_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_cabin_no2, po_IRCreateIrother_cpp.cabin_no2.to_string());
	}

	if(po_IRCreateIrother_cpp.tour_no2.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_tour_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_tour_no2, po_IRCreateIrother_cpp.tour_no2.to_string());
	}

	if(po_IRCreateIrother_cpp.itin_type3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_itin_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_itin_type3, po_IRCreateIrother_cpp.itin_type3.to_string());
	}

	if(po_IRCreateIrother_cpp.line_number3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrother.c_line_number3 = po_IRCreateIrother_cpp.line_number3.to_long();
	}

	if(po_IRCreateIrother_cpp.start_date3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_start_date3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_start_date3, po_IRCreateIrother_cpp.start_date3.to_string());
	}

	if(po_IRCreateIrother_cpp.start_time3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_start_time3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_start_time3, po_IRCreateIrother_cpp.start_time3.to_string());
	}

	if(po_IRCreateIrother_cpp.orig_city_id3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_orig_city_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_orig_city_id3, po_IRCreateIrother_cpp.orig_city_id3.to_string());
	}

	if(po_IRCreateIrother_cpp.end_date3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_end_date3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_end_date3, po_IRCreateIrother_cpp.end_date3.to_string());
	}

	if(po_IRCreateIrother_cpp.end_time3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_end_time3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_end_time3, po_IRCreateIrother_cpp.end_time3.to_string());
	}

	if(po_IRCreateIrother_cpp.dest_city_id3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_dest_city_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_dest_city_id3, po_IRCreateIrother_cpp.dest_city_id3.to_string());
	}

	if(po_IRCreateIrother_cpp.unit_type3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_unit_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_unit_type3, po_IRCreateIrother_cpp.unit_type3.to_string());
	}

	if(po_IRCreateIrother_cpp.unit_ct3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_unit_ct3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrother.c_unit_ct3 = po_IRCreateIrother_cpp.unit_ct3.to_long();
	}

	if(po_IRCreateIrother_cpp.sup_id3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_sup_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_sup_id3, po_IRCreateIrother_cpp.sup_id3.to_string());
	}

	if(po_IRCreateIrother_cpp.reference3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_reference3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_reference3, po_IRCreateIrother_cpp.reference3.to_string());
	}

	if(po_IRCreateIrother_cpp.confirmation3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_confirmation3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_confirmation3, po_IRCreateIrother_cpp.confirmation3.to_string());
	}

	if(po_IRCreateIrother_cpp.saving_code3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_saving_code3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_saving_code3, po_IRCreateIrother_cpp.saving_code3.to_string());
	}

	if(po_IRCreateIrother_cpp.departure_point3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_departure_point3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_departure_point3, po_IRCreateIrother_cpp.departure_point3.to_string());
	}

	if(po_IRCreateIrother_cpp.arrival_point3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_arrival_point3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_arrival_point3, po_IRCreateIrother_cpp.arrival_point3.to_string());
	}

	if(po_IRCreateIrother_cpp.svc_description3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_svc_description3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_svc_description3, po_IRCreateIrother_cpp.svc_description3.to_string());
	}

	if(po_IRCreateIrother_cpp.cabin_no3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_cabin_no3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_cabin_no3, po_IRCreateIrother_cpp.cabin_no3.to_string());
	}

	if(po_IRCreateIrother_cpp.tour_no3.NotIsNull()) {
		ps_IRCreateIrotherInd.ind_tour_no3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrother.c_tour_no3, po_IRCreateIrother_cpp.tour_no3.to_string());
	}

}

extern "C" {
extern int
dbIRCleanupIrtransport(SCIRCleanupIrtransport *ps_IRCleanupIrtransport,
          SCIRCleanupIrtransportInd *ps_IRCleanupIrtransportInd);
}

//Constructor
CIRCleanupIrtransport::CIRCleanupIrtransport() :
	session_id(8)
{
}

void
CDIR::cleanupIrtransport(CIRCleanupIrtransport &po_IRCleanupIrtransport_cpp)
{
	SCIRCleanupIrtransport ls_IRCleanupIrtransport;
	SCIRCleanupIrtransportInd ls_IRCleanupIrtransportInd;

	memset(&ls_IRCleanupIrtransport, 0, sizeof(SCIRCleanupIrtransport));
	memset(&ls_IRCleanupIrtransportInd, -1, sizeof(SCIRCleanupIrtransportInd));

	cleanupIrtransport_appToDb(ls_IRCleanupIrtransport, ls_IRCleanupIrtransportInd,
			po_IRCleanupIrtransport_cpp);

	logTimer("B:IRCleanupIrtransport", NULL);
	if(::dbIRCleanupIrtransport(&ls_IRCleanupIrtransport, &ls_IRCleanupIrtransportInd) != 0) {
		logTimer("A:IRCleanupIrtransport (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCleanupIrtransport", NULL);

	cleanupIrtransport_dbToApp(ls_IRCleanupIrtransport, ls_IRCleanupIrtransportInd,
			po_IRCleanupIrtransport_cpp);
}

// C to C++ assignments:
void
CDIR::cleanupIrtransport_dbToApp(SCIRCleanupIrtransport &ps_IRCleanupIrtransport,
		SCIRCleanupIrtransportInd &ps_IRCleanupIrtransportInd,
		CIRCleanupIrtransport &po_IRCleanupIrtransport_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::cleanupIrtransport_appToDb(SCIRCleanupIrtransport &ps_IRCleanupIrtransport,
		SCIRCleanupIrtransportInd &ps_IRCleanupIrtransportInd,
		CIRCleanupIrtransport &po_IRCleanupIrtransport_cpp)
{
	int ii;

	if(po_IRCleanupIrtransport_cpp.session_id.NotIsNull()) {
		ps_IRCleanupIrtransportInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCleanupIrtransport.c_session_id, po_IRCleanupIrtransport_cpp.session_id.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrrental(SCIRCreateIrrental *ps_IRCreateIrrental,
          SCIRCreateIrrentalInd *ps_IRCreateIrrentalInd);
}

//Constructor
CIRCreateIrrental::CIRCreateIrrental() :
	session_id(8),
	cop_id(2),
	itin_type(1),
	start_date(7),
	start_time(8),
	origin_city_id(4),
	end_date(7),
	end_time(8),
	dest_city_id(4),
	unit_type(4),
	unit_ct(15),
	sup_id(3),
	source(1),
	action_code(2),
	reason_code(10),
	crs_id(3),
	ex_day_charge(15),
	ex_hr_charge(15),
	ex_mile_charge(15),
	free_mile_code(1),
	free_mileage(10),
	dropoff_charge(15),
	regular_rate(15),
	booked_rate(15),
	local_rate(15),
	cur_id(3),
	rate_type(3),
	rate_code(10),
	rate_duration(1),
	rate_multip(15),
	comm_rate(15),
	multi_lvl_rate(1),
	saving_code(10),
	location(40),
	location_id(6),
	site_name(80),
	addr1(80),
	addr2(80),
	addr3(80),
	addr4(80),
	phone_acode(10),
	phone_no(10),
	fax_acode(10),
	fax_no(10),
	guarantee(40),
	confirmation(40),
	corporate_disc(40),
	corporation_id(40),
	cancel_policy(40),
	reference(40),
	fup(40),
	customer_id(40),
	man_creation(1),
	pickup_location(10),
	dropoff_location(10),
	discount_code(1),
	commissionable(1),
	preferred(1),
	service_status(1),
	virtualCard_cc(2),
	virtualCard_exp_date(4),
	virtualCard_deployment(40),
	virtualCard_card_usage_id(20),
	virtualCard_gross_value_ind(1),
	pcc_cur_id(3)
{
}

void
CDIR::createIrrental(CIRCreateIrrental &po_IRCreateIrrental_cpp)
{
	SCIRCreateIrrental ls_IRCreateIrrental;
	SCIRCreateIrrentalInd ls_IRCreateIrrentalInd;

	memset(&ls_IRCreateIrrental, 0, sizeof(SCIRCreateIrrental));
	memset(&ls_IRCreateIrrentalInd, -1, sizeof(SCIRCreateIrrentalInd));

	createIrrental_appToDb(ls_IRCreateIrrental, ls_IRCreateIrrentalInd,
			po_IRCreateIrrental_cpp);

	logTimer("B:IRCreateIrrental", NULL);
	if(::dbIRCreateIrrental(&ls_IRCreateIrrental, &ls_IRCreateIrrentalInd) != 0) {
		logTimer("A:IRCreateIrrental (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrrental", NULL);

	createIrrental_dbToApp(ls_IRCreateIrrental, ls_IRCreateIrrentalInd,
			po_IRCreateIrrental_cpp);
}

// C to C++ assignments:
void
CDIR::createIrrental_dbToApp(SCIRCreateIrrental &ps_IRCreateIrrental,
		SCIRCreateIrrentalInd &ps_IRCreateIrrentalInd,
		CIRCreateIrrental &po_IRCreateIrrental_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrrental_appToDb(SCIRCreateIrrental &ps_IRCreateIrrental,
		SCIRCreateIrrentalInd &ps_IRCreateIrrentalInd,
		CIRCreateIrrental &po_IRCreateIrrental_cpp)
{
	int ii;

	if(po_IRCreateIrrental_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_session_id, po_IRCreateIrrental_cpp.session_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.cop_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_cop_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_cop_id, po_IRCreateIrrental_cpp.cop_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.itin_type.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_itin_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_itin_type, po_IRCreateIrrental_cpp.itin_type.to_string());
	}

	if(po_IRCreateIrrental_cpp.line_number.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_line_number = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrrental.c_line_number = po_IRCreateIrrental_cpp.line_number.to_long();
	}

	if(po_IRCreateIrrental_cpp.start_date.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_start_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_start_date, po_IRCreateIrrental_cpp.start_date.to_string());
	}

	if(po_IRCreateIrrental_cpp.start_time.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_start_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_start_time, po_IRCreateIrrental_cpp.start_time.to_string());
	}

	if(po_IRCreateIrrental_cpp.origin_city_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_origin_city_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_origin_city_id, po_IRCreateIrrental_cpp.origin_city_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.end_date.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_end_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_end_date, po_IRCreateIrrental_cpp.end_date.to_string());
	}

	if(po_IRCreateIrrental_cpp.end_time.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_end_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_end_time, po_IRCreateIrrental_cpp.end_time.to_string());
	}

	if(po_IRCreateIrrental_cpp.dest_city_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_dest_city_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_dest_city_id, po_IRCreateIrrental_cpp.dest_city_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.unit_type.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_unit_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_unit_type, po_IRCreateIrrental_cpp.unit_type.to_string());
	}

	if(po_IRCreateIrrental_cpp.unit_ct.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_unit_ct = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_unit_ct, po_IRCreateIrrental_cpp.unit_ct.to_string());
	}

	if(po_IRCreateIrrental_cpp.sup_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_sup_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_sup_id, po_IRCreateIrrental_cpp.sup_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.source.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_source = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_source, po_IRCreateIrrental_cpp.source.to_string());
	}

	if(po_IRCreateIrrental_cpp.action_code.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_action_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_action_code, po_IRCreateIrrental_cpp.action_code.to_string());
	}

	if(po_IRCreateIrrental_cpp.reason_code.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_reason_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_reason_code, po_IRCreateIrrental_cpp.reason_code.to_string());
	}

	if(po_IRCreateIrrental_cpp.crs_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_crs_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_crs_id, po_IRCreateIrrental_cpp.crs_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.number_of_pax.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_number_of_pax = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrrental.c_number_of_pax = po_IRCreateIrrental_cpp.number_of_pax.to_long();
	}

	if(po_IRCreateIrrental_cpp.ex_day_charge.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_ex_day_charge = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_ex_day_charge, po_IRCreateIrrental_cpp.ex_day_charge.to_string());
	}

	if(po_IRCreateIrrental_cpp.ex_hr_charge.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_ex_hr_charge = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_ex_hr_charge, po_IRCreateIrrental_cpp.ex_hr_charge.to_string());
	}

	if(po_IRCreateIrrental_cpp.ex_mile_charge.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_ex_mile_charge = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_ex_mile_charge, po_IRCreateIrrental_cpp.ex_mile_charge.to_string());
	}

	if(po_IRCreateIrrental_cpp.free_mile_code.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_free_mile_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_free_mile_code, po_IRCreateIrrental_cpp.free_mile_code.to_string());
	}

	if(po_IRCreateIrrental_cpp.free_mileage.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_free_mileage = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_free_mileage, po_IRCreateIrrental_cpp.free_mileage.to_string());
	}

	if(po_IRCreateIrrental_cpp.dropoff_charge.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_dropoff_charge = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_dropoff_charge, po_IRCreateIrrental_cpp.dropoff_charge.to_string());
	}

	if(po_IRCreateIrrental_cpp.regular_rate.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_regular_rate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_regular_rate, po_IRCreateIrrental_cpp.regular_rate.to_string());
	}

	if(po_IRCreateIrrental_cpp.booked_rate.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_booked_rate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_booked_rate, po_IRCreateIrrental_cpp.booked_rate.to_string());
	}

	if(po_IRCreateIrrental_cpp.local_rate.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_local_rate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_local_rate, po_IRCreateIrrental_cpp.local_rate.to_string());
	}

	if(po_IRCreateIrrental_cpp.cur_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_cur_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_cur_id, po_IRCreateIrrental_cpp.cur_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.rate_type.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_rate_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_rate_type, po_IRCreateIrrental_cpp.rate_type.to_string());
	}

	if(po_IRCreateIrrental_cpp.rate_code.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_rate_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_rate_code, po_IRCreateIrrental_cpp.rate_code.to_string());
	}

	if(po_IRCreateIrrental_cpp.rate_duration.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_rate_duration = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_rate_duration, po_IRCreateIrrental_cpp.rate_duration.to_string());
	}

	if(po_IRCreateIrrental_cpp.rate_multip.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_rate_multip = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_rate_multip, po_IRCreateIrrental_cpp.rate_multip.to_string());
	}

	if(po_IRCreateIrrental_cpp.comm_rate.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_comm_rate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_comm_rate, po_IRCreateIrrental_cpp.comm_rate.to_string());
	}

	if(po_IRCreateIrrental_cpp.multi_lvl_rate.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_multi_lvl_rate = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_multi_lvl_rate, po_IRCreateIrrental_cpp.multi_lvl_rate.to_string());
	}

	if(po_IRCreateIrrental_cpp.saving_code.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_saving_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_saving_code, po_IRCreateIrrental_cpp.saving_code.to_string());
	}

	if(po_IRCreateIrrental_cpp.location.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_location = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_location, po_IRCreateIrrental_cpp.location.to_string());
	}

	if(po_IRCreateIrrental_cpp.location_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_location_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_location_id, po_IRCreateIrrental_cpp.location_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.site_name.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_site_name = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_site_name, po_IRCreateIrrental_cpp.site_name.to_string());
	}

	if(po_IRCreateIrrental_cpp.addr1.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_addr1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_addr1, po_IRCreateIrrental_cpp.addr1.to_string());
	}

	if(po_IRCreateIrrental_cpp.addr2.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_addr2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_addr2, po_IRCreateIrrental_cpp.addr2.to_string());
	}

	if(po_IRCreateIrrental_cpp.addr3.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_addr3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_addr3, po_IRCreateIrrental_cpp.addr3.to_string());
	}

	if(po_IRCreateIrrental_cpp.addr4.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_addr4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_addr4, po_IRCreateIrrental_cpp.addr4.to_string());
	}

	if(po_IRCreateIrrental_cpp.phone_acode.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_phone_acode = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_phone_acode, po_IRCreateIrrental_cpp.phone_acode.to_string());
	}

	if(po_IRCreateIrrental_cpp.phone_no.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_phone_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_phone_no, po_IRCreateIrrental_cpp.phone_no.to_string());
	}

	if(po_IRCreateIrrental_cpp.fax_acode.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_fax_acode = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_fax_acode, po_IRCreateIrrental_cpp.fax_acode.to_string());
	}

	if(po_IRCreateIrrental_cpp.fax_no.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_fax_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_fax_no, po_IRCreateIrrental_cpp.fax_no.to_string());
	}

	if(po_IRCreateIrrental_cpp.guarantee.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_guarantee = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_guarantee, po_IRCreateIrrental_cpp.guarantee.to_string());
	}

	if(po_IRCreateIrrental_cpp.confirmation.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_confirmation = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_confirmation, po_IRCreateIrrental_cpp.confirmation.to_string());
	}

	if(po_IRCreateIrrental_cpp.corporate_disc.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_corporate_disc = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_corporate_disc, po_IRCreateIrrental_cpp.corporate_disc.to_string());
	}

	if(po_IRCreateIrrental_cpp.corporation_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_corporation_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_corporation_id, po_IRCreateIrrental_cpp.corporation_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.cancel_policy.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_cancel_policy = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_cancel_policy, po_IRCreateIrrental_cpp.cancel_policy.to_string());
	}

	if(po_IRCreateIrrental_cpp.reference.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_reference, po_IRCreateIrrental_cpp.reference.to_string());
	}

	if(po_IRCreateIrrental_cpp.fup.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_fup = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_fup, po_IRCreateIrrental_cpp.fup.to_string());
	}

	if(po_IRCreateIrrental_cpp.customer_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_customer_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_customer_id, po_IRCreateIrrental_cpp.customer_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.harp_htlkey.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_harp_htlkey = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrrental.c_harp_htlkey = po_IRCreateIrrental_cpp.harp_htlkey.to_long();
	}

	if(po_IRCreateIrrental_cpp.man_creation.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_man_creation = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_man_creation, po_IRCreateIrrental_cpp.man_creation.to_string());
	}

	if(po_IRCreateIrrental_cpp.pickup_location.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_pickup_location = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_pickup_location, po_IRCreateIrrental_cpp.pickup_location.to_string());
	}

	if(po_IRCreateIrrental_cpp.dropoff_location.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_dropoff_location = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_dropoff_location, po_IRCreateIrrental_cpp.dropoff_location.to_string());
	}

	if(po_IRCreateIrrental_cpp.discount_code.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_discount_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_discount_code, po_IRCreateIrrental_cpp.discount_code.to_string());
	}

	if(po_IRCreateIrrental_cpp.commissionable.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_commissionable = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_commissionable, po_IRCreateIrrental_cpp.commissionable.to_string());
	}

   if(po_IRCreateIrrental_cpp.preferred.NotIsNull()) {
      ps_IRCreateIrrentalInd.ind_preferred = C_SYBASE_INDICATOR_NOT_NULL;
      strcpy(ps_IRCreateIrrental.c_preferred, po_IRCreateIrrental_cpp.preferred.to_string());
   }
 

	if(po_IRCreateIrrental_cpp.service_status.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_service_status = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_service_status, po_IRCreateIrrental_cpp.service_status.to_string());
	}
	if(po_IRCreateIrrental_cpp.virtualCard_cc.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_virtualCard_cc = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_virtualCard_cc, po_IRCreateIrrental_cpp.virtualCard_cc.to_string());
	}

	if(po_IRCreateIrrental_cpp.virtualCard_exp_date.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_virtualCard_exp_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_virtualCard_exp_date, po_IRCreateIrrental_cpp.virtualCard_exp_date.to_string());
	}

	if(po_IRCreateIrrental_cpp.virtualCard_deployment.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_virtualCard_deployment = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_virtualCard_deployment, po_IRCreateIrrental_cpp.virtualCard_deployment.to_string());
	}

	if(po_IRCreateIrrental_cpp.virtualCard_card_usage_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_virtualCard_card_usage_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_virtualCard_card_usage_id, po_IRCreateIrrental_cpp.virtualCard_card_usage_id.to_string());
	}

	if(po_IRCreateIrrental_cpp.virtualCard_gross_value_ind.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_virtualCard_gross_value_ind = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_virtualCard_gross_value_ind, po_IRCreateIrrental_cpp.virtualCard_gross_value_ind.to_string());
	}

	if(po_IRCreateIrrental_cpp.pcc_cur_id.NotIsNull()) {
		ps_IRCreateIrrentalInd.ind_pcc_cur_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrrental.c_pcc_cur_id, po_IRCreateIrrental_cpp.pcc_cur_id.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrticket(SCIRCreateIrticket *ps_IRCreateIrticket,
          SCIRCreateIrticketInd *ps_IRCreateIrticketInd);
}

//Constructor
CIRCreateIrticket::CIRCreateIrticket() :
	session_id(8),
	cop_id(2),
	action_code(2),
	source(1),
	reason_code(10),
	crs_id(3),
	commission_type(1),
	net_ticket_ind(1),
	net_ticket_type(1),
	nett_fare_to_airline(13, 2),
	selling_price_to_shop(13, 2),
	selling_price_to_client(13, 2),
	tour_oper_commission(13, 2),
	ticket_centre_reference(30),
	grid_reference(30),
	shop_reference(30),
	publish_fare_tc_commission(13, 2),
	publish_fare_agent_commission(13, 2),
	reference1(40),
	sup_id1(3),
	cur_id1(3),
	form1(6),
	ticket_no1(30),
	pic1(6),
	orig_ticket_no1(30),
	comm_rate1(13, 2),
	commission1(13, 2),
	no_of_books1(15),
	exchange_code1(1),
	penalty_ch_fee1(13, 2),
	comm_penalty1(13, 2),
	base_cost1(13, 2),
	saving_code1(10),
	full_fare1(13, 2),
	low_prac_fare1(13, 2),
	personal_crs1(1),
	stp_home_city1(4),
	stp_host_city1(4),
	sel_access1(30),
	crs_tour1(20),
	rebate1(13, 2),
	doc_type1(3),
	e_ticket1(1),
	refund1(1),
	reference2(40),
	sup_id2(3),
	cur_id2(3),
	form2(6),
	ticket_no2(30),
	pic2(6),
	orig_ticket_no2(30),
	comm_rate2(13, 2),
	commission2(13, 2),
	no_of_books2(15),
	exchange_code2(1),
	penalty_ch_fee2(13, 2),
	comm_penalty2(13, 2),
	base_cost2(13, 2),
	saving_code2(10),
	full_fare2(13, 2),
	low_prac_fare2(13, 2),
	personal_crs2(1),
	stp_home_city2(4),
	stp_host_city2(4),
	sel_access2(30),
	crs_tour2(20),
	rebate2(13, 2),
	doc_type2(3),
	e_ticket2(1),
	refund2(1),
	reference3(40),
	sup_id3(3),
	cur_id3(3),
	form3(6),
	ticket_no3(30),
	pic3(6),
	orig_ticket_no3(30),
	comm_rate3(13, 2),
	commission3(13, 2),
	no_of_books3(15),
	exchange_code3(1),
	penalty_ch_fee3(13, 2),
	comm_penalty3(13, 2),
	base_cost3(13, 2),
	saving_code3(10),
	full_fare3(13, 2),
	low_prac_fare3(13, 2),
	personal_crs3(1),
	stp_home_city3(4),
	stp_host_city3(4),
	sel_access3(30),
	crs_tour3(20),
	rebate3(13, 2),
	doc_type3(3),
	e_ticket3(1),
	refund3(1)
{
}

void
CDIR::createIrticket(CIRCreateIrticket &po_IRCreateIrticket_cpp)
{
	SCIRCreateIrticket ls_IRCreateIrticket;
	SCIRCreateIrticketInd ls_IRCreateIrticketInd;

	memset(&ls_IRCreateIrticket, 0, sizeof(SCIRCreateIrticket));
	memset(&ls_IRCreateIrticketInd, -1, sizeof(SCIRCreateIrticketInd));

	createIrticket_appToDb(ls_IRCreateIrticket, ls_IRCreateIrticketInd,
			po_IRCreateIrticket_cpp);

	logTimer("B:IRCreateIrticket", NULL);
	if(::dbIRCreateIrticket(&ls_IRCreateIrticket, &ls_IRCreateIrticketInd) != 0) {
		logTimer("A:IRCreateIrticket (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrticket", NULL);

	createIrticket_dbToApp(ls_IRCreateIrticket, ls_IRCreateIrticketInd,
			po_IRCreateIrticket_cpp);
}

// C to C++ assignments:
void
CDIR::createIrticket_dbToApp(SCIRCreateIrticket &ps_IRCreateIrticket,
		SCIRCreateIrticketInd &ps_IRCreateIrticketInd,
		CIRCreateIrticket &po_IRCreateIrticket_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrticket_appToDb(SCIRCreateIrticket &ps_IRCreateIrticket,
		SCIRCreateIrticketInd &ps_IRCreateIrticketInd,
		CIRCreateIrticket &po_IRCreateIrticket_cpp)
{
	int ii;

	if(po_IRCreateIrticket_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_session_id, po_IRCreateIrticket_cpp.session_id.to_string());
	}

	if(po_IRCreateIrticket_cpp.cop_id.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_cop_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_cop_id, po_IRCreateIrticket_cpp.cop_id.to_string());
	}

	if(po_IRCreateIrticket_cpp.action_code.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_action_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_action_code, po_IRCreateIrticket_cpp.action_code.to_string());
	}

	if(po_IRCreateIrticket_cpp.source.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_source = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_source, po_IRCreateIrticket_cpp.source.to_string());
	}

	if(po_IRCreateIrticket_cpp.reason_code.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_reason_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_reason_code, po_IRCreateIrticket_cpp.reason_code.to_string());
	}

	if(po_IRCreateIrticket_cpp.crs_id.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_crs_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_crs_id, po_IRCreateIrticket_cpp.crs_id.to_string());
	}

	if(po_IRCreateIrticket_cpp.commission_type.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_commission_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_commission_type, po_IRCreateIrticket_cpp.commission_type.to_string());
	}

	if(po_IRCreateIrticket_cpp.net_ticket_ind.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_net_ticket_ind = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_net_ticket_ind, po_IRCreateIrticket_cpp.net_ticket_ind.to_string());
	}

	if(po_IRCreateIrticket_cpp.net_ticket_type.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_net_ticket_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_net_ticket_type, po_IRCreateIrticket_cpp.net_ticket_type.to_string());
	}

	if(po_IRCreateIrticket_cpp.nett_fare_to_airline.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_nett_fare_to_airline = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_nett_fare_to_airline = po_IRCreateIrticket_cpp.nett_fare_to_airline.to_double();
	}

	if(po_IRCreateIrticket_cpp.selling_price_to_shop.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_selling_price_to_shop = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_selling_price_to_shop = po_IRCreateIrticket_cpp.selling_price_to_shop.to_double();
	}

	if(po_IRCreateIrticket_cpp.selling_price_to_client.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_selling_price_to_client = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_selling_price_to_client = po_IRCreateIrticket_cpp.selling_price_to_client.to_double();
	}

	if(po_IRCreateIrticket_cpp.tour_oper_commission.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_tour_oper_commission = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_tour_oper_commission = po_IRCreateIrticket_cpp.tour_oper_commission.to_double();
	}

	if(po_IRCreateIrticket_cpp.ticket_centre_reference.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_ticket_centre_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_ticket_centre_reference, po_IRCreateIrticket_cpp.ticket_centre_reference.to_string());
	}

	if(po_IRCreateIrticket_cpp.grid_reference.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_grid_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_grid_reference, po_IRCreateIrticket_cpp.grid_reference.to_string());
	}

	if(po_IRCreateIrticket_cpp.shop_reference.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_shop_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_shop_reference, po_IRCreateIrticket_cpp.shop_reference.to_string());
	}

	if(po_IRCreateIrticket_cpp.publish_fare_tc_commission.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_publish_fare_tc_commission = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_publish_fare_tc_commission = po_IRCreateIrticket_cpp.publish_fare_tc_commission.to_double();
	}

	if(po_IRCreateIrticket_cpp.publish_fare_agent_commission.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_publish_fare_agent_commission = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_publish_fare_agent_commission = po_IRCreateIrticket_cpp.publish_fare_agent_commission.to_double();
	}

	if(po_IRCreateIrticket_cpp.pax_id1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_pax_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_pax_id1 = po_IRCreateIrticket_cpp.pax_id1.to_long();
	}

	if(po_IRCreateIrticket_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_line_number1 = po_IRCreateIrticket_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrticket_cpp.cost_line1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_cost_line1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_cost_line1 = po_IRCreateIrticket_cpp.cost_line1.to_long();
	}

	if(po_IRCreateIrticket_cpp.reference1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_reference1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_reference1, po_IRCreateIrticket_cpp.reference1.to_string());
	}

	if(po_IRCreateIrticket_cpp.sup_id1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_sup_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_sup_id1, po_IRCreateIrticket_cpp.sup_id1.to_string());
	}

	if(po_IRCreateIrticket_cpp.cur_id1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_cur_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_cur_id1, po_IRCreateIrticket_cpp.cur_id1.to_string());
	}

	if(po_IRCreateIrticket_cpp.form1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_form1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_form1, po_IRCreateIrticket_cpp.form1.to_string());
	}

	if(po_IRCreateIrticket_cpp.ticket_no1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_ticket_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_ticket_no1, po_IRCreateIrticket_cpp.ticket_no1.to_string());
	}

	if(po_IRCreateIrticket_cpp.pic1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_pic1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_pic1, po_IRCreateIrticket_cpp.pic1.to_string());
	}

	if(po_IRCreateIrticket_cpp.orig_ticket_no1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_orig_ticket_no1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_orig_ticket_no1, po_IRCreateIrticket_cpp.orig_ticket_no1.to_string());
	}

	if(po_IRCreateIrticket_cpp.comm_rate1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_comm_rate1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_comm_rate1 = po_IRCreateIrticket_cpp.comm_rate1.to_double();
	}

	if(po_IRCreateIrticket_cpp.commission1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_commission1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_commission1 = po_IRCreateIrticket_cpp.commission1.to_double();
	}

	if(po_IRCreateIrticket_cpp.no_of_books1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_no_of_books1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_no_of_books1, po_IRCreateIrticket_cpp.no_of_books1.to_string());
	}

	if(po_IRCreateIrticket_cpp.exchange_code1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_exchange_code1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_exchange_code1, po_IRCreateIrticket_cpp.exchange_code1.to_string());
	}

	if(po_IRCreateIrticket_cpp.penalty_ch_fee1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_penalty_ch_fee1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_penalty_ch_fee1 = po_IRCreateIrticket_cpp.penalty_ch_fee1.to_double();
	}

	if(po_IRCreateIrticket_cpp.comm_penalty1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_comm_penalty1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_comm_penalty1 = po_IRCreateIrticket_cpp.comm_penalty1.to_double();
	}

	if(po_IRCreateIrticket_cpp.base_cost1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_base_cost1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_base_cost1 = po_IRCreateIrticket_cpp.base_cost1.to_double();
	}

	if(po_IRCreateIrticket_cpp.saving_code1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_saving_code1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_saving_code1, po_IRCreateIrticket_cpp.saving_code1.to_string());
	}

	if(po_IRCreateIrticket_cpp.full_fare1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_full_fare1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_full_fare1 = po_IRCreateIrticket_cpp.full_fare1.to_double();
	}

	if(po_IRCreateIrticket_cpp.low_prac_fare1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_low_prac_fare1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_low_prac_fare1 = po_IRCreateIrticket_cpp.low_prac_fare1.to_double();
	}

	if(po_IRCreateIrticket_cpp.personal_crs1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_personal_crs1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_personal_crs1, po_IRCreateIrticket_cpp.personal_crs1.to_string());
	}

	if(po_IRCreateIrticket_cpp.stp_home_city1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_stp_home_city1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_stp_home_city1, po_IRCreateIrticket_cpp.stp_home_city1.to_string());
	}

	if(po_IRCreateIrticket_cpp.stp_host_city1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_stp_host_city1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_stp_host_city1, po_IRCreateIrticket_cpp.stp_host_city1.to_string());
	}

	if(po_IRCreateIrticket_cpp.sel_access1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_sel_access1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_sel_access1, po_IRCreateIrticket_cpp.sel_access1.to_string());
	}

	if(po_IRCreateIrticket_cpp.crs_tour1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_crs_tour1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_crs_tour1, po_IRCreateIrticket_cpp.crs_tour1.to_string());
	}

	if(po_IRCreateIrticket_cpp.rebate1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_rebate1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_rebate1 = po_IRCreateIrticket_cpp.rebate1.to_double();
	}

	if(po_IRCreateIrticket_cpp.doc_type1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_doc_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_doc_type1, po_IRCreateIrticket_cpp.doc_type1.to_string());
	}

	if(po_IRCreateIrticket_cpp.e_ticket1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_e_ticket1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_e_ticket1, po_IRCreateIrticket_cpp.e_ticket1.to_string());
	}

	if(po_IRCreateIrticket_cpp.refund1.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_refund1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_refund1, po_IRCreateIrticket_cpp.refund1.to_string());
	}

	if(po_IRCreateIrticket_cpp.pax_id2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_pax_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_pax_id2 = po_IRCreateIrticket_cpp.pax_id2.to_long();
	}

	if(po_IRCreateIrticket_cpp.line_number2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_line_number2 = po_IRCreateIrticket_cpp.line_number2.to_long();
	}

	if(po_IRCreateIrticket_cpp.cost_line2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_cost_line2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_cost_line2 = po_IRCreateIrticket_cpp.cost_line2.to_long();
	}

	if(po_IRCreateIrticket_cpp.reference2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_reference2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_reference2, po_IRCreateIrticket_cpp.reference2.to_string());
	}

	if(po_IRCreateIrticket_cpp.sup_id2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_sup_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_sup_id2, po_IRCreateIrticket_cpp.sup_id2.to_string());
	}

	if(po_IRCreateIrticket_cpp.cur_id2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_cur_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_cur_id2, po_IRCreateIrticket_cpp.cur_id2.to_string());
	}

	if(po_IRCreateIrticket_cpp.form2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_form2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_form2, po_IRCreateIrticket_cpp.form2.to_string());
	}

	if(po_IRCreateIrticket_cpp.ticket_no2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_ticket_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_ticket_no2, po_IRCreateIrticket_cpp.ticket_no2.to_string());
	}

	if(po_IRCreateIrticket_cpp.pic2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_pic2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_pic2, po_IRCreateIrticket_cpp.pic2.to_string());
	}

	if(po_IRCreateIrticket_cpp.orig_ticket_no2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_orig_ticket_no2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_orig_ticket_no2, po_IRCreateIrticket_cpp.orig_ticket_no2.to_string());
	}

	if(po_IRCreateIrticket_cpp.comm_rate2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_comm_rate2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_comm_rate2 = po_IRCreateIrticket_cpp.comm_rate2.to_double();
	}

	if(po_IRCreateIrticket_cpp.commission2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_commission2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_commission2 = po_IRCreateIrticket_cpp.commission2.to_double();
	}

	if(po_IRCreateIrticket_cpp.no_of_books2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_no_of_books2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_no_of_books2, po_IRCreateIrticket_cpp.no_of_books2.to_string());
	}

	if(po_IRCreateIrticket_cpp.exchange_code2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_exchange_code2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_exchange_code2, po_IRCreateIrticket_cpp.exchange_code2.to_string());
	}

	if(po_IRCreateIrticket_cpp.penalty_ch_fee2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_penalty_ch_fee2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_penalty_ch_fee2 = po_IRCreateIrticket_cpp.penalty_ch_fee2.to_double();
	}

	if(po_IRCreateIrticket_cpp.comm_penalty2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_comm_penalty2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_comm_penalty2 = po_IRCreateIrticket_cpp.comm_penalty2.to_double();
	}

	if(po_IRCreateIrticket_cpp.base_cost2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_base_cost2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_base_cost2 = po_IRCreateIrticket_cpp.base_cost2.to_double();
	}

	if(po_IRCreateIrticket_cpp.saving_code2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_saving_code2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_saving_code2, po_IRCreateIrticket_cpp.saving_code2.to_string());
	}

	if(po_IRCreateIrticket_cpp.full_fare2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_full_fare2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_full_fare2 = po_IRCreateIrticket_cpp.full_fare2.to_double();
	}

	if(po_IRCreateIrticket_cpp.low_prac_fare2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_low_prac_fare2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_low_prac_fare2 = po_IRCreateIrticket_cpp.low_prac_fare2.to_double();
	}

	if(po_IRCreateIrticket_cpp.personal_crs2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_personal_crs2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_personal_crs2, po_IRCreateIrticket_cpp.personal_crs2.to_string());
	}

	if(po_IRCreateIrticket_cpp.stp_home_city2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_stp_home_city2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_stp_home_city2, po_IRCreateIrticket_cpp.stp_home_city2.to_string());
	}

	if(po_IRCreateIrticket_cpp.stp_host_city2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_stp_host_city2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_stp_host_city2, po_IRCreateIrticket_cpp.stp_host_city2.to_string());
	}

	if(po_IRCreateIrticket_cpp.sel_access2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_sel_access2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_sel_access2, po_IRCreateIrticket_cpp.sel_access2.to_string());
	}

	if(po_IRCreateIrticket_cpp.crs_tour2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_crs_tour2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_crs_tour2, po_IRCreateIrticket_cpp.crs_tour2.to_string());
	}

	if(po_IRCreateIrticket_cpp.rebate2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_rebate2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_rebate2 = po_IRCreateIrticket_cpp.rebate2.to_double();
	}

	if(po_IRCreateIrticket_cpp.doc_type2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_doc_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_doc_type2, po_IRCreateIrticket_cpp.doc_type2.to_string());
	}

	if(po_IRCreateIrticket_cpp.e_ticket2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_e_ticket2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_e_ticket2, po_IRCreateIrticket_cpp.e_ticket2.to_string());
	}

	if(po_IRCreateIrticket_cpp.refund2.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_refund2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_refund2, po_IRCreateIrticket_cpp.refund2.to_string());
	}

	if(po_IRCreateIrticket_cpp.pax_id3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_pax_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_pax_id3 = po_IRCreateIrticket_cpp.pax_id3.to_long();
	}

	if(po_IRCreateIrticket_cpp.line_number3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_line_number3 = po_IRCreateIrticket_cpp.line_number3.to_long();
	}

	if(po_IRCreateIrticket_cpp.cost_line3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_cost_line3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_cost_line3 = po_IRCreateIrticket_cpp.cost_line3.to_long();
	}

	if(po_IRCreateIrticket_cpp.reference3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_reference3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_reference3, po_IRCreateIrticket_cpp.reference3.to_string());
	}

	if(po_IRCreateIrticket_cpp.sup_id3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_sup_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_sup_id3, po_IRCreateIrticket_cpp.sup_id3.to_string());
	}

	if(po_IRCreateIrticket_cpp.cur_id3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_cur_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_cur_id3, po_IRCreateIrticket_cpp.cur_id3.to_string());
	}

	if(po_IRCreateIrticket_cpp.form3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_form3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_form3, po_IRCreateIrticket_cpp.form3.to_string());
	}

	if(po_IRCreateIrticket_cpp.ticket_no3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_ticket_no3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_ticket_no3, po_IRCreateIrticket_cpp.ticket_no3.to_string());
	}

	if(po_IRCreateIrticket_cpp.pic3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_pic3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_pic3, po_IRCreateIrticket_cpp.pic3.to_string());
	}

	if(po_IRCreateIrticket_cpp.orig_ticket_no3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_orig_ticket_no3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_orig_ticket_no3, po_IRCreateIrticket_cpp.orig_ticket_no3.to_string());
	}

	if(po_IRCreateIrticket_cpp.comm_rate3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_comm_rate3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_comm_rate3 = po_IRCreateIrticket_cpp.comm_rate3.to_double();
	}

	if(po_IRCreateIrticket_cpp.commission3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_commission3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_commission3 = po_IRCreateIrticket_cpp.commission3.to_double();
	}

	if(po_IRCreateIrticket_cpp.no_of_books3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_no_of_books3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_no_of_books3, po_IRCreateIrticket_cpp.no_of_books3.to_string());
	}

	if(po_IRCreateIrticket_cpp.exchange_code3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_exchange_code3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_exchange_code3, po_IRCreateIrticket_cpp.exchange_code3.to_string());
	}

	if(po_IRCreateIrticket_cpp.penalty_ch_fee3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_penalty_ch_fee3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_penalty_ch_fee3 = po_IRCreateIrticket_cpp.penalty_ch_fee3.to_double();
	}

	if(po_IRCreateIrticket_cpp.comm_penalty3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_comm_penalty3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_comm_penalty3 = po_IRCreateIrticket_cpp.comm_penalty3.to_double();
	}

	if(po_IRCreateIrticket_cpp.base_cost3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_base_cost3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_base_cost3 = po_IRCreateIrticket_cpp.base_cost3.to_double();
	}

	if(po_IRCreateIrticket_cpp.saving_code3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_saving_code3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_saving_code3, po_IRCreateIrticket_cpp.saving_code3.to_string());
	}

	if(po_IRCreateIrticket_cpp.full_fare3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_full_fare3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_full_fare3 = po_IRCreateIrticket_cpp.full_fare3.to_double();
	}

	if(po_IRCreateIrticket_cpp.low_prac_fare3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_low_prac_fare3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_low_prac_fare3 = po_IRCreateIrticket_cpp.low_prac_fare3.to_double();
	}

	if(po_IRCreateIrticket_cpp.personal_crs3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_personal_crs3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_personal_crs3, po_IRCreateIrticket_cpp.personal_crs3.to_string());
	}

	if(po_IRCreateIrticket_cpp.stp_home_city3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_stp_home_city3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_stp_home_city3, po_IRCreateIrticket_cpp.stp_home_city3.to_string());
	}

	if(po_IRCreateIrticket_cpp.stp_host_city3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_stp_host_city3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_stp_host_city3, po_IRCreateIrticket_cpp.stp_host_city3.to_string());
	}

	if(po_IRCreateIrticket_cpp.sel_access3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_sel_access3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_sel_access3, po_IRCreateIrticket_cpp.sel_access3.to_string());
	}

	if(po_IRCreateIrticket_cpp.crs_tour3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_crs_tour3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_crs_tour3, po_IRCreateIrticket_cpp.crs_tour3.to_string());
	}

	if(po_IRCreateIrticket_cpp.rebate3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_rebate3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrticket.c_rebate3 = po_IRCreateIrticket_cpp.rebate3.to_double();
	}

	if(po_IRCreateIrticket_cpp.doc_type3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_doc_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_doc_type3, po_IRCreateIrticket_cpp.doc_type3.to_string());
	}

	if(po_IRCreateIrticket_cpp.e_ticket3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_e_ticket3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_e_ticket3, po_IRCreateIrticket_cpp.e_ticket3.to_string());
	}

	if(po_IRCreateIrticket_cpp.refund3.NotIsNull()) {
		ps_IRCreateIrticketInd.ind_refund3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrticket.c_refund3, po_IRCreateIrticket_cpp.refund3.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrtripheader(SCIRCreateIrtripheader *ps_IRCreateIrtripheader,
          SCIRCreateIrtripheaderInd *ps_IRCreateIrtripheaderInd);
}

//Constructor
CIRCreateIrtripheader::CIRCreateIrtripheader() :
	session_id(8),
	interface_type(3),
	iata_no(8),
	cop_id(2),
	bra_id(4),
	crs_id(3),
	cfa_type(1),
	cfa_code(3),
	cfa_id(15),
	con_id(3),
	iata_dest(2),
	city_id(4),
	pnr_date(7),
	pnr_time(8),
	start_date(7),
	cfa_dpt(40),
	mktg_list(1),
	reference(40),
	booking_no(40),
	employee_number(10),
	booking_pcc(10),
	booking_gds_agent(10),
	ticketing_pcc(10),
	ticketing_gds_agent(10),
	record_creation_date(7),
	record_creation_time(8)
{
}

void
CDIR::createIrtripheader(CIRCreateIrtripheader &po_IRCreateIrtripheader_cpp)
{
	SCIRCreateIrtripheader ls_IRCreateIrtripheader;
	SCIRCreateIrtripheaderInd ls_IRCreateIrtripheaderInd;

	memset(&ls_IRCreateIrtripheader, 0, sizeof(SCIRCreateIrtripheader));
	memset(&ls_IRCreateIrtripheaderInd, -1, sizeof(SCIRCreateIrtripheaderInd));

	createIrtripheader_appToDb(ls_IRCreateIrtripheader, ls_IRCreateIrtripheaderInd,
			po_IRCreateIrtripheader_cpp);

	logTimer("B:IRCreateIrtripheader", NULL);
	if(::dbIRCreateIrtripheader(&ls_IRCreateIrtripheader, &ls_IRCreateIrtripheaderInd) != 0) {
		logTimer("A:IRCreateIrtripheader (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrtripheader", NULL);

	createIrtripheader_dbToApp(ls_IRCreateIrtripheader, ls_IRCreateIrtripheaderInd,
			po_IRCreateIrtripheader_cpp);
}

// C to C++ assignments:
void
CDIR::createIrtripheader_dbToApp(SCIRCreateIrtripheader &ps_IRCreateIrtripheader,
		SCIRCreateIrtripheaderInd &ps_IRCreateIrtripheaderInd,
		CIRCreateIrtripheader &po_IRCreateIrtripheader_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrtripheader_appToDb(SCIRCreateIrtripheader &ps_IRCreateIrtripheader,
		SCIRCreateIrtripheaderInd &ps_IRCreateIrtripheaderInd,
		CIRCreateIrtripheader &po_IRCreateIrtripheader_cpp)
{
	int ii;

	if(po_IRCreateIrtripheader_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_session_id, po_IRCreateIrtripheader_cpp.session_id.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.interface_type.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_interface_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_interface_type, po_IRCreateIrtripheader_cpp.interface_type.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.creation_date.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_creation_date = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtripheader.c_creation_date = po_IRCreateIrtripheader_cpp.creation_date.toCSDateTime();
	}

	if(po_IRCreateIrtripheader_cpp.iata_no.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_iata_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_iata_no, po_IRCreateIrtripheader_cpp.iata_no.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.cop_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_cop_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_cop_id, po_IRCreateIrtripheader_cpp.cop_id.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.bra_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_bra_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_bra_id, po_IRCreateIrtripheader_cpp.bra_id.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.crs_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_crs_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_crs_id, po_IRCreateIrtripheader_cpp.crs_id.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.cfa_type.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_cfa_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_cfa_type, po_IRCreateIrtripheader_cpp.cfa_type.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.cfa_code.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_cfa_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_cfa_code, po_IRCreateIrtripheader_cpp.cfa_code.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.cfa_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_cfa_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_cfa_id, po_IRCreateIrtripheader_cpp.cfa_id.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.con_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_con_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_con_id, po_IRCreateIrtripheader_cpp.con_id.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.iata_dest.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_iata_dest = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_iata_dest, po_IRCreateIrtripheader_cpp.iata_dest.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.city_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_city_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_city_id, po_IRCreateIrtripheader_cpp.city_id.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.pnr_date.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_pnr_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_pnr_date, po_IRCreateIrtripheader_cpp.pnr_date.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.pnr_time.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_pnr_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_pnr_time, po_IRCreateIrtripheader_cpp.pnr_time.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.start_date.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_start_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_start_date, po_IRCreateIrtripheader_cpp.start_date.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.cfa_dpt.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_cfa_dpt = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_cfa_dpt, po_IRCreateIrtripheader_cpp.cfa_dpt.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.mktg_list.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_mktg_list = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_mktg_list, po_IRCreateIrtripheader_cpp.mktg_list.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.reference.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_reference, po_IRCreateIrtripheader_cpp.reference.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.booking_no.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_booking_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_booking_no, po_IRCreateIrtripheader_cpp.booking_no.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.ticketer_user_id.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_ticketer_user_id = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrtripheader.c_ticketer_user_id = po_IRCreateIrtripheader_cpp.ticketer_user_id.to_long();
	}

	if(po_IRCreateIrtripheader_cpp.employee_number.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_employee_number = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_employee_number, po_IRCreateIrtripheader_cpp.employee_number.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.booking_pcc.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_booking_pcc = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_booking_pcc, po_IRCreateIrtripheader_cpp.booking_pcc.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.booking_gds_agent.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_booking_gds_agent = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_booking_gds_agent, po_IRCreateIrtripheader_cpp.booking_gds_agent.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.ticketing_pcc.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_ticketing_pcc = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_ticketing_pcc, po_IRCreateIrtripheader_cpp.ticketing_pcc.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.ticketing_gds_agent.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_ticketing_gds_agent = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_ticketing_gds_agent, po_IRCreateIrtripheader_cpp.ticketing_gds_agent.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.record_creation_date.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_record_creation_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_record_creation_date, po_IRCreateIrtripheader_cpp.record_creation_date.to_string());
	}

	if(po_IRCreateIrtripheader_cpp.record_creation_time.NotIsNull()) {
		ps_IRCreateIrtripheaderInd.ind_record_creation_time = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrtripheader.c_record_creation_time, po_IRCreateIrtripheader_cpp.record_creation_time.to_string());
	}

}

extern "C" {
extern int
dbIRGetSupServiceType(SCIRGetSupServiceType *ps_IRGetSupServiceType,
          SCIRGetSupServiceTypeInd *ps_IRGetSupServiceTypeInd);
}

//Constructor
CIRGetSupServiceType::CIRGetSupServiceType() :
	pc_sup_id(3),
	pc_service_type(1)
{
}

void
CDIR::getSupServiceType(CIRGetSupServiceType &po_IRGetSupServiceType_cpp)
{
	SCIRGetSupServiceType ls_IRGetSupServiceType;
	SCIRGetSupServiceTypeInd ls_IRGetSupServiceTypeInd;

	memset(&ls_IRGetSupServiceType, 0, sizeof(SCIRGetSupServiceType));
	memset(&ls_IRGetSupServiceTypeInd, -1, sizeof(SCIRGetSupServiceTypeInd));

	getSupServiceType_appToDb(ls_IRGetSupServiceType, ls_IRGetSupServiceTypeInd,
			po_IRGetSupServiceType_cpp);

	logTimer("B:IRGetSupServiceType", NULL);
	if(::dbIRGetSupServiceType(&ls_IRGetSupServiceType, &ls_IRGetSupServiceTypeInd) != 0) {
		logTimer("A:IRGetSupServiceType (FAILED)", NULL);
		return;
	}
	logTimer("A:IRGetSupServiceType", NULL);

	getSupServiceType_dbToApp(ls_IRGetSupServiceType, ls_IRGetSupServiceTypeInd,
			po_IRGetSupServiceType_cpp);
}

// C to C++ assignments:
void
CDIR::getSupServiceType_dbToApp(SCIRGetSupServiceType &ps_IRGetSupServiceType,
		SCIRGetSupServiceTypeInd &ps_IRGetSupServiceTypeInd,
		CIRGetSupServiceType &po_IRGetSupServiceType_cpp)
{
	int ii;

	if(ps_IRGetSupServiceTypeInd.ind_pc_service_type
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRGetSupServiceType_cpp.pc_service_type = ps_IRGetSupServiceType.c_pc_service_type;
	}

}

// C++ to C assignments:
void
CDIR::getSupServiceType_appToDb(SCIRGetSupServiceType &ps_IRGetSupServiceType,
		SCIRGetSupServiceTypeInd &ps_IRGetSupServiceTypeInd,
		CIRGetSupServiceType &po_IRGetSupServiceType_cpp)
{
	int ii;

	if(po_IRGetSupServiceType_cpp.pc_sup_id.NotIsNull()) {
		ps_IRGetSupServiceTypeInd.ind_pc_sup_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetSupServiceType.c_pc_sup_id, po_IRGetSupServiceType_cpp.pc_sup_id.to_string());
	}

	if(po_IRGetSupServiceType_cpp.pc_service_type.NotIsNull()) {
		ps_IRGetSupServiceTypeInd.ind_pc_service_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetSupServiceType.c_pc_service_type, po_IRGetSupServiceType_cpp.pc_service_type.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrdistribution(SCIRCreateIrdistribution *ps_IRCreateIrdistribution,
          SCIRCreateIrdistributionInd *ps_IRCreateIrdistributionInd);
}

//Constructor
CIRCreateIrdistribution::CIRCreateIrdistribution() :
	session_id(8),
	acc_id_1(6),
	amt_1(13, 2),
	acc_id_2(6),
	amt_2(13, 2),
	acc_id_3(6),
	amt_3(13, 2),
	acc_id_4(6),
	amt_4(13, 2),
	acc_id_5(6),
	amt_5(13, 2),
	acc_id_6(6),
	amt_6(13, 2)
{
}

void
CDIR::createIrdistribution(CIRCreateIrdistribution &po_IRCreateIrdistribution_cpp)
{
	SCIRCreateIrdistribution ls_IRCreateIrdistribution;
	SCIRCreateIrdistributionInd ls_IRCreateIrdistributionInd;

	memset(&ls_IRCreateIrdistribution, 0, sizeof(SCIRCreateIrdistribution));
	memset(&ls_IRCreateIrdistributionInd, -1, sizeof(SCIRCreateIrdistributionInd));

	createIrdistribution_appToDb(ls_IRCreateIrdistribution, ls_IRCreateIrdistributionInd,
			po_IRCreateIrdistribution_cpp);

	logTimer("B:IRCreateIrdistribution", NULL);
	if(::dbIRCreateIrdistribution(&ls_IRCreateIrdistribution, &ls_IRCreateIrdistributionInd) != 0) {
		logTimer("A:IRCreateIrdistribution (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrdistribution", NULL);

	createIrdistribution_dbToApp(ls_IRCreateIrdistribution, ls_IRCreateIrdistributionInd,
			po_IRCreateIrdistribution_cpp);
}

// C to C++ assignments:
void
CDIR::createIrdistribution_dbToApp(SCIRCreateIrdistribution &ps_IRCreateIrdistribution,
		SCIRCreateIrdistributionInd &ps_IRCreateIrdistributionInd,
		CIRCreateIrdistribution &po_IRCreateIrdistribution_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrdistribution_appToDb(SCIRCreateIrdistribution &ps_IRCreateIrdistribution,
		SCIRCreateIrdistributionInd &ps_IRCreateIrdistributionInd,
		CIRCreateIrdistribution &po_IRCreateIrdistribution_cpp)
{
	int ii;

	if(po_IRCreateIrdistribution_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrdistribution.c_session_id, po_IRCreateIrdistribution_cpp.session_id.to_string());
	}

	if(po_IRCreateIrdistribution_cpp.line_number_1.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_line_number_1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_line_number_1 = po_IRCreateIrdistribution_cpp.line_number_1.to_long();
	}

	if(po_IRCreateIrdistribution_cpp.acc_id_1.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_acc_id_1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrdistribution.c_acc_id_1, po_IRCreateIrdistribution_cpp.acc_id_1.to_string());
	}

	if(po_IRCreateIrdistribution_cpp.amt_1.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_amt_1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_amt_1 = po_IRCreateIrdistribution_cpp.amt_1.to_double();
	}

	if(po_IRCreateIrdistribution_cpp.line_number_2.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_line_number_2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_line_number_2 = po_IRCreateIrdistribution_cpp.line_number_2.to_long();
	}

	if(po_IRCreateIrdistribution_cpp.acc_id_2.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_acc_id_2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrdistribution.c_acc_id_2, po_IRCreateIrdistribution_cpp.acc_id_2.to_string());
	}

	if(po_IRCreateIrdistribution_cpp.amt_2.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_amt_2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_amt_2 = po_IRCreateIrdistribution_cpp.amt_2.to_double();
	}

	if(po_IRCreateIrdistribution_cpp.line_number_3.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_line_number_3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_line_number_3 = po_IRCreateIrdistribution_cpp.line_number_3.to_long();
	}

	if(po_IRCreateIrdistribution_cpp.acc_id_3.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_acc_id_3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrdistribution.c_acc_id_3, po_IRCreateIrdistribution_cpp.acc_id_3.to_string());
	}

	if(po_IRCreateIrdistribution_cpp.amt_3.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_amt_3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_amt_3 = po_IRCreateIrdistribution_cpp.amt_3.to_double();
	}

	if(po_IRCreateIrdistribution_cpp.line_number_4.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_line_number_4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_line_number_4 = po_IRCreateIrdistribution_cpp.line_number_4.to_long();
	}

	if(po_IRCreateIrdistribution_cpp.acc_id_4.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_acc_id_4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrdistribution.c_acc_id_4, po_IRCreateIrdistribution_cpp.acc_id_4.to_string());
	}

	if(po_IRCreateIrdistribution_cpp.amt_4.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_amt_4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_amt_4 = po_IRCreateIrdistribution_cpp.amt_4.to_double();
	}

	if(po_IRCreateIrdistribution_cpp.line_number_5.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_line_number_5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_line_number_5 = po_IRCreateIrdistribution_cpp.line_number_5.to_long();
	}

	if(po_IRCreateIrdistribution_cpp.acc_id_5.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_acc_id_5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrdistribution.c_acc_id_5, po_IRCreateIrdistribution_cpp.acc_id_5.to_string());
	}

	if(po_IRCreateIrdistribution_cpp.amt_5.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_amt_5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_amt_5 = po_IRCreateIrdistribution_cpp.amt_5.to_double();
	}

	if(po_IRCreateIrdistribution_cpp.line_number_6.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_line_number_6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_line_number_6 = po_IRCreateIrdistribution_cpp.line_number_6.to_long();
	}

	if(po_IRCreateIrdistribution_cpp.acc_id_6.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_acc_id_6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrdistribution.c_acc_id_6, po_IRCreateIrdistribution_cpp.acc_id_6.to_string());
	}

	if(po_IRCreateIrdistribution_cpp.amt_6.NotIsNull()) {
		ps_IRCreateIrdistributionInd.ind_amt_6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrdistribution.c_amt_6 = po_IRCreateIrdistribution_cpp.amt_6.to_double();
	}

}

extern "C" {
extern int
dbIRValidateSetFixed(SCIRValidateSetFixed *ps_IRValidateSetFixed,
          SCIRValidateSetFixedInd *ps_IRValidateSetFixedInd);
}

//Constructor
CIRValidateSetFixed::CIRValidateSetFixed() :
	pc_session_id(8),
	pc_iata(8),
	pc_company(2),
	pc_branch(4),
	pc_consultant(3),
	pc_mml_ind(1),
	pc_cfa_type(1),
	pv_process_name(3),
	pc_braMktList(1),
	pc_tktBranch(4),
	pc_city(4),
	pc_rejectCon(3),
	pc_rejectCop(2),
	pc_rejectBra(4),
	pv_rejectirSub(3)
{
}

void
CDIR::validateSetFixed(CIRValidateSetFixed &po_IRValidateSetFixed_cpp)
{
	SCIRValidateSetFixed ls_IRValidateSetFixed;
	SCIRValidateSetFixedInd ls_IRValidateSetFixedInd;

	memset(&ls_IRValidateSetFixed, 0, sizeof(SCIRValidateSetFixed));
	memset(&ls_IRValidateSetFixedInd, -1, sizeof(SCIRValidateSetFixedInd));

	validateSetFixed_appToDb(ls_IRValidateSetFixed, ls_IRValidateSetFixedInd,
			po_IRValidateSetFixed_cpp);

	logTimer("B:IRValidateSetFixed", NULL);
	if(::dbIRValidateSetFixed(&ls_IRValidateSetFixed, &ls_IRValidateSetFixedInd) != 0) {
		logTimer("A:IRValidateSetFixed (FAILED)", NULL);
		return;
	}
	logTimer("A:IRValidateSetFixed", NULL);

	validateSetFixed_dbToApp(ls_IRValidateSetFixed, ls_IRValidateSetFixedInd,
			po_IRValidateSetFixed_cpp);
}

// C to C++ assignments:
void
CDIR::validateSetFixed_dbToApp(SCIRValidateSetFixed &ps_IRValidateSetFixed,
		SCIRValidateSetFixedInd &ps_IRValidateSetFixedInd,
		CIRValidateSetFixed &po_IRValidateSetFixed_cpp)
{
	int ii;

	if(ps_IRValidateSetFixedInd.ind_pc_braMktList
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pc_braMktList = ps_IRValidateSetFixed.c_pc_braMktList;
	}

	if(ps_IRValidateSetFixedInd.ind_pc_tktBranch
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pc_tktBranch = ps_IRValidateSetFixed.c_pc_tktBranch;
	}

	if(ps_IRValidateSetFixedInd.ind_pc_city
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pc_city = ps_IRValidateSetFixed.c_pc_city;
	}

	if(ps_IRValidateSetFixedInd.ind_lcl_date_time
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.lcl_date_time.fromCSDateTime(ps_IRValidateSetFixed.c_lcl_date_time);
	}

	if(ps_IRValidateSetFixedInd.ind_pc_rejectCon
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pc_rejectCon = ps_IRValidateSetFixed.c_pc_rejectCon;
	}

	if(ps_IRValidateSetFixedInd.ind_pc_rejectCop
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pc_rejectCop = ps_IRValidateSetFixed.c_pc_rejectCop;
	}

	if(ps_IRValidateSetFixedInd.ind_pc_rejectBra
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pc_rejectBra = ps_IRValidateSetFixed.c_pc_rejectBra;
	}

	if(ps_IRValidateSetFixedInd.ind_pi_rejectirId
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pi_rejectirId = ps_IRValidateSetFixed.c_pi_rejectirId;
	}

	if(ps_IRValidateSetFixedInd.ind_pv_rejectirSub
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValidateSetFixed_cpp.pv_rejectirSub = ps_IRValidateSetFixed.c_pv_rejectirSub;
	}

}

// C++ to C assignments:
void
CDIR::validateSetFixed_appToDb(SCIRValidateSetFixed &ps_IRValidateSetFixed,
		SCIRValidateSetFixedInd &ps_IRValidateSetFixedInd,
		CIRValidateSetFixed &po_IRValidateSetFixed_cpp)
{
	int ii;

	if(po_IRValidateSetFixed_cpp.pc_session_id.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_session_id, po_IRValidateSetFixed_cpp.pc_session_id.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_iata.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_iata = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_iata, po_IRValidateSetFixed_cpp.pc_iata.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_company.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_company = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_company, po_IRValidateSetFixed_cpp.pc_company.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_branch.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_branch = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_branch, po_IRValidateSetFixed_cpp.pc_branch.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_consultant.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_consultant = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_consultant, po_IRValidateSetFixed_cpp.pc_consultant.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_mml_ind.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_mml_ind = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_mml_ind, po_IRValidateSetFixed_cpp.pc_mml_ind.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_cfa_type.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_cfa_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_cfa_type, po_IRValidateSetFixed_cpp.pc_cfa_type.to_string());
	}

	if(po_IRValidateSetFixed_cpp.gmt_date_time.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_gmt_date_time = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRValidateSetFixed.c_gmt_date_time = po_IRValidateSetFixed_cpp.gmt_date_time.toCSDateTime();
	}

	if(po_IRValidateSetFixed_cpp.pv_process_name.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pv_process_name = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pv_process_name, po_IRValidateSetFixed_cpp.pv_process_name.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_braMktList.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_braMktList = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_braMktList, po_IRValidateSetFixed_cpp.pc_braMktList.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_tktBranch.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_tktBranch = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_tktBranch, po_IRValidateSetFixed_cpp.pc_tktBranch.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_city.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_city = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_city, po_IRValidateSetFixed_cpp.pc_city.to_string());
	}

	if(po_IRValidateSetFixed_cpp.lcl_date_time.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_lcl_date_time = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRValidateSetFixed.c_lcl_date_time = po_IRValidateSetFixed_cpp.lcl_date_time.toCSDateTime();
	}

	if(po_IRValidateSetFixed_cpp.pc_rejectCon.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_rejectCon = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_rejectCon, po_IRValidateSetFixed_cpp.pc_rejectCon.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_rejectCop.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_rejectCop = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_rejectCop, po_IRValidateSetFixed_cpp.pc_rejectCop.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pc_rejectBra.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pc_rejectBra = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pc_rejectBra, po_IRValidateSetFixed_cpp.pc_rejectBra.to_string());
	}

	if(po_IRValidateSetFixed_cpp.pi_rejectirId.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pi_rejectirId = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRValidateSetFixed.c_pi_rejectirId = po_IRValidateSetFixed_cpp.pi_rejectirId.to_long();
	}

	if(po_IRValidateSetFixed_cpp.pv_rejectirSub.NotIsNull()) {
		ps_IRValidateSetFixedInd.ind_pv_rejectirSub = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValidateSetFixed.c_pv_rejectirSub, po_IRValidateSetFixed_cpp.pv_rejectirSub.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrreceipt(SCIRCreateIrreceipt *ps_IRCreateIrreceipt,
          SCIRCreateIrreceiptInd *ps_IRCreateIrreceiptInd);
}

//Constructor
CIRCreateIrreceipt::CIRCreateIrreceipt() :
	session_id(8),
	cop_id(2),
	bra_id(4),
	name(60),
	total_amt(13, 2),
	bank_id(6),
	form_of_pay(2),
	cc_id(2),
	cc_no(30),
	cc_exp_date(4),
	cc_auth_no(20),
	acc_id1(6),
	amt1(13, 2),
	acc_id2(6),
	amt2(13, 2),
	acc_id3(6),
	amt3(13, 2),
	rec_valid(1),
	lang_id(1),
	link_to(1),
	payment_reference(30)
{
}

void
CDIR::createIrreceipt(CIRCreateIrreceipt &po_IRCreateIrreceipt_cpp)
{
	SCIRCreateIrreceipt ls_IRCreateIrreceipt;
	SCIRCreateIrreceiptInd ls_IRCreateIrreceiptInd;

	memset(&ls_IRCreateIrreceipt, 0, sizeof(SCIRCreateIrreceipt));
	memset(&ls_IRCreateIrreceiptInd, -1, sizeof(SCIRCreateIrreceiptInd));

	createIrreceipt_appToDb(ls_IRCreateIrreceipt, ls_IRCreateIrreceiptInd,
			po_IRCreateIrreceipt_cpp);

	logTimer("B:IRCreateIrreceipt", NULL);
	if(::dbIRCreateIrreceipt(&ls_IRCreateIrreceipt, &ls_IRCreateIrreceiptInd) != 0) {
		logTimer("A:IRCreateIrreceipt (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrreceipt", NULL);

	createIrreceipt_dbToApp(ls_IRCreateIrreceipt, ls_IRCreateIrreceiptInd,
			po_IRCreateIrreceipt_cpp);
}

// C to C++ assignments:
void
CDIR::createIrreceipt_dbToApp(SCIRCreateIrreceipt &ps_IRCreateIrreceipt,
		SCIRCreateIrreceiptInd &ps_IRCreateIrreceiptInd,
		CIRCreateIrreceipt &po_IRCreateIrreceipt_cpp)
{
	int ii;

	if(ps_IRCreateIrreceiptInd.ind_rec_valid
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRCreateIrreceipt_cpp.rec_valid = ps_IRCreateIrreceipt.c_rec_valid;
	}

	if(ps_IRCreateIrreceiptInd.ind_lang_id
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRCreateIrreceipt_cpp.lang_id = ps_IRCreateIrreceipt.c_lang_id;
	}

}

// C++ to C assignments:
void
CDIR::createIrreceipt_appToDb(SCIRCreateIrreceipt &ps_IRCreateIrreceipt,
		SCIRCreateIrreceiptInd &ps_IRCreateIrreceiptInd,
		CIRCreateIrreceipt &po_IRCreateIrreceipt_cpp)
{
	int ii;

	if(po_IRCreateIrreceipt_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_session_id, po_IRCreateIrreceipt_cpp.session_id.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.line_number.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_line_number = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrreceipt.c_line_number = po_IRCreateIrreceipt_cpp.line_number.to_long();
	}

	if(po_IRCreateIrreceipt_cpp.cop_id.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_cop_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_cop_id, po_IRCreateIrreceipt_cpp.cop_id.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.bra_id.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_bra_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_bra_id, po_IRCreateIrreceipt_cpp.bra_id.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.name.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_name = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_name, po_IRCreateIrreceipt_cpp.name.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.total_amt.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_total_amt = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrreceipt.c_total_amt = po_IRCreateIrreceipt_cpp.total_amt.to_double();
	}

	if(po_IRCreateIrreceipt_cpp.bank_id.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_bank_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_bank_id, po_IRCreateIrreceipt_cpp.bank_id.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.form_of_pay.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_form_of_pay = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_form_of_pay, po_IRCreateIrreceipt_cpp.form_of_pay.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.cc_id.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_cc_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_cc_id, po_IRCreateIrreceipt_cpp.cc_id.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.cc_no.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_cc_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_cc_no, po_IRCreateIrreceipt_cpp.cc_no.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.cc_exp_date.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_cc_exp_date = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_cc_exp_date, po_IRCreateIrreceipt_cpp.cc_exp_date.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.cc_auth_no.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_cc_auth_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_cc_auth_no, po_IRCreateIrreceipt_cpp.cc_auth_no.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.acc_id1.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_acc_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_acc_id1, po_IRCreateIrreceipt_cpp.acc_id1.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.amt1.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_amt1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrreceipt.c_amt1 = po_IRCreateIrreceipt_cpp.amt1.to_double();
	}

	if(po_IRCreateIrreceipt_cpp.acc_id2.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_acc_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_acc_id2, po_IRCreateIrreceipt_cpp.acc_id2.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.amt2.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_amt2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrreceipt.c_amt2 = po_IRCreateIrreceipt_cpp.amt2.to_double();
	}

	if(po_IRCreateIrreceipt_cpp.acc_id3.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_acc_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_acc_id3, po_IRCreateIrreceipt_cpp.acc_id3.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.amt3.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_amt3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrreceipt.c_amt3 = po_IRCreateIrreceipt_cpp.amt3.to_double();
	}

	if(po_IRCreateIrreceipt_cpp.link_to.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_link_to = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_link_to, po_IRCreateIrreceipt_cpp.link_to.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.rec_valid.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_rec_valid = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_rec_valid, po_IRCreateIrreceipt_cpp.rec_valid.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.lang_id.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_lang_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_lang_id, po_IRCreateIrreceipt_cpp.lang_id.to_string());
	}

	if(po_IRCreateIrreceipt_cpp.payment_reference.NotIsNull()) {
		ps_IRCreateIrreceiptInd.ind_payment_reference = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrreceipt.c_payment_reference, po_IRCreateIrreceipt_cpp.payment_reference.to_string());
	}

}

extern "C" {
extern int
dbIRMoveQueue(SCIRMoveQueue *ps_IRMoveQueue,
          SCIRMoveQueueInd *ps_IRMoveQueueInd);
}

//Constructor
CIRMoveQueue::CIRMoveQueue() :
	pc_session_id(8),
	pc_queue_type(3),
	pc_rejectCop(2),
	pc_rejectBra(4),
	pc_rejectCon(3)
{
}

void
CDIR::moveQueue(CIRMoveQueue &po_IRMoveQueue_cpp)
{
	SCIRMoveQueue ls_IRMoveQueue;
	SCIRMoveQueueInd ls_IRMoveQueueInd;

	memset(&ls_IRMoveQueue, 0, sizeof(SCIRMoveQueue));
	memset(&ls_IRMoveQueueInd, -1, sizeof(SCIRMoveQueueInd));

	moveQueue_appToDb(ls_IRMoveQueue, ls_IRMoveQueueInd,
			po_IRMoveQueue_cpp);

	logTimer("B:IRMoveQueue", NULL);
	if(::dbIRMoveQueue(&ls_IRMoveQueue, &ls_IRMoveQueueInd) != 0) {
		logTimer("A:IRMoveQueue (FAILED)", NULL);
		return;
	}
	logTimer("A:IRMoveQueue", NULL);

	moveQueue_dbToApp(ls_IRMoveQueue, ls_IRMoveQueueInd,
			po_IRMoveQueue_cpp);
}

// C to C++ assignments:
void
CDIR::moveQueue_dbToApp(SCIRMoveQueue &ps_IRMoveQueue,
		SCIRMoveQueueInd &ps_IRMoveQueueInd,
		CIRMoveQueue &po_IRMoveQueue_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::moveQueue_appToDb(SCIRMoveQueue &ps_IRMoveQueue,
		SCIRMoveQueueInd &ps_IRMoveQueueInd,
		CIRMoveQueue &po_IRMoveQueue_cpp)
{
	int ii;

	if(po_IRMoveQueue_cpp.pc_session_id.NotIsNull()) {
		ps_IRMoveQueueInd.ind_pc_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRMoveQueue.c_pc_session_id, po_IRMoveQueue_cpp.pc_session_id.to_string());
	}

	if(po_IRMoveQueue_cpp.pc_queue_type.NotIsNull()) {
		ps_IRMoveQueueInd.ind_pc_queue_type = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRMoveQueue.c_pc_queue_type, po_IRMoveQueue_cpp.pc_queue_type.to_string());
	}

	if(po_IRMoveQueue_cpp.pc_rejectCop.NotIsNull()) {
		ps_IRMoveQueueInd.ind_pc_rejectCop = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRMoveQueue.c_pc_rejectCop, po_IRMoveQueue_cpp.pc_rejectCop.to_string());
	}

	if(po_IRMoveQueue_cpp.pc_rejectBra.NotIsNull()) {
		ps_IRMoveQueueInd.ind_pc_rejectBra = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRMoveQueue.c_pc_rejectBra, po_IRMoveQueue_cpp.pc_rejectBra.to_string());
	}

	if(po_IRMoveQueue_cpp.pc_rejectCon.NotIsNull()) {
		ps_IRMoveQueueInd.ind_pc_rejectCon = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRMoveQueue.c_pc_rejectCon, po_IRMoveQueue_cpp.pc_rejectCon.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIremailaddress(SCIRCreateIremailaddress *ps_IRCreateIremailaddress,
          SCIRCreateIremailaddressInd *ps_IRCreateIremailaddressInd);
}

//Constructor
CIRCreateIremailaddress::CIRCreateIremailaddress() :
	session_id(8),
	email_id1(255),
	email_type1(1),
	email_id2(255),
	email_type2(1),
	email_id3(255),
	email_type3(1),
	email_id4(255),
	email_type4(1),
	email_id5(255),
	email_type5(1),
	email_id6(255),
	email_type6(1)
{
}

void
CDIR::createIremailaddress(CIRCreateIremailaddress &po_IRCreateIremailaddress_cpp)
{
	SCIRCreateIremailaddress ls_IRCreateIremailaddress;
	SCIRCreateIremailaddressInd ls_IRCreateIremailaddressInd;

	memset(&ls_IRCreateIremailaddress, 0, sizeof(SCIRCreateIremailaddress));
	memset(&ls_IRCreateIremailaddressInd, -1, sizeof(SCIRCreateIremailaddressInd));

	createIremailaddress_appToDb(ls_IRCreateIremailaddress, ls_IRCreateIremailaddressInd,
			po_IRCreateIremailaddress_cpp);

	logTimer("B:IRCreateIremailaddress", NULL);
	if(::dbIRCreateIremailaddress(&ls_IRCreateIremailaddress, &ls_IRCreateIremailaddressInd) != 0) {
		logTimer("A:IRCreateIremailaddress (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIremailaddress", NULL);

	createIremailaddress_dbToApp(ls_IRCreateIremailaddress, ls_IRCreateIremailaddressInd,
			po_IRCreateIremailaddress_cpp);
}

// C to C++ assignments:
void
CDIR::createIremailaddress_dbToApp(SCIRCreateIremailaddress &ps_IRCreateIremailaddress,
		SCIRCreateIremailaddressInd &ps_IRCreateIremailaddressInd,
		CIRCreateIremailaddress &po_IRCreateIremailaddress_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIremailaddress_appToDb(SCIRCreateIremailaddress &ps_IRCreateIremailaddress,
		SCIRCreateIremailaddressInd &ps_IRCreateIremailaddressInd,
		CIRCreateIremailaddress &po_IRCreateIremailaddress_cpp)
{
	int ii;

	if(po_IRCreateIremailaddress_cpp.session_id.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_session_id, po_IRCreateIremailaddress_cpp.session_id.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_id1.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_id1, po_IRCreateIremailaddress_cpp.email_id1.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_type1.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_type1, po_IRCreateIremailaddress_cpp.email_type1.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_id2.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_id2, po_IRCreateIremailaddress_cpp.email_id2.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_type2.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_type2, po_IRCreateIremailaddress_cpp.email_type2.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_id3.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_id3, po_IRCreateIremailaddress_cpp.email_id3.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_type3.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_type3, po_IRCreateIremailaddress_cpp.email_type3.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_id4.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_id4, po_IRCreateIremailaddress_cpp.email_id4.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_type4.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_type4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_type4, po_IRCreateIremailaddress_cpp.email_type4.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_id5.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_id5, po_IRCreateIremailaddress_cpp.email_id5.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_type5.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_type5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_type5, po_IRCreateIremailaddress_cpp.email_type5.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_id6.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_id6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_id6, po_IRCreateIremailaddress_cpp.email_id6.to_string());
	}

	if(po_IRCreateIremailaddress_cpp.email_type6.NotIsNull()) {
		ps_IRCreateIremailaddressInd.ind_email_type6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIremailaddress.c_email_type6, po_IRCreateIremailaddress_cpp.email_type6.to_string());
	}

}

extern "C" {
extern int
dbIRIsPFSSupplier(SCIRIsPFSSupplier *ps_IRIsPFSSupplier,
          SCIRIsPFSSupplierInd *ps_IRIsPFSSupplierInd);
}

//Constructor
CIRIsPFSSupplier::CIRIsPFSSupplier() :
	cop_id(2),
	sup_id(3),
	pb_is_pfs(1)
{
}

void
CDIR::isPFSSupplier(CIRIsPFSSupplier &po_IRIsPFSSupplier_cpp)
{
	SCIRIsPFSSupplier ls_IRIsPFSSupplier;
	SCIRIsPFSSupplierInd ls_IRIsPFSSupplierInd;

	memset(&ls_IRIsPFSSupplier, 0, sizeof(SCIRIsPFSSupplier));
	memset(&ls_IRIsPFSSupplierInd, -1, sizeof(SCIRIsPFSSupplierInd));

	isPFSSupplier_appToDb(ls_IRIsPFSSupplier, ls_IRIsPFSSupplierInd,
			po_IRIsPFSSupplier_cpp);

	logTimer("B:IRIsPFSSupplier", NULL);
	if(::dbIRIsPFSSupplier(&ls_IRIsPFSSupplier, &ls_IRIsPFSSupplierInd) != 0) {
		logTimer("A:IRIsPFSSupplier (FAILED)", NULL);
		return;
	}
	logTimer("A:IRIsPFSSupplier", NULL);

	isPFSSupplier_dbToApp(ls_IRIsPFSSupplier, ls_IRIsPFSSupplierInd,
			po_IRIsPFSSupplier_cpp);
}

// C to C++ assignments:
void
CDIR::isPFSSupplier_dbToApp(SCIRIsPFSSupplier &ps_IRIsPFSSupplier,
		SCIRIsPFSSupplierInd &ps_IRIsPFSSupplierInd,
		CIRIsPFSSupplier &po_IRIsPFSSupplier_cpp)
{
	int ii;

	if(ps_IRIsPFSSupplierInd.ind_pb_is_pfs
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRIsPFSSupplier_cpp.pb_is_pfs = ps_IRIsPFSSupplier.c_pb_is_pfs;
	}

}

// C++ to C assignments:
void
CDIR::isPFSSupplier_appToDb(SCIRIsPFSSupplier &ps_IRIsPFSSupplier,
		SCIRIsPFSSupplierInd &ps_IRIsPFSSupplierInd,
		CIRIsPFSSupplier &po_IRIsPFSSupplier_cpp)
{
	int ii;

	if(po_IRIsPFSSupplier_cpp.cop_id.NotIsNull()) {
		ps_IRIsPFSSupplierInd.ind_cop_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRIsPFSSupplier.c_cop_id, po_IRIsPFSSupplier_cpp.cop_id.to_string());
	}

	if(po_IRIsPFSSupplier_cpp.sup_id.NotIsNull()) {
		ps_IRIsPFSSupplierInd.ind_sup_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRIsPFSSupplier.c_sup_id, po_IRIsPFSSupplier_cpp.sup_id.to_string());
	}

	if(po_IRIsPFSSupplier_cpp.pd_lcl_date.NotIsNull()) {
		ps_IRIsPFSSupplierInd.ind_pd_lcl_date = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRIsPFSSupplier.c_pd_lcl_date = po_IRIsPFSSupplier_cpp.pd_lcl_date.toCSDateTime();
	}

	if(po_IRIsPFSSupplier_cpp.pb_is_pfs.NotIsNull()) {
		ps_IRIsPFSSupplierInd.ind_pb_is_pfs = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRIsPFSSupplier.c_pb_is_pfs, po_IRIsPFSSupplier_cpp.pb_is_pfs.to_string());
	}

}

extern "C" {
extern int
dbIRGetCopIdByIata(SCIRGetCopIdByIata *ps_IRGetCopIdByIata,
          SCIRGetCopIdByIataInd *ps_IRGetCopIdByIataInd);
}

//Constructor
CIRGetCopIdByIata::CIRGetCopIdByIata() :
	pc_iata_no(8),
	pc_cop_id(2)
{
}

void
CDIR::getCopIdByIata(CIRGetCopIdByIata &po_IRGetCopIdByIata_cpp)
{
	SCIRGetCopIdByIata ls_IRGetCopIdByIata;
	SCIRGetCopIdByIataInd ls_IRGetCopIdByIataInd;

	memset(&ls_IRGetCopIdByIata, 0, sizeof(SCIRGetCopIdByIata));
	memset(&ls_IRGetCopIdByIataInd, -1, sizeof(SCIRGetCopIdByIataInd));

	getCopIdByIata_appToDb(ls_IRGetCopIdByIata, ls_IRGetCopIdByIataInd,
			po_IRGetCopIdByIata_cpp);

	logTimer("B:IRGetCopIdByIata", NULL);
	if(::dbIRGetCopIdByIata(&ls_IRGetCopIdByIata, &ls_IRGetCopIdByIataInd) != 0) {
		logTimer("A:IRGetCopIdByIata (FAILED)", NULL);
		return;
	}
	logTimer("A:IRGetCopIdByIata", NULL);

	getCopIdByIata_dbToApp(ls_IRGetCopIdByIata, ls_IRGetCopIdByIataInd,
			po_IRGetCopIdByIata_cpp);
}

// C to C++ assignments:
void
CDIR::getCopIdByIata_dbToApp(SCIRGetCopIdByIata &ps_IRGetCopIdByIata,
		SCIRGetCopIdByIataInd &ps_IRGetCopIdByIataInd,
		CIRGetCopIdByIata &po_IRGetCopIdByIata_cpp)
{
	int ii;

	if(ps_IRGetCopIdByIataInd.ind_pc_cop_id
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRGetCopIdByIata_cpp.pc_cop_id = ps_IRGetCopIdByIata.c_pc_cop_id;
	}

}

// C++ to C assignments:
void
CDIR::getCopIdByIata_appToDb(SCIRGetCopIdByIata &ps_IRGetCopIdByIata,
		SCIRGetCopIdByIataInd &ps_IRGetCopIdByIataInd,
		CIRGetCopIdByIata &po_IRGetCopIdByIata_cpp)
{
	int ii;

	if(po_IRGetCopIdByIata_cpp.pc_iata_no.NotIsNull()) {
		ps_IRGetCopIdByIataInd.ind_pc_iata_no = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetCopIdByIata.c_pc_iata_no, po_IRGetCopIdByIata_cpp.pc_iata_no.to_string());
	}

	if(po_IRGetCopIdByIata_cpp.pc_cop_id.NotIsNull()) {
		ps_IRGetCopIdByIataInd.ind_pc_cop_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRGetCopIdByIata.c_pc_cop_id, po_IRGetCopIdByIata_cpp.pc_cop_id.to_string());
	}

}

extern "C" {
extern int
dbIRUpdateIrRows(SCIRUpdateIrRows *ps_IRUpdateIrRows,
          SCIRUpdateIrRowsInd *ps_IRUpdateIrRowsInd);
}

//Constructor
CIRUpdateIrRows::CIRUpdateIrRows() :
	pc_session_id(8)
{
}

void
CDIR::updateIrRows(CIRUpdateIrRows &po_IRUpdateIrRows_cpp)
{
	SCIRUpdateIrRows ls_IRUpdateIrRows;
	SCIRUpdateIrRowsInd ls_IRUpdateIrRowsInd;

	memset(&ls_IRUpdateIrRows, 0, sizeof(SCIRUpdateIrRows));
	memset(&ls_IRUpdateIrRowsInd, -1, sizeof(SCIRUpdateIrRowsInd));

	updateIrRows_appToDb(ls_IRUpdateIrRows, ls_IRUpdateIrRowsInd,
			po_IRUpdateIrRows_cpp);

	logTimer("B:IRUpdateIrRows", NULL);
	if(::dbIRUpdateIrRows(&ls_IRUpdateIrRows, &ls_IRUpdateIrRowsInd) != 0) {
		logTimer("A:IRUpdateIrRows (FAILED)", NULL);
		return;
	}
	logTimer("A:IRUpdateIrRows", NULL);

	updateIrRows_dbToApp(ls_IRUpdateIrRows, ls_IRUpdateIrRowsInd,
			po_IRUpdateIrRows_cpp);
}

// C to C++ assignments:
void
CDIR::updateIrRows_dbToApp(SCIRUpdateIrRows &ps_IRUpdateIrRows,
		SCIRUpdateIrRowsInd &ps_IRUpdateIrRowsInd,
		CIRUpdateIrRows &po_IRUpdateIrRows_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::updateIrRows_appToDb(SCIRUpdateIrRows &ps_IRUpdateIrRows,
		SCIRUpdateIrRowsInd &ps_IRUpdateIrRowsInd,
		CIRUpdateIrRows &po_IRUpdateIrRows_cpp)
{
	int ii;

	if(po_IRUpdateIrRows_cpp.pc_session_id.NotIsNull()) {
		ps_IRUpdateIrRowsInd.ind_pc_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRUpdateIrRows.c_pc_session_id, po_IRUpdateIrRows_cpp.pc_session_id.to_string());
	}

}

extern "C" {
extern int
dbIRCreateIrstopovercity(SCIRCreateIrstopovercity *ps_IRCreateIrstopovercity,
          SCIRCreateIrstopovercityInd *ps_IRCreateIrstopovercityInd);
}

//Constructor
CIRCreateIrstopovercity::CIRCreateIrstopovercity() :
	session_id(8),
	itin_type1(1),
	city_id1(4),
	itin_type2(1),
	city_id2(4),
	itin_type3(1),
	city_id3(4),
	itin_type4(1),
	city_id4(4),
	itin_type5(1),
	city_id5(4),
	itin_type6(1),
	city_id6(4)
{
}

void
CDIR::createIrstopovercity(CIRCreateIrstopovercity &po_IRCreateIrstopovercity_cpp)
{
	SCIRCreateIrstopovercity ls_IRCreateIrstopovercity;
	SCIRCreateIrstopovercityInd ls_IRCreateIrstopovercityInd;

	memset(&ls_IRCreateIrstopovercity, 0, sizeof(SCIRCreateIrstopovercity));
	memset(&ls_IRCreateIrstopovercityInd, -1, sizeof(SCIRCreateIrstopovercityInd));

	createIrstopovercity_appToDb(ls_IRCreateIrstopovercity, ls_IRCreateIrstopovercityInd,
			po_IRCreateIrstopovercity_cpp);

	logTimer("B:IRCreateIrstopovercity", NULL);
	if(::dbIRCreateIrstopovercity(&ls_IRCreateIrstopovercity, &ls_IRCreateIrstopovercityInd) != 0) {
		logTimer("A:IRCreateIrstopovercity (FAILED)", NULL);
		return;
	}
	logTimer("A:IRCreateIrstopovercity", NULL);

	createIrstopovercity_dbToApp(ls_IRCreateIrstopovercity, ls_IRCreateIrstopovercityInd,
			po_IRCreateIrstopovercity_cpp);
}

// C to C++ assignments:
void
CDIR::createIrstopovercity_dbToApp(SCIRCreateIrstopovercity &ps_IRCreateIrstopovercity,
		SCIRCreateIrstopovercityInd &ps_IRCreateIrstopovercityInd,
		CIRCreateIrstopovercity &po_IRCreateIrstopovercity_cpp)
{
	int ii;

}

// C++ to C assignments:
void
CDIR::createIrstopovercity_appToDb(SCIRCreateIrstopovercity &ps_IRCreateIrstopovercity,
		SCIRCreateIrstopovercityInd &ps_IRCreateIrstopovercityInd,
		CIRCreateIrstopovercity &po_IRCreateIrstopovercity_cpp)
{
	int ii;

	if(po_IRCreateIrstopovercity_cpp.session_id.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_session_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_session_id, po_IRCreateIrstopovercity_cpp.session_id.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.itin_type1.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_itin_type1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_itin_type1, po_IRCreateIrstopovercity_cpp.itin_type1.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.line_number1.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_line_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_line_number1 = po_IRCreateIrstopovercity_cpp.line_number1.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.order_number1.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_order_number1 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_order_number1 = po_IRCreateIrstopovercity_cpp.order_number1.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.city_id1.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_city_id1 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_city_id1, po_IRCreateIrstopovercity_cpp.city_id1.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.itin_type2.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_itin_type2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_itin_type2, po_IRCreateIrstopovercity_cpp.itin_type2.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.line_number2.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_line_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_line_number2 = po_IRCreateIrstopovercity_cpp.line_number2.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.order_number2.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_order_number2 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_order_number2 = po_IRCreateIrstopovercity_cpp.order_number2.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.city_id2.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_city_id2 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_city_id2, po_IRCreateIrstopovercity_cpp.city_id2.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.itin_type3.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_itin_type3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_itin_type3, po_IRCreateIrstopovercity_cpp.itin_type3.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.line_number3.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_line_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_line_number3 = po_IRCreateIrstopovercity_cpp.line_number3.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.order_number3.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_order_number3 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_order_number3 = po_IRCreateIrstopovercity_cpp.order_number3.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.city_id3.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_city_id3 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_city_id3, po_IRCreateIrstopovercity_cpp.city_id3.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.itin_type4.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_itin_type4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_itin_type4, po_IRCreateIrstopovercity_cpp.itin_type4.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.line_number4.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_line_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_line_number4 = po_IRCreateIrstopovercity_cpp.line_number4.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.order_number4.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_order_number4 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_order_number4 = po_IRCreateIrstopovercity_cpp.order_number4.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.city_id4.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_city_id4 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_city_id4, po_IRCreateIrstopovercity_cpp.city_id4.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.itin_type5.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_itin_type5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_itin_type5, po_IRCreateIrstopovercity_cpp.itin_type5.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.line_number5.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_line_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_line_number5 = po_IRCreateIrstopovercity_cpp.line_number5.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.order_number5.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_order_number5 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_order_number5 = po_IRCreateIrstopovercity_cpp.order_number5.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.city_id5.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_city_id5 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_city_id5, po_IRCreateIrstopovercity_cpp.city_id5.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.itin_type6.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_itin_type6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_itin_type6, po_IRCreateIrstopovercity_cpp.itin_type6.to_string());
	}

	if(po_IRCreateIrstopovercity_cpp.line_number6.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_line_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_line_number6 = po_IRCreateIrstopovercity_cpp.line_number6.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.order_number6.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_order_number6 = C_SYBASE_INDICATOR_NOT_NULL;
		ps_IRCreateIrstopovercity.c_order_number6 = po_IRCreateIrstopovercity_cpp.order_number6.to_long();
	}

	if(po_IRCreateIrstopovercity_cpp.city_id6.NotIsNull()) {
		ps_IRCreateIrstopovercityInd.ind_city_id6 = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRCreateIrstopovercity.c_city_id6, po_IRCreateIrstopovercity_cpp.city_id6.to_string());
	}

}

extern "C" {
extern int
dbIRProcessVoidTicket(SCIRProcessVoidTicket *ps_IRProcessVoidTicket,
          SCIRProcessVoidTicketInd *ps_IRProcessVoidTicketInd);
}

//Constructor
CIRProcessVoidTicket::CIRProcessVoidTicket() :
   pc_form(6),
   pc_ticket_no(30),
	pc_reference(40)
{
}

void
CDIR::processVoidTicket(CIRProcessVoidTicket &po_IRProcessVoidTicket_cpp)
{
   SCIRProcessVoidTicket ls_IRProcessVoidTicket;
   SCIRProcessVoidTicketInd ls_IRProcessVoidTicketInd;

   memset(&ls_IRProcessVoidTicket, 0, sizeof(SCIRProcessVoidTicket));
   memset(&ls_IRProcessVoidTicketInd, -1, sizeof(SCIRProcessVoidTicketInd));

   processVoidTicket_appToDb(ls_IRProcessVoidTicket, ls_IRProcessVoidTicketInd,
         po_IRProcessVoidTicket_cpp);

	logTimer("B:IRProcessVoidTicket",NULL);
	if(::dbIRProcessVoidTicket(&ls_IRProcessVoidTicket,&ls_IRProcessVoidTicketInd) != 0 )
	{
		logTimer("A:IRProcessVoidTicket (FAILED)",NULL);
		return;
	}
	logTimer("A:IRProcessVoidTicket",NULL);

   processVoidTicket_dbToApp(ls_IRProcessVoidTicket, ls_IRProcessVoidTicketInd,
         po_IRProcessVoidTicket_cpp);
}

// C to C++ assignment:
void
CDIR::processVoidTicket_dbToApp(SCIRProcessVoidTicket &ps_IRProcessVoidTicket,
      SCIRProcessVoidTicketInd &ps_IRProcessVoidTicketInd,
      CIRProcessVoidTicket &po_IRProcessVoidTicket_cpp)
{
}

// C++ to C assignments:
void
CDIR::processVoidTicket_appToDb(SCIRProcessVoidTicket &ps_IRProcessVoidTicket,
      SCIRProcessVoidTicketInd &ps_IRProcessVoidTicketInd,
      CIRProcessVoidTicket &po_IRProcessVoidTicket_cpp)
{

   if(po_IRProcessVoidTicket_cpp.pc_form.NotIsNull()) {
      ps_IRProcessVoidTicketInd.ind_pc_form = C_SYBASE_INDICATOR_NOT_NULL;
      strcpy(ps_IRProcessVoidTicket.c_pc_form, po_IRProcessVoidTicket_cpp.pc_form.to_string());
   }

   if(po_IRProcessVoidTicket_cpp.pc_ticket_no.NotIsNull()) {
      ps_IRProcessVoidTicketInd.ind_pc_ticket_no = C_SYBASE_INDICATOR_NOT_NULL;
      strcpy(ps_IRProcessVoidTicket.c_pc_ticket_no, po_IRProcessVoidTicket_cpp.pc_ticket_no.to_string());
   }

   if(po_IRProcessVoidTicket_cpp.pc_reference.NotIsNull()) {
      ps_IRProcessVoidTicketInd.ind_pc_reference = C_SYBASE_INDICATOR_NOT_NULL;
      strcpy(ps_IRProcessVoidTicket.c_pc_reference, po_IRProcessVoidTicket_cpp.pc_reference.to_string());
   }

}

extern "C" {
extern int
dbIRValReasonCode(SCIRValReasonCode *ps_IRValReasonCode,
          SCIRValReasonCodeInd *ps_IRValReasonCodeInd);
}

//Constructor
CIRValReasonCode::CIRValReasonCode() :
	cop_id(2),
	reason_code(10),
	rc_valid(1)
{
}

void
CDIR::valReasonCode(CIRValReasonCode &po_IRValReasonCode_cpp)
{
	SCIRValReasonCode ls_IRValReasonCode;
	SCIRValReasonCodeInd ls_IRValReasonCodeInd;

	memset(&ls_IRValReasonCode, 0, sizeof(SCIRValReasonCode));
	memset(&ls_IRValReasonCodeInd, -1, sizeof(SCIRValReasonCodeInd));

	valReasonCode_appToDb(ls_IRValReasonCode, ls_IRValReasonCodeInd,
			po_IRValReasonCode_cpp);

	logTimer("B:IRValReasonCode", NULL);
	if(::dbIRValReasonCode(&ls_IRValReasonCode, &ls_IRValReasonCodeInd) != 0) {
		logTimer("A:IRValReasonCode (FAILED)", NULL);
		return;
	}
	logTimer("A:IRValReasonCode", NULL);

	valReasonCode_dbToApp(ls_IRValReasonCode, ls_IRValReasonCodeInd,
			po_IRValReasonCode_cpp);
}

// C to C++ assignments:
void
CDIR::valReasonCode_dbToApp(SCIRValReasonCode &ps_IRValReasonCode,
		SCIRValReasonCodeInd &ps_IRValReasonCodeInd,
		CIRValReasonCode &po_IRValReasonCode_cpp)
{
	int ii;

	if(ps_IRValReasonCodeInd.ind_rc_valid
			== C_SYBASE_INDICATOR_NOT_NULL) {
		po_IRValReasonCode_cpp.rc_valid = ps_IRValReasonCode.c_rc_valid;
	}

}

// C++ to C assignments:
void
CDIR::valReasonCode_appToDb(SCIRValReasonCode &ps_IRValReasonCode,
		SCIRValReasonCodeInd &ps_IRValReasonCodeInd,
		CIRValReasonCode &po_IRValReasonCode_cpp)
{
	int ii;

	if(po_IRValReasonCode_cpp.cop_id.NotIsNull()) {
		ps_IRValReasonCodeInd.ind_cop_id = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValReasonCode.c_cop_id, po_IRValReasonCode_cpp.cop_id.to_string());
	}

	if(po_IRValReasonCode_cpp.reason_code.NotIsNull()) {
		ps_IRValReasonCodeInd.ind_reason_code = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValReasonCode.c_reason_code, po_IRValReasonCode_cpp.reason_code.to_string());
	}

	if(po_IRValReasonCode_cpp.rc_valid.NotIsNull()) {
		ps_IRValReasonCodeInd.ind_rc_valid = C_SYBASE_INDICATOR_NOT_NULL;
		strcpy(ps_IRValReasonCode.c_rc_valid, po_IRValReasonCode_cpp.rc_valid.to_string());
	}

}
