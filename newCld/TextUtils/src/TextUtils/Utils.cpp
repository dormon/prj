#include<fstream>

#include<TextUtils/Utils.h>

/**
 * This function splits string using splitter string
 * @param txt string that will be splitted
 * @param splitter splitter string
 *
 * @return list of substring, for string "a,b,c" and splitter "," ["a","b","c"]
 */
std::vector<std::string>splitString(
    std::string const&txt     ,
    std::string const&splitter){
  std::vector<std::string>result;
  size_t endPosition  = 0;
  size_t lastPosition = 0;
  size_t const splitterSize = splitter.size();
  while((endPosition = txt.find(splitter,lastPosition)) != std::string::npos){
    result.push_back(txt.substr(lastPosition,endPosition - lastPosition));
    lastPosition = endPosition + splitterSize;
  }
  result.push_back(txt.substr(lastPosition,txt.size() - lastPosition));
  return result;
}

/**
 * @brief This function removes white spaces from string
 *
 * @param txt input string
 *
 * @return string without with spaces
 */
std::string removeSpaces(std::string const&txt){
  std::string result = "";
  for(auto const&x:txt)
    if(!std::isspace(x))result += x;
  return result;
}

/**
 * @brief This function loads text file.
 *
 * @param fileName file name
 *
 * @return content of the file or empty string
 */
std::string loadTextFile(std::string const&fileName){
  std::ifstream f(fileName.c_str());
  if(!f.is_open())
    return "";
  std::string str((std::istreambuf_iterator<char>(f)),std::istreambuf_iterator<char>());
  f.close();
  return str;
}

