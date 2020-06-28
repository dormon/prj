#include<ArgumentViewer/ArgumentViewer.h>
#include<iostream>
#include<fstream>

#include<catch.hpp>

using namespace argumentViewer;
using namespace std;

SCENARIO("Basic ArgumentViewer implicit cast tests"){
  char const*args[] = {"test","a","1.3f","b","+1.1e1f","c","-.2e2f","d","12","e","+13","f","-122","g","1.4","h","+1.5","i","-1.6"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  REQUIRE(a->getApplicationName()=="test");
  REQUIRE(a->getNofArguments()==nofArgs-1);
  REQUIRE(a->getf32("a",0.f)==1.3f);
  REQUIRE(a->getf32("b",0.f)==1.1e1f);
  REQUIRE(a->getf32("c",0.f)==-.2e2f);
  REQUIRE(a->getf32("d",0.f)==12.f);
  REQUIRE(a->getf32("e",0.f)==13.f);
  REQUIRE(a->getf32("f",0.f)==-122.f);
  REQUIRE(a->getf32("g",0.f)==1.4f);
  REQUIRE(a->getf32("h",0.f)==1.5f);
  REQUIRE(a->getf32("i",0.f)==-1.6f);
  //std::cerr<<a->toStr()<<std::endl;
}

SCENARIO("ArgumentViewer toStr test"){
  char const*args[] = {"test"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  a->getf32("length",10.f,"x size of vector");
  a->getf64("width" ,11.1,"y size of vector");
  a->geti32("xpix"  ,1000,"x size of window in pixels");
  a->geti64("ypix"  ,1331,"y size of window in pixels");
  a->getu32("xoff"  ,0   ,"x offset of window");
  a->getu64("yoff"  ,0   ,"y offset of window");
  a->getContext("light","attributes of light source")->getf32v("position",{0,0,0},"world space position");
  a->getContext("light")->getf64v("color",{1,0,0},"normalized color");
  a->getContext("light")->gets("name","light","name identificator");
  a->getContext("light")->getContext("info","additional informations");
  a->getContext("light")->getContext("info")->geti32v("sampling",{10,10,0,0,0,0,0,0,0,1,128,1,3},"sampling of area light sources");
  a->getContext("light")->getContext("info")->geti64v("soffset",{0,0},"offset of sampling area");
  a->getContext("light")->getContext("info")->getu32v("bytesPerColor",{1,2,1},"number of bytes per color channel");
  a->getContext("light")->getContext("info")->getu64v("bytesPerPosition",{4,8,4},"number of bytes per position");
  a->getContext("light")->getContext("info")->isPresent("directional","changes omnidirectional light into directional");
  a->getContext("light")->getContext("info")->getsv("attributes",{"castShadows","movable","scriptable","printable"},"additional attributes");
  std::string expectedOutput = 
R".(length = 10   [f32] - x size of vector
width  = 11.1 [f64] - y size of vector
xoff   = 0    [u32] - x offset of window
xpix   = 1000 [i32] - x size of window in pixels
yoff   = 0    [u64] - y offset of window
ypix   = 1331 [i64] - y size of window in pixels
light { - attributes of light source
  color    = 1 0 0 [f64*  ] - normalized color
  name     = light [string] - name identificator
  position = 0 0 0 [f32*  ] - world space position
  info { - additional informations
    attributes       = {castShadows  
                       movable       
                       scriptable    
                       printable}     [string*] - additional attributes
    bytesPerColor    = 1 2 1          [u32*   ] - number of bytes per color channel
    bytesPerPosition = 4 8 4          [u64*   ] - number of bytes per position
    directional                                 - changes omnidirectional light into directional
    sampling         = 10 10 0 0 0 0 
                       0 0 0 1 128 1 
                       3              [i32*   ] - sampling of area light sources
    soffset          = 0 0            [i64*   ] - offset of sampling area
  }
}
).";
  REQUIRE(a->toStr()==expectedOutput);
}




SCENARIO("ArgumentViewer - default value tests"){
  char const*args[] = {"test"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  REQUIRE(a->getApplicationName()=="test");
  REQUIRE(a->getf32("a",1.1f)==1.1f);
  REQUIRE(a->getf64("b",1.1 )==1.1 );
  REQUIRE(a->geti32("c",-111)==-111);
  REQUIRE(a->geti64("d",-222)==-222);
  REQUIRE(a->getu32("e",1212)==1212);
  REQUIRE(a->getu64("f",3212)==3212);
  REQUIRE(a->gets  ("g","ah")=="ah");
  REQUIRE(a->getf32v("h",{1.1f})==std::vector<float      >({1.1f}));
  REQUIRE(a->getf64v("i",{1.1 })==std::vector<double     >({1.1 }));
  REQUIRE(a->geti32v("j",{-111})==std::vector<int32_t    >({-111}));
  REQUIRE(a->geti64v("k",{-222})==std::vector<int64_t    >({-222}));
  REQUIRE(a->getu32v("l",{1212})==std::vector<uint32_t   >({1212}));
  REQUIRE(a->getu64v("m",{3212})==std::vector<uint64_t   >({3212}));
  REQUIRE(a->getsv  ("n",{"ah"})==std::vector<std::string>({"ah"}));
  REQUIRE(a->validate()==true);
  //std::cerr<<a->toStr()<<std::endl;
}

SCENARIO("Basic ArgumentViewer tests"){
  char const*args[] = {"test","aa","a","13.3f","b","1","2","bb","1","c","14.4","13.3","word","hello","types","{","GL_RGB","\\{","\\}","\\<","GL_GBR","GL_RGBA32UI","}","-p1","\\{","-p2","\\}","-p3","\\<"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  REQUIRE(a->getApplicationName()=="test");
  REQUIRE(a->getNofArguments()==nofArgs-1);
  REQUIRE(a->getArgument(3)=="b");
  REQUIRE(a->getf32("a",10.f)==13.3f);
  REQUIRE(a->geti32v("b",{0,0,0})==std::vector<int32_t>({1,2,0}));
  REQUIRE(a->getf32("bb",3.f)==1.f);
  REQUIRE(a->getf64v("c",{0.,0.,0.})==std::vector<double>({14.4,13.3,0.}));
  REQUIRE(a->isPresent("aa")==true);
  REQUIRE(a->isPresent("d")==false);
  REQUIRE(a->gets("word","none")=="hello");
  REQUIRE(a->gets("word2","none")=="none");
  REQUIRE(a->getsv("types",{"a","b","c","d"})==std::vector<std::string>({"GL_RGB","{","}","<","GL_GBR","GL_RGBA32UI"}));
  REQUIRE(a->gets("-p1")=="{");
  REQUIRE(a->gets("-p2")=="}");
  REQUIRE(a->gets("-p3")=="<");
  REQUIRE(a->validate()==true);
  //std::cerr<<a->toStr()<<std::endl;
}

SCENARIO("Vector ArgumentViewer tests"){
  char const*args[] = {"test","a","13.3f","1","2","b","1","2","14","c","13.3","1e32","d","-12","+32","-1","s","{","a","b","c","}"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  REQUIRE(a->getf32v("a")==std::vector<float>({13.3f,1.f,2.f}));
  REQUIRE(a->getu32v("b")==std::vector<uint32_t>({1,2,14}));
  REQUIRE(a->getf64v("c")==std::vector<double>({13.3,1e32}));
  REQUIRE(a->geti32v("d")==std::vector<int32_t>({-12,+32,-1}));
  REQUIRE(a->getsv  ("s")==std::vector<std::string>({"a","b","c"}));
  REQUIRE(a->validate()==true);
}


SCENARIO("Basic ArgumentViewer context tests"){
  char const*args[] = {"test","a","b","light","{","a","b","c","}","d"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  REQUIRE(a->getApplicationName()=="test");
  REQUIRE(a->isPresent("a")==true );
  REQUIRE(a->isPresent("b")==true );
  REQUIRE(a->isPresent("c")==false);
  REQUIRE(a->isPresent("d")==true );
  REQUIRE(a->getContext("light")->isPresent("a")==true);
  REQUIRE(a->getContext("light")->isPresent("b")==true);
  REQUIRE(a->getContext("light")->isPresent("c")==true);
  REQUIRE(a->validate()==true);
}

SCENARIO("Complex ArgumentViewer context tests"){
  char const*args[] = {"test","a","b","light","{","a","b","position","{","a","b","}","c","}"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  REQUIRE(a->getApplicationName()=="test");
  REQUIRE(a->isPresent("a")==true );
  REQUIRE(a->isPresent("b")==true );
  REQUIRE(a->isPresent("c")==false);
  REQUIRE(a->getContext("light")->isPresent("a")==true);
  REQUIRE(a->getContext("light")->isPresent("b")==true);
  REQUIRE(a->getContext("light")->isPresent("c")==true);
  REQUIRE(a->getContext("light")->getContext("position")->isPresent("a")==true);
  REQUIRE(a->getContext("light")->getContext("position")->isPresent("b")==true);
  REQUIRE(a->validate()==true);
}

SCENARIO("ArgumentViewer file tests"){
  std::ofstream("args0.txt")<<"a b c < args1.txt"<<std::endl;
  std::ofstream("args1.txt")<<"#ahoj \nposition { a b }\n #dormon\n"<<std::endl;
  char const*args[] = {"test","a","light","{","<","args0.txt","}","b"};
  int const nofArgs = sizeof(args)/sizeof(char const*);
  auto a = make_shared<ArgumentViewer>(nofArgs,(char**)args);
  REQUIRE(a->getApplicationName()=="test");
  REQUIRE(a->isPresent("a")==true );
  REQUIRE(a->isPresent("b")==true );
  REQUIRE(a->getContext("light")->isPresent("a")==true);
  REQUIRE(a->getContext("light")->isPresent("b")==true);
  REQUIRE(a->getContext("light")->isPresent("c")==true);
  REQUIRE(a->getContext("light")->getContext("position")->isPresent("a")==true);
  REQUIRE(a->getContext("light")->getContext("position")->isPresent("b")==true);
  REQUIRE(a->validate()==true);
  //std::cerr<<a->toStr()<<std::endl;
}






