/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 09:04:45 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/31 11:57:19 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"

class	Parser
{
	private:
		std::vector<Server>	servers;
		VecString 			validDirectives;
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
		void	fileValideDirectives();
		bool	isValideForLocation(const string& key);
		bool	isValideDirective(const string& _directive);
	public:
		Parser(const string& fileName);
		~Parser();
		Server					getDefaultServer() const;
		Server					getServerbyHost(const string& _host);
		std::vector<Server>		getServers() const;
		void					dump();
};
