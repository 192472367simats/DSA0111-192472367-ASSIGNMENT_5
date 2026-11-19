
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <limits>
#include <new>
using namespace std;

class BillingError : public runtime_error {
public:
    using runtime_error::runtime_error;
};

class ClaimError : public BillingError {
public:
    using BillingError::BillingError;
};

class ResourceGuard {
    string name;

public:
    explicit ResourceGuard(string n) : name(n) {
        cout << name << " created\n";
    }

    ~ResourceGuard() {
        cout << name << " destroyed\n";
    }
};

double approve(double billed, double cover) {
    if (billed < 0 || cover < 0) {
        throw invalid_argument("negative amount");
    }

    if (billed > cover) {
        throw ClaimError("exceeds cover");
    }

    return billed;
}

double settle(double billed, double cover) {
    try {
        return approve(billed, cover);
    }
    catch (const ClaimError& e) {
        cout << "audit: " << e.what() << '\n';
        throw;
    }
}

double governmentPackage(int code) {
    if (code == 101)
        return 10000;

    if (code == 102)
        return 25000;

    throw BillingError("unknown package");
}

void validateCharge(double amount) {
    if (amount < 0)
        throw invalid_argument("negative charge");

    cout << "Charge is valid\n";
}

double readAmount(const string& input) {
    return stod(input);
}

// Simulated memory allocation failure
void testAllocationFailure() {
    cout << "Testing allocation failure\n";

    throw bad_alloc();
}

void runTests() {
    ResourceGuard guard("Billing resource");

    try {
        validateCharge(-100);
    }
    catch (const exception& e) {
        cout << "Negative charge caught: "
             << e.what() << '\n';
    }

    try {
        settle(60000, 50000);
    }
    catch (const ClaimError& e) {
        cout << "Claim rejected: "
             << e.what() << '\n';
    }

    try {
        cout << governmentPackage(999) << '\n';
    }
    catch (const BillingError& e) {
        cout << "Package error: "
             << e.what() << '\n';
    }

    try {
        double value = readAmount("hospital");
        cout << value << '\n';
    }
    catch (const invalid_argument& e) {
        cout << "Invalid number: "
             << e.what() << '\n';
    }

    try {
        testAllocationFailure();
    }
    catch (const bad_alloc& e) {
        cout << "Memory allocation failed: "
             << e.what() << '\n';
    }
}

int main() {
    cout << "HOSPITAL EXCEPTION TESTING\n";
    cout << "==========================\n";

    try {
        runTests();
    }
    catch (...) {
        cout << "Top-level handler caught an error\n";
    }

    cout << "Program continues safely\n";

    // Complete Code exercise:
    // Successful claim, rejected claim and negative amount.
    cout << "\nCLAIM APPROVAL TEST\n";

    double cases[][2] = {
        {40000, 50000},
        {60000, 50000},
        {-1, 50000}
    };

    for (auto& c : cases) {
        try {
            double paid = settle(c[0], c[1]);
            cout << "approved " << paid << '\n';
        }
        catch (const ClaimError& e) {
            cout << "claim rejected: "
                 << e.what() << '\n';
        }
        catch (const exception& e) {
            cout << "error: "
                 << e.what() << '\n';
        }
    }

    return 0;
}
