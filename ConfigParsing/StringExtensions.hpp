/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringExtensions.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/08 08:31:30 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/24 12:58:27 by kchaouki         ###   ########.fr       */
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


#define MAXCLIENTBODYSIZE 8589934592

typedef std::string						string; 
typedef std::map<int, string>			MapIntString;
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
	string		to_string(int value);
	size_t		find_first_of(const string& str, const string& charset);
	string&		trim(string& str);
	string		trim(const string& str);
	VecString 	split(const string& toSplit, char c);
	VecString 	ultimatSplit(const string& toSplit, const string& charset);
	bool		hasSpaceOnly(string& str);
	bool		endsWith(const string& toCheck, const string& with);
};
