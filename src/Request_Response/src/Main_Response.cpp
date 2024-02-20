/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/20 21:49:29 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

bool Response::isMethodAllowed(const Location& location, const Request& Req)
{
    std::vector<std::string> Methods = location.getAllowedMethods();
    std::string method = Req.getReqLine().getMethod();
    return std::find(Methods.begin(), Methods.end(), method) != Methods.end();
}

bool Response::isRequestBodySizeAllowed(const Location& location, const Request& Req)
{
    return MaxBodySize(Req) <= location.getClientMaxBodySize();
}

std::string Response::constructAbsolutePath(const Location& location, const Request& Req)
{
    return location.getRoot() + Req.getReqLine().getPath();
}

void Response::sendMethodNotAllowedResponse(int clientSocket, const Location& location, const std::string& _host)
{
    if (location.getErrorPages()[405].empty())
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
}

void Response::sendRequestBodyTooLargeResponse(int clientSocket, const Location& location, const std::string& _host)
{
    if (location.getErrorPages()[413].empty())
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
}

bool Response::serveRequestedResource(int clientSocket, const Request& Req, const std::string& Root_ReqPath, const Location& location, const std::string& _host)
{
    std::ifstream File(Root_ReqPath.c_str());
    if (File.is_open())
	{
        struct stat fileInfo;
        if (stat(Root_ReqPath.c_str(), &fileInfo) == 0)
		{
            if (S_ISDIR(fileInfo.st_mode))
                handleDirectoryRequest(clientSocket, Req, _host, Root_ReqPath, location);
            else if (S_ISREG(fileInfo.st_mode))
                handleFileRequest(clientSocket, Root_ReqPath, location);
            return true;
        }
    }
    return false;
}



void Response::ft_Response(int clientSocket, Request& Req, const Parser& parser)
{
    std::string _host = findHostFromHeaders(Req);
    Server server = parser.getServerbyHost(_host);
    Location location = server.getLocationByPath(Req.getReqLine().getPath());
    
    if (!isMethodAllowed(location, Req))
	{
        sendMethodNotAllowedResponse(clientSocket, location, _host);
        return;
    }
    if (!isRequestBodySizeAllowed(location, Req))
	{
        sendRequestBodyTooLargeResponse(clientSocket, location, _host);
        return;
    }
    std::string Root_ReqPath = constructAbsolutePath(location, Req);
    if (!serveRequestedResource(clientSocket, Req, Root_ReqPath, location, _host))
        handleNotFound(clientSocket, location, _host);
}