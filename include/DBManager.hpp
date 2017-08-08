#pragma once
#include <memory>
class DBManager : public std::enable_shared_from_this<DBManager>
{
  public:
    typedef std::shared_ptr<DBManager> ptr_t;
    virtual ~DBManager(void);

    virtual int add(void *connect_info) {};
    virtual int del(void *connect_info) {};
    virtual int del_all() {};

    inline static ptr_t create(void *connect_info)
    {
        return ptr_t(new DBManager(connect_info));
    }

  protected:
    DBManager(void *connect_info);
}