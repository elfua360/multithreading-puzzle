#include "vase.h"

using namespace std;
vase::vase(int numGuests) : guests(numGuests, 0), busy(false){}

// One tick selects a guest at random and tries to see the vase (acquire the lock)
void vase::tick(int guestNum)
{
    lock_guard<mutex> lg(m);
    // The room is now busy
    busy = true;
    // This guest has now seen the vase +1 times
    vase::updateGuests(guestNum);
    cout << "Guest #" << guestNum << " is viewing the vase..." << endl;
    // Simulate the guest viewing the vase
    this_thread::sleep_for(chrono::seconds(2));
    cout << "Guest #" << guestNum << " is finished viewing the vase...\n" << endl;
    busy = false;
    return;
}

void vase::updateGuests(int i) {guests.at(i) += 1;}

int vase::getGuest(int i) {return guests.at(i);}

bool vase::getUse(){return busy;}