/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:56:32 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/27 18:45:01 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "CommonDirectives.hpp"
#include "Location.hpp"

typedef std::vector<std::pair<string, Location> > Locations;

class Server : public CommonDirectives
{
	private:
		string					server_name;
		unsigned int			ip_address;
		VecInt					ports;
		Locations				locations;
		bool					nullObject;
		bool	isLocationAlreadyExists(const string& _path);
		bool	isPorteAlreadyExists(int _port);
	public:
		Server();
		~Server();
		Server(const Server& _copy);
		Server& operator=(const Server& _assignment);
		static Server createNullObject();
		bool	isNull();

		/*==============>SETTERES<================*/
		void	setServerName(const string& _server_name);
		void	setIpAddress(const string& _host);
		bool	AddPort(const string& _port);
		void	AddLocation(const string& path, const Location& _location);	
		/*==============>GETTERES<================*/
		const string&		getServerName() const;
		unsigned int		getIpAddress() const;
		VecInt				getPorts() const;
		Locations			getLocations() const;
		Location			getLocationByPath(const string& _path);
};
