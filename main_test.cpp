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

class TaxiType {
public:
    // Default constructor.
    TaxiType(): /*routeType('N'),*/ boardingStatus("available"), taxiCapacity(5) {}

    // Function to set the current passenger boarding
    void setCurrentPassenger(passengerType cPassenger) {
        currentPassengerBoarding = cPassenger;
    }

    // Function to set the route of the taxi
    void setRoute(char tRoute) {
        routeType = tRoute;
    }

    // Function to determine if a passenger is boarding.
    bool isPassengerBoarding() const {
        return (boardingStatus == "unavailable");
    }

    // Function to set boarding status to busy.
    void setToUnavailable() {
        boardingStatus = "unavailable";
    }

    // Function to set boarding status to free.
    void setToAvailable() {
        boardingStatus = "available";
    }

    // Fucntion to set capacity
    void setCapacity(int capacity) {
        taxiCapacity = capacity;
    }

    // Function to get the info of the current passenger.
    passengerType& getCurrentPassenger() {
        return currentPassengerBoarding;
    }

    // Function to get the route of the taxi.
    char getRoute() const {
        return routeType;
    }

    // Function to return the boarding status of the taxi.
    string getBoardingStatus() const {
        return boardingStatus;
    }

    // Function to return taxi capacity.
    int getCapacity() const {
        return taxiCapacity;
    }

    // Decrease the taxi capacity as passengers board.
    void decreaseTaxiCapacity() {
        taxiCapacity--;
    }

    // Deconstructor
    //~TaxiType();

private:
    passengerType currentPassengerBoarding;
    char routeType;
    string boardingStatus;
    int taxiCapacity;
};
