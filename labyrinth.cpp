#include "labyrinth.h"

using namespace std;

labyrinth::labyrinth(int numGuests) : guests(numGuests, false), cake(true), count(0), complete(false)
{
    default_random_engine generator;
    uniform_int_distribution<int> distribution(0, numGuests - 1);
    counterId = distribution(generator);
}

void labyrinth::traverseLabyrinth(int guestId)
{
    lock_guard<mutex> lg(m);

    // If this is the counter and there is no cake, update count and check if all guests have had cake
    if (guestId == counterId && !cake)
    {
        cake = true;
        count++;
        complete = count == guests.size() - 1 ? true : false; 
        if (complete) guests.at(counterId) = true;
    }
            
    // otherwise if there is cake and this guest hasn't had some yet, eat some and update guests vector
    else if (cake && !guests.at(guestId) && guestId != counterId)
    {
        cake = false;
        guests.at(guestId) = true;  
    }
}

bool labyrinth::getCompleteStatus(){return complete;}

int labyrinth::countGuests()
{
    int eatCount = 0;
    for (auto g : guests)
        if (g) eatCount++;
            
    return eatCount;
}  

vector<bool> labyrinth::getGuests(){return guests;}
