/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:15:25 by kchaouki          #+#    #+#             */
/*   Updated: 2024/03/09 00:13:25 by rrhnizar         ###   ########.fr       */
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

void	Location::setUri_Max(const long& uri_max)
{
	Uri_Max = uri_max;
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



const string&	Location::getAlias() const {return (alias);}
const long&		Location::getUri_Max() const {return Uri_Max;}