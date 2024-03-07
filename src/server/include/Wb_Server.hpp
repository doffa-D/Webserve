/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:55 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/07 16:24:23 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp" 

#define PORT 8080		 // the port users will be connecting to
#define BACKLOG 5		 // how many pending connections queue will hold
#define BUFFER_SIZE 1024 // the size of the buffer that will be used to store data
#define KEEPALIVE_TIMEOUT 0.1
#ifndef SERVER_HP
#define SERVER_HPP


struct RequestClient
{
	int SocketID;
	std::string		request;
	VecStringString	cookie_tracker;
	time_t			KeepAliveTimeOut;
	bool			keepAlive;
	std::string		CheckSeend;
	std::string		ClientRespont;
	std::string		Host;
	ssize_t     byteSent;
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
    fd_set fd_set_Read, Tmp_fd_set_Read;
    fd_set fd_set_write, Tmp_fd_set_write;
	int Number_of_ports;
	std::vector<std::pair<Uint, int> > HostAndPorts;
	int socket_fd_server[FD_SETSIZE]; // socket_fd_server is the file descriptor of the server socket
	int socket_fd_client;			  // socket_fd_client is the file descriptor of the client socket
	int bytes_read;					  // bytes_read is the number of bytes read from the client
	sockaddr_in address[FD_SETSIZE];			   // address is the address of the server socket
	std::string sBuffer;						   // sBuffer is the buffer that will be used to store data as a string
	std::string HtmlFile;						   // HtmlFile is the html file that will be sent to the client
	Wb_Server();
	char *argv;
	std::vector<Client> clients_request;
	int bindFailed;

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
	bool send_full_response(std::map<int,RequestClient>& Client, int SocketID);
	void get_method(std::string request);
	int find_clinet_response(std::vector<SendTracker> &clients_respont, int socket_fd);
	int getNumber_of_ports();
};

// void init_server(Wb_Server &server, char *argv);

#endif

