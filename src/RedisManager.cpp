#include "RedisManager.hpp"

RedisManager::~RedisManager()
{
    if (del_all())
    {
        return;
    }
    else
    {
        std::cout << "delete all return fail!" << std::endl;
    }
}