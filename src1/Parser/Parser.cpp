#include "Parser.hpp"

Parser::Parser( void )
{
}

void	Parser::checkSyntax( void )
{
	int	openBrackets = 0;
	std::vector<String> server;
	bool	insideServer = false;
	std::vector<String>::iterator iterBegin = fileContent.begin();
	std::vector<String>::iterator iterEnd = fileContent.end();

	while (iterBegin < iterEnd)
	{
		openBrackets += iterBegin->countRepeating('{');
		openBrackets -= iterBegin->countRepeating('}');
		if (!iterBegin->compare(0, 7, "server "))
		{
			insideServer = !insideServer;
			iterBegin++;
			continue ;
		}
		if (insideServer == true)
		{
			if (iterBegin->countRepeating('{') || iterBegin->countRepeating('}'))
			{
				iterBegin++;
				continue ;
			}
			if (*(iterBegin->end() - 1) != ';')
			{
				String message;
				message.append("[emerg] invalid parameter \"").append(*iterBegin).append("\" in ").append(fileName);
				message.append(" test failed.");
				throw (ParsingException(message));
			}
		}
		iterBegin++;
	}
	if (openBrackets)
	{
		String message;
		message.append("webserv: [emerg] unexpected end of file, expecting \"").append((openBrackets < 0) ? "{" : "}").append("\" in ").append(fileName);
		message.append("\nwebserv: configuration file ").append(fileName).append(" test failed.");
		throw (ParsingException(message));
	}
}

Parser::Parser(String _fileName) : fileName(_fileName)
{
	getFileContent();
	checkSyntax();
	splitContentIntoServers();
	getFinalResualt();
}

Parser::Parser(const Parser& copy)
{
	*this = copy;
}

Parser::~Parser( void )
{
	try
	{
		std::vector<ServerModel>::iterator b = servers.begin();
		std::vector<ServerModel>::iterator e = servers.end();
		while (b < e)
		{
			std::vector<Location*>::const_iterator locBegin = b->getLocation().begin();
			std::vector<Location*>::const_iterator locEnd = b->getLocation().end();
			while (locBegin < locEnd)
			{
				freeLocations(*locBegin);
				delete *locBegin;
				locBegin++;
			}
			b++;
		}
	}
	catch (...)
	{
		std::cout << "catching Exception in Parser detructor(~Parser)." << std::endl;
	}
}

Parser& Parser::operator=(const Parser& target)
{
	if (this != &target)
	{
		fileName = target.fileName;
		servers = target.servers;
		serversContents = target.serversContents;
	}
	return (*this);
}

const std::vector<std::vector<String> >&	Parser::getServersContents( void ) const
{
	return (serversContents);
}

const	std::vector<ServerModel>&	Parser::getServers( void ) const
{
	return (servers);
}

Data	Parser::extractDataFromString(String& line)
{
	std::vector<String> vec = line.split();
	std::vector<String>::iterator ib = vec.begin();
	std::vector<String>::iterator ie = vec.end();
	String	key = *ib;
	String	value("");
	while (++ib < ie)
		value.append(" ").append(ib->trim(" \t;"));
	return (Data(key, value));
}

void	Parser::printLocations(Location* locs)
{
	static String s;
	if (!locs)
		return ;
	std::cout << s << "Path : "<< locs->getPath() << std::endl;
	std::vector<Data> vec = locs->getAllData();
	std::vector<Data>::iterator iBegin = vec.begin();
	std::vector<Data>::iterator iEnd = vec.end();
	while (iBegin != iEnd)
	{
		std::cout << s << "\tKey = " << iBegin->getKey() << "  Value = " << iBegin->getValue() << std::endl;
		iBegin++;
	}
	std::vector<Location*> *innerLoc = locs->getInnerLocation();
	std::vector<Location*>::iterator b = innerLoc->begin();
	std::vector<Location*>::iterator e = innerLoc->end();
	while (b < e)
	{
		s.append("\t");
		printLocations(*b);
		s.erase(s.length() - 1);
		b++;
	}
}

Location*	Parser::getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path)
{
	std::vector<Location*>* newLocation = new std::vector<Location*>;
	GlobalModel model;
	while (begin < end)
	{
		if (!begin->compare(0, 1, "}"))
		{
			begin++;
			break ;
		}
		if (!begin->compare(0, 9, "location "))
		{
			String _path = extractDataFromString(*begin).getValue();
			newLocation->push_back(getLocations(++begin, end, _path.trim(" {")));
		}
		else
		{
			model.addData(extractDataFromString(*begin));
			begin++;
		}
	}
	return (new Location(model, path, newLocation));
}

