#include "libft.h"

char	**ft_doublecpy(char **line)
{
	int		i;
	char	**str;

	i = 0;
	if (!line)
		return 0;
	while (line[i])
		i++;
	if (!(str = malloc(sizeof(char *) * (i + 1))))
		return (NULL);
	i = -1;
	while (line[++i])
		if (!(str[i] = ft_strdup(line[i])))
			return (NULL);
	str[i] = 0;
	return (str);
}
