// Thread_7.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>
#include <list>
using namespace std;

std::mutex resource_mutex;
std::once_flag g_flag; //这是个系统定义的标记

class A {
public:
    //把收到的消息放到一个队列
    void inMsgRecvQueue() {
        for (int i = 0; i < 100000; ++i) {
            cout << "inMsgRecvQueue()执行，插入元素" << i << endl;

            std::unique_lock<std::mutex> sbguard1(my_mutex1);
            msgRecvQueue.push_back(i);
            my_cond.notify_one();   //尝试把wait()线程唤醒
            //如果其他线程在执行一些比较耗时的操作，并没有卡在wait上，执行notify_one没有效果
        }
        return;
    }

    //bool outMsgLULProc(int& command) {
    //    std::unique_lock<std::mutex> sbguard1(my_mutex1);

    //    if (!msgRecvQueue.empty()) {
    //        command = msgRecvQueue.front();
    //        msgRecvQueue.pop_front();
    //        return true;
    //    }
    //    return false;

    //}
    //把数据从消息队列取出
    void outMsgRecvQueue() {
        int command = 0;
        while (1) 
        {
            std::unique_lock<std::mutex> sbguard1(my_mutex1);

            //wait() 就是在等一个东西，如果第二个参数（lambda返回值）是false，wait将解锁互斥量，并堵塞本行
                //就是说拿到互斥量进行堵塞
                //一直堵塞到其他某个线程调用notify_one()成员函数为止；
                // 如果wait()第二个参数是true，那么wait()直接返回，不堵塞本行
            //如果wait()没有第二个参数，那么就默认第二个参数为false

            //wait()在被notify_one()唤醒后，就不断尝试获取互斥量，如果获取不到流程就卡在此处
            //如果获取到互斥量，wait就继续执行
            my_cond.wait(sbguard1, [this] { //一个lambda表达式就是一个可调用对象（函数）
                if (!msgRecvQueue.empty()) 
                    return true;
                return false;
            });

            command = msgRecvQueue.front();
            msgRecvQueue.pop_front();
            sbguard1.unlock();
            cout << "取出一个元素" << endl;

            //以下执行一些操作


        }


        //for (int i = 0; i < 100000; ++i) {
        //    bool result = outMsgLULProc(command);
        //    if (result == true) {
        //        cout << "outMsgRecvQueue()执行，取出一个元素" << command << endl;
        //    }
        //    else {
        //        cout << "outMsgRecvQueue()执行，但消息队列为空" << i << endl;
        //    }
        //}
        //cout << "end" << endl;
    }
private:
    std::list<int> msgRecvQueue;
    std::mutex my_mutex1; //创建一个互斥量
    std::condition_variable my_cond; //生成一个条件变量对象
};

////类静态初始化
//MyCAS* MyCAS::m_instance = NULL;
//
////线程入口函数
//void mythread() {
//    cout << "我的线程 开始执行" << endl;
//    MyCAS* p_a = MyCAS::GetInstance();
//    p_a->func();
//    cout << "我的线程 执行完毕" << endl;
//
//
//}


int main()
{
    //一、条件变量 std::condition_variable 、wait()、notify_one()
    //线程A：等待一个条件满足
    //线程B：专门往消息队列发送数据
    //std::condition_variable是一个类，就是等待一个条件达成，需要和互斥量配合工作；
    //notify_one()通知一个线程唤醒


    //二、代码深入思考

    //三、notify_all()
    //一旦有多个进程在wait中，可以使用notify_all()唤醒所有处于wait的线程中

    A myobja;
    std::thread myOutnMsgObj(&A::outMsgRecvQueue, &myobja);
    std::thread myOutnMsgObj2(&A::outMsgRecvQueue, &myobja);
    std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
    myInMsgObj.join();
    myOutnMsgObj.join();
    myOutnMsgObj2.join();

    std::cout << "Hello World!\n";
    return 0;
}

