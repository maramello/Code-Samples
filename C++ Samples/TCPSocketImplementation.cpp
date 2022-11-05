/*
* ==============================================================
* File   : TCPSocketImplementation.cpp
* Purpose: Creates a Windows socket that can resolve HTTP
           requests from anywhere on the web and receive
           data via TCP connection. Also resolves DNS requests.
  Author : Rohit Saini
* Date   : 11/2022
* All code is Copyrighted to owner, Rohit Saini
* ==============================================================
*/

#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>

//Process HTTP request, called on separate thread
void ProcessRequest(SOCKET cliSocket, sockaddr* addrInfo)
{
    //Set blocking mode since it is on a different thread
    u_long nonZeroValue = 0;
    ioctlsocket(cliSocket, FIONBIO, &nonZeroValue);

    //Declare buffer for storing server message
    char receiveBuf[2048];

    //Declare bytes received and total
    int bytesReceived = 0;
    int totalBytesReceived = 0;

    do
    {
        //Call recv to receive data at end of buffer
        bytesReceived = recv(cliSocket, receiveBuf + totalBytesReceived, 1024, 0);

        //check for socket error
        if (bytesReceived == SOCKET_ERROR)
        {
            //get last error
            int error = WSAGetLastError();

            //confirm blocking error
            if ((error != EAGAIN) && (error != WSAEWOULDBLOCK))
            {
                std::cout << "Error in recv function for HTTP request: " << error << std::endl;
                return;
            }

            //output . while we wait for all data
            std::cout << ". ";
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::flush(std::cout);
        }

        //add total bytes if bytes were received this frame
        else if (bytesReceived > 0)
        {
            //confirm there is no buffer overflow
            if (totalBytesReceived + bytesReceived > 2048)
            {
                std::cout << "Buffer cannot store more information without overflow, breaking" << std::endl;
                break;
            }

            totalBytesReceived += bytesReceived;
        }

    } while (bytesReceived != 0);

    //9b. Show that there is nothing left to receive
    int shutdownErr = shutdown(cliSocket, SD_RECEIVE);

    //Confirm shutdown had no errors
    if (shutdownErr == SOCKET_ERROR)
    {
        std::cout << "Error in shutting down socket for HTTP request:" << WSAGetLastError() << std::endl;
        return;
    }

    //9c. Host name resolution 
    char* ind = strstr(receiveBuf, ":");
    ind = (ind + 1);

    //do nothing with empty string
    if (ind == NULL) return;

    //create static buffer for host name
    char hostname[1024];
    int i = 0; //index to insert in buffer

    //loop for no carriage returns and newlines, and add characters to host name
    while (*ind != '\r' && *ind != '\n' && i < 1024) {
        if (*ind != ' ') hostname[i++] = *ind;
        ind++;
    }

    //end string
    hostname[i] = '\0'; 

    //intiialise socket address for server
    sockaddr_in webAdd; 
    memset(webAdd.sin_zero, 0, sizeof(webAdd.sin_zero));
    webAdd.sin_family = AF_INET;  
    webAdd.sin_port = htons(80); 

    //server settings
    int fam = AF_INET;
    int prot = IPPROTO_TCP;
    int socType = SOCK_STREAM;
    int sSize = sizeof(webAdd);

    // Try inet_pton for DNS
    int result = inet_pton(AF_INET, hostname, &webAdd.sin_addr);

     //incase inet pton fails
    addrinfo* info = nullptr;

     //cast to sockaddr
    sockaddr* web = (sockaddr*)&webAdd;

    if (result == SOCKET_ERROR)
    {
        std::cout << "Error trying to resolve host as a 4 digit separated string: " << WSAGetLastError() << std::endl;
        return;
    }

    //assume didnt fail 
    bool inet_pton_failed = false;

    //check if failed
    if (result == 0)
    {
        //set failed
        inet_pton_failed = true;

        //DNS resolution
        result = getaddrinfo(hostname, "80", nullptr, &info);

        if (result != 0)
        {
            std::cout << "Error trying to resolve hostname using DNS: " << WSAGetLastError() << std::endl;
            return;
        }

        //server settings set again
        sSize = static_cast<int> (info->ai_addrlen);
        web = info->ai_addr;
        fam = info->ai_family;
        prot = info->ai_protocol;
        socType = info->ai_socktype;
    }

    //web server connection socket
    SOCKET serverSock = socket(fam, socType, prot);

    if (serverSock == INVALID_SOCKET)
    {
        // Exit if the socket couldn't be created
        std::cout << "Error in socket creation: " << WSAGetLastError() << std::endl;
        return;
    }

    //connect error code check
    int connErr = connect(serverSock, web, sSize);

    //validate tcp conn
    if (connErr == SOCKET_ERROR)
    {
        std::cout << "Error connecting tcp: " << WSAGetLastError() << std::endl;
        return;
    }

    //free addr info
    if (inet_pton_failed) freeaddrinfo(info);

    //send request
    result = send(serverSock, receiveBuf, totalBytesReceived, 0);

    //validate request
    if (result == SOCKET_ERROR) {
        std::cout << "Error, http request failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSock);
        WSACleanup();
        return;
    }

    //loop to receive server data and send client response
    do
    {
        char recvBuffer[2048];

        //receive data
        result = recv(serverSock, recvBuffer, 1024, 0);

        //error, failed to recv
        if (result == SOCKET_ERROR) {
            std::cout << "Error in recv and receiving server data: " << WSAGetLastError() << std::endl;
            return;
        }

        //client sent data
        else if (result > 0) {
            int err = send(cliSocket, recvBuffer, result, 0);
            if (err == SOCKET_ERROR)
                std::cout << "Error in client send: " << WSAGetLastError() << std::endl;
        }

    } while (result != 0);

    //sockets close
    //error codes stored
    int err = shutdown(cliSocket, SD_SEND);
    int err2 = shutdown(serverSock, SD_RECEIVE);
    int err3 = shutdown(serverSock, SD_SEND);

    //shut down socket error code check
    if (err == SOCKET_ERROR || err2 == SOCKET_ERROR || err3 == SOCKET_ERROR)
    {
        std::cout << "Error shutting down sockets: " << WSAGetLastError() << std::endl;
        return;
    }

    //close the sockets
    if (closesocket(serverSock) == SOCKET_ERROR)
    {
        std::cout << "Error closing server socket: " << WSAGetLastError() << std::endl;
    }

    //close second socket
    if (closesocket(cliSocket) == SOCKET_ERROR)
        std::cout << "Error closing client socket: " << WSAGetLastError() << std::endl;

}

