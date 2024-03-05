/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:21 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/05 16:11:48 by hdagdagu         ###   ########.fr       */
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

int WaitCgi(pid_t pid, time_t BeginTime)
{
    int status;
    pid_t result;
    do {
        result = waitpid(pid, &status, WNOHANG);
        if (result == 0)
        {
            if (difftime(time(0), BeginTime) > 5) {
                kill(pid, SIGKILL);
                std::cout << "CGI process was killed" << std::endl;
                return CGI_TIMEOUT;
            }
        } 
        else
        {
            if (WIFEXITED(status))
            {
                return WEXITSTATUS(status);
            }
            else
            {
                std::cerr << "CGI process was signaled" << std::endl;
                return CGI_ERROR;
            }
        }
    } while (result == 0);
    return CGI_ERROR;
}


std::pair<std::string, int> CGI::fill_env() {
    int input[2];
    int output[2];
    time_t BeginTime;
    BeginTime = time(0);

    if (pipe(input) == -1 || pipe(output) == -1) {
        return std::make_pair("500 Internal Server Error", CGI_ERROR);
    }

    int status = -1;
    pid_t pid = fork();

    if (pid == -1) {
        close(input[0]);
        close(input[1]);
        close(output[0]);
        close(output[1]);
        return std::make_pair("500 Internal Server Error", CGI_ERROR);

    }
    if (pid == 0) {
        close(input[1]);
        close(output[0]);

        if (dup2(input[0], STDIN_FILENO) == -1 ||
            dup2(output[1], STDOUT_FILENO) == -1 ||
            dup2(output[1], STDERR_FILENO) == -1) {

            close(input[0]);
            close(output[1]);
            return std::make_pair("500 Internal Server Error", CGI_ERROR);

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
            return std::make_pair("500 Internal Server Error", CGI_ERROR);

        }

        if (execve(this->bin.c_str(), arge, envp) == -1) {
            return std::make_pair("500 Internal Server Error", CGI_ERROR);
        }
    } else {
        close(input[0]);
        close(output[1]);

        size_t totalBytesWritten = 0;
        size_t bytesToWrite = body.length();
        while (totalBytesWritten < bytesToWrite) {
            ssize_t bytesWritten = write(input[1], body.c_str() + totalBytesWritten, bytesToWrite - totalBytesWritten);
            if (bytesWritten == -1) {
                close(input[1]);
                waitpid(pid, &status, 0);
                break;
            }
            totalBytesWritten += bytesWritten;
        }
        close(input[1]);

        int wait_status = WaitCgi(pid, BeginTime);
        std::string response = "";
        switch (wait_status) {
            case CGI_TIMEOUT:
                {
                    std::cout << "CGI process was killed Timeout" << std::endl;
                    close(output[0]);
                    response = "504 Gateway Timeout";
                    break;
                }
            case CGI_ERROR:
                {
                    close(output[0]);   
                    response = "500 Internal Server Error";
                    wait_status = CGI_ERROR;
                    std::cerr << "CGI process was ERROR " << wait_status << std::endl;
                    break;
                }
            default:
                {
                    char buffer[1024];
                    ssize_t bytesRead;
                    while ((bytesRead = read(output[0], buffer, sizeof(buffer))) > 0) {
                        response.append(buffer, bytesRead);
                    }
                    close(output[0]);
                    return std::make_pair(response, wait_status);
                }
        }
    }
    return std::make_pair("500 Internal Server Error", CGI_ERROR);
}
