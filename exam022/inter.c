#include <unistd.h>
#define SET_NUM 300
#define TRUE 1
#define FALSE 0

int		is_in_set(char ch, char *set)
{
	for (int i = 0; set[i]; i++)
	{
		if (ch == set[i])
		{
			return (TRUE);
		}
	}
	return (FALSE);
}

void	make_set(char *set, char *str)
{
	int set_i = 0;

	for (int i = 0; str[i]; i++)
	{
		if (!is_in_set(str[i], set))
		{
			set[set_i] = str[i];
			set_i++;
		}
	}
}

int		main(int argc, char **argv)
{
	if (argc != 3)
	{
		write(1, "\n", 1);
		return (0);
	}

	char set[SET_NUM];
	for (int i = 0; i < SET_NUM; i++)
		set[i] = 0;
	make_set(set, argv[1]);
	for (int i = 0; set[i]; i++)
	{
		if (is_in_set(set[i], argv[2]))
		{
			write(1, &set[i], 1);
		}
	}
	write(1, "\n", 1);
	return (0);
}
