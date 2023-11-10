/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/10 15:35:32 by hdagdagu         ###   ########.fr       */
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



void Server::get_server_by_hostname(std::string host,int fd_clien)
{
    try
    {
        Parser parser(argv);
        ServerData servers(parser.getServers());
        ServerModel smodel = servers.getServerByServerName(host.c_str());
		std::vector<Location> location =	smodel.getLocation();
        std::vector<Location>::iterator ibegin = location.begin();
        std::vector<Location>::iterator iend = location.end();
        int i = 0;
        for(;ibegin != iend; ibegin++)
        {
            std::cout << "=====================>> " << std::endl;
            std::cout << "folder in config :" << ibegin->getPath() << std::endl;
            std::cout << "folder in request :" << get_folder() << std::endl;
            std::cout << "lenght of folder in config :" << ibegin->getPath().length() << std::endl;
            std::cout << "lenght of folder in request :" << get_folder().length() << std::endl;
            std::cout << "=====================>> " << std::endl;
            if( ibegin->getPath().length() >= 1 && (ibegin->getPath() + "/") == get_folder())
            {
                i++;
                std::cout << "i found the path" << std::endl;
                break;
            }
            else if(ibegin->getPath().length() == 1 && ibegin->getPath() == get_folder())
            {
                i++;
                std::cout << "i found the path" << std::endl;
                break;
            }
        }
        if(i == 0)
            std::cout << "i didn't found the path" << std::endl;
        // std::cout << "how mush path i have :" << ibegin - location.begin() << " i :" << i<< std::endl;
        // std::cout << "folder in request :" << get_folder() << std::endl;
        // std::cout << "file :" << get_file() << std::endl; 
        // Check_file_existence(smodel.getRoot());
    }
    catch(const std::exception& e)
    {
        set_status("404 Not Found");
        Send_Error_Response(fd_clien);
        // std::cerr << e.what() << '\n';
    }
    

    // std::vector<Data> data = smodel.getData("listen");

}


Server::Server(char *argv)
{
    this->argv = argv;
    init_server(*this, argv);
    MIME_types_init();
    getMyIpAddress();
    for (int i = 0; i < Number_of_ports; i++)
        Setup_Server(i);          // Setup_Server function is used to setup the server
    listen_to_multiple_clients(); // listen_to_multiple_clients function is used to listen to multiple clients
}

bool Server::check_socket(int i)
{
    for(int j = 0; j < FD_SETSIZE; j++)
    {
        if(socket_fd_server[j] == i)
            return true;
    }
    return false;
}

void Server::listen_to_multiple_clients()
{
    int id = 1;
    fd_set current_sockets, ready_sockets;
    FD_ZERO(&current_sockets);
    
    for (int i = 0; i < Number_of_ports; i++)
    {
        FD_SET(socket_fd_server[i], &current_sockets);
    }
    char buffer[BUFFER_SIZE] = {0};
    while (true)
    {
        ready_sockets = current_sockets;
        if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0)
        {
            perror("Error in select");
            exit(1);
        }


        for(int i = 0 ; i < FD_SETSIZE;++i)
        {
            if(FD_ISSET(i,&ready_sockets))
            {
                if(check_socket(i) == true)
                {
                    socklen_t addrlen = sizeof(address[0]);
                    if ((socket_fd_client = accept(i, (struct sockaddr *)&address[0], (socklen_t *)&addrlen)) < 0) {
                        perror("accept");
                        exit(EXIT_FAILURE);
                        
                    } 
                    else
                    {
                        fcntl(socket_fd_client, F_SETFL, O_NONBLOCK);
                        FD_SET(socket_fd_client, &current_sockets);
                    }
                }
                else
                {
                    int valread = recv(i, buffer, BUFFER_SIZE,0);
                    if (valread > 0) {
                        printf("Received data: %s id=>[%d]\n", buffer,id++);
                        send(i, buffer, valread, 0);
                    } else {
                        close(i);
                        FD_CLR(i, &current_sockets);
                    }
                }
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

    int opt = 1;
    if (setsockopt(socket_fd_server[port_index], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        std::cout << "Failed to set socket options for port " << Ports[port_index] << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // initialize address
    address[port_index].sin_family = AF_INET;
    address[port_index].sin_port = htons(Ports[port_index]);
    address[port_index].sin_addr.s_addr = INADDR_ANY;

    if (bind(socket_fd_server[port_index], (struct sockaddr *)&address[port_index], sizeof(address[port_index])) == -1)
    {
        std::cout << "Failed to bind to port " << Ports[port_index] << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(socket_fd_server[port_index], 10) == -1)
    {
        std::cout << "Failed to listen on port " << Ports[port_index] << std::endl;
        exit(EXIT_FAILURE);
    }
    fcntl(socket_fd_server[port_index], F_SETFL, O_NONBLOCK);
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

void Server::Send_Error_Response(int fd_client)
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
	if (send(fd_client, message.c_str(), message.length(), 0) < 0)
	{
		puts("Send failed");
		return;
	}
}
void Server::Check_file_existence(std::string path)
{
	std::string filename = path;
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
