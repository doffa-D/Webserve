#include "ServerException.hpp"

ServerException::ServerException( void ) : std::exception(), message()
{
}

ServerException::ServerException(const String& Message) : std::exception(), message(Message)
{
}

ServerException::ServerException(const ServerException& copy) : std::exception(copy)
{
	*this = copy;
}
ServerException::~ServerException( void ) throw()
{
}

ServerException&	ServerException::operator=(const ServerException& target)
{
	if (this != &target)
	{
		std::exception::operator=(target);
		message = target.message;
	}
	return (*this);
}

const char*	ServerException::what( void ) const throw()
{
	return (message.c_str());
}
