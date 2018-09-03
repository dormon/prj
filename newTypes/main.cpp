#include<cstdint>
#include<type_traits>
#include<cstring>

namespace types{
enum class Qualifier{
  IN    = 1 << 0  ,
  OUT   = 1 << 1  ,
  INOUT = IN | OUT,
};

//value / valueContainer
//type of value
//type of container
//type can be value
//
//true|false    is value that can be stored in container with type: bit
//bit           is value that can be stored in container with type: type
//type          is value that can be stored in container with type: typeContainer
//typeContainer is value that can be stored in container with type: typeContainerContainer
//
//
//container<bit>  <=> bit
//container<type> <=> type
//container<bit            > a = true;
//container<type           > a = bit ;
//container<container<type>> a = type;
//
//variable is instantiation of container that can store values
//value    is instantiation of type
//type     is 
//
//int      A = 1       ;
//typename T = int     ;
//keyword  K = typename;
//keyword  k = keyword ;
//keyword  k = 
//atomicType A = bit | any
//typeContainers = type | atomicType |
//type A = bit | any
//any A = 1
//
//
//
//
//
//
//
//value -> value -> typeCon
//int  A = 13 ;
//type A = int;
//meta A = type
//typename<emptyTypenaame> T = int
//typename<typename<emptyTypename> T = typename<emptyTypename>

class Type                                              {                                                              };
class Parameter             :public Type                {                                                              };
class NonParameter          :public Type                {                                                              };
class StructType            :public NonParameter        {                                                              };
class NonStruct             :public NonParameter        {                                                              };
class EmptyStruct           :public StructType          {                                                              };
class Struct                :public EmptyStruct         {public:NonStruct*         inner    ;StructType*         tail ;};
class EmptyVector           :public NonStruct           {                                                              };
class Vector                :public EmptyVector         {public:NonParameter*      inner    ;                          };
class Atomic                :public NonStruct           {                                                              };
class Bit                   :public Atomic              {                                                              };
class Any                   :public Atomic              {                                                              };
class NonStructParameter    :public Parameter           {                                                              };
class StructParameterType   :public Parameter           {                                                              };
class EmptyParameterStruct  :public StructParameterType {                                                              };
class ParameterStruct       :public EmptyParameterStruct{public:NonStructParameter*inner    ;StructParameterType*tail ;};
class EmptyParameterVector  :public NonStructParameter  {                                                              };
class ParameterVector       :public EmptyParameterVector{public:Parameter*         inner    ;                          };
class AtomicParameter       :public NonStructParameter  {public:Qualifier          qualifier;NonParameter*       inner;};
}


namespace operations{
class Operation                         {                                               };
class NonPair      :public Operation    {                                               };
class PairOperation:public Operation    {                                               };
class EmptyPair    :public PairOperation{                                               };
class Pair         :public EmptyPair    {public:NonPair*  inner     ;PairOperation*tail;};
class If           :public NonPair      {public:Operation*trueBranch;                   };
class Loop         :public NonPair      {public:Operation*inner     ;                   };
class Atomic       :public NonPair      {                                               };
class Break        :public Atomic       {                                               };
class Nand         :public Atomic       {                                               };
class IsType       :public Atomic       {                                               };
class IsParameter  :public Atomic       {                                               };
class IsStruct     :public Atomic       {                                               };
class IsVector     :public Atomic       {                                               };
class IsBit        :public Atomic       {                                               };
class GetInner     :public Atomic       {                                               };
class GetTail      :public Atomic       {                                               };
class IsEmpty      :public Atomic       {                                               };
}

//<Type>               ::= <Parameter>        | <NonParameter>
//<Parameter>          ::= <StructParameter>  | <NonStructParameter>
//<StructParameter>    ::= "emptyStructParam" | "structParam" "<" <NonStructParameter> "," <StructParameter> ">"
//<NonStructParameter> ::= <VectorParameter>  | <AtomicParameter>
//<VectorParameter>    ::= "emptyVectorParam" | vectorParam < <VectorParameter> ">"
//<AtomicParameter>    ::= "param" "<" <Qualifier> "," <NonParameter> ">"
//<NonParameter>       ::= <StructType>       | <NonStruct>
//<StructType>         ::= "emptyStruct"      | "struct" "<" <NonStruct> "," <StructType> ">"
//<NonStruct>          ::= <Vector>           | <Atomic>
//<Vector>             ::= emptyVector        | "vector" "<" <NonParameter> ">"
//<Atomic>             ::= "bit" | "any"
//<Qualifier>          ::= "in" | "out" | "inout"
//
//<DECLARATION> ::= "declare" "(" <IDENTIFIER> "," <TYPE> ")"
//
//keywords:
//type            = paramType       | nonParamType
//paramType       = structParamType | nonStructParamType
//nonParamType    = structType      | nonStructType
//structType      = emptyStruct     | struct<nonStructType,structType>
//nonStructType   = vectorType      | atomicType
//vectorType      = emptyVector     | vector<nonParamType>
//atomicType      = bit             | any
//structParamType = emptyStructParam | structParam<structParam>
//paramStructType
//emptyParamStruct
//paramStruct
//emptyParamVector
//paramVector
//atomicParam
//
//nonPairFunction
//pairFunction = emptyFunction | pair{nonPairFunction,pairFunction}
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
