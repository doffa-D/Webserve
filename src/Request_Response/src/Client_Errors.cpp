/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_Errors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:47:24 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/21 13:31:57 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

void Response::handleErrorResponse(const Location& location, int errorCode, std::string errorMsg)
{
	if (location.getErrorPages()[errorCode].empty())
	{
        ResPath = Error_HmlPage(std::to_string(errorCode), errorMsg);
        Fill_Response(std::to_string(errorCode), errorMsg, 1, location);
    }
	else
	{
        ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[errorCode]);
		if(errorCode == 403)
			ResHeader.setLocation("http://" + _host +  Req.getReqLine().getPath() + location.getErrorPages()[errorCode]);
        ResPath = "";
        Fill_Response("302", "Moved Temporarily", 1, location);
    }
}


void Response::handleMethodNotAllowed(const Location& location)
{
	handleErrorResponse(location, 405, "Method Not Allowed");
}

void Response::handleBodyTooLarge(const Location& location)
{
	handleErrorResponse(location, 413, "Content Too Large");
}

void Response::handleForbidden(const Location& location)
{
	handleErrorResponse(location, 403, "Forbidden");
}

void Response::handleNotFound(Location& location)
{
	handleErrorResponse(location, 404, "Not Found");
}