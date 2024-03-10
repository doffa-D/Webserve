/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Wb_Server.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:55 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/10 16:36:31 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp" 

#define BUFFER_SIZE 1024
#define KEEPALIVE_TIMEOUT 1
#ifndef SERVER_HP
#define SERVER_HPP


struct RequestClient
{
	int SocketID;
	std::string		request;
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
	std::string buffer;
	long double contentLength;
	size_t start;
	size_t bytes_read;
	bool isChunked;
	size_t startFContent;
	std::vector<std::string> parts;
	std::string header;
	bool CompleteHeader;
};

class Wb_Server
{
	private:
		VecStringString track_cookie;
		fd_set fd_set_Read, Tmp_fd_set_Read;
		fd_set fd_set_write, Tmp_fd_set_write;
		int Number_of_ports;
		std::vector<std::pair<Uint, int> > HostAndPorts;
		int socket_fd_server[FD_SETSIZE];
		int socket_fd_client;
		sockaddr_in address[FD_SETSIZE];
		Wb_Server();
		std::vector<Client> clients_request;
		int bindFailed;

	public:

		Wb_Server(const Parser& parsedData);
		~Wb_Server();
		void Setup_Server(int port_index);
		void listen_to_multiple_clients(const Parser& parsedData);
		bool check_socket(int i);
		std::string read_full_request(int socket_fd, fd_set &fd_set_Read, fd_set &fd_set_write);
		bool send_full_response(std::map<int,RequestClient>& Client, int SocketID);
		int getNumber_of_ports();
};


#endif

