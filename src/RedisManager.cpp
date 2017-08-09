#include "RedisManager.hpp"

RedisManager::~RedisManager()
{
    if (del_all())
    {
        return;
    }
    else
    {
        __LOG(error, "delete all return fail!!!!!!");
    }
}
// the input should be pointer of RedisConnInfo
// if the return value <=0. that means fail.
// else return the index of the Redis. It will use when delete a Redis
int RedisManager::add(void *connect_info)
{
    if (!connect_info)
    {
        __LOG(error, "connect_info is invalid");
        return false;
    }
    if (!((RedisConnInfo *)connect_info)->IsValid())
    {
        __LOG(error, "RedisConnInfo is invalid, magic number is not right one");
        return false;
    }

    RedisConnInfo ConnInfo = *((RedisConnInfo *)connect_info);
    RedisDB::ptr_t db(new RedisDB(ConnInfo));
    if (db->check_success())
    {
    }
    else
    {
        __LOG(error, "add one Redis DB fail, connection info is :");
        ConnInfo.dump();
        return false;
    }
    int ret = getUniqueID();
    RedisPool.emplace(ret, db);
    return ret;
}

int RedisManager::del(void *connect_info)
{
    if (!connect_info)
    {
        __LOG(error, "connect_info is invalid");
        return false;
    }
    if (!((RedisConnInfo *)connect_info)->IsValid())
    {
        __LOG(error, "RedisConnInfo is invalid, magic number is not right one");
        return false;
    }
    RedisConnInfo ConnInfo = *((RedisConnInfo *)connect_info);
    int index = 0;

    for (auto db : RedisPool)
    {
        if ((db.second)->get_info() == ConnInfo)
        {
            index = db.first;
        }
    }
    if (!index)
    {
        __LOG(warn, "did not find the entry!");
        return false;
    }

    auto db_iter = RedisPool.find(index);
    if (db_iter != RedisPool.end())
    {
        __LOG(debug, "find index " << index << " in the map");
        auto db = db_iter->second;
        RedisPool.erase(index);
        return true;
    }
    else
    {
        __LOG(warn, "did not find index : " << index << " in the map");
        return false;
    }
}

int RedisManager::del_all()
{
    RedisPool.clear();
    return true;
}