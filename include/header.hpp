/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 16:24:29 by hdagdagu          #+#    #+#             */
/*   Updated: 2024/02/15 12:34:48 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_HPP
#define HEADER_HPP

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib>      // For exit() and EXIT_FAILURE
#include <iostream>     // For cout
#include <unistd.h>     // For read
#include <arpa/inet.h>  // For inet_ntoa
#include <netdb.h>      // For gethostbyname
#include <fstream>      // For ifstream
#include <fcntl.h>      // For open
#include <vector>       // For vector
#include <string>       // For string
#include <map>          // For map

// ===========Config============= //

#include "../src/config/include/StringExtensions.hpp"
#include "../src/config/include/CommonDirectives.hpp"
#include "../src/config/include/CustomException.hpp"
#include "../src/config/include/Location.hpp"
#include "../src/config/include/Parser.hpp"
#include "../src/config/include/Server.hpp"

// ===========Server============= //

#include "../src/server/include/Upload.hpp"
#include "../src/server/include/SendTracker.hpp"
#include "../src/server/include/Wb_Server.hpp" 
#include "../src/CGI/include/CGI.hpp"

// ======================== //





#endif
