
#include <iostream>
#include <vector>
#include <memory>
#include <chrono>
using namespace std;
using namespace chrono;

class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
};

class OP : public Bill {
public:
    double total() const override { return 1700; }
};

class IP : public Bill {
public:
    double total() const override { return 47500; }
};

class DayCare : public Bill {
public:
    double total() const override { return 18000; }
};

class Emergency : public Bill {
public:
    double total() const override { return 5000; }
};

struct Record {
    int type;
};

double switchTotal(const Record& b) {
    switch (b.type) {
        case 0: return 1700;
        case 1: return 47500;
        case 2: return 18000;
        case 3: return 5000;
        default: return 0;
    }
}

void benchmark(int n) {
    vector<unique_ptr<Bill>> virtualBills;
    vector<Record> switchBills;

    for (int i = 0; i < n; i++) {
        int type = i % 4;

        switchBills.push_back({type});

        if (type == 0)
            virtualBills.push_back(make_unique<OP>());
        else if (type == 1)
            virtualBills.push_back(make_unique<IP>());
        else if (type == 2)
            virtualBills.push_back(make_unique<DayCare>());
        else
            virtualBills.push_back(make_unique<Emergency>());
    }

    volatile double sum1 = 0;
    volatile double sum2 = 0;

    auto start1 = steady_clock::now();

    for (const auto& b : virtualBills)
        sum1 += b->total();

    auto end1 = steady_clock::now();

    auto start2 = steady_clock::now();

    for (const auto& b : switchBills)
        sum2 += switchTotal(b);

    auto end2 = steady_clock::now();

    cout << "\nNumber of bills: " << n << '\n';
    cout << "Virtual total: " << sum1 << '\n';
    cout << "Switch total: " << sum2 << '\n';

    cout << "Virtual time: "
         << duration<double, milli>(end1 - start1).count()
         << " ms\n";

    cout << "Switch time: "
         << duration<double, milli>(end2 - start2).count()
         << " ms\n";
}

int main() {
    benchmark(10);
    benchmark(1000);
    benchmark(1000000);
    benchmark(10000000);

    return 0;
}
