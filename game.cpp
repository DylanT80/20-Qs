#include "client.cpp"
#include "gameServer.cpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

enum GameState { inLobby, waitingLobby, joinLobby, inGame, gameDone };
GameServer* gameServer;

int main() {
    cout << "----======== Welcome to 20 Q's ========----" << endl;
    Client cl;

    // Register the user
    cout << "Enter a username: ";
    char name[16];
    cin >> name;
    cout << "Welcome " << name << "!" << endl;
    cin.clear();
    cin.ignore(10000, '\n');        

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
            cout << "Create a lobby by entering a name / Enter lobby name to join that lobby / Q (quit):" << endl;
            string input;
            getline(cin, input);

            if (input == "L") {         // Not in game if refresh lobby
                continue;
            }
            else if (input == "Q") {    // Unregister
                quit = true;
                continue;
            }

            char lobbyName[64];
            bzero(&lobbyName, sizeof(lobbyName));
            for (int i = 0; i < input.size(); i++) {
                lobbyName[i] = input[i];
            }
            
            // Determine if in game or not
            gameServer = new GameServer();
            bool joined = cl.joinLobby(lobbyName);
            if (joined) {
                gameServer->setp2pIP(cl.getPeerIP());    // Set peer IP
                gs = joinLobby;
            }
            else {
                gs = waitingLobby;
            }
        }

        // Waiting in lobby
        else if (gs == waitingLobby) {
            gameServer->waitInLobby();
            gs = inGame;
        }

        // Joining a lobby
        else if (gs == joinLobby) {
            bool connected = gameServer->connectToLobby();
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
            if (gameServer->getNumQuestions() == 20) {
                cout << "\n--== 20 Questions asked! ==--\n";
                gs = gameDone;
            }
            else if (gameServer->isHost()) {
                if (gameServer->recvMessage() || gameServer->sendMessage()) {
                    gs = gameDone;
                }
            }
            else {
                if (gameServer->sendMessage() || gameServer->recvMessage()) {
                    gs = gameDone;
                }
            }
        }

        else if (gs == gameDone) {
            cout << "The answer was...: " << gameServer->getAnswer() << "!\n\n\n";
            cout << "game done :)\nDisconnecting...\n";
            delete gameServer;
            // cl.reconnect();
            // gs = inLobby;
            break;
        }
    }
}
