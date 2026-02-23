/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 21:04:34 by jlabrous          #+#    #+#             */
/*   Updated: 2026/02/17 19:35:15 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (str);
		str++;
	}
	if (c == '\0')
		return (str);
	return (NULL);
}

void	free_split(char **tab)
{
	size_t	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

char	**get_paths(char *const envp[])
{
	int		i;
	char	*value;

	i = 0;
	while (envp && envp[i])
	{
		if (envp[i][0] == 'P' && envp[i][1] == 'A'
			&& envp[i][2] == 'T' && envp[i][3] == 'H'
			&& envp[i][4] == '=')
		{
			value = envp[i] + 5;
			if (*value == '\0')
				return (NULL);
			return (split_path(value));
		}
		i++;
	}
	return (NULL);
}

char	*join_path(const char *dir, const char *cmd)
{
	size_t	i;
	size_t	j;
	size_t	dir_len;
	size_t	cmd_len;
	char	*path;

	if (!dir || !cmd)
		return (NULL);
	dir_len = ft_strlen(dir);
	cmd_len = ft_strlen(cmd);
	path = malloc(dir_len + cmd_len + 2);
	if (!path)
		return (NULL);
	i = 0;
	while (i < dir_len)
	{
		path[i] = dir[i];
		i++;
	}
	path[i++] = '/';
	j = 0;
	while (j < cmd_len)
		path[i++] = cmd[j++];
	path[i] = '\0';
	return (path);
}
