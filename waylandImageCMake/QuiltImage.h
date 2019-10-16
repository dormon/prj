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
      #line 29

      in highp vec2 texCoords;
      
      out highp vec4 fragColor;
      
      uniform int showQuilt;
      uniform int showAsSequence;
      uniform uint selectedView;
      // HoloPlay values
      uniform highp float pitch;
      uniform highp float tilt;
      uniform highp float center;
      uniform highp float invView;
      uniform highp float subp;
      uniform int ri;
      uniform int bi;
      uniform highp vec4 tile;
      uniform highp vec4 viewPortion;
      uniform highp vec4 aspect;
      uniform uint drawOnlyOneImage;
      
      uniform sampler2D screenTex;
      
      uniform highp float focus;

      vec2 texArr(vec3 uvz)
      {
          // decide which section to take from based on the z.
          
          highp float z = floor(uvz.z * tile.z);
          highp float focusMod = focus*(1.f-2.f*clamp(uvz.z,0.f,1.f));
          highp float x = (mod(z, tile.x) + clamp(uvz.x+focusMod,0.f,1.f)) / tile.x;
          highp float y = (floor(z / tile.x) + uvz.y) / tile.y;
          return vec2(x, y) * viewPortion.xy;
      }
      #line 64
      void main()
      {
      	highp vec3 nuv = vec3(texCoords.xy, 0.0);
      
      	highp vec4 rgb[3];
      	for (int i=0; i < 3; i = i+1) 
      	{
      		nuv.z = (texCoords.x + float(i) * subp + texCoords.y * tilt) * pitch - center;
      		nuv.z = fract(nuv.z);
      		nuv.z = (1.f - nuv.z);
          if(drawOnlyOneImage == 1u){
            if(uint(nuv.z *tile.z) == selectedView)
      		    rgb[i] = texture(screenTex, texArr(nuv));
            else
              rgb[i] = vec4(0.f);
          }else{
      		  rgb[i] = texture(screenTex, texArr(nuv));
          }
      		//rgb[i] = vec4(nuv.z, nuv.z, nuv.z, 1.f);
      	}
        







        if(showQuilt == 0){
          fragColor = vec4(rgb[ri].r, rgb[1].g, rgb[bi].b, 1.f);
        }else{
          if(showAsSequence == 0){
            fragColor = texture(screenTex, texCoords.xy);
          }else{
            uint sel = min(selectedView,uint(tile.x*tile.y-1.f));
            fragColor = texture(screenTex, texCoords.xy/vec2(tile.xy) + vec2(vec2(1.f)/tile.xy)*vec2(sel%uint(tile.x),sel/uint(tile.x)));
          }
        }
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
          ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
      else if(comp == 4)
          ge::gl::glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
      ___;

      vars.addFloat      ("quiltView.pitch"      ,673.6839f);
      vars.addFloat      ("quiltView.tilt"       ,-0.0719594f);
      vars.addFloat      ("quiltView.center"     ,0.4528985f);
      vars.addFloat      ("quiltView.invView"    ,1.00f);
      vars.addFloat      ("quiltView.subp"       ,1.f/(1920*2*3.f));
      vars.addInt32      ("quiltView.ri"         ,0);
      vars.addInt32      ("quiltView.bi"         ,2);
      vars.add<glm::vec4>("quiltView.tile"       ,5.00f, 9.00f, 45.00f, 45.00f);
      vars.add<glm::vec4>("quiltView.viewPortion",0.99976f, 0.99976f, 0.00f, 0.00f);
      vars.addFloat      ("quiltView.focus"      ,0.00f);
      //addVarsLimitsF(vars,"quiltView.focus",-1,+1,0.001f);
      vars.addBool ("showQuilt");
      vars.addBool ("renderQuilt");
      vars.addBool ("renderScene",false);
      vars.addBool ("showAsSequence",false);
      vars.addBool ("drawOnlyOneImage",false);
      vars.addUint32("selectedView",0);
      //addVarsLimitsU(vars,"selectedView",0,44);
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
      prg->set1i ("showQuilt"       ,                vars.getBool       ("showQuilt"            ))
         ->set1i ("showAsSequence"  ,                vars.getBool       ("showAsSequence"       ))
         ->set1ui("selectedView"    ,                vars.getUint32     ("selectedView"         ))
         ->set1i ("showQuilt"       ,                vars.getBool       ("showQuilt"            ))
         ->set1f ("pitch"           ,                vars.getFloat      ("quiltView.pitch"      ))
         ->set1f ("tilt"            ,                vars.getFloat      ("quiltView.tilt"       ))
         ->set1f ("center"          ,                vars.getFloat      ("quiltView.center"     ))
         //->set1f ("invView"         ,                vars.getFloat      ("quiltView.invView"    ))
         ->set1f ("subp"            ,                vars.getFloat      ("quiltView.subp"       ))
         ->set1i ("ri"              ,                vars.getInt32      ("quiltView.ri"         ))
         ->set1i ("bi"              ,                vars.getInt32      ("quiltView.bi"         ))
         ->set4fv("tile"            ,glm::value_ptr(*vars.get<glm::vec4>("quiltView.tile"       )))
         ->set4fv("viewPortion"     ,glm::value_ptr(*vars.get<glm::vec4>("quiltView.viewPortion")))
         ->set1ui("drawOnlyOneImage",                vars.getBool       ("drawOnlyOneImage"     ))
         ->set1f ("focus"           ,                vars.getFloat      ("quiltView.focus"      ))
         ->use();
      ge::gl::glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    }
  protected:
    vars::Vars&vars;
    std::shared_ptr<ge::gl::Program>prg;
    GLuint tex;
};

