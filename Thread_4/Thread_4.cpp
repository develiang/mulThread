// Thread_4.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <vector>
#include <list>
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
        for (int i= 0; i < 100000; ++i) {
            cout << "inMsgRecvQueue()执行，插入元素" << i << endl;
            msgRecvQueue.push_back(i);
        }
    }
    //把数据从消息队列取出
    void outMsgRecvQueue() {
        for (int i = 0; i < 100000; ++i) {
            if (!msgRecvQueue.empty()) {
                int command = msgRecvQueue.front();
                msgRecvQueue.pop_front();
            }
            else {
                cout << "outMsgRecvQueue()执行，但消息队列为空" << i << endl;
            }
        }
        cout << "end" << endl;
    }
private:
    std::list<int> msgRecvQueue;
};

int main()
{

    vector <thread> mythreads;
    //创建10个线程
    for (int i = 0; i < 10; i++) {
        mythreads.push_back(thread(myprint, i)); //创建10个线程，并且线程已经开始执行
    }
    for (auto iter = mythreads.begin(); iter != mythreads.end(); ++iter) {
        iter->join();
    }

    //二、数据共享问题分析
    //（2.1）只读的数据， 是安全稳定的，可以直接读取
    //（2.2）有读有写，如果没加特殊处理，程序会崩溃
    //（2.3）其他案例：
        //数据共享：比如订票系统

    //三、共享数据的保护案例代码
    //网络游戏服务器。比如有两个线程，一个线程收集玩家命令（用数字代表命令），并把命令写到一个队列
    //                                  另一个线程从队列中取出，解析命令并执行
    //vector和list区别，list：对随机插入删除效率高。 vector：用于频繁按顺序插入和删除数据效率高
    // 
    //
    A myobja;
    thread myOutMsgObj(&A::outMsgRecvQueue, &myobja);
    std::thread myInMsgObj(&A::inMsgRecvQueue, &myobja);
    myInMsgObj.join();
    myOutMsgObj.join();
    //这种同时读写共享数据，不加操作会出问题，因此需要引入“互斥量”的概念



    std::cout << "Hello World!\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
