/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:36:55 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/02 11:53:03 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <arpa/inet.h> // For inet_ntoa
#include <netdb.h> // For gethostbyname

#define PORT 9999 // the port users will be connecting to
#define BACKLOG 10 // how many pending connections queue will hold
#define BUFFER_SIZE 1024 // the size of the buffer that will be used to store data

#ifndef SERVER_HPP
#define SERVER_HPP

class Server
{
    private:
        int socket_fd_server; // socket_fd_server is the file descriptor of the server socket
        sockaddr_in address; // address is the address of the server socket
        int socket_fd_client; // socket_fd_client is the file descriptor of the client socket
        char buffer[BUFFER_SIZE]; // buffer is the buffer that will be used to store data
        int bytes_read; // bytes_read is the number of bytes read from the client
        int message_size; // message_size is the size of the message that will be sent to the client
       std::string ip_address; // ip_address is the ip address of the server        
    public:
        void Setup_Server();
        void Accept_Connection();
        void Read_From_Client();
        void getMyIpAddress();
        Server();
        ~Server();
};

#endif