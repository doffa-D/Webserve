/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/08 19:07:45 by rrhnizar         ###   ########.fr       */
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

void logging(std::string FilePath,bool isError,std::string Message,Request Request)
{
	time_t now = time(0);
	std::string host = Request.getHttp_Header()["Host"];
	std::string level = isError ? "[error]" : "[success]";
	std::string timeStr = ctime(&now);
    timeStr.erase(timeStr.find_last_not_of("\n") + 1);
    std::string uri = "\'" + Request.getReqLine().getMethod() + " " + Request.getReqLine().getPath() + " " + Request.getReqLine().getHttpVersion() + "\'";
	std::string logmessage = level + " " + timeStr + " " + Message + " " + uri + " " + host;
	std::ofstream logFile;
    if(!FilePath.empty())
    {
        if(isError)
            logFile.open(FilePath, std::ios::out | std::ios::app);
        else
            logFile.open(FilePath, std::ios::out | std::ios::app);


        if (logFile.is_open())
        {
            logFile << logmessage << std::endl;
            logFile.close();
        }
        else
        {
            std::cerr << "Unable to open log file" << std::endl;
        }
    }
	std::cout << logmessage << std::endl;
}

void    Response::HandleDeletMethod(const std::string& Root_ReqPath, const Location& location)
{
    if(std::remove(Root_ReqPath.c_str()) == 0)
    {
        Fill_Response("200", "OK", 1, location);
        std::cout << "deleted\n";
    }
    else
        std::cout << "not deleted\n";
}

void    Response::Upload(const Location& location)
{
    struct statvfs buffer;
    if (location.getUpload().c_str() != NULL && statvfs(location.getUpload().c_str(), &buffer) == 0) 
    {
        unsigned long freeSpace = buffer.f_bavail * buffer.f_frsize; // free space:
        if(Req.getBody().size() > freeSpace)
        {
            logging(location.getErrorLog(),true,"No space left on device",Req);
        }
        else
        {
            logging(location.getAccessLog(),false,"file saved successfully",Req);
            upload_file(Req.getBody(),  location.getUpload(), Req.getHttp_Header());
        }
    }
    else
        logging(location.getErrorLog(),true,"Failed to get disk space information",Req);
    
}


bool Response::serveRequestedResource(const std::string& Root_ReqPath, const Location& location)
{
    struct stat	fileInfo;
    if(stat(Root_ReqPath.c_str(), &fileInfo) == 0)
    {
        size_t _pos = str_utils::r_find(Root_ReqPath, '.');
        std::string bin = location.getCgi()[Root_ReqPath.substr(_pos + 1)];
        if(bin.empty() && Req.getReqLine().getMethod() == "DELETE")
        {
            HandleDeletMethod(Root_ReqPath, location);
            return true;
        }
        if(bin.empty() && Req.getReqLine().getMethod() == "POST")
		    Upload(location);
        if (access(Root_ReqPath.c_str(), R_OK) == -1)
            handleForbidden(location);
        else if (S_ISDIR(fileInfo.st_mode))
            handleDirectoryRequest(Root_ReqPath, location);
        else if (S_ISREG(fileInfo.st_mode))
            handleFileRequest(Root_ReqPath, location);
        return true;
    }
    return false;
}

bool    Response::isUriTooLong(const long& _value)
{
    size_t LenghtOfPath = Req.getReqLine().getPath().size();
    if(LenghtOfPath > (size_t)_value)
        return true;
    return false;
}

std::string Response::ft_Response(const Parser& parser)
{
    _host = Req.getHttp_Header()["Host"];
    Server server = parser.getServerbyHost(_host);
    pair<string, Location> Loc = server.getLocationByPath(Req.getReqLine().getPath());
    std::string LocationName = Loc.first;
    Location location = Loc.second;

    handleBadRequest(location);
    if(ReqErr == 1)
        return response;
    if(isUriTooLong(server.getClientMaxHeaderBufferSize()))
    {
        handleUriTooLong(location);
        return response;
    }
    if (!isRequestBodySizeAllowed(location))
	{
        handleBodyTooLarge(location);
        return response;
    }
    if (!isMethodAllowed(location))
	{
        handleMethodNotAllowed(location);
        std::cout << "Response : \n" << response << std::endl;
        return response;
    }
    string Redirection = location.getRedirection();
    if(!Redirection.empty())
    {
        ResHeader.setLocation("http://" + _host + Redirection);
		ResPath = "";
        Fill_Response("303", "See Other", 1, location);
        return response;
    }
    
    std::string Root_ReqPath;
    std::string ReqPath = Req.getReqLine().getPath();
    if(location.getAlias().empty())
        Root_ReqPath = location.getRoot() + Req.getReqLine().getPath(); // construct Absolute Path
    else
    {
        size_t found = ReqPath.find(LocationName);
        Root_ReqPath = ReqPath.replace(found, LocationName.length(), location.getAlias());
    }
    if (!serveRequestedResource(Root_ReqPath, location))
        handleNotFound(location);
    return response;
}
