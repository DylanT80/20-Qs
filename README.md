# 20-Qs
A text-based game based from 20 Questions implemented with networking concepts learned in class.

## How to play / Networking protocol ##
Documentation of protocol and game

### Registration ###
Send __maximum__ of 16 ASCII bytes for your name.
```
Please enter an unique username:
> UniqueID
```
### List of Lobbies ###
A list of lobbies will appear once registered.
```
Available Lobbies:
JOIN NOW
feeling lonely
...

Create a lobby by entering a name / Enter lobby name to join / L (Refresh lobbies) / Q (quit):
> 
```
### Create a Lobby ###
Create a lobby by entering a string name.
```
Available Lobbies:
JOIN NOW
feeling lonely
...

Create a lobby by entering a name / Enter lobby name to join / L (Refresh lobbies) / Q (quit):
> 1v1 me
```
### Join a Game ###
Join a lobby by inputting the name of it.
```
Available Lobbies:
JOIN NOW
feeling lonely
...

Create a lobby by entering a name / Enter lobby name to join / L (Refresh lobbies) / Q (quit):
> JOIN NOW
```

### During a Game ###
## Host ##
Start of game
```
Enter subject/idea for an answer:
> Subject
```

Waiting for response
```
Waiting for question...
Is it pink?

Send response to other player (Y = they got it!):
> Y
```

## Other player ##
Start of game
```
Host is choosing answer...
```

Sending response
```
Waiting for response...
No

Send question:
> Is it yellow?
```

## End of game ## 
```
The answer was pink!
game done
Disconnecting...
```

### Unregister ###
Unregister by pressing Q at the lobby menu
```
Available Lobbies:
JOIN NOW
feeling lonely
...

Create a lobby by entering a name / Enter lobby name to join / L (Refresh lobbies) / Q (quit):
> Q
```

### A couple notes ###
Used 127.0.0.1 for local play only. Remote play doesn't seem to be working, and therefore the commented out code in the game.cpp is present. Instead it breaks ending the whole process.

### Instructions to run ###
csslab13 is used as the main server, same machine is used to run both clients. 
There are 2 sh scripts. 1 is for client, 1 is for server. Be sure to seperate server script along with server.cpp to a different directory so that main() functions do not interfere.

### Criteria ###
1)  __Register__

You can regsiter with a username to the server. Server keeps track of the user and logs using the information given.

2)  __List Games__

A list of games is sent from the server to the client upon registration

3)  __Create Game__

Users can create games after registration by typing a string name

4)  __Join Game__

Users can join games after registration by typing the target server name

5)  __Exit Game / Unregister__

Exiting a game is performed by inputting Q at lobby menu

6)  __Application Specific protocol__

Certain strings that are sent are recognized by the protocol. For example, L is lobby refresh and Q is exiting the game / unregistering.
Other inputs are meant for other functions like creating/joining a lobby.


# Bonus Features #

7)  __Implement chat.  This would enable users to chat with each other before games and in games.__

Chat is implemented as 20 Qs would not be possible without some sort of communication :)
