/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringExtensions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 08:31:30 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/29 18:45:00 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <list>
#include <map>
#include <sstream>
#include <fstream>
#include <iostream>
#include <limits.h>
#include <unistd.h>
#include <fcntl.h>


#define MAX_CLIENT_BODY_SIZE 8589934592
#define ERROR_LOG "./logs/error_log.log"
#define ACCESS_LOG "./logs/access_log.log"

typedef std::string						string; 
typedef std::map<int, string>			MapIntString;
typedef std::map<string, string>		MapStringString;
typedef MapStringString::iterator		MapStringString_iter;
typedef MapIntString::iterator			MapIntString_iter;
typedef std::vector<string>				VecString;
typedef VecString::iterator				VecString_iter;
typedef std::vector<int> 				VecInt;
typedef VecInt::iterator 				VecInt_iter;
typedef std::list<string>				ListString;
typedef ListString::iterator			ListString_iter;

using	std::cout;
using	std::cerr;
using	std::endl;

namespace str_utils
{
	bool			createFile(const string& fileName);
	unsigned int	ip(int n1, int n2, int n3, int n4);
	string			to_string(int value);
	int				to_int(const string& value);
	size_t			find_first_of(const string& str, const string& charset);
	string&			trim(string& str);
	string			trim(const string& str);
	string			remove_quotes(const string& str);
	VecString 		split(const string& toSplit, char c);
	VecString 		ultimatSplit(const string& toSplit, const string& charset);
	VecString		proSplit(const string& toSplit);
	bool			hasSpaceOnly(string& str);
	bool			endsWith(const string& toCheck, const string& with);
};
