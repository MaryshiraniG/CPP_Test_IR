/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996

      SID: 1.4
      Delta Created: 14/11/04 - 14:29:53
    Last Delta: app/src/common/InterfaceRecord/_us/ 11/4/14 - 14:29:53 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.MirSvcFee.cpp) 
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us/}[11/4/14-14:29:53 1.4] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.MirSvcFee.cpp (Jinsong Liu)\n" )
/*
  The contents of this file, its programs, all comments, code and information 
          are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
============================================================ END GLOBAL BANNER*/

#include "MirSvcFee.h"
#include "Log.h"

CMirSvcFee::CMirSvcFee() : mc_amount(13),
	                        mc_formOfPay(2),
	                        mc_supplier(3),
	                        mc_creditCardId(18),
	                        mc_expirationDate(4)

{
    mc_amount          = AB_null;
    mc_formOfPay       = AB_null;
    mc_supplier        = AB_null;
    mc_creditCardId    = "                  ";
    mc_expirationDate  = "    ";
    mi_status          = AB_Integer(0);
}

CMirSvcFee::CMirSvcFee( const AB_Varchar& pv_svcSF,
								const AB_Varchar& pv_svcFEE,
								const AB_Varchar& pv_fopFromA11,
								const AB_Varchar& pv_ccIdFromA11,
								const AB_Varchar& pv_expDateFromA11) : 
                        mc_amount(13),
	                     mc_formOfPay(2),
	                     mc_supplier(3),
	                     mc_creditCardId(18),
	                     mc_expirationDate(4)
{
    mc_amount          = AB_null;
    mc_formOfPay       = AB_null;
    mc_supplier        = AB_null;
    mc_creditCardId    = "               ";
    mc_expirationDate  = "    ";
    mi_status          = AB_Integer(0);

    initialize(pv_svcSF,
					pv_svcFEE,
					pv_fopFromA11,
					pv_ccIdFromA11,
					pv_expDateFromA11);
}

// ---------------------------------------------------------
// Initialize record
// ---------------------------------------------------------

