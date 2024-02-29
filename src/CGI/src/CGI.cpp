/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:21 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/29 21:09:44 by rrhnizar         ###   ########.fr       */
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
    // for (int i = 0; envp[i]; ++i)
    //     delete[] envp[i];
    // delete[] envp;
}

std::string CGI::fill_env()
{
    int status;
    int fd[2];
    int _fd[2];
    std::string response = "";

    pid_t pid;
    if (pipe(fd) == -1 || pipe(_fd) == -1)
    {
        perror("pipe");
        throw std::runtime_error("500 internal server error");
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        throw std::runtime_error("500 internal server error");
 
    }
    if (pid == 0)
    {

        if (dup2(fd[1], 1) == -1 || dup2(fd[0], 0) == -1 || dup2(_fd[1], 2) == -1)
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
        write(fd[1], body.c_str(), body.length());
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0)
        {
            char buffer[1024];
            close(_fd[1]);
            while (true)
            {
                int byte = read(_fd[0], buffer, 1023);
                if (byte <= 0)
                    break;
                buffer[byte] = '\0';
                response += buffer;
            }
        }
        else
        {
            close(fd[1]);
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
    }
    return response;
}