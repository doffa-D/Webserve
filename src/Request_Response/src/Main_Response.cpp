/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/09 20:08:04 by rrhnizar         ###   ########.fr       */
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
    long MaxBodySize = atoi(Req.getHttp_Header()["Content-Length"].c_str());
    return MaxBodySize <= location.getClientMaxBodySize();
}


bool    Response::isUriTooLong(const long& _value)
{
    size_t LenghtOfPath = Req.getReqLine().getPath().size();
    if(LenghtOfPath > (size_t)_value)
        return true;
    return false;
}


std::pair<std::string, Location> Response::extractRequestInformation(const Parser& parser)
{
    // Extract necessary information from the request
    _host = Req.getHttp_Header()["Host"];
    Server server = parser.getServerbyHost(_host);
    std::pair<std::string, Location> locationPair = server.getLocationByPath(Req.getReqLine().getPath());
    locationPair.second.setUri_Max(server.getClientMaxHeaderBufferSize());
    return locationPair;
}

bool    Response::HttpVerNotSuported()
{
    std::string HttpVersion = Req.getReqLine().getHttpVersion();
     return std::find(httpVersions.begin(), httpVersions.end(), HttpVersion) != httpVersions.end(); // return true if found HttpVersion and false if not found HttpVersion
}

bool Response::handleCommonRequestErrors(const Location& location)
{
    // Handle common request errors
    if(HttpVerNotSuported())
    {
        handleHttpVerNotSuported(location);
        return true;
    }
    handleBadRequest(location);
    if (ReqErr == 1)
        return true;
    if (isUriTooLong(location.getUri_Max()))
    {
        handleUriTooLong(location);
        return true;
    }
    if (!isRequestBodySizeAllowed(location))
    {
        handleBodyTooLarge(location);
        return true;
    }
    if (!isMethodAllowed(location))
    {
        handleMethodNotAllowed(location);
        return true;
    }
    return false;
}


bool Response::handleRedirection(const Location& location)
{
    // Handle redirection if specified
    std::string redirection = location.getRedirection();
    if (!redirection.empty())
    {
        ResHeader.setLocation("http://" + _host + redirection);
        ResPath = "";
        Fill_Response("303", "See Other", REGULAR_STRING, location);
        return true;
    }
    return false;
}

std::string Response::constructRootRequestPath(const Location& location, const std::string& LocationName)
{
    // Construct absolute path for serving requested resource
    std::string reqPath = Req.getReqLine().getPath();
    std::string rootReqPath;
    if (location.getAlias().empty())
        rootReqPath = location.getRoot() + reqPath; // construct Absolute Path
    else
    {
        size_t found = reqPath.find(LocationName);
        rootReqPath = reqPath.replace(found, LocationName.length(), location.getAlias());
    }
    return rootReqPath;
}


std::string Response::ft_Response(const Parser& parser)
{
    // Extract necessary information
    std::pair<std::string, Location> locationPair = extractRequestInformation(parser);
    // Handle common request errors
    if(handleCommonRequestErrors(locationPair.second))
        return response;
    // Handle redirection if specified
    if(handleRedirection(locationPair.second))
        return response;
    // Construct absolute path for serving requested resource
    std::string rootRequestPath = constructRootRequestPath(locationPair.second, locationPair.first);
    // Serve requested resource or handle not found
    if (!serveRequestedResource(rootRequestPath, locationPair.second))
        handleNotFound(locationPair.second);
    return response;
}
