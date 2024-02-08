/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:56:53 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/08 18:17:56 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Server.hpp"
#include "../../../include/header.hpp"


Server::Server() : CommonDirectives("./", "index.html", "", false, (1024 * 1024), ERROR_LOG, ACCESS_LOG, UPLOAD), ip_address(str_utils::ip(127,0,0,1)), nullObject(false) {}
Server::~Server() {}
Server::Server(const Server& _copy) : CommonDirectives(_copy) {*this = _copy;}
Server& Server::operator=(const Server& _assignment)
{
	if (this != &_assignment)
	{
		server_name = _assignment.server_name;
		ip_address = _assignment.ip_address;
		ports = _assignment.ports;
		locations = _assignment.locations;
		nullObject = _assignment.nullObject;
		CommonDirectives::operator=(_assignment);
	}
	return (*this);
}

Server Server::createNullObject()
{
	Server s;
	s.nullObject = true;
	return (s);
}

bool			Server::isNull()
{
	return (nullObject);
}

/*==============>SETTERES<================*/
void	Server::setServerName(const string& _server_name) {server_name = _server_name;}
void	Server::setIpAddress(const string& _ip_address)
{
	if (_ip_address == "localhost")
		ip_address = str_utils::ip(127, 0, 0, 1);
	else
	{
		VecString split = str_utils::split(_ip_address, '.');
		VecString_iter it = split.begin();
		if (split.size() != 4)
			throw CustomException("host not found in \"" + _ip_address + "\" of the \"host\" directive");
		for (; it != split.end(); it++)
		{
			char* endp;
			double ret = strtod(it->c_str(), &endp);
			string string_ret = endp;
			if (string_ret != "" || ret < 0 || ret > 255)
				throw CustomException("host not found in \"" + _ip_address + "\" of the \"host\" directive");
		}
		ip_address = str_utils::ip(str_utils::to_int(split[0]),
							 str_utils::to_int(split[1]),
							 str_utils::to_int(split[2]),
							 str_utils::to_int(split[3]));
	}		
}

bool	Server::isPorteAlreadyExists(int _porte)
{
	VecInt_iter it = ports.begin();
	for (;it != ports.end();it++)
		if (*it == _porte)
			return (true);
	return (false);
}

bool	Server::AddPort(const string& _porte)
{
	VecString split = str_utils::ultimatSplit(_porte, "\t ");
	if (_porte.empty() || split.size() > 2)
		throw CustomException("Directive have invalid number of arguments", "listen");
	char* endp;
	double ret = strtod(split[0].c_str(), &endp);
	string string_ret = endp;
	if (string_ret != "")
		throw CustomException("host not found of the \"listen\" directive", split[0]);
	if (isPorteAlreadyExists(ret))
		throw CustomException("a duplicate listen", str_utils::to_string(ret));
	if (ret < 0 || ret > 65536)
		throw CustomException("invalid port ", str_utils::to_string(ret));
	ports.push_back(ret);
	if (split.size() == 2)
	{
		if(split[1] != "default_server")
			throw CustomException("invalid parameter for listen", split[1]);
		return (true);
	}
	return (false);
}

bool	Server::isLocationAlreadyExists(const string& _path)
{
	Locations::iterator it = locations.begin();
	for (;it != locations.end();it++)
		if (it->first == _path)
			return (true);
	return (false);
}

void	Server::AddLocation(const string& path, const Location& _location)
{
	if (isLocationAlreadyExists(path))
		throw CustomException("duplicate location", path);
	locations.push_back(make_pair(path, _location));
}

/*==============>GETTERES<================*/
VecString				Server::getServerNames() const 
{
	VecString out = str_utils::proSplit(server_name);
	for (VecString_iter it = out.begin(); it != out.end();it++)
		*it = str_utils::remove_quotes(*it);
	return (out);
}

Uint					Server::getIpAddress() const 
{
	return (ip_address);
}

VecInt					Server::getPorts() const 
{
	VecInt v;
	v.push_back(80);
	if (ports.size() == 0)
		return (v);
	return (ports);
}

Locations				Server::getLocations() const {return (locations);}
Location				Server::getLocationByPath(const string& _path)
{
	Location l(*this);
	string tmp = _path;
	while (tmp.size())
	{
		Locations::iterator it = locations.begin();
		for (;it != locations.end();it++)
			if (it->first == tmp)
				return (it->second);
		tmp = tmp.substr(0, str_utils::r_find(tmp, '/'));
	}
	return (l);
}
