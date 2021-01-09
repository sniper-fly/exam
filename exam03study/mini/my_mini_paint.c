typedef struct
{
	char	type;
	float	x;
	float	y;
	float	radius;
	char	paint;
} t_circle;

typedef struct
{
	int		width;
	int		height;
	char	paint;
} t_bg;

#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int		close_with_error_msg(FILE *fp, char *msg);
void	ft_putstr_fd(char *str, int fd);
int		close_and_free_with_error_msg(FILE *fp, char *canvas, char *msg);
void	fill_background_paint(t_bg *bg, char *canvas);
int		show_error_msg(char *msg);
void	print_canvas(char *canvas, t_bg *bg);
void	ft_putchar_fd(char ch, int fd);
int		draw_circle(FILE *fp, char *canvas, t_bg *bg);
int		draw_each_circle(char *canvas, t_bg *bg, t_circle *circle);
int		check_format(t_circle *circle, t_bg *bg, int i);

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define ERROR -1

int		main(int argc, char **argv)
{
	FILE		*fp;
	t_bg		bg;
	char		*canvas;

	if (argc != 2)
		return (show_error_msg("Error: argument\n"));
	if ((fp = fopen(argv[1], "r")) == NULL)
		return (show_error_msg("Error: cannot open file\n"));
	if (fscanf(fp, "%d %d %c\n", &bg.width, &bg.height, &bg.paint) != 3)
		return (close_with_error_msg(fp, "Error: fscanf failed\n"));
	if ((canvas = (char*)malloc(sizeof(char) * (bg.width * bg.height + 1))) == NULL)
		return (close_and_free_with_error_msg(fp, canvas, "Error: cannot allocate memory\n"));
	fill_background_paint(&bg, canvas);
	if (draw_circle(fp, canvas, &bg) == ERROR)
		return (close_and_free_with_error_msg(fp, canvas, "Error: format\n"));
	print_canvas(canvas, &bg);
	free(canvas);
	fclose(fp);
}

int		close_with_error_msg(FILE *fp, char *msg)
{
	fclose(fp);
	return (show_error_msg(msg));
}

void	ft_putstr_fd(char *str, int fd)
{
	if (str == NULL)
		return ;
	for (int i = 0; str[i]; i++)
		write(fd, &str[i], 1);
}

int		show_error_msg(char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	return (ERROR);
}

int		close_and_free_with_error_msg(FILE *fp, char *canvas, char *msg)
{
	fclose(fp);
	free(canvas);
	return (show_error_msg(msg));
}

void	fill_background_paint(t_bg *bg, char *canvas)
{
	int i = 0;
	for (i = 0; i < bg->width * bg->height; i++)
		canvas[i] = bg->paint;
	// canvas[i] = '\n';
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

int		draw_circle(FILE *fp, char *canvas, t_bg *bg)
{
	t_circle	circle;

	while (fscanf(fp, "%c %f %f %f %c\n",
		&circle.type, &circle.x, &circle.y, &circle.radius, &circle.paint) == 5)
	{
		if (draw_each_circle(canvas, bg, &circle) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

int		draw_each_circle(char *canvas, t_bg *bg, t_circle *circle)
{
	int			should_draw;

	for (int i = 0; i < bg->width * bg->height; i++)
	{
		if ((should_draw = check_format(circle, bg, i)) == ERROR)
			return (ERROR);
		else if (should_draw)
			canvas[i] = circle->paint;
	}
	return (SUCCESS);
}

int		check_format(t_circle *circle, t_bg *bg, int i)
{
	int		is_in_circle;
	int		i_x = i % bg->width;
	int		i_y = i / bg->width;
	float	distance_from_center_sqrd;
	float	distance_from_center;

	distance_from_center_sqrd = powf(i_x - circle->x, 2) + powf(i_y - circle->y, 2);
	is_in_circle =
		(distance_from_center_sqrd <= powf(circle->radius, 2));
	distance_from_center = sqrtf(distance_from_center_sqrd);
	if (circle->type == 'C')
	{
		return (is_in_circle);
	}
	else if (circle->type == 'c')
	{
		return (is_in_circle && circle->radius - distance_from_center < 1);
	}
	else
		return (ERROR);
}

void	ft_putchar_fd(char ch, int fd)
{
	write(fd, &ch, 1);
}