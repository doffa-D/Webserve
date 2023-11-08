#ifndef SERVER_MODEL
#define SERVER_MODEL

#include "../Parser/Location.hpp"
#include "GlobalModel.hpp"

class	ServerModel : public GlobalModel
{
	std::vector<Location*>	location;

	public :
		ServerModel( void );
		ServerModel(GlobalModel&, std::vector<Location*>);
		ServerModel& operator=(const ServerModel& target);
		ServerModel(const ServerModel& copy);
		~ServerModel( void )	throw();
		void	setLocation(std::vector<Location*> _location);
		void	addLocation(Location *_location);

		const std::vector<Location*>&	getLocation( void ) const;

		static	void	printServerModelInfo(const ServerModel& serverModel);
		
};

#endif

