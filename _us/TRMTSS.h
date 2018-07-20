/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996

                SID: 1.3
      Delta Created: 00/03/27 - 13:33:27
         Last Delta: app/src/common/InterfaceRecord/_us 3/27/00 - 13:33:27 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.TRMTSS.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/
#include "dbGirrawdetail.h"
class CTRMTSS : public CTSS
{
   public:
		void processRMA(D_session_id &pd_sessionId,
   						D_client_file_type  &pd_cfa_type,
   						D_client_file_code  &pd_cfa_code,
   						D_client_file_no    &pd_cfa_id,
   						D_person_name       &pd_last_name,
   						D_person_name       &pd_first_name,
   						D_consultant        &pd_con_id,
							D_company           &pd_cfaCop,
							D_branch            &pd_cfaBra) ;

      CTRMTSS() ;
      ~CTRMTSS() {cout << "OBJECT TRMTSS DESTROYED" << endl ;}

   private:
      void errorHandler() ;
		void createTRMTSS(CPNR 				  &po_PNR,
   							D_client_file_type  &pd_cfa_type,
   							D_client_file_code  &pd_cfa_code,
   							D_client_file_no    &pd_cfa_id,
   							D_person_name       &pd_last_name,
   							D_person_name       &pd_first_name,
   							D_consultant        &pd_con_id) ;
		void initGirrawdetail(CGirrawdetailRead  &po_GirrawRead)  ;
      void saveSvcIrregularity();
} ;

