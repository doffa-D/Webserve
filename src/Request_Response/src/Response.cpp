/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:14 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/05 23:37:48 by rrhnizar         ###   ########.fr       */
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
ResponseHeader::ResponseHeader():ContentType(""), ContentLength(""), ContentFile(""), Location(""), ContentDisposition("")
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


void	ResponseHeader::setContentDisposition(std::string content_disposition)
{
	ContentDisposition = content_disposition;
}


std::string	ResponseHeader::getContentDisposition() const
{
	return ContentDisposition;
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

void	Response::Fill_Response(std::string	Stat_Code, std::string	Stat_Msg, int File_Or_Str, int isCgi, Location location)
{
	ResLine.setHttpVersion("HTTP/1.1");
	ResLine.setStatus_Code(Stat_Code);
	ResLine.setStatus_Message(Stat_Msg);


	if(File_Or_Str == 0)
	{
		size_t pos = str_utils::r_find(ResPath, '.');
		ResHeader.setContentFile(ReadFile(ResPath));
		ResHeader.setContentType(location.getMimeTypeByKey(ResPath.substr(pos + 1)));
		
		if(isCgi == 1)
		{
			size_t  pos = str_utils::r_find(ResPath, '/');
			std::string FileName = ResPath.substr(pos + 1);
			ResHeader.setContentDisposition("attachment; filename=\"" + FileName + "\"");
		}
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
				"Content-Disposition: " + ResHeader.getContentDisposition() + "\r\n" +
				"Location: " + ResHeader.getLocation() + "\r\n" +
                "\r\n" + ResHeader.getContentFile();
}

void	Response::handleDirectoryRequest(const std::string& Root_ReqPath, const Location& location)
{
	if(Req.getReqLine().getPath()[Req.getReqLine().getPath().size() - 1] != '/')
	{
		ResHeader.setLocation("http://" + _host + Req.getReqLine().getPath() + "/");
		ResPath = "";
		Fill_Response("301", "Moved Permanently", 1, 0, location);
		return ;
	}
	
	// std::cout << "Root_ReqPath =  " << Root_ReqPath << std::endl;
	ResPath = location.getIndexFilePathByRoot(Root_ReqPath); // this function kat9alb 3la file ila mal9atxe xi file kat3tina empty
	// std::cout << "ResPath = " << ResPath <<  std::endl;
	if(ResPath.empty() == 0)
	{
		Fill_Response("200", "OK", 0, 0, location);
		return;
	}
	if(location.getAutoIndex() == 1)
	{
		ResPath = AutoIndex(Root_ReqPath, Req.getReqLine().getPath());
		Fill_Response("200", "OK", 1, 0, location);
		return;
	}
	handleForbidden(location);
}

void	Response::processCgiResponse(const std::string& Cgi_Response)
{
	std::map<string, string> Cgi_Header;
	std::istringstream ss(Cgi_Response);
	std::string line;
	bool isBody = false;
	while (std::getline(ss, line))
	{
		line += "\n";
		if(isBody)
		{
			ResPath += line;
			continue;
		}
		size_t pos = line.find(':');
		if (pos != std::string::npos)
		{
			std::string key = line.substr(0, pos);
    	    std::string value = line.substr(pos + 1);
			// Storing at cgimap
			Cgi_Header[key] = value;
		}
		if(line == "\r\n")
			isBody = true;
	}
	response = "HTTP/1.1 200 OK\r\n";
	if(!Cgi_Header["Content-Type"].empty())
		ResHeader.setContentType(Cgi_Header["Content-Type"]);
	else
		ResHeader.setContentType("text/html\r\n");
    response += "Content-Type: " + ResHeader.getContentType();
	if(!Cgi_Header["Content-Length"].empty())
	{
		ResHeader.setContentLength(Cgi_Header["Content-Length"]);
		response += "Content-Length: " + ResHeader.getContentLength();
	}
	if(!Cgi_Header["Content-Disposition"].empty())
	{
		ResHeader.setContentLength(Cgi_Header["Content-Disposition"]);
		response += "Content-Disposition: " + ResHeader.getContentDisposition();
	}
	if(!Cgi_Header["Location"].empty())
	{
		ResHeader.setContentLength(Cgi_Header["Location"]);
		response += "Location: " + ResHeader.getLocation();
	}
    response += "\r\n" + ResPath;
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
			Fill_Response("200", "OK", 1, 0, location);
		}
		return;
	}
	if(Cgi_Stat_Code == -1) // sys call faild
	{
		ResPath = Error_HmlPage("500", "Internal Server Error");
		Fill_Response("500", "Internal Server Error", 1, 0, location);
		return ;
	}
	if(Cgi_Stat_Code == -5) // Gateway Timeout
	{
		ResPath = Error_HmlPage("504", "Gateway Timeout");
		Fill_Response("504", "Gateway Timeout", 1, 0, location);
		return ;
	}
	else
	{
		ResPath = Error_HmlPage("502", "Bad Gateway");
		Fill_Response("502", "Bad Gateway", 1, 0, location);
		return ;
	}
}


void Response::handleFileRequest(const std::string& filePath, const Location& location)
{
	size_t pos = str_utils::r_find(filePath, '.');
    string  extension = filePath.substr(pos + 1); // get extension
	string bin = location.getCgi()[extension];
    if (bin.empty())
	{
		ResPath = filePath;
		// here i need respond with download file requested if method is post and file is backend file 
		// here i need add this check ==> && ALLOWED_EXTENSION.find("Extension of file serve")
		// but this method to storage the ALLOWED_EXTENSION is prevents me from search on Extension
		// i need add check because i serve CGI file with download this file
		// But you must download only the backend file such as (py, php, c, c++...)
		VecString	vec = str_utils::split(ALLOWED_EXTENSION, ' ');
 		if(Req.getReqLine().getMethod() == "POST" &&
		   find(vec.begin(), vec.end(), extension) != vec.end()) // this condition needs more checks 
			Fill_Response("200", "ok", 0, 1, location);
		else
			Fill_Response("200", "ok", 0, 0, location);
        //normale way
	}
    else
	{
		// std::cout << "filePath = " << filePath <<std::endl;
		size_t _pos = str_utils::r_find(filePath, '/');
		// std::cout << "filePath.substr(_pos + 1)  " << filePath.substr(_pos + 1) << std::endl;
		std::map<std::string, std::string> env;
		env["SCRIPT_NAME"] = filePath.substr(_pos + 1); //It will be the name of the file ???
		env["SCRIPT_FILENAME"] = filePath; //It will be the path of the file ??? 

		env["CONTENT_TYPE"] = Req.getHttp_Header()["Content-Type"];
		env["REQUEST_METHOD"] = Req.getReqLine().getMethod();
		env["CONTENT_LENGTH"] = Req.getHttp_Header()["Content-Length"];
		env["QUERY_STRING"] = Req.getReqLine().getQuery_Params();//It will be empty in the POST and fill it in GET so you can put the form parameter just if you have it
		env["SERVER_PROTOCOL"] = Req.getReqLine().getHttpVersion();
		
		env["SERVER_NAME"] = "WebServer"; // ????
		env["REDIRECT_STATUS"] = "200"; // ????
	
		std::string body = Req.getBody();// it will be empty in GET !!!
		CGI cgi_obj(body, env, bin);
		std::pair<std::string, int> respont = cgi_obj.fill_env();
		Check_CGI_Response(respont.first, respont.second, location);
	}
}



//defining when  to handle upload the file yourself, and when to handle this with CGI