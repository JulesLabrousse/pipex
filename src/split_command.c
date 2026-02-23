/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 17:44:51 by jlabrous          #+#    #+#             */
/*   Updated: 2026/02/17 19:34:16 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	token_bounds(const char *s, size_t start, size_t *end)
{
	size_t	i;
	char	quote;

	i = start;
	quote = 0;
	while (s[i] != '\0' && (quote || (s[i] != ' ' && s[i] != '\t')))
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		i++;
	}
	*end = i;
	if (quote)
		return (1);
	return (0);
}

static char	*dup_token(const char *s, size_t start, size_t end)
{
	char	*tok;
	size_t	i;
	size_t	j;
	char	quote;

	tok = (char *)malloc(end - start + 1);
	if (!tok)
		return (NULL);
	i = start;
	j = 0;
	quote = 0;
	while (i < end)
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i];
		else if (quote && s[i] == quote)
			quote = 0;
		else
			tok[j++] = s[i];
		i++;
	}
	tok[j] = '\0';
	return (tok);
}

static int	count_tokens(const char *s, size_t *out_count)
{
	size_t	i;
	size_t	end;

	i = 0;
	*out_count = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && (s[i] == ' ' || s[i] == '\t'))
			i++;
		if (s[i] == '\0')
			return (0);
		if (token_bounds(s, i, &end))
			return (1);
		(*out_count)++;
		i = end;
	}
	return (0);
}

static int	fill_tokens(char **tab, const char *s)
{
	size_t	i;
	size_t	j;
	size_t	end;

	i = 0;
	j = 0;
	while (s[i] != '\0')
	{
		while (s[i] != '\0' && (s[i] == ' ' || s[i] == '\t'))
			i++;
		if (s[i] == '\0')
			return (0);
		if (token_bounds(s, i, &end))
			return (1);
		tab[j] = dup_token(s, i, end);
		if (!tab[j])
			return (1);
		j++;
		i = end;
	}
	return (0);
}

char	**split_command(const char *s)
{
	char	**tab;
	size_t	i;
	size_t	tokens;

	if (!s)
		return (NULL);
	if (count_tokens(s, &tokens))
		return (NULL);
	tab = (char **)malloc(sizeof(char *) * (tokens + 1));
	if (!tab)
		return (NULL);
	i = 0;
	while (i < tokens + 1)
		tab[i++] = NULL;
	if (fill_tokens(tab, s))
		return (free_split(tab), NULL);
	return (tab);
}
