//
//  cClient.h
//  Game_Server
//
//  Created by Вадим Навражных on 05.03.16.
//  Copyright © 2016 VadimAndVova. All rights reserved.
//

#ifndef cClient_h
#define cClient_h


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
#include "cServer.h"

#define BUFSIZE sizeof(SyncData)

using std::string;
using std::list;

class Client {
private:
    static struct ev_loop *main_loop;
    
    static int set_nonblock(int);
    //CALLBACK ON SERVER MESSAGE / DISCONNECT
    static void read_cb(struct ev_loop *loop, struct ev_io *client, int revents);
    
public:
    static int client_socket;
    static std::thread client_thread;
    static Scene *m_pscene;
    static struct sockaddr_in serv_addr;
    static uint id;
    
    static void connect(int port);
    static void sendToServer(void *src, int size);
    static void close();
    static void loop();
};


#endif /* cClient_h */
