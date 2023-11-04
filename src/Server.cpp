/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/04 17:55:19 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"



Server::Server()
{
	getMyIpAddress();					   // getMyIpAddress function is used to get the ip address of the server
	Setup_Server();						   // Setup_Server function is used to setup the server
	fd_set current_sockets, ready_sockets; // fd_set is a set of file descriptors. It is actually a bit array. Each bit corresponds to a file descriptor. If the bit is set, it means that the corresponding file descriptor is set. The FD_ZERO macro clears the set. The FD_SET macro adds a file descriptor to the set. The FD_CLR macro removes a file descriptor from the set. The FD_ISSET macro checks to see if a file descriptor is part of the set.

	FD_ZERO(&current_sockets);					// FD_ZERO macro clears the set
	FD_SET(socket_fd_server, &current_sockets); // FD_SET macro adds a file descriptor to the set
	for (; true;)
	{
		ready_sockets = current_sockets;								// ready_sockets is the set of file descriptors that are ready to be read from
		if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) == -1) // select function is used to monitor file descriptors. It takes five arguments: The first argument is the highest-numbered file descriptor in any of the three sets, plus 1. The second argument is the set of file descriptors that you're interested in reading. The third argument is the set of file descriptors that you're interested in writing. The fourth argument is the set of file descriptors that you're interested in excepting. The fifth argument is a timeout. If the timeout is NULL, select will block until at least one file descriptor is ready. If the timeout is 0, select will return immediately. Otherwise, it will block until either the timeout expires or a file descriptor becomes ready. The select function returns the number of file descriptors that are ready, or -1 on error.
		{
			std::cout << "Failed to select. " << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &ready_sockets)) // FD_ISSET macro checks to see if a file descriptor is part of the set
			{
				if (i == socket_fd_server) // if the file descriptor is the server socket, then we have a new connection
				{
					Accept_Connection();						// accept function is used to accept a connection request from a client. It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
					FD_SET(socket_fd_client, &current_sockets); // FD_SET macro adds a file descriptor to the set
				}
				else
				{
					socket_fd_client = i;
					Read_Request_From_Client();
					FD_CLR(socket_fd_client, &current_sockets); // FD_CLR macro removes a file descriptor from the set
					close(socket_fd_client);					// close function is used to close a socket. It takes a single argument, which is the file descriptor of the socket to be closed.
				}
			}
		}
	}
}
Server::~Server()
{
	// std::cout << "Server destructor called. " << std::endl;
}

void Server::Setup_Server()
{
	socket_fd_server = socket(AF_INET, SOCK_STREAM, 0);
	// what is socket? A socket is one endpoint of a two-way communication link between two programs running on the network. A socket is bound to a port number so that the TCP layer can identify the application that data is destined to be sent to.
	// AF_INET is the address family that is used for the socket you're creating (in this case an Internet Protocol address).
	// SOCK_STREAM is the type of socket you're creating (TCP, for example).
	// The third argument is the protocol. If this argument is zero (and it always should be except for unusual circumstances), the operating system will choose the most appropriate protocol. It will choose TCP for stream sockets and UDP for datagram sockets.
	if (socket_fd_server == -1)
	{
		std::cout << "Failed to create socket. " << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Creat socket. " << std::endl;
	}

	// initialize address
	address.sin_family = AF_INET;	// what is sin_family? sin_family contains a code for the address family. It should always be set to AF_INET.
	address.sin_port = htons(PORT); // what is sin_port? sin_port contains the port number. However, instead of simply copying the port number to this field, it is necessary to convert this to network byte order using the function htons() which converts a port number in host byte order to a port number in network byte order.
	std::cout << "IP Address: http://" << ip_address << ":" << PORT << std::endl;
	int opt = 1;
	setsockopt(socket_fd_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));		// why we need setsockopt function to set socket options? The setsockopt function is used to allow the local address to be reused when the server is restarted before the required wait time expires. The setsockopt function takes four arguments: The first argument is the socket descriptor. The second argument is the level at which the option resides. The third argument is the name of the option. The fourth argument is a pointer to the buffer containing the value for the option. The fifth argument is the size of the buffer containing the value for the option.
	if (bind(socket_fd_server, (struct sockaddr *)&address, sizeof(address)) == -1) // why we need bind function to bind socket? The bind function is used to bind the socket to a particular "address and port combination". In this case, we are binding to
	{
		std::cout << "Faild to bind to port " << PORT << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(socket_fd_server, BACKLOG) == -1) // why we need listen function to listen socket? The listen function is used on the server side. It tells the socket library that we want to listen in on the server side for incoming connections. The second argument is called the backlog, and it tells the socket library that how many incoming connections can be queued up before the system starts to reject incoming connections.
	{
		std::cout << "Faild to listen. " << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Listen. " << std::endl;
	}
}

