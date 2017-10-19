#include "server.h"
//#include "player.h"
void send_position_bullet(sf::IpAddress ip, unsigned short port, sf::RectangleShape bullet)
{
    sf::UdpSocket socket;
    sf::Packet posPacket;
    posPacket<<bullet.getPosition.x << bullet.getPosition.y;
    if (socket.send(posPacket, ip, port) != sf::Socket::Done)
    {
        //std::cout<<"whoops... some data wasn't sent";
    }
}
void draw_everything(sf::RenderWindow &window, std::vector<player> &players, sf::RectangleShape bullet)
{
    window.clear();

    window.draw(bullet);

    for(int i=0; i<static_cast<int>(players.size()); ++i)
    {
       players[i].display(window);
    }

    window.display();
}

void receive_and_set_other_players_position(sf::UdpSocket &socket, std::vector<player> &players)
{
    sf::Vector2f changingPosition;
    sf::Packet posPacket;
    sf::IpAddress sender;
    unsigned short port;
    if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
    {
        //std::cout<<"whoops... some data wasn't received";
    }
    if(posPacket>>changingPosition.x>>changingPosition.y)//>>playerNumber)
    {
        players[1].setPlayerPosition(changingPosition.x, changingPosition.y);
    }
}

bool player_check_walking(std::vector<player> &players, sf::Vector2f prevPosition)
{
    if(prevPosition.x!=players[0].getPosX() || prevPosition.y!=players[0].getPosY())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void create_and_shoot_bullet(bool &bulletCreated, sf::RectangleShape &bullet)
{

     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
     {
         bulletCreated=true;
     }
        bullet.setSize(sf::Vector2f(20, 20));
        bullet.setPosition(20, 30);
}

void server_receive_ip_port(sf::TcpSocket &TcpSocket, sf::TcpListener &listener)
{
    //sf::TcpSocket TcpSocket;
    sf::Packet cIPandPortPacket;
    std::string cIP;
    unsigned short clientPort;
    int currentAmountOfPlayers=2;
    sf::IpAddress clientIpReceived;
    std::vector<unsigned short> vectorClientPorts;

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
        }
    }
}

void client_send_ip_port(std::string cIP, unsigned short clientPort, sf::IpAddress serverIP)
{
    sf::TcpSocket TcpSocket;
    sf::Packet cIPandPortPacket;
    sf::TcpListener listenerForClients;
    listenerForClients.setBlocking(false);

    listenerForClients.listen(clientPort);
    TcpSocket.connect(serverIP,2000);
    cIPandPortPacket<<cIP<<clientPort;
    TcpSocket.send(cIPandPortPacket);
}

std::vector<player> makePlayers(int& amount_players) noexcept
{
    std::vector<player> newPlayer;
    for(int i=0; i!= amount_players; ++i)
    {
        newPlayer.push_back(player(30,30,0,0));
    }
    return newPlayer;
}
void send_position(sf::IpAddress ip, unsigned short port, std::vector<player> &players)
{
    sf::UdpSocket socket;
    sf::Packet posPacket;
    posPacket<<players[0].getPosX() <<players[0].getPosY();//<<playerNumber;
    if (socket.send(posPacket, ip, port) != sf::Socket::Done)
    {
        //std::cout<<"whoops... some data wasn't sent";
    }
}
void playerWalking(std::vector<player> &players, bool &update)
{
    float posX=players[0].getPosX();
    float posY=players[0].getPosY();

    if(update==true)
    {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                    posX+=3;
                    players[0].setPlayerPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                    posX-=3;
                   players[0].setPlayerPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                posY-=3;
                players[0].setPlayerPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                posY+=3;
                players[0].setPlayerPosition(posX, posY);
            }
        }
}

void do_server(bool &initializing,std::vector<player> &players, bool &update, sf::RenderWindow &window)
{
    sf::RectangleShape bullet;
    bool bulletCreated=false;
    sf::Event Event;
    sf::Vector2f prevPosition;
    sf::UdpSocket socket;

    socket.bind(2000);

    //unsigned short port;
    sf::IpAddress clientIP=sf::IpAddress::getLocalAddress();
    std::string cIP=clientIP.sf::IpAddress::toString();

    socket.setBlocking(false);
    initializing=false;

    sf::TcpSocket TcpSocket;
    sf::TcpListener listener;

    listener.listen(2000);
    listener.setBlocking(false);
    TcpSocket.setBlocking(false);

    while(window.isOpen())
    {
        while(window.pollEvent(Event))
        {
           if(Event.type == sf::Event::GainedFocus)
                update=true;
           if(Event.type == sf::Event::LostFocus)
               update = false;
        }
    server_receive_ip_port(TcpSocket, listener);
    prevPosition = sf::Vector2f(players[0].getPosX(), players[0].getPosY());
    playerWalking(players, update);

    sf::IpAddress recipient = clientIP;
    //unsigned short clientPort = port;
    if(player_check_walking(players, prevPosition)==true)
    {
         send_position(recipient, 2001, players);
    }

    receive_and_set_other_players_position(socket, players);

    create_and_shoot_bullet(bulletCreated, bullet);

    draw_everything(window, players, bullet);
   }
}
