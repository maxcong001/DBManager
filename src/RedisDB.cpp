#include "RedisDB.hpp"

bool RedisDB::genConfigFile(std::string &CfgName)
{
    // get PWD

    char processdir[REDIS_PATH_MAX];
    char *path_end;
    ssize_t size;
    size = readlink("/proc/self/exe", processdir, sizeof(processdir));
    if (size < 0)
    {
        __LOG(error, "readlink return fail");
        return false;
    }
    else
    {
        path_end = strrchr(processdir, '/');
        if (path_end == NULL)
        {
            __LOG(error, "strrchr return fail");
            return false;
        }
        ++path_end;
        if (path_end == NULL)
        {
            __LOG(error, "path_end invalid");
            return false;
        }
        *path_end = '\0';
    }

    // gen config file
    FILE *fp;
    std::string config_string;
    // for Auth
    if (info.AuthReq)
    {
        if ((info.PassWd).empty())
        {
            __LOG(debug, "empty passwd");
            // empty passwd
        }
        else
        {
            __LOG(debug, "config with passwd " << info.PassWd);
            config_string = "requirepass  " + (info.PassWd) + "\n";
        }
    }
    // for IP
    if ((info.ListenIP).empty())
    {
        __LOG(debug, "empty IP, use default IP 127.0.0.1");
        config_string += "bind 127.0.0.1\n";
    }
    else
    {
        __LOG(debug, "config with IP " << info.ListenIP);
        config_string += "bind " + (info.ListenIP) + "\n";
    }

    // for port
    if ((info.ListenPort).empty())
    {
        __LOG(debug, "empty port, use default port 6379");
        config_string += "port 6379\n";
    }
    else
    {
        __LOG(debug, "config with port " << info.ListenPort);
        config_string += "port " + (info.ListenPort) + "\n";
    }

    // for unix socket
    if ((info.UnixSocket).empty())
    {
        __LOG(debug, "empty port, use default port 6379");
        config_string += "unixsocket /tmp/redis.sock\n";
    }
    else
    {
        __LOG(debug, "config with port " << info.ListenPort);

        config_string += "unixsocket " + info.UnixSocket + "\n";
    }

    std::string CfgName_tmp = info.PassWd + info.ListenIP + info.ListenPort;
    if (CfgName_tmp.empty())
    {
        // something bad happen here, return "bad"
        CfgName_tmp = "bad.conf";

        CfgName = CfgName_tmp;
        __LOG(error, "something bad happened!");
        return false;
    }

    if ((fp = fopen(CfgName_tmp.c_str(), "w")) == NULL)
    {
        __LOG(error, "fopen return error!");
        return false;
    }
    fputs(config_string.c_str(), fp);
    fclose(fp);
    CfgName = CfgName_tmp;
    __LOG(debug, "final config file is :\n"
                     << "*************************\n"
                     << config_string << "\n"
                     << "*************************\n"
                     << "config name is " << CfgName);
    return true;
}

std::string RedisDB::GenCommand(RedisDBCmdType::type cmdType)
{
    std::string cmd = "", str, cfgName;

    const std::string redisSrvCmd = "redis-server";
    const std::string redisCliCmd = "redis-cli";

    switch (cmdType)
    {
    case RedisDBCmdType::REDIS_START:
        if (genConfigFile(cfgName))
        {
            __LOG(debug, "genConfigFile success");
        }
        else
        {
            __LOG(error, "genConfigFile return fail!");
            return cmd;
        }

        cmd = redisSrvCmd + " " + cfgName + " &" + " > /dev/null";
        __LOG(debug, "generate start cmd, cmd is " << cmd);
        break;
    case RedisDBCmdType::REDIS_STOP:

        cmd = redisCliCmd + " ";
        if (!(info.ListenIP).empty())
        {
            cmd += "-h " + info.ListenIP + " ";
        }
        if (!(info.ListenPort).empty())
        {
            cmd += "-p " + info.ListenPort + " ";
        }

        if (info.AuthReq)
        {
            cmd += "-a " + info.PassWd + " ";
        }
        cmd += " shutdown& > /dev/null;";
        __LOG(debug, "generate shutdown cmd, cmd is " << cmd);
        break;
    case RedisDBCmdType::REDIS_CHECK_PORT:
        // only check port used by redis server,
        // so the port maybe used by other users.

        cmd = "ps -ef | grep -v \"grep\" |grep \"" + redisSrvCmd + " " + info.ListenIP + ":" + info.ListenPort + "\" > /dev/null";
        __LOG(debug, "generate check port cmd, cmd is " << cmd);
        break;

    default:
        __LOG(error, "unsupport type!");
        break;
    }

    return cmd;
}

bool RedisDB::portIsUsed()
{

    std::string cmd = GenCommand(RedisDBCmdType::REDIS_CHECK_PORT);
    if (cmd.empty())
    {
        __LOG(error, "GenCommand fail");
        return false;
    }

    pid_t status = system(cmd.c_str());

    if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
    {
        __LOG(warn, "port " << info.ListenPort << " is used!!!!");
        return true;
    }
    else
    {
        __LOG(debug, "port " << info.ListenPort << " is not used");
        return false;
    }
}

int RedisDB::destroy()
{

    std::string cmd = GenCommand(RedisDBCmdType::REDIS_STOP);
    if (cmd.empty())
    {
        __LOG(error, "GenCommand fail");
        return false;
    }

    pid_t status = system(cmd.c_str());
    if (WIFEXITED(status) && (WEXITSTATUS(status) == 0))
    {
        return true;
    }
    else
    {
        __LOG(error, "exec command " << cmd << " .......return fail");
        return false;
    }
}
int RedisDB::create()
{
    if (!portIsUsed())
    {
        __LOG(debug, "will start Redis will port  " << info.ListenPort);
    }
    else
    {
        __LOG(error, "port is used by other Redis server, please check!!!!!");
        return false;
    }
    std::string cmd = GenCommand(RedisDBCmdType::REDIS_START);
    pid_t status = system(cmd.c_str());
    if (!(WIFEXITED(status) && (WEXITSTATUS(status) == 0)))
    {
        __LOG(error, "exec fail" << cmd);
        return false;
    }
    else
    {
        return true;
    }
}

RedisDB::RedisDB(RedisConnInfo &infoo)
{
    create_success = false;
    info = infoo;
    if (create())
    {
        // success
        // even the return is success, the port may used by other application, So please check before usg.
        // will enhance later, no target day
        create_success = true;
    }
    else
    {
        __LOG(error, "create Redis DB entry fail");
        //fail
    }
}

RedisDB::~RedisDB()
{

    if (destroy())
    {
        __LOG(warn, "delete this Redis DB entry");
        info.dump();
    }
    else
    {
        __LOG(error, "delete this Redis DB fail");
        info.dump();
    }
}