/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.1
      Delta Created: 12/05/25 - 18:33:40
         Last Delta: app/src/common/InterfaceRecord/_us/ 5/25/12 - 18:33:40 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.AirlineTicketingFee.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/

#ifndef CAirlineTicketingFee_H
#define CAirlineTicketingFee_H

#include "pnrstlcommon.h"

class CInterfaceRecord;

class CAirlineTicketingFee {
	public:
	class CTicketingFee {
		public:
			CTicketingFee(
				  const D_passenger_no& 		pd_pax_id,
				  const D_line_number&  		pd_line_number,
				  const D_amount& 				pd_fee_amount,
				  const AB_Text& 				   pd_ticket_number) {
				md_pax_id			= pd_pax_id;
				md_line_number		= pd_line_number;
				md_fee_amount		= pd_fee_amount;
				md_ticket_number	= pd_ticket_number;
			}

			CTicketingFee(const CTicketingFee& po) {
				md_pax_id			= po.md_pax_id;
				md_line_number		= po.md_line_number;
				md_fee_amount		= po.md_fee_amount;
				md_ticket_number	= po.md_ticket_number;
			}

			void dump() const {
				cout << "P_id:" << md_pax_id << " "
						  << "LN:" << md_line_number << " "
						  << "fee:" << md_fee_amount << " "
						  << "T#:" << md_ticket_number << " "
						  << endl;
			}

		public:
			D_passenger_no 		md_pax_id;
			D_line_number  		md_line_number;
			D_amount 				md_fee_amount;
			AB_Text					md_ticket_number;
	};

	CAirlineTicketingFee() {}
	virtual ~CAirlineTicketingFee();

	CTicketingFee* findFee(const CTicketingFee& po_fee) const;
	void addFee(const CTicketingFee& po_fee);
	void createOBTaxes(CInterfaceRecord& po) const;
	void clear();
	void dump() const;

	private:
		vector<CTicketingFee*> mv_fees;
	
} ;

#endif //CAirlineTicketingFee_H