void Server::Accept_Connection()
{
	socklen_t address_lenght = sizeof(address);															   // socklen_t is a type of data type in socket.h header file which is used to represent the size of objects in bytes. It is used to represent the sizes of socket address structures. It is used in accept(), bind(), connect(), getsockname(), getpeername(), and socket() functions.
	if ((socket_fd_client = accept(socket_fd_server, (struct sockaddr *)&address, &address_lenght)) == -1) // accept function is used to accept a connection request from a client. It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
	{
		std::cout << "Failed to grab connection. " << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::cout << "Grab connection. " << std::endl;
	}
}

void Server::Send_Response_To_Client()
{
	size_t pos = get_URI().find('.');
	std::string ContentType;
	if(pos != std::string::npos)
	{
		std::string ext = get_URI().substr(pos);
		ContentType = GetMIMEType(ext);
	}
	std::string status = get_status();
	// print_all_parseRequest();
	// std::cout << "===> ContentType: " << ContentType << std::endl;
	// if (status.empty() == 0)
	// {
	// 	std::string message = get_Version() + " " + status + "\r\nContent-Type: text/html\r\n\r\n"
	// 						"<html>"
	// 						"<head>"
	// 						"<style>"
	// 						"h1 {"
	// 						"    text-align: center;"
	// 						"}"
	// 						"</style>"
	// 						"</head>"
	// 						"<body>"
	// 						"<h1>" +
	// 						status + "</h1>"
	// 						"</body>"
	// 						"</html>";
	// 	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
	// 	{
	// 		puts("Send failed");
	// 		return;
	// 	}
	// }
	// else
	// {

	std::string message = "HTTP/1.1 200 OK\r\nContent-Type: "+ContentType +"\r\n\r\n" + HtmlFile;
	// std::cout << message << std::endl;
	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
	{
		std::cout << "Send failed" << std::endl;
		return;
	}
	// }
	clear_all();
}

void Server::Read_Request_From_Client()
{
	char buffer[BUFFER_SIZE];
	while (1)
	{
		bzero(buffer, BUFFER_SIZE);
		bytes_read = read(socket_fd_client, buffer, BUFFER_SIZE - 1);
		if (bytes_read < 0)
		{
			std::cout << "Failed to read from socket. " << std::endl;
			exit(EXIT_FAILURE);
		}
		else
		{
			std::cout << "Read from socket. " << std::endl;
		}
		sBuffer.append(buffer);
		if (bytes_read < BUFFER_SIZE - 1)
		{
			buffer[bytes_read] = '\0';
			break;
		}
	}
	ReadHtmlFile();
	parseRequest(sBuffer); // parseRequest function is used to parse the request and store the parsed data in the map
	set_status("200 OK");
	std::cout << sBuffer << std::endl;
	// print_all_parseRequest();
	sBuffer.clear();
	Send_Response_To_Client(); // Send_Response_To_Client function is used to send a response to the client
	HtmlFile.clear();
}

