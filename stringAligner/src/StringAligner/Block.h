#pragma once

#include<StringAligner/Fwd.h>
#include<StringAligner/stringaligner_export.h>
#include<string>
#include<functional>

class STRINGALIGNER_EXPORT stringAligner::Block{
  public:
    using OnChange = std::function<void(Block*)>;
    virtual size_t      getWidth    (        )const = 0;
    virtual size_t      getHeight   (        )const = 0;
    virtual std::string getLine     (size_t i)const = 0;
    virtual std::string getData     (        )const = 0;
    void                setOnChange (OnChange const&fce);
    void                callOnChange();
  protected:
    OnChange onChange;
};
