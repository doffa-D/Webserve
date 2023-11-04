/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:55 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/04 16:16:01 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <arpa/inet.h>  // For inet_ntoa
#include <netdb.h>      // For gethostbyname
#include "parseHttpRequest.hpp" // For parseHttpRequest class
#include <fstream>      // For ifstream

#define PORT 8080        // the port users will be connecting to
#define BACKLOG 10       // how many pending connections queue will hold
#define BUFFER_SIZE 1024 // the size of the buffer that will be used to store data

#ifndef SERVER_HPP
#define SERVER_HPP

class Server : public parseHttpRequest
{
	private:
		int socket_fd_server;     // socket_fd_server is the file descriptor of the server socket
		sockaddr_in address;      // address is the address of the server socket
		int socket_fd_client;     // socket_fd_client is the file descriptor of the client socket
		int bytes_read;           // bytes_read is the number of bytes read from the client
		std::string ip_address;   // ip_address is the ip address of the server
		std::string sBuffer;      // sBuffer is the buffer that will be used to store data as a string
		std::string HtmlFile;     // HtmlFile is the html file that will be sent to the client

	public:
		Server();
		~Server();
		void Setup_Server();             // Setup_Server function is used to setup the server
		void getMyIpAddress();           // getMyIpAddress function is used to get the ip address of the server
		void Accept_Connection();        // Accept_Connection function is used to accept a connection request from a client
		void Read_Request_From_Client(); // Read_From_Client function is used to read data from a client
		void Send_Response_To_Client();  // Send_Response_To_Client function is used to send a response to the client
		void ReadHtmlFile();             // ReadHtmlFile function is used to read the html file
		std::string GetMIMEType(std::string key);              // GetMimeType function is used to get the mime type of the html file
};

#endif