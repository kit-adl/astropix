// Chuck Benz, Hollis, NH   Copyright (c)2002
//
// The information and description contained herein is the
// property of Chuck Benz.
//
// Permission is granted for any reuse of this information
// and description as long as this copyright notice is
// preserved.  Modifications may be made as long as this
// notice is preserved.

// per Widmer and Franaszek

module encode (datain, dispin, dataout, clk, dispout) ;
  input [8:0]   datain ;
  input 	dispin ;  // 0 = neg disp; 1 = pos disp
  output [9:0]	dataout ;
  output	dispout ;//not used
  input 	clk;
  reg ai,bi,ci,di,ei,fi,gi,hi,ki,aeqb,ceqd,l22,l40,l04,l13,l31;
  reg l22_p1,l13_p1,l13_p2,l31_p1,l31_p2;
  reg ao,bo,co,do,eo,fo,go,ho,io,jo ;
  reg ao_del1,ao_del2,ao_del3,bo_del1,bo_del2,co_del1,co_del2,do_del1,do_del2,do_del3,eo_del1,io_del1,fo_del1,fo_del2,jo_del1,jo_del2,ho_del1,ho_del2,ho_del3,ho_del4,ho_del5,go_del1,go_del2,go_del3,go_del4,go_del5 ;
  reg ei_del1,ei_del2,ki_del1,ki_del2,di_del1,di_del2,fi_del1,fi_del2,fi_del3,gi_del1,gi_del2,gi_del3,hi_del1,hi_del2,hi_del3;
  reg bi_del1,ci_del1,dispin_del1,dispin_del2,dispin_del3,dispin_del4;
  reg co_p3;
  reg eo_p2_del,eo_p2;
  reg io_p2,io_p2_del,io_p3,io_p4,io_p4_del,io_p5,io_p5_del;
  reg jo_p1,jo_p1_del1,jo_p1_del2;
  reg pd1s6_p1,pd1s6_p1_del,nd1s6_p3_del,nd1s6_p3,pdos6_del1;
  reg pd1s4_del4,pd1s4_del3,pd1s4_del2,pd1s4_del1,nd1s4_del4,nd1s4_del3,nd1s4_del2,nd1s4_del1;
  reg pd1s6,nd1s6,ndos6,pdos6,alt7,nd1s4,pd1s4,compls6,disp6,compls4;
  reg ndos4,ndos4_del1,pdos4,pdos4_del1 ;
  reg [5:0] dataout_p2_del,dataout_p2 ;
  reg dispout_del ;
  
  //put here reg instead of wire
  //use alway block  
  always @(posedge clk) begin

  ai <= datain[0];
  bi <= datain[1];
  ci <= datain[2];
  di <= datain[3];
  ei <= datain[4];
  fi <= datain[5];
  gi <= datain[6];
  hi <= datain[7];
  ki <= datain[8];

  aeqb 		<= (ai & bi) | (!ai & !bi) ;
  ceqd 		<= (ci & di) | (!ci & !di) ;
  l22_p1 	<= (ai & bi & !ci & !di) | (ci & di & !ai & !bi) ;
  l22 		<= l22_p1 | ( !aeqb & !ceqd) ;
  l40 		<= ai & bi & ci & di ;
  l04 		<= !ai & !bi & !ci & !di ;
  l13_p1 	<= (!ci & !di) ;
  l13_p2 	<= (!ai & !bi) ;
  l31_p1 	<= (ci & di) ;
  l31_p2 	<= (ai & bi) ;
  l13 		<= ( !aeqb & l13_p1) | ( !ceqd & l13_p2 ) ;
  l31 		<= ( !aeqb & l31_p1) | ( !ceqd & l31_p2) ;

