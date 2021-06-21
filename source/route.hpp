//
// Created by Pamula File on 5/8/21.
//

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>
#include <list>
#include <string>

class route {
public:
    typedef std::pair<std::string, std::string> CgiType;

    route(std::string const &name, std::string const &root, const std::list<std::string> &http_methods,
          bool autoindex = false, std::string const &def_page = "index.html");
    ~route(void);
    route(route const &src);
    route &operator=(route const &src);

    bool check_name(std::string request);
    std::string swap_path( std::string request);
    void autoindex_on();
    void autoindex_off();

    std::string get_default_page() const;
    void set_default_page(std::string const &page);
    bool get_autoindex() const;
    const std::list<std::string> &get_http_methods() const;
    void set_http_methods(std::list<std::string> const &http_methods);
    std::string const &get_name() const;
    std::string const &get_root() const;
    std::string get_upload_location() const;
    long int get_client_max_body_size() const;
    void set_upload_location(const std::string &location);
    const CgiType &get_cgi() const;
    void set_cgi(CgiType const &cgi_);
    void set_client_body_size(long int newSize);
protected:
    route();
private:
    std::string            _name;
    std::string            _root;
    std::list<std::string> _http_methods;
    std::string            _default_page;
    CgiType                _cgi; // first - extension, second - extension
    std::string            _upload_location;
    bool                   _autoindex;
    long int               _client_body_size;
};

std::ostream &operator<<(std::ostream &o, const route &route_);

#endif //ROUTE_HPP
