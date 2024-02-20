/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:17 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/20 21:37:18 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../../include/header.hpp"
#include <sys/stat.h>
#include <dirent.h> // include this headre for use DIR struct


class ResponseLine
{
	private:
		std::string HttpVersion;
		std::string Status_Code;
		std::string	Status_Message;
		// std::string Eroor_Page;
	public:
		ResponseLine();
		~ResponseLine();
		std::string getHttpVersion() const;
		std::string getStatus_Code() const;
		std::string getStatus_Message() const;

		void setHttpVersion(std::string htttpversion);
		void setStatus_Code(std::string status_code);
		void setStatus_Message(std::string status_message);
};

class ResponseHeader
{
	private:
		std::string	ContentType;
		std::string ContentLength;
		std::string	ContentFile;
		std::string	Location;
	public:	
		ResponseHeader();
		~ResponseHeader();
		std::string getContentType() const;
		std::string getContentLength() const;
		std::string getContentFile() const;
		std::string	getLocation() const;

		void 	setContentType(std::string contenttype);
		void 	setContentLength(std::string contentlength);
		void	setContentFile(std::string contentfile);
		void	setLocation(std::string	location);
};

class Response
{
	private:
		ResponseLine 	ResLine;
		ResponseHeader	ResHeader;
		std::string		ResBody;
		std::string		ResPath;
		std::string		response;
	public:
		Response();
		ResponseLine	getResLine() const;
		ResponseHeader	getResHeader() const;
		std::string		getResBody() const;
		std::string		getResPath() const;

		void	setResLine(ResponseLine resline);
		void	setResHeader(ResponseHeader resheader);
		void	setResBody(std::string resbody);
		void	setResPath(std::string respath);
		
		void	ft_Response(int clientSocket, Request& Req, const Parser& parser);
		// std::string	Fill_Response(std::string	Stat_Code, std::string	Stat_Msg, int File_Or_Str, Location location);
		void	Fill_Response(std::string	Stat_Code, std::string	Stat_Msg, int File_Or_Str, Location location);
		std::string	Error_HmlPage(const std::string& stat_code, const std::string& stat_msg);
		// Location	Find_Location(Parser& parser, std::string& _host, std::string Path_Req);

		void		handleDirectoryRequest(int clientSocket, const Request& Req, const std::string& _host, const std::string& Root_ReqPath, const Location& location);
		void		handleFileRequest(int clientSocket, const std::string& filePath, const Location& location);
		void 		handleNotFound(int clientSocket, Location& location, const std::string& _host);
		// std::string ReadFile();
		~Response();

		// organization
		bool isMethodAllowed(const Location& location, const Request& Req);
		bool isRequestBodySizeAllowed(const Location& location, const Request& Req);
		std::string constructAbsolutePath(const Location& location, const Request& Req);
		void sendMethodNotAllowedResponse(int clientSocket, const Location& location, const std::string& _host);
		void sendRequestBodyTooLargeResponse(int clientSocket, const Location& location, const std::string& _host);
		bool serveRequestedResource(int clientSocket, const Request& Req, const std::string& Root_ReqPath, const Location& location, const std::string& _host);
		long		MaxBodySize(const Request& Req);
		std::string	findHostFromHeaders(const Request& Req);
		std::string	ReadFile(std::string&	ResPath);
		
		
		
};

long	MaxBodySize(Request& Req);
std::string	findHostFromHeaders(const Request& Req);
std::string	ReadFile(std::string&	ResPath);
std::string	 AutoIndex(std::string ResPath, std::string ReqPath);