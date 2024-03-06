/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 11:13:37 by kchaouki          #+#    #+#             */
/*   Updated: 2024/03/06 19:26:26 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../../../include/header.hpp"

#define MAX_BUFFER_SIZE 8589934592
#define ERROR_LOG "./logs/error.log"
#define ACCESS_LOG "./logs/access.log"
#define UPLOAD "./upload/"
#define ALLOWED_METHODS "GET POST DELETE"
#define ALLOWED_EXTENSION "php py pl"
#define DEFAULT_INDEX "index.html"
#define CHECK_SIZE 11

class CommonDirectives
{
	private:
		string					root;
		VecString				index;
		string					redirection;
		bool					autoindex;
		long					client_max_body_size;
		string					error_log;
		string					access_log;
		MapStringString			mimeTypes;
		MapIntString			error_pages;
		bool			_bool[2];
		VecString 				allowed_methods;
		string					upload;
		MapStringString			cgi;
		int						check[CHECK_SIZE];
		CommonDirectives();
	protected:
		bool					root_alias;
	public:
		~CommonDirectives();
		CommonDirectives(const CommonDirectives& _copy);
		CommonDirectives& operator=(const CommonDirectives& _assignment);
		CommonDirectives(const string& _root, const bool& _autoindex , const long& _client_max_body_size,
						 const string& _error_log, const string& _access_log, const string& _upload);

		void	check_unexpected(const string& to_check, const string& direc);
		/*==============>SETTERS<================*/
		void	setRoot(const string& _root);
		void	setIndex(const string& _index);
		void	setRedirection(const string& _redirection);
		void	setAllowedMethod(const string& _allowed_methods);
		void	setAutoIndex(const string& _autoindex);
		void	setClientMaxBodySize(const string& _client_max_body_size);
		void	setErrorLog(const string& _error_log);
		void	setAccessLog(const string& _access_log);
		void	addErrorPage(const string& _error_pages);
		void	addMimeType(const string& _key, const string& _value);
		void	setUpload(const string& _upload);
		void	setCgi(const string& _cgi);
		
		/*==============>GETTERS<================*/
		const string&		getRoot() const;
		VecString			getAllowedMethods() const;
		bool				getAutoIndex() const;
		long				getClientMaxBodySize() const;
		const string&		getErrorLog() const;
		const string&		getAccessLog() const;
		MapIntString		getErrorPages() const;
		VecString			getIndexes() const;
		string				getRedirection() const;
		MapStringString		getMimeTypes() const;
		string				getMimeTypeByKey(const string& _key) const;
		const string&		getUpload() const;
		MapStringString		getCgi() const;
};
