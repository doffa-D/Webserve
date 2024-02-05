/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:15:25 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/05 09:29:15 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Location.hpp"
#include "../../../include/header.hpp"


Location::Location() : CommonDirectives("/", "index.html", "", false, (1024 * 1024), ERROR_LOG, ACCESS_LOG, UPLOAD), nullObject(false) {}
Location::~Location() {}
Location::Location(const Location& _copy) : CommonDirectives(_copy) {*this = _copy;}
Location::Location(const CommonDirectives& _copy) : CommonDirectives(_copy) {}

Location& Location::operator=(const Location& _assignment)
{
	if (this != &_assignment)
	{
		nullObject = _assignment.nullObject;
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