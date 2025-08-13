#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    double balance; 

protected:
    int accountNumber;
    string accountHolder;

public:
    BankAccount(int accNo, string name, double bal) {
        accountNumber = accNo;
        accountHolder = name;
        balance = 0;
        deposit(bal);
    }

    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited Rs." << amount << " successfully." << endl;
        } else {
            cout << "Invalid deposit amount" << endl;
        }
    }

    virtual bool withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
            cout << "Withdrew Rs." << amount << " successfully." << endl;
            return true;
        } else {
            cout << "Insufficient balance or invalid amount!" << endl;
            return false;
        }
    }

    double getBalance() const { return balance; }

protected:
    void setBalance(double newBal) { balance = newBal; }

public:
    virtual void displayAccountInfo() const = 0;
    virtual void calculateInterest() = 0;       

    int getAccountNumber() const { return accountNumber; }

    virtual ~BankAccount() {}
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(int accNo, string name, double bal, double rate)
        : BankAccount(accNo, name, bal), interestRate(rate) {
        cout << "Savings Account created successfully." << endl;
    }

    void calculateInterest() override {
        double interest = getBalance() * interestRate / 100;
        setBalance(getBalance() + interest);
        cout << "Savings Interest Rs." << interest << " added." << endl;
    }

    void displayAccountInfo() const override {
        cout << endl;
        cout << "--- Savings Account ---" << endl;
        cout << "Account No: " << accountNumber << endl;
        cout << "Holder: " << accountHolder << endl;
        cout << "Balance: Rs." << getBalance() << endl;
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};

class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(int accNo, string name, double bal, double limit)
        : BankAccount(accNo, name, bal), overdraftLimit(limit) {
        cout << "Checking Account created successfully." << endl;
    }

    bool withdraw(double amount) override {
        if (amount > 0 && (getBalance() + overdraftLimit) >= amount) {
            double newBalance = getBalance() - amount;
            setBalance(newBalance);
            if (newBalance < 0) {
                cout << "Overdraft used: Rs." << -newBalance << endl;
            }
            cout << "Withdrew Rs." << amount << " successfully." << endl;
            return true;
        } else {
            cout << "Withdrawal exceeds overdraft limit!" << endl;
            return false;
        }
    }

    void calculateInterest() override {
        cout << "Checking account does not earn interest." << endl;
    }

    void displayAccountInfo() const override {
        cout << endl;
        cout << "--- Checking Account ---" << endl;
        cout << "Account No: " << accountNumber << endl;
        cout << "Holder: " << accountHolder << endl;
        cout << "Balance: Rs." << getBalance() << endl;
        cout << "Overdraft Limit: Rs." << overdraftLimit << endl;
    }
};

class FixedDepositAccount : public BankAccount {
private:
    int termMonths;
    double interestRate;

public:
    FixedDepositAccount(int accNo, string name, double bal, int term, double rate)
        : BankAccount(accNo, name, bal), termMonths(term), interestRate(rate) {
        cout << "Fixed Deposit Account created successfully." << endl;
    }

    void calculateInterest() override {
        double interest = getBalance() * interestRate * termMonths / (12 * 100);
        setBalance(getBalance() + interest);
        cout << "FD Interest Rs." << interest << " added for " << termMonths << " months." << endl;
    }

    void displayAccountInfo() const override {
        cout << endl;
        cout << "--- Fixed Deposit Account ---" << endl;
        cout << "Account No: " << accountNumber << endl;
        cout << "Holder: " << accountHolder << endl;
        cout << "Balance: Rs." << getBalance() << endl;
        cout << "Term: " << termMonths << " months" << endl;
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};


int main() {
    const int MAX_ACCOUNTS = 10;
    BankAccount* accounts[MAX_ACCOUNTS];
    int accountCount = 0;
    int choice;

    do {
        cout << endl;
        cout << "---- BANKING SYSTEM MENU ----" << endl;
        cout << "1. Create Savings Account" << endl;
        cout << "2. Create Checking Account" << endl;
        cout << "3. Create Fixed Deposit Account" << endl;
        cout << "4. Deposit" << endl;
        cout << "5. Withdraw" << endl;
        cout << "6. Display All Accounts" << endl;
        cout << "7. Calculate Interest" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int accNo;
            string name;
            double bal, rate;
            cout << "Enter Account Number: ";
            cin >> accNo;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Initial Balance: ";
            cin >> bal;
            cout << "Enter Interest Rate (%): ";
            cin >> rate;
            accounts[accountCount++] = new SavingsAccount(accNo, name, bal, rate);
            break;
        }
        case 2: {
            int accNo;
            string name;
            double bal, limit;
            cout << "Enter Account Number: ";
            cin >> accNo;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Initial Balance: ";
            cin >> bal;
            cout << "Enter Overdraft Limit: ";
            cin >> limit;
            accounts[accountCount++] = new CheckingAccount(accNo, name, bal, limit);
            break;
        }
        case 3: {
            int accNo, term;
            string name;
            double bal, rate;
            cout << "Enter Account Number: ";
            cin >> accNo;
            cin.ignore();
            cout << "Enter Name: ";
            getline(cin, name);
            cout << "Enter Initial Balance: ";
            cin >> bal;
            cout << "Enter Term (months): ";
            cin >> term;
            cout << "Enter Interest Rate (%): ";
            cin >> rate;
            accounts[accountCount++] = new FixedDepositAccount(accNo, name, bal, term, rate);
            break;
        }
        case 4: {
            int accNo;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter Deposit Amount: ";
            cin >> amount;
            bool found = false;
            for (int i = 0; i < accountCount; i++) {
                if (accounts[i]->getAccountNumber() == accNo) {
                    accounts[i]->deposit(amount);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found!" << endl;
            break;
        }
        case 5: {
            int accNo;
            double amount;
            cout << "Enter Account Number: ";
            cin >> accNo;
            cout << "Enter Withdrawal Amount: ";
            cin >> amount;
            bool found = false;
            for (int i = 0; i < accountCount; i++) {
                if (accounts[i]->getAccountNumber() == accNo) {
                    accounts[i]->withdraw(amount);
                    found = true;
                    break;
                }
            }
            if (!found) cout << "Account not found!" << endl;
            break;
        }
        case 6: {
            for (int i = 0; i < accountCount; i++) {
                accounts[i]->displayAccountInfo();
            }
            break;
        }
        case 7: {
            for (int i = 0; i < accountCount; i++) {
                accounts[i]->calculateInterest();
            }
            break;
        }
        case 8:
            cout << "Thank you, visit again!" << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 8);

    for (int i = 0; i < accountCount; i++) {
        delete accounts[i];
    }

    return 0;
}
