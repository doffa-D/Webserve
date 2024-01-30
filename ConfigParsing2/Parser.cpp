/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 09:31:57 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/30 15:26:10 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"

Parser::Parser(){}
Parser::~Parser() {}
Parser::Parser(const Parser& _copy) {*this = _copy;}

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
    validDirectives.push_back("include");
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
	string validDirectives[12] = {"root", "try_files", "index",
								 "location", "alias", "allowed_method",
								 "autoindex", "client_max_body_size", 
								 "error_log", "access_log", "error_pages",
								 "include"};
	for (size_t i = 0; i < 12; i++)
		if (key == validDirectives[i])
			return (true);
	return (false);
}

void	parseMimeTypes(CommonDirectives& common, const string& filePath)
{
	string line;
	std::ifstream mimeTypeFile(filePath.c_str());
	if (mimeTypeFile.fail())
		throw CustomException("Failed to open file!!", filePath);
	while (std::getline(mimeTypeFile, line, '\n'))
	{
		str_utils::trim(line);
		if (line.empty())
			continue ;
		string	key = line.substr(0, str_utils::find_first_of(line, " \t"));
		string	value = str_utils::trim(line.substr(str_utils::find_first_of(line, " \t"), line.length()));
		common.addMimeType(key, value);
	}
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
	else if (key == "include")
		parseMimeTypes(common, value);
}

void	Parser::fillLocationDirective(Server& server, CommonDirectives& old_location, ListString_iter& it, const string& befor)
{
	string	path = str_utils::trim(it->substr(str_utils::find_first_of(*it, " \t"), it->length()));

	if (befor != "" && !strnstr(path.c_str(), befor.c_str(), befor.length()))
		throw CustomException("location \"" + path + "\" is outside location \"" + befor + "\"");

	if (*(++it) != "{")
		throw CustomException("Directive \"location\" has no opening \"{\"");
	++it;
	Location location(old_location);

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
		server.setIpAddress(value);
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
				fillLocationDirective(server, server, it, "");
			else
			{
				fillDirectives(server, it, hasDefualtTage);
				if (it == tokens.end())
					throw CustomException("unexpected end of file, expecting \";\" or \"}\"");
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
	size_t i = 0;
	while (i < dataToParse.size())
	{
		if (!strchr(";{}", dataToParse[i]))
		{
			while (i < dataToParse.size() && !strchr(";{}", dataToParse[i]))
			{
				if (strchr("\"\'", dataToParse[i]))
				{
					string s = strchr("\"\'", dataToParse[i]);
					if (i < dataToParse.size()) line += dataToParse[i++];
					while (i < dataToParse.size() && dataToParse[i] != s[0]) line += dataToParse[i++];
					if (i < dataToParse.size()) line += dataToParse[i++];
				}
				else
					line += dataToParse[i++];
			}
		}
		else
			line += dataToParse[i++];
		if (!str_utils::hasSpaceOnly(line))
			tokens.push_back(str_utils::trim(line));
		line.clear();
	}
}

void	checkFiles(CommonDirectives common)
{
	if (common.getAccessLog() != ACCESS_LOG)
			if (str_utils::createFile(common.getAccessLog()))
				throw CustomException("open() \"" + common.getAccessLog() + "\" failed (No such file or directory)");
	if (common.getErrorLog() != ERROR_LOG)
		if (str_utils::createFile(common.getErrorLog()))
			throw CustomException("open() \"" + common.getErrorLog() + "\" failed (No such file or directory)");
}

void	Parser::createFiles()
{
	std::vector<Server>::iterator it = servers.begin();
	for (; it != servers.end();it++)
	{
		checkFiles(*it);
		Locations locations = it->getLocations();
		Locations::iterator it2 = locations.begin();
		for (;it2 != locations.end();it2++)
			checkFiles(it2->second);
	}
}

// Parser::Parser(int ac, char**av)
// {
// 	string fileName = av[1];
// 	string line;
// 	if (ac != 2)
// 		throw CustomException("Usage: \n\t./webserv [configuration file]");
// 	if (!str_utils::endsWith(fileName, ".conf"))
// 		throw CustomException("File must end with [.conf] extension!!", fileName);
// 	fileValideDirectives();
// 	std::ifstream configFile(fileName.c_str());
// 	if (configFile.fail())
// 		throw CustomException("Failed to open file!!", fileName);
// 	while (std::getline(configFile, line, '\n'))
// 	{
// 		if (line.size() && line.find("#") != string::npos)
// 			line = line.substr(0, line.find("#"));
// 		str_utils::trim(line);
// 		if (line.empty() || line[0] == '#')
// 			continue ;
// 		dataToParse += line;
// 		dataToParse += " ";
// 	}
// 	tokenizer();
// 	analyzer();
// 	createFiles();
// }

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
	createFiles();
}


