#ifndef CLIENT
#define CLIENT

#include <iostream>
#include <sys/types.h>    // socket, bind
#include <sys/socket.h>   // socket, bind, listen, inet_ntoa
#include <netinet/in.h>   // htonl, htons, inet_ntoa
#include <arpa/inet.h>    // inet_ntoa
#include <netdb.h>     // gethostbyname
#include <unistd.h>    // read, write, close
#include <strings.h>     // bzero
#include <netinet/tcp.h>  // SO_REUSEADDR
#include <sys/uio.h>      // writev
#include <iostream>

using namespace std;

class Client {

private:
    int serverPort;                     // Port #
    struct hostent* host;               // Host
    char* hostName;                     // Host name
    char* p2pIP;
    sockaddr_in sendSockAddr;           // SockAddr
    int clientSd;                       // Client socket descriptor
    int peerSd;

public:
    Client() {
        // Set up sock addr
        sendSockAddr.sin_family = AF_INET;

        // Set up client socket
        clientSd = socket(AF_INET, SOCK_STREAM, 0);

        p2pIP = new char[INET_ADDRSTRLEN];
    }

    ~Client() {
        delete[] p2pIP;
    }

    // Connect to server
    void connectToServer() {   
        serverPort = 1885;
        hostName = "csslab13.uwb.edu";

        // Set up host
        host = gethostbyname(hostName);
        sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)* host->h_addr_list));
        sendSockAddr.sin_port = htons(serverPort);

        int connectStatus = connect(clientSd, (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
        if (connectStatus < 0) {
            std::cerr << "Failed to connect to the server" << std::endl;
        }
    }

    // Register user
    // @param username - unique username for server
    void registerUser(char* username) {
        if (write(clientSd, username, sizeof(username)) < 0) {
            std::cerr << "Registration unsuccessful" << std::endl;
        }
    }

    // Show the available lobbies
    void showLobbies() {
        char msg[2] = "L";
        if (write(clientSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Showing lobbies unsuccessful" << std::endl;
        }

        char lobbies[2048];
        if (read(clientSd, lobbies, sizeof(lobbies)) < 0) {
            std::cerr << "Lobby joining unsuccessful" << std::endl;
        }
        cout << "\n" << lobbies << endl;
    }
    
    // Join a lobby
    // @param lobbyName - the name of the lobby to join to or create
    // @return true if lobby joined, false if lobby created
    bool joinLobby(char* lobbyName) {
        if (write(clientSd, lobbyName, sizeof(lobbyName)) < 0) {
            std::cerr << "Lobby joining unsuccessful" << std::endl;
        }

        char res[2];
        read(clientSd, res, sizeof(res));

        string response = string(res);
        if (response == "J") {
            read(clientSd, p2pIP, INET_ADDRSTRLEN);
            return true;
        }
        return false;
    }

    char* getPeerIP() {
        return p2pIP;
    }

    // Reconnect with the main server
    void reconnect() {
        char status[2];
        status[0] = 'D';
        if (write(clientSd, status, sizeof(status)) < 0) {
            std::cerr << "Reconnection unsuccessful" << std::endl;
        }
    }
};
#endif