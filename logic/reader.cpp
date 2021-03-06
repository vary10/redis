#include "reader.h"

char Reader::read_char() {
    if (rpos_ == end_) read_more();
    return buffer_[rpos_++];
}

std::string Reader::read_string() {
    std::string res;
    char tmp = read_char();
    while (tmp != '\r') {
        res.push_back(tmp);
        tmp = read_char();
    }
    read_char();
    return res;
}

std::string Reader::read_raw(int64_t len) {
    std::string res;
    char tmp = read_char();
    for (int64_t i = 0; i < len; ++i) {
        res.push_back(tmp);
        tmp = read_char();
    }
    read_char();
    return res;
}

int64_t Reader::read_int() {
    int64_t i = 0;
    bool negative = false;

    char first = read_char(), next;
    if (first == '-') {
        negative = true;
        next = read_char();
    } else {
        next = first;
    }

    do {
        i *= 10;
        i += next - '0';

        next = read_char();
    } while(next != '\r');
    read_char(); // skip '\n'

    return negative ? -i : i;
}

void StringReader::read_more() {
    if (input.empty()) throw std::runtime_error("end of input");

    end_ = 0;
    rpos_ = 0;
    for (; end_ < input.size() && end_ < buffer_.size(); ++end_) {
        buffer_[end_] = input[end_];
    }

    input.erase(input.begin(), input.begin() + end_);
}

void SocketReader::read_more() {
    rpos_ = 0;
    end_ = sock.get_data(buffer_.data(), buffer_.size());
}