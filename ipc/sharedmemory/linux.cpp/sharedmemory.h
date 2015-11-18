#ifndef _SHAREDMEMORY_H
#define _SHAREDMEMORY_H
#ifdef __linux__
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <string.h>

class SimpleSharedMemory
{
public:
    explicit SimpleSharedMemory();
    SimpleSharedMemory(bool isServer, size_t size=1024);
    SimpleSharedMemory(bool isServer, std::string keyfile, size_t size=1024);
    ~SimpleSharedMemory();
public:
    void create(bool isServer, std::string keyfile, size_t size);

    int write(void *, int size);
    int read(void*);
    int read(char*);
    void *head() { return _shmHandle; }
    size_t size() { return _size; }
private:
    key_t _key;
    int _shmID;
    void *_shmHandle;

    bool _isServer;
    size_t _size;
};
#endif

#endif // _SHAREDMEMORY_H
