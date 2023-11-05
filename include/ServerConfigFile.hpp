/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigFile.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 10:29:16 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/05 11:16:51 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIGFILE_HPP
# define SERVERCONFIGFILE_HPP



#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class ServerConfigFile 
{
    private:
        std::string listen_port;
        std::vector<std::string> serverName;
        std::string root;
        std::string index;
        std::string tryFiles;
        std::vector<std::string> locations;

    public:
        void parseConfig(const std::string& configFilePath);

        std::string getlisten_port();
        std::vector<std::string> getServerName();
        std::string getRoot();
        std::string getIndex();
        std::string getTryFiles();
        std::vector<std::string> getLocations();
};

#endif