#pragma once

#include<glm/glm.hpp>
#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<memory>

class SVGLine;
class SVGCircle;

class SVGElement{
  public:
    virtual ~SVGElement(){}
    SVGLine const&toLine()const;
    SVGCircle const&toCircle()const;
    enum Type{
      CIRCLE,
      LINE  ,
    }type;
    SVGElement(Type t):type(t){}
    virtual glm::vec2 getMin()const = 0;
    virtual glm::vec2 getMax()const = 0;
};

class SVGLine: public SVGElement{
  public:
    SVGLine(glm::vec2 const&a,glm::vec2 const&b,glm::vec3 const&color = glm::vec3(1,0,0),float width = 1):SVGElement(LINE),a(a),b(b),color(color),width(width){}
    glm::vec2 a;
    glm::vec2 b;
    glm::vec3 color;
    float width;
    virtual glm::vec2 getMin()const override{
      return glm::min(a,b);
    }
    virtual glm::vec2 getMax()const override{
      return glm::max(a,b);
    }
};

class SVGCircle: public SVGElement{
  public:
    SVGCircle(glm::vec2 const&p,float r,float w,glm::vec3 const&sC,glm::vec3 const&fC):SVGElement(CIRCLE),pos(p),radius(r),width(w),sColor(sC),fColor(fC){}
    glm::vec2 pos;
    float radius;
    float width;
    glm::vec3 sColor;
    glm::vec3 fColor;
    virtual glm::vec2 getMin()const override{
      return pos;
    }
    virtual glm::vec2 getMax()const override{
      return pos;
    }
};

SVGLine const&SVGElement::toLine()const{
  return*(SVGLine*)this;
}
SVGCircle const&SVGElement::toCircle()const{
  return*(SVGCircle*)this;
}


class SVG{
  public:
    glm::vec2 size;
    SVG(glm::vec2 const&size):size(size){}

    std::string attribf(std::string const&name,float value){
      std::stringstream ss;
      ss << name << "=" << "\"" << value <<"\"";
      return ss.str();
    }
    std::string attribs(std::string const&name,std::string const&value){
      std::stringstream ss;
      ss << name << "=" << "\"" << value <<"\"";
      return ss.str();
    }
    std::string width(float value){
      return attribf("width",value);
    }
    std::string height(float value){
      return attribf("height",value);
    }
    std::string rgb(glm::vec3 const&color){
      std::stringstream ss;
      ss << "rgb(";
      ss << uint32_t(color.x*255);
      ss << ",";
      ss << uint32_t(color.y*255);
      ss << ",";
      ss << uint32_t(color.z*255);
      ss << ")";
      return ss.str();
    }
    std::string stroke(glm::vec3 const&color){
      return attribs("stroke",rgb(color));
    }
    std::string fill(glm::vec3 const&color){
      return attribs("fill",rgb(color));
    }
    std::string strokeWidth(float value){
      return attribf("stroke-width",value);
    }

    std::string headerBegin(glm::vec2 const&s){
      std::stringstream ss;
      ss << "<svg ";
      ss << width(s.x);
      ss << " ";
      ss << height(s.y);
      ss << ">" << std::endl;
      return ss.str();
    }
    std::string headerEnd(){
      std::stringstream ss;
      ss << "</svg>";
      return ss.str();
    }
    std::string line(glm::vec2 const&a,glm::vec2 const&b,glm::vec3 const&color = glm::vec3(1,0,0),float width = 1){
      std::stringstream ss;
      ss << "<line ";
      ss << attribf("x1",a.x);
      ss << " ";
      ss << attribf("y1",a.y);
      ss << " ";
      ss << attribf("x2",b.x);
      ss << " ";
      ss << attribf("y2",b.y);
      ss << " ";
      ss << stroke(color);
      ss << " ";
      ss << strokeWidth(width);
      ss << "/>";
      return ss.str();
    }
    std::string circle(glm::vec2 const&pos,float r,float w,glm::vec3 const&sColor,glm::vec3 const&fColor){
      std::stringstream ss;
      ss << "<circle ";
      ss << attribf("cx",pos.x);
      ss << " ";
      ss << attribf("cy",pos.y);
      ss << " ";
      ss << attribf("r",r);
      ss << " ";
      ss << stroke(sColor);
      ss << " ";
      ss << strokeWidth(w);
      ss << " ";
      ss << fill(fColor);
      ss << "/>";
      return ss.str();
    }

    void save(std::string const&n){
      std::ofstream ss;
      ss.open(n);//, std::ios_base::app);
      glm::vec2 mmin = glm::vec2(+1e10);
      glm::vec2 mmax = glm::vec2(-1e10);
      for(auto const&e:elements){
        mmin = glm::min(mmin,e->getMin());
        mmax = glm::max(mmax,e->getMax());
      }
      //auto cen = (mmax + mmin)/2.f;
      //mmax = (mmax-cen)*2.f + cen;
      //mmin = (mmin-cen)*2.f + cen;
      size = mmax - mmin;

      ss << headerBegin(size);
      for(auto const&e:elements){
        if(e->type == SVGElement::LINE){
          auto l = e->toLine();
          ss << line(l.a-mmin,l.b-mmin,l.color,l.width) << std::endl;
        }
        if(e->type == SVGElement::CIRCLE){
          auto c = e->toCircle();
          ss << circle(c.pos-mmin,c.radius,c.width,c.sColor,c.fColor) << std::endl;
        }
      }
      ss << headerEnd();
    }
    std::vector<std::unique_ptr<SVGElement>>elements;
    void addLine(SVGLine const&l){
      elements.emplace_back(std::make_unique<SVGLine>(l));
    }
    void addCircle(SVGCircle const&c){
      elements.emplace_back(std::make_unique<SVGCircle>(c));
    }
};
