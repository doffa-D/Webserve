/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/26 14:34:31 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"



Wb_Server::Wb_Server(const Parser& parsedData)
{
	std::vector<std::pair<Uint, int> > hAndP = parsedData.getHostsAndPorts();
	this->HostAndPorts = hAndP;
	this->Number_of_ports = hAndP.size();
	for (size_t i = 0; i < hAndP.size(); i++)
		Setup_Server(i);		  // Setup_Server function is used to setup the server
	listen_to_multiple_clients(parsedData); // listen_to_multiple_clients function is used to listen to multiple clients_request
}

void Wb_Server::Setup_Server(int port_index)
{
	socket_fd_server[port_index] = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd_server[port_index] == -1)
	{
		std::cout << "Failed to create socket for port " << HostAndPorts[port_index].second << std::endl;
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	if (setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1 || setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) == -1)
	{
		std::cout << "Failed to set socket options for port " << HostAndPorts[port_index].second << std::endl;
		exit(EXIT_FAILURE);
	}

	// initialize address
	address[port_index].sin_family = AF_INET;
	address[port_index].sin_port = htons(HostAndPorts[port_index].second);
	address[port_index].sin_addr.s_addr = htonl(HostAndPorts[port_index].first);

	if (bind(socket_fd_server[port_index], (struct sockaddr *)&address[port_index], sizeof(address[port_index])) == -1)
	{
		std::cout << "Failed to bind to port " << HostAndPorts[port_index].second << std::endl;
		perror("");
		exit(EXIT_FAILURE);
	}

	if (listen(socket_fd_server[port_index], 10) == -1)
	{
		std::cout << "Failed to listen on port " << HostAndPorts[port_index].second << std::endl;
		exit(EXIT_FAILURE);
	}
	fcntl(socket_fd_server[port_index], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	// std::cout << "Listening on port " << HostAndPorts[port_index].second << std::endl;
}

// #include <arpa/inet.h> to check later
void Wb_Server::listen_to_multiple_clients(const Parser&  parsedData)
{
	std::string httpRequest = "";
	fd_set fd_set_Read, Tmp_fd_set_Read;
	fd_set fd_set_write, Tmp_fd_set_write;
	FD_ZERO(&fd_set_Read);
	FD_ZERO(&fd_set_write);

	for (int i = 0; i < Number_of_ports; i++)
	{
		FD_SET(socket_fd_server[i], &fd_set_Read);
	}
	std::cout << "Server is running"<< std::endl;
	while (true)
	{
		Tmp_fd_set_Read = fd_set_Read;
		Tmp_fd_set_write = fd_set_write;

		if (select(FD_SETSIZE, &Tmp_fd_set_Read, &Tmp_fd_set_write, NULL, NULL) < 0)
		{
			perror("Error in select");
			exit(1);
		}

		for (int i = 0; i < FD_SETSIZE; ++i)
		{
			if (FD_ISSET(i, &Tmp_fd_set_Read))
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
						// cout << "port: " << ntohs(address[i].sin_port) << endl;
						// cout << "ip: " << str_utils::ip(ntohl(address[i].sin_addr.s_addr)) << endl;
						fcntl(socket_fd_client, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
						FD_SET(socket_fd_client, &fd_set_Read);
					}
					
				}
				else
				{
					httpRequest = read_full_request(i, fd_set_Read, fd_set_write);
				}
			}
			else if (FD_ISSET(i, &Tmp_fd_set_write))
			{
				if (!httpRequest.empty())
				{
						std::string htmlResponse = "hello world";
					// try
					// {
						// std::cout << "request: " << httpRequest << std::endl;
						
						// std::map<std::string, std::string> env;
						// env["SCRIPT_NAME"] = "ll.py"; //It will be the name of the file
						// env["SCRIPT_FILENAME"] = "/Users/rrhnizar/Desktop/DofaCgi/ll.py"; //It will be the path of the file 
						// env["CONTENT_TYPE"] = "application/x-www-form-urlencoded";
						// env["REQUEST_METHOD"] = "POST";
						// env["CONTENT_LENGTH"] = "12";
						// env["QUERY_STRING"] = "";//It will be empty in the POST and fill it in GET so you can put the form parameter just if you have it
						// env["SERVER_PROTOCOL"] = "HTTP/1.1"; 
						// env["SERVER_NAME"] = "WebServer";
						// env["REDIRECT_STATUS"] = "200";
					
						// std::string body = "Hello=Wollllrld!"; // it will be empty in GET !!!
						// std::string bin = "/usr/bin/python3";

						// CGI cgi_obj(body, env, bin);
						// std::string respont = cgi_obj.fill_env();
						// std::cout <<  respont << std::endl;
						
					// }
					// catch(const std::exception& e)
					// {
					// 	std::cerr << e.what() << '\n';
					// }
					

					Request request;
					request.Parse_Request(httpRequest);
					
					Response	response;
					response.setReq(request);
					response.ft_Response(i, parsedData);

					if (send_full_response(i, htmlResponse) == true)
					{
						FD_CLR(i, &fd_set_write);
						close(i);
					}
				}
			}
		}
	}
}


