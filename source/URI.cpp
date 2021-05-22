#include "URI.hpp"

//URI = scheme ":" hier-part [ "?" query ] [ "#" fragment ]

URI::URI(const std::string &uri) : uri(uri), position(0) {

}

const std::string &URI::getUri() const {
    return uri;
}

bool URI::validate() {
    std::string::size_type start_pos = position;
    if (!scheme())
        return false;
    reset(start_pos);
    return true;
}

char URI::getNextChar() {
    if (position >= uri.size())
        return -1;
    return uri[position++];
}

char URI::peekNextChar() {
    if (position >= uri.size())
        return -1;
    return uri[position];
}

bool URI::scheme() {
    for (int i = 0; i < 4; ++i) {
        if (getNextChar() != "http"[i]) {
            return false;
        }
    }
    if (peekNextChar() == 's') {
        getNextChar();
    }
    return true;
}

void URI::reset(std::string::size_type pos) {
    position = pos;
}

bool URI::is_reserved(char c) {
    return is_gen_delim(c) || is_sub_delim(c);
}

bool URI::is_gen_delim(char c) {
    return c == ':' || c == '/' || c == '?' || c == '#' ||
           c == '[' || c == ']' || c == '@';
}

bool URI::is_sub_delim(char c) {
    return c == '!' || c == '$' || c == '&' || c == '\'' || c == '(' ||
           c == ')' || c == '*' || c == '+' || c == ',' || c == ';' || c == '=';
}

bool URI::is_unreserved(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') || c == '-' || c == '.' || c == '_' || c == '~';
}

std::string URI::get_encoded(char c) {
    std::string encoded("%");
    encoded += "0123456789ABCDEF"[(c >> 4) % 0x10];
    encoded += "0123456789ABCDEF"[c % 0x10];
    return encoded;
}

char URI::get_decoded(const std::string &c) {
    return 0;
}

std::string URI::encode(const std::string &uri) {
    std::string::size_type size = uri.size();
    std::string encoded_uri;
    int num_slashes = 0;
    int num_questions = uri[0] == '?';
    for (std::string::size_type i = 0; i < size; ++i) {
        if (is_unreserved(uri[i])) {
            encoded_uri += uri[i];
        }
        else if (is_reserved(uri[i])) {
            if ((num_slashes > 2 && (uri[i] != '/' || num_questions > 0)) ||
                (num_questions != 0 && uri[i] == '?')) {
                encoded_uri += get_encoded(uri[i]);
            }
            else {
                encoded_uri += uri[i];
            }
            if (uri[i] == '/') {
                ++num_slashes;
            }
            if (uri[i] == '?')
                ++num_questions;
        }
        else {
            if (uri[i] == ' ')
                encoded_uri += '+';
            else
                encoded_uri += get_encoded(uri[i]);
        }
    }
    return encoded_uri;
}

std::string URI::decode(const std::string &encoded_uri) {
    return std::string();
}
