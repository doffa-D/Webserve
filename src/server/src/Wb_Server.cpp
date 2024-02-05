/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/05 11:36:35 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../include/Server.hpp"
#include "../../../include/header.hpp"


int Wb_Server::getNumber_of_ports()
{
	return this->Number_of_ports;
}

Wb_Server::Wb_Server(std::vector<std::pair<Uint, int> > hAndP)
{
	this->HostAndPorts = hAndP;
	
	// MIME_types_init();
	this->Number_of_ports = hAndP.size();
	getMyIpAddress();
	for (size_t i = 0; i < hAndP.size(); i++)
		Setup_Server(i);		  // Setup_Server function is used to setup the server
	listen_to_multiple_clients(); // listen_to_multiple_clients function is used to listen to multiple clients_request
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
		std::cout << "file name: " << filename << std::endl;
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
}

void split_request(Client &client)
{
	std::string header;
	std::string body;
	size_t bodyStart = client.buffer.find("\r\n\r\n");
	if (bodyStart != std::string::npos)
	{
		header = client.buffer.substr(0, bodyStart);
		body = client.buffer.substr(bodyStart + 2);
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
	client.buffer.clear();
	client.buffer = header;
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
	// bool has_finished = false;

	for (size_t i = 0; i < clients_request.size(); i++)
	{
		if (clients_request[i].fd == socket_fd)
		{
			client_index = static_cast<int>(i);
			break;
		}
	}
	// std::cout << "client_index: " << client_index << std::endl;

	int valread = recv(socket_fd, buffer, BUFFER_SIZE, 0);
	// std::cout << "valread: " << valread << std::endl;
	// exit(0);
	if (valread <= 0)
	{
		close(socket_fd);
		FD_CLR(socket_fd, &fd_set_Read);
		clients_request.erase(clients_request.begin() + client_index);
		return "";
	}
	// std::cout << valread << std::endl;
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
		if (!clients_request[client_index].lastboundaryValue.empty())
		{
			std::cout << "this is chuncked" << " hi from POST" << std::endl;

			clients_request[client_index].bytes_read += valread;
			if (clients_request[client_index].isChunked)
			{
				size_t searchLength = std::min(static_cast<size_t>(10), clients_request[client_index].bytes_read);
				std::string substring = clients_request[client_index].buffer.substr(clients_request[client_index].bytes_read - searchLength, searchLength);
				size_t foundPos = substring.rfind("\r\n0\r\n");
				if (foundPos != std::string::npos)
				{
					std::cout << "hello" << std::endl;
					std::string body;
					size_t startPos = clients_request[client_index].start;
					size_t endPos = clients_request[client_index].bytes_read;

					while (startPos < endPos)
					{
						// Read the chunk size
						size_t sizePos = clients_request[client_index].buffer.find("\r\n", startPos);
						if (sizePos == std::string::npos)
						{
							// Handle error, invalid chunk format
							break;
						}

						std::string chunkSizeHex = clients_request[client_index].buffer.substr(startPos, sizePos - startPos);
						int chunkSize = std::stoi(chunkSizeHex, 0, 16);

						// Move to the beginning of the chunk data
						startPos = sizePos + 2; // Skip "\r\n"

						// Read the chunk
						if (startPos + chunkSize <= endPos)
						{
							std::string chunkData = clients_request[client_index].buffer.substr(startPos, chunkSize);
							body += chunkData;

							// Move to the next chunk
							startPos += chunkSize + 2; // Skip the chunk data and "\r\n"
						}
						else
						{
							// Not enough data for the complete chunk, wait for more data
							break;
						}
					}
					clients_request[client_index].start = startPos;
					clients_request[client_index].buffer.clear();
					clients_request[client_index].buffer = clients_request[client_index].header + body;

					// std::cout << clients_request[client_index].buffer << std::endl;
					// exit(0);
					split_request(clients_request[client_index]);
					FD_CLR(socket_fd, &fd_set_Read);
					FD_SET(socket_fd, &fd_set_write);

					std::string request = clients_request[client_index].buffer;
					clients_request.erase(clients_request.begin() + client_index);
					return request;
				}
			}
			else
			{
				std::cout << "this not chuncked" << " hi from POST" << std::endl;

				if (clients_request[client_index].bytes_read - clients_request[client_index].startFContent == clients_request[client_index].contentLength)
				{
					split_request(clients_request[client_index]);
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

std::string readFileContent(const std::string &filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		return ""; // Handle file not found or other errors
	}

	std::ostringstream contentStream;
	contentStream << fileStream.rdbuf();
	return contentStream.str();
}

void Wb_Server::listen_to_multiple_clients()
{

	std::string htmlResponse = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
	htmlResponse += "<!DOCTYPE html>\n";
	htmlResponse += "<html lang=\"en\">\n";
	htmlResponse += "  <head>\n";
	htmlResponse += "    <meta charset=\"UTF-8\" />\n";
	htmlResponse += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\" />\n";
	htmlResponse += "    <title>Text File Content</title>\n";
	htmlResponse += "    <style>\n";
	htmlResponse += "      .square {\n";
	htmlResponse += "        width: 600px; /* Set your desired width */\n";
	htmlResponse += "        height: 1000px; /* Set your desired height */\n";
	htmlResponse += "        border: 2px solid #000; /* Set border style and color */\n";
	htmlResponse += "        padding: 10px; /* Add padding for better readability */\n";
	htmlResponse += "        overflow: auto; /* Add scrollbars if content overflows */\n";
	htmlResponse += "      }\n";
	htmlResponse += "    </style>\n";
	htmlResponse += "  </head>\n";
	htmlResponse += "  <body>\n";
	htmlResponse += "    <h1>Text File Content</h1>\n";

	// Read the content of the text file
	std::string textFilePath = "/goinfre/hdagdagu/600k.txt";
	std::string fileContent = readFileContent(textFilePath);

	// Include the file content in the HTML response within a square
	htmlResponse += "    <div class=\"square\">\n";
	htmlResponse += "      <pre>\n";
	htmlResponse += fileContent;
	htmlResponse += "      </pre>\n";
	htmlResponse += "    </div>\n";

	htmlResponse += "  </body>\n";
	htmlResponse += "</html>\n";

	// std::string filePath = "/goinfre/hdagdagu/vv.mp4";

	// std::ifstream fileStream(filePath, std::ios::in | std::ios::binary);

	// // Read the contents of the file into a string
	// std::string fileContent((std::istreambuf_iterator<char>(fileStream)),
	// 						std::istreambuf_iterator<char>());

	// // Generate the HTTP response with appropriate headers
	// std::string htmlResponse = "HTTP/1.1 200 OK\r\n";
	// htmlResponse += "Content-Type: application/octet-stream\r\n";
	// htmlResponse += "Content-Disposition: attachment; filename=\"vv.mp4\"\r\n";
	// htmlResponse += "Content-Length: " + std::to_string(fileContent.size()) + "\r\n\r\n";
	// htmlResponse += fileContent;

	std::string request = "";
	fd_set fd_set_Read, Tmp_fd_set_Read;
	fd_set fd_set_write, Tmp_fd_set_write;
	// (void)Tmp_fd_set_write;
	FD_ZERO(&fd_set_Read);
	FD_ZERO(&fd_set_write);

	for (int i = 0; i < Number_of_ports; i++)
	{
		FD_SET(socket_fd_server[i], &fd_set_Read);
	}

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
						fcntl(socket_fd_client, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
						FD_SET(socket_fd_client, &fd_set_Read);
					}
				}
				else
				{
					request = read_full_request(i, fd_set_Read, fd_set_write);
				}
			}
			else if (FD_ISSET(i, &Tmp_fd_set_write))
			{
				if (!request.empty())
				{
					std::cout << "request: " << request << std::endl;
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

Wb_Server::~Wb_Server()
{
	// std::cout << "Wb_Server destructor called. " << std::endl;
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
	
	cout << "Host: " << htonl(HostAndPorts[port_index].first) << endl;
	cout << "inet Host: " << htonl(inet_addr("127.0.0.1")) << endl;

	address[port_index].sin_addr.s_addr = INADDR_ANY;

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

void Wb_Server::getMyIpAddress()
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
