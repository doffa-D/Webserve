#ifndef	SERVERDATA_HPP
#define	SERVERDATA_HPP

#include "../Parser/Parser.hpp"

class ServerData
{
	std::vector<ServerModel> servers;

	public :
		ServerData( void );
		ServerData(const std::vector<ServerModel>&);
		ServerData(const ServerData&);
		ServerData&	operator=(const ServerData&);
		void	setServerData(const std::vector<ServerModel>&);
		void	displayData( void );
		ServerModel	getServer(const String& serverName);
};

#endif
