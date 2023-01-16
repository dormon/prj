template <typename TC>
class class1
{
public:
    template <typename TCM>
    void class_method1(TC test){};
    //void class_method2(){};
};

/*template <template<typename> typename T1, typename T2, typename T3>
void method1(T1<T2>& data, T3 a)
{
    data.class_method1<T3>(a);
    data.class_method2();
};*/

template <typename T1,typename T2>
void method2(T1& data, T2 a)
{
    data.template class_method1<T2>(a);
    //data.class_method2();
};


int main(int argc, char **argv)
{
    unsigned int a = 3;
    class1<int> test;
    //method1<class1, int, unsigned char>(test, a);
    method2<class1<int>, unsigned char>(test, a);

} 
