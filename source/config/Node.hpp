#ifndef NODE_HPP
#define NODE_HPP

#include <iostream>
#include <list>
#include "Token.hpp"
#include "Position.hpp"

class Node
{
public:
	virtual ~Node();
	virtual void display_to(std::ostream& o) const;
};

std::ostream& operator<<(std::ostream& o, Node const& node);

class IdentifierNode : virtual public Node {
public:
	explicit IdentifierNode(const Token &value);
	virtual void display_to(std::ostream &o) const;

	Token value;
};

class ParamNode : virtual public Node {
public:
	typedef std::list<Token> ValuesType;

	ParamNode(const Token &name_, const ValuesType &values_);
	virtual void display_to(std::ostream &o) const;

	Token name;
	ValuesType values;
};

class RouteNode : virtual public Node {
public:
	typedef std::list<ParamNode> ParamValuesType;

	explicit RouteNode(const Token &endpoint, const ParamValuesType &values_);
	virtual void display_to(std::ostream &o) const;

	Token endpoint;
	ParamValuesType values;
};

class ServerNode : virtual public Node {
public:
	typedef std::list<ParamNode> ParamValuesType;
	typedef std::list<RouteNode> RouteValuesType;

	ServerNode(const ParamValuesType &params_, const RouteValuesType &routes_);
	virtual void display_to(std::ostream &o) const;

	ParamValuesType params;
	RouteValuesType routes;
};

class ConfigNode : virtual public Node {
public:
	typedef std::list<ParamNode> ParamValuesType;
	typedef std::list<ServerNode> ServerValuesType;

	ConfigNode(const ParamValuesType &params_, const ServerValuesType &servers_);
	virtual void display_to(std::ostream &o) const;

	ParamValuesType params;
	ServerValuesType servers;
};

class ErrorNode : virtual public Node {
public:
	ErrorNode(const Position &start, const Position &end, const std::string& reason);
	virtual std::string getType() const;
	const Position& getStart() const;
	const Position& getEnd() const;
	const std::string& getReason() const;

protected:
	Position start;
	Position end;
	std::string reason;
};

std::ostream& operator<<(std::ostream &o, const ErrorNode &errorNode);

class InvalidSyntaxErrorNode : virtual public ErrorNode {
public:
	InvalidSyntaxErrorNode(const Position &start, const Position &end, const std::string &reason);
	virtual std::string get_type() const;
};

#endif
