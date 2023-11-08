#ifndef	STRING_HPP
#define STRING_HPP

#include <iostream>
#include <sstream>
#include <vector>

class String : public std::string
{
	public :
		String( void );
		String(const char *str);
		String(const String& str);
		~String( void );
		String&	operator=(const String& str);
		String&	trim(const String& characters);
		String&	leftTrim(const String& characters);
		String&	rightTrim(const String& characters);
		std::vector<String>	split( void );
		std::vector<String>	split(const char& separators);
		int		countRepeating(const char& c);
		bool	contains(const String& str) const;
		bool	equal(const String& value) const;
		static String	convertVectorToString(const std::vector<String>& strings, unsigned int begin/* = 0*/, unsigned int len/* = 1*/, char separator/* = 0*/);
};



#endif
