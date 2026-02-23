/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 00:00:00 by jlabrous          #+#    #+#             */
/*   Updated: 2026/02/17 19:33:22 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <sys/types.h>
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <stdio.h>

/* utils */
size_t	ft_strlen(const char *str);
char	*ft_strchr(char *str, char c);

/* parsing */
char	**get_paths(char *const envp[]);
char	*join_path(const char *dir, const char *cmd);
char	**split_command(const char *s);
char	**split_path(const char *s);
void	free_split(char **tab);

/* exec */
int		exec(char *cmd, char *const envp[]);

#endif
