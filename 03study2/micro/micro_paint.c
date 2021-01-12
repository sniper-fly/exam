#define TRUE 1
#define FALSE 0
#define PASS 31
#define SUCCESS 32
#define SHOULD_DRAW 33
#define ERROR -1

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
typedef struct
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	paint;
}	t_rect;
typedef struct
{
	int		width;
	int		height;
	char	paint;
}	t_bg;
void	print_canvas(t_bg *bg, char *canvas);
int		my_ceil(float num);
int		check_format(t_rect *rect, t_bg *bg, int i);
int		draw_each_rectangle(t_bg *bg, char *canvas, t_rect *rect);
int		draw_rectangles(FILE *fp, t_bg *bg, char *canvas);
int		free_close_with_p_error(char *msg, FILE *fp, char *canvas);
int		is_bg_valid_value(t_bg *bg);
void	fill_background_paint(char *canvas, t_bg *bg);
int		print_error(char *msg);
void	ft_putchar_fd(char ch, int fd);
void	ft_putstr_fd(char *str, int fd);
int		close_with_p_error(char *msg, FILE *fp);

int		main(int argc, char **argv)
{
	FILE	*fp;
	char	*filename;
	t_bg	bg;
	char	*canvas;

	if (argc != 2)
		return (print_error("argument"));
	filename = argv[1];
	fp = fopen(filename, "r");
	if (fp == NULL)
		return (print_error("cannot open the file"));
	if (fscanf(fp, "%d %d %c\n", &bg.width, &bg.height, &bg.paint) != 3)
		return (close_with_p_error("format", fp));
	if (!is_bg_valid_value(&bg))
		return (close_with_p_error("invalid value range", fp));
	canvas = (char*)malloc(sizeof(char) * (bg.width * bg.height));
	if (canvas == NULL)
		return (close_with_p_error("cannot allocate memory", fp));
	fill_background_paint(canvas, &bg);
	if (draw_rectangles(fp, &bg, canvas) == ERROR)
		return (free_close_with_p_error("rule format", fp, canvas));
	print_canvas(&bg, canvas);
	free(canvas);
	fclose(fp);
}

void	ft_putchar_fd(char ch, int fd)
{
	write(fd, &ch, 1);
}

void	ft_putstr_fd(char *str, int fd)
{
	if (str == NULL)
		return ;
	for (int i = 0; str[i]; i++)
		ft_putchar_fd(str[i], fd);
}

int		print_error(char *msg)
{
	ft_putstr_fd("Error: ", STDERR_FILENO);
	ft_putstr_fd(msg, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);
	return (ERROR);
}

int		close_with_p_error(char *msg, FILE *fp)
{
	fclose(fp);
	return (print_error(msg));
}

void	fill_background_paint(char *canvas, t_bg *bg)
{
	for (int i = 0; i < bg->width * bg->height; i++)
		canvas[i] = bg->paint;
}

int		free_close_with_p_error(char *msg, FILE *fp, char *canvas)
{
	free(canvas);
	return (close_with_p_error(msg, fp));
}

int		is_bg_valid_value(t_bg *bg)
{
	return (0 < bg->width && bg->width <= 300
			&& 0 < bg->height && bg->height <= 300);
}

int		draw_rectangles(FILE *fp, t_bg *bg, char *canvas)
{
	t_rect	rect;
	int		scanf_ret;
	int		is_first = TRUE;

	do
	{
		scanf_ret = fscanf(fp, "%c %f %f %f %f %c",
			&rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.paint);
		if (is_first && scanf_ret != 6)
			return (ERROR);
		is_first = FALSE;
		if (rect.width <= 0 || rect.height <= 0)
			return (ERROR);
		if (draw_each_rectangle(bg, canvas, &rect) == ERROR)
			return (ERROR);
	} while (scanf_ret == 6);
	return (SUCCESS);
}

int		draw_each_rectangle(t_bg *bg, char *canvas, t_rect *rect)
{
	int check_result;

	for (int i = 0; i < bg->width * bg->height; i++)
	{
		check_result = check_format(rect, bg, i);
		if (check_result == ERROR)
			return (ERROR);
		else if (check_result == SHOULD_DRAW)
			canvas[i] = rect->paint;
	}
	return (SUCCESS);
}

int		check_format(t_rect *rect, t_bg *bg, int i)
{
	int	i_x = i % bg->width;
	int	i_y = i / bg->width;
	int	is_in_rect;
	int	is_in_border;
	float	x_end = rect->x + rect->width;
	float	y_end = rect->y + rect->height;

	is_in_rect = (rect->x <= i_x && i_x <= x_end
		&& rect->y <= i_y && i_y <= y_end);
	if (rect->type == 'R')
	{
		if (is_in_rect)
			return (SHOULD_DRAW);
	}
	else if (rect->type == 'r')
	{
		is_in_border = (
			i_x == my_ceil(rect->x)
			|| i_x == (int)x_end
			|| i_y == my_ceil(rect->y)
			|| i_y == (int)y_end
		);
		if (is_in_rect && is_in_border)
			return (SHOULD_DRAW);
	}
	else
		return (ERROR);
	return (PASS);
}

int		my_ceil(float num)
{
	int	inum = (int)num;

	if ((float)inum == num)
		return ((int)num);
	else
		return ((int)(num + 1));
	
}

void	print_canvas(t_bg *bg, char *canvas)
{
	for (int i = 0; i < bg->height; i++)
		for (int j = 0; j < bg->width; j++)
			ft_putchar_fd(canvas[i * bg->width + j], STDOUT_FILENO); 
}