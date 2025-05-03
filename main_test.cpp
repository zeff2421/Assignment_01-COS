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
    TaxiType(): boardingStatus("available"), taxiCapacity(5) {}

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

private:
    passengerType currentPassengerBoarding;
    char routeType;
    string boardingStatus;
    int taxiCapacity;
};

// Function to overload the insertion operator
ostream& operator<<(ostream& os, const passengerType& p) {
    os << "Arrival Time: " << p.arrivalTime
       << ", Route: " << p.passengerRoute
       << ", Boarding Time: " << p.boardingTime;
    return os;
}

void initializeTaxiQueues(map<char, queue<TaxiType>>& taxiQueues);

// Print output header
void printHeader();

// print the simulation rows
void printRows(int time, map<char, queue<TaxiType>>& taxiQueues, map<char, queue<passengerType>>& waitingQueues);

// Function to format the each taxi's data.
string formatEachTaxi(queue<TaxiType>& taxiQueues);

// Function to format waiting queue.
string formatWaitingQueue(queue<passengerType>& waitingQueues);

int main() {
    ifstream inputFile;
    string line;
    char route;
    int timeInstance, boardingTime, location;
    map<int, queue<passengerType>> next;

    // Initialize empty queues of taxis
    map<char, queue<TaxiType>> taxiQueues;
    initializeTaxiQueues(taxiQueues);

    // Initialize the waiting queues
    map<char, queue<passengerType>> waitingQueues;
    waitingQueues[SHORT_ROUTE] = queue<passengerType>();
    waitingQueues[LONG_ROUTE] = queue<passengerType>();
    waitingQueues[CITY_ROUTE] = queue<passengerType>();

    inputFile.open("taxiData.txt");
    if (!inputFile.is_open()) {
        cerr << "Couldn't open file. Make sure it's the right location!!";
    }

    int lastTimeInstance = 0;

    while (getline(inputFile, line)) {
        if (line.empty()) {
            continue;
        }
        location = line.find(',');
        timeInstance = stoi(line.substr(0, location));
        line = line.substr(location + 1, line.length());

        location = line.find(',');
        route = line.substr(0, location)[0];
        line = line.substr(location + 1, line.length());

        boardingTime = stoi(line);

        cout << "Time instance: " << timeInstance << endl
             << " Route: " << route << endl
             << " Boarding Time: " << boardingTime << endl << endl;

        passengerType passenger(timeInstance, route, boardingTime);
        next[timeInstance].push(passenger);
        lastTimeInstance = timeInstance;
    }

    // Close file after reading
    inputFile.close();

    // Print the header
    printHeader();

    for (int currentTime = 0; currentTime <= lastTimeInstance; currentTime++) {
        // Do house keeping for all taxis.
        for (auto& pair : taxiQueues) {
            char route = pair.first;
            queue<TaxiType>& taxiQueue = pair.second;

            // Ensures the taxi does exist.
            if (!taxiQueue.empty()) {
                TaxiType& taxi = taxiQueue.front();
                queue<passengerType>& waitingQueue = waitingQueues[route];

                // Checks if taxi is available
                if (taxi.getBoardingStatus() == "available") {
                    if (!waitingQueue.empty()) {
                        passengerType nextPassenger = waitingQueue.front();
                        waitingQueue.pop();
                        if (taxi.getCapacity() == 0) {
                            taxi.setCapacity(PASSENGERS_PER_TAXI);
                        }
                        taxi.setCurrentPassenger(nextPassenger);
                        taxi.setToUnavailable();
                    }
                }
                // Checks if taxi has a passenger currently boarding.
                else if (taxi.getBoardingStatus() == "unavailable") {
                    passengerType& currentPassenger = taxi.getCurrentPassenger();
                    currentPassenger.decreaseBoardingTime();

                    if (currentPassenger.getBoardingTime() == 0) {
                        taxi.setToAvailable();
                        taxi.decreaseTaxiCapacity();

                        if (!waitingQueue.empty()) {
                            passengerType nextPassenger = waitingQueue.front();
                            waitingQueue.pop();
                            if (taxi.getCapacity() == 0) {
                                taxi.setCapacity(PASSENGERS_PER_TAXI);
                            }
                            taxi.setCurrentPassenger(nextPassenger);
                            taxi.setToUnavailable();
                        }
                    }
                }
            }
        }
        // Process each arriving passenger
        // Check if there is a passenger that arrives at currentTime
        if (next.find(currentTime) != next.end()) {
            queue<passengerType>& queueOfPassengers = next[currentTime];

            while (!queueOfPassengers.empty()) {
                passengerType p = queueOfPassengers.front();
                queueOfPassengers.pop();

                char route = p.getPassengerRoute();
                queue<TaxiType>& taxiForThisRoute = taxiQueues[route];
                queue<passengerType>& waitingQueueForThisRoute = waitingQueues[route];

                // Ensures that corresponding taxi queue exist.
                if (!taxiForThisRoute.empty()) {
                    TaxiType& taxi = taxiForThisRoute.front();

                    if (taxi.getBoardingStatus() == "unavailable") {
                        waitingQueueForThisRoute.push(p);
                    } else if (taxi.getBoardingStatus() == "available") {
                        if (taxi.getCapacity() == 0) {
                            taxi.setCapacity(PASSENGERS_PER_TAXI);
                        }
                        taxi.setCurrentPassenger(p);
                        taxi.setToUnavailable();
                    }
                }
            }
        }

        // Ensures there are no mid-boarding passengers and no passenger waiting before finishing simulation
        if (currentTime == lastTimeInstance) {
            for (auto& pair : taxiQueues) {
                char route = pair.first;
                queue<TaxiType>& taxiQueue = pair.second;
    
                if (!taxiQueue.empty()) {
                    TaxiType& taxi = taxiQueue.front();
                    queue<passengerType>& waitingQueue = waitingQueues[route];
                    
                    if (taxi.getBoardingStatus() == "unavailable" || !waitingQueue.empty()) {
                        lastTimeInstance++;
                        break;
                    }
                }
            }
        }

        printRows(currentTime, taxiQueues, waitingQueues);
    }

    return 0;
}

