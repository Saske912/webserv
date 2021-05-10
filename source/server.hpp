//
// Created by Pamula File on 5/8/21.
//

#ifndef SERVER_HPP
#define SERVER_HPP
#include <string>
#include <list>
#include <map>
#include "route.hpp"
#include "../wpersimm.h"
#include <fcntl.h>

class server {
public:
    server(std::string  const & host, unsigned int port, const std::map<int, std::string>& error_pages,
             std::list<route> const & routs, long int client_body_size = 1);
    ~server();
    server( server const &src );
    server &operator=( server const &src );

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
    char                        *get_error(int);

    int                        responce( Header & head );
protected:

    int                         request_processing(std::string const & request, \
    std::string const & def_file, route const & route, Header & head );
    bool                        is_file(std::string request);
    int                         get_path_to_request(std::string const & request, Header & head );
    std::string                 dirs(std::string request);
    int                         exception_processing(int except, Header &head);
    int                         targeting(Header &head, std::string request);
    server();
private:
    std::string                 _host;
    unsigned int                _port;
    std::list<std::string>      _server_names;
    std::map<int, std::string>  _error_pages;
    std::list<route>            _routs;
    long int                    _client_body_size;
};


#endif //SERVER_HPP
