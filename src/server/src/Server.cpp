/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/08 09:26:17 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server()
{
	init_server(*this);
	
	MIME_types_init();					// MIME_types_init function is used to initialize the map with the mime types
	getMyIpAddress();					// getMyIpAddress function is used to get the ip address of the server
	Setup_Server();						// Setup_Server function is used to setup the server
	listen_to_multiple_clients();		// listen_to_multiple_clients function is used to listen to multiple clients
}

void Server::listen_to_multiple_clients()
{
	fd_set current_sockets, ready_sockets; // fd_set is a set of file descriptors. It is actually a bit array. Each bit corresponds to a file descriptor. If the bit is set, it means that the corresponding file descriptor is set. The FD_ZERO macro clears the set. The FD_SET macro adds a file descriptor to the set. The FD_CLR macro removes a file descriptor from the set. The FD_ISSET macro checks to see if a file descriptor is part of the set.

	FD_ZERO(&current_sockets);					// FD_ZERO macro clears the set
	FD_SET(socket_fd_server, &current_sockets); // FD_SET macro adds a file descriptor to the set
	for (; true;)
	{
		ready_sockets = current_sockets;								// ready_sockets is the set of file descriptors that are ready to be read from
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) == -1) // select function is used to monitor file descriptors. It takes five arguments: The first argument is the highest-numbered file descriptor in any of the three sets, plus 1. The second argument is the set of file descriptors that you're interested in reading. The third argument is the set of file descriptors that you're interested in writing. The fourth argument is the set of file descriptors that you're interested in excepting. The fifth argument is a timeout. If the timeout is NULL, select will block until at least one file descriptor is ready. If the timeout is 0, select will return immediately. Otherwise, it will block until either the timeout expires or a file descriptor becomes ready. The select function returns the number of file descriptors that are ready, or -1 on error.
		{
			std::cout << "Failed to select. " << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets)) // FD_ISSET macro checks to see if a file descriptor is part of the set
			{
				if (i == socket_fd_server) // if the file descriptor is the server socket, then we have a new connection
				{
					Accept_Connection();						// accept function is used to accept a connection request from a client. It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
					FD_SET(socket_fd_client, &current_sockets); // FD_SET macro adds a file descriptor to the set
				}
				else
				{
					socket_fd_client = i;
					Read_Request_From_Client();
					FD_CLR(socket_fd_client, &current_sockets); // FD_CLR macro removes a file descriptor from the set
					close(socket_fd_client);					// close function is used to close a socket. It takes a single argument, which is the file descriptor of the socket to be closed.
				}
			}
		}
	}
}

Server::~Server()
{
	// std::cout << "Server destructor called. " << std::endl;
}

