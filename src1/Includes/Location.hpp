#ifndef	LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include "GlobalModel.hpp"

class	Location : public GlobalModel
{
	std::vector<Location>	innerLocation;
	String		path;

	public :
		Location( void );
		Location(const GlobalModel&, const String&, const std::vector<Location>&);
		Location(const Location& copy);
		~Location( void );
		Location& operator=(const Location& target);
		void	setPath(const String& Path);
		const String&	getPath( void ) const;
		const std::vector<Location>&	getInnerLocation( void ) const;

		static	void	printAllLocations(const std::vector<Location>&, String&);
};

#endif
