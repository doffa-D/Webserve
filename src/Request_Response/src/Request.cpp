/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:09 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/25 12:27:49 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"


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
	setPath(line.substr(pos1 + 1, pos2 - pos1 - 1)); // pos2 - pos1 - 1 ==> is lenght of the substring
	setHttpVersion(line.substr(pos3 + 1, line.length() - pos2 - 2)); // add line.length() - pos2 - 2 for not need to add \n in this word 
	// std::cout << "method =  [" << getMethod() << "]  Path =  [" << getPath() << "]  HttpVersion = [" << getHttpVersion() << "]" << std::endl;

	size_t	pos = getPath().find('?');
	size_t	TmpPos = pos;
	while (pos != std::string::npos && getPath()[pos] == '?')
		pos++;
	if (pos != std::string::npos && pos != getPath().size() - 1)
	{
    	std::string queryparams = getPath().substr(pos);
    	setPath(getPath().substr(0, TmpPos));
		// here Parse query parameters and fill vector the Query_Params;
		std::vector<std::string> split1 = str_utils::split(queryparams, '&');
		for(size_t i=0; i<split1.size(); i++)
		{
			size_t p = split1[i].find('=');
			if(p != std::string::npos)
				Query_Params.push_back(std::make_pair(split1[i].substr(0, p), split1[i].substr(p + 1)));
		}
		for(size_t i=0; i<Query_Params.size(); i++)
		{
			std::cout << "Key = " << Query_Params[i].first << "			";
			std::cout << "Value = " << Query_Params[i].second << std::endl;
		}
	}
}


// part of Header Request 


Request::Request() : Referer(0), ReqLine(RequestLine()), Http_Header()
{
	BadRequest = 0;
	// Add uppercase letters
    for (char c = 'A'; c <= 'Z'; c++)
        allowedCharacters.push_back(c);
    // Add lowercase letters
    for (char c = 'a'; c <= 'z'; c++)
        allowedCharacters.push_back(c);
    // Add digits
    for (char c = '0'; c <= '9'; c++)
        allowedCharacters.push_back(c);
	// Add other unreserved characters
    const std::string unreservedChars = "-._~";
    for (size_t i = 0; i < unreservedChars.size(); i++)
        allowedCharacters.push_back(unreservedChars[i]);
    // Add reserved characters
	const std::string reservedChars = ":/?#[]@!$&'()*+,;=";
    for (size_t i = 0; i < reservedChars.size(); i++)
        allowedCharacters.push_back(reservedChars[i]);
}

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

std::string	ToUpperStr(std::string str)
{
	std::string res = str;
	for(size_t i=0; i<str.size(); i++)
		res[i] = toupper(str[i]);
	return res;
}

bool Request::LookingForKey()
{
	// Key to search for
    std::string key_to_find = "HOST";

    bool found = false;
    for (size_t i = 0; i < getHttp_Header().size(); ++i)
	{
        if (ToUpperStr(getHttp_Header()[i].first) == key_to_find)
		{
            found = true;
            break;
        }
    }
	return found;
}

bool	Request::FoundDisallowedChar()
{
	bool	disallowedCharFound = false;
	std::string	ReqPath = ReqLine.getPath();
	// for (std::string::iterator it = ReqLine.getPath().begin(); it != ReqLine.getPath().end(); it++)
	for(size_t i=0; i<ReqPath.size(); i++)
	{
		if(std::find(allowedCharacters.begin(), allowedCharacters.end(), ReqPath[i]) == allowedCharacters.end())
		{
			disallowedCharFound = true;
			break;
		}
	}
	return disallowedCharFound;
}

void	Request::Parse_Request(std::string& HttpRequest)
{ 
	std::istringstream ss(HttpRequest);
	std::string line;
	std::getline(ss, line);
	ReqLine.Parse_ReqLine(line);
	// I Check any character not allowed in RequestPath
	if(FoundDisallowedChar() == true)
	{
		BadRequest = 1;
		return;
	}
	// I Fill request header and i check if i have a bad request 
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
            key.erase(0, key.find_first_not_of(" \t\r"));
            key.erase(key.find_last_not_of(" \t\r") + 1);
            value.erase(0, value.find_first_not_of(" \t\r"));
            value.erase(value.find_last_not_of(" \t\r") + 1);
			
			// Store in the Vector
			Header.push_back(std::make_pair(key, value));
		}
		else if(pos == std::string::npos && line != "\r")
		{
			BadRequest = 1;
			return;
		}
	}
	setHttp_Header(Header);
	if(LookingForKey() == false)
	{
		BadRequest = 1;
		return;
	}
	CheckReferer();
}
