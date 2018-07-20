/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.10
      Delta Created: 04/04/15 - 12:19:45
         Last Delta: app/src/common/InterfaceRecord/_us 4/15/04 - 12:19:45 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.dbGirrawdetail.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/

#ifndef dbGirrawdetail_h
#define dbGirrawdetail_h

#include "tbGirrawdetail.h"

class CGirrawdetailRead {
public:
// Constructor
	CGirrawdetailRead();

	AB_Varchar pc_session_id;
	AB_Smallint pi_subsect_id;
	AB_Smallint pi_line_number;
	AB_Varchar pc_interface_type;
	AB_Date pd_interface_cdate;
	AB_Varchar sec1;
	AB_Smallint sub1;
	AB_Smallint lno1;
	AB_Varchar det1;
	AB_Varchar sec2;
	AB_Smallint sub2;
	AB_Smallint lno2;
	AB_Varchar det2;
	AB_Varchar sec3;
	AB_Smallint sub3;
	AB_Smallint lno3;
	AB_Varchar det3;
	AB_Varchar sec4;
	AB_Smallint sub4;
	AB_Smallint lno4;
	AB_Varchar det4;
	AB_Varchar sec5;
	AB_Smallint sub5;
	AB_Smallint lno5;
	AB_Varchar det5;
	AB_Varchar sec6;
	AB_Smallint sub6;
	AB_Smallint lno6;
	AB_Varchar det6;
	AB_Varchar sec7;
	AB_Smallint sub7;
	AB_Smallint lno7;
	AB_Varchar det7;
	AB_Varchar sec8;
	AB_Smallint sub8;
	AB_Smallint lno8;
	AB_Varchar det8;
	AB_Varchar sec9;
	AB_Smallint sub9;
	AB_Smallint lno9;
	AB_Varchar det9;
	AB_Varchar sec10;
	AB_Smallint sub10;
	AB_Smallint lno10;
	AB_Varchar det10;
	AB_Varchar sec11;
	AB_Smallint sub11;
	AB_Smallint lno11;
	AB_Varchar det11;
	AB_Varchar sec12;
	AB_Smallint sub12;
	AB_Smallint lno12;
	AB_Varchar det12;
	AB_Varchar sec13;
	AB_Smallint sub13;
	AB_Smallint lno13;
	AB_Varchar det13;
	AB_Varchar sec14;
	AB_Smallint sub14;
	AB_Smallint lno14;
	AB_Varchar det14;
	AB_Varchar sec15;
	AB_Smallint sub15;
	AB_Smallint lno15;
	AB_Varchar det15;
	AB_Integer pi_counter;
	AB_Varchar pc_more_data;
	AB_Smallint pi_key_subsect_id;
	AB_Smallint pi_key_line_number;
};

class CDGirrawdetail {
public:
	void read(CGirrawdetailRead &po_GirrawdetailRead_cpp);

private:
	void read_dbToApp(SCGirrawdetailRead &ps_GirrawdetailRead,
			SCGirrawdetailReadInd &ps_GirrawdetailReadInd,
			CGirrawdetailRead &po_GirrawdetailRead_cpp);
	void read_appToDb(SCGirrawdetailRead &ps_GirrawdetailRead,
			SCGirrawdetailReadInd &ps_GirrawdetailReadInd,
			CGirrawdetailRead &po_GirrawdetailRead_cpp);
};

#endif /* dbGirrawdetail_h */
