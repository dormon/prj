/**
 * @file 
 * @brief This file contains declarations for N dimensional index.
 *
 * @author Tomáš Milet, imilet@fit.vutbr.cz
 */

#pragma once

#include<vector>
#include<memory>

class NIndexImpl;
class NIndex{
  public:
    NIndex(std::vector<size_t>const&sizes);
    void zero();
    bool add(size_t value,std::vector<size_t>const&ordering = {});
    bool sub(size_t value,std::vector<size_t>const&ordering = {});
    bool inc(std::vector<size_t>const&ordering = {});
    bool dec(std::vector<size_t>const&ordering = {});
    size_t getDimension()const;
    size_t getPart(size_t axis)const;
    size_t getSize(size_t axis)const;
    size_t&getPart(size_t axis);
    size_t&getSize(size_t axis);
    size_t getSize()const;
    size_t get1DIndex(std::vector<bool>const&axisFlip = {})const;
    std::vector<size_t>const&getParts()const;
  protected:
    friend class NIndexImpl;
    std::shared_ptr<NIndexImpl>pimpl;
};

