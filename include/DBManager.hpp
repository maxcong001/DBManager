#pragma once
#include "util.hpp"
class DBManager 
{
  public:
    typedef std::shared_ptr<DBManager> ptr_t;
    virtual ~DBManager();

    virtual int add(void *connect_info) { return false; };
    virtual int del(void *connect_info) { return false; };
    virtual int del_all() { return false; };



  protected:
    //    //DBManager(void *connect_info);
};