//end 
// end putting reg
  // The 5B/6B encoding
  
  ei_del1 	<= ei;
  ei_del2 	<= ei_del1 ;
  ki_del1       <= ki;
  ki_del2       <= ki_del1 ;
  di_del1	<= di ;
  di_del2	<= di_del1 ;
  fi_del1	<= fi ;
  fi_del2	<= fi_del1 ;
  fi_del3	<= fi_del2 ;
  gi_del1	<= gi ;
  gi_del2	<= gi_del1 ;
  gi_del3       <= gi_del2 ;
  hi_del1	<= hi ;
  hi_del2	<= hi_del1 ;
  hi_del3	<= hi_del2 ;
  bi_del1 	<= bi;
  ci_del1 	<= ci;
  dispin_del1	<= dispin ;
  dispin_del2	<= dispin_del1 ;
  dispin_del3	<= dispin_del2 ;
  dispin_del4	<= dispin_del3 ;
  co_p3 	<= (ei & di & !ci & !bi & !ai) ;
  eo_p2_del 	<= !(ei & di & !ci & !bi & !ai) ;
  eo_p2 	<= eo_p2_del ;
  io_p2_del 	<= (ei & !di & !ci & !(ai&bi)) ;
  io_p2 	<= io_p2_del;
  io_p3 	<= (ei_del1 & l40);
  io_p4_del 	<= (ki & ei & di & ci & !bi & !ai) ;
  io_p4 	<= io_p4_del ;
  io_p5_del 	<= (ei & !di & ci & !bi & !ai) ;
  io_p5 	<= io_p5_del ;
  pd1s6_p1_del 	<= (ei & di & !ci & !bi & !ai) ;
  pd1s6_p1      <= pd1s6_p1_del ;
  nd1s6_p3_del	<= (!ei & !di & ci & bi & ai) ;
  nd1s6_p3      <= nd1s6_p3_del ;
  jo_p1_del2	<= (!hi & (gi ^ fi)) ;
  jo_p1_del1	<= jo_p1_del2 ;
  jo_p1		<= jo_p1_del1 ;

  ao 		<= ai ;
  bo 		<= (bi_del1 & !l40) | l04 ;
  co 		<= l04 | ci_del1 | co_p3 ;
  do 		<= di & ! (ai & bi & ci) ;
  eo 		<= (ei_del2 | l13) & eo_p2;
  io 		<= (l22 & !ei_del2) |
	    		io_p2 |  // D16, D17, D18
	    		io_p3 |
	    		io_p4 | // K.28
	   		io_p5 ;
   
  ao_del1	<= ao ;
  ao_del2	<= ao_del1 ;
  ao_del3	<= ao_del2 ;
  bo_del1	<= bo ;
  bo_del2	<= bo_del1 ;
  co_del1	<= co ;
  co_del2	<= co_del1 ;
  do_del1	<= do ;
  do_del2	<= do_del1 ;
  do_del3	<= do_del2 ;

  // pds16 indicates cases where d-1 is assumed + to get our encoded value
  pd1s6 <= pd1s6_p1 | (!ei_del2 & !l22 & !l31) ;
  // nds16 indicates cases where d-1 is assumed - to get our encoded value
  nd1s6 <= ki_del2 | (ei_del2 & !l22 & !l13) | nd1s6_p3 ;

  // ndos6 is pds16 cases where d-1 is + yields - disp out - all of them
  ndos6 <= pd1s6 ;
  // pdos6 is nds16 cases where d-1 is - yields + disp out - all but one
  pdos6 <= ki_del2 | (ei_del2 & !l22 & !l13) ;


  // some Dx.7 and all Kx.7 cases result in run length of 5 case unless
  // an alternate coding is used (referred to as Dx.A7, normal is Dx.P7)
  // specifically, D11, D13, D14, D17, D18, D19.
  alt7 		<= fi_del2 & gi_del2 & hi_del2 & (ki_del2 | 
			      (dispin_del2 ? (!ei_del2 & di_del2 & l31) : (ei_del2 & !di_del2 & l13))) ;

  eo_del1	<= eo ; 
  fo 		<= fi_del3 & ! alt7 ;
  fo_del1	<= fo ;
  fo_del2 	<= fo_del1 ;
  go 		<= gi | (!fi & !gi & !hi) ;
  go_del1	<= go ;
  go_del2	<= go_del1 ;
  go_del3	<= go_del2 ;
  go_del4	<= go_del3 ;
  go_del5	<= go_del4 ;
  ho 		<= hi ;
  ho_del1	<= ho ;
  ho_del2 	<= ho_del1 ;
  ho_del3	<= ho_del2 ;
  ho_del4	<= ho_del3 ;
  ho_del5	<= ho_del4 ;
  io_del1	<= io ;
  jo 		<= jo_p1 | alt7 ;
  jo_del1	<= jo ;
  jo_del2	<= jo_del1 ;
   
  // nd1s4 is cases where d-1 is assumed - to get our encoded value
  nd1s4 	<= fi & gi ;
  nd1s4_del1	<= nd1s4 ;
  nd1s4_del2	<= nd1s4_del1 ;
  nd1s4_del3 	<= nd1s4_del2 ;
  nd1s4_del4 	<= nd1s4_del3 ;

  // pd1s4 is cases where d-1 is assumed + to get our encoded value
  pd1s4 	<= (!fi & !gi) | (ki & ((fi & !gi) | (!fi & gi))) ;
  pd1s4_del1 	<= pd1s4 ;
  pd1s4_del2	<= pd1s4_del1 ;
  pd1s4_del3	<= pd1s4_del2 ;
  pd1s4_del4	<= pd1s4_del3 ;

