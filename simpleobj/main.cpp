#include<iostream>
#include<fstream>
#include<cstdlib>
#include<geUtil/geUtil.h>

class ObjInfo{
  protected:
    enum ObjState{
      LINE_BEGINNING,
      LINE_ENDING_CR,
      LINE_ENDING_LF,
      VERTEX        ,
    };
    static void printProgress(void*,float percent){
      std::cout<<percent*100<<"%"<<std::endl;
    }
    unsigned _nofVertices;
    unsigned _nofLines;
  public:
    ObjInfo(std::ifstream&stream){
      stream.seekg(0,stream.end);
      std::streampos length = stream.tellg();
      stream.seekg(0,stream.beg);
      std::cout<<"size: "<<length<<std::endl;
      ge::util::Progress*bar=new ge::util::Progress(length);
      bar->setDefaultCallBack("loading");
      //bar->setUpdateCallBack(printProgress);
      static const char CR     = '\r';
      static const char LF     = '\n';
      static const char VERTEX = 'o' ;
      this->_nofLines=1;
      ObjState state=LINE_BEGINNING;
      while(!stream.eof()){
        int c=stream.get();
        switch(state){
          case LINE_BEGINNING:
            if(c==CR){
              state=LINE_ENDING_CR;
            }if(c==VERTEX){

            }
            break;
          case LINE_ENDING_CR:
            if(c==LF){
              state=LINE_ENDING_LF;
            }else if(c==CR){
              this->_nofLines++;
            }else{
              this->_nofLines++;
              state=LINE_BEGINNING;
            }
            break;
          case LINE_ENDING_LF:
            if(c==CR){
              this->_nofLines++;
              state=LINE_ENDING_CR;
            }else if(c==LF){
              this->_nofLines++;
              state=LINE_ENDING_LF;
            }else{
              this->_nofLines++;
              state=LINE_BEGINNING;
            }
            break;
        }
        bar->update();
      }
      delete bar;
      std::cout<<"this->_nofLines: "<<this->_nofLines<<std::endl;
    }
};

int main(int argc,char*argv[]){
  if(argc<2)return EXIT_SUCCESS;
  std::cout<<"opening: "<<argv[1]<<std::endl;

  std::ifstream f(argv[1]);
  ObjInfo*obj=new ObjInfo(f);
  f.close();

  delete obj;
  return EXIT_SUCCESS;

}
