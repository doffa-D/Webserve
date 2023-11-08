#ifndef	PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include "Location.hpp"
#include <cctype>
#include "../Exception/ParsingException.hpp"
#include "../Models/ServerModel.hpp"
#include "../Utils/Logger.hpp"

class Parser
{
	String	fileName;
	std::vector<String>	fileContent;
	std::vector<std::vector<String> >	serversContents;
	std::vector<ServerModel> servers;
	Parser( void );

	public :
		Parser(String _fileName);
		Parser(const Parser& copy);
		~Parser( void );
		Parser& operator=(const Parser& target);
		const std::vector<std::vector<String> >& getServersContents( void ) const;
		const	std::vector<ServerModel>&	getServers( void ) const;
		void	printServerModel(ServerModel& server);
		static std::vector<Data>	parseHeader(const String& header);
	
	private :
		Data	extractDataFromString(String& line);
		void	 printLocations(Location* locs);
		Location*	 getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path);
		void	 getFileContent( void );
		std::vector<String>	 getServerConfig(std::vector<String>::iterator& iterBegin, const std::vector<String>::iterator& iterEnd);
		void	parsingFile(std::vector<String> content);
		void	splitContentIntoServers( void );
		void	freeLocations(Location* locs);
		void	getFinalResualt( void );
		void	checkSyntax( void );
};

#endif
