
#include <iostream>
#include <string>
using namespace std;

class Bill {
public:
    virtual ~Bill() = default;
    virtual double total() const = 0;
};

class OutPatientBill : public Bill {
    double amount;

public:
    explicit OutPatientBill(double a) : amount(a) {}

    double total() const override {
        return amount;
    }
};

class Payer {
public:
    virtual ~Payer() = default;
    virtual double amountCovered(const Bill& b) const = 0;
};

class SelfPay : public Payer {
public:
    double amountCovered(const Bill&) const override {
        return 0;
    }
};

class Insurance : public Payer {
    double coverLimit;
    double copayPercent;

public:
    Insurance(double limit, double copay)
        : coverLimit(limit), copayPercent(copay) {}

    double amountCovered(const Bill& b) const override {
        double eligible = min(b.total(), coverLimit);
        return eligible * (100 - copayPercent) / 100;
    }
};

class GovernmentScheme : public Payer {
    double packageRate;

public:
    explicit GovernmentScheme(double rate)
        : packageRate(rate) {}

    double amountCovered(const Bill& b) const override {
        return min(b.total(), packageRate);
    }
};

class CorporateTieUp : public Payer {
    double percentage;

public:
    explicit CorporateTieUp(double p) : percentage(p) {}

    double amountCovered(const Bill& b) const override {
        return b.total() * percentage / 100;
    }
};

int main() {
    OutPatientBill bill(5000);

    SelfPay self;
    Insurance insurance(4000, 20);
    GovernmentScheme government(3000);
    CorporateTieUp corporate(80);

    cout << "Bill Amount: " << bill.total() << '\n';
    cout << "Self Pay Coverage: "
         << self.amountCovered(bill) << '\n';
    cout << "Insurance Coverage: "
         << insurance.amountCovered(bill) << '\n';
    cout << "Government Coverage: "
         << government.amountCovered(bill) << '\n';
    cout << "Corporate Coverage: "
         << corporate.amountCovered(bill) << '\n';

    return 0;
}
