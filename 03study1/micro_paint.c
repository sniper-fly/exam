#define ERROR -1
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define SHOULD_DRAW 31
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>

typedef struct {
	int width;
	int height;
	char paint;
} t_bg;

typedef struct {
	char type;
	float x;
	float y;
	float width;
	float height;
	char paint;
} t_rect;

int		print_error_msg(char *msg, int err_val);
void	ft_putchar_fd(char ch, int fd);
void	ft_putstr_fd(char *str, int fd);
int		close_with_error_msg(FILE *fp, char *msg, int err_val);
void	fill_back_ground(char *canvas, t_bg *bg);
void	print_canvas(char *canvas, t_bg *bg);
int		check_format(int i, t_bg *bg, t_rect *rect);
int		draw_each_circle(char *canvas, t_bg *bg, t_rect *rect);
int		draw_circles(char *canvas, t_bg *bg, FILE *fp);
int		free_and_close_with_error_msg(char *canvas, FILE *fp, char *msg, int err_val);
int		is_bg_invalid_value(t_bg *bg);
int		my_ceil(float num);

int		main(int argc, char **argv)
{
	t_bg bg;

	if (argc != 2)
		return (print_error_msg("Error: argment\n", ERROR));
	char *file_name = argv[1];
	FILE *fp = fopen(file_name, "r");
	if (fp == NULL)
		return (print_error_msg("Error: cannot open file\n", ERROR));
	if (fscanf(fp, "%d %d %c\n", &bg.width, &bg.height, &bg.paint) != 3)
		return (close_with_error_msg(fp, "Error: format\n", ERROR));
	if (is_bg_invalid_value(&bg))
		return (close_with_error_msg(fp, "Error: too big width/height\n", ERROR));
	char *canvas = (char*)malloc(sizeof(char) * (bg.width * bg.height + 1));
	if (canvas == NULL)
		return (close_with_error_msg(fp, "Error: cannnot allocate memory", ERROR));
	fill_back_ground(canvas, &bg);
	if (draw_circles(canvas, &bg, fp) == ERROR)
		return (free_and_close_with_error_msg(canvas, fp, "Error: format\n", ERROR));
	print_canvas(canvas, &bg);
	fclose(fp);
	free(canvas);
}

int		print_error_msg(char *msg, int err_val)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (err_val);
}

void	ft_putchar_fd(char ch, int fd)
{
	write(fd, &ch, 1);
}

void	ft_putstr_fd(char *str, int fd)
{
	if (str == NULL)
		return ;
	for (int i = 0; str[i]; i++) {
		write(fd, &str[i], 1);
	}
}

int		close_with_error_msg(FILE *fp, char *msg, int err_val)
{
	fclose(fp);
	return (print_error_msg(msg, err_val));
}

void	fill_back_ground(char *canvas, t_bg *bg)
{
	for (int i = 0; i < bg->width * bg->height; i++)
		canvas[i] = bg->paint;
}

void	print_canvas(char *canvas, t_bg *bg)
{
	for (int i = 0; i < bg->height; i++)
	{
		for (int j = 0; j < bg->width; j++)
		{
			ft_putchar_fd(canvas[i * bg->width + j], STDOUT_FILENO);
		}
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
}

int		is_bg_invalid_value(t_bg *bg)
{
	if (!(0 < bg->width && bg->width <= 300))
		return (TRUE);
	else if (!(0 < bg->height && bg->height <= 300))
		return (TRUE);
	else
		return (FALSE);
}

int		draw_circles(char *canvas, t_bg *bg, FILE *fp)
{
	t_rect rect;
	int	info_num;

	info_num = fscanf(fp, "%c %f %f %f %f %c\n",
	&rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.paint);
	if (info_num != 6)
		return (ERROR);
	while (info_num == 6)
	{
		int retval = draw_each_circle(canvas, bg, &rect);
		if (retval == ERROR)
			return (ERROR);
		info_num = fscanf(fp, "%c %f %f %f %f %c\n",
		&rect.type, &rect.x, &rect.y, &rect.width, &rect.height, &rect.paint);
	}
	return (SUCCESS);
}

int		free_and_close_with_error_msg(char *canvas, FILE *fp, char *msg, int err_val)
{
	free(canvas);
	fclose(fp);
	return (print_error_msg(msg, err_val));
}

int		draw_each_circle(char *canvas, t_bg *bg, t_rect *rect)
{
	int check_flag;

	for (int i = 0; i < bg->width * bg->height; i++)
	{
		if ((check_flag = check_format(i, bg, rect)) == ERROR)
			return (ERROR);
		else if (check_flag == SHOULD_DRAW)
			canvas[i] = rect->paint;
	}
	return (SUCCESS);
}

int		check_format(int i, t_bg *bg, t_rect *rect)
{
	int is_in_rect;
	int i_x = i % bg->width;
	int i_y = i / bg->width;

	is_in_rect = (rect->x <= i_x && i_x <= rect->x + rect->width
				&& rect->y <= i_y && i_y <= rect->y + rect->height);
	if (rect->type == 'r')
	{
		if ((my_ceil(rect->x) == i_x && is_in_rect) ||
			(my_ceil(rect->y) == i_y && is_in_rect) ||
			((int)(rect->x + rect->width) == i_x && is_in_rect) ||
			((int)(rect->y + rect->height) == i_y && is_in_rect))
			return (SHOULD_DRAW);
	}
	else if (rect->type == 'R')
	{
		if (is_in_rect)
			return (SHOULD_DRAW);
	}
	else
		return (ERROR);
	return (SUCCESS);
}

int		my_ceil(float num)
{
	int inum = (int)num;
	if (num == (float)inum)
		return ((int)num);
	else
		return ((int)(num + 1));
}