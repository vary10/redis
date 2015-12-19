#pragma once

#include <string>
#include "../logic/redis.h"


class Request {
public:
    Reguest();
    ~Request();
    RedisValue set();
    RedisValue get();
};
