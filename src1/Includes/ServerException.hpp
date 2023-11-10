#ifndef SERVEREXCEPTION_HPP
#define SERVEREXCEPTION_HPP

#include <exception>
#include "String.hpp"

class ServerException : public std::exception
{
	String	message;

	public :
		ServerException( void );
		ServerException(const String&);
		ServerException(const ServerException&);
		virtual ~ServerException( void ) throw();
		ServerException&	operator=(const ServerException&);
		virtual const char*	what( void ) const throw();
};


#endif