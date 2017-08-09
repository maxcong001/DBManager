
#include "DBM.hpp"
#include <unistd.h>
int main()
{
    set_log_level(logger_iface::log_level::debug);
    __LOG(error, "hello logger!"
                     << "this is error log");
    __LOG(warn, "hello logger!"
                    << "this is warn log");
    __LOG(info, "hello logger!"
                    << "this is info log");
    __LOG(debug, "hello logger!"
                     << "this is debug log");
    RedisConnInfo info = RedisConnInfo("127.0.0.1", "6380","");
    DBManager *dbm = new RedisManager();
    int index = 0;
    index = dbm->add(&info);
    if (!index)
    {
        __LOG(error, "add return fail!");
    }
    __LOG(debug, "assign index " << index << " for redis DB");
    sleep(10);
    dbm->del(&info);
}
