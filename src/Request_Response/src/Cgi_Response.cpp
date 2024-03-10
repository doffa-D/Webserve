/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cgi_Response.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 23:14:09 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/10 15:04:59 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

static void	parseSetCookie(const std::string& attribute, VecStringString& track_cookie)
{
	// string attribute = header.substr(12, header.length());
	VecString _attributes = str_utils::split(attribute, ';');
	VecString_iter _it = _attributes.begin();
	std::pair<string, string> cookie;
	for (;_it != _attributes.end();_it++)
	{
		*_it = str_utils::trim(*_it);
		if (_it->find("SID=") != string::npos)
			cookie.first = str_utils::trim(_it->substr(4, _it->length()));
		else if (_it->find("expires=") != string::npos)
			cookie.second = str_utils::trim(_it->substr(8, _it->length()));
	}
	if (!cookie.first.empty())
	{
		// cout << "cookie.first: [" << cookie.first << "] cookie.second: [" << cookie.second << "]" << endl;
		track_cookie.push_back(cookie);
	}
}

std::string    FindValueOfKey(std::vector <std::pair<string, string> > Cgi_Header, std::string Key)
{
    std::vector <std::pair<string, string> >::iterator it;
    for(it = Cgi_Header.begin(); it != Cgi_Header.end(); it++)
    {
        if(it->first == Key)
            return it->second;
    }
    return "";
}

bool    CheckKeyinHeader(std::vector <std::pair<string, string> > Cgi_Header, std::string Key)
{
    std::vector <std::pair<string, string> >::iterator it;
    for(it = Cgi_Header.begin(); it != Cgi_Header.end(); it++)
    {
        if(it->first == Key)
            return true;
    }
    return false;
}

void	Response::processCgiResponse(const std::string& Cgi_Response)
{
	std::vector <std::pair<string, string> > Cgi_Header;
    std::string    Header;
    std::string 	Body;

    std::istringstream ss(Cgi_Response);
    std::string line;
    bool isBody = false;
    while (std::getline(ss, line))
    {
		line += "\n";
        if(isBody)
        {
            Body += line;
            continue;
        }
		//
		if(line.find("Set-Cookie: ") != std::string::npos)
			parseSetCookie(line.substr(12),track_cookie);
        if(line.find("Status") == std::string::npos)
            Header += line;
        size_t pos = line.find(':');
        if (pos != std::string::npos)
        {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
			key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            // Storing at cgivector
            Cgi_Header.push_back(std::make_pair(key, value));
        }
        if(line == "\r\n")
            isBody = true;
    }
    if(!CheckKeyinHeader(Cgi_Header, "Status"))
        response = "HTTP/1.1 200 OK\r\n";
    else
        response = "HTTP/1.1 " + FindValueOfKey(Cgi_Header, "Status");
    if(!CheckKeyinHeader(Cgi_Header, "Content-Length"))
        response += "Content-Length: " + std::to_string(Body.size()) + "\r\n";
    if(!CheckKeyinHeader(Cgi_Header, "Content-Type") && !CheckKeyinHeader(Cgi_Header, "Location"))
        response += "Content-Type: text/html\r\n";
    response += Header + Body;
}

void	Response::Check_CGI_Response(std::string Cgi_Response, int Cgi_Stat_Code, const Location& location)
{
	if(Cgi_Stat_Code == 0) // CGI Succes
	{
		size_t pos = Cgi_Response.find("\r\n\r\n");
		if(pos != std::string::npos) // it mean response fiha separater bin header o body
			processCgiResponse(Cgi_Response);
		else
		{
			ResPath = Cgi_Response;
			Fill_Response("200", "OK", REGULAR_STRING, location);
		}
		Req.logging(location.getAccessLog(), 0, "Respond: using the CGI Way");
		return;
	}
	if(Cgi_Stat_Code == 260) // sys call faild
	{
		Req.logging(location.getErrorLog(), 1, "Internal Server Error");
		ResPath = Error_HmlPage("500", "Internal Server Error");
		Fill_Response("500", "Internal Server Error", REGULAR_STRING, location);
		return ;
	}
	if(Cgi_Stat_Code == 266) // Gateway Timeout
	{
		Req.logging(location.getErrorLog(), 1, "Gateway Timeout");
		ResPath = Error_HmlPage("504", "Gateway Timeout");
		Fill_Response("504", "Gateway Timeout", REGULAR_STRING, location);
		return ;
	}
	else
	{
		Req.logging(location.getErrorLog(), 1, "Bad Gateway");
		ResPath = Error_HmlPage("502", "Bad Gateway<br/><p style=\"font-size: 15;\">" + Cgi_Response + "<p/>");
		Fill_Response("502", "Bad Gateway", REGULAR_STRING, location);
		return ;
	}
}