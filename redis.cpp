#include "redis.h"

void WriteRedisValue(Writer* w, const RedisValue& value) {
    switch (value.which()) {
        case REDIS_INT:
            w->write_char(':');
            w->write_int(boost::get<int64_t>(value));
            w->write_crlf();
            break;
        case REDIS_NULL:
            w->write_char('$');
            w->write_int(-1);
            w->write_crlf();
            break;
        case REDIS_STRING:
            w->write_char('+');
            w->write_raw(boost::get<std::string>(value), boost::get<std::string>(value).length());
            w->write_crlf();
            break;
        case REDIS_BULK_STRING:
            w->write_char('$');
            w->write_raw(boost::get<std::vector<char>>(value), boost::get<std::vector<char>>(value).size());
            w->write_crlf();
            break;
        case REDIS_ERROR:
            w->write_char('-');
            w->write_raw(boost::get<RedisError>(value).msg);
            w->write_crlf();
            break;
        case REDIS_ARRAY:
            w->write_char('*');
            w->write_int(boost::get<std::vector<RedisValue>>(value).size());
            w->write_crlf;
            for (auto elem : boost::get<std::vector<RedisValue>>(value)) {
                WriteRedisValue(w, elem);
            }
        default:
            throw std::runtime_error("unsupported type");
            break;
    }
}

void ReadRedisValue(Reader* r, RedisValue* value) {
    switch(r->read_char()) {
        case ':':
            *value = r->read_int();
            break;
        case '+':
            *value = r->read_string();
            break;
        case '-':
            *value = RedisError(r->read_line());
            break;
        case '$':
            int64_t len = r->read_int();
            if (len == -1) {
                *value = RedisNull();
            } else {
                *value = RedisBulkString(r->read_raw(len));
            }
            break;
        case '*':
            int64_t len = r->read_int();
            std::vector<RedisValue> *value(len);
            for (int64_t i = 0; i < len; ++i) {
                ReadRedisValue(r, &(boost::get<std::vector<RedisValue>>(value)));
            }
            break;
        default:
            throw std::runtime_error("invalid redis value");
            break;
    }
}
