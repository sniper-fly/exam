#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

#define BUFFER_SIZE 300

#define ERROR -1
#define EOF_REACHED 0
#define SUCCESS 1

int		ft_strlen(char *str)
{
	if (str == NULL)
		return (-1);
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

// NULLチェックを忘れず

char	*ft_strdup(char *str)
{
	if (str == NULL)
		return (NULL);
	
	int len = ft_strlen(str);
	char *dup = (char *)malloc(sizeof(char) * (len + 1));
	int i;

	for (i = 0; i < len; i++)
	{
		dup[i] = str[i];
	}
	dup[i] = 0;
	return (dup);
}

// 二回目のforループでi++を忘れがち

char	*ft_strjoin(char *str1, char *str2)
{
	if (str1 == NULL || str2 == NULL)
		return (NULL);
	
	int l1 = ft_strlen(str1);
	int l2 = ft_strlen(str2);
	int i;
	char *joined = (char*)malloc(sizeof(char) * (l1 + l2 + 1));

	for (i = 0; str1[i]; i++)
	{
		joined[i] = str1[i];
	}
	for (int j = 0; str2[j]; j++)
	{
		joined[i] = str2[j];
		i++;
	}
	joined[i] = 0;
	return (joined);
}

char	*has_endl(char *str)
{
	if (str == NULL)
		return (NULL);

	for (int i = 0; str[i]; i++)
	{
		if (str[i] == '\n')
			return (&str[i]);
	}
	return (NULL);
}

int		divide_dup(char **line, char **remain)
{
	char *endl_p = has_endl(*line);
	*endl_p = 0;

	char *fmt_part = ft_strdup(*line);

	endl_p++;
	char *ltr_part = ft_strdup(endl_p);

	free(*line);
	*line = fmt_part;

	free(*remain);
	*remain = ltr_part;

	return (SUCCESS);
}

int		read_buf(char **line, char **remain)
{
	//かならずbufferサイズより1個大きく確保
	char buf[BUFFER_SIZE + 1];
	int	read_size;

	while ((read_size = read(0, buf, BUFFER_SIZE)) >= 0)
	{
		buf[read_size] = 0;
		if (*line == NULL)
		{
			*line = ft_strdup(buf);
		}
		else
		{
			char *tmp = *line;
			*line = ft_strjoin(*line, buf);
			free(tmp);
		}
		if (has_endl(*line))
		{
				//return (func)をfuncと書かないように
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

int		get_next_line(char **line)
{
	static char *remain;
	//必ずstaticで宣言

	if (line == NULL)
		return (ERROR);
	*line = NULL;
	if (remain)
	{
		//ここではfreeしない
		*line = ft_strdup(remain);
	}
	if (has_endl(*line))
	{
		//＆忘れずに
		return (divide_dup(line, &remain));
	}
		//＆忘れずに
	return (read_buf(line, &remain));
}

int		main(void)
{
	int val;
	char *line;
	int ct = 0;

	while ((val = get_next_line(&line)) > 0)
	{
		printf("line%d:%d:%s\n", ct, val, line);
		free(line);
		line = NULL;
		ct++;
	}
	if (val == 0)
	{
		printf("line%d:%d:%s\n", ct, val, line);
		free(line);
		line = NULL;
	}
	else if (val < 0)
	{
		printf("error");
	}
	return (0);
}