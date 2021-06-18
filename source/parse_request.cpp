//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

static void parse_first_line(std::string const & line, Header &head)
{
    head.addEnv((char *)("REQUEST_URI=" + head.getRequest()).c_str());
    head.addEnv((char *)("REQUEST_METHOD=" + head.getMethod()).c_str());
    head.addEnv((char *)("SERVER_PROTOCOL=" + head.getHttp()).c_str());
    head.addEnv((char *)("PATH_INFO=" + head.getRequest()).c_str());
    head.addEnv((char *)("PATH_TRANSLATED=" + head.getRequest()).c_str());
    str = head.getRequest();
    if (str.find('?') != std::string::npos)
    head.addEnv((char *)("QUERY_STRING=" + str.erase(0, str.find('?') + 1)).c_str());
    head.addEnv((char *)("CONTENT_TYPE=" + std::string(str, 0, str.find(','))).c_str());
    head.addEnv((char *)("AUTH_TYPE=" + std::string(tmp, 0, strchr(tmp, ' ') - tmp)).c_str());
    if (std::string(head.getEnvValue("AUTH_TYPE=")) == "BASIC" || std::string(head.getEnvValue("AUTH_TYPE=")) == "DIGEST")
    {
        tmp = strchr(tmp, ' ') + 1;
        head.addEnv((char *)("REMOTE_USER=" + std::string(tmp, 0, strchr(tmp, ':') - tmp)).c_str());
        tmp = strchr(tmp, ':') + 1;
        head.addEnv((char *)("REMOTE_IDENT=" + std::string(tmp)).c_str());
    }
}

std::string  parse_request(std::string string, Header &head)
{
    if (head.empty_line)
    {

    }
    else
    {
        head.setter(string);
    }
}