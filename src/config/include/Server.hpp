/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:56:32 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/09 18:44:31 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"
#include "Location.hpp"

typedef std::vector<std::pair<string, Location> > Locations;
typedef std::vector<std::pair<Uint, int> >	IpPorts;

class Server : public CommonDirectives
{
	private:
		string				server_name;
		IpPorts				ip_ports;
		Locations			locations;
		bool				nullObject;
		bool	isLocationAlreadyExists(const string& _path);
		bool	isIpPortAlreadyExists(Uint _ip, int _port);
		Uint	getIp(const string& _value);
		int		getPort(const string& _value);
	public:
		Server();
		~Server();
		Server(const Server& _copy);
		Server& operator=(const Server& _assignment);
		static Server createNullObject();
		bool	isNull();

		/*==============>SETTERS<================*/
		void	setServerName(const string& _server_name);
		bool	AddIpPort(const string& _ip_port);
		void	AddLocation(const string& path, const Location& _location);	
		/*==============>GETTERS<================*/
		VecString			getServerNames() const;
		IpPorts				getIpPorts() const;
		Locations			getLocations() const;
		Location			getLocationByPath(const string& _path);
};
