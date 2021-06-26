#ifndef ITRESSA_H
#define ITRESSA_H
# undef BUFFER_SIZE
# define BUFFER_SIZE 1

# include <unistd.h>
# include <stdlib.h>

void	ft_doublefree(void *);
int		get_next_line(int fd, char **line);
int		ft_bufersize(long int i);
int		ft_newline(char *buf, int a);
char	*ft_strdupper(char *buf);
char	*ft_strduplic(char *buf);
char	*ft_strjoiner(char *s1, char *s2);

#include "config.hpp"
#include "Token.hpp"

typedef const char* (*param_validator)(const std::list<Token> &values);

struct ContextInfo {
    const char *paramName;
    int numberOfParams;
    param_validator validate;
};

config parse(int fd);
config parse(const char *filename);

#endif
