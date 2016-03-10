//
//  cClient.cpp
//  Game_Server
//
//  Created by Вадим Навражных on 05.03.16.
//  Copyright © 2016 VadimAndVova. All rights reserved.
//

#include "cClient.h"

struct ev_loop *Client::main_loop = NULL;
int Client::client_socket;
std::thread Client::client_thread;
Scene *Client::m_pscene;
struct sockaddr_in Client::serv_addr;
uint Client::id = 0;

int Client::set_nonblock(int fd)
{
    int flags;
#ifdef O_NONBLOCK
    if (-1 == (flags = fcntl(fd, F_GETFL, 0))) flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#endif
    flags = 1;
    return ioctl(fd, FIONBIO, &flags);
}

void Client::read_cb(struct ev_loop *loop, struct ev_io *server, int revents)
{
    char cache[BUFSIZE];
    
    struct sockaddr_in addr;
    socklen_t addr_len;
    if (recvfrom(client_socket, cache, BUFSIZE, 0, (struct sockaddr*) &addr, &addr_len) > 0)
    {
        std::cout << "got something" << std::endl;
        SyncData *data = (SyncData *) cache;
        
        switch (data->type)
        {
            case SyncData::HandShake:
                if (id == 0)
                {
                    id = data->id;
                    std::cout << "id is " << id << std::endl;
                }
                break;
                
            case SyncData::StC:
                if (m_pscene)
                {
                    switch (m_pscene->m_monsters.size()) {
                        case 2:
                            m_pscene->m_monsters[1]->setPos(data->bots[1]);
                        case 1:
                            m_pscene->m_monsters[0]->setPos(data->bots[0]);
                        default:
                            break;
                    }
                }
                break;
                
            default:
                break;
        }
    }
    
    /*
    //SERVER DISCONNECTED
    if (0 == recv(server->fd, cache, BUFSIZE, 0))
    {
        std::cout << "connection terminated\n";
        ev_io_stop(loop, server);
        delete server;
        ::close(server->fd);
    }
    //RECIEVED MESSAGE FROM SERVER
    else
    {
        memcpy(buf, cache, BUFSIZE);
        
        //reading whole message in buf
        while (recv(server->fd, cache, BUFSIZE, 0) > 0)
        {
            buf_len += BUFSIZE;
            buf = (char *) realloc(buf, buf_len);
            
            memcpy(buf + (buf_len - BUFSIZE), cache, BUFSIZE);
        }
        
        SyncData *data = (SyncData *) cache;
        
        if ((m_pscene) && (data->type == SyncData::StC))
        {
            switch (m_pscene->m_monsters.size()) {
                case 2:
                    m_pscene->m_monsters[1]->setPos(data->bots[1]);
                case 1:
                    m_pscene->m_monsters[0]->setPos(data->bots[0]);
                default:
                    break;
            }
        }
    }
    
    free(buf);
     */
}

void Client::connect(int port)
{
    signal(SIGPIPE, SIG_IGN);
    
    //CREATING A SOCKET FOR CLIENT
    client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    //FILLING SERVER ADDRESS STRUCTURE
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //CONNECTING TO SERVER
    //if (::connect(server_socket, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    //    throw "Error connecting";
    
    set_nonblock(client_socket);
    
    //MAKING EVENT LOOP
    main_loop = ev_default_loop(0);
    
    //MAKING EVENT FOR RECV
    struct ev_io *watcher = new ev_io;
    ev_io_init(watcher, read_cb, client_socket, EV_WRITE);
    //ev_io_set(watcher, server_socket, EV_READ);
    ev_io_start(main_loop, watcher);
    
    client_thread = std::thread(loop);
    client_thread.detach();
    
    SyncData data;
    data.type = SyncData::HandShake;
    sendToServer(&data, sizeof(SyncData));
    
    std::cout << "Connection succeed" << std::endl;
}

void Client::sendToServer(void *src, int size)
{
    sendto(client_socket, src, size, 0, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr_in));
}

void Client::loop()
{
    ev_run(main_loop);
}

void Client::close()
{
    //shutdown(server_socket, SHUT_RDWR);
    //::close(server_socket);
}

