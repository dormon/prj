#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<list>
#include<cstring>
#include<cassert>
#include<functional>
#include<typeinfo>
#include<typeindex>


// B - bottom (left to right)
// L - left (bottom to top)
// T - top (right to left
// R - right (top to bottom)
// P - primary diagonal
// S - secondary diagonal
//
// E - even
// O - odd
//
// EO - x even, y odd
// 
// branching 4 2x2:
//
// L -> TL
//      BL
//
// B -> BB   
//      LR
//
// R -> RT
//      RB
//
// T -> LR
//      TT
//
// branching 8 4x2:
//
// L -> TTTL
//      BBBL
//
// B -> BBBB
//      LRLR
//
// R -> RTTT
//      RBBB
//
// T -> LRLR
//      TTTT
//
// branching 8 2x4:
//
// L -> TL
//      BL
//      TL
//      BL
//
// B -> BB
//      LR
//      LR
//      LR
//
// R -> RT
//      RB
//      RT
//      RB
//
// T -> LR
//      LR
//      LR
//      TT
//
// branching 16 4x4:
//
// L -> LRTL TTTL  BBBLLTTTBBBLLTTT  BBBLLTTTBBBLLTTT 
//      TTBL BBBL
//      BBTL TTTL
//      LRBL BBBL
//
// B -> BBBB BBBB
//      LRLR LRLR
//      TLRT LRLR
//      BLRB LRLR
//
// R -> RTLR RTTT
//      RBTT RBBB
//      RTBB RTTT
//      RBLR RBBB
//
// T -> TLRT LRLR
//      BLRB LRLR
//      LRLR LRLR
//      TTTT TTTT
//
//
// 4x3
//
//  BBB
//  TTTL
//  BBBL
//
// 
//
//
//
//
//
//
//

int main(){
  return 0;
}
