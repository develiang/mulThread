// Thread_3.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
using namespace std;

class A {
public:
    int m_i;
    A(int a) : m_i(a) { cout << "[A::A(int a)构造函数执行]  " << this << "  thread_id = " << this_thread::get_id() << endl; };
    A(const A& a) : m_i(a.m_i) { cout << "[A::A(const a)拷贝构造函数执行]  " << this << "  thread_id = " << this_thread::get_id() << endl; };
    ~A(){ cout << "[A::~A()析构函数执行]  " << this << "  thread_id = " << this_thread::get_id() << endl;}
};


//void mypoint(const int& i, char* pmybuf) {
void mypoint(const int& i, const string& pmybuf) {
    cout << i << endl;  //经过地址分析得知，i并不是mvar的引用，实际是值传递，那么即使使用了detach，主线程销毁了，子线程依然可以使用参数
    cout << pmybuf.c_str() << endl; //指针引用一定有问题，因此创建多线程，如果使用了detach，禁止使用指针

    /*
    * （1.1）
        解决方法：
        将char* pmybuf修改成const string &pmybuf,意思是将传入的char类型强制转换成string，因此这两种数据结构肯定在两个内存中，因此当main释放后，string内存也是安全的
        相应的将输出代码改成  cout << pmybuf.c_str() << endl;
    * （1.2）
    *   使用（1.1）解决办法时产生的问题，main定义的mybuf是在何时将char转换成string？
    *   实际上存在当main函数执行完了，所有变量都被回收后，系统才将char转换成string
    * 
    * 终极解决办法：在main的创建thread时直接转换数据类型：
    *       thread mytobj(mypoint, mvary, string(mybuf));
    */
    return;
}

void myprint2(const A& pmybuf) {
    cout << "子线程myprint2参数地址 = " << &pmybuf << "  thread_id = " << this_thread::get_id() << endl;
}

int main()
{
    //一、传递临时对象作为线程参数
        // (1.1) 要避免的陷阱
        // 详见mypoint函数中注释
        // (1.2) 要避免的参数
    //二、
        //（2.1）线程id概念：id是个数字，每个线程实际上都对应一个数字，并且唯一，可以获取std::this_thread::get_id()

    //int mvar = 1;
    //int& mvary = mvar;
    //char mybuf[] = "this is a test!";
    ////thread mytobj(mypoint, mvary, mybuf);
    //thread mytobj(mypoint, mvary, string(mybuf));  //终极解决办法 
    ////mytobj.join();
    //mytobj.detach();    //如果使用detach让子线程和主线程分别执行

    cout << "主线程id = " << this_thread::get_id() << endl;
    int mvar = 1;
    thread mytobj(myprint2, A(mvar));
    mytobj.join();



    std::cout << "Hello World!\n";

    return 0;
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
