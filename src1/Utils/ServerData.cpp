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

void	ServerData::displayData( void )
{

}

ServerModel	ServerData::getServer(const String& serverName)
{
	std::vector<ServerModel>::iterator iterBegin = servers.begin();
	std::vector<ServerModel>::iterator iterEnd = servers.end();
	while (iterBegin < iterEnd)
	{
		std::vector<Data> value = iterBegin->getData("server_name");
		if (value.empty() == false && value.begin()->getValue().contains(serverName) == true)
			return (*iterBegin);
		iterBegin++;
	}
	throw (std::exception());
}
