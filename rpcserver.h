#pragma once
#include "tqueque/tqueque.h"
#include <pthread.h>
#include <stdatomic.h>
#include "freertos/idf_additions.h"
#include <arpa/inet.h>
#include <stddef.h>
#include "rpccall.h"
#include <ffi.h>
#include "hashtable.c/hashtable.h"

typedef void (*rpcserver_newclient_cb)(void* userdata, struct sockaddr_in);
typedef void (*rpcserver_clientdiscon_cb)(void* userdata, struct sockaddr_in);
struct rpcserver{
    int sfd;
    int stop;
    pthread_mutex_t edit;
    TaskHandle_t acceptor;
    hashtable_t fn_ht;
    hashtable_t users;
    atomic_ullong clientcount;
    void* interfunc;
    void* newclient_cb_data;
    rpcserver_newclient_cb newclient_cb;
    void* clientdiscon_cb_data;
    rpcserver_clientdiscon_cb clientdiscon_cb;
};
struct fn{
    char* fn_name;
    void* fn; //libffi;
    uint8_t nargs;
    enum rpctypes rtype;
    enum rpctypes* argtypes;
    ffi_type** ffi_type_free;
    ffi_cif cif;
    void* personal;
    int perm;
};
struct client_thread{
    int client_fd;
    struct rpcserver* serv;
    char client_uniq[65];
    struct sockaddr_in addr;
};



struct rpcserver* rpcserver_create(uint16_t port);
void rpcserver_start(struct rpcserver* rpcserver);
void rpcserver_free(struct rpcserver* serv);
void rpcserver_stop(struct rpcserver* serv);
int rpcserver_register_fn(struct rpcserver* serv, void* fn, char* fn_name,
                          enum rpctypes rtype, enum rpctypes* argstype,
                          uint8_t argsamm, void* pstorage,int perm);
void rpcserver_register_newclient_cb(struct rpcserver* serv,rpcserver_newclient_cb callback, void* user);
void rpcserver_register_clientdiscon_cb(struct rpcserver* serv,rpcserver_clientdiscon_cb callback, void* user);
void rpcserver_unregister_fn(struct rpcserver* serv, char* fn_name);
void rpcserver_load_keys(struct rpcserver* serv, char* filename);
