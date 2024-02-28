/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/28 12:12:47 by hdagdagu         ###   ########.fr       */
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


std::string location = "./upload";

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
void saveFile(std::string body, size_t filenamePos,std::string const &location)
{
	if (filenamePos != std::string::npos)
	{
		std::string filename = extractFileName(body, filenamePos);
		std::cout << location << std::endl;
		filename = location + "/" + filename;
		size_t firstContentPos = body.find("\r\n\r\n", filenamePos + 9);
		if (firstContentPos != std::string::npos)
		{
			std::ofstream file(filename.c_str());
			if (file.is_open())
			{
				file << body.substr(firstContentPos + 4);
				file.close();
			}
		}
	}
}

std::string  split_request(std::string const &body,std::string const &boundary,std::string const &location = "./upload")
{
	std::string delimiter = "--" + boundary;
	size_t pos = body.find(delimiter);
	std::vector<std::string> parts;
	std::string param;
	while (pos != std::string::npos)
	{
		size_t nextPos = body.find(boundary, pos + boundary.length() + 2);
		std::string part = body.substr(pos + boundary.length() + 2, nextPos - pos - boundary.length() - 4);
		parts.push_back(part);
		pos = nextPos;
	}
	for (size_t i = 0; i < parts.size(); i++)
	{
		size_t filenamePos = parts[i].find("filename=");
		if(filenamePos != std::string::npos)
		{
			saveFile(parts[i], filenamePos,location);
		}
		else
		{
			param += "\r\n";
			param += parts[i];
			std::cout << "==>" << parts[i] << std::endl;	
		}
	}
	return param;
}

std::string upload_file(std::string const &bodyyyyy,std::string const &location,std::string chunked,long long content_lenght,std::string boundary)
{
	std::string body;
	if(chunked == "chunked")
	{
		size_t startPos = 0;
		size_t endPos = content_lenght;
		while (startPos < endPos)
		{
			size_t sizePos = bodyyyyy.find("\r\n", startPos);
			if (sizePos != std::string::npos)
			{
				std::string chunkSizeHex = bodyyyyy.substr(startPos, sizePos - startPos);
				int chunkSize = std::stoi(chunkSizeHex, 0, 16);
				
				startPos = sizePos + 2;
				if (startPos + chunkSize <= endPos)
				{
					std::string chunkData = bodyyyyy.substr(startPos, chunkSize);
					body += chunkData;
					startPos += chunkSize + 2;
				}
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		body = bodyyyyy;
	}
	return split_request(body,boundary,location);
}

std::string readFileContent(const std::string &filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		return "";
	}

	std::ostringstream contentStream;
	contentStream << fileStream.rdbuf();
	return contentStream.str();
}
void Wb_Server::listen_to_multiple_clients(const Parser&  parsedData)
{
	std::string httpRequest = "";
	std::string resss = "";
	fd_set fd_set_Read, Tmp_fd_set_Read;
	fd_set fd_set_write, Tmp_fd_set_write;
	FD_ZERO(&fd_set_Read);
	FD_ZERO(&fd_set_write);
	std::map<int,bool > checker;
	for (int i = 0; i < Number_of_ports; i++)
	{
		FD_SET(socket_fd_server[i], &fd_set_Read);
	}
	std::cout << "Server is running" << std::endl;

	while (true)
	{
		Tmp_fd_set_Read = fd_set_Read;
		Tmp_fd_set_write = fd_set_write;

		if (select(FD_SETSIZE, &Tmp_fd_set_Read, &Tmp_fd_set_write, NULL, NULL) < 0)
		{
			perror("Error in select");
			exit(1); // respond with server error 
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
						exit(EXIT_FAILURE); // respond with server error 
					}
					else
					{
						// cout << "port: " << ntohs(address[i].sin_port) << endl;
						// cout << "ip: " << str_utils::ip(ntohl(address[i].sin_addr.s_addr)) << endl;
						fcntl(socket_fd_client, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
						FD_SET(socket_fd_client, &fd_set_Read);
						checker[socket_fd_client] = true;

					}
					
				}
				else
				{
					httpRequest = read_full_request(i, fd_set_Read, fd_set_write);
					
				}
			}
			else if (FD_ISSET(i, &Tmp_fd_set_write))
			{
					// try
					// {
						std::string chunked;
						size_t pos = httpRequest.find("\r\n\r\n");
						std::string body = httpRequest.substr(pos + 4);
						pos =  httpRequest.find("boundary=");
						std::string boundary = httpRequest.substr(pos + 9, httpRequest.find("\r\n",pos) - (pos + 9));
						pos = httpRequest.find("chunked");
						if (pos != std::string::npos)
							chunked = "chunked";
						else
							chunked = "";
						

						upload_file(body,location,chunked,body.size(),boundary);						
						// std::map<std::string, std::string> env;
						// env["SCRIPT_NAME"] = "ll.py"; //It will be the name of the file
						// env["SCRIPT_FILENAME"] = "./ll.py"; //It will be the path of the file 
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
					// std::cout << checker[i] << std::endl;
					if (checker[i] == true)
					{
						Request request;
						request.Parse_Request(httpRequest);
						Response	response;
						response.setReq(request);
						resss = response.ft_Response(i, parsedData);
						checker[i] = false;
					}
					if (send_full_response(i, resss) == true)
					{
						checker[i] = true;
						FD_CLR(i, &fd_set_write);
						close(i);
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
		if(client_index != -1)
			clients_request.erase(clients_request.begin() + client_index);
		return "";
	}

	buffer[valread] = '\0';
	std::string bufferr(buffer, valread);
	if (client_index == -1)
	{
		Client newClient;
		newClient.fd = socket_fd;
		newClient.fisrtboundaryValue = "";
		newClient.header = "";
		newClient.lastboundaryValue = "";
		newClient.isFile = false;
		newClient.bytes_read = 0;
		newClient.contentLength = 0;
		newClient.first_respont = true;
		ContentLength(bufferr, newClient);
		size_t contentLengthPos = bufferr.find("Transfer-Encoding: chunked");
		if (contentLengthPos != std::string::npos)
			newClient.isChunked = true;
		else
			newClient.isChunked = false;
		size_t start = bufferr.find("\r\n\r\n");
		if (start != std::string::npos)
		{
			newClient.header = bufferr.substr(0, start + 4);
			newClient.start = start + 4;
		}
		else
			newClient.start = 0;

		clients_request.push_back(newClient);
		client_index = static_cast<int>(clients_request.size()) - 1;
	}
	clients_request[client_index].buffer.append(bufferr);
	clients_request[client_index].bytes_read += valread;

	if (clients_request[client_index].contentLength == 0)
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
	else
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