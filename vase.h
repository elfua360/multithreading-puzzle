#ifndef VASE_H
#define VASE_H

#include <vector>
#include <mutex>
#include <random>
#include <iostream>
#include <thread>
#include <chrono>

class vase 
{
    private:
        // A vector where each index represents the ith guest and guests[i] represents the number of time that guest has seen the vase
        std::vector<int> guests;
        // Whether or not someone is in the vase room
        bool busy;
        // A mutex object
        std::mutex m;

    public:
        vase(int numGuests);
        // One tick simulates what happens when a guest is in the vase room
        void tick(int guestNum);
        // add 1 to the number of times the ith guest has seen the vase
        void updateGuests(int i);
        int getGuest(int i);
        bool getUse();
};
#endif
