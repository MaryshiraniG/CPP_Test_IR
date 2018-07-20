/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999, 
															 2000, 2001
                SID: 1.4
      Delta Created: 15/02/13 - 10:12:35
         Last Delta: app/src/common/InterfaceRecord/_us/ 2/13/15 - 10:12:35 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.AirlineTicketingFee.cpp) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
============================================================ END GLOBAL BANNER*/
#include "gtypes.h"

#include "AirlineTicketingFee_D.h"
#include "dbIR.h"
#include "InterfaceRecord.h"
#include "AirlineTicketingFee.h"

#define ENTER      LOG(logINFO) << "*** Enter>> " << __FUNCTION__ << endl;
#define EXIT       LOG(logINFO) << "*** Exit << " << __FUNCTION__ << endl;
#define RETURN(a)  LOG(logINFO) << "*** Exit << " << __FUNCTION__ << endl;return(a);

CAirlineTicketingFee::~CAirlineTicketingFee() {
	ENTER;

	for (vector<CTicketingFee*>::iterator iter = mv_fees.begin();
		  iter < mv_fees.end(); iter++) {
		CTicketingFee* lop = *iter;
		delete lop;
	}

	EXIT;
}

CAirlineTicketingFee::CTicketingFee*
CAirlineTicketingFee::findFee(const CAirlineTicketingFee::CTicketingFee& po_fee) const {
	// ENTER;

	CAirlineTicketingFee::CTicketingFee* lo_retval = 0;
	for (int i = 0; i < mv_fees.size(); i++) {
		CAirlineTicketingFee::CTicketingFee* lop = mv_fees[i];
		if (
			 lop->md_pax_id			== po_fee.md_pax_id && 
			 lop->md_line_number		== po_fee.md_line_number &&
			 lop->md_ticket_number	== po_fee.md_ticket_number) {
			lo_retval = lop;
			break;
		}
	}

	return lo_retval;
}

void
CAirlineTicketingFee::addFee(const CAirlineTicketingFee::CTicketingFee& po_fee) {
	// ENTER;

	CAirlineTicketingFee::CTicketingFee* lop = findFee(po_fee);
	if (lop == 0) {
		lop = new CAirlineTicketingFee::CTicketingFee(po_fee);
		mv_fees.push_back(lop);
	} else {
		AB_Decimal  lm_amt1(13,2);
		AB_Decimal  lm_amt2(13,2);

		lm_amt1 = lop->md_fee_amount;
		lm_amt2 = po_fee.md_fee_amount;

		lop->md_fee_amount = lm_amt1 + lm_amt2;
	}

	// EXIT;
}

void
CAirlineTicketingFee::createOBTaxes(CInterfaceRecord& po) const {
	ENTER;

	for (int i = 0; i < mv_fees.size(); i++) {
		CAirlineTicketingFee::CTicketingFee* lop = mv_fees[i];

		int idx = po.mi_ITD_I;

		po.md_ITD_pax_id[idx] 			= lop->md_pax_id;
		po.md_ITD_line_number[idx] 	= lop->md_line_number;
		po.md_ITD_tax_id[idx] 			= "OB";
		po.md_ITD_tax_amount[idx] 		= lop->md_fee_amount;
		po.md_ITD_tax_source[idx] 		= "MA";

		po.mi_ITD_I++;
	}

		LOG(logDEBUG) << "-- po.mi_IRP_I = " << po.mi_IRP_I << endl;

		/* AUSTRALIA SPECIFIC -> IGNORE FOR US*/
		/* 519611CWTUS put it back to common code with AU*/
	for (int i = 0; i <= po.mi_IRP_I; i++) {
		CTicketingFee lo_fee(po.md_IRP_pax_id[i],
									po.md_IRP_line_number[i],
									0.00,
									0);//519611

		CTicketingFee* lop = findFee(lo_fee);

		if (lop == 0) {
		LOG(logDEBUG) << "-- NOT found lop" << endl;
		}

		if (lop != 0) {
		LOG(logDEBUG) << "-- found lop" << endl;
			AB_Decimal  lm_amt1(13,2);
			AB_Decimal  lm_amt2(13,2);

			lm_amt1 = lop->md_fee_amount;
			lm_amt2 = 0.00;

			if (po.md_IRP_form_of_pay[i] == "CC" && 
				 po.md_IRP_amount[i].NotIsNull()) {
				lm_amt2 = po.md_IRP_amount[i];
				po.md_IRP_amount[i] = lm_amt1 + lm_amt2;
			}
            /*
			if (po.md_IRP_SEC_form_of_pay[i] == "CC" && 
				 po.md_IRP_SEC_amount[i].NotIsNull()) {
				lm_amt2 = po.md_IRP_SEC_amount[i];
				po.md_IRP_SEC_amount[i] = lm_amt1 + lm_amt2;
			}
			*/
		}
	}
		

	EXIT;
}

void
CAirlineTicketingFee::clear() {
	ENTER;

	for (vector<CTicketingFee*>::iterator iter = mv_fees.begin();
		  iter < mv_fees.end(); iter++) {
		CTicketingFee* lop = *iter;
		delete lop;
	}

	mv_fees.clear();

	EXIT;
}

void
CAirlineTicketingFee::dump() const {
	ENTER;

	for (int i = 0; i < mv_fees.size(); i++) {
		CAirlineTicketingFee::CTicketingFee* lop = mv_fees[i];
		lop->dump();
	}

	EXIT;
}


