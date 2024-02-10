/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommonDirectives.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 11:13:37 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/10 09:38:22 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "StringExtensions.hpp"
#include "CustomException.hpp"

#define MAX_CLIENT_BODY_SIZE 8589934592
#define ERROR_LOG "./logs/error_log.log"
#define ACCESS_LOG "./logs/access_log.log"
#define UPLOAD "./upload/"
#define ALLOWED_METHODS "GET POST DELETE"
#define CHECK_SIZE 9

class CommonDirectives
{
	private:
		string				root;
		string				index;
		string				try_files;
		bool				autoindex;
		long				client_max_body_size;
		string				error_log;
		string				access_log;
		MapStringString		mimeTypes;
		MapIntString		error_pages;
		VecString 			allowed_methods;
		string				upload;
		int					check[CHECK_SIZE];
		CommonDirectives();
	public:
		~CommonDirectives();
		CommonDirectives(const CommonDirectives& _copy);
		CommonDirectives& operator=(const CommonDirectives& _assignment);
		CommonDirectives(const string& _root, const string& _index,
						 const string& _try_files,
						 const bool& _autoindex , const long& _client_max_body_size,
						 const string& _error_log, const string& _access_log, const string& _upload);

		void	setRoot(const string& _root);
		void	setIndex(const string& _index);
		void	setTryFiles(const string& _try_files);
		void	setAllowedMethod(const string& _allowed_methods);
		void	setAutoIndex(const string& _autoindex);
		void	setClientMaxBodySize(const string& _client_max_body_size);
		void	setErrorLog(const string& _error_log);
		void	setAccessLog(const string& _access_log);
		void	addErrorPage(const string& _error_pages);
		void	addMimeType(const string& _key, const string& _value);
		void	setUpload(const string& _upload);

		const string&		getRoot() const;
		VecString			getAllowedMethods() const;
		bool				getAutoIndex() const;
		long				getClientMaxBodySize() const;
		const string&		getErrorLog() const;
		const string&		getAccessLog() const;
		MapIntString		getErrorPages() const;
		VecString			getIndexes() const;
		VecString			getTryFiles() const;
		MapStringString		getMimeTypes() const;
		string				getMimeTypeByKey(const string& _key);
		string				getIndexFilePathByRoot(const string& _value);
		const string&		getUpload() const;
};
