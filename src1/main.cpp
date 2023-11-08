#include <fstream>
#include "Parser/Location.hpp"
#include "Parser/Parser.hpp"
#include "Models/ServerModel.hpp"
#include "Server/Server.hpp"
#include "Utils/ServerData.hpp"
#include "Utils/Logger.hpp"
#include <cctype>
#include <unistd.h>


void	runServer(Server& server)
{
	int res, fd, newsocket;
	String	header;

	while (1)
	{
		res = server.waitingRequest();
		if (!res)
			continue ;
		if (res < 0)
			throw (std::exception());
		fd = server.getAvailabeFD();
		if (fd < 0)
			throw (std::exception());	
		newsocket = server.accept(fd);
		if (newsocket < 0)
			continue ;
		header = server.recieve(newsocket);
		if (header.empty() == true)
			break ;
		Parser::parseHeader(header);
		if (server.send(newsocket, "HTTP/1.1 200 ok\r\n\r\n<h1>hello world</h1>") == -1)
			Logger::error(std::cerr, "Send Failed.", "");
		close(newsocket);
		header.clear();
	}
}

void	createServer(const ServerModel& serv)
{
	std::vector<Data> data = serv.getData("listen");
	std::vector<Data>::iterator ibegin = data.begin();
	std::vector<Data>::iterator iend = data.end();
	Server server;
	while (ibegin < iend)
	{
		unsigned short port = (unsigned short)strtol(ibegin->getValue().c_str(), NULL, 10);
		Logger::debug(std::cout, "Port : ", port);
		if (server.createNewSocket(port) == false)
			std::cout << "can not open the port : " << port << std::endl;
		ibegin++;
	}
	runServer(server);
}

void	printAllData(Parser& parser)
{
	__unused ServerData servers(parser.getServers());
	try
	{
		ServerModel smodel = servers.getServer("mehdi.com");
		ServerModel::printServerModelInfo(smodel);
		createServer(smodel);
	}
	catch ( ... )
	{
		std::cout << "I can't found the exact server." << std::endl;
	}
}

void	testLeaks(char *fileName)
{
	try
	{
		Parser* parser = new Parser(fileName);
		String str("the configuration file");
		str.append(fileName);
		Logger::success(std::cout, str, " syntax is ok.");
		Logger::success(std::cout, str, " test is successfuli.");
		printAllData(*parser);
		delete parser;
	}
	catch (ParsingException& e)
	{
		Logger::error(std::cerr, e.what(), "");
	}
}

int	main(int ac, char **av)
{
	if (ac < 2)
	{
		std::cerr << "Error :\fInvalid argument" << std::endl;
		return (1);
	}
	testLeaks(av[1]);
	return (0);
}
