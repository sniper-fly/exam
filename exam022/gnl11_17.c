#include <unistd.h>
#include <stdlib.h>
#define EOF_REACHED 0
#define SUCCESS 1
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 300

int		read_buf(char **line, char **remain);
int		divide_dup(char **line, char **remain);

int		ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*get_endl(char *str)
{
	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\n')
		{
			return (&str[i]);
		}
	}
	return (NULL);
}

char	*ft_strdup(char *str)
{
	char *dup = (char*)malloc(sizeof(char) * (ft_strlen(str) + 1));
	int i;
	for (i = 0; str[i]; i++)
	{
		dup[i] = str[i];
	}
	dup[i] = 0;
	return (dup);
}

char	*ft_strjoin(char *s1, char *s2)
{
	int l1 = ft_strlen(s1);
	int l2 = ft_strlen(s2);
	char *joined = (char*)malloc(sizeof(char) * (l1 + l2 + 1));
	int i;

	for (i = 0; s1[i]; i++)
	{
		joined[i] = s1[i];
	}
	for (int j = 0; s2[j]; j++)
	{
		joined[i] = s2[j];
		i++;
	}
	joined[i] = 0;
	return (joined);
}

int		get_next_line(char **line)
{
	static char *remain;

	if (line == NULL)
		return (ERROR);

	*line = NULL;
	if (remain)
	{
		*line = ft_strdup(remain);
		if (get_endl(*line))
		{
			return (divide_dup(line, &remain));
		}
	}
	return (read_buf(line, &remain));
}

int		read_buf(char **line, char **remain)
{
	char buf[BUFFER_SIZE + 1];
	int val;

	while ((val = read(0, buf, BUFFER_SIZE)) >= 0)
	{
		buf[val] = 0;
		if (*line == NULL)
		{
			*line = ft_strdup(buf);
		}
		else
		{
			char *tmp = *line;
			*line = ft_strjoin(*line , buf);
			free(tmp);
		}
		if (get_endl(*line))
		{
			return (divide_dup(line, remain));
		}
		else if (val == 0)
		{
			free (*remain);
			return (EOF_REACHED);
		}
	}
	free(*remain);
	return (ERROR);
}

int		divide_dup(char **line, char **remain)
{
	char *endlptr = get_endl(*line);
	*endlptr = 0;

	char *fmt_part = ft_strdup(*line);

	endlptr++;
	char *ltr_part = ft_strdup(endlptr);

	free(*line);
	*line = fmt_part;

	free(*remain);
	*remain = ltr_part;

	return (SUCCESS);
}

#include <stdio.h>
int		main(void)
{
	int ct = 0;
	char *line;
	int val;

	while ((val = get_next_line(&line)) > 0)
	{
		printf("line:%d:%d:%s\n", ct, val, line);
		free(line);
		line = NULL;
		ct++;
	}
	if (val == 0)
	{
		printf("line:%d:%d:%s\n", ct, val, line);
		free(line);
	}
	else
	{
		printf("error");
	}
	return (0);
}