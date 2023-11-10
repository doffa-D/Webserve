#include "Location.hpp"

Location::Location( void ) : GlobalModel()
{
}

Location::Location(const GlobalModel& model, const String& Path, const std::vector<Location>& _innerLocation) :
	GlobalModel(model),
	innerLocation(_innerLocation),
	path(Path)
{
}

Location::Location(const Location& copy) : GlobalModel(copy)
{
	*this = copy;
}

Location::~Location( void )
{
	innerLocation.clear();
}

Location& Location::operator=(const Location& target)
{
	if (this != &target)
	{
		GlobalModel::operator=(target);
		path = target.getPath();
		innerLocation = target.innerLocation;
	}
	return (*this);
}

void	Location::setPath(const String& Path)
{
	path = Path;
}

const String&	Location::getPath( void ) const
{
	return (path);
}

const std::vector<Location>&	Location::getInnerLocation( void ) const
{
	return (innerLocation);
}

void	Location::printAllLocations(const std::vector<Location>& locations, String& str)
{
	std::vector<Location>::const_iterator ibegin = locations.begin();
	std::vector<Location>::const_iterator iend = locations.end();
	while (ibegin < iend)
	{
		std::cout << str << "PATH ==> " << ibegin->getPath() << std::endl;
		str.append("\t");
		printGlobalModel(*ibegin, str);
		if (ibegin->getInnerLocation().empty() == false)
		{
			str.append("\t");
			printAllLocations(ibegin->getInnerLocation(), str);
			str.erase(str.length() - 1);
		}
		str.erase(str.length() - 1);
		ibegin++;
	}
}
