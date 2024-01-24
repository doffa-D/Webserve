/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:48:02 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/24 12:45:18 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommonDirectives.hpp"

CommonDirectives::CommonDirectives() {}
CommonDirectives::~CommonDirectives() {}
CommonDirectives::CommonDirectives(const CommonDirectives& _copy) {*this = _copy;}
CommonDirectives& CommonDirectives::operator=(const CommonDirectives& _assignment)
{
	if (this != &_assignment)
	{
		root = _assignment.root;
		index = _assignment.index;
		try_files = _assignment.try_files;
		alias = _assignment.alias;
		allowed_methods = _assignment.allowed_methods;
		autoindex = _assignment.autoindex;
		client_max_body_size = _assignment.client_max_body_size;
		error_log = _assignment.error_log;
		access_log = _assignment.access_log;
		error_pages = _assignment.error_pages;
	}
	return (*this);
}

CommonDirectives::CommonDirectives(const string& _root, const string& _index,
								   const string& _try_files, const string& _alias,
								   const bool& _autoindex , const long& _client_max_body_size,
								   const string& _error_log, const string& _access_log)
{
	root = _root;
	index = _index;
	try_files = _try_files;
	alias = _alias;
	autoindex = _autoindex;
	client_max_body_size = _client_max_body_size;
	error_log = _error_log;
	access_log = _access_log;
}

void	CommonDirectives::setRoot(const string& _root)
{
	// if (root != "/")
		// throw CustomException("directive is duplicate", _root);
	root = _root;
}

void	CommonDirectives::setIndex(const string& _index) {index = _index;}
void	CommonDirectives::setAlias(const string& _alias) {alias = _alias;}
void	CommonDirectives::setTryFiles(const string& _try_files) {try_files = _try_files;}

static bool	isValideMethod(const string& to_check)
{
	string validMethods[] = {"GET", "POST", "DELETE"};
	for (int i = 0; i < 3;i++)
		if (to_check == validMethods[i])
			return (true);
	return (false);
}

void	CommonDirectives::setAllowedMethod(const string& _allowed_methods)
{
	VecString	values = str_utils::ultimatSplit(_allowed_methods, " \t");
	VecString_iter	it = values.begin();
	for (; it != values.end(); it++)
	{
		if (!isValideMethod(*it))
			throw CustomException("is invalid value for \"allowed_method\" directive", _allowed_methods); 
		allowed_methods.push_back(*it);
	}
}

void	CommonDirectives::setAutoIndex(const string& _autoindex)
{
	if (_autoindex != "on" && _autoindex != "off")
		throw CustomException("is invalid value for \"autoindex\" directive, it must be \"on\" or \"off\"", _autoindex);
	autoindex = (_autoindex == "on" ? true : false);
}

void	CommonDirectives::setClientMaxBodySize(const string& _client_max_body_size)
{
	char*	endp;
	double	value = strtod(_client_max_body_size.c_str(), &endp);
	string unit = endp;
	str_utils::trim(unit);

	if (value < 0)
		throw CustomException("is invalid value for \"client_max_body_size\" directive", _client_max_body_size);

	if (unit == "k" && value < MAXCLIENTBODYSIZE * 1024 * 1024)
		client_max_body_size = value * 1024;
	else if ((unit == "m" || unit == "") && value < MAXCLIENTBODYSIZE * 1024)
		client_max_body_size = value * 1024 * 1024;
	else if (unit == "g" && value < MAXCLIENTBODYSIZE)
		client_max_body_size = value * (1024 * 1024 * 1024);
	else
		throw CustomException("is invalid value for \"client_max_body_size\" directive", _client_max_body_size);
}

void	CommonDirectives::setErrorLog(const string& _error_log) {error_log = _error_log;}
void	CommonDirectives::setAccessLog(const string& _access_log) {access_log = _access_log;}
void	CommonDirectives::addErrorPage(const string& _error_pages)
{
	VecString values = str_utils::ultimatSplit(_error_pages, " \t");
	VecString::reverse_iterator r_it = values.rbegin();
	string replacedBy = *r_it;
	r_it++;
	for (; r_it != values.rend(); r_it++)
	{
		char*	endp;
		double	value = strtod(r_it->c_str(), &endp);
		string rest = endp;
		if (rest != "")
			throw CustomException("is invalid value for \"error_pages\" directive", _error_pages); 
		error_pages.insert(make_pair(value, replacedBy));
	}
}

const string&		CommonDirectives::getRoot() const {return (root);}
const string&		CommonDirectives::getIndex() const {return (index);}
const string&		CommonDirectives::getAlias() const {return (alias);}
const string&		CommonDirectives::getTryFiles() const {return (try_files);}
const string&		CommonDirectives::getErrorLog() const {return (error_log);}
const string&		CommonDirectives::getAccessLog() const {return (access_log);}
long				CommonDirectives::getClientMaxBodySize() const {return (client_max_body_size);}
bool				CommonDirectives::getAutoIndex() const {return (autoindex);}
MapIntString		CommonDirectives::getErrorPages() const {return (error_pages);}
VecString			CommonDirectives::getAllowedMethods() const {return (allowed_methods);}
VecString 			CommonDirectives::getAllIndexes() const {return (str_utils::ultimatSplit(index, " \t"));}
VecString 			CommonDirectives::getAllTryFiles() const {return (str_utils::ultimatSplit(try_files, " \t"));}
