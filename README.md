# 20-Qs
A text-based game based from 20 Questions implemented with networking concepts learned in class.

## How to play / Networking protocol ##
Initial template of protocol and visuals

### Registration ###
Send __maximum__ of 8 ASCII bytes for your name.
```
Please enter an unique username:
> UniqueID
```
### List of Lobbies ###
A list of lobbies will appear once registered.
```
Available Lobbies:
[1] JOIN NOW - UniqueID
[2] feeling lonely - OtherID's game
[3] ...

Create a lobby by entering a name / Enter lobby number to join / Q (quit):
> 
```
### Create a Lobby ###
Create a lobby by entering a string name.
```
Available Lobbies:
[1] JOIN NOW - UniqueID
[2] feeling lonely - OtherID's game
[3] ...

Create a lobby by entering a name / Enter lobby number to join / Q (quit):
> 1v1 me
```
### Join a Game ###
Join a lobby by inputting the index of it.
```
Available Lobbies:
[1] JOIN NOW - UniqueID
[2] feeling lonely - OtherID's game
[3] ...

Create a lobby by entering a name / Enter lobby number to join / Q (quit):
> JOIN NOW
```
### Exit a Game ###
```
Is it pink?

Y/N or E (exit):
> E
```
### Unregister ###
Join a lobby by inputting the index of it.
```
Available Lobbies:
[1] JOIN NOW - UniqueID
[2] feeling lonely - OtherID's game
[3] ...

Create a lobby by entering a name / Enter lobby number to join / Q (quit):
> Q
```

## TODO's ##

| File(s)   |      What      |
|----------|:-------------:
| game.cpp, gameServer.cpp |  bug - sendMessage() called twice | 
| game.cpp, gameServer.cpp |  complete game logic | 
| server.cpp, gameServer.cpp, client.cpp |    unregsister(), handle user infos   |
