#include <iostream>
using namespace std;
class A {
public:
    int a, *b;
    A() {
        a = 1;
        b = new int [10];
        b[1] = 2;
    }
    virtual void print() {
        cout << a << b[1];
    }
    virtual ~A() {
        delete[] b;
    }
};

class B : public A {
public:
    int c;
    B () : A() {
        c = 2;
    }
    void print() override {
        cout << a << b[1] << c;
    }
    void printB() {
        cout << c;
    }
    ~B() {

    }
};

class C : public A {
public:
    void print() {

    }
};


int main() {
    A a;
    B b;
    a.print();
    b.print();
    A *pa = new B;
    pa -> print();
    dynamic_cast<B*>(pa) -> printB();
}
