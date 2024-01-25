/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:12 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/25 15:09:41 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <sstream>


class RequestLine
{
	private:
		std::string Method;
		std::string Path;
		std::string HttpVersion;
	public:
		RequestLine();
		~RequestLine();
		std::string	getMethod() const;
		std::string	getPath() const;
		std::string	getHttpVersion() const;
		void	setMethod(std::string method);
		void	setPath(std::string path);
		void	setHttpVersion(std::string httpversion);
		void	Parse_ReqLine(std::string line);
		void	PrintReqLine();
};

class Request
{
	private:
		bool		Referer;
		RequestLine ReqLine;
		std::vector<std::pair<std::string, std::string> > Http_Header;
	public:
		Request();
		~Request();
		int		getReferer() const;
		void	settReferer(int referer);
		std::vector<std::pair<std::string, std::string> > getHttp_Header() const;
		void	setHttp_Header(std::vector<std::pair<std::string, std::string> > http_header);
		RequestLine	getReqLine() const;
		void	Parse_Request(std::string& HttpRequest);
		void	CheckReferer();
		void	PrintHttp_Header();
		// void	Response(int clientSocket);
};