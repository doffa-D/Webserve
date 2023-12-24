/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:21 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/24 11:37:54 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cgi.hpp"

cgi::cgi(std::string head, std::string body, std::string SCRIPT_NAME, std::string SCRIPT_FILENAME, std::string CONTENT_TYPE, std::string REQUEST_METHOD, std::string CONTENT_LENGTH, std::string QUERY_STRING, std::string SERVER_PROTOCOL, std::string SERVER_SOFTWARE, std::string SERVER_NAME, std::string GATEWAY_INTERFACE, std::string REDIRECT_STATUS)
{
    this->head = head;
    this->body = body;
    this->env["SCRIPT_NAME"] = SCRIPT_NAME;
    this->env["SCRIPT_FILENAME"] = SCRIPT_FILENAME;
    this->env["REQUEST_METHOD"] = REQUEST_METHOD;
    this->env["CONTENT_LENGTH"] = CONTENT_LENGTH;
    this->env["CONTENT_TYPE"] = CONTENT_TYPE;
    this->env["QUERY_STRING"] = QUERY_STRING;
    this->env["SERVER_PROTOCOL"] = SERVER_PROTOCOL;
    this->env["SERVER_SOFTWARE"] = SERVER_SOFTWARE;
    this->env["SERVER_NAME"] = SERVER_NAME;
    this->env["GATEWAY_INTERFACE"] = GATEWAY_INTERFACE;
    this->env["REDIRECT_STATUS"] = REDIRECT_STATUS;

    this->envp = new char *[this->env.size() + 1];
    std::map<std::string, std::string>::iterator it = this->env.begin();
    for (int i = 0; it != this->env.end(); it++)
    {
        std::string entry = it->first + "=" + it->second;
        envp[i] = new char[entry.length() + 1];
        std::strcpy(envp[i], entry.c_str());
        i++;
    }
    envp[this->env.size()] = NULL;
}

cgi::~cgi()
{
    for (int i = 0; envp[i]; i++)
        delete[] envp[i];
    delete[] envp;
}

void cgi::fill_env(std::string SCRIPT_FILENAME)
{

    int status;
    int fd[2];
    pid_t pid;

    if (pipe(fd) == -1)
    {
        std::cout << "pipe error" << std::endl;
        exit(1);
    }
    pid = fork();
    if (pid == -1)
    {
        std::cout << "fork error" << std::endl;
        exit(1);
    }
    if (pid == 0)
    {

        if (dup2(fd[1], 1) == -1)
        {
            std::cout << "dup2 error" << std::endl;
            exit(1);
        }
        if (dup2(fd[0], 0) == -1)
        {
            std::cout << "dup2 error" << std::endl;
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);

        char **arge = new char *[3];
        arge[0] = new char[std::strlen(CGI) + 1];
        arge[1] = new char[std::strlen(SCRIPT_FILENAME.c_str()) + 1];
        arge[2] = NULL;
        std::strcpy(arge[0], CGI);
        std::strcpy(arge[1], SCRIPT_FILENAME.c_str());
        if (execve(CGI, arge, envp) == -1)
        {
            std::cout << "execve error" << std::endl;
            exit(1);
        }
        exit(0);
    }
    else
    {
        waitpid(pid, &status, 0);
        if (WEXITSTATUS(status) != 0)
        {
            std::cout << "CGI ERROR" << std::endl;
            return;
        }
        write(fd[1], body.c_str(), body.length());
        close(fd[1]);
        char buffer[1024];
        std::string response;
        while (true)
        {
            int byte = read(fd[0], buffer, 1023);
            if (byte <= 0)
                break;
            buffer[byte + 1] = '\0';
            response += buffer;
        }
        std::cout << response << std::endl;
    }
}
