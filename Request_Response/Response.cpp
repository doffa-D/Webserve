/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:14 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/05 23:43:18 by rrhnizar         ###   ########.fr       */
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
	std::string	Error_Page = "<html>\r\n<head><title>" + stat_code + "  " + stat_msg + "</title></head>\r\n" +
							"<body>\r\n<center><h1>" +  stat_code + "  " + stat_msg + "</h1></center>\r\n" +
							"<hr><center> Fake nginx </center></body></html>";
	return Error_Page;
}

std::string	ReadFile(std::string&	ResPath)
{
    std::ifstream file(ResPath);
    std::string line;
    std::string res;
	while (std::getline(file, line))
        res += line + "\n";
    return res;
}

std::string	Response::Fill_Response(std::string	Stat_Code, std::string	Stat_Msg, int File_Or_Str, Location location)
{
	ResponseLine	Resline;
	ResLine.setHttpVersion("HTTP/1.1");
	ResLine.setStatus_Code(Stat_Code);
	ResLine.setStatus_Message(Stat_Msg);
	
	ResponseHeader	ResHeader;
	if(File_Or_Str == 0)
		ResHeader.setContentFile(ReadFile(ResPath));
	else
		ResHeader.setContentFile(ResPath);
	ResHeader.setContentLength(str_utils::to_string(ResHeader.getContentFile().size()));
	size_t pos = ResPath.find('.');
	ResHeader.setContentType(location.getMimeTypeByKey(ResPath.substr(pos + 1)));

	
	// ResHeader.setContentType();

	std::string	response = ResLine.getHttpVersion() + " " + ResLine.getStatus_Code() + " " + ResLine.getStatus_Message() + "\r\n" +
                "Content-Type: " + ResHeader.getContentType() + "\r\n" +
                "Content-Length: " + ResHeader.getContentLength() + "\r\n" +
                "\r\n" + ResHeader.getContentFile();
    return response;
}

std::string Directories_names(std::vector<std::string> d_names)
{
	std::string str = "";
	for(size_t i=0; i<d_names.size(); i++)
		str += "<li><a href=\"" + d_names[i] + "\">" + d_names[i] + "</a></li>\n";
	return str;
}

std::string	 AutoIndex(std::string ResPath, std::string ReqPath)
{
	std::vector<std::string> d_names;
	DIR *directory;
    struct dirent *entry;

    // Open the current directory (replace "." with the path to your desired directory)
    if ((directory = opendir(ResPath.c_str())) != NULL) {
        // Read each entry in the directory
        while ((entry = readdir(directory)) != NULL) {
			d_names.push_back(entry->d_name);
        }

        // Close the directory
        closedir(directory);
    } else {
        // Handle error opening directory
        std::cerr << "Error opening directory." << std::endl;
        // return 1;
    }

	std::string str = 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Index of " + ReqPath + "</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Index of " + ReqPath + "</h1>\n"
        "    <hr>\n"
        "    <ul>\n"
        "        <!-- Directory listing will be generated here dynamically -->\n"
        "        <!-- Each list item should represent a file or subdirectory -->\n"
        "        <!-- You can use server-side scripting or tools to automate this process -->\n"
			+ 		Directories_names(d_names) +
        "        <!-- Add more entries as needed -->\n"
        "    </ul>\n"
        "    <hr>\n"
        "</body>\n"
        "</html>";
	return str;
}

std::string	findHostFromHeaders(const Request& Req)
{
	std::string	_host;
	for(size_t i=0; i<Req.getHttp_Header().size(); i++)
	{
		if (Req.getHttp_Header().at(i).first == "Host")
		{
			_host = Req.getHttp_Header().at(i).second;
			break;
		}
	}
	return _host;
}

void	Response::handleDirectoryRequest(int clientSocket, const Request& Req, const std::string& _host, const std::string& Root_ReqPath, const Location& location)
{
	if(Req.getReqLine().getPath()[Req.getReqLine().getPath().size() - 1] != '/')
	{
		response = "HTTP/1.1 301 Moved Permanently\r\nContent-Type: text/html\r\nContent-Length: 0\r\nLocation: http://"
				 	+ _host + Req.getReqLine().getPath() + "/\r\n\r\n";
		send(clientSocket, response.c_str(), response.size(), 0);
		close(clientSocket);
		return ;
	}
	ResPath = Root_ReqPath + "index.html";
	std::ifstream FileIndex(ResPath.c_str());
	if(FileIndex.is_open())
	{
		response = Fill_Response("200", "OK", 0, location);
		//servi file 
	}
	else if(location.getAutoIndex() == 1)
	{
		ResPath = AutoIndex(Root_ReqPath, Req.getReqLine().getPath());
		response = Fill_Response("200", "OK", 1, location);
		//check auto index 
	}
	else
	{
		ResPath = Error_HmlPage("403", "Forbiden");
		response = Fill_Response("403", "Forbiden", 1, location);
		// Forbiden 403
	}
	send(clientSocket, response.c_str(), response.size(), 0);
	close(clientSocket);
}

void Response::handleFileRequest(int clientSocket, const std::string& filePath, Location& location)
{
	ResPath = filePath;
	response = Fill_Response("200", "ok", 0, location);
	send(clientSocket, response.c_str(), response.size(), 0);
	close(clientSocket);
}

void Response::handleNotFound(int clientSocket, Location& location)
{
	ResPath = Error_HmlPage("404", "Not Found");
	response = Fill_Response("404", "Not Found", 1, location);
	send(clientSocket, response.c_str(), response.size(), 0);
	close(clientSocket);
}

void	Response::ft_Response(int clientSocket, Request& Req, Parser& parser)
{
	std::string	_host;
	Location	location;
	std::string	Root_ReqPath;
	Server		server;


	_host = findHostFromHeaders(Req);
	server = parser.getServerbyHost(_host);
	location = server.getLocationByPath(Req.getReqLine().getPath());

	
	Root_ReqPath = location.getRoot() + Req.getReqLine().getPath();
	setResPath(Root_ReqPath);
	std::ifstream File(Root_ReqPath.c_str());
	if(File.is_open())
	{
		struct stat fileInfo;
		if (stat(Root_ReqPath.c_str(), &fileInfo) == 0)
		{
			if (S_ISDIR(fileInfo.st_mode))
				handleDirectoryRequest(clientSocket, Req, _host, Root_ReqPath, location);
			else if (S_ISREG(fileInfo.st_mode))
				handleFileRequest(clientSocket, Root_ReqPath, location);
		}
	}
	else
		handleNotFound(clientSocket, location);
}
