Chatting system
Chatting system is a project of a client application (messenger_app).

CONTENTS
    1. Directory structure
    2. Build
    3. Usage
    4. Clean
    5. Maintainers


1. Directory Structure
    - chatting_app/
        - src/ (Contains the source files of the client application.)
      //TODO  - resources/ (Contains the resources files of the client application.)
        - include/ (Contains the header files of the client application.)
        - Makefile (A Makefile that provides various build and clean options.)
        - README.md (readme file)

2. Build
    $ make - builds the project

3. Usage
    $ make run - runs the client-side application
        NOTE: specify server IP and port in server.conf file

4. Clean
    $ make clean - removes all object and generated source files
    $ make distclean - removes all generated files include executables

5. Maintainers
    Izabella Atajanyan
