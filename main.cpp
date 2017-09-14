#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <map>
#include <conio.h>
//#include <Packet.hpp>
#include "player.h"
#include "server.h"


void do_client(sf::TcpListener &listenerForClients)
{
    int currentAmountOfPlayers=2;
    sf::TcpSocket TcpSocket;
    sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    std::string cIP=clientIP.sf::IpAddress::toString();
    sf::IpAddress severIP=sf::IpAddress::getLocalAddress();
    sf::UdpSocket socket;

    sf::Packet playerAssignNumber;
    sf::Uint16 playerNumberClient=0;

    sf::Packet cIPandPortPacket;

    sf::TcpSocket tcpSocketClient1;
    sf::TcpSocket tcpSocketClient2;

    unsigned short port;
    unsigned short clientPort;

    std::string sIP="192.168.10.158"; //IP hardcoded, needs to change
    //std::cout << "Enter server ip: ";
    //std::cin>> sIP;
    severIP=sIP;
    std::cout << "set port number: ";
    std::cin >> clientPort;

    socket.bind(clientPort);
    //listenerForClient1.listen(clientPort);
    listenerForClients.listen(clientPort);
    TcpSocket.connect(severIP,clientPort);
    cIPandPortPacket<<cIP<<clientPort;
    TcpSocket.send(cIPandPortPacket);

    sf::Vector2f prevPosition;
    sf::Vector2f changingPosition;
    sf::RenderWindow window(
       sf::VideoMode(500, 500),
       "multiplayer",
       sf::Style::Titlebar | sf::Style::Close);
    std::vector<player> player{add_new_player(currentAmountOfPlayers)};

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);
    TcpSocket.setBlocking(false);
    tcpSocketClient1.setBlocking(false);
    tcpSocketClient2.setBlocking(false);
    socket.setBlocking(false);
    bool  update =false;
    while(window.isOpen())
    {
        sf::Event Event;
        while(window.pollEvent(Event))
        {
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
        }

        prevPosition = sf::Vector2f(player[0].getPosX(), player[0].getPosY());

        playerWalking(update, player);

        sf::Packet posPacket;

            listenerForClients.accept(tcpSocketClient1);
            if(tcpSocketClient1.receive(playerAssignNumber)==sf::Socket::Done)
            {
                std::cout<<"tcpSocketClient1 received a message!  \n";
                if(playerAssignNumber>>playerNumberClient)
                {
                    std::cout<<playerNumberClient<< "\n";
                }
            }
            listenerForClients.accept(tcpSocketClient2);
            if(tcpSocketClient2.receive(playerAssignNumber)==sf::Socket::Done)
            {
                std::cout<<"tcpSocketClient2 received a message! \n";
                if(playerAssignNumber>>playerNumberClient)
                {
                    std::cout<<playerNumberClient<< "\n";
                }
            }

            if(prevPosition != sf::Vector2f(player[0].getPosX(), player[0].getPosY()))
            {
                sf::IpAddress recipient = severIP;
                unsigned short serverPort = 2000;
                posPacket<<player[0].getPosX() <<player[0].getPosY();//<<playerNumber;
                if (socket.send(posPacket, recipient, serverPort) != sf::Socket::Done)
                {
                    //std::cout<<"whoops... some data wasn't sent";
                }

            }

        sf::IpAddress sender;
        unsigned short port;
        if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
        {
            //std::cout<<"whoops... some data wasn't received";
        }
        if(posPacket>>changingPosition.x>>changingPosition.y)//>>playerNumber)
        {
            player[1].setPlayerPosition(changingPosition.x, changingPosition.y);
        }
        window.clear();
        for(int i=0; i<static_cast<int>(player.size()); ++i)
        player[i].display(window);
        window.display();
    }
}


