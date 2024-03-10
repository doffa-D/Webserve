/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:09 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/10 16:18:21 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"


// part of Request Line 
RequestLine::RequestLine() : Method(""), Path(""), HttpVersion(""), Query_Params(""){}
RequestLine::~RequestLine(){}

std::string	RequestLine::getMethod() const{return Method;}
std::string	RequestLine::getPath() const{return Path;}
std::string	RequestLine::getHttpVersion() const{return HttpVersion;}
std::string	RequestLine::getQuery_Params() const{return Query_Params;}

void	RequestLine::setMethod(std::string method){Method = method;}
void	RequestLine::setPath(std::string path){Path = path;}
void	RequestLine::setHttpVersion(std::string httpversion){HttpVersion = httpversion;}


void	RequestLine::Parse_ReqLine(std::string line)
{
	// Find positions of spaces
    size_t pos1 = line.find(' ');
    size_t pos2 = line.find(' ', pos1 + 1);
    size_t pos3 = line.find_last_of(' ');

	// Extract values
	setMethod(line.substr(0, pos1));
	setPath(line.substr(pos1 + 1, pos2 - pos1 - 1)); // pos2 - pos1 - 1 ==> is lenght of the substring
	setHttpVersion(line.substr(pos3 + 1, line.length() - pos2 - 2)); // add line.length() - pos2 - 2 for not need to add \r in this word 
	size_t	pos = getPath().find('?');
	size_t	TmpPos = pos;
	while (pos != std::string::npos && getPath()[pos] == '?')
		pos++;
	if (pos != std::string::npos && pos != getPath().size() - 1)
	{
    	Query_Params = getPath().substr(pos);
    	setPath(getPath().substr(0, TmpPos));
	}
}


// part of Request ==> Header and Body

Request::Request() : ReqLine(RequestLine()), Http_Header(), Body(){BadRequest = 0;}
Request::~Request(){}

RequestLine	Request::getReqLine() const{return ReqLine;}
std::map<std::string, std::string> Request::getHttp_Header() const{return Http_Header;}
std::string	Request::getBody() const{return Body;}

void	Request::setHttp_Header(std::map<std::string, std::string> http_header){Http_Header = http_header;}

void	Request::Parse_Request(std::string& HttpRequest)
{
	bool isBody = false;
	std::istringstream ss(HttpRequest);
	std::string line;
	std::getline(ss, line);
	ReqLine.Parse_ReqLine(line);
	// I Fill request header and i check if i have a bad request 
	std::map<std::string, std::string> Header;
	while (std::getline(ss, line))
	{
		line += "\n";
		if(isBody)
		{
			Body += line;
			continue;
		}
		// Find the position of the colon in the line
        size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
			// Remove leading and trailing whitespaces from key and value
            key.erase(0, key.find_first_not_of(" \t\r\n"));
            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);
			// Store in the map
			Header[key] = value;
		}
		else if(pos == std::string::npos && line != "\r\n")
		{
			BadRequest = 1;
			return;
		}
		if(line == "\r\n")
			isBody = true;
	}
	setHttp_Header(Header);
	if(getHttp_Header()["Host"].empty() 
	|| (ReqLine.getMethod() == "POST" && getHttp_Header()["Content-Length"].empty() && getHttp_Header()["Transfer-Encoding"] != "chunked"))
	{
		BadRequest = 1;
		return;
	}
}

void Request::logging(std::string FilePath, bool isError, std::string Message)
{
	time_t now = time(0);
	std::string host = getHttp_Header()["Host"];
	std::string level = isError ? "[error]" : "[success]";
	std::string timeStr = ctime(&now);
    timeStr.erase(timeStr.find_last_not_of("\n") + 1);
    std::string uri = "\'" + getReqLine().getMethod() + " " + getReqLine().getPath() + " " + getReqLine().getHttpVersion() + "\'";
	std::string logmessage = level + " " + timeStr + " " + Message + " " + uri + " " + host;
	std::ofstream logFile;
    if(!FilePath.empty())
    {
        if(isError)
            logFile.open(FilePath.c_str(), std::ios::out | std::ios::app);
        else
            logFile.open(FilePath.c_str(), std::ios::out | std::ios::app);


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
	// std::cout << logmessage << std::endl;
}