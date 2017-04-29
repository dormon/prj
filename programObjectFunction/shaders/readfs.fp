#version 440 core

#ifndef READ_BINDING_TEXTURE
#define READ_BINDING_TEXTURE 0
#endif//READ_BINDING_TEXTURE

#ifndef READTEXTURE_SIZE_X
#define READTEXTURE_SIZE_X 1024
#endif//READTEXTURE_SIZE_X

#ifndef READTEXTURE_SIZE_Y
#define READTEXTURE_SIZE_Y 1024
#endif//READTEXTURE_SIZE_Y

#ifndef WORKGROUP_SIZE_X
#define WORKGROUP_SIZE_X 8
#endif//WORKGROUP_SIZE_X

#ifndef WORKGROUP_SIZE_Y
#define WORKGROUP_SIZE_Y 8
#endif//WORKGROUP_SIZE_Y

#ifndef NOF_CHANNELS
#define NOF_CHANNELS 4
#endif//NOF_CHANNELS

#if   NOF_CHANNELS == 1
#define INTERNALFORMAT r32f
#elif NOF_CHANNELS == 2
#define INTERNALFORMAT rg32f
#elif NOF_CHANNELS == 3
#define INTERNALFORMAT rgba32f
#else
#define INTERNALFORMAT rgba32f
#endif

#if   NOF_CHANNELS == 1
#define DATATYPE float
#elif NOF_CHANNELS == 2
#define DATATYPE vec2
#elif NOF_CHANNELS == 3
#define DATATYPE vec3
#else
#define DATATYPE vec4
#endif

layout(INTERNALFORMAT,binding=READ_BINDING_READTEXTURE)uniform image2D readTexture;

layout(location=0)out vec4 fColor;

void main(){
  DATATYPE data = DATATYPE(imageLoad(readTexture,ivec2(gl_FragCoord.xy)));
#if    NOF_CHANNELS == 1
  if(data == float(10000))
#else//NOF_CHANNELS
  if(all(equal(data,DATATYPE(10000))))
#endif//NOF_CHANNELS
    fColor=vec4(1,0,0,1);
  else
    discard;
}
