#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

/*
	+ = Added
	- = Remove
	! = Priority
	* = Fix
	? = Maybe

	To Do:
		[!] IPv6 support
		[?] Cache Our Responses

*/
class PmmHTTPRequest
{
private:

	short Port;
	SOCKET theSocket;
	std::string Host, Path, Response;

	/*
	Init Winsock
	*/
	bool InitWinsock()
	{
		WSADATA WsaDat;

		if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
		{
			std::cerr << "Failed To Start Winsocket With WSAStartup()\n";
			WSACleanup();
			return(false);
		}

		return(true);
	}

	/*
	Loop Recieve (Including Headers)
	*/
	std::string loopRecieve(SOCKET Sock)
	{
		char recvBuf[256];	// For Transporting Data
		std::string outBuf; // Output String
		unsigned int nret;

		while (true)
		{
			nret = recv(Sock, recvBuf, sizeof(recvBuf), 0);

			if (nret == -1)
			{
				std::cerr << "A Socket Error Occured(-1)\n";
				break;
			}
			else if (nret == 0)
			{
				break;
			}
			// Append Newly Recieved Bytes To String
			outBuf.append(recvBuf, nret);
		}

		return(outBuf);
	}

	/*
		Resolve && Connect To Host
		(Supports Round Robin DNS)
	*/
	bool Resolve_And_Connect()
	{
		size_t i;

		ADDRINFO hints = { 0 };
		hints.ai_flags = AI_ALL;
		hints.ai_family = PF_INET;
		hints.ai_protocol = IPPROTO_IPV4;

		ADDRINFO *pResult = nullptr;
		if (getaddrinfo(Host.c_str(), nullptr, &hints, &pResult))
		{
			std::cerr << "Failed To getaddrinfo()\n";
			return(false);
		}

		sockaddr_in servAddr = { 0 };
		servAddr.sin_family = AF_INET; // AF_INET6
		servAddr.sin_port = htons(Port);

		for (i = 0; i < pResult->ai_addrlen; i++)
		{
			servAddr.sin_addr.S_un.S_addr = (ULONG)((sockaddr_in*)&pResult->ai_addr[i])->sin_addr.S_un.S_addr;
			if (connect(theSocket, (sockaddr*)&servAddr, sizeof(servAddr)) != SOCKET_ERROR)
			{
				return(true);
			}
		}

		freeaddrinfo(pResult);
		return(false);
	}
public:

	/*
		Get Our Response
	*/
	std::string getResponse()
	{
		return(Response);
	}

	/*
		Set Our Host
	*/
	void setHost(std::string argHost)
	{
		Host = argHost;
	}

	/*
		Set Our Port
	*/
	void setPort(short argPort)
	{
		Port = argPort;
	}

	/*
		Constructor
	*/
	PmmHTTPRequest(std::string argHost, short argPort) : Host(argHost), Port(argPort)
	{
		if (InitWinsock() != true)
		{
			std::cerr << "Failed To Start Winsock\n";
		}

		theSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (theSocket == INVALID_SOCKET)
		{
			std::cerr << "Socket Is Invalid, Is Winsock Initialized?\n";
		}
	}

	/*
		Destructor
	*/
	~PmmHTTPRequest()
	{
		closesocket(theSocket);
	}


	/*
		Get Web Page Contents
	*/
	void getWebPage(std::string Path)
	{
		int nret;

		// Strip Out 'http://' && 'https://'
		if (Host.find("http://") != -1) {
			Host = Host.substr(Host.find("http://") + 7);
		}
		else if (Host.find("https://") != -1) {
			std::cerr << "SSL Not Supported Yet\n";
			return;
		}

		if (Resolve_And_Connect() != true)
		{
			std::cerr << "Couldnt Resolve & Connect\n";
		}

		std::string request;
		request = "GET " + Path + " HTTP/1.1" + "\r\n";
		request += "Host: " + Host + "\r\n";
		request += "Accept: */*\r\n";
		request += "Accept-Language: en-us\r\n";
		request += "Connection: close\r\n";
		request += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0\r\n";
		request += "Referer: http://" + Host + "\r\n";
		request += "\r\n";

		nret = send(theSocket, request.c_str(), request.length(), 0);
		if (nret == SOCKET_ERROR)
		{
			std::cerr << "Failed To Send To Host\n";
			return;
		}

		// Get Our Response
		Response = loopRecieve(theSocket);
	}

	/*
		Basic POST
	*/
	void postWebPage(std::string Path, std::vector<std::pair<std::string, std::string>> PostParams)
	{
		int nret;

		// Strip Out 'http://' && 'https://'
		if (Host.find("http://") != -1) {
			Host = Host.substr(Host.find("http://") + 7);
		}
		else if (Host.find("https://") != -1) {
			std::cerr << "SSL Not Supported Yet\n";
			return;
		}

		if (Resolve_And_Connect() != true)
		{
			std::cerr << "Couldnt Resolve & Connect\n";
		}

		// Structure POST Data Properly
		std::string concatPostData;
		for (auto i : PostParams)
		{
			concatPostData += i.first + "=" + i.second + "&";
		}

		// Pop Off Extra '&' At The End Of Loop
		concatPostData.pop_back();

		// Construct HEADER
		std::string header;
		header = "POST " + Path + " HTTP/1.1\r\n";
		header += "Host: " + Host + ":" + std::to_string(Port) + "\r\n";
		header += "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0\r\n";
		header += "Referer: http://" + Host + "\r\n";
		header += "Content-Type: application/x-www-form-urlencoded\r\n";
		header += "Content-Length: " + std::to_string(concatPostData.length()) + "\r\n";
		header += "Accept-Charset: utf-8\r\n";
		header += "Connection: close\r\n\r\n";
		header += concatPostData + "\r\n";
		header += "\r\n";

		// Need To Check Here That We Sent All Data
		nret = send(theSocket, header.c_str(), header.length(), 0);
		if (nret == SOCKET_ERROR)
		{
			std::cerr << "Failed To Send To Host\n";
			return;
		}

		// Get Our Response
		Response = loopRecieve(theSocket);
	}
};