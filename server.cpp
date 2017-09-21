#include "server.h"
//#include "player.h"

std::vector<player> makePlayers(int& amount_players) noexcept
{
    std::vector<player> newPlayer;
    for(int i=0; i!= amount_players; ++i)
    {
        newPlayer.push_back(player(30,30,0,0));
    }
    return newPlayer;
}

void playerWalking(const bool update ,std::vector<player> &players, float &posXplayer1,
float &posYplayer1)
{
    if(update)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
                posXplayer1+=3;
                players[0].setPlayerPosition(posXplayer1, posYplayer1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
                posXplayer1-=3;
                players[0].setPlayerPosition(posXplayer1, posYplayer1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            posYplayer1-=3;
            players[0].setPlayerPosition(posXplayer1, posYplayer1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
            || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            posYplayer1+=3;
            players[0].setPlayerPosition(posXplayer1, posYplayer1);
        }
    }
}
void do_server(std::string &text,bool &initializing,std::vector<player> &players)
{
    float posXplayer1;
    float posYplayer1;
    sf::TcpListener listener;
    listener.setBlocking(false);
    unsigned short port;
    unsigned short clientPort;
    sf::TcpSocket TcpSocket;
    sf::TcpSocket tcpSocketClient1;
    sf::TcpSocket tcpSocketClient2;
    sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    std::string cIP=clientIP.sf::IpAddress::toString();
    sf::IpAddress clientIpReceived;
    sf::Packet cIPandPortPacket;
    sf::UdpSocket socket;
    int currentAmountOfPlayers=2;
    sf::Uint16 playerNumber=1;
    std::vector<unsigned short> vectorClientPorts;
    sf::Packet playerAssignNumber;
    port =2000;

    if(initializing==true)
    {
         posXplayer1=100;
         posYplayer1=110;
         initializing=false;
    }
    socket.bind(port);
    text += "server";
    listener.listen(port);

    sf::Vector2f prevPosition;
    sf::Vector2f changingPosition;

    TcpSocket.setBlocking(false);
    tcpSocketClient1.setBlocking(false);
    tcpSocketClient2.setBlocking(false);
    socket.setBlocking(false);
    bool  update =true;
    playerWalking(update, players, posXplayer1, posYplayer1);
    listener.accept(TcpSocket);

        if(TcpSocket.receive(cIPandPortPacket)==sf::Socket::Done)
        {
            std::cout<<"message received from client";
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
                std::cout<<"message send to: ";
                std::cout<<clientPort <<"\n";
                tcpSocketClient1.connect(cIP,clientPort);
                tcpSocketClient1.send(playerAssignNumber);
            }
        }
        prevPosition = sf::Vector2f(players[0].getPosX(), players[0].getPosY());

        sf::Packet posPacket;
        if(prevPosition != sf::Vector2f(players[0].getPosX(), players[0].getPosY()))
        {
            sf::IpAddress recipient = clientIpReceived;
            //unsigned short clientPort = port;
            posPacket<<players[0].getPosX() <<players[0].getPosY();
            for(unsigned i=0; i<vectorClientPorts.size(); ++i)
            {
                if (socket.send(posPacket, recipient, vectorClientPorts[i]) != sf::Socket::Done)
                {
                    //std::cout<<"whoops... some data wasn't sent";
                }
            }
        }
        sf::IpAddress sender;
        //unsigned short port;
        if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
        {
            //std::cout<<"whoops... some data wasn't received";
        }
        if(posPacket>>changingPosition.x>>changingPosition.y)//>>playerNumber)
        {
            players[1].setPlayerPosition(changingPosition.x, changingPosition.y);
        }
}
    /* if(playerNumber==3)
     {
         std::cout<<"message send to: ";
         std::cout<<clientPort <<"\n";
         tcpSocketClient2.connect(cIP,2002);
         tcpSocketClient2.send(playerAssignNumber);
     }*/



