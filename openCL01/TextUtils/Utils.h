#pragma once

#include<iostream>
#include<vector>

#include<TextUtils/textutils_export.h>

TEXTUTILS_EXPORT std::vector<std::string>splitString(
    std::string const&txt     ,
    std::string const&splitter);

TEXTUTILS_EXPORT std::string removeSpaces(std::string const&txt);

TEXTUTILS_EXPORT std::string loadTextFile(std::string const&fileName);
