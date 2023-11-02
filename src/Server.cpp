/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/02 15:04:31 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server()
{
    getMyIpAddress();
    Setup_Server();
    fd_set current_sockets, ready_sockets; // fd_set is a set of file descriptors. It is actually a bit array. Each bit corresponds to a file descriptor. If the bit is set, it means that the corresponding file descriptor is set. The FD_ZERO macro clears the set. The FD_SET macro adds a file descriptor to the set. The FD_CLR macro removes a file descriptor from the set. The FD_ISSET macro checks to see if a file descriptor is part of the set.

    FD_ZERO(&current_sockets); // FD_ZERO macro clears the set
    FD_SET(socket_fd_server, &current_sockets); // FD_SET macro adds a file descriptor to the set
    for(;true;)
    {
        ready_sockets = current_sockets; // ready_sockets is the set of file descriptors that are ready to be read from
        if(select(FD_SETSIZE,&ready_sockets,NULL,NULL,NULL) == -1) // select function is used to monitor file descriptors. It takes five arguments: The first argument is the highest-numbered file descriptor in any of the three sets, plus 1. The second argument is the set of file descriptors that you're interested in reading. The third argument is the set of file descriptors that you're interested in writing. The fourth argument is the set of file descriptors that you're interested in excepting. The fifth argument is a timeout. If the timeout is NULL, select will block until at least one file descriptor is ready. If the timeout is 0, select will return immediately. Otherwise, it will block until either the timeout expires or a file descriptor becomes ready. The select function returns the number of file descriptors that are ready, or -1 on error.
        {
            std::cout << "Failed to select. " << std::endl;
            exit(EXIT_FAILURE);
        }
        for(int i = 0; i < FD_SETSIZE;i++)
        {
            if(FD_ISSET(i,&ready_sockets)) // FD_ISSET macro checks to see if a file descriptor is part of the set
            {
                if(i == socket_fd_server) // if the file descriptor is the server socket, then we have a new connection
                {
                    Accept_Connection(); // accept function is used to accept a connection request from a client. It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
                    FD_SET(socket_fd_client,&current_sockets); // FD_SET macro adds a file descriptor to the set
                }
                else
                {
                    Read_From_Client(); // read function is used to read data from a socket
                    close(socket_fd_client); // close function is used to close a socket. It takes a single argument, which is the file descriptor of the socket to be closed.
                    FD_CLR(socket_fd_client,&current_sockets); // FD_CLR macro removes a file descriptor from the set
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
    if(socket_fd_server == -1)
    {
        std::cout << "Failed to create socket. " << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Creat socket. " << std::endl;
    }

    // initialize address
    address.sin_family = AF_INET; // what is sin_family? sin_family contains a code for the address family. It should always be set to AF_INET.
    address.sin_port = htons(PORT); // what is sin_port? sin_port contains the port number. However, instead of simply copying the port number to this field, it is necessary to convert this to network byte order using the function htons() which converts a port number in host byte order to a port number in network byte order.    
    std::cout << "IP Address: http://" << ip_address << ":" << PORT << std::endl;
    if(bind(socket_fd_server, (struct sockaddr*)&address, sizeof(address)) == -1) // why we need bind function to bind socket? The bind function is used to bind the socket to a particular "address and port combination". In this case, we are binding to
    {
        std::cout << "Faild to bind to port 8080. " << std::endl;
        exit(EXIT_FAILURE);
    }
    if(listen(socket_fd_server,BACKLOG) == -1) // why we need listen function to listen socket? The listen function is used on the server side. It tells the socket library that we want to listen in on the server side for incoming connections. The second argument is called the backlog, and it tells the socket library that how many incoming connections can be queued up before the system starts to reject incoming connections.
    {
        std::cout << "Faild to listen. " << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Listen. " << std::endl;
    }
}

void Server::Accept_Connection()
{
    socklen_t address_lenght = sizeof(address);    // socklen_t is a type of data type in socket.h header file which is used to represent the size of objects in bytes. It is used to represent the sizes of socket address structures. It is used in accept(), bind(), connect(), getsockname(), getpeername(), and socket() functions.
    if((socket_fd_client = accept(socket_fd_server, (struct sockaddr*)&address, &address_lenght)) == -1) // accept function is used to accept a connection request from a client. It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
    {
        std::cout << "Failed to grab connection. " << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        std::cout << "Grab connection. " << std::endl;
    }
}

void Server::Read_From_Client()
{
    while((bytes_read = read(socket_fd_client,buffer+message_size,sizeof(buffer)-message_size-1))) // read function is used to read data from a socket. It takes the following three arguments: The first argument is the socket file descriptor, and the second is the buffer in which to store the data, and the third is the maximum number of bytes to read. The read function returns the number of bytes actually read, and -1 on error.
    {
        if(bytes_read == -1)
        {
            std::cout << "Failed to read from socket. " << std::endl;
            exit(EXIT_FAILURE);
        }
        else
        {
            std::cout << "Read from socket. " << std::endl;
        }
        message_size += bytes_read; // message_size is the size of the message that will be sent to the client
        if(message_size > BUFFER_SIZE -1 || buffer[message_size-1] == '\n')
        {
            break;
        }
    }
    buffer[message_size-1] = '\0';
    std::cout << "Client said: \n" << buffer << std::endl;
    std::string message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>Hello, World!</h1>";
    if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0) {
        puts("Send failed");
        return ;
    }
    std::cout << "==============================" << std::endl;
}

void Server::getMyIpAddress() {
    char buffer[1024];
    ip_address = "";

    struct hostent* host; // Structure containing host information
    if (gethostname(buffer, sizeof(buffer)) != -1) {
        host = gethostbyname(buffer); // Get the IP address
        if (host != NULL) {
            ip_address = inet_ntoa(*((struct in_addr *)host->h_addr_list[0])); // Convert IP to string
            address.sin_addr.s_addr = inet_addr(ip_address.c_str()); // Use c_str() to get a const char* from the string
        }
    }
    return ;
}

