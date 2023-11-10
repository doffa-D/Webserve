#ifndef	DATA_HPP
#define DATA_HPP

#include <iostream>
#include "String.hpp"
#include "Logger.hpp"

class Data
{
	String	key;
	String value;

	public :
		Data(const String& _key, const String& _value);
		void	setKey(const String& Key);
		void	setValue(const String& Value);
		const String&	getKey( void ) const;
		const String&	getValue( void ) const;
		
		static	void	printData(const Data& data, String&);
};

#endif
