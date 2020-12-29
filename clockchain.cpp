/**
 * CSE 411
 * Fall 2019
 * Project #3
 *
 * The purpose of this assignment is to get you thinking about what it means for
 * something to happen "before" something else.  We will explore this notion of
 * "before" using a variation on BlockChain technologies.
 *
 * The main idea in a BlockChain is that the provenance of a piece of
 * information becomes increasingly difficult to forge over time.  That is,
 * every time data (a "block") is added to the chain, that data reinforces the
 * fact that the data that precedes it in the chain is, indeed, present and
 * immutable.
 *
 * In this project, we will make "clockchain".  The idea behind "clockchain" is
 * that a server (the "clockchain server") will periodically insert timestamps
 * into the chain.  A user can also add blocks of data to the chain.  If we
 * trust that the timestamps are, indeed, inserted at the time they claim to
 * have been inserted, then we can look at a block of data, and the next clock,
 * and conclude that the block of data had to have been inserted before the
 * clock's time.
 *
 * Clockchain could be a useful idea for an inventor: they could use it to prove
 * that they had an idea before someone else, by showing that their idea is in the
 * clockchain before someone else's idea.  In that case, one might only insert a
 * hash of the idea into the clockchain (because the idea might need to be kept
 * secret), but that's fine... the inventor could still prove that they had the
 * idea by presenting the idea and showing that it generates the hash value that
 * is in the chain.
 *
 * There are several steps to this project.
 *
 * - First, you should read this reference:
 *   https://davenash.com/2017/10/build-a-blockchain-with-c/.  Then you should
 *   implement it.
 *
 * - Second, you should extend your implementation so that it is possible to
 *   display blocks from the blockchain.
 *
 * - Third, you should transform the program so that it is an always-running
 *   server, and so that it can receive connections on a port (specified by the
 *   -p command-line option), read data over those connections, and then put
 *   that data into the chain.  Note that you will need to design a reasonable
 *   protocol for how to send data,
 *   so that it includes the sender ID,
 *   the content, etc.
 *
 * - Fourth, you should ensure that every block gets correctly persisted to a
 *   file, so that the program can be stopped and started without losing data.
 *   You will need an intelligent file format for this, since blocks have
 *   variable length.
 *
 * -  Fifth, you should add a timer to your server, so that a new block is added
 *   every minute.  This will provide the proof of "before" for entries.
 *
 * This assignment is intentionally under-specified.  You should ask questions
 * on Piazza to get more information about exactly what I expect of your
 * program.
 *
 * Your program should use advanced features of C++ whenever appropriate.  It
 * should compile with g++ version 7.  You should organize your code into files
 * in a way that is appropriate.  You should comment your code.
 *
 * Turn-in will be via CourseSite.
 */

#include <iostream>
#include <libgen.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <netdb.h>
#include <string>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <signal.h>
//#define PORT 8080
#include "Blockchain.h"


int server_fd = 0;
std::atomic<int> i(0);
Blockchain bChain;


/** Print some helpful usage information */
void usage(const char *progname) {
    using std::cout;
    cout << "ClockChain Server\n";
    cout << "  Usage: " << progname << " [options]\n";
    cout << "    -p <int> : Port on which to listen (default 41100)\n";
    cout << "    -h       : print this message\n";
}

void sys_error(std::size_t err, const char *prefix) {
    char buf[1024];
    std::cerr << prefix << " " << strerror_r(err, buf, sizeof(buf)) << std::endl;
}

//Construct a socket to receive client requests (from a seperate thread)
void create_socket(int port) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char const *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Forcefully attaching socket to the port 8080
    if (::bind(server_fd, (struct sockaddr *) &address,
               sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                             (socklen_t * ) & addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    bChain.AddBlock(Block(i, buffer),1);
    i++;
    send(new_socket, hello, strlen(hello), 0);
    printf("Message sent\n");
}

//Close the socket for good measure
void close_socket(sig_atomic_t s){
    std::cout << std::endl << "Closing Socket..." << std::endl;
    close(server_fd);
    std::cout << "Socket Closed" << std::endl;
    if (bChain.writing) {
        sleep(1);
    }
    bChain.file->close();
    exit(1);
}

int main(int argc, char *argv[]) {
    // Config vars that we get via getopt
    bool show_help = false; // show usage?
    bool display = false;
    int port = 41100;       // random seed

    // Parse the command line options:
    int o;
    while ((o = getopt(argc, argv, "p:hd")) != -1) {
        switch (o) {
            case 'h':
                show_help = true;
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
                display = true;
                break;
            default:
                show_help = true;
                break;
        }
    }

    // Print help and exit
    if (show_help) {
        usage(basename(argv[0]));
        exit(0);
    }


    // Print the configuration... this makes results of scripted experiments
    // much easier to parse
    std::cout << "p = " << port << std::endl;


    // Time for you to start writing code :)
    bChain = Blockchain();

    if (display) {
        bChain.DisplayBlocks();
        exit(0);
    }

    std::thread socket_thread(create_socket, port);
    //Handle Ctrl C to close socket
    signal(SIGINT,close_socket);

    i = bChain._vChain.size();
    while (true) {
        std::string data = "Block " + to_string(i) + " Data";
        bChain.AddBlock(Block(i, data),0);
        i++;
    }
}