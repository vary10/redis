#include "redis.h"

#include <gtest/gtest.h>

TEST(RedisValue, Construct) {
    RedisValue integer = 10;
    RedisValue string = "abcd";
    RedisValue error = RedisError("Permission denied");
    RedisValue bulk = RedisBulkString("a");
    RedisValue null = RedisNull();
    RedisValue array = std::vector<RedisValue>{integer, string, error, bulk, null};
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
    RedisValue str1 = "abcda", str2 = "1234";

    StringWriter writer(1024);
    WriteRedisValue(&writer, str1);
    writer.flush();

    EXPECT_STREQ("+abcda\r\n", writer.result.c_str());

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
    RedisValue bulk = RedisBulkString("a\nbcd");

    StringWriter writer(1024);
    WriteRedisValue(&writer, bulk);
    writer.flush();

    EXPECT_STREQ("$5\r\na\nbcd\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Null) {
    RedisValue nul = RedisNull();

    StringWriter writer(1024);
    WriteRedisValue(&writer, nul);
    writer.flush();

    EXPECT_STREQ("$-1\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Array) {
    RedisValue integer = -12;
    RedisValue string = "abcd";
    RedisValue error = RedisError("nope");
    RedisValue null = RedisNull();

    RedisValue array = std::vector<RedisValue>{integer, string, error, null};

    StringWriter write(1024);
    WriteRedisValue(&write, array);
    write.flush();

    EXPECT_STREQ("*4\r\n:-12\r\n+abcd\r\n-nope\r\n$-1\r\n", write.result.c_str());
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

TEST(ReadRedisValue, String) {
    RedisValue str;

    StringReader reader;

    reader.input = "+abcd\r\n";
    ReadRedisValue(&reader, &str);
    EXPECT_EQ("abcd", boost::get<std::string>(str));
}

TEST(ReadRedisValue, Error) {
    RedisValue err;

    StringReader reader;

    reader.input = "-kek\r\n";
    ReadRedisValue(&reader, &err);
    EXPECT_EQ("kek", boost::get<RedisError>(err).msg);
}

TEST(ReadRedisValue, BulkString) {
    RedisValue bulk;

    StringReader reader;

    reader.input = "$5\r\na\nbcd\r\n";
    ReadRedisValue(&reader, &bulk);

    EXPECT_EQ("a\nbcd", boost::get<RedisBulkString>(bulk).str);
}

TEST(ReadRedisValue, Null) {
    RedisValue null;

    StringReader reader;

    reader.input = "$-1\r\n";
    ReadRedisValue(&reader, &null);
    EXPECT_EQ(REDIS_NULL, null.which());
}

TEST(ReadRedisValue, Array) {
    RedisValue array;

    StringReader reader;

    reader.input = "*4\r\n:-12\r\n+abcd\r\n-nope\r\n$-1\r\n";
    ReadRedisValue(&reader, &array);
    EXPECT_EQ(-12, boost::get<int64_t>(boost::get<std::vector<RedisValue>>(array)[0]));
    EXPECT_STREQ("abcd", boost::get<std::string>(boost::get<std::vector<RedisValue>>(array)[1]).c_str());
    EXPECT_STREQ("nope", boost::get<RedisError>(boost::get<std::vector<RedisValue>>(array)[2]).msg.c_str());
    EXPECT_EQ(REDIS_NULL, boost::get<std::vector<RedisValue>>(array)[3].which());
}

