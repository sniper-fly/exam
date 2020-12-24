#include <stdio.h>

int		main(void)
{
	FILE	*fp;
	int		ret;
	int		width;
	int		height;
	char	background;

	fp = fopen("ex1", "r");
	if (fp == NULL)
	{
		perror("Error:");
		return (-1);
	}
	// できるだけ原文のフォーマットに近い形にする
	while ((ret = fscanf(fp, "%d %d %c\n", &width, &height, &background)) != EOF)
	{
		printf("width = %d\n", width);
		printf("height = %d\n", height);
		printf("background = %c\n", background);
	}

	printf("\nret = %d\n", ret);

	fclose(fp);
}