/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 09:31:57 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/24 17:00:42 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(){}
Parser::~Parser() {}
Parser::Parser(const Parser& _copy) {*this = _copy;}
std::vector<Server> Parser::getServers() const {return (servers);}


char	*ft_strnstr(const char *str1, const char *str2, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (str2[i] == '\0')
		return ((char *)str1);
	if (len == 0)
		return (0);
	while (str1[i] && i < len)
	{
		j = 0;
		while (str1[i + j] == str2[j] && i + j < len)
		{
			if (str2[j + 1] == '\0')
				return (((char *)&str1[i]));
			j++;
		}
		i++;
	}
	return (0);
}

void	Parser::fileValideDirectives()
{
	validDirectives.push_back("server_name");
    validDirectives.push_back("autoindex");
    validDirectives.push_back("allowed_method");
    validDirectives.push_back("host");
    validDirectives.push_back("listen");
    validDirectives.push_back("client_max_body_size");
    validDirectives.push_back("error_log");
    validDirectives.push_back("access_log");
    validDirectives.push_back("error_page");
    validDirectives.push_back("location");
    validDirectives.push_back("root");
    validDirectives.push_back("index");
    validDirectives.push_back("alias");
    validDirectives.push_back("try_files");
    /* validDirectives.push_back("include");*/
}

bool	Parser::isValideDirective(const string& _directive)
{
	std::vector<string>::iterator it = validDirectives.begin();
	for (; it < validDirectives.end(); it++)
		if (*it == _directive)
			return (true);
	return (false);
}

bool	Parser::isValideForLocation(const string& key)
{
	string validDirectives[11] = {"root", "try_files", "index",
								 "location", "alias", "allowed_method",
								 "autoindex", "client_max_body_size", 
								 "error_log", "access_log", "error_pages"};
	for (size_t i = 0; i < 11; i++)
		if (key == validDirectives[i])
			return (true);
	return (false);
}

void	fillCommonDirectives(CommonDirectives& common, const string& key, const string& value)
{
	if (key == "root")
		common.setRoot(value);
	else if (key == "index")
		common.setIndex(value);
	else if (key == "try_files")
		common.setTryFiles(value);
	else if (key == "alias")
		common.setAlias(value);
	else if (key == "autoindex")
		common.setAutoIndex(value);
	else if (key == "client_max_body_size")
		common.setClientMaxBodySize(value);
	else if (key == "error_log")
		common.setErrorLog(value);
	else if (key == "access_log")
		common.setAccessLog(value);
	else if (key == "error_page")
		common.addErrorPage(value);
	else if (key == "allowed_method")
		common.setAllowedMethod(value);
}

void	Parser::fillLocationDirective(Server& server, Location& old_location, ListString_iter& it, const string& befor)
{
	string	path = str_utils::trim(it->substr(str_utils::find_first_of(*it, " \t"), it->length()));

	if (befor != "" && !ft_strnstr(path.c_str(), befor.c_str(), befor.length()))
		throw CustomException("location \"" + path + "\" is outside location \"" + befor + "\"");

	if (*(++it) != "{")
		throw CustomException("Directive \"location\" has no opening \"{\"");
	++it;
	Location location;
	if (old_location.getRoot() != "NULL")
		location = old_location;

	for (;it != tokens.end() && *it != "}";)
	{
		string key = it->substr(0, it->find_first_of(" \t"));
		if (isValideDirective(key) || key == "server")
		{
			if (!isValideForLocation(key))
				throw CustomException("directive is not allowed inside location", key);
			else if (key == "location")
				fillLocationDirective(server, location, it, path);
			else
			{
				string	value = str_utils::trim(it->substr(str_utils::find_first_of(*it, " \t"), it->length()));
				fillCommonDirectives(location, key, value);
				it++;
				if (*it == "}")
					throw CustomException("unexpected \"}\"");
				else if (*it == "{")	
					throw CustomException("is not terminated by \";\"", key);
			}
		}
		else
		{
			if (*it == ";")
				throw CustomException("unexpected \";\"");	
			throw CustomException("not a valide directive", key);
		}
		it++;
	}
	server.AddLocation(path, location);
}

void	Parser::fillDirectives(Server& server, ListString_iter& it, bool& ok)
{
	string	key = it->substr(0, str_utils::find_first_of(*it, " \t"));
	string	value = str_utils::trim(it->substr(str_utils::find_first_of(*it, " \t"), it->length()));
	if (value.empty())
		throw CustomException("Directive have invalid number of arguments", key);
	if (key == "server_name")
		server.setServerName(value);
	else if (key == "host")
		server.setHost(value);
	else if (key == "include")
		(void)key; //handle include
	else if (key == "listen")
		ok = (ok == false) ? server.AddPort(value) : server.AddPort(value);
	else
		fillCommonDirectives(server, key, value);
}

