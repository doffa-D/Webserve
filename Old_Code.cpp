/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Old_Code.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 22:00:31 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/08 22:16:22 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*------------------------------ Old Code Of Request --------------------------------*/


// here Parse query parameters and fill vector the Query_Params;
// std::vector<std::string> split1 = str_utils::split(queryparams, '&');
// for(size_t i=0; i<split1.size(); i++)
// {
// 	size_t p = split1[i].find('=');
// 	if(p != std::string::npos)
// 		Query_Params.push_back(std::make_pair(split1[i].substr(0, p), split1[i].substr(p + 1)));
// }
// for(size_t i=0; i<Query_Params.size(); i++)
// {
// 	std::cout << "Query String\n";
// 	std::cout << "Key = " << Query_Params[i].first << "			";
// 	std::cout << "Value = " << Query_Params[i].second << std::endl;
// }


// std::cout << "method =  [" << getMethod() << "]  Path =  [" << getPath() << "]  HttpVersion = [" << getHttpVersion() << "]" << std::endl;


// Constrator of Request with allowed Characters

Request::Request() : Referer(0), ReqLine(RequestLine()), Http_Header(), Body()
{
	BadRequest = 0;
	// Add uppercase letters
    for (char c = 'A'; c <= 'Z'; c++)
        allowedCharacters.push_back(c);
    // Add lowercase letters
    for (char c = 'a'; c <= 'z'; c++)
        allowedCharacters.push_back(c);
    // Add digits
    for (char c = '0'; c <= '9'; c++)
        allowedCharacters.push_back(c);
	// Add other unreserved characters
    const std::string unreservedChars = "-._~";
    for (size_t i = 0; i < unreservedChars.size(); i++)
        allowedCharacters.push_back(unreservedChars[i]);
    // Add reserved characters
	const std::string reservedChars = ":/?#[]@!$&'()*+,;=%";
    for (size_t i = 0; i < reservedChars.size(); i++)
        allowedCharacters.push_back(reservedChars[i]);
}

// PrintHttp_Header 
void	Request::PrintHttp_Header()
{
	std::cout << "\n---------------------------------- HTTP_Header ---------------------------------------\n";
	std::map<std::string, std::string>::iterator it = Http_Header.begin();
	for (; it != Http_Header.end(); it++)
    {
		std::cout << "Key: " << it->first << "   |   Value: " << it->second << std::endl;
    }
	std::cout << "\n---------------------------------- ====== ---------------------------------------\n";
}


std::string	ToUpperStr(std::string str)
{
	std::string res = str;
	for(size_t i=0; i<str.size(); i++)
		res[i] = toupper(str[i]);
	return res;
}


// bool Request::LookingForKey()
// {
// 	// Key to search for
//     std::string key_to_find = "HOST";

//     bool found = false;
//     for (size_t i = 0; i < getHttp_Header().size(); ++i)
// 	{
//         if (ToUpperStr(getHttp_Header()[i].) == key_to_find)
// 		{
//             found = true;
//             break;
//         }
//     }
// 	return found;
// }


bool	Request::FoundDisallowedChar()
{
	bool	disallowedCharFound = false;
	std::string	ReqPath = ReqLine.getPath();
	// for (std::string::iterator it = ReqLine.getPath().begin(); it != ReqLine.getPath().end(); it++)
	for(size_t i=0; i<ReqPath.size(); i++)
	{
		if(std::find(allowedCharacters.begin(), allowedCharacters.end(), ReqPath[i]) == allowedCharacters.end())
		{
			disallowedCharFound = true;
			break;
		}
	}
	return disallowedCharFound;
}

// I Check any character not allowed in RequestPath
if(FoundDisallowedChar() == true)
{
	BadRequest = 1;
	return;
}

// Remove leading and trailing whitespaces from key and value
// but is useless remove leading and trailing whitespaces because in nginx it's possible serve
// file with spaces or tab


void	Request::CheckReferer()
{
	std::map<std::string, std::string>::iterator it = Http_Header.begin();
	for (; it != Http_Header.end(); it++)
    {
        if(strncmp(it->first.c_str(), "Referer", 7) == 0)
            Referer = 1;
    }
}


void	RequestLine::PrintReqLine()
{
	std::cout << "\n---------------  HttpRequestLine --------------------------------\n";
	std::cout << "Method:	" << getMethod() << std::endl;
    std::cout << "Path: 	" << getPath() << std::endl;
    std::cout << "HTTP Version: 	" << getHttpVersion() << std::endl;
	std::cout << "\n--------------------- ========== ---------------------------------\n";
}




/*------------------------------ Old Code Of Response --------------------------------*/


