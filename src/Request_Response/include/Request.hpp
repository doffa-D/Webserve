/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 11:20:12 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/08 22:19:42 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "../../../include/header.hpp"
// #define AUTOINDEX (arg, arg2) ("<a href=\"" + d_names[i] + "\">" + d_names[i] + "</a>" +
//            "<span style=\"display: block; text-align: center;\">" + d_dates[i] + "</span>" +
//            "<span style=\"display: block; text-align: right;\">" + d_sizes[i] + "</span>" +
//        "</li>") 


class RequestLine
{
	private:
		std::string Method;
		std::string Path;
		std::string HttpVersion;
		std::string Query_Params;
	public:
		RequestLine();
		~RequestLine();
		std::string	getMethod() const;
		std::string	getPath() const;
		std::string	getHttpVersion() const;
		std::string	getQuery_Params() const;
		void		setMethod(std::string method);
		void		setPath(std::string path);
		void		setHttpVersion(std::string httpversion);
		void		Parse_ReqLine(std::string line);
};

class Request
{
	private:
		RequestLine ReqLine;
		std::map<std::string, std::string> Http_Header;
		std::string Body;
	public:
		bool BadRequest;
		std::vector<char> allowedCharacters;
		Request();
		~Request();
		std::map<std::string, std::string> getHttp_Header() const;
		void		setHttp_Header(std::map<std::string, std::string> http_header);
		RequestLine	getReqLine() const;
		std::string	getBody() const;
		void		Parse_Request(std::string& HttpRequest);
};