/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:17 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/05 14:46:37 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Request.hpp"
#include <fstream>
#include "../ConfigParsing/StringExtensions.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include "../ConfigParsing/Parser.hpp"
#include "../ConfigParsing/StringExtensions.hpp"
#include <sys/stat.h>
#include <iostream>
#include <dirent.h>



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
	public:	
		ResponseHeader();
		~ResponseHeader();
		std::string getContentType() const;
		std::string getContentLength() const;
		std::string getContentFile() const;

		void setContentType(std::string contenttype);
		void setContentLength(std::string contentlength);
		void setContentFile(std::string contentfile);
};

class Response
{
	private:
		ResponseLine 	ResLine;
		ResponseHeader	ResHeader;
		std::string		ResBody;
		std::string		ResPath;
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
		
		void	ft_Response(int clientSocket, Request& Req, Parser& parser);
		std::string	Fill_Response(std::string	Stat_Code, std::string	Stat_Msg, int File_Or_Str);
		std::string	Error_HmlPage(const std::string& stat_code, const std::string& stat_msg);
		Location	Find_Location(Parser& parser, std::string& _host, std::string Path_Req);

		// std::string ReadFile();
		~Response();
};