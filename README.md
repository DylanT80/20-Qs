# 20-Qs
A text-based game based from 20 Questions implemented with networking concepts learned in class.

## How to play / Networking protocol ##
Initial template of protocol and visuals

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

Create a lobby by entering a name / Enter lobby name to join / Q (quit):
> 
```
### Create a Lobby ###
Create a lobby by entering a string name.
```
Available Lobbies:
JOIN NOW
feeling lonely
...

Create a lobby by entering a name / Enter lobby name to join / Q (quit):
> 1v1 me
```
### Join a Game ###
Join a lobby by inputting the name of it.
```
Available Lobbies:
JOIN NOW
feeling lonely
...

Create a lobby by entering a name / Enter lobby name to join / Q (quit):
> JOIN NOW
```
### During a Game ###
```
Is it pink?

Send response to other player (Y = they got it!):
> Y
```
### Unregister ###
Unregister by pressing Q at the lobby menu
```
Available Lobbies:
JOIN NOW
feeling lonely
...

Create a lobby by entering a name / Enter lobby number to join / Q (quit):
> Q
```

### A couple notes ###
Used 127.0.0.1 for local play only. Remote play doesn't seem to be working, and therefore the commented out code in the game.cpp is present. Instead it breaks ending the whole process.
