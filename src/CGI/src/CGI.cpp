/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:21 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/01 18:17:24 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"

CGI::CGI(std::string const &body, std::map<std::string, std::string> const &env, std::string const &bin)
{
    this->body = body;
    this->env = env;
    this->bin = bin;

    this->envp = new char *[this->env.size() + 1];
    std::map<std::string, std::string>::const_iterator it = this->env.begin();
    int i = 0;
    for (; it != this->env.end(); ++it)
    {
        std::string entry = it->first + "=" + it->second;
        envp[i] = new char[entry.length() + 1];
        std::strcpy(envp[i], entry.c_str());
        ++i;
    }
    envp[this->env.size()] = NULL;
}

CGI::~CGI()
{
    for (int i = 0; envp[i]; ++i)
        delete[] envp[i];
    delete[] envp;
}

std::pair<std::string, int> CGI::fill_env()
{
    int status;
    int fd[2];
    std::string response = "";

    pid_t pid;
    if (pipe(fd) == -1)
    {
        perror("pipe");
        throw std::runtime_error("500 internal server error");
    }
    fcntl(fd[0], F_SETFL, O_NONBLOCK, FD_CLOEXEC);

    pid = fork();

    if (pid == -1)
    {
        perror("fork");
        throw std::runtime_error("500 internal server error");
 
    }
    if (pid == 0)
    {

        if (dup2(fd[1], 1) == -1 || dup2(fd[0], 0) == -1)
        {
            perror("dup2");
            exit(1);
        }


        char **arge = new char *[3];
        arge[0] = new char[this->bin.length() + 1];
        arge[1] = new char[this->env["SCRIPT_FILENAME"].length() + 1];
        arge[2] = NULL;
        std::strcpy(arge[0], this->bin.c_str());
        std::strcpy(arge[1], this->env["SCRIPT_FILENAME"].c_str());
        
        if (execve(this->bin.c_str(), arge, envp) == -1)
        {
            perror("execve");
            exit(1);
        }
    }
    else
    {
        if(read(fd[0], NULL, 0) != -1)
        {
            size_t len = 0;
            while(true)
            {
                size_t bytes_left = this->body.size() - len;
                size_t bytes_to_write = std::min<size_t>(1024, bytes_left);
                int byte = write(fd[1], &this->body.c_str()[len], bytes_to_write);
                if (byte == -1)
                {
                    perror("write");
                    break;
                }
                if (byte == 0 || len >= this->body.size())
                {
                    break;
                }
                len += byte;
            }
            waitpid(pid, &status, 0);
            if (WEXITSTATUS(status) != 0)
            {
                response = "500 internal server error";
            }


            char buffer[1024];
            while (true)
            {
                int byte = read(fd[0], buffer, 1023);
                if (byte <= 0)
                    break;
                buffer[byte] = '\0';
                response += buffer;
            }
        }
        close(fd[1]);
        close(fd[0]);
    }

    return std::make_pair(response, WEXITSTATUS(status));
}

