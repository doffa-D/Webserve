/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:17 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/09 20:09:44 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../../include/header.hpp"
#include <sys/stat.h>
#include <dirent.h> // include this headre for use DIR struct

#define FILE_PATH 1
#define REGULAR_STRING 0


class ResponseLine
{
	private:
		std::string HttpVersion;
		std::string Status_Code;
		std::string	Status_Message;
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
		std::string		_host;
		Request			Req;
		VecStringString	&track_cookie;
	public:
		int	ReqErr;
		std::vector<std::string> httpMethods;
		std::vector<std::string> httpVersions;
		// Response();
		Response(VecStringString& track_cookie);

		ResponseLine	getResLine() const;
		ResponseHeader	getResHeader() const;
		std::string		getResBody() const;
		std::string		getResPath() const;

		void	setResLine(ResponseLine resline);
		void	setResHeader(ResponseHeader resheader);
		void	setResBody(std::string resbody);
		void	setResPath(std::string respath);

		void	setReq(Request req);
		
		std::string	ft_Response(const Parser& parser);
		void		Fill_Response(std::string Stat_Code, std::string Stat_Msg, bool File_Or_Str, Location location);
		std::string	Error_HmlPage(const std::string& stat_code, const std::string& stat_msg);

		void		handleDirectoryRequest(const std::string& Root_ReqPath, const Location& location);
		void		handleFileRequest(const std::string& filePath, const Location& location);
		void 		handleNotFound(const Location& location);
		void		handleForbidden(const Location& location);
		void		handleBadRequest(const Location& location);
		
		~Response();

		bool	isMethodAllowed(const Location& location);
		bool	isRequestBodySizeAllowed(const Location& location);
		bool	isUriTooLong(const long& _value);
		bool    HttpVerNotSuported();
		void	handleMethodNotAllowed(const Location& location);
		void    handleHttpVerNotSuported(const Location& location);
		void	handleBodyTooLarge(const Location& location);
		void	handleUriTooLong(const Location& location);
		bool	serveRequestedResource(const std::string& Root_ReqPath, const Location& location);
		
		
		void	Check_CGI_Response(std::string Cgi_Response, int Cgi_Stat_Code, const Location& location);
		void	processCgiResponse(const std::string& Cgi_Response);
		void	handleErrorResponse(const Location& location, int errorCode, std::string errorMsg);
		void   	Upload(const Location& location);
		void	HandleDeletMethod(const std::string& Root_ReqPath, const Location& location);


		std::pair<std::string, Location> extractRequestInformation(const Parser& parser);
		bool		handleCommonRequestErrors(const Location& location);
		bool		handleRedirection(const Location& location);
		std::string	constructRootRequestPath(const Location& location, const std::string& LocationName);
};

std::string	 AutoIndex(std::string ResPath, std::string ReqPath);