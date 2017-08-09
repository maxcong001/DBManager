#pragma once
#include "util.hpp"
#define REDIS_PATH_MAX 4096
class RedisDB : public std::enable_shared_from_this<RedisDB>
{
  public:
    typedef std::shared_ptr<RedisDB> ptr_t;
    virtual ~RedisDB();

    virtual int destroy();
    virtual int create();
    bool check_success() { return create_success; }

    RedisDB() = delete;
    RedisDB(RedisConnInfo &infoo);
    RedisConnInfo get_info()
    {
        return info;
    }

  protected:
  private:
    virtual bool
    genConfigFile(std::string &CfgName);
    virtual std::string GenCommand(RedisDBCmdType::type cmdType);
    bool portIsUsed();

    RedisConnInfo info;
    bool create_success;
};