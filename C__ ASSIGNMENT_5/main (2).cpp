#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <stdexcept>
using namespace std;
using namespace chrono;

class Bill {
public:
    virtual double total() const = 0;
    virtual ~Bill() = default;
};

class OutPatientBill : public Bill {
    double consultation, tests;

public:
    OutPatientBill(double c, double t)
        : consultation(c), tests(t) {}

    double total() const override {
        return consultation + tests;
    }
};

class InPatientBill : public Bill {
    int days;
    double rate, surgery;

public:
    InPatientBill(int d, double r, double s)
        : days(d), rate(r), surgery(s) {}

    double total() const override {
        return days * rate + surgery;
    }
};

struct SimpleBill {
    int type;
    int days;
    double consultation;
    double tests;
    double rate;
    double surgery;
};

double switchTotal(const SimpleBill& b) {
    switch (b.type) {
        case 1:
            return b.consultation + b.tests;
        case 2:
            return b.days * b.rate + b.surgery;
        default:
            return 0;
    }
}

volatile double resultStore = 0;

void benchmark(int n) {
    OutPatientBill op(500, 1200);
    InPatientBill ip(3, 2500, 40000);

    vector<const Bill*> virtualBills;
    vector<SimpleBill> switchBills;

    virtualBills.reserve(n);
    switchBills.reserve(n);

    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            virtualBills.push_back(&op);
            switchBills.push_back({1, 0, 500, 1200, 0, 0});
        } else {
            virtualBills.push_back(&ip);
            switchBills.push_back({2, 3, 0, 0, 2500, 40000});
        }
    }

    double virtualSum = 0;
    auto start1 = steady_clock::now();

    for (const Bill* b : virtualBills) {
        virtualSum += b->total();
    }

    auto end1 = steady_clock::now();

    double switchSum = 0;
    auto start2 = steady_clock::now();

    for (const SimpleBill& b : switchBills) {
        switchSum += switchTotal(b);
    }

    auto end2 = steady_clock::now();

    resultStore = virtualSum + switchSum;

    double virtualTime =
        duration<double, milli>(end1 - start1).count();

    double switchTime =
        duration<double, milli>(end2 - start2).count();

    cout << "\nNumber of bills: " << n << '\n';
    cout << "Virtual total: " << virtualSum << '\n';
    cout << "Switch total: " << switchSum << '\n';
    cout << "Virtual time: " << virtualTime << " ms\n";
    cout << "Switch time: " << switchTime << " ms\n";
}

bool validateReturnCode(double amount) {
    return amount >= 0 && amount <= 1000000;
}

void validateException(double amount) {
    if (amount < 0)
        throw invalid_argument("negative amount");

    if (amount > 1000000)
        throw out_of_range("amount too large");
}

void exceptionBenchmark(int n, int invalidPercent) {
    vector<double> amounts(n);

    int invalidCount = n * invalidPercent / 100;

    for (int i = 0; i < n; ++i) {
        amounts[i] = (i < invalidCount) ? -10 : 500;
    }

    int returnErrors = 0;
    auto start1 = steady_clock::now();

    for (double amount : amounts) {
        if (!validateReturnCode(amount)) {
            ++returnErrors;
        }
    }

    auto end1 = steady_clock::now();

    int exceptionErrors = 0;
    auto start2 = steady_clock::now();

    for (double amount : amounts) {
        try {
            validateException(amount);
        }
        catch (const exception&) {
            ++exceptionErrors;
        }
    }

    auto end2 = steady_clock::now();

    cout << "\nInvalid percentage: "
         << invalidPercent << "%\n";

    cout << "Return-code errors: " << returnErrors << '\n';
    cout << "Exception errors: " << exceptionErrors << '\n';

    cout << "Return-code time: "
         << duration<double, milli>(end1 - start1).count()
         << " ms\n";

    cout << "Exception time: "
         << duration<double, milli>(end2 - start2).count()
         << " ms\n";
}

int main() {
    cout << fixed << setprecision(3);

    cout << "BILLING PERFORMANCE COMPARISON\n";
    cout << "==============================\n";

    benchmark(1000);
    benchmark(1000000);
    benchmark(10000000);

    cout << "\nEXCEPTION PERFORMANCE COMPARISON\n";
    cout << "================================\n";

    exceptionBenchmark(1000000, 0);
    exceptionBenchmark(1000000, 1);
    exceptionBenchmark(1000000, 50);

    return 0;
}