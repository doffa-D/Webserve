/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/25 19:26:18 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

bool Response::isMethodAllowed(const Location& location)
{
    std::vector<std::string> Methods = location.getAllowedMethods();
    std::string method = Req.getReqLine().getMethod();
    return std::find(Methods.begin(), Methods.end(), method) != Methods.end(); // return true if MA and false if MNA
}

bool Response::isRequestBodySizeAllowed(const Location& location)
{
    return MaxBodySize() <= location.getClientMaxBodySize();
}

bool Response::serveRequestedResource(const std::string& Root_ReqPath, const Location& location)
{
    std::ifstream File(Root_ReqPath.c_str());
    if (File.is_open())
	{
        struct stat fileInfo;
        if (stat(Root_ReqPath.c_str(), &fileInfo) == 0)
		{
            if (S_ISDIR(fileInfo.st_mode))
                handleDirectoryRequest(Root_ReqPath, location);
            else if (S_ISREG(fileInfo.st_mode))
                handleFileRequest(Root_ReqPath, location);
            return true;
        }
    }
    return false;
}


bool    Response::isUriTooLong(const long& _value)
{
    size_t LenghtOfPath = Req.getReqLine().getPath().size();
	// cout << "_value: " << _value << endl;
	// cout << "LenghtOfPath: " << LenghtOfPath << endl;
    // size_t TmpLenghtIsDefinedInConfig = 10; // i need this from config file
    if(LenghtOfPath > (size_t)_value)
        return true;
    return false;
}

void Response::ft_Response(int clientSocket, const Parser& parser)
{
    _host = findHostFromHeaders();
    Server server = parser.getServerbyHost(_host);
    Location location = server.getLocationByPath(Req.getReqLine().getPath());

    handleBadRequest(location);
    if(ReqErr == 1)
    {
        send(clientSocket, response.c_str(), response.size(), 0);
        return;
    }
    if(isUriTooLong(server.getClientMaxHeaderBufferSize()))
    {
        handleUriTooLong(location);
        send(clientSocket, response.c_str(), response.size(), 0);
        return;
    }
    if (!isMethodAllowed(location))
	{
        handleMethodNotAllowed(location);
        send(clientSocket, response.c_str(), response.size(), 0);
        return;
    }
    if (!isRequestBodySizeAllowed(location))
	{
        handleBodyTooLarge(location);
        send(clientSocket, response.c_str(), response.size(), 0);
        return;
    }
    std::string Root_ReqPath = location.getRoot() + Req.getReqLine().getPath(); // construct Absolute Path
    if (!serveRequestedResource(Root_ReqPath, location))
        handleNotFound(location);
    send(clientSocket, response.c_str(), response.size(), 0);
}