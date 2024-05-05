// Thread_7.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;

std::mutex resource_mutex;
std::once_flag g_flag; //这是个系统定义的标记

class MyCAS
{   //创建一个单例类
    static void CreateInstance() //只被调用一次
    {
        std::chrono::milliseconds dura(2000);
        this_thread::sleep_for(dura);

        m_instance = new MyCAS();
        static CGarhuishou cl;
    }
private:
    MyCAS() {} //将构造函数私有化

private:
    static MyCAS* m_instance;
public:
    static MyCAS* GetInstance()
    {
        //if (m_instance == NULL)
        //{ //双重锁定（双重检查） 可以提高效率
        //    unique_lock<mutex> mymutex(resource_mutex); //自动加锁
        //    if (m_instance == NULL)
        //    {
        //        m_instance = new MyCAS();
        //        static CGarhuishou cl;  //引入一个静态对象，在回收代码时使用析构CGarhuishou，可以回收new出来的对象。
        //    }
        //}
        std::call_once(g_flag, CreateInstance); //假设两个线程同时执行到这里，其中一个线程要等待另外一个线程执行完毕
        cout << "call once done" << endl;
        return m_instance;
    }

    class CGarhuishou
    { //类中套类 ，用来释放对象
    public:
        ~CGarhuishou()
        {
            if (MyCAS::m_instance)
            {
                delete MyCAS::m_instance;
                MyCAS::m_instance = NULL;
            }
        }
    };

    void func() {
        cout << "测试" << endl;
    }
};

//类静态初始化
MyCAS* MyCAS::m_instance = NULL;

//线程入口函数
void mythread() {
    cout << "我的线程 开始执行" << endl;
    MyCAS* p_a = MyCAS::GetInstance();
    p_a->func();
    cout << "我的线程 执行完毕" << endl;


}


int main()
{
    //一：设计模式
        //一般是代码的一些写法
    //二、单例设计模式：比较常用的设计模式
        //单例类

    //MyCAS* p_a = MyCAS::GetInstance();  //创建一个对象，返回该类对象的指针
    //MyCAS* p_b = MyCAS::GetInstance();  //再次创建对象，两个指针指向同一个对象

    //p_a->func();
    //MyCAS::GetInstance()->func();

    //三、单例设计模式共享数据问题分析、解决
        //面临的问题：需要在自己创建的线程，而不是主线程中创建单例类对象，并且这种线程可能不止一个
        //我们可能面临GetInstance()函数需要互斥
    thread mytobj1(mythread);
    thread mytobj2(mythread);
    mytobj1.join();
    mytobj2.join();

    //四、std::call_once()：该函数第二个参数是函数名
        //功能是能够保证填入的函数，只能被调用一次，因此它具备互斥量能力，并且消耗的资源比互斥量更少
        //这个函数需要和一个标记结合使用，std::once_flag;

    std::cout << "Hello World!\n";
}

