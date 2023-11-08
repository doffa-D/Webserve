#ifndef	LOCATION_HPP
# define LOCATION_HPP

#include <iostream>
#include "../Models/GlobalModel.hpp"

class	Location : public GlobalModel
{
	std::vector<Location*>	*innerLocation;
	String		path;

	public :
		Location( void );
		Location(GlobalModel&, String, std::vector<Location*>*);
		Location(const Location& copy);
		~Location( void );
		Location& operator=(const Location& target);
//		void	addInnerLocation(Location* Inner);
		void	setPath(String Path);
		String	getPath( void ) const;
		std::vector<Location*>	*getInnerLocation( void ) const;
		void	deleteLocations( void );


//		static	void	printLocation(const Location&);
		static	void	printAllLocations(const std::vector<Location*>*, String&);
};

#endif
