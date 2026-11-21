
#include <iostream>
#include <stdexcept>
using namespace std;

class BillingError : public runtime_error {
public:
    using runtime_error::runtime_error;
};

class InvalidCharge : public BillingError {
public:
    using BillingError::BillingError;
};

class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};

class UnknownPackage : public BillingError {
public:
    using BillingError::BillingError;
};

class PatientNotFound : public BillingError {
public:
    using BillingError::BillingError;
};

void validateCharge(double amount) {
    if (amount < 0)
        throw InvalidCharge("Negative charge");
}

void validateClaim(double bill, double cover) {
    if (bill > cover)
        throw ClaimError("Claim exceeds cover");
}

void validatePackage(int code) {
    if (code != 101 && code != 102)
        throw UnknownPackage("Unknown package");
}

int main() {
    try {
        validateCharge(-100);
        validateClaim(5000, 3000);
        validatePackage(999);
    }
    catch (const BillingError& e) {
        cout << "Billing Error: " << e.what() << '\n';
    }
    catch (const bad_alloc& e) {
        cout << "Memory Error: " << e.what() << '\n';
    }

    return 0;
}
