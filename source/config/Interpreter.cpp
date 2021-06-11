#include "Interpreter.hpp"
#include "Number.hpp"

Interpreter::Interpreter(config &config_) : conf(config_), current_server() {

}

Interpreter::Interpreter(const Interpreter &other) : conf(other.conf), current_server(other.current_server) {

}

Interpreter &Interpreter::operator=(const Interpreter &other) {
    this->conf = other.conf;
    current_server = other.current_server;
    return *this;
}

Interpreter::~Interpreter() {

}

void Interpreter::visit(ANode *node) {
    if (IdentifierNode *identifierNode = dynamic_cast<IdentifierNode *>(node)) {
        return visit(identifierNode);
    }
    else if (ParamNode *paramNode = dynamic_cast<ParamNode *>(node)) {
        return visit(paramNode);
    }
    else if (RouteNode *routeNode = dynamic_cast<RouteNode *>(node)) {
        visit(routeNode);
    }
    else if (ServerNode *serverNode = dynamic_cast<ServerNode *>(node)) {
        return visit(serverNode);
    }
    else if (ConfigNode *configNode = dynamic_cast<ConfigNode *>(node)) {
        return visit(configNode);
    }
    else {
        throw NoVisitMethodException();
    }
}

void Interpreter::visit(IdentifierNode *) {
}

void Interpreter::visit(ParamNode *) {
}

route Interpreter::visit(RouteNode *node) {
    std::string            name      = node->endpoint.value;
    std::string            root;
    std::list<std::string> methods;
    bool                   autoindex = false;
    std::list<std::string> index;
    std::string            upload_location;
    route::CgiType         cgi;
    std::string            client_max_body_size;

    for (RouteNode::ParamValuesType::iterator it = node->values.begin();
         it != node->values.end(); ++it) {
        if (it->name.value == "root") {
            root = it->values.front().value;
        }
        else if (it->name.value == "allowed_methods") {
            for (ParamNode::ValuesType::iterator pit = it->values.begin();
                 pit != it->values.end(); ++pit) {
                methods.push_back(pit->value);
            }
        }
        else if (it->name.value == "directory_listing") {
            if (it->values.front().value == "on") {
                autoindex = true;
            }
        }
        else if (it->name.value == "index") {
            for (ParamNode::ValuesType::iterator pit = it->values.begin();
                 pit != it->values.end(); ++pit) {
                index.push_back(pit->value);
            }
        }
        else if (it->name.value == "upload_location") {
            upload_location = it->values.front().value;
        }
        else if (it->name.value == "cgi_extension") {
            ParamNode::ValuesType::iterator pit = it->values.begin();
            cgi.first = pit->value;
            ++pit;
            cgi.second = pit->value;
        }
        else if (it->name.value == "client_max_body_size") {
            client_max_body_size = it->values.front().value;
        }
        else {
            // this error is handled by parser, should not be accessible.
            std::cerr << "Unknown Route param: " << *it << std::endl;
        }
    }

    route route_ = route(name, root, methods, autoindex,
                         index.empty() ? std::string() : index.front());
    route_.set_upload_location(upload_location);
    route_.set_cgi(cgi);
    if (client_max_body_size.empty()) {
        route_.set_client_body_size(current_server->get_client_body_size());
    }
    else {
        set_client_max_body_size(route_, client_max_body_size);
    }
    return route_;
}

void Interpreter::visit(ServerNode *node) {
    server                                     serv;
    for (ServerNode::ParamValuesType::iterator it = node->params.begin();
         it != node->params.end(); ++it) {
        std::string name = it->name.value;
        if (name == "host") {
            serv.set_host(it->values.front().value);
        }
        else if (name == "port") {
            int port;
            strtot(it->values.front().value, port);
            serv.set_port(port);
        }
        else if (name == "error_page") {
            add_error_page(serv, it->values);
        }
        else if (name == "client_max_body_size") {
            set_client_max_body_size(serv, it->values.front().value);
        }
        else if (name == "server_names") {
            serv.set_server_names(std::list<std::string>()); // `.clear()`
            for (std::list<Token>::iterator pit = it->values.begin();
                 pit != it->values.end(); ++pit) {
                serv.add_server_name(pit->value);
            }
        }
        else if (name == "allow") {
            std::list<Token>::const_iterator pit = it->values.begin();
            std::string                      ext = pit->value;
            ++pit;
            std::string method = pit->value;
            serv.setAllow(std::pair<std::string, std::string>(ext, method));
        }
        else if (name == "cgi_path") {
            serv.setCgiPath(it->values.front().value);
        }
        else {
            // this error is handled by parser, should not be accessible.
            std::cerr << "Unknown Server param: " << *it << std::endl;
        }
    }
    current_server = &serv;
    for (ServerNode::RouteValuesType::iterator it = node->routes.begin();
         it != node->routes.end(); ++it) {
        RouteNode routeNode = *it;
        serv.add_route(visit(&routeNode));
    }
    current_server = NULL;
    conf.add_server(serv);
}

void Interpreter::visit(ConfigNode *node) {
    for (ConfigNode::ServerValuesType::iterator it = node->servers.begin();
         it != node->servers.end(); ++it) {
        ServerNode server = *it;
        visit(&server);
    }
}

void Interpreter::add_error_page(server &serv, const std::list<Token> &values) {
    ParamNode::ValuesType::const_iterator it = values.begin();
    int                                   code;
    strtot(it->value, code);
    ++it;
    std::string filename = it->value;
    serv.add_error_page(code, filename);
}

template<class T>
void Interpreter::set_client_max_body_size(T &serv, const std::string &value) {
    long client_body_size;
    strtot(value, client_body_size);
    char last = value[value.length() - 1];
    switch(last) {
        case 'K':
            client_body_size *= 1024;
            break;
        case 'M':
            client_body_size *= 1048576;
            break;
        case 'G':
            client_body_size *= 1073741824;
            break;
    }
    serv.set_client_body_size(client_body_size);
}
