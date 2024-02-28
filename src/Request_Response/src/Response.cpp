/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:14 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/28 21:53:01 by rrhnizar         ###   ########.fr       */
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
	httpMethods.push_back("GET");
	httpMethods.push_back("POST");
	httpMethods.push_back("DELETE");
	httpMethods.push_back("PUT");
	httpMethods.push_back("HEAD");
	httpMethods.push_back("OPTIONS");
	httpMethods.push_back("TRACE");
	httpMethods.push_back("CONNECT");
	httpMethods.push_back("PATCH");

	ReqErr = 0;

	// httpMethods = {"GET", "POST", "DELETE", "PUT", "HEAD", "OPTIONS", "TRACE", "CONNECT", "PATCH"};
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

void	Response::setReq(Request req)
{
	Req = req;
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
	{
		size_t pos = str_utils::r_find(ResPath, '.');
		ResHeader.setContentFile(ReadFile(ResPath));
		ResHeader.setContentType(location.getMimeTypeByKey(ResPath.substr(pos + 1)));
	}
	else
	{
		ResHeader.setContentFile(ResPath);
		ResHeader.setContentType("text/html");
	}
	ResHeader.setContentLength(str_utils::to_string(ResHeader.getContentFile().size()));
	
	response = ResLine.getHttpVersion() + " " + ResLine.getStatus_Code() + " " + ResLine.getStatus_Message() + "\r\n" +
                "Content-Type: " + ResHeader.getContentType() + "\r\n" +
                "Content-Length: " + ResHeader.getContentLength() + "\r\n" +
				"Location: " + ResHeader.getLocation() + "\r\n" +
                "\r\n" + ResHeader.getContentFile();
}

void	Response::handleDirectoryRequest(const std::string& Root_ReqPath, const Location& location)
{
	if(Req.getReqLine().getPath()[Req.getReqLine().getPath().size() - 1] != '/')
	{
		ResHeader.setLocation("http://" + _host + Req.getReqLine().getPath() + "/");
		ResPath = "";
		Fill_Response("301", "Moved Permanently", 1, location);
		return ;
	}
	ResPath = location.getIndexFilePathByRoot(Root_ReqPath); // this function kat9alb 3la file ila mal9atxe xi file kat3tina empty
	if(ResPath.empty() == 0)
	{
		Fill_Response("200", "OK", 0, location);
		return;
	}
	if(location.getAutoIndex() == 1)
	{
		ResPath = AutoIndex(Root_ReqPath, Req.getReqLine().getPath());
		Fill_Response("200", "OK", 1, location);
		return;
	}
	handleForbidden(location);
}

void Response::handleFileRequest(const std::string& filePath, const Location& location)
{
	// here i need to check if file to serve is CGI extension or no
	// if file is a CGI extension i need to serve with Download option

	MapStringString cgi = location.getCgi();

	size_t pos = str_utils::r_find(filePath, '.');
    string  extiension = filePath.substr(pos + 1); // get extension
	string bin = cgi[extiension];
    if (bin.empty())
	{
		ResPath = filePath;
		Fill_Response("200", "ok", 0, location);
        //normale way
	}
    else
	{
		std::map<std::string, std::string> env;
		env["SCRIPT_NAME"] = "ll.py"; //It will be the name of the file ???
		env["SCRIPT_FILENAME"] = "./ll.py"; //It will be the path of the file ??? 
		
		env["CONTENT_TYPE"] = Req.getHttp_Header()["CONTENT_TYPE"];
		env["REQUEST_METHOD"] = Req.getReqLine().getMethod();
		env["CONTENT_LENGTH"] = Req.getHttp_Header()["CONTENT_LENGTH"];
		env["QUERY_STRING"] = Req.getReqLine().getQuery_Params();//It will be empty in the POST and fill it in GET so you can put the form parameter just if you have it
		env["SERVER_PROTOCOL"] = Req.getReqLine().getHttpVersion();
		
		env["SERVER_NAME"] = "WebServer"; // ????
		env["REDIRECT_STATUS"] = "200"; // ????
	
		std::string body = Req.getBody();// it will be empty in GET !!!
		// std::string bin = "/usr/bin/python";
		CGI cgi_obj(body, env, bin);
		std::string respont = cgi_obj.fill_env();
		// std::cout <<  respont << std::endl;
		// here i need function that return respond of cgi
        //cgi way
	}
	// ResPath = filePath;
	// Fill_Response("200", "ok", 0, location);
}


