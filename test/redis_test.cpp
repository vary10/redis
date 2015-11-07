#include "redis.h"

#include <gtest/gtest.h>

TEST(RedisValue, Construct) {
    RedisValue integer = 10;
    RedisValue string = "abcd";
    RedisValue error = RedisError("Permission denied");
    RedisValue bulk = RedisBulkString("ab\r\ncd\0")
    RedisValue null = RedisNull();
    RedisValue array = std::vector<RedisValue>{integer, string, error, bulk, null}
}

TEST(WriteRedisValue, Int) {
    RedisValue i = 10, j = -5;

    StringWriter writer(1024);
    WriteRedisValue(&writer, i);
    writer.flush();

    EXPECT_STREQ(":10\r\n", writer.result.c_str());

    writer.result.clear();
    WriteRedisValue(&writer, j);
    writer.flush();

    EXPECT_STREQ(":-5\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, String) {
    RedisValue str1 = "abcd", str2 = "1234";

    StringWriter writer(1024);
    WriteRedisValue(&writer, str1);
    writer.flush();

    EXPECT_STREQ("+abcd\r\n", writer.result.c_str());

    writer.result.clear();
    WriteRedisValue(&writer, str2);
    writer.flush();

    EXPECT_STREQ("+1234\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Error) {
    RedisValue err = RedisError("test");

    StringWriter writer(1024);
    WriteRedisValue(&writer, err);
    writer.flush();

    EXPECT_STREQ("-test\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, BulkString) {
    RedisValue bulk = "a\nbcd\0";

    StringWriter writer(1024);
    WriteRedisValue(&writer, bulk);
    writer.flush();

    EXPECT_STREQ("$6\r\na\nbcd\0\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Null) {
    RedisValue nul = RedisNull();

    StringWriter writer(1024);
    WriteRedisValue(&writer, nul);
    writer.flush();

    EXPECT_STREQ("$-1\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Array) {
    RedisValue array = 

    StringWriter writer(1024);
    WriteRedisValue(&writer, array);
    writer.flush();

    EXPECT_STREQ("*2\r\nlkm\r\nmm\r\n", writer.result.c_str());
}

TEST(ReadRedisValue, Int) {
    RedisValue val;

    StringReader reader;

    reader.input = ":10\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(10, boost::get<int64_t>(val));

    reader.input = ":-5\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(-5, boost::get<int64_t>(val));
}


TEST(ReadRedisValue, Int) {
    RedisValue val;

    StringReader reader;

    reader.input = ":10\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(10, boost::get<int64_t>(val));

    reader.input = ":-5\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(-5, boost::get<int64_t>(val));
}
