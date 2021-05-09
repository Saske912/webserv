#include "Interpreter.hpp"

int Interpreter::poor_atoi(const std::string &num) {
    int value = 0;
    for (std::string::const_iterator it = num.begin();
         it != num.end(); ++it) {
        if (*it < '0' || *it > '9')
            break;
        value = value * 10 + *it - '0';
    }
    return value;
}

Interpreter::Interpreter(config &config_) : conf(config_) {

}

Interpreter::Interpreter(const Interpreter &other) : conf(other.conf) {

}

Interpreter &Interpreter::operator=(const Interpreter &other) {
    return *this;
}

Interpreter::~Interpreter() {

}

void Interpreter::visit(Node *node)
{
	if (IdentifierNode *identifierNode = dynamic_cast<IdentifierNode*>(node)) {
		return visit(identifierNode);
	}
	else if (ParamNode *paramNode = dynamic_cast<ParamNode*>(node)) {
		return visit(paramNode);
	}
	else if (RouteNode *routeNode = dynamic_cast<RouteNode*>(node)) {
		return visit(routeNode);
	}
	else if (ServerNode *serverNode = dynamic_cast<ServerNode*>(node)) {
		return visit(serverNode);
	}
	else if (ConfigNode *configNode = dynamic_cast<ConfigNode*>(node)) {
		return visit(configNode);
	}
	else {
		throw NoVisitMethodException();
	}
}

void Interpreter::visit(IdentifierNode *node)
{
}

void Interpreter::visit(ParamNode *node)
{
}

route Interpreter::visit(RouteNode *node)
{

}

void Interpreter::visit(ServerNode *node)
{
    std::string host;
    unsigned int port = 0;
    std::map<int, std::string> error_pages;
    std::list<route> routes;
    long client_max_body_size = 0;
    for (ServerNode::ParamValuesType::iterator it = node->params.begin();
        it != node->params.end(); ++it) {
        std::string name = it->name.value;
        if (name == "host") {
            host = it->values.front().value;
        }
        else if (name == "port") {
            port = poor_atoi(it->values.front().value);
        }
        else if (name == "error_page") {
            ParamNode::ValuesType::iterator erit = it->values.begin();
            error_pages.insert(std::pair<int, std::string>(poor_atoi(erit->value), (++erit)->value));
        }
        else if (name == "client_max_body_size") {
            client_max_body_size = poor_atoi(it->values.front().value);
        }
    }
    for (ServerNode::RouteValuesType::iterator it = node->routes.begin();
        it != node->routes.end(); ++it) {
        RouteNode routeNode = *it;
        routes.push_back(visit(&routeNode));
    }
    conf.add_server(server(host, port, error_pages, routes, client_max_body_size));
}

void Interpreter::visit(ConfigNode *node)
{
    for (ConfigNode::ServerValuesType::iterator it = node->servers.begin();
        it != node->servers.end(); ++it) {
        ServerNode server = *it;
        visit(&server);
    }
}
