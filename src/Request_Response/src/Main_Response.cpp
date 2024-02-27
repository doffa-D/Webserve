/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/27 16:05:05 by rrhnizar         ###   ########.fr       */
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

std::string Response::ft_Response(int clientSocket, const Parser& parser)
{
    (void)clientSocket;
    _host = findHostFromHeaders();
    Server server = parser.getServerbyHost(_host);
    Location location = server.getLocationByPath(Req.getReqLine().getPath());

    // cout << "Path: " << location.getUpload() << endl; /* get upload path of a certine 
    //                                                         location if it has ben set in config file, else get defualt path*/
    // MapStringString cgi = location.getCgi();

    // string  extionsion ; // get extension
    // if (cgi[extionsion].empty())
    //     (void)extionsion;//normale way
    // else
    //     (void)extionsion;//cgi way

    handleBadRequest(location);
    if(ReqErr == 1)
    {
        // send(clientSocket, response.c_str(), response.size(), 0);
        // return;
        return response;
    }
    if(isUriTooLong(server.getClientMaxHeaderBufferSize()))
    {
        handleUriTooLong(location);
        return response;
        // send(clientSocket, response.c_str(), response.size(), 0);
        // return;
    }
    if (!isMethodAllowed(location))
	{
        handleMethodNotAllowed(location);
        return response;
        // send(clientSocket, response.c_str(), response.size(), 0);
        // return;
    }
    if (!isRequestBodySizeAllowed(location))
	{
        handleBodyTooLarge(location);
        return response;
        // send(clientSocket, response.c_str(), response.size(), 0);
        // return;
    }
    std::string Root_ReqPath = location.getRoot() + Req.getReqLine().getPath(); // construct Absolute Path
    // if(location.getRoot().empty())
    // {
    //     Root_ReqPath = Root_ReqPath.substr(1);
    // }
    // std::string Root_ReqPath = Req.getReqLine().getPath(); // construct Absolute Path
    // std::cout << "Root_ReqPath  =  " << Root_ReqPath << std::endl;
    if (!serveRequestedResource(Root_ReqPath, location))
        handleNotFound(location);
    return response;
    // send_full_response(clientSocket, response.c_str());
    // send(clientSocket, response.c_str(), response.size(), 0);
}