void	Parser::fillServerData(ListString_iter& it)
{
	Server server;
	bool	hasDefualtTage = false;
	if (*(++it) != "{")
		throw CustomException("Directive \"server\" has no opening \"{\"");
	it++;
	while (it != tokens.end() && *it != "}")
	{
		string key = it->substr(0, str_utils::find_first_of(*it, " \t"));
		if (isValideDirective(key))
		{
			if (key == "location")
			{
				Location l;
				l.setRoot("NULL");
				fillLocationDirective(server, l, it, "");
			}
			else
			{
				fillDirectives(server, it, hasDefualtTage);
				it++;
				if (*it == "}")
					throw CustomException("unexpected \"}\"");
				else if (*it == "{")	
					throw CustomException("is not terminated by \";\"", key);
			}
		}
		else
		{
			if (*it == ";")
				throw CustomException("unexpected \";\"");	
			else if (key == "server")
				throw CustomException("Directive \"server\" is not allowed inside \"server\" directive");
			throw CustomException("not a valide directive", key);
		}
		it++;
	}
	if (*it != "}")
		throw CustomException("Directive \"server\" is not terminated by \"}\"");
	it++;
	if (hasDefualtTage)
		servers.insert(servers.begin(), server);
	else
		servers.push_back(server);
}

void	Parser::analyzer()
{
	ListString_iter it = tokens.begin();

	while (it != tokens.end())
	{
		if (*it == "server")
			fillServerData(it);
		else
		{
			string key = it->substr(0, str_utils::find_first_of(*it, " \t"));
			if (isValideDirective(key))
				throw CustomException("directive is not allowed here", key);
			throw CustomException("not a valide directive", key);
		}
	}
}

void	Parser::tokenizer() 
{
	string line;
	for (size_t i = 0; i < dataToParse.size();i++)
	{
		if (!strchr(";{}", dataToParse[i]))
			line += dataToParse[i];
		else
		{
			if (!str_utils::hasSpaceOnly(line))
				tokens.push_back(str_utils::trim(line));
			line.clear();
			line += dataToParse[i];
			tokens.push_back(line);
			line.clear();
		}
	}
}

Parser::Parser(const string& fileName)
{
	fileValideDirectives();
	string line;

	if (!str_utils::endsWith(fileName, ".conf"))
		throw CustomException("File must end with [.conf] extension!!", fileName);
	std::ifstream configFile(fileName.c_str());
	if (configFile.fail())
		throw CustomException("Failed to open file!!", fileName);
	while (std::getline(configFile, line, '\n'))
	{
		if (line.size() && line.find("#") != string::npos)
			line = line.substr(0, line.find("#"));
		str_utils::trim(line);
		if (line.empty() || line[0] == '#')
			continue ;
		dataToParse += line;
		dataToParse += " ";
	}
	tokenizer();
	analyzer();
}

Parser& Parser::operator=(const Parser& _assignment)
{
	if (this != &_assignment)
		servers = _assignment.servers;
	return (*this);
}

Server Parser::getDefaultServer() const
{
	if (servers.size())
		return (servers.front());
	throw CustomException("No Server Found!!");
}
























void	printPorts(std::vector<int> ports)
{
	VecInt_iter it = ports.begin();
	cout << "ports: " << endl;
	for (;it != ports.end();it++)
		cout << "\t" << *it << endl;
}

void	printCommonDirectives(const CommonDirectives& common)
{
	cout << "root: [" << common.getRoot() << "]" << endl;
	cout << "alias: [" << common.getAlias() << "]" << endl;
	cout << "index: " << endl;
	VecString indexes = common.getAllIndexes();
	VecString_iter it1 = indexes.begin();
	for (; it1 != indexes.end();it1++)
		cout << "\t" << *it1 << endl;

	cout << "try_files: " << endl;
	VecString tryFiles = common.getAllTryFiles();
	VecString_iter it2 = tryFiles.begin();
	for (; it2 != tryFiles.end();it2++)
		cout << "\t" << *it2 << endl;

	cout << "autoindex: [" << common.getAutoIndex() << "]" << endl;
	cout << "client_max_body_size: [" << common.getClientMaxBodySize() << "]" << endl;
	cout << "error_log: [" << common.getErrorLog() << "]" << endl;
	cout << "access_log: [" << common.getAccessLog() << "]" << endl;

	cout << "error_pages: " << endl;
	MapIntString errorPages = common.getErrorPages();
	MapIntString_iter it3 = errorPages.begin();
	for (; it3 != errorPages.end();it3++)
		cout << "\t" << it3->first << ": " << it3->second << endl;

	cout << "allowed_methods: " << endl;
	VecString allowedMethods = common.getAllowedMethods();
	VecString_iter it4 = allowedMethods.begin();
	for (; it4 != allowedMethods.end();it4++)
		cout << "\t" << *it4 << endl;
}

void	printLocation(Locations locations)
{
	Locations::iterator it = locations.begin();
	cout << "locations: " << endl;
	for (;it != locations.end();it++)
	{
		cout << "=====================>path: " << it->first << endl;
		printCommonDirectives(it->second);
		cout << "=====================>" << endl;
	}
}

void	Parser::dump()
{
	std::vector<Server>::iterator it = servers.begin();
	for (int i = 0; it != servers.end();it++)
	{
		cout << "==========> Server[" << i << "] <==========" << endl;
		cout << "server name: [" << it->getServerName() << "]" << endl;
		cout << "host: [" << it->getHost() << "]" << endl;
		printPorts(it->getPorts());
		printCommonDirectives(*it);
		printLocation(it->getLocations());
		i++;
	}
}
