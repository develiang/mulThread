
#include <iostream>
#include <thread>
#include <vector>
#include <list>
#include <mutex>
using namespace std;

vector <int> g_v = { 1,2,3 }; //共享数据，只读

void myprint(int inum) {
    //cout << "线程开始执行， 线程id=" << inum << endl;
    //cout << "线程结束执行， 线程id=" << inum << endl;
    cout << "id为" << this_thread::get_id() << "的线程，打印g_v：" << g_v[0] << g_v[1] << g_v[2] << endl;
    return;
}

class A {
public:
    //把收到的消息放到一个队列
    void inMsgRecvQueue() {
        for (int i = 0; i < 100000; ++i) {
            cout << "inMsgRecvQueue()执行，插入元素" << i << endl;

            std::lock(my_mutex1, my_mutex2);
            lock_guard<mutex> sbguard1(my_mutex1, std::adopt_lock);
            lock_guard<mutex> sbguard2(my_mutex2, std::adopt_lock);
            //my_mutex.lock
            msgRecvQueue.push_back(i);
            //my_mutex.unlock();
        }
        return;
    }

    bool outMsgLULProc(int& command) {
        std::lock_guard<std::mutex> sbguard(my_mutex1);
        //my_mutex.lock();
        if (!msgRecvQueue.empty()) {
            int command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            //my_mutex.unlock();
            return true;
        }
        //my_mutex.unlock();
        return false;

    }
    //把数据从消息队列取出
    void outMsgRecvQueue() {
        int command = 0;
        for (int i = 0; i < 100000; ++i) {
            bool result = outMsgLULProc(command);
            if (result == true) {
                cout << "outMsgRecvQueue()执行，取出一个元素" << command << endl;
            }
            else {
                cout << "outMsgRecvQueue()执行，但消息队列为空" << i << endl;
            }
        }
        cout << "end" << endl;
    }
private:
    std::list<int> msgRecvQueue;
    std::mutex my_mutex1; //创建一个互斥量
    std::mutex my_mutex2; //创建一个互斥量
};

int main()
{

    A myobja;
    thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);
    std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
    myInMsgObj.join();
    myOutMsgObj.join();
    //这种同时读写共享数据，不加操作会出问题，因此需要引入“互斥量”的概念

    //保护共享数据，操作时把共享数据锁住
    //一、互斥量（mutex）：就是一个类对象，理解成一把锁
    //多个线程尝试用lock()的成员函数来加锁，只有一个线程能锁定（成功标志lock()锁定成功）
                                //如果没成功，不断尝试加锁直到成功

    //二、互斥量用法
    //（1）引入include <mutex>
    //（2.1） lock(), unlock()：需要成对使用
    //先lock，操作共享数据，unlock释放共享数据'

    //为了防止忘记unlock(),引入了std::lock_guard()类模板，忘记unlock，他会帮忙lock

    //(2.2) std::lock_guard()可以直接取代lock和unlock();
    //有了std::lock_guard()，不要在使用lock和unlock

    //三、死锁
    //产生的前提田间是，有两个互斥量才会产生
    // 死锁一般解决方案：保证两个互斥量上锁顺序一只就不会死锁

    //std::lock()函数模板：用来处理多个互斥量
    //能力：一次锁住两个或者两个以上的互斥量
    //不存在因为多个线程中，因为上锁顺序不一样产生死锁的风险
    //特点是要么两个互斥量都锁住，要么都释放，有一个没锁住，那么会把锁住的解锁

    //std::lock_guard() 和 std::adopt_lock()参数
    // std::adopt_lock结构体对象
    //std::lock_guard()是在调用函数中调用lock，析构函数中调用unlock，因此在退出该作用域之后会解锁
    //在std::lock_guard()使用std::adopt_lock，表示该互斥量已经lock，不需要再次lock





    std::cout << "Hello World!\n";
}