Parser& Parser::operator=(const Parser& _assignment)
{
	if (this != &_assignment)
		servers = _assignment.servers;
	return (*this);
}

std::vector<Server>	Parser::getServers() const {return (servers);}
Server				Parser::getServerbyHost(const string& _host)
{
	int				port = 80;
	VecString		split = str_utils::split(_host, ':');
	if(split.size() > 1)
		port = str_utils::to_int(split[1]);
	unsigned int	ip;
	Server s = Server::createNullObject();
	if (split[0] == "localhost")
		ip = str_utils::ip(127, 0, 0, 1);
	else
	{
		VecString	ip_values = str_utils::split(split[0], '.');
		if (ip_values.size() != 4)
			return (s);
		ip = str_utils::ip(str_utils::to_int(ip_values[0]),
						   str_utils::to_int(ip_values[1]),
						   str_utils::to_int(ip_values[2]),
						   str_utils::to_int(ip_values[3]));
	}
	std::vector<Server>::iterator it = servers.begin();
	for (;it != servers.end();it++)
	{
		VecInt ports = it->getPorts();
		// std::cout << port << std::endl;
		// ila kane 3aks waslat 127.0.0.1 o 7na fi confile mdfinine server_name so khas server_name it7awl 127.0.0.1
		if (it->getIpAddress() == ip && find(ports.begin(), ports.end(), port) != ports.end())
		{
			// std::cout << "found server " << port << std::endl;
			return (*it);
		}
	}
	return (s);
}

Server				Parser::getDefaultServer() const
{
	Server s = Server::createNullObject();
	if (servers.size())
		return (servers.front());
	return (s);
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
	VecString indexes = common.getIndexes();
	for (VecString_iter it = indexes.begin(); it != indexes.end();it++)
		cout << "\t" << *it << endl;

	cout << "try_files: " << endl;
	VecString tryFiles = common.getTryFiles();
	for (VecString_iter it = tryFiles.begin(); it != tryFiles.end();it++)
		cout << "\t" << *it << endl;

	cout << "autoindex: [" << common.getAutoIndex() << "]" << endl;
	cout << "client_max_body_size: [" << common.getClientMaxBodySize() << "]" << endl;
	cout << "error_log: [" << common.getErrorLog() << "]" << endl;
	cout << "access_log: [" << common.getAccessLog() << "]" << endl;

	cout << "error_pages: " << endl;
	MapIntString errorPages = common.getErrorPages();
	for (MapIntString_iter it= errorPages.begin(); it!= errorPages.end();it++)
		cout << "\t" << it->first << ": " << it->second << endl;

	cout << "allowed_methods: " << endl;
	VecString allowedMethods = common.getAllowedMethods();
	for (VecString_iter it = allowedMethods.begin(); it != allowedMethods.end();it++)
		cout << "\t" << *it << endl;

	cout << "types: " << endl;
	MapStringString types = common.getMimeTypes();
	for (MapStringString_iter it = types.begin(); it != types.end();it++)
		cout << "\t" << it->first << ": " << it->second << endl;
}

void	printLocation(Locations locations)
{
	Locations::iterator it = locations.begin();
	cout << "locations: " << endl;
	for (;it != locations.end();it++)
	{
		cout << "---------------->path: " << it->first << endl;
		printCommonDirectives(it->second);
		cout << "<----------------" << endl;
	}
}

void	Parser::dump()
{
	std::vector<Server>::iterator it = servers.begin();
	for (int i = 0; it != servers.end();it++)
	{
		cout << "==========================> Server[" << i << "] <==========================" << endl;
		cout << "server name: [" << it->getServerName() << "]" << endl;
		cout << "host: [" << it->getIpAddress() << "]" << endl;
		printPorts(it->getPorts());
		printCommonDirectives(*it);
		printLocation(it->getLocations());
		i++;
	}
}
