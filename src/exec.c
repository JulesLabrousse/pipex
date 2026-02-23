/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 20:22:42 by jlabrous          #+#    #+#             */
/*   Updated: 2026/02/17 20:08:58 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	exec_direct(char *const path,
	char *const argv[], char *const envp[])
{
	execve(path, argv, envp);
	if (errno == ENOENT || errno == ENOTDIR)
		return (perror(argv[0]), 127);
	if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
		return (perror(argv[0]), 126);
	return (perror(argv[0]), 1);
}

static int	exec_from_path(char *const argv[], char *const envp[])
{
	char	**paths;
	char	*candidate;
	int		i;
	int		found_not_exec;
	int		saved_errno;

	paths = get_paths(envp);
	if (!paths)
		return (write(2, argv[0], ft_strlen(argv[0])),
			write(2, ": command not found\n", 20), 127);
	found_not_exec = 0;
	saved_errno = 0;
	i = 0;
	while (paths[i])
	{
		candidate = join_path(paths[i], argv[0]);
		if (!candidate)
			return (free_split(paths), 1);
		execve(candidate, argv, envp);
		if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
		{
			found_not_exec = 1;
			saved_errno = errno;
		}
		else if (!found_not_exec && errno != ENOENT && errno != ENOTDIR)
			saved_errno = errno;
		free(candidate);
		i++;
	}
	free_split(paths);
	if (found_not_exec)
		return (errno = saved_errno, perror(argv[0]), 126);
	return (write(2, argv[0], ft_strlen(argv[0])),
		write(2, ": command not found\n", 20), 127);
}

int	exec(char *cmd, char *const envp[])
{
	char	**argv;
	int		ret;

	argv = split_command(cmd);
	if (argv && (!argv[0] || argv[0][0] == '\0'))
		return (write(2, ": command not found\n", 20), free_split(argv), 127);
	if (!argv)
		return (write(2, "pipex: unmatched quote\n", 24), 1);
	if (ft_strchr(argv[0], '/'))
		ret = exec_direct(argv[0], argv, envp);
	else
		ret = exec_from_path(argv, envp);
	return (free_split(argv), ret);
}

/* norminette-friendly for exec_from_path :

typedef struct s_path_state
{
	int	found_not_exec;
	int	saved_errno;
}	t_path_state;

static int	try_candidate(char *dir, t_path_state *st,
		char *const argv[], char *const envp[])
{
	char	*candidate;

	candidate = join_path(dir, argv[0]);
	if (!candidate)
		return (1);
	execve(candidate, argv, envp);
	if (errno == EACCES || errno == EISDIR || errno == ENOEXEC)
	{
		st->found_not_exec = 1;
		st->saved_errno = errno;
	}
	else if (!st->found_not_exec && errno != ENOENT && errno != ENOTDIR)
		st->saved_errno = errno;
	free(candidate);
	return (0);
}

static int	exec_from_path(char *const argv[], char *const envp[])
{
	char			**paths;
	int				i;
	t_path_state	st;

	paths = get_paths(envp);
	if (!paths)
		return (write(2, argv[0], ft_strlen(argv[0])),
			write(2, ": command not found\n", 20), 127);
	st.found_not_exec = 0;
	st.saved_errno = 0;
	i = 0;
	while (paths[i])
	{
		if (try_candidate(paths[i], &st, argv, envp))
			return (free_split(paths), 1);
		i++;
	}
	free_split(paths);
	if (st.found_not_exec)
		return (errno = st.saved_errno, perror(argv[0]), 126);
	return (write(2, argv[0], ft_strlen(argv[0])),
		write(2, ": command not found\n", 20), 127);
} */
