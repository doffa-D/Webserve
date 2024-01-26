/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TmpServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:35:32 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/26 10:40:57 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TmpServer.hpp"
#include "Request.hpp"
#include "Response.hpp"

int portno = 80;

void error(std::string msg)
{
	perror(msg.c_str());
	exit(1);
}

void server(const std::string& fileName)
{

	Parser parser(fileName);
	
	int sockfd, newsockfd, n;
	struct sockaddr_in serv_add, cli_add;
	socklen_t clilen;
	char buffer[1024];
	sockfd = socket(AF_INET, SOCK_STREAM, 6);
	if(sockfd == -1)
		error("Error Opening Socket .\n");
	bzero((char *) &serv_add, sizeof(serv_add));
	serv_add.sin_family = AF_INET;
	serv_add.sin_addr.s_addr = INADDR_ANY;
	// serv_add.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_add.sin_port = htons(portno);
	
	if(bind(sockfd, (struct sockaddr *) &serv_add, sizeof(serv_add)) < 0)
		error("Binding Failed .\n");

	listen(sockfd, 5);


	while(1)
	{
		std::string httpRequest;
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_add, &clilen);
		if(newsockfd < 0)
			error("Error On Accept.\n");
		bzero(buffer, 1024);
		n = recv(newsockfd, buffer, 1024, 0);
		if (n < 0)
			error("Error on reading.\n");
		std::string buf(buffer, n);
		httpRequest += buf;
		
		Request request;
		request.Parse_Request(httpRequest);

		// request.getReqLine().PrintReqLine();
		// request.PrintHttp_Header();
		Response	response;
		response.ft_Response(newsockfd, request, parser);
		//  response
		// request.~Request();
	}
	close(newsockfd);
	close(sockfd);
}
