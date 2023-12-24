#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <map>

#define CGI "cgi-bin/php-cgi"

int main()
{
    int status;
    std::map<std::string, std::string> env;

    env["SCRIPT_NAME"] = "index.php";
    env["SCRIPT_FILENAME"] = "php/index.php";
    env["REQUEST_METHOD"] = "GET";
    env["CONTENT_LENGTH"] = "0";
    env["CONTENT_TYPE"] = "text/html";
    env["QUERY_STRING"] = "name=hdagdagu&age=21";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = "webserv";
    env["SERVER_NAME"] = "localhost";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["REDIRECT_STATUS"] = "200";

    char **envp = new char *[env.size() + 1];
    std::map<std::string, std::string>::iterator it = env.begin();
    for (int i = 0; it != env.end(); it++)
    {
        std::string entry = it->first + "=" + it->second;
        envp[i] = new char[entry.length() + 1];
        std::strcpy(envp[i], entry.c_str());
        i++;
    }
    envp[env.size()] = NULL;

    // for (int i = 0; envp[i]; i++)
    //     std::cout << envp[i] << std::endl;

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
        arge[1] = new char[std::strlen("php/index.php") + 1];
        arge[2] = NULL;
        std::strcpy(arge[0], CGI);
        std::strcpy(arge[1], "php/index.php");
        if (execve(CGI, arge, envp) == -1)
        {
            std::cout << "execve error" << std::endl;
            exit(1);
        }
        exit(0);
    }
    else
    {

        close(fd[1]);
        waitpid(pid, &status, 0);
        std::cout << WEXITSTATUS(status) << std::endl;
        char buf[1024];
        ssize_t bytesRead;
        while ((bytesRead = read(fd[0], buf, sizeof(buf))) > 0)
        {
            write(STDOUT_FILENO, buf, bytesRead);
        }
        close(fd[0]);
    }
}