//Beide Signale werden nicht verwendet, da kein dispout
  // ndos4 is pd1s4 cases where d-1 is + yields - disp out - just some
  //wire ndos4 = (!fi & !gi) ;
  // pdos4 is nd1s4 cases where d-1 is - yields + disp out 
  //wire pdos4 = fi & gi & hi ;

//Nachfolgende Abschnitt (illegalk Signal) wird in diese Konfiguration nicht verwendet
//daher komplett auskommentiert.
  // only legal K codes are K28.0->.7, K23/27/29/30.7
  //	K28.0->7 is ei=di=ci=1,bi=ai=0
  //	K23 is 10111
  //	K27 is 11011
  //	K29 is 11101
  //	K30 is 11110 - so K23/27/29/30 are ei & l31
//  wire illegalk = ki & 
//		  (ai | bi | !ci | !di | !ei) & // not K28.0->7
//		  (!fi | !gi | !hi | !ei | !l31) ; // not K23/27/29/30.7

  // now determine whether to do the complementing
  // complement if prev disp is - and pd1s6 is set, or + and nd1s6 is set
  compls6 <= (pd1s6 & !dispin_del3) | (nd1s6 & dispin_del3) ;

  // disparity out of 5b6b is disp in with pdso6 and ndso6
  // pds16 indicates cases where d-1 is assumed + to get our encoded value
  // ndos6 is cases where d-1 is + yields - disp out
  // nds16 indicates cases where d-1 is assumed - to get our encoded value
  // pdos6 is cases where d-1 is - yields + disp out
  // disp toggles in all ndis16 cases, and all but that 1 nds16 case
  pdos6_del1    <= pdos6 ;
  disp6   	<= dispin_del4 ^ (ndos6 | pdos6_del1) ;

  compls4 	<= (pd1s4_del4 & !disp6) | (nd1s4_del4 & disp6) ;
  ndos4 	<= (!fi_del3 & !gi_del3) ;
  pdos4 	<= fi_del3 & gi_del3 & hi_del3 ; 
  ndos4_del1	<= ndos4 ;
  pdos4_del1	<= pdos4 ;
  dataout_p2    <= {(io_del1 ^ compls6), (eo_del1 ^ compls6),
                     (do_del3 ^ compls6), (co_del2 ^ compls6), 
                     (bo_del2 ^ compls6), (ao_del3 ^ compls6)} ;
 dataout_p2_del <= dataout_p2 ;
 dispout_del <= disp6 ^ (ndos4_del1 | pdos4_del1) ;//not used in this configuration 
end  
 assign dispout = dispout_del;//not used in this configuration
 assign dataout = {(jo_del2 ^ compls4), (ho_del5 ^ compls4),
		    (go_del5 ^ compls4), (fo_del2 ^ compls4),
		    dataout_p2_del[5:0]} ;


endmodule
