#ifndef LABYRINTH_H
#define LABYRINTH_H
#include <mutex>
#include <thread>
#include <vector>
#include <random>

class labyrinth 
{
     private:
        // A vector where each index represents the ith guest and a boolean value indicates if they have had cake
        std::vector<bool> guests;
        
        // A boolean representing whether or not cake is available
        bool cake;

        // A count representing the number of times the counter has refilled the cake
        int count;

        // An int representing the index of the counter in the guest vector
        int counterId;

        // Indicates whether all guests have had cake
        bool complete;

        // A mutex object
        std::mutex m;

        public:
            // Constructor initializes the member variables and picks a random guest to serve as counter
            labyrinth(int numGuests);
            // Simulate one guest traversing the labyrinth
            void traverseLabyrinth(int guestId);
            bool getCompleteStatus();
            // for reporting results
            int countGuests();
            std::vector<bool> getGuests();
};
#endif