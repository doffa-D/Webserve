/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:48:02 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/08 21:53:14 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"


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
		allowed_methods = _assignment.allowed_methods;
		autoindex = _assignment.autoindex;
		client_max_body_size = _assignment.client_max_body_size;
		error_log = _assignment.error_log;
		access_log = _assignment.access_log;
		error_pages = _assignment.error_pages;
		mimeTypes = _assignment.mimeTypes;
		upload = _assignment.upload;
		memset(check, 0, sizeof(check)); 
	}
	return (*this);
}

CommonDirectives::CommonDirectives(const string& _root, const string& _index,
								   const string& _try_files,
								   const bool& _autoindex , const long& _client_max_body_size,
								   const string& _error_log, const string& _access_log, const string& _upload)
{
	root = _root;
	index = _index;
	try_files = _try_files;
	autoindex = _autoindex;
	client_max_body_size = _client_max_body_size;
	error_log = _error_log;
	access_log = _access_log;
	upload = _upload;
	memset(check, 0, sizeof(check)); 
}

void	CommonDirectives::setRoot(const string& _root)
{
	if (check[0])
		throw CustomException("directive is duplicate", "root");
	VecString vec = str_utils::proSplit(_root);
	if (vec.size() > 1)
		throw CustomException("invalid number of arguments in \"root\" directive");

	root = str_utils::remove_quotes(_root);
	check[0] = 1;
}

void	CommonDirectives::setIndex(const string& _index)
{
	if (check[1])
		throw CustomException("directive is duplicate", "index");
	index = _index;
	check[1] = 1;
}

void	CommonDirectives::setTryFiles(const string& _try_files)
{
	if (check[2])
		throw CustomException("directive is duplicate", "index");
	try_files = _try_files;
	check[2] = 1;
}

static bool	isValideMethod(const string& to_check)
{
	VecString validMethods = str_utils::split(ALLOWED_METHODS, ' ');
	for (size_t i = 0; i < validMethods.size();i++)
		if (to_check == validMethods[i])
			return (true);
	return (false);
}

void	CommonDirectives::setAutoIndex(const string& _autoindex)
{
	if (check[3])
		throw CustomException("directive is duplicate", "autoindex");
	if (_autoindex != "on" && _autoindex != "off")
		throw CustomException("is invalid value for \"autoindex\" directive, it must be \"on\" or \"off\"", _autoindex);
	autoindex = (_autoindex == "on" ? true : false);
	check[3] = 1;
}

void	CommonDirectives::setClientMaxBodySize(const string& _client_max_body_size)
{
	if (check[4])
		throw CustomException("directive is duplicate", "client_max_body_size");
	char*	endp;
	double	value = strtod(_client_max_body_size.c_str(), &endp);
	string unit = endp;
	str_utils::trim(unit);

	if (value < 0)
		throw CustomException("is invalid value for \"client_max_body_size\" directive", _client_max_body_size);

	if (unit == "k" && value < MAX_CLIENT_BODY_SIZE * 1024 * 1024)
		client_max_body_size = value * 1024;
	else if ((unit == "m" || unit == "") && value < MAX_CLIENT_BODY_SIZE * 1024)
		client_max_body_size = value * 1024 * 1024;
	else if (unit == "g" && value < MAX_CLIENT_BODY_SIZE)
		client_max_body_size = value * (1024 * 1024 * 1024);
	else
		throw CustomException("is invalid value for \"client_max_body_size\" directive", _client_max_body_size);
	check[4] = 1;
}

void	CommonDirectives::setErrorLog(const string& _error_log)
{
	if (check[5])
		throw CustomException("directive is duplicate", "error_log");
	error_log = _error_log;
	check[5] = 1;
}

void	CommonDirectives::setAccessLog(const string& _access_log)
{
	if (check[6])
		throw CustomException("directive is duplicate", "access_log");
	access_log = _access_log;
	check[6] = 1;
}

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
		if (value < 300 || value > 599)
			throw CustomException("is invalid value for \"error_pages\" directive,\n\tvalue must be between 300 and 599", _error_pages); 
		error_pages.insert(make_pair(value, replacedBy));
	}
}

void	CommonDirectives::setAllowedMethod(const string& _allowed_methods)
{
	if (check[7])
		throw CustomException("directive is duplicate", "allowed_methods");
	VecString		values = str_utils::ultimatSplit(_allowed_methods, " \t");
	VecString_iter	it = values.begin();
	VecString		_methods;
	for (; it != values.end(); it++)
	{
		if (!isValideMethod(*it))
			throw CustomException("is invalid value for \"allowed_method\" directive", _allowed_methods);
		if (find(_methods.begin(), _methods.end(), *it) != _methods.end())
			throw CustomException("a duplicate allowed_method", *it);
		_methods.push_back(*it);
	}
	allowed_methods = _methods;
	check[7] = 1;
}

void	CommonDirectives::addMimeType(const string& _key, const string& _value)
{
	VecString split = str_utils::ultimatSplit(_value, " \t");
	VecString_iter it = split.begin();
	for (;it != split.end(); it++)
		mimeTypes.insert(std::make_pair(*it, _key));
}

void	CommonDirectives::setUpload(const string& _upload){upload = _upload;}


const string&		CommonDirectives::getRoot() const {return (root);}
const string&		CommonDirectives::getErrorLog() const {return (error_log);}
const string&		CommonDirectives::getAccessLog() const {return (access_log);}
long				CommonDirectives::getClientMaxBodySize() const {return (client_max_body_size);}
bool				CommonDirectives::getAutoIndex() const {return (autoindex);}
MapIntString		CommonDirectives::getErrorPages() const {return (error_pages);}
VecString			CommonDirectives::getAllowedMethods() const 
{
	if (!allowed_methods.size())
		return (str_utils::split(ALLOWED_METHODS, ' '));
	return (allowed_methods);
}

VecString 			CommonDirectives::getIndexes() const
{
	VecString out = str_utils::proSplit(index);

	for (VecString_iter it = out.begin(); it != out.end();it++)
		*it = str_utils::remove_quotes(*it);
	return (out);
}

VecString 			CommonDirectives::getTryFiles() const
{
	VecString out = str_utils::proSplit(try_files);

	for (VecString_iter it = out.begin(); it != out.end();it++)
		*it = str_utils::remove_quotes(*it);
	return (out);
}

MapStringString		CommonDirectives::getMimeTypes() const {return (mimeTypes);}
string				CommonDirectives::getMimeTypeByKey(const string& _key)
{
	MapStringString_iter it = mimeTypes.begin();
	for (;it != mimeTypes.end(); it++)
		if (it->first == _key)
			return (it->second);
	return ("text/html");
}
const string&		CommonDirectives::getUpload() const {return (upload);}
