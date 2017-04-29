#include<iostream>
#include<GL/glew.h>

/* This file implements function nonOf(args...)
 * This function will return integer value that is different to every argument args...
 * It is done by adding 1 until resulting value is different
 * This may be useful for default arguments
 *
 * It is composed of functions notEqual and nonOf_rec
 *  - notEqual(ref,args...) function return true if ref is different to every arguments args.
 *  - nonOf_rec(t0,args...) function return t0 if notEqual(t0,args...) or nonOf_rec(halt,t0+1,args...)
 *    so it basical increments t0 until it is different to every argument args
 */


/* terminal version of notEqual function
 */
template<typename REFERENCE_TYPE>
constexpr REFERENCE_TYPE notEqual(
    REFERENCE_TYPE const& ref  ,
    REFERENCE_TYPE const& value){
  return ref != value;
}

/* variadic version of notEqual function
 */
template<
  typename    REFERENCE_TYPE,
  typename... VALUE_TYPES   >
constexpr REFERENCE_TYPE notEqual(
    REFERENCE_TYPE    const&     ref       ,
    REFERENCE_TYPE    const&     firstValue,
    VALUE_TYPES       const& ... values    ){
  return notEqual(ref,firstValue) && notEqual(ref,values...);
}

/* this function returns first argument if it is different to every other argument
 * or call itself with first argument incremented
 */
template<
  typename    REFERENCE_TYPE,
  typename... VALUE_TYPES   >
constexpr REFERENCE_TYPE nonOf_rec(
    REFERENCE_TYPE const&     ref   ,
    VALUE_TYPES    const& ... values){
  return notEqual(ref,values...) ? ref : nonOf_rec(ref+1,values...);
}

/* this is nonOf implementation
 * first there is a check for too many arguments.
 * 256 arguments for unsigned char do not leave space for nonOf
 * then it calls nonOf_rec
 */
template<typename T,typename... Args>
constexpr T nonOf(const T t0,Args... args){
  return sizeof(T)*256==sizeof...(args)?0:nonOf_rec(t0+1,args...);
}


/* usage - function createShader()
 *
 */

const GLenum nonOfShader = nonOf(


      GL_VERTEX_SHADER,
      GL_TESS_CONTROL_SHADER,
      GL_TESS_EVALUATION_SHADER,
      GL_GEOMETRY_SHADER,
      GL_FRAGMENT_SHADER
      );


/**
 * @brief This function will add definitions into shader stage of program
 *
 * @param program shader program
 * @param definitions definitions
 * @param type type of shader stage if no type is specified, definitions will be added to every shader stage
 *
 */
void setSubroutine(
    GLuint      const& program                  ,
    std::string const& definitions              ,
    GLenum      const& stage       = nonOfShader){

  static_cast<void>(program    );
  static_cast<void>(definitions);

  if(stage == nonOfShader){
    //ADD definition to all stages
  }else{
    //ADD definition to type stage
  }
}

/* Ofcourse you could use
 * createShader(std::string shader,GLenum type = 0){}
 * but you will be never sure if 0 does not refer for example to GL_VERTEX_SHADER
 * this will make your code less prone to error when changing constants
 */

int main(){
  int a=1 ;
  int b=2 ;
  int c=10;
  std::cout<<nonOf(a,b,c)<<std::endl;
  std::cout<<nonOf(1,2,3,4,5,6)<< std::endl;
  return 0;
}
