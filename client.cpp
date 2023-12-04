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
    sockaddr_in sendSockAddr;           // SockAddr
    int clientSd;                       // Client socket descriptor

public:
    Client() {
        serverPort = 1885;
        hostName = "csslab13.uwb.edu";

        // Set up host
        host = gethostbyname(hostName);

        // Set up sock addr
        sendSockAddr.sin_family = AF_INET;
        sendSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)* host->h_addr_list));
        sendSockAddr.sin_port = htons(serverPort);

        // Set up client socket
        clientSd = socket(AF_INET, SOCK_STREAM, 0);
    }

    // Connect to server
    void connectTCP() {   
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
        showLobbies();
    }
    
    // Unregister user
    void unregisterUser() {
        close(clientSd);
    }

    // Show the available lobbies
    void showLobbies() {
        char msg[2] = "L";
        if (write(clientSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Showing lobbies unsuccessful" << std::endl;
        }

        char lobbies[5000];
        if (read(clientSd, lobbies, sizeof(lobbies)) < 0) {
            std::cerr << "Lobby joining unsuccessful" << std::endl;
        }
        cout << "\n\n\n" << lobbies << endl;
    }
    
    // Join a lobby
    bool joinLobby(char* lobbyName) {
        if (write(clientSd, lobbyName, sizeof(lobbyName)) < 0) {
            std::cerr << "Lobby joining unsuccessful" << std::endl;
            return false;
        }
        return true;
    }

    // Send messages in game
    void sendMessage(char* msg) {
        if (write(clientSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Sending message unsuccessful" << std::endl;
        }
    }

    // Send messages in game
    void recvMessage() {
        char msg[1024];
        if (read(clientSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Receiving message unsuccessful" << std::endl;
        }
        cout << msg << endl;
    }
};
#endif