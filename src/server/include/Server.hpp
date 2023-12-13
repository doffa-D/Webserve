/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:55 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/13 17:53:00 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>				   // For socket functions
#include <netinet/in.h>				   // For sockaddr_in
#include <cstdlib>					   // For exit() and EXIT_FAILURE
#include <iostream>					   // For cout
#include <unistd.h>					   // For read
#include <arpa/inet.h>				   // For inet_ntoa
#include <netdb.h>					   // For gethostbyname
#include <fstream>					   // For ifstream
#include "../../../include/header.hpp" // in this header file we have all the libraries that we need
#include <fcntl.h>
#include <vector>
#include <string>

#define PORT 8080			  // the port users will be connecting to
#define BACKLOG 5			  // how many pending connections queue will hold
#define BUFFER_SIZE 1024	  // the size of the buffer that will be used to store data
#define MAX_REQUEST_SIZE 4096 // the size of the buffer that will be used to store data

#ifndef SERVER_HP
#define SERVER_HPP
struct Client
{
	int fd;
	std::string method;
	std::string fisrtboundaryValue;
	std::string lastboundaryValue;
	std::string buffer;
	size_t contentLength;
	size_t start;
	size_t bytes_read;
	bool isChunked;
	size_t startFContent;
	size_t endFile;
	std::vector<std::string> parts;
	bool isFile;
	std::string header;
};

class Server : public parseHttpRequest
{
private:
	int Ports[FD_SETSIZE];
	int socket_fd_server[FD_SETSIZE]; // socket_fd_server is the file descriptor of the server socket
	int socket_fd_client;			  // socket_fd_client is the file descriptor of the client socket
	int bytes_read;					  // bytes_read is the number of bytes read from the client
	int Number_of_ports;
	sockaddr_in address[FD_SETSIZE];			   // address is the address of the server socket
	std::string ip_address;						   // ip_address is the ip address of the server
	std::string sBuffer;						   // sBuffer is the buffer that will be used to store data as a string
	std::string HtmlFile;						   // HtmlFile is the html file that will be sent to the client
	std::map<std::string, std::string> MIME_types; // MIME_types is the map that will be used to store the mime types
	Server();
	char *argv;
	std::vector<Client> clients;

public:
	Server(char *argv);
	~Server();
	void Setup_Server(int port_index);						   // Setup_Server function is used to setup the server
	void getMyIpAddress();									   // getMyIpAddress function is used to get the ip address of the server
	void Accept_Connection(int port_index);					   // Accept_Connection function is used to accept a connection request from a client
	void Read_Request_From_Client(int client, int port_index); // Read_From_Client function is used to read data from a client
	void Send_Response_To_Client();							   // Send_Response_To_Client function is used to send a response to the client
	void ReadHtmlFile();									   // ReadHtmlFile function is used to read the html file
	std::string GetMIMEType(std::string key);				   // GetMimeType function is used to get the mime type of the html file
	void MIME_types_init();									   // MIME_types_init function is used to initialize the map with the mime types
	void Send_Error_Response(int fd_client);				   // Send_Error_Response function is used to send an error response to the client
	void get_matched_location_for_request_uri();			   // Send_Correct_Response function is used to send a correct response to the client
	void listen_to_multiple_clients();						   // listen_to_multiple_clients function is used to listen to multiple clients
	void Check_file_existence(std::string path);			   // Check_file_existence function is used to check if the file exists
	void Send_Correct_Response();							   // Send_Correct_Response function is used to send a correct response to the client
	void set_ports(int ports, int index);					   // set_ports function is used to set the ports
	void set_Number_of_ports(int Number_of_ports);
	void get_server_by_hostname(std::string host, int fd_clien);
	void find_file();
	bool check_socket(int i);
	// bool read_full_request(int socket_fd, fd_set &current_sockets);
	std::string read_full_request(int socket_fd, fd_set &current_sockets);
	void get_method(std::string request);
};

void init_server(Server &server, char *argv);

#endif