
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
using namespace std;

class Bill {
protected:
    string patient;

public:
    Bill(string p) : patient(p) {}
    virtual ~Bill() = default;

    virtual double total() const = 0;
    virtual string type() const = 0;

    string getPatient() const {
        return patient;
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

// New bill type
class PharmacyBill : public Bill {
    double medicineCost;

public:
    PharmacyBill(string p, double cost)
        : Bill(p), medicineCost(cost) {}

    double total() const override {
        return medicineCost;
    }

    string type() const override {
        return "Pharmacy";
    }
};

class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
};

// New payer type
class CorporatePayer : public Payer {
    double percentage;

public:
    CorporatePayer(double p) : percentage(p) {
        if (p < 0 || p > 100)
            throw invalid_argument("Invalid percentage");
    }

    double amountCovered(const Bill& b) const override {
        return b.total() * percentage / 100;
    }
};

int main() {
    try {
        vector<unique_ptr<Bill>> bills;

        bills.push_back(
            make_unique<OutPatientBill>("Ravi", 500, 1200));

        bills.push_back(
            make_unique<PharmacyBill>("Meena", 2500));

        CorporatePayer corporate(80);

        for (const auto& b : bills) {
            double total = b->total();
            double covered = corporate.amountCovered(*b);

            cout << b->type() << " - "
                 << b->getPatient() << '\n';

            cout << "Bill Amount: " << total << '\n';
            cout << "Corporate Coverage: " << covered << '\n';
            cout << "Patient Payment: "
                 << total - covered << "\n\n";
        }

        try {
            throw runtime_error("Test exception");
        }
        catch (const exception& e) {
            cout << "Exception caught: " << e.what() << '\n';
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << '\n';
    }

    return 0;
}
