/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/20 18:57:23 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"


void	Response::ft_Response(int clientSocket, Request& Req, const Parser& parser)
{
	std::string	_host;
	Location	location;
	std::string	Root_ReqPath;
	Server		server;

	_host = findHostFromHeaders(Req);
	server = parser.getServerbyHost(_host);
	std::cout << "Host = " << _host << std::endl;
	location = server.getLocationByPath(Req.getReqLine().getPath());
	std::cout << "error pages : \n" << location.getErrorPages()[400] << std::endl;
	// exit(1);
	VecString Methods = location.getAllowedMethods();
	std::string method = Req.getReqLine().getMethod();
	// std::cout << "Max Body size = " << location.getClientMaxBodySize() << std::endl;
	long	MBS = MaxBodySize(Req);
	// std::cout << "MBS =  " << MBS << std::endl;
	if(std::find(Methods.begin(), Methods.end(), method) == Methods.end()) // Check allowed methods
	{
		std::cout << "method : \n" << method << std::endl;
		if(location.getErrorPages()[405].empty() == 1)
		{
			ResPath = Error_HmlPage("405", "Method Not Allowed");
			Fill_Response("405", "Method Not Allowed", 1, location);
		}
		else
		{
			ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[405]);
			ResPath = "";
			Fill_Response("302", "Moved Temporarily", 1, location);
		}
		send(clientSocket, response.c_str(), response.size(), 0);
		return ;
	}
	else if(MBS > location.getClientMaxBodySize()) // Check CLient Max Body Size 
	{
		if(location.getErrorPages()[413].empty() == 1)
		{
			ResPath = Error_HmlPage("413", "Content Too Large");
			Fill_Response("413", "Content Too Large", 1, location);
		}
		else
		{
			ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[413]);
			ResPath = "";
			Fill_Response("302", "Moved Temporarily", 1, location);
		}
		send(clientSocket, response.c_str(), response.size(), 0);
		return ;
	}
	Root_ReqPath = location.getRoot() + Req.getReqLine().getPath();
	std::cout << "Root = " << location.getRoot() << "ReqPath " << Req.getReqLine().getPath() << std::endl;
	setResPath(Root_ReqPath);
	std::ifstream File(Root_ReqPath.c_str());
	std::cout << "file open : \n " << Root_ReqPath << std::endl;
	if(File.is_open())
	{
		struct stat fileInfo;
		if (stat(Root_ReqPath.c_str(), &fileInfo) == 0)
		{
			if (S_ISDIR(fileInfo.st_mode))
				handleDirectoryRequest(clientSocket, Req, _host, Root_ReqPath, location);
			else if (S_ISREG(fileInfo.st_mode))
				handleFileRequest(clientSocket, Root_ReqPath, location);
		}
	}
	else
		handleNotFound(clientSocket, location, Req, _host);
}
