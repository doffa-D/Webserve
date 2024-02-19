/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki < kchaouki@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:15:25 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/19 10:49:41 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

Location::Location() : CommonDirectives("./", false, (1024 * 1024), ERROR_LOG, ACCESS_LOG, UPLOAD), nullObject(false) {}
Location::~Location() {}
Location::Location(const Location& _copy) : CommonDirectives(_copy) {*this = _copy;}
Location::Location(const CommonDirectives& _copy) : CommonDirectives(_copy) {}

Location& Location::operator=(const Location& _assignment)
{
	if (this != &_assignment)
	{
		nullObject = _assignment.nullObject;
		alias = _assignment.alias;
		CommonDirectives::operator=(_assignment);
	}
	return (*this);
}

Location Location::createNullObject()
{
	Location l;
	l.nullObject = true;
	return (l);
}

bool	Location::isNull()
{
	return (nullObject);	
}

void	Location::setAlias(const string& _alias)
{
	if (alias != "")
		throw CustomException("directive is duplicate", "alias");
	if (this->root_alias)
		throw CustomException("directive is duplicate, \"root\" directive was specified earlier", "alias");
	VecString vec = str_utils::proSplit(_alias);
	if (vec.size() > 1)
		throw CustomException("invalid number of arguments in \"alias\" directive");
	this->check_unexpected(_alias, "alias");
	alias = str_utils::remove_quotes(_alias);
	root_alias = true;
}

string			Location::getIndexFilePathByRoot(const string& _value) const
{
	string fullPath;
	VecString out = this->getIndexes();
	for (VecString_iter it = out.begin(); it != out.end();it++)
	{
		fullPath = _value + *it;
		std::ifstream file(fullPath);
		if (file.is_open())
			return (fullPath);
	}
	return ("");
}

string		Location::getErrorPageByStatus(int status) const
{
	try{return (this->getErrorPages().at(status));}
	catch(const std::exception& e){(void)e;}
	return ("");	
}

const string&		Location::getAlias() const {return (alias);}