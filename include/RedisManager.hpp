#pragma once
#include "util.hpp"
#include "RedisManager.hpp"
#include "DBManager.hpp"
#include "RedisDB.hpp"

class RedisManager : public DBManager, public std::enable_shared_from_this<RedisManager>
{
  public:
    //   class RedisDB;
    //  class DBManager;
    typedef std::shared_ptr<RedisManager> ptr_t;
    virtual ~RedisManager();

    virtual int add(void *connect_info) override;
    virtual int del(void *connect_info) override;
    virtual int del_all() override;

    inline static ptr_t create()
    {
        return ptr_t(new RedisManager());
    }
    RedisManager() { uniqueID_atomic = 221; }

  protected:
  private:
    std::unordered_map<int, RedisDB::ptr_t> RedisPool;
    int getUniqueID() { return (uniqueID_atomic ++); }
    std::atomic<int> uniqueID_atomic;
};