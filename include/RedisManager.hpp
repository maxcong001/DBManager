#pragma once
#include <memory>
#include "DBManager.hpp"
#define MAGIC_STRING "MAXX MAGIC!!!"
class RedisDB;
struct RedisConnInfo
{
    bool AuthReq;
    std::string Passwd;
    std::string ListenIP;
    std::string ListenPort;
    std::string Magic;

  public:
    RedisConnInfo()
    {
        AuthReq = false;
        // actuclly no need to call clear()
        // just leave it here
        Passwd.clear();
        ListenIP.clear();
        ListenPort.clear();
        Magic = MAGIC_STRING;
    }

    bool
    IsValid()
    {
        return (Magic.compare(MAGIC_STRING) == 0 ? true : false);
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