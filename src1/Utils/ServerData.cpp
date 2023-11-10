#include "ServerData.hpp"

ServerData::ServerData( void )
{
}

ServerData::ServerData(const std::vector<ServerModel>& _data) : servers(_data)
{
}

ServerData::ServerData(const ServerData& copy)
{
	*this = copy;
}

ServerData&	ServerData::operator=(const ServerData& target)
{
	servers = target.servers;
	return (*this);
}

void	ServerData::setServerData(const std::vector<ServerModel>& serversData)
{
	servers = serversData;
}

void	ServerData::displayServers( void )
{
	std::vector<ServerModel>::iterator	iterBegin = servers.begin();
	std::vector<ServerModel>::iterator	iterEnd = servers.end();
	int i = 0;
	while (iterBegin < iterEnd)
	{
		std::cout << "\n===================== Server " << ++i << " Info =====================\n\n";
		ServerModel::printServerModelInfo(*iterBegin);
		iterBegin++;
	}
}

ServerModel	ServerData::getServerByServerName(const String& serverName)
{
	std::vector<ServerModel>::iterator iterBegin = servers.begin();
	std::vector<ServerModel>::iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		std::vector<Data> value = iterBegin->getData("server_name");
		for (int i = 0; i < (int)value.size(); i++)
		{
			String str = value.at((std::vector<Data>::size_type)i).getValue();
			std::vector<String> values = str.split();
			if (find(values.begin(), values.end(), serverName) != values.end())
				return (*iterBegin);
		}
		iterBegin++;
	}
	throw (std::exception());
}

ServerModel	ServerData::getServerByPort(const unsigned short& port)
{
	std::vector<ServerModel>::iterator iterBegin = servers.begin();
	std::vector<ServerModel>::iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		std::vector<Data> value = iterBegin->getData("listen");
		if (value.empty() == false && (unsigned short)std::strtol(value.begin()->getValue().c_str(), NULL, 10) == port)
			return (*iterBegin);
		iterBegin++;
	}
	throw (std::exception());
}

const std::vector<ServerModel>&	ServerData::getAllServers()
{
	return (servers);
}

ServerModel	ServerData::getDefaultServer( void )
{
	std::vector<ServerModel>::iterator iterBegin = servers.begin();
	std::vector<ServerModel>::iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		std::vector<Data> value = iterBegin->getData("listen");
		for (int i = 0; i < (int)value.size(); i++)
		{
			String str = value.at((std::vector<Data>::size_type)i).getValue();
			std::vector<String> values = str.split();
			if (find(values.begin(), values.end(), "default_server") != values.end())
				return (*iterBegin);
			// if (value.at((std::vector<Data>::size_type)i).getValue().contains("default_server") == true)
		}
		// std::vector<Data> value = iterBegin->getData("listen");
		// for (int i = 0; i < (int)value.size(); i++)
		// 	if (value.at(i).getValue().contains("default_server") == true)
		// 		return (*iterBegin);
		iterBegin++;
	}
	throw (std::exception());
}

bool		ServerData::checkDuplicateServer( void )
{
	// std::vector<ServerModel>::iterator iterBegin = servers.begin();
	// std::vector<ServerModel>::iterator iterEnd = servers.end();
	// while (iterBegin < iterEnd)
	// {
	// 	std::vector<Data> value = iterBegin->getData("listen");
	// 	for (int i = 0; i < (int)value.size(); i++)
	// 		if (isExist(value) == true)
	// 			return (false);
	// 	}
	// 	iterBegin++;
	// }
	return (false);
}
