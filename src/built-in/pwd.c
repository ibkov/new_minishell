#include "minishell.h"

int		sh_pwd(void)
{
	char	*cwd;

	cwd = (char *)malloc(sizeof(char) * 1024);
	if (!cwd)
		return (0);
	if (getcwd(cwd, 1024))
	{
		printf("%s\n", cwd);
		free(cwd);
		return (1);
	}
	else
	{
		free(cwd);
		return (0);
	}
}