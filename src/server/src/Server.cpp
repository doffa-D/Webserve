/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/29 15:19:02 by hdagdagu         ###   ########.fr       */
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

int hexToDecimal(const std::string hexString)
{
	std::stringstream ss;
	ss << std::hex << hexString;
	int decimalValue;
	ss >> decimalValue;
	return decimalValue;
}
std::string FileName(std::string filename)
{
	std::string name = "";
	for (int i = filename.length() - 1; i >= 0; i--)
	{
		if (filename[i] == '/')
			break;
		name += filename[i];
	}
	std::reverse(name.begin(), name.end());
	return name;
}
std::string extractFileName(const std::string& contentDisposition) {
    size_t filenamePos = contentDisposition.find("filename=");
    if (filenamePos != std::string::npos) {
        size_t startQuote = contentDisposition.find('"', filenamePos);
        if (startQuote != std::string::npos) {
            size_t endQuote = contentDisposition.find('"', startQuote + 1);
            if (endQuote != std::string::npos) {
                return contentDisposition.substr(startQuote + 1, endQuote - startQuote - 1);
            }
        }
    }

    return "";
}


std::vector<std::string> split_request(std::string full_request, std::string boundary)
{
	std::vector<std::string> parts;
	size_t start = 0;
	size_t end = full_request.find(boundary);
	while (end != std::string::npos)
	{
		parts.push_back(full_request.substr(start, end - start));
		start = end + boundary.length();
		end = full_request.find(boundary, start);
	}

	parts.push_back(full_request.substr(start, end));

	for (std::vector<std::string>::iterator it = parts.begin(); it != parts.end(); ++it)
	{
		size_t filenamePos = (*it).find("filename=");
		if (filenamePos != std::string::npos)
		{
			std::string filename = extractFileName(*it);
            std::cout << "File Name: " << filename << std::endl;
			size_t firstNewline = (*it).find("\r\n\r\n\r\n", filenamePos - 2);
			if (filenamePos != std::string::npos)
			{
				size_t secondNewline = (*it).find("\r\n", firstNewline + 5);
				if (secondNewline != std::string::npos)
				{
					int read_size = hexToDecimal((*it).substr(firstNewline + 5, secondNewline - firstNewline - 5));
					std::ofstream file(filename.c_str());
					 if (file.is_open()) {
						file << (*it).substr(secondNewline + 2, read_size);

						file.close();

					} 
					// std::cout << "=>" << (*it).substr(secondNewline + 2, read_size) << "<=" << std::endl;
				}
			}
		}
	}

	return parts;
}

std::string Server::read_full_request(int socket_fd)
{
	char buffer[BUFFER_SIZE + 1];
	std::string full_request;
	bool is_post = false;
	std::string boundaryValue;
	// size_t start = 0;

	while (true)
	{
		int valread = recv(socket_fd, buffer, BUFFER_SIZE, 0);
		std::cout << buffer << std::endl;
		std::string body(buffer);

		if (body.find("POST") != std::string::npos && !is_post)
		{
			size_t contentTypePos = body.find("Content-Type:");
			if (contentTypePos != std::string::npos)
			{
				size_t boundaryPos = body.find("boundary=", contentTypePos);
				if (boundaryPos != std::string::npos)
				{
					size_t boundaryEnd = body.find("\r\n", boundaryPos);
					boundaryValue = body.substr(boundaryPos + 9, boundaryEnd - (boundaryPos + 9));
					is_post = true;
				}
			}
		}
		else if(full_request.find("GET") != std::string::npos)
		{
			if(valread < BUFFER_SIZE || valread <= 0)
			{
				std::cout << "==============================" << std::endl;
				break;
			}
		}

		if (valread > 0)
		{
			buffer[valread] = '\0';
			full_request.append(buffer, buffer + valread);
			size_t lastBoundaryPos = full_request.rfind("--" + boundaryValue + "--");
			if (lastBoundaryPos != std::string::npos && is_post)
			{
				break;
			}
		}
		memset(buffer, 0, BUFFER_SIZE + 1);
	}
	// std::cout << full_request << std::endl;
	// std::vector<std::string> part = split_request(full_request, "--" + boundaryValue);
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
						fcntl(socket_fd_client, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
						FD_SET(socket_fd_client, &current_sockets);
					}
				}
				else
				{
					// Reading the full request
					std::string full_request = read_full_request(i);

						set_status("404 Not Found");
						Send_Error_Response(i); 
					// Process the received request
					if (!full_request.empty())
					{
						// parseRequest(full_request.c_str()); // Implement your request parsing logic
						// Implement your error response logic
												// printf("Received data: %s %d\n", full_request.c_str(), id++);
												// std::cout << "Received data: " << full_request.c_str() << std::endl;
												// std::cout << "==============================" << std::endl;
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
	fcntl(socket_fd_server[port_index], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
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
    std::ifstream file("/Users/hdagdagu/Desktop/Webserve/Test_page/index.html");
    std::string message_body = "";

    if (file.is_open())
    {
        std::string line;
        while (getline(file, line))
        {
            message_body += line;
        }

        file.close(); // Close the file after reading its content
    }
    else
    {
        // Handle file open error
        perror("Error opening file");
        return;
    }

    std::string header = get_Version() + " " + get_status() + "\r\n"
                         "Content-Type: text/html\r\n"
                         "Content-Length: " + std::to_string(message_body.length()) + "\r\n\r\n";

    std::string full_message = header + message_body;
	std::cout << full_message << std::endl;
	
    if (send(fd_client, full_message.c_str(), full_message.length(), 0) < 0)
    {
        // Handle send error, possibly with additional cleanup or error reporting
        perror("Send failed");
        return;
    }
}


// void Server::Send_Error_Response(int fd_client)
// {
// 	// Read the image file into a string
// 	std::ifstream image_file("/Users/hdagdagu/Desktop/Webserve/hdagdagu.jpeg", std::ios::binary);
// 	std::string image_data((std::istreambuf_iterator<char>(image_file)), std::istreambuf_iterator<char>());

// 	// Set the appropriate Content-Type header for an image (e.g., JPEG)
// 	std::string header = get_Version() + " " + get_status() + "\r\n"
// 															  "Content-Type: image/jpeg\r\n"
// 															  "Content-Length: " +
// 						 std::to_string(image_data.length()) + "\r\n\r\n";

// 	// Combine the header and image data
// 	std::string full_message = header + image_data;

// 	// Send the response
// 	if (send(fd_client, full_message.c_str(), full_message.length(), 0) < 0)
// 	{
// 		std::cerr << "Send failed" << std::endl;
// 		// Handle send error, possibly with additional cleanup or error reporting
// 		return;
// 	}
// }

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
