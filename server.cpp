#include "server.h"
#include <sstream>
#include <cassert>

bool fire_able(int time, int rate_in_seconds)
{
    time+=1;
    //rate_in_seconds= rate_in_seconds*60;
    if(time>30)
    {
        return true;
    }
    else
    {
        return false;
    }
}
void receive_bullet_created(sf::Packet bulletPacket,std::vector<bullet> &bullets, std::vector<player> &players)
{
    std::string messageType=" ";
    if(bulletPacket>>messageType)
    {
        if(messageType=="bulletCreated")
        {
            std::cout<<"message received"; std::cout<< std::flush;
            bullets.push_back(bullet(10,10, players[1].getPosX(), players[1].getPosY()));
        }
    }
}

void set_player_position(std::vector<player> &players, sf::Packet posPacket)
{
    sf::Vector2f changingPosition;
    std::string messageType;

    if(posPacket>>changingPosition.x>>changingPosition.y>> messageType) //>> objectNumber -> needs to be added
    {
        if(messageType=="player")
        {
            players[1].setPlayerPosition(changingPosition.x, changingPosition.y);
        }
    }
}

void set_bullet_position(std::vector<bullet> &bullets, sf::Packet posPacket,std::string bulletText)
{
    sf::Vector2f changingPosition;
    std::ostringstream messageType;
    std::string messageReceived;

    if(posPacket>>changingPosition.x>>changingPosition.y>> messageReceived)
    {
        for(unsigned int i=0; i<bullets.size(); ++i)
        {
            messageType<<bulletText<<i;
            if(messageType.str()==messageReceived)
            {
                bullets[i].setBulletPosition(changingPosition.x, changingPosition.y);
            }
        }
    }
}

void send_position_bullet(const sf::IpAddress ip, const unsigned short port,
                          std::vector<bullet> &bullets)
{    
    std::ostringstream bulletMessage;
    std::string bulletText="bullet";

    sf::UdpSocket socket;
    sf::Packet posPacket;
    if(bullets.size()>0)
    {
        for(unsigned int i=0; i<bullets.size(); ++i)
        {
            bulletMessage<<bulletText<<i;
            posPacket<<bullets[i].getPosX() << bullets[i].getPosY() << bulletMessage.str();
            if (socket.send(posPacket, ip, port) != sf::Socket::Done)
            {
                //std::cout<<"whoops... some data wasn't sent";
            }
        }
    }
}

void draw_everything(sf::RenderWindow &window, std::vector<player> &players, std::vector<bullet> &bullets)
{
    window.clear();


    for(int i=0; i<static_cast<int>(bullets.size()); ++i)
    {
       bullets[i].display(window);
    }
    for(int i=0; i<static_cast<int>(players.size()); ++i)
    {
       players[i].display(window);
    }
    for(int i=0; i<17; ++i)
    {
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(30*i, 0)),
            sf::Vertex(sf::Vector2f(30*i, 500))
        };
        sf::Vertex line2[] =
        {
            sf::Vertex(sf::Vector2f(0, 30*i)),
            sf::Vertex(sf::Vector2f(500, 30*i))
        };
        window.draw(line, 2, sf::Lines);
        window.draw(line2, 2, sf::Lines);
    }
    window.display();
}

void receive_position_packets(sf::UdpSocket &socket, std::vector<player> &players, std::vector<bullet> &bullets)
{
    std::string bulletText ="bullet";
    sf::IpAddress sender;
    unsigned short port;
    sf::Packet posPacket;
    int j=0;

    while(j<6)
    {
    j+=1;
        if (socket.receive(posPacket,sender,port) != sf::Socket::Done)
        {
            //std::cout<<"whoops... some data wasn't received";
        }
        set_player_position(players, posPacket);
        set_bullet_position(bullets, posPacket, bulletText);
        receive_bullet_created(posPacket, bullets, players);
    }

}

/// player::has_walked() const noexcept { return getPos() != getPrevPos(); }
bool player_check_walking(const std::vector<player> &players, sf::Vector2f prevPosition)
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


/// shoot_bullets
void shoot_bullet(std::vector<bullet> &bullets,sf::IpAddress &ip, unsigned short &port, std::vector<player> &players, bool &update, int &time)
{
     sf::Packet posPacket;
     int bulletSpeed =3;
     if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && update==true && fire_able(time,2)==true)
     {
         bullets.push_back(bullet(10,10, players[0].getPosX(), players[0].getPosY()));
         std::string bulletMessage ="bulletCreated";
         sf::UdpSocket socket;
         posPacket<<bulletMessage;
         if (socket.send(posPacket, ip, port) != sf::Socket::Done)
         {
             //std::cout<<"whoops... some data wasn't sent";
         }
         time=0;
     }
     if(bullets.size()>0)
     {
        for(unsigned int i=0; i<bullets.size(); ++i)
        {
            bullets[i].setBulletPosition(bullets[i].getPosX()+bulletSpeed, bullets[i].getPosY());
        }
     }
}

void server_receive_ip_port(sf::TcpSocket &TcpSocket, sf::TcpListener &listener, unsigned short &clientPort)
{
    //sf::TcpSocket TcpSocket;
    sf::Packet cIPandPortPacket;
    std::string cIP;
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

void send_position(sf::IpAddress ip, unsigned short port, const std::vector<player> &players)
{
    std::string playerMessage ="player";
    sf::UdpSocket socket;
    sf::Packet posPacket;
    posPacket<<players[0].getPosX() <<players[0].getPosY()<< playerMessage;//<<playerNumber;
    if (socket.send(posPacket, ip, port) != sf::Socket::Done)
    {
        //std::cout<<"whoops... some data wasn't sent";
    }
}
void playerWalking(std::vector<player> &players, bool &update, int &time)
{
    float posX=players[0].getPosX();
    float posY=players[0].getPosY();

    if(update==true&& time>10)
    {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                    time=0;
                    posX+=30;
                    players[0].setPlayerPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                   time=0;
                    posX-=30;
                   players[0].setPlayerPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                time=0;
                posY-=30;
                players[0].setPlayerPosition(posX, posY);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)
                || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                time=0;
                posY+=30;
                players[0].setPlayerPosition(posX, posY);
            }
        }
}

void do_server(bool &initializing,std::vector<player> &players, bool &update, sf::RenderWindow &window)
{
    int time=0;
    std::vector<bullet> bullets{};
    sf::Event Event;
    sf::Vector2f prevPosition;
    sf::UdpSocket socket;
    unsigned short clientPort;
    socket.bind(2000);

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

    time+=1;
    server_receive_ip_port(TcpSocket, listener, clientPort);
    prevPosition = sf::Vector2f(players[0].getPosX(), players[0].getPosY());
    playerWalking(players, update, time);

    sf::IpAddress recipient = clientIP;

    if(player_check_walking(players, prevPosition)==true)
    {
         send_position(recipient, clientPort, players);
    }

    receive_position_packets(socket, players, bullets);

    shoot_bullet(bullets, recipient, clientPort,players, update, time);

    send_position_bullet(recipient, clientPort, bullets);

    draw_everything(window, players, bullets);
   }
}
