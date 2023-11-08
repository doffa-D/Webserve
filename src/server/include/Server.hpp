/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:55 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/08 09:27:16 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>			// For socket functions
#include <netinet/in.h>			// For sockaddr_in
#include <cstdlib>				// For exit() and EXIT_FAILURE
#include <iostream>				// For cout
#include <unistd.h>				// For read
#include <arpa/inet.h>			// For inet_ntoa
#include <netdb.h>				// For gethostbyname
#include <fstream>				// For ifstream
#include "../../../include/header.hpp"



#define PORT 8080		 // the port users will be connecting to
#define BACKLOG 10		 // how many pending connections queue will hold
#define BUFFER_SIZE 1024 // the size of the buffer that will be used to store data

#ifndef SERVER_HP
#define SERVER_HPP

class Server : public parseHttpRequest
{
private:
	int socket_fd_server;						   // socket_fd_server is the file descriptor of the server socket
	sockaddr_in address;						   // address is the address of the server socket
	int socket_fd_client;						   // socket_fd_client is the file descriptor of the client socket
	int bytes_read;								   // bytes_read is the number of bytes read from the client
	std::string ip_address;						   // ip_address is the ip address of the server
	std::string sBuffer;						   // sBuffer is the buffer that will be used to store data as a string
	std::string HtmlFile;						   // HtmlFile is the html file that will be sent to the client
	std::map<std::string, std::string> MIME_types; // MIME_types is the map that will be used to store the mime types
	int *port;

public:
	Server();
	~Server();
	void Setup_Server();						 // Setup_Server function is used to setup the server
	void getMyIpAddress();						 // getMyIpAddress function is used to get the ip address of the server
	void Accept_Connection();					 // Accept_Connection function is used to accept a connection request from a client
	void Read_Request_From_Client();			 // Read_From_Client function is used to read data from a client
	void Send_Response_To_Client();				 // Send_Response_To_Client function is used to send a response to the client
	void ReadHtmlFile();						 // ReadHtmlFile function is used to read the html file
	std::string GetMIMEType(std::string key);	 // GetMimeType function is used to get the mime type of the html file
	void MIME_types_init();						 // MIME_types_init function is used to initialize the map with the mime types
	void Send_Error_Response();					 // Send_Error_Response function is used to send an error response to the client
	void get_matched_location_for_request_uri(); // Send_Correct_Response function is used to send a correct response to the client
	void listen_to_multiple_clients();			 // listen_to_multiple_clients function is used to listen to multiple clients
	void Check_file_existence();				 // Check_file_existence function is used to check if the file exists
	void Send_Correct_Response();				 // Send_Correct_Response function is used to send a correct response to the client
};

void init_server(Server &server);

#endif