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
#include "gameServer.cpp"

using namespace std;

// Convert int* to ints
unordered_map<int, string> users;
unordered_map<string, int> lobbies;        // Lobby name : SD of host 
unordered_map<int, sockaddr_in> socks;     // Host SD : Host's sock information

void *handleUserInLobby(void *ptrSd);
void showLobbies(void *ptrSd);

int main() {
    // Create the socket
    int server_port = 1885;

    sockaddr_in acceptSockAddr;
    bzero((char*) &acceptSockAddr, sizeof(acceptSockAddr));     // zero out the data structure
    acceptSockAddr.sin_family = AF_INET;                        // using IP
    acceptSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);         // listen on any address this computer has
    acceptSockAddr.sin_port = htons(server_port);               // set the port to listen on

    int serverSd = socket(AF_INET, SOCK_STREAM, 0);             // creates a new socket for IP using TCP

    // Bind the socket
    bind(serverSd, (sockaddr*) &acceptSockAddr, sizeof(acceptSockAddr));  // bind the socket using the parameters we set earlier
    
    // Listen on the socket
    int n = 10;
    listen(serverSd, n);  // listen on the socket and allow up to n connections to wait.
 
    while (1) {
        // Accept the connection as a new socket
        sockaddr_in newsock;   // place to store parameters for the new connection
        socklen_t newsockSize = sizeof(newsock);

	    int newSd = accept(serverSd, (sockaddr *)&newsock, &newsockSize);  // grabs the new connection and assigns it a temporary socket
        if (newSd < 0) {
            cerr << "Accept unsuccessful" << endl;
        }
        socks[newSd] = newsock;

        // Read username from the socket
        char name[8];   
	    if (read(newSd, name, 8) < 0) {
            cerr << "Reading failed" << endl;
        }

        // Register user
        users[newSd] = name;
	    cout << "User: " << name << " has registered" << endl;
        for (auto item : users) {
            cout << item.first << " | " << item.second << endl;
        }

        // Create a new thread
        pthread_t thread;
        // Execute thread
        pthread_create(&thread, NULL, handleUserInLobby, (void *)&newSd);
    }
    return 0;
}

void *handleUserInLobby(void *ptrSd) {
    int sd = *(int *) ptrSd; // socket descriptor
    char msg[16];

    // While user is in lobby menu
    bool inLobby = true;
    while (1) {
        if (inLobby) {
            read(sd, msg, sizeof(msg));
            string message = string(msg);
            // Show lobbies
            if (message == "L") {
                showLobbies(ptrSd);
            }
            // Unregister/Log off
            else if (message == "Q") {
                cout << users[sd] << " unregistered" << endl;
                users.erase(sd);
                socks.erase(sd);
                close(sd);
                break;
            }

            // Existing lobby
            else if (lobbies.count(message)) {
                int hostSd = lobbies[message];
                cout << users[sd] << " joined " << users[hostSd] << "'s lobby: " << message << endl;

                // Remove lobby from list
                lobbies.erase(message);

                // Response of joining an exisitng lobby
                char res[2] = "J";
                write(sd, res, sizeof(res));
                inLobby = false;

                // Send the host's IP to peer
                sockaddr_in hostSockAddr = socks[hostSd];
                char* hostIP = inet_ntoa(hostSockAddr.sin_addr);
                cout << hostSockAddr.sin_addr.s_addr << endl;
                cout << "Host IP being sent: " << hostIP << endl;
                write(sd, hostIP, INET_ADDRSTRLEN);
            }

            // Creating a lobby
            else {
                cout << users[sd] << " created lobby: " << message << endl;
                lobbies[message] = sd;
                for (auto item : lobbies) {
                    cout << item.first << " | " << item.second << endl;
                }

                // Response of waiting in lobby
                char res[2] = "W";
                write(sd, res, sizeof(res));
                inLobby = false;
            }
        }
        // In game
        else {
            char status[2];
            read(sd, status, sizeof(status));

            // Read - done
            if (status[0] == 'D') {
                cout << users[sd] << " finished a game!" << endl;
                inLobby = true;
            }
        }
    }
}

void showLobbies(void* ptrSd) {
    string resStr = "Available Lobbies:\n";
    int lobbyNum = 1;
    for (const auto [name, sd] : lobbies) {
        resStr.append(name);
        resStr.append("\n");
    }

    char res[resStr.length() + 1];
    res[resStr.length()] = '\0';

    for (int i = 0; i < resStr.length(); i++) {
        res[i] = resStr[i];
    }
    cout << res << endl;

    int sd = *(int *)ptrSd;         // socket descriptor
    if (write(sd, res, sizeof(res)) < 0) {
        cerr << "Showing lobbies failed" << endl;
    }
}