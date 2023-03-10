/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aahlyel <aahlyel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 21:10:36 by aahlyel           #+#    #+#             */
/*   Updated: 2023/02/09 03:52:59 by aahlyel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	ft_execute(t_list **garbg, t_args **args, char **envp)
{
	int	i;

	i = 0;
	if ((*args)->infile >= 0)
		faillure(garbg, dup2((*args)->infile, STDIN_FILENO), ERRDUP2);
	while (i < (*args)->ac - 3)
	{
		if (i < (*args)->ac - 4)
		{
			faillure(garbg, pipe((*args)->fd), ERRPIPE);
			exec_command(*args, garbg, envp, i);
		}
		else if ((*args)->cmds_path[i])
		{
			faillure(garbg, dup2((*args)->outfile, STDOUT_FILENO), ERRDUP2);
			faillure(garbg, \
			execve((*args)->cmds_path[i], (*args)->cmds[i], envp), ERREXEC);
		}
		i++;
	}
}

void	exec_command_noinfile(t_args *args, t_list **garbg)
{
	faillure(garbg, dup2(args->fd[0], STDIN_FILENO), ERRDUP2);
	faillure(garbg, close(args->fd[1]), ERRCLOSE);
	faillure(garbg, close(args->fd[0]), ERRCLOSE);
}

void	exec_command(t_args *args, t_list **garbg, char **envp, int i)
{
	int	pid;

	if ((i || args->infile >= 0) && args->cmds_path[i])
	{
		pid = fork();
		if (pid == 0)
		{
			faillure(garbg, dup2(args->fd[1], STDOUT_FILENO), ERRDUP2);
			faillure(garbg, close(args->fd[1]), ERRCLOSE);
			faillure(garbg, close(args->fd[0]), ERRCLOSE);
			faillure(garbg, \
			execve(args->cmds_path[i], args->cmds[i], envp), ERREXEC);
		}
		else
		{
			faillure(garbg, dup2(args->fd[0], STDIN_FILENO), ERRDUP2);
			faillure(garbg, close(args->fd[1]), ERRCLOSE);
			faillure(garbg, close(args->fd[0]), ERRCLOSE);
		}
	}
	else
		exec_command_noinfile(args, garbg);
}
