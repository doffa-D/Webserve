#ifndef	PARSINGEXCEPTION_HPP
# define PARSINGEXCEPTION_HPP

#include <exception>
#include "String.hpp"

class ParsingException : public std::exception
{
	String	message;

	public :
		ParsingException( void );
		ParsingException(const String&);
		ParsingException(const ParsingException&);
		virtual ~ParsingException( void ) throw();
		ParsingException&	operator=(const ParsingException&);
		virtual const char*	what( void ) const throw();
};

#endif

