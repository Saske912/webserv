//
// Created by Pamula File on 5/8/21.
//

#ifndef ROUTE_HPP
#define ROUTE_HPP
#include <list>
#include <string>

class route {
public:
    route(std::string const & name, std::string const & root, const std::list<std::string>& http_methods,
          bool autoindex = false, std::string const & def_page = "index.html");

    ~route( void );

    route( route const &src );

    route &operator=( route const &src );
    bool            check_name(std::string const & request);
    std::string     swap_path(std::string const & request);
    std::string     get_default_page() const;
    void            set_default_page(std::string const & page);
    void            autoindex_on();
    void            autoindex_off();
    bool            get_autoindex() const;
private:
    route( );
    std::string             _name;
    std::string             _root;
    std::list<std::string>  _http_methods;
    std::string             _default_page;
    bool                    _autoindex;
};


#endif //ROUTE_HPP
