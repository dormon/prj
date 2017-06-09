#pragma once
//
// 0 ...    0
// 0 ...   01
// 0 ...  010
// 0 ...  011
// 0 ... 0100
//
//
//
// Numbers:
// B:
// 0
// 1
// N0:
// ...0
// ...1
// N1:
// ......0
// ......1
// 0+0 = 0
// 0+1 = 1
// 1+0 = 1
// 1+1 = 1
//
// 0*0 = 0
// 0*1 = 0
// 1*0 = 0
// 1*1 = 0
//
// 0^0 = 0
// 0^1 = 1
// 1^0 = 1
// 1^1 = 0
// (A^B)^C = A^(B^C)
//
// D(i  ) = A(i) ^ B(i) ^ C(i)
// C(i+1) = A(i)*B(i) + A(i)*C(i) + B(i)*C(i)
// C(0  ) = 0
//

// ###########################
// Atomic:
// ###########################
// DECLARE()
// 0. 0   (inout Real a);
// 1. ++  (inout Real a);
// 2. -   (out Real r,in Real a);
// 3. <   (out Bool c,in Real a,in Real b);
// 4. nand(out Bool r,in Bool a,in Bool b);
//
// forall r,a,b: nand(r,a,b) <=> nand(r,b,a)
//
// ###########################
// Composite
// ###########################
// !(out Bool r,in Bool a){
//   nand(r,a,a);
// }
//
// &&(out Bool r,in Bool a,in Bool b){
//   DECLACE(Bool,c);
//   nand(c,a,b);
//   !(r,c);
// }
//
// ||(out Bool r,in Bool a,in Bool b){
//   DECLARE(Bool,aa);
//   DECLARE(Bool,bb);
//   !(aa,a);
//   !(bb,b);
//   nand(r,aa,bb);
// }
//
// ^(out Bool r,in Bool a,in Bool b){
//   DECLARE(Bool,nota);
//   DECLARE(Bool,notb);
//   DECLARE(Bool,notab);
//   DECLARE(Bool,notba);
//   !(nota,a);
//   !(notb,b);
//   &&(notab,nota,b);
//   &&(notba,notb,a);
//   ||(notab,notba);
// }
//
// ==(out Bool r,in Real a,in Real b){
//   DECLARE(Bool,x);
//   DECLARE(Bool,y);
//   DECLARE(Bool,xx);
//   DECLARE(Bool,yy);
//   <(x,a,b);
//   <(y,b,a);
//   !(xx,x);
//   !(yy,y);
//   &&(r,xx,yy);
// }
//
// !=(out Bool r,in Real a,in Real b){
//   DECLARE(Bool,x);
//   ==(x,a,b);
//   !(r,x);
// }
// 
// <=(out Bool r,in Real a,in Real b){
//   DECLARE(Bool,less);
//   DECLARE(Bool,equal);
//   <(less,a,b);
//   ==(equal,a,b);
//   &&(r,less,equal);
// }
//
// >(out Bool r,in Real a,in Real b){
//   DECLARE(Bool,lequal);
//   <=(lequal,a,b);
//   !(r,lequal);
// }
//
// >=(out Bool r,in Real a,in Real b){
//   DECLARE(Bool,less);
//   <(less,a,b);
//   !(r,less);
// }
//
// =(out Real r,in Real a){
//   DECLARE(Real,b);
//   -(b,a);
//   -(r,b);
// }
//
// abs(out Real r,in Real a){
//   DECLARE(Real,zero);
//   DECLARE(Bool,c);
//   0(zero);
//   =(r,a);
//   <(c,a,zero);
//   if(c)
//     -(r,a);
// }
//
// --(inout Real a){
//   DECLARE(Real,b);
//   -(b,a);
//   ++(b);
//   -(a,b);
// }
//
// +(out Real r,in Real a,in Real b){
//   DECLARE(Real,i);
//   DECLARE(Bool,c);
//   DECLARE(Bool,bnegative);
//   DECLARE(Real,bb);
//   0(i);
//   0(c);
//   =(bb,b);
//   =(r,a);
//   <(bnegative,b,i);
//   if(bnegative){
//     -(r,r);
//     -(bb,bb);
//   }
//   <(c,i,bb);
//   while(c){
//     ++(i);
//     ++(r);
//     <(c,i,bb);
//   }
//   if(bnegative)
//     -(r,r);
// }
//
// -(out Real r,in Real a,in Real b){
//   DECLARE(Real,bb);
//   -(bb,b);
//   +(r,a,bb);
// }
//
// *(out Real r,in Real a,in Real b){
//   DECLARE(Real,aa);
//   DECLARE(Real,ab);
//   DECLARE(Bool,c1);
//   DECLARE(Bool,c2);
//   DECLARE(Real,i);
//   DECLARE(Real,j);
//   0(r);
//   abs(aa,a);
//   abs(ab,b);
//   0(i);
//   <(c1,i,aa);
//   while(c1){
//     0(j);
//     <(c2,j,ab);
//     while(c2){
//       ++(r);
//       ++(j);
//       <(c2,j,ab);
//     }
//     ++(i);
//     <(c1,i,aa);
//   }
//   DECLARE(Bool,anegative);
//   DECLARE(Bool,bnegative);
//   0(i);
//   <(anegative,a,i);
//   <(bnegative,b,i);
//   DECLARE(Bool,oneNegative);
//   ^(oneNegative,anegative,bnegative);
//   if(oneNegative)
//     -(r,r);
// }
//
//
// Loop transform:
//
// 1. Loop swapping:
// while(a){
//   while(b){
//     f(x0,x1,...);
//   }
// }
//
// ->
// 
// while(b){
//   while(a){
//     f(x0,x1,...);
//   }
// }
//
//
// 2. Loop distribution:
// while(a){
//   f(x0,x1,...);
//   g(y0,y1,...);
// }
//
// ->
//
// while(a){
//   f(x0,x1,...);
// }
// while(a){
//   f(y0,y1,...);
// }
//
// 3. Loop elimination
// while(a){
//   f(in^out x0,in^out x1,in^out ...);
// }
//
// ->
//
// f(in^out x0,in^out x1,in^out ...}
//
// 1. statement swapping:
// Swap two statements order if they are independent to each other
// S0;
// S1;
//
// ->
//
// S1;
// S0;

