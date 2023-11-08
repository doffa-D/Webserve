#include "ParsingException.hpp"

ParsingException::ParsingException( void ) : std::exception(), message()
{
}

ParsingException::ParsingException(const String& Message) : std::exception(), message(Message)
{
}

ParsingException::ParsingException(const ParsingException& copy) : std::exception(copy)
{
	*this = copy;
}
ParsingException::~ParsingException( void ) throw()
{
}

ParsingException&	ParsingException::operator=(const ParsingException& target)
{
	if (this != &target)
	{
		std::exception::operator=(target);
		message = target.message;
	}
	return (*this);
}

const char*	ParsingException::what( void ) const throw()
{
	return (message.c_str());
}