void	Parser::getFileContent( void )
{
	String	tmp;
	size_t	pos;
	std::ifstream outfile(fileName.c_str());
	// Check File is opened or not.
	if (outfile.is_open() == false)
	{
		outfile.close();
		// throw exception with the same nginx message.
		String message;
		message.append("webserv: [emerg] open \"").append(fileName).append("\" failed (No such file or directory)");
		message.append("\nwebserv: configuration file \"").append(fileName).append("\" test failed.");
		throw (ParsingException(message));
	}
	// read data from file and store it in vector of String.
	while (!outfile.eof())
	{
		std::getline(outfile, tmp, '\n');
		tmp.trim(" \t");
		if (tmp.size() == 0 || *tmp.begin() == '#')
			continue ;

		// this part for remove comment inside strings
		{
			pos = tmp.find('#');
			if (pos != String::npos)
				tmp.erase(pos);
		}
		// pushing data into vector
		fileContent.push_back(tmp);
	}
	outfile.close();
}


std::vector<String>	Parser::getServerConfig(std::vector<String>::iterator& iterBegin, const std::vector<String>::iterator& iterEnd)
{
	int	openBrackets = 0;
	std::vector<String> server;
	bool	insideServer = false;

	while (iterBegin < iterEnd)
	{
		openBrackets += iterBegin->countRepeating('{');
		openBrackets -= iterBegin->countRepeating('}');
		if (!openBrackets)
		{
			iterBegin++;
			break ;
		}
		if (!iterBegin->compare(0, 7, "server "))
		{
			insideServer = !insideServer;
			iterBegin++;
			continue ;
		}
		if (insideServer == true)
			server.push_back(iterBegin->trim(" \t"));
		iterBegin++;
	}
	return server;
}

void	Parser::parsingFile(std::vector<String> content)
{
	ServerModel server;
	std::vector<String>::iterator iBegin = content.begin();
	std::vector<String>::iterator iEnd = content.end();
	while (iBegin < iEnd)
	{
		if (iBegin->compare(0, 9, "location "))
		{
			server.addData(extractDataFromString(*iBegin));
			iBegin++;
		}
		else
		{
			String _path = extractDataFromString(*iBegin).getValue();
			server.addLocation(getLocations(++iBegin, iEnd, _path.trim(" {")));
		}
	}
	servers.push_back(server);
}

void	Parser::splitContentIntoServers( void )
{
	std::vector<String>::iterator begin = fileContent.begin();
	const std::vector<String>::iterator end = fileContent.end();
	while (begin < end)
		serversContents.push_back(getServerConfig(begin, end));
}


void	Parser::freeLocations(Location* locs)
{
	if (!locs)
		return ;
	std::vector<Location*> *innerLoc = locs->getInnerLocation();
	std::vector<Location*>::iterator b = innerLoc->begin();
	std::vector<Location*>::iterator e = innerLoc->end();
	while (b < e)
	{
		freeLocations(*b);
		delete *b;
		*b = NULL;
		b++;
	}
}

void	Parser::printServerModel(ServerModel& server)
{
	std::vector<Location*>::const_iterator b = server.getLocation().begin();
	std::vector<Location*>::const_iterator e = server.getLocation().end();
	while (b < e)
	{
		printLocations(*b);
		freeLocations(*b);
		b++;
	}
}


void	Parser::getFinalResualt( void )
{
	std::vector<std::vector<String> >::iterator begin = serversContents.begin();
	std::vector<std::vector<String> >::iterator end = serversContents.end();
	while (begin < end)
	{
		parsingFile(*begin);
		begin++;
	}
}

std::vector<Data>	Parser::parseHeader(const String& header)
{
	(void)header;
	std::vector<Data> vec;
	std::istringstream	iss(header);
	String			tmp, key, value;
	while (iss.eof() == false)
	{
		std::getline(iss, tmp, '\n');
		if (tmp.length() == 0 || tmp[0] == '\r')
			continue ;
		Logger::info(std::cout, tmp, "");
		std::vector<String> split = tmp.split(':');
		if (split.empty() == true)
			continue ;
		key = split[0];
		if (split.size() > 1)
			value = String::convertVectorToString(split, 1, static_cast<unsigned int>(split.size() - 1), ':');
		vec.push_back(Data(key.trim(" \t"), value.trim(" \t")));
	}
	return (vec);
}



