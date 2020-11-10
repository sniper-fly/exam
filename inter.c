#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define SET_NUM 500

int		is_in_set(char ch, char *set)
{
	for (int i = 0; set[i]; i++)
	{
		if (set[i] == ch)
		{
			return (TRUE);
		}
	}
	return (FALSE);
}

void	make_set(char *set, char *str)
{
	int		set_idx = 0;

	for (int i = 0; str[i]; i++)
	{
		if (!is_in_set(str[i], set))
		{
			set[set_idx] = str[i];
			set_idx++;
		}
	}
}

int		main(int argc, char **argv)
{
	char	set[SET_NUM];

	if (argc != 3)
		return (0);
	for (int i = 0; i < SET_NUM; i++)
	{
		set[i] = 0;
	}
	make_set(set, argv[1]);
	for (int i = 0; set[i]; i++)
	{
		if (is_in_set(set[i], argv[2]))
		{
			write(1, &set[i], 1);
		}
	}
	return (0);
}