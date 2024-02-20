/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Autoindex.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rrhnizar <rrhnizar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 21:01:14 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/02/20 21:45:21 by rrhnizar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

std::string Directories_names(std::vector<std::string> d_names, std::vector<std::string> d_dates, std::vector<std::string> d_sizes)
{
	std::string str = "<li>";
	for(size_t i=0; i<d_names.size(); i++)
	{
		str += "<a href=\"" + d_names[i] + "\">" + d_names[i] + "</a>" +
           "<span style=\"display: block; text-align: center;\">" + d_dates[i] + "</span>" +
           "<span style=\"display: block; text-align: right;\">" + d_sizes[i] + "</span>" +
       "</li>";
	}
	return str;
}


std::string getModificationDate(const std::string& filePath)
{
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) != 0)
        return "Error in getting file info";
    // Extract the last modification time
    std::time_t modifiedTime = fileInfo.st_mtime;
    char dateStr[100];
    std::strftime(dateStr, sizeof(dateStr), "%Y-%m-%d %H:%M:%S", std::localtime(&modifiedTime));
    return dateStr;
}

std::string getFileSize(const std::string& filePath)
{
    struct stat fileInfo;
    if (stat(filePath.c_str(), &fileInfo) != 0)
        return "Error in getting file info";

    // Extract the file size
    return str_utils::to_string(fileInfo.st_size);
}

std::string	 AutoIndex(std::string ResPath, std::string ReqPath)
{
	std::vector<std::string> d_names;
	std::vector<std::string> d_dates;
	std::vector<std::string> d_sizes;
	DIR *directory;
    struct dirent *entry;

    // Open the current directory (replace "." with the path to your desired directory)
    if ((directory = opendir(ResPath.c_str())) != NULL)
	{
        // Read each entry in the directory
        while ((entry = readdir(directory)) != NULL)
		{
			// std::cout << "--->  " << entry->d_name << std::endl;
			// std::cout << "date  : " << getModificationDate(ResPath + entry->d_name) << std::endl;
			// std::cout << "size  : " << getFileSize(ResPath + entry->d_name) << std::endl;
			d_names.push_back(entry->d_name);
			d_dates.push_back(getModificationDate(ResPath + entry->d_name));
			d_sizes.push_back(getFileSize(ResPath + entry->d_name));
        }

        // Close the directory
        closedir(directory);
    }
	else
	{
        // Handle error opening directory
        std::cerr << "Error opening directory." << std::endl;
    }
	std::string str = 
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "    <meta charset=\"UTF-8\">\n"
        "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "    <title>Index of " + ReqPath + "</title>\n"
        "</head>\n"
        "<body>\n"
        "    <h1>Index of " + ReqPath + "</h1>\n"
        "    <hr>\n"
        "    <ul style=\"margin-right:50px; margin-left:50px;\">\n"
        "        <!-- Directory listing will be generated here dynamically -->\n"
        "        <!-- Each list item should represent a file or subdirectory -->\n"
        "        <!-- You can use server-side scripting or tools to automate this process -->\n"
			+ 		Directories_names(d_names, d_dates, d_sizes) +
        "        <!-- Add more entries as needed -->\n"
        "    </ul>\n"
        "    <hr>\n"
        "</body>\n"
        "</html>";
	return str;
}