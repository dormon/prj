#pragma once

#include <iostream>
#include <memory>
#include <geGL/geGL.h>
#include <geGL/StaticCalls.h>
#include <Vars/Vars.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "util.h"

class QuiltImage{
  public:
    QuiltImage(std::string const&fileName,vars::Vars&vars):vars(vars){
      auto vs = std::make_shared<ge::gl::Shader>(GL_VERTEX_SHADER,
      R".(
      #version 310 es
      #line 19
      out highp vec2 texCoords;
      void main(){
        texCoords = vec2(gl_VertexID&1,gl_VertexID>>1);
        gl_Position = vec4(texCoords*2.f-1.f,0.f,1.f);
      }
      ).");
      auto fs = std::make_shared<ge::gl::Shader>(GL_FRAGMENT_SHADER,
      R".(
      #version 310 es
      #line 32

      in highp vec2 texCoords;
      
      out lowp vec4 fragColor;
      
      // HoloPlay values
      uniform lowp float pitch;
      uniform lowp float tilt;
      uniform lowp float center;
      uniform lowp float invView;
      uniform lowp float subp;
      uniform lowp vec4 tile;
      uniform lowp vec4 viewPortion;
      uniform lowp vec4 aspect;
      
      uniform sampler2D screenTex;
      
      uniform lowp float focus;

      vec2 texArr(highp vec3 uvz)
      {
          // decide which section to take from based on the z.
          
          lowp float z = floor(uvz.z * 45.f /*tile.z*/);
          lowp float focusMod = focus*(1.f-2.f*clamp(uvz.z,0.f,1.f));
          highp float x = (mod(z, tile.x) + clamp(uvz.x+focusMod,0.f,1.f)) * (1.f/5.f) /* / tile.x*/;
          highp float y = (floor(z *(1.f/5.f) /* / tile.x*/) + uvz.y) * (1.f/9.f) /* / tile.y*/;
          return vec2(x, y) * viewPortion.xy;
      }
      #line 64
      void main()
      {
//        fragColor = vec4(texCoords,0.f,1.f);
//        lowp float z = (texCoords.x + texCoords.y * tilt) * pitch - center;
//        fragColor[0] = texArr(vec3(texCoords.xy,1.f-fract(z)))[0];
//        z = z + subp*pitch;
//        fragColor[1] = texArr(vec3(texCoords.xy,1.f-fract(z)))[1];
//        fragColor[2] = 0.f;
//*
        lowp float z = (texCoords.x + texCoords.y * tilt) * pitch - center;
        fragColor[0] = texture(screenTex, texArr(vec3(texCoords.xy,1.f-fract(z))))[0];
        z = z + subp*pitch;
        fragColor[1] = texture(screenTex, texArr(vec3(texCoords.xy,1.f-fract(z))))[1];
        z = z + subp*pitch;
        fragColor[2] = texture(screenTex, texArr(vec3(texCoords.xy,1.f-fract(z))))[2];
// */      
      }
      ).");
      prg = std::make_shared<ge::gl::Program>(vs,fs);

      int w;
      int h;
      int comp;
      unsigned char* image = stbi_load(fileName.c_str(), &w, &h, &comp, STBI_rgb);

      for(int y=0;y<h/2;++y)
        for(int x=0;x<w;++x)
          for(int c=0;c<comp;++c){
            auto z = image[(y*w+x)*comp+c];
            image[(y*w+x)*comp+c] = image[((h-y-1)*w+x)*comp+c];
            image[((h-y-1)*w+x)*comp+c] = z;
          }
      ___;
      /*
      auto tex = std::make_shared<ge::gl::Texture>(GL_TEXTURE_2D,GL_RGB8,1,w,h);
      ge::gl::glBindTexture(GL_TEXTURE_2D,tex->getId());
      ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      */
      ge::gl::glGenTextures(1, &tex);
      ___;

      ge::gl::glBindTexture(GL_TEXTURE_2D, tex);
      ___;
      
      ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      ___;
      ge::gl::glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      ___;
      
      ge::gl::glPixelStorei(GL_UNPACK_ROW_LENGTH,w);
      ge::gl::glPixelStorei(GL_UNPACK_ALIGNMENT ,1);
      if(comp == 3)
          ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB565, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      else if(comp == 4)
          ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
      ___;

      vars.addFloat      ("quiltView.pitch"      ,673.6839f);
      vars.addFloat      ("quiltView.tilt"       ,-0.0719594f);
      vars.addFloat      ("quiltView.center"     ,0.4528985f);
      vars.addFloat      ("quiltView.invView"    ,1.00f);
      vars.addFloat      ("quiltView.subp"       ,1.f/(1920*2*3.f));
      vars.add<glm::vec4>("quiltView.tile"       ,5.00f, 9.00f, 45.00f, 45.00f);
      vars.add<glm::vec4>("quiltView.viewPortion",0.99976f, 0.99976f, 0.00f, 0.00f);
      vars.addFloat      ("quiltView.focus"      ,-0.1f);
      //addVarsLimitsF(vars,"quiltView.focus",-1,+1,0.001f);
      //addVarsLimitsF(vars,"quiltView.tilt",-10,10,0.01);

      vars.addFloat("quiltRender.size",5.f);
      vars.addFloat("quiltRender.fov",90.f);
      vars.addFloat("quiltRender.viewCone",10.f);
      vars.addFloat("quiltRender.texScale",1.64f);
      //addVarsLimitsF(vars,"quiltRender.texScale",0.1f,5,0.01f);
      vars.addFloat("quiltRender.texScaleAspect",0.745f);
      //addVarsLimitsF(vars,"quiltRender.texScaleAspect",0.1f,10,0.01f);

    }
    ~QuiltImage(){
      ge::gl::glDeleteTextures(1,&tex);
    }
    void draw(){
      auto&focus = vars.getFloat("quiltView.focus");
      focus += 0.004f;
      if(focus > .1f)focus = -0.1f;
      prg
         ->set1f ("pitch"           ,                vars.getFloat      ("quiltView.pitch"      ))
         ->set1f ("tilt"            ,                vars.getFloat      ("quiltView.tilt"       ))
         ->set1f ("center"          ,                vars.getFloat      ("quiltView.center"     ))
         ->set1f ("subp"            ,                vars.getFloat      ("quiltView.subp"       ))
         ->set4fv("tile"            ,glm::value_ptr(*vars.get<glm::vec4>("quiltView.tile"       )))
         ->set4fv("viewPortion"     ,glm::value_ptr(*vars.get<glm::vec4>("quiltView.viewPortion")))
         ->set1f ("focus"           ,                vars.getFloat      ("quiltView.focus"      ))
         ->use();
      //auto const w = 1920*2;
      //auto const h = 1080*2;
      //auto const ww = 2560;
      //auto const hh = 1600;
      //ge::gl::glEnable(GL_SCISSOR_TEST);
      //ge::gl::glScissor((w-ww)/2,(h-hh)/2,ww,hh);
      ge::gl::glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
  protected:
    vars::Vars&vars;
    std::shared_ptr<ge::gl::Program>prg;
    GLuint tex;
};

