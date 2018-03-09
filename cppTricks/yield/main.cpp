#include<iostream>
#include<memory>
#include<set>
#include<vector>
#include<map>
#include<cstring>

void fsa(std::string data){
  enum State{
    START,
    INTEGER,
    COMMENT_SLASH,
    COMMENT,
    COMMENT_STAR
  };
  static State state = START;
  static int integer=0;
  unsigned pos=0;
  while(pos<data.size()){
    char c=data[pos];
    switch(state){
      case START:
        if(c>='0'&&c<='9'){
          integer*=10;
          integer+=c-'0';
          state = INTEGER;
        }else if(c==' '){

        }else if(c=='/'){
          state = COMMENT_SLASH;
        }else{
          std::cerr<<"START CHYBA"<<std::endl;
        }
        break;
      case INTEGER:
        if(c>='0'&&c<='9'){
          integer*=10;
          integer+=c-'0';
        }else if(c==' '){
          std::cout<<"CISLO JE: "<<integer<<std::endl;
          integer = 0;
          state = START;
        }else if(c=='/'){
          std::cout<<"CISLO JE: "<<integer<<std::endl;
          integer = 0;
          state = COMMENT_SLASH;
        }else{
          std::cerr<<"INTEGER CHYBA"<<std::endl;
        }
        break;
      case COMMENT_SLASH:
        if(c=='*'){
          state = COMMENT;
        }else{
          std::cerr<<"COMMENT_SLASH CHYBA"<<std::endl;
        }
        break;
      case COMMENT:
        if(c=='*'){
          state = COMMENT_STAR;
        }else{
        }
        break;
      case COMMENT_STAR:
        if(c=='/'){
          state = START;
        }else{

        }
        break;
    }
    pos++;
  }
}

int main(){
  fsa("   123");
  fsa(" 23  /");
  fsa("*");
  fsa(" 2123 2");
  fsa("*/");
  fsa("23222 ");
  return 0;
}
