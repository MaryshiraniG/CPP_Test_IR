/*===================c_source_include_header================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
                                              2000, 2001
                SID: 1.18
      Delta Created: 16/04/27 - 15:29:21
         Last Delta: app/src/common/InterfaceRecord/_us/ 4/27/16 - 15:29:21 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.AIR.h) 

  The contents of this file, its programs, all comments, code and information 
               are the sole property of GLOBAL TRAVEL Inc. 
     No duplication by any method, electronic or otherwise is permitted.
==============================================================END GLOBAL BANNER*/
#ifndef CAIR_H
#define CAIR_H

#include "dbIR.h"
#include "InterfaceRecord_D.h"
#include "InterfaceRecord.h"
#include "airRecord.h"

class CAIR : public CInterfaceRecord
{

public:

void
printAll();

void
createAIR(const D_session_id &pd_session_id,
			 const AB_Date &pd_creation_date,
			 const AB_Char &pc_from);

void
processRMA(D_session_id        &pd_session_id,
			  AB_Date				 &pd_lcl_date,
			  D_client_file_type  &pd_cfa_type,
			  D_client_file_code  &pd_cfa_code,
			  D_client_file_no    &pd_cfa_id,
			  D_person_name       &pd_last_name,
			  D_person_name       &pd_first_name,
			  D_consultant        &pd_con_id,
			  D_company           &pd_cfaCop,
			  D_branch            &pd_cfaBra) ;

private:

				AB_Date				mt_lclPnrDate;

				int 					mi_IT_SEGNBR[500];
				int					mi_IT_TYPE[500];
				V_AB_Varchar		mva_IT_PNRSTAT;
				V_AB_Varchar		mva_IT_BAGALL;
				int					mi_TicketedSeg[500];
				int					mi_UnicketedSeg[500];

				int					mi_IR_SEGNBR[500];
				int					mi_IR_TYPE[500];
				V_AB_Varchar		mva_IR_PSGRNBR;

				int					mi_IO_SEGNBR[500];
				int					mi_IO_TYPE[500];

