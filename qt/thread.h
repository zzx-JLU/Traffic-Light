#ifndef THREAD_H
#define THREAD_H

#include <QMainWindow>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <atomic>
using namespace std;

class Thread : public QThread
{
    Q_OBJECT

private:
    // North & South: 0xffbe    10111110
    // East  & West:  0xffdd    11011101
    const unsigned int STATES[2];

    int fd;
    int state;
    int delay;
    
    atomic_bool stopFlag;
	atomic_bool pauseFlag;
	QMutex mutex;
	QWaitCondition condition;

    void openDevice();

protected:
    virtual void run();
    virtual void process();

public:
    Thread();
    Thread(int delay);
    ~Thread();

    void stop();
    void pause();
    void resume();
    
    void switchState();
    void sleepByDelay();
    void reset();
};

#endif // THREAD_H
