#pragma once

#include"LibraryLoader.h"

/**
 * @brief This class loads object that is created by factory function getObject inside
 * dynamic library
 * It can be used for easy runtime creation of new object from dynamic library
 */
class ObjectLoader: public LibraryLoader{
  public:
    /**
     * @brief This function returns object that is created by factory function getObject inside lib
     * dynamic library
     *
     * @param lib name of library
     *
     * @return created object
     */
    void*getObject(std::string lib);
};

