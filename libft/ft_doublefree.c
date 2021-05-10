#include "libft.h"

void	ft_doublefree(void *star)
{
	int		i;
	char	**str;

	str = (char **)star;
	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	str = 0;
}
