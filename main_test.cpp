#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <queue>
#include <map>
#include <iomanip>

using namespace std;

const char SHORT_ROUTE = 'S';
const char LONG_ROUTE = 'L';
const char CITY_ROUTE = 'C';
const int PASSENGERS_PER_TAXI = 5;

class passengerType {
public:
    // Friend function declaration.
    friend ostream& operator<<(ostream& os, const passengerType& p);

    // Default constructor
    passengerType(): arrivalTime(0), passengerRoute('N'), boardingTime(0) {}

    // Constructor
    passengerType(int aTime, char route, int bTime) {
        arrivalTime = aTime;
        passengerRoute = route;
        boardingTime = bTime;
    }

    // Function to set the time it will take for a passenger to board.
    void setBoardingTime(int bTime) {
        boardingTime = bTime;
    }

    /* // Function to set the time the passenger arrives.
    void setArrivalTime( int aTime); */

    // Function to set the route the passenger will take (S, L. or C)
    void setPassengerRoute(char route) {
        passengerRoute = route;
    }

    // Function to return the boarding time of a passenger.
    int getBoardingTime() const {
        return boardingTime;
    }

    // Function to return the arrival time of the passenger.
    int getArrivalTime() const {
        return arrivalTime;
    }

    // Function to return the route a passenger will take.
    char getPassengerRoute() const{
        return passengerRoute;
    }

    // Function to decrease the boarding time.
    void decreaseBoardingTime() {
        boardingTime--;
    }
    
private:
    int arrivalTime;
    char passengerRoute;
    int boardingTime;
    
};
