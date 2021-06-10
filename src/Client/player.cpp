/**
 * SECTION
 * The Client class, it is responsible for:
 *  - Launch a new client
 *  - Interact with server
 *  - Challenge another client
 *  - Play a four-in-a-row game
 *  - Init a board
 */
#include "../../include/Client/slave.hpp"

void
InitPeerReceiver(
    int peerfd,
    std::string peeraddr,
    std::string peerport,
	int domain, 
    int socktype, 
    int protocol, 
    int family,
	int level,
	int optname,
    int optval,
    int backlog_queue, 
    struct sockaddr_in _peersock
)
{
	peerfd = InitSocket(domain, socktype, protocol);
	SetSockOpt(peerfd, level, optname, &optval);
	SockBind(peerfd, peeraddr, peerport, family, _peersock);
	// TOCHECK: remove this print?
	std::cout << "READY! Waiting for a client to connect..." << std::endl;
	SockListen(peerfd, backlog_queue);
	int _acceptfd = SockAccept(peerfd, _peersock);
	// TOCHECK could be useful to have getnameinfo here to keep track of received messages ?
	std::cout << "Connected with peer!" << std::endl;	
}

int main()
{
    short int ret{-1};
    bool _check{false};
    unsigned char sbuf[] = "Hello Server!";
    unsigned char rbuf[14];
    struct sockaddr_in _peersock;
    Slave client = Slave();
    
    // Create socket, get server info and connect
    ret = client.InitClient(AF_INET, SOCK_STREAM, 0, AF_INET);
    if(ret < 0)
    {
        std::cerr << "client->InitClient() failed!\nReturn code: " << ret << std::endl;
        exit(1);
    }

    _check = SockSend(client.GetClientfd(), sbuf, sizeof(sbuf));
    if(_check)
        std::cout << "HELLO SENT!" << std::endl;

    _check = SockReceive(client.GetClientfd(), rbuf, sizeof(rbuf));
    if(_check)
        std::cout << "RECEIVED: " << rbuf << std::endl;

    // Manage peer-to-peer connection
    // TOCHECK
    std::string assign;
    std::cout << "Please, type s (for sender) r (for receiver): ";
    getline(std::cin, assign);
    if (!std::cin)
    {
        std::cerr << "Error during input\n";
        exit(1);
    }

    if(assign == "r")
    {   
        std::thread t1(&Slave::InitPeerReceiver, &client, AF_INET, SOCK_STREAM, 0, AF_INET, SOL_SOCKET, SO_REUSEADDR, 1, BACKLOG_QUEUE, _peersock);
        t1.join();
    }
    else if (assign == "s")
    {
        std::thread t2(&Slave::InitPeerSender, &client, AF_INET, SOCK_STREAM, 0, AF_INET, _peersock);
        t2.join();
    }
    else
        std::cout << "Invalid value!" << std::endl;

    return 1;
}