#pragma once
#include <memory>
#include "DBManager.hpp"
#define MAGIC_STRING "MAXX MAGIC!!!"
#define MAGIC_NUM 12345;
class RedisDB;
struct RedisConnInfo
{
    // we need to put the magic number first
    // even this may waste of memory
    int Magic;
    bool AuthReq;
    std::string PassWd;
    std::string ListenIP;
    std::string ListenPort;
    //    std::string Magic;

  public:
    RedisConnInfo()
    {

        int Magic = MAGIC_NUM;
        AuthReq = false;
        // actuclly no need to call clear()
        // just leave it here
        PassWd.clear();
        ListenIP.clear();
        ListenPort.clear();
        // Magic = MAGIC_STRING;
    }

    bool
    IsValid()
    {
        //return (Magic.compare(MAGIC_STRING) == 0 ? true : false);

        return (Magic == MAGIC_NUM ? true : false);
    }
};

class RedisManager : public DBManager, public std::enable_shared_from_this<RedisManager>
{
  public:
    typedef std::shared_ptr<RedisManager> ptr_t;
    virtual ~RedisManager();

    virtual int add(void *connect_info);
    virtual int del(void *connect_info);
    virtual int del_all();

    inline static ptr_t create(void *connect_info)
    {
        return ptr_t(new RedisManager(connect_info));
    }
    RedisManager() = delete;

  protected:
    RedisManager(void *connect_info);
};