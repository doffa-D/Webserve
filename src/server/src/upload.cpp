/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 16:13:29 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/28 16:20:11 by hdagdagu         ###   ########.fr       */
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
		std::cout << location << std::endl;
		filename = location + "/" + filename;
		size_t firstContentPos = body.find("\r\n\r\n", filenamePos + 9);
		if (firstContentPos != std::string::npos)
		{
			std::ofstream file(filename.c_str());
			if (file.is_open())
			{
				file << body.substr(firstContentPos + 4);
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
	std::string param;
	while (pos != std::string::npos)
	{
		size_t nextPos = body.find(boundary, pos + boundary.length() + 2);
		std::string part = body.substr(pos + boundary.length() + 2, nextPos - pos - boundary.length() - 4);
		parts.push_back(part);
		pos = nextPos;
	}
	for (size_t i = 0; i < parts.size(); i++)
	{
		size_t filenamePos = parts[i].find("filename=");
		if(filenamePos != std::string::npos)
		{
			saveFile(parts[i], filenamePos,location);
		}
		else
		{
			param += "\r\n";
			param += parts[i];
			std::cout << "==>" << parts[i] << std::endl;	
		}
	}
	return param;
}

std::string upload_file(std::string const &bodyyyyy,std::string const &location,std::string chunked,long long content_lenght,std::string boundary)
{
	std::string body;
	if(chunked == "chunked")
	{
		size_t startPos = 0;
		size_t endPos = content_lenght;
		while (startPos < endPos)
		{
			size_t sizePos = bodyyyyy.find("\r\n", startPos);
			if (sizePos != std::string::npos)
			{
				std::string chunkSizeHex = bodyyyyy.substr(startPos, sizePos - startPos);
				int chunkSize = std::stoi(chunkSizeHex, 0, 16);
				
				startPos = sizePos + 2;
				if (startPos + chunkSize <= endPos)
				{
					std::string chunkData = bodyyyyy.substr(startPos, chunkSize);
					body += chunkData;
					startPos += chunkSize + 2;
				}
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		body = bodyyyyy;
	}
	return split_request(body,boundary,location);
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