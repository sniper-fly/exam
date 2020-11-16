#include <unistd.h>
#define TRUE 1
#define FALSE 0
#define SET_SIZE 500

int		is_in_set(char ch, char *set);
char	*make_set(char *str1, char *set);

int		is_in_set(char ch, char *set)
{
	for (int i = 0; set[i]; i++)
	{
		if (set[i] == ch)
			return (TRUE);
	}
	return (FALSE);
}

void	init_set(char *set)
{
	for (int i = 0; i < SET_SIZE; i++) set[i] = 0;
}

char	*make_set(char *str1, char *set)
{
	int		i = 0;
	int		set_idx = 0;

	init_set(set);
	while (str1[i])
	{
		if (!is_in_set(str1[i], set))
		{
			set[set_idx] = str1[i];
			set_idx++;
		}
		i++;
	}
	return (set);
}

#include <stdio.h>
int		main(int argc, char **argv)
{
	char	set[SET_SIZE];

	if (argc != 3)
		return (0);
	make_set(argv[1], set);
	(void)argc;
	for (int i = 0; set[i]; i++)
	{
		if (is_in_set(set[i], argv[2]))
			write(1, &set[i], 1);
	}
}