#pragma once

#include <string>
#include "../logic/redis.h"


class Command {
public:
    virtual ~Command() {};
    virtual std::string name() = 0;
    virtual RedisValue exec(RedisValue args) = 0;
};


class Set : public Command {
public:
    Set(std::unordered_map<int, int> * table);
    std::string name();
    RedisValue exec(RedisValue args);
};


class Get : public Command {
public:
    Get(std::unordered_map<int, int> * table);
    std::string name();
    RedisValue exec(RedisValue args);
};