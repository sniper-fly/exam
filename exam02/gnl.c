#include <unistd.h>


#include <fcntl.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define EOF_REACHED 0
#define ERROR -1
#define BUFFER_SIZE 30

int		read_buf(char **line, int fd, char **remain);
int		divide_dup(char **line, char **remain);

char	*ft_strchr(char *line, char ch)
{
	if (line == NULL)
		return (NULL);
	for (int i = 0; line[i]; i++)
	{
		if (line[i] == ch)
		{
			return (&line[i]);
		}
	}
	return (NULL);
}

int		ft_strlen(char *str)
{
	int	i;
	for (i = 0; str[i]; i++) {;}
	return (i);
}

char	*ft_strdup(char *str)
{
	int		len = ft_strlen(str);
	char	*dup = (char *)malloc(sizeof(char) * (len + 1));
	int		i;

	for (i = 0; i < len; i++)
	{
		dup[i] = str[i];
	}
	dup[i] = 0;
	return (dup);
}

char	*ft_strjoin(char *str1, char *str2)
{
	int		len1 = ft_strlen(str1);
	int		len2 = ft_strlen(str2);
	char	*joined = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	int		i;

	for (i = 0; i < len1; i++)
	{
		joined[i] = str1[i];
	}
	for (int j = 0; j < len2; j++)
	{
		joined[i] = str2[j];
		i++;
	}
	joined[i] = 0;
	return (joined);
}

int		get_next_line(int fd, char **line)
{
	static char	*remain;

	if (line == NULL || fd == 1 || fd == 2 || fd < 0)
		return (ERROR);
	*line = NULL;
	if (remain)
	{
		*line = ft_strdup(remain);
		if (ft_strchr(*line, '\n'))
		{
			return (divide_dup(line, &remain));
		}
	}
	return (read_buf(line, fd, &remain));
}

int		read_buf(char **line, int fd, char **remain)
{
	int	read_size;
	char	buf[BUFFER_SIZE + 1];
	char	*tmp;

	while ((read_size = read(fd, buf, BUFFER_SIZE)) >= 0)
	{
		buf[read_size] = '\0';
		if (*line == NULL) {
			*line = ft_strdup(buf);
		}
		else
		{
			tmp = *line;
			*line = ft_strjoin(*line, buf);
			free(tmp);
		}
		if (ft_strchr(*line, '\n'))
		{
			return (divide_dup(line, remain));
		}
		else if (read_size == 0)
		{
			free(*remain);
			return (EOF_REACHED);
		}
	}
	free(*remain);
	return (ERROR);
}

int		divide_dup(char **line, char **remain)
{
	char	*endlptr = ft_strchr(*line, '\n');

	*endlptr = 0;
	char	*fmr = ft_strdup(*line);

	endlptr++;
	char	*ltr = ft_strdup(endlptr);

	free(*line);
	*line = fmr;

	free(*remain);
	*remain = ltr;

	return (TRUE);

	// char	*tmp;

	// *endlptr = 0;
	// tmp = *line;
	// *line = ft_strdup(*line);

	// endlptr++;
	// free(*remain);
	// *remain = ft_strdup(endlptr);

	// free(tmp);
	// return (TRUE);
}

#include <stdio.h>
int		main(int argc, char **argv)
{
	int		fd = 0;
	int		val;
	char	*line;
	int		ct = 1;

	(void)argc;
	while ((val = get_next_line(fd, &line)) >= 0)
	{
		printf("val%d:line%d:[%s]\n", val, ct, line);
		free(line);
		line = NULL;
		if (val == 0)
			break;
		ct++;
	}
}