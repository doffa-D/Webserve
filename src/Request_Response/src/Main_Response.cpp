/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main_Response.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/20 18:15:56 by rrhnizar          #+#    #+#             */
/*   Updated: 2024/03/05 18:03:36 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/header.hpp"

bool Response::isMethodAllowed(const Location& location)
{
    std::vector<std::string> Methods = location.getAllowedMethods();
    std::string method = Req.getReqLine().getMethod();
    return std::find(Methods.begin(), Methods.end(), method) != Methods.end(); // return true if MA and false if MNA
}

bool Response::isRequestBodySizeAllowed(const Location& location)
{
    long MaxBodySize = atoi(Req.getHttp_Header()["Content-Length"].c_str());
    return MaxBodySize <= location.getClientMaxBodySize();
}

bool Response::serveRequestedResource(const std::string& Root_ReqPath, const Location& location)
{

    std::ifstream File(Root_ReqPath.c_str());
	
    if (File.is_open())
	{
		std::string bnd  = Req.getHttp_Header()["Content-Type"];
		// std::cout << "bnd  =   " << bnd << std::endl;
		size_t pos = str_utils::r_find(bnd, '=');
		std::string bondry = bnd.substr(pos + 1);
        // here i need to call upload function
        // 9bl khasni ntchecki wax kayn nit upload 
        // ghadi ntchiki bu contentType 
        // if contentType = multipart/form-data  &&  this Req Path not CGI  && Method is Post 
        // Like this  :
        
		
		
		
        size_t _pos = str_utils::r_find(Root_ReqPath, '.');
        std::string bin = location.getCgi()[Root_ReqPath.substr(_pos + 1)];
        if(bin.empty() && Req.getReqLine().getMethod() == "POST")
            upload_file(Req.getBody(),  location.getUpload(), Req.getHttp_Header());
			// upload_file(Req.getBody(), "./upload", "", Req.getBody().size(), bondry);
        
        struct stat fileInfo;
        if (stat(Root_ReqPath.c_str(), &fileInfo) == 0)
		{
            if (S_ISDIR(fileInfo.st_mode))
                handleDirectoryRequest(Root_ReqPath, location);
            else if (S_ISREG(fileInfo.st_mode))
                handleFileRequest(Root_ReqPath, location);
            return true;
        }
    }
    return false;
}


bool    Response::isUriTooLong(const long& _value)
{
    size_t LenghtOfPath = Req.getReqLine().getPath().size();
    if(LenghtOfPath > (size_t)_value)
        return true;
    return false;
}

std::string Response::ft_Response(const Parser& parser)
{
    _host = Req.getHttp_Header()["Host"];
    Server server = parser.getServerbyHost(_host);
    pair<string, Location> Loc = server.getLocationByPath(Req.getReqLine().getPath());
    std::string LocationName = Loc.first;
    Location location = Loc.second;
    handleBadRequest(location);
    if(ReqErr == 1)
        return response;
    if(isUriTooLong(server.getClientMaxHeaderBufferSize()))
    {
        handleUriTooLong(location);
        return response;
    }
    if (!isRequestBodySizeAllowed(location))
	{
        handleBodyTooLarge(location);
        return response;
    }
    if (!isMethodAllowed(location))
	{
        handleMethodNotAllowed(location);
        return response;
    }
    string Redirection = location.getRedirection();
    if(!Redirection.empty())
    {
        ResHeader.setLocation("http://" + _host + Redirection);
		ResPath = "";
        Fill_Response("303", "See Other", 1, 0, location);
        return response;
    }
    

    std::string Root_ReqPath;
    std::string ReqPath = Req.getReqLine().getPath();
    if(location.getAlias().empty())
        Root_ReqPath = location.getRoot() + Req.getReqLine().getPath(); // construct Absolute Path
    else
    {
        size_t found = ReqPath.find(LocationName);
        Root_ReqPath = ReqPath.replace(found, LocationName.length(), location.getAlias());
    }
    if (!serveRequestedResource(Root_ReqPath, location))
        handleNotFound(location);
    return response;
}