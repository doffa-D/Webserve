/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 09:31:57 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/15 13:13:11 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "Parser.hpp"
#include "../../../include/header.hpp"


Parser::Parser(){}
Parser::~Parser() {}
Parser::Parser(const Parser& _copy) {*this = _copy;}

void	Parser::fillValideDirectives()
{
	allowed_directives = str_utils::split(ALLOWED_DIRECTIVES, ' ');
	location_valid_directives = str_utils::split(LOCATION_DIRECTIVES, ' ');
}

bool	Parser::isValideDirective(const string& _directive)
{
	vector<string>::iterator it = allowed_directives.begin();
	for (; it < allowed_directives.end(); it++)
		if (*it == _directive)
			return (true);
	return (false);
}

bool	Parser::isValideForLocation(const string& key)
{
	for (size_t i = 0; i < location_valid_directives.size(); i++)
		if (key == location_valid_directives[i])
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
	std::string  keys[12] = {"root", "alias", "index", "autoindex",  "client_max_body_size", \
							 "error_log", "access_log", "error_page", "allowed_method", "upload", \
							 "redirection", "cgi"};

	void	(CommonDirectives::*functionPtr[12])(const string&) = 
			{&CommonDirectives::setRoot, &CommonDirectives::setAlias, &CommonDirectives::setIndex, 
			&CommonDirectives::setAutoIndex, &CommonDirectives::setClientMaxBodySize, 
			&CommonDirectives::setErrorLog, &CommonDirectives::setAccessLog, &CommonDirectives::addErrorPage,
			&CommonDirectives::setAllowedMethod, &CommonDirectives::setUpload, &CommonDirectives::setRedirection,
			&CommonDirectives::setCgi};

	int i = 0;
	while (i < 12 && key != keys[i])
		i++;
	if (i < 12)
		(common.*functionPtr[i])(value);
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
				if (it == tokens.end())
					throw CustomException("unexpected end of file, expecting \";\" or \"}\"");
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
	else if (key == "listen")
	{
		ok = server.AddIpPort(value);
		if (ok)
			server.setAsDefaultServer();
	}
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
				it++;
				if (it == tokens.end())
					throw CustomException("unexpected end of file, expecting \";\" or \"}\"");
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
	vector<Server>::iterator it = servers.begin();
	for (; it != servers.end();it++)
	{
		checkFiles(*it);
		Locations locations = it->getLocations();
		Locations::iterator it2 = locations.begin();
		for (;it2 != locations.end();it2++)
			checkFiles(it2->second);
	}
}

Parser::Parser(int ac, char**av)
{
	string line;
	string fileName;
	if (ac > 2)
		throw CustomException("Usage: \n\t./webserv\n\t./webserv [configuration file]");
	else if (ac == 2)
		fileName = av[1];
	else
		fileName = "ConfileFiles/default.conf";
	if (!str_utils::endsWith(fileName, ".conf"))
		throw CustomException("File must end with [.conf] extension!!", fileName);
	fillValideDirectives();
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

Uint	Parser::getIPv4FromDns(const string& _dns)
{
	struct addrinfo hints, *results, *ptr;
	Uint IpAddr;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if (getaddrinfo(_dns.c_str(), NULL, &hints, &results) != 0)
		return (-1);
	IpAddr = -1;
	for (ptr = results; ptr; ptr = ptr->ai_next)
		IpAddr = ntohl(((struct sockaddr_in *) ptr->ai_addr)->sin_addr.s_addr);
	freeaddrinfo(results);
	return (IpAddr);
}

pair<Uint, int>	Parser::parseHost(const string& _host)
{
	VecString	split = str_utils::split(_host, ':');
	pair<Uint, int> ip_port;
	ip_port.first = getIPv4FromDns(split[0]);
	ip_port.second = 80;
    if(split.size() == 2)
        ip_port.second = str_utils::to_int(split[1]);
    return (ip_port);
}

Server	Parser::findServer(vector<Server>& _servers, const string& _host)
{
	if (_servers.front().isDefaultServer())
			return (_servers.front());
	Server s = Server::createNullObject();
	pair<Uint, int> ip_port = parseHost(_host);
	vector<Server>::iterator it = _servers.begin();
	for (;it != _servers.end();it++)
	{
		IpPorts	_IpPorts = it->getIpPorts();
		for (size_t i = 0; i < _IpPorts.size();i++)
			if (_IpPorts[i].first == ip_port.first && _IpPorts[i].second == ip_port.second)
				return (*it);
	}
	return (s);
}

Server	Parser::getServerbyHost(const string& _host)
{
	Server s = Server::createNullObject();
	if (_host.empty())
		return (s);

	vector<Server> _s;
	for (size_t i = 0; i < servers.size();i++)
	{
		VecString server_names = servers[i].getServerNames();
		if (find(server_names.begin(), server_names.end(), _host) != server_names.end())
			_s.push_back(servers[i]);
	}

	if (!_s.size())
		return (findServer(servers, _host));
	if (_s.size() == 1)
		return (_s.front());
	else
		return (findServer(_s, _host));
    return (s);
}

vector<Server>	Parser::getServers() const {return (servers);}

Server				Parser::getDefaultServer() const
{
	Server s = Server::createNullObject();
	if (servers.size())
		return (servers.front());
	return (s);
}

IpPorts	Parser::getHostsAndPorts()
{
	IpPorts	Hosts;
	for (size_t i = 0; i < servers.size();i++)
	{
		IpPorts _ip_ports = servers[i].getIpPorts();
		for (size_t j = 0; j < _ip_ports.size(); j++)
			Hosts.push_back(std::make_pair(_ip_ports[j].first, _ip_ports[j].second));
	}
	return (Hosts);
}





















void	printHosts(IpPorts ports)
{
	IpPorts::iterator it = ports.begin();
	cout << "Hosts: " << endl;
	for (;it != ports.end();it++)
		cout << "\t" << "ip: " << str_utils::ip(it->first) << " port: " << it->second <<  endl;
}

void	printCommonDirectives(const CommonDirectives& common)
{
	cout << "root: [" << common.getRoot() << "]" << endl;
	cout << "alias: [" << common.getAlias() << "]" << endl;
	cout << "index: " << endl;
	VecString indexes = common.getIndexes();
	for (VecString_iter it = indexes.begin(); it != indexes.end();it++)
		cout << "\t" << *it << endl;

	cout << "redirection: " << endl;
	pair<int, string>	redirection = common.getRedirection();
	cout << "\tstatus: " << redirection.first << " path: " << redirection.second << endl;
	

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
	
	cout << "upload: [" << common.getUpload() << "]" << endl;
	
	cout << "Cgi: " << endl;
	MapStringString cgi = common.getCgi();
	for (MapStringString_iter it = cgi.begin(); it != cgi.end();it++)
		cout << "\t" << it->first << " => " << it->second << endl;
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
	vector<Server>::iterator it = servers.begin();
	for (int i = 0; it != servers.end();it++)
	{
		cout << "==========================> Server[" << i << "] <==========================" << endl;
		cout << "server name: " << endl;
		VecString server_names = it->getServerNames();
		for (VecString_iter it = server_names.begin(); it != server_names.end();it++)
			cout << "\t" << *it << endl;

		printHosts(it->getIpPorts());
		printCommonDirectives(*it);
		printLocation(it->getLocations());
		i++;
	}
}
