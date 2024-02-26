/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/09 09:31:57 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/25 18:19:59 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	VecString vec = str_utils::proSplit(filePath);
	if (vec.size() > 1)
		throw CustomException("invalid number of arguments in \"include\" directive", filePath);
	common.check_unexpected(vec[0], "include");
	string _filePath = str_utils::remove_quotes(filePath);
	std::ifstream mimeTypeFile(_filePath.c_str());
	if (mimeTypeFile.fail())
		throw CustomException("Failed to open file!!", _filePath);
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
	std::string  keys[11] = {"root", "index", "autoindex",  "client_max_body_size", \
							 "error_log", "access_log", "error_page", "allowed_method", "upload", \
							 "redirection", "cgi"};

	void	(CommonDirectives::*functionPtr[11])(const string&) = 
			{&CommonDirectives::setRoot, &CommonDirectives::setIndex, 
			&CommonDirectives::setAutoIndex, &CommonDirectives::setClientMaxBodySize, 
			&CommonDirectives::setErrorLog, &CommonDirectives::setAccessLog, &CommonDirectives::addErrorPage,
			&CommonDirectives::setAllowedMethod, &CommonDirectives::setUpload, &CommonDirectives::setRedirection,
			&CommonDirectives::setCgi};

	int i = 0;
	while (i < 11 && key != keys[i])
		i++;
	if (i < 11)
		(common.*functionPtr[i])(value);
	else if (key == "include")
		parseMimeTypes(common, value);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*needle || haystack == needle)
		return ((char *)haystack);
	while (haystack[i] && len != 0)
	{
		j = 0;
		while (haystack[i + j] == needle[j] && i + j < len)
		{
			if (needle[j + 1] == '\0')
				return ((char *)&haystack[i]);
			if (haystack[i + j + 1] != needle[j + 1])
				break ;
			j++;
		}
		i++;
	}
	return (0);
}

void	Parser::fillLocationDirective(Server& server, CommonDirectives& old_location, ListString_iter& it, const string& befor)
{
	string	path = str_utils::trim(it->substr(str_utils::find_first_of(*it, " \t"), it->length()));

	if (befor != "" && !ft_strnstr(path.c_str(), befor.c_str(), befor.length()))
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
				if (key == "alias")
					location.setAlias(value);
				else
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
	else if (key == "client_max_header_buffer_size")
		server.setClientMaxHeaderBufferSize(value);
	else if (key == "listen")
	{
		ok = server.AddIpPort(value);
		if (ok)
			server.setAsDefaultServer();
	}
	else if (key == "alias")
		throw CustomException("directive is not allowed inside server", key);
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
				else if (*it == "}")
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
		fileName = DEFUALT_CONFIG_PATH;
	if (!str_utils::endsWith(fileName, ".conf"))
		throw CustomException("File must end with [.conf] extension!!", fileName);
	fillValideDirectives();
	std::ifstream configFile(fileName.c_str());
	if (configFile.fail())
	{
		std::cout << "no is here \n";
		throw CustomException("Failed to open file!!", fileName);
	}
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

Uint	Parser::getIPv4FromDns(const string& _dns) const
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

pair<Uint, int>	Parser::parseHost(const string& _host) const
{
	VecString	split = str_utils::split(_host, ':');
	pair<Uint, int> ip_port;
	ip_port.first = getIPv4FromDns(split[0]);
	ip_port.second = 80;
    if(split.size() == 2)
        ip_port.second = str_utils::to_int(split[1]);
    return (ip_port);
}

Server	Parser::findServer(vector<Server>& _servers, const string& _host) const
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

Server	Parser::getServerbyHost(const string& _host) const
{
	Server s = Server::createNullObject();
	if (_host.empty())
		return (s);

	vector<Server> _s;
	vector<Server> _servers = servers;
	for (size_t i = 0; i < _servers.size();i++)
	{
		VecString server_names = _servers[i].getServerNames();
		if (find(server_names.begin(), server_names.end(), _host) != server_names.end())
			_s.push_back(_servers[i]);
	}

	if (!_s.size())
		return (findServer(_servers, _host));
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

IpPorts	Parser::getHostsAndPorts() const
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

/*==============>print parsed config data<==============*/
void	printHosts(IpPorts ports)
{
	IpPorts::iterator it = ports.begin();
	cout << "Hosts: " << endl;
	for (;it != ports.end();it++)
		cout << "\t" << "ip: " << str_utils::ip(it->first) << " port: " << it->second <<  endl;
}

void	printCommonDirectives(const CommonDirectives& common, string befor)
{
	cout << befor << "root: [" << common.getRoot() << "]" << endl;
	cout << befor << "index: " << endl;
	VecString indexes = common.getIndexes();
	for (VecString_iter it = indexes.begin(); it != indexes.end();it++)
		cout << befor << "\t" << *it << endl;

	cout << befor << "redirection: " << endl;
	pair<int, string>	redirection = common.getRedirection();
	cout << befor << "\tstatus: " << redirection.first << " path: " << redirection.second << endl;
	

	cout << befor << "autoindex: [" << common.getAutoIndex() << "]" << endl;
	cout << befor << "client_max_body_size: [" << common.getClientMaxBodySize() << "]" << endl;
	cout << befor << "error_log: [" << common.getErrorLog() << "]" << endl;
	cout << befor << "access_log: [" << common.getAccessLog() << "]" << endl;

	cout << befor << "error_pages: " << endl;
	MapIntString errorPages = common.getErrorPages();
	for (MapIntString_iter it= errorPages.begin(); it!= errorPages.end();it++)
		cout << befor << "\t" << it->first << ": " << it->second << endl;

	cout << befor << "allowed_methods: " << endl;
	VecString allowedMethods = common.getAllowedMethods();
	for (VecString_iter it = allowedMethods.begin(); it != allowedMethods.end();it++)
		cout << befor << "\t" << *it << endl;

	cout << befor << "types: " << endl;
	MapStringString types = common.getMimeTypes();
	for (MapStringString_iter it = types.begin(); it != types.end();it++)
		cout << befor << "\t" << it->first << ": " << it->second << endl;
	
	cout << befor << "upload: [" << common.getUpload() << "]" << endl;
	
	cout << befor << "Cgi: " << endl;
	MapStringString cgi = common.getCgi();
	for (MapStringString_iter it = cgi.begin(); it != cgi.end();it++)
		cout << befor << "\t" << it->first << " => " << it->second << endl;
}

void	printLocation(Locations locations)
{
	Locations::iterator it = locations.begin();
	cout << "locations: " << endl;
	for (;it != locations.end();it++)
	{
		cout << "--->path: " << it->first << endl;
		cout << "\talias: [" << it->second.getAlias() << "]" << endl;
		printCommonDirectives(it->second, "\t");
		cout << "<----------------" << endl;
	}

}

void	Parser::dump()
{
	vector<Server>::iterator it = servers.begin();
	for (int i = 0; it != servers.end();it++)
	{
		cout << "==========================> Server[" << i << "] <==========================" << endl;
		cout << "client_max_header_buffer_size: [" << it->getClientMaxHeaderBufferSize() << "]" << endl;
		cout << "server name: " << endl;
		VecString server_names = it->getServerNames();
		for (VecString_iter it = server_names.begin(); it != server_names.end();it++)
			cout << "\t" << *it << endl;

		printHosts(it->getIpPorts());
		printCommonDirectives(*it, "");
		printLocation(it->getLocations());
		i++;
	}
}
