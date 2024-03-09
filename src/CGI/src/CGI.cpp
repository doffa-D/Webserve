/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/23 14:40:21 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/09 20:56:12 by hdagdagu         ###   ########.fr       */
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
    while(true)
    {
        result = waitpid(pid, &status, WNOHANG);
        if (result == pid && (WIFSIGNALED(status) || WIFEXITED(status)))
        {
            if(WEXITSTATUS(status) == 0)
                return status;
            else
                return CGI_ERROR;
        }
        else if (difftime(time(0), BeginTime) > 4)
        {
            kill(pid, SIGKILL);
            return CGI_TIMEOUT;
        }
    }
    return CGI_ERROR;
}


std::pair<std::string, int> CGI::fill_env() {

    time_t BeginTime;
    BeginTime = time(0);
	int  Inputfd = open ("/tmp/Input", O_CREAT | O_WRONLY, 0644);

    pid_t pid = fork();


    if (pid == 0) {

        char **arge = new char *[3];
        arge[0] = strdup(this->bin.c_str());
        arge[1] = strdup(this->env["SCRIPT_FILENAME"].c_str());
        arge[2] = NULL;
        int  Inputfd = open ("/tmp/Input", O_RDONLY);
        int  Output = open ("/tmp/Output", O_CREAT | O_WRONLY, 0644);
        if (arge[0] == NULL || arge[1] == NULL) {

            delete[] arge[0];
            delete[] arge[1];
            delete[] arge;
            return std::make_pair("500 Internal Server Error", CGI_ERROR);

        }
        dup2(Inputfd, STDIN_FILENO);
		close (Inputfd);
		dup2(Output, STDOUT_FILENO);
		close (Output);

        if (execve(this->bin.c_str(), arge, envp) == -1) {
            return std::make_pair("500 Internal Server Error", CGI_ERROR);
        }
    } else {

        size_t totalBytesWritten = 0;
        size_t bytesToWrite = body.length();
        while (totalBytesWritten < bytesToWrite)
        {
            if (difftime(time(0), BeginTime) > 4)
            {
                kill(pid, SIGKILL);
                return std::make_pair("504 Gateway Timeout", CGI_TIMEOUT);
            }
            std::cout << "sss" << std::endl;
            ssize_t bytesWritten = write(Inputfd, body.c_str() + totalBytesWritten, bytesToWrite - totalBytesWritten);
            std::cout << bytesWritten << std::endl;
            if (bytesWritten == -1) {
                close(Inputfd);
                break;
            }
            totalBytesWritten += bytesWritten;
        }

        close(Inputfd);
        int  Output = open ("/tmp/Output", O_RDONLY);
        int wait_status = WaitCgi(pid, BeginTime);
        switch (wait_status) {
            case CGI_TIMEOUT:
                {
                    remove("/tmp/Input");
                    remove("/tmp/Output");
                    return std::make_pair("504 Gateway Timeout", CGI_TIMEOUT);
                }
            case CGI_ERROR:
                { 
                    remove("/tmp/Input");
                    remove("/tmp/Output");
                    wait_status = CGI_ERROR;
                    return std::make_pair("500 Internal Server Error", CGI_ERROR);
                }
            default:
                {
                    std::string response = "";
                    char buffer[1024];
                    ssize_t bytesRead;
                    while ((bytesRead = read(Output, buffer, sizeof(buffer))) > 0) {
                        response.append(buffer, bytesRead);
                    }
                    response.push_back('\0');
                    close(Output);
                    remove("/tmp/Input");
                    remove("/tmp/Output");
                    return std::make_pair(response, wait_status);
                }
        }
    }
    return std::make_pair("500 Internal Server Error", CGI_ERROR);
}
