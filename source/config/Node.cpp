#include "Node.hpp"

IdentifierNode::IdentifierNode(const Token &value_) : value(value_)
{
}

ParamNode::ParamNode(const Token &name_, const ValuesType &values_) : name(name_), values(values_)
{
}

RouteNode::RouteNode(const Token &endpoint_, const ParamValuesType &values_) : endpoint(endpoint_), values(values_)
{
}

ServerNode::ServerNode(const ParamValuesType &params_,
	   const RouteValuesType &routes_) : params(params_), routes(routes_)
{
}

ConfigNode::ConfigNode(const ParamValuesType &params_,
	   const ServerValuesType &servers_) : params(params_), servers(servers_)
{
}

void Node::display_to(std::ostream &o) const
{
	o << "(Node) ";
}

void IdentifierNode::display_to(std::ostream &o) const
{
	Node::display_to(o);
	o << "(" << value << ")";
}

void ParamNode::display_to(std::ostream &o) const
{
	Node::display_to(o);
	o << "(Param: " << name << ": ";
	if (!values.empty()) {
		for (ValuesType::const_iterator it = values.begin();
			 it != values.end(); ++it) {
			o << *it << ", ";
		}
		std::cout << "\b\b";
	}
	std::cout << ")";
}

void RouteNode::display_to(std::ostream &o) const
{
	Node::display_to(o);
	o << "(Route: " << endpoint << ": ";
	if (!values.empty()) {
		for (ParamValuesType ::const_iterator it = values.begin();
			 it != values.end(); ++it) {
			o << *it << ", ";
		}
		std::cout << "\b\b";
	}
	std::cout << ")";
}

void ServerNode::display_to(std::ostream &o) const
{
	Node::display_to(o);
	o << "(Server: ";
	if (!params.empty()) {
		for (ParamValuesType ::const_iterator it = params.begin();
			 it != params.end(); ++it) {
			o << *it << ", ";
		}
		std::cout << "\b\b";
	}
	o << " | ";
	if (!routes.empty()) {
		for (RouteValuesType::const_iterator it = routes.begin();
			 it != routes.end(); ++it) {
			o << *it << ", ";
		}
		std::cout << "\b\b";
	}
	std::cout << ")";
}

void ConfigNode::display_to(std::ostream &o) const
{
	Node::display_to(o);
	o << "(Config: ";
	if (!params.empty()) {
		for (ParamValuesType ::const_iterator it = params.begin();
			 it != params.end(); ++it) {
			o << *it << ", ";
		}
		std::cout << "\b\b";
	}
	o << " | ";
	if (!servers.empty()) {
		for (ServerValuesType::const_iterator it = servers.begin();
			 it != servers.end(); ++it) {
			o << *it << ", ";
		}
		std::cout << "\b\b";
	}
	std::cout << ")";
}

std::ostream &operator<<(std::ostream &o, const Node &node)
{
	node.display_to(o);
	return o;
}

ErrorNode::ErrorNode(const Position &start, const Position &end,
					 const std::string &reason) : start(start), end(end), reason(reason)
{
}

std::string ErrorNode::getType() const
{
	return std::string("Unknown Error");
}

const Position &ErrorNode::getStart() const
{
	return start;
}

const Position &ErrorNode::getEnd() const
{
	return end;
}

const std::string &ErrorNode::getReason() const
{
	return reason;
}

std::ostream& operator<<(std::ostream& o, const ErrorNode& errorNode) {
	o << errorNode.getType() << ": " << errorNode.getReason() << std::endl <<
		"at " << errorNode.getStart().line << ":" << errorNode.getStart().col <<
		" (probably) until " << errorNode.getEnd().line << ":" << errorNode.getEnd().col <<
		std::endl;
	return o;
}

InvalidSyntaxErrorNode::InvalidSyntaxErrorNode(const Position &start,
											   const Position &end,
											   const std::string &reason)
	: ErrorNode(start, end, reason)
{
}

std::string InvalidSyntaxErrorNode::get_type() const
{
	return "Invalid Syntax";
}

Node::~Node()
{
}
