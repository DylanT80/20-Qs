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
    int server_port;
    sockaddr_in acceptSockAddr;
    int serverSd;
    // Accept the connection as a new socket
    sockaddr_in newsock;   // place to store parameters for the new connection
    socklen_t newsockSize = sizeof(newsock);

    char* p2pIp;
    sockaddr_in peerSockAddr;
    int peerSd;

    string answer;
    int questions;
    bool host;

public:
    GameServer() {
        server_port = 987;
        questions = 0;
        host = false;
        
        bzero(&acceptSockAddr, sizeof(acceptSockAddr));     // zero out the data structure
        acceptSockAddr.sin_family = AF_INET;                        // using IP
        acceptSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);         // listen on any address this computer has
        acceptSockAddr.sin_port = htons(server_port);               // set the port to listen on
    }
    
    ~GameServer() {
        close(serverSd);
        close(peerSd);
    }

    void waitInLobby() {
        serverSd = socket(AF_INET, SOCK_STREAM, 0);             // creates a new socket for IP using TCP

        // Bind the socket
        bind(serverSd, (sockaddr*) &acceptSockAddr, sizeof(acceptSockAddr));  // bind the socket using the parameters we set earlier
        
        // Listen on the socket
        int n = 10;
        listen(serverSd, n);  // listen on the socket and allow up to n connections to wait.

        host = true;
        cout << "Waiting for other player..." << endl;
        
        peerSd = accept(serverSd, (sockaddr *)&newsock, &newsockSize);  // grabs the new connection and assigns it a temporary socket
        if (peerSd < 0) {
            cerr << "Starting lobby unsuccessful" << endl;
        }
        cout << "Player found!" << endl;
        cout << "\n=================\n" << endl;

        cout << "Enter subject/idea for an answer: ";
        string answer;
        getline(cin, answer);
        this->answer = answer;

        char msg[2048];
        bzero(&msg, sizeof(msg));
        for (int i = 0; i < answer.size(); i++) {
            msg[i] = answer[i];
        }

        if (write(peerSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Sending answer unsuccessful" << std::endl;
        }
    }

    // Connect to peer's lobby
    bool connectToLobby() {
        cout << "Joining Lobby..." << endl;

        bzero(&peerSockAddr, sizeof(peerSockAddr));
        cout << p2pIp << endl;

        peerSockAddr.sin_family = AF_INET;  
        peerSockAddr.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)* gethostbyname(p2pIp)->h_addr_list));
        peerSockAddr.sin_port = htons(server_port);

        peerSd = socket(AF_INET, SOCK_STREAM, 0);

        int connectStatus = connect(peerSd, (sockaddr*) &peerSockAddr, sizeof(peerSockAddr));
        if (connectStatus < 0) {
            cerr << "Failed to connect to the peer server" << endl;
            return false;
        }
        cout << "Connected!" << endl;
        cout << "\n=================\n" << endl;
        
        // Get answer
        cout << "Host is choosing answer..." << endl;

        char msg[2048];
        if (read(peerSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Receiving answer unsuccessful" << std::endl;
        }
        answer = string(msg);

        return true;
    }

    // Set peer's IP
    void setp2pIP(char* IP) {
        p2pIp = IP;
    }

    // Send messages in game
    bool sendMessage() {
        // Send and receive messages
        if (host) {
            cout << "Send response to other player (Y = they got it!): ";
        }
        else {
            questions++;
            cout << "Send question: ";
        }
        string input;
        getline(cin, input);
        char msg[2048];
        bzero(&msg, sizeof(msg));
        for (int i = 0; i < input.size(); i++) {
            msg[i] = input[i];
        }

        if (write(peerSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Sending message unsuccessful" << std::endl;
        }
    
        if (input == "Y" || !host && input == answer) {
            cout << "\n--== Answer found! ==--\n" << endl;
            return true;
        }
        return false;
    }

    // Recieve messages in game
    bool recvMessage() {
        if (host) {
            questions++;
            cout << "Waiting for question..." << endl;
        }
        else {
            cout << "Waiting for response..." << endl;
        }
        char msg[2048];
        if (read(peerSd, msg, sizeof(msg)) < 0) {
            std::cerr << "Receiving message unsuccessful" << std::endl;
        }

        cout << msg << endl;
        if (host && string(msg) == answer || !host && string(msg) == "Y") {
            cout << "\n--== Answer found! ==--\n";
            return true;
        }
        return false;
    }

    bool isHost() {
        return host;
    }

    int getNumQuestions() {
        return questions;
    }

    string getAnswer() {
        return answer;
    }
};