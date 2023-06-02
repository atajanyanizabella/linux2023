Chatting System
Chatting System is a project consisting of a client application (chatting_app) and a server application (chatting_server). This project allows users to communicate with each other in real-time.
Open chatting_app and chatting_server directories for their respective source code and further details.

CONTENTS
    1. Directory structure
    2. Installation
    3. Build
    4. Usage
    5. Clean
    6. Maintainers


1. Directory Structure
    - chatting_system/
        - chatting_app/ (Contains the source code and files for the client application.)
        - chatting_server/ (Contains the source code and files for the server application.)
        - installer.sh (An installation script that installs all the required dependencies for the project.)
        - Makefile (A Makefile that provides various build and clean options.)
        - README.md (readme file)

2. Installation
    $ ./installer.sh - installs all the necessary dependencies

3. Build
    $ make - builds the project
    $ make app - builds the client-side application
    $ make server - builds the server-side application

4. Usage
    $ make app-run - runs the client-side application
        NOTE: specify server IP and port in server.conf file
    $ make server-run - runs the server-side application

5. Clean
    $ make clean - removes all object and generated source files
    $ make distclean - removes all generated files include executables

6. Maintainers
    Izabella Atajanyan
