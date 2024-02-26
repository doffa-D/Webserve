/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:48:02 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/25 18:14:32 by kchaouki         ###   ########.fr       */
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
		redirection = _assignment.redirection;
		allowed_methods = _assignment.allowed_methods;
		autoindex = _assignment.autoindex;
		client_max_body_size = _assignment.client_max_body_size;
		error_log = _assignment.error_log;
		access_log = _assignment.access_log;
		error_pages = _assignment.error_pages;
		mimeTypes = _assignment.mimeTypes;
		upload = _assignment.upload;
		cgi = _assignment.cgi;
		root_alias = false;
		memset(check, 0, sizeof(check)); 
	}
	return (*this);
}

CommonDirectives::CommonDirectives(const string& _root, const bool& _autoindex , const long& _client_max_body_size,
								   const string& _error_log, const string& _access_log, const string& _upload)
{
	root = _root;
	autoindex = _autoindex;
	client_max_body_size = _client_max_body_size;
	error_log = _error_log;
	access_log = _access_log;
	upload = _upload;
	root_alias = false;
	memset(check, 0, sizeof(check)); 
	memset(_bool, 0, sizeof(_bool)); 
}

void	CommonDirectives::check_unexpected(const string& to_check, const string& direc)
{
	if (strchr("\"\'", to_check[0]))
	{
		string	new_str = to_check;
		new_str.erase(0, 1);
		size_t pos = str_utils::find_first_of(new_str, "\"\'");
		if (pos != new_str.length() - 1)
			throw CustomException("unexpected \"" + string(1, new_str[pos + 1]) + "\" in \"" + direc + "\" directive");
	}
}

void	CommonDirectives::setRoot(const string& _root)
{
	if (check[0])
		throw CustomException("directive is duplicate", "root");
	VecString vec = str_utils::proSplit(_root);
	if (vec.size() > 1)
		throw CustomException("invalid number of arguments in \"root\" directive");
	check_unexpected(_root, "root");
	root = str_utils::remove_quotes(_root);
	if (root_alias)
		throw CustomException("directive is duplicate, \"alias\" directive was specified earlier", "root");
	root_alias = true;
	check[0] = 1;
}

void	CommonDirectives::setIndex(const string& _index)
{
	if (check[1])
		throw CustomException("directive is duplicate", "index");
	VecString vec = str_utils::proSplit(_index);
	for (size_t i = 0; i < vec.size();i++)
	{
		check_unexpected(vec[i], "index");
		index.push_back(str_utils::remove_quotes(vec[i]));
	}
	check[1] = 1;
}

void	CommonDirectives::setRedirection(const string& _redirection)
{
	if (check[2])
		throw CustomException("directive is duplicate", "redirection");
	VecString split = str_utils::proSplit(_redirection);
	if (split.size() != 2)
		throw CustomException("invalid number of arguments in \"redirection\" directive\n\t Hint: must have a status code and a path to the final location");
	int status = str_utils::to_int(split[0]);
	if (!str_utils::is_number(split[0]) || status < 300 || status > 308 )
		throw CustomException("first argument in \"redirection\" directive \n\t Hint: must be a status code between 300 and 308");
	check_unexpected(split[1], "redirection");
	redirection = std::make_pair(status, str_utils::remove_quotes(split[1]));
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

	if (unit == "k" && value < MAX_BUFFER_SIZE * 1024 * 1024)
		client_max_body_size = value * 1024;
	else if ((unit == "m" || unit == "") && value < MAX_BUFFER_SIZE * 1024)
		client_max_body_size = value * 1024 * 1024;
	else if (unit == "g" && value < MAX_BUFFER_SIZE)
		client_max_body_size = value * (1024 * 1024 * 1024);
	else
		throw CustomException("is invalid value for \"client_max_body_size\" directive", _client_max_body_size);
	check[4] = 1;
}

void	CommonDirectives::setErrorLog(const string& _error_log)
{
	if (check[5])
		throw CustomException("directive is duplicate", "error_log");
	VecString vec = str_utils::proSplit(_error_log);
	if (vec.size() > 1)
		throw CustomException("invalid number of arguments in \"root\" directive");
	check_unexpected(_error_log, "error_log");
	error_log = str_utils::remove_quotes(_error_log);
	check[5] = 1;
}

void	CommonDirectives::setAccessLog(const string& _access_log)
{
	if (check[6])
		throw CustomException("directive is duplicate", "access_log");
	VecString vec = str_utils::proSplit(_access_log);
	if (vec.size() > 1)
		throw CustomException("invalid number of arguments in \"root\" directive");
	check_unexpected(_access_log, "access_log");
	access_log = str_utils::remove_quotes(_access_log);
	check[6] = 1;
}

void	CommonDirectives::addErrorPage(const string& _error_pages)
{
	if (!_bool[0])
		error_pages.clear();
	_bool[0] = true;
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
	if (!_bool[1])
		mimeTypes.clear();
	_bool[1] = true;
	VecString split = str_utils::ultimatSplit(_value, " \t");
	VecString_iter it = split.begin();
	for (;it != split.end(); it++)
		mimeTypes.insert(std::make_pair(*it, _key));
}

void	CommonDirectives::setUpload(const string& _upload)
{
	if (check[8])
		throw CustomException("directive is duplicate", "upload");
	VecString split = str_utils::ultimatSplit(_upload, " \t");
	if (split.size() != 1)
		throw CustomException("invalid number of arguments in \"upload\" directive");
	check_unexpected(_upload, "upload");
	upload = str_utils::remove_quotes(_upload);
	check[8] = 1;
}

static bool	isValideExtension(const string& to_check)
{
	VecString validMethods = str_utils::split(ALLOWED_EXTENSION, ' ');
	for (size_t i = 0; i < validMethods.size();i++)
		if (to_check == validMethods[i])
			return (true);
	return (false);
}

void	CommonDirectives::setCgi(const string& _cgi)
{
	if (check[9])
		throw CustomException("directive is duplicate", "cgi");
	VecString	split = str_utils::ultimatSplit(_cgi, " \t");

	for (size_t i = 0; i < split.size(); i++)
	{
		VecString sp = str_utils::split(split[i], ':');
		if (sp.size() != 2)
			throw CustomException("invalid number of arguments in \"cgi\" directive", split[i]);
		if (!isValideExtension(str_utils::trim(sp[0])))
			throw CustomException("not a valid extension in \"cgi\" directive");
		pair<MapStringString::iterator, bool> res = cgi.insert(std::make_pair(sp[0], sp[1]));
		if (!res.second)
			throw CustomException("is duplicate value in \"cgi\" directive", split[i]);
	}
	check[9] = 1;
}

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
	if (!index.size())
		return (str_utils::split(DEFAULT_INDEX, ' '));
	return (index);
}

pair<int, string>	CommonDirectives::getRedirection() const{return (redirection);}
MapStringString		CommonDirectives::getMimeTypes() const {return (mimeTypes);}
string				CommonDirectives::getMimeTypeByKey(const string& _key) const
{
	try{return (mimeTypes.at(_key));}
	catch (const std::out_of_range& e) { (void) e;}
	return ("text/html");
}

const string&		CommonDirectives::getUpload() const {return (upload);}
MapStringString		CommonDirectives::getCgi() const {return (cgi);}