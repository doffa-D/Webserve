/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:04:45 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/05 10:39:17 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

#define ALLOWED_DIRECTIVES "server_name autoindex allowed_method host \
listen client_max_body_size error_log access_log error_page location root index \
try_files include upload"

#define LOCATION_DIRECTIVES "root try_files index location alias allowed_method \
autoindex client_max_body_size error_log access_log \
error_pages include upload"

class	Parser
{
	private:
		std::vector<Server>	servers;
		VecString 			allowed_directives;
		VecString 			location_valid_directives;
		string				dataToParse;
		ListString			tokens;
		Parser();
		Parser(const Parser& _copy);
		Parser& operator=(const Parser& _assignment);
		void	tokenizer();
		void	analyzer();
		void	createFiles();
		void	fillServerData(ListString_iter& it);
		void	fillLocationDirective(Server& server, CommonDirectives& old_location, ListString_iter& it, const  string& befor);
		void	fillDirectives(Server& server, ListString_iter& it, bool& ok);
		void	fillValideDirectives();
		bool	isValideForLocation(const string& key);
		bool	isValideDirective(const string& _directive);
	public:
		Parser(int ac, char**av);
		~Parser();
		Server								getDefaultServer() const;
		Server								getServerbyHost(const string& _host);
		std::vector<Server>					getServers() const;
		std::vector<std::pair<Uint, int> >	getHostsAndPorts();
		void								dump();
};
