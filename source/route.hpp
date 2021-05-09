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
    void            autoindex_on();
    void            autoindex_off();

    std::string             get_default_page() const;
    void                    set_default_page(std::string const & page);
    bool                    get_autoindex() const;
    std::list<std::string>  get_http_methods() const;
    void                    set_http_methods(std::list<std::string> const & http_methods);
    std::string const &     get_name() const;
    std::string const &     get_root() const;
protected:
    route( );
private:
    std::string             _name;
    std::string             _root;
    std::list<std::string>  _http_methods;
    std::string             _default_page;
    std::pair<std::string, std::string>     _sgi; // first - extension, second - filename
    bool                    _autoindex;
};


#endif //ROUTE_HPP
