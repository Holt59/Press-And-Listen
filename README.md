Press And Listen
================

**Press And Listen** is a tool to control web music players with keyboard shortcuts even if the windows if not focus or is in backgorund !

Web site supported :
* Youtube
* Grooveshark
* Deezer
* Spotify

The project has been tested on Windows and Linux.

How does it work?
------------------
The project is composed in two parts connected by a websocket
* A plugin for the browser
* A native application

The plugin injects a javascript code in the website to get information on the status of the player (running or not, current music playing etc). Theses data are sent by the websocket to the native app.
On the other side, when a correct shorcut is captured by the native app, a message is sent to the plugin which controls of the player to execute the action (play, stop, next, previous)

Browsers supported
------------------

* Google Chrome
* Firefox (Soon)
