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
            w->write_string(boost::get<std::string>(value));
            w->write_crlf();
            break;
        case REDIS_BULK_STRING:
            w->write_char('$');
            w->write_int(boost::get<RedisBulkString>(value).str.size());
            w->write_crlf();
            w->write_string(boost::get<RedisBulkString>(value).str);
            w->write_crlf();
            break;
        case REDIS_ERROR:
            w->write_char('-');
            w->write_string(boost::get<RedisError>(value).msg);
            w->write_crlf();
            break;
        case REDIS_ARRAY:
            if (boost::get<std::vector<RedisValue>>(value).size() > 1000) {
                throw std::invalid_argument("array is too long");
            }
            w->write_char('*');
            w->write_int(boost::get<std::vector<RedisValue>>(value).size());
            w->write_crlf();
            for (auto elem : boost::get<std::vector<RedisValue>>(value)) {
                WriteRedisValue(w, elem);
            }
            break;
        default:
            throw std::runtime_error("unsupported type");
    }
}

void ReadRedisValue(Reader* r, RedisValue* value) {
    int64_t len = 0;
    switch(r->read_char()) {
        case ':':
            *value = r->read_int();
            break;
        case '+':
            *value = r->read_string();
            break;
        case '-':
            *value = RedisError(r->read_string());
            break;
        case '$':
            len = r->read_int();
            if (len == -1) {
                *value = RedisNull();
            }
            else {
                *value = RedisBulkString(r->read_raw(len));
            }
            break;
        case '*':
            len = r->read_int();
            *value = std::vector<RedisValue>(len);
            for (int64_t i = 0; i < len; ++i) {
                ReadRedisValue(r, &(boost::get<std::vector<RedisValue>>(*value)[i]));
            }
            break;
        default:
            throw std::runtime_error("invalid redis value");
    }
}
