//
// Created by Pamula File on 5/8/21.
//

#ifndef T_CONFIG_HPP
#define T_CONFIG_HPP
#include <string>
#include <list>

class t_config {
public:
    t_config(std::string  const & host, unsigned int port,
             const std::list<std::string>& error_pages, long int client_body_size = 1);
    ~t_config();
    t_config( void );
    t_config( t_config const &src );

    t_config &operator=( t_config const &src );
    std::string     get_host();
    void            set_host(std::string  const & host);
    unsigned int    get_port();
    void            set_port(unsigned int port);
    std::string     get_server_name();
    void            set_server_name(std::string  const & server_name);

protected:
private:
    std::string             _host;
    unsigned int            _port;
    std::string             _server_name;
    std::list<std::string>  _error_page;
    long int                _client_body_size;
    std::list<route>        _routs
};


#endif //T_CONFIG_HPP
