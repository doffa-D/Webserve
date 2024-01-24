/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:56:53 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/24 12:34:28 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() : CommonDirectives("/", "index.html", "", "", false, (1024 * 1024), "./logs/error_log.log", "./logs/access_log.log") {}
Server::~Server() {}
Server::Server(const Server& _copy) : CommonDirectives(_copy) {*this = _copy;}
Server& Server::operator=(const Server& _assignment)
{
	if (this != &_assignment)
	{
		server_name = _assignment.server_name;
		host = _assignment.host;
		ports = _assignment.ports;
		locations = _assignment.locations;
		CommonDirectives::operator=(_assignment);
	}
	return (*this);
}

/*==============>SETTERES<================*/
void	Server::setServerName(const string& _server_name) {server_name = _server_name;}
void	Server::setHost(const string& _host) {host = _host;}

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
const string&			Server::getServerName() const {return (server_name);}
const string&			Server::getHost() const {return (host);}
VecInt					Server::getPorts() const {return (ports);}
Locations				Server::getLocations() const {return (locations);}

Location			Server::getLocationByPath(const string& _path)
{
	Location l;
	l.setRoot("NULL");
	Locations::iterator it = locations.begin();
	for (;it != locations.end();it++)
		if (it->first == _path)
			return (it->second);
	return (l);
}