void
CMirSvcFee::initialize(const AB_Varchar& pv_svcSF,
							  const AB_Varchar& pv_svcFEE,
							  const AB_Varchar& pv_fopFromA11,
							  const AB_Varchar& pv_ccIdFromA11,
							  const AB_Varchar& pv_expDateFromA11)
{
    AB_Integer li_index;

    /**
     * Service data is derived from A14 amount data and 
     * form of payment data
	  * 
	  * A. amount part is :  #####.##
	  *                      #####.##/CC 
	  *                      (CC is optional can be ignored) 
     *
     * (Leading SF- is alreay removed)
	  *
     **/

    if ( pv_svcSF.IsNull() && pv_svcFEE.IsNull() )
    {
        // No fees to create, no A14-SF,A14-FEE

        mi_status = AB_Integer(0);   // 
    }
    else if ( pv_svcSF.IsNull() && pv_svcFEE.NotIsNull() )
    {
        // No fees to create, A14-SF present without A14-FEE

        mi_status = AB_Integer(3611);  
    }
    else if ( pv_svcSF.NotIsNull() && pv_svcFEE.IsNull() )
    {
        // No fees to create, A14-FEE present without A14-SF

        mi_status = AB_Integer(3612);  
    } 
    else 
    {
        // Extract amount info from SF part

        AB_Varchar lv_amount(13); 
         
        lv_amount = pv_svcSF;

        li_index  = lv_amount.findSubstring("/CC");

        if ( li_index > AB_Integer(0) ) 
        {
            // Ignore form of pay

            lv_amount = lv_amount.For(li_index-AB_Integer(1)); 
        } 

		  if ( ! lv_amount.isAllNumeric() || lv_amount.length() <= 2 ) {

				// Invalid amount

		      mi_status = AB_Integer(3610);
				return;
		  }

		  // Plug in decimal point into dddddcc

		  int li_length; 
		  li_length = lv_amount.length();

        mc_amount = lv_amount.For(li_length-2) +  "." +
                    lv_amount.At(li_length-1);

        /**
	      * B. valid form of payment part are as followings :
         *
	      *    B1. FEE-MR2/FP:CCAX370000000000028/D1200
	      *    B2. FEE-MR2/FP:CA
	      *    B3. FEE-MR2/FP:CK
	      *    B4. FEE-MR2/CC
	      *    B5. FEE-MR2/
	      *    B6. FEE-MR2
	      * 
	      * The invalid forms of payment part are :
	      *
	      *    B7. FEE-
	      *    B8. FEE
         *
         * A B must be present at the same time
         **/

        if ( pv_svcFEE == AB_Text("FEE") || pv_svcFEE == AB_Text("FEE-") )
        {
            // Invalid A14-FEE

            mi_status = AB_Integer(3609);  
        }
        else if ( pv_svcFEE.For(4) == "FEE-" )
        {
            // Remove leading G*FEE-

            AB_Varchar lv_fopData(100);

            lv_fopData = pv_svcFEE.At(5);

            if ( lv_fopData.length() == 3 ) 
            {
                // Case B6: FEE-MR2

                mc_supplier       = lv_fopData;
                mc_formOfPay      = pv_fopFromA11;

					 if ( mc_formOfPay == AB_Text("CC") ) 
					 {
	                mc_creditCardId   = pv_ccIdFromA11;
	                mc_expirationDate = pv_expDateFromA11;
					 }
            }
            else if ( lv_fopData.length() == 4 && 
							 lv_fopData.At(4) == AB_Text("/")) 
            {
                // Case B5: FEE-MR2/

                mc_supplier       = lv_fopData.For(3);
                mc_formOfPay      = pv_fopFromA11;
	             
					 if ( mc_formOfPay == AB_Text("CC") ) 
					 {
	                mc_creditCardId   = pv_ccIdFromA11;
	                mc_expirationDate = pv_expDateFromA11;
					 }
            }
            else if ( lv_fopData.length() == 6 && 
							 lv_fopData.At(4).For(3) == AB_Text("/CC")) 
            {
                // Case B4: FEE-MR2/CC

                mc_supplier       = lv_fopData.For(3);
                mc_formOfPay      = pv_fopFromA11;
	            
					 if ( mc_formOfPay == AB_Text("CC") ) 
					 {
	                mc_creditCardId   = pv_ccIdFromA11;
	                mc_expirationDate = pv_expDateFromA11;
					 }
            }
            else if ( lv_fopData.length() == 9 && 
							 lv_fopData.At(4).For(6) == AB_Text("/FP:CK")) 
            {
                // Case B3: FEE-MR2/FP:CK

                mc_supplier  = lv_fopData.For(3);
                mc_formOfPay = "CK";
            }
            else if ( lv_fopData.length() == 9 && 
							 lv_fopData.At(4).For(6) == AB_Text("/FP:CA")) 
            {
                // Case B2: FEE-MR2/FP:CA

                mc_supplier  = lv_fopData.For(3);
                mc_formOfPay = "CA";
            }
            else if ( lv_fopData.At(4).For(6) == AB_Text("/FP:CC")) 
            {
                // Case B1: FEE-MR2/FP:CCAX370000000000028/D1200

                AB_Varchar lv_ccPart(80);

                lv_ccPart = lv_fopData.At(10);

                li_index  = lv_ccPart.findSubstring("/D");
                   
                if ( li_index > AB_Integer(0) ) 
                {
                    mc_supplier        = lv_fopData.For(3);
                    mc_formOfPay       = "CC";
	                 mc_creditCardId    = lv_ccPart.For(li_index-AB_Integer(1));
	                 mc_expirationDate  = lv_ccPart.At(li_index+AB_Integer(2));
                 }
                 else
                 {
                     // Invalid A-14 FEE record 

                     mi_status = AB_Integer(3609);
                 } 
            }
        }
        else 
        {
            // Invalid A-14 FEE record 

            mi_status = AB_Integer(3609);
        }
    }
} 

// ---------------------------------------------------------
// Edit each field  
// ---------------------------------------------------------

AB_Integer 
CMirSvcFee::edit()
{
    AB_Integer li_returnValue(0);
	 return li_returnValue;
} 

// ---------------------------------------------------------
// Return record detail
// ---------------------------------------------------------

void
CMirSvcFee::getDetail( AB_Char    & pc_amount,
                       AB_Char    & pc_formOfPay,
                       AB_Char    & pc_supplier,
                       AB_Char    & pc_creditCardId,
                       AB_Char    & pc_expirationDate,
                       AB_Integer & pi_status)

{ 
    pc_amount          = mc_amount;
    pc_formOfPay       = mc_formOfPay;
    pc_supplier        = mc_supplier;
    pc_creditCardId    = mc_creditCardId;
    pc_expirationDate  = mc_expirationDate;
    pi_status          = mi_status;
}

// ---------------------------------------------------------
// Print record     
// ---------------------------------------------------------
void
CMirSvcFee::print()
{
    LOG(logDEBUG) << "***** Service Data Set *****" ;

    LOG(logDEBUG) << "mi_status  "         <<  mi_status          ;
    LOG(logDEBUG) << "mc_amount "          <<  mc_amount          ;
    LOG(logDEBUG) << "mc_formOfPay "       <<  mc_formOfPay       ;
    LOG(logDEBUG) << "mc_supplier "        <<  mc_supplier        ;
    LOG(logDEBUG) << "mc_creditCardId "    <<  mc_creditCardId    ;
    LOG(logDEBUG) << "mc_expirationDate "  <<  mc_expirationDate  ;

    LOG(logDEBUG) << "*****         E O F            *****" ;
} 

