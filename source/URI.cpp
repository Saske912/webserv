#include "URI.hpp"

URI::URI(const std::string &uri) : uri(uri), position(0) {

}

const std::string &URI::getUri() const {
    return uri;
}

//   URI           = scheme ":" hier-part [ "?" query ] [ "#" fragment ]
//   query         = *( pchar / "/" / "?" )
//   fragment      = *( pchar / "/" / "?" )
//
// example uri:
//    "http://example.com/some/path/?arg=val&oarg=oval#main",
//    "http://127.0.0.1/some/path/?arg=val&oarg=oval%9f#main",
//    "http://[vf.ads]/some/path/?arg=val&oarg=oval#main"
bool URI::validate() {
    std::string::size_type start_pos = position;
    bool valid = true;
    if ((!scheme()) ||
        (getNextChar() != ':') ||
        (!hier_part())) {
        valid = false;
    }
    if (getNextCharIf('?')) {
        while (peekNextChar() == '/' || peekNextChar() == '?' || pchar()) {
            getNextChar();
        }
    }
    if (getNextCharIf('#')) {
        while (peekNextChar() == '/' || peekNextChar() == '?' || pchar()) {
            getNextChar();
        }
    }
    if (position != uri.size()) {
        valid = false;
    }
    reset(start_pos);
    return valid;
}

char URI::getNextChar() {
    if (position >= uri.size())
        return -1;
    return uri[position++];
}

char URI::getNextCharIf(char c) {
    if (peekNextChar() == c)
        return getNextChar();
    return 0;
}

char URI::peekNextChar() {
    if (position >= uri.size())
        return -1;
    return uri[position];
}

//   pchar         = unreserved / pct-encoded / sub-delims / ":" / "@"
bool URI::pchar() {
    char next = peekNextChar();
    return is_unreserved(next) || pct_encoded() ||
           is_sub_delim(next) || next == ':' || next == '@';
}

//   scheme        = ALPHA *( ALPHA / DIGIT / "+" / "-" / "." )
// accept: http https
bool URI::scheme() {
    for (int i = 0; i < 4; ++i) {
        if (!getNextCharIf("http"[i])) {
            return false;
        }
    }
    getNextCharIf('s');
    return true;
}

//   hier-part     = "//" authority path-abempty
//                 / path-absolute
//                 / path-rootless
//                 / path-empty
bool URI::hier_part() {
    if (getNextCharIf('/')) {
        if (getNextCharIf('/')) {
            if (!authority()) {
                return false;
            }
            path_abempty();
        }
        else if (!path_absolute()) {
            return false;
        }
    }
    else if (!path_rootless()) {
        return false;
    }
    return true;
}

//   authority     = [ userinfo "@" ] host [ ":" port ]
//   userinfo      = *( unreserved / pct-encoded / sub-delims / ":" )
bool URI::authority() {
    std::string::size_type start_pos = position;
    bool valid = true;
    char next = getNextChar();
    while (next > 0 && next != '@') {
        next = getNextChar();
    }
    reset(start_pos);
    if (next == '@') {
        while (is_unreserved(peekNextChar()) || pct_encoded() ||
               is_sub_delim(peekNextChar()) || peekNextChar() == ':') {
            getNextChar();
        }
        if (!getNextCharIf('@'))
            valid = false;
    }
    host();
    if (getNextCharIf(':')) {
        next = peekNextChar();
        while ('0' <= next && next <= '9') {
            getNextChar();
            next = peekNextChar();
        }
    }
    if (!valid)
        reset(start_pos);
    return valid;
}

//   path-abempty  = *( "/" segment )
bool URI::path_abempty() {
    while (getNextCharIf('/')) {
        while (pchar()) {
            getNextChar();
        }
    }
    return true;
}

bool URI::path_absolute() {
    return false;
}

bool URI::path_rootless() {
    return false;
}

//   pct-encoded   = "%" HEXDIG HEXDIG
bool URI::pct_encoded() {
    std::string::size_type start = position;
    bool valid = true;

    if ((!getNextCharIf('%')) ||
        (-1 == get_unhexed(getNextChar())) ||
        (-1 == get_unhexed(peekNextChar())))
        valid = false;

    if (!valid)
        reset(start);
    return valid;
}

//   host          = IP-literal / IPv4address / reg-name
bool URI::host() {
    if (!ip_literal() && !ipv4address())
        reg_name();
    return true;
}

//   reg-name      = *( unreserved / pct-encoded / sub-delims )
bool URI::reg_name() {
    char next = peekNextChar();
    while (is_unreserved(next) || is_sub_delim(next) || pct_encoded()) {
        getNextChar();
        next = peekNextChar();
    }
    return true;
}

//   IPv4address   = dec-octet "." dec-octet "." dec-octet "." dec-octet
bool URI::ipv4address() {
    std::string::size_type start = position;
    bool valid = true;
    for (int i = 0; i < 4; ++i) {
        if (!dec_octet() || (i < 3 && !getNextCharIf('.'))) {
            valid = false;
            break;
        }
    }
    if (!valid)
        reset(start);
    return valid;
}

