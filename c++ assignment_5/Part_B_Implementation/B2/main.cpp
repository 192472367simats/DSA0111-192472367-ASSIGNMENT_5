
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

class ClaimError : public runtime_error {
public:
    using runtime_error::runtime_error;
};

double approve(double billed, double cover) {
    if (billed < 0 || cover < 0)
        throw invalid_argument("negative amount");

    if (billed > cover)
        throw ClaimError("exceeds cover");

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

int main() {
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
            cout << "claim rejected: " << e.what() << '\n';
        }
        catch (const exception& e) {
            cout << "error: " << e.what() << '\n';
        }
    }

    // Unknown package test
    try {
        int packageCode = 999;

        if (packageCode != 101 && packageCode != 102)
            throw runtime_error("unknown package");

    }
    catch (const exception& e) {
        cout << "package error: " << e.what() << '\n';
    }

    // Invalid text input test
    try {
        string input = "abc";
        double amount = stod(input);
        cout << amount << '\n';
    }
    catch (const exception& e) {
        cout << "input error: " << e.what() << '\n';
    }

    // Memory allocation failure demonstration
    try {
        size_t size = static_cast<size_t>(-1);
        int* p = new int[size];
        delete[] p;
    }
    catch (const bad_alloc&) {
        cout << "memory allocation failed\n";
    }

    return 0;
}
