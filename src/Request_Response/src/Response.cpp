/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:14 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/20 21:44:54 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

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
ResponseHeader::ResponseHeader():ContentType(""), ContentLength(""), ContentFile(""), Location("")
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

std::string	ResponseHeader::getLocation() const
{
	return Location;
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
void	ResponseHeader::setLocation(std::string	location)
{
	Location = location;
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
	std::string	Error_Page = "<html>\r\n<head><title>" + stat_code + "  " + stat_msg + "</title></head>\r\n" +
							"<body>\r\n<center><h1>" +  stat_code + "  " + stat_msg + "</h1></center>\r\n" +
							"<hr><center> Fake nginx </center></body></html>";
	return Error_Page;
}

void	Response::Fill_Response(std::string	Stat_Code, std::string	Stat_Msg, int File_Or_Str, Location location)
{
	ResLine.setHttpVersion("HTTP/1.1");
	ResLine.setStatus_Code(Stat_Code);
	ResLine.setStatus_Message(Stat_Msg);

	if(File_Or_Str == 0)
		ResHeader.setContentFile(ReadFile(ResPath));
	else
		ResHeader.setContentFile(ResPath);
	ResHeader.setContentLength(str_utils::to_string(ResHeader.getContentFile().size()));
	size_t pos = ResPath.find('.');
	ResHeader.setContentType(location.getMimeTypeByKey(ResPath.substr(pos + 1)));//this line needs to more checks.



	response = ResLine.getHttpVersion() + " " + ResLine.getStatus_Code() + " " + ResLine.getStatus_Message() + "\r\n" +
                "Content-Type: " + ResHeader.getContentType() + "\r\n" +
                "Content-Length: " + ResHeader.getContentLength() + "\r\n" +
				"Location: " + ResHeader.getLocation() + "\r\n" +
                "\r\n" + ResHeader.getContentFile();
}

void	Response::handleDirectoryRequest(int clientSocket, const Request& Req, const std::string& _host, const std::string& Root_ReqPath, const Location& location)
{
	if(Req.getReqLine().getPath()[Req.getReqLine().getPath().size() - 1] != '/')
	{
		ResHeader.setLocation("http://" + _host + Req.getReqLine().getPath() + "/");
		ResPath = "";
		Fill_Response("301", "Moved Permanently", 1, location);
		send(clientSocket, response.c_str(), response.size(), 0);
		// close(clientSocket);
		return ;
	}
	ResPath = location.getIndexFilePathByRoot(Root_ReqPath); // this function kat9alb 3la file ila mal9atxe xi file kat3tina empty
	if(ResPath.empty() == 0)
	{
		Fill_Response("200", "OK", 0, location);
	}
	else if(location.getAutoIndex() == 1)
	{
		ResPath = AutoIndex(Root_ReqPath, Req.getReqLine().getPath());
		Fill_Response("200", "OK", 1, location);
	}
	else
	{
		if(location.getErrorPages()[403].empty() == 1)
		{
			ResPath = Error_HmlPage("403", "Forbiden");
			Fill_Response("403", "Forbiden", 1, location);
		}
		else
		{
			ResHeader.setLocation("http://" + _host +  Req.getReqLine().getPath() + location.getErrorPages()[403]);
			ResPath = "";
			Fill_Response("302", "Moved Temporarily", 1, location);
		}
	}
	send(clientSocket, response.c_str(), response.size(), 0);
	// close(clientSocket);
}

void Response::handleFileRequest(int clientSocket, const std::string& filePath, const Location& location)
{
	ResPath = filePath;
	Fill_Response("200", "ok", 0, location);
	send(clientSocket, response.c_str(), response.size(), 0);
	// close(clientSocket);
}

void Response::handleNotFound(int clientSocket, Location& location, const std::string& _host)
{
	if(location.getErrorPages()[404].empty() == 1)
	{
		ResPath = Error_HmlPage("404", "Not Found");
		Fill_Response("404", "Not Found", 1, location);
	}
	else
	{
		// ResHeader.setLocation("http://" + _host + Req.getReqLine().getPath() + "/");
		// had line fo9ani ghaalt : 3laxe ? hna makhasnixe njoine Req.getReqLine().getPath()
		// ghdi njone host m3a errorPage nixane
		ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[404]);
		ResPath = "";
		Fill_Response("302", "Moved Temporarily", 1, location);
	}
	
	send(clientSocket, response.c_str(), response.size(), 0);
	// close(clientSocket);
}
