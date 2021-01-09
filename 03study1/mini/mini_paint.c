
#define ERROR -1
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define PASS 30
#define SHOULD_DRAW 31
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
typedef struct {
	int width;
	int height;
	char paint;
} t_bg;
typedef struct {
	char type;
	float x;
	float y;
	float radius;
	char paint;
} t_circle;

int		draw_each_circle(t_circle *circle, t_bg *bg, char *canvas);
int		draw_circles(char *canvas, t_bg *bg, FILE *fp);
int		print_error_msg(char *msg, int err_val);
void	ft_putchar_fd(char ch, int fd);
void	ft_putstr_fd(char *str, int fd);
int		check_format(t_circle *circle, t_bg *bg, int i);
int		close_with_error_msg(FILE *fp, char *msg, int err_val);
void	fill_back_ground(char *canvas, t_bg *bg);
void	print_canvas(char *canvas, t_bg *bg);
int		free_and_close_with_error_msg(char *canvas, FILE *fp, char *msg, int err_val);
int		is_bg_invalid_value(t_bg *bg);

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

int		free_and_close_with_error_msg(char *canvas, FILE *fp, char *msg, int err_val)
{
	free(canvas);
	fclose(fp);
	return (print_error_msg(msg, err_val));
}

int		draw_circles(char *canvas, t_bg *bg, FILE *fp)
{
	t_circle	circle;
	int			info_num;

	info_num = fscanf(fp, "%c %f %f %f %c\n",
		&circle.type, &circle.x, &circle.y, &circle.radius, &circle.paint);
	if (info_num != 5)
		return (ERROR);
	if (circle.radius <= 0)
		return (ERROR);
	while (info_num == 5)
	{
		if (draw_each_circle(&circle, bg, canvas) == ERROR)
			return (ERROR);
		info_num = fscanf(fp, "%c %f %f %f %c\n",
			&circle.type, &circle.x, &circle.y, &circle.radius, &circle.paint);
		if (circle.radius <= 0)
			return (ERROR);
	}
	return (SUCCESS);
}

int		draw_each_circle(t_circle *circle, t_bg *bg, char *canvas)
{
	int		check_result;

	for (int i = 0; i < bg->width * bg->height; i++)
	{
		check_result = check_format(circle, bg, i);
		if (check_result == ERROR)
			return (ERROR);
		else if (check_result == SHOULD_DRAW)
			canvas[i] = circle->paint;
	}
	return (PASS);
}

int		check_format(t_circle *circle, t_bg *bg, int i)
{
	int		is_in_circle;
	int		i_x = i % bg->width;
	int		i_y = i / bg->width;
	float	dist_from_center_squared;
	float	dist_from_center;

	dist_from_center_squared =
		powf(i_x - circle->x, 2) + powf(i_y - circle->y, 2);
	dist_from_center = sqrtf(dist_from_center_squared);
	is_in_circle = (dist_from_center_squared <= powf(circle->radius, 2));
	if (circle->type == 'C')
	{
		if (is_in_circle)
			return (SHOULD_DRAW);
	}
	else if (circle->type == 'c')
	{
		if (is_in_circle && circle->radius - dist_from_center < 1)
			return (SHOULD_DRAW);
	}
	else
		return (ERROR);
	return (PASS);
}