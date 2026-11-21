
#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

class Bill {
protected:
    string patient;

public:
    Bill(string p) : patient(p) {}
    virtual ~Bill() = default;

    virtual double total() const = 0;
    virtual string type() const = 0;

    virtual void print() const {
        cout << type() << " " << patient
             << ": " << total() << '\n';
    }
};

class OutPatientBill : public Bill {
    double consultation, tests;

public:
    OutPatientBill(string p, double c, double t)
        : Bill(p), consultation(c), tests(t) {}

    double total() const override {
        return consultation + tests;
    }

    string type() const override {
        return "Outpatient";
    }
};

class InPatientBill : public Bill {
    int days;
    double perDay, procedures;

public:
    InPatientBill(string p, int d, double rate, double proc)
        : Bill(p), days(d), perDay(rate), procedures(proc) {}

    double total() const override {
        return days * perDay + procedures;
    }

    string type() const override {
        return "Inpatient";
    }
};

class DayCareBill : public Bill {
    double packagePrice;

public:
    DayCareBill(string p, double price)
        : Bill(p), packagePrice(price) {}

    double total() const override {
        return packagePrice;
    }

    string type() const override {
        return "Day Care";
    }
};

class EmergencyBill : public Bill {
    double baseCharge, timeCharge;

public:
    EmergencyBill(string p, double base, double time)
        : Bill(p), baseCharge(base), timeCharge(time) {}

    double total() const override {
        return baseCharge + timeCharge;
    }

    string type() const override {
        return "Emergency";
    }
};

int main() {
    vector<unique_ptr<Bill>> bills;

    bills.push_back(make_unique<OutPatientBill>("Ravi", 500, 1200));
    bills.push_back(make_unique<InPatientBill>("Meena", 3, 2500, 40000));
    bills.push_back(make_unique<DayCareBill>("Anu", 18000));
    bills.push_back(make_unique<EmergencyBill>("Kiran", 3000, 2000));

    double grandTotal = 0;

    for (const auto& b : bills) {
        b->print();
        grandTotal += b->total();
    }

    cout << "Daily Total: " << grandTotal << '\n';

    return 0;
}
