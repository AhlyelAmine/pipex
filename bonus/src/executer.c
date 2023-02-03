/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aahlyel <aahlyel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 21:10:36 by aahlyel           #+#    #+#             */
/*   Updated: 2023/02/03 18:36:10 by aahlyel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_execute(t_list **garbg, t_args **args, char **envp)
{
	int	i;
	int	pid;

	i = 0;
	if ((*args)->infile >= 0)
		dup2((*args)->infile, STDIN_FILENO);
	while (i < (*args)->ac - 3)
	{
		if (i < (*args)->ac  - 4)
		{
			if (pipe((*args)->fd) < 0)
				ft_exit(ERRPIPE, garbg, 0);
			exec_command(*args, garbg, envp, i);
		}
		else
		{
			pid = fork();
			if (!pid)
			{
				dup2((*args)->outfile, STDOUT_FILENO);
				if (execve((*args)->cmds_path[i], (*args)->cmds[i], envp) == -1)
					ft_exit(ERREXEC, garbg, 0);
			}
		}
		i++;
	}
}

void	exec_command(t_args *args, t_list **garbg, char **envp, int i)
{
	int	pid;

	if (i || args->infile >= 0)
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(args->fd[1], STDOUT_FILENO);
			close(args->fd[1]);
			close(args->fd[0]);
			if (execve(args->cmds_path[i], args->cmds[i], envp) == -1)
				ft_exit(ERREXEC, garbg, 0);
		}
		else
		{
			dup2(args->fd[0], STDIN_FILENO);
			close(args->fd[1]);
			close(args->fd[0]);
			waitpid(pid, NULL, F_OK);
		}
	}
	else
		dup2(args->outfile, STDIN_FILENO);
}