int main()
{
    /*
    sf::TcpSocket TcpSocket;
    sf::TcpSocket tcpSocketClient1;
    sf::TcpSocket tcpSocketClient2;
    sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    sf::IpAddress severIP=sf::IpAddress::getLocalAddress();
    std::string cIP=clientIP.sf::IpAddress::toString();
    sf::IpAddress clientIpReceived;
    sf::Packet cIPandPortPacket;
    bool server =false;
    bool client =false;
    sf::UdpSocket socket;
    std::string connectionType ="";
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";
    std::string clientCheckShort ="c";
    std::string clientCheck ="client";
    float posXplayer1=0;
    float posYplayer1=0;
    int currentAmountOfPlayers=2;
    sf::Uint16 playerNumber=1;
    std::vector<unsigned short> vectorClientPorts;
    std::vector<int> vectorPlayers;
    sf::Packet playerAssignNumber;
    sf::Uint16 playerNumberClient=0;
    */
    std::string connectionType ="";
    std::string serverCheck ="server";
    std::string serverCheckShort ="s";

    sf::TcpListener listener;
    listener.setBlocking(false);

    sf::TcpListener listenerForClients;
    listenerForClients.setBlocking(false);

    sf::TcpListener listenerForClient1;
    listenerForClient1.setBlocking(false);

    std::string text = "Connected to: ";

    std::cout <<"Enter (server) for server, Enter (client) for client: \n";
    std::cin >> connectionType;

    if(connectionType==serverCheckShort || connectionType==serverCheck)
    {
      do_server(text);
    }
    else
    {
       do_client(listenerForClients);
    }
    //LOS!
    /*
    unsigned short port;
    unsigned short clientPort;

    if(connectionType==serverCheckShort || connectionType==serverCheck)
    {
        port =2000;
        socket.bind(port);
        server=true;
        text += "server";
        listener.listen(port);
        //playerNumber=1;
        vectorPlayers.push_back(playerNumber);
    }
    if(connectionType==clientCheckShort || connectionType==clientCheck)
    {
        client=true;
        std::string sIP="192.168.10.158"; //IP hardcoded, needs to change
        //std::cout << "Enter server ip: ";
        //std::cin>> sIP;
        severIP=sIP;
        std::cout << "set port number: ";
        std::cin >> clientPort;

        socket.bind(clientPort);
        //listenerForClient1.listen(clientPort);
        listenerForClients.listen(clientPort);
        TcpSocket.connect(severIP,2000);
        cIPandPortPacket<<cIP<<clientPort;
        TcpSocket.send(cIPandPortPacket);
    }

    sf::Vector2f prevPosition;
    sf::Vector2f changingPosition;
    sf::RenderWindow window(
       sf::VideoMode(500, 500),
       "multiplayer",
       sf::Style::Titlebar | sf::Style::Close);
    std::vector<player> player{add_new_player(currentAmountOfPlayers)};

    window.setFramerateLimit(60);
    window.setKeyRepeatEnabled(true);
    TcpSocket.setBlocking(false);
    tcpSocketClient1.setBlocking(false);
    tcpSocketClient2.setBlocking(false);
    socket.setBlocking(false);
    bool  update =false;
    while(window.isOpen())
    {
        sf::Event Event;
        while(window.pollEvent(Event))
        {
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
        }

        //Receive messages from clients, then send message to clients which player number they have
        if(connectionType==serverCheckShort || connectionType==serverCheck)
        {

            listener.accept(TcpSocket);
            if(TcpSocket.receive(cIPandPortPacket)==sf::Socket::Done)
            {
                if(cIPandPortPacket>>cIP>>clientPort)
                {
                    currentAmountOfPlayers+=1;
                    std::cout<<cIP<<"\n";
                    clientIpReceived=cIP;
                    std::cout<<clientPort<<"\n";
                    vectorClientPorts.push_back(clientPort);
                    playerNumber+=1;
                    std::cout<<playerNumber<< "\n";
                    playerAssignNumber<<playerNumber;
                    //if(playerNumber==2)
                    //{
                        std::cout<<"message send to: ";
                        std::cout<<clientPort <<"\n";
                        tcpSocketClient1.connect(cIP,clientPort);
                        tcpSocketClient1.send(playerAssignNumber);
                    //}
                    if(playerNumber==3)
                    {
                        std::cout<<"message send to: ";
                        std::cout<<clientPort <<"\n";
                        tcpSocketClient2.connect(cIP,2002);
                        tcpSocketClient2.send(playerAssignNumber);
                    }
                }
            }
        }


        prevPosition = sf::Vector2f(player[0].getPosX(), player[0].getPosY());
        if(update)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                    posXplayer1+=3;
                    player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                    posXplayer1-=3;
                    player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                posYplayer1-=3;
                player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                posYplayer1+=3;
                player[0].setPlayerPosition(posXplayer1, posYplayer1);
            }
        }

        sf::Packet posPacket;
        if(server==true)
        {
            if(prevPosition != sf::Vector2f(player[0].getPosX(), player[0].getPosY()))
            {
                sf::IpAddress recipient = clientIpReceived;
                //unsigned short clientPort = port;
                posPacket<<player[0].getPosX() <<player[0].getPosY();
                for(unsigned i=0; i<vectorClientPorts.size(); ++i)
                {
                    if (socket.send(posPacket, recipient, vectorClientPorts[i]) != sf::Socket::Done)
                    {
                        //std::cout<<"whoops... some data wasn't sent";
                    }
                }

            }
        }
        if(client==true)
        {
            listenerForClients.accept(tcpSocketClient1);
            if(tcpSocketClient1.receive(playerAssignNumber)==sf::Socket::Done)
            {
                std::cout<<"tcpSocketClient1 received a message!  \n";
                if(playerAssignNumber>>playerNumberClient)
                {
                    std::cout<<playerNumberClient<< "\n";
                }
            }
            listenerForClients.accept(tcpSocketClient2);
            if(tcpSocketClient2.receive(playerAssignNumber)==sf::Socket::Done)
            {
                std::cout<<"tcpSocketClient2 received a message! \n";
                if(playerAssignNumber>>playerNumberClient)
                {
                    std::cout<<playerNumberClient<< "\n";
                }
            }

            if(prevPosition != sf::Vector2f(player[0].getPosX(), player[0].getPosY()))
            {
                sf::IpAddress recipient = severIP;
                unsigned short serverPort = 2000;
                posPacket<<player[0].getPosX() <<player[0].getPosY();//<<playerNumber;
                if (socket.send(posPacket, recipient, serverPort) != sf::Socket::Done)
                {
                    //std::cout<<"whoops... some data wasn't sent";
                }

            }

        }
        sf::IpAddress sender;
        unsigned short port;
        if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
        {
            //std::cout<<"whoops... some data wasn't received";
        }
        if(posPacket>>changingPosition.x>>changingPosition.y)//>>playerNumber)
        {
            player[1].setPlayerPosition(changingPosition.x, changingPosition.y);
        }
        window.clear();
        for(int i=0; i<static_cast<int>(player.size()); ++i)
        player[i].display(window);
        window.display();
    }
    */
    return 0;
}
