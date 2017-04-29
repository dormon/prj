#pragma once

class Int{
  public:
    enum Type{
      ADD,
      MUL,
      SUB,
    };
    Type _type;
    Int(Type type);
    virtual ~Int();
    template<typename TYPE>
      inline TYPE to()const;
    virtual bool operator==(Int const&other)const;
};

template<typename TYPE>
inline TYPE Int::to()const{
  return *((TYPE*)this);
}


