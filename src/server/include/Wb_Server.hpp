/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:55 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/05 18:02:16 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp" // in this header file we have all the libraries that we need
// #include "../../Request_Response/Request.hpp"
// #include "../../Request_Response/Response.hpp"

#define PORT 8080		 // the port users will be connecting to
#define BACKLOG 5		 // how many pending connections queue will hold
#define BUFFER_SIZE 1024 // the size of the buffer that will be used to store data
#define KEEPALIVE_TIMEOUT 1 
#ifndef SERVER_HP
#define SERVER_HPP


struct RequestClient
{
	std::string request;
	std::string cookies;
	time_t KeepAliveTimeOut;
	bool keepAlive;
};

struct Client
{
	int fd;
	std::string method;
	std::string fisrtboundaryValue;
	std::string lastboundaryValue;
	std::string buffer;
	long double contentLength;
	size_t start;
	size_t bytes_read;
	bool isChunked;
	size_t startFContent;
	size_t endFile;
	std::vector<std::string> parts;
	bool isFile;
	std::string header;
	bool first_respont;
};

class Wb_Server
{
private:
	int Number_of_ports;
	std::vector<std::pair<Uint, int> > HostAndPorts;
	int socket_fd_server[FD_SETSIZE]; // socket_fd_server is the file descriptor of the server socket
	int socket_fd_client;			  // socket_fd_client is the file descriptor of the client socket
	int bytes_read;					  // bytes_read is the number of bytes read from the client
	sockaddr_in address[FD_SETSIZE];			   // address is the address of the server socket
	std::string sBuffer;						   // sBuffer is the buffer that will be used to store data as a string
	std::string HtmlFile;						   // HtmlFile is the html file that will be sent to the client
	// std::map<std::string, std::string> MIME_types; // MIME_types is the map that will be used to store the mime types
	Wb_Server();
	char *argv;
	std::vector<Client> clients_request;
	std::vector<SendTracker> clients_respont;

public:
	Wb_Server(const Parser& parsedData);
	~Wb_Server();
	void Setup_Server(int port_index);						   // Setup_Server function is used to setup the server
	void Accept_Connection(int port_index);					   // Accept_Connection function is used to accept a connection request from a client
	void Read_Request_From_Client(int client, int port_index); // Read_From_Client function is used to read data from a client
	void Send_Response_To_Client();							   // Send_Response_To_Client function is used to send a response to the client
	void ReadHtmlFile();									   // ReadHtmlFile function is used to read the html file
	std::string GetMIMEType(std::string key);				   // GetMimeType function is used to get the mime type of the html file
	void MIME_types_init();									   // MIME_types_init function is used to initialize the map with the mime types
	void Send_Error_Response(int fd_client);				   // Send_Error_Response function is used to send an error response to the client
	void get_matched_location_for_request_uri();			   // Send_Correct_Response function is used to send a correct response to the client
	void listen_to_multiple_clients(const Parser& parsedData);						   // listen_to_multiple_clients function is used to listen to multiple clients
	void Check_file_existence(std::string path);			   // Check_file_existence function is used to check if the file exists
	void Send_Correct_Response();							   // Send_Correct_Response function is used to send a correct response to the client
	void get_server_by_hostname(std::string host, int fd_clien);
	void find_file();
	bool check_socket(int i);
	std::string read_full_request(int socket_fd, fd_set &fd_set_Read, fd_set &fd_set_write);
	int find_clinet(int socket_fd);
	void create_client(std::string body, int socket_fd);
	void handle_chunked_data(int client_index);
	void handle_non_chunked_data();
	bool send_full_response(int socket_fd, std::string respont);
	void get_method(std::string request);
	int find_clinet_response(std::vector<SendTracker> &clients_respont, int socket_fd);
	int getNumber_of_ports();
};

// void init_server(Wb_Server &server, char *argv);

#endif

