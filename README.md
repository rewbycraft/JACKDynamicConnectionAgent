#JACK Dynamic Connection Agent

The purpose of this program is to dynamically match JACK clients and remap their ports as required.
It does this based on regex rules. (See config.yml for an example with vlc)
This program is primarily for things like vlc which do not allow you to specify what ports to link to what (VLC is forgiving in that it lets you say what program, but it sadly doesn't let you say what ports to connect to).

#Quickstart
First off, you're gonna wanna make sure to install the dependencies. This is specific to your distro and I can not help you with this.
Then you will wanna compile this program, do this by running the following commands in the root of the cloned repository:
````
mkdir build
cd build
cmake ..
make
sudo make install
````
This will have installed the program as jackdynconagent under /usr/local.
Now you'll wanna copy the example config file to, for example, your home directory.
You can then run the program with:
````
jackdynconagent -c ~/config.yml
````
The example file contains rules based on my personal setup, so you're gonna have to go change it to match your personal setup.
If you are having trouble with any of these steps, feel free to ask for support. (see contact section below)

On Arch Linux it is even easier to install and use the JACK Dynamic Connection Agent!
Performing the following commands will get you set-up almost immediately, you don't even have to clone the repo (you will, however, have to run these commands as root)!
These commands will install all dependencies and work everything out for you. (REMINDER: THIS IS ONLY FOR ARCH LINUX)
````
mkdir /tmp/build
cd /tmp/build
pacman -S base-devel wget
wget https://raw.githubusercontent.com/rewbycraft/JACKDynamicConnectionAgent/master/PKGBUILD
makepkg -si
````
And done!


##Dependencies:
- JACK
- Yaml-C++
- LibProcPS (most linux distros include this, as it is what ps and such use)
- The Lean Mean C++ Option Parser (included in repository)
- EasyLogging++ (automatically downloaded into repository via git subtree)
- LibFindMacros (automatically downloaded into repository via git subtree)

Important note: Even though "The Lean Mean C++ Option Parser", EasyLogging++ and LibFindMacros are automatically imported into this repository, they are not my property. They each have their own licence and owners. I merely include them as to make life easier on whomever decided to clone this repository. All credit for these dependencies goes to their respective (and respectable) owners.

##Contributors:
- rewbycraft (Project Owner)

##TODO:
- Allow the regex pid parser to parse more than one pid per regex.

##Contact
You can find my contact information at [http://roelf.org/contact](http://roelf.org/contact)
