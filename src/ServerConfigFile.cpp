/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigFile.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 10:31:35 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/05 11:16:30 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ServerConfigFile.hpp"

void ServerConfigFile::parseConfig(const std::string &configFilePath)
{
    std::ifstream configFile(configFilePath.c_str());

    std::string line;
    while (std::getline(configFile, line))
    {
        if (line.find("listen") != std::string::npos)
        {
            listen_port = line.substr(line.find(" ") + 1);
        }
        else if (line.find("server_name") != std::string::npos)
        {
            std::string names = line.substr(line.find(" ") + 1);
            size_t pos = 0;
            std::string delimiter = " ";
            while ((pos = names.find(delimiter)) != std::string::npos)
            {
                serverName.push_back(names.substr(0, pos));
                names.erase(0, pos + delimiter.length());
            }
            serverName.push_back(names);
        }
        else if (line.find("root") != std::string::npos)
        {
            root = line.substr(line.find(" ") + 1);
        }
        else if (line.find("index") != std::string::npos)
        {
            index = line.substr(line.find(" ") + 1);
        }
        else if (line.find("try_files") != std::string::npos)
        {
            tryFiles = line.substr(line.find(" ") + 1);
        }
        else if (line.find("location") != std::string::npos)
        {
            locations.push_back(line.substr(line.find(" ") + 1));
        }
    }

    configFile.close();
}

std::string ServerConfigFile::getlisten_port()
{
    return listen_port;
}
std::vector<std::string> ServerConfigFile::getServerName()
{
    return serverName;
}
std::string ServerConfigFile::getRoot()
{
    return root;
}
std::string ServerConfigFile::getIndex()
{
    return index;
}
std::string ServerConfigFile::getTryFiles()
{
    return tryFiles;
}
std::vector<std::string> ServerConfigFile::getLocations()
{
    return locations;
}