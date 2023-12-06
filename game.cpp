#include "client.cpp"
#include "gameServer.cpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

enum GameState { inLobby, waitingLobby, joinLobby, inGame };
GameServer gameServer;

int main() {
    cout << "----======== Welcome to 20 Q's ========----" << endl;
    Client cl;

    // Register the user
    cout << "Enter a username: ";
    char name[8];
    cin >> name;
    cout << "Welcome " << name << "!" << endl;

    cl.connectToServer();
    cl.registerUser(name);

    bool quit = false;
    GameState gs = inLobby; // Initially in lobby

    // While in the game and in a certain game state
    while (!quit) {
        // In the lobby
        if (gs == inLobby) {
            // Show lobby
            cl.showLobbies();

            // Options
            cout << "Create a lobby by entering a name / Enter lobby number to join / Q (quit):" << endl;
            char lobbyName[32];
            cin >> lobbyName;
            
            if (string(lobbyName) == "L") {   // Not in game if refresh lobby
                continue;
            }

            // Determine if in game or not
            bool joined = cl.joinLobby(lobbyName);
            if (joined) {
                gameServer.setp2pIP(cl.getPeerIP());    // Set peer IP
                gs = joinLobby;
            }
            else {
                gs = waitingLobby;
            }
        }

        // Waiting in lobby
        else if (gs == waitingLobby) {
            gameServer.waitInLobby();
            gs = inGame;
        }

        // Joining a lobby
        else if (gs == joinLobby) {
            bool connected = gameServer.connectToLobby();
            if (connected) {
                gs = inGame;
            } 
            else {
                cout << "Returning back to lobbies..." << endl;
                gs = inLobby;
            }
        }

        // In the game
        else if (gs == inGame) {
            // Send and receive messages
            cout << "Enter message: ";
            char msg[1024];
            cin >> msg;

            gameServer.sendMessage(msg);
            gameServer.recvMessage();
        }
    }
}