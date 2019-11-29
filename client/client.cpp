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

using namespace std;

void sys_error(std::size_t err, const char *prefix) {
    char buf[1024];
    std::cerr << prefix << " " << strerror_r(err, buf, sizeof(buf)) << std::endl;
}

/** Print some helpful usage information */
void usage(const char *progname) {
    using std::cout;
    cout << "ClockChain Server\n";
    cout << "  Usage: " << progname << " [options]\n";
    cout << "    -p <int> : Port on which to send to (default 41100)\n";
    cout << "    -h       : print this message\n";
}

bool send_reliably(int sd, string msg) {
    // When we send, we need to be ready for the possibility that not all the
    // data will transmit at once
    std::cout << msg << std::endl;
    const char *next_byte = msg.c_str();
    std::cout << next_byte << std::endl;
    int remain = msg.length();
    std::cout << remain << std::endl;
    while (remain) {
        std::cout << "SD: " << sd << std::endl;
        int sent = send(sd, next_byte, remain, 0);
        // NB: Sending 0 bytes means the server closed the socket, and we should
        //     fail, so it's only EINTR that is recoverable.
        if (sent <= 0) {
            if (errno != EINTR) {
                sys_error(errno, "Error in send():");
                return false;
            }
        } else {
            next_byte += sent;
            remain -= sent;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
//    // Config vars that we get via getopt
    int port = 41100;       // random seed
//    bool show_help = false; // show usage?
////    int server_fd;
//    int o;
//
//    while ((o = getopt(argc, argv, "p:h")) != -1) {
//        switch (o) {
//            case 'h':
//                show_help = true;
//                break;
//            case 'p':
//                port = atoi(optarg);
//                break;
//            default:
//                show_help = true;
//                break;
//        }
//    }
//
//    // Print help and exit
//    if (show_help) {
//        usage(basename(argv[0]));
//        exit(0);
//    }
//
//    send_reliably(0,"Whatup");
    int sock = 3, valread;
    struct sockaddr_in serv_addr;
    char const *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}