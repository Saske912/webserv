#ifndef URI_HPP
#define URI_HPP

#include <iostream>

class URI {
    const std::string      &uri;
    std::string::size_type position;

    char getNextChar();

    char getNextCharIf(char c);

    char peekNextChar();

    bool pchar();

    bool scheme();

    bool hier_part();

    bool host();

    bool ip_literal();

    bool reg_name();

    bool ipv4address();

    bool dec_octet();

    bool ipv6address();

    bool ipvfuture();

    bool authority();

    bool path_abempty();

    bool path_absolute();

    bool path_rootless();

    bool pct_encoded();

    void reset(std::string::size_type pos = 0);

    static bool is_reserved(char c);

    static bool is_gen_delim(char c);

    static bool is_sub_delim(char c);

    static bool is_unreserved(char c);

    static int get_unhexed(char c);

    static std::string get_encoded(char c);

    static char get_decoded(const std::string &c);

public:

    explicit URI(const std::string &uri);

    const std::string &getUri() const;

    bool validate();

    static std::string encode(const std::string &uri);

    static std::string decode(const std::string &encoded_uri);
};

#endif
