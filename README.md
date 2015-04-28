#JACK Dynamic Connection Agent

The purpose of this program is to dynamically match JACK clients and remap their ports as required.
It does this based on regex rules. (See config.yml for an example with vlc)
This program is primarily for things like vlc which do not allow you to specify what ports to link to what (VLC is forgiving in that it lets you say what program, but it sadly doesn't let you say what ports to connect to).

##Dependencies:
- JACK
- Yaml-C++
- LibProcPS (most linux distros include this, as it is what ps and such use)
- The Lean Mean C++ Option Parser (included in repository)
- EasyLogging++ (automatically downloaded into repository via git subtree)

##Contributors:
- rewbycraft (Project Owner)

##TODO:
- Add a way to inject the pid of a program into a replace rule. (This is to allow one to map programs that don't support JACK natively, but instead use the alsa-jack plugin.)
- Add a way to rename clients (if possible).

