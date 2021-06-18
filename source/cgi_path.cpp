//
// Created by Pamula File on 6/11/21.
//
#include "header.h"

std::string get_path_to_cgi( const std::string& binary, std::string path, std::string const & pwd)
{
    size_t  n = 0;
    size_t  k;
    std::string tmp;
    DIR     *dir;
    dirent  *dt;

    path += ':' + pwd + ':';
    while ((k = path.find(':', n)) != std::string::npos)
    {
        tmp = path.substr(n, k - n);
        n = k + 1;
        if ((dir = opendir(tmp.c_str())) == nullptr)
			continue ;
        while ((dt = readdir(dir)) != nullptr)
        {
            if (std::string(dt->d_name) == binary)
            {
                tmp.push_back('/');
                closedir(dir);
                return tmp + binary;
            }
        }
        closedir(dir);
    }
    return std::string();
}