//   dec-octet     = DIGIT                 ; 0-9
//                 / %x31-39 DIGIT         ; 10-99
//                 / "1" 2DIGIT            ; 100-199
//                 / "2" %x30-34 DIGIT     ; 200-249
//                 / "25" %x30-35          ; 250-255
bool URI::dec_octet() {
    std::string::size_type start = position;
    bool valid = true;
    char digits[3];
    int i = 0;
    for (; i < 3 && '0' <= peekNextChar() && peekNextChar() <= '9'; ++i)
        digits[i] = getNextChar();
    if (('0' <= peekNextChar() && peekNextChar() <= '9') ||
        (i == 3 && digits[0] > '2') ||
        (i == 3 && digits[0] == '2' && digits[1] > '5') ||
        (i == 3 && digits[0] == '2' && digits[1] == '5' && digits[2] > '5') ||
        (i > 1 && digits[0] == '0'))
        valid = false;
    if (!valid)
        reset(start);
    return valid;
}

//   IP-literal    = "[" ( IPv6address / IPvFuture  ) "]"
bool URI::ip_literal() {
    std::string::size_type start = position;
    bool valid = true;
    if (!getNextCharIf('['))
        valid = false;
    if (!ipvfuture() && !ipv6address()) {
        valid = false;
    }
    if (!getNextCharIf(']'))
        valid = false;
    if (!valid)
        reset(start);
    return valid;
}

//   IPv6address   =                            6( h16 ":" ) ls32
//                 /                       "::" 5( h16 ":" ) ls32
//                 / [               h16 ] "::" 4( h16 ":" ) ls32
//                 / [ *1( h16 ":" ) h16 ] "::" 3( h16 ":" ) ls32
//                 / [ *2( h16 ":" ) h16 ] "::" 2( h16 ":" ) ls32
//                 / [ *3( h16 ":" ) h16 ] "::"    h16 ":"   ls32
//                 / [ *4( h16 ":" ) h16 ] "::"              ls32
//                 / [ *5( h16 ":" ) h16 ] "::"              h16
//                 / [ *6( h16 ":" ) h16 ] "::"
bool URI::ipv6address() {
    return false;
}

//   IPvFuture     = "v" 1*HEXDIG "." 1*( unreserved / sub-delims / ":" )
bool URI::ipvfuture() {
    std::string::size_type start = position;
    bool valid = true;
    if (peekNextChar() != 'v')
        valid = false;
    getNextChar();
    if (-1 == get_unhexed(getNextChar()))
        valid = false;
    while (get_unhexed(peekNextChar()) != -1) {
        getNextChar();
    }
    if (!getNextCharIf('.'))
        valid = false;
    char next = peekNextChar();
    if (!(is_unreserved(next) || is_sub_delim(next) || next == ':'))
        valid = false;
    while (is_unreserved(next) || is_sub_delim(next) || next == ':') {
        getNextChar();
        next = peekNextChar();
    }
    if (!valid)
        reset(start);
    return valid;
}

void URI::reset(std::string::size_type pos) {
    position = pos;
}

bool URI::is_reserved(char c) {
    return is_gen_delim(c) || is_sub_delim(c);
}

bool URI::is_gen_delim(char c) {
    const char symbols[] = ":/?#[]@";
    return std::find(std::begin(symbols), std::end(symbols), c) != std::end(symbols);
}

bool URI::is_sub_delim(char c) {
    const char symbols[] = "!$&'()*+,;=";
    return std::find(std::begin(symbols), std::end(symbols), c) != std::end(symbols);
}

bool URI::is_unreserved(char c) {
    const char symbols[] = "-._~";
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') ||
           ('0' <= c && c <= '9') ||
           std::find(std::begin(symbols), std::end(symbols), c) != std::end(symbols);
}

int URI::get_unhexed(char c) {
    const char symbols[] = "0123456789ABCDEF";
    if ('a' <= c && c <= 'f')
        c += 'A' - 'a';
    const char *pos = std::find(std::begin(symbols), std::end(symbols), c);
    if (pos == std::end(symbols))
        return -1;
    return static_cast<int>(pos - std::begin(symbols));
}

std::string URI::get_encoded(char c) {
    std::string encoded("%");
    encoded += "0123456789ABCDEF"[(c >> 4) % 0x10];
    encoded += "0123456789ABCDEF"[c % 0x10];
    return encoded;
}

char URI::get_decoded(const std::string &c) {
    return c[0];
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
    std::string uri;
    for (std::string::const_iterator it = encoded_uri.begin();
        it != encoded_uri.end(); ++it) {
        if (*it == '%') {
            ++it;
            char code = 0;
            if (*it == '%')
                code = '%';
            else if (get_unhexed(*it) != -1) {
                code = get_unhexed(*it);
                ++it;
                if (get_unhexed(*it) != -1) {
                    code = code * 16 + get_unhexed(*it);
                }
                else
                    --it;
            }
            else
                --it;
            uri += code;
        }
        else if (*it == '+') {
            uri += ' ';
        }
        else {
            uri += *it;
        }
    }
    return uri;
}
