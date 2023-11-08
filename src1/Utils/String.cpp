#include "String.hpp"

String::String( void ) : std::string()
{
}

String::String(const char* str) : std::string(str)
{
}

String::String(const String& str) : std::string(str)
{
}

String::~String( void )
{
}

String& String::operator=(const String& str)
{
	std::string::operator=(str);
	return (*this);
}

bool	String::equal(const String& value) const
{
	if (!compare(value))
		return (true);
	return (false);
}

bool	String::contains(const String& str) const
{
	if ((int)find(str) != -1)
		return (true);
	return (false);
}

String&	String::leftTrim(const String& characters)
{
	iterator iterBegin = begin();
	iterator iterEnd = end();

	while (iterBegin < iterEnd)
	{
		if (characters.find(*iterBegin) != npos)
			iterBegin++;
		else
			break ;
	}
	erase(begin(), iterBegin);
	return (*this);
}

String&	String::rightTrim(const String& characters)
{
	iterator iterBegin = begin();
	iterator iterEnd = end();

	while (iterBegin < --iterEnd)
	{
		if (characters.find(*iterEnd) != npos)
			erase(iterEnd);
		else
			break ;
	}
	return (*this);
}


String&	String::trim(const String& characters)
{
	leftTrim(characters);
	rightTrim(characters);
	return (*this);
}


std::vector<String> String::split( void )
{
	String tmp(*this);
	iterator	b = tmp.begin();
	iterator	e = tmp.end();
	while (b < e)
	{
		if (*b == ' ' || *b == '\t')
			*b = ' ';
		b++;
	}
	std::vector<String> vec;
	std::istringstream str(tmp);
    String s;
    while (getline(str, s, ' '))
	{
		if (s.size() == 0)
			continue ;
        vec.push_back(s);
    }
	return (vec);
}


std::vector<String> String::split(const char& separator)
{
	std::vector<String> vec;
	std::istringstream str(*this);
    String s;
    while (getline(str, s, separator))
	{
		if (s.size() == 0)
			continue ;
        vec.push_back(s);
    }
	return (vec);
}

int	String::countRepeating(const char& c)
{
	int	counter = 0;
	iterator iBeg = begin();
	iterator iEnd = end();
	while (iBeg < iEnd)
	{
		if (*iBeg == c)
			counter++;
		iBeg++;
	}
	return (counter);
}


String	String::convertVectorToString(const std::vector<String>& strings, unsigned int begin = 0, unsigned int len = 1, char separator = 0)
{
	String dest;
	
	if (len + begin > strings.size())
		len = (unsigned int)strings.size();
	while (begin <= len)
	{
		dest.append(strings[begin]);
		begin++;
		if (separator != 0 && begin <= len)
			dest.append(&separator);
	}
	return (dest);
}
