#include <iostream>
using namespace std;

class A {
public:
    virtual void show() const {
        cout << "A::show()" << endl;
    }
};

class B : public A {
public:
    void show() const override { // 重写 A 的 show()
        cout << "B::show()" << endl;
    }
};

class C : public B {
public:
    void show() const override { // 再次重写 B 的 show()
        cout << "C::show()" << endl;
    }
};

int main() {
    A* obj1 = new A();
    A* obj2 = new B();
    A* obj3 = new C();

    obj1->show(); // 输出：A::show()
    obj2->show(); // 输出：B::show()
    obj3->show(); // 输出：C::show()

    delete obj1;
    delete obj2;
    delete obj3;

    return 0;
}
