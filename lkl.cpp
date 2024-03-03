#include <iostream>
#include <string>
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <map>

class CGI {
private:
    std::string body;
    std::string bin;
    std::map<std::string, std::string> env;
    char **envp;

public:
    CGI(std::string const &body, std::map<std::string, std::string> const &env, std::string const &bin) {
        this->body = body;
        this->env = env;
        this->bin = bin;

        this->envp = new char *[this->env.size() + 1];
        std::map<std::string, std::string>::const_iterator it = this->env.begin();
        int i = 0;
        for (; it != this->env.end(); ++it) {
            std::string entry = it->first + "=" + it->second;
            envp[i] = new char[entry.length() + 1];
            std::strcpy(envp[i], entry.c_str());
            ++i;
        }
        envp[this->env.size()] = NULL;
    }

    ~CGI() {
        for (int i = 0; envp[i]; ++i)
            delete[] envp[i];
        delete[] envp;
    }

    std::pair<std::string, int> fill_env() {
        int status;
        int fd[2];
        std::string response = "";

        pid_t pid;
        if (pipe(fd) == -1) {
            perror("pipe");
            throw std::runtime_error("500 internal server error");
        }

        pid = fork();

        if (pid == -1) {
            perror("fork");
            throw std::runtime_error("500 internal server error");
        }

        if (pid == 0) {
            if (dup2(fd[1], 1) == -1 || dup2(fd[0], 0) == -1) {
                perror("dup2");
                exit(1);
            }

            char **arge = new char *[3];
            arge[0] = new char[this->bin.length() + 1];
            arge[1] = new char[this->env["SCRIPT_FILENAME"].length() + 1];
            arge[2] = NULL;
            std::strcpy(arge[0], this->bin.c_str());
            std::strcpy(arge[1], this->env["SCRIPT_FILENAME"].c_str());

            if (execve(this->bin.c_str(), arge, envp) == -1) {
                perror("execve");
                exit(1);
            }
        }
        else {
            close(fd[0]); // Close the reading end of the pipe

            // Set the writing end of the pipe to non-blocking mode
            if (fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK) == -1) {
                perror("fcntl");
                throw std::runtime_error("500 internal server error");
            }

            ssize_t bytes_written = -1;
            while (bytes_written < static_cast<ssize_t>(body.size())) {
                bytes_written = write(fd[1], body.c_str() + bytes_written, body.size() - bytes_written);
                if (bytes_written == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // Write would block, retry later
                        usleep(1000); // Sleep for 1 millisecond
                    } else {
                        // Other errors
                        perror("write");
                        throw std::runtime_error("500 internal server error");
                    }
                }
            }
            close(fd[1]); // Close the writing end of the pipe

            // Wait for the child process to exit
            waitpid(pid, &status, 0);

            // Read the response from the child process
            char buffer[1024];
            while (true) {
                ssize_t bytes_read = read(fd[0], buffer, sizeof(buffer) - 1);
                if (bytes_read == -1) {
                    perror("read");
                    throw std::runtime_error("500 internal server error");
                } else if (bytes_read == 0) {
                    // End of file
                    break;
                } else {
                    buffer[bytes_read] = '\0';
                    response += buffer;
                }
            }
            close(fd[0]); // Close the reading end of the pipe

            if (WIFEXITED(status)) {
                return std::make_pair(response, WEXITSTATUS(status));
            } else {
                throw std::runtime_error("500 internal server error");
            }
        }
        return std::make_pair(response, WEXITSTATUS(status));
    }
};

int main() {
    std::string body = "This is a sample request body.";
    std::map<std::string, std::string> env;
    env["SCRIPT_FILENAME"] = "/Users/hdagdagu/Desktop/Webserve/Test_page/ll.py";
    std::string bin = "/usr/bin/python3";

    CGI cgi(body, env, bin);
    std::pair<std::string, int> result = cgi.fill_env();
    std::cout << "Response: " << result.first << std::endl;
    std::cout << "Exit Status: " << result.second << std::endl;

    return 0;
}
