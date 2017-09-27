#pragma once
#include <memory>
#include <unistd.h>
#include <string.h>
#include <unordered_map>
#include <atomic>
#include <iostream>
#include <algorithm>
#include <mutex>
#include "logger.hpp"

#define MAGIC_STRING "MAXX MAGIC!!!"
#define MAGIC_NUM 12345
class RedisConnInfo
{
  public:
    // we need to put the magic number first
    // even this may waste of memory
    int Magic;
    bool AuthReq;
    std::string PassWd;
    std::string ListenIP;
    std::string ListenPort;
    std::string UnixSocket;

  public:
    RedisConnInfo()
    {

        Magic = MAGIC_NUM;
        AuthReq = false;
        // actuclly no need to call clear()
        // just leave it here
        PassWd.clear();
        ListenIP.clear();
        ListenPort.clear();
        UnixSocket.clear();
    }

    RedisConnInfo(std::string ip, std::string port, std::string pw = "", std::string Unix_socket_path = "")
    {

        Magic = MAGIC_NUM;
        AuthReq = !pw.empty();
        // actuclly no need to call clear()
        // just leave it here
        PassWd = pw;
        ListenIP = ip;
        ListenPort = port;
        UnixSocket = Unix_socket_path;
    }


    bool operator==(const RedisConnInfo &rhs)
    {
        return Magic == rhs.Magic && AuthReq == rhs.AuthReq && !((PassWd).compare(rhs.PassWd)) && !((ListenIP).compare(rhs.ListenIP)) && !((ListenPort).compare(rhs.ListenPort));
    }

    bool IsValid()
    {
        //return (Magic.compare(MAGIC_STRING) == 0 ? true : false);

        return (Magic == MAGIC_NUM ? true : false);
    }
    void dump()
    {
        __LOG(debug, "Magic is " << Magic << " AuthReq is " << AuthReq << " PassWd is " << PassWd << " ListenIP is " << ListenIP << " ListenPort is " << ListenPort);
    }
};

// message types
struct RedisDBCmdType
{
    enum type
    {
        REDIS_START = 0,  /* Start DB */
        REDIS_STOP,       /* Stop DB */
        REDIS_CHECK_PORT, /* Check port is used or not */

        REDIS_DB_CMD_MAX
    };

    static std::string toString(const RedisDBCmdType::type _type)
    {
        return (_type > REDIS_DB_CMD_MAX || _type < REDIS_START) ? "UNDEFINED_TEST_DB_CMD_TYPE" : (const std::string[]){
                                                                                                      "REDIS_START",
                                                                                                      "REDIS_STOP",
                                                                                                      "REDIS_CHECK_PORT",

                                                                                                      "REDIS_DB_CMD_MAX",
                                                                                                  }[_type];
    }
};