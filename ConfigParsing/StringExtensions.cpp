/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringExtensions.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kchaouki <kchaouki@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/21 08:10:28 by kchaouki          #+#    #+#             */
/*   Updated: 2024/01/31 19:46:05 by kchaouki         ###   ########.fr       */
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

unsigned int	str_utils::ip(int n1, int n2, int n3, int n4)
{
	return (n4 << 24 | n3 << 16 | n2 << 8 | n1);
}

int	str_utils::to_int(const string& value)
{
	double ret = strtod(value.c_str(), NULL);
	return (ret);
}

bool	str_utils::createFile(const string& fileName)
{
	if (open(fileName.c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644) != -1)
		return (false);
	return (true);
}

string	str_utils::remove_quotes(const string& str)
{
	string out = str;
	if ((out[0] == '\"' && out[out.length() - 1] == '\"') ||
		(out[0] == '\'' && out[out.length() - 1] == '\''))
	{
		out.erase(0, 1);
		out.erase(out.length() - 1, 1);
	}
	return (out);
}

VecString str_utils::proSplit(const string& toSplit)
{
	string _toSplit;
	size_t i = 0;
	while (i < toSplit.length())
	{
		if (strchr("\'\"", toSplit[i]))
		{
			string s = strchr("\'\"", toSplit[i]);
			_toSplit += toSplit[i++];
			while (i < toSplit.length() && s[0] != toSplit[i])
				_toSplit += toSplit[i++];
			if (i < toSplit.length())
				_toSplit += toSplit[i++];
		}
		else if (toSplit[i] == ' ' || toSplit[i] == '\t')
		{
			_toSplit += '\x07';
			i++;
		}
		else
			_toSplit += toSplit[i++];
	}
	return (split(_toSplit, '\x07'));
}

size_t	str_utils::r_find(const string& fileName, char to_find)
{
	int i = fileName.length() - 1;
	for (;i > 0;--i)
		if (fileName[i] == to_find)
			return (i);
	return (i);
}
