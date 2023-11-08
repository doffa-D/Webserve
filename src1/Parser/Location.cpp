#include "Location.hpp"

Location::Location( void ) : GlobalModel()
{
	innerLocation = NULL;
}

Location::Location(GlobalModel& model, String Path, std::vector<Location*> *_innerLocation) :
	GlobalModel(model),
	innerLocation(_innerLocation),
	path(Path)
{
}

Location::Location(const Location& copy) : GlobalModel(copy)
{
	innerLocation = NULL;
	*this = copy;
}

Location::~Location( void )
{
	try
	{
		std::vector<Location*>::iterator b = innerLocation->begin();
		std::vector<Location*>::iterator e = innerLocation->end();
		while (b < e)
		{
			delete *b;
			*b = NULL;
			b++;
		}
		delete innerLocation;
	}
	catch (...)
	{
		std::cout << "catching Exception in Location detructor(~Location)." << std::endl;
	}
}

Location& Location::operator=(const Location& target)
{
	if (this != &target)
	{
		GlobalModel::operator=(target);
		path = target.getPath();
		delete innerLocation;
		innerLocation = new std::vector<Location*>(*target.getInnerLocation());
	}
	return (*this);
}

/*
void	Location::addInnerLocation(Location& Inner)
{
	innerLocation->push_back(Inner);
}*/

void	Location::setPath(String Path)
{
	path = Path;
}

String	Location::getPath( void ) const
{
	return (path);
}

std::vector<Location*>	*Location::getInnerLocation( void ) const
{
	return (innerLocation);
}

void	Location::deleteLocations( void )
{
	if (!innerLocation)
		return ;
	std::vector<Location*>::iterator b = innerLocation->begin();
	std::vector<Location*>::iterator e = innerLocation->end();
	while (b < e)
	{
		delete *b;
		b++;
	}
}

void	Location::printAllLocations(const std::vector<Location*>* locations, String& str)
{
	std::vector<Location*>::const_iterator ibegin = locations->begin();
	std::vector<Location*>::const_iterator iend = locations->end();
	while (ibegin < iend)
	{
		std::cout << str << "PATH ==> " << (*ibegin)->getPath() << std::endl;
		str.append("\t");
		printGlobalModel(**ibegin, str);
		if ((*ibegin)->getInnerLocation() != NULL)
		{
			str.append("\t");
			printAllLocations((*ibegin)->getInnerLocation(), str);
			str.erase(str.length() - 1);
		}
		str.erase(str.length() - 1);
		ibegin++;
	}
}
