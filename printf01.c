#include <stdarg.h>
#include <unistd.h>

#define MAX(x, y) (x <= y) ? y : x
#define MIN(x, y) (x <= y) ? x : y

typedef struct {
	int		width;
	int		precision;
	int		total_len;
} t_flags;

int		init_flags(t_flags *flags)
{
	flags->width = 0;
	flags->precision = 0;
}

int		ft_printf(char *fmt, ...)
{
	va_list ap;
	t_flags flags;

	flags.total_len = 0;
	va_start(ap, fmt);
	while (*fmt)
	{
		if (*fmt == '%')
		{
			fmt++;
			init_flags(&flags);
			parse_fmt(&fmt, &flags, &ap);
		}
		else
		{
			write(1, fmt, 1);
			flags.total_len++;
			fmt++;
		}
	}
	va_end(ap);
}

void		parse_fmt(char **fmt, t_flags *flags, va_list *ap)
{
	while (**fmt == '0')
		(*fmt)++;
	if ('1' <= **fmt && **fmt <= '9')
		set_width(fmt, flags);
	if (**fmt == '.')
	{
		(*fmt)++;
		set_precision(fmt, flags);
	}
	parse_type(fmt, flags, ap);
}

void		set_width(char **fmt, t_flags *flags)
{
	int		total = 0;

	while ('1' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt - '0');
		(*fmt)++;
	}
	flags->width = total;
}

void		set_precisition(char **fmt, t_flags *flags)
{
	int		total = 0;

	while ('1' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt - '0');
		(*fmt)++;
	}
	flags->precision = total;
}

void		parse_type(char **fmt, t_flags *flags, va_list *ap)
{
	if (**fmt == 'd')
		output_d(flags, ap);
	else if (**fmt == 'x')
		output_x(flags, ap);
	else if (**fmt == 's')
		output_s(flags, ap);
	(*fmt)++;
}

void		output_d(t_flags *flags, va_list *ap)
{
	int	num = va_arg(*ap, int);
	int		actual_numlen = MAX(keta(num), flags->precision) + (num < 0) ? 1 : 0;
	int		actual_width = MAX(actual_numlen, flags->width);

	put_it_x_times(' ', actual_width - actual_numlen);
	if (num < 0)
		ft_putchar('-');
	put_it_x_times('0', actual_numlen - keta(num) - (num < 0) ? 1 : 0);
}