/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:04:45 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/15 12:31:06 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#define ALLOWED_DIRECTIVES "server_name autoindex allowed_method \
listen client_max_body_size error_log access_log error_page location root index \
try_files include upload redirection cgi alias"

#define LOCATION_DIRECTIVES "root try_files index location alias allowed_method \
autoindex client_max_body_size error_log access_log \
error_pages include upload redirection cgi alias"

class	Parser
{
	private:
		vector<Server>		servers;
		VecString 			allowed_directives;
		VecString 			location_valid_directives;
		string				dataToParse;
		ListString			tokens;
		Parser();
		Parser(const Parser& _copy);
		Parser& operator=(const Parser& _assignment);
		void			tokenizer();
		void			analyzer();
		void			createFiles();
		void			fillServerData(ListString_iter& it);
		void			fillLocationDirective(Server& server, CommonDirectives& old_location, ListString_iter& it, const  string& befor);
		void			fillDirectives(Server& server, ListString_iter& it, bool& ok);
		void			fillValideDirectives();
		bool			isValideForLocation(const string& key);
		bool			isValideDirective(const string& _directive);
		Uint			getIPv4FromDns(const std::string& _dns);
		Server			findServer(std::vector<Server>& _servers, const string& _host);
		pair<Uint, int>	parseHost(const string& _host);
	public:
		Parser(int ac, char**av);
		~Parser();
		Server								getDefaultServer() const;
		Server								getServerbyHost(const string& _host);
		vector<Server>						getServers() const;
		IpPorts								getHostsAndPorts();
		void								dump();
};
