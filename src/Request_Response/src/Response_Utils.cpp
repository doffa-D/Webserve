/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response_Utils.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:54:30 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/20 21:01:04 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

long	MaxBodySize(Request& Req)
{
	for(size_t i=0; i<Req.getHttp_Header().size(); i++)
	{
		if(Req.getHttp_Header()[i].first == "Content-Length")
			return atoi(Req.getHttp_Header()[i].second.c_str());// this return needs more checks, i think bad practice using atoi here
	}
	return 0; // in case not found Content-Length i return 0
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

std::string	ReadFile(std::string&	ResPath)
{
    std::ifstream file(ResPath.c_str());
    std::string line;
    std::string res;
	while (std::getline(file, line))
        res += line + "\n";
    return res;
}
