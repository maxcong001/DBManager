# DBManager
DBmanager. Now only for redis

It is pretty easy to use. Just include the header file and new a instance.

```

#include "DBM.hpp"

int main()
{
    set_log_level(logger_iface::log_level::debug);

    RedisConnInfo info = RedisConnInfo("127.0.0.1", "6380", "");

    auto dbm = new RedisManager();

    int index = dbm->add(&info);
    if (!index)
    {
        __LOG(error, "add return fail!");
    }

    // do some work
    sleep(3);

    dbm->del(&info);

    delete(dbm);
    dbm = NULL;

}
```