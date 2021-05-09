//
// Created by Pamula File on 5/8/21.
//

#ifndef T_CONFIG_HPP
#define T_CONFIG_HPP
#include <string>
#include <list>
#include <map>
#include "route.hpp"

class t_config {
public:
    t_config(std::string  const & host, unsigned int port, const std::map<int, std::string>& error_pages,
             std::list<route> const & routs, long int client_body_size = 1);
    ~t_config();
    t_config( t_config const &src );
    t_config &operator=( t_config const &src );

    std::string                 get_host() const;
    void                        set_host(std::string  const & host);
    unsigned int                get_port() const;
    void                        set_port(unsigned int port);
    std::list<std::string>      get_server_names() const;
    void                        set_server_name(std::list<std::string>  const & server_name);
    std::map<int, std::string >     get_error_pages() const;
    void                        set_error_pages(std::map<int, std::string> const & err_pages);
    long int                    get_client_body_size() const;
    std::list<route>            get_routes() const;

    std::string                 get_path_to_request(std::string const & request);
protected:
    std::string                 request_processing(std::string const & request, std::string const & def_file);
    t_config( void );
    bool                        is_file(std::string request);
private:
    std::string                 _host;
    unsigned int                _port;
    std::list<std::string>      _server_names;
    std::map<int, std::string>  _error_pages;
    std::list<route>            _routs;
    long int                    _client_body_size;
};


#endif //T_CONFIG_HPP
