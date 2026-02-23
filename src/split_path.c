/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:29:40 by jlabrous          #+#    #+#             */
/*   Updated: 2026/02/17 19:34:34 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static size_t	count_parts(const char *s)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 1;
	while (s[i] != '\0')
	{
		if (s[i] == ':')
			count++;
		i++;
	}
	return (count);
}

static char	*dup_part(const char *s, size_t start, size_t end)
{
	char	*part;
	size_t	i;
	size_t	len;

	len = end - start;
	if (len == 0)
		len = 1;
	part = (char *)malloc(len + 1);
	if (!part)
		return (NULL);
	if (end == start)
		part[0] = '.';
	else
	{
		i = 0;
		while (start + i < end)
		{
			part[i] = s[start + i];
			i++;
		}
	}
	part[len] = '\0';
	return (part);
}

static int	fill_parts(char **tab, const char *s)
{
	size_t	i;
	size_t	j;
	size_t	start;

	i = 0;
	j = 0;
	start = 0;
	while (s[i] != '\0')
	{
		if (s[i] == ':')
		{
			tab[j] = dup_part(s, start, i);
			if (!tab[j])
				return (0);
			j++;
			start = i + 1;
		}
		i++;
	}
	tab[j] = dup_part(s, start, i);
	if (!tab[j])
		return (0);
	tab[j + 1] = NULL;
	return (1);
}

char	**split_path(const char *s)
{
	char	**tab;
	size_t	i;
	size_t	parts;

	if (!s || s[0] == '\0')
		return (NULL);
	parts = count_parts(s);
	tab = (char **)malloc(sizeof(char *) * (parts + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (i < parts + 1)
		tab[i++] = NULL;
	if (!fill_parts(tab, s))
		return (free_split(tab), NULL);
	return (tab);
}