				int					mi_IP_PSGRNBR[500];

AB_Text
retSupName(const AB_Varchar &pv_sup_id);

AB_Text
retSupPrimDoc(const AB_Varchar &pv_sup_id);

void
createITRemark(const AB_Varchar &pv_sup_id);

void
createIRRemark(const AB_Varchar &pv_rmk, const AB_Varchar &pv_itin_type);

void
createIORemark(const AB_Varchar &pv_rmk, const AB_Varchar &pv_itin_type);

void
createExchageCostRemark(const AB_Varchar &pv_rmk);

void
retListNumbers(const AB_Varchar &pd_delete_list,
					AB_Integer   &pi_count,
					V_AB_Integer &pvi_list);

AB_Char
deriveDte(const AB_Varchar &pd_dte);

void
retSegAttributes(const int &i, AB_Varchar	&pc_itin_type, 
					  D_line_number &pd_tra_line_number,
					  D_line_number &pd_ren_line_number,
					  D_line_number &pd_oth_line_number);

void
retCarRateData(const AB_Varchar&		pv_rbgq_opt,
					D_currency&				pd_cur_id,
					D_char_numeric&		pd_local_rate,
					D_char_numeric&		pd_booked_rate,
					D_char_numeric& 		pd_regular_rate,
					D_free_mileage& 		pd_free_mileage,
					D_rate_duration& 		pd_rate_duration,
					D_free_mileage_code& pd_free_mile_code,
					D_char_numeric& 		pd_ex_day_charge,
					D_char_numeric& 		pd_ex_hr_charge,
					D_char_numeric& 		pd_ex_mile_charge) const;

void
getXFbreakdown(const D_amount& 			pd_tot_amt,
					const D_passenger_no&	pd_pax_id,
					const D_line_number&		pd_line_number,
					const AB_Varchar&			pv_fare);

AB_Integer
deriveArrInd(const AB_Varchar &pd_depdte,
				 const AB_Varchar &pd_arrdte);

void
getAcodePhone(const AB_Varchar &token,
				  D_area_code &acode,
				  D_phone_number &phone);

void
getNumbersFromI_(CairRecord &po_a,
					  D_area_code &pd_IA_phone_acode,
					  D_phone_number &pd_IA_phone_no,
					  D_area_code &pd_IA_bphone_acode,
					  D_phone_number &pd_IA_bphone_no,
					  D_area_code &pd_IA_fax_acode,
					  D_phone_number &pd_IA_fax_no);

void
getAddressFromAB(CairRecord &po_a,const int &i,
						D_address_line &pd_IA_addr1,
						D_address_line &pd_IA_addr2,
						D_address_line &pd_IA_addr3,
						D_address_line &pd_IA_addr4,
						D_address_line &pd_IA_addr5,
						D_address_line &pd_IA_addr6);

void
getAddressFromAM(CairRecord &po_a,const int &i,
						D_address_line &pd_IA_addr1,
						D_address_line &pd_IA_addr2,
						D_address_line &pd_IA_addr3,
						D_address_line &pd_IA_addr4,
						D_address_line &pd_IA_addr5,
						D_address_line &pd_IA_addr6);

void
getAddressFromABS(CairRecord &po_a,const int &i,
						D_address_line &pd_IA_addr1,
						D_address_line &pd_IA_addr2,
						D_address_line &pd_IA_addr3,
						D_address_line &pd_IA_addr4,
						D_address_line &pd_IA_addr5,
						D_address_line &pd_IA_addr6);

void
getAddressFromAMS(CairRecord &po_a,const int &i,
						D_address_line &pd_IA_addr1,
						D_address_line &pd_IA_addr2,
						D_address_line &pd_IA_addr3,
						D_address_line &pd_IA_addr4,
						D_address_line &pd_IA_addr5,
						D_address_line &pd_IA_addr6);

void
retCommission(const int &pax_id, CairRecord &po_a, AB_Varchar &comm_rate, AB_Varchar &commission);

void
errorHandler(CairRecord  &pd_errorDescription);

void
TRM_errorHandler();

void
populateIrArrays(CairRecord &po_a);

AB_Text
retFromRM(CairRecord &po_a,const AB_Text pt_what,const int &pax_id);

// SCR: 514859
AB_Text
retSavingCodeFromRM(CairRecord &po_a,const AB_Text pt_what,const int &pax_id);

AB_Text
retCharFromRM(CairRecord &po_a,const AB_Text pt_matchCreteria,const AB_Text pt_whatKey, const int &pax_id);

void
populateBaseVars(CairRecord &po_a);

void
populateIrtripheader(CairRecord &po_a);

void
populateIraddress(CairRecord &po_a);

void
populateIrperson(CairRecord &po_a);

void
populateIrticket(CairRecord &po_a);

void
populateIrtransport(CairRecord &po_a);

void
populateIrother(CairRecord &po_a);

void
populateIrfrequentuser(CairRecord &po_a);

void
populateIrconsumdetail(CairRecord &po_a);

void
populateIrrental(CairRecord &po_a);

void
populateIrreceipt(CairRecord &po_a);

void
populateIrdistribution(CairRecord &po_a);

void
populateIremailaddress(CairRecord &po_a);

void
populateIrremark(CairRecord &po_a);

void
populateNetTicketInfo(CairRecord &po_a);

void
populateMembership(CairRecord &po_a);

void
populateIrtaxdetail(CairRecord &po_a);

void
populateIrpayment(CairRecord &po_a);

void
populateIrpayment(CairRecord &po_a, int pi_id, const AB_Text& pv_fop);

void
processNonArcDoc(CairRecord &po_a);

AB_Boolean
processNonArcCost(CairRecord& po_a, const AB_Varchar&  pv_fop);

void
processSFC(CairRecord &po_a);

void
processSFM_SFO_SFA_SFN(CairRecord &po_a);
void 
create_SFN_Remark(AB_Boolean &pb_FR_found);

void
processMCO(CairRecord &po_a);

void
processEMD(CairRecord &po_a);

void
getServiceFeeDetail(	CairRecord&   		po_a,
							const AB_Varchar& pv_svcSF,
							const AB_Varchar& pv_svcFEE,
							AB_Char    & pc_amount,
                     AB_Char    & pc_tax1Code,
                     AB_Char    & pc_tax1Amount,
                     AB_Char    & pc_tax2Code,
                     AB_Char    & pc_tax2Amount,
                     AB_Char    & pc_tax3Code,
                     AB_Char    & pc_tax3Amount,
                     AB_Char    & pc_formOfPay,
                     AB_Char    & pc_supplier,
                     AB_Char    & pc_creditCardId,
                     AB_Char    & pc_expirationDate,
                     AB_Integer & pi_status);

void 
getFormOfPay(CairRecord& 	po_a,
				 AB_Varchar&   pv_form_of_pay,
				 AB_Varchar&	pv_cc_id,
				 AB_Varchar&	pv_cc_no,
				 AB_Varchar&	pv_cc_exp_date,
				 AB_Varchar&	pv_auth_no
				);

void
saveSvcIrregularity();

int
findTax(const D_tax&					pd_tax_id,
		  const D_passenger_no&		pd_pax_id,
		  const D_line_number&		pd_line_number) const;

void RP_parseRemarks(CairRecord &po_a);

public:

// constructor
	CAIR();

// destructor
	~CAIR() {};
};

#endif //CAIR_H
