#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

void unchunkBody(std::string &chunkedBody)
{
    std::string body;
    std::istringstream input(chunkedBody);

    char c;
    while (input.get(c))
    {
        // Read the chunk size
        std::string chunk_size_hex;
        while (c != '\r')
        {
            chunk_size_hex += c;
            input.get(c);
        }
        input.get(c); // '\n'
        int chunk_size = std::stoi(chunk_size_hex, 0, 16);
        if (chunk_size == 0)
        {
            break;
        }
        // Read the chunk
        std::string chunk(chunk_size, '\0');
        input.read(&chunk[0], chunk_size);
        input.get(c); // '\r'
        input.get(c); // '\n'
        body += chunk;
    }
    std::cout << body << std::endl;
}

int main()
{

    // Your chunked HTTP request string
    std::string chunkedRequest = "95\r\n"
                                 "----------------------------380105625425180886922099\r\n"
                                 "Content-Disposition: form-data; name=\"file\"; filename=\"hello.txt\"\r\n"
                                 "Content-Type: text/plain\r\n"
                                 "\r\n"
                                 "6\r\n"
                                 "hello\r\n"
                                 "\r\n"
                                 "2\r\n"
                                 "\r\n"
                                 "65\r\n"
                                 "----------------------------380105625425180886922099\r\n"
                                 "Content-Disposition: form-data; name=\"hhhh\"\r\n"
                                 "\r\n"
                                 "7\r\n"
                                 "dasdasd\r\n"
                                 "\r\n"
                                 "2\r\n"
                                 "\r\n"
                                 "6b\r\n"
                                 "----------------------------380105625425180886922099\r\n"
                                 "Content-Disposition: form-data; name=\"5646546465\"\r\n"
                                 "\r\n"
                                 "8\r\n"
                                 "56456445\r\n"
                                 "3a\r\n"
                                 "\r\n"
                                 "----------------------------380105625425180886922099--\r\n"
                                 "\r\n"
                                 "0\r\n";

    // Convert to non-chunked
    unchunkBody(chunkedRequest);

    return 0;
}
