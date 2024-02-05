/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:14 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/05 15:51:29 by rrhnizar         ###   ########.fr       */
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
	// std::string	Error_Page = "error " + stat_code + "  " + stat_msg + " \n";
	std::string	Error_Page = "<html>\r\n<head><title>" + stat_code + "  " + stat_msg + "</title></head>\r\n" +
							"<body>\r\n<center><h1>" +  stat_code + "  " + stat_msg + "</h1></center>\r\n" +
							"<hr><center> Fake nginx </center></body></html>";


	// "<html>" CRLF
	// "<head><title>403 Forbidden</title></head>" CRLF
	// "<body>" CRLF
	// "<center><h1>403 Forbidden</h1></center>" CRLF;
	return Error_Page;
}

std::string	ReadFile(std::string&	ResPath)
{
	// if(ResPath[ResPath.size() - 1] == '/')
	// 	ResPath = ResPath.substr(0, ResPath.size() - 1);
    std::ifstream file(ResPath);
    std::string line;
    std::string res;
	while (std::getline(file, line))
        res += line + "\n";
    return res;
}

std::string	Response::Fill_Response(std::string	Stat_Code, std::string	Stat_Msg, int File_Or_Str)
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

std::string Directories_names(std::vector<std::string> d_names)
{
	std::string str = "";
	for(size_t i=0; i<d_names.size(); i++)
	{
		std::cout << "heeeere\n";
		str += "<li><a href=\"" + d_names[i] + "\">" + d_names[i] + "</a></li>\n";
	}
	std::cout << "["  << str << "]" << std::endl;
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
            std::cout << entry->d_name << std::endl;
        }

        // Close the directory
        closedir(directory);
    } else {
        // Handle error opening directory
        std::cerr << "Error opening directory." << std::endl;
        // return 1;
    }


	std::cout << "ReqPath ========================>       " << ResPath << std::endl;
	std::string f1 = "./file1.html";
	std::string f2 = "./file2.html";
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
        // "        <li><a href=\"" + f1 + "\">" + f1 + "</a></li>\n"
        // "        <li><a href=\"" + f2 + "\">" + f2 + "</a></li>\n"
        // "        <li><a href=\"subdirectory/\">subdirectory/</a></li>\n"
        "        <!-- Add more entries as needed -->\n"
        "    </ul>\n"
        "    <hr>\n"
        "</body>\n"
        "</html>";

	return str;
}

void	Response::ft_Response(int clientSocket, Request& Req, Parser& parser)
{
	std::string	_host;
	Location	location;
	std::string	Root_ReqPath;

	for(size_t i=0; i<Req.getHttp_Header().size(); i++)
	{
		if (Req.getHttp_Header().at(i).first == "Host")
		{
			_host = Req.getHttp_Header().at(i).second;
			break;
		}
	}
	location = Find_Location(parser, _host, Req.getReqLine().getPath());
	Root_ReqPath = location.getRoot() + Req.getReqLine().getPath();
	setResPath(Root_ReqPath);
	
	

	std::ifstream File(Root_ReqPath.c_str());
	std::string response;
	if(File.is_open())
	{
		struct stat fileInfo;
		if (stat(Root_ReqPath.c_str(), &fileInfo) == 0)
		{
			if (S_ISDIR(fileInfo.st_mode))
            {
				if(Req.getReqLine().getPath()[Req.getReqLine().getPath().size() - 1] != '/')
				{
					response = "HTTP/1.1 301 Moved Permanently\r\nContent-Type: text/html\r\nContent-Length: 0\r\nLocation: http://"
							 	+ _host + Req.getReqLine().getPath() + "/\r\n\r\n";
					// std::cout << "Response =   \n" << response << std::endl;
					send(clientSocket, response.c_str(), response.size(), 0);
					close(clientSocket);
					return ;
				}
				ResPath = Root_ReqPath + "index.html";
				std::ifstream FileIndex(ResPath.c_str());
				if(FileIndex.is_open())
				{
					response = Fill_Response("200", "OK", 0);
					//servi file 
				}
				else if(location.getAutoIndex() == 1)
				{
					ResPath = AutoIndex(Root_ReqPath, Req.getReqLine().getPath());
					response = Fill_Response("200", "OK", 1);
					//check auto index 
				}
				else
				{
					ResPath = Error_HmlPage("403", "Forbiden");
					response = Fill_Response("403", "Forbiden", 1);
					// Forbiden 403
				}
				std::cout << "It is a directory." << std::endl;
			}
			else if (S_ISREG(fileInfo.st_mode))
            {
				ResPath = Root_ReqPath;
                std::cout << "It is a regular file." << std::endl;
            }
		}
	}
	else
	{
		std::cout << "404 Not Found ......." << std::endl;
		ResPath = Error_HmlPage("404", "Not Found");
		response = Fill_Response("404", "Not Found", 1);
	}

	// response = Fill_Response();
		
	// std::cout << "Res_Path ==>  " << ResPath << std::endl;
	
	// std::cout << "Response =   \n" << response << std::endl;
	send(clientSocket, response.c_str(), response.size(), 0);
	close(clientSocket);
}




// 127.0.0.1       localhost
// 127.0.0.1       login.42.fr