bool Wb_Server::check_socket(int i)
{
	for (int j = 0; j < getNumber_of_ports(); j++)
	{
		if (socket_fd_server[j] == i)
			return true;
	}
	return false;
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
		Client.contentLength = std::strtold(length.c_str(), NULL);
		Client.startFContent = request.find("\r\n\r\n") + 4;
	}
}

int Wb_Server::find_clinet_response(std::vector<SendTracker> &clients_respont, int socket_fd)
{
	int client_index = -1;
	for (size_t i = 0; i < clients_respont.size(); i++)
	{
		if (clients_respont[i].getFd() == socket_fd)
		{
			client_index = static_cast<int>(i);
			break;
		}
	}
	return client_index;
}

bool Wb_Server::send_full_response(int socket_fd, std::string respont)
{
	int client_index = find_clinet_response(clients_respont, socket_fd);
	bool respont_status;
	if (client_index == -1)
	{
		SendTracker newClient(respont);
		newClient.setFd(socket_fd);
		clients_respont.push_back(newClient);
		client_index = static_cast<int>(clients_respont.size()) - 1;
	}
	respont_status = clients_respont[client_index].writeNextChunk();
	if (respont_status == true)
	{
		clients_respont.erase(clients_respont.begin() + client_index);
	}
	return respont_status;
}

std::string Wb_Server::read_full_request(int socket_fd, fd_set &fd_set_Read, fd_set &fd_set_write)
{
	char buffer[BUFFER_SIZE + 1];
	std::string full_request;
	int client_index = -1;

	for (size_t i = 0; i < clients_request.size(); i++)
	{
		if (clients_request[i].fd == socket_fd)
		{
			client_index = static_cast<int>(i);
			break;
		}
	}

	int valread = recv(socket_fd, buffer, BUFFER_SIZE, 0);
	if (valread <= 0)
	{
		close(socket_fd);
		FD_CLR(socket_fd, &fd_set_Read);
		clients_request.erase(clients_request.begin() + client_index);
		return "";
	}
	if (valread > 0)
	{
		buffer[valread] = '\0';
		std::string body(buffer, valread);
		if (client_index == -1)
		{
			Client newClient;
			newClient.fd = socket_fd;
			newClient.fisrtboundaryValue = "";
			newClient.header = "";
			newClient.lastboundaryValue = "";
			newClient.isFile = false;
			newClient.bytes_read = 0;
			ContentLength(body, newClient);
			size_t method = body.find("POST");
			if (method != std::string::npos)
				newClient.method = "POST";
			else
				newClient.method = "GET";
			size_t contentLengthPos = body.find("Transfer-Encoding: chunked");
			if (contentLengthPos != std::string::npos)
				newClient.isChunked = true;
			else
				newClient.isChunked = false;
			size_t start = body.find("\r\n\r\n");
			if (start != std::string::npos)
			{
				newClient.header = body.substr(0, start + 4);
				newClient.start = start + 4;
			}
			else
				newClient.start = 0;

			clients_request.push_back(newClient);
			client_index = static_cast<int>(clients_request.size()) - 1;
		}
		clients_request[client_index].buffer.append(body);
		clients_request[client_index].bytes_read += valread;

		if (clients_request[client_index].method == "POST")
		{
			if (clients_request[client_index].isChunked)
			{
				size_t searchLength = std::min(static_cast<size_t>(10), clients_request[client_index].bytes_read);
				std::string substring = clients_request[client_index].buffer.substr(clients_request[client_index].bytes_read - searchLength, searchLength);
				size_t foundPos = substring.rfind("\r\n0\r\n");
				if (foundPos != std::string::npos)
				{
					FD_CLR(socket_fd, &fd_set_Read);
					FD_SET(socket_fd, &fd_set_write);
					std::string request = clients_request[client_index].buffer;
					clients_request.erase(clients_request.begin() + client_index);
					return request;
				}
			}
			else
			{
				if (clients_request[client_index].bytes_read - clients_request[client_index].startFContent == clients_request[client_index].contentLength)
				{
					FD_CLR(socket_fd, &fd_set_Read);
					FD_SET(socket_fd, &fd_set_write);
					std::string request = clients_request[client_index].buffer;
					clients_request.erase(clients_request.begin() + client_index);
					return request;
				}
			}
		}
		else
		{
			size_t pos = clients_request[client_index].buffer.find("\r\n\r\n");
			if (pos != std::string::npos)
			{
				FD_CLR(socket_fd, &fd_set_Read);
				FD_SET(socket_fd, &fd_set_write);
				std::string request = clients_request[client_index].buffer;
				clients_request.erase(clients_request.begin() + client_index);
				return request;
			}
		}
	}

	return "";
}


Wb_Server::~Wb_Server()
{
	// std::cout << "Wb_Server destructor called. " << std::endl;
}
int Wb_Server::getNumber_of_ports()
{
	return this->Number_of_ports;
}