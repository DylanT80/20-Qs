#include "client.cpp"
#include <iostream>
#include <stdlib.h>

using namespace std;

enum GameState { inLobby, inGame };

int main() {
    cout << "----======== Welcome to 20 Q's ========----" << endl;
    Client cl;

    // Register the user
    cout << "Enter a username: ";
    char name[8];
    cin >> name;

    cl.connectTCP();
    cl.registerUser(name);

    bool quit = false;
    GameState gs = inLobby; // Initially in lobby

    // While in game
    while (!quit) {
        // In the lobby
        if (gs == inLobby) {
            cl.showLobbies();
          
            cout << "Create a lobby by entering a name / Enter lobby number to join / Q (quit):" << endl;
            char lobbyName[32];
            cin >> lobbyName;
                
            bool joined = cl.joinLobby(lobbyName);
            if (joined) {
                gs = inGame;
            }
        }
        // In the game
        else if (gs == inGame) {
            cout << "Enter message:" << endl;
            char msg[1024];
            cin >> msg;

            cl.sendMessage(msg);
            cl.recvMessage();
        }
    }
}