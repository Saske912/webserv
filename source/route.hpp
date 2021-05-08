//
// Created by Pamula File on 5/8/21.
//

#ifndef ROUTE_HPP
#define ROUTE_HPP
#include <list>
#include <string>

class route {
public:
    route( void );

    ~route( void );

    route( route const &src );

    route &operator=( route const &src );
    bool            check_name(std::string const & request);
    std::string     swap_path(std::string const & request);

private:
    std::pair<std::string const & , std::string const & >   _name_path;
    std::list<std::string>                                  _http_methods;
    bool                                                    _autoindex;
    std::string                                             _default_page;
};


#endif //ROUTE_HPP
