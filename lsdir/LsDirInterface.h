#pragma once

#include<iostream>
#include<vector>

class LsDir{
  protected:
    std::string             _currentDir;
    std::vector<std::string>_dirFiles  ;
    std::vector<std::string>_dirDirs   ;
    virtual void _getDirContent()=0;
  public:
    LsDir(std::string dir);
    virtual ~LsDir();
    virtual std::string getFile      (unsigned    i  );
    virtual unsigned    getNofFiles  (               );
    virtual std::string getDir       (unsigned    i  );
    virtual unsigned    getNofDirs   (               );
    virtual void        cd           (std::string dir);
    virtual std::string getCurrentDir(               );
};
