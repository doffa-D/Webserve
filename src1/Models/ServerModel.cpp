#include "ServerModel.hpp"

ServerModel::ServerModel( void ) : GlobalModel()
{
}

ServerModel::ServerModel(const GlobalModel& model, const std::vector<Location>& _location) : GlobalModel(model), location(_location)
{
}

ServerModel::~ServerModel( void ) throw()
{
	location.clear();
}

ServerModel::ServerModel(const ServerModel& copy) : GlobalModel(copy)
{
	*this = copy;
}

ServerModel& ServerModel::operator=(const ServerModel& target)
{
	if (this != &target)
	{
		GlobalModel::operator=(target);
		location = target.location;
	}
	return (*this);
}

void	ServerModel::setLocation(std::vector<Location>& _location)
{
	location = _location;
}

const std::vector<Location>&	ServerModel::getLocation( void ) const
{
	return (location);
}

void	ServerModel::addLocation(Location _location)
{
	location.push_back(_location);
}

bool	ServerModel::findLocationByPath(
						const std::vector<Location>& locations,
						String& destPath, const String& srcPath,
						void (*to_do) (const Location&))
{
	std::vector<Location>::const_iterator ibegin = locations.begin();
	std::vector<Location>::const_iterator iend = locations.end();
	bool	isDone = false;
	while (ibegin < iend)
	{
		String tmpPath(destPath);
		tmpPath.append(ibegin->getPath());
		if (!srcPath.compare(tmpPath) && tmpPath.length() == srcPath.length())
		{
			to_do(*ibegin);
			isDone = true;
		}
		if (isDone)
			return (true);
		if (ibegin->getInnerLocation().empty() == false && findLocationByPath(ibegin->getInnerLocation(), tmpPath, srcPath, to_do) == true)
			return (true);
		ibegin++;
	}
	return (false);
}

void	ServerModel::printServerModelInfo(const ServerModel& serverModel)
{
	String s("\t");
	GlobalModel::printGlobalModel(serverModel, s);
	std::cout << s << ">>>> Location Info <<<<\n";
	Location::printAllLocations(serverModel.getLocation(), s);
}
