#pragma once

#include<iostream>

/**
 * @brief This class represents loader of dynamic libraries
 * It can be used for loading libraries at runtime
 */
class LibraryLoader{
  public:
    /**
     * @brief Constructor
     */
    LibraryLoader(){}
    /**
     * @brief Destructor - it closes every open library
     */
    virtual ~LibraryLoader(){}
    /**
     * @brief it loads library by its name
     *
     * @param name name of library with path
     *
     * @return index of loaded library
     */
    virtual unsigned load(std::string name)=0;
    /**
     * @brief it loads function from library
     *
     * @param i index of loaded library that is returned by load(name)
     * @param name name of function inside loaded library
     *
     * @return funtion pointer
     */
    virtual void* get(unsigned i,std::string name)=0;
};

/**
 * @brief This function is factory of LibraryLoader
 * it hides implementation details and os dependency as well
 *
 * @return LibraryLoader
 */
LibraryLoader*getLoader();
