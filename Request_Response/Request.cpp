/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:09 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/25 15:10:44 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// part of Request Line 

RequestLine::RequestLine() : Method(""), Path(""), HttpVersion("")
{}

RequestLine::~RequestLine()
{}

std::string	RequestLine::getMethod() const
{
	return Method;
}

std::string	RequestLine::getPath() const
{
	return Path;
}

std::string	RequestLine::getHttpVersion() const
{
	return HttpVersion;
}

void	RequestLine::setMethod(std::string method)
{
	Method = method;
}

void	RequestLine::setPath(std::string path)
{
	Path = path;	
}

void	RequestLine::setHttpVersion(std::string httpversion)
{
	HttpVersion = httpversion;	
}

void	RequestLine::PrintReqLine()
{
	std::cout << "\n---------------  HttpRequestLine --------------------------------\n";
	std::cout << "Method:	" << getMethod() << std::endl;
    std::cout << "Path: 	" << getPath() << std::endl;
    std::cout << "HTTP Version: 	" << getHttpVersion() << std::endl;
	std::cout << "\n--------------------- ========== ---------------------------------\n";
}

void	RequestLine::Parse_ReqLine(std::string line)
{
	// Find positions of spaces
    size_t pos1 = line.find(' ');
    size_t pos2 = line.find(' ', pos1 + 1);
    size_t pos3 = line.find_last_of(' ');

	 // Extract values
	setMethod(line.substr(0, pos1));
	setPath(line.substr(pos1 + 1, pos2 - pos1 - 1));
	setHttpVersion(line.substr(pos3 + 1));
}


// part of Header 


Request::Request() : Referer(0), ReqLine(RequestLine()), Http_Header()
{}

Request::~Request()
{
}

int	Request::getReferer() const
{
	return Referer;
}

void	Request::settReferer(int referer)
{
	Referer = referer;
}

std::vector<std::pair<std::string, std::string> > Request::getHttp_Header() const
{
	return Http_Header;
}

void	Request::setHttp_Header(std::vector<std::pair<std::string, std::string> > http_header)
{
	Http_Header = http_header;
}

RequestLine	Request::getReqLine() const
{
	return ReqLine;
}

void	Request::CheckReferer()
{
	std::vector<std::pair<std::string, std::string> >::iterator it = Http_Header.begin();
	for (; it != Http_Header.end(); it++)
    {
        if(strncmp(it->first.c_str(), "Referer", 7) == 0)
            Referer = 1;
    }
}

void	Request::PrintHttp_Header()
{
	std::cout << "\n---------------------------------- HTTP_Header ---------------------------------------\n";
	std::vector<std::pair<std::string, std::string> >::iterator it = Http_Header.begin();
	for (; it != Http_Header.end(); it++)
    {
		std::cout << "Key: " << it->first << "   |   Value: " << it->second << std::endl;
    }
	std::cout << "\n---------------------------------- ====== ---------------------------------------\n";
}

void	Request::Parse_Request(std::string& HttpRequest)
{
	// Create an input string stream because getline not take string as a parametre 
	std::istringstream ss(HttpRequest);
	std::string line;
	std::getline(ss, line);
	ReqLine.Parse_ReqLine(line);
	std::vector<std::pair<std::string, std::string> > Header;
	
	while (std::getline(ss, line))
	{
		// Find the position of the colon in the line
        size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
			// Remove leading and trailing whitespaces from key and value
			// but is useless remove leading and trailing whitespaces because in nginx it's possible serve
			// file with spaces or tab
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
			// Store in the Vector
			Header.push_back(std::make_pair(key, value));
		}
	}
	setHttp_Header(Header);
	CheckReferer();
}
