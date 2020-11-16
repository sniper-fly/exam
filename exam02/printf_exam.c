#include <unistd.h>
#include <stdarg.h>
#include <stddef.h>
#include <limits.h>

#define MAX(a, b) (a <= b) ? b : a
#define MIN(a, b) (a <= b) ? a : b

typedef	struct {
	int	width;
	int	precision;
	int total_len;
}		t_flags;

int		ft_strlen(char *str);
void	ft_putchar(char ch);
void	ft_putstr(char *str);
void	ft_putnbr_16(unsigned int num);
void	ft_putnbr(int num);
int		keta_int(int num);
int		keta_ui16(unsigned int num);
void	put_it_xx_times(char ch, int times);
void	output_s(t_flags *flags, va_list *ap);
void	output_di(t_flags *flags, va_list *ap);
void	output_x(t_flags *flags, va_list *ap);
void	init_flags(t_flags *flags);
void	parse_fmt(char **fmt, t_flags *flags, va_list *ap);
int		ft_printf(char *fmt, ...);
void	parse_type(char **fmt, t_flags *flags, va_list *ap);
void	set_width(char **fmt, t_flags *flags);
void	set_precision(char **fmt, t_flags *flags);

int		ft_strlen(char *str) {
	int	len = 0;
	while (str[len]) {
		len++;
	}
	return (len);
}

void	ft_putnbr_16(unsigned int num) {
	if (num < 16) {
		ft_putchar("0123456789abcdef"[num]);
	} else {
		ft_putnbr_16(num / 16);
		ft_putchar("0123456789abcdef"[num % 16]);
	}
}

void	ft_putstr(char *str) {
	for (int i = 0; str[i]; i++) write(1, &str[i], 1);
}

void	ft_putchar(char ch) {
	write(1, &ch, 1);
}

void	pnum(int num) {
	if (num < 10) {
		ft_putchar(num + '0');
	} else {
		pnum(num / 10);
		ft_putchar(num % 10 + '0');
	}
}

void	ft_putnbr(int num) {
	if (num == INT_MIN) {
		ft_putstr("-1221121212");
		return ;
	} else if (num < 0) {
		num *= -1;
	}
	pnum(num);
}

void	init_flags(t_flags *flags)
{
	flags->width = 0;
	flags->precision = -1;
}

void	set_width(char **fmt, t_flags *flags)
{
	int	total = 0;
	while ('0' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt - '0');
		(*fmt)++;
	}
	flags->width = total;
}

void	set_precision(char **fmt, t_flags *flags)
{
	int	total = 0;

	while ('0' <= **fmt && **fmt <= '9')
	{
		total = total * 10 + (**fmt - '0');
		(*fmt)++;
	}
	flags->precision = total;
}

void	parse_type(char **fmt, t_flags *flags, va_list *ap)
{
	if (**fmt == 'd' || **fmt == 'i') {
		output_di(flags, ap);
	} else if (**fmt == 's') {
		output_s(flags, ap);
	} else if (**fmt == 'x') {
		output_x(flags, ap);
	}
	(*fmt)++;
}

int		keta_int(int num) {
	int	keta = 1;
	if (num == INT_MIN) return (keta_int(INT_MAX));
	if (num < 0) num *= -1;	
	while (num >= 10) {
		num /= 10;
		keta++;
	}
	return (keta);
}

int		keta_ui16(unsigned int num) {
	int keta = 1;
	while (num >= 16) {
		num /= 16;
		keta++;
	}
	return (keta);
}

void	put_it_xx_times(char ch, int times) {
	while (times) {
		write(1, &ch, 1);
		times--;
	}
}

void	output_di(t_flags *flags, va_list *ap)
{
	int	num = va_arg(*ap, int);
	int	actual_numlen = MAX(keta_int(num), flags->precision);
	if (num < 0) actual_numlen++;
	else if (flags->precision == 0 && num == 0) actual_numlen = 0;
	int	actual_width = MAX(actual_numlen, flags->width);

	put_it_xx_times(' ', actual_width - actual_numlen);
	if (num < 0) ft_putchar('-');
	put_it_xx_times('0', actual_numlen - keta_int(num) - ((num < 0) ? 1 : 0));
	ft_putnbr(num);
	flags->total_len += actual_width;
}

void	output_x(t_flags *flags, va_list *ap)
{
	int	num = va_arg(*ap, unsigned int);
	int	actual_numlen = MAX(keta_ui16(num), flags->precision);
	if (flags->precision == 0 && num == 0) actual_numlen = 0;
	int	actual_width = MAX(actual_numlen, flags->width);

	put_it_xx_times(' ', actual_width - actual_numlen);
	put_it_xx_times('0', actual_numlen - keta_ui16(num));
	ft_putnbr_16(num);
	flags->total_len += actual_width;
}

void	output_s(t_flags *flags, va_list *ap)
{
	char	*str = va_arg(*ap, char*);
	if (str == NULL) str = "(null)";
	int		len = ft_strlen(str);
	int		actual_strlen = (flags->precision == -1) ? len : MIN(len, flags->precision);
	int		actual_width = MAX(actual_strlen, flags->width);
	int		i = 0;

	put_it_xx_times(' ', actual_width - actual_strlen);
	while (actual_strlen) {
		write(1, &str[i], 1);
		i++;
		actual_strlen--;
	}
	flags->total_len += actual_width;
}

void	parse_fmt(char **fmt, t_flags *flags, va_list *ap)
{
	while (**fmt == '0')
		(*fmt)++;
	if ('1' <= **fmt && **fmt <= '9')
	{
		set_width(fmt, flags);
	}
	if (**fmt == '.')
	{
		(*fmt)++;
		set_precision(fmt, flags);
	}
	parse_type(fmt, flags, ap);
}

int		ft_printf(char *fmt, ...)
{
	va_list	ap;
	t_flags	flags;

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
	return (flags.total_len);
}

#include <stdio.h>
int		main(void)
{
	int hoge1 = ft_printf("hoge[%10.8d][%10.3s][oge\n", -31, "helllo");
	int hoge2 =    printf("hoge[%10.8d][%10.3s][oge\n", -31, "helllo");
	//    printf("hoge[%10.8d][%.3s]oge\n", -31, NULL);
	ft_printf("my %d, ori %d\n", hoge1, hoge2);
	return (0);
}