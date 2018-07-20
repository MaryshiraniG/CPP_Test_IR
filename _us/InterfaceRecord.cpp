/*===================c++_source==============================START GLOBAL BANNER
(c) Copyright GLOBAL Travel Computer Services 1994, 1995, 1996, 1997, 1998, 1999,
															 2000, 2001
                SID: 1.227
      Delta Created: 17/10/18 - 15:12:28
         Last Delta: app/src/common/InterfaceRecord/_us 10/18/17 - 15:12:28 (/TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.InterfaceRecord.cpp) 
*/
#pragma comment ( copyright, "@(#)MATRIX{app/src/common/InterfaceRecord/_us}[10/18/17-15:12:28 1.227] /TaskEnv/sw/app/src/common/InterfaceRecord/_us/s.InterfaceRecord.cpp (Daniel Wu)\n" )
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
30APR2014   516462  c1433    SL  data standardization p3, 516462
24MAR2013   516409  v7848    WW  Parse money and time for LIMO
28OCT2013   515485  c1262    JL  fixing a EB issue at PNR level.
24OCT2013   514016  c1257    JL  retrofit version updated.
11SEP2013   514016  c1195    JL  addinb "MB" for moible booking.
16MAY2012   509845  c0572    WW  Process void ticket for CWT US M&G
============================================================================
*/

#define ENTER      LOG(logINFO) << "\n*** Enter>> " << __FUNCTION__
#define EXIT       LOG(logINFO) << "\n*** Exit << " << __FUNCTION__; return

#include "gtypes.h"
#include "InterfaceRecord_D.h"
#include "matsysT.h"
#include "MsgHandler.h"
#include "Money.h"
#include "dbraw.h"
#include "PNRDetail.h"
#include "PNR.h"
#include "dbIR.h"
#include "InterfaceRecord.h"
#include "PaymentForm.h"
#include "AccountingTxn.h"
#include "CFAId.h"
#include "CopComp.h"
#include "Receipt_D.h"
#include "Receipt.h"
#include "dbReceipt.h"
#include "Log.h"
#include "pcre_api.h"
#include "dbCCVendor.h"

extern "C" {
extern int
dbIR_ExecSqlStatement(char* stmt);
}

void Text2Decimal::add(set <Text2Decimal> &_set, const AB_Text key, const AB_Decimal value)
{
				set <Text2Decimal>::iterator it;
				it = _set.find(Text2Decimal(key));
				if ( it == _set.end() )
				{ 
				  _set.insert(Text2Decimal(key, value));
				}
				else
				{
				  it->add(value);
				}
}

/*
void Text2Decimal::add(set <Text2Decimal> &_set, const AB_Text key, const AB_Text value)
{
  add(_set, key, AB_Decimal(value));
}

void Text2Decimal::add(set <Text2Decimal> &_set, const AB_Varchar key, const AB_Varchar value)
{
  add(_set, AB_Text(key), AB_Decimal(value));
}
*/
string Text2Decimal::toString(string title, set <Text2Decimal> &_set)
{
	string retValue = "\n*** " + title + "  START *********\n";

	set <Text2Decimal>::iterator it;
	for( it = _set.begin(); it != _set.end(); it++ )
	{
				 retValue += "[";
				 retValue += it->_key.to_string();
				 retValue += "]->[";

				 retValue += it->_value.to_string();
				 retValue += "]\n";
	}
	retValue += "*** " + title + "  END *********\n";
	return retValue;
}


CInterfaceRecord::CInterfaceRecord() :	mva_surfaceTransport(255,100),
													mv_actionCode(2),
													mv_source(1),
													mv_reasonCode(10),
													mv_rejectirSub(10),
													mv_phoneField(200),
													mv_IRT_employee_number(10),
													gd_receiptType(3,100),
													gd_receiptData(500,50),
													mv_rec_queType(3),
													md_IRT_booking_pcc(10),
													md_IRT_booking_gds_agent(10),
													md_IRT_ticketing_pcc(10),
													md_IRT_ticketing_gds_agent(10)
{
  Log::ReportingLevel() = PARSER_LOG_LEVEL; // logDEBUG; PARSER_LOG_LEVEL;
   pRemarkParser = new RemarkParser();

   mi_IO_I    = 0 ;  // Other
   mi_IR_I    = 0 ;  // Rental
   mi_IA_I    = 0 ;  // Address
   mi_ICD_I   = 0 ;  // Consumedetail
   mi_IT_I    = 0 ;  // Transport
   mi_ITD_I   = 0 ;  // Irtaxdetail
   mi_IRF_I   = 0 ;  // Irfrequestuser
   mi_IP_I    = 0 ;  // Irperson
   mi_ID_I    = 0 ;  // Irticket
   mi_IRP_I   = 0 ;  // Irpayment
   mi_IRMK_I  = 0 ;  // Irremark
	mi_RC_I    = 0 ;  // Irreceipt
	mi_DI_I    = 0 ;  // Irdistribution
	mi_EM_I    = 0 ;
	mi_ISOC_I  = 0 ;  // Irstopovercity

	mv_actionCode = AB_null;
	mv_source     = AB_null;

   mi_rejectirId = AB_Integer(0);
   mv_rejectirSub = AB_null;

	mi_surfaceTransport = 1 ; // Surfare Transport G145

	// GAP US107 START
	md_ID_net_ticket_ind							= "N";
	md_ID_net_ticket_type						= "N";
	// GAP US107 END

	mi_receiptIndex = 0;

	mb_motli_branch = AB_false;

	mb_ticket4aofRecorded = AB_false;

	// QREX START
	mb_refundPNR = AB_false;
	// QREX END

   md_CCRFP_cc_id = AB_null;

	mb_itIsBatchTRM = AB_false;

   LOG(logINFO) << "CONSTRUCTOR InterfaceRecord"  ;
}

CInterfaceRecord::~CInterfaceRecord()
{
   LOG(logINFO) << "DESTRUCTOR InterfaceRecord"  ;
	LOG(logINFO) << pRemarkParser->toString().data() ;

}

void CInterfaceRecord::createQueue(D_description pd_errorDescription)
{
   CDIR             lo_dbRIR ;
   CIRReject        lo_createQueue ;
   AB_Date          ld_gmt;
	D_description    ld_errorDescription ;

   ld_gmt.getGMTNow() ;

	ld_errorDescription = pd_errorDescription ;

	setMsg(ld_errorDescription) ;

	if ( ld_errorDescription.IsNull() )
		CMsgHandler::getMsgDescription(3471, "E", ld_errorDescription) ;

	ld_errorDescription.leftJustify() ;

   lo_createQueue.pc_session_id = md_session_id ;
   lo_createQueue.pc_rejectCop = md_rejectCop;
   lo_createQueue.pc_rejectBra = md_rejectBra;
   lo_createQueue.pc_rejectCon = md_rejectCon;
   lo_createQueue.reference = md_IRT_reference ;
   lo_createQueue.gmt_date_time = ld_gmt ;
   lo_createQueue.iata_no = md_IRT_iata_no ;
   lo_createQueue.description = ld_errorDescription;
   lo_createQueue.queue_type = md_queType ;

   lo_dbRIR.reject(lo_createQueue) ;
}

void CInterfaceRecord::createClientfile()
{
   CDIR                lo_dbRIR ;
   CIRLoadClientFile   lo_LoadClientFile ;
   AB_Date             ld_gmt;

   ld_gmt.getGMTNow() ;

   lo_LoadClientFile.pc_session_id = md_session_id ;
   lo_LoadClientFile.pc_tran_id = md_txn_id;
   lo_LoadClientFile.pc_con_id = md_IRT_con_id ;
   lo_LoadClientFile.reference = md_IRT_reference ;
   lo_LoadClientFile.cfa_id = md_IRT_cfa_id ;
   lo_LoadClientFile.mktg_list = md_IRT_mktg_list ;
   lo_LoadClientFile.gmt_date_time = ld_gmt ;
   lo_LoadClientFile.pc_PNR_date = md_IRT_pnr_date ;
   lo_LoadClientFile.iata_no = md_IRT_iata_no ;

	// For US version Add cop_id and bra_id

   lo_LoadClientFile.pc_company = md_IRT_cop_id ;
   lo_LoadClientFile.pc_branch = md_IRT_bra_id ;
   lo_LoadClientFile.pi_rejectirId = mi_rejectirId;

   lo_dbRIR.loadClientFile(lo_LoadClientFile) ;

	md_cfa_type    = lo_LoadClientFile.pc_cfa_type ;
	md_cfa_code    = lo_LoadClientFile.pc_cfa_code ;
	md_cfa_id      = lo_LoadClientFile.pi_cfa_no ;
	md_last_name   = lo_LoadClientFile.pc_lname ;
	md_first_name  = lo_LoadClientFile.pc_fname ;
	md_con_id      = lo_LoadClientFile.pc_cfa_con ;

	if (lo_LoadClientFile.pc_rejectCop.NotIsNull() &&
		 lo_LoadClientFile.pc_rejectBra.NotIsNull()) {
		md_rejectCop   = lo_LoadClientFile.pc_rejectCop;
		md_rejectBra   = lo_LoadClientFile.pc_rejectBra;
	}

	md_rejectCon   = lo_LoadClientFile.pc_rejectCon;
	mi_rejectirId  = lo_LoadClientFile.pi_rejectirId;
	mv_rejectirSub = lo_LoadClientFile.pv_rejectirSub;

}

void CInterfaceRecord::initIraddress(CIRCreateIraddress &po_createIraddress)
{
   po_createIraddress.session_id = AB_null;
   po_createIraddress.type1 = AB_null;
   po_createIraddress.addr11 = AB_null;
   po_createIraddress.addr21 = AB_null;
   po_createIraddress.addr31 = AB_null;
   po_createIraddress.addr41 = AB_null;
   po_createIraddress.addr51 = AB_null;
   po_createIraddress.addr61 = AB_null;
   po_createIraddress.phone_acode1   = AB_null ;
   po_createIraddress.phone_no1      = AB_null ;
   po_createIraddress.bphone_acode1  = AB_null ;
   po_createIraddress.bphone_no1     = AB_null ;
   po_createIraddress.mphone_acode1  = AB_null ;
   po_createIraddress.bphone_no1     = AB_null ;
   po_createIraddress.fax_acode1     = AB_null ;
   po_createIraddress.fax_no1        = AB_null ;
   po_createIraddress.type2 = AB_null;
   po_createIraddress.addr12 = AB_null;
   po_createIraddress.addr22 = AB_null;
   po_createIraddress.addr32 = AB_null;
   po_createIraddress.addr42 = AB_null;
   po_createIraddress.addr52 = AB_null;
   po_createIraddress.addr62 = AB_null;
   po_createIraddress.phone_acode2   = AB_null ;
   po_createIraddress.phone_no2      = AB_null ;
   po_createIraddress.bphone_acode2  = AB_null ;
   po_createIraddress.bphone_no2     = AB_null ;
   po_createIraddress.mphone_acode2  = AB_null ;
   po_createIraddress.mphone_no2     = AB_null ;
   po_createIraddress.fax_acode2     = AB_null ;
   po_createIraddress.fax_no2        = AB_null ;
}

void CInterfaceRecord::initIrpayment(CIRCreateIrpayment &po_createIrpayment)
{
   po_createIrpayment.session_id     = AB_null;
   po_createIrpayment.pax_id1        = AB_null;
   po_createIrpayment.line_number1   = AB_null;
   po_createIrpayment.pay_id1        = AB_null;
   po_createIrpayment.amount1        = AB_null;
   po_createIrpayment.form_of_pay1   = AB_null;
   po_createIrpayment.cc_id1         = AB_null;
   po_createIrpayment.cc_no1         = AB_null;
   po_createIrpayment.cc_exp_date1   = AB_null;
   po_createIrpayment.auth_no1       = AB_null;
   po_createIrpayment.auth_source1   = AB_null;
   po_createIrpayment.pax_id2        = AB_null;
   po_createIrpayment.line_number2   = AB_null;
   po_createIrpayment.pay_id2        = AB_null;
   po_createIrpayment.amount2        = AB_null;
   po_createIrpayment.form_of_pay2   = AB_null;
   po_createIrpayment.cc_id2         = AB_null;
   po_createIrpayment.cc_no2         = AB_null;
   po_createIrpayment.cc_exp_date2   = AB_null;
   po_createIrpayment.auth_no2       = AB_null;
   po_createIrpayment.auth_source2   = AB_null;
   po_createIrpayment.pax_id3        = AB_null;
   po_createIrpayment.line_number3   = AB_null;
   po_createIrpayment.pay_id3        = AB_null;
   po_createIrpayment.amount3        = AB_null;
   po_createIrpayment.form_of_pay3   = AB_null;
   po_createIrpayment.cc_id3         = AB_null;
   po_createIrpayment.cc_no3         = AB_null;
   po_createIrpayment.cc_exp_date3   = AB_null;
   po_createIrpayment.auth_no3       = AB_null;
   po_createIrpayment.auth_source3   = AB_null;
   po_createIrpayment.pax_id4        = AB_null;
   po_createIrpayment.line_number4   = AB_null;
   po_createIrpayment.pay_id4        = AB_null;
   po_createIrpayment.amount4        = AB_null;
   po_createIrpayment.form_of_pay4   = AB_null;
   po_createIrpayment.cc_id4         = AB_null;
   po_createIrpayment.cc_no4         = AB_null;
   po_createIrpayment.cc_exp_date4   = AB_null;
   po_createIrpayment.auth_no4       = AB_null;
   po_createIrpayment.auth_source4   = AB_null;
   po_createIrpayment.pax_id5        = AB_null;
   po_createIrpayment.line_number5   = AB_null;
   po_createIrpayment.pay_id5        = AB_null;
   po_createIrpayment.amount5        = AB_null;
   po_createIrpayment.form_of_pay5   = AB_null;
   po_createIrpayment.cc_id5         = AB_null;
   po_createIrpayment.cc_no5         = AB_null;
   po_createIrpayment.cc_exp_date5   = AB_null;
   po_createIrpayment.auth_no5       = AB_null;
   po_createIrpayment.auth_source5   = AB_null;
}

void CInterfaceRecord::initIrtaxdetail(CIRCreateIrtaxdetail &po_createIrtaxdetail)
{
   po_createIrtaxdetail.session_id   = AB_null;
   po_createIrtaxdetail.pax_id1      = AB_null;
   po_createIrtaxdetail.line_number1 = AB_null;
   po_createIrtaxdetail.tax_id1      = AB_null;
   po_createIrtaxdetail.tax_source1  = AB_null;
   po_createIrtaxdetail.city_id1     = AB_null;
   po_createIrtaxdetail.tax_amount1  = AB_null;
   po_createIrtaxdetail.pax_id2      = AB_null;
   po_createIrtaxdetail.line_number2 = AB_null;
   po_createIrtaxdetail.tax_id2      = AB_null;
   po_createIrtaxdetail.tax_source2  = AB_null;
   po_createIrtaxdetail.city_id2     = AB_null;
   po_createIrtaxdetail.tax_amount2  = AB_null;
   po_createIrtaxdetail.pax_id3      = AB_null;
   po_createIrtaxdetail.line_number3 = AB_null;
   po_createIrtaxdetail.tax_id3      = AB_null;
   po_createIrtaxdetail.tax_source3  = AB_null;
   po_createIrtaxdetail.city_id3     = AB_null;
   po_createIrtaxdetail.tax_amount3  = AB_null;
   po_createIrtaxdetail.pax_id4      = AB_null;
   po_createIrtaxdetail.line_number4 = AB_null;
   po_createIrtaxdetail.tax_id4      = AB_null;
   po_createIrtaxdetail.tax_source4  = AB_null;
   po_createIrtaxdetail.city_id4     = AB_null;
   po_createIrtaxdetail.tax_amount4  = AB_null;
   po_createIrtaxdetail.pax_id5      = AB_null;
   po_createIrtaxdetail.line_number5 = AB_null;
   po_createIrtaxdetail.tax_id5      = AB_null;
   po_createIrtaxdetail.tax_source5  = AB_null;
   po_createIrtaxdetail.city_id5     = AB_null;
   po_createIrtaxdetail.tax_amount5  = AB_null;
   po_createIrtaxdetail.pax_id6      = AB_null;
   po_createIrtaxdetail.line_number6 = AB_null;
   po_createIrtaxdetail.tax_id6      = AB_null;
   po_createIrtaxdetail.tax_source6  = AB_null;
   po_createIrtaxdetail.city_id6     = AB_null;
   po_createIrtaxdetail.tax_amount6  = AB_null;
   po_createIrtaxdetail.pax_id7      = AB_null;
   po_createIrtaxdetail.line_number7 = AB_null;
   po_createIrtaxdetail.tax_id7      = AB_null;
   po_createIrtaxdetail.tax_source7  = AB_null;
   po_createIrtaxdetail.city_id7     = AB_null;
   po_createIrtaxdetail.tax_amount7  = AB_null;
   po_createIrtaxdetail.pax_id8      = AB_null;
   po_createIrtaxdetail.line_number8 = AB_null;
   po_createIrtaxdetail.tax_id8      = AB_null;
   po_createIrtaxdetail.tax_source8  = AB_null;
   po_createIrtaxdetail.city_id8     = AB_null;
   po_createIrtaxdetail.tax_amount8  = AB_null;
   po_createIrtaxdetail.pax_id9      = AB_null;
   po_createIrtaxdetail.line_number9 = AB_null;
   po_createIrtaxdetail.tax_id9      = AB_null;
   po_createIrtaxdetail.tax_source9  = AB_null;
   po_createIrtaxdetail.city_id9     = AB_null;
   po_createIrtaxdetail.tax_amount9  = AB_null;
   po_createIrtaxdetail.pax_id0      = AB_null;
   po_createIrtaxdetail.line_number0 = AB_null;
   po_createIrtaxdetail.tax_id0      = AB_null;
   po_createIrtaxdetail.tax_source0  = AB_null;
   po_createIrtaxdetail.city_id0     = AB_null;
   po_createIrtaxdetail.tax_amount0  = AB_null;
}

void CInterfaceRecord::initIrfrequentuser(CIRCreateIrfrequentuser &po_createIrfrequentuser)
{
   po_createIrfrequentuser.session_id = AB_null;
   po_createIrfrequentuser.pax_id1    = AB_null;
   po_createIrfrequentuser.sup_id1    = AB_null;
   po_createIrfrequentuser.fup_type1  = AB_null;
   po_createIrfrequentuser.fup1       = AB_null;
   po_createIrfrequentuser.fup_status1= AB_null;
   po_createIrfrequentuser.srvc_sup_id1= AB_null;
   po_createIrfrequentuser.pax_id2    = AB_null;
   po_createIrfrequentuser.sup_id2    = AB_null;
   po_createIrfrequentuser.fup_type2  = AB_null;
   po_createIrfrequentuser.fup2       = AB_null;
   po_createIrfrequentuser.fup_status2= AB_null;
   po_createIrfrequentuser.srvc_sup_id2= AB_null;
   po_createIrfrequentuser.pax_id3    = AB_null;
   po_createIrfrequentuser.sup_id3    = AB_null;
   po_createIrfrequentuser.fup_type3  = AB_null;
   po_createIrfrequentuser.fup3       = AB_null;
   po_createIrfrequentuser.fup_status3= AB_null;
   po_createIrfrequentuser.srvc_sup_id3= AB_null;
   po_createIrfrequentuser.pax_id4    = AB_null;
   po_createIrfrequentuser.sup_id4    = AB_null;
   po_createIrfrequentuser.fup_type4  = AB_null;
   po_createIrfrequentuser.fup4       = AB_null;
   po_createIrfrequentuser.fup_status4= AB_null;
   po_createIrfrequentuser.srvc_sup_id4= AB_null;
   po_createIrfrequentuser.pax_id5    = AB_null;
   po_createIrfrequentuser.sup_id5    = AB_null;
   po_createIrfrequentuser.fup_type5  = AB_null;
   po_createIrfrequentuser.fup5       = AB_null;
   po_createIrfrequentuser.fup_status5= AB_null;
   po_createIrfrequentuser.srvc_sup_id5= AB_null;
}

void CInterfaceRecord::initIrother(CIRCreateIrother &po_createIrother)
{
   po_createIrother.session_id    = AB_null;
   po_createIrother.itin_type1    = AB_null;
   po_createIrother.line_number1  = AB_null;
   po_createIrother.start_date1   = AB_null;
   po_createIrother.start_time1   = AB_null;
   po_createIrother.orig_city_id1 = AB_null;
   po_createIrother.end_date1     = AB_null;
   po_createIrother.end_time1     = AB_null;
   po_createIrother.dest_city_id1 = AB_null;
   po_createIrother.unit_type1    = AB_null;
   po_createIrother.unit_ct1      = AB_null;
   po_createIrother.sup_id1       = AB_null;
   po_createIrother.reference1    = AB_null;
   po_createIrother.saving_code1  = AB_null;
   po_createIrother.itin_type2    = AB_null;
   po_createIrother.line_number2  = AB_null;
   po_createIrother.start_date2   = AB_null;
   po_createIrother.start_time2   = AB_null;
   po_createIrother.orig_city_id2 = AB_null;
   po_createIrother.end_date2     = AB_null;
   po_createIrother.end_time2     = AB_null;
   po_createIrother.dest_city_id2 = AB_null;
   po_createIrother.unit_type2    = AB_null;
   po_createIrother.unit_ct2      = AB_null;
   po_createIrother.sup_id2       = AB_null;
   po_createIrother.reference2    = AB_null;
   po_createIrother.saving_code2  = AB_null;
   po_createIrother.itin_type3    = AB_null;
   po_createIrother.line_number3  = AB_null;
   po_createIrother.start_date3   = AB_null;
   po_createIrother.start_time3   = AB_null;
   po_createIrother.orig_city_id3 = AB_null;
   po_createIrother.end_date3     = AB_null;
   po_createIrother.end_time3     = AB_null;
   po_createIrother.dest_city_id3 = AB_null;
   po_createIrother.unit_type3    = AB_null;
   po_createIrother.unit_ct3      = AB_null;
   po_createIrother.sup_id3       = AB_null;
   po_createIrother.reference3    = AB_null;
   po_createIrother.saving_code3  = AB_null;
}

void CInterfaceRecord::initIrperson(CIRCreateIrperson &po_createIrperson)
{
   po_createIrperson.session_id    = AB_null;
   po_createIrperson.pax_id1       = AB_null;
   po_createIrperson.title1        = AB_null;
   po_createIrperson.last_name1    = AB_null;
   po_createIrperson.first_name1   = AB_null;
   po_createIrperson.new_pax_id1   = AB_null;
   po_createIrperson.new_pax_line1 = AB_null;
   po_createIrperson.sex1          = AB_null;
   po_createIrperson.smoking1      = AB_null;
   po_createIrperson.ctry_id1      = AB_null;
   po_createIrperson.passport_no1  = AB_null;
   po_createIrperson.passport_exp1 = AB_null;
   po_createIrperson.reference1    = AB_null;
   po_createIrperson.crs_invoice1  = AB_null;
   po_createIrperson.traveller_id1 = AB_null;
   po_createIrperson.pax_id2       = AB_null;
   po_createIrperson.title2        = AB_null;
   po_createIrperson.last_name2    = AB_null;
   po_createIrperson.first_name2   = AB_null;
   po_createIrperson.new_pax_id2   = AB_null;
   po_createIrperson.new_pax_line2 = AB_null;
   po_createIrperson.sex2          = AB_null;
   po_createIrperson.smoking2      = AB_null;
   po_createIrperson.ctry_id2      = AB_null;
   po_createIrperson.passport_no2  = AB_null;
   po_createIrperson.passport_exp2 = AB_null;
   po_createIrperson.reference2    = AB_null;
   po_createIrperson.crs_invoice2  = AB_null;
   po_createIrperson.traveller_id2 = AB_null;
   po_createIrperson.pax_id3       = AB_null;
   po_createIrperson.title3        = AB_null;
   po_createIrperson.last_name3    = AB_null;
   po_createIrperson.first_name3   = AB_null;
   po_createIrperson.new_pax_id3   = AB_null;
   po_createIrperson.new_pax_line3 = AB_null;
   po_createIrperson.sex3          = AB_null;
   po_createIrperson.smoking3      = AB_null;
   po_createIrperson.ctry_id3      = AB_null;
   po_createIrperson.passport_no3  = AB_null;
   po_createIrperson.passport_exp3 = AB_null;
   po_createIrperson.reference3    = AB_null;
   po_createIrperson.crs_invoice3  = AB_null;
   po_createIrperson.traveller_id3 = AB_null;
   po_createIrperson.pax_id4       = AB_null;
   po_createIrperson.title4        = AB_null;
   po_createIrperson.last_name4    = AB_null;
   po_createIrperson.first_name4   = AB_null;
   po_createIrperson.new_pax_id4   = AB_null;
   po_createIrperson.new_pax_line4 = AB_null;
   po_createIrperson.sex4          = AB_null;
   po_createIrperson.smoking4      = AB_null;
   po_createIrperson.ctry_id4      = AB_null;
   po_createIrperson.passport_no4  = AB_null;
   po_createIrperson.passport_exp4 = AB_null;
   po_createIrperson.reference4    = AB_null;
   po_createIrperson.crs_invoice4  = AB_null;
   po_createIrperson.traveller_id4 = AB_null;

}

void CInterfaceRecord::initIrticket(CIRCreateIrticket &po_createIrticket)
{
   po_createIrticket.session_id      = AB_null;
   po_createIrticket.cop_id          = AB_null;

   po_createIrticket.action_code     = AB_null; // NEW
   po_createIrticket.source          = AB_null; // NEW
   po_createIrticket.crs_id          = AB_null; // NEW

   po_createIrticket.pax_id1         = AB_null;
   po_createIrticket.line_number1    = AB_null;
   po_createIrticket.cost_line1      = AB_null;
   po_createIrticket.reference1      = AB_null;
   po_createIrticket.sup_id1         = AB_null;
   po_createIrticket.cur_id1         = AB_null;
   po_createIrticket.form1           = AB_null;
   po_createIrticket.ticket_no1      = AB_null;
   po_createIrticket.pic1            = AB_null;
   po_createIrticket.orig_ticket_no1 = AB_null;
   po_createIrticket.comm_rate1      = AB_null;
   po_createIrticket.commission1     = AB_null;
   po_createIrticket.no_of_books1    = AB_null;
   po_createIrticket.exchange_code1  = AB_null;
   po_createIrticket.penalty_ch_fee1 = AB_null;
   po_createIrticket.comm_penalty1   = AB_null;
   po_createIrticket.base_cost1      = AB_null;
   po_createIrticket.saving_code1    = AB_null;
   po_createIrticket.full_fare1      = AB_null;
   po_createIrticket.low_prac_fare1  = AB_null;
   po_createIrticket.personal_crs1   = AB_null;
   po_createIrticket.stp_home_city1  = AB_null;
   po_createIrticket.stp_host_city1  = AB_null;
   po_createIrticket.sel_access1     = AB_null;
   po_createIrticket.crs_tour1       = AB_null;
   po_createIrticket.rebate1         = AB_null;
   po_createIrticket.doc_type1       = AB_null;
   po_createIrticket.e_ticket1       = AB_null;
   po_createIrticket.pax_id2         = AB_null;
   po_createIrticket.line_number2    = AB_null;
   po_createIrticket.cost_line2      = AB_null;
   po_createIrticket.reference2      = AB_null;
   po_createIrticket.sup_id2         = AB_null;
   po_createIrticket.cur_id2         = AB_null;
   po_createIrticket.form2           = AB_null;
   po_createIrticket.ticket_no2      = AB_null;
   po_createIrticket.pic2            = AB_null;
   po_createIrticket.orig_ticket_no2 = AB_null;
   po_createIrticket.comm_rate2      = AB_null;
   po_createIrticket.commission2     = AB_null;
   po_createIrticket.no_of_books2    = AB_null;
   po_createIrticket.exchange_code2  = AB_null;
   po_createIrticket.penalty_ch_fee2 = AB_null;
   po_createIrticket.comm_penalty2   = AB_null;
   po_createIrticket.base_cost2      = AB_null;
   po_createIrticket.saving_code2    = AB_null;
   po_createIrticket.full_fare2      = AB_null;
   po_createIrticket.low_prac_fare2  = AB_null;
   po_createIrticket.personal_crs2   = AB_null;
   po_createIrticket.stp_home_city2  = AB_null;
   po_createIrticket.stp_host_city2  = AB_null;
   po_createIrticket.sel_access2     = AB_null;
   po_createIrticket.crs_tour2       = AB_null;
   po_createIrticket.rebate2         = AB_null;
   po_createIrticket.doc_type2       = AB_null;
   po_createIrticket.e_ticket2       = AB_null;
   po_createIrticket.pax_id3         = AB_null;
   po_createIrticket.line_number3    = AB_null;
   po_createIrticket.cost_line3      = AB_null;
   po_createIrticket.reference3      = AB_null;
   po_createIrticket.sup_id3         = AB_null;
   po_createIrticket.cur_id3         = AB_null;
   po_createIrticket.form3           = AB_null;
   po_createIrticket.ticket_no3      = AB_null;
   po_createIrticket.pic3            = AB_null;
   po_createIrticket.orig_ticket_no3 = AB_null;
   po_createIrticket.comm_rate3      = AB_null;
   po_createIrticket.commission3     = AB_null;
   po_createIrticket.no_of_books3    = AB_null;
   po_createIrticket.exchange_code3  = AB_null;
   po_createIrticket.penalty_ch_fee3 = AB_null;
   po_createIrticket.comm_penalty3   = AB_null;
   po_createIrticket.base_cost3      = AB_null;
   po_createIrticket.saving_code3    = AB_null;
   po_createIrticket.full_fare3      = AB_null;
   po_createIrticket.low_prac_fare3  = AB_null;
   po_createIrticket.personal_crs3   = AB_null;
   po_createIrticket.stp_home_city3  = AB_null;
   po_createIrticket.stp_host_city3  = AB_null;
   po_createIrticket.sel_access3     = AB_null;
   po_createIrticket.crs_tour3       = AB_null;
   po_createIrticket.rebate3         = AB_null;
   po_createIrticket.doc_type3       = AB_null;
   po_createIrticket.e_ticket3       = AB_null;
	po_createIrticket.commission_type					= AB_null;
	po_createIrticket.net_ticket_ind						= AB_null;
	po_createIrticket.nett_fare_to_airline				= AB_null;
	po_createIrticket.selling_price_to_shop			= AB_null;
	po_createIrticket.selling_price_to_client			= AB_null;
	po_createIrticket.tour_oper_commission				= AB_null;
	po_createIrticket.ticket_centre_reference			= AB_null;
	po_createIrticket.grid_reference						= AB_null;
	po_createIrticket.shop_reference						= AB_null;
	po_createIrticket.publish_fare_tc_commission		= AB_null;
	po_createIrticket.publish_fare_agent_commission	= AB_null;
}

void CInterfaceRecord::initIrconsumdetail(CIRCreateIrconsumdetail &po_createIrconsumdetail)
{
   po_createIrconsumdetail.session_id        = AB_null;
   po_createIrconsumdetail.pax_id1           = AB_null;
   po_createIrconsumdetail.itin_type1        = AB_null;
   po_createIrconsumdetail.itin_line_number1 = AB_null;
   po_createIrconsumdetail.seat1             = AB_null;
   po_createIrconsumdetail.connection1       = AB_null;
   po_createIrconsumdetail.smoking1          = AB_null;
   po_createIrconsumdetail.fare_by_leg1      = AB_null;
   po_createIrconsumdetail.fare_basis1       = AB_null;
   po_createIrconsumdetail.tkt_desig1        = AB_null;
   po_createIrconsumdetail.cog_seat1         = AB_null;
   po_createIrconsumdetail.line_number1      = AB_null;
   po_createIrconsumdetail.baggage1          = AB_null;
   po_createIrconsumdetail.crs_priced1       = AB_null;
   po_createIrconsumdetail.service_status1   = AB_null;
   po_createIrconsumdetail.ftp_miles1        = AB_null;
   po_createIrconsumdetail.boarding_pass1    = AB_null;
   po_createIrconsumdetail.special_meal1     = AB_null;
   po_createIrconsumdetail.special_need1     = AB_null;
   po_createIrconsumdetail.pax_id2           = AB_null;
   po_createIrconsumdetail.itin_type2        = AB_null;
   po_createIrconsumdetail.itin_line_number2 = AB_null;
   po_createIrconsumdetail.seat2             = AB_null;
   po_createIrconsumdetail.connection2       = AB_null;
   po_createIrconsumdetail.smoking2          = AB_null;
   po_createIrconsumdetail.fare_by_leg2      = AB_null;
   po_createIrconsumdetail.fare_basis2       = AB_null;
   po_createIrconsumdetail.tkt_desig2        = AB_null;
   po_createIrconsumdetail.cog_seat2         = AB_null;
   po_createIrconsumdetail.line_number2      = AB_null;
   po_createIrconsumdetail.baggage2          = AB_null;
   po_createIrconsumdetail.crs_priced2       = AB_null;
   po_createIrconsumdetail.service_status2   = AB_null;
   po_createIrconsumdetail.ftp_miles2        = AB_null;
   po_createIrconsumdetail.boarding_pass2    = AB_null;
   po_createIrconsumdetail.special_meal2     = AB_null;
   po_createIrconsumdetail.special_need2     = AB_null;
   po_createIrconsumdetail.pax_id3           = AB_null;
   po_createIrconsumdetail.itin_type3        = AB_null;
   po_createIrconsumdetail.itin_line_number3 = AB_null;
   po_createIrconsumdetail.seat3             = AB_null;
   po_createIrconsumdetail.connection3       = AB_null;
   po_createIrconsumdetail.smoking3          = AB_null;
   po_createIrconsumdetail.fare_by_leg3      = AB_null;
   po_createIrconsumdetail.fare_basis3       = AB_null;
   po_createIrconsumdetail.tkt_desig3        = AB_null;
   po_createIrconsumdetail.cog_seat3         = AB_null;
   po_createIrconsumdetail.line_number3      = AB_null;
   po_createIrconsumdetail.baggage3          = AB_null;
   po_createIrconsumdetail.crs_priced3       = AB_null;
   po_createIrconsumdetail.service_status3   = AB_null;
   po_createIrconsumdetail.ftp_miles3        = AB_null;
   po_createIrconsumdetail.boarding_pass3    = AB_null;
   po_createIrconsumdetail.special_meal3     = AB_null;
   po_createIrconsumdetail.special_need3     = AB_null;
   po_createIrconsumdetail.pax_id4           = AB_null;
   po_createIrconsumdetail.itin_type4        = AB_null;
   po_createIrconsumdetail.itin_line_number4 = AB_null;
   po_createIrconsumdetail.seat4             = AB_null;
   po_createIrconsumdetail.connection4       = AB_null;
   po_createIrconsumdetail.smoking4          = AB_null;
   po_createIrconsumdetail.fare_by_leg4      = AB_null;
   po_createIrconsumdetail.fare_basis4       = AB_null;
   po_createIrconsumdetail.tkt_desig4        = AB_null;
   po_createIrconsumdetail.cog_seat4         = AB_null;
   po_createIrconsumdetail.line_number4      = AB_null;
   po_createIrconsumdetail.baggage4          = AB_null;
   po_createIrconsumdetail.crs_priced4       = AB_null;
   po_createIrconsumdetail.service_status4   = AB_null;
   po_createIrconsumdetail.ftp_miles4        = AB_null;
   po_createIrconsumdetail.boarding_pass4    = AB_null;
   po_createIrconsumdetail.special_meal4     = AB_null;
   po_createIrconsumdetail.special_need4     = AB_null;
}

void CInterfaceRecord::initIrremark(CIRCreateIrremark &po_createIrremark)
{
   po_createIrremark.session_id = AB_null;
   po_createIrremark.line_number1 = AB_null;
   po_createIrremark.pax_id1 = AB_null;
   po_createIrremark.type1 = AB_null;
   po_createIrremark.remark1 = AB_null;
   po_createIrremark.itin_type1 = AB_null;
   po_createIrremark.tra_line_number1 = AB_null;
   po_createIrremark.ren_line_number1 = AB_null;
   po_createIrremark.oth_line_number1 = AB_null;
   po_createIrremark.udi_line_number1 = AB_null;
   po_createIrremark.line_number2 = AB_null;
   po_createIrremark.pax_id2 = AB_null;
   po_createIrremark.type2 = AB_null;
   po_createIrremark.remark2 = AB_null;
   po_createIrremark.itin_type2 = AB_null;
   po_createIrremark.tra_line_number2 = AB_null;
   po_createIrremark.ren_line_number2 = AB_null;
   po_createIrremark.oth_line_number2 = AB_null;
   po_createIrremark.udi_line_number2 = AB_null;
   po_createIrremark.line_number3 = AB_null;
   po_createIrremark.pax_id3 = AB_null;
   po_createIrremark.type3 = AB_null;
   po_createIrremark.remark3 = AB_null;
   po_createIrremark.itin_type3 = AB_null;
   po_createIrremark.tra_line_number3 = AB_null;
   po_createIrremark.ren_line_number3 = AB_null;
   po_createIrremark.oth_line_number3 = AB_null;
   po_createIrremark.udi_line_number3 = AB_null;
   po_createIrremark.line_number4 = AB_null;
   po_createIrremark.pax_id4 = AB_null;
   po_createIrremark.type4 = AB_null;
   po_createIrremark.remark4 = AB_null;
   po_createIrremark.itin_type4 = AB_null;
   po_createIrremark.tra_line_number4 = AB_null;
   po_createIrremark.ren_line_number4 = AB_null;
   po_createIrremark.oth_line_number4 = AB_null;
   po_createIrremark.udi_line_number4 = AB_null;
   po_createIrremark.line_number5 = AB_null;
   po_createIrremark.pax_id5 = AB_null;
   po_createIrremark.type5 = AB_null;
   po_createIrremark.remark5 = AB_null;
   po_createIrremark.itin_type5 = AB_null;
   po_createIrremark.tra_line_number5 = AB_null;
   po_createIrremark.ren_line_number5 = AB_null;
   po_createIrremark.oth_line_number5 = AB_null;
   po_createIrremark.udi_line_number5 = AB_null;
   po_createIrremark.line_number6 = AB_null;
   po_createIrremark.pax_id6 = AB_null;
   po_createIrremark.type6 = AB_null;
   po_createIrremark.remark6 = AB_null;
   po_createIrremark.itin_type6 = AB_null;
   po_createIrremark.tra_line_number6 = AB_null;
   po_createIrremark.ren_line_number6 = AB_null;
   po_createIrremark.oth_line_number6 = AB_null;
   po_createIrremark.udi_line_number6 = AB_null;
   po_createIrremark.line_number7 = AB_null;
   po_createIrremark.pax_id7 = AB_null;
   po_createIrremark.type7 = AB_null;
   po_createIrremark.remark7 = AB_null;
   po_createIrremark.itin_type7 = AB_null;
   po_createIrremark.tra_line_number7 = AB_null;
   po_createIrremark.ren_line_number7 = AB_null;
   po_createIrremark.oth_line_number7 = AB_null;
   po_createIrremark.udi_line_number7 = AB_null;
   po_createIrremark.line_number8 = AB_null;
   po_createIrremark.pax_id8 = AB_null;
   po_createIrremark.type8 = AB_null;
   po_createIrremark.remark8 = AB_null;
   po_createIrremark.itin_type8 = AB_null;
   po_createIrremark.tra_line_number8 = AB_null;
   po_createIrremark.ren_line_number8 = AB_null;
   po_createIrremark.oth_line_number8 = AB_null;
   po_createIrremark.udi_line_number8 = AB_null;
   po_createIrremark.line_number9 = AB_null;
   po_createIrremark.pax_id9 = AB_null;
   po_createIrremark.type9 = AB_null;
   po_createIrremark.remark9 = AB_null;
   po_createIrremark.itin_type9 = AB_null;
   po_createIrremark.tra_line_number9 = AB_null;
   po_createIrremark.ren_line_number9 = AB_null;
   po_createIrremark.oth_line_number9 = AB_null;
   po_createIrremark.udi_line_number9 = AB_null;
   po_createIrremark.line_number0 = AB_null;
   po_createIrremark.pax_id0 = AB_null;
   po_createIrremark.type0 = AB_null;
   po_createIrremark.remark0 = AB_null;
   po_createIrremark.itin_type0 = AB_null;
   po_createIrremark.tra_line_number0 = AB_null;
   po_createIrremark.ren_line_number0 = AB_null;
   po_createIrremark.oth_line_number0 = AB_null;
   po_createIrremark.udi_line_number0 = AB_null;
}

