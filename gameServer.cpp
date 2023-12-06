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
#include <pthread.h>
#include <unordered_map>

using namespace std;

class GameServer {

private:
    int server_port = 987;
    sockaddr_in acceptSockAddr;
    int serverSd;
    // Accept the connection as a new socket
    sockaddr_in newsock;   // place to store parameters for the new connection
    socklen_t newsockSize = sizeof(newsock);

    char* p2pIp;
    sockaddr_in peerSockAddr;
    int peerSd;

public:
    GameServer() {
        bzero(&acceptSockAddr, sizeof(acceptSockAddr));     // zero out the data structure
        acceptSockAddr.sin_family = AF_INET;                        // using IP
        acceptSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);         // listen on any address this computer has
        acceptSockAddr.sin_port = htons(server_port);               // set the port to listen on

        serverSd = socket(AF_INET, SOCK_STREAM, 0);             // creates a new socket for IP using TCP

        // Bind the socket
        bind(serverSd, (sockaddr*) &acceptSockAddr, sizeof(acceptSockAddr));  // bind the socket using the parameters we set earlier
        
        // Listen on the socket
        int n = 1;
        listen(serverSd, n);  // listen on the socket and allow up to n connections to wait.
    }
    
    void waitInLobby() {
        cout << "Waiting for other player..." << endl;
        
        peerSd = accept(serverSd, (sockaddr *)&newsock, &newsockSize);  // grabs the new connection and assigns it a temporary socket
        if (peerSd < 0) {
            cerr << "Starting lobby unsuccessful" << endl;
        }
        cout << "YAY" << endl;
    }

    // Connect to peer's lobby
    bool connectToLobby() {
        cout << "Joining Lobby..." << endl;

        bzero(&peerSockAddr, sizeof(peerSockAddr));
        peerSockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        peerSockAddr.sin_port = htons(server_port);

        peerSd = socket(AF_INET, SOCK_STREAM, 0);

        int connectStatus = connect(peerSd, (sockaddr*) &peerSockAddr, sizeof(peerSockAddr));
        if (connectStatus < 0) {
            cerr << "Failed to connect to the peer server" << endl;
            return false;
        }
        cout << "Connected!" << endl;
        return true;
    }

    // Set peer's IP
    void setp2pIP(char* IP) {
        p2pIp = IP;
    }

    // Send messages in game
    void sendMessage(char* msg) {
        if (write(peerSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Sending message unsuccessful" << std::endl;
        }
    }

    // Recieve messages in game
    void recvMessage() {
        char msg[1024];
        if (read(peerSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Receiving message unsuccessful" << std::endl;
        }
        cout << msg << endl;
    }
};