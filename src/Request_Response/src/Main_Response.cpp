/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/21 11:47:49 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

bool Response::isMethodAllowed(const Location& location, const Request& Req)
{
    std::vector<std::string> Methods = location.getAllowedMethods();
    std::string method = Req.getReqLine().getMethod();
    return std::find(Methods.begin(), Methods.end(), method) != Methods.end(); // return true if MA and false if MNA
}

bool Response::isRequestBodySizeAllowed(const Location& location, const Request& Req)
{
    return MaxBodySize(Req) <= location.getClientMaxBodySize();
}

bool Response::serveRequestedResource(const Request& Req, const std::string& Root_ReqPath, const Location& location)
{
    std::ifstream File(Root_ReqPath.c_str());
    if (File.is_open())
	{
        struct stat fileInfo;
        if (stat(Root_ReqPath.c_str(), &fileInfo) == 0)
		{
            if (S_ISDIR(fileInfo.st_mode))
                handleDirectoryRequest(Req, Root_ReqPath, location);
            else if (S_ISREG(fileInfo.st_mode))
                handleFileRequest(Root_ReqPath, location);
            return true;
        }
    }
    return false;
}



void Response::ft_Response(int clientSocket, Request& Req, const Parser& parser)
{
    _host = findHostFromHeaders(Req);
    Server server = parser.getServerbyHost(_host);
    Location location = server.getLocationByPath(Req.getReqLine().getPath());

    if (!isMethodAllowed(location, Req))
	{
        sendMethodNotAllowedResponse(location);
        send(clientSocket, response.c_str(), response.size(), 0);
        return;
    }
    if (!isRequestBodySizeAllowed(location, Req))
	{
        sendRequestBodyTooLargeResponse(location);
        send(clientSocket, response.c_str(), response.size(), 0);
        return;
    }
    std::string Root_ReqPath = location.getRoot() + Req.getReqLine().getPath(); // construct Absolute Path
    if (!serveRequestedResource(Req, Root_ReqPath, location))
        handleNotFound(location);
    
    send(clientSocket, response.c_str(), response.size(), 0);
}