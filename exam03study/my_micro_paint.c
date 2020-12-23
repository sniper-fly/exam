#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <stdlib.h>

int		is_dicimal(double num);
int		should_draw(char mode, int i, double x, double y, double width, double height, int c_wid);
void	ft_putstr_fd(int fd, char *str);
void	ft_putchar_fd(int fd, char ch);
void	print_canvas(int width, int height, char *canvas);
void	draw_canvas_art(FILE *fp, int c_wid, int c_hei, char *canvas);
void	draw_canvas_bg(int width, int height, char *canvas, char bg_ch);
void	draw_each_art(int c_wid, int c_hei, char *canvas,
		char mode, double x, double y, double width, double height, char paint_ch);

#define ERROR -1
#define TRUE 1
#define FALSE 0

void	ft_putstr_fd(int fd, char *str)
{
	int	i = 0;
	if (str == NULL)
		return ;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}

int		main(int argc, char **argv)
{
	FILE	*fp;
	int		width, height;
	char	bg_ch;
	int		value_num;
	char	*canvas;

	if (argc != 2)
	{
		ft_putstr_fd(STDERR_FILENO, "Error: argument\n");
		return (ERROR);
	}
	fp = fopen(argv[1], "r");
	if (fp == NULL)
	{
		ft_putstr_fd(STDERR_FILENO, "Error: cannot open file\n");
		return (ERROR);
	}
	value_num = fscanf(fp, "%d %d %c\n", &width, &height, &bg_ch);
	if (value_num != 3)
	{
		ft_putstr_fd(STDERR_FILENO, "Error: format\n");
		return (ERROR);
	}
	canvas = (char *)malloc(sizeof(char) * (width * height + 1)); //not yet free
	if (canvas == NULL)
	{
		ft_putstr_fd(STDERR_FILENO, "Error: cannot allocate memory\n");
		return (ERROR);
	}
	draw_canvas_bg(width, height, canvas, bg_ch);
	draw_canvas_art(fp, width, height, canvas);
	print_canvas(width, height, canvas);
	//free
	//close file
}

void	draw_canvas_bg(int width, int height, char *canvas, char bg_ch)
{
	for (int i = 0; i < width * height; i++)
		canvas[i] = bg_ch;
}

void	draw_canvas_art(FILE *fp, int c_wid, int c_hei, char *canvas)
{
	int	value_num;
	double	x, y, width, height;
	char	mode;
	char	paint_ch;

	while ((value_num = fscanf(fp, "%c %lf %lf %lf %lf %c\n",
		&mode, &x, &y, &width, &height, &paint_ch)) == 6)
	{
		draw_each_art(c_wid, c_hei, canvas, mode, x, y, width, height, paint_ch);
	}
}

void	draw_each_art(int c_wid, int c_hei, char *canvas,
		char mode, double x, double y, double width, double height, char paint_ch)
{
	for (int i = 0; i < c_wid * c_hei; i++)
	{
		if (should_draw(mode, i, x, y, width, height, c_wid))
			canvas[i] = paint_ch;
	}
}

int		should_draw(char mode, int i, double x, double y, double width, double height, int c_wid)
{
	int	i_x = i % c_wid;
	int	i_y = i / c_wid;
	int	is_in_rect = (x <= i_x && i_x <= x + width && y <= i_y && i_y <= y + height);
	int	fmted_x;
	int fmted_y;

	if (mode == 'r')
	{
		fmted_x = (is_dicimal(x) ? (int)x + 1 : x);
		fmted_y = (is_dicimal(y) ? (int)y + 1 : y);
		return ((is_in_rect && fmted_x == i_x) ||
				(is_in_rect && fmted_y == i_y) ||
				(is_in_rect && i_x == (int)(x + width)) ||
				(is_in_rect && i_y == (int)(y + height)));
	}
	else if (mode == 'R')
	{
		return (is_in_rect);
	}
	else
	{
		return (FALSE);
	}
}

int		is_dicimal(double num)
{
	if (num / (int)num == 1)
		return (FALSE);
	else
		return (TRUE);
}

void	print_canvas(int width, int height, char *canvas)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			ft_putchar_fd(STDOUT_FILENO, canvas[i * width + j]);
		}
		ft_putchar_fd(STDOUT_FILENO, '\n');
	}
}

void	ft_putchar_fd(int fd, char ch)
{
	write(fd, &ch, 1);
}