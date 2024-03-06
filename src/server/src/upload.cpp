/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:13:29 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/03/06 18:23:47 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/Upload.hpp"

std::string extractFileName(const std::string &contentDisposition, size_t filenamePos)
{

	size_t startQuote = contentDisposition.find('"', filenamePos);
	if (startQuote != std::string::npos)
	{
		size_t endQuote = contentDisposition.find('"', startQuote + 1);
		if (endQuote != std::string::npos)
		{
			return contentDisposition.substr(startQuote + 1, endQuote - startQuote - 1);
		}
	}

	return "";
}

void saveFile(std::string body, size_t filenamePos,std::string const &location)
{
	if (filenamePos != std::string::npos)
	{
		std::string filename = extractFileName(body, filenamePos);
		// std::cout << location << std::endl;
		filename = location + "/" + filename;
		size_t firstContentPos = body.find("\r\n\r\n", filenamePos + 9);
		if (firstContentPos != std::string::npos)
		{
			std::ofstream file(filename.c_str());
			if (file.is_open())
			{
				file << body.substr(firstContentPos + 4 , body.size() - firstContentPos - 6);
				file.close();
			}
		}
	}
}

std::string  split_request(std::string const &body,std::string const &boundary,std::string const &location)
{
	std::string delimiter = "--" + boundary;
	size_t pos = body.find(delimiter);
	std::vector<std::string> parts;
	std::string param = "";
	while (pos != std::string::npos)
	{
		size_t nextPos = body.find(boundary, pos + boundary.length() + 2);
		std::string part = body.substr(pos + boundary.length() + 2, nextPos - pos - boundary.length() - 4);
		parts.push_back(part);
		pos = nextPos;
	}
	for (size_t i = 0; i < parts.size(); i++)
	{
		size_t first = parts[i].find("\r\n\r\n");
		if (first != std::string::npos)
		{
			std::string first_part = parts[i].substr(0, first + 4);
			size_t start = first_part.find_first_not_of("\r\n");
			size_t end = first_part.find_last_not_of("\r\n");

			if (start != std::string::npos && end != std::string::npos) {
			    first_part = first_part.substr(start, end - start + 1);
			}
			std::istringstream iss(first_part);
			std::string line;
			int j = 0;
			while (std::getline(iss, line)) {
				j++;
			}
			if(j == 2)
			{
				size_t filenamePos = parts[i].find("filename=");
				if (filenamePos != std::string::npos)
				{
					saveFile(parts[i], filenamePos,location);
				}
			}
			else
			{
				param += "\r\n";
				param += parts[i];
			}
		}
	}
	return param;
}

std::string upload_file(std::string const &body,std::string const &location,MapStringString header)
{
	std::string boundary = header["Content-Type"];
	std::string param = "";
	if(!boundary.empty())
	{
		size_t pos = boundary.find("boundary=");
		boundary = boundary.substr(pos + 9);
		param = split_request(body,boundary,location);

	}
	return param;
}

std::string readFileContent(const std::string &filePath)
{
	std::ifstream fileStream(filePath);
	if (!fileStream.is_open())
	{
		return "";
	}

	std::ostringstream contentStream;
	contentStream << fileStream.rdbuf();
	return contentStream.str();
}