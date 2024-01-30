/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CheckFile.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 18:03:26 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/01/26 18:04:32 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <filesystem>
#include <string>
#include <map>
#include <vector>

//How do you need to receive configuration data
int main()
{
	// Path in one time is root + ReqLine_Path but you need check refere 
	// and join root + refer + ReqLine_Path
	std::string Path = "rr.png";
	std::ifstream File(Path.c_str());

	// std::map<std::string, std::pair<std::string, std::vector<int> > > Conf_File;
	// std::map<std::string, std::pair<std::string, std::string> > Conf_File;
	// Conf_File.insert(std::make_pair("root1", std::make_pair("paire1", "paire1")));
	// std::map<std::string, std::pair<std::string, std::string> >::iterator it;
	// for(it = Conf_File.begin(); it != Conf_File.end(); it++)
	// {
	// 	std::cout << it->second.first << std::endl;
	// }
	
	if(File.is_open())
	{
		std::cout << "File exists " << std::endl;
		struct stat fileInfo;
        if (stat(Path.c_str(), &fileInfo) == 0)
        {
			if (S_ISDIR(fileInfo.st_mode))
            {
				// join directory with index.html 
				// and serach if this file is exest or no 
				// if exests serve this other ways  print not found
				
				std::string Full_Path = Path + "/index.html";
				std::ifstream FileIndex(Full_Path.c_str());
				if(FileIndex.is_open())
				{
					// const std::string htmlContent = readHtmlFile(Full_Path);
					std::cout << "yeees\n";
				}
				else
				{
					// if index.html file not exests check autoindex is on or of
					// if autoindex on i think i need in other process to serve this
					// if autoindex off or not exests respondse with not found 
				}
                std::cout << "It is a directory." << std::endl;
            }
            else if (S_ISREG(fileInfo.st_mode))
            {
                std::cout << "It is a regular file." << std::endl;
            }
            else
            {
                std::cout << "It is neither a directory nor a regular file." << std::endl;
            }
		}
		else
			std::cout << "Error getting file information." << std::endl;
		// here check is a directory or file for if is a directory you should serve index.html 
		// in case is a file you should serve this file 
	}
	else
		std::cout << "File not exists" << std::endl;
	return 0;
}