//Start a new thread for a received request
void HandleConnection(SOCKET socket, sockaddr* addrInfo, int* addrLength)
{
    std::thread clientThread(ProcessRequest, socket, addrInfo);
    clientThread.detach();
}


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Please provide a port for the client to run on" << std::endl;
         return 1;
    }

    //Parse port from command line
    int port = atoi(argv[1]);

    //Local port bind, so must be within standard range
    if (port < 1025 || port > 65535)
    {
        std::cout << "Invalid port number entered. Enter a number between 1025 and 65535 for proper port bind attempt." << std::endl;

        return 1;
    }

    //Initialize WSA
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    //Validate WSA, print error if invalid
    if (result != 0) {
        std::cout << "Error in WSA initialization:" << WSAGetLastError() << std::endl;
    }

    //Create the listening address for local host
    sockaddr_in dest;
    memset(dest.sin_zero, 0, sizeof(dest.sin_zero));
    dest.sin_port = htons(port);
    dest.sin_family = AF_INET;
    dest.sin_addr.S_un.S_addr = INADDR_ANY;

     //Create socket
    SOCKET tcpSocket = socket(AF_INET, SOCK_STREAM, 0);

    //Validate socket, print error message if invalid
    if (tcpSocket == INVALID_SOCKET)
    {
        std::cout << "Error in Socket creation:" << WSAGetLastError() << std::endl;
        return -1;
    }

    //Set non blocking mode, ensures program does not break anything else
    u_long nonZeroValue = 1;
    ioctlsocket(tcpSocket, FIONBIO, &nonZeroValue);

    //Call bind for listening socket
    int bindErr = bind(tcpSocket, (sockaddr*)&dest, sizeof(dest));

    if (bindErr == SOCKET_ERROR)
    {
        std::cout << "Error in binding TCP Socket:" << WSAGetLastError() << std::endl;
        return -1;
    }

    //Call listen for the TCP socket
    int listenErr = listen(tcpSocket, SOMAXCONN);

    if (listenErr == SOCKET_ERROR)
    {
        std::cout << "Error in listening for TCP Socket:" << WSAGetLastError() << std::endl;
        return -1;
    }

    std::cout << "Listening on port " << port << std::endl;

    //Listening loop
    do
    {
        //Accept incoming connections
        sockaddr connAddress;
        int connLen = sizeof(connAddress);
        SOCKET conn = accept(tcpSocket, &connAddress, &connLen);

        if (conn == INVALID_SOCKET)
        {
            int err = WSAGetLastError();

            if (err != WSAEWOULDBLOCK && err != EAGAIN)
            {
                std::cout << "Error in accepting a TCP Socket:" << WSAGetLastError() << std::endl;
                continue;
            }
        }

        else
        {
            //Accept succeeded, work on new thread here
            HandleConnection(conn, &connAddress, &connLen);
        }

        //sleep for 100ms while waiting
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    } while (true);
    
    //Close sockets and cleanup
    closesocket(tcpSocket);

    WSACleanup();
}

