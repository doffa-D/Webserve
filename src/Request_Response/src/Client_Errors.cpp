/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_Errors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:47:24 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/10 13:25:10 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

void Response::handleErrorResponse(const Location& location, int errorCode, std::string errorMsg)
{
	if (location.getErrorPages()[errorCode].empty())
	{
        ResPath = Error_HmlPage(std::to_string(errorCode), errorMsg);
        Fill_Response(std::to_string(errorCode), errorMsg, REGULAR_STRING, location);
    }
	else
	{
        ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[errorCode]);
		if(errorCode == 403)
			ResHeader.setLocation("http://" + _host +  Req.getReqLine().getPath() + location.getErrorPages()[errorCode]);
        ResPath = "";
        Fill_Response("302", "Moved Temporarily", REGULAR_STRING, location);
    }
}

void    Response::handleHttpVerNotSuported(const Location& location)
{
	handleErrorResponse(location, 505, "HTTP Version Not Supported");
}

void    Response::handleBadRequest(const Location& location)
{
	std::string method = Req.getReqLine().getMethod();
    if(std::find(httpMethods.begin(), httpMethods.end(), method) == httpMethods.end())
    {
		handleErrorResponse(location, 400, "Bad Request");
		ReqErr = 1;
        return;
    }
	if(Req.getReqLine().getHttpVersion() != "HTTP/1.1")
	{
		handleErrorResponse(location, 400, "Bad Request");
		ReqErr = 1;
        return;
	}
	if(Req.BadRequest == 1)
	{
		handleErrorResponse(location, 400, "Bad Request");
		ReqErr = 1;
        return;
	}
}

void Response::handleMethodNotAllowed(const Location& location)
{
	handleErrorResponse(location, 405, "Method Not Allowed");
}

void	Response::handleUriTooLong(const Location& location)
{
	handleErrorResponse(location, 414, "URI Too Long");
}

void Response::handleBodyTooLarge(const Location& location)
{
	handleErrorResponse(location, 413, "Content Too Large");
}

void Response::handleForbidden(const Location& location)
{
	Req.logging(location.getErrorLog(), 1, "Forbidden");
	handleErrorResponse(location, 403, "Forbidden");
}

void Response::handleNotFound(const Location& location)
{
	Req.logging(location.getErrorLog(), 1, "Not Found");
	handleErrorResponse(location, 404, "Not Found");
}
