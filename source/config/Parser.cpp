#include "Parser.hpp"

Parser::Parser(std::vector<Token> &tokens_) : tokens(tokens_), current_token(), tok_idx(-1) {
    advance();
}

Token &Parser::advance() {
    ++tok_idx;
    if (static_cast<std::vector<Token>::size_type>(tok_idx) < tokens.size()) {
        current_token = &tokens[tok_idx];
    }
    return *current_token;
}

ParseResult Parser::parse() {
    ParseResult result = config();
    if (!result.error && current_token->type != Token::TT_EOF) {
        return result.failure(getSyntaxError("Expected End Of File"));
    }
    return result;
}

ParseResult Parser::config() {
    ParseResult                  result;
    ConfigNode::ServerValuesType servers;
    while (current_token->type != Token::TT_EOF) {
        if (current_token->type == Token::IDENTIFIER &&
            current_token->value == "server") {
            advance();
            ANode *serv = result.checkIn(server());
            if (!result.error && serv != NULL) {
                servers.push_back(*dynamic_cast<ServerNode *>(serv));
            }
            delete serv;
        }
        else {
            result.failure(getSyntaxError("Expected 'server', got '" + current_token->text() + "'"));
            skip_param_or_group_tokens();
        }
        skip_end_of_line_tokens();
    }
    return result.checkInSuccess ? result.success(new ConfigNode(servers)) : result;
}

ParseResult Parser::server() {
    ParseResult                 result;
    ServerNode::ParamValuesType params;
    ServerNode::RouteValuesType routes;
    if (expect_lcurly(result))
        return result;
    while (current_token->type == Token::IDENTIFIER) {
        if (current_token->value == "route") {
            advance();
            ANode *route_ = result.checkIn(route());
            if (result.checkInSuccess) {
                routes.push_back(*dynamic_cast<RouteNode *>(route_));
            }
            delete route_;
        }
        else {
            ANode *param_ = result.checkIn(param(ServerNode::validParamNames));
            if (result.checkInSuccess) {
                params.push_back(*dynamic_cast<ParamNode *>(param_));
            }
            delete param_;
        }
        skip_end_of_line_tokens();
    }
    if (expect_rcurly(result))
        return result;
    if (result.error)
        result.checkInSuccess = false;
    ServerNode *node = new ServerNode(params, routes);
    if (!node->isValid()) {
        delete node;
        result.failure(getSyntaxError(
            "'server' directive at least must have 'host', 'port' and 'client_max_body_size' and must not have duplicates for anything except `error_page` and at least 1 route"));
    }
    return result.checkInSuccess ? result.success(node) : result;
}

ParseResult Parser::route() {
    ParseResult                result;
    Token                      endpoint = *current_token;
    RouteNode::ParamValuesType params;
    if (current_token->type == Token::IDENTIFIER) {
        advance();
    }
    else {
        skip_param_or_group_tokens();
        return result.failure(getSyntaxError("Expected route path."));
    }
    if (expect_lcurly(result))
        return result;
    while (current_token->type == Token::IDENTIFIER) {
        ANode *param_ = result.checkIn(param(RouteNode::validParamNames));
        if (result.checkInSuccess) {
            params.push_back(*dynamic_cast<ParamNode *>(param_));
        }
        delete param_;
    }
    if (expect_rcurly(result))
        return result;
    if (result.error)
        result.checkInSuccess = false;
    RouteNode *node = new RouteNode(endpoint, params);
    if (!node->isValid()) {
        delete node;
        result.failure(getSyntaxError(
            "'route' directive at least must have 'allowed_methods', 'root' and 'index' and must not have any duplicates"));
    }
    return result.checkInSuccess ? result.success(node) : result;
}

ParseResult Parser::param(const ContextInfo *paramsInfo) {
    ParseResult           result;
    Token                 name = *current_token;
    ParamNode::ValuesType values;

    if (current_token->type == Token::IDENTIFIER) {
        advance();
        const ContextInfo &cinfo = getParamInfo(paramsInfo, name.value);
        if (cinfo.paramName != NULL && cinfo.paramName == name.value) {
            while (current_token->type == Token::IDENTIFIER) {
                values.push_back(*current_token);
                advance();
            }
            if ((cinfo.numberOfParams >= 0 &&
                 static_cast<int>(values.size()) != cinfo.numberOfParams) ||
                (cinfo.numberOfParams == -1 && values.empty())) {
                result.failure(getSyntaxError("Wrong number of values for '" + name.value + "'"));
            }
            const char *error;
            if (cinfo.validate && (error = cinfo.validate(values))) {
                result.failure(getSyntaxError(error, values.front()));
            }
            skip_end_of_line_tokens();
            return result.checkInSuccess ? result.success(new ParamNode(name, values)) : result;
        }
        else {
            result.failure(getSyntaxError("Undexpected param name '" + name.value + "'", name));
        }
    }
    else {
        result.failure(getSyntaxError("Expected param name identifier"));
    }
    skip_param_or_group_tokens();
    return result;
}

const ContextInfo &Parser::getParamInfo(const ContextInfo *paramsInfo, const std::string &param) {
    while (paramsInfo->paramName != NULL && paramsInfo->paramName != param) {
        ++paramsInfo;
    }
    return *paramsInfo;
}

bool Parser::expect_lcurly(ParseResult &result) {
    skip_end_of_line_tokens();
    if (current_token->type == Token::LCURLY) {
        advance();
        skip_end_of_line_tokens();
        return false;
    }
    skip_end_of_line_tokens();
    result.failure(getSyntaxError("Expected '{'"));
    return true;
}

bool Parser::expect_rcurly(ParseResult &result) {
    skip_end_of_line_tokens();
    if (current_token->type == Token::RCURLY) {
        advance();
        skip_end_of_line_tokens();
        return false;
    }
    skip_end_of_line_tokens();
    result.failure(getSyntaxError("Expected '}'"));
    return true;
}

InvalidSyntaxErrorNode *Parser::getSyntaxError(const std::string &reason) const {
    return getSyntaxError(reason, *current_token);
}

InvalidSyntaxErrorNode *Parser::getSyntaxError(const std::string &reason, const Token &token) {
    return new InvalidSyntaxErrorNode(token.start, token.end, reason);
}

void Parser::skip_end_of_line_tokens() {
    while (current_token->type == Token::NEWLINE ||
           current_token->type == Token::COMMENT ||
           current_token->type == Token::SEMICOLON) {
        advance();
    }
}

void Parser::skip_param_or_group_tokens() {
    while (current_token->type != Token::NEWLINE &&
           current_token->type != Token::SEMICOLON &&
           current_token->type != Token::LCURLY) {
        advance();
    }
    if (current_token->type == Token::LCURLY) {
        while (current_token->type != Token::RCURLY) {
            advance();
        }
    }
    advance();
}
