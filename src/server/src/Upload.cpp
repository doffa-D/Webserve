/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 09:52:04 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/12/14 11:10:51 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Upload.hpp"


int Upload::find_clinet(int socket_fd)
{
	int client_index = -1;
    
	for (size_t i = 0; i < clients.size(); i++)
	{
		if (clients[i].fd == socket_fd)
		{
			client_index = static_cast<int>(i);
			break;
		}
	}
    return client_index;
}

void ContentLength(std::string requestString, request &client)
{
    std::string content = "Content-Length: ";
    size_t pos = requestString.find(content);
    if (pos != std::string::npos)
    {
        std::string substring = requestString.substr(pos + content.length());
        size_t end = substring.find_first_not_of("0123456789");
        std::string length = substring.substr(0, end);
        client.contentLength = std::strtold(length.c_str(), NULL);

        client.startFContent = requestString.find("\r\n\r\n") + 4;
    }
    std::cout << "content length: " << client.contentLength << std::endl;
    std::cout << "startFContent: " << client.startFContent << std::endl;
}


// void Upload::create_client(std::string body, int socket_fd)
// {
//     int client_index = this->find_clinet(socket_fd);
//     std::cout << "client_index: " << client_index << std::endl;
//     if(client_index == - 1)
//     {
//         	request newClient;
// 			newClient.fd = socket_fd;
// 			newClient.fisrtboundaryValue = "";
// 			newClient.header = "";
// 			newClient.lastboundaryValue = "";
// 			newClient.isFile = false;
// 			newClient.bytes_read = 0;
// 			size_t contentTypePos = body.find("Content-Type:");
// 			ContentLength(body, newClient);
// 			if (contentTypePos != std::string::npos)
// 			{
// 				size_t boundaryPos = body.find("boundary=", contentTypePos);
// 				if (boundaryPos != std::string::npos)
// 				{
// 					size_t start = body.find("\r\n", boundaryPos);
// 					newClient.fisrtboundaryValue = body.substr(boundaryPos + 9, start - (boundaryPos + 9));
// 					newClient.lastboundaryValue = "--" + newClient.fisrtboundaryValue + "--";
// 				}
// 			}
// 			size_t contentLengthPos = body.find("Transfer-Encoding: chunked");
// 			if (contentLengthPos != std::string::npos)
// 				newClient.isChunked = true;
// 			else
// 				newClient.isChunked = false;

// 			size_t start = body.find("\r\n\r\n");
// 			if (start != std::string::npos)
// 			{
// 				newClient.header = body.substr(0, start + 4);
// 				newClient.start = start + 4;
// 			}
// 			else
// 				newClient.start = 0;

// 			clients.push_back(newClient);
// 			client_index = static_cast<int>(clients.size()) - 1;
//     }
//     std::cout << body << std::endl;
// }
