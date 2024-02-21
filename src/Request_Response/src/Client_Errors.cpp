/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client_Errors.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:47:24 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/21 11:48:51 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

void Response::sendMethodNotAllowedResponse(const Location& location)
{
    if (location.getErrorPages()[405].empty())
	{
        ResPath = Error_HmlPage("405", "Method Not Allowed");
        Fill_Response("405", "Method Not Allowed", 1, location);
    }
	else
	{
        ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[405]);
        ResPath = "";
        Fill_Response("302", "Moved Temporarily", 1, location);
    }
}

void Response::sendRequestBodyTooLargeResponse(const Location& location)
{
    if (location.getErrorPages()[413].empty())
	{
        ResPath = Error_HmlPage("413", "Content Too Large");
        Fill_Response("413", "Content Too Large", 1, location);
    } 
	else
	{
        ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[413]);
        ResPath = "";
        Fill_Response("302", "Moved Temporarily", 1, location);
    }
}

void Response::handleForbiden(const Request& Req, const Location& location)
{
	if(location.getErrorPages()[403].empty() == 1)
	{
		ResPath = Error_HmlPage("403", "Forbiden");
		Fill_Response("403", "Forbiden", 1, location);
	}
	else
	{
		ResHeader.setLocation("http://" + _host +  Req.getReqLine().getPath() + location.getErrorPages()[403]);
		ResPath = "";
		Fill_Response("302", "Moved Temporarily", 1, location);
	}
}

void Response::handleNotFound(Location& location)
{
	if(location.getErrorPages()[404].empty() == 1)
	{
		ResPath = Error_HmlPage("404", "Not Found");
		Fill_Response("404", "Not Found", 1, location);
	}
	else
	{
		// ResHeader.setLocation("http://" + _host + Req.getReqLine().getPath() + "/");
		// had line fo9ani ghaalt : 3laxe ? hna makhasnixe njoine Req.getReqLine().getPath()
		// ghdi njone host m3a errorPage nixane
		ResHeader.setLocation("http://" + _host + "/" + location.getErrorPages()[404]);
		ResPath = "";
		Fill_Response("302", "Moved Temporarily", 1, location);
	}
}