void CInterfaceRecord::initIrtransport(CIRCreateIrtransport &po_createIrtransport)
{
   po_createIrtransport.session_id      = AB_null;
   po_createIrtransport.itin_type       = AB_null;
   po_createIrtransport.line_number     = AB_null;
   po_createIrtransport.start_date      = AB_null;
   po_createIrtransport.start_time      = AB_null;
   po_createIrtransport.orig_city_id    = AB_null;
   po_createIrtransport.orig_cityname   = AB_null;
   po_createIrtransport.cog_dep_city    = AB_null;
   po_createIrtransport.cog_dep_date    = AB_null;
   po_createIrtransport.cog_dep_time    = AB_null;
   po_createIrtransport.cog_dep_term    = AB_null;
   po_createIrtransport.cog_dep_gate    = AB_null;
   po_createIrtransport.cog_of_miles    = AB_null;
   po_createIrtransport.end_time        = AB_null;
   po_createIrtransport.dest_city_id    = AB_null;
   po_createIrtransport.dest_cityname   = AB_null;
   po_createIrtransport.classofservice  = AB_null;
   po_createIrtransport.service         = AB_null;
   po_createIrtransport.sup_id          = AB_null;
   po_createIrtransport.reference       = AB_null;
   po_createIrtransport.no_of_stops     = AB_null;
   po_createIrtransport.dep_terminal    = AB_null;
   po_createIrtransport.dep_gate        = AB_null;
   po_createIrtransport.report_time     = AB_null;
   po_createIrtransport.arr_gate        = AB_null;
   po_createIrtransport.arr_terminal    = AB_null;
   po_createIrtransport.carrier_type    = AB_null;
   po_createIrtransport.meal            = AB_null;
   po_createIrtransport.aff_carrier     = AB_null;
   po_createIrtransport.actual_miles    = AB_null;
   po_createIrtransport.arrival_ind     = AB_null;
   po_createIrtransport.elapsed_time    = AB_null;
   po_createIrtransport.itin_type1      = AB_null;
   po_createIrtransport.line_number1    = AB_null;
   po_createIrtransport.start_date1     = AB_null;
   po_createIrtransport.start_time1     = AB_null;
   po_createIrtransport.orig_city_id1   = AB_null;
   po_createIrtransport.orig_cityname1  = AB_null;
   po_createIrtransport.cog_dep_city1   = AB_null;
   po_createIrtransport.cog_dep_date1   = AB_null;
   po_createIrtransport.cog_dep_time1   = AB_null;
   po_createIrtransport.cog_dep_term1   = AB_null;
   po_createIrtransport.cog_dep_gate1   = AB_null;
   po_createIrtransport.cog_of_miles1   = AB_null;
   po_createIrtransport.end_time1       = AB_null;
   po_createIrtransport.dest_city_id1   = AB_null;
   po_createIrtransport.dest_cityname1  = AB_null;
   po_createIrtransport.classofservice1 = AB_null;
   po_createIrtransport.service1        = AB_null;
   po_createIrtransport.sup_id1         = AB_null;
   po_createIrtransport.reference1      = AB_null;
   po_createIrtransport.no_of_stops1    = AB_null;
   po_createIrtransport.dep_terminal1   = AB_null;
   po_createIrtransport.dep_gate1       = AB_null;
   po_createIrtransport.report_time1    = AB_null;
   po_createIrtransport.arr_gate1       = AB_null;
   po_createIrtransport.arr_terminal1   = AB_null;
   po_createIrtransport.carrier_type1   = AB_null;
   po_createIrtransport.meal1           = AB_null;
   po_createIrtransport.aff_carrier1    = AB_null;
   po_createIrtransport.actual_miles1   = AB_null;
   po_createIrtransport.arrival_ind1    = AB_null;
   po_createIrtransport.elapsed_time1   = AB_null;
}

void CInterfaceRecord::initIremailaddress(CIRCreateIremailaddress &po_createIremailaddress)
{
   po_createIremailaddress.session_id 	= AB_null;
   po_createIremailaddress.email_id1 	= AB_null;
   po_createIremailaddress.email_type1 	= AB_null;
   po_createIremailaddress.email_id2 	= AB_null;
   po_createIremailaddress.email_type2 	= AB_null;
   po_createIremailaddress.email_id3 	= AB_null;
   po_createIremailaddress.email_type3 	= AB_null;
   po_createIremailaddress.email_id4 	= AB_null;
   po_createIremailaddress.email_type4 	= AB_null;
   po_createIremailaddress.email_id5 	= AB_null;
   po_createIremailaddress.email_type5 	= AB_null;
   po_createIremailaddress.email_id6 	= AB_null;
   po_createIremailaddress.email_type6 	= AB_null;
}

void CInterfaceRecord::initIrstopovercity(CIRCreateIrstopovercity &po_createIrstopovercity)
{
	po_createIrstopovercity.session_id		= AB_null;
	po_createIrstopovercity.itin_type1		= AB_null;
	po_createIrstopovercity.line_number1	= AB_null;
	po_createIrstopovercity.order_number1	= AB_null;
	po_createIrstopovercity.city_id1			= AB_null;

	po_createIrstopovercity.itin_type2		= AB_null;
	po_createIrstopovercity.line_number2	= AB_null;
	po_createIrstopovercity.order_number2	= AB_null;
	po_createIrstopovercity.city_id2			= AB_null;

	po_createIrstopovercity.itin_type3		= AB_null;
	po_createIrstopovercity.line_number3	= AB_null;
	po_createIrstopovercity.order_number3	= AB_null;
	po_createIrstopovercity.city_id3			= AB_null;

	po_createIrstopovercity.itin_type4		= AB_null;
	po_createIrstopovercity.line_number4	= AB_null;
	po_createIrstopovercity.order_number4	= AB_null;
	po_createIrstopovercity.city_id4			= AB_null;

	po_createIrstopovercity.itin_type5		= AB_null;
	po_createIrstopovercity.line_number5	= AB_null;
	po_createIrstopovercity.order_number5	= AB_null;
	po_createIrstopovercity.city_id5			= AB_null;

	po_createIrstopovercity.itin_type6		= AB_null;
	po_createIrstopovercity.line_number6	= AB_null;
	po_createIrstopovercity.order_number6	= AB_null;
	po_createIrstopovercity.city_id6			= AB_null;
}

