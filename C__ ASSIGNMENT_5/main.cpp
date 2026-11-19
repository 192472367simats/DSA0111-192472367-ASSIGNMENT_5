#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
using namespace std;

class Bill {
protected:
    string patient;

public:
    explicit Bill(string p) : patient(p) {}

    virtual double total() const = 0;
    virtual string type() const = 0;

    virtual void print() const {
        cout << type() << " " << patient
             << ": " << total() << '\n';
    }

    virtual ~Bill() = default;
};

class OutPatient : public Bill {
    double consultation, tests;

public:
    OutPatient(string p, double c, double t)
        : Bill(p), consultation(c), tests(t) {}

    double total() const override {
        return consultation + tests;
    }

    string type() const override {
        return "OP";
    }
};

class InPatient : public Bill {
    int days;
    double perDay, surgery, medicines;

public:
    InPatient(string p, int d, double rate,
              double s, double m)
        : Bill(p), days(d), perDay(rate),
          surgery(s), medicines(m) {}

    double total() const override {
        return days * perDay + surgery + medicines;
    }

    string type() const override {
        return "IP";
    }
};

class DayCare : public Bill {
    double packagePrice;

public:
    DayCare(string p, double price)
        : Bill(p), packagePrice(price) {}

    double total() const override {
        return packagePrice;
    }

    string type() const override {
        return "DayCare";
    }
};

class Emergency : public Bill {
    double baseCharge, hourlyCharge;
    int hours;

public:
    Emergency(string p, double base, double hourly, int h)
        : Bill(p), baseCharge(base),
          hourlyCharge(hourly), hours(h) {}

    double total() const override {
        return baseCharge + hourlyCharge * hours;
    }

    string type() const override {
        return "Emergency";
    }
};

class Payer {
public:
    virtual double amountCovered(const Bill& bill) const = 0;
    virtual string name() const = 0;
    virtual ~Payer() = default;
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill& bill) const override {
        return bill.total();
    }

    string name() const override {
        return "Self Pay";
    }
};

class Insurance : public Payer {
    double coverLimit;
    double coPaymentPercent;

public:
    Insurance(double limit, double copay)
        : coverLimit(limit), coPaymentPercent(copay) {}

    double amountCovered(const Bill& bill) const override {
        double eligible = min(bill.total(), coverLimit);
        return eligible * (100.0 - coPaymentPercent) / 100.0;
    }

    string name() const override {
        return "Insurance";
    }
};

class GovernmentScheme : public Payer {
public:
    double amountCovered(const Bill& bill) const override {
        if (bill.type() == "OP")
            return 1500;
        if (bill.type() == "IP")
            return 40000;
        if (bill.type() == "DayCare")
            return 20000;
        if (bill.type() == "Emergency")
            return 10000;

        throw runtime_error("Unknown government package");
    }

    string name() const override {
        return "Government Scheme";
    }
};

int main() {
    cout << fixed << setprecision(2);

    vector<unique_ptr<Bill>> bills;

    bills.push_back(make_unique<OutPatient>("Ravi", 500, 1200));
    bills.push_back(make_unique<InPatient>(
        "Meena", 3, 2500, 40000, 3000));
    bills.push_back(make_unique<DayCare>("Arun", 25000));
    bills.push_back(make_unique<Emergency>(
        "Priya", 2000, 1500, 3));

    SelfPay self;
    Insurance insurance(50000, 20);
    GovernmentScheme government;

    vector<Payer*> payers = {
        &self, &insurance, &government
    };

    cout << "HOSPITAL BILLING SYSTEM\n";
    cout << "=======================\n";

    for (const auto& bill : bills) {
        bill->print();

        for (const Payer* payer : payers) {
            double covered = payer->amountCovered(*bill);
            double balance = bill->total() - covered;

            cout << payer->name()
                 << " | Covered: Rs. " << covered
                 << " | Patient Balance: Rs. " << balance
                 << '\n';
        }

        cout << "-----------------------\n";
    }

    return 0;
}