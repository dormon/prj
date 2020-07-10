#include <Project.hpp>
#include <saveTxtFile.hpp>
#include <loadTxtFile.hpp>

#include <json.hpp>
using namespace nlohmann;

void Project::load(std::string const&file){
  auto j = json::parse(loadTxtFile(file));
  auto const&js = j.at("streams");
  auto nVideos = js.size();

  auto const loadKeyData = [&](keyframe::Keyframes&s,int frame,json const&j){
    for(auto const&it:j.items()){
      if(it.value().is_number_integer())s.set<int  >(frame,it.key(),it.value().get<int  >());
      if(it.value().is_number_float  ())s.set<float>(frame,it.key(),it.value().get<float>());
      if(it.value().is_array()&&it.value().size() == 4&&it.value().at(0).is_number_float()){
        glm::vec4 v;
        for(int i=0;i<4;++i)
          v[i] = it.value().at(i).get<float>();
        s.set<glm::vec4>(frame,it.key(),v);
      }
    }
  };
  auto const loadKey = [&](keyframe::Keyframes&s,json const&j){
    int frame = j.at("frame");
    loadKeyData(s,frame,j.at("data"));
  };

  auto const loadKeys = [&](keyframe::Keyframes&s,json const&j){
    if(j.count("keys")==0)return;
    for(size_t i=0;i<j.at("keys").size();++i)
      loadKey(s,j.at("keys").at(i));
  };

  auto const loadStartEnd = [&](int&start,int&end,json const&j){
    if(j.count("start"))start = j["start"].get<int>();
    if(j.count("end"  ))end   = j["end"  ].get<int>();
  };

  for(size_t i=0;i<nVideos;++i){
    videoManager.addVideo(js.at(i).at("file"));
    auto&last = videoManager.streams.back();
    loadKeys(last.keys,js.at(i));
    loadStartEnd(last.start,last.end,js.at(i));
  }
}
void Project::save(std::string const&file){
  json j;
  size_t counter=0;
  auto const saveKeyData = [&](json&j,keyframe::Keyframe const&key){
    for(auto const&d:key.data){
      if(d.second.type == keyframe::Data::FLOAT)
        j[d.first] = d.second.value.f32;
      if(d.second.type == keyframe::Data::INT)
        j[d.first] = d.second.value.i32;
      if(d.second.type == keyframe::Data::DOUBLE)
        j[d.first] = d.second.value.f64;
      if(d.second.type == keyframe::Data::VEC4){
        for(int i=0;i<4;++i)
          j[d.first][i] = d.second.value.vec4[i];
      }
    }
  };

  auto const saveKey = [&](json&j,keyframe::Keyframe const&key,int frame){
    j["frame"] = frame;
    saveKeyData(j["data"],key);
  };

  auto const saveKeys = [&](json&j,keyframe::Keyframes const&keys){
    size_t keyCounter = 0;
    for(auto const&k:keys.keyframes){
      saveKey(j["keys"][keyCounter],k.second,k.first);
      keyCounter++;
    }
  };

  auto const saveStartEnd = [&](json&j,int start,int end){
    j["start"] = start;
    j["end"  ] = end  ;
  };

  for(auto const&s:videoManager.streams){
    j["streams"][counter]["file"] = s.file;
    saveKeys    (j["streams"][counter],s.keys);
    saveStartEnd(j["streams"][counter],s.start,s.end);
    counter++;
  }

  saveTxtFile(file,j.dump(2));
}
