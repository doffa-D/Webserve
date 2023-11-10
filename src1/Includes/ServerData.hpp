#ifndef	SERVERDATA_HPP
#define	SERVERDATA_HPP

#include "Parser.hpp"
#include <algorithm>    // std::find

class ServerData
{
	std::vector<ServerModel> servers;

	public :
		ServerData( void );
		ServerData(const std::vector<ServerModel>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const std::vector<ServerModel>&);
		void	displayServers( void );
		ServerModel	getServerByServerName(const String& serverName);
		ServerModel	getServerByPort(const unsigned short& port);
		const std::vector<ServerModel>&	getAllServers();
		ServerModel	getDefaultServer( void );
		bool		checkDuplicateServer( void );
};

#endif
