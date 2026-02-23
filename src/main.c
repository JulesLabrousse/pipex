/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jlabrous <jlabrous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 01:30:19 by jlabrous          #+#    #+#             */
/*   Updated: 2026/02/17 19:33:54 by jlabrous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	wait_and_return(pid_t pid_left, pid_t pid_right)
{
	int	status_left;
	int	status_right;

	if (waitpid(pid_left, &status_left, 0) < 0)
		return (perror("waitpid"), 1);
	if (waitpid(pid_right, &status_right, 0) < 0)
		return (perror("waitpid"), 1);
	if (WIFEXITED(status_right))
		return (WEXITSTATUS(status_right));
	if (WIFSIGNALED(status_right))
		return (128 + WTERMSIG(status_right));
	return (1);
}

static int	child1(char *infile, char *cmd1, int pipefd[2], char **envp)
{
	int	input_fd;

	input_fd = open(infile, O_RDONLY);
	if (input_fd < 0)
		return (perror(infile), close(pipefd[0]), close(pipefd[1]), 1);
	if (dup2(input_fd, STDIN_FILENO) < 0)
		return (perror("dup2"), close(input_fd), close(pipefd[0]),
			close(pipefd[1]), 1);
	if (dup2(pipefd[1], STDOUT_FILENO) < 0)
		return (perror("dup2"), close(input_fd), close(pipefd[0]),
			close(pipefd[1]), 1);
	return (close(input_fd), close(pipefd[0]), close(pipefd[1]),
		exec(cmd1, envp));
}

static int	child2(char *outfile, char *cmd2, int pipefd[2], char **envp)
{
	int	output_fd;

	output_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (output_fd < 0)
		return (perror(outfile), close(pipefd[0]), close(pipefd[1]), 1);
	if (dup2(pipefd[0], STDIN_FILENO) < 0)
		return (perror("dup2"), close(output_fd), close(pipefd[0]),
			close(pipefd[1]), 1);
	if (dup2(output_fd, STDOUT_FILENO) < 0)
		return (perror("dup2"), close(output_fd), close(pipefd[0]),
			close(pipefd[1]), 1);
	return (close(output_fd), close(pipefd[0]), close(pipefd[1]),
		exec(cmd2, envp));
}

int	main(int argc, char *argv[], char *envp[])
{
	int		pipe_fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;

	if (argc != 5)
		return (write(2, "Usage: ./pipex infile cmd1 cmd2 outfile\n", 41), 1);
	if (pipe(pipe_fd) < 0)
		return (perror("pipe"), 1);
	pid_left = fork();
	if (pid_left < 0)
		return (perror("fork"), close(pipe_fd[0]), close(pipe_fd[1]), 1);
	if (pid_left == 0)
		return (child1(argv[1], argv[2], pipe_fd, envp));
	pid_right = fork();
	if (pid_right < 0)
		return (perror("fork"),
			close(pipe_fd[0]), close(pipe_fd[1]),
			waitpid(pid_left, &status_left, 0), 1);
	if (pid_right == 0)
		return (child2(argv[4], argv[3], pipe_fd, envp));
	return (close(pipe_fd[0]), close(pipe_fd[1]),
		wait_and_return(pid_left, pid_right));
}
