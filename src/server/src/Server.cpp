/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/15 10:29:00 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

void Server::set_ports(int ports, int index)
{
	Ports[index] = ports;
}

void Server::set_Number_of_ports(int number_of_ports)
{
	Number_of_ports = number_of_ports;
}

Server::Server(char *argv)
{
	this->argv = argv;
	init_server(*this, argv);
	MIME_types_init();
	getMyIpAddress();
	for (int i = 0; i < Number_of_ports; i++)
		Setup_Server(i);		  // Setup_Server function is used to setup the server
	listen_to_multiple_clients(); // listen_to_multiple_clients function is used to listen to multiple clients
}

bool Server::check_socket(int i)
{
	for (int j = 0; j < Number_of_ports; j++)
	{
		if (socket_fd_server[j] == i)
			return true;
	}
	return false;
}

std::string Server::read_full_request(int socket_fd)
{
    char buffer[BUFFER_SIZE];
    std::string full_request;

    while (true)
    {
        int valread = recv(socket_fd, buffer, BUFFER_SIZE, 0);
        if (valread > 0)
        {
            full_request.append(buffer, buffer + valread);
            if (valread < BUFFER_SIZE)
                break; // All data received
        }
        else if (valread == 0)
        {
            // Connection closed by client
            close(socket_fd);
            return "";
        }
        else if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            // No more data to read
            break;
        }
        else
        {
            perror("recv");
            close(socket_fd);
            return "";
        }
    }

    return full_request;
}

void Server::listen_to_multiple_clients()
{
    fd_set current_sockets, ready_sockets;
    FD_ZERO(&current_sockets);

    for (int i = 0; i < Number_of_ports; i++)
    {
        FD_SET(socket_fd_server[i], &current_sockets);
    }

    while (true)
    {
        ready_sockets = current_sockets;
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error in select");
            exit(1);
        }

        for (int i = 0; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET(i, &ready_sockets))
            {
                if (check_socket(i) == true)
                {
                    socklen_t addrlen = sizeof(address[i]);
                    if ((socket_fd_client = accept(i, (struct sockaddr *)&address[i], (socklen_t *)&addrlen)) < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    else
                    {
                        fcntl(socket_fd_client, F_SETFL, O_NONBLOCK);
                        FD_SET(socket_fd_client, &current_sockets);
                    }
                }
                else
                {
                    // Reading the full request
                    std::string full_request = read_full_request(i);

                    // Process the received request
                    if (!full_request.empty())
                    {
                        parseRequest(full_request.c_str()); // Implement your request parsing logic
                        set_status("404 Not Found");
                        Send_Error_Response(i); // Implement your error response logic
                        // printf("Received data: %s %d\n", full_request.c_str(), id++);
						std::cout << "Received data: " << full_request.c_str() << std::endl;
					}
				
					close(i);
					FD_CLR(i, &current_sockets);
                    
                }
            }
        }
    }
}

Server::~Server()
{
	// std::cout << "Server destructor called. " << std::endl;
}

void Server::Setup_Server(int port_index)
{
	socket_fd_server[port_index] = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd_server[port_index] == -1)
	{
		std::cout << "Failed to create socket for port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cout << "Failed to set socket options for port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}

	// initialize address
	address[port_index].sin_family = AF_INET;
	address[port_index].sin_port = htons(Ports[port_index]);
	address[port_index].sin_addr.s_addr = INADDR_ANY;

	if (bind(socket_fd_server[port_index], (struct sockaddr *)&address[port_index], sizeof(address[port_index])) == -1)
	{
		std::cout << "Failed to bind to port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(socket_fd_server[port_index], 10) == -1)
	{
		std::cout << "Failed to listen on port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}
	fcntl(socket_fd_server[port_index], F_SETFL, O_NONBLOCK);
	std::cout << "Listening on port " << Ports[port_index] << std::endl;
}

void Server::Accept_Connection(int port_index)
{
	socklen_t address_length = sizeof(address);
	if ((socket_fd_client = accept(socket_fd_server[port_index], (struct sockaddr *)&address, &address_length)) == -1)
	{
		std::cout << "Failed to grab connection on port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::getMyIpAddress()
{
	char buffer[1024];
	ip_address = "";

	struct hostent *host; // Structure containing host information
	if (gethostname(buffer, sizeof(buffer)) != -1)
	{
		host = gethostbyname(buffer); // Get the IP address
		if (host != NULL)
		{
			ip_address = inet_ntoa(*((struct in_addr *)host->h_addr_list[0])); // Convert IP to string
		}
	}
	return;
}


void Server::Send_Error_Response(int fd_client)
{
	std::string message_body = "<html>"
							"<head>"
							"<style>"
							"h1 { text-align: center; }"
							"</style>"
							"</head>"
							"<body>"
							"<h1>" + get_status() + "</h1>"
							"</body>"
							"</html>";

	std::string header = get_Version() + " " + get_status() + "\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: " + std::to_string(message_body.length()) + "\r\n\r\n";

	std::string full_message = header + message_body;

	if (send(fd_client, full_message.c_str(), full_message.length(), 0) < 0)
	{
		std::cerr << "Send failed" << std::endl;
		// Handle send error, possibly with additional cleanup or error reporting
		return;
	}
}
void Server::Check_file_existence(std::string path)
{
	std::string filename = path;
	std::fstream file;
	file.open(filename.c_str(), std::ios::in); // Open file in read mode

	if (file.is_open())
	{
		file.close();
	}
	else
	{
		std::cout << "test" << std::endl;

		MY_exception e("404 Not Found");
		throw e;
	}
}
