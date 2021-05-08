//
// Created by Pamula File on 5/8/21.
//

#ifndef ROUTE_HPP
#define ROUTE_HPP
#include <list>
#include <string>

class route {
public:
    route(std::string const & name, std::string const & path) : _name(name), _path(path) {};

    ~route( void );

    route( route const &src );

    route &operator=( route const &src );
    bool            check_name(std::string const & request);
    std::string     swap_path(std::string const & request);
    bool                    _autoindex;
private:
    route( );
    std::string             _name;
    std::string             _path;
    std::list<std::string>  _http_methods;
    std::string             _default_page;
};


#endif //ROUTE_HPP
