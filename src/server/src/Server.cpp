/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hdagdagu <hdagdagu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 15:48:53 by hdagdagu          #+#    #+#             */
/*   Updated: 2023/11/06 16:03:38 by hdagdagu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server()
{
	parseConfig("config/default.conf"); // parseConfig function is used to parse the config file and store the parsed data in the map
	MIME_types_init();					// MIME_types_init function is used to initialize the map with the mime types
	getMyIpAddress();					// getMyIpAddress function is used to get the ip address of the server
	Setup_Server();						// Setup_Server function is used to setup the server
	listen_to_multiple_clients();		// listen_to_multiple_clients function is used to listen to multiple clients
}

void Server::listen_to_multiple_clients()
{
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
}

void Server::Accept_Connection()
{
	socklen_t address_lenght = sizeof(address);															   // socklen_t is a type of data type in socket.h header file which is used to represent the size of objects in bytes. It is used to represent the sizes of socket address structures. It is used in accept(), bind(), connect(), getsockname(), getpeername(), and socket() functions.
	if ((socket_fd_client = accept(socket_fd_server, (struct sockaddr *)&address, &address_lenght)) == -1) // accept function is used to accept a connection request from a client. It extracts the first connection request on the queue of pending connections for the listening socket, sockfd, creates a new connected socket, and returns a new file descriptor referring to that socket. At this point, connection is established between client and server, and they are ready to transfer data.
	{
		std::cout << "Failed to grab connection. " << std::endl;
		exit(EXIT_FAILURE);
	}
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
		sBuffer.append(buffer);
		if (bytes_read < BUFFER_SIZE - 1)
		{
			buffer[bytes_read] = '\0';
			break;
		}
	}

	try
	{
		ReadHtmlFile();
		parseRequest(sBuffer);
		get_matched_location_for_request_uri();
	}
	catch (const MY_exception::status_code_exception &e)
	{
		set_status(e.what());
		Send_Error_Response();
	}
	sBuffer.clear();
	HtmlFile.clear();
	clear_all();
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

std::string Server::GetMIMEType(std::string key)
{
	std::map<std::string, std::string>::iterator it = MIME_types.find(key);
	if (it != MIME_types.end())
	{
		return it->second;
	}
	return "";
}

void Server::Send_Error_Response()
{
	std::string message = get_Version() + " " + get_status() + "\r\nContent-Type: text/html\r\n\r\n"
															   "<html>"
															   "<head>"
															   "<style>"
															   "h1 {"
															   "    text-align: center;"
															   "}"
															   "</style>"
															   "</head>"
															   "<body>"
															   "<h1>" +
						  get_status() + "</h1>"
										 "</body>"
										 "</html>";
	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
	{
		puts("Send failed");
		return;
	}
}
void Server::Check_file_existence()
{
	std::string filename = getRoot() + "/" + getIndex();
	std::fstream file;
	file.open(filename.c_str(), std::ios::in); // Open file in read mode

	if (file.is_open())
	{
		file.close();
		
	}
	else
	{
		std::cout << "test" << std::endl;

		MY_exception e("404 Not Found");
		throw e;
	}
}
void Server::Send_Correct_Response()
{
	size_t pos = get_URI().find('.');
	std::string ContentType;
	if (pos != std::string::npos)
	{
		std::string ext = get_URI().substr(pos);
		ContentType = GetMIMEType(ext);
	}
	std::string message = "HTTP/1.1 200 OK\r\nContent-Type: " + ContentType + "\r\n\r\n" + HtmlFile;
	// std::cout << message << std::endl;
	if (send(socket_fd_client, message.c_str(), message.length(), 0) < 0)
	{
		std::cout << "Send failed" << std::endl;
		return;
	}
}
void Server::get_matched_location_for_request_uri()
{
	try
	{
		Check_file_existence();
		Send_Correct_Response();
	}
	catch (MY_exception &e)
	{
		set_status(e.what());
		Send_Error_Response();
	}
}

void Server::MIME_types_init()
{
	MIME_types[".bmp"] = "image/bmp";
	MIME_types[".css"] = "text/css";
	MIME_types[".html"] = "text/html";
	MIME_types[".js"] = "text/javascript";
	MIME_types[".json"] = "application/json";
	MIME_types[".xml"] = "application/xml";
	MIME_types[".pdf"] = "application/pdf";
	MIME_types[".zip"] = "application/zip";
	MIME_types[".jpeg"] = "image/jpeg";
	MIME_types[".png"] = "image/png";
	MIME_types[".gif"] = "image/gif";
	MIME_types[".svg"] = "image/svg+xml";
	MIME_types[".mp3"] = "audio/mpeg";
	MIME_types[".ogg"] = "audio/ogg";
	MIME_types[".wav"] = "audio/wav";
	MIME_types[".mp4"] = "video/mp4";
	MIME_types[".mpeg"] = "video/mpeg";
	MIME_types[".mov"] = "video/quicktime";
	MIME_types[".ogg"] = "application/ogg";
	MIME_types[".swf"] = "application/x-shockwave-flash";
	MIME_types[".js"] = "application/javascript";
	MIME_types[".js"] = "application/x-javascript";
	MIME_types[".xml"] = "text/xml";
	MIME_types[".x-www-form-urlencoded"] = "application/x-www-form-urlencoded";
	MIME_types[".form-data"] = "multipart/form-data";
	MIME_types[".rss"] = "application/rss+xml";
	MIME_types[".atom"] = "application/atom+xml";
	MIME_types[".rdf"] = "application/rdf+xml";
	MIME_types[".soap"] = "application/soap+xml";
	MIME_types[".mathml"] = "application/mathml+xml";
	MIME_types[".rdf"] = "application/rdf+xml";
	MIME_types[".zip"] = "application/zip";
	MIME_types[".gz"] = "application/gzip";
	MIME_types[".tar"] = "application/x-tar";
	MIME_types[".compressed"] = "application/x-compressed";
	MIME_types[".gzip"] = "application/x-gzip";
	MIME_types[".bzip2"] = "application/x-bzip2";
	MIME_types[".7z"] = "application/x-7z-compressed";
	MIME_types[".rar"] = "application/x-rar-compressed";
	MIME_types[".zip-compressed"] = "application/x-zip-compressed";
	MIME_types[".stuffit"] = "application/x-stuffit";
	MIME_types[".dvi"] = "application/x-dvi";
	MIME_types[".rtf"] = "application/rtf";
	MIME_types[".latex"] = "application/x-latex";
	MIME_types[".postscript"] = "application/postscript";
	MIME_types[".tex"] = "application/x-tex";
	MIME_types[".sh"] = "application/x-sh";
	MIME_types[".bsh"] = "application/x-bsh";
	MIME_types[".csh"] = "application/x-csh";
	MIME_types[".perl"] = "application/x-perl";
	MIME_types[".python"] = "application/x-python";
	MIME_types[".php"] = "application/x-php";
	MIME_types[".java"] = "application/x-java";
	MIME_types[".ruby"] = "application/x-ruby";
	MIME_types[".shellscript"] = "application/x-shellscript";
	MIME_types[".xhtml"] = "application/xhtml+xml";
	MIME_types[".xop"] = "application/xop+xml";
	MIME_types[".soap"] = "application/soap+xml";
	MIME_types[".atom"] = "application/atom+xml";
	MIME_types[".xslt"] = "application/xslt+xml";
	MIME_types[".xspf"] = "application/xspf+xml";
	MIME_types[".xaml"] = "application/xaml+xml";
	MIME_types[".x-silverlight-app"] = "application/x-silverlight-app";
	MIME_types[".cdlink"] = "application/x-cdlink";
	MIME_types[".certificates"] = "application/x-pkcs7-certificates";
	MIME_types[".pkcs7-mime"] = "application/pkcs7-mime";
	MIME_types[".pkcs7-signature"] = "application/pkcs7-signature";
	MIME_types[".pkcs8"] = "application/pkcs8";
	MIME_types[".pkcs10"] = "application/pkcs10";
	MIME_types[".pkcs12"] = "application/x-pkcs12";
	MIME_types[".x509-ca-cert"] = "application/x-x509-ca-cert";
	MIME_types[".x509-user-cert"] = "application/x-x509-user-cert";
	MIME_types[".pkcs7-certreqresp"] = "application/x-pkcs7-certreqresp";
	MIME_types[".s-mime"] = "application/s-mime";
	MIME_types[".pkix-crl"] = "application/pkix-crl";
	MIME_types[".pgp-encrypted"] = "application/pgp-encrypted";
	MIME_types[".pgp-signature"] = "application/pgp-signature";
	MIME_types[".pem-file"] = "application/x-pem-file";
	MIME_types[".x509-ca-cert"] = "application/x-x509-ca-cert";
	MIME_types[".x509-user-cert"] = "application/x-x509-user-cert";
	MIME_types[".x509-email-cert"] = "application/x-x509-email-cert";
	MIME_types[".x509-root-cert"] = "application/x-x509-root-cert";
	MIME_types[".odt"] = "application/vnd.oasis.opendocument.text";
	MIME_types[".odp"] = "application/vnd.oasis.opendocument.presentation";
	MIME_types[".ods"] = "application/vnd.oasis.opendocument.spreadsheet";
	MIME_types[".cab-compressed"] = "application/vnd.ms-cab-compressed";
	MIME_types[".odg"] = "application/vnd.oasis.opendocument.graphics";
	MIME_types[".tika-msoffice"] = "application/x-tika-msoffice";
	MIME_types[".docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
	MIME_types[".dotx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.template";
	MIME_types[".xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	MIME_types[".xltx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
	MIME_types[".pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
	MIME_types[".potx"] = "application/vnd.openxmlformats-officedocument.presentationml.template";
	MIME_types[".ppsx"] = "application/vnd.openxmlformats-officedocument.presentationml.slideshow";
	MIME_types[".sldx"] = "application/vnd.openxmlformats-officedocument.presentationml.slide";
	MIME_types[".opendocument.graphics"] = "application/vnd.openxmlformats-officedocument.graphics";
	MIME_types[".opendocument.graphics-template"] = "application/vnd.openxmlformats-officedocument.graphics-template";
	MIME_types[".opendocument.graphics-template"] = "application/vnd.openxmlformats-officedocument.graphics-template";
	MIME_types[".opendocument.formula-template"] = "application/vnd.openxmlformats-officedocument.formula-template";
	MIME_types[".opendocument.formula-template"] = "application/vnd.openxmlformats-officedocument.formula-template";
	MIME_types[".opendocument.drawingml.diagram-layout"] = "application/vnd.openxmlformats-officedocument.drawingml.diagram-layout";
	MIME_types[".opendocument.drawingml.diagram"] = "application/vnd.openxmlformats-officedocument.drawingml.diagram";
	MIME_types[".opendocument.drawingml.chart"] = "application/vnd.openxmlformats-officedocument.drawingml.chart";
	MIME_types[".ms-project"] = "application/vnd.ms-project";
	MIME_types[".visio"] = "application/vnd.visio";
	MIME_types[".x-mspublisher"] = "application/x-mspublisher";
	MIME_types[".xpsdocument"] = "application/vnd.ms-xpsdocument";
	MIME_types[".3gpp.pic-bw-large"] = "application/vnd.3gpp.pic-bw-large";
	MIME_types[".3gpp.pic-bw-small"] = "application/vnd.3gpp.pic-bw-small";
	MIME_types[".3gpp.pic-bw-var"] = "application/vnd.3gpp.pic-bw-var";
	MIME_types[".3gpp2.tcap"] = "application/vnd.3gpp2.tcap";
	MIME_types[".icq"] = "application/x-icq";
	MIME_types[".aim"] = "application/x-aim";
	MIME_types[".msn-messenger"] = "application/x-msn-messenger";
	MIME_types[".miranda"] = "application/x-miranda";
	MIME_types[".t-relay"] = "application/t-relay";
	MIME_types[".kopete"] = "application/x-kopete";
	MIME_types[".gaim"] = "application/x-gaim";
	MIME_types[".mbox"] = "application/mbox";
	MIME_types[".vocaltec-media"] = "application/vocaltec-media";
	MIME_types[".vocaltec-media-control"] = "application/vocaltec-media-control";
	MIME_types[".dvi"] = "application/x-dvi";
	MIME_types[".xpinstall"] = "application/x-xpinstall";
	MIME_types[".sdp"] = "application/sdp";
	MIME_types[".cpio"] = "application/x-cpio";
	MIME_types[".tcl"] = "application/x-tcl";
	MIME_types[".tex-tfm"] = "application/x-tex-tfm";
	MIME_types[".texinfo"] = "application/x-texinfo";
	MIME_types[".tei+xml"] = "application/tei+xml";
	MIME_types[".sru+xml"] = "application/sru+xml";
	MIME_types[".font-woff"] = "application/x-font-woff";
	MIME_types[".font-otf"] = "application/x-font-otf";
	MIME_types[".font-ttf"] = "application/x-font-ttf";
	MIME_types[".java-archive"] = "application/java-archive";
	MIME_types[".java-vm"] = "application/java-vm";
	MIME_types[".msdownload"] = "application/x-msdownload";
	MIME_types[".executable"] = "application/x-executable";
	MIME_types[".oda"] = "application/oda";
	MIME_types[".opentype-font"] = "application/x-opentype-font";
	MIME_types[".ms-wmd"] = "application/x-ms-wmd";
	MIME_types[".ms-wmz"] = "application/x-ms-wmz";
	MIME_types[".winhlp"] = "application/winhlp";
	MIME_types[".wais-source"] = "application/x-wais-source";
	MIME_types[".ms-access"] = "application/vnd.ms-access";
	MIME_types[".bcpio"] = "application/x-bcpio";
	MIME_types[".dbf"] = "application/x-dbf";
	MIME_types[".director"] = "application/x-director";
	MIME_types[".freemind"] = "application/x-freemind";
	MIME_types[".msmediaview"] = "application/x-msmediaview";
	MIME_types[".msmoney"] = "application/x-msmoney";
	MIME_types[".pkcs7-certificates"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.template";
	MIME_types[".pkcs7-certificates"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
	MIME_types[".ms-pki.stl"] = "application/vnd.ms-pki.stl";
	MIME_types[".ms-powerpoint"] = "application/vnd.ms-powerpoint";
	MIME_types[".ms-pki.pko"] = "application/vnd.ms-pki.pko";
	MIME_types[".ms-pki.seccat"] = "application/vnd.ms-pki.seccat";
	MIME_types[".quicktimeplayer"] = "application/x-quicktimeplayer";
	MIME_types[".shar"] = "application/x-shar";
	MIME_types[".stuffit"] = "application/x-stuffit";
	MIME_types[".sv4cpio"] = "application/x-sv4cpio";
	MIME_types[".sv4crc"] = "application/x-sv4crc";
	MIME_types[".sun.xml.draw"] = "application/vnd.sun.xml.draw";
	MIME_types[".sun.xml.calc"] = "application/vnd.sun.xml.calc";
	MIME_types[".sun.xml.math"] = "application/vnd.sun.xml.math";
	MIME_types[".sun.xml.impress"] = "application/vnd.sun.xml.impress";
	MIME_types[".sun.xml.writer"] = "application/vnd.sun.xml.writer";
	MIME_types[".sun.xml.writer.global"] = "application/vnd.sun.xml.writer.global";
	MIME_types[".sun.xml.writer.template"] = "application/vnd.sun.xml.writer.template";
}