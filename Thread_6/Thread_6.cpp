
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

            std::unique_lock<std::mutex> sbguard1(my_mutex1, std::defer_lock);//创建了没有加锁的my_mutex1
            sbguard1.lock(); //不用unlock，因为unique_lock可以自己解锁

            msgRecvQueue.push_back(i);


            //if (sbguard1.owns_lock()) {
            //    //拿到了锁
            //    //执行接受消息代码
            //    msgRecvQueue.push_back(i);
            //}
            //else {
            //    cout << "inMsgRecvQueue()执行，但没拿到锁" << endl;
            //}
            //std::lock(my_mutex1, my_mutex2);
            //lock_guard<mutex> sbguard1(my_mutex1, std::adopt_lock);
            //lock_guard<mutex> sbguard2(my_mutex2, std::adopt_lock);
            //my_mutex.lock
            //msgRecvQueue.push_back(i);
            //my_mutex.unlock();
        }
        return;
    }

    bool outMsgLULProc(int& command) {
        std::unique_lock<std::mutex> sbguard1(my_mutex1);

        std::chrono::milliseconds dura(200);
        std::this_thread::sleep_for(dura); //让线程休息一定时间


        //my_mutex.lock();
        if (!msgRecvQueue.empty()) {
            command = msgRecvQueue.front();
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

    //一、unique_lock取代lock_guard
    //unique_lock比lock_guard灵活；但是效率差一点，内存占用多一点，在使用上与lock_guard没区别
    // 
    //二、lock_guard可以带第二个参数：
        //std::lock_guard<std::mutex> sbguard1(my_mutex1, std::adopt_lock)  第二个参数起到标记作用
        //(1)std::adopt_lock:标记作用，表示互斥量已经lock不需要再次lock，使用前需要确定互斥量已经lock，不然报错
        //(2)std::try_to_lock:尝试用mutex的lock锁定mutex，如果没有 锁定成功，也会立即返回，不会阻塞在那里
        //(3)std::defer_lock:使用前提是不能先自己lock，初始化了一个没有枷锁的mutex，是为了调用一些成员函数 ，展现灵活性

    //三、unique_lock的成员函数
        //(1)lock()：加锁
        //(2)unlock()：解锁
        //(3)try_lock()：尝试给互斥量加锁，如果没锁定成功，也会立即返回
        //(4)release()：返回他所管理的mutex对象指针，释放所有权；也就是说unique_lock和mutex不再有关系

    //为什么需要unlock，因为lock的代码段越少，程序运行效率越高；
    //粒度，代表锁头锁住代码的多少

    //四、unique_lock所有权的传递
    //所有权概念就是定义的unique_lock拥有定义的mutex的所有权
        //std::unique_lock<std::mutex> sbguard1(my_mutex1);
    //如何转移：
        //1.
        //std::unique_lock<std::mutex> sbguard2(std::move(my_mutex1));
        //将sbguard1拥有的mutex1的所有权转移给sbguard2 
        //2.
        //创建一个函数，返回其临时的unique_lock,也能转移所有权

    





    std::cout << "Hello World!\n";
}


