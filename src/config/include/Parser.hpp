/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:04:45 by kchaouki          #+#    #+#             */
/*   Updated: 2024/03/09 18:24:02 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"


#define ALLOWED_DIRECTIVES "server_name autoindex allowed_method \
listen client_max_body_size error_log access_log error_page location root index \
include upload_pass redirection cgi_pass alias client_max_uri"

#define LOCATION_DIRECTIVES "root index location alias allowed_method \
autoindex client_max_body_size error_log access_log \
error_page include upload_pass redirection cgi_pass"

#define DEFUALT_CONFIG_PATH "ConfigFiles/default.conf"

class	Parser
{
	private:
		vector<Server>		servers;
		VecString 			allowed_directives;
		VecString 			location_valid_directives;
		ListString			tokens;
		Parser();
		Parser(const Parser& _copy);
		Parser& operator=(const Parser& _assignment);
		string					remove_hashtag(const string& _line);
		ListString				tokenizer(const string& dataToParse);
		void					analyzer();
		void					createFiles();
		void					fillServerData(ListString_iter& it);
		void					fillLocationDirective(Server& server, CommonDirectives& old_location, ListString_iter& it, const  string& befor);
		void					fillDirectives(Server& server, ListString_iter& it, bool& ok);
		void					fillValideDirectives();
		bool					isValideForLocation(const string& key);
		bool					isValideDirective(const string& _directive);
		vector<Uint>			getIPv4FromDns(const std::string& _dns) const;
		Server					findServer(std::vector<Server>& _servers, const string& _host) const;
		pair<vector<Uint>, int>	parseHost(const string& _host) const;
	public:
		Parser(int ac, char**av);
		~Parser();
		Server								getDefaultServer() const;
		Server								getServerbyHost(const string& _host) const;
		vector<Server>						getServers() const;
		IpPorts								getHostsAndPorts() const;
		void								dump();
};
