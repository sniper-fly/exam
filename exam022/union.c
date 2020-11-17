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

void	make_set(char *set, char *s1, char *s2)
{
	int set_i = 0;

	for (int i = 0; s1[i]; i++)
	{
		if (!is_in_set(s1[i], set))
		{
			set[set_i] = s1[i];
			set_i++;
		}
	}
	for (int i = 0; s2[i]; i++)
	{
		if (!is_in_set(s2[i], set))
		{
			set[set_i] = s2[i];
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

	make_set(set, argv[1], argv[2]);
	for (int i = 0; set[i]; i++)
		write(1, &set[i], 1);
	write(1, "\n", 1);
	return (0);
}