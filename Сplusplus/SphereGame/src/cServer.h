//
//  cServer.hpp
//  Game_Server
//
//  Created by Вадим Навражных on 04.03.16.
//  Copyright © 2016 VadimAndVova. All rights reserved.
//

#ifndef cServer_h
#define cServer_h

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <vector>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <iostream>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <signal.h>
#include <list>
#include <netdb.h>
#include <thread>
#include <ev++.h>

#include "cScene.h"

#define BUFSIZE sizeof(SyncData)
#define THREAD_COUNT 1

using std::string;
using std::list;

class my_io: public ev_io
{
public:
    int val;
};

struct SyncData
{
    enum {StC, CtS, HandShake} type;
    uint id;
    
    union
    {
        SDL_Rect bots[2];
        SDL_Rect player;
    };
    
    SyncData() {}
};

class Server {
private:
    static struct ev_loop *main_loop;
    static int server_socket;
    
    static int set_nonblock(int);
    //CALLBACK ON CLIENT ACCEPT
    static void accept_cb(struct ev_loop *loop, struct ev_io *server, int revents);
    //CALLBACK ON CLIENT MESSAGE / DISCONNECT
    static void read_cb(struct ev_loop *loop, struct ev_io *client, int revents);
    
public:
    static list<int> clients;
    static list<std::thread> threads;
    static std::thread server_thread;
    static Scene *m_pscene;
    static int connections;
    
    static void launch(int port);
    static void close();
    static void loop();
};

#endif /* cServer_h */
