/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:14 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/31 19:26:48 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

// part of Response line

ResponseLine::ResponseLine(): HttpVersion(""), Status_Code(""), Status_Message("")
{}

ResponseLine::~ResponseLine()
{}

std::string ResponseLine::getHttpVersion() const
{
	return 	HttpVersion;
}

std::string ResponseLine::getStatus_Code() const
{
	return Status_Code;
}

std::string ResponseLine::getStatus_Message() const
{
	return Status_Message;
}


void ResponseLine::setHttpVersion(std::string htttpversion)
{
	HttpVersion = htttpversion;
}

void ResponseLine::setStatus_Code(std::string status_code)
{
	Status_Code = status_code;
}

void ResponseLine::setStatus_Message(std::string status_message)
{
	Status_Message = status_message;
}

// part of Response headre 
ResponseHeader::ResponseHeader():ContentType(""), ContentLength(""), ContentFile("")
{}

ResponseHeader::~ResponseHeader()
{}

std::string ResponseHeader::getContentType() const
{
	return ContentType;
}

std::string ResponseHeader::getContentLength() const
{
	return ContentLength;
}

std::string ResponseHeader::getContentFile() const
{
	return ContentFile;
}

void ResponseHeader::setContentType(std::string contenttype)
{
	ContentType = contenttype;
}
void ResponseHeader::setContentLength(std::string contentlength)
{
	ContentLength = contentlength;	
}
void ResponseHeader::setContentFile(std::string contentfile)
{
	ContentFile = contentfile;	
}

// part of Response 
Response::Response() : ResLine(ResponseLine()), ResHeader(ResponseHeader()), ResBody(""), ResPath("")
{
}

Response::~Response()
{
}


ResponseLine	Response::getResLine() const
{
	return ResLine;
}

ResponseHeader	Response::getResHeader() const
{
	return ResHeader;
}

std::string		Response::getResBody() const
{
	return ResBody;
}

std::string		Response::getResPath() const
{
	return ResPath;
}

void	Response::setResLine(ResponseLine resline)
{
	ResLine = resline;
}
void	Response::setResHeader(ResponseHeader resheader)
{
	ResHeader = resheader;	
}
void	Response::setResBody(std::string resbody)
{
	ResBody = resbody;
}
void	Response::setResPath(std::string respath)
{
	ResPath = respath;
}

std::string	Response::Error_HmlPage(const std::string& stat_code, const std::string& stat_msg)
{
	std::string	Error_Page = "error " + stat_code + "  " + stat_msg + " \n";
	return Error_Page;
}

std::string	ReadFile(std::string&	ResPath)
{
	// std::cout << "---->        " << ResPath << std::endl;
    std::ifstream file(ResPath);
    std::string line;
    std::string res;
	while (std::getline(file, line))
        res += line + "\n";
    return res;
}

std::string	Response::Fill_Response()
{
	ResponseLine	Resline;

	ResLine.setHttpVersion("HTTP/1.1");
	ResLine.setStatus_Code("200");
	ResLine.setStatus_Message("OK");
	
	ResponseHeader	ResHeader;

	ResHeader.setContentFile(ReadFile(ResPath));
	ResHeader.setContentLength(str_utils::to_string(ResHeader.getContentFile().size()));
	ResHeader.setContentType("text/html");

	std::string	response = ResLine.getHttpVersion() + " " + ResLine.getStatus_Code() + " " + ResLine.getStatus_Message() + "\r\n" +
                "Content-Type: " + ResHeader.getContentType() + "\r\n" +
                "Content-Length: " + ResHeader.getContentLength() + "\r\n" +
                "\r\n" + ResHeader.getContentFile();
    return response;
}

std::string root = "./Resources";



Location	Response::Find_Location(Parser& parser, std::string& _host, std::string Path_Req)
{
	Server		server;
	Location	location;
	
	server = parser.getServerbyHost(_host);
	if(server.isNull() == false)
	{
		std::cout << "found server" << std::endl;
		location = server.getLocationByPath(Path_Req);
		if(location.isNull() == false)
		{
			std::cout << "found location" << std::endl;
			std::cout << location.getIndexes().at(0) << std::endl;
			std::cout << location.getAutoIndex() << std::endl;
		}
		else
		{
			// if not found location use directive dyal server
			// location.setRoot(server.getRoot());
			// location.setAutoIndex(server.getAutoIndex());
			std::cout << "not found location " << std::endl;
			std::cout << "Root = " << location.getRoot() << std::endl;
		}
	}
	else
		std::cout << "not found server" << std::endl;
	return location;
}

void	Response::ft_Response(int clientSocket, Request& Req, Parser& parser)
{
	std::string	_host;
	Location	location;

	for(size_t i=0; i<Req.getHttp_Header().size(); i++)
	{
		if (Req.getHttp_Header().at(i).first == "Host")
		{
			_host = Req.getHttp_Header().at(i).second;
			break;
		}
	}
	location = Find_Location(parser, _host, Req.getReqLine().getPath());
	
	// if(Req.getReferer() == 0)
    // {
		setResPath(root + Req.getReqLine().getPath()  + "index.html");
	// }
	
	std::cout << getResPath() << std::endl;
	std::string response = Fill_Response();
	send(clientSocket, response.c_str(), response.size(), 0);
}