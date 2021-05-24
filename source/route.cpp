//
// Created by Pamula File on 5/8/21.
//

#include "header.h"
#include "route.hpp"

route::route( const std::string &name, const std::string &root, const std::list<std::string> &http_methods,
              bool autoindex, const std::string &def_page )
        : _name(name), _root(root), _http_methods(http_methods), _default_page(def_page), _autoindex(autoindex) {
}

route::route( void ) { }

route::~route( void ) { }

route::route( route const &src ) {
    this->_root = src._root;
    this->_name = src._name;
    this->_http_methods = src._http_methods;
    this->_autoindex = src._autoindex;
    this->_default_page = src._default_page;
    this->_upload_location = src._upload_location;
    this->_cgi = src._cgi;
}

route &route::operator=( route const &src ) {
    this->_root = src._root;
    this->_name = src._name;
    this->_http_methods = src._http_methods;
    this->_autoindex = src._autoindex;
    this->_default_page = src._default_page;
    this->_upload_location = src._upload_location;
    this->_cgi = src._cgi;
    return *this;
}

bool route::check_name( std::string request ) {
    std::string tmp = request.substr(0, request.find('/', 2));
    tmp = tmp.empty() ? request : tmp;
//    std::cerr << "tmp " << tmp << " _name " << _name  << std::endl;
    return (strcmp(trim(_name, "/").c_str(), trim(tmp, "/").c_str()));
}

std::string route::swap_path( const std::string &request ) {
    std::string tmp;
    if (_name.length() < request.length())
        tmp = std::string(request, _name.length(), request.length() - _name.length());
    else
        tmp = "/";
    if (*_root.rbegin() != '/' and *tmp.begin() != '/')
        _root += '/';
    return _root += tmp;
}

void route::set_default_page( const std::string &page ) {
    _default_page = page;
}

std::string route::get_default_page() const {
    return _default_page;
}

void route::autoindex_on( ) {
    _autoindex = true;
}

void route::autoindex_off( ) {
    _autoindex = false;
}

bool route::get_autoindex( ) const {
    return _autoindex;
}

const std::list<std::string>& route::get_http_methods( ) const {
    return _http_methods;
}

void route::set_http_methods( const std::list<std::string> &http_methods ) {
    _http_methods = http_methods;
}

std::string const &route::get_name( ) const {
    return _name;
}

std::string const &route::get_root( ) const {
    return _root;
}

std::string route::get_upload_location() const
{
	return _upload_location;
}

void route::set_upload_location(const std::string &location)
{
	_upload_location = location;
}

const route::CgiType& route::get_cgi() const
{
	return _cgi;
}

void route::set_cgi(const route::CgiType &cgi_)
{
	_cgi = cgi_;
}

std::ostream& operator<<(std::ostream& o, const route &route_) {
	o << "\t\troute '" << route_.get_name() << "': " << std::endl;
	o << "\t\t\troot: " << route_.get_root() << std::endl;
	o << "\t\t\tautoindex: " << (route_.get_autoindex() ? "on" : "false") << std::endl;
	o << "\t\t\tindex: " << route_.get_default_page() << std::endl;
	o << "\t\t\tallowed_methods:";
	for (std::list<std::string>::const_iterator it = route_.get_http_methods().begin();
		 it != route_.get_http_methods().end(); ++it) {
		o << " " << *it;
	}
	o << std::endl;
	o << "\t\t\tupload_location: " << route_.get_upload_location() << std::endl;
	o << "\t\t\tcgi: [" << route_.get_cgi().first << "] " << route_.get_cgi().second << std::endl;
	return o;
}
