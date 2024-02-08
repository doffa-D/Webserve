/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringExtensions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 08:31:30 by kchaouki          #+#    #+#             */
/*   Updated: 2024/02/08 21:42:58 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <list>
#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>

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
typedef unsigned int 					Uint;
using	std::cout;
using	std::cerr;
using	std::endl;

namespace str_utils
{
	size_t			r_find(const string& fileName, char to_find);
	bool			createFile(const string& fileName);
	Uint			ip(const string& value);
	Uint			ip(int n1, int n2, int n3, int n4);
	std::string		ip(Uint value);
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