void initializeTaxiQueues(map<char, queue<TaxiType>>& taxiQueues) {
    const int NUM_TAXIS_PER_ROUTE = 1;

    for (char route : {'S', 'L', 'C'}) {
        for (int i = 0; i < NUM_TAXIS_PER_ROUTE; ++i) {
            TaxiType taxi;
            taxi.setRoute(route);
            taxi.setCapacity(PASSENGERS_PER_TAXI);
            taxi.setToAvailable();
            taxiQueues[route].push(taxi);
        }
    }
}

void printHeader() {
    cout << left
         << setw(6) << "T"
         << setw(12) << "next"
         << setw(10) << "S"
         << setw(10) << "L"
         << setw(10) << "C"
         << setw(12) << "WQS"
         << setw(12) << "WQL"
         << setw(12) << "WQC"
         << setw(8) << "CS"
         << setw(8) << "CL"
         << setw(8) << "CC"
         << endl;
    cout << string(108, '-') << endl;
}

void printRows(int time, map<char, queue<TaxiType>>& taxiQueues, map<char, queue<passengerType>>& waitingQueues) {

    // The correct format for boarding passengers.
    string info_S = formatEachTaxi(taxiQueues[SHORT_ROUTE]);
    string info_L = formatEachTaxi(taxiQueues[LONG_ROUTE]);
    string info_C = formatEachTaxi(taxiQueues[CITY_ROUTE]);

    // Correct format of each waiting queue.
    string waitingQueue_S = formatWaitingQueue(waitingQueues[SHORT_ROUTE]);
    string waitingQueue_L = formatWaitingQueue(waitingQueues[LONG_ROUTE]);
    string waitingQueue_C = formatWaitingQueue(waitingQueues[CITY_ROUTE]);

    // Keep track of how each taxi capacity changes.
    int taxiCapacity_S = taxiQueues[SHORT_ROUTE].front().getCapacity();
    int taxiCapacity_L = taxiQueues[LONG_ROUTE].front().getCapacity();
    int taxiCapacity_C = taxiQueues[CITY_ROUTE].front().getCapacity();


    cout << left
         << setw(6) << time
         << setw(12) << " "
         << setw(10) << info_S
         << setw(10) << info_L
         << setw(10) << info_C
         << setw(12) << waitingQueue_S
         << setw(12) << waitingQueue_L
         << setw(12) << waitingQueue_C
         << setw(8) << taxiCapacity_S
         << setw(8) << taxiCapacity_L
         << setw(8) << taxiCapacity_C
         << endl;
}

string formatEachTaxi(queue<TaxiType>& taxiQueues) {
    if (taxiQueues.empty())
        return " ";

    TaxiType& taxi = taxiQueues.front();
    if (taxi.getBoardingStatus() == "unavailable") {
        char route = taxi.getCurrentPassenger().getPassengerRoute();
        int boardingTimeLeft = taxi.getCurrentPassenger().getBoardingTime();
        return string(1, route) + "(" + to_string(boardingTimeLeft) + ")";
    }

    return " ";
}

string formatWaitingQueue(queue<passengerType>& waitingQueues) {
    if (waitingQueues.empty())
        return " ";

    while (!waitingQueues.empty()) {
        passengerType& passenger = waitingQueues.front();

        char route = passenger.getPassengerRoute();
        int boardingTime = passenger.getBoardingTime();
        return string(1, route) + "(" + to_string(boardingTime) + ")";
    }

    return " ";
}