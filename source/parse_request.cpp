//
// Created by Pamula File on 6/17/21.
//
#include "header.h"

static void parse_first_line(char *line, Header &head)
{
    int i = -1;
    int j;
    std::string str;

    while (line[++i])
        if (line[i] == ' ')
            break ;
    if (!line[i])
    {
        head.setMethod(std::string("THIS SHIT NOT IMPLEMENTED", 0, i));
        return ;
    }
    head.setMethod(std::string(line, 0, i));
    j = i + 1;
    while (line[++i])
        if (line[i] == ' ')
            break ;
    if (!line[i])
    {
        head.setMethod(std::string("THIS SHIT NOT IMPLEMENTED", 0, i));
        return ;
    }
    head.setRequest(std::string(line, j, i - j));
    j = i + 1;
    while (line[++i]);
    head.setHttp(std::string(line, j, i - j));
    head.addEnv((char *)("REQUEST_URI=" + head.getRequest()).c_str());
    head.addEnv((char *)("REQUEST_METHOD=" + head.getMethod()).c_str());
    head.addEnv((char *)("SERVER_PROTOCOL=" + head.getHttp()).c_str());
    head.addEnv((char *)("PATH_INFO=" + head.getRequest()).c_str());
    head.addEnv((char *)("PATH_TRANSLATED=" + head.getRequest()).c_str());
    str = head.getRequest();
    if (str.find('?') != std::string::npos)
        head.addEnv((char *)("QUERY_STRING=" + str.erase(0, str.find('?') + 1)).c_str());
}

void        setter(std::string find, std::string set, void (Header::*func)(std::string const &), Header &head)
{
    head.func = func;
    head.*func
}

std::string  parse_request(std::string string, Header &head)
{
    if (head.empty_line)
    {

    }
    else
    {
        if (string.find("Accept-Language: ") != std::string::npos)
        {
            setter("Accept-Language: ", "Content-Language: ", &head.setContent_Language, head);
            string.erase("Accept-Language: ");
            head.setContent_Language("Content-Language: " + string.era)
        }
        if ((tmp = strstr(line, "Accept-Language: ")))
        {
            tmp += strlen("Accept-Language: ");
            head.setContent_Language("Content-Language: " + std::string(tmp) + "\n");
        }
        else if ((tmp = strstr(line, "Host: ")))
        {
            tmp += strlen("Host: ");
            tmp2 = strchr(tmp, ':');
            if (head.getHost().empty() and tmp2 != NULL )
            {
                head.setHost(std::string(tmp, 0, tmp2 - tmp));
            }
            else
            {
//                response(server::exception_processing(400, head));
                std::cout << "line : " << line  << std::endl;
                head.setHost("400");
                std::cout << "HOST ERROR"  << std::endl;
            }
            head.setPort(atoi(tmp2 + 1));
        }
        else if ((tmp = strstr(line, "Referer: ")))
        {

            tmp += strlen("Referer: ");
            head.setReferer(tmp);
            while (line[++i])
            {
                if (line[i] == '/')
                    count++;
                if (count == 3)
                    break ;
            }
            str = std::string(line + i);
            if ((i = str.find('?')) != std::string::npos)
                str.erase(i);
            if ((i = str.rfind('.')) != std::string::npos)
                str.erase(0, i);
            if ((i = str.find('/')) != std::string::npos)
            {
                char tmpp[150];
                str.erase(0, i);
                head.addEnv((char *)("PATH_INFO=" + str).c_str());
                head.addEnv((char *)("PATH_TRANSLATED=" + std::string(getcwd(tmpp, sizeof(tmpp))) + str).c_str());
            }
        }
        else if ((tmp = strstr(line, "Accept: ")))
        {

            tmp += strlen("Accept: ");
            str = std::string(tmp);
            head.addEnv((char *)("CONTENT_TYPE=" + std::string(str, 0, str.find(','))).c_str());
        }
        else if (head.getMethod() == "POST" && strchr(line, '='))
        {

            head.addEnv((char *)("QUERY_STRING=" + std::string(line)).c_str());
        }
        else if ((tmp = strstr(line, "Transfer-Encoding: ")))
        {

            tmp += strlen("Transfer-Encoding: ");
            head.setTransfer_Encoding(std::string(tmp));
        }
        else if ((tmp = strstr(line, "Authorization: ")))
        {
            tmp += strlen("Authorization: ");
            head.addEnv((char *)("AUTH_TYPE=" + std::string(tmp, 0, strchr(tmp, ' ') - tmp)).c_str());
            if (std::string(head.getEnvValue("AUTH_TYPE=")) == "BASIC" || std::string(head.getEnvValue("AUTH_TYPE=")) == "DIGEST")
            {
                tmp = strchr(tmp, ' ') + 1;
                head.addEnv((char *)("REMOTE_USER=" + std::string(tmp, 0, strchr(tmp, ':') - tmp)).c_str());
                tmp = strchr(tmp, ':') + 1;
                head.addEnv((char *)("REMOTE_IDENT=" + std::string(tmp)).c_str());
            }
        }
    }
}