void CInterfaceRecord::createIrtransport()
{
   ENTER;

   int                    li_counter ;
   int                    li_index ;
   CDIR                   dbIR ;
   CIRCreateIrtransport   lo_createIrtransport ;

   li_index = 0 ;
   li_counter = 0 ;

   while ((li_counter < mi_IT_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++ ;

      if (li_index == 1)
      {
         lo_createIrtransport.session_id     = md_session_id;
         lo_createIrtransport.itin_type      = md_IT_itin_type[li_counter];
         lo_createIrtransport.line_number    = md_IT_line_number[li_counter];
         lo_createIrtransport.start_date     = md_IT_start_date[li_counter];
         lo_createIrtransport.start_time     = md_IT_start_time[li_counter];
         lo_createIrtransport.orig_city_id   = md_IT_orig_city_id[li_counter];
         lo_createIrtransport.orig_cityname  = md_IT_orig_cityname[li_counter];
         lo_createIrtransport.cog_dep_city   = md_IT_cog_dep_city[li_counter];
         lo_createIrtransport.cog_dep_date   = md_IT_cog_dep_date[li_counter];
         lo_createIrtransport.cog_dep_time   = md_IT_cog_dep_time[li_counter];
         lo_createIrtransport.cog_dep_term   = md_IT_cog_dep_term[li_counter];
         lo_createIrtransport.cog_dep_gate   = md_IT_cog_dep_gate[li_counter];
         lo_createIrtransport.cog_of_miles   = md_IT_cog_of_miles[li_counter];
         lo_createIrtransport.end_date       = md_IT_end_date[li_counter];
         lo_createIrtransport.end_time       = md_IT_end_time[li_counter];
         lo_createIrtransport.dest_city_id   = md_IT_dest_city_id[li_counter];
         lo_createIrtransport.dest_cityname  = md_IT_dest_cityname[li_counter];
         lo_createIrtransport.classofservice = md_IT_classofservice[li_counter];
         lo_createIrtransport.service        = md_IT_service[li_counter];
         lo_createIrtransport.sup_id         = md_IT_sup_id[li_counter];
         lo_createIrtransport.reference      = md_IT_reference[li_counter];
         lo_createIrtransport.no_of_stops    = md_IT_no_of_stops[li_counter];
         lo_createIrtransport.dep_terminal   = md_IT_dep_terminal[li_counter];
         lo_createIrtransport.dep_gate       = md_IT_dep_gate[li_counter];
         lo_createIrtransport.report_time    = md_IT_report_time[li_counter];
         lo_createIrtransport.arr_gate       = md_IT_arr_gate[li_counter];
         lo_createIrtransport.arr_terminal   = md_IT_arr_terminal[li_counter];
         lo_createIrtransport.airline_locator= md_IT_airline_locator[li_counter];
         lo_createIrtransport.carrier_type   = md_IT_carrier_type[li_counter];
         lo_createIrtransport.meal           = md_IT_meal[li_counter];
         lo_createIrtransport.aff_carrier    = md_IT_aff_carrier[li_counter];
         lo_createIrtransport.actual_miles   = md_IT_actual_miles[li_counter];
         lo_createIrtransport.elapsed_time   = md_IT_elapsed_time[li_counter];
         lo_createIrtransport.arrival_ind    = md_IT_arrival_ind[li_counter++];
      }
      else if (li_index == 2)
      {
         lo_createIrtransport.itin_type1      = md_IT_itin_type[li_counter];
         lo_createIrtransport.line_number1    = md_IT_line_number[li_counter];
         lo_createIrtransport.start_date1     = md_IT_start_date[li_counter];
         lo_createIrtransport.start_time1     = md_IT_start_time[li_counter];
         lo_createIrtransport.orig_city_id1   = md_IT_orig_city_id[li_counter];
         lo_createIrtransport.orig_cityname1  = md_IT_orig_cityname[li_counter];
         lo_createIrtransport.cog_dep_city1   = md_IT_cog_dep_city[li_counter];
         lo_createIrtransport.cog_dep_date1   = md_IT_cog_dep_date[li_counter];
         lo_createIrtransport.cog_dep_time1   = md_IT_cog_dep_time[li_counter];
         lo_createIrtransport.cog_dep_term1   = md_IT_cog_dep_term[li_counter];
         lo_createIrtransport.cog_dep_gate1   = md_IT_cog_dep_gate[li_counter];
         lo_createIrtransport.cog_of_miles1   = md_IT_cog_of_miles[li_counter];
         lo_createIrtransport.end_date1       = md_IT_end_date[li_counter];
         lo_createIrtransport.end_time1       = md_IT_end_time[li_counter];
         lo_createIrtransport.dest_city_id1   = md_IT_dest_city_id[li_counter];
         lo_createIrtransport.dest_cityname1  = md_IT_dest_cityname[li_counter];
         lo_createIrtransport.classofservice1 = md_IT_classofservice[li_counter];
         lo_createIrtransport.service1        = md_IT_service[li_counter];
         lo_createIrtransport.sup_id1         = md_IT_sup_id[li_counter];
         lo_createIrtransport.reference1      = md_IT_reference[li_counter];
         lo_createIrtransport.no_of_stops1    = md_IT_no_of_stops[li_counter];
         lo_createIrtransport.dep_terminal1   = md_IT_dep_terminal[li_counter];
         lo_createIrtransport.dep_gate1       = md_IT_dep_gate[li_counter];
         lo_createIrtransport.report_time1    = md_IT_report_time[li_counter];
         lo_createIrtransport.arr_gate1       = md_IT_arr_gate[li_counter];
         lo_createIrtransport.arr_terminal1   = md_IT_arr_terminal[li_counter];
         lo_createIrtransport.airline_locator1= md_IT_airline_locator[li_counter];
         lo_createIrtransport.carrier_type1   = md_IT_carrier_type[li_counter];
         lo_createIrtransport.meal1           = md_IT_meal[li_counter];
         lo_createIrtransport.aff_carrier1    = md_IT_aff_carrier[li_counter];
         lo_createIrtransport.actual_miles1   = md_IT_actual_miles[li_counter];
         lo_createIrtransport.elapsed_time1   = md_IT_elapsed_time[li_counter];
         lo_createIrtransport.arrival_ind1    = md_IT_arrival_ind[li_counter++];

         dbIR.createIrtransport(lo_createIrtransport) ;
         initIrtransport(lo_createIrtransport) ;

         li_index = 0 ;
      }
   } // End of While Loop

   if (li_index > 0)
      dbIR.createIrtransport(lo_createIrtransport) ;

   EXIT;
}

void CInterfaceRecord::createIrremark()
{
	ENTER;

   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrremark      lo_createIrremark ;

   li_index = 0 ;
   li_counter = 0 ;

	validateCC_BINsInFeeRemarks();
	cleanUpAOFRemarksIfNeeded();

   while ((li_counter < mi_IRMK_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++;

      if (li_index == 1 )
      {
         lo_createIrremark.session_id       = md_session_id;
         lo_createIrremark.pax_id1          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type1            = md_IRMK_type[li_counter];
         lo_createIrremark.remark1          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type1       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number1 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number1 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number1 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number1 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number1     = li_counter ;
      }
      else if (li_index == 2)
      {
         lo_createIrremark.pax_id2          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type2            = md_IRMK_type[li_counter];
         lo_createIrremark.remark2          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type2       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number2 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number2 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number2 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number2 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number2     = li_counter ;
      }
      else if (li_index == 3)
      {
         lo_createIrremark.pax_id3          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type3            = md_IRMK_type[li_counter];
         lo_createIrremark.remark3          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type3       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number3 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number3 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number3 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number3 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number3     = li_counter ;
      }
      else if (li_index == 4)
      {
         lo_createIrremark.pax_id4          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type4            = md_IRMK_type[li_counter];
         lo_createIrremark.remark4          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type4       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number4 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number4 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number4 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number4 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number4     = li_counter ;
      }
      else if (li_index == 5)
      {
         lo_createIrremark.pax_id5          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type5            = md_IRMK_type[li_counter];
         lo_createIrremark.remark5          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type5       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number5 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number5 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number5 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number5 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number5     = li_counter ;
      }
      else if (li_index == 6)
      {
         lo_createIrremark.pax_id6          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type6            = md_IRMK_type[li_counter];
         lo_createIrremark.remark6          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type6       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number6 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number6 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number6 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number6 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number6     = li_counter ;
      }
      else if (li_index == 7)
      {
         lo_createIrremark.pax_id7          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type7            = md_IRMK_type[li_counter];
         lo_createIrremark.remark7          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type7       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number7 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number7 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number7 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number7 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number7     = li_counter ;
      }
      else if (li_index == 8)
      {
         lo_createIrremark.pax_id8          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type8            = md_IRMK_type[li_counter];
         lo_createIrremark.remark8          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type8       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number8 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number8 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number8 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number8 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number8     = li_counter ;
      }
      else if (li_index == 9)
      {
         lo_createIrremark.pax_id9          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type9            = md_IRMK_type[li_counter];
         lo_createIrremark.remark9          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type9       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number9 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number9 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number9 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number9 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number9     = li_counter ;
      }
      else if (li_index == 10)
      {
         lo_createIrremark.pax_id0          = md_IRMK_pax_id[li_counter];
         lo_createIrremark.type0            = md_IRMK_type[li_counter];
         lo_createIrremark.remark0          = md_IRMK_remark[li_counter];
         lo_createIrremark.itin_type0       = md_IRMK_itin_type[li_counter];
   		lo_createIrremark.tra_line_number0 = md_IRMK_tra_line_number[li_counter];
   		lo_createIrremark.ren_line_number0 = md_IRMK_ren_line_number[li_counter];
   		lo_createIrremark.oth_line_number0 = md_IRMK_oth_line_number[li_counter];
   		lo_createIrremark.udi_line_number0 = md_IRMK_udi_line_number[li_counter++];
         lo_createIrremark.line_number0     = li_counter ;

         dbIR.createIrremark(lo_createIrremark) ;
         initIrremark(lo_createIrremark) ;

         li_index = 0 ;
      }
   }

   if (li_index > 0)
      dbIR.createIrremark(lo_createIrremark) ;

   EXIT;
}


void CInterfaceRecord::createIrconsumdetail()
{
	ENTER;

   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrconsumdetail  lo_createIrconsumdetail ;

   li_index = 0 ;
   li_counter = 0 ;
   while ((li_counter < mi_ICD_I) && (CMsgHandler::getMsgStatus()))
   {
		if ( md_ICD_pax_id[li_counter].NotIsNull() )
		{
      li_index++ ;

      if (li_index == 1)
      {
         lo_createIrconsumdetail.session_id        = md_session_id ;
         lo_createIrconsumdetail.pax_id1           = md_ICD_pax_id[li_counter];
   		lo_createIrconsumdetail.line_number1      = md_ICD_line_number[li_counter];
         lo_createIrconsumdetail.itin_type1        = md_ICD_itin_type[li_counter];
         lo_createIrconsumdetail.itin_line_number1 = md_ICD_itin_line_number[li_counter];
         lo_createIrconsumdetail.seat1             = md_ICD_seat[li_counter];
         lo_createIrconsumdetail.connection1       = md_ICD_connection[li_counter];
         lo_createIrconsumdetail.smoking1          = md_ICD_smoking[li_counter];
         lo_createIrconsumdetail.fare_by_leg1      = md_ICD_fare_by_leg[li_counter];
         lo_createIrconsumdetail.fare_basis1       = md_ICD_fare_basis[li_counter];
         lo_createIrconsumdetail.tkt_desig1        = md_ICD_tkt_desig[li_counter];
         lo_createIrconsumdetail.cog_seat1         = md_ICD_cog_seat[li_counter];
   		lo_createIrconsumdetail.baggage1          = md_ICD_baggage[li_counter];
   		lo_createIrconsumdetail.crs_priced1       = md_ICD_crs_priced[li_counter];
   		lo_createIrconsumdetail.service_status1   = md_ICD_service_status[li_counter];
   		lo_createIrconsumdetail.ftp_miles1        = md_ICD_ftp_miles[li_counter];
   		lo_createIrconsumdetail.boarding_pass1    = md_ICD_boarding_pass[li_counter];
   		lo_createIrconsumdetail.special_meal1     = md_ICD_special_meal[li_counter];
   		lo_createIrconsumdetail.special_need1     = md_ICD_special_need[li_counter];
   		lo_createIrconsumdetail.cur_id1           = md_ICD_cur_id[li_counter++];
      }
      else if (li_index == 2)
      {
         lo_createIrconsumdetail.session_id        = md_session_id ;
         lo_createIrconsumdetail.pax_id2           = md_ICD_pax_id[li_counter];
   		lo_createIrconsumdetail.line_number2      = md_ICD_line_number[li_counter];
         lo_createIrconsumdetail.itin_type2        = md_ICD_itin_type[li_counter];
         lo_createIrconsumdetail.itin_line_number2 = md_ICD_itin_line_number[li_counter];
         lo_createIrconsumdetail.seat2             = md_ICD_seat[li_counter];
         lo_createIrconsumdetail.connection2       = md_ICD_connection[li_counter];
         lo_createIrconsumdetail.smoking2          = md_ICD_smoking[li_counter];
         lo_createIrconsumdetail.fare_by_leg2      = md_ICD_fare_by_leg[li_counter];
         lo_createIrconsumdetail.fare_basis2       = md_ICD_fare_basis[li_counter];
         lo_createIrconsumdetail.tkt_desig2        = md_ICD_tkt_desig[li_counter];
         lo_createIrconsumdetail.cog_seat2         = md_ICD_cog_seat[li_counter];
   		lo_createIrconsumdetail.baggage2          = md_ICD_baggage[li_counter];
   		lo_createIrconsumdetail.crs_priced2       = md_ICD_crs_priced[li_counter];
   		lo_createIrconsumdetail.service_status2   = md_ICD_service_status[li_counter];
   		lo_createIrconsumdetail.ftp_miles2        = md_ICD_ftp_miles[li_counter];
   		lo_createIrconsumdetail.boarding_pass2    = md_ICD_boarding_pass[li_counter];
   		lo_createIrconsumdetail.special_meal2     = md_ICD_special_meal[li_counter];
   		lo_createIrconsumdetail.special_need2     = md_ICD_special_need[li_counter];
   		lo_createIrconsumdetail.cur_id2           = md_ICD_cur_id[li_counter++];
      }
      else if (li_index == 3)
      {
         lo_createIrconsumdetail.session_id        = md_session_id ;
         lo_createIrconsumdetail.pax_id3           = md_ICD_pax_id[li_counter];
   		lo_createIrconsumdetail.line_number3      = md_ICD_line_number[li_counter];
         lo_createIrconsumdetail.itin_type3        = md_ICD_itin_type[li_counter];
         lo_createIrconsumdetail.itin_line_number3 = md_ICD_itin_line_number[li_counter];
         lo_createIrconsumdetail.seat3             = md_ICD_seat[li_counter];
         lo_createIrconsumdetail.connection3       = md_ICD_connection[li_counter];
         lo_createIrconsumdetail.smoking3          = md_ICD_smoking[li_counter];
         lo_createIrconsumdetail.fare_by_leg3      = md_ICD_fare_by_leg[li_counter];
         lo_createIrconsumdetail.fare_basis3       = md_ICD_fare_basis[li_counter];
         lo_createIrconsumdetail.tkt_desig3        = md_ICD_tkt_desig[li_counter];
         lo_createIrconsumdetail.cog_seat3         = md_ICD_cog_seat[li_counter];
   		lo_createIrconsumdetail.baggage3          = md_ICD_baggage[li_counter];
   		lo_createIrconsumdetail.crs_priced3       = md_ICD_crs_priced[li_counter];
   		lo_createIrconsumdetail.service_status3   = md_ICD_service_status[li_counter];
   		lo_createIrconsumdetail.ftp_miles3        = md_ICD_ftp_miles[li_counter];
   		lo_createIrconsumdetail.boarding_pass3    = md_ICD_boarding_pass[li_counter];
   		lo_createIrconsumdetail.special_meal3     = md_ICD_special_meal[li_counter];
   		lo_createIrconsumdetail.special_need3     = md_ICD_special_need[li_counter];
   		lo_createIrconsumdetail.cur_id3           = md_ICD_cur_id[li_counter++];
      }
      else if (li_index == 4)
      {
         lo_createIrconsumdetail.session_id        = md_session_id ;
         lo_createIrconsumdetail.pax_id4           = md_ICD_pax_id[li_counter];
   		lo_createIrconsumdetail.line_number4      = md_ICD_line_number[li_counter];
         lo_createIrconsumdetail.itin_type4        = md_ICD_itin_type[li_counter];
         lo_createIrconsumdetail.itin_line_number4 = md_ICD_itin_line_number[li_counter];
         lo_createIrconsumdetail.seat4             = md_ICD_seat[li_counter];
         lo_createIrconsumdetail.connection4       = md_ICD_connection[li_counter];
         lo_createIrconsumdetail.smoking4          = md_ICD_smoking[li_counter];
         lo_createIrconsumdetail.fare_by_leg4      = md_ICD_fare_by_leg[li_counter];
         lo_createIrconsumdetail.fare_basis4       = md_ICD_fare_basis[li_counter];
         lo_createIrconsumdetail.tkt_desig4        = md_ICD_tkt_desig[li_counter];
         lo_createIrconsumdetail.cog_seat4         = md_ICD_cog_seat[li_counter];
   		lo_createIrconsumdetail.baggage4          = md_ICD_baggage[li_counter];
   		lo_createIrconsumdetail.crs_priced4       = md_ICD_crs_priced[li_counter];
   		lo_createIrconsumdetail.service_status4   = md_ICD_service_status[li_counter];
   		lo_createIrconsumdetail.ftp_miles4        = md_ICD_ftp_miles[li_counter];
   		lo_createIrconsumdetail.boarding_pass4    = md_ICD_boarding_pass[li_counter];
   		lo_createIrconsumdetail.special_meal4     = md_ICD_special_meal[li_counter];
   		lo_createIrconsumdetail.special_need4     = md_ICD_special_need[li_counter];
   		lo_createIrconsumdetail.cur_id4           = md_ICD_cur_id[li_counter++];

         dbIR.createIrconsumdetail(lo_createIrconsumdetail) ;
         initIrconsumdetail(lo_createIrconsumdetail) ;

         li_index = 0 ;
      }
   	}
		else
		{
			li_counter++ ;
		}
   }

   if (li_index > 0)
      dbIR.createIrconsumdetail(lo_createIrconsumdetail) ;
   EXIT;
}

void CInterfaceRecord::createIrperson()
{
	ENTER;
   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrperson       lo_createIrperson ;
   int                     li_pax_created = 0 ;

   li_index = 0 ;
   li_counter = 0 ;
   while ((li_counter < mi_IP_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++ ;

      if (li_index == 1)
      {
   		lo_createIrperson.session_id    = md_session_id;
   		lo_createIrperson.pax_id1       = md_IP_pax_id[li_counter];
   		lo_createIrperson.title1        = md_IP_title[li_counter];
   		lo_createIrperson.last_name1    = md_IP_last_name[li_counter];
   		lo_createIrperson.first_name1   = md_IP_first_name[li_counter];
   		lo_createIrperson.new_pax_id1   = md_IP_new_pax_id[li_counter];
   		lo_createIrperson.new_pax_line1 = md_IP_new_pax_line[li_counter];
   		lo_createIrperson.sex1          = md_IP_sex[li_counter];
   		lo_createIrperson.smoking1      = md_IP_smoking[li_counter];
   		lo_createIrperson.ctry_id1      = md_IP_ctry_id[li_counter];
   		lo_createIrperson.passport_no1  = md_IP_passport_no[li_counter];
   		lo_createIrperson.passport_exp1 = md_IP_passport_exp[li_counter];
   		lo_createIrperson.reference1    = md_IP_reference[li_counter];
   		lo_createIrperson.crs_invoice1  = md_IP_crs_invoice[li_counter++];
   		lo_createIrperson.traveller_id1 = li_counter;
      }
      else if (li_index == 2)
		{
   		lo_createIrperson.session_id    = md_session_id;
   		lo_createIrperson.pax_id2       = md_IP_pax_id[li_counter];
   		lo_createIrperson.title2        = md_IP_title[li_counter];
   		lo_createIrperson.last_name2    = md_IP_last_name[li_counter];
   		lo_createIrperson.first_name2   = md_IP_first_name[li_counter];
   		lo_createIrperson.new_pax_id2   = md_IP_new_pax_id[li_counter];
   		lo_createIrperson.new_pax_line2 = md_IP_new_pax_line[li_counter];
   		lo_createIrperson.sex2          = md_IP_sex[li_counter];
   		lo_createIrperson.smoking2      = md_IP_smoking[li_counter];
   		lo_createIrperson.ctry_id2      = md_IP_ctry_id[li_counter];
   		lo_createIrperson.passport_no2  = md_IP_passport_no[li_counter];
   		lo_createIrperson.passport_exp2 = md_IP_passport_exp[li_counter];
   		lo_createIrperson.reference2    = md_IP_reference[li_counter];
   		lo_createIrperson.crs_invoice2  = md_IP_crs_invoice[li_counter++];
   		lo_createIrperson.traveller_id2 = li_counter;
		}
      else if (li_index == 3)
		{
   		lo_createIrperson.session_id    = md_session_id;
   		lo_createIrperson.pax_id3       = md_IP_pax_id[li_counter];
   		lo_createIrperson.title3        = md_IP_title[li_counter];
   		lo_createIrperson.last_name3    = md_IP_last_name[li_counter];
   		lo_createIrperson.first_name3   = md_IP_first_name[li_counter];
   		lo_createIrperson.new_pax_id3   = md_IP_new_pax_id[li_counter];
   		lo_createIrperson.new_pax_line3 = md_IP_new_pax_line[li_counter];
   		lo_createIrperson.sex3          = md_IP_sex[li_counter];
   		lo_createIrperson.smoking3      = md_IP_smoking[li_counter];
   		lo_createIrperson.ctry_id3      = md_IP_ctry_id[li_counter];
   		lo_createIrperson.passport_no3  = md_IP_passport_no[li_counter];
   		lo_createIrperson.passport_exp3 = md_IP_passport_exp[li_counter];
   		lo_createIrperson.reference3    = md_IP_reference[li_counter];
   		lo_createIrperson.crs_invoice3  = md_IP_crs_invoice[li_counter++];
   		lo_createIrperson.traveller_id3 = li_counter;
		}
      else if (li_index == 4)
      {
   		lo_createIrperson.session_id    = md_session_id;
   		lo_createIrperson.pax_id4       = md_IP_pax_id[li_counter];
   		lo_createIrperson.title4        = md_IP_title[li_counter];
   		lo_createIrperson.last_name4    = md_IP_last_name[li_counter];
   		lo_createIrperson.first_name4   = md_IP_first_name[li_counter];
   		lo_createIrperson.new_pax_id4   = md_IP_new_pax_id[li_counter];
   		lo_createIrperson.new_pax_line4 = md_IP_new_pax_line[li_counter];
   		lo_createIrperson.sex4          = md_IP_sex[li_counter];
   		lo_createIrperson.smoking4      = md_IP_smoking[li_counter];
   		lo_createIrperson.ctry_id4      = md_IP_ctry_id[li_counter];
   		lo_createIrperson.passport_no4  = md_IP_passport_no[li_counter];
   		lo_createIrperson.passport_exp4 = md_IP_passport_exp[li_counter];
   		lo_createIrperson.reference4    = md_IP_reference[li_counter];
   		lo_createIrperson.crs_invoice4  = md_IP_crs_invoice[li_counter++];
   		lo_createIrperson.traveller_id4 = li_counter;

         dbIR.createIrperson(lo_createIrperson) ;
         initIrperson(lo_createIrperson) ;

         li_index = 0 ;
			li_pax_created = 1;
      }

   }
   if (li_index > 0)
	{
      dbIR.createIrperson(lo_createIrperson) ;
   } else {
      // because new clientfile will be created later, there must be
      // passengers
		if ( !li_pax_created )
      	CMsgHandler::setMsg(3471, " ", "CRIRPERSON", 1, AB_Varchar(20,AB_null));
   }

	EXIT;
}

void CInterfaceRecord::createIrticket()
{
	ENTER;

   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrticket lo_createIrticket ;

   li_index = 0 ;
   li_counter = 0 ;
   while ((li_counter < mi_ID_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++ ;

   	lo_createIrticket.session_id  = md_session_id;
   	lo_createIrticket.cop_id      = md_IRT_cop_id;

      cout << "FG: CInterfaceRecord::createIrticket: mv_actionCode= " << mv_actionCode << flush << endl;

      lo_createIrticket.action_code = mv_actionCode; // NEW
      lo_createIrticket.source      = mv_source;     // NEW
      lo_createIrticket.reason_code = mv_reasonCode; // NEW
      lo_createIrticket.crs_id      = md_IRT_crs_id; // NEW

		// GAP US107 START
		lo_createIrticket.commission_type					= md_ID_commission_type;
		lo_createIrticket.net_ticket_ind						= md_ID_net_ticket_ind;
		lo_createIrticket.net_ticket_type					= md_ID_net_ticket_type;
		lo_createIrticket.nett_fare_to_airline				= md_ID_nett_fare_to_airline;
		lo_createIrticket.selling_price_to_shop			= md_ID_selling_price_to_shop;
		lo_createIrticket.selling_price_to_client			= md_ID_selling_price_to_client;
		lo_createIrticket.tour_oper_commission				= md_ID_tour_oper_commission;
		lo_createIrticket.ticket_centre_reference			= md_ID_ticket_centre_reference;
		lo_createIrticket.grid_reference						= md_ID_grid_reference;
		lo_createIrticket.shop_reference						= md_ID_shop_reference;
		lo_createIrticket.publish_fare_tc_commission		= md_ID_publish_fare_tc_commission;
		lo_createIrticket.publish_fare_agent_commission	= md_ID_publish_fare_agent_commission;
		// GAP US107 END

      if (li_index == 1)
      {
   		lo_createIrticket.pax_id1         = md_ID_pax_id[li_counter];
   		lo_createIrticket.cost_line1      = md_ID_cost_line[li_counter];
   		lo_createIrticket.reference1      = md_ID_reference[li_counter];
   		lo_createIrticket.sup_id1         = md_ID_sup_id[li_counter];
   		lo_createIrticket.cur_id1         = md_ID_cur_id[li_counter];
   		lo_createIrticket.form1           = md_ID_form[li_counter];
   		lo_createIrticket.ticket_no1      = md_ID_ticket_no[li_counter];
   		lo_createIrticket.pic1            = md_ID_pic[li_counter];
   		lo_createIrticket.orig_ticket_no1 = md_ID_orig_ticket_no[li_counter];
   		lo_createIrticket.comm_rate1      = md_ID_comm_rate[li_counter];
   		lo_createIrticket.commission1     = md_ID_commission[li_counter];
   		lo_createIrticket.no_of_books1    = md_ID_no_of_books[li_counter];
   		lo_createIrticket.exchange_code1  = md_ID_exchange_code[li_counter];
   		lo_createIrticket.penalty_ch_fee1 = md_ID_penalty_ch_fee[li_counter];
   		lo_createIrticket.comm_penalty1   = md_ID_comm_penalty[li_counter];
   		lo_createIrticket.base_cost1      = md_ID_base_cost[li_counter];
   		lo_createIrticket.saving_code1    = md_ID_saving_code[li_counter];
   		lo_createIrticket.full_fare1      = md_ID_full_fare[li_counter];
   		lo_createIrticket.low_prac_fare1  = md_ID_low_prac_fare[li_counter];
   		lo_createIrticket.personal_crs1   = md_ID_personal_crs[li_counter];
   		lo_createIrticket.stp_home_city1  = md_ID_stp_home_city[li_counter];
   		lo_createIrticket.stp_host_city1  = md_ID_stp_host_city[li_counter];
   		lo_createIrticket.sel_access1     = md_ID_sel_access[li_counter];
   		lo_createIrticket.crs_tour1       = md_ID_crs_tour[li_counter];
   		lo_createIrticket.rebate1         = md_ID_rebate[li_counter];
   		lo_createIrticket.doc_type1       = md_ID_doc_type[li_counter];
   		lo_createIrticket.e_ticket1       = md_ID_e_ticket[li_counter];
			// QREX START
			lo_createIrticket.refund1			 = md_ID_refund[li_counter];
			// QREX END

			li_counter++;
   		lo_createIrticket.line_number1    = li_counter;
      }
      else if (li_index == 2)
      {
   		lo_createIrticket.pax_id2         = md_ID_pax_id[li_counter];
   		lo_createIrticket.cost_line2      = md_ID_cost_line[li_counter];
   		lo_createIrticket.reference2      = md_ID_reference[li_counter];
   		lo_createIrticket.sup_id2         = md_ID_sup_id[li_counter];
   		lo_createIrticket.cur_id2         = md_ID_cur_id[li_counter];
   		lo_createIrticket.form2           = md_ID_form[li_counter];
   		lo_createIrticket.ticket_no2      = md_ID_ticket_no[li_counter];
   		lo_createIrticket.pic2            = md_ID_pic[li_counter];
   		lo_createIrticket.orig_ticket_no2 = md_ID_orig_ticket_no[li_counter];
   		lo_createIrticket.comm_rate2      = md_ID_comm_rate[li_counter];
   		lo_createIrticket.commission2     = md_ID_commission[li_counter];
   		lo_createIrticket.no_of_books2    = md_ID_no_of_books[li_counter];
   		lo_createIrticket.exchange_code2  = md_ID_exchange_code[li_counter];
   		lo_createIrticket.penalty_ch_fee2 = md_ID_penalty_ch_fee[li_counter];
   		lo_createIrticket.comm_penalty2   = md_ID_comm_penalty[li_counter];
   		lo_createIrticket.base_cost2      = md_ID_base_cost[li_counter];
   		lo_createIrticket.saving_code2    = md_ID_saving_code[li_counter];
   		lo_createIrticket.full_fare2      = md_ID_full_fare[li_counter];
   		lo_createIrticket.low_prac_fare2  = md_ID_low_prac_fare[li_counter];
   		lo_createIrticket.personal_crs2   = md_ID_personal_crs[li_counter];
   		lo_createIrticket.stp_home_city2  = md_ID_stp_home_city[li_counter];
   		lo_createIrticket.stp_host_city2  = md_ID_stp_host_city[li_counter];
   		lo_createIrticket.sel_access2     = md_ID_sel_access[li_counter];
   		lo_createIrticket.crs_tour2       = md_ID_crs_tour[li_counter];
   		lo_createIrticket.rebate2         = md_ID_rebate[li_counter];
   		lo_createIrticket.doc_type2       = md_ID_doc_type[li_counter];
   		lo_createIrticket.e_ticket2       = md_ID_e_ticket[li_counter];
			// QREX START
			lo_createIrticket.refund2			 = md_ID_refund[li_counter];
			// QREX END

			li_counter++;
   		lo_createIrticket.line_number2    = li_counter;
		}
      else if (li_index == 3)
      {
   		lo_createIrticket.pax_id3         = md_ID_pax_id[li_counter];
   		lo_createIrticket.cost_line3      = md_ID_cost_line[li_counter];
   		lo_createIrticket.reference3      = md_ID_reference[li_counter];
   		lo_createIrticket.sup_id3         = md_ID_sup_id[li_counter];
   		lo_createIrticket.cur_id3         = md_ID_cur_id[li_counter];
   		lo_createIrticket.form3           = md_ID_form[li_counter];
   		lo_createIrticket.ticket_no3      = md_ID_ticket_no[li_counter];
   		lo_createIrticket.pic3            = md_ID_pic[li_counter];
   		lo_createIrticket.orig_ticket_no3 = md_ID_orig_ticket_no[li_counter];
   		lo_createIrticket.comm_rate3      = md_ID_comm_rate[li_counter];
   		lo_createIrticket.commission3     = md_ID_commission[li_counter];
   		lo_createIrticket.no_of_books3    = md_ID_no_of_books[li_counter];
   		lo_createIrticket.exchange_code3  = md_ID_exchange_code[li_counter];
   		lo_createIrticket.penalty_ch_fee3 = md_ID_penalty_ch_fee[li_counter];
   		lo_createIrticket.comm_penalty3   = md_ID_comm_penalty[li_counter];
   		lo_createIrticket.base_cost3      = md_ID_base_cost[li_counter];
   		lo_createIrticket.saving_code3    = md_ID_saving_code[li_counter];
   		lo_createIrticket.full_fare3      = md_ID_full_fare[li_counter];
   		lo_createIrticket.low_prac_fare3  = md_ID_low_prac_fare[li_counter];
   		lo_createIrticket.personal_crs3   = md_ID_personal_crs[li_counter];
   		lo_createIrticket.stp_home_city3  = md_ID_stp_home_city[li_counter];
   		lo_createIrticket.stp_host_city3  = md_ID_stp_host_city[li_counter];
   		lo_createIrticket.sel_access3     = md_ID_sel_access[li_counter];
   		lo_createIrticket.crs_tour3       = md_ID_crs_tour[li_counter];
   		lo_createIrticket.rebate3         = md_ID_rebate[li_counter];
   		lo_createIrticket.doc_type3       = md_ID_doc_type[li_counter];
   		lo_createIrticket.e_ticket3       = md_ID_e_ticket[li_counter];
			// QREX START
			lo_createIrticket.refund3			 = md_ID_refund[li_counter];
			// QREX END

			li_counter++;
   		lo_createIrticket.line_number3    = li_counter;

         dbIR.createIrticket(lo_createIrticket) ;
         initIrticket(lo_createIrticket) ;

         li_index = 0 ;
      }

   }

   if (li_index > 0)
      dbIR.createIrticket(lo_createIrticket) ;

	EXIT;
}

void CInterfaceRecord::createIrother()
{
	ENTER;

   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrother       lo_createIrother ;

   li_counter = 0 ;
   li_index = 0 ;

   while ((li_counter < mi_IO_I) && (CMsgHandler::getMsgStatus()))
   {
		// G9090c start
		if (md_IO_prod_code[li_counter].NotIsNull() &&
		    md_IO_prod_code[li_counter].For(6) == "IGNORE") {
			li_counter++;
			continue;
		}
		// G9090c end

      li_index++ ;
      if (li_index == 1)
      {
         lo_createIrother.session_id   = md_session_id ;
         lo_createIrother.itin_type1    = md_IO_itin_type[li_counter];
         lo_createIrother.line_number1  = md_IO_line_number[li_counter];
         lo_createIrother.start_date1   = md_IO_start_date[li_counter];
         lo_createIrother.start_time1   = md_IO_start_time[li_counter];
         lo_createIrother.orig_city_id1 = md_IO_orig_city_id[li_counter];
         lo_createIrother.end_date1     = md_IO_end_date[li_counter];
         lo_createIrother.end_time1     = md_IO_end_time[li_counter];
         lo_createIrother.dest_city_id1 = md_IO_dest_city_id[li_counter];
         lo_createIrother.unit_type1    = md_IO_unit_type[li_counter];
         lo_createIrother.unit_ct1      = md_IO_unit_ct[li_counter];
         lo_createIrother.sup_id1       = md_IO_sup_id[li_counter];
         lo_createIrother.reference1    = md_IO_reference[li_counter];
         // GAP US089 START
         lo_createIrother.confirmation1 = md_IO_confirmation[li_counter];
         // GAP US089 END
			// GAP US092 START
         lo_createIrother.departure_point1 	= md_IO_departure_point[li_counter];
         lo_createIrother.arrival_point1	 	= md_IO_arrival_point[li_counter];
         lo_createIrother.svc_description1 	= md_IO_svc_description[li_counter];
         lo_createIrother.cabin_no1 		 	= md_IO_cabin_no[li_counter];
         lo_createIrother.tour_no1 		 	 	= md_IO_tour_no[li_counter];
			// GAP US092 END
         lo_createIrother.saving_code1  = md_IO_saving_code[li_counter++];
      }
      else if (li_index == 2)
      {
         lo_createIrother.itin_type2    = md_IO_itin_type[li_counter];
         lo_createIrother.line_number2  = md_IO_line_number[li_counter];
         lo_createIrother.start_date2   = md_IO_start_date[li_counter];
         lo_createIrother.start_time2   = md_IO_start_time[li_counter];
         lo_createIrother.orig_city_id2 = md_IO_orig_city_id[li_counter];
         lo_createIrother.end_date2     = md_IO_end_date[li_counter];
         lo_createIrother.end_time2     = md_IO_end_time[li_counter];
         lo_createIrother.dest_city_id2 = md_IO_dest_city_id[li_counter];
         lo_createIrother.unit_type2    = md_IO_unit_type[li_counter];
         lo_createIrother.unit_ct2      = md_IO_unit_ct[li_counter];
         lo_createIrother.sup_id2       = md_IO_sup_id[li_counter];
         lo_createIrother.reference2    = md_IO_reference[li_counter];
         // GAP US089 START
         lo_createIrother.confirmation2 = md_IO_confirmation[li_counter];
         // GAP US089 END
			// GAP US092 START
         lo_createIrother.departure_point2	= md_IO_departure_point[li_counter];
         lo_createIrother.arrival_point2	 	= md_IO_arrival_point[li_counter];
         lo_createIrother.svc_description2	= md_IO_svc_description[li_counter];
         lo_createIrother.cabin_no2 			= md_IO_cabin_no[li_counter];
         lo_createIrother.tour_no2 		 	 	= md_IO_tour_no[li_counter];
			// GAP US092 END
         lo_createIrother.saving_code2  = md_IO_saving_code[li_counter++];
      }
      else if (li_index == 3)
      {
         lo_createIrother.itin_type3    = md_IO_itin_type[li_counter];
         lo_createIrother.line_number3  = md_IO_line_number[li_counter];
         lo_createIrother.start_date3   = md_IO_start_date[li_counter];
         lo_createIrother.start_time3   = md_IO_start_time[li_counter];
         lo_createIrother.orig_city_id3 = md_IO_orig_city_id[li_counter];
         lo_createIrother.end_date3     = md_IO_end_date[li_counter];
         lo_createIrother.end_time3     = md_IO_end_time[li_counter];
         lo_createIrother.dest_city_id3 = md_IO_dest_city_id[li_counter];
         lo_createIrother.unit_type3    = md_IO_unit_type[li_counter];
         lo_createIrother.unit_ct3      = md_IO_unit_ct[li_counter];
         lo_createIrother.sup_id3       = md_IO_sup_id[li_counter];
         lo_createIrother.reference3    = md_IO_reference[li_counter];
         // GAP US089 START
         lo_createIrother.confirmation3 = md_IO_confirmation[li_counter];
         // GAP US089 END
			// GAP US092 START
         lo_createIrother.departure_point3	= md_IO_departure_point[li_counter];
         lo_createIrother.arrival_point3	 	= md_IO_arrival_point[li_counter];
         lo_createIrother.svc_description3 	= md_IO_svc_description[li_counter];
         lo_createIrother.cabin_no3 			= md_IO_cabin_no[li_counter];
         lo_createIrother.tour_no3 		 	 	= md_IO_tour_no[li_counter];
			// GAP US092 END
          lo_createIrother.saving_code3  = md_IO_saving_code[li_counter++];

         dbIR.createIrother(lo_createIrother) ;
         initIrother(lo_createIrother) ;

         li_index = 0 ;
      }
   }

   if (li_index > 0)
      dbIR.createIrother(lo_createIrother) ;

	EXIT;
}


void CInterfaceRecord::createIrfrequentuser()
{
	ENTER;

   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrfrequentuser    lo_createIrfrequentuser;

   li_counter = 0 ;
   li_index = 0 ;
   while ((li_counter < mi_IRF_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++ ;

		if (md_IRF_srvc_sup_id[li_counter].IsNull()) {
			md_IRF_srvc_sup_id[li_counter] = " ";
		}

      if (li_index == 1)
      {
         lo_createIrfrequentuser.session_id = md_session_id ;
         lo_createIrfrequentuser.pax_id1 = md_IRF_pax_id[li_counter] ;
         lo_createIrfrequentuser.sup_id1 = md_IRF_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup_type1 = md_IRF_fup_type[li_counter] ;
         lo_createIrfrequentuser.fup_status1 = md_IRF_fup_status[li_counter] ;
         lo_createIrfrequentuser.srvc_sup_id1 = md_IRF_srvc_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup1 = md_IRF_fup[li_counter++] ;
      }
      else if (li_index == 2)
      {
         lo_createIrfrequentuser.pax_id2 = md_IRF_pax_id[li_counter] ;
         lo_createIrfrequentuser.sup_id2 = md_IRF_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup_type2 = md_IRF_fup_type[li_counter] ;
         lo_createIrfrequentuser.fup_status2 = md_IRF_fup_status[li_counter] ;
         lo_createIrfrequentuser.srvc_sup_id2 = md_IRF_srvc_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup2 = md_IRF_fup[li_counter++] ;
      }
      else if (li_index == 3)
      {
         lo_createIrfrequentuser.pax_id3 = md_IRF_pax_id[li_counter] ;
         lo_createIrfrequentuser.sup_id3 = md_IRF_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup_type3 = md_IRF_fup_type[li_counter] ;
         lo_createIrfrequentuser.fup_status3 = md_IRF_fup_status[li_counter] ;
         lo_createIrfrequentuser.srvc_sup_id3 = md_IRF_srvc_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup3 = md_IRF_fup[li_counter++] ;
      }
      else if (li_index == 4)
      {
         lo_createIrfrequentuser.pax_id4 = md_IRF_pax_id[li_counter] ;
         lo_createIrfrequentuser.sup_id4 = md_IRF_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup_type4 = md_IRF_fup_type[li_counter] ;
         lo_createIrfrequentuser.fup_status4 = md_IRF_fup_status[li_counter] ;
         lo_createIrfrequentuser.srvc_sup_id4 = md_IRF_srvc_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup4 = md_IRF_fup[li_counter++] ;
      }
      else if (li_index == 5)
      {
         lo_createIrfrequentuser.pax_id5 = md_IRF_pax_id[li_counter] ;
         lo_createIrfrequentuser.sup_id5 = md_IRF_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup_type5 = md_IRF_fup_type[li_counter] ;
         lo_createIrfrequentuser.fup_status5 = md_IRF_fup_status[li_counter] ;
         lo_createIrfrequentuser.srvc_sup_id5 = md_IRF_srvc_sup_id[li_counter] ;
         lo_createIrfrequentuser.fup5 = md_IRF_fup[li_counter++] ;

         dbIR.createIrfrequentuser(lo_createIrfrequentuser) ;
         initIrfrequentuser(lo_createIrfrequentuser) ;

         li_index = 0 ;
      }
   }

   if (li_index > 0)
      dbIR.createIrfrequentuser(lo_createIrfrequentuser) ;

	EXIT;
}

void CInterfaceRecord::createIrtaxdetail()
{
   ENTER;

   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrtaxdetail lo_createIrtaxdetail ;

   li_counter = 0 ;
   li_index = 0 ;
   while ((li_counter < mi_ITD_I) && (CMsgHandler::getMsgStatus()))
   {
		if ( md_ITD_tax_amount[li_counter] != "0.00" )
		{
      	li_index++ ;

      	if (li_index == 1)
      	{
         	lo_createIrtaxdetail.session_id   = md_session_id;
         	lo_createIrtaxdetail.pax_id1      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number1 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id1      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source1  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id1     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount1  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 2)
      	{
         	lo_createIrtaxdetail.pax_id2      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number2 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id2      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source2  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id2     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount2  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 3)
      	{
         	lo_createIrtaxdetail.pax_id3      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number3 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id3      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source3  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id3     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount3  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 4)
      	{
         	lo_createIrtaxdetail.pax_id4      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number4 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id4      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source4  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id4     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount4  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 5)
      	{
         	lo_createIrtaxdetail.pax_id5      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number5 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id5      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source5  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id5     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount5  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 6)
      	{
         	lo_createIrtaxdetail.pax_id6      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number6 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id6      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source6  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id6     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount6  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 7)
      	{
         	lo_createIrtaxdetail.pax_id7      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number7 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id7      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source7  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id7     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount7  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 8)
      	{
         	lo_createIrtaxdetail.pax_id8      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number8 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id8      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source8  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id8     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount8  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 9)
      	{
         	lo_createIrtaxdetail.pax_id9      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number9 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id9      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source9  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id9     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount9  = md_ITD_tax_amount[li_counter++];
      	}
      	else if (li_index == 10)
      	{
         	lo_createIrtaxdetail.pax_id0      = md_ITD_pax_id[li_counter];
   			lo_createIrtaxdetail.line_number0 = md_ITD_line_number[li_counter];
         	lo_createIrtaxdetail.tax_id0      = md_ITD_tax_id[li_counter];
         	lo_createIrtaxdetail.tax_source0  = md_ITD_tax_source[li_counter];
         	lo_createIrtaxdetail.city_id0     = md_ITD_city_id[li_counter];
         	lo_createIrtaxdetail.tax_amount0  = md_ITD_tax_amount[li_counter++];

         	dbIR.createIrtaxdetail(lo_createIrtaxdetail) ;
         	initIrtaxdetail(lo_createIrtaxdetail) ;

         	li_index = 0 ;
      	}
   	}
		else
			li_counter++ ;

	}
   if (li_index > 0)
     	dbIR.createIrtaxdetail(lo_createIrtaxdetail) ;

	EXIT;
}

void CInterfaceRecord::editAddress() {
	// SCR#292494 start: Mel wants to do a guess if input address is garbage data
	for (int i = 0; i < mi_IA_I; i++) {
		D_address_line ld_addr[5];
		ld_addr[0] = AB_null;
		ld_addr[1] = AB_null;
		ld_addr[2] = AB_null;
		ld_addr[3] = AB_null;
		ld_addr[4] = AB_null;

		int j = 0;

		if (md_IA_addr1[i].NotIsNull()) { ld_addr[j++] = md_IA_addr1[i]; }
		if (md_IA_addr2[i].NotIsNull()) { ld_addr[j++] = md_IA_addr2[i]; }
		if (md_IA_addr3[i].NotIsNull()) { ld_addr[j++] = md_IA_addr3[i]; }
		if (md_IA_addr4[i].NotIsNull()) { ld_addr[j++] = md_IA_addr4[i]; }
		if (md_IA_addr5[i].NotIsNull()) { ld_addr[j++] = md_IA_addr5[i]; }

		md_IA_addr1[i] = ld_addr[0];
		md_IA_addr2[i] = ld_addr[1];
		md_IA_addr3[i] = ld_addr[2];
		md_IA_addr4[i] = ld_addr[3];
		md_IA_addr5[i] = ld_addr[4];

	}
	// SCR#292494 end
}

void CInterfaceRecord::createIraddress()
{
	ENTER;

   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIraddress       lo_createIraddress ;

	editAddress();

   li_index = 0 ;
   li_counter = 0 ;
   while ((li_counter < mi_IA_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++ ;

      if (li_index == 1)
      {
   		lo_createIraddress.session_id     = md_session_id;
   		lo_createIraddress.type1          = md_IA_type[li_counter];
   		lo_createIraddress.addr11         = md_IA_addr1[li_counter];
			if (lo_createIraddress.addr11.IsNull())
				lo_createIraddress.addr11 = " " ;
   		lo_createIraddress.addr21         = md_IA_addr2[li_counter];
   		lo_createIraddress.addr31         = md_IA_addr3[li_counter];
   		lo_createIraddress.addr41         = md_IA_addr4[li_counter];
   		lo_createIraddress.addr51         = md_IA_addr5[li_counter];
   		lo_createIraddress.addr61         = md_IA_addr6[li_counter];
   		lo_createIraddress.phone_acode1   = md_IA_phone_acode[li_counter] ;
   		lo_createIraddress.phone_no1      = md_IA_phone_no[li_counter] ;
   		lo_createIraddress.bphone_acode1  = md_IA_bphone_acode[li_counter] ;
   		lo_createIraddress.bphone_no1     = md_IA_bphone_no[li_counter] ;
   		lo_createIraddress.mphone_acode1  = md_IA_mphone_acode[li_counter] ;
   		lo_createIraddress.mphone_no1     = md_IA_mphone_no[li_counter] ;
   		lo_createIraddress.fax_acode1     = md_IA_fax_acode[li_counter] ;
   		lo_createIraddress.fax_no1        = md_IA_fax_no[li_counter++] ;
      }
      else if (li_index == 2)
      {
   		lo_createIraddress.type2          = md_IA_type[li_counter];
   		lo_createIraddress.addr12         = md_IA_addr1[li_counter];
   		lo_createIraddress.addr22         = md_IA_addr2[li_counter];
   		lo_createIraddress.addr32         = md_IA_addr3[li_counter];
   		lo_createIraddress.addr42         = md_IA_addr4[li_counter];
   		lo_createIraddress.addr52         = md_IA_addr5[li_counter];
   		lo_createIraddress.addr62         = md_IA_addr6[li_counter];
   		lo_createIraddress.phone_acode2   = md_IA_phone_acode[li_counter] ;
   		lo_createIraddress.phone_no2      = md_IA_phone_no[li_counter] ;
   		lo_createIraddress.bphone_acode2  = md_IA_bphone_acode[li_counter] ;
   		lo_createIraddress.bphone_no2     = md_IA_bphone_no[li_counter] ;
   		lo_createIraddress.mphone_acode2  = md_IA_mphone_acode[li_counter] ;
   		lo_createIraddress.mphone_no2     = md_IA_mphone_no[li_counter] ;
   		lo_createIraddress.fax_acode2     = md_IA_fax_acode[li_counter] ;
   		lo_createIraddress.fax_no2        = md_IA_fax_no[li_counter++] ;

         dbIR.createIraddress(lo_createIraddress) ;
         initIraddress(lo_createIraddress) ;

         li_index = 0 ;
      }

   }
   if (li_index > 0)
      dbIR.createIraddress(lo_createIraddress) ;

	EXIT;
}

void CInterfaceRecord::createIrpayment()
{
   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrpayment       lo_createIrpayment ;

   li_index = 0 ;
   li_counter = 0 ;
   while ((li_counter < mi_IRP_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++ ;

      if (li_index == 1)
      {
   		lo_createIrpayment.session_id     = md_session_id;
   		lo_createIrpayment.pax_id1        = md_IRP_pax_id[li_counter];
   		lo_createIrpayment.pay_id1        = md_IRP_pay_id[li_counter];
   		lo_createIrpayment.line_number1   = md_IRP_line_number[li_counter];
   		lo_createIrpayment.amount1        = md_IRP_amount[li_counter];
   		lo_createIrpayment.form_of_pay1   = md_IRP_form_of_pay[li_counter] ;
   		lo_createIrpayment.cc_id1         = md_IRP_cc_id[li_counter] ;
   		lo_createIrpayment.cc_no1         = md_IRP_cc_no[li_counter] ;
   		lo_createIrpayment.cc_exp_date1   = md_IRP_cc_exp_date[li_counter] ;
   		lo_createIrpayment.auth_no1       = md_IRP_auth_no[li_counter] ;
   		lo_createIrpayment.auth_source1   = md_IRP_auth_source[li_counter++] ;
      }
      else if (li_index == 2)
      {
   		lo_createIrpayment.pax_id2        = md_IRP_pax_id[li_counter];
   		lo_createIrpayment.pay_id2        = md_IRP_pay_id[li_counter];
   		lo_createIrpayment.line_number2   = md_IRP_line_number[li_counter];
   		lo_createIrpayment.amount2        = md_IRP_amount[li_counter];
   		lo_createIrpayment.form_of_pay2   = md_IRP_form_of_pay[li_counter] ;
   		lo_createIrpayment.cc_id2         = md_IRP_cc_id[li_counter] ;
   		lo_createIrpayment.cc_no2         = md_IRP_cc_no[li_counter] ;
   		lo_createIrpayment.cc_exp_date2   = md_IRP_cc_exp_date[li_counter] ;
   		lo_createIrpayment.auth_no2       = md_IRP_auth_no[li_counter] ;
   		lo_createIrpayment.auth_source2   = md_IRP_auth_source[li_counter++] ;
		}
      else if (li_index == 3)
      {
   		lo_createIrpayment.pax_id3        = md_IRP_pax_id[li_counter];
   		lo_createIrpayment.pay_id3        = md_IRP_pay_id[li_counter];
   		lo_createIrpayment.line_number3   = md_IRP_line_number[li_counter];
   		lo_createIrpayment.amount3        = md_IRP_amount[li_counter];
   		lo_createIrpayment.form_of_pay3   = md_IRP_form_of_pay[li_counter] ;
   		lo_createIrpayment.cc_id3         = md_IRP_cc_id[li_counter] ;
   		lo_createIrpayment.cc_no3         = md_IRP_cc_no[li_counter] ;
   		lo_createIrpayment.cc_exp_date3   = md_IRP_cc_exp_date[li_counter] ;
   		lo_createIrpayment.auth_no3       = md_IRP_auth_no[li_counter] ;
   		lo_createIrpayment.auth_source3   = md_IRP_auth_source[li_counter++] ;
		}
      else if (li_index == 4)
      {
   		lo_createIrpayment.pax_id4        = md_IRP_pax_id[li_counter];
   		lo_createIrpayment.pay_id4        = md_IRP_pay_id[li_counter];
   		lo_createIrpayment.line_number4   = md_IRP_line_number[li_counter];
   		lo_createIrpayment.amount4        = md_IRP_amount[li_counter];
   		lo_createIrpayment.form_of_pay4   = md_IRP_form_of_pay[li_counter] ;
   		lo_createIrpayment.cc_id4         = md_IRP_cc_id[li_counter] ;
   		lo_createIrpayment.cc_no4         = md_IRP_cc_no[li_counter] ;
   		lo_createIrpayment.cc_exp_date4   = md_IRP_cc_exp_date[li_counter] ;
   		lo_createIrpayment.auth_no4       = md_IRP_auth_no[li_counter] ;
   		lo_createIrpayment.auth_source4   = md_IRP_auth_source[li_counter++] ;
		}
      else if (li_index == 5)
      {
   		lo_createIrpayment.pax_id5        = md_IRP_pax_id[li_counter];
   		lo_createIrpayment.pay_id5        = md_IRP_pay_id[li_counter];
   		lo_createIrpayment.line_number5   = md_IRP_line_number[li_counter];
   		lo_createIrpayment.amount5        = md_IRP_amount[li_counter];
   		lo_createIrpayment.form_of_pay5   = md_IRP_form_of_pay[li_counter] ;
   		lo_createIrpayment.cc_id5         = md_IRP_cc_id[li_counter] ;
   		lo_createIrpayment.cc_no5         = md_IRP_cc_no[li_counter] ;
   		lo_createIrpayment.cc_exp_date5   = md_IRP_cc_exp_date[li_counter] ;
   		lo_createIrpayment.auth_no5       = md_IRP_auth_no[li_counter] ;
   		lo_createIrpayment.auth_source5   = md_IRP_auth_source[li_counter++] ;

         dbIR.createIrpayment(lo_createIrpayment) ;
         initIrpayment(lo_createIrpayment) ;

         li_index = 0 ;
      }

   }
   if (li_index > 0)
      dbIR.createIrpayment(lo_createIrpayment) ;
}

void CInterfaceRecord::createIrreceipt()
{
   int                     li_counter ;
   CDIR                    dbIR ;
   CIRCreateIrreceipt      lo_createIrreceipt ;
   bool  lb_create;

   li_counter = 0 ;

   for (li_counter=1; (li_counter <= mi_RC_I) && (CMsgHandler::getMsgStatus()); li_counter++)
   {
		lo_createIrreceipt.session_id   	= md_session_id;
		lo_createIrreceipt.line_number  	= li_counter;
		lo_createIrreceipt.cop_id   		= md_RC_cop_id;
		lo_createIrreceipt.bra_id   		= md_RC_bra_id;
		lo_createIrreceipt.lang_id   		= md_RC_lang_id;
		lo_createIrreceipt.name        	= md_RC_name[li_counter] ;
		lo_createIrreceipt.total_amt   	= md_RC_total_amt[li_counter] ;
		lo_createIrreceipt.bank_id     	= md_RC_bank_acc[li_counter] ;
		lo_createIrreceipt.form_of_pay 	= md_RC_form_of_pay[li_counter] ;
		lo_createIrreceipt.cc_id       	= md_RC_cc_id[li_counter] ;
		lo_createIrreceipt.cc_no       	= md_RC_cc_no[li_counter] ;
		lo_createIrreceipt.cc_exp_date 	= md_RC_cc_exp_date[li_counter] ;
		lo_createIrreceipt.cc_auth_no  	= md_RC_auth_no[li_counter] ;
		lo_createIrreceipt.acc_id1     	= md_RC_gl_acc1[li_counter] ;
		lo_createIrreceipt.acc_id2     	= md_RC_gl_acc2[li_counter] ;
		lo_createIrreceipt.acc_id3     	= md_RC_gl_acc3[li_counter] ;
		lo_createIrreceipt.amt1        	= md_RC_gl_amt1[li_counter] ;
		lo_createIrreceipt.amt2        	= md_RC_gl_amt2[li_counter] ;
		lo_createIrreceipt.amt3        	= md_RC_gl_amt3[li_counter] ;
		lo_createIrreceipt.link_to					= md_RC_link_to[li_counter] ;
		lo_createIrreceipt.payment_reference	= md_RC_payment_reference[li_counter] ;

		lb_create = true;
		if ( md_RC_form_of_pay[li_counter] == "CC" )
   		{
			LOG(logDEBUG) << "createIrreceipt:[" << md_RC_cc_id[li_counter] << "] ["  << md_RC_cc_no[li_counter] << "] [";

			LOG(logDEBUG) << "1 of 2. checking if cc_id is valid for the company....";
			if ( !isCCExists4copid( md_RC_cc_id[li_counter].to_string() ) )
			{
					LOG(logDEBUG) << "CC_ID IS NOT VALID, NO IRRECEIPT CREATING....";

					createRemark("__Q", "REC/REC CC VENDOR INVALID");

					lb_create = false;
			}

			if ( lb_create )
			{
				LOG(logDEBUG) << "2 of 2. checking if cc_no is valid for the cc_id...." ;

				string cc_id_no = md_RC_cc_id[li_counter].to_string();
				cc_id_no += md_RC_cc_no[li_counter].to_string();

				if ( !validCreditCardBIN(cc_id_no.data()) )
				{
					LOG(logDEBUG) << "BIN IS NOT VALID, NO IRRECEIPT CREATING....";

					createRemark("__Q", "REC/REC CC VENDOR BIN INVALID");

					lb_create = false;
				}
			}
		}

		if ( lb_create )
		{
			dbIR.createIrreceipt(lo_createIrreceipt) ;

			assignIrdistribution(lo_createIrreceipt, li_counter);
		}

		initIrreceipt(lo_createIrreceipt) ;
   }
}

void CInterfaceRecord::assignIrdistribution( CIRCreateIrreceipt &po_createIrreceipt, int pi_rec_line )
{
	AB_Integer li_error_code;

	if ( po_createIrreceipt.rec_valid == "Y" )
	{
		if ( po_createIrreceipt.acc_id1.NotIsNull() )
		{
			md_DI_rec_line[++mi_DI_I] = pi_rec_line;
			md_DI_gl_acc[mi_DI_I]     = po_createIrreceipt.acc_id1;
			md_DI_gl_amt[mi_DI_I]     = po_createIrreceipt.amt1;
		}
		if ( po_createIrreceipt.acc_id2.NotIsNull() )
		{
			md_DI_rec_line[++mi_DI_I] = pi_rec_line;
			md_DI_gl_acc[mi_DI_I]     = po_createIrreceipt.acc_id2;
			md_DI_gl_amt[mi_DI_I]     = po_createIrreceipt.amt2;
		}
		if ( po_createIrreceipt.acc_id3.NotIsNull() )
		{
			md_DI_rec_line[++mi_DI_I] = pi_rec_line;
			md_DI_gl_acc[mi_DI_I]     = po_createIrreceipt.acc_id3;
			md_DI_gl_amt[mi_DI_I]     = po_createIrreceipt.amt3;
		}
	}
	else if ( po_createIrreceipt.rec_valid == "N" )
	{
		li_error_code = CMsgHandler::getMsgId();
		createInfoQueue(mv_rec_queType, li_error_code);

		CMsgHandler::clearMsg();
	}
}

void CInterfaceRecord::createIrdistribution()
{
   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrdistribution      lo_createIrdistribution ;

   li_index = 0 ;
   li_counter = 1 ;
   while ((li_counter <= mi_DI_I) && (CMsgHandler::getMsgStatus()))
   {
      li_index++ ;

      if (li_index == 1)
      {
   		lo_createIrdistribution.session_id    = md_session_id;
   		lo_createIrdistribution.line_number_1 = md_DI_rec_line[li_counter] ;
   		lo_createIrdistribution.acc_id_1      = md_DI_gl_acc[li_counter] ;
   		lo_createIrdistribution.amt_1         = md_DI_gl_amt[li_counter++] ;
		}
      else if (li_index == 2)
      {
   		lo_createIrdistribution.session_id    = md_session_id;
   		lo_createIrdistribution.line_number_2 = md_DI_rec_line[li_counter] ;
   		lo_createIrdistribution.acc_id_2      = md_DI_gl_acc[li_counter] ;
   		lo_createIrdistribution.amt_2         = md_DI_gl_amt[li_counter++] ;
		}
      else if (li_index == 3)
      {
   		lo_createIrdistribution.session_id    = md_session_id;
   		lo_createIrdistribution.line_number_3 = md_DI_rec_line[li_counter] ;
   		lo_createIrdistribution.acc_id_3      = md_DI_gl_acc[li_counter] ;
   		lo_createIrdistribution.amt_3         = md_DI_gl_amt[li_counter++] ;
		}
      else if (li_index == 4)
      {
   		lo_createIrdistribution.session_id    = md_session_id;
   		lo_createIrdistribution.line_number_4 = md_DI_rec_line[li_counter] ;
   		lo_createIrdistribution.acc_id_4      = md_DI_gl_acc[li_counter] ;
   		lo_createIrdistribution.amt_4         = md_DI_gl_amt[li_counter++] ;
		}
      else if (li_index == 5)
      {
   		lo_createIrdistribution.session_id    = md_session_id;
   		lo_createIrdistribution.line_number_5 = md_DI_rec_line[li_counter] ;
   		lo_createIrdistribution.acc_id_5      = md_DI_gl_acc[li_counter] ;
   		lo_createIrdistribution.amt_5         = md_DI_gl_amt[li_counter++] ;
		}
      else if (li_index == 6)
      {
   		lo_createIrdistribution.session_id    = md_session_id;
   		lo_createIrdistribution.line_number_6 = md_DI_rec_line[li_counter] ;
   		lo_createIrdistribution.acc_id_6      = md_DI_gl_acc[li_counter] ;
   		lo_createIrdistribution.amt_6         = md_DI_gl_amt[li_counter++] ;

         dbIR.createIrdistribution(lo_createIrdistribution) ;
         initIrdistribution(lo_createIrdistribution) ;

         li_index = 0 ;
      }

   }

   if (li_index > 0)
      dbIR.createIrdistribution(lo_createIrdistribution) ;
}

void CInterfaceRecord::createIremailaddress()
{
   int                     li_counter ;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIremailaddress lo_createIremailaddress ;

   li_index = 0 ;

	for ( li_counter = 0; (li_counter < mi_EM_I) && (CMsgHandler::getMsgStatus()); li_counter++ )
   {
      li_index++ ;

      if (li_index == 1)
      {
   		lo_createIremailaddress.session_id   	= md_session_id;
   		lo_createIremailaddress.email_id1 		= mt_EM_email_id[li_counter] ;
   		lo_createIremailaddress.email_type1  	= mc_EM_email_type[li_counter] ;
		}
      else if (li_index == 2)
      {
   		lo_createIremailaddress.session_id   	= md_session_id;
   		lo_createIremailaddress.email_id2 		= mt_EM_email_id[li_counter] ;
   		lo_createIremailaddress.email_type2  	= mc_EM_email_type[li_counter] ;
		}
      else if (li_index == 3)
      {
   		lo_createIremailaddress.session_id   	= md_session_id;
   		lo_createIremailaddress.email_id3 		= mt_EM_email_id[li_counter] ;
   		lo_createIremailaddress.email_type3  	= mc_EM_email_type[li_counter] ;
		}
      else if (li_index == 4)
      {
   		lo_createIremailaddress.session_id   	= md_session_id;
   		lo_createIremailaddress.email_id4 		= mt_EM_email_id[li_counter] ;
   		lo_createIremailaddress.email_type4  	= mc_EM_email_type[li_counter] ;
		}
      else if (li_index == 5)
      {
   		lo_createIremailaddress.session_id   	= md_session_id;
   		lo_createIremailaddress.email_id5 		= mt_EM_email_id[li_counter] ;
   		lo_createIremailaddress.email_type5  	= mc_EM_email_type[li_counter] ;
		}
      else if (li_index == 6)
      {
   		lo_createIremailaddress.session_id   	= md_session_id;
   		lo_createIremailaddress.email_id6 		= mt_EM_email_id[li_counter] ;
   		lo_createIremailaddress.email_type6  	= mc_EM_email_type[li_counter] ;

         dbIR.createIremailaddress(lo_createIremailaddress) ;
         initIremailaddress(lo_createIremailaddress) ;

         li_index = 0 ;
      }

   }

   if (li_index > 0)
      dbIR.createIremailaddress(lo_createIremailaddress) ;
}

void CInterfaceRecord::createIrstopovercity()
{
	ENTER;

   int                     li_counter ;
   int                     li_index ;
	CDIR                    dbIR;
	CIRCreateIrstopovercity lo_createIrstopovercity;

   li_index = 0 ;

	for ( li_counter = 0; (li_counter < mi_ISOC_I) && (CMsgHandler::getMsgStatus()); li_counter++ )
   {
     	li_index++;
		lo_createIrstopovercity.session_id		= md_session_id;

		if ( li_index == 1 )
		{
			lo_createIrstopovercity.itin_type1		= md_ISOC_itin_type[li_counter];
			lo_createIrstopovercity.line_number1	= md_ISOC_line_number[li_counter];
			lo_createIrstopovercity.order_number1	= md_ISOC_order_number[li_counter];
			lo_createIrstopovercity.city_id1			= md_ISOC_stop_over_city[li_counter];
		}
		else if (li_index == 2)
		{
			lo_createIrstopovercity.itin_type2		= md_ISOC_itin_type[li_counter];
			lo_createIrstopovercity.line_number2	= md_ISOC_line_number[li_counter];
			lo_createIrstopovercity.order_number2	= md_ISOC_order_number[li_counter];
			lo_createIrstopovercity.city_id2			= md_ISOC_stop_over_city[li_counter];
		}
		else if (li_index == 3)
		{
			lo_createIrstopovercity.itin_type3		= md_ISOC_itin_type[li_counter];
			lo_createIrstopovercity.line_number3	= md_ISOC_line_number[li_counter];
			lo_createIrstopovercity.order_number3	= md_ISOC_order_number[li_counter];
			lo_createIrstopovercity.city_id3			= md_ISOC_stop_over_city[li_counter];
		}
		else if (li_index == 4)
		{
			lo_createIrstopovercity.itin_type4		= md_ISOC_itin_type[li_counter];
			lo_createIrstopovercity.line_number4	= md_ISOC_line_number[li_counter];
			lo_createIrstopovercity.order_number4	= md_ISOC_order_number[li_counter];
			lo_createIrstopovercity.city_id4			= md_ISOC_stop_over_city[li_counter];
		}
		else if (li_index == 5)
		{
			lo_createIrstopovercity.itin_type5		= md_ISOC_itin_type[li_counter];
			lo_createIrstopovercity.line_number5	= md_ISOC_line_number[li_counter];
			lo_createIrstopovercity.order_number5	= md_ISOC_order_number[li_counter];
			lo_createIrstopovercity.city_id5			= md_ISOC_stop_over_city[li_counter];
		}
		else if (li_index == 6)
		{
			lo_createIrstopovercity.itin_type6		= md_ISOC_itin_type[li_counter];
			lo_createIrstopovercity.line_number6	= md_ISOC_line_number[li_counter];
			lo_createIrstopovercity.order_number6	= md_ISOC_order_number[li_counter];
			lo_createIrstopovercity.city_id6			= md_ISOC_stop_over_city[li_counter];

			dbIR.createIrstopovercity(lo_createIrstopovercity);
			initIrstopovercity(lo_createIrstopovercity);

			li_index = 0;
		}
	}

	if( li_index > 0 )
	{
		dbIR.createIrstopovercity(lo_createIrstopovercity);
	}
}

void CInterfaceRecord::createIrRows()
{
	ENTER;
   int                     li_counter = 0;
   int                     li_index ;
   CDIR                    dbIR ;
   CIRCreateIrtripheader   lo_createIrtripheader ;
   CIRCreateIrrental       lo_createIrrental ;

	CIRValReasonCode        lo_valReasoncode;
	AB_Varchar	lv_conId(3) ;

	/* REMARK_PARSER: process all GENERIC remarks: STEP 1: move to IR memory */
	RP_storeParsedRemarksInMemory();

	lv_conId = md_IRT_con_id ;
	lv_conId.removeLeadAndTrailBlanks() ;

	// G117 If con_id is NULL make it 000
	/* SCR#12815 : if con_id is null make it 000
		if it is invalid - length >0 and less than 3 make it 900 */

   if (lv_conId.IsNull() )
	{
		md_IRT_con_id = "000" ;
	}
	else if (( lv_conId.length() > 0 && lv_conId.length() < 3 ) ||
				( ! lv_conId.isAllAlphaNumeric() ) )
	{
		md_IRT_con_id = "900" ;
	}

   lo_createIrtripheader.session_id    = md_session_id ;
   lo_createIrtripheader.interface_type= md_interface_type;
   lo_createIrtripheader.creation_date = md_creation_date ;
   lo_createIrtripheader.iata_no       = md_IRT_iata_no ;
   lo_createIrtripheader.crs_id        = md_IRT_crs_id ;
   lo_createIrtripheader.cfa_type      = md_IRT_cfa_type ;
   lo_createIrtripheader.cfa_code      = md_IRT_cfa_code ;
   lo_createIrtripheader.cfa_id        = md_IRT_cfa_id ;
   lo_createIrtripheader.con_id        = md_IRT_con_id ;
   lo_createIrtripheader.iata_dest     = md_IRT_iata_dest ;
   lo_createIrtripheader.city_id       = md_IRT_city_id ;
   lo_createIrtripheader.pnr_date      = md_IRT_pnr_date ;
   lo_createIrtripheader.pnr_time      = md_IRT_pnr_time ;
   lo_createIrtripheader.record_creation_date      = md_IRT_record_creation_date ;
   lo_createIrtripheader.record_creation_time      = md_IRT_record_creation_time ;
   lo_createIrtripheader.start_date    = md_IRT_start_date ;
   lo_createIrtripheader.cfa_dpt       = md_IRT_cfa_dpt ;
   lo_createIrtripheader.mktg_list     = md_IRT_mktg_list ;
   lo_createIrtripheader.reference     = md_IRT_reference ;
   lo_createIrtripheader.cop_id        = md_IRT_cop_id ;
   lo_createIrtripheader.bra_id        = md_IRT_bra_id ;
   lo_createIrtripheader.booking_no    = md_IRT_booking_no ;
	// GAP US107 START
   lo_createIrtripheader.ticketer_user_id    = md_IRT_ticketer_user_id ;
   lo_createIrtripheader.employee_number    = mv_IRT_employee_number ;
	// GAP US107 END

	// GAP G9123 SCR 440488 START
	lo_createIrtripheader.booking_pcc         = md_IRT_booking_pcc ;
	lo_createIrtripheader.booking_gds_agent   = md_IRT_booking_gds_agent ;
	lo_createIrtripheader.ticketing_pcc       = md_IRT_ticketing_pcc ;
	lo_createIrtripheader.ticketing_gds_agent = md_IRT_ticketing_gds_agent ;
	// GAP G9123 SCR 440488 END

   dbIR.createIrtripheader(lo_createIrtripheader) ;

	/* REMARK_PARSER: process all GENERIC remarks: STEP 2: store in IR tables */
	RP_storeParsedRemarksInDatabase();

   if (! CMsgHandler::getMsgStatus())
      return ;

   while ((li_counter < mi_IR_I) && (CMsgHandler::getMsgStatus()))
   {
      lo_createIrrental.session_id     = md_session_id;
      lo_createIrrental.cop_id         = md_IRT_cop_id;
      lo_createIrrental.itin_type      = md_IR_itin_type[li_counter];
      lo_createIrrental.line_number    = md_IR_line_number[li_counter];
      lo_createIrrental.start_date     = md_IR_start_date[li_counter];
      lo_createIrrental.start_time     = md_IR_start_time[li_counter];
      lo_createIrrental.origin_city_id = md_IR_origin_city_id[li_counter];
      lo_createIrrental.end_date       = md_IR_end_date[li_counter];
      lo_createIrrental.end_time       = md_IR_end_time[li_counter];
      lo_createIrrental.dest_city_id   = md_IR_dest_city_id[li_counter];
      lo_createIrrental.unit_type      = md_IR_unit_type[li_counter];
      lo_createIrrental.unit_ct        = md_IR_unit_ct[li_counter];
      lo_createIrrental.sup_id         = md_IR_sup_id[li_counter];
      lo_createIrrental.number_of_pax  = md_IR_number_of_pax[li_counter];
      lo_createIrrental.ex_day_charge  = md_IR_ex_day_charge[li_counter];
      lo_createIrrental.ex_hr_charge   = md_IR_ex_hr_charge[li_counter];
      lo_createIrrental.ex_mile_charge = md_IR_ex_mile_charge[li_counter];
      lo_createIrrental.free_mile_code = md_IR_free_mile_code[li_counter];
      lo_createIrrental.free_mileage   = md_IR_free_mileage[li_counter];
      lo_createIrrental.dropoff_charge = md_IR_dropoff_charge[li_counter];
      lo_createIrrental.regular_rate   = md_IR_regular_rate[li_counter];
      lo_createIrrental.booked_rate    = md_IR_booked_rate[li_counter];
      lo_createIrrental.local_rate     = md_IR_local_rate[li_counter];
      lo_createIrrental.cur_id         = md_IR_cur_id[li_counter];
      lo_createIrrental.rate_type      = md_IR_rate_type[li_counter];
      lo_createIrrental.rate_code      = md_IR_rate_code[li_counter];
      lo_createIrrental.rate_duration  = md_IR_rate_duration[li_counter];
      lo_createIrrental.rate_multip    = md_IR_rate_multip[li_counter];
      lo_createIrrental.comm_rate      = md_IR_com_rate[li_counter];
      lo_createIrrental.multi_lvl_rate = md_IR_multi_lvl_rate[li_counter];
      lo_createIrrental.saving_code    = md_IR_saving_code[li_counter];
      lo_createIrrental.location       = md_IR_location[li_counter];
      lo_createIrrental.location_id    = md_IR_location_id[li_counter];
      lo_createIrrental.site_name      = md_IR_site_name[li_counter];
      lo_createIrrental.addr1          = md_IR_addr1[li_counter];
      lo_createIrrental.addr2          = md_IR_addr2[li_counter];
      lo_createIrrental.addr3          = md_IR_addr3[li_counter];
      lo_createIrrental.addr4          = md_IR_addr4[li_counter];
      lo_createIrrental.phone_acode    = md_IR_phone_acode[li_counter];
      lo_createIrrental.phone_no       = md_IR_phone_no[li_counter];
      lo_createIrrental.fax_acode      = md_IR_fax_acode[li_counter];
      lo_createIrrental.fax_no         = md_IR_fax_no[li_counter];
      lo_createIrrental.guarantee      = md_IR_guarantee[li_counter];
      lo_createIrrental.confirmation   = md_IR_confirmation[li_counter];
      lo_createIrrental.reference      = md_IR_reference[li_counter];
      lo_createIrrental.corporate_disc = md_IR_corporate_disc[li_counter];
      lo_createIrrental.corporation_id = md_IR_corporation_id[li_counter];
      lo_createIrrental.cancel_policy  = md_IR_cancel_policy[li_counter];
      lo_createIrrental.fup            = md_IR_fup[li_counter];
      lo_createIrrental.pickup_location   = md_IR_pickup_location[li_counter];
      lo_createIrrental.dropoff_location  = md_IR_dropoff_location[li_counter];
      lo_createIrrental.discount_code  = md_IR_discount_code[li_counter];

      cout << " FG: CInterfaceRecord::createIrRows: md_IR_action_code[li_counter]= " << md_IR_action_code[li_counter] << flush << endl;
      lo_createIrrental.action_code    = md_IR_action_code[li_counter];
      lo_createIrrental.source         = md_IR_EB_source[li_counter];
      lo_createIrrental.reason_code    = md_IR_reason_code[li_counter];
      lo_createIrrental.commissionable = md_IR_commissionable[li_counter];
		lo_createIrrental.preferred      = md_IR_preferred[li_counter];
      lo_createIrrental.service_status = md_IR_service_status[li_counter];

      lo_createIrrental.virtualCard_cc    = md_IR_virtualCard_cc[li_counter];
      lo_createIrrental.virtualCard_exp_date    = md_IR_virtualCard_exp_date[li_counter];
      lo_createIrrental.virtualCard_deployment    = md_IR_virtualCard_deployment[li_counter];
      lo_createIrrental.virtualCard_card_usage_id    = md_IR_virtualCard_card_usage_id[li_counter];
      lo_createIrrental.virtualCard_gross_value_ind    = md_IR_virtualCard_gross_value_ind[li_counter];
      lo_createIrrental.pcc_cur_id    = md_IR_pcc_cur_id[li_counter];

      lo_createIrrental.man_creation	= (md_IR_source[li_counter] == "M" ? "Y" : "N");
      if ( lo_createIrrental.source.NotIsNull() )
		{
			 /**
			  * crs + source reference Reservationproduct
			  * must appear together
			  **/

          lo_createIrrental.crs_id  = md_IRT_crs_id;
		}
		else
		{
          lo_createIrrental.crs_id  = AB_null;
		}

      lo_createIrrental.harp_htlkey    = md_IR_harp_htlkey[li_counter];
      lo_createIrrental.customer_id    = md_IR_customer_id[li_counter++];

      dbIR.createIrrental(lo_createIrrental) ;
   }

   if (! CMsgHandler::getMsgStatus())
      return ;

	adjustTransportItinType();

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrtransport() ;

   if (! CMsgHandler::getMsgStatus())
      return ;


   createIrstopovercity() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIraddress() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrperson() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrticket() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrpayment() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrconsumdetail() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrother() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrfrequentuser() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

   createIrtaxdetail() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

	createIrreceipt() ;

	if (! CMsgHandler::getMsgStatus())
		return ;

	createIrdistribution() ;

	if (! CMsgHandler::getMsgStatus())
		return ;

	createIremailaddress() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

    createIrremark() ;

   if (! CMsgHandler::getMsgStatus())
      return ;

	cleanupIrtransport();

   if (! CMsgHandler::getMsgStatus())
      return ;

	updateIrRows();

   if (! CMsgHandler::getMsgStatus())
      return ;

   EXIT;
}

void CInterfaceRecord::cleanupIrtransport()
{
   CDIR                   	dbIR ;
   CIRCleanupIrtransport	lo_cleanupIrtransport;

	lo_cleanupIrtransport.session_id		= md_session_id;

	dbIR.cleanupIrtransport(lo_cleanupIrtransport);
}

void CInterfaceRecord::updateIrRows()
{
   CDIR                   	dbIR ;
   CIRUpdateIrRows			lo_updateIrRows;

	lo_updateIrRows.pc_session_id		= md_session_id;

	dbIR.updateIrRows(lo_updateIrRows);
}

void CInterfaceRecord::editDomains()
{
   D_char_numeric ld_cfa_id ;
   AB_Char lc_iata_test ;
   AB_Varchar lv_city_id(4) ;
	AB_Varchar lv_conId(3);

   D_amount ld_low_prac_fare, ld_full_fare, ld_value ;
   int li_counter ;

   li_counter = 0 ;
   lc_iata_test = md_IRT_iata_no.At(2) ;
   //lv_city_id = md_IRT_city_id ;
   lv_city_id.removeTrailingBlanks() ;

	LOG(logDEBUG) << "\nMELCF: md_IRT_cfa_id= " << md_IRT_cfa_id  ;

   ld_cfa_id = md_IRT_cfa_id.At(4).For(5) ;
	lv_conId  = md_IRT_con_id;

	lv_conId.removeLeadAndTrailBlanks() ;

	// G117 If con_id is NULL make it 900

  /* SCR#12815 : if con_id is null make it 000
    if it is invalid - length >0 and less than 3 make it 900 */

   if (lv_conId.IsNull() )
   {
      md_IRT_con_id = "000" ;
   }
   else if (( lv_conId.length() > 0 && lv_conId.length() < 3  ) ||
				( ! lv_conId.isAllAlphaNumeric() ) )
   {
      md_IRT_con_id = "900" ;
   }


	// Check Booking bra id
	if ( md_IRT_cop_id.IsNull() || md_IRT_bra_id.IsNull())
      CMsgHandler::setMsg(2982,
									" ",
									"ED-DOM",
									1,
									AB_Varchar(20,AB_null));

	// Check Format Booking Br
	if ((!md_IRT_cop_id.isAllAlphaNumeric()) && (!md_IRT_bra_id.isAllNumeric()))
      CMsgHandler::setMsg(2983,
								  " ",
								  "ED-DOM",
								  1,
								  AB_Varchar(20,AB_null));

	if (!CMsgHandler::getMsgStatus())
	{
   	md_rejectCon = AB_null;
   	md_rejectCop = AB_null;
   	md_rejectBra = AB_null;
		return ;
	}
	else
	{
   	md_rejectCop = md_IRT_cop_id;
   	md_rejectBra = md_IRT_bra_id;
		md_rejectCon = lv_conId;
	}


	// Consultant Id Check is on isAllAlphaNumeric

   if ((lv_conId.NotIsNull()) && (! lv_conId.isAllAlphaNumeric()))
	{
      CMsgHandler::setMsg( 1801,
									" ",
									"ED-DOM",
									1,
									md_IRT_con_id);
		md_rejectCon = AB_null;
	}

   else if (md_IRT_cfa_id.IsNull())
      CMsgHandler::setMsg( 3142,
									" ",
									"D-CFA",
									4,
									AB_Varchar(20,AB_null));

   else if (! md_IRT_cfa_code.isAllAlphaNumeric())
      CMsgHandler::setMsg( 1330,
									" ",
									"D-CFACODE",
									5,
									md_IRT_cfa_id);

   else if (! ld_cfa_id.isAllNumeric())
      CMsgHandler::setMsg(326,
								" ",
								"D-CFAID",
								6,
								md_IRT_cfa_id) ;

   else if (md_IRT_cfa_type.findFirstCharNotInList("NCTGPFQ") > 0)
      CMsgHandler::setMsg(1007,
									" ",
									"D-CFATYPE",
									7,
									md_IRT_cfa_id) ;

   else if (! editAddressDomain())
      CMsgHandler::setMsg(50,
									" ",
									"D-ADDRESS",
									7,
									md_IRT_cfa_id) ;

   else if ((! md_IRT_iata_no.isAllNumeric()) && (lc_iata_test.findFirstCharNotInList("123") == 0))
      CMsgHandler::setMsg(258,
									" ",
									"D-IATANO",
									11,
									md_IRT_cfa_id) ;

   else if (! editFormOfPayment())
      return ;

   else if (! editPhoneDomain())
      return ;

   else if (! editDocumentDomain())
      return ;

   while (li_counter < mi_IP_I)
   {
      if (! editNameDomain(md_IP_last_name[li_counter]))
         return ;
      else if (! editNameDomain(md_IP_first_name[li_counter]))
         return ;
      li_counter++ ;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CInterfaceRecord::editFormOfPayment()
{
   int li_counter = 0 ;

   while (li_counter < mi_IRP_I && CMsgHandler::getMsgStatus())
   {
   	if (md_IRP_form_of_pay[li_counter] == "CC")
   	{
      	if ((md_IRP_cc_id[li_counter].IsNull()) || (md_IRP_cc_no[li_counter].IsNull()))
         	CMsgHandler::setMsg(AB_Integer(53), AB_Char(15," "), D_error_track("D-CREDITCARD"), AB_Integer(1), AB_Varchar(20,AB_null)) ;
   	}
   	else if ((md_IRP_form_of_pay[li_counter] == "CA") || (md_IRP_form_of_pay[li_counter] == "CK") ||(md_IRP_form_of_pay[li_counter] == "IV"))
   	{
      	if ((md_IRP_cc_id[li_counter].NotIsNull()) || (md_IRP_cc_no[li_counter].NotIsNull()))
         	CMsgHandler::setMsg(AB_Integer(235), AB_Char(15," "), D_error_track("D-CREDITCARD"), AB_Integer(2), AB_Varchar(20,AB_null)) ;
      	else if (md_IRP_cc_exp_date[li_counter].NotIsNull())
         	CMsgHandler::setMsg(AB_Integer(416), AB_Char(15," "), D_error_track("D-CREDITCARD"), AB_Integer(3), AB_Varchar(20,AB_null)) ;
      	else if (md_IRP_auth_no[li_counter].NotIsNull())
         	CMsgHandler::setMsg(AB_Integer(417), AB_Char(15," "), D_error_track("D-CREDITCARD"), AB_Integer(4), AB_Varchar(20,AB_null)) ;
   	}
   	else if ( md_IRP_form_of_pay[li_counter] == "AP" )
   	{
		  // do nothing, AP is allowed after C0160(SCR 511862)
   	}
   	else
      	CMsgHandler::setMsg(AB_Integer(102), AB_Char(15," "), D_error_track("D-FORMOFPAY"), AB_Integer(5), AB_Varchar(20,AB_null)) ;

     	li_counter++ ;
   }

   if (! CMsgHandler::getMsgStatus())
      return AB_false ;
   else
      return AB_true ;
}
//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CInterfaceRecord::editAddressDomain()
{
   int li_counter = 0 ;
   while (li_counter < mi_IA_I && CMsgHandler::getMsgStatus())
   {
   	if ( md_IA_addr1[li_counter].IsNull() &&
			  (md_IA_addr2[li_counter].NotIsNull() ||
				md_IA_addr3[li_counter].NotIsNull() ||
				md_IA_addr4[li_counter].NotIsNull() ||
				md_IA_addr5[li_counter].NotIsNull() ||
				md_IA_addr6[li_counter].NotIsNull()) )
			return AB_false ;

   	if ( md_IA_addr2[li_counter].IsNull() &&
			  (md_IA_addr3[li_counter].NotIsNull() ||
			   md_IA_addr4[li_counter].NotIsNull() ||
			   md_IA_addr5[li_counter].NotIsNull() ||
				md_IA_addr6[li_counter].NotIsNull()) )
			return AB_false ;

   	if ( md_IA_addr3[li_counter].IsNull() &&
			  (md_IA_addr4[li_counter].NotIsNull() ||
			   md_IA_addr5[li_counter].NotIsNull() ||
				md_IA_addr6[li_counter].NotIsNull()) )
			return AB_false ;

   	if ( md_IA_addr4[li_counter].IsNull() &&
			  (md_IA_addr5[li_counter].NotIsNull() ||
				md_IA_addr6[li_counter].NotIsNull()) )
			return AB_false ;

   	if (md_IA_addr5[li_counter].IsNull() && md_IA_addr6[li_counter].NotIsNull())
			return AB_false ;

      li_counter++ ;
   }

   return AB_true ;
}

//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CInterfaceRecord::editDocumentDomain()
{
   int li_counter = 0 ;
   D_amount ld_low_prac_fare, ld_full_fare, ld_value ;

   while (li_counter < mi_ID_I && CMsgHandler::getMsgStatus())
   {
   	if (! editAmountDomain(md_ID_full_fare[li_counter], "#########.##",ld_full_fare))
      	CMsgHandler::setMsg(AB_Integer(268), AB_Char(15," "), D_error_track("D-FULLFARE"), AB_Integer(12), AB_Varchar(20,AB_null)) ;

   	else if (! editAmountDomain(md_ID_low_prac_fare[li_counter], "#########.##", ld_low_prac_fare))
      	CMsgHandler::setMsg(AB_Integer(269), AB_Char(15," "), D_error_track("D-LOWPRACFARE"), AB_Integer(13), AB_Varchar(20,AB_null)) ;

   	else if (! editAmountDomain(md_ID_rebate[li_counter], "#########.##", ld_low_prac_fare))
      	CMsgHandler::setMsg(AB_Integer(531), AB_Char(15," "), D_error_track("D-REBATE"), AB_Integer(14), AB_Varchar(20,AB_null)) ;

		/* SCR 2632
   	else if ((ld_low_prac_fare.NotIsNull() && ld_full_fare.NotIsNull()) && (ld_full_fare < ld_low_prac_fare))
      	CMsgHandler::setMsg(AB_Integer(269), AB_Char(15," "), D_error_track("D-FULLFARE"), AB_Integer(14), AB_Varchar(20,AB_null)) ;
		*/

      else if (! editAmountDomain(md_ID_base_cost[li_counter], "######.##", ld_value))
         CMsgHandler::setMsg(AB_Integer(116), AB_Char(15," "), D_error_track("D-BASECOST"), AB_Integer(2), AB_Varchar(20,AB_null)) ;

      else if (! editAmountDomain(md_ID_comm_rate[li_counter], "##.##", ld_value))
         CMsgHandler::setMsg(AB_Integer(120), AB_Char(15," "), D_error_track("D-COMMRATE"), AB_Integer(3), AB_Varchar(20,AB_null)) ;

      else if (! editAmountDomain(md_ID_commission[li_counter], "######.##", ld_value))
         CMsgHandler::setMsg(AB_Integer(120), AB_Char(15," "), D_error_track("D-COMMISSION"), AB_Integer(4), AB_Varchar(20,AB_null)) ;

      else if (! editAmountDomain(md_ID_penalty_ch_fee[li_counter], "#########.##", ld_value))
         CMsgHandler::setMsg(AB_Integer(196), AB_Char(15," "), D_error_track("D-PENCHFEE"), AB_Integer(5), AB_Varchar(20,AB_null)) ;

      else if (! md_ID_exchange_code[li_counter].NotIsNull() &&
                 md_ID_orig_ticket_no[li_counter].NotIsNull() &&
                 md_ID_penalty_ch_fee[li_counter].NotIsNull())
         CMsgHandler::setMsg(AB_Integer(2043), AB_Char(15," "), D_error_track("D-GIRDOCUMENTS"), AB_Integer(6), AB_Varchar(20,AB_null)) ;

      li_counter++ ;
   }

   if (! CMsgHandler::getMsgStatus())
      return AB_false ;
   else
      return AB_true ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::overrideHomePhone()
{
        // 516462
	editPhoneNumber(mv_phoneField, md_IA_phone_acode[mi_IA_I], md_IA_phone_no[mi_IA_I]);
        /*
	editPhoneNumber(mv_phoneField);

	if ( mv_phoneField.NotIsNull() || mv_phoneField != "") {
		if ( mv_phoneField.length() > 10 )
		{
			md_IA_phone_acode[mi_IA_I] = mv_phoneField.At(1).For(5) ;
			md_IA_phone_no[mi_IA_I]   = mv_phoneField.At(6) ;
		}
		else if ( mv_phoneField.length() > 7 )
		{
			md_IA_phone_acode[mi_IA_I] = mv_phoneField.At(1).For(3) ;
			md_IA_phone_no[mi_IA_I]   = mv_phoneField.At(4) ;
		}
		else
			md_IA_phone_no[mi_IA_I]    = mv_phoneField ;
	}
        */
}

AB_Boolean CInterfaceRecord::editPhoneDomain()
{
   int            li_counter = 0 ;
	D_area_code 	ld_phone_acode;
	D_phone_number ld_phone_no;
	D_area_code 	ld_bphone_acode;
	D_phone_number ld_bphone_no;
	D_area_code 	ld_mphone_acode;
	D_phone_number ld_mphone_no;
	D_area_code 	ld_fax_acode;
	D_phone_number ld_fax_no;

   while (li_counter < mi_IA_I && CMsgHandler::getMsgStatus())
   {
		ld_phone_acode = md_IA_phone_acode[li_counter] ;
		ld_phone_no    = md_IA_phone_no[li_counter] ;
		ld_bphone_acode = md_IA_bphone_acode[li_counter] ;
		ld_bphone_no    = md_IA_bphone_no[li_counter] ;
		ld_mphone_acode = md_IA_mphone_acode[li_counter] ;
		ld_mphone_no    = md_IA_mphone_no[li_counter] ;
		ld_fax_acode    = md_IA_fax_acode[li_counter] ;
		ld_fax_no       = md_IA_fax_no[li_counter] ;

		ld_phone_acode.removeTrailingBlanks() ;
		ld_phone_no.removeTrailingBlanks() ;
		ld_bphone_acode.removeTrailingBlanks() ;
		ld_bphone_no.removeTrailingBlanks() ;
		ld_mphone_acode.removeTrailingBlanks() ;
		ld_mphone_no.removeTrailingBlanks() ;
		ld_fax_acode.removeTrailingBlanks() ;
		ld_fax_no.removeTrailingBlanks() ;

   	if (! ld_phone_acode.isAllNumeric())
		{
			md_IA_phone_acode[li_counter] = AB_null ;
			md_IA_phone_no[li_counter] = AB_null ;
		}

   	else if (! ld_phone_no.isAllNumeric())
		{
			md_IA_phone_acode[li_counter] = AB_null ;
			md_IA_phone_no[li_counter] = AB_null ;
		}

   	else if ((ld_phone_acode.NotIsNull()) && (ld_phone_no.Length() < AB_Integer(7)))
		{
			md_IA_phone_acode[li_counter] = AB_null ;
			md_IA_phone_no[li_counter] = AB_null ;
		}

   	else if (! ld_bphone_acode.isAllNumeric())
		{
			md_IA_bphone_no[li_counter] = AB_null ;
			md_IA_bphone_acode[li_counter] = AB_null ;
		}

   	else if (! ld_bphone_no.isAllNumeric())
		{
			md_IA_bphone_acode[li_counter] = AB_null ;
			md_IA_bphone_no[li_counter] = AB_null ;
		}

   	else if ((ld_bphone_acode.NotIsNull()) && (ld_bphone_no.Length() < AB_Integer(7)))
		{
			md_IA_bphone_acode[li_counter] = AB_null ;
			md_IA_bphone_no[li_counter] = AB_null ;
		}

   	else if (! ld_mphone_acode.isAllNumeric())
		{
			md_IA_mphone_no[li_counter] = AB_null ;
			md_IA_mphone_acode[li_counter] = AB_null ;
		}

   	else if (! ld_mphone_no.isAllNumeric())
		{
			md_IA_mphone_acode[li_counter] = AB_null ;
			md_IA_mphone_no[li_counter] = AB_null ;
		}

   	else if ((ld_mphone_acode.NotIsNull()) && (ld_mphone_no.Length() < AB_Integer(7)))
		{
			md_IA_mphone_acode[li_counter] = AB_null ;
			md_IA_mphone_no[li_counter] = AB_null ;
		}

   	else if (! ld_fax_acode.isAllNumeric())
		{
			md_IA_fax_acode[li_counter]  = AB_null ;
			md_IA_fax_no[li_counter]  = AB_null ;

		}

   	else if (! ld_fax_no.isAllNumeric())
		{
			md_IA_fax_acode[li_counter]  = AB_null ;
			md_IA_fax_no[li_counter]  = AB_null ;
		}

   	else if ((ld_fax_acode.NotIsNull()) && (ld_fax_no.Length() < AB_Integer(7)))
		{
			md_IA_fax_acode[li_counter]  = AB_null ;
			md_IA_fax_no[li_counter]  = AB_null ;
		}

      li_counter++ ;
   }

   if (! CMsgHandler::getMsgStatus())
      return AB_false ;
   else
      return AB_true ;
}


//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::editCity(D_city &pm_city)
{
   // LOG(logDEBUG) << "START CInterfaceRecord::editCity"  ;
   AB_Varchar lv_city(4) ;

   lv_city = pm_city ;
   lv_city.removeTrailingBlanks() ;


   if (!lv_city.isAllAlpha() || lv_city.Length() < AB_Integer(3))
      pm_city = "ZZZ" ;

   // LOG(logDEBUG) << "END CInterfaceRecord::editCity"  ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::editUnitType(D_unit_type &pm_unitType)
{
   // LOG(logDEBUG) << "START CInterfaceRecord::editUnitType"  ;

   AB_Varchar lv_unitType(4) ;

   lv_unitType = pm_unitType ;
   lv_unitType.removeTrailingBlanks();

   if (!lv_unitType.isAllAlphaNumeric() || lv_unitType.IsNull())
      pm_unitType = "ZZZZ" ;

   // LOG(logDEBUG) << "END CInterfaceRecord::editUnitType"  ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::editSupplier(D_supplier &pm_supId)
{
   // LOG(logDEBUG) << "START CInterfaceRecord::editSupplier"  ;

   if (!pm_supId.For(2).isAllAlphaNumeric() || pm_supId.IsNull())
      pm_supId = "ZZ" ;

   // LOG(logDEBUG) << "END CInterfaceRecord::editSupplier"  ;
}

//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CInterfaceRecord::editNameDomain(D_person_name pd_name)
{
   int li_pos, li_pos2 ;

   pd_name.removeLeadingBlanks() ;
   pd_name.removeTrailingBlanks() ;
   li_pos = pd_name.findSubstring("*") ;
   li_pos2 = pd_name.findSubstring("/") ;

   if ((pd_name.IsNull()) || (li_pos > 0) || (li_pos2 > 0))
      CMsgHandler::setMsg(AB_Integer(1221), AB_Char(15," "), D_error_track("D-NAME"), AB_Integer(1), AB_Varchar(20,AB_null)) ;

   if (! CMsgHandler::getMsgStatus())
      return AB_false ;
   else
      return AB_true ;
}

//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CInterfaceRecord::editReceiptAmount(D_char_numeric pd_amount, const char* lv_format, D_amount &pd_value) {
	bool  lb_negative = false;
	if (pd_amount.For(1) == "-") {
		pd_amount = pd_amount.At(2);
		lb_negative = true;
	}

	AB_Boolean lb_ok = editAmountDomain(pd_amount, lv_format, pd_value);

	if (lb_ok) {
		if (lb_negative) {
			pd_value = pd_value * D_amount(-1);
		}
	}

	return lb_ok;
}

AB_Boolean CInterfaceRecord::editAmountDomain(D_char_numeric pd_amount, const char* lv_format, D_amount &pd_value)
{
   AB_Money lm_amount ;
   AB_Varchar lv_rawAmount(15) ;
   AB_BOOL lb_ok ;
	int	pos;

   if (pd_amount.IsNull())
      return AB_true ;

   lv_rawAmount = pd_amount ;
   lv_rawAmount.removeLeadingBlanks() ;
   lv_rawAmount.removeTrailingBlanks() ;
	pos = lv_rawAmount.findSubstring(".");

	if ( !pos )
		lv_rawAmount = lv_rawAmount + ".00";
	else
		if ( AB_Integer(pos) == lv_rawAmount.Length() )
			lv_rawAmount = lv_rawAmount + "00";
   lm_amount = lv_rawAmount.to_money(lv_format, lb_ok) ;
   pd_value = lm_amount.to_string() ;

   if ((!lb_ok) && (pd_value.to_double() != 0.00))
      return AB_false ;
   else
      return AB_true ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::print_Irtripheader()
{
   LOG(logDEBUG) << "***************************IRTRIPHEADER************************" ;
   LOG(logDEBUG) << "ms_Irtripheader.md_session_id= "     << md_session_id        ;
   LOG(logDEBUG) << "ms_Irtripheader.md_interface_type= " << md_interface_type    ;
   LOG(logDEBUG) << "ms_Irtripheader.md_creation_date= "  << md_creation_date     ;
   LOG(logDEBUG) << "ms_Irtripheader.md_iata_no= "        << md_IRT_iata_no        ;
   LOG(logDEBUG) << "ms_Irtripheader.md_crs_id= "         << md_IRT_crs_id         ;
   LOG(logDEBUG) << "ms_Irtripheader.md_cfa_type= "       << md_IRT_cfa_type       ;
   LOG(logDEBUG) << "ms_Irtripheader.md_cfa_code= "       << md_IRT_cfa_code       ;
   LOG(logDEBUG) << "ms_Irtripheader.md_cfa_id= "         << md_IRT_cfa_id         ;
   LOG(logDEBUG) << "ms_Irtripheader.md_con_id= "         << md_IRT_con_id         ;
   LOG(logDEBUG) << "ms_Irtripheader.md_iata_dest= "      << md_IRT_iata_dest      ;
   LOG(logDEBUG) << "ms_Irtripheader.md_city_id= "        << md_IRT_city_id        ;
   LOG(logDEBUG) << "ms_Irtripheader.md_pnr_date= "       << md_IRT_pnr_date       ;
   LOG(logDEBUG) << "ms_Irtripheader.md_pnr_time= "       << md_IRT_pnr_time       ;
   LOG(logDEBUG) << "ms_Irtripheader.md_record_creation_date= "       << md_IRT_record_creation_date       ;
   LOG(logDEBUG) << "ms_Irtripheader.md_record_creation_time= "       << md_IRT_record_creation_time       ;
   LOG(logDEBUG) << "ms_Irtripheader.md_start_date= "     << md_IRT_start_date     ;
   LOG(logDEBUG) << "ms_Irtripheader.md_cfa_dpt= "        << md_IRT_cfa_dpt        ;
   LOG(logDEBUG) << "ms_Irtripheader.md_mktg_list= "      <<  md_IRT_mktg_list     ;
   LOG(logDEBUG) << "ms_Irtripheader.md_reference= "      << md_IRT_reference      ;
   LOG(logDEBUG) << "ms_Irtripheader.md_booking_no= "      << md_IRT_booking_no    ;
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrremark()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IrRental's Remarks ************ "  ;
   while (li_counter < mi_IRMK_I)
   {
      LOG(logDEBUG) << "md_session_id= "     << md_session_id                        ;
      LOG(logDEBUG) << "md_segment_type= "   << md_IRMK_type[li_counter]    ;
      LOG(logDEBUG) << "md_segment_number= " << md_IRMK_line_number[li_counter]  ;
      LOG(logDEBUG) << "md_remark= "         << md_IRMK_remark[li_counter]          ;
      LOG(logDEBUG) << "md_pax_id= "         << md_IRMK_pax_id[li_counter]        ;
		li_counter++;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrtransport()
{
   int li_counter ;

   li_counter = 0 ;

    LOG(logDEBUG) << " ************ DATA FOR GIRTRANSPORT ************ "  ;
   while (li_counter < mi_IT_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= "     << md_session_id                     ;
      LOG(logDEBUG) << "md_itin_type= "      << md_IT_itin_type[li_counter]       ;
      LOG(logDEBUG) << "md_line_number= "    << md_IT_line_number[li_counter]     ;
      LOG(logDEBUG) << "md_start_date= "     << md_IT_start_date[li_counter]      ;
      LOG(logDEBUG) << "md_start_time= "     << md_IT_start_time[li_counter]      ;
      LOG(logDEBUG) << "md_orig_city_id= "   << md_IT_orig_city_id[li_counter]     ;
      LOG(logDEBUG) << "md_orig_cityname= "  << md_IT_orig_cityname[li_counter]   ;
      LOG(logDEBUG) << "md_cog_dep_city= "   << md_IT_cog_dep_city[li_counter]    ;
      LOG(logDEBUG) << "md_cog_dep_date= "   << md_IT_cog_dep_date[li_counter]    ;
      LOG(logDEBUG) << "md_cog_dep_time= "   << md_IT_cog_dep_time[li_counter]    ;
      LOG(logDEBUG) << "md_cog_dep_term= "   << md_IT_cog_dep_term[li_counter]    ;
      LOG(logDEBUG) << "md_cog_dep_gate= "   << md_IT_cog_dep_gate[li_counter]    ;
      LOG(logDEBUG) << "md_cog_of_miles= "   << md_IT_cog_of_miles[li_counter]    ;
      LOG(logDEBUG) << "md_end_date= "       << md_IT_end_date[li_counter]        ;
      LOG(logDEBUG) << "md_end_time= "       << md_IT_end_time[li_counter]        ;
      LOG(logDEBUG) << "md_dest_city_id= "   << md_IT_dest_city_id[li_counter]    ;
      LOG(logDEBUG) << "md_dest_cityname= "  << md_IT_dest_cityname[li_counter]   ;
      LOG(logDEBUG) << "md_classofservice= " << md_IT_classofservice[li_counter]  ;
      LOG(logDEBUG) << "md_service= "        << md_IT_service[li_counter]         ;
      LOG(logDEBUG) << "md_sup_id= "         << md_IT_sup_id[li_counter]          ;
      LOG(logDEBUG) << "md_reference= "      << md_IT_reference[li_counter]       ;
      LOG(logDEBUG) << "md_no_of_stops= "    << md_IT_no_of_stops[li_counter]     ;
      LOG(logDEBUG) << "md_dep_terminal= "   << md_IT_dep_terminal[li_counter]    ;
      LOG(logDEBUG) << "md_dep_gate= "       << md_IT_dep_gate[li_counter]        ;
      LOG(logDEBUG) << "md_report_time= "    << md_IT_report_time[li_counter]     ;
      LOG(logDEBUG) << "md_arr_gate= "       << md_IT_arr_gate[li_counter]        ;
      LOG(logDEBUG) << "md_arr_terminal= "   << md_IT_arr_terminal[li_counter]    ;
      LOG(logDEBUG) << "md_airline_locator= "<< md_IT_airline_locator[li_counter] ;
      LOG(logDEBUG) << "md_carrier_type= "   << md_IT_carrier_type[li_counter]    ;
      LOG(logDEBUG) << "md_meal= "           << md_IT_meal[li_counter]            ;
      LOG(logDEBUG) << "md_aff_carrier= "    << md_IT_aff_carrier[li_counter]     ;
      LOG(logDEBUG) << "md_actual_miles= "   << md_IT_actual_miles[li_counter]    ;
      LOG(logDEBUG) << "md_arrival_ind= "    << md_IT_arrival_ind[li_counter]     ;
      LOG(logDEBUG) << "md_elapsed_time= "   << md_IT_elapsed_time[li_counter]    ;
      li_counter++ ;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrconsumdetail()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IRCONSUMEDETAIL ************ "  ;
   while (li_counter < mi_ICD_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= "  << md_session_id                   ;
      LOG(logDEBUG) << "md_pax_id= "      << md_ICD_pax_id[li_counter]       ;
      LOG(logDEBUG) << "md_itin_type= "   << md_ICD_itin_type[li_counter]    ;
      LOG(logDEBUG) << "md_itin_line= "   << md_ICD_itin_line_number[li_counter]    ;
      LOG(logDEBUG) << "md_line_number= " << md_ICD_line_number[li_counter]  ;
      LOG(logDEBUG) << "md_seat= "        << md_ICD_seat[li_counter]         ;
      LOG(logDEBUG) << "md_connection= "  << md_ICD_connection[li_counter]   ;
      LOG(logDEBUG) << "md_smoking= "     << md_ICD_smoking[li_counter]      ;
      LOG(logDEBUG) << "md_fare_by_leg= " << md_ICD_fare_by_leg[li_counter]  ;
      LOG(logDEBUG) << "md_cur_id= "      << md_ICD_cur_id[li_counter]       ;
      LOG(logDEBUG) << "md_fare_basis= "  << md_ICD_fare_basis[li_counter]   ;
      LOG(logDEBUG) << "md_cog_seat= "    << md_ICD_cog_seat[li_counter]     ;
      LOG(logDEBUG) << "md_service_status= "    << md_ICD_service_status[li_counter]     ;
      LOG(logDEBUG) << "Text STORED= "    << md_ICD_text[li_counter]     ;
      li_counter ++ ;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::print_Irrental()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IRRENTAL ************ "  ;
   while (li_counter < mi_IR_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= "     << md_session_id                     ;
      LOG(logDEBUG) << "md_prod_code= "      << md_IR_prod_code[li_counter]       ;
      LOG(logDEBUG) << "md_itin_type= "      << md_IR_itin_type[li_counter]       ;
      LOG(logDEBUG) << "md_line_number= "    << md_IR_line_number[li_counter]     ;
      LOG(logDEBUG) << "md_start_date= "     << md_IR_start_date[li_counter]      ;
      LOG(logDEBUG) << "md_start_time= "     << md_IR_start_time[li_counter]      ;
      LOG(logDEBUG) << "md_origin_city_id= " << md_IR_origin_city_id[li_counter]  ;
      LOG(logDEBUG) << "md_end_date= "       << md_IR_end_date[li_counter]        ;
      LOG(logDEBUG) << "md_end_time= "       << md_IR_end_time[li_counter]        ;
      LOG(logDEBUG) << "md_dest_city_id= "   << md_IR_dest_city_id[li_counter]    ;
      LOG(logDEBUG) << "md_unit_type= "      << md_IR_unit_type[li_counter]       ;
      LOG(logDEBUG) << "md_unit_ct= "        << md_IR_unit_ct[li_counter]         ;
      LOG(logDEBUG) << "md_service_status= " << md_IR_service_status[li_counter]  ;
      LOG(logDEBUG) << "md_sup_id= "         << md_IR_sup_id[li_counter]          ;
      LOG(logDEBUG) << "md_source= "         << md_IR_source[li_counter]          ;
      LOG(logDEBUG) << "md_number_of_pax= "  << md_IR_number_of_pax[li_counter]   ;
      LOG(logDEBUG) << "md_ex_day_charge= "  << md_IR_ex_day_charge[li_counter]   ;
      LOG(logDEBUG) << "md_ex_hr_charge= "   << md_IR_ex_hr_charge[li_counter]    ;
      LOG(logDEBUG) << "md_ex_mile_charge= " << md_IR_ex_mile_charge[li_counter]  ;
      LOG(logDEBUG) << "md_free_mile_code= " << md_IR_free_mile_code[li_counter]  ;
      LOG(logDEBUG) << "md_free_mileage= "   << md_IR_free_mileage[li_counter]     ;
      LOG(logDEBUG) << "md_dropoff_charge= " << md_IR_dropoff_charge[li_counter]  ;
      LOG(logDEBUG) << "md_regular_rate= "   << md_IR_regular_rate[li_counter]    ;
      LOG(logDEBUG) << "md_booked_rate= "    << md_IR_booked_rate[li_counter]     ;
      LOG(logDEBUG) << "md_local_rate= "     << md_IR_local_rate[li_counter]      ;
      LOG(logDEBUG) << "md_cur_id= "         << md_IR_cur_id[li_counter]          ;
      LOG(logDEBUG) << "md_rate_type= "      << md_IR_rate_type[li_counter]       ;
      LOG(logDEBUG) << "md_rate_code= "      << md_IR_rate_code[li_counter]       ;
      LOG(logDEBUG) << "md_rate_duration= "  << md_IR_rate_duration[li_counter]   ;
      LOG(logDEBUG) << "md_rate_multip= "    << md_IR_rate_multip[li_counter]     ;
      LOG(logDEBUG) << "md_com_rate= "       << md_IR_com_rate[li_counter]        ;
      LOG(logDEBUG) << "md_multi_lvl_rate= " << md_IR_multi_lvl_rate[li_counter]  ;
      LOG(logDEBUG) << "md_saving_code= "    << md_IR_saving_code[li_counter]     ;
      LOG(logDEBUG) << "md_location= "       << md_IR_location[li_counter]        ;
      LOG(logDEBUG) << "md_location_id= "    << md_IR_location_id[li_counter]     ;
      LOG(logDEBUG) << "md_site_name= "      << md_IR_site_name[li_counter]       ;
      LOG(logDEBUG) << "md_addr1= "          << md_IR_addr1[li_counter]           ;
      LOG(logDEBUG) << "md_addr2= "          << md_IR_addr2[li_counter]           ;
      LOG(logDEBUG) << "md_addr3= "          << md_IR_addr3[li_counter]           ;
      LOG(logDEBUG) << "md_addr4= "          << md_IR_addr4[li_counter]           ;
      LOG(logDEBUG) << "md_phone_acode= "    << md_IR_phone_acode[li_counter]     ;
      LOG(logDEBUG) << "md_phone_no= "       << md_IR_phone_no[li_counter]        ;
      LOG(logDEBUG) << "md_fax_acode= "      << md_IR_fax_acode[li_counter]        ;
      LOG(logDEBUG) << "md_fax_no= "         << md_IR_fax_no[li_counter]          ;
      LOG(logDEBUG) << "md_fup= "            << md_IR_fup[li_counter]             ;
      LOG(logDEBUG) << "md_frequent_flier= " << md_IR_customer_id[li_counter]  ;
      LOG(logDEBUG) << "md_guarantee= "      << md_IR_guarantee[li_counter]       ;
      LOG(logDEBUG) << "md_confirmation= "   << md_IR_confirmation[li_counter]    ;
      LOG(logDEBUG) << "md_corporate_disc= " << md_IR_corporate_disc[li_counter]  ;
      LOG(logDEBUG) << "md_corporation_id= " << md_IR_corporation_id[li_counter]  ;
      LOG(logDEBUG) << "md_cancel_policy= "  << md_IR_cancel_policy[li_counter]   ;
      LOG(logDEBUG) << "md_IR_pickup_location= "  << md_IR_pickup_location[li_counter]   ;
      LOG(logDEBUG) << "md_IR_dropoff_location= " << md_IR_dropoff_location[li_counter]  ;
      LOG(logDEBUG) << "md_IR_discount_code= "    << md_IR_discount_code[li_counter]  ;
      LOG(logDEBUG) << "md_IR_commissionable= "   << md_IR_commissionable[li_counter]  ;
		LOG(logDEBUG) << "md_IR_preferred= "   << md_IR_preferred[li_counter]  ;
      LOG(logDEBUG) << "md_IR_service_status= "   << md_IR_service_status[li_counter]  ;
      LOG(logDEBUG) << "md_IR_action_code= " << md_IR_action_code[li_counter]  ;
      LOG(logDEBUG) << "md_IR_EB_source= "   << md_IR_EB_source[li_counter]          ;
      LOG(logDEBUG) << "md_IR_reason_code= " << md_IR_reason_code[li_counter]  ;

      li_counter++ ;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrperson()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IRPERSON ************ "  ;

   while (li_counter < mi_IP_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= "     << md_session_id                    ;
      LOG(logDEBUG) << "md_pax_id= "         << md_IP_pax_id[li_counter]        ;
      LOG(logDEBUG) << "md_title= "          << md_IP_title[li_counter]         ;
      LOG(logDEBUG) << "md_last_name= "      << md_IP_last_name[li_counter]     ;
      LOG(logDEBUG) << "md_first_name= "     << md_IP_first_name[li_counter]    ;
      LOG(logDEBUG) << "md_new_pax_id= "     << md_IP_new_pax_id[li_counter]    ;
      LOG(logDEBUG) << "md_new_pax_line= "   << md_IP_new_pax_line[li_counter]  ;
      LOG(logDEBUG) << "md_sex= "            << md_IP_sex[li_counter]           ;
      LOG(logDEBUG) << "md_smoking= "        << md_IP_smoking[li_counter]       ;
      LOG(logDEBUG) << "md_ctry_id= "        << md_IP_ctry_id[li_counter]       ;
      LOG(logDEBUG) << "md_passport_no= "    << md_IP_passport_no[li_counter]   ;
      LOG(logDEBUG) << "md_passport_exp= "   << md_IP_passport_exp[li_counter]  ;
      LOG(logDEBUG) << "md_reference= "      << md_IP_reference[li_counter]     ;
      LOG(logDEBUG) << "md_crs_invoice= "    << md_IP_crs_invoice[li_counter]   ;

      li_counter++ ;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrticket()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IRTICKET ************ "  ;

   while (li_counter < mi_ID_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= "     << md_session_id                    ;
      LOG(logDEBUG) << "md_pax_id= "         << md_ID_pax_id[li_counter]        ;
      LOG(logDEBUG) << "md_line_number = "   << md_ID_line_number[li_counter]  ;
      LOG(logDEBUG) << "md_saving_code = "   << md_ID_saving_code[li_counter]  ;
      LOG(logDEBUG) << "md_cost_line= "      << md_ID_cost_line[li_counter]     ;
      LOG(logDEBUG) << "md_reference= "      << md_ID_reference[li_counter]     ;
      LOG(logDEBUG) << "md_sup_id= "         << md_ID_sup_id[li_counter]         ;
      LOG(logDEBUG) << "md_cur_id= "         << md_ID_cur_id[li_counter]        ;
      LOG(logDEBUG) << "md_form= "           << md_ID_form[li_counter]          ;
      LOG(logDEBUG) << "md_ticket_no= "      << md_ID_ticket_no[li_counter]     ;
      LOG(logDEBUG) << "md_pic= "            << md_ID_pic[li_counter]           ;
      LOG(logDEBUG) << "md_orig_ticket_no= " << md_ID_orig_ticket_no[li_counter] ;
      LOG(logDEBUG) << "md_comd_rate= "      << md_ID_comm_rate[li_counter]     ;
      LOG(logDEBUG) << "md_commission= "     << md_ID_commission[li_counter]    ;
      LOG(logDEBUG) << "md_no_of_books= "    << md_ID_no_of_books[li_counter]   ;
      LOG(logDEBUG) << "md_exchange_code= "  << md_ID_exchange_code[li_counter] ;
      LOG(logDEBUG) << "md_penalty_ch_fee= " << md_ID_penalty_ch_fee[li_counter] ;
      LOG(logDEBUG) << "md_base_cost= "      << md_ID_base_cost[li_counter]     ;
      LOG(logDEBUG) << "md_e_ticket= "       << md_ID_e_ticket[li_counter]     ;

      li_counter++ ;
   }
}
void CInterfaceRecord::printFreqUser()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IRFREQUSER ************ "  ;
   while (li_counter < mi_IRF_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= " << md_session_id                ;
      LOG(logDEBUG) << "md_pax_id= "     << md_IRF_pax_id[li_counter]    ;
      LOG(logDEBUG) << "md_sup_id= "     << md_IRF_sup_id[li_counter]    ;
      LOG(logDEBUG) << "md_fup_type= "   << md_IRF_fup_type[li_counter]  ;
      LOG(logDEBUG) << "md_fup= "        << md_IRF_fup[li_counter]       ;
      LOG(logDEBUG) << "md_fup_status= " << md_IRF_fup_status[li_counter]  ;
      LOG(logDEBUG) << "md_srvc_sup_id= "<< md_IRF_srvc_sup_id[li_counter] ;
      li_counter++ ;
   }
}

//---------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIraddress()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IRADDRESS ************ "  ;
   while (li_counter < mi_IA_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= " << md_session_id                  ;
      LOG(logDEBUG) << "md_IA_type= "     << md_IA_type[li_counter]       ;
      LOG(logDEBUG) << "md_IA_addr1= "     << md_IA_addr1[li_counter]       ;
      LOG(logDEBUG) << "md_IA_addr2= "     << md_IA_addr2[li_counter]       ;
      LOG(logDEBUG) << "md_IA_addr3= "     << md_IA_addr3[li_counter]       ;
      LOG(logDEBUG) << "md_IA_addr4= "     << md_IA_addr4[li_counter]       ;
      LOG(logDEBUG) << "md_IA_addr5= "     << md_IA_addr5[li_counter]       ;
      LOG(logDEBUG) << "md_IA_addr6= "     << md_IA_addr6[li_counter]       ;
      LOG(logDEBUG) << "md_IA_phone_no= " << md_IA_phone_no[li_counter]   ;

      li_counter++ ;
   }
}

//---------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrtaxdetail()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IRTAXDETAIL ************ "  ;
   while (li_counter < mi_ITD_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= " << md_session_id                  ;
      LOG(logDEBUG) << "md_pax_id= "     << md_ITD_pax_id[li_counter]       ;
      LOG(logDEBUG) << "md_tax_id= "     << md_ITD_tax_id[li_counter]       ;
      LOG(logDEBUG) << "md_tax_source= " << md_ITD_tax_source[li_counter]       ;
      LOG(logDEBUG) << "md_city_id= "     << md_ITD_city_id[li_counter]       ;
      LOG(logDEBUG) << "md_line_number= "     << md_ITD_line_number[li_counter]       ;
      LOG(logDEBUG) << "md_tax_amount= " << md_ITD_tax_amount[li_counter]   ;

      li_counter++ ;
   }
}

//---------------------------------------------------------------------------------------------------
void CInterfaceRecord::printOther()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ DATA FOR IROTHER ************ "  ;
   while (li_counter < mi_IO_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;
      LOG(logDEBUG) << "md_session_id= " << md_session_id                  ;
      LOG(logDEBUG) << "prod_code= "    << md_IO_prod_code[li_counter]  ;
      LOG(logDEBUG) << "itin_type= "    << md_IO_itin_type[li_counter]  ;
      LOG(logDEBUG) << "line_number= "  << md_IO_line_number[li_counter]  ;
      LOG(logDEBUG) << "start_date= "   << md_IO_start_date[li_counter]  ;
      LOG(logDEBUG) << "start_time= "   << md_IO_start_time[li_counter]  ;
      LOG(logDEBUG) << "orig_city_id= " << md_IO_orig_city_id[li_counter]  ;
      LOG(logDEBUG) << "end_date= "     << md_IO_end_date[li_counter]  ;
      LOG(logDEBUG) << "end_time= "     << md_IO_end_time[li_counter]  ;
      LOG(logDEBUG) << "dest_city_id= " << md_IO_dest_city_id[li_counter]  ;
      LOG(logDEBUG) << "unit_type= "    << md_IO_unit_type[li_counter]  ;
      LOG(logDEBUG) << "unit_ct= "      << md_IO_unit_ct[li_counter]  ;
      LOG(logDEBUG) << "sup_id= "       << md_IO_sup_id[li_counter]  ;
      LOG(logDEBUG) << "reference= "    << md_IO_reference[li_counter]  ;
      LOG(logDEBUG) << "saving_code= "  << md_IO_saving_code[li_counter]  ;
      // GAP US089 START
      LOG(logDEBUG) << "confirmation= " << md_IO_confirmation[li_counter]  ;
      // GAP US089 END
      // GAP US092 START
      LOG(logDEBUG) << "departure_point= "   << md_IO_departure_point[li_counter]  ;
      LOG(logDEBUG) << "arrival_point= "    	<< md_IO_arrival_point[li_counter]  ;
      LOG(logDEBUG) << "svc_description= "   << md_IO_svc_description[li_counter]  ;
      LOG(logDEBUG) << "cabin_no= " 			<< md_IO_cabin_no[li_counter]  ;
      LOG(logDEBUG) << "tour_no= " 				<< md_IO_tour_no[li_counter]  ;
      // GAP US092 END

      li_counter++;
   }
}


void CInterfaceRecord::checkDomainEdits()
{
   editDomains() ;

   if (! CMsgHandler::getMsgStatus())
   {
      LOG(logERROR) << "1) FAILED!!!!!!!"  ;
      CMsgHandler::getAndDisplayMsg("E") ;
   }
}


//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrpayment()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ IRPAYMENT ************ "  ;
   while (li_counter < mi_IRP_I)
   {
      LOG(logDEBUG) << "md_session_id= "     << md_session_id     ;
		LOG(logDEBUG) << "md_IRP_form_of_pay= " << md_IRP_form_of_pay[li_counter]  ;
		LOG(logDEBUG) << "md_IRP_cc_id= " << md_IRP_cc_id[li_counter]  ;
		LOG(logDEBUG) << "md_IRP_cc_no= " << md_IRP_cc_no[li_counter]  ;
		LOG(logDEBUG) << "md_IRP_cc_exp_date= " << md_IRP_cc_exp_date[li_counter]  ;
		LOG(logDEBUG) << "md_IRP_auth_no= " << md_IRP_auth_no[li_counter]  ;
		LOG(logDEBUG) << "md_IRP_line_number= " << md_IRP_line_number[li_counter]  ;
		LOG(logDEBUG) << "md_IRP_pay_id= " << md_IRP_pay_id[li_counter]  ;
		LOG(logDEBUG) << "md_IRP_amount= " << md_IRP_amount[li_counter]  ;
      LOG(logDEBUG) << "md_IRP_auth_source= " << md_IRP_auth_source[li_counter]  ;
		li_counter++;
   }
}

void CInterfaceRecord::printIrstopovercity()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ IRSTOPOVERCITY ************ "  ;
   while (li_counter < mi_ISOC_I)
   {
      LOG(logDEBUG) << "md_session_id= "     << md_session_id     ;
		LOG(logDEBUG) << "md_ISOC_itin_type= " << md_ISOC_itin_type[li_counter]  ;
		LOG(logDEBUG) << "md_ISOC_line_number= " << md_ISOC_line_number[li_counter]  ;
		LOG(logDEBUG) << "md_ISOC_order_number= " << md_ISOC_order_number[li_counter]  ;
		LOG(logDEBUG) << "md_ISOC_stop_over_city= " << md_ISOC_stop_over_city[li_counter]  ;
		li_counter++;
   }
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::printIrreceipt()
{
   int li_counter ;

   li_counter = 1 ;

   LOG(logDEBUG) << " ************ IRRECEIPT ************ "  ;
   while (li_counter <= mi_RC_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;

      LOG(logDEBUG) << "md_session_id= "     << md_session_id     ;
		LOG(logDEBUG) << "md_RC_cop_id= " << md_RC_cop_id  ;
		LOG(logDEBUG) << "md_RC_bra_id= " << md_RC_bra_id  ;
		LOG(logDEBUG) << "md_RC_lang_id= " << md_RC_lang_id  ;
		LOG(logDEBUG) << "md_RC_name= " << md_RC_name[li_counter]  ;
		LOG(logDEBUG) << "md_RC_bank_acc= " << md_RC_bank_acc[li_counter]  ;
		LOG(logDEBUG) << "md_RC_total_amt= " << md_RC_total_amt[li_counter]  ;
      LOG(logDEBUG) << "md_RC_gl_acc1= "     << md_RC_gl_acc1[li_counter]     ;
		LOG(logDEBUG) << "md_RC_gl_amt1= " << md_RC_gl_amt1[li_counter]  ;
		LOG(logDEBUG) << "md_RC_gl_acc2= " << md_RC_gl_acc2[li_counter]  ;
		LOG(logDEBUG) << "md_RC_gl_amt2= " << md_RC_gl_amt2[li_counter]  ;
		LOG(logDEBUG) << "md_RC_gl_acc3= " << md_RC_gl_acc3[li_counter]  ;
		LOG(logDEBUG) << "md_RC_gl_amt3= " << md_RC_gl_amt3[li_counter]  ;
      LOG(logDEBUG) << "md_RC_form_of_pay= " << md_RC_form_of_pay[li_counter]  ;
      LOG(logDEBUG) << "md_RC_cc_id= " << md_RC_cc_id[li_counter]  ;
      LOG(logDEBUG) << "md_RC_cc_no= " << md_RC_cc_no[li_counter]  ;
      LOG(logDEBUG) << "md_RC_cc_exp_date= " << md_RC_cc_exp_date[li_counter]  ;
      LOG(logDEBUG) << "md_RC_auth_no= " << md_RC_auth_no[li_counter]  ;
      LOG(logDEBUG) << "md_RC_link_to= " << md_RC_link_to[li_counter]  ;
      LOG(logDEBUG) << "md_RC_payment_reference= " << md_RC_payment_reference[li_counter]  ;
		li_counter++;
   }
}

//------------------------------------------------------------------------------------------
void CInterfaceRecord::createARNK()
{
/*
	int li_index,li_arnkLine ;
	AB_Varchar    lv_prevAir(255) ;  // Previour Air Line
	AB_Varchar    lv_presAir(255) ;  // Present Air Line
	AB_Integer    li_prevAirLine ;  // Previour itin Line
	AB_Integer    li_presAirLine ;  // Present itin Line
	V_AB_Integer  lia_tktLine(200) ;  // Tkt Line no.
	V_AB_Integer  lia_paxLine(200) ;  // Tkt Line no.

	li_index = 1 ;
	li_arnkLine = 200 ;

	while ( li_index < mi_surfaceTransport )
	{
		LOG(logDEBUG) << "lv_prevAir: " << lv_prevAir  ;

		if ( li_index > 1 )
		{
			lv_presAir = mva_surfaceTransport[li_index] ;
			li_presAirLine = lv_presAir.For(2) ;
			lv_presAir = lv_presAir.At(3) ;

			LOG(logDEBUG) << "lv_presAir: " << lv_presAir  ;;

			if ( lv_prevAir.At(5).For(3) != lv_presAir.For(3) )
			{
				//check whether both itineraries belong to same tixket
				//if li_tktLine = 0 go ahead and associate itin
				//					with all the ticket
				//if li_tktLine > 0 go ahead and associate itin
				//					with the ticket no
				//if li_tktLine = NULL Dont create ARNK the itins are for
				//					different ticket ( Split ticket )

				checkTicket(li_prevAirLine,li_presAirLine,lia_tktLine,lia_paxLine) ;

				//if ( lia_tktLine[1].IsNull() || lia_tktLine[1] != AB_Integer(0) )

				if ( lia_tktLine[1].NotIsNull() )
				{
					// Needed a surface Segment

					md_IT_itin_type[mi_IT_I] = "A" ;
					md_IT_line_number[mi_IT_I]    = li_arnkLine++ ;
					md_IT_start_date[mi_IT_I] = lv_presAir.At(9).For(7) ;
					md_IT_orig_city_id[mi_IT_I] = lv_prevAir.At(5).For(3) ;
					md_IT_dest_city_id[mi_IT_I] = lv_presAir.For(3) ;
					md_IT_sup_id[mi_IT_I]       = "ZZ" ;
					md_IT_service[mi_IT_I]      = "ARNK" ;
					md_IT_classofservice[mi_IT_I] = "A" ;
					md_IT_arrival_ind[mi_IT_I]    = lv_presAir.At(16).For(1) ;
					md_IT_reference[mi_IT_I]      = md_IRT_reference ;

					if ( lia_tktLine[1].IsNull())
					{
						// Create Irconsumdetail for all passenger
						createAllArnkICD() ;
					}
					else
					{
						// create Irconsumdetail for selected pax and ticket
						createSpecArnkICD(lia_tktLine,lia_paxLine) ;
					}
					mi_IT_I++ ;
				}

			}
		}

		lv_prevAir = mva_surfaceTransport[li_index] ;
		li_prevAirLine = lv_prevAir.For(2) ;
		lv_prevAir = lv_prevAir.At(3) ;


		li_index++ ;
	}
	*/
	return ;
}

//---------------------------------------------------------------------------------------------------------
void CInterfaceRecord::checkTicket( AB_Integer   &pi_firstAirLine,
												AB_Integer   &pi_secAirLine,
												V_AB_Integer &pia_tktLine,
												V_AB_Integer &pia_paxLine)
{
	AB_Integer		li_firstTktLine,li_secTktLine ;
	AB_Integer		li_firstPaxLine,li_secPaxLine ;
	int				li_index = 0,li_counter = 1,li_pax = 0 ; ;
	AB_Boolean		lb_manual ;


	while ( li_pax < mi_IP_I )
	{
		li_index = 0 ;
		lb_manual = AB_false ;
		li_firstTktLine = AB_null ;
		li_secTktLine = AB_null ;

		while ( li_index < mi_ICD_I )
		{
			if ( lb_manual )
				break ;
			if ( md_ICD_itin_type[li_index] == "A" )
			{
				if ( md_ICD_pax_id[li_index] == md_IP_pax_id[li_pax] )
				{

					if ( md_ICD_itin_line_number[li_index] == pi_firstAirLine )
					{
						li_firstTktLine = md_ICD_line_number[li_index] ;
						li_firstPaxLine = md_ICD_pax_id[li_index] ;

						if ( li_firstTktLine.IsNull() )
						{
							lb_manual = AB_true ;
							pia_tktLine[li_counter] = AB_Integer(0) ;
							pia_paxLine[li_counter++] = li_firstPaxLine ;
						}
					}

					if ( md_ICD_itin_line_number[li_index] == pi_secAirLine )
					{
						li_secTktLine = md_ICD_line_number[li_index] ;
						li_secPaxLine = md_ICD_pax_id[li_index] ;

						if ( li_secTktLine != li_firstTktLine )
						{
							pia_tktLine[1] = AB_null ;
							return ;
						}
						else
						{
							pia_tktLine[li_counter]   = li_firstTktLine ;
							pia_paxLine[li_counter++] = li_firstPaxLine ;
						}
					}
				}
			}

			li_index++ ;
		}
		li_pax++ ;
	}

}

//---------------------------------------------------------------------------------------------------------
void CInterfaceRecord::createAllArnkICD()
{
	int   li_counter=0 ;

	while ( li_counter < mi_IP_I )
	{
		md_ICD_pax_id[mi_ICD_I] = md_IP_pax_id[li_counter] ;
		md_ICD_itin_line_number[mi_ICD_I] = md_IT_line_number[mi_IT_I] ;
		md_ICD_itin_type[mi_ICD_I] = md_IT_itin_type[mi_IT_I] ;

		li_counter++ ;
		mi_ICD_I++ ;
	}
}

//---------------------------------------------------------------------------------------------------------
void CInterfaceRecord::createSpecArnkICD( V_AB_Integer  &pia_tktLine,
														V_AB_Integer  &pia_paxLine)
{
	int li_counter = 1 ;

	while ( pia_tktLine[li_counter].NotIsNull() )
	{
		md_ICD_pax_id[mi_ICD_I] = pia_paxLine[li_counter] ;

		if ( pia_tktLine[li_counter] == AB_Integer(0) )
		{
			md_ICD_line_number[mi_ICD_I] = AB_null ;
		}
		else
		{
			md_ICD_line_number[mi_ICD_I] = pia_tktLine[li_counter] ;
		}

		md_ICD_itin_line_number[mi_ICD_I] = md_IT_line_number[mi_IT_I] ;
		md_ICD_itin_type[mi_ICD_I] = md_IT_itin_type[mi_IT_I] ;

		li_counter++ ;
		mi_ICD_I++ ;
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void CInterfaceRecord::processPassengerName(AB_Text &pc_full_name)
{

	// WS 17 START
   V_AB_Varchar lt_name(70,60) ;
	// WS 17 END
   int li_count ;

   md_IP_last_name[mi_IP_I]  = AB_null ;
   md_IP_first_name[mi_IP_I] = AB_null ;

   li_count = lt_name.parseString(pc_full_name,"/") ;

   if (li_count == 0)
   {
      li_count = pc_full_name.findSubstring(" ") ;
      if ((li_count != 0) && (li_count < pc_full_name.length()))
      {
         md_IP_last_name[mi_IP_I]  = pc_full_name.For(li_count - 1) ;
         md_IP_first_name[mi_IP_I] = pc_full_name.At(li_count + 1) ;
      }
      else
      {
         CMsgHandler::setMsg(3373, " ", "PROCESS_PAX", 1, AB_Varchar(20,AB_null));
      }
   }
   else if (li_count == 2)
   {
      md_IP_last_name[mi_IP_I]  = lt_name[1] ;
      md_IP_first_name[mi_IP_I] = lt_name[2] ;
   }
   else
      CMsgHandler::setMsg(3373, " ", "PROCESS_PAX", 2, AB_Varchar(20,AB_null));

   if (CMsgHandler::getMsgStatus())
   {
      if (md_IP_last_name[mi_IP_I].IsNull() || md_IP_first_name[mi_IP_I].IsNull())
         CMsgHandler::setMsg(532, " ", "PROCESS_PAX", 3, AB_Varchar(20,AB_null));
      else if (md_IP_last_name[mi_IP_I] == " " || md_IP_first_name[mi_IP_I] == " ")
         CMsgHandler::setMsg(532, " ", "PROCESS_PAX", 4, AB_Varchar(20,AB_null));
   }

}

//----------------------------------------------------------------------------------------------
void CInterfaceRecord::setMsg(D_description &pd_errorDescription)
{
	AB_Integer    li_msgId ;

	li_msgId = AB_null ;

	// LOG(logDEBUG)  << "mv_syb_message:" << mv_syb_message  ;

	if ( mi_syb_code.NotIsNull() )
	{
		int li_pos ;

		// BAD DATA BASE CALL OCCURRED

		li_pos = mv_syb_message.findSubstring("Irremark") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3455 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irconsumdetail") ;

		if ( li_pos > 0 )
		{
			LOG(logDEBUG) << "WE ARE IN IRCONSUMDETIAL" ;
			if (md_interface_type == "IUR" )
			{
				li_msgId = 3456 ;
				goto outOfIf ;
			}
			else
			{
				li_msgId = 3457 ;
				goto outOfIf ;
			}
		}

		li_pos = mv_syb_message.findSubstring("Irfrequentuser") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3458 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irtaxdetail") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3459 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irpayment") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3460 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irticket") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3461 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irperson") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3462 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irrental") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3463 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irtransport") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3464 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irother") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3465 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Iraddress") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3466 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Irtripheader") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3467 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("deadlock") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3468 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Rental") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3463 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Transport") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3464 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Other") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3465 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Personitin") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3469 ;
			goto outOfIf ;
		}

		li_pos = mv_syb_message.findSubstring("Citytax") ;

		if ( li_pos > 0 )
		{
			li_msgId = 3470 ;
			goto outOfIf ;
		}

	}

	outOfIf: // LOG(logDEBUG) << "Found error Msg"  ;

	if ( li_msgId.NotIsNull() )
	{
		if ( md_txn_id != "TRM" )
			CMsgHandler::getMsgDescription(li_msgId, "E", pd_errorDescription) ;
		else
		{
      	CMsgHandler::setMsg(li_msgId," ","ED-DOM",1,AB_Varchar(20,AB_null));
			CMsgHandler::getMsgDescription(li_msgId, "E", pd_errorDescription) ;
		}
	}
	else
	{
		if ( md_txn_id == "TRM" && md_queType != "SFC" && md_queType != "REC" && md_queType != "POS")
		{
      	CMsgHandler::setMsg(3471," ","ED-DOM",1,AB_Varchar(20,AB_null));
		}
	}
}

//----------------------------------------------------------------------------------------------
void CInterfaceRecord::initTicket(int  &pi_index)
{

	md_ID_pax_id[pi_index] = AB_null ;
	md_ID_cost_line[pi_index] = AB_null ;
	md_ID_reference[pi_index] = AB_null ;
	md_ID_sup_id[pi_index] = AB_null ;
	md_ID_cur_id[pi_index] = AB_null ;
	md_ID_form[pi_index] = AB_null ;
	md_ID_ticket_no[pi_index] = AB_null ;
	md_ID_pic[pi_index] = AB_null ;
	md_ID_orig_ticket_no[pi_index] = AB_null ;
	md_ID_comm_rate[pi_index] = AB_null ;
	md_ID_commission[pi_index] = AB_null ;
	md_ID_no_of_books[pi_index] = AB_null ;
	md_ID_exchange_code[pi_index] = AB_null ;
	md_ID_penalty_ch_fee[pi_index] = AB_null ;
	md_ID_comm_penalty[pi_index] = AB_null ;
	md_ID_base_cost[pi_index] = AB_null ;
	md_ID_saving_code[pi_index] = AB_null ;
	md_ID_full_fare[pi_index] = AB_null ;
	md_ID_low_prac_fare[pi_index] = AB_null ;
	md_ID_personal_crs[pi_index] = AB_null ;
	md_ID_stp_home_city[pi_index] = AB_null ;
	md_ID_stp_host_city[pi_index] = AB_null ;
	md_ID_sel_access[pi_index] = AB_null ;
	md_ID_crs_tour[pi_index] = AB_null ;
	md_ID_rebate[pi_index] = AB_null ;
	md_ID_doc_type[pi_index] = AB_null ;
			// QREX START
	md_ID_refund[pi_index] = AB_null;
			// QREX END
}

void CInterfaceRecord::initIrtransport(const int& index) {
   md_IT_itin_type[index] = AB_null;
   md_IT_line_number[index] = AB_null;
   md_IT_start_date[index] = AB_null;
   md_IT_start_time[index] = AB_null;
   md_IT_orig_city_id[index] = AB_null;
   md_IT_orig_cityname[index] = AB_null;
   md_IT_chg_of_gauge[index] = AB_null;
   md_IT_cog_dep_city[index] = AB_null;
   md_IT_cog_dep_date[index] = AB_null;
   md_IT_cog_dep_time[index] = AB_null;
   md_IT_cog_dep_term[index] = AB_null;
   md_IT_cog_dep_gate[index] = AB_null;
   md_IT_cog_of_miles[index] = AB_null;
   md_IT_end_date[index] = AB_null;
   md_IT_end_time[index] = AB_null;
   md_IT_dest_city_id[index] = AB_null;
   md_IT_dest_cityname[index] = AB_null;
   md_IT_classofservice[index] = AB_null;
   md_IT_service[index] = AB_null;
   md_IT_sup_id[index] = AB_null;
   md_IT_reference[index] = AB_null;
   md_IT_no_of_stops[index] = AB_null;
   md_IT_dep_terminal[index] = AB_null;
   md_IT_dep_gate[index] = AB_null;
   md_IT_report_time[index] = AB_null;
   md_IT_arr_gate[index] = AB_null;
   md_IT_arr_terminal[index] = AB_null;
   md_IT_carrier_type[index] = AB_null;
   md_IT_meal[index] = AB_null;
   md_IT_aff_carrier[index] = AB_null;
   md_IT_actual_miles[index] = AB_null;
   md_IT_arrival_ind[index] = AB_null;
   md_IT_elapsed_time[index] = AB_null;
   md_IT_airline_locator[index] = AB_null;
}

void CInterfaceRecord::initIrconsumdetail(const int& index) {
   md_ICD_pax_id[index] = AB_null;
   md_ICD_itin_type[index] = AB_null;
   md_ICD_line_number[index] = AB_null;
   md_ICD_itin_line_number[index] = AB_null;
   md_ICD_seat[index] = AB_null;
   md_ICD_connection[index] = AB_null;
   md_ICD_smoking[index] = AB_null;
   md_ICD_fare_by_leg[index] = AB_null;
   md_ICD_fare_basis[index] = AB_null;
   md_ICD_tkt_desig[index] = AB_null;
   md_ICD_cog_seat[index] = AB_null;
   md_ICD_baggage[index] = AB_null;
   md_ICD_crs_priced[index] = AB_null;
   md_ICD_service_status[index] = AB_null;
   md_ICD_ftp_miles[index] = AB_null;
   md_ICD_boarding_pass[index] = AB_null;
   md_ICD_special_meal[index] = AB_null;
   md_ICD_special_need[index] = AB_null;
   md_ICD_text[index] = AB_null;
	md_ICD_cur_id[index] = AB_null;
}

void CInterfaceRecord::initIrpayment(int index) {
   md_IRP_pax_id[index] = AB_null;
   md_IRP_line_number[index] = AB_null;
   md_IRP_pay_id[index] = AB_null;
   md_IRP_amount[index] = AB_null;
   md_IRP_form_of_pay[index] = AB_null;
   md_IRP_cc_id[index] = AB_null;
   md_IRP_cc_no[index] = AB_null;
   md_IRP_cc_exp_date[index] = AB_null;
   md_IRP_auth_no[index] = AB_null;
   md_IRP_auth_source[index] = AB_null;
}

//----------------------------------------------------------------------------------------------
void CInterfaceRecord::adjustPNRDate( const AB_Date &pt_creation_date,
												  const AB_Text &pv_pnrDate,
								 		 				  AB_Text &pv_IRT_pnr_date,
										 				  AB_Date &pt_pnrDate)
{
	AB_Text 		lv_yyyy;
	AB_Integer 	li_yyyy;
	AB_Date 		lt_pnrDate;
	AB_Text 		lv_pnrDate;

	lv_pnrDate = pv_pnrDate;
	lv_pnrDate.removeLeadAndTrailBlanks();

	if ( lv_pnrDate.NotIsNull() )
	{
		if ( lv_pnrDate.length() != 5 )
		{
			pv_IRT_pnr_date 	= pv_pnrDate;
			pt_pnrDate 			= pv_pnrDate;
			return;
		}
	}
	else
	{
		pv_IRT_pnr_date 	= AB_null;
		pt_pnrDate 			= AB_null;
		return;
	}

	// get current year
	lv_yyyy = pt_creation_date.to_stringFormat( "YYYY" );

	// append current year to pnr date string
	lt_pnrDate = lv_pnrDate + lv_yyyy;
	if ( lt_pnrDate.IsNull() )
	{
		CMsgHandler::setMsg(3980, " ", "ADJ-DATE", 1, AB_Varchar(20,AB_null));
		return;
	}



//	if ( lt_pnrDate > pt_creation_date.get_start_of_day() )
	if ( (lt_pnrDate - pt_creation_date.get_start_of_day()) > AB_Integer(24L * 60L * 60L) )
	{
		lt_pnrDate.addDate(-1,0,0);
	}

	if ( lt_pnrDate.IsNull() )
	{
		CMsgHandler::setMsg(3980, " ", "ADJ-DATE", 2, AB_Varchar(20,AB_null));
		return;
	}

	pt_pnrDate 			= lt_pnrDate;
	pv_IRT_pnr_date 	= lt_pnrDate.to_stringFormat("DDMMMYY");

}

//----------------------------------------------------------------------------------------------
void CInterfaceRecord::adjustRecordCreationDate(
									const AB_Date &pt_creation_date,
									const AB_Text &pv_RecordCreationDate,
									const AB_Text &pv_RecordCreationTime,
								 		 	AB_Text &pv_IRT_RecordCreation_date)
{
	AB_Text 		lv_yyyy;
	AB_Integer 	li_yyyy;
	AB_Date 		lt_RecordCreationDate;
	AB_Text 		lv_RecordCreationDate;
	AB_Text 		lv_RecordCreationTime;
	AB_Date 		lt_creation_date;

	// AM DELETE !!!!!!!!!!!!!!!!!!
	lt_creation_date = pt_creation_date;
	// AM COMMENT OUT (THIS IS HARD CODED TEST CASE)>>>>>>>>>>>>> lt_creation_date.addDate(0,+6,+16);

	// get current year
	lv_yyyy = lt_creation_date.to_stringFormat( "YYYY" );

	lv_RecordCreationTime = pv_RecordCreationTime;
	lv_RecordCreationTime.removeLeadAndTrailBlanks();

	lv_RecordCreationDate = pv_RecordCreationDate;
	lv_RecordCreationDate.removeLeadAndTrailBlanks();

	if ( lv_RecordCreationDate.NotIsNull() )
	{
		if ( lv_RecordCreationDate.length() != 5 )
		{
			pv_IRT_RecordCreation_date 	= pv_RecordCreationDate;
			return;
		}
	}
	else
	{
		pv_IRT_RecordCreation_date 	= AB_null;
		return;
	}

	LOG(logDEBUG) << "AM====RCD== ORIG pt_creation_date=[" << lt_creation_date.to_stringFormat("DDMMMYY-HH:mm") << "]"  ;

	// append current year to RecordCreation date string
	lt_RecordCreationDate = lv_RecordCreationDate + lv_yyyy;
	lt_RecordCreationDate.addDate(lv_RecordCreationTime);
	if ( lt_RecordCreationDate.IsNull() )
	{
		CMsgHandler::setMsg(3980, " ", "ADJ-DATE", 1, AB_Varchar(20,AB_null));
		return;
	}

	LOG(logDEBUG) << "AM====RCD== lt_RecordCreationDate=[" << lt_RecordCreationDate.to_stringFormat("DDMMMYY-HH:mm") << "]"  ;
	LOG(logDEBUG) << "AM====RCD== pt_creation_date=[" << lt_creation_date.to_stringFormat("DDMMMYY-HH:mm") << "]"  ;
	LOG(logDEBUG) << "AM====RCD== (lt_RecordCreationDate-pt_creation_date)=[" << lt_RecordCreationDate - lt_creation_date.get_start_of_day() << "]"  ;


	if ( (lt_RecordCreationDate - lt_creation_date) > AB_Integer(24L * 60L * 60L) )
	{
		lt_RecordCreationDate.addDate(-1,0,0);
		LOG(logDEBUG) << "AM====RCD=****GO TO PREV YEAR*****= lt_RecordCreationDate=[" << lt_RecordCreationDate.to_stringFormat("DDMMMYY") << "]"  ;
	}
	else
	{
	     // check if RCD is in new year case (RCD is 01JAN and localdate is 31DEC) => add year
		lt_RecordCreationDate.addDate(+1,0,0);
		LOG(logDEBUG) << "AM====RCD=*****IN NEXT YEAR****= lt_RecordCreationDate=[" << lt_RecordCreationDate.to_stringFormat("DDMMMYY-HH:mm") << "]"  ;
		if ( (   lt_RecordCreationDate - lt_creation_date) > AB_Integer(24L * 60L * 60L) )
		{
			lt_RecordCreationDate.addDate(-1,0,0);
			LOG(logDEBUG) << "AM====RCD=********* IT IS NOT ADDING YEAR CASE, reverting lt_RecordCreationDate=[" << lt_RecordCreationDate.to_stringFormat("DDMMMYY-HH:mm") << "]"  ;
		}
		else
		{
			LOG(logDEBUG) << "AM====RCD= !!!!!!!! IT IS NEXT YEAR CASE !!!!!!!!!, lt_RecordCreationDate=[" << lt_RecordCreationDate.to_stringFormat("DDMMMYY-HH:mm") << "]"  ;
		}
	}


	if ( lt_RecordCreationDate.IsNull() )
	{
		CMsgHandler::setMsg(3980, " ", "ADJ-DATE", 2, AB_Varchar(20,AB_null));
		return;
	}

	pv_IRT_RecordCreation_date 	= lt_RecordCreationDate.to_stringFormat("DDMMMYY");
}

// WS 17 START *******
void CInterfaceRecord::getLocalDate(AB_Date &pd_gmtDateTime, AB_Date &pd_lclDateTime)
{
	CDIR lo_dbIR ;
	CIRGetLocalDate lo_CIRGetLocalDate;

   lo_CIRGetLocalDate.pc_copId = md_IRT_cop_id;
   lo_CIRGetLocalDate.pc_braId = md_IRT_bra_id;
   lo_CIRGetLocalDate.pc_iataNo = md_IRT_iata_no;
   lo_CIRGetLocalDate.pc_cityId = md_IRT_city_id;
   lo_CIRGetLocalDate.pd_gmtDateTime = pd_gmtDateTime;
   lo_CIRGetLocalDate.pd_lclDateTime = AB_null;

	 lo_dbIR.getLocalDate(lo_CIRGetLocalDate	);

	if (CMsgHandler::getMsgStatus())
	{
		pd_lclDateTime = lo_CIRGetLocalDate.pd_lclDateTime;
	}
}
// WS 17 END *******
//
// G9150, is used in CommonTransportLabels
AB_Boolean CInterfaceRecord::createTransportAssociatedRemark( string label, string remark )
{
	string labelValue;
	if ( pRemarkParser->getLabelData(remark, label, labelValue))
	{
	  return createTransportAssociatedRemark( mi_IT_I - 1, labelValue );
	} else
          return AB_false;
}

// G9150
AB_Boolean CInterfaceRecord::createTransportAssociatedRemark( int IT_ind, string& remarkStr )
{
	  D_line_number line_number = D_line_number(md_IT_line_number[IT_ind]);
	  AB_Text remark = AB_Text(remarkStr.data());
	  return createTransportAssociatedRemark( line_number, remark);
}

// US044 START
AB_Boolean
CInterfaceRecord::createTransportAssociatedRemark( D_line_number &pd_line_number, AB_Text &pd_remark )
{
	AB_Boolean lb_result = AB_false;

	for ( int li_counter = 0; li_counter < mi_IT_I; li_counter++ )
	{
		if ( pd_line_number == md_IT_line_number[li_counter] )
		{
			md_IRMK_remark[mi_IRMK_I]  = pd_remark.For(80);
         // GAP US089 START
         //md_IRMK_remark[mi_IRMK_I].removeLeadingBlanks() ;
         // GAP US089 END

			if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
			{
				md_IRMK_type[mi_IRMK_I]             = "PN" + md_IT_itin_type[li_counter];
				md_IRMK_tra_line_number[mi_IRMK_I]  = md_IT_line_number[li_counter] ;
				md_IRMK_pax_id[mi_IRMK_I]  			= AB_null;
				md_IRMK_itin_type[mi_IRMK_I]        = md_IT_itin_type[li_counter] ;
				mi_IRMK_I++;

				lb_result = AB_true;
			}
		}
	}

	return lb_result;
}

AB_Boolean CInterfaceRecord::createRentalAssociatedRemark( int IR_ind, string remarkStr )
{
	  D_line_number line_number = D_line_number(md_IR_line_number[IR_ind]);
	  AB_Text remark = AB_Text(remarkStr.data());
	  return createRentalAssociatedRemark( line_number, remark);
}

AB_Boolean
CInterfaceRecord::createRentalAssociatedRemark( D_line_number &pd_line_number, AB_Text &pd_remark )
{
	AB_Boolean lb_result = AB_false;

	for ( int li_counter = 0; li_counter < mi_IR_I; li_counter++ )
	{
		if ( pd_line_number == md_IR_line_number[li_counter] )
		{
			md_IRMK_remark[mi_IRMK_I]  = pd_remark.For(80);
         // GAP US089 START
         //md_IRMK_remark[mi_IRMK_I].removeLeadingBlanks() ;
         // GAP US089 END

			if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
			{
				md_IRMK_type[mi_IRMK_I]             = "PN" + md_IR_itin_type[li_counter];
				md_IRMK_ren_line_number[mi_IRMK_I]  = md_IR_line_number[li_counter] ;
				md_IRMK_pax_id[mi_IRMK_I]  			= AB_null;
				md_IRMK_itin_type[mi_IRMK_I]        = md_IR_itin_type[li_counter] ;
				mi_IRMK_I++;

				lb_result = AB_true;
			}
		}
	}

	return lb_result;
}

AB_Boolean CInterfaceRecord::createOtherAssociatedRemark( int IO_ind, string remarkStr )
{
	  D_line_number line_number = D_line_number(md_IO_line_number[IO_ind]);
	  AB_Text remark = AB_Text(remarkStr.data());
	  return createOtherAssociatedRemark( line_number, remark);
}

AB_Boolean
CInterfaceRecord::createOtherAssociatedRemark( D_line_number &pd_line_number, AB_Text &pd_remark )
{
	AB_Boolean lb_result = AB_false;

	for ( int li_counter = 0; li_counter < mi_IO_I; li_counter++ )
	{
		if ( pd_line_number == md_IO_line_number[li_counter] )
		{
			md_IRMK_remark[mi_IRMK_I]  = pd_remark.For(80);
         // GAP US089 START
         //md_IRMK_remark[mi_IRMK_I].removeLeadingBlanks() ;
         // GAP US089 END

			if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
			{
				md_IRMK_type[mi_IRMK_I]             = "PN" + md_IO_itin_type[li_counter];
				md_IRMK_oth_line_number[mi_IRMK_I]  = md_IO_line_number[li_counter] ;
				md_IRMK_pax_id[mi_IRMK_I]  			= AB_null;
				md_IRMK_itin_type[mi_IRMK_I]        = md_IO_itin_type[li_counter] ;
				mi_IRMK_I++;

				lb_result = AB_true;
			}
		}
	}

	return lb_result;
}

// GAP US089 START
void
CInterfaceRecord::createUnAssociatedInvoiceRemark( AB_Text &pd_remark )
{
   md_IRMK_remark[mi_IRMK_I]  = pd_remark;

   if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
   {
      md_IRMK_type[mi_IRMK_I]    = "UNI";
      md_IRMK_pax_id[mi_IRMK_I]  = AB_null;
      mi_IRMK_I++;
   }
}
// GAP US089 END

void
CInterfaceRecord::createUnAssociatedRemark( AB_Text &pd_remark )
{
	md_IRMK_remark[mi_IRMK_I]  = pd_remark;
   // GAP US089 START
   //md_IRMK_remark[mi_IRMK_I].removeLeadingBlanks();
   // GAP US089 END

   if ( md_IRMK_remark[mi_IRMK_I].NotIsNull() )
   {
      md_IRMK_type[mi_IRMK_I]    = "UNA" ;
      md_IRMK_pax_id[mi_IRMK_I]  = AB_null;
      mi_IRMK_I++;
   }
}

AB_Boolean
CInterfaceRecord::editEmailaddress(AB_Varchar& pv_email)
{
	AB_Boolean 	lb_result;
	int			li_pos;
	int			li_atPos;
	int			li_dotPos;

	lb_result = AB_false;

	LOG(logDEBUG)  << "EMAIL>> pv_email = " << pv_email  ;

	pv_email.removeLeadAndTrailBlanks();

	if ( pv_email.IsNull() )
		return lb_result;

	li_pos = pv_email.findFirstCharNotInSet("AN-_@.");

	while( li_pos != 0 && pv_email.NotIsNull() )
	{
		pv_email.substitute( pv_email.At(li_pos).For(1).to_string(), "" );

		li_pos = pv_email.findFirstCharNotInSet("AN-_@.");
	}

	pv_email.removeLeadAndTrailBlanks();

	if ( pv_email.NotIsNull() )
	{
		li_atPos 	= pv_email.findSubstring( "@", 1 );
		li_dotPos 	= pv_email.findSubstring( ".", li_atPos+1 );

		if ( 	li_atPos > 0 && li_dotPos> 0)
			lb_result = AB_true;
	}

	return lb_result;
}

AB_Text
CInterfaceRecord::CheckTime(const AB_Text &pv_time) const {
	AB_Text lv_time = pv_time;
	lv_time.removeLeadAndTrailBlanks();

   if (lv_time.Length().to_long() == 4 && !lv_time.At(4).isAllNumeric()) {
      return "0" + lv_time;
	}

   if (lv_time.Length().to_long() == 3) {
      return lv_time.For(2) + "00" + lv_time.At(3) ;
	}

	if (lv_time.Length().to_long() == 2) {
      return "0" + lv_time.For(1) + "00" + lv_time.At(2) ;
	}

   return lv_time ;
}

// GAP US107 START
void CInterfaceRecord::editAmount(	AB_Text lt_amount_string,
												D_amount pd_min_amount,
												D_amount pd_max_amount,
												AB_Boolean pb_percent_allowed,
												AB_Boolean pb_percent_restricted)
{
   AB_Char        lc_amount(13);
	D_amount       ld_value;

	if ( lt_amount_string.At(1).For(1) == "P" && pb_percent_allowed)
	{
		lc_amount = lt_amount_string.At(2);
		if ( pb_percent_restricted )
		{
			pd_min_amount = 0.00;
			pd_max_amount = 100.00;
		}
	}
	else
	{
		lc_amount = lt_amount_string;
	}

	ld_value = ld_value.input_conversion_trigger(lc_amount);

	if (! CMsgHandler::getMsgStatus())
	{
		return;
	}

	if ( ! (	ld_value <= pd_max_amount  &&
		  		ld_value >= pd_min_amount ))
	{
		CMsgHandler::setMsg(3958, " ", "AMT-PARSE", 1, AB_Varchar(20,AB_null));
		return;
	}
// GAP US107 END

}

void CInterfaceRecord::editPhoneNumber( AB_Text& pt_phone_number, bool oldRules)
{
   int li_pos = 0;

   if ( pt_phone_number.IsNull() )
      return;

	//LOG(logDEBUG) << "AM==== CInterfaceRecord::editPhoneNumber starts... "  ;
   pt_phone_number.removeLeadAndTrailBlanks();

	// CWT_US doesn't want any cleaning of phone numbers (SCR # 485292)
	// AM(23Feb2012): this is no longer the true, CWT gets used to phone cleanup,
	// we'll remove this check for good
	//if ( isCWT_USstream() == AB_true )
	//{
	  //LOG(logDEBUG) << "AM==== NO PHONE EDITING FOR CWT_US [" << pt_phone_number << "]"  ;
	//  return;
	//}

   if ( pt_phone_number.IsNull() )
      return;

   // remove trailing fax number
   li_pos = pt_phone_number.findSubstring("FAX");
   if ( li_pos > 1 ) //only if it is a TRAILING fax number, may be 1 is too small...
   {
      pt_phone_number = pt_phone_number.For(li_pos - 1) ;
   }

   if (!oldRules)
   {
      // 516462 remove extension number
      li_pos = pt_phone_number.findSubstring("X");
      if ( li_pos > 1 )
      {
         pt_phone_number = pt_phone_number.For(li_pos - 1) ;
      }
   }

   // remove everything after / (including /)
   li_pos = pt_phone_number.findSubstring("/");
   if ( li_pos > 0 )
   {
      pt_phone_number = pt_phone_number.For(li_pos - 1) ;
   }

   while ( ! pt_phone_number.isAllNumeric() && pt_phone_number.length() != 0 )
   {
      li_pos = pt_phone_number.findFirstCharNotInSet("N");
      if ( li_pos > 0 )
         pt_phone_number.substitute(pt_phone_number.At(li_pos).For(1).to_string(), "" );
      else
         break;
   }

	// if it doesn't look like phone => clear this field
	if ( pt_phone_number.NotIsNull() && pt_phone_number.length() < 6 )
	{
	  pt_phone_number = AB_null;
	}
	//LOG(logDEBUG) << "AM==== CInterfaceRecord::editPhoneNumber end [" << pt_phone_number << "]" ;
}
void CInterfaceRecord::editPhoneNumber( AB_Text& pt_phone_number, AB_Varchar& areaCode, AB_Varchar& phoneNum, bool oldRules)
{
   editPhoneNumber(pt_phone_number, oldRules);


	if ( pt_phone_number.NotIsNull() && pt_phone_number != "")
   {
        // 516462
	areaCode = AB_null ;
	phoneNum = AB_null ;
        if (!oldRules)
        {
		if ( pt_phone_number.length() > 9 )
		{
			phoneNum = pt_phone_number.At(pt_phone_number.length()-7+1) ;
			areaCode = pt_phone_number.At(pt_phone_number.length()-10+1).For(3) ;
		}
		else // <10 AND >5
		{
                        if (pt_phone_number.length() > 7)
                        {
			    phoneNum = pt_phone_number.At(pt_phone_number.length()-7+1);
			    areaCode = pt_phone_number.At(1).For(pt_phone_number.length()-7) ;
                        }
                        else
                        {
			    phoneNum = pt_phone_number;
			    areaCode = AB_null ;
                            // nothing in area code
                        }
		}
        }
        else
        {

		if ( pt_phone_number.length() > 10 )
		{
			areaCode = pt_phone_number.At(1).For(5) ;
			phoneNum = pt_phone_number.At(6) ;
		}
		else if ( pt_phone_number.length() > 7 )
		{
			areaCode = pt_phone_number.At(1).For(3) ;
			phoneNum = pt_phone_number.At(4) ;
		}
		else
		{
			areaCode = AB_null ;
			phoneNum = pt_phone_number ;
		}
        }
    }
}
void CInterfaceRecord::adjustTransportItinType()
{
   CDIR                   lo_CDIR ;
   CIRGetSupServiceType   lo_CIRGetSupServiceType ;

	for (int li_IT_index = 0; li_IT_index < mi_IT_I && CMsgHandler::getMsgStatus(); li_IT_index++)
	{
		if ( md_IT_itin_type[li_IT_index] == "A" )
		{
			lo_CIRGetSupServiceType.pc_sup_id = md_IT_sup_id[li_IT_index];

			lo_CDIR.getSupServiceType(lo_CIRGetSupServiceType);

			if (!CMsgHandler::getMsgStatus()) {
				return;
			}

			if ( lo_CIRGetSupServiceType.pc_service_type == "R" )
			{
				for (int li_ICD_index = 0; li_ICD_index < mi_ICD_I && CMsgHandler::getMsgStatus(); li_ICD_index++ )
				{
					if ( 	md_ICD_itin_type[li_ICD_index].NotIsNull() 								&&
							md_ICD_itin_type[li_ICD_index] == md_IT_itin_type[li_IT_index] 	&&
							md_ICD_itin_line_number[li_ICD_index].NotIsNull()						&&
							md_ICD_itin_line_number[li_ICD_index] == md_IT_line_number[li_IT_index] )
					{
						md_ICD_itin_type[li_ICD_index] = lo_CIRGetSupServiceType.pc_service_type;
					}
				}

				for (int li_IRMK_index = 0; li_IRMK_index < mi_IRMK_I && CMsgHandler::getMsgStatus(); li_IRMK_index++ )
				{
					if ( 	md_IRMK_itin_type[li_IRMK_index].NotIsNull() 							&&
							md_IRMK_itin_type[li_IRMK_index] == md_IT_itin_type[li_IT_index] 	&&
							md_IRMK_tra_line_number[li_IRMK_index].NotIsNull()						&&
							md_IRMK_tra_line_number[li_IRMK_index] == md_IT_line_number[li_IT_index] )
					{
						if ( 	md_IRMK_type[li_IRMK_index].NotIsNull()	&&
								md_IRMK_type[li_IRMK_index] == "PN" + md_IT_itin_type[li_IT_index] )
						{
							md_IRMK_type[li_IRMK_index] = "PN" + lo_CIRGetSupServiceType.pc_service_type;
						}

						md_IRMK_itin_type[li_IRMK_index] = lo_CIRGetSupServiceType.pc_service_type;
					}
				}
				md_IT_itin_type[li_IT_index] = lo_CIRGetSupServiceType.pc_service_type;
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CInterfaceRecord::editReceipt()
{
   CDIR                   	dbIR ;
   CIRValidateSetFixed  	lo_validateSetFixed ;

	CCreditCard             lo_creditCard;
	CPaymentForm   			lo_payForm;

	AB_Boolean 		lb_validate;
	CCFAId			lo_cfa;
	AB_Varchar		lv_fullCC(80);
	AB_Char			lc_authInd;
	AB_Integer		li_error;

LOG(logDEBUG) << "ENTER editReceipt"  ;

/*
	if ( !editReceiptDomains() )
		return AB_false;
*/

	lb_validate = AB_true;
	lv_fullCC = md_RC_cc_id[mi_RC_I] + md_RC_cc_no[mi_RC_I];

	lo_payForm.setData ( lo_cfa,  // cfa
				md_RC_form_of_pay[mi_RC_I],
				lv_fullCC,
				md_RC_cc_exp_date[mi_RC_I],
				md_RC_auth_no[mi_RC_I],
				lc_authInd,
				"R",
				lb_validate );

	if ( ! CMsgHandler::getMsgStatus() )
	{
		li_error = CMsgHandler::getMsgId();
		createInfoQueue(mv_rec_queType, li_error);
		CMsgHandler::clearMsg();

		LOG(logERROR) << "\nERROR1= " << mv_rec_queType  ;
		LOG(logERROR) << "\nERROR1:ID= " << li_error  ;

		return AB_false;
	}

	/* need to determine company and branch */
	if ( md_RC_cop_id.IsNull() || md_RC_bra_id.IsNull() )
	{
		lo_validateSetFixed.pc_session_id = md_session_id;
		lo_validateSetFixed.pc_iata = md_IRT_iata_no;
		lo_validateSetFixed.pc_consultant = md_IRT_con_id;
		lo_validateSetFixed.pc_mml_ind = NULL;;
		lo_validateSetFixed.pc_cfa_type = NULL;;
		lo_validateSetFixed.gmt_date_time = NULL;;

		dbIR.validateSetFixed(lo_validateSetFixed);

		if ( ! CMsgHandler::getMsgStatus() )
		{
			/* create receipt - but do not validate accounts */
			md_RC_cop_id = AB_null;
			md_RC_bra_id = AB_null;

			CMsgHandler::clearMsg();
		}

		else
		{
			/* assign company and branch */
			md_RC_cop_id = lo_validateSetFixed.pc_company;
			md_RC_bra_id = lo_validateSetFixed.pc_branch;
		}
	}

	md_RC_name[mi_RC_I].removeTrailingBlanks();

	/* G9136: Derive Optional Values before the Edit */
	if ( md_RC_bank_acc[mi_RC_I].IsNull() ||
		  md_RC_gl_acc1[mi_RC_I].IsNull() )
	{

		CReceiptGetDefault inputData;

		inputData.copId = md_RC_cop_id;
		inputData.braId = md_RC_bra_id;
		inputData.fop = md_RC_form_of_pay[mi_RC_I];
		inputData.ccId = md_RC_cc_id[mi_RC_I];

		CDReceipt dbReceipt;

		dbReceipt.getDefault(inputData);

		if ( md_RC_bank_acc[mi_RC_I].IsNull() )
		{
			md_RC_bank_acc[mi_RC_I] = inputData.bankId;
		}

		if ( md_RC_gl_acc1[mi_RC_I].IsNull() )
		{
			md_RC_gl_acc1[mi_RC_I] = inputData.glAccount;
			md_RC_gl_amt1[mi_RC_I] = md_RC_total_amt[mi_RC_I];
		}

//		LOG(logDEBUG) << "inputData.bankId:= " << inputData.bankId  ;
//		LOG(logDEBUG) << "inputData.glAccount:= " << inputData.glAccount  ;
//		LOG(logDEBUG) << "RecPaxName:= " << md_RC_name[mi_RC_I]  ;
//		LOG(logDEBUG) << "md_RC_gl_amt1:= " << md_RC_gl_amt1[mi_RC_I]  ;
//		LOG(logDEBUG) << "md_RC_gl_acc1:= " << md_RC_gl_acc1[mi_RC_I]  ;

	}

	if ( !editReceiptDomains() )
		return AB_false;

	// use receipt object for edits only
	CReceipt		lo_receipt(md_RC_cop_id,
						md_RC_bra_id,
						"RIRI",
						md_RC_bank_acc[mi_RC_I],
						md_RC_name[mi_RC_I],
						AB_null,			// invoice no
						md_RC_gl_amt1[mi_RC_I],
						md_RC_gl_amt2[mi_RC_I],
						md_RC_gl_amt3[mi_RC_I],
						md_RC_total_amt[mi_RC_I],
						md_RC_gl_acc1[mi_RC_I],
						md_RC_gl_acc2[mi_RC_I],
						md_RC_gl_acc3[mi_RC_I],
						AB_null );	// sup id

	lo_receipt.editIRReceipt();

	if ( ! CMsgHandler::getMsgStatus() )
	{
		li_error = CMsgHandler::getMsgId();
		createInfoQueue(mv_rec_queType, li_error);
		CMsgHandler::clearMsg();

		LOG(logERROR) << "\nERROR2= " << mv_rec_queType  ;
		LOG(logERROR) << "\nERROR2:ID= " << li_error  ;

		return AB_false;
	}

	if ( md_RC_form_of_pay[mi_RC_I] == "CC" )
	{
		lo_creditCard = lo_payForm.getCreditCard(lb_validate);

		if ( ! CMsgHandler::getMsgStatus() )
		{
			if ( CMsgHandler::getMsgId().to_long() == 408 &&
				md_RC_cc_id[mi_RC_I] == "SE" && md_RC_bank_acc[mi_RC_I] == "137000" )
			{
				CMsgHandler::clearMsg();
			}
			else
			{
				li_error = CMsgHandler::getMsgId();
				createInfoQueue(mv_rec_queType, li_error);
				CMsgHandler::clearMsg();

		LOG(logERROR) << "\nERROR2= " << mv_rec_queType  ;
		LOG(logERROR) << "\nERROR2:ID= " << li_error  ;

				return AB_false;
			}
		}
	}

LOG(logDEBUG) << "EXIT editReceipt"  ;
	return AB_true;
}
//---------------------------------------------------------------------------------------------------------------------------
AB_Boolean CInterfaceRecord::editReceiptDomains()
{
	D_amount		ld_glAmtTot;
	D_amount		ld_amtTot;
	D_amount		ld_value;
	AB_Varchar	lv_varStr(30);
	AB_Integer	li_error;

	lv_varStr = md_RC_bank_acc[mi_RC_I];

	if ( md_RC_bank_acc[mi_RC_I].IsNull() ||
		  lv_varStr.length() != 6 ||
		  !md_RC_bank_acc[mi_RC_I].isAllNumeric() )
	{
		/* bank account is not a valid bank account */
		li_error = 347;
		createInfoQueue(mv_rec_queType, li_error);

		LOG(logERROR) << "ERROR5:createInfoQueue:mv_rec_queType= " << mv_rec_queType  ;
		LOG(logERROR) << "ERROR5:createInfoQueue:li_error= " << li_error  ;

		return AB_false;
	}

	lv_varStr = md_RC_gl_acc1[mi_RC_I];

	if ( md_RC_gl_acc1[mi_RC_I].IsNull() ||
		  lv_varStr.length() != 6 ||
		  !md_RC_gl_acc1[mi_RC_I].isAllNumeric() )
	{
		/* gl account must be six numerics */
		li_error = 3106;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}

	if ( md_RC_gl_amt1[mi_RC_I].IsNull() ||
		!editReceiptAmount(md_RC_gl_amt1[mi_RC_I], "#######.##", ld_value) )
	{
		/* receipt amount must equal sum of amount lines */
		li_error = 1285;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}
	else ld_glAmtTot = ld_value;

	lv_varStr = md_RC_gl_acc2[mi_RC_I];

	if ( md_RC_gl_acc2[mi_RC_I].NotIsNull() &&
		   ( lv_varStr.length() != 6 ||
		  	!md_RC_gl_acc2[mi_RC_I].isAllNumeric() ))
	{
		/* gl account must be six numerics */
		li_error = 3106;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}

	lv_varStr = md_RC_gl_acc3[mi_RC_I];

	if ( md_RC_gl_acc3[mi_RC_I].NotIsNull() &&
		   ( lv_varStr.length() != 6 ||
		  	!md_RC_gl_acc3[mi_RC_I].isAllNumeric() ))
	{
		/* gl account must be six numerics */
		li_error = 3106;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}

	if (( md_RC_gl_amt2[mi_RC_I].IsNull() && md_RC_gl_acc2[mi_RC_I].NotIsNull() ) ||
		( md_RC_gl_amt2[mi_RC_I].NotIsNull() && md_RC_gl_acc2[mi_RC_I].IsNull() ) ||
		( md_RC_gl_amt3[mi_RC_I].IsNull() && md_RC_gl_acc3[mi_RC_I].NotIsNull() ) ||
		( md_RC_gl_amt3[mi_RC_I].NotIsNull() && md_RC_gl_acc3[mi_RC_I].IsNull() ) )
	{
		/* receipt amount must equal sum of amount lines */
		li_error = 1285;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}

	if ( md_RC_gl_amt2[mi_RC_I].NotIsNull() )
	{
		if ( !editReceiptAmount(md_RC_gl_amt2[mi_RC_I], "#######.##", ld_value) )
		{
			/* receipt amount must equal sum of amount lines */
			li_error = 1285;
			createInfoQueue(mv_rec_queType, li_error);
			return AB_false;
		}
		else ld_glAmtTot = ld_glAmtTot + ld_value;
	}

	if ( md_RC_gl_amt3[mi_RC_I].NotIsNull() )
	{
		if ( !editReceiptAmount(md_RC_gl_amt3[mi_RC_I], "#######.##", ld_value) )
		{
			/* receipt amount must equal sum of amount lines */
			li_error = 1285;
			createInfoQueue(mv_rec_queType, li_error);
			return AB_false;
		}
		else ld_glAmtTot = ld_glAmtTot + ld_value;
	}

	if ( md_RC_total_amt[mi_RC_I].IsNull() ||
		!editAmountDomain(md_RC_total_amt[mi_RC_I], "#######.##", ld_value) )
	{
		/* gl account must be six numerics */
		li_error = 3106;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}

	if ( ld_glAmtTot != ld_value || ld_value == AB_Decimal(0.00) )
	{
		/* receipt amount must equal sum of amount lines */
		li_error = 1285;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}
	if ( (md_RC_form_of_pay[mi_RC_I] == "CC" || md_RC_form_of_pay[mi_RC_I] == "AP") &&
		  (md_RC_cc_exp_date[mi_RC_I].IsNull() ||
		   md_RC_cc_exp_date[mi_RC_I].Length() != AB_Integer(4) ||
		  !md_RC_cc_exp_date[mi_RC_I].isAllNumeric())) {
		li_error = 54;
		createInfoQueue(mv_rec_queType, li_error);
		return AB_false;
	}

	return AB_true;
}

void CInterfaceRecord::editReceiptCert( AB_Text &pv_cert, AB_Text &pc_ccId,
	AB_Text &pc_bnkId, AB_Boolean &pb_valid )
{
	pb_valid = AB_false;
   pv_cert.removeLeadAndTrailBlanks();

   if (pv_cert.NotIsNull())
   {
      if ( pv_cert.length() != 5 || !(pv_cert.isAllNumeric()) )
      {
         CMsgHandler::setMsg(3523, "", "ed-cert", 3, AB_null);
         return;
      }
      if (pc_bnkId != "139000" && pc_bnkId != "141000" && pc_bnkId != "111000"
               && pc_ccId == "SE")
      {
         CMsgHandler::setMsg(3521, "", "ed-cert", 1, pc_bnkId);
         return;
      }

      if (pc_bnkId != "164000" && pc_bnkId != "166000" && pc_ccId == "EA")
      {
         CMsgHandler::setMsg(3521, "", "ed-cert", 2, pc_bnkId);
         return;
      }
   }

	pb_valid = AB_true;
}

void CInterfaceRecord::initIrreceipt(CIRCreateIrreceipt &po_createIrreceipt)
{
   po_createIrreceipt.session_id = AB_null;
   po_createIrreceipt.line_number = AB_null;
   po_createIrreceipt.cop_id = AB_null;
   po_createIrreceipt.bra_id = AB_null;
   po_createIrreceipt.name = AB_null;
   po_createIrreceipt.total_amt = AB_null;
   po_createIrreceipt.bank_id = AB_null;
   po_createIrreceipt.form_of_pay = AB_null;
   po_createIrreceipt.cc_id = AB_null;
   po_createIrreceipt.cc_no = AB_null;
   po_createIrreceipt.cc_exp_date = AB_null;
   po_createIrreceipt.cc_auth_no = AB_null;
   po_createIrreceipt.acc_id1 = AB_null;
   po_createIrreceipt.amt1 = AB_null;
   po_createIrreceipt.acc_id2 = AB_null;
   po_createIrreceipt.amt2 = AB_null;
   po_createIrreceipt.acc_id3 = AB_null;
   po_createIrreceipt.amt3 = AB_null;
   po_createIrreceipt.rec_valid = AB_null;
   po_createIrreceipt.lang_id = AB_null;
   po_createIrreceipt.link_to = AB_null;
   po_createIrreceipt.payment_reference = AB_null;
}

void CInterfaceRecord::initIrdistribution(CIRCreateIrdistribution &po_createIrdistribution)
{
   po_createIrdistribution.session_id = AB_null;
   po_createIrdistribution.line_number_1 = AB_null;
   po_createIrdistribution.acc_id_1 = AB_null;
   po_createIrdistribution.amt_1 = AB_null;
   po_createIrdistribution.line_number_2 = AB_null;
   po_createIrdistribution.acc_id_2 = AB_null;
   po_createIrdistribution.amt_2 = AB_null;
   po_createIrdistribution.line_number_3 = AB_null;
   po_createIrdistribution.acc_id_3 = AB_null;
   po_createIrdistribution.amt_3 = AB_null;
   po_createIrdistribution.line_number_4 = AB_null;
   po_createIrdistribution.acc_id_4 = AB_null;
   po_createIrdistribution.amt_4 = AB_null;
   po_createIrdistribution.line_number_5 = AB_null;
   po_createIrdistribution.acc_id_5 = AB_null;
   po_createIrdistribution.amt_5 = AB_null;
   po_createIrdistribution.line_number_6 = AB_null;
   po_createIrdistribution.acc_id_6 = AB_null;
   po_createIrdistribution.amt_6 = AB_null;
}

// GAP US111 START
void CInterfaceRecord::createUDID(	D_line_number pd_UDID_number,
												AB_Text pt_remark,
												AB_Boolean pb_matrix_UDID)
{
/*
		New UDID	|	Old UDID		|	TODO
		------------------------------------
		Matrix	|	Matrix		|	Replace
		Matrix	|	Other			|	Replace
		Matrix	|	Not found	|	Create
		Other		|	Matrix		|	Nothing
		Other		|	Other			|	Nothing
		Other		|	Not found	|	Create
		------------------------------------
*/
	for (int li_counter=0; li_counter < mi_IRMK_I; li_counter++)
   {
		if (	md_IRMK_type[li_counter] 				== "UDI" &&
				md_IRMK_udi_line_number[li_counter] == pd_UDID_number ) {

			if (pb_matrix_UDID == AB_true)
			{
				md_IRMK_remark[li_counter] = pt_remark;
				return;
			}
			else
			{
				return;
			}
		}
	}

	createUDIDifRange1to99(pd_UDID_number, pt_remark);
	//md_IRMK_type[mi_IRMK_I]   				= "UDI";
	//md_IRMK_udi_line_number[mi_IRMK_I] 	= pd_UDID_number;
	//md_IRMK_remark[mi_IRMK_I]         	= pt_remark;
	//md_IRMK_pax_id[mi_IRMK_I++]       	= AB_null;
}
// GAP US111 END
bool CInterfaceRecord::createUDIDifRange1to99(	D_line_number pd_UDID_number,
						AB_Text pt_remark)
{
    bool retValue = false;

    if (pd_UDID_number > AB_Integer(0) && pd_UDID_number < AB_Integer(100))
    {
	md_IRMK_type[mi_IRMK_I]   				= "UDI";
	md_IRMK_udi_line_number[mi_IRMK_I] 	= pd_UDID_number;
	md_IRMK_remark[mi_IRMK_I]         	= pt_remark;
	md_IRMK_pax_id[mi_IRMK_I++]       	= AB_null;

        retValue = true;
    }

    return retValue;
}


void CInterfaceRecord::processReceipt()
{
	int				li_pos;
	int				li_reverseIndex;
	int				li_type_ct;
	AB_Boolean		lb_notFinished;
	V_AB_Varchar	lva_recFields(80,50);
	V_AB_Varchar	lva_type(3,50);
	AB_Varchar		lv_data(500);
	AB_Varchar		lv_tmpData(500);


	LOG(logDEBUG) << "ENTER processReceipt"  ;

	/* process each receipt remark */
	for ( int li_ctr = 1; li_ctr <= mi_receiptIndex; li_ctr++ )
	{
		/* separate into fields N-, GL-, C- etc. */
		li_type_ct = 0;
		lb_notFinished = AB_true;
		for ( int li_ctr2 = 1 ;li_ctr2 < 21; li_ctr2++ )
		{
			lva_type[li_ctr2] = AB_null;
			lva_recFields[li_ctr2] = AB_null;
		}

		lv_data = gd_receiptData[li_ctr];

		LOG(logDEBUG) << "ENTER processReceipt: lv_data= " << lv_data  ;

/*
   	V_AB_Varchar lva_tokens(40, 10);
		int li_noOfTokens = lva_tokens.parseString( lv_data, "/" );

		LOG(logDEBUG) << "li_noOfTokens:= "  ;
		LOG(logDEBUG) << li_noOfTokens  ;

		for ( int tokenCtr = 1; tokenCtr <= li_noOfTokens; tokenCtr++)
		{
			LOG(logDEBUG) << "tokenCtr:= " << tokenCtr  ;
			LOG(logDEBUG) << "token:= " << lva_tokens[tokenCtr]  ;

			li_pos = lva_tokens[tokenCtr].findSubstring("-");

			li_type_ct = li_type_ct + 1;

			lva_type[li_type_ct] = lva_tokens[tokenCtr].For(li_pos);
			lva_type[li_type_ct].removeLeadAndTrailBlanks();
			lva_recFields[li_type_ct] = lva_tokens[tokenCtr].At(li_pos + 1);
			lva_recFields[li_type_ct].removeLeadAndTrailBlanks();

			LOG(logDEBUG) << "lva_type:= " << lva_type[li_type_ct]  ;
			LOG(logDEBUG) << "lva_recFields:= " << lva_recFields[li_type_ct]  ;

		}
*/

		while ( lb_notFinished )
		{
			li_pos = lv_data.findSubstring("-");

			LOG(logDEBUG) << "1ENTER lv_data:= " << lv_data  ;
			LOG(logDEBUG) << "1ENTER li_pos:= " << li_pos  ;
			LOG(logDEBUG) << "2ENTER li_type_ct= " << li_type_ct  ;

			if ( li_pos > 1 )
			{

				li_type_ct = li_type_ct + 1;

				//first data element type
				lva_type[li_type_ct] = lv_data.For(li_pos);
				lva_type[li_type_ct].removeLeadAndTrailBlanks();

				LOG(logDEBUG) << "TENTER lva_type:= " << lva_type[li_type_ct]  ;

				lv_data = lv_data.At(li_pos + 1);
				lv_data.removeLeadAndTrailBlanks();

				//Get the value of the data element and set it up for the next data element
				li_pos = lv_data.findSubstring("-");

				if ( li_pos > 0 )
				{
					lv_tmpData = lv_data.For(li_pos);
				}
				else
				{
					lv_tmpData = lv_data;
				}

            //li_reverseIndex = lv_tmpData.getReverseIndex("/");
				li_reverseIndex = 0;

				if ( li_reverseIndex < 1 )
				{
					li_reverseIndex = lv_tmpData.getReverseIndex(" ");
				}

				LOG(logDEBUG) << "2ENTER li_pos:= " << li_pos  ;
				LOG(logDEBUG) << "2ENTER li_reverseIndex:= " << li_reverseIndex  ;

				//A-888/FP-CA

				if ( li_pos > 0 )
				{
					if ( li_reverseIndex > 0 )
					{
						lva_recFields[li_type_ct] = lv_data.For(li_reverseIndex - 1);
						lva_recFields[li_type_ct].removeLeadAndTrailBlanks();

						lv_data = lv_data.At(li_reverseIndex + 1);
					}
					else
					{
						//This should never happen; wrong format so force it to get out of the loop
						lva_recFields[li_type_ct] = lv_data;
						lb_notFinished = AB_false;
					}

				}
				else
				{
					lva_recFields[li_type_ct] = lv_data;
				}

				LOG(logDEBUG) << "TENTER lva_recFields:= " << lva_recFields[li_type_ct]  ;

/*
				LOG(logDEBUG) << "2ENTER li_type_ct= " << li_type_ct  ;
				LOG(logDEBUG) << "2ENTER lva_type[++li_type_ct]:= " << lva_type[li_type_ct]  ;

				if ( li_pos > 3 )
				while (li_pos > 3 &&
						 lv_data.At(li_pos - 2).For(1) != " " &&
						 lv_data.At(li_pos - 3).For(1) != " ") {
					li_pos = lv_data.findSubstring("-", li_pos+1);
				}

				if ( li_pos > 3 )
				{
					int li_lth;

					if ( lv_data.At(li_pos - 2).For(1) == " " )
						li_lth = li_pos-2;
					else if ( lv_data.At(li_pos - 3).For(1) == " " )
						li_lth = li_pos-3;

					LOG(logDEBUG) << "3ENTER li_lth:= " << li_lth  ;

					lva_recFields[li_type_ct] = lv_data.For(li_lth);

					LOG(logDEBUG) << "3ENTER lva_recFields:= " << lva_recFields[li_type_ct]  ;

					lv_data = lv_data.At(li_lth+1);

					LOG(logDEBUG) << "4ENTER li_pos:= " << li_lth + 1  ;
					LOG(logDEBUG) << "4ENTER lv_data:= " << lv_data  ;

				}
				else lb_notFinished = AB_false;

*/
			}

			else lb_notFinished = AB_false;

		}

		/* assign fields to IR receipt */
		int li_fldCtr = 1;

		for ( mi_RC_I++; li_fldCtr <= li_type_ct; li_fldCtr++ )
		{
			if ( lva_type[li_fldCtr] == "N-" && md_RC_name[mi_RC_I].IsNull() )
				md_RC_name[mi_RC_I] = lva_recFields[li_fldCtr];

			else if (lva_type[li_fldCtr]== "A-" && md_RC_total_amt[mi_RC_I].IsNull() )
				md_RC_total_amt[mi_RC_I] = lva_recFields[li_fldCtr];

			else if (lva_type[li_fldCtr]== "B-" && md_RC_bank_acc[mi_RC_I].IsNull() )
				md_RC_bank_acc[mi_RC_I] = lva_recFields[li_fldCtr];

			else if (lva_type[li_fldCtr]== "LK-" && md_RC_link_to[mi_RC_I].IsNull() )
				md_RC_link_to[mi_RC_I] = lva_recFields[li_fldCtr];

			else if (lva_type[li_fldCtr]== "GL-" )
			{
				li_pos = lva_recFields[li_fldCtr].findSubstring("/");

				if ( li_pos > 6 )
				{
					if ( md_RC_gl_acc1[mi_RC_I].IsNull() )
					{
						md_RC_gl_acc1[mi_RC_I] = lva_recFields[li_fldCtr].For(6);
						md_RC_gl_amt1[mi_RC_I] = lva_recFields[li_fldCtr].At(li_pos+1);
					}
					else if ( md_RC_gl_acc2[mi_RC_I].IsNull() )
					{
						md_RC_gl_acc2[mi_RC_I] = lva_recFields[li_fldCtr].For(6);
						md_RC_gl_amt2[mi_RC_I] = lva_recFields[li_fldCtr].At(li_pos+1);
					}
					else if ( md_RC_gl_acc3[mi_RC_I].IsNull() )
					{
						md_RC_gl_acc3[mi_RC_I] = lva_recFields[li_fldCtr].For(6);
						md_RC_gl_amt3[mi_RC_I] = lva_recFields[li_fldCtr].At(li_pos+1);
					}
				}	/* li_pos > 6 */
			}	/* GL- */

			else if (lva_type[li_fldCtr] == "FP-" &&
						md_RC_form_of_pay[mi_RC_I].IsNull() )
			{
				V_AB_Varchar	lva_fp(30,5);
				AB_Varchar	fop_in_pair(30);

				LOG(logDEBUG) << " AM==========whole string =[" << lva_recFields[li_fldCtr] << "]"  ;
                //M913R1/A-1000.00 FP-CC/VI4444333322221111/1212/111111 LK-T
				lva_fp.parseString(lva_recFields[li_fldCtr], "/");

				fop_in_pair = lva_fp[1];
				md_RC_form_of_pay[mi_RC_I] = lva_fp[1];

				//if ( md_RC_form_of_pay[mi_RC_I] == "CC" )
				if ( fop_in_pair == "CC" )
				{
					md_RC_form_of_pay[mi_RC_I]  = "CC";
					md_RC_cc_id[mi_RC_I] 		= lva_fp[2].For(2);
					md_RC_cc_no[mi_RC_I] 		= lva_fp[2].At(3);
					md_RC_cc_exp_date[mi_RC_I]  = lva_fp[3];
					md_RC_auth_no[mi_RC_I] 		= lva_fp[4];
				}
				else if ( fop_in_pair == "CCRFP" )
				{
					md_RC_form_of_pay[mi_RC_I]  = "CC";
					md_RC_cc_id[mi_RC_I] 		= md_CCRFP_cc_id;
					md_RC_cc_no[mi_RC_I] 		= md_CCRFP_cc_no;
					md_RC_cc_exp_date[mi_RC_I]  = md_CCRFP_cc_exp_date;
					md_RC_auth_no[mi_RC_I] 		= lva_fp[2];
				}
				LOG(logDEBUG) << " AM======md_RC_form_of_pay =[" << md_RC_form_of_pay[mi_RC_I] << "]"  ;
				LOG(logDEBUG) << " AM======md_RC_cc_id       =[" << md_RC_cc_id[mi_RC_I] << "]"  ;
				LOG(logDEBUG) << " AM======md_RC_cc_no       =[" << md_RC_cc_no[mi_RC_I] << "]"  ;
				LOG(logDEBUG) << " AM======md_RC_cc_exp_date =[" << md_RC_cc_exp_date[mi_RC_I] << "]"  ;
				LOG(logDEBUG) << " AM======md_RC_auth_no     =[" << md_RC_auth_no[mi_RC_I] << "]"  ;

			}	/* FP- */

		}	/* for loop for fields */

		// add RemarkParser here, we have link between mi_RC_I->R(\d)
		string remarkName = gd_receiptType[li_ctr].to_string();
		remarkName += "/";
		RP_saveDataTo(	remarkName, "GC", md_RC_payment_reference[mi_RC_I]);
		RP_saveDataTo(	remarkName, "PR", md_RC_payment_reference[mi_RC_I]);

		/* assign company and branch from trip header - may be modified by */
		/* editReceipt.                                                    */
		if ( md_RC_cop_id.IsNull() )
		{
			md_RC_cop_id = md_IRT_cop_id;
			md_RC_bra_id = md_IRT_bra_id;
		}


		/*
			Because of C3010, instead of certificates, we are using coupons.
			Parsing rules remain the same.
		*/
		bool lb_receipt_remark_found = false;
		getReceiptQueueType();
		AB_Boolean lb_receiptValid = editReceipt();

		if (lb_receiptValid)
		{
			/* add remarks and coupons to valid receipts, note only 2 coupons per receipt are allowed */
			int li_coupon_no = 0;
			V_AB_Varchar	lva_coupon_id(80,2);
			for ( li_fldCtr = 1; li_fldCtr <= li_type_ct; li_fldCtr++ )
			{
				if ( lva_type[li_fldCtr] == "C-" || lva_type[li_fldCtr] == "RM-" )
				{
					if ( lva_type[li_fldCtr] == "C-" )
					{
						li_coupon_no++;
						if (li_coupon_no < 3 && lva_recFields[li_fldCtr].NotIsNull()) {
							lva_coupon_id[li_coupon_no] = lva_recFields[li_fldCtr];
						}
					}

					if ( lva_type[li_fldCtr] == "RM-" ||
						 (lva_type[li_fldCtr] == "C-" && li_coupon_no < 3))
					{
						if (lva_type[li_fldCtr] == "RM-") {
							lb_receipt_remark_found = true;
						}

						/* store as type R + rec no or C nd rec no IE. R1 or C1 */
						md_IRMK_type[mi_IRMK_I] = lva_type[li_fldCtr].For(1) + AB_Text(mi_RC_I);

						md_IRMK_remark[mi_IRMK_I++] = lva_recFields[li_fldCtr];
					}
				}
			}	/* for loop - coupons and rmks */

			if (li_coupon_no > 0) {
				D_char_numeric ld_amt = 0;
				if (md_RC_gl_amt1[mi_RC_I].NotIsNull()) {
					ld_amt = ld_amt + md_RC_gl_amt1[mi_RC_I];
				}

				if (md_RC_gl_amt2[mi_RC_I].NotIsNull()) {
					ld_amt = ld_amt + md_RC_gl_amt2[mi_RC_I];
				}

				if (md_RC_gl_amt3[mi_RC_I].NotIsNull()) {
					ld_amt = ld_amt + md_RC_gl_amt3[mi_RC_I];
				}
			}

			if ( ! lb_receipt_remark_found ) {

				//default the Receipt remark value

				md_IRMK_type[mi_IRMK_I] = "R" + AB_Text(mi_RC_I);
				md_IRMK_remark[mi_IRMK_I] = "PAYMENT RECEIVED";

				LOG(logDEBUG) << "REMARK md_IRMK_type[mi_IRMK_I]:= " << md_IRMK_type[mi_IRMK_I]  ;
				LOG(logDEBUG) << "REMARK md_IRMK_remark[mi_IRMK_I]:= " << md_IRMK_remark[mi_IRMK_I]  ;

				lb_receipt_remark_found = true;

				//must increment the counter by 1 after using the current slot
				mi_IRMK_I = mi_IRMK_I + 1;

				LOG(logDEBUG) << "REMARK mi_IRMK_I:= " << mi_IRMK_I  ;

//				lb_receiptValid = AB_false;
//				createInfoQueue(mv_rec_queType, 3862);

			}
		}	/* valid receipt */

		if (!lb_receiptValid) {
			/* reduce receipt count and null out */
			md_RC_name[mi_RC_I] 			= AB_null;
			md_RC_bank_acc[mi_RC_I] 	= AB_null;
			md_RC_total_amt[mi_RC_I] 	= AB_null;
			md_RC_gl_acc1[mi_RC_I] 		= AB_null;
			md_RC_gl_amt1[mi_RC_I] 		= AB_null;
			md_RC_gl_acc2[mi_RC_I] 		= AB_null;
			md_RC_gl_amt2[mi_RC_I] 		= AB_null;
			md_RC_gl_acc3[mi_RC_I] 		= AB_null;
			md_RC_gl_amt3[mi_RC_I] 		= AB_null;
			md_RC_form_of_pay[mi_RC_I]      = AB_null;
			md_RC_cc_id[mi_RC_I] 		= AB_null;
			md_RC_cc_no[mi_RC_I] 		= AB_null;
			md_RC_cc_exp_date[mi_RC_I]      = AB_null;
			md_RC_link_to[mi_RC_I]          = AB_null;
			md_RC_auth_no[mi_RC_I] 	= AB_null;
			md_RC_payment_reference[mi_RC_I--] 	= AB_null;
		}	/* invalid receipt */
	}	/* for loop for receipt remarks */
}

void CInterfaceRecord::createInfoQueue(D_que_type pd_que_type, AB_Integer pi_msg_id)
{
   ENTER;

   D_description   ld_errorDescription ;

   md_queType = pd_que_type;

	LOG(logERROR)  << "DKERR >> pi_msg_id, pd_que_type = " << pi_msg_id << "," << pd_que_type  ;
	ld_errorDescription = CMsgHandler::getMsgWithSub("E");
	LOG(logERROR)  << "DKERR >> ld_errorDescription = " << ld_errorDescription  ;
	if (ld_errorDescription.IsNull())
	{
   	CMsgHandler::getMsgDescription(pi_msg_id, "E", ld_errorDescription) ;
	}

   createQueue("REC: " + ld_errorDescription) ;

   EXIT;

}

bool CInterfaceRecord::validCreditCardNumber(const AB_Varchar& pv_cc_id) {
   ENTER;
	bool lb_valid = true;

	if (pv_cc_id.NotIsNull()) {
		AB_Date 	  ld_local_date = AB_Date::getLocalDate();
		AB_Varchar lv_local_date(15);
		AB_Char 	  lc_exp_date(4);
		AB_Integer li_localDateYY;
		AB_Varchar lv_authCode(4);
		AB_Char	  lc_authCodeInd(1);

		ld_local_date.addDate(1, 0, 0);
		lv_local_date = ld_local_date;
		lc_exp_date = "01" + lv_local_date.At(9).For(2);
		LOG(logDEBUG) << "-- lc_exp_date = " << lc_exp_date ;

		CCreditCard lo_CCreditCard;
		lo_CCreditCard.setCC(pv_cc_id, lc_exp_date, lv_authCode, lc_authCodeInd, AB_TRUE, AB_FALSE);
		if (!lo_CCreditCard.isValid()) {
			lb_valid = false;
		}

		if (!CMsgHandler::getMsgStatus()) {
			AB_Integer li_msgId = CMsgHandler::getMsgId();
			D_description ld_errorDescription;
			CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;
			LOG(logERROR) << "-- ld_errorDescription = " << ld_errorDescription ;

			CMsgHandler::clearMsg();
			lb_valid = false;
		}
	}

   return lb_valid;
}

bool CInterfaceRecord::validCreditCard(const AB_Varchar& pv_input_cc_id,
													const AB_Char& pc_exp_date,
													long&	pl_error_code) {
   ENTER;

	// create clone
	AB_Varchar pv_cc_id(40);
	pv_cc_id = pv_input_cc_id.to_string();

	bool lb_valid = true;

	if (pv_cc_id.NotIsNull()) {
		AB_Varchar lv_authCode(4);
		AB_Char	  lc_authCodeInd(1);

		AB_Boolean lb_nonNumCCNo = AB_false;
		if (!pv_cc_id.At(3).isAllNumeric() &&
			 !pv_cc_id.At(3).findFirstCharNotInList("0123456789*X") >= 1) {
			lb_nonNumCCNo = AB_true;
		}

		CCreditCard lo_CCreditCard;
		if (lb_nonNumCCNo) {
			/*
			 * AB_FALSE: do not validate using ld_creditCard.input_validation_trigger() in CreditCard.cpp
			 * AB_TRUE: ignore MOD10 etc validation in CCSupplier.cpp
			 */
			lo_CCreditCard.setCC(pv_cc_id, pc_exp_date, lv_authCode, lc_authCodeInd, AB_FALSE, AB_TRUE);
		} else {
			/*
			 * AB_TRUE: validate using ld_creditCard.input_validation_trigger() in CreditCard.cpp
			 * AB_FALSE: do not ignore MOD10 etc validation in CCSupplier.cpp
			 */
			lo_CCreditCard.setCC(pv_cc_id, pc_exp_date, lv_authCode, lc_authCodeInd, AB_TRUE, AB_FALSE);
		}

		if (!lo_CCreditCard.isValid()) {
			lb_valid = false;
		}

		if (!CMsgHandler::getMsgStatus()) {
			AB_Integer li_msgId = CMsgHandler::getMsgId();
			D_description ld_errorDescription;
			CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;
			pl_error_code = li_msgId.to_long();
			LOG(logERROR) << "-- pl_error_code = " << pl_error_code ;
			LOG(logERROR) << "-- ld_errorDescription = " << ld_errorDescription ;

			CMsgHandler::clearMsg();
			lb_valid = false;
		}
	}

   return lb_valid;
}

bool CInterfaceRecord::validCreditCardBIN(const AB_Varchar& pv_input_cc_id)
{
   ENTER;

	// create clone
	AB_Varchar pv_cc_id(40);
	pv_cc_id = pv_input_cc_id.to_string();

	bool lb_valid = true;

	CCreditCard lo_CCreditCard;
	lo_CCreditCard.setCC(pv_cc_id);

	if (!lo_CCreditCard.isValidBIN())
	{
		lb_valid = false;
	}

	if (!CMsgHandler::getMsgStatus())
	{
		AB_Integer li_msgId = CMsgHandler::getMsgId();
		D_description ld_errorDescription;
		CMsgHandler::getMsgDescription(li_msgId, "E", ld_errorDescription) ;
		long pl_error_code = li_msgId.to_long();
		LOG(logERROR) << "-- pl_error_code = " << pl_error_code ;
		LOG(logERROR) << "-- ld_errorDescription = " << ld_errorDescription ;

		CMsgHandler::clearMsg();
		lb_valid = false;
	}

   	return lb_valid;
}

/*
AB_Boolean
CInterfaceRecord::validateCoupon(const D_bank_account& 	pd_bnk_id,
											const AB_Text& 			pt_coupon_id1,
											const AB_Text& 			pt_coupon_id2,
											const D_char_numeric&	pd_total_amt) {
	CDIR					lo_CDIR;
	CIRValidateCoupon	lo_CIRValidateCoupon;
	int					li_error;

	lo_CIRValidateCoupon.pc_cop_id = md_RC_cop_id;
	lo_CIRValidateCoupon.pc_iata_no = md_IRT_iata_no;
	lo_CIRValidateCoupon.pc_bnk_id = pd_bnk_id;
	lo_CIRValidateCoupon.pv_coupon1 = pt_coupon_id1;
	lo_CIRValidateCoupon.pv_coupon2 = pt_coupon_id2;
	lo_CIRValidateCoupon.pd_creation_date = md_creation_date;
	lo_CIRValidateCoupon.pt_total_amount = pd_total_amt;

	lo_CDIR.validateCoupon(lo_CIRValidateCoupon);

	if (!CMsgHandler::getMsgStatus()) {
		li_error = CMsgHandler::getMsgId();
		createInfoQueue(mv_rec_queType, li_error);
		CMsgHandler::clearMsg();
		return AB_false;
	}

	if (lo_CIRValidateCoupon.pi_isValid == "Y") {
		return AB_true;
	}

	li_error = 3946;
	createInfoQueue(mv_rec_queType, li_error);
	return AB_false;
}
*/

void CInterfaceRecord::getReceiptQueueType()
{

	CCopComp lc_copComp;
	mv_rec_queType = "REC"; // by default

	if ( md_RC_cop_id.IsNull() )
		lc_copComp.setCopId(md_IRT_cop_id);
	else
		lc_copComp.setCopId(md_RC_cop_id);
	lc_copComp.setSysId("RECB");

	lc_copComp.get_copcom();

	if ( ! CMsgHandler::getMsgStatus())
	{
		 CMsgHandler::clearMsg();
	}
	else
	{
		if ( lc_copComp.getSysParm6().NotIsNull() && AB_Integer(lc_copComp.getSysParm6()).to_long() == 1 )
			mv_rec_queType = "POS";
	}

}

void CInterfaceRecord::moveReceiptQueues( AB_Varchar pd_que_type)
{
   CIRMoveQueue     lo_moveQueue ;
   CDIR             lo_dbRIR ;

   lo_moveQueue.pc_session_id = md_session_id ;
   lo_moveQueue.pc_rejectCop = md_rejectCop;
   lo_moveQueue.pc_rejectBra = md_rejectBra;
   lo_moveQueue.pc_rejectCon = md_rejectCon;
   lo_moveQueue.pc_queue_type = pd_que_type ;

   lo_dbRIR.moveQueue(lo_moveQueue) ;
}


void CInterfaceRecord::printIremailaddress()
{
   int li_counter ;

   li_counter = 0 ;

   LOG(logDEBUG) << " ************ IREMAILADDRESS ************ "  ;
   while (li_counter < mi_EM_I)
   {
      LOG(logDEBUG) << " ************ " << li_counter << " ************ "  ;

      LOG(logDEBUG) << "md_session_id		= " << md_session_id   					  ;
		LOG(logDEBUG) << "mt_EM_email_id		= " << mt_EM_email_id[li_counter] 	 ;
		LOG(logDEBUG) << "mc_EM_email_type	= " << mc_EM_email_type[li_counter]  ;
		li_counter++;
   }
}

void
CInterfaceRecord::setCopIdByIata()
{
   ENTER;

   CDIR              lo_dbIR;
   CIRGetCopIdByIata lo_getCopIdByIata;

   if ( ! md_IRT_cop_id.IsNull() ) return;

   lo_getCopIdByIata.pc_iata_no = md_IRT_iata_no;
   lo_dbIR.getCopIdByIata( lo_getCopIdByIata );

   md_IRT_cop_id = lo_getCopIdByIata.pc_cop_id;

   LOG(logDEBUG) << "-- CInterfaceRecord::setCopIdByIata() md_IRT_cop_id = " << md_IRT_cop_id ;

   EXIT;
}

AB_Boolean CInterfaceRecord::isCCExists4copid(AB_Varchar cc_id)
{
   CDCCVendor 	lo_dbVendor; 	//CDIR              lo_dbIR;
   CCCVendorGet	lo_getVendor; 	//CIRGetCopIdByIata lo_getCopIdByIata;

   if ( md_IRT_cop_id.IsNull() ) return  AB_false;

   lo_getVendor.pc_cop_id = md_IRT_cop_id;	//lo_getCopIdByIata.pc_iata_no = md_IRT_iata_no;
   lo_getVendor.pc_vend_id = "____" + cc_id;
   lo_dbVendor.get( lo_getVendor );

   LOG(logDEBUG) << "-- CInterfaceRecord::isCCExists4copid() number of vendors = " << lo_getVendor.pi_num_cc_id ;

   if ( lo_getVendor.pi_num_cc_id == AB_Integer(0) ) return AB_false;

   return AB_true;
}


AB_Boolean CInterfaceRecord::isPFSSupplier( const D_company& cop_id, const D_supplier& sup_id )
{
      AB_Date          ld_gmt;
      CDIR             lo_dbIR;
      CIRIsPFSSupplier lo_isPFSSupplier;

      ld_gmt.getGMTNow() ;

      lo_isPFSSupplier.cop_id      = cop_id;
      lo_isPFSSupplier.sup_id      = sup_id;
      lo_isPFSSupplier.pd_lcl_date = ld_gmt;

      lo_dbIR.isPFSSupplier( lo_isPFSSupplier );

      if ( lo_isPFSSupplier.pb_is_pfs == "Y" ) return AB_true;

      return AB_false;
}

/*
	GAP9110:
	Pay For Seats is a non-bsp ticket. It has to link to the first air itin
	in the pnr.

	Here we try to find the first air itin in the consumdetail arrays and create
	a new consumdetail using that air itin
 */
bool
CInterfaceRecord::createPFSConsumdetail(const D_passenger_no&	pc_paxId,
								    				 const D_line_number&	pc_docId) {
   ENTER;

	int li_index = 0 ;
	bool lb_foundICD = false ;
	LOG(logDEBUG) << "-- mi_ICD_I = " << mi_ICD_I ;

	for (; li_index < mi_ICD_I; li_index++) {
		LOG(logDEBUG) << "-- md_ICD_itin_type[" << li_index << "] = " << md_ICD_itin_type[li_index] ;

		if (md_ICD_itin_type[li_index] == "A") {
			lb_foundICD = true;
			break;
		}
	}

	/*
		Do not create consumdetail here for PFS Costitin because we will call a new stored procedure
		"SP_Irticket__PFSCostItin" in SP_IUR__createAllCost for this purpose.
		This is different from what we do in the CA system. The reason is that in the US system,
		the stored procedure SP_Irticket__manualCostItin can not reuse transport itins already used
		by tickets in Irconsumdetail table and the CA system does not have this restriction.
		This code change is triggered by the PNR FDURYS.2010-02-14_16_35_54-0166.IUR for CWT US in the scr 435961
		where SP_Irticket__manualCostItin is used to create costitin for the first ticket and the second ticket is a PFS
		ticket (assuming that the component PFS component is set up for 'FPS' sup_id and the company 'U1')

	if (lb_foundICD) {
		md_ICD_itin_type[mi_ICD_I]          = md_ICD_itin_type[li_index];
		md_ICD_itin_line_number[mi_ICD_I]   = md_ICD_itin_line_number[li_index];
		md_ICD_ftp_miles[mi_ICD_I]          = md_ICD_ftp_miles[li_index];
		md_ICD_text[mi_ICD_I]               = md_ICD_text[li_index];
		md_ICD_fare_basis[mi_ICD_I]         = md_ICD_fare_basis[li_index];
		md_ICD_fare_by_leg[mi_ICD_I]        = md_ICD_fare_by_leg[li_index];
		md_ICD_service_status[mi_ICD_I]  	= md_ICD_service_status[li_index];

		md_ICD_pax_id[mi_ICD_I] 				= pc_paxId;
		md_ICD_line_number[mi_ICD_I] 			= pc_docId;

		mi_ICD_I++ ;
	}
	*/

   return lb_foundICD;
}

void CInterfaceRecord::parseRemarkCNF(AB_Text lv_data) {
  int             li_pos;
  AB_Boolean      lb_continue = AB_true;
  int             li_len;
  AB_Boolean      lb_ignoreWholeRemark = AB_false;
  AB_Varchar      lv_tyData(1) ;
  AB_Varchar      lv_cpData(16) ;
  AB_Boolean      lb_cpDataFount = AB_false;
  AB_Varchar      lv_singlePair(256) ;
  AB_Varchar      lv_npData(16) ;
  AB_Boolean      lb_npDataFount = AB_false;

  lv_tyData = " ";

  li_pos       = lv_data.findSubstring("!");
  if (li_pos > 0) {
    lv_data = lv_data.For(li_pos - 1);
  }

  while (lb_continue && !lb_ignoreWholeRemark)
    {
      lv_singlePair = lv_data;

      // get pair KEY-VALUE
      li_pos       = lv_data.findSubstring("/");
      if (li_pos > 0) {
	lv_singlePair = lv_data.For(li_pos - 1);
	lv_data = lv_data.At(li_pos + 1);
      }
      else {
	lb_continue = AB_false;
      }

      // work on pair
      lv_singlePair.removeTrailingBlanks() ;
      li_len = lv_singlePair.length();

      // TY- pair
      if ( lv_singlePair.For(3) == "TY-") {
	if ( li_len == 3 ) {
	  lv_tyData = "N";
	} else {
	  lv_tyData = lv_singlePair.At(4).For(1);
	}
      }

      // CP- pair
      else if ( lv_singlePair.For(3) == "CP-") {
	if ( li_len < 6 ) {
	  lb_ignoreWholeRemark = AB_true;
	} else {
	  lv_cpData = lv_singlePair.At(4).For(li_len - 3);
	  lb_cpDataFount = AB_true;
	}
      }

      // NP- pair
      else if ( lv_singlePair.For(3) == "NP-") {
	if ( li_len < 6 ) {
	  lb_ignoreWholeRemark = AB_true;
	} else {
	  lv_npData = lv_singlePair.At(4).For(li_len - 3);
	  lb_npDataFount = AB_true;
	}
      }

    }

  if ( !lb_ignoreWholeRemark )
    {
      md_IRMK_type[mi_IRMK_I]     = "NFT" ; // CNF is already in use, use NFT for NetFareTicket
      md_IRMK_remark[mi_IRMK_I]   = lv_tyData;
      md_IRMK_pax_id[mi_IRMK_I++] = AB_null;

      if ( lb_cpDataFount )
	{
	  md_IRMK_type[mi_IRMK_I]     = "NT2" ; // use NT2 for Client Plrice of NetFareTicket
	  md_IRMK_remark[mi_IRMK_I]   = lv_cpData;
	  md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
	}
      if ( lb_npDataFount )
	{
	  md_IRMK_type[mi_IRMK_I]     = "NPT" ; // use NPT for Net Plrice of NetFareTicket
	  md_IRMK_remark[mi_IRMK_I]   = lv_npData;
	  md_IRMK_pax_id[mi_IRMK_I++] = AB_null;
	}
    }

}

void CInterfaceRecord::saveTicketNo4AOF(AB_Varchar xxxTTTTTTTTTT)
{
	int           li_length ;

	LOG(logDEBUG) << "AM=====[saveTicketNo4AOF]: input[" << xxxTTTTTTTTTT << "]"  ;

  if ( mb_ticket4aofRecorded == AB_true ) return;

	xxxTTTTTTTTTT.removeLeadAndTrailBlanks() ;

	li_length = xxxTTTTTTTTTT.Length().to_long() ;
	if ( li_length >= 13 )
	{
	  //lv_ticketNoField = lv_ticketNoField.At(4).For(10);

		md_IRMK_type[mi_IRMK_I]     = "FT#" ;
		md_IRMK_remark[mi_IRMK_I]   = xxxTTTTTTTTTT.At(4).For(10);
		md_IRMK_pax_id[mi_IRMK_I++] = AB_null ;

		//md_IRMK_type[mi_IRMK_I]     = "FT2" ;
		//md_IRMK_remark[mi_IRMK_I]   = xxxTTTTTTTTTT.At(8).For(6);
		//md_IRMK_pax_id[mi_IRMK_I++] = AB_null ;

		mb_ticket4aofRecorded = AB_true;
	}

}

void CInterfaceRecord::parseAOFremark(AB_Text remark)
{
  int          li_count, li_pos;
	V_AB_Varchar lv_arrayField(100,100) ;
	AB_Varchar   lv_ticketNo(80) ;
	AB_Varchar   lv_linkToFEE(80) ;

	li_pos       = remark.findSubstring("!");
	if (li_pos > 0) {
		remark = remark.For(li_pos - 1);
	}
	//LOG(logDEBUG) << "AM===== parsing AOF remark:[" << remark << "]"  ;

	li_count = lv_arrayField.parseStringMultDel(remark,"/") ;

	lv_linkToFEE = "000";
	if ( li_count > 1 )
	{
		lv_linkToFEE = lv_arrayField[2];
		lv_linkToFEE.removeLeadAndTrailBlanks();
		if ( lv_linkToFEE.length() == 0 ) lv_linkToFEE = "000";
	}

	if ( lv_arrayField[1].length() == 0  ) lv_arrayField[1] = "TKT- ";
	lv_ticketNo = lv_arrayField[1];
	if ( lv_ticketNo.length() < 5 )
	{
	   lv_ticketNo = "TKT- ";
	}
	lv_ticketNo = lv_ticketNo.At(5);

	lv_ticketNo.removeLeadAndTrailBlanks();
	if ( lv_ticketNo.length() == 0 )
	{
		 lv_ticketNo = "0000000000";
	}
	if ( lv_ticketNo.length() > 10 )
	{
	  lv_ticketNo = lv_ticketNo.At(lv_ticketNo.length() - 9).For(10);
	}
	lv_ticketNo.padLeading( 10 - lv_ticketNo.length(), '0');

	md_IRMK_type[mi_IRMK_I]     = "AOF" ;
	md_IRMK_remark[mi_IRMK_I]   = "TKT-" + lv_ticketNo + "/" + lv_linkToFEE;
	//LOG(logDEBUG) << "AM===== adding AOF remark:[" << md_IRMK_remark[mi_IRMK_I] << "]"  ;
	md_IRMK_pax_id[mi_IRMK_I++] = AB_null;

}

void CInterfaceRecord::cleanUpAOFRemarksIfNeeded()
{
	// Rule 1: if P1/T1,etc is missing we will keep only first AOF without P1/T1,etc.
	// Rule 2: if P1.P2,etc doesn't exist in FEE remark we will keep only first AOF without P1/T1,etc.
	// Rule 3(opposite of R2):  if P1/T1 doesn't exist in AOF we will keep only first AOF without P1/T1,etc.

	AB_Varchar   lv_linkToFEE(80) ;
	V_AB_Varchar 	lv_arrayField(100,100) ;
	int li_cleanItUp = 0;
	int li_matchFound = 0;
	int li_counter;
	// remark format:
	//12345678901234567890
	//TKT-1234567890/123
	int li_delimeterPos = 4 + 10 + 1;

	//LOG(logDEBUG) << "AM====[cleanUpAOFRemarksIfNeeded]:"  ;
	for ( int aof_ind = 0; aof_ind < mi_IRMK_I && !li_cleanItUp; aof_ind++ )
	{
	   if ( md_IRMK_type[aof_ind] == "AOF" )
		{
		  //LOG(logDEBUG) << "    AM====[cleanUpAOFRemarksIfNeeded]:[" << md_IRMK_remark[aof_ind] << "]"  ;

			lv_linkToFEE = md_IRMK_remark[aof_ind].At(li_delimeterPos+1).For(md_IRMK_remark[aof_ind].length() - li_delimeterPos );
			//LOG(logDEBUG) << "    AM====[cleanUpAOFRemarksIfNeeded]: link[" << lv_linkToFEE << "]"  ;
			// check R1.
			if (lv_linkToFEE == "000" )
			{
			  //LOG(logDEBUG) << "    AM====[cleanUpAOFRemarksIfNeeded]: li_cleanItUp 1111111"  ;
				li_cleanItUp = 1;
			}
			else
			{
			   // check R2.
				li_matchFound = 0;
				for ( int fee_ind = 0; fee_ind < mi_IRMK_I && !li_matchFound; fee_ind++ )
			   {
					if ( md_IRMK_type[fee_ind] == "FEE" )
					{
					  //LOG(logDEBUG) << "        AM====[cleanUpAOFRemarksIfNeeded]:[" << md_IRMK_remark[fee_ind] << "]"  ;
						li_counter = lv_arrayField.parseStringMultDel(md_IRMK_remark[fee_ind],"/") ;
						//LOG(logDEBUG) << "        AM====[cleanUpAOFRemarksIfNeeded]:fee link[" << lv_arrayField[1] << "]"  ;
						if ( li_counter > 0 && lv_arrayField[1] == lv_linkToFEE )
						{
						  //LOG(logDEBUG) << "        AM====[cleanUpAOFRemarksIfNeeded]: li_matchFound - YES"  ;
							li_matchFound = 1;
						}
					}
			   }

				// check if we DID NOT found P1/T1, etc. in FEEs
				if ( !li_matchFound )
				{
					li_cleanItUp = 1;
				}
		   }
		}
	}

	if ( !li_cleanItUp ) // no cleanup yet, check rule 3
	{
		for ( int fee_ind = 0; fee_ind < mi_IRMK_I && !li_cleanItUp; fee_ind++ )
		{
			if ( md_IRMK_type[fee_ind] == "FEE" )
			{
			  //LOG(logDEBUG) << "        AM====[R3.cleanUpAOFRemarksIfNeeded]:[" << md_IRMK_remark[fee_ind] << "]"  ;
				li_counter = lv_arrayField.parseStringMultDel(md_IRMK_remark[fee_ind],"/") ;
				//LOG(logDEBUG) << "        AM====[R3.cleanUpAOFRemarksIfNeeded]:fee link[" << lv_arrayField[1] << "]"  ;
				if ( li_counter > 0 )
				{

					li_matchFound = 0;
				   // LOOP of AOF
					for ( int aof_ind = 0; aof_ind < mi_IRMK_I && !li_matchFound; aof_ind++ )
					{
						if ( md_IRMK_type[aof_ind] == "AOF" )
						{
						  //LOG(logDEBUG) << "    AM====[R3.cleanUpAOFRemarksIfNeeded]:[" << md_IRMK_remark[aof_ind] << "]"  ;

							lv_linkToFEE = md_IRMK_remark[aof_ind].At(li_delimeterPos+1).For(md_IRMK_remark[aof_ind].length() - li_delimeterPos );
							//LOG(logDEBUG) << "    AM====[R3.cleanUpAOFRemarksIfNeeded]: link[" << lv_linkToFEE << "]"  ;

							if ( lv_linkToFEE == "000" || lv_arrayField[1] == lv_linkToFEE )
							{
							  //LOG(logDEBUG) << "        AM====[R3.cleanUpAOFRemarksIfNeeded]: li_matchFound - YES"  ;
								li_matchFound = 1;
							}
						}
					}

					// check if we DID NOT found P1/T1, etc. in AOFs
					if ( !li_matchFound )
					{
						li_cleanItUp = 1;
					}
				}
			}
		}
	}

	// finally, do we need cleanUp ?
	if ( li_cleanItUp )
	{
		li_counter = 0;
		for ( int aof_ind = 0; aof_ind < mi_IRMK_I ; aof_ind++ )
		{
			if ( md_IRMK_type[aof_ind] == "AOF" )
			{
				li_counter++;
				if ( li_counter == 1 ) // we will keep only first AOF with 000 for P1/T1,etc. part
				{
				   md_IRMK_remark[aof_ind] = md_IRMK_remark[aof_ind].For(li_delimeterPos) + "000";
				}
				else
				{
				   md_IRMK_type[aof_ind] = "BAD";
				}
			}
		}
	}
}

void CInterfaceRecord::validateCC_BINsInFeeRemarks()
{
	string cc_id, cc_no, cc_id_no;

	LOG(logDEBUG) << "AM====[validateCC_BINsInFeeRemarks]:"  ;

	for ( int fee_ind = 0; fee_ind < mi_IRMK_I; fee_ind++ )
	{
		if (   md_IRMK_type[fee_ind] == "FEE"
			|| md_IRMK_type[fee_ind] == "SFC"
			|| md_IRMK_type[fee_ind] == "SFT"
			|| md_IRMK_type[fee_ind] == "SFM"
			)
		{
			LOG(logDEBUG) << "        AM====[validateCC_BINsInFeeRemarks]:[" << md_IRMK_remark[fee_ind] << "]"  ;
			//FEE---T1/USD2.00/OPT/FOP-CCVI4147099938152285/1217/
			//SFC,SFT---SABCCCA54723300076794580916AA                    18.64        01
			if ( PCRE::pcre_match("CC([A-Z][A-Z])(\\d+)[^\\d]" , md_IRMK_remark[fee_ind].to_string() ))
			{
				PCRE::pcre_get_group(1, cc_id);
				PCRE::pcre_get_group(2, cc_no);
				LOG(logDEBUG) << "li_matchFound - YES "  << cc_id.data() << " " << cc_no.data();

				LOG(logDEBUG) << "1 of 2. checking if cc_id is valid for the company....";
				if ( !isCCExists4copid(cc_id.data()) )
				{
					LOG(logDEBUG) << "CC_ID IS NOT VALID, INVALIDATING REMARK....";
				   	md_IRMK_type[fee_ind] = "BAD";

					createRemark("__Q", "FEE/FEE CC VENDOR INVALID");

				   	continue;
				}

				LOG(logDEBUG) << "2 of 2. checking if cc_no is valid for the cc_id....";
				cc_id_no = cc_id + cc_no;
				if ( !validCreditCardBIN(cc_id_no.data()) )
				{
					LOG(logDEBUG) << "BIN IS NOT VALID, INVALIDATING REMARK....";
				   	md_IRMK_type[fee_ind] = "BAD";

					createRemark("__Q", "FEE/FEE CC VENDOR BIN INVALID");

				   	continue;
				}
			}
		}
	}

}

void
CInterfaceRecord::createUniqueRemark( AB_Char pc_type, AB_Text pt_remark )
{
	set <string>::iterator it;
	it = singleRemarkFlags.find(pc_type.to_string());
	if ( it != singleRemarkFlags.end() )
	{
		LOG(logDEBUG) << "FOUND in singleRemarkFlags MAP, ignoring this remark:" << pc_type ;
	}
	else
	{
	  //LOG(logDEBUG) << "NOT FOUND in singleRemarkFlags MAP, will create one:" << pc_type ;
		md_IRMK_type[mi_IRMK_I]    = pc_type ;
		md_IRMK_remark[mi_IRMK_I]  = pt_remark;
		md_IRMK_pax_id[mi_IRMK_I]  = AB_null;
		mi_IRMK_I++;
		singleRemarkFlags.insert(pc_type.to_string());
	}
}

void
CInterfaceRecord::createRemark( const char* pc_type, const char* pt_remark )
{
  LOG(logINFO) << "CInterfaceRecord::createRemark" << pc_type << "%%%" << pt_remark;

		md_IRMK_type[mi_IRMK_I]    = pc_type ;
		md_IRMK_remark[mi_IRMK_I]  = pt_remark;
		md_IRMK_pax_id[mi_IRMK_I]  = AB_null;
		mi_IRMK_I++;
}

void
CInterfaceRecord::processIRVoidTicket()
{
   ENTER;

   int               li_counter;
   CDIR              dbIR;
   CIRProcessVoidTicket lo_processVoidTicket;

   li_counter  = 0;

   while ((li_counter < mi_ID_I) && CMsgHandler::getMsgStatus())
   {
      lo_processVoidTicket.pc_form       = md_ID_form[li_counter];
      lo_processVoidTicket.pc_ticket_no  = md_ID_ticket_no[li_counter];
      lo_processVoidTicket.pc_reference  = md_ID_reference[li_counter];
LOG(logDEBUG)<<"WW==InterfaceRecord::processIRVoidTicket";
		li_counter++;
   }

   dbIR.processVoidTicket(lo_processVoidTicket);

   EXIT;
}

void CInterfaceRecord::createRemarkRows(Remark& r)
{
   LOG(logINFO) << "AM===[CInterfaceRecord::createRemarkRows] storing Remark [" << r.getRemarkName().data() << "]...";

	if ( r.getRemarkName() == "NULL_REMARK" )
	{
		LOG(logDEBUG) << "AM===[CInterfaceRecord::createRemarkRows] can't create for NULL_REMARK";
		return;
	}
	for( r.it_params_begin(); !r.it_params_end(); r.it_next_param() )
	{
	  if ( r.it_param_is_storable_in_remark_table() )
	  {
			LOG(logINFO) << "AM===[CInterfaceRecord::createRemarkRows] Parameter["
			<< r.getRemarkName().data() << "]"
			<< " makes new IR table row:\n\t[" << r.it_param_type() << "]->["
			<< r.it_param_data() << "]";
			//       type in remark table      data for remark table
			createRemark(r.it_param_type(),             r.it_param_data());
	  }
	}
}



void CInterfaceRecord::RP_populateIritineraryudid(	D_service_type	itin_type,
																	D_line_number	line_number,
																	D_char_date		start_date,
																	D_city			city_id)
{
   ENTER;

	// should be AS15DECYYZ/
	//                                 A              S         15DEC                   YYZ
	string dynamicRemarkName = itin_type.to_string();
	dynamicRemarkName += "S";
	dynamicRemarkName += start_date.For(5).to_string();
   dynamicRemarkName += city_id.For(3).to_string();
   dynamicRemarkName += "/";

	LOG(logDEBUG) << "AM===[CInterfaceRecord::RP_populateIritineraryudid] populating for [" << dynamicRemarkName.data() << "]";

	Remark& r = pRemarkParser->getParsedRemark(dynamicRemarkName);
	if ( !r.isNullRemark() )
	{
		for( r.it_params_begin(); !r.it_params_end(); r.it_next_param() )
		{
			if ( r.it_param_is_for_table("Iritineraryudid") && r.it_param_is_parsed())
			{
					LOG(logINFO) << "AM===[CInterfaceRecord::populateIritineraryudid] Parameter["
					<< r.getRemarkName().data() << "]"
					<< " makes new Iritineraryudid table row:\n\t["
					<< r.it_param_name() << "]->["
					<< r.it_param_data() << "]";
					//       column name in table               data for table

					string stmt = "insert into Iritineraryudid";
					stmt += " (session_id, itin_type, line_number, report_code, report_code_value) SELECT ";

					stmt += "\'";
					stmt += md_session_id.to_string();
					stmt += "\'";

					stmt += ", \'";
					stmt += itin_type.to_string();
					stmt += "\'";

					stmt += ", ";
					stmt += line_number.to_string();

					stmt += ", \'";
					stmt += r.it_param_name();
					stmt += "\'";

					stmt += ", \'";
					stmt += r.it_param_data();
					stmt += "\'";

					stmt += "  WHERE EXISTS ( SELECT * FROM Gbllookup G";
					stmt +=                  " WHERE G.name   = \'REPORTCODE\' ";
					stmt +=                  " AND   G.code 	= \'";
					stmt +=                                       r.it_param_name();
					stmt +=                                                        "\' )";


					LOG(logDEBUG) << "AM===[CInterfaceRecord::RP_populateIritineraryudid] running SQL...[" << stmt.data() << "]";
					dbIR_ExecSqlStatement((char*)stmt.data());
					LOG(logDEBUG) << "AM===[CInterfaceRecord::RP_populateIritineraryudid] running SQL completed";
			}
		}
	}

   EXIT;
}




void CInterfaceRecord::RP_storeParsedRemarksInMemory()
{
   ENTER;

	LOG(logINFO) << pRemarkParser->parsedDataToString().data() ;

	// 1. Irremark table=============================
	int i = 0;
	for( pRemarkParser->it_remarks_begin(); !pRemarkParser->it_remarks_end(); pRemarkParser->it_next_remark() )
	{
	  createRemarkRows(pRemarkParser->it_remark());
	}

  // 2. populate  info from HS remarks (formofpayment, etc)
	for ( int li_counter = 0; li_counter < mi_IR_I; li_counter++ )
	{
		if ( md_IR_itin_type[li_counter] == "H" )
		{
			string dynamicRemarkName = "HS";
			dynamicRemarkName += md_IR_start_date[li_counter].For(5).to_string();
			//dynamicRemarkName += md_IR_origin_city_id[li_counter].For(3).to_string();
			dynamicRemarkName += "/";

			RP_saveDataTo(	dynamicRemarkName, "CC", md_IR_virtualCard_cc[li_counter]);
			RP_saveDataTo(	dynamicRemarkName, "CD", md_IR_virtualCard_exp_date[li_counter]);
			RP_saveDataTo(	dynamicRemarkName, "CN", md_IR_virtualCard_deployment[li_counter]);
			RP_saveDataTo(	dynamicRemarkName, "VP", md_IR_virtualCard_card_usage_id[li_counter]);

			AB_Text   virtualCard_gross_value_ind;
			RP_saveDataTo(	dynamicRemarkName, "CA", virtualCard_gross_value_ind);
		  if ( virtualCard_gross_value_ind == "GROSSVAL" )
				md_IR_virtualCard_gross_value_ind[li_counter] = "Y";
		  if ( virtualCard_gross_value_ind == "NETTVAL" )
				md_IR_virtualCard_gross_value_ind[li_counter] = "N";

		
			RP_saveDataTo(	dynamicRemarkName, "RAC", md_IR_rate_type[li_counter]);
			RP_saveDataTo(	dynamicRemarkName, "CG" , md_IR_rate_code[li_counter]);
			RP_saveDataTo(	dynamicRemarkName, "HK" , md_IR_harp_htlkey[li_counter]);
			LOG(logDEBUG) << "md_IR_harp_htlkey=" << md_IR_harp_htlkey[li_counter]  ;
		}
	}

   // 3. populate Irtripheader with ORT remark(override for record_issue_date)
	Remark& r = pRemarkParser->getParsedRemark("ORT/");
	if ( !r.isNullRemark() )
	{
		// get very first parameter
		r.it_params_begin();
		if ( !r.it_params_end() && r.it_param_is_parsed() )
		{
			LOG(logDEBUG) << "AM===[CInterfaceRecord::RP_createNotRemarkRows] ORT--- [" << r.it_param_data() << "]";
			md_IRT_record_creation_date = r.it_param_data();
		}
	}

   EXIT;
}

void CInterfaceRecord::RP_storeParsedRemarksInDatabase()
{
   ENTER;

	LOG(logINFO) << pRemarkParser->parsedDataToString().data() ;

  // 1. populate Iritineraryudid for AS and RS remarks
	for ( int li_counter = 0; li_counter < mi_IT_I; li_counter++ )
	{
	  RP_populateIritineraryudid(md_IT_itin_type[li_counter],
										  md_IT_line_number[li_counter],
										  md_IT_start_date[li_counter],
										  md_IT_orig_city_id[li_counter]);
	}


	// 2. Queue table=============================
	int i = 0;
	for( pRemarkParser->it_queues_begin(); !pRemarkParser->it_queues_end(); pRemarkParser->it_next_queue() )
	{
	  LOG(logINFO) << pRemarkParser->it_queue().toString().data() ;

	  string remark = pRemarkParser->it_queue().getType();
	  remark += "/";
	  remark += pRemarkParser->it_queue().getMessage();
	  createRemark("__Q", remark.data());
	}

   EXIT;
}

bool CInterfaceRecord::RP_saveDataTo(const string& remarkName, const string& paramName, AB_Char& where)
{
	string value;
	bool retValue = pRemarkParser->getData(remarkName, paramName, value);
	if ( retValue )
	{
		where = value.data();
	}

   LOG(logINFO) << "AM===[CInterfaceRecord::RP_saveDataTo]1 for [" << remarkName.data() << "::" << paramName.data() << "] returns [" << retValue << "] (" << value.data() << ")";

	return retValue;
}

bool CInterfaceRecord::RP_saveDataTo(const string& remarkName, const string& paramName, const string& groupName, AB_Char& where)
{
	string value;
	bool retValue = pRemarkParser->getData(remarkName, paramName, groupName, value);
	if ( retValue )
	{
		where = value.data();
	}

   LOG(logINFO) << "AM===[CInterfaceRecord::RP_saveDataTo]1 for [" << remarkName.data() << "::" << paramName.data() << "] returns [" << retValue << "] (" << value.data() << ")";

	return retValue;
}

bool CInterfaceRecord::RP_saveDataTo(const string& remarkName, const string& paramName, AB_Varchar& where)
{
	string value;
	bool retValue = pRemarkParser->getData(remarkName, paramName, value);
	if ( retValue )
	{
		where = value.data();
	}

   LOG(logINFO) << "AM===[CInterfaceRecord::RP_saveDataTo]2 for [" << remarkName.data() << "::" << paramName.data() << "] returns [" << retValue << "] (" << value.data() << ")";

	return retValue;
}

bool CInterfaceRecord::RP_saveDataTo(const string& remarkName, const string& paramName, const string& groupName, AB_Varchar& where)
{
	string value;
	bool retValue = pRemarkParser->getData(remarkName, paramName, groupName, value);
	if ( retValue )
	{
		where = value.data();
	}

   LOG(logINFO) << "AM===[CInterfaceRecord::RP_saveDataTo]2 for [" << remarkName.data() << "::" << paramName.data() << "] returns [" << retValue << "] (" << value.data() << ")";

	return retValue;
}

bool CInterfaceRecord::RP_saveDataTo(const string& remarkName, const string& paramName, AB_Text& where)
{
	string value;
	bool retValue = pRemarkParser->getData(remarkName, paramName, value);
	if ( retValue )
	{
		where = value.data();
	}

   LOG(logINFO) << "AM===[CInterfaceRecord::RP_saveDataTo]3 for [" << remarkName.data() << "::" << paramName.data() << "] returns [" << retValue << "] (" << value.data() << ")";

	return retValue;
}

bool CInterfaceRecord::RP_saveDataTo(const string& remarkName, const string& paramName, const string& groupName, AB_Text& where)
{
	string value;
	bool retValue = pRemarkParser->getData(remarkName, paramName, groupName, value);
	if ( retValue )
	{
		where = value.data();
	}

   LOG(logINFO) << "AM===[CInterfaceRecord::RP_saveDataTo]3 for [" << remarkName.data() << "::" << paramName.data() << "] returns [" << retValue << "] (" << value.data() << ")";

	return retValue;
}

AB_Text CInterfaceRecord::ParseTime(const AB_Text &pv_time) const {

	AB_Text lv_time = pv_time;
	AB_Text lv_AMPM ;
	AB_Text lv_result ;
	int 	  li_pos ;
	int	  li_length ;

	lv_time.removeLeadAndTrailBlanks();
	li_pos = lv_time.findFirstCharNotInList( "0123456789:" ) ;

	if ( li_pos > 0 )
	{
		lv_AMPM = lv_time.At( li_pos ).Upper() ;
		lv_time = lv_time.For( li_pos - 1 );
	}

	li_length = lv_time.length() ;
	li_pos	 = lv_time.findSubstring( ":" ) ;

   if ( li_length ==  1 )  // X
	{
       lv_result = "0" + lv_time + ":00";
	}
	else if ( li_length == 2 ) //XX
	{
       lv_result = lv_time + ":00";
	}
	else if ( li_length == 4 ) //XXXX or X:XX
	{
		if ( li_pos == 0 )
		{
		 	lv_result = lv_time.For(2) + ":" + lv_time.At(3) ;
		}
		else if ( li_pos == 2 )
		{
			lv_result = "0" + lv_time ;
		}
		else
		{
			lv_result = lv_time ;
		}

	}
	else if ( li_length == 5 && li_pos == 3 ) //XX:XX
	{
		lv_result = lv_time ;
	}
	else
	{
		lv_result = "" ;
	}

   if ( lv_AMPM.For(1) == "A" )
	{
      lv_AMPM = "AM" ;
	}
	else if ( lv_AMPM.For(1) == "P" )
	{
		lv_AMPM = "PM" ;
	}
	else
	{
		lv_AMPM = "" ;
	}


	if ( lv_result.length() == 5 )
	{
		lv_result = lv_result + lv_AMPM ;
	}
	else
	{
		lv_result = "12:00AM" ;
	}

	lv_result = lv_result.For(2) + lv_result.At(4) ;
	return lv_result ;
}

void CInterfaceRecord::ParseMoney(AB_Varchar &pv_inString, AB_Varchar &pv_outMoney)
{
   int            li_pos;

   pv_inString.removeLeadAndTrailBlanks();
   li_pos   = pv_inString.findFirstCharNotInList(".0123456789") ;

   if ( li_pos > 0 )
   {
       pv_outMoney = "0.00" ;
   }
   else
   {
       li_pos = pv_inString.findSubstring(".") ;

       if ( li_pos == 0 )
       {
            pv_outMoney = pv_inString + ".00" ;
       }
		 else if ( li_pos ==  pv_inString.length() )
		 {
				pv_outMoney = pv_inString + "00" ;
		 }
       else if ( li_pos ==  ( pv_inString.length() - 1 )  )
       {
            pv_outMoney = pv_inString + "0" ;
       }
       else
       {
            pv_outMoney = pv_inString.For( li_pos + 2 )  ;
       }
   }

}

// G9150
void CInterfaceRecord::CommonTransportLabels()
{
	string labelValue;
	AB_Char    lc_year(2) ;

	if (pRemarkParser->isRemarkParsed("LABELS"))
        {
	  //------ SUN ------------------
	  createTransportAssociatedRemark( "SUN" );

	  //------ STS Handled in fillIrBusM3------------------
	  //RP_saveDataTo(	"LABELS", "STS", md_IT_service_status[mi_IT_I - 1]);

	  //------ STP ------------------
	  createTransportAssociatedRemark( "STP" );

	  //------  EP ------------------
	  createTransportAssociatedRemark( "EP" );

	  //------  SW ------------------
	  createTransportAssociatedRemark( "SW" );

	  //------  EW ------------------
	  createTransportAssociatedRemark( "EW" );

	  //------ ST ------------------
	  RP_saveDataTo(	"LABELS", "ST",	md_IT_start_time[mi_IT_I - 1]);

	  //------ ET ------------------
	  RP_saveDataTo(	"LABELS", "ET",	md_IT_end_time[mi_IT_I - 1]);

	  //------ ED ------------------
	  if ( pRemarkParser->getData("LABELS", "ED", labelValue))
          {
            lc_year = mt_pnrDate.get_default_year(labelValue.data()) ;
            lc_year.rightJustify('0') ;
            labelValue = labelValue + lc_year.to_string() ;
            md_IT_end_date[mi_IT_I - 1]  = labelValue.data() ;
          }

	  //------ SF ------------------
	  RP_saveDataTo(	"LABELS", "SF",	md_IT_service[mi_IT_I - 1]);

	  //------ CL ------------------
	  if ( pRemarkParser->getData("LABELS", "CL", labelValue))
	  {
	    if      (labelValue.length()<3)
            {
              md_IT_classofservice[mi_IT_I - 1] = labelValue.data();
            }
            else if ((labelValue=="FIRST") || (labelValue=="FIRST CLASS") ||
                     (labelValue=="PREMIUM") || (labelValue=="PREMIUM CLASS") )
            {
	      md_IT_classofservice[mi_IT_I - 1] = "F" ;
            }
            else if ((labelValue=="BUSINESS") || (labelValue=="BUSINESS CLASS") ||
                     (labelValue=="EXECUTIVE") || (labelValue=="EXECUTIVE CLASS") )
            {
	      md_IT_classofservice[mi_IT_I - 1] = "C" ;
            }
            else
            {
	      md_IT_classofservice[mi_IT_I - 1] = "Y" ;
            }
	  }

	  //------ CF ------------------
	  RP_saveDataTo(	"LABELS", "CF",	md_IT_airline_locator[mi_IT_I - 1]);

	  //------ SEN ------------------
	  createTransportAssociatedRemark( "SEN" );
        }
}


// G9150
void CInterfaceRecord::RailLabels()
{
	string labelValue;
	if (pRemarkParser->isRemarkParsed("LABELS"))
        {
	  //------ CAR ------------------
	  createTransportAssociatedRemark( "CAR" );

	  //------  CN ------------------
	  createTransportAssociatedRemark( "CN" );

	  //------ SC ------------------
	  if ( pRemarkParser->getData("LABELS", "SC", labelValue))
          {
            labelValue += "R";
            md_IT_orig_city_id[mi_IT_I - 1] = labelValue.data();
          }

	  //------ EC ------------------
	  if ( pRemarkParser->getData("LABELS", "EC", labelValue))
          {
            labelValue += "R";
            md_IT_dest_city_id[mi_IT_I - 1] = labelValue.data();
          }

        }

	//------ SUC ------------------
	if ( pRemarkParser->getData("LABELS", "SUC", labelValue))
	{
		md_IT_sup_id[mi_IT_I - 1] = labelValue.data();
	}

}

// G9150
void CInterfaceRecord::BusLabels()
{
	string labelValue;
	if (pRemarkParser->isRemarkParsed("LABELS"))
        {
	  //------ SC ------------------
	  if ( pRemarkParser->getData("LABELS", "SC", labelValue))
          {
            labelValue += "B";
            md_IT_orig_city_id[mi_IT_I - 1] = labelValue.data();
          }

	  //------ EC ------------------
	  if ( pRemarkParser->getData("LABELS", "EC", labelValue))
          {
            labelValue += "B";
            md_IT_dest_city_id[mi_IT_I - 1] = labelValue.data();
          }
        }
}

void CInterfaceRecord::populateRestOfLabels(ItinType itin_type)
{
	ENTER;

	string labelValue;

   LOG(logINFO) << "[populateRestOfLabels] for ItinType[" << itin_type << "]0-NO_TYPE,...";

	switch (itin_type)
	{
		case NO_TYPE:
			break;


	     //--------RENTAL TABLE START -------------
		case CAR:
		   CommonRentalLabels();
			CarLabels();
			break;

		case LIMO:
		   CommonRentalLabels();
			LimoLabels();
			break;

	     //--------RENTAL TABLE END -------------

 		  // -------OTHER TABLE START -----------
		case SHIP:
		   CommonOtherLabels();
			ShipLabels();
			break;

		case TOUR:
		   CommonOtherLabels();
			TourLabels();
			break;

		case LAND:
		   CommonOtherLabels();
			LandLabels();
			break;

		case INSURANCE:
		   CommonOtherLabels();
			break;

 		  // -------OTHER TABLE END -----------

		case RAIL:
		   CommonTransportLabels();
			RailLabels();
			break;

		case BUS:
		   CommonTransportLabels();
			BusLabels();
			break;

	  default:
			break;

	}

	EXIT;
}

void CInterfaceRecord::CommonRentalLabels()
{
	ENTER;

	string labelValue;

	//------ SUC ------------------
	if ( pRemarkParser->getData("LABELS", "SUC", labelValue))
	{
		labelValue = labelValue + "C";
		md_IR_sup_id[mi_IR_I - 1] = labelValue.data();
	}

	//------ SUN ------------------
	if ( pRemarkParser->getData("LABELS", "SUN", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/SUN-" + labelValue );
	}

	//------ STS, ALSO SET IN: fill_IrRentalCar(IUR,MIR) ------------------
	//------                   fill_IrRentalLimo(IUR,MIR) ------------------
	RP_saveDataTo(	"LABELS", "STS",	md_IR_service_status[mi_IR_I - 1]);

	//------ STP ------------------
	if ( pRemarkParser->getData("LABELS", "STP", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/STP-" + labelValue );
	}

	//------ SC, ALSO SET IN: fill_IrRentalCar(IUR) ------------------
	RP_saveDataTo(	"LABELS", "SC",	md_IR_origin_city_id[mi_IR_I - 1]);

	//------ ST ------------------
	RP_saveDataTo(	"LABELS", "ST",	md_IR_start_time[mi_IR_I - 1]);

	//------ EP ------------------
	if ( pRemarkParser->getData("LABELS", "EP", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/EP-" + labelValue );
	}

	//------ EC  ------------------
	RP_saveDataTo(	"LABELS", "EC",	md_IR_dest_city_id[mi_IR_I - 1]);

	//------ ET ------------------
	RP_saveDataTo(	"LABELS", "ET",	md_IR_end_time[mi_IR_I - 1]);

	//------ TTL ------------------
	if ( pRemarkParser->getData("LABELS", "TTL", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/TTL-" + labelValue );
	}

	//------ CF ------------------
	RP_saveDataTo(	"LABELS", "CF",	md_IR_confirmation[mi_IR_I - 1]);

	//------ URA ------------------
	RP_saveDataTo(	"LABELS", "URA", "CUR_ID",	md_IR_cur_id     [mi_IR_I - 1]);
	RP_saveDataTo(	"LABELS", "URA", "AMOUNT",	md_IR_local_rate [mi_IR_I - 1]);
	RP_saveDataTo(	"LABELS", "URA", "AMOUNT",	md_IR_booked_rate[mi_IR_I - 1]);

	EXIT;
}

void CInterfaceRecord::CarLabels()
{
	ENTER;

	string labelValue;

	//------ CAR ------------------
	if ( pRemarkParser->getData("LABELS", "CAR", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/CAR-" + labelValue );
	}

	//------ DM ------------------
	if ( pRemarkParser->getData("LABELS", "DM", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/DM-" + labelValue );
	}

	//------ DA ------------------
	if ( pRemarkParser->getData("LABELS", "DA", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/DA-" + labelValue );
	}

	//------ DUR ------------------
	if ( pRemarkParser->getData("LABELS", "DUR", labelValue))
	{
	  if      ( labelValue == "DAILY")   md_IR_rate_duration[mi_IR_I-1] = "D" ;
	  else if ( labelValue == "MONTHLY") md_IR_rate_duration[mi_IR_I-1] = "M" ;
	  else if ( labelValue == "WEEKLY")  md_IR_rate_duration[mi_IR_I-1] = "W" ;
	  else if ( labelValue == "WEEKEND") md_IR_rate_duration[mi_IR_I-1] = "E" ;
	  else if ( labelValue == "SPECIAL") md_IR_rate_duration[mi_IR_I-1] = "X" ;
	  else                               md_IR_rate_duration[mi_IR_I-1] = "X" ;
	}

	//------ MI ------------------
	if ( pRemarkParser->getData("LABELS", "MI", labelValue))
	{
	  createRentalAssociatedRemark( mi_IR_I - 1, "/MI-" + labelValue );
	}

	EXIT;
}

void CInterfaceRecord::LimoLabels()
{
	ENTER;

	string labelValue;

	/* spec v1.2 says - ignore
	//------ ED ------------------
	if ( pRemarkParser->getData("LABELS", "ED", labelValue))
   {
		AB_Char    lc_year(2) ;
		lc_year = mt_pnrDate.get_default_year(labelValue.data()) ;
      lc_year.rightJustify('0') ;
      labelValue = labelValue + lc_year.to_string() ;
      md_IR_end_date[mi_IR_I - 1]  = labelValue.data() ;
   }
	*/

	//------ URA, additions for LIMO ------------------
	if ( pRemarkParser->getData("LABELS", "URA", labelValue))
	{
	  md_IR_rate_multip[mi_IR_I-1] = "1";
	}

	EXIT;
}

void CInterfaceRecord::CommonOtherLabels()
{
	ENTER;

	string labelValue;

	//------ SUN ------------------
	if ( pRemarkParser->getData("LABELS", "SUN", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/SUN-" + labelValue );
	}

	//------ STP ------------------
	if ( pRemarkParser->getData("LABELS", "STP", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/STP-" + labelValue );
	}

	//------ SC ------------------
	RP_saveDataTo(	"LABELS", "SC",	md_IO_orig_city_id[mi_IO_I - 1]);

	//------ ST ------------------
	RP_saveDataTo(	"LABELS", "ST",	md_IO_start_time[mi_IO_I - 1]);

	//------ EP ------------------
	if ( pRemarkParser->getData("LABELS", "EP", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/EP-" + labelValue );
	}

	//------ EC  ------------------
	RP_saveDataTo(	"LABELS", "EC",	md_IO_dest_city_id[mi_IO_I - 1]);

	//------ ED  ------------------
	if ( pRemarkParser->getData("LABELS", "ED", labelValue))
	{
		AB_Char      lc_year(2) ;
		lc_year = mt_pnrDate.get_default_year(labelValue.data()) ;
      lc_year.rightJustify('0') ;
      labelValue = labelValue + lc_year.to_string() ;
      md_IO_end_date[mi_IO_I - 1] = labelValue.data();
	}

	//------ ET ------------------
	RP_saveDataTo(	"LABELS", "ET",	md_IO_end_time[mi_IO_I - 1]);

	//------ TK ------------------
	if ( pRemarkParser->getData("LABELS", "TK", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/TK-" + labelValue );
	}

	EXIT;
}

void CInterfaceRecord::TourLabels()
{
	ENTER;

	//------ CF ------------------
	RP_saveDataTo(	"LABELS", "CF",	md_IO_confirmation[mi_IO_I - 1]);

	EXIT;
}

void CInterfaceRecord::ShipLabels()
{
	ENTER;

	//------ SW ------------------
	RP_saveDataTo(	"LABELS", "SW",	md_IO_departure_point[mi_IO_I - 1]);

	//------ SF ------------------
	RP_saveDataTo(	"LABELS", "SF",	md_IO_svc_description[mi_IO_I - 1]);

	//------ EW ------------------
	RP_saveDataTo(	"LABELS", "EW",	md_IO_arrival_point[mi_IO_I - 1]);

	//------ SEN ------------------
	RP_saveDataTo(	"LABELS", "SEN",	md_IO_cabin_no[mi_IO_I - 1]);

	//------ CF ------------------
	RP_saveDataTo(	"LABELS", "CF",	md_IO_confirmation[mi_IO_I - 1]);

	EXIT;
}

void CInterfaceRecord::LandLabels()
{
	ENTER;

	string labelValue;

	//------ SW ------------------
	if ( pRemarkParser->getData("LABELS", "SW", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/SW-" + labelValue );
	}

	//------ SF ------------------
	if ( pRemarkParser->getData("LABELS", "SF", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/SF-" + labelValue );
	}

	//------ EW ------------------
	if ( pRemarkParser->getData("LABELS", "EW", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/EW-" + labelValue );
	}

	//------ CL ------------------
	if ( pRemarkParser->getData("LABELS", "CL", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/CL-" + labelValue );
	}

	//------ TTL ------------------
	if ( pRemarkParser->getData("LABELS", "TTL", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/TTL-" + labelValue );
	}

	//------ URA ------------------
	if ( pRemarkParser->getData("LABELS", "URA", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/URA-" + labelValue );
	}

	//------ CAR ------------------
	if ( pRemarkParser->getData("LABELS", "CAR", labelValue))
	{
	  createOtherAssociatedRemark( mi_IO_I - 1, "/CAR-" + labelValue );
	}

	//------ CF ------------------
	RP_saveDataTo(	"LABELS", "CF",	md_IO_confirmation[mi_IO_I - 1]);

	EXIT;
}

void CInterfaceRecord::setIrReasonCodeIfOverride(AB_Varchar &lv_data) 
{
	int li_pos ;
	AB_Varchar      lv_rateData(60) ;

	LOG(logDEBUG) << "setIrReasonCodeIfOverride:lv_data=[" <<  lv_data << "]";

	//M912HS/15FEBYHZ/SV-P/EB-AMD/ABC

	li_pos = lv_data.findSubstring("/EB-") ;
	if (li_pos > 0) 
	{
		mb_reasonCodeFoundInRentalSegment = AB_true;
      cout << "FG: CInterfaceRecord::setIrReasonCodeIfOverride: lv_data.At(li_pos+4).For(2).Upper()= " << lv_data.At(li_pos+4).For(2).Upper() << flush << endl;
		md_IR_action_code[mi_IR_I] =  lv_data.At(li_pos+4).For(2).Upper();
		md_IR_EB_source[mi_IR_I] =  lv_data.At(li_pos+6).For(1).Upper();
		LOG(logDEBUG) << "DEBUG locating " <<  lv_data.At(li_pos+7).For(1);

		// if followed by /ABC-.... then there is no reason code in EB data element
		if ( PCRE::pcre_match("^/\\w+-", lv_data.At(li_pos+7).to_string() ) )
		{
			LOG(logDEBUG)  << "there is no reason code in EB, it is next data element...";
		}
		else
		{
			if (lv_data.At(li_pos+7).For(1) == "/") 
			{
				AB_Text lc_tmp;
				lc_tmp = lv_data.At(li_pos+8).For(3).Upper();
				lc_tmp.removeLeadAndTrailBlanks();

				if (lc_tmp.length()!=3) 
				{
					lc_tmp = AB_null;
				}

				md_IR_reason_code[mi_IR_I] = lc_tmp;
			}
		} 
	}
	LOG(logDEBUG) << "setIrReasonCodeIfOverride:action_code=[" <<  md_IR_action_code[mi_IR_I] << "]";
	LOG(logDEBUG) << "setIrReasonCodeIfOverride:source     =[" <<  md_IR_EB_source[mi_IR_I] << "]";
	LOG(logDEBUG) << "setIrReasonCodeIfOverride:reason_code=[" <<  md_IR_reason_code[mi_IR_I] << "]";
}

void CInterfaceRecord::TRM_Processor__updateQueueString(	AB_Varchar lv_columnName, AB_Varchar lv_columnStringValue)
{
   ENTER;

	string stmt = "update Queue set ";
	stmt += lv_columnName.to_string();
	stmt += "=";
	stmt += "\'";
	stmt += lv_columnStringValue.to_string();
	stmt += "\' ";

	stmt += "WHERE session_id=  ";
	stmt += "\'";
	stmt += md_session_id.to_string();
	stmt += "\'";

	LOG(logDEBUG) << "AM===[CInterfaceRecord::TRM_Processor__updateQueueString] running SQL...[" << stmt.data() << "]";
	dbIR_ExecSqlStatement((char*)stmt.data());
	LOG(logDEBUG) << "AM===[CInterfaceRecord::TRM_Processor__updateQueueString] running SQL completed";

   EXIT;
}

void CInterfaceRecord::TRM_Processor__updateQueueStringStringString(	AB_Varchar lv_columnName, AB_Varchar lv_columnStringValue, 	AB_Varchar lv_columnName2, AB_Varchar lv_columnStringValue2, 	AB_Varchar lv_columnName3, AB_Varchar lv_columnStringValue3)
{
   ENTER;

	string stmt = "update Queue set ";
	stmt += lv_columnName.to_string();
	stmt += "=";
	stmt += "\'";
	stmt += lv_columnStringValue.to_string();
	stmt += "\' ";

	stmt += ",";
	stmt += lv_columnName2.to_string();
	stmt += "=";
	stmt += "\'";
	stmt += lv_columnStringValue2.to_string();
	stmt += "\' ";

	stmt += ",";
	stmt += lv_columnName3.to_string();
	stmt += "=";
	stmt += "\'";
	stmt += lv_columnStringValue3.to_string();
	stmt += "\' ";

	stmt += "WHERE session_id=  ";
	stmt += "\'";
	stmt += md_session_id.to_string();
	stmt += "\'";

	LOG(logDEBUG) << "AM===[CInterfaceRecord::TRM_Processor__updateQueueString] running SQL...[" << stmt.data() << "]";
	dbIR_ExecSqlStatement((char*)stmt.data());
	LOG(logDEBUG) << "AM===[CInterfaceRecord::TRM_Processor__updateQueueString] running SQL completed";

   EXIT;
}

void CInterfaceRecord::TRM_Processor__updateQueueInt(	AB_Varchar lv_columnName, AB_Integer lv_columnIntValue)
{
   ENTER;

	string stmt = "update Queue set ";
	stmt += lv_columnName.to_string();
	stmt += "=";
	stmt += lv_columnIntValue.to_string();

	stmt += "WHERE session_id=  ";
	stmt += "\'";
	stmt += md_session_id.to_string();
	stmt += "\'";

	LOG(logDEBUG) << "AM===[CInterfaceRecord::TRM_Processor__updateQueueInt] running SQL...[" << stmt.data() << "]";
	dbIR_ExecSqlStatement((char*)stmt.data());
	LOG(logDEBUG) << "AM===[CInterfaceRecord::TRM_Processor__updateQueueInt] running SQL completed";

   EXIT;
}


