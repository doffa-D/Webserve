#ifndef GLOBALMODEL_HPP
#define GLOBALMODEL_HPP

#include <exception>
#include "Data.hpp"


class GlobalModel
{
	std::vector<Data> data;

	public	:
		GlobalModel( void );
		GlobalModel(const GlobalModel& copy);
		~GlobalModel( void );
		GlobalModel&	operator=(const GlobalModel& target);
		void	addData(const Data& _data);
		std::vector<Data>	getData(const String& key) const;
		const std::vector<Data>&	getAllData( void ) const;

		static	void	printGlobalModel(const GlobalModel&, String&);
};

#endif
