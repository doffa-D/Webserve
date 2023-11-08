/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/08 11:42:58 by hdagdagu         ###   ########.fr       */
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
	init_server(*this, argv);
	MIME_types_init(); // MIME_types_init function is used to initialize the map with the mime types
	getMyIpAddress();  // getMyIpAddress function is used to get the ip address of the server
	for (int i = 0; i < Number_of_ports; i++)
		Setup_Server(i); // Setup_Server function is used to setup the server	
	listen_to_multiple_clients();		// listen_to_multiple_clients function is used to listen to multiple clients
}

void Server::listen_to_multiple_clients()
{
	fd_set current_sockets;
	FD_ZERO(&current_sockets);


	while (true)
	{
		for (int i = 0; i < Number_of_ports; i++)
		{
			FD_SET(socket_fd_server[i], &current_sockets);
		}
		int max_fd = -1;
        for (int i = 0; i < Number_of_ports; i++) {
            if (socket_fd_server[i] > max_fd) {
                max_fd = socket_fd_server[i];
            }
        }

        if (select(max_fd + 1, &current_sockets, NULL, NULL, NULL) < 0) {
            perror("Error in select");
            exit(1);
        }


        // Check which socket has incoming data
        for (int i = 0; i < Number_of_ports; i++) 
		{
            if (FD_ISSET(socket_fd_server[i], &current_sockets)) 
			{
                struct sockaddr_in client_addr;
                socklen_t client_len = sizeof(client_addr);
                int client_sock = accept(socket_fd_server[i], (struct sockaddr *)&client_addr, &client_len);
                if (client_sock < 0) {
                    perror("Error accepting connection");
                    continue;
                }

                // Read and print the request from the client
                char request[MAX_REQUEST_SIZE];
                ssize_t bytes_received = recv(client_sock, request, sizeof(request), 0);
                if (bytes_received < 0) {
                    perror("Error reading from socket");
                } else if (bytes_received == 0) {
                    printf("Client disconnected\n");
                } else {
                    request[bytes_received] = '\0';
                    printf("Received request from client on port %d:\n%s\n", Ports[i], request);
                }

                // Close the client socket
                close(client_sock);
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

	// initialize address
	address.sin_family = AF_INET;
	address.sin_port = htons(Ports[port_index]);
	address.sin_addr.s_addr = INADDR_ANY;

	// std::cout << "IP Address: http://" << ip_address << ":" << Ports[port_index] << std::endl;
	// std::cout << "	  : http://localhost:" << Ports[port_index] << std::endl;

	int opt = 1;
	if (setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		std::cout << "Failed to set socket options for port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}

	if (bind(socket_fd_server[port_index], (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		std::cout << "Failed to bind to port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}

	if (listen(socket_fd_server[port_index], BACKLOG) == -1)
	{
		std::cout << "Failed to listen on port " << Ports[port_index] << std::endl;
		exit(EXIT_FAILURE);
	}

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


void Server::Read_Request_From_Client(int client,int port_index)
{
    char buffer[BUFFER_SIZE];
    while (true)
    {
        bzero(buffer, BUFFER_SIZE);
        bytes_read = read(client, buffer, BUFFER_SIZE - 1);
        if (bytes_read < 0)
        {
            std::cout << "Failed to read from socket on port " << Ports[port_index] << std::endl;
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
    // try
    // {
    //     ReadHtmlFile();
    //     parseRequest(sBuffer);
    //     // print_all_parseRequest();
    //     get_matched_location_for_request_uri();
    // }
    // catch (const MY_exception::status_code_exception &e)
    // {
    //     set_status(e.what());
    //     Send_Error_Response();
    // }
    sBuffer.clear();
    // HtmlFile.clear();
    // clear_all();  // Assuming this is a member function, it should be handled elsewhere in the class
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

// void Server::ReadHtmlFile()
// {
// 	std::ifstream file("Test_page/index.html");
// 	std::string str;
// 	while (std::getline(file, str))
// 	{
// 		HtmlFile += str;
// 	}
// 	// std::cout << HtmlFile << std::endl;
// 	file.close();
// }

// std::string Server::GetMIMEType(std::string key)
// {
// 	std::map<std::string, std::string>::iterator it = MIME_types.find(key);
// 	if (it != MIME_types.end())
// 	{
// 		return it->second;
// 	}
// 	return "";
// }

// void Server::Send_Error_Response()
// {
// 	std::string message = get_Version() + " " + get_status() + "\r\nContent-Type: text/html\r\n\r\n"
// 															   "<html>"
// 															   "<head>"
// 															   "<style>"
// 															   "h1 {"
// 															   "    text-align: center;"
// 															   "}"
// 															   "</style>"
// 															   "</head>"
// 															   "<body>"
// 															   "<h1>" +
// 						  get_status() + "</h1>"
// 										 "</body>"
// 										 "</html>";
// 	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
// 	{
// 		puts("Send failed");
// 		return;
// 	}
// }
// // void Server::Check_file_existence()
// // {
// // 	std::string filename = getRoot() + "/" + getIndex();
// // 	std::fstream file;
// // 	file.open(filename.c_str(), std::ios::in); // Open file in read mode

// // 	if (file.is_open())
// // 	{
// // 		file.close();

// // 	}
// // 	else
// // 	{
// // 		std::cout << "test" << std::endl;

// // 		MY_exception e("404 Not Found");
// // 		throw e;
// // 	}
// // }
// void Server::Send_Correct_Response()
// {
// 	size_t pos = get_URI().find('.');
// 	std::string ContentType;
// 	if (pos != std::string::npos)
// 	{
// 		std::string ext = get_URI().substr(pos);
// 		ContentType = GetMIMEType(ext);
// 	}
// 	std::string message = "HTTP/1.1 200 OK\r\nContent-Type: " + ContentType + "\r\n\r\n" + HtmlFile;
// 	// std::cout << message << std::endl;
// 	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
// 	{
// 		std::cout << "Send failed" << std::endl;
// 		return;
// 	}
// }
// void Server::get_matched_location_for_request_uri()
// {
// 	try
// 	{
// 		// Check_file_existence();
// 		Send_Correct_Response();
// 	}
// 	catch (MY_exception &e)
// 	{
// 		set_status(e.what());
// 		Send_Error_Response();
// 	}
// }
