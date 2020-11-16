#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#define MAX(x, y) (x >= y) ? x : y
#define MIN(x, y) (x >= y) ? y : x

typedef struct
{
	int	total_len;
	int	width;
	int	precision;
}		t_flags;

static void	pnum(int num);
void		ft_putnbr(int num);
int			keta(int num);
void		ft_putstr(char *str);
void		ft_putchar(char ch);
void		ft_putnbr16(unsigned int num);
int			keta16(unsigned int num);
void		put_it_x_times(char ch, int times);
void		output_x(t_flags *flags, va_list *ap);
void		output_s(t_flags *flags, va_list *ap);
void		output_d(t_flags *flags, va_list *ap);
void		parse_width(char **fmt, t_flags *flags);
void		parse_fmt(char **fmt, t_flags *flags, va_list *ap);
void		parse_precision(char **fmt, t_flags *flags);
void		parse_type(char **fmt, t_flags *flags, va_list *ap);
void		init_flags(t_flags *flags);
int			ft_strlen(char *str);

void	init_flags(t_flags *flags)
{
	flags->width = 0;
	flags->precision = -1;
}

int		ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

int			ft_printf(char *fmt, ...)
{
	va_list	ap;
	t_flags	flags;

	flags.total_len = 0;
	//va_startを忘れがち
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
	return (flags.total_len);
}

void		parse_fmt(char **fmt, t_flags *flags, va_list *ap)
{
	while (**fmt == '0')
		(*fmt)++;
	if ('1' <= **fmt && **fmt <= '9')
	{
		parse_width(fmt, flags);
	}
	if (**fmt == '.')
	{
		(*fmt)++;
		parse_precision(fmt, flags);
	}
	parse_type(fmt, flags, ap);
}


void		parse_width(char **fmt, t_flags *flags)
{
	int	total = 0; 
	while ('0' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt) - '0';
		(*fmt)++;
	}
	flags->width = total;
}

void		parse_precision(char **fmt, t_flags *flags)
{
	int	total = 0; 
	while ('0' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt) - '0';
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
	int num = va_arg(*ap, int);
	int actual_numlen = MAX(flags->precision, keta(num));
	if (flags->precision == 0 && num == 0)
		actual_numlen = 0;
		//ここをnumじゃない数値にしちゃだめ（actual_numlenなど)
	actual_numlen += (num < 0) ? 1 : 0;
	int actual_width = MAX(flags->width, actual_numlen);

	put_it_x_times(' ', actual_width - actual_numlen);
	if (num < 0)
		ft_putchar('-');
	put_it_x_times('0', actual_numlen - keta(num) - (num < 0 ? 1 : 0));
	if (!(flags->precision == 0 && num == 0))
		ft_putnbr(num);
	flags->total_len += actual_width;
}

void		output_x(t_flags *flags, va_list *ap)
{
	unsigned int	num = va_arg(*ap, unsigned int);
	int		actual_numlen = MAX(flags->precision, keta16(num));
	if (flags->precision == 0 && num == 0)
		actual_numlen = 0;
	int		actual_width = MAX(flags->width, actual_numlen);

	put_it_x_times(' ', actual_width - actual_numlen);
	put_it_x_times('0', actual_numlen - keta16(num));
	if (!(flags->precision == 0 && num == 0))
		ft_putnbr16(num);
	flags->total_len += actual_width;
}

void		output_s(t_flags *flags, va_list *ap)
{
	char *str = va_arg(*ap, char*);
	if (str == NULL)
		str = "(null)";
	int actual_strlen;
	if (flags->precision == -1)
		actual_strlen = ft_strlen(str);
	else
		actual_strlen = MIN(flags->precision, ft_strlen(str));
	int actual_width = MAX(actual_strlen, flags->width);

	put_it_x_times(' ', actual_width - actual_strlen);
	write(1, str, actual_strlen);
}

void		put_it_x_times(char ch, int times)
{
	for (int i = 0; i < times; i++)
		write(1, &ch, 1);
}

int			keta16(unsigned int num)
{
	int keta = 1;
	while (num >= 16)
	{
		num /= 16;
		keta++;
	}
	return (keta);
}

void		ft_putnbr16(unsigned int num)
{
	if (num < 16)
	{
		ft_putchar("0123456789abcdef"[num]);
	}
	else
	{
		ft_putnbr16(num / 16);
		ft_putchar("0123456789abcdef"[num % 16]);
	}
}

void		ft_putchar(char ch)
{
	write(1, &ch, 1);
}

void		ft_putstr(char *str)
{
	for (int i = 0; str[i]; i++)
		write(1, &str[i], 1);
}

int			keta(int num)
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

void		ft_putnbr(int num)
{
	if (num == INT_MIN)
	{
		//ここマイナスつけちゃダメ
		ft_putstr("2147483648");
		return ;
	}
	if (num < 0)
		num *= -1;
	pnum(num);
}

static void	pnum(int num)
{
	if (num < 10)
	{
		ft_putchar(num + '0');
	}
	else
	{
		pnum(num / 10);
		ft_putchar(num % 10 + '0');
	}
}


#include <stdio.h>
int			main(void)
{
	ft_printf("[%1.3s]\n", NULL);
	   printf("[%1.3s]\n", NULL);
}
