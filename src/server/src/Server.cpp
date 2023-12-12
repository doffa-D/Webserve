/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/12 13:13:04 by hdagdagu         ###   ########.fr       */
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

std::string extractFileName(const std::string &contentDisposition, size_t filenamePos)
{

	size_t startQuote = contentDisposition.find('"', filenamePos);
	if (startQuote != std::string::npos)
	{
		size_t endQuote = contentDisposition.find('"', startQuote + 1);
		if (endQuote != std::string::npos)
		{
			return contentDisposition.substr(startQuote + 1, endQuote - startQuote - 1);
		}
	}

	return "";
}

void saveFile(std::string body, size_t filenamePos)
{
	if (filenamePos != std::string::npos)
	{
		std::string filename = extractFileName(body, filenamePos);
		filename = "/goinfre/hdagdagu/test/" + filename;
		size_t firstContentPos = body.find("\r\n\r\n", filenamePos + 9);
		if (firstContentPos != std::string::npos)
		{
			std::ofstream file(filename.c_str());
			if (file.is_open())
			{
				std::cout << "file is open" << std::endl;
				file << body.substr(firstContentPos + 4);
				file.close();
				std::cout << "file is closed" << std::endl;
			}
		}
	}
	exit(0);
}
#include <stdio.h>
void split_request(Client &client)
{
	std::string header;
	std::string body;
	size_t bodyStart = client.buffer.find("\r\n\r\n");
	if (bodyStart != std::string::npos)
	{
		header = client.buffer.substr(0, bodyStart);
		body = client.buffer.substr(bodyStart + 2);
		// std::cout << header << std::endl
	}

	std::string boundary = "--" + client.fisrtboundaryValue;
	size_t pos = body.find(boundary);
	std::vector<std::string> parts;
	while (pos != std::string::npos)
	{
		size_t nextPos = body.find(boundary, pos + boundary.length() + 2);
		std::string part = body.substr(pos + boundary.length() + 2, nextPos - pos - boundary.length() - 2);
		parts.push_back(part);
		pos = nextPos;
	}

	for (size_t i = 0; i < parts.size(); ++i)
	{
		size_t filenamePos = parts[i].find("filename=");
		if (filenamePos != std::string::npos)
		{
			saveFile(parts[i], filenamePos);
		}
		else
		{
			header += "\r\n\r\n";
			header += parts[i];
		}
	}
	std::cout << header << ">" << std::endl;
	return;
}

void ContentLength(std::string request, Client &Client)
{
	std::string content = "Content-Length: ";
	size_t pos = request.find(content);
	if (pos != std::string::npos)
	{
		std::string substring = request.substr(pos + content.length());
		size_t end = substring.find_first_not_of("0123456789");
		std::string length = substring.substr(0, end);
		Client.contentLength = std::stoi(length);
		Client.startFContent = request.find("\r\n\r\n") + 4;
	}
}

std::string Server::read_full_request(int socket_fd, fd_set &current_sockets)
{
	char buffer[BUFFER_SIZE];
	std::string full_request;
	int client_index = -1;
	// bool has_finished = false;

	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].fd == socket_fd)
		{
			client_index = static_cast<int>(i);
			break;
		}
	}

	while (true)
	{
		int valread = recv(socket_fd, buffer, BUFFER_SIZE, 0);

		if (valread > 0)
		{
			buffer[valread] = '\0';
			std::string body(buffer, valread);
			if (client_index == -1)
			{
				Client newClient;
				newClient.fd = socket_fd;
				newClient.fisrtboundaryValue = "";
				newClient.lastboundaryValue = "";
				newClient.isFile = false;
				size_t contentTypePos = body.find("Content-Type:");
				ContentLength(body, newClient);
				if (contentTypePos != std::string::npos)
				{
					size_t boundaryPos = body.find("boundary=", contentTypePos);
					if (boundaryPos != std::string::npos)
					{
						size_t start = body.find("\r\n", boundaryPos);
						newClient.fisrtboundaryValue = body.substr(boundaryPos + 9, start - (boundaryPos + 9));
						newClient.lastboundaryValue = "--" + newClient.fisrtboundaryValue + "--";
					}
				}
				size_t contentLengthPos = body.find("Transfer-Encoding: chunked");
				if (contentLengthPos != std::string::npos)
					newClient.isChunked = true;
				else
					newClient.isChunked = false;
				clients.push_back(newClient);
				client_index = static_cast<int>(clients.size()) - 1;
			}

			clients[client_index].buffer.append(body);
			// std::cout << "==> " << body << std::endl;
			if (!clients[client_index].lastboundaryValue.empty())
			{
				clients[client_index].bytes_read += valread;
				if (clients[client_index].isChunked)
				{
				}
				{
					if (clients[client_index].bytes_read - clients[client_index].startFContent == clients[client_index].contentLength)
					{
						std::cout << "this is not chunked" << std::endl;
						// std::cout << "==> " << clients[client_index].buffer << std::endl;
						split_request(clients[client_index]);
						// clients.erase(clients.begin() + client_index);
						close(socket_fd);
						FD_CLR(socket_fd, &current_sockets);
					}
				}
			}
			else
			{
				std::string htmlResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
				htmlResponse += "<!DOCTYPE html>\n";
				htmlResponse += "<html lang=\"en\">\n";
				htmlResponse += "  <head>\n";
				htmlResponse += "    <meta charset=\"UTF-8\" />\n";
				htmlResponse += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n";
				htmlResponse += "    <title>File Upload</title>\n";
				htmlResponse += "  </head>\n";
				htmlResponse += "  <body>\n";
				htmlResponse += "    <h1>File Upload</h1>\n";
				htmlResponse += "    <form\n";
				htmlResponse += "      action=\"/upload\"\n";
				htmlResponse += "      method=\"post\"\n";
				htmlResponse += "      enctype=\"multipart/form-data\"\n";
				htmlResponse += "    >\n";
				htmlResponse += "      <label for=\"file\">Select a file:</label>\n";
				htmlResponse += "      <input type=\"file\" name=\"file\" id=\"file\" required />\n";
				htmlResponse += "      <br />\n";
				htmlResponse += "      <input type=\"submit\" value=\"Upload File\" />\n";
				htmlResponse += "    </form>\n";
				htmlResponse += "  </body>\n";
				htmlResponse += "</html>\n";

				write(socket_fd, htmlResponse.c_str(), htmlResponse.size());
				(void)current_sockets;
				// clients.erase(clients.begin() + client_index);
				close(socket_fd);
				FD_CLR(socket_fd, &current_sockets);
			}
		}
		break;
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
						fcntl(socket_fd_client, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
						FD_SET(socket_fd_client, &current_sockets);
					}
				}
				else
				{
					read_full_request(i, current_sockets);
					// close(i);
					// FD_CLR(i, &current_sockets);
					// read_full_request(i, current_sockets);
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
															  "Content-Length: " +
						 std::to_string(message_body.length()) + "\r\n\r\n";

	std::string full_message = header + message_body;

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
