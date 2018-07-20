/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999

                SID: 1.2
      Delta Created: 00/03/14 - 14:16:46
         Last Delta: app/src/common/InterfaceRecord/_us/ 3/14/00 - 14:16:46 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.MirSvcFee.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/

#include "gtypes.h"

class CMirSvcFee
{

public:

    AB_Integer mi_status;
    AB_Char    mc_amount;
	 AB_Char    mc_formOfPay;
	 AB_Char    mc_supplier;
	 AB_Char    mc_creditCardId;
	 AB_Char    mc_expirationDate;

    AB_Varchar   mv_errorField;

    CMirSvcFee(); // Constructor 
    CMirSvcFee(const AB_Varchar& pv_svcSF,
					const AB_Varchar& pv_svcFEE,
					const AB_Varchar& pv_fopFromA11,
					const AB_Varchar& pv_ccIdFromA11,
					const AB_Varchar& pv_expDateFromA11); // Constructor 

    virtual ~CMirSvcFee(){}; // Destructor

    // ---------------------------------------------------------
    // Initialize record
    // ---------------------------------------------------------
    void
    initialize(const AB_Varchar& pv_svcSF, 
					const AB_Varchar& pv_svcFEE,
					const AB_Varchar& pv_fopFromA11,
					const AB_Varchar& pv_ccIdFromA11,
					const AB_Varchar& pv_expDateFromA11);

    // ---------------------------------------------------------
    // Edit each field  
    // ---------------------------------------------------------
    AB_Integer
    edit() ;

    // ---------------------------------------------------------
    // Print raw data   
    // ---------------------------------------------------------
    void
    print() ;

    // ---------------------------------------------------------
    // Return error field    
    // ---------------------------------------------------------
    AB_Varchar 
    getErrorField(){ return mv_errorField; } 

    // ---------------------------------------------------------
    // Return record detail
    // ---------------------------------------------------------
    void getDetail( AB_Char    & pc_amount,
                    AB_Char    & pc_formOfPay,
                    AB_Char    & pc_supplier,
                    AB_Char    & pc_creditCardId,
                    AB_Char    & pc_expirationDate,
                    AB_Integer & pi_status);

};


