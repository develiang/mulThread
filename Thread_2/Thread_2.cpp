#include <iostream>
#include <thread>
#include <ctime>
using namespace std;

void print() {
    cout << "start run thread" << endl;
    cout << "end run thread" << endl;
}

class TA
{
public:
    // 由于是使用引用来获取int值，所以一旦主线程被销毁，所有定义在主线程中的变量丢失，因此子线程无法通过引用获取
    int m_i;
    TA(int i) :m_i(i) {
        cout << "TA()构造函数执行" << endl;
    }
    TA(const TA& ta) : m_i(ta.m_i) {
        cout << "TA()拷贝构造函数执行" << endl;
    }
    ~TA(){
        cout << "TA()析构构造函数执行" << endl;
    }
    void operator()()
    {
        //cout << "run operate" << endl;
        //cout << "end operate" << endl;
        cout << "m_i1的值为：" << m_i << endl;
        cout << "m_i2的值为：" << m_i << endl;
        cout << "m_i3的值为：" << m_i << endl;
        cout << "m_i4的值为：" << m_i << endl;
        cout << "m_i5的值为：" << m_i << endl;
    }
};

int main()
{
    std::cout << "Hello World!\n";
    
    //主线程从main（）开始运行，自己创建的线程也需要从一个函数开始运行

    //创建一个线程的步骤:
        // a） 添加头文件include <thread>
        // b） 写初始函数
        // c） main中开始写多线程代码
    //多线程是指有两个通路，即使一条路被堵住了，另一个线程依然可以运行
        
    //  (1.1)  thread: 就是标准库里的类
    //  (1.2)  join(): 阻塞主线程，让主线程等待子线程执行完毕
    //thread mytobj(print);   //(1)创建了线程，线程起点的入口是print()  (2)print线程开始执行
    //mytobj.join();  //主线程阻塞，等待print子线程执行完毕


    //  (1.3)  detach(): 传统多线程中主线程要等待子线程执行完毕，之后主线程再退出
        //使用detach后，主线程不和子线程汇合，两者各自执行，主线程执行完毕并不影响子线程的执行
        //一旦detach()之后，主线程关联的thread对象就会失去与主线程的关联,失去对子线程中的控制，子线程由后台接管
    //mytobj.detach();


    //  (1.4)  joinable(): 判断是否可以成功使用join或者detach

    //  二，其他创建线程方法
    //  (2.1) 用类对象（可调用对象）创建线程可调用对象


    //  (2.2) 用lambda表达式创建对象
    auto mylamthread = [] {
        cout << "lambda run" << endl;
        cout << "lambda endl" << endl;
    };
    thread mytobj4(mylamthread);
    mytobj4                                                             .join();
    int myi = 6;
    TA ta(myi);
    thread mytobj3(ta);
    mytobj3.detach();
    //mytobj3.join();
    cout << "end ta" << endl;
    // 一旦调用detach(),主线程结束了，定义的ta对象还在吗？实际上原对象不在了，但是还能运行ta原因是对象被复制到线程中去了


    thread mytobj(print);
    if (mytobj.joinable()) {
        cout << "joinable() == true" << endl;
    }
    else {
        cout << "joinable() == false" << endl;
    }

    mytobj.join();


    cout << "end main" << endl;

    return 0;
}


