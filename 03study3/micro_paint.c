#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#define ERROR -1
#define PASS 31
#define SHOULD_DRAW 32
#define TRUE 1
#define FALSE 0

typedef struct {
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	paint;
}	t_rect;

typedef struct {
	int	width;
	int	height;
	char	paint;
}	t_bg;

int		my_ceil(float num);
int		check_format(t_bg *bg, t_rect *rect, int i);
int		draw_each_rect(t_bg *bg, char *canvas, t_rect *rect);
int		is_rect_valid_range(t_rect *rect);
int		draw_rectangles(FILE *fp, t_bg *bg, char *canvas);
void	print_canvas(t_bg *bg, char *canvas);
int		free_close_with_error(FILE *fp, char *canvas, char *msg);
int		close_with_error(FILE *fp, char *msg);
void	fill_canvas_background(t_bg *bg, char *canvas);
int		is_bg_valid_range(t_bg *bg);
int		print_error(char *msg);
void	ft_putchar_fd(char ch, int fd);
void	ft_putstr_fd(char *str, int fd);

int		main(int argc, char **argv)
{
	FILE	*fp;
	char	*filename = argv[1];
	t_bg	bg;
	char	*canvas;

	if (argc != 2)
		return (print_error("argument"));
	fp = fopen(filename, "r");
	if (fp == NULL)
		return (print_error("cannot open file"));
	if (fscanf(fp, "%d %d %c\n", &bg.width, &bg.height, &bg.paint) != 3)
		return (close_with_error(fp, "format"));
	if (!is_bg_valid_range(&bg))
		return (close_with_error(fp, "first line invalid range"));
	canvas = (char*)malloc(sizeof(char) * (bg.width * bg.height));
	if (canvas == NULL)
		return (close_with_error(fp, "cannot allocate memory"));
	fill_canvas_background(&bg, canvas);
	if (draw_rectangles(fp, &bg, canvas) == ERROR)
		return (free_close_with_error(fp, canvas, "direction format"));

	print_canvas(&bg, canvas);

	free(canvas);
	fclose(fp);
}

int		draw_each_rect(t_bg *bg, char *canvas, t_rect *rect)
{
	int		check_res;

	for (int i = 0; i < bg->width * bg->height; i++)
	{
		check_res = check_format(bg, rect, i);
		if (check_res == ERROR)
			return (ERROR);
		else if (check_res == SHOULD_DRAW)
			canvas[i] = rect->paint;
	}
	return (PASS);
}

int		check_format(t_bg *bg, t_rect *rect, int i)
{
	int		i_x = i % bg->width;
	int		i_y = i / bg->height;
	int		is_in_rect;
	int		is_on_border;
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
		is_on_border = (
			i_x == my_ceil(rect->x)
			|| i_x == (int)x_end
			|| i_y == my_ceil(rect->y)
			|| i_y == (int)y_end
		);
		if (is_in_rect && is_on_border)
			return (SHOULD_DRAW);
	}
	else
		return (ERROR);
	return (PASS);
}

int		my_ceil(float num)
{
	int		inum = (int)num;
	if ((float)inum == num)
		return ((int)num);
	else
		return ((int)(num + 1));
}

int		is_rect_valid_range(t_rect *rect)
{
	return (rect->width > 0 && rect->height > 0);
}

int		draw_rectangles(FILE *fp, t_bg *bg, char *canvas)
{
	t_rect	rect;
	int		scanf_res;

	scanf_res = fscanf(fp, "%c %f %f %f %f %c\n",
		&rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.paint);
	if (scanf_res == -1)
		return (PASS);
	else if (scanf_res != 6)
		return (ERROR);
	while (scanf_res == 6)
	{
		if (!is_rect_valid_range(&rect))
			return (ERROR);
		if (draw_each_rect(bg, canvas, &rect) == ERROR)
			return (ERROR);
		scanf_res = fscanf(fp, "%c %f %f %f %f %c\n",
			&rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.paint);
	}
	// printf("%d\n", scanf_res);
	if (scanf_res != -1)
		return (ERROR);
	return (PASS);
}

void	print_canvas(t_bg *bg, char *canvas)
{
	for (int i = 0; i < bg->height; i++)
	{
		for (int j = 0; j < bg->width; j++)
			ft_putchar_fd(canvas[i * bg->width + j], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
}

int		free_close_with_error(FILE *fp, char *canvas, char *msg)
{
	free(canvas);
	return (close_with_error(fp, msg));
}

int		close_with_error(FILE *fp, char *msg)
{
	fclose(fp);
	return (print_error(msg));
}

void	fill_canvas_background(t_bg *bg, char *canvas)
{
	for (int i = 0; i < bg->width * bg->height; i++)
		canvas[i] = bg->paint;
}

int		is_bg_valid_range(t_bg *bg)
{
	return (0 < bg->width && bg->width <= 300
		&& 0 < bg->height && bg->height <= 300);
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