#include "server.h"
//#include "player.h"

std::vector<player> add_new_player(int& amount_players) noexcept
{
    std::vector<player> newPlayer;
    for(int i=0; i!= amount_players; ++i)
    {
        newPlayer.push_back(player(30,30,0,0));
    }
    return newPlayer;
}

void playerWalking(const bool update ,std::vector<player> &players)
{
    float posXplayer1=0;
    float posYplayer1=0;
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
void do_server(std::string &text)
{
     sf::TcpListener listener;
    /*sf::TcpSocket tcpSocketClient1;
    sf::UdpSocket socket;
    sf::Packet cIPandPortPacket;
    float posXplayer1=0;
    float posYplayer1=0;
    int currentAmountOfPlayers=2;
    sf::Uint16 playerNumber=1;
    std::vector<unsigned short> vectorClientPorts;
    sf::Packet playerAssignNumber;
    */
    unsigned short port;
    unsigned short clientPort;
    sf::TcpSocket TcpSocket;
    sf::TcpSocket tcpSocketClient1;
    sf::TcpSocket tcpSocketClient2;
    sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    sf::IpAddress severIP=sf::IpAddress::getLocalAddress();
    std::string cIP=clientIP.sf::IpAddress::toString();
    sf::IpAddress clientIpReceived;
    sf::Packet cIPandPortPacket;
    bool server =false;
    sf::UdpSocket socket;
    float posXplayer1=0;
    float posYplayer1=0;
    int currentAmountOfPlayers=2;
    sf::Uint16 playerNumber=1;
    std::vector<unsigned short> vectorClientPorts;
    std::vector<int> vectorPlayers;
    sf::Packet playerAssignNumber;
    sf::Uint16 playerNumberClient=0;
    //if(connectionType==serverCheckShort || connectionType==serverCheck)
    //{
        port =2000;
        socket.bind(port);
        //server=true;
        text += "server";
        listener.listen(port);
        //playerNumber=1;
        //vectorPlayers.push_back(playerNumber);
    //}

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
        //if(connectionType==serverCheckShort || connectionType==serverCheck)
        //{

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
        //}


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


