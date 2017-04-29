#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

#include<geDE/TypeRegister.h>

class StrangeClass{
  public:
    StrangeClass(){
      std::cout<<"StrangeClass::StrangeClass()"<<std::endl;
    }
    ~StrangeClass(){
      std::cout<<"StrangeClass::~StrangeClass()"<<std::endl;
    }
};

int main(){
  ge::de::TypeRegister tr;
  
  auto Void = tr.addAtomicType("void" ,0               );(void)Void;
  auto Bool = tr.addAtomicType("bool" ,sizeof(bool    ));(void)Bool;
  auto I8   = tr.addAtomicType("i8"   ,sizeof(int8_t  ));(void)I8  ;
  auto I16  = tr.addAtomicType("i16"  ,sizeof(int16_t ));(void)I16 ;
  auto I32  = tr.addAtomicType("i32"  ,sizeof(int32_t ));(void)I32 ;
  auto I64  = tr.addAtomicType("i64"  ,sizeof(int64_t ));(void)I64 ;
  auto U8   = tr.addAtomicType("u8"   ,sizeof(uint8_t ));(void)U8  ;
  auto U16  = tr.addAtomicType("u16"  ,sizeof(uint16_t));(void)U16 ;
  auto U32  = tr.addAtomicType("u32"  ,sizeof(uint32_t));(void)U32 ;
  auto U64  = tr.addAtomicType("u64"  ,sizeof(uint64_t));(void)U64 ;
  auto F32  = tr.addAtomicType("f32"  ,sizeof(float   ));(void)F32 ;
  auto F64  = tr.addAtomicType("f64"  ,sizeof(double  ));(void)F64 ;
  auto Vec3 = tr.addCompositeType("vec3",{ge::de::TypeRegister::ARRAY,3,F32});(void)Vec3;
  auto Mat4 = tr.addCompositeType("mat4",{ge::de::TypeRegister::ARRAY,4,ge::de::TypeRegister::ARRAY,4,F32});(void)Mat4;
  auto Light = tr.addCompositeType("light",{ge::de::TypeRegister::STRUCT,2,Vec3,F32});(void)Light;
  auto Fce   = tr.addCompositeType("sin",{ge::de::TypeRegister::FCE,F32,1,F32});(void)Fce;
  auto Strange = tr.addAtomicType("Strange",sizeof(StrangeClass),[](uint8_t*ptr){((StrangeClass*)ptr)->~StrangeClass();},[](int8_t*ptr){new(ptr)StrangeClass;});(void)Strange;
  auto SharedFloat = tr.addAtomicType("SharedFloat",sizeof(std::shared_ptr<float>),[](uint8_t*ptr){((std::shared_ptr<float>*)ptr)->~shared_ptr();},[](int8_t*ptr){new(ptr)std::shared_ptr<float>;});(void)SharedFloat;
  for(size_t i=0;i<tr.getNofTypes();++i)
    std::cout<<tr.type2Str(i)<<std::endl;
  auto ptr = tr.construct(Strange);
  tr.destroy(ptr,Strange);
  auto ptr2 = tr.construct(SharedFloat);
  *((std::shared_ptr<float>&)ptr2) = 32;
  tr.destroy(ptr2,SharedFloat);

  return 0;
}


