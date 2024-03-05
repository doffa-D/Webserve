/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/05 18:43:37 by kchaouki         ###   ########.fr       */
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
	try
	{
		listen_to_multiple_clients(parsedData); // listen_to_multiple_clients function is used to listen to multiple clients_request

	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}
void Wb_Server::Setup_Server(int port_index)
{
	socket_fd_server[port_index] = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd_server[port_index] == -1)
	{
		std::cout << "Failed to create socket for port " << HostAndPorts[port_index].second;
		perror("");

	}
	int opt = 1;
	if (setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1 || setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_NOSIGPIPE, &opt, sizeof(opt)) == -1)
	{
		std::cout << "Failed to set socket options for port " << HostAndPorts[port_index].second;
		perror("");
	}

	address[port_index].sin_family = AF_INET;
	address[port_index].sin_port = htons(HostAndPorts[port_index].second);
	address[port_index].sin_addr.s_addr = htonl(HostAndPorts[port_index].first);

	if (bind(socket_fd_server[port_index], (struct sockaddr *)&address[port_index], sizeof(address[port_index])) == -1)
	{
		std::cout << "Failed to bind to port " << HostAndPorts[port_index].second;
		perror("");

	}

	if (listen(socket_fd_server[port_index], 10) == -1)
	{
		std::cout << "Failed to listen on port " << HostAndPorts[port_index].second;
		perror("");

	}
	fcntl(socket_fd_server[port_index], F_SETFL, O_NONBLOCK, FD_CLOEXEC);
}

static void	TrackCookie(const std::string& res, RequestClient& tracker)
{
	string SID;
	size_t pos = res.find("\r\n\r\n");
	if (pos != string::npos)
	{
		std::string header =  res.substr(0, pos);
		// cout << header << endl;	
		std::string to_find = "Set-Cookie: ";
		size_t _pos = header.find(to_find);
		if (_pos != string::npos)
		{
			std::string attribute = header.substr(_pos, header.length());
			attribute = attribute.substr(0, attribute.find("\r\n"));
			attribute = attribute.substr(to_find.length(), attribute.length());
			VecString vec = str_utils::split(attribute, ';');
			for (size_t i = 0; i < vec.size();i++)
			{
				vec[i] = str_utils::trim(vec[i]);
				VecString _vec = str_utils::split(vec[i], '=');
				if (_vec.size() == 2 && _vec[0] == "SID")
				{
					SID = _vec[1];
					break ;
				}
			}
			// cout << "SID: [" << SID << "]" << endl;
			tracker.cookies = SID;
			tracker.session_begin_time = time(0);
		}
	}
}

static void	check_session_expiration(time_t start_time, string fileName)
{
	std::ifstream sessionFile(fileName.c_str());
	if (sessionFile.fail())
		return ;
	string line, _line;
	while (std::getline(sessionFile, line, '\n'))
		if (strnstr(line.c_str(), "expires:", 8) != NULL)
			break ;
	unsigned long _time = str_utils::to_Ulong(line.substr(str_utils::find_first_of(line, " \t"), line.length()));
	time_t current_time = time(0);
	if (current_time - start_time >= (time_t)_time)
		std::remove(fileName.c_str());
}

void Wb_Server::listen_to_multiple_clients(const Parser& parsedData)
{
    std::string httpRequest = "";
    std::string ClientRespont = "";
    fd_set fd_set_Read, Tmp_fd_set_Read;
    fd_set fd_set_write, Tmp_fd_set_write;
    FD_ZERO(&fd_set_Read);
    FD_ZERO(&fd_set_write);
    std::map<int, bool> checker;
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
        if (select(FD_SETSIZE, &Tmp_fd_set_Read, &Tmp_fd_set_write, NULL, &timeout) < 0)
        {
            perror("Error in select");
        }
        for (int SocketID = 3; SocketID < FD_SETSIZE; ++SocketID)
        {
			check_session_expiration(Client[SocketID].session_begin_time, Client[SocketID].cookies);
		 	if(Client[SocketID].keepAlive == true && difftime(time(0) , Client[SocketID].KeepAliveTimeOut) > KEEPALIVE_TIMEOUT)
			{
				close(SocketID);
				FD_CLR(SocketID, &Tmp_fd_set_write);
				FD_CLR(SocketID, &Tmp_fd_set_Read);
				FD_CLR(SocketID, &fd_set_Read);
				FD_CLR(SocketID, &fd_set_write);
				Client.erase(SocketID);
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
                        fcntl(socket_fd_client, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
                        FD_SET(socket_fd_client, &fd_set_Read);
                        checker[socket_fd_client] = true;
                    }
                }
                else
                {
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
						requestClient.cookies = "";
						requestClient.KeepAliveTimeOut = time(0);
                        Client[SocketID] = requestClient;
                    }
                }
            }
            else if (FD_ISSET(SocketID, &Tmp_fd_set_write))
            {
                if (checker[SocketID] == true)
                {
                    Request request;
                    request.Parse_Request(Client[SocketID].request);
                    Response response;
                    response.setReq(request);
                    ClientRespont = response.ft_Response(parsedData);
                    checker[SocketID] = false;
                }
                if (send_full_response(SocketID, ClientRespont) == true)
                {
                    FD_CLR(SocketID, &fd_set_write);
                    checker[SocketID] = true;
					Client[SocketID].KeepAliveTimeOut = time(0);
					TrackCookie(ClientRespont, Client[SocketID]);
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
			int chunkSize = std::stoi(chunkSizeHex, 0, 16);
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
}

int Wb_Server::getNumber_of_ports()
{
	return this->Number_of_ports;
}