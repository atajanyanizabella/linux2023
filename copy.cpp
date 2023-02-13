#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cerrno>
#include <fstream>
#include <string>

int main(int argc, char** argv)
{
    //checking for arguments
    if (argc < 3) {
        std::cerr << "not enough arguments" << std::endl;
        return 1;
    }
    //saving source and target files' paths
    const char* source = argv[1];
    const char* target = argv[2];
    //opening source file
    int source_fd = open(source, O_RDONLY);
    //checking for issues
    if (source_fd < 0) {
        std::cerr << "couldn't open source file" << std::endl;
    }
    //opening target file
    int target_fd = open(target, O_TRUNC | O_WRONLY, 0644);
    //checking for issues
    if (target_fd < 0) {
        //if target file doesn't exist, create it
        target_fd = open(target, O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (target_fd < 0) {
            std::cerr << "couldn't open/create target file" << std::endl;
        }
    } else {
        //if target file exists ask to overwrite it or not
        std::cout << "overwrite? yes/no" << std::endl;
        std::string answer;
        std::cin >> answer;
        //if no end the program
        if (answer == "no") {
            close(source_fd);
            close(target_fd);
            return 1;
        }
    }
    //if yes overwrite it
    char buffer[4096];

    while (true) {
        //read 4096 bytes from source file into buffer
        int count = read(source_fd, buffer, 4096);
        //if no byte was read
        if (count == 0) {
            break;
        }
        //checking for issues
        if (count < 0) {
            exit(errno);
        }
        //write up count bytes from the buffer to target file
        int write_count = write(target_fd, buffer, count);
        //checking for issues
        if (write_count < 0) {
            exit(errno);
        }
    }
    //closing opened files
    close(source_fd);
    close(target_fd);
    return 0;
}
