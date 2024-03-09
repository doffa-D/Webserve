/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/09 12:57:48 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

Wb_Server::Wb_Server(const Parser& parsedData)
{
	try
	{
		std::vector<std::pair<Uint, int> > hAndP = parsedData.getHostsAndPorts();
		this->HostAndPorts = hAndP;
		this->Number_of_ports = hAndP.size();
		this->bindFailed = 0;
		for (size_t i = 0; i < hAndP.size(); i++)
			Setup_Server(i);	  // Setup_Server function is used to setup the server
		listen_to_multiple_clients(parsedData); // listen_to_multiple_clients function is used to listen to multiple clients_request
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		exit(1);
	}
}
void Wb_Server::Setup_Server(int port_index)
{
	socket_fd_server[port_index] = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd_server[port_index] == -1)
	{
		std::string error = "[ socket: Port: " + std::to_string(HostAndPorts[port_index].second) + " ]";
		perror(error.c_str());
	}
	int opt = 1;
	if (setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1 || setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) == -1)
	{
		std::string error = "[ setsockopt: Port: " + std::to_string(HostAndPorts[port_index].second) + " ]";
		perror(error.c_str());
	}

	address[port_index].sin_family = AF_INET;
	address[port_index].sin_port = htons(HostAndPorts[port_index].second);
	address[port_index].sin_addr.s_addr = htonl(HostAndPorts[port_index].first);

	if (bind(socket_fd_server[port_index], (struct sockaddr *)&address[port_index], sizeof(address[port_index])) == -1)
	{
		std::string error = "[ bind: Port: " + std::to_string(HostAndPorts[port_index].second) + " ]";

		this->bindFailed++;
		perror(error.c_str());

	}

	if (listen(socket_fd_server[port_index], 128) == -1)
	{
		std::string error = "[ listen: Port: " + std::to_string(HostAndPorts[port_index].second) + " ]";
		perror(error.c_str());


	}
	if(fcntl(socket_fd_server[ port_index], F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
	{
		std::string error = "[ fcntl: Port: " + std::to_string(HostAndPorts[port_index].second) + " ]";
		perror(error.c_str());

	}
	if(this->bindFailed == this->Number_of_ports)
	{
		std::cerr << "[ All ports are already in use ]" << std::endl;
		exit(1);
	}
}

static void	check_session_expiration(VecStringString& cookie)
{
	for (size_t i = 0; i < cookie.size();i++)
	{
		if (time(0) >= str_utils::to_Time_t(cookie[i].second.c_str()))
		{
			std::string fullPath = "./session/sess_" + cookie[i].first;
			std::remove(fullPath.c_str());
			cookie.erase(cookie.begin() + i);
		}
	}
}

void Wb_Server::listen_to_multiple_clients(const Parser& parsedData)
{
    FD_ZERO(&fd_set_Read);
    FD_ZERO(&fd_set_write);
    std::map<int, RequestClient> Client;

	struct timeval timeout;

    for (int i = 0; i < Number_of_ports; i++)
    {
        FD_SET(socket_fd_server[i], &fd_set_Read);
    }
    std::cout << "Server is running" << std::endl;

    while (true)
    {
        Tmp_fd_set_Read = fd_set_Read;
        Tmp_fd_set_write = fd_set_write;
        timeout.tv_sec = 1; 
        timeout.tv_usec = 0;
		check_session_expiration(track_cookie);
		int ready = select(FD_SETSIZE, &Tmp_fd_set_Read, &Tmp_fd_set_write, NULL, &timeout);
        if (ready < 0)
        {
            perror("Error in select");
        }
		else if(ready == 0)
		{
			for(int SocketID = 3; SocketID < FD_SETSIZE; SocketID++)
			{
				if(Client[SocketID].CheckSeend == "finish")
				{
					if(Client[SocketID].keepAlive == true && difftime(time(0) , Client[SocketID].KeepAliveTimeOut) > KEEPALIVE_TIMEOUT)
					{
						close(SocketID);
						FD_CLR(SocketID, &Tmp_fd_set_write);
						FD_CLR(SocketID, &Tmp_fd_set_Read);
						FD_CLR(SocketID, &fd_set_Read);
						FD_CLR(SocketID, &fd_set_write);
						Client.erase(SocketID);
					}
				}
			}

		}
		else if (ready > 0)
		{
			for (int SocketID = 3; SocketID < FD_SETSIZE; ++SocketID)
			{
				if(Client[SocketID].CheckSeend == "finish")
				{
					if(Client[SocketID].keepAlive == true && difftime(time(0) , Client[SocketID].KeepAliveTimeOut) > KEEPALIVE_TIMEOUT)
					{
						close(SocketID);
						FD_CLR(SocketID, &Tmp_fd_set_write);
						FD_CLR(SocketID, &Tmp_fd_set_Read);
						FD_CLR(SocketID, &fd_set_Read);
						FD_CLR(SocketID, &fd_set_write);
						Client.erase(SocketID);
					}
				}
				else if (FD_ISSET(SocketID, &Tmp_fd_set_Read))
				{
					if (check_socket(SocketID) == true)
					{
						socklen_t addrlen = sizeof(address[SocketID]);
						if ((socket_fd_client = accept(SocketID, (struct sockaddr *)&address[SocketID], (socklen_t *)&addrlen)) < 0)
						{
							perror("accept ");
						}
						else
						{
							if(fcntl(socket_fd_client, F_SETFL, O_NONBLOCK, FD_CLOEXEC) == -1)
							{
								std::string error = "[ fcntl: Port: " + std::to_string(HostAndPorts[SocketID].second) + " ]";
								perror(error.c_str());
							}
							FD_SET(socket_fd_client, &fd_set_Read);
						}
					}
					else
					{
					    std::string httpRequest = "";

						httpRequest = read_full_request(SocketID, fd_set_Read, fd_set_write);
						if (FD_ISSET(SocketID, &fd_set_write))
						{
							RequestClient requestClient;
							std::string header;
							size_t pos = httpRequest.find("\r\n\r\n");
							if(pos != std::string::npos)
							{
								header = httpRequest.substr(0,pos);
								size_t pos = header.find("Connection: keep-alive\r\n");
								if (pos != std::string::npos)
								{
									requestClient.keepAlive = true;
									requestClient.KeepAliveTimeOut = time(0);
								}
								else
									requestClient.keepAlive = false;

							}
							requestClient.request = httpRequest;
							requestClient.SocketID = SocketID;
							requestClient.byteSent = 0;
							requestClient.KeepAliveTimeOut = time(0);
							requestClient.CheckSeend = "init";
							Client[SocketID] = requestClient;

						}
					}
				}
				else if (FD_ISSET(SocketID, &Tmp_fd_set_write))
				{
					if (Client[SocketID].CheckSeend == "init")
					{
						Request request;
						request.Parse_Request(Client[SocketID].request);
						Response response(track_cookie);
						response.setReq(request);
						Client[SocketID].ClientRespont = response.ft_Response(parsedData);
						Client[SocketID].CheckSeend = "checker";
					}
					if (send_full_response(Client,SocketID) == true)
					{
						FD_CLR(SocketID, &fd_set_write);
						Client[SocketID].CheckSeend = "finish";
						Client[SocketID].KeepAliveTimeOut = time(0);
						if (Client[SocketID].keepAlive == false)
						{
							FD_CLR(SocketID, &fd_set_write);
							close(SocketID);
							Client.erase(SocketID);
						}
						else
						{
							FD_SET(SocketID, &fd_set_Read);
						}
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
bool Wb_Server::send_full_response(std::map<int, RequestClient>& Client, int SocketID)
{
    RequestClient& client = Client[SocketID];
    ssize_t bytesWritten = 0;
    size_t bytesRemaining = client.ClientRespont.size() - client.byteSent;
    bytesWritten = write(client.SocketID, client.ClientRespont.c_str() + client.byteSent, bytesRemaining);
    if (bytesWritten == -1) {
		// std::cout << "Error in write" << std::endl;
		FD_CLR(client.SocketID, &fd_set_write);
		Client.erase(SocketID);
		close(SocketID);
		return false;
    }
    client.byteSent += bytesWritten;
    if (client.byteSent >= static_cast<ssize_t>(client.ClientRespont.size()))
        return true; 
    return false;
}

int find_client(std::vector<Client> &clients_request, int socket_fd)
{
	int client_index = -1;
	for (size_t i = 0; i < clients_request.size(); i++)
	{
		if (clients_request[i].fd == socket_fd)
		{
			client_index = static_cast<int>(i);
			break;
		}
	}
	return client_index;
}
std::string unchunked_request(std::string const &request, int start,int bytes_read)
{
	std::string body;
	size_t startPos = start;
	size_t endPos = bytes_read;

	while (startPos < endPos)
	{
		size_t sizePos = request.find("\r\n", startPos);
		if (sizePos != std::string::npos)
		{
			std::string chunkSizeHex = request.substr(startPos, sizePos - startPos);
			int chunkSize = static_cast<int>(std::strtol(chunkSizeHex.c_str(),NULL,16));
			startPos = sizePos + 2;
			if (startPos + chunkSize <= endPos)
			{
				std::string chunkData = request.substr(startPos, chunkSize);
				body += chunkData;
				startPos += chunkSize + 2;
			}
			else
			{
				break;
			}
		}
	}
	return body;
}
std::string Wb_Server::read_full_request(int socket_fd, fd_set &fd_set_Read, fd_set &fd_set_write)
{
	char buffer[BUFFER_SIZE + 1];
	std::string full_request;
	int client_index = -1;
	client_index = find_client(clients_request, socket_fd);

	ssize_t valread = recv(socket_fd, buffer, BUFFER_SIZE, 0);
	if (valread < 0)
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
		newClient.CompleteHeader = false;
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
			newClient.start = start;
			newClient.CompleteHeader = true;
		}
		else
			newClient.CompleteHeader = false;

		clients_request.push_back(newClient);
		client_index = static_cast<int>(clients_request.size()) - 1;
	}
	clients_request[client_index].buffer.append(bufferr);
	clients_request[client_index].bytes_read += valread;
	if(clients_request[client_index].CompleteHeader == false)
	{
		size_t start = clients_request[client_index].buffer.find("\r\n\r\n");
		if (start != std::string::npos)
		{
			clients_request[client_index].header = clients_request[client_index].buffer.substr(0, start + 4);
			clients_request[client_index].start = start;
			clients_request[client_index].CompleteHeader = true;
		}
		else
			clients_request[client_index].CompleteHeader = false;
	}
	if(clients_request[client_index].CompleteHeader == true)
	{
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
					std::string body = unchunked_request(clients_request[client_index].buffer, clients_request[client_index].start, clients_request[client_index].bytes_read);
					clients_request[client_index].buffer = clients_request[client_index].header + body;
					FD_CLR(socket_fd, &fd_set_Read);
					FD_SET(socket_fd, &fd_set_write);

					std::string request = clients_request[client_index].buffer;
					clients_request.erase(clients_request.begin() + client_index);
					return request;
				}
			}
			else
			{
				if (clients_request[client_index].bytes_read - clients_request[client_index].start >= clients_request[client_index].contentLength)
				{
					FD_CLR(socket_fd, &fd_set_Read);
					FD_SET(socket_fd, &fd_set_write);
					std::string request = clients_request[client_index].buffer;
					clients_request.erase(clients_request.begin() + client_index);
					return request;
				}
			}
		}
	}
	return "";
}


Wb_Server::~Wb_Server()
{
}

int Wb_Server::getNumber_of_ports()
{
	return this->Number_of_ports;
}
