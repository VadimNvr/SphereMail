//
//  cServer.cpp
//  Game_Server
//
//  Created by Вадим Навражных on 04.03.16.
//  Copyright © 2016 VadimAndVova. All rights reserved.
//

#include "cServer.h"

struct ev_loop *Server::main_loop = NULL;
int Server::server_socket;
list<int> Server::clients;
list<std::thread> Server::threads;
std::thread Server::server_thread;
Scene *Server::m_pscene;
int Server::connections = 0;

int Server::set_nonblock(int fd)
{
    int flags;
    #ifdef O_NONBLOCK
    if (-1 == (flags = fcntl(fd, F_GETFL, 0))) flags = 0;
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    #endif
    flags = 1;
    return ioctl(fd, FIONBIO, &flags);
}

void Server::accept_cb(struct ev_loop *loop, struct ev_io *server, int revents)
{
    int client_socket = accept(server->fd, 0, 0);
    set_nonblock(client_socket);
    clients.push_front(client_socket);
    
    struct ev_io *watcher = new my_io;
    ((my_io *) watcher)->val = clients.size() - 1;
    ev_init(watcher, read_cb);
    ev_io_set(watcher, client_socket, EV_READ);
    ev_io_start(loop, watcher);
    
    std::cout << "accepted connection\n";
}

void Server::read_cb(struct ev_loop *loop, struct ev_io *watcher, int revents)
{
    char cache[BUFSIZE];
    
    struct sockaddr_in addr;
    socklen_t addr_len;
    if (recvfrom(server_socket, cache, BUFSIZE, 0, (struct sockaddr*) &addr, &addr_len) > 0)
    {
        SyncData *data = (SyncData *) cache;
        
        switch (data->type)
        {
            case SyncData::HandShake:
                if (connections < 2)
                {
                    SyncData reply;
                    reply.type = SyncData::HandShake;
                    reply.id = ++connections;
                    sendto(server_socket, &reply, sizeof(SyncData), 0, (struct sockaddr*) &addr, addr_len);
                }
                break;
                
            case SyncData::CtS:
                if (m_pscene)
                    switch (data->id)
                    {
                        case 1:
                            m_pscene->m_knight->setPos(data->player);
                            break;
                        
                        case 2:
                            m_pscene->m_wizard->setPos(data->player);
                            break;
                        
                    default:
                        break;
                }
                break;
                
            default:
                break;
        }
    }
    
    //CLIENT DISCONNECTED
    /*
    if (0 == recv(client->fd, cache, BUFSIZE, 0))
    {
        clients.remove(client->fd);
        std::cout << "connection terminated\n";
        ev_io_stop(loop, client);
        delete static_cast<my_io *>(client);
        ::close(client->fd);
    }
    //RECIEVED MESSAGE FROM CLIENT
    else
    {
     */
    
    /*
        memcpy(buf, cache, BUFSIZE);
        
        //reading whole message in buf
        while (recv(client->fd, cache, BUFSIZE, 0) > 0)
        {
            buf_len += BUFSIZE;
            buf = (char *) realloc(buf, buf_len);
            
            memcpy(buf + (buf_len - BUFSIZE), cache, BUFSIZE);
        }
        
        std::cout << "Recieved from client" << std::endl;
        
        SyncData *data = (SyncData *) cache;
    
        if (m_pscene && (data->type == SyncData::CtS))
            switch (((my_io *) client)->val)
            {
                case 0:
                    m_pscene->m_knight->setPos(data->player);
                    break;
                    
                case 1:
                    m_pscene->m_wizard->setPos(data->player);
                    break;
                    
                default:
                    break;
            }
    //}
    
    free(buf);
     */
}

void Server::launch(int port)
{
    std::cout << "Launching server" << std::endl;
    //CREATING SERVER
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    
    //FILLING SERVER ADDRESS STRUCTURE
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    //BINDING SERVER TO ADDRESS
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        throw "Error in bind";
    
    //PREPAIRING TO ACCEPT CLIENTS
    //if (listen(server_socket, SOMAXCONN) < 0)
      //  throw "Error in listen";
    
    //MAKING EVENT LOOP
    main_loop = ev_default_loop(0);
    
    //MAKING EVENT FOR ACCEPT
    struct ev_io *watcher = new my_io;
    ev_io_init(watcher, read_cb, server_socket, EV_WRITE);
    //ev_io_set(watcher, server_socket, EV_READ);
    ev_io_start(main_loop, watcher);
}

void Server::loop()
{
    std::cout << "Starting loop" << std::endl;
    ev_run(main_loop);
}

void Server::close()
{
    shutdown(server_socket, SHUT_RDWR);
    ::close(server_socket);
}
