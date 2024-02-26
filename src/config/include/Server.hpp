/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:56:32 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/25 18:16:11 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"
#include "Location.hpp"

#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT 8080

typedef vector<pair<string, Location> >	Locations;
typedef vector<pair<Uint, int> >		IpPorts;

class Server : public CommonDirectives
{
	private:
		VecString			server_name;
		IpPorts				ip_ports;
		Locations			locations;
		bool				default_server;
		bool				nullObject;
		long				client_max_header_buffer_size;
		bool				is_set;
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
		void	setAsDefaultServer();
		bool	AddIpPort(const string& _ip_port);
		void	AddLocation(const string& path, const Location& _location);
		void	setClientMaxHeaderBufferSize(const string& _value);
		/*==============>GETTERS<================*/
		bool				isDefaultServer() const;
		VecString			getServerNames() const;
		IpPorts				getIpPorts() const;
		Locations			getLocations() const;
		Location			getLocationByPath(const string& _path);
		long				getClientMaxHeaderBufferSize() const;						
};
