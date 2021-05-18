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
        return result.failure(ErrorNode::getSyntaxError(*current_token, "Expected End Of File"));
    }
    return result;
}

ParseResult Parser::config() {
    ParseResult                  result;
    ConfigNode::ServerValuesType servers;
    while (current_token->type != Token::TT_EOF) {
        if (current_token->type == Token::IDENTIFIER &&
            current_token->value == "server") {
            ANode *serv = result.checkIn(server());
            if (!result.error && serv != NULL) {
                servers.push_back(*dynamic_cast<ServerNode *>(serv));
            }
            delete serv;
        }
        else {
            result.failure(ErrorNode::getSyntaxError(*current_token, "Expected 'server', got '" + current_token->text() + "'"));
            skip_param_or_group_tokens();
        }
        skip_end_of_line_tokens();
    }
    return result.checkInSuccess ? result.success(new ConfigNode(servers)) : result;
}

ParseResult Parser::server() {
    ParseResult                 result;
    Token                       name = *current_token;
    ServerNode::ParamValuesType params;
    ServerNode::RouteValuesType routes;

    advance();
    if (expect_lcurly(result))
        return result;
    while (current_token->type == Token::IDENTIFIER) {
        if (current_token->value == "route") {
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
    ServerNode *node = new ServerNode(name, params, routes);
    if (!node->isValid(result) || !result.checkInSuccess) {
        delete node;
        return result;
    }
    return result.success(node);
}

ParseResult Parser::route() {
    ParseResult                result;
    Token                      name = *current_token;
    advance();
    Token                      endpoint = *current_token;
    RouteNode::ParamValuesType params;
    if (current_token->type == Token::IDENTIFIER) {
        advance();
    }
    else {
        skip_param_or_group_tokens();
        return result.failure(ErrorNode::getSyntaxError(*current_token, "Expected route path."));
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
    RouteNode *node = new RouteNode(name, endpoint, params);
    if (!node->isValid(result) || !result.checkInSuccess) {
        delete node;
        return result;
    }
    return result.success(node);
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
                result.failure(ErrorNode::getSyntaxError(*current_token, "Wrong number of values for '" + name.value + "'"));
            }
            const char *error;
            if (cinfo.validate && (error = cinfo.validate(values))) {
                result.failure(ErrorNode::getSyntaxError(values.front(), error));
            }
            skip_end_of_line_tokens();
            return result.checkInSuccess ? result.success(new ParamNode(name, values)) : result;
        }
        else {
            result.failure(ErrorNode::getSyntaxError(name, "Undexpected param name '" + name.value + "'"));
        }
    }
    else {
        result.failure(ErrorNode::getSyntaxError(*current_token, "Expected param name identifier"));
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
    result.failure(ErrorNode::getSyntaxError(*current_token, "Expected '{'"));
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
    result.failure(ErrorNode::getSyntaxError(*current_token, "Expected '}'"));
    return true;
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
