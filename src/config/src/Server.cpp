/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:56:53 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/15 12:09:32 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Server.hpp"
#include "../../../include/header.hpp"


Server::Server() : CommonDirectives("./", false, (1024 * 1024), ERROR_LOG, ACCESS_LOG, UPLOAD), default_server(false), nullObject(false) {}
Server::~Server() {}
Server::Server(const Server& _copy) : CommonDirectives(_copy) {*this = _copy;}
Server& Server::operator=(const Server& _assignment)
{
	if (this != &_assignment)
	{
		server_name = _assignment.server_name;
		ip_ports = _assignment.ip_ports;
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
void	Server::setAsDefaultServer() {default_server = true;}

Uint	Server::getIp(const string& _value)
{
	Uint _ip;
	if (_value == "localhost")
		_ip = str_utils::ip(127, 0, 0, 1);
	else
	{
		VecString split = str_utils::split(_value, '.');
		VecString_iter it = split.begin();
		if (split.size() != 4)
			throw CustomException("host not found in \"" + _value + "\" of the \"listen\" directive");
		for (; it != split.end(); it++)
		{
			char* endp;
			double ret = strtod(it->c_str(), &endp);
			string string_ret = endp;
			if (string_ret != "" || ret < 0 || ret > 255)
				throw CustomException("host not found in \"" + _value + "\" of the \"listen\" directive");
		}
		_ip = str_utils::ip(str_utils::to_int(split[0]),
							str_utils::to_int(split[1]),
							str_utils::to_int(split[2]),
							str_utils::to_int(split[3]));
	}
	return (_ip);
}

int		Server::getPort(const string& _value)
{
	char* endp;
	double	_port = strtod(_value.c_str(), &endp);
	string string_ret = endp;
	if (_port < 0 || _port > 65536)
		throw CustomException("invalid port ", _value);
	if (string_ret != "")
		throw CustomException("is invalid port for the \"listen\" directive ", _value);
	return (_port);
}

bool	Server::isIpPortAlreadyExists(Uint _ip, int _port)
{
	IpPorts::iterator it = ip_ports.begin();
	for (;it != ip_ports.end();it++)
		if (it->first == _ip && it->second == _port)
			return (true);
	return (false);
}

bool	Server::AddIpPort(const string& _ip_port)
{
	VecString split = str_utils::ultimatSplit(_ip_port, "\t ");
	if (_ip_port.empty() || split.size() > 2)
		throw CustomException("Directive have invalid number of arguments", "listen");
	
	VecString	split2 = str_utils::split(split[0], ':');
	Uint		_ip = str_utils::ip(DEFAULT_IP);
	int			_port = DEFAULT_PORT;
	if (split2.size() > 2)
		throw CustomException("is invalid port for the \"listen\" directive", _ip_port);

	if (split2.size() == 1)
	{
		if (str_utils::is_number(split2[0]))
			_port = getPort(split2[0]);
		else
			_ip = getIp(split2[0]);
	}
	else
	{
		_ip = getIp(split2[0]);
		_port = getPort(split2[1]);
	}
	if (isIpPortAlreadyExists(_ip, _port))
		throw CustomException("a duplicate listen", _ip_port);
	ip_ports.push_back(std::make_pair(_ip, _port));
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

IpPorts				Server::getIpPorts() const
{
	if (ip_ports.size() != 0)
		return (ip_ports);
	IpPorts v;
	v.push_back(std::make_pair(str_utils::ip(DEFAULT_IP), DEFAULT_PORT));
	return (v);
}

Locations				Server::getLocations() const {return (locations);}
bool					Server::isDefaultServer() const {return (default_server);}
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