void Server::getMyIpAddress()
{
	char buffer[1024];
	ip_address = "";

	struct hostent *host; // Structure containing host information
	if (gethostname(buffer, sizeof(buffer)) != -1)
	{
		host = gethostbyname(buffer); // Get the IP address
		if (host != NULL)
		{
			ip_address = inet_ntoa(*((struct in_addr *)host->h_addr_list[0])); // Convert IP to string
			address.sin_addr.s_addr = inet_addr(ip_address.c_str());		   // Use c_str() to get a const char* from the string
		}
	}
	return;
}

void Server::ReadHtmlFile()
{
	std::ifstream file("Test_page/index.html");
	std::string str;
	while (std::getline(file, str))
	{
		HtmlFile += str;
	}
	// std::cout << HtmlFile << std::endl;
	file.close();
}


std::string  Server::GetMIMEType(std::string key)
{
    std::map<std::string, std::string> mime_types;
    mime_types[".bmp"] = "image/bmp";
    mime_types[".css"] = "text/css";
    mime_types[".html"] = "text/html";
    mime_types[".js"] = "text/javascript";
    mime_types[".json"] = "application/json";
    mime_types[".xml"] = "application/xml";
    mime_types[".pdf"] = "application/pdf";
    mime_types[".zip"] = "application/zip";
    mime_types[".jpeg"] = "image/jpeg";
    mime_types[".png"] = "image/png";
    mime_types[".gif"] = "image/gif";
    mime_types[".svg"] = "image/svg+xml";
    mime_types[".mp3"] = "audio/mpeg";
    mime_types[".ogg"] = "audio/ogg";
    mime_types[".wav"] = "audio/wav";
    mime_types[".mp4"] = "video/mp4";
    mime_types[".mpeg"] = "video/mpeg";
    mime_types[".mov"] = "video/quicktime";
    mime_types[".ogg"] = "application/ogg";
    mime_types[".swf"] = "application/x-shockwave-flash";
    mime_types[".js"] = "application/javascript";
    mime_types[".js"] = "application/x-javascript";
    mime_types[".xml"] = "text/xml";
    mime_types[".x-www-form-urlencoded"] = "application/x-www-form-urlencoded";
    mime_types[".form-data"] = "multipart/form-data";
    mime_types[".rss"] = "application/rss+xml";
    mime_types[".atom"] = "application/atom+xml";
    mime_types[".rdf"] = "application/rdf+xml";
    mime_types[".soap"] = "application/soap+xml";
    mime_types[".mathml"] = "application/mathml+xml";
    mime_types[".rdf"] = "application/rdf+xml";
    mime_types[".zip"] = "application/zip";
    mime_types[".gz"] = "application/gzip";
    mime_types[".tar"] = "application/x-tar";
    mime_types[".compressed"] = "application/x-compressed";
    mime_types[".gzip"] = "application/x-gzip";
    mime_types[".bzip2"] = "application/x-bzip2";
    mime_types[".7z"] = "application/x-7z-compressed";
    mime_types[".rar"] = "application/x-rar-compressed";
    mime_types[".zip-compressed"] = "application/x-zip-compressed";
    mime_types[".stuffit"] = "application/x-stuffit";
    mime_types[".dvi"] = "application/x-dvi";
    mime_types[".rtf"] = "application/rtf";
    mime_types[".latex"] = "application/x-latex";
    mime_types[".postscript"] = "application/postscript";
    mime_types[".tex"] = "application/x-tex";
    mime_types[".sh"] = "application/x-sh";
    mime_types[".bsh"] = "application/x-bsh";
    mime_types[".csh"] = "application/x-csh";
    mime_types[".perl"] = "application/x-perl";
    mime_types[".python"] = "application/x-python";
    mime_types[".php"] = "application/x-php";
    mime_types[".java"] = "application/x-java";
    mime_types[".ruby"] = "application/x-ruby";
    mime_types[".shellscript"] = "application/x-shellscript";
    mime_types[".xhtml"] = "application/xhtml+xml";
    mime_types[".xop"] = "application/xop+xml";
    mime_types[".soap"] = "application/soap+xml";
    mime_types[".atom"] = "application/atom+xml";
    mime_types[".xslt"] = "application/xslt+xml";
    mime_types[".xspf"] = "application/xspf+xml";
    mime_types[".xaml"] = "application/xaml+xml";
    mime_types[".x-silverlight-app"] = "application/x-silverlight-app";
    mime_types[".cdlink"] = "application/x-cdlink";
    mime_types[".certificates"] = "application/x-pkcs7-certificates";
    mime_types[".pkcs7-mime"] = "application/pkcs7-mime";
    mime_types[".pkcs7-signature"] = "application/pkcs7-signature";
    mime_types[".pkcs8"] = "application/pkcs8";
    mime_types[".pkcs10"] = "application/pkcs10";
    mime_types[".pkcs12"] = "application/x-pkcs12";
    mime_types[".x509-ca-cert"] = "application/x-x509-ca-cert";
    mime_types[".x509-user-cert"] = "application/x-x509-user-cert";
    mime_types[".pkcs7-certreqresp"] = "application/x-pkcs7-certreqresp";
    mime_types[".s-mime"] = "application/s-mime";
    mime_types[".pkix-crl"] = "application/pkix-crl";
    mime_types[".pgp-encrypted"] = "application/pgp-encrypted";
    mime_types[".pgp-signature"] = "application/pgp-signature";
    mime_types[".pem-file"] = "application/x-pem-file";
    mime_types[".x509-ca-cert"] = "application/x-x509-ca-cert";
    mime_types[".x509-user-cert"] = "application/x-x509-user-cert";
    mime_types[".x509-email-cert"] = "application/x-x509-email-cert";
    mime_types[".x509-root-cert"] = "application/x-x509-root-cert";
    mime_types[".odt"] = "application/vnd.oasis.opendocument.text";
    mime_types[".odp"] = "application/vnd.oasis.opendocument.presentation";
    mime_types[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    mime_types[".cab-compressed"] = "application/vnd.ms-cab-compressed";
    mime_types[".odg"] = "application/vnd.oasis.opendocument.graphics";
    mime_types[".tika-msoffice"] = "application/x-tika-msoffice";
    mime_types[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mime_types[".dotx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.template";
    mime_types[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mime_types[".xltx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
    mime_types[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mime_types[".potx"] = "application/vnd.openxmlformats-officedocument.presentationml.template";
    mime_types[".ppsx"] = "application/vnd.openxmlformats-officedocument.presentationml.slideshow";
    mime_types[".sldx"] = "application/vnd.openxmlformats-officedocument.presentationml.slide";
    mime_types[".opendocument.graphics"] = "application/vnd.openxmlformats-officedocument.graphics";
    mime_types[".opendocument.graphics-template"] = "application/vnd.openxmlformats-officedocument.graphics-template";
    mime_types[".opendocument.graphics-template"] = "application/vnd.openxmlformats-officedocument.graphics-template";
    mime_types[".opendocument.formula-template"] = "application/vnd.openxmlformats-officedocument.formula-template";
    mime_types[".opendocument.formula-template"] = "application/vnd.openxmlformats-officedocument.formula-template";
    mime_types[".opendocument.drawingml.diagram-layout"] = "application/vnd.openxmlformats-officedocument.drawingml.diagram-layout";
    mime_types[".opendocument.drawingml.diagram"] = "application/vnd.openxmlformats-officedocument.drawingml.diagram";
    mime_types[".opendocument.drawingml.chart"] = "application/vnd.openxmlformats-officedocument.drawingml.chart";
    mime_types[".ms-project"] = "application/vnd.ms-project";
    mime_types[".visio"] = "application/vnd.visio";
    mime_types[".x-mspublisher"] = "application/x-mspublisher";
    mime_types[".xpsdocument"] = "application/vnd.ms-xpsdocument";
    mime_types[".3gpp.pic-bw-large"] = "application/vnd.3gpp.pic-bw-large";
    mime_types[".3gpp.pic-bw-small"] = "application/vnd.3gpp.pic-bw-small";
    mime_types[".3gpp.pic-bw-var"] = "application/vnd.3gpp.pic-bw-var";
    mime_types[".3gpp2.tcap"] = "application/vnd.3gpp2.tcap";
    mime_types[".icq"] = "application/x-icq";
    mime_types[".aim"] = "application/x-aim";
    mime_types[".msn-messenger"] = "application/x-msn-messenger";
    mime_types[".miranda"] = "application/x-miranda";
    mime_types[".t-relay"] = "application/t-relay";
    mime_types[".kopete"] = "application/x-kopete";
    mime_types[".gaim"] = "application/x-gaim";
    mime_types[".mbox"] = "application/mbox";
    mime_types[".vocaltec-media"] = "application/vocaltec-media";
    mime_types[".vocaltec-media-control"] = "application/vocaltec-media-control";
    mime_types[".dvi"] = "application/x-dvi";
    mime_types[".xpinstall"] = "application/x-xpinstall";
    mime_types[".sdp"] = "application/sdp";
    mime_types[".cpio"] = "application/x-cpio";
    mime_types[".tcl"] = "application/x-tcl";
    mime_types[".tex-tfm"] = "application/x-tex-tfm";
    mime_types[".texinfo"] = "application/x-texinfo";
    mime_types[".tei+xml"] = "application/tei+xml";
    mime_types[".sru+xml"] = "application/sru+xml";
    mime_types[".font-woff"] = "application/x-font-woff";
    mime_types[".font-otf"] = "application/x-font-otf";
    mime_types[".font-ttf"] = "application/x-font-ttf";
    mime_types[".java-archive"] = "application/java-archive";
    mime_types[".java-vm"] = "application/java-vm";
    mime_types[".msdownload"] = "application/x-msdownload";
    mime_types[".executable"] = "application/x-executable";
    mime_types[".oda"] = "application/oda";
    mime_types[".opentype-font"] = "application/x-opentype-font";
    mime_types[".ms-wmd"] = "application/x-ms-wmd";
    mime_types[".ms-wmz"] = "application/x-ms-wmz";
    mime_types[".winhlp"] = "application/winhlp";
    mime_types[".wais-source"] = "application/x-wais-source";
    mime_types[".ms-access"] = "application/vnd.ms-access";
    mime_types[".bcpio"] = "application/x-bcpio";
    mime_types[".dbf"] = "application/x-dbf";
    mime_types[".director"] = "application/x-director";
    mime_types[".freemind"] = "application/x-freemind";
    mime_types[".msmediaview"] = "application/x-msmediaview";
    mime_types[".msmoney"] = "application/x-msmoney";
    mime_types[".pkcs7-certificates"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
    mime_types[".pkcs7-certificates"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mime_types[".ms-pki.stl"] = "application/vnd.ms-pki.stl";
    mime_types[".ms-powerpoint"] = "application/vnd.ms-powerpoint";
    mime_types[".ms-pki.pko"] = "application/vnd.ms-pki.pko";
    mime_types[".ms-pki.seccat"] = "application/vnd.ms-pki.seccat";
    mime_types[".quicktimeplayer"] = "application/x-quicktimeplayer";
    mime_types[".shar"] = "application/x-shar";
    mime_types[".stuffit"] = "application/x-stuffit";
    mime_types[".sv4cpio"] = "application/x-sv4cpio";
    mime_types[".sv4crc"] = "application/x-sv4crc";
    mime_types[".sun.xml.draw"] = "application/vnd.sun.xml.draw";
    mime_types[".sun.xml.calc"] = "application/vnd.sun.xml.calc";
    mime_types[".sun.xml.math"] = "application/vnd.sun.xml.math";
    mime_types[".sun.xml.impress"] = "application/vnd.sun.xml.impress";
    mime_types[".sun.xml.writer"] = "application/vnd.sun.xml.writer";

    std::map<std::string, std::string>::iterator it = mime_types.find(key);
    if (it != mime_types.end()) {
		return it->second;
        std::cout << "Value for key " << key << " is: " << it->second << std::endl;
    } else {
        std::cout << "Key not found in the map." << std::endl;
    }
	return "";
}