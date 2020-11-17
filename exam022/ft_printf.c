#include <unistd.h>
#include <stdarg.h>
#include <limits.h>

typedef struct
{
	int total_len;
	int width;
	int precision;
} t_flags;

void	output_s(t_flags *flags, va_list *ap);
int		ft_strlen(char *str);
int		keta(int num);
void	output_d(t_flags *flags, va_list *ap);
void	ft_putnbr(int num);
void	pnum(int num);
void	put_nbr16(unsigned int num);
void	put_it_x_times(char ch, int times);
int		keta16(unsigned int num);
void	output_x(t_flags *flags, va_list *ap);
void	parse_type(char **fmt, t_flags *flags, va_list *ap);
void	parse_precision(char **fmt, t_flags *flags);
void	parse_width(char **fmt, t_flags *flags);
void	parse_fmt(char **fmt, t_flags *flags, va_list *ap);
int		ft_printf(char *fmt, ...);
void	ft_putstr(char *str);
void	ft_putchar(char ch);
void	init_flags(t_flags *flags);

#define MAX(x, y) (x <= y) ? y : x
#define MIN(x, y) (x <= y) ? x : y

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
			ft_putchar(*fmt);
			flags.total_len++;
			fmt++;
		}
	}
	va_end(ap);
	return (flags.total_len);
}

void	init_flags(t_flags *flags)
{
	flags->precision = -1;
	flags->width = 0;
}

void	ft_putchar(char ch)
{
	write(1, &ch, 1);
}

void	ft_putstr(char *str)
{
	for (int i = 0; str[i]; i++)
		ft_putchar(str[i]);
}

void	parse_fmt(char **fmt, t_flags *flags, va_list *ap)
{
	while (**fmt == '0')
		(*fmt)++;
	if ('1' <= **fmt && **fmt <= '9')
		parse_width(fmt, flags);
	if (**fmt == '.')
	{
		(*fmt)++;
		parse_precision(fmt, flags);
	}
	parse_type(fmt, flags, ap);
}

void	parse_width(char **fmt, t_flags *flags)
{
	int total = 0;
	while ('0' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt - '0');
		(*fmt)++;
	}
	flags->width = total;
}

void	parse_precision(char **fmt, t_flags *flags)
{
	int total = 0;
	while ('0' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt - '0');
		(*fmt)++;
	}
	flags->precision = total;
}

void	parse_type(char **fmt, t_flags *flags, va_list *ap)
{
	if (**fmt == 'd')
		output_d(flags, ap);
	else if (**fmt == 'x')
		output_x(flags, ap);
	else if (**fmt == 's')
		output_s(flags, ap);
	(*fmt)++;
}

void	output_x(t_flags *flags, va_list *ap)
{
	unsigned int num = va_arg(*ap, unsigned int);
	int actual_numlen = MAX(flags->precision, keta16(num));
	if (flags->precision == 0 && num == 0)
		actual_numlen = 0;
	int actual_width = MAX(actual_numlen, flags->width);

	put_it_x_times(' ', actual_width - actual_numlen);
	put_it_x_times('0', actual_numlen - keta16(num));
	if (!(flags->precision == 0 && num == 0))
		put_nbr16(num);
	flags->total_len += actual_width;
}

int		keta16(unsigned int num)
{
	int keta = 1;
	while (num >= 16)
	{
		num /= 16;
		keta++;
	}
	return (keta);
}

void	put_it_x_times(char ch, int times)
{
	for (int i = 0; i < times; i++)
		ft_putchar(ch);
}

void	put_nbr16(unsigned int num)
{
	if (num < 16)
		ft_putchar("0123456789abcdef"[num]);
	else
	{
		put_nbr16(num / 16);
		ft_putchar("0123456789abcdef"[num % 16]);
	}
}

void	output_d(t_flags *flags, va_list *ap)
{
	int num = va_arg(*ap, int);
	int actual_numlen = MAX(flags->precision, keta(num));
	if (num < 0)
		actual_numlen++;
	if (flags->precision == 0 && num == 0)
		actual_numlen = 0;
	int actual_width = MAX(flags->width, actual_numlen);

	put_it_x_times(' ', actual_width - actual_numlen);
	if (num < 0)
		ft_putchar('-');
	put_it_x_times('0', actual_numlen - keta(num) - (num < 0 ? 1 : 0));
	if (!(flags->precision == 0 && num == 0))
		ft_putnbr(num);
	flags->total_len += actual_width;
}

void	ft_putnbr(int num)
{
	if (num == INT_MIN)
		ft_putstr("");
	if (num < 0)
		num *= -1;
	pnum(num);
}

void	pnum(int num)
{
	if (num < 10)
		ft_putchar(num + '0');
	else
	{
		pnum(num / 10);
		ft_putchar(num % 10 + '0');
	}
}

int		keta(int num)
{
	if (num == INT_MIN)
		return (keta(INT_MAX));
	if (num < 0)
		num *= -1;
	
	int keta = 1;
	while (num >= 10)
	{
		num /= 10;
		keta++;
	}
	return (keta);
}

void	output_s(t_flags *flags, va_list *ap)
{
	char *str = va_arg(*ap, char*);
	if (str == NULL)
		str = "(null)";
	int actual_strlen;
	if (flags->precision == -1)
		actual_strlen = ft_strlen(str);
	else
		actual_strlen = MIN(ft_strlen(str), flags->precision);
	int actual_width = MAX(actual_strlen, flags->width);

	put_it_x_times(' ', actual_width - actual_strlen);
	write(1, str, actual_strlen);
}

int		ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

#include <stdio.h>
int		main(void)
{
	ft_printf("[%10.3s]\n", NULL);
	   printf("[%10s]\n", "hello_world");
	return (0);
}
