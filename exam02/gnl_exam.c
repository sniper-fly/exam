#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#define ERROR -1
#define TRUE 1
#define FALSE 0

#define SUCCESS 1
#define EOF_REACHED 0
#define BUFFER_SIZE 30

int		ft_strlen(char *str) {
	int len = 0;
	while (str[len])
		len++;
	return (len);
}

char	*ft_strchr(char *str, char ch) {
	int idx = 0;

	if (str == NULL)
		return (NULL);
	if (ch == '\0') {
		while (str[idx])
			idx++;
		return (&str[idx]);
	}
	while (str[idx]) {
		if (str[idx] == ch) {
			return (&str[idx]);
		}
		idx++;
	}
	return (NULL);
}

char	*ft_strdup(char *str) {
	char *dup;
	int idx = 0;

	if ((dup = (char *)malloc(sizeof(char) * (ft_strlen(str)) + 1)) == NULL)
		return (NULL);
	while (str[idx]) {
		dup[idx] = str[idx];
		idx++;
	}
	dup[idx] = '\0';
	return (dup);
}

char	*ft_strjoin(char *str1, char *str2) {
	int		str1len = ft_strlen(str1);
	int		str2len = ft_strlen(str2);
	int		join_len = str1len + str2len + 1;
	int		idx = 0;
	int		jdx = 0;
	char	*joined = (char *)malloc(sizeof(char) * join_len);
	
	if (joined == NULL)
		return (NULL);
	while (idx < str1len) {
		joined[idx] = str1[idx];
		idx++;
	}
	while (idx < str1len + str2len) {
		joined[idx] = str2[jdx];
		idx++;
		jdx++;
	}
	joined[idx] = '\0';
	return (joined);
}

//改行があるかぎり必ず入る 改行が無ければ入らない
int		divide_dup(char **line, char **remain)
{
	//最初にlineを\nで分割
	char *endl_ptr = ft_strchr(*line, '\n');
	*endl_ptr = '\0';

	//前半、後半部分を分けて複製しそれぞれ一時保管
	char *fmr_part = ft_strdup(*line);
	endl_ptr++; //endl_ptrが今指しているのはさっきnull文字で埋めたptrなので、1バイト進めると文字列がある
	char *ltr_part = ft_strdup(endl_ptr);

	//*lineをfreeし、*lineに前半部を入れる
	free(*line);
	*line = fmr_part;
	//*remainに後半部分をいれる

	free(*remain);
	*remain = ltr_part;

	return (SUCCESS);
}

int		read_join(char **line, int fd, char **remain)
{
	int		read_size;
	char	*tmp_line;
	char	read_buf[BUFFER_SIZE + 1];

	//基本的にはread_size > 0の条件で良いが、空白のファイルをgnlしたときに、read_size>=0の条件でないと*line=NULLのままになってしまう
	while ((read_size = read(fd, read_buf, BUFFER_SIZE)) >= 0)
	{
		read_buf[read_size] = '\0';
		if (*line == NULL)
		{
			*line = ft_strdup(read_buf);
			if (*line == NULL) return (ERROR);
			// return (divide_dup(line, &remain));
		}
		else
		{
			tmp_line = *line;
			*line = ft_strjoin(*line, read_buf);
			if (*line == NULL) return (ERROR);
			free(tmp_line);
		}
		if (ft_strchr(*line, '\n'))
		{
			return (divide_dup(line, remain));
		}
		else if (read_size == 0) //最後の行に差し掛かった時
		{
			free(*remain);
			return (EOF_REACHED);
		}
	}
	// read_size < 0 つまり異常終了
	return (ERROR);
}

int		get_next_line(int fd, char **line)
{
	static char	*remain;

	if (line == NULL || fd < 0 || fd == 1 || fd == 2)
		return (ERROR);
	// 一番最初のループのwhile文の条件で、何かゴミが入ってると困るため
	*line = NULL;
	//まず、残り物が入っていたら*lineに入れておく
	if (remain)
		*line = ft_strdup(remain);

	//*lineの中に改行があるか調べる
	if (ft_strchr(*line, '\n'))
	{
		//あれば分割して*lineに詰めて返す
		return (divide_dup(line, &remain));
	}
	//なければreadを繰り返し、joinして改行があるかを調べる繰り返し
	return (read_join(line, fd, &remain));
}

int		main(int argc, char **argv)
{
	char	*line;
	int		fd;
	int		ct;
	int		val;

	ct = 1;
	(void)argc;
	fd = open(argv[1], O_RDONLY);
	while ((val = get_next_line(fd, &line)) >= 0)
	{
		printf("val = %d, line %d:%s\n", val, ct, line);
		free(line);
		line = NULL;
		ct++;
		if (val == 0)
			break;
	}
	if (val == ERROR)
	{
		printf("error");
	}
	close(fd);

	return (0);
}
