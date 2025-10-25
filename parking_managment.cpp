#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

// Base class
class Vehicle {
protected:
    string number;
    int days;

public:
    virtual void inputDetails() {
        cout << "Enter vehicle number: ";
        cin >> number;
        cout << "Enter number of days parked: ";
        cin >> days;
    }

    string getNumber() {
        return number;
    }

    int getDays() {
        return days;
    }

    virtual void calculateFee() = 0;  // Pure virtual
    virtual string getType() = 0;
};

// Derived classes
class Bike : public Vehicle {
public:
    void calculateFee() override {
        int fee = days * 10;
        cout << "Bike [" << number << "] Fee: ₹" << fee << endl;
    }

    string getType() override {
        return "Bike";
    }
};

class Car : public Vehicle {
public:
    void calculateFee() override {
        int fee = days * 50;
        cout << "Car [" << number << "] Fee: ₹" << fee << endl;
    }

    string getType() override {
        return "Car";
    }
};

class Truck : public Vehicle {
public:
    void calculateFee() override {
        int fee = days * 100;
        cout << "Truck [" << number << "] Fee: ₹" << fee << endl;
    }

    string getType() override {
        return "Truck";
    }
};

// Parking Lot Manager
class ParkingLot {
private:
    const string filename = "parked_vehicles.txt";

public:
    void parkVehicle() {
        int choice;
        cout << "\n1. Bike\n2. Car\n3. Truck\nEnter vehicle type: ";
        cin >> choice;

        Vehicle* v = nullptr;
        if (choice == 1) v = new Bike();
        else if (choice == 2) v = new Car();
        else if (choice == 3) v = new Truck();
        else {
            cout << "Invalid choice!\n";
            return;
        }

        v->inputDetails();

        ofstream fout(filename, ios::app);
        fout << v->getType() << "," << v->getNumber() << "," << v->getDays() << endl;
        fout.close();

        cout << v->getType() << " parked successfully.\n";
        delete v;
    }

    void exitVehicle() {
        string number;
        cout << "Enter vehicle number to exit: ";
        cin >> number;

        ifstream fin(filename);
        ofstream temp("temp.txt");

        string line;
        bool found = false;

        while (getline(fin, line)) {
            stringstream ss(line);
            string type, num;
            int days;
            getline(ss, type, ',');
            getline(ss, num, ',');
            ss >> days;

            if (num == number) {
                found = true;
                Vehicle* v = nullptr;
                if (type == "Bike") v = new Bike();
                else if (type == "Car") v = new Car();
                else if (type == "Truck") v = new Truck();

                if (v) {
                    v->inputDetails(); // Just to update 'days' if needed
                    v->calculateFee();
                    delete v;
                }
            } else {
                temp << line << endl;
            }
        }

        fin.close();
        temp.close();

        remove(filename.c_str());
        rename("temp.txt", filename.c_str());

        if (found) cout << "Vehicle exited successfully.\n";
        else cout << "Vehicle not found!\n";
    }

    void showParkedVehicles() {
        ifstream fin(filename);
        string line;
        cout << "\n--- Parked Vehicles ---\n";
        bool empty = true;
        while (getline(fin, line)) {
            empty = false;
            stringstream ss(line);
            string type, num;
            int days;
            getline(ss, type, ',');
            getline(ss, num, ',');
            ss >> days;
            cout << type << " - " << num << " (" << days << " days)\n";
        }
        if (empty) cout << "No vehicles parked.\n";
        fin.close();
    }
};

// Main
int main() {
    ParkingLot lot;
    int choice;

    while (true) {
        cout << "\n--- Parking Lot Menu ---\n";
        cout << "1. Park Vehicle\n2. Exit Vehicle (Calculate Fee)\n3. Show All Parked Vehicles\n4. Exit Program\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: lot.parkVehicle(); break;
            case 2: lot.exitVehicle(); break;
            case 3: lot.showParkedVehicles(); break;
            case 4: return 0;
            default: cout << "Invalid choice!\n";
        }
    }

    return 0;
}
