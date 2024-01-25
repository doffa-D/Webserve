/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringExtensions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 08:10:28 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/24 12:35:55 by kchaouki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "StringExtensions.hpp"

string&	str_utils::trim(string& str)
{
	size_t	start = 0;
	size_t	end = str.length() - 1;
	while (str.size() && start < str.size() && (str[start] == ' ' || str[start] == '\t'))
		start++;
	while (str.size() && end > 0 && (str[end] == ' ' || str[end] == '\t'))
		end--;
	str = str.substr(start, end - start + 1);
	return (str);
}

string		str_utils::trim(const string& str)
{
	size_t	start = 0;
	size_t	end = str.length() - 1;
	while (str.size() && start < str.size() && (str[start] == ' ' || str[start] == '\t'))
		start++;
	while (str.size() && end > 0 && (str[end] == ' ' || str[end] == '\t'))
		end--;
	return (str.substr(start, end - start + 1));
}

bool str_utils::hasSpaceOnly(string& str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (str[i] != ' ' && str[i] != '\t')
			return (false);
	return (true);
}

bool str_utils::endsWith(const string& toCheck, const string& with)
{
	size_t _toCheck = toCheck.length() - 1;
	size_t _with = with.length() - 1;

	while (_with > 0 && _toCheck != 0)
		if (with[_with--] != toCheck[_toCheck--])
			return (false);
	return (true);
}

VecString str_utils::split(const string& toSplit, char c)
{
	VecString splited;
    std::stringstream ss(toSplit); 
	string str;
    while (getline(ss, str, c))
		if (!str_utils::hasSpaceOnly(str))
        	splited.push_back(str);
    return splited;
}

VecString str_utils::ultimatSplit(const string& toSplit, const string& charset)
{
	VecString split;
    size_t start = 0;
    size_t end = toSplit.find_first_of(charset);

    while (end != string::npos) {
		string str = toSplit.substr(start, end - start);
		if (!str_utils::hasSpaceOnly(str))
        	split.push_back(str);
        start = end + 1;
        end = toSplit.find_first_of(charset, start);
    }
	string str = toSplit.substr(start);
	if (!str_utils::hasSpaceOnly(str))
		split.push_back(str);
	return (split);
}

size_t	str_utils::find_first_of(const string& str, const string& charset)
{
	size_t len = str.find_first_of(charset);
	if (len == string::npos)
		len = str.length();
	return (len);
}

string	str_utils::to_string(int value)
{
	std::stringstream ss;
	ss << value;
	return (ss.str());
}
