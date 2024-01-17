#include <iostream>
#include <thread>
#include <functional> // 添加必要的头文件
#include <mutex>
using namespace std;

class Foo {
    mutex mut1, mut2;

public:
    Foo() {
        mut1.lock();
        mut2.lock();
    }

    void first(function<void()> printFirst) {
        printFirst();
        mut1.unlock();
    }

    void second(function<void()> printSecond) {
        mut1.lock();
        printSecond();
        mut1.unlock();
        mut2.unlock();
    }

    void third(function<void()> printThird) {
        mut2.lock();
        printThird();
        mut2.unlock();
    }
};

int main() {
    Foo foo;

    auto printFirst = []() { cout << "first" << endl; };
    auto printSecond = []() { cout << "second" << endl; };
    auto printThird = []() { cout << "third" << endl; };

    thread t1(&Foo::first, &foo, printFirst); // 为 first() 创建一个线程并执行它
    thread t2(&Foo::second, &foo, printSecond); // 为 second() 创建一个线程并执行它
    thread t3(&Foo::third, &foo, printThird); // 为 third() 创建一个线程并执行它

    t2.join(); // 等待线程 t1 完成
    t1.join(); // 等待线程 t2 完成
    t3.join(); // 等待线程 t3 完成

    return 0;
}


