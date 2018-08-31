#include<cstdint>
#include<type_traits>
#include<cstring>

namespace types{
enum class Qualifier{
  IN    = 1 << 0  ,
  OUT   = 1 << 1  ,
  INOUT = IN | OUT,
};

class Type                                              {                                                               };
class Parameter             :public Type                {                                                               };
class NonParameter          :public Type                {                                                               };
class StructType            :public NonParameter        {                                                               };
class NonStruct             :public NonParameter        {                                                               };
class EmptyStruct           :public StructType          {                                                               };
class Struct                :public EmptyStruct         {public:NonStruct*         inner     ;StructType*         tail ;};
class EmptyVector           :public NonStruct           {                                                               };
class Vector                :public EmptyVector         {public:Type*              inner     ;                          };
class Atomic                :public NonStruct           {                                                               };
class Bit                   :public Atomic              {                                                               };
class Any                   :public Atomic              {                                                               };
class NonParameterStruct    :public Parameter           {                                                               };
class ParameterStructType   :public Parameter           {                                                               };
class EmptyParameterStruct  :public ParameterStructType {                                                               };
class ParameterStruct       :public EmptyParameterStruct{public:NonParameterStruct*inner     ;ParameterStructType*tail ;};
class EmptyParameterVector  :public NonParameterStruct  {                                                               };
class ParameterVector       :public EmptyParameterVector{public:Parameter*         inner     ;                          };
class AtomicParameter       :public NonParameterStruct  {public:Qualifier          qualifier;NonParameter*        inner;};
}


namespace operations{
class Statement                         {                                               };
class NonPair      :public Statement    {                                               };
class PairStatement:public Statement    {                                               };
class EmptyPair    :public PairStatement{                                               };
class Pair         :public EmptyPair    {public:NonPair*  inner     ;PairStatement*tail;};
class If           :public NonPair      {public:Statement*trueBranch;                   };
class Loop         :public NonPair      {public:Statement*inner     ;                   };
class Atomic       :public NonPair      {                                               };
class Break        :public Atomic       {                                               };
class Nand         :public Atomic       {                                               };
class IsParameter  :public Atomic       {                                               };
class IsStruct     :public Atomic       {                                               };
class IsVector     :public Atomic       {                                               };
class IsBit        :public Atomic       {                                               };
class GetInner     :public Atomic       {                                               };
class GetTail      :public Atomic       {                                               };
class IsEmpty      :public Atomic       {                                               };
}

//keywords:
//bit any struct<_,_> emptyStruct paramStruct<_,_> emptyParamStruct
//let(_,_)
//declare(_,_)
//assign(_,_)
//break(_)
//type
//param
//nonParam
//structType
//nonStructType
//emptyStruct
//struct<_,_>
//emptyVector
//vector<_>
//atomic
//bit
//any
//nonParamStruct
//paramStructType
//emptyParamStruct
//paramStruct
//emptyParamVector
//paramVector
//atomicParam
//
//<VECTOR>      ::= emptyVector | vector < <TYPE> >
//<STRUCT>      ::= emptyStruct | struct < <NON_STRUCT_TYPE> , <STRUCT> >
//<TYPE>        ::= type | param | nonParam | structType | nonStructType | <STRUCT> | emptyStruct | emptyVector | <VECTOR> | atomic | bit  | any
//<DECLARATION> ::= declare ( <IDENTIFIER> , <TYPE> )
//
//
//
//
//let(Bit0             ,emptyStruct             ) 
//let(Bit1             ,struct<bit,Bit0 >       ) 
//let(Bit2             ,struct<bit,Bit1 >       ) 
//let(Bit32            ,struct<bit,Bit31>       ) 
//let(Int32            ,Bit32                   ) 
//let(Char             ,Bit8                    ) 
//let(String           ,vector<string>          )
//let(MainReturnType   ,param<out,Int32>        ) 
//let(MainArgcType     ,param<in,Int32>         ) 
//let(MainArgvType     ,param<in,vector<String>>) 
//let(MainParameterType,paramStruct<MainReturnType,paramStruct<MainArgcType,paramStruct<MainArgvType,emptyParamStruct>>>)
//let(MainType         ,operation<MainParameterType>) 
//
//declare(MainType,main);
//
//assign(main,{
//  declare(param,mainParameters);
//  getParameters(mainParameters,main);
//  get
//
//
//  identifier(argcArgv);
//  getTail(argcArgv,main);
//  getHead(argc,);
//
//  identifier(argv);
//  
//  let(argc  ,getHead(getTail(
//  assign(return,0);
//});
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//



int main(void) {
  return 0;
}
