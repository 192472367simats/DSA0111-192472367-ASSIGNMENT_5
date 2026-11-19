#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
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
        cout << type() << " | " << patient
             << " | Rs. " << total() << '\n';
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
        return "Outpatient";
    }
};

class InPatient : public Bill {
    int days;
    double dailyRate, surgery;

public:
    InPatient(string p, int d, double r, double s)
        : Bill(p), days(d), dailyRate(r), surgery(s) {}

    double total() const override {
        return days * dailyRate + surgery;
    }

    string type() const override {
        return "Inpatient";
    }
};

class PharmacyBill : public Bill {
    double medicineCost;
    double serviceCharge;

public:
    PharmacyBill(string p, double medicine, double service)
        : Bill(p), medicineCost(medicine),
          serviceCharge(service) {}

    double total() const override {
        return medicineCost + serviceCharge;
    }

    string type() const override {
        return "Pharmacy";
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
    double limit;

public:
    explicit Insurance(double l) : limit(l) {}

    double amountCovered(const Bill& bill) const override {
        return min(bill.total(), limit);
    }

    string name() const override {
        return "Insurance";
    }
};

// New payer added without changing the existing payer classes.
class CorporatePayer : public Payer {
    double discountPercent;

public:
    explicit CorporatePayer(double discount)
        : discountPercent(discount) {}

    double amountCovered(const Bill& bill) const override {
        return bill.total() * discountPercent / 100.0;
    }

    string name() const override {
        return "Corporate Payer";
    }
};

int main() {
    cout << fixed << setprecision(2);

    vector<unique_ptr<Bill>> bills;

    bills.push_back(
        make_unique<OutPatient>("Ravi", 500, 1200));

    bills.push_back(
        make_unique<InPatient>("Meena", 3, 2500, 40000));

    // New bill type
    bills.push_back(
        make_unique<PharmacyBill>("Arun", 3500, 200));

    SelfPay self;
    Insurance insurance(30000);
    CorporatePayer corporate(70);

    vector<Payer*> payers = {
        &self, &insurance, &corporate
    };

    cout << "HOSPITAL BILLING SYSTEM\n";
    cout << "=======================\n";

    for (const auto& bill : bills) {
        bill->print();

        for (const Payer* payer : payers) {
            double covered = payer->amountCovered(*bill);
            double balance = bill->total() - covered;

            cout << payer->name()
                 << " covers Rs. " << covered
                 << " | Balance Rs. " << balance << '\n';
        }

        cout << "-----------------------\n";
    }

    // Demonstrate exception handling and recovery.
    cout << "\nEXCEPTION SAFETY TEST\n";

    try {
        throw runtime_error("Test billing error");
    }
    catch (const exception& e) {
        cout << "Error safely handled: "
             << e.what() << '\n';
    }

    cout << "Program completed successfully.\n";

    return 0;
}