//
// Created by Pamula File on 5/9/21.
//

#ifndef PFILE_H
#define PFILE_H

void    tests(std::list<server> conf);
server  find_server(std::list<server> serv, std::string const & host, unsigned int port);

#endif //PFILE_H
