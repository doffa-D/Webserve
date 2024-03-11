/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serveRequestedResource.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 23:25:00 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/11 13:04:01 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"


void    Response::HandleDeletMethod(const std::string& Root_ReqPath, const Location& location)
{
    if(std::remove(Root_ReqPath.c_str()) == 0)
    {
        Req.logging(location.getAccessLog(), false, "file deleted successfully");
        ResPath = "file deleted successfully";
        Fill_Response("200", "OK", REGULAR_STRING, location);
    }
    else
    {
        Req.logging(location.getErrorLog(), true, "file deleted successfully");
        ResPath = "failed to delete  " + Root_ReqPath;
        Fill_Response("409", "Conflict", REGULAR_STRING, location);
    }
}

void    Response::Upload(const Location& location)
{
    struct statvfs buffer;
    if (location.getUpload().c_str() != NULL && statvfs(location.getUpload().c_str(), &buffer) == 0) 
    {
        unsigned long freeSpace = buffer.f_bavail * buffer.f_frsize; // free space:
        if(Req.getBody().size() > freeSpace)
        {
            Req.logging(location.getErrorLog(), true, "No space left on device");
        }
        else
        {
            Req.logging(location.getAccessLog(), false, "file saved successfully");
            upload_file(Req.getBody(),  location.getUpload(), Req.getHttp_Header());
        }
    }
    else
        Req.logging(location.getErrorLog(), true, "Failed to get disk space information");
}

void	Response::handleDirectoryRequest(const std::string& Root_ReqPath, const Location& location)
{
	if(Req.getReqLine().getPath()[Req.getReqLine().getPath().size() - 1] != '/')
	{
        Req.logging(location.getAccessLog(), 0, "Moved Permanently");
		ResHeader.setLocation("http://" + _host + Req.getReqLine().getPath() + "/");
		ResPath = "";
		Fill_Response("301", "Moved Permanently", REGULAR_STRING, location);
		return ;
	}
	ResPath = location.getIndexFilePathByRoot(Root_ReqPath); // this function search to file in case not found will return empty
	if(ResPath.empty() == 0)
	{
        Req.logging(location.getAccessLog(), 0, "serving file successfully");
		Fill_Response("200", "OK", FILE_PATH, location);
		return;
	}
	if(location.getAutoIndex() == 1)
	{
        Req.logging(location.getAccessLog(), 0, "Using AutoIndex to respond");
		ResPath = AutoIndex(Root_ReqPath, Req.getReqLine().getPath());
		Fill_Response("200", "OK", REGULAR_STRING, location);
		return;
	}
	handleForbidden(location);
}

void Response::handleFileRequest(const std::string& filePath, const Location& location)
{
	size_t pos = str_utils::r_find(filePath, '.');
    string  extension = filePath.substr(pos + 1); // get extension
	string bin = location.getCgi()[extension];
    if (bin.empty())
	{
        Req.logging(location.getAccessLog(), 0, "Respond: using the Normal Way");
        Req.logging(location.getAccessLog(), 0, "serving file successfully");
		ResPath = filePath;
		Fill_Response("200", "ok", FILE_PATH, location);
        //normal way
	}
    else
	{
		size_t _pos = str_utils::r_find(filePath, '/');
		std::map<std::string, std::string> env;
		env["SCRIPT_NAME"] = filePath.substr(_pos + 1); //It will be the name of the file
		env["SCRIPT_FILENAME"] = filePath; //It will be the path of the file
		env["PATH_INFO"] = filePath;

		env["CONTENT_TYPE"] = Req.getHttp_Header()["Content-Type"];
		env["REQUEST_METHOD"] = Req.getReqLine().getMethod();
		env["CONTENT_LENGTH"] = Req.getHttp_Header()["Content-Length"];
		env["QUERY_STRING"] = Req.getReqLine().getQuery_Params();
		env["SERVER_PROTOCOL"] = Req.getReqLine().getHttpVersion();
		
		env["HTTP_COOKIE"] = Req.getHttp_Header()["Cookie"];
		env["SERVER_NAME"] = "WebServer";
		env["REDIRECT_STATUS"] = "200";
	
		std::string body = Req.getBody();// it will be empty in GET !!!
		CGI cgi_obj(body, env, bin);
		std::pair<std::string, int> respont = cgi_obj.fill_env();
        Req.logging(location.getAccessLog(), 0, "Respond: using the CGI Way");
		Check_CGI_Response(respont.first, respont.second, location);
	}
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

