/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aahlyel <aahlyel@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 21:10:36 by aahlyel           #+#    #+#             */
/*   Updated: 2023/01/27 19:08:40 by aahlyel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	ft_execute(t_list **garbg, t_args **args, char **envp)
{
	int	i;

	i = 0;
	while (i < 2)
	{
		if (!i)
		{
			if (pipe((*args)->fd) < 0)
				ft_exit("Error pipe faillure", garbg);
			exec_command(*args, garbg, envp, i);
		}
		else
		{
			dup2((*args)->outfile, STDOUT_FILENO);
			execve((*args)->cmds_path[i], (*args)->cmds[i], envp);
		}
		i++;
	}
}

void	exec_command(t_args *args, t_list **garbg, char **envp, int i)
{
	int	pid;

	garbg = NULL;
	pid = fork();
	if (pid == 0)
	{
		dup2(args->fd[1], STDOUT_FILENO);
		close(args->fd[1]);
		close(args->fd[0]);
		execve(args->cmds_path[i], args->cmds[i], envp);
	}
	else
	{
		dup2(args->fd[0], STDIN_FILENO);
		close(args->fd[1]);
		close(args->fd[0]);
		waitpid(pid, NULL, F_OK);
	}
}
