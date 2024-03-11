/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/10 19:07:19 by rrhnizar         ###   ########.fr       */
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
        Req.logging(location.getErrorLog(), 1, "HTTP Version Not Supported");
        return true;
    }
    handleBadRequest(location);
    if (ReqErr == 1)
    {
        Req.logging(location.getErrorLog(), 1, "Bad Request");
        return true;
    }
    if (isUriTooLong(location.getUri_Max()))
    {
        Req.logging(location.getErrorLog(), 1, "URI Too Long");
        handleUriTooLong(location);
        return true;
    }
    if (!isRequestBodySizeAllowed(location))
    {
        Req.logging(location.getErrorLog(), 1, "Content Too Large");
        handleBodyTooLarge(location);
        return true;
    }
    if (!isMethodAllowed(location))
    {
        Req.logging(location.getErrorLog(), 1, "Method Not Allowed");
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
        std::string Msg = "Redirect to " + redirection + " location";
        Req.logging(location.getAccessLog(), 0, Msg);
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

void    Response::ProvideDefaultWelcomePage(const Location& location)
{
    std::string str = "<!DOCTYPE html>\n"
                  "<html>\n"
                  "<head>\n"
                  "<title>Welcome to fake nginx!</title>\n"
                  "<style>\n"
                  "html { color-scheme: light dark; }\n"
                  "body { width: 35em; margin: 0 auto;\n"
                  "font-family: Tahoma, Verdana, Arial, sans-serif; }\n"
                  "</style>\n"
                  "</head>\n"
                  "<body>\n"
                  "<h1>Welcome to fake nginx!</h1>\n"
                  "<p>If you see this page, the fake nginx web server is successfully installed and\n"
                  "working. Further configuration is required.</p>\n"
                  "<p><em>Thank you for using fake nginx.</em></p>\n"
                  "<p>For online documentation and support please refer to\n"
                  "<a href=\"http://nginx.org/\">nginx.org</a>.<br/>\n"
                  "Commercial support is available at\n"
                  "<a href=\"http://nginx.com/\">nginx.com</a>.</p>\n"
                  "<p><em>Thank you for using fake nginx.</em></p><br/>\n"
                  "<p><em> Developed By kchaouki hdagdagu rrhnizar .<em></p>\n"
                  "</body>\n"
                  "</html>";
    ResPath = str;
    Fill_Response("200", "Ok", REGULAR_STRING, location);
}

std::string Response::ft_Response(const Parser& parser)
{
    // Extract necessary information
    std::pair<std::string, Location> locationPair = extractRequestInformation(parser);
    // 
    Req.logging(locationPair.second.getAccessLog(), 0, "Accept the connection");
    // Handle common request errors
    if(handleCommonRequestErrors(locationPair.second))
        return response;
    // Handle redirection if specified
    if(handleRedirection(locationPair.second))
        return response;
    // Provide default Welcome Page 
    if(Req.getReqLine().getPath() == "/" && locationPair.second.getRoot() == "." && locationPair.second.getIndexes()[0] == "index.html")
    {
        ProvideDefaultWelcomePage(locationPair.second);
        std::cout << "Provide with default Welcome Page";
        return response;
    }
    // Construct absolute path for serving requested resource
    std::string rootRequestPath = constructRootRequestPath(locationPair.second, locationPair.first);
    // Serve requested resource or handle not found
    if (!serveRequestedResource(rootRequestPath, locationPair.second))
        handleNotFound(locationPair.second);
    return response;
}
