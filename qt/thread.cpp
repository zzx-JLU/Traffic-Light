#include "thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

Thread::Thread(): STATES{0xffbe, 0xffdd}, state(0), delay(5), stopFlag(false), pauseFlag(false)
{	
    openDevice();
}

Thread::Thread(int delay): STATES{0xffbe, 0xffdd}, state(0), stopFlag(false), pauseFlag(false)
{
    openDevice();

    if (delay <= 0)
    {
        printf("Invalid delay time!\n");
        delay = 5;
    }
    this->delay = delay;
}

Thread::~Thread()
{
    if (fd >= 0)
    {
        close(fd);
    }
    
    stop();
}

void Thread::openDevice()
{
    fd = open("/dev/s3c2440_led0", O_RDWR);
    if (fd < 0)
    {
        printf("Led device open failed!\n");
    }
}

void Thread::stop()
{
    if (QThread::isRunning())
    {
        stopFlag = true;
        condition.wakeAll();
        QThread::quit();
        QThread::wait();
    }
}

void Thread::pause()
{
    if (QThread::isRunning())
    {
        pauseFlag = true;
    }
}

void Thread::resume()
{
    if (QThread::isRunning())
    {
        pauseFlag = false;
        condition.wakeAll();
    }
}

void Thread::switchState()
{
    state = (state + 1) % 2;
    ioctl(fd, 0x12, STATES[state]);
}

void Thread::sleepByDelay()
{
    QThread::sleep(delay);
}

void Thread::reset()
{
    ioctl(fd, 0x12, 0xffff);
    state = 0;
}

void Thread::process()
{
    switchState();
    sleepByDelay();
}

void Thread::run()
{
    if (fd < 0)
    {
        printf("Error: Led device didn't open.\n");
        return;
    }

    __asm__(
        "MOV R5,#0X011;"
        "STRB R5,[R4];"
    );

    ioctl(fd, 0x12, STATES[state]);
    sleepByDelay();

    while(!stopFlag)
    {
        process();
        if (pauseFlag)
        {
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }
    }
    
    pauseFlag = false;
    stopFlag = false;
}
