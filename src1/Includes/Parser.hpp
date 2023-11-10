#ifndef	PARSER_HPP
#define PARSER_HPP

#include <iostream>
#include <fstream>
#include "Location.hpp"
#include <cctype>
#include "ParsingException.hpp"
#include "ServerModel.hpp"

class Parser
{
	String	fileName;
	std::vector<String>	fileContent;
	std::vector<std::vector<String> >	serversContents;
	std::vector<ServerModel> servers;

	Parser( void );

	public :
		Parser(const String& _fileName);
		Parser(const Parser& copy);
		~Parser( void ) throw();
		Parser& operator=(const Parser& target);
		const std::vector<std::vector<String> >& getServersContents( void ) const;
		const	std::vector<ServerModel>&	getServers( void ) const;
		static std::vector<Data>	parseHeader(const String& header);
		
		void	printServerModel(const ServerModel& server);
	
	private :
		Data	extractDataFromString(String& line);
		void	 printLocations(const Location& locs);
		Location	 getLocations(std::vector<String>::iterator& begin, const std::vector<String>::iterator& end, String	path);
		void	 getFileContent( void );
		std::vector<String>	 getServerConfig(std::vector<String>::iterator&, const std::vector<String>::iterator&);
		void	parsingFile(std::vector<String> content);
		void	splitContentIntoServers( void );
		void	getFinalResualt( void );
		void	checkSyntax( void );
};

#endif
