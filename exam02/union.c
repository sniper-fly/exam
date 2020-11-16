#include <unistd.h>
#define FALSE 0
#define TRUE 1
#define SET_NUM 500

int		is_in_set(char ch, char *set)
{
	for (int i = 0; set[i]; i++)
	{
		if (set[i] == ch)
			return (TRUE);
	}
	return (FALSE);
}

void	make_set(char *set, char *str1, char *str2)
{
	int		set_idx = 0;
	for (int i = 0; str1[i]; i++)
	{
		if (!is_in_set(str1[i], set))
		{
			set[set_idx] = str1[i];
			set_idx++;
		}
	}
	for (int i = 0; str2[i]; i++)
	{
		if (!is_in_set(str2[i], set))
		{
			set[set_idx] = str2[i];
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
		set[i] = 0;
	make_set(set, argv[1], argv[2]);
	for (int i = 0; set[i]; i++)
		write(1, &set[i], 1);
}
