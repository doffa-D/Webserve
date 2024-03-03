/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:21 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/03 12:31:28 by hdagdagu         ###   ########.fr       */
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

short WaitCgi(pid_t prc_id, time_t BeginTime)
{
    int status;
    int process;

    process = waitpid(prc_id, &status, WNOHANG);
    if (process == prc_id && (WIFSIGNALED(status) || WIFEXITED(status)))
        return CGI_SUCCESS;
    if (difftime(time(0), BeginTime) >= 5)
    { 
        kill(prc_id, SIGKILL);
        return CGI_TIMEOUT;
    }
    return CGI_WAITING;
}

std::pair<std::string, int> CGI::fill_env() {
    int input[2];
    int output[2];
    time_t BeginTime;
    BeginTime = time(0);
    std::string response = "";

    if(pipe(input) == -1 || pipe(output) == -1) {
        perror("pipe");
        return std::make_pair("500 internal server error", -1);
    }

    int status = -1;
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        close(input[0]);
        close(input[1]);
        close(output[0]);
        close(output[1]);
        return std::make_pair("500 internal server error", -1);

    }
    if (pid == 0) {
        close(input[1]);
        close(output[0]);

        if (dup2(input[0], STDIN_FILENO) == -1 ||
            dup2(output[1], STDOUT_FILENO) == -1 ||
            dup2(output[1], STDERR_FILENO) == -1) {

            close(input[0]);
            close(output[1]);
            return std::make_pair("500 internal server error", -1);
        }

        close(input[0]);
        close(output[1]);

        char **arge = new char *[3];
        arge[0] = strdup(this->bin.c_str());
        arge[1] = strdup(this->env["SCRIPT_FILENAME"].c_str());
        arge[2] = NULL;

        if (arge[0] == NULL || arge[1] == NULL) {

            delete[] arge[0];
            delete[] arge[1];
            delete[] arge;
            return std::make_pair("500 internal server error", -1);
        }

        if (execve(this->bin.c_str(), arge, envp) == -1) {
            return std::make_pair("500 internal server error", -1);
        }
    }
    else 
    {
        close(input[0]);
        close(output[1]);

        size_t totalBytesWritten = 0;
        size_t bytesToWrite = body.length();
        while (totalBytesWritten < bytesToWrite)
        {
            ssize_t bytesWritten = write(input[1], body.c_str() + totalBytesWritten, bytesToWrite - totalBytesWritten);
            if (bytesWritten == -1) 
            {
                close(input[1]);
                waitpid(pid, &status, 0);
                return std::make_pair("500 internal server error", -1);
            }
            totalBytesWritten += bytesWritten;
        }
        close(input[1]);

        while ((status = WaitCgi(pid, BeginTime)) == CGI_WAITING);
            
        if (status == CGI_SUCCESS)
        {
            char buffer[1024];
            ssize_t bytesRead;
            while ((bytesRead = read(output[0], buffer, sizeof(buffer))) > 0) {
                response.append(buffer, bytesRead);
            }
        }
        else if (status == CGI_TIMEOUT)
        {
            return std::make_pair("504 Gateway Timeout", -1);
        }

        close(output[0]);
    }

    return std::make_pair(response, status);
}