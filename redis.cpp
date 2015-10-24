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
            // w->write_int(boost::get<RedisError>(value));
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
            w->
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
            
            break;
        case '-':

            break;
        case '$':

            break;
        case '*':

            break;
        default:
            throw std::runtime_error("invalid redis value");
            break;
    }
}
