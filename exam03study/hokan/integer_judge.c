#include <stdio.h>

int		main(void)
{
	double test = 1.2;

	// if (test / (int)test == 1)
	// 	printf("整数です。");
	// else
	// {
	// 	printf("小数です");
	// }
	printf("test = %f: casted = %d", test, (int)test);

	return (0);
}