void Server::Setup_Server()
{
	socket_fd_server = socket(AF_INET, SOCK_STREAM, 0);
	// what is socket? A socket is one endpoint of a two-way communication link between two programs running on the network. A socket is bound to a port number so that the TCP layer can identify the application that data is destined to be sent to.
	// AF_INET is the address family that is used for the socket you're creating (in this case an Internet Protocol address).
	// SOCK_STREAM is the type of socket you're creating (TCP, for example).
	// The third argument is the protocol. If this argument is zero (and it always should be except for unusual circumstances), the operating system will choose the most appropriate protocol. It will choose TCP for stream sockets and UDP for datagram sockets.
	if (socket_fd_server == -1)
	{
		std::cout << "Failed to create socket. " << std::endl;
		exit(EXIT_FAILURE);
	}

	// initialize address
	address.sin_family = AF_INET;	// what is sin_family? sin_family contains a code for the address family. It should always be set to AF_INET.
	address.sin_port = htons(PORT); // what is sin_port? sin_port contains the port number. However, instead of simply copying the port number to this field, it is necessary to convert this to network byte order using the function htons() which converts a port number in host byte order to a port number in network byte order.
	address.sin_addr.s_addr = INADDR_ANY;
	std::cout << "IP Address: http://" << ip_address << ":" << PORT << std::endl;
	std::cout << "	  : http://localhost:" << PORT << std::endl;
	int opt = 1;
	setsockopt(socket_fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));		// why we need setsockopt function to set socket options? The setsockopt function is used to allow the local address to be reused when the server is restarted before the required wait time expires. The setsockopt function takes four arguments: The first argument is the socket descriptor. The second argument is the level at which the option resides. The third argument is the name of the option. The fourth argument is a pointer to the buffer containing the value for the option. The fifth argument is the size of the buffer containing the value for the option.
	if (bind(socket_fd_server, (struct sockaddr *)&address, sizeof(address)) == -1) // why we need bind function to bind socket? The bind function is used to bind the socket to a particular "address and port combination". In this case, we are binding to
	{
		std::cout << "Faild to bind to port " << PORT << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(socket_fd_server, BACKLOG) == -1) // why we need listen function to listen socket? The listen function is used on the server side. It tells the socket library that we want to listen in on the server side for incoming connections. The second argument is called the backlog, and it tells the socket library that how many incoming connections can be queued up before the system starts to reject incoming connections.
	{
		std::cout << "Faild to listen. " << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::Accept_Connection()
{
	socklen_t address_lenght = sizeof(address);															   // socklen_t is a type of data type in socket.h header file which is used to represent the size of objects in bytes. It is used to represent the sizes of socket address structures. It is used in accept(), bind(), connect(), getsockname(), getpeername(), and socket() functions.
	if ((socket_fd_client = accept(socket_fd_server, (struct sockaddr *)&address, &address_lenght)) == -1) // accept function is used to accept a connection request from a client. It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
	{
		std::cout << "Failed to grab connection. " << std::endl;
		exit(EXIT_FAILURE);
	}
}

void Server::Read_Request_From_Client()
{
	char buffer[BUFFER_SIZE];
	while (1)
	{
		bzero(buffer, BUFFER_SIZE);
		bytes_read = read(socket_fd_client, buffer, BUFFER_SIZE - 1);
		if (bytes_read < 0)
		{
			std::cout << "Failed to read from socket. " << std::endl;
			exit(EXIT_FAILURE);
		}
		sBuffer.append(buffer);
		if (bytes_read < BUFFER_SIZE - 1)
		{
			buffer[bytes_read] = '\0';
			break;
		}
	}
	std::cout << "Request: " << std::endl
			  << sBuffer << std::endl;
	try
	{
		ReadHtmlFile();
		parseRequest(sBuffer);
		// print_all_parseRequest();
		get_matched_location_for_request_uri();
	}
	catch (const MY_exception::status_code_exception &e)
	{
		set_status(e.what());
		Send_Error_Response();
	}
	sBuffer.clear();
	HtmlFile.clear();
	clear_all();
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
			// address.sin_addr.s_addr = inet_addr(ip_address.c_str());		   // Use c_str() to get a const char* from the string
		}
	}
	return;
}

void Server::ReadHtmlFile()
{
	std::ifstream file("Test_page/index.html");
	std::string str;
	while (std::getline(file, str))
	{
		HtmlFile += str;
	}
	// std::cout << HtmlFile << std::endl;
	file.close();
}

std::string Server::GetMIMEType(std::string key)
{
	std::map<std::string, std::string>::iterator it = MIME_types.find(key);
	if (it != MIME_types.end())
	{
		return it->second;
	}
	return "";
}

void Server::Send_Error_Response()
{
	std::string message = get_Version() + " " + get_status() + "\r\nContent-Type: text/html\r\n\r\n"
															   "<html>"
															   "<head>"
															   "<style>"
															   "h1 {"
															   "    text-align: center;"
															   "}"
															   "</style>"
															   "</head>"
															   "<body>"
															   "<h1>" +
						  get_status() + "</h1>"
										 "</body>"
										 "</html>";
	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
	{
		puts("Send failed");
		return;
	}
}
void Server::Check_file_existence()
{
	std::string filename = getRoot() + "/" + getIndex();
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
void Server::Send_Correct_Response()
{
	size_t pos = get_URI().find('.');
	std::string ContentType;
	if (pos != std::string::npos)
	{
		std::string ext = get_URI().substr(pos);
		ContentType = GetMIMEType(ext);
	}
	std::string message = "HTTP/1.1 200 OK\r\nContent-Type: " + ContentType + "\r\n\r\n" + HtmlFile;
	// std::cout << message << std::endl;
	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
	{
		std::cout << "Send failed" << std::endl;
		return;
	}
}
void Server::get_matched_location_for_request_uri()
{
	try
	{
		Check_file_existence();
		Send_Correct_Response();
	}
	catch (MY_exception &e)
	{
		set_status(e.what());
		Send_Error_Response();
	}
}
