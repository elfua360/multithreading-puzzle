/*
    Problem 1 solution outline:
    This seems similar to the prisoners with the light switch problem
    Select one guest (thread) to be the one to let the minotaur know that 
    all guests have had cake. 
    Each guest will eat cake once, and only once
    For each thread: if there is cake and you have yet to eat cake, eat it
    Otherwise ignore it
    When the counter comes in he asks for cake but does not eat it, otherwise ignore it and don't update count
    After the counter has refilled the cake N-1 times every guest will have had some to eat
    and  he/she can announce that everyone has had some

    Problem 2 solution outline:
    Create a queue to add the guests to. For each tick pop the queue and have the guest
    view the art (simulated using thread.sleep) and while they are doing that more guests
    can queue. the tick is over after the guest is finished viewing. Repeat.

*/
#include "labyrinth.h"
#include "vase.h"
#include <fstream>
#include <queue>
#include <algorithm>

using namespace std;

// Number of threads depends on hardware
const int NUM_THREADS = thread::hardware_concurrency();
default_random_engine generator;

// Naive implementation of a basic threadpool
// Credit to C++ Concurrency in Action and many Stack Overflow
// posts for help with putting this together
template<typename job>
void start_thread(vector<thread>& threads, job&& j)
{
    // Find an empty thread
    for(auto&& t: threads)
    {
        if(t.joinable()) 
            continue;

        t = thread(j);
        return;
    }

    // Wait for a thread to become available
    for(auto&& t: threads)
    {
        if(!t.joinable()) 
            continue;

        t.join();
        t = thread(j);
        return;
    }
}

// Code that executes the labyrinth problem
void runProblem1()
{
    // The thread pool
    vector<thread> pool(NUM_THREADS);
    const int MAX_GUEST_COUNT =  200; 
    int guestNum = 0;

    // Get number of guests
    do
    {
        cout << "Enter an integer representing the number of guests between 2 and " << MAX_GUEST_COUNT << endl;
        cin >> guestNum;
    } while (guestNum < 2 or guestNum > MAX_GUEST_COUNT);

    // Initialize labyrinth
    labyrinth l(guestNum);
    
    uniform_int_distribution<int> distribution(0, guestNum - 1);

    //For recording time
    auto start = chrono::high_resolution_clock::now();

    int guestId = -1;
    // Simulate the puzzle
    while (!l.getCompleteStatus())
    {
        // Randomly select a guest to enter the labyrinth 
        guestId = distribution(generator);
        start_thread(pool, [&]{l.traverseLabyrinth(guestId);});
    }

    // Clear any remaining threads
    for (auto&& t : pool)
        if (t.joinable())
            t.join();

    // Record total time
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    
    int numEaten = l.countGuests();

    // Write results to file
    ofstream f;
    f.open("results-problem1.txt");
    f << "RESULTS\n\nNumber of guests who ate cake: " << numEaten << "\nNumber of guests who have yet to eat cake: " << guestNum - numEaten << endl;
    f << "Time to execute(MS): " << duration.count() << endl;
    f << "-----------------------------------" << endl;
    f << "GUEST#\tHAS HAD CAKE?" << endl;
    vector<bool> guests = l.getGuests();
    for (int i = 0; i < guests.size(); i++)
        f << i << "\t" << guests[i] << endl;
    f.close();
    cout << "Results written to \"results-problem1.txt\"" << endl;
}

// Code that executes the vase problem
void runProblem2()
{
    // Get the number of ticks to run for
    int ticks = 20;
    cout << "Enter how many ticks to run (20 recommended)" << endl;
    cin >> ticks;
    cout << "Starting simulation with 50 guests and " << ticks << " ticks...\n" << endl;

    // Use 50 guests
    int numGuests = 50;

    // Create a vase object
    vase v(numGuests);
    vector<int> guests;
    queue<int> vaseQ;

    // Shuffle the guests so the order is random
    for (int i = 0; i < numGuests; i++) guests.push_back(i);
    shuffle(begin(guests), end(guests), generator);

    // Push the first randomly chosen guest onto the queue
    vaseQ.push(guests.at(0));

   // In order to avoid early repetition, remove first guest from vector
    guests.erase(guests.begin());
    uniform_int_distribution<int> distribution(0, 2000);
    for (int i = 1; i <= ticks; i++)
    {
        cout << "TICK " << i << "\n--------------" << endl;
        int g = vaseQ.front();
        vaseQ.pop();
        thread t(&vase::tick, ref(v), g);
        this_thread::sleep_for(chrono::milliseconds(200));
        // While the first guest is viewing the vase, queue the next randomly chosen guests 
        while (v.getUse() && !guests.empty())
        {
            vaseQ.push(guests.at(0));
            cout << "Guest #" << guests.at(0) << " queues to see the vase" << endl;
            guests.erase(guests.begin());
            this_thread::sleep_for(chrono::milliseconds(distribution(generator)));
        }
        // Make sure the thread finishes before continuing
        t.join();
        // guests.insert(guests.end(), guestsInUse.begin(), guestsInUse.end());
        // Add the guest back to the vector and shuffle
        guests.push_back(g);
        shuffle(begin(guests), end(guests), generator);
    }

    // Write final results
    ofstream f;
    f.open("results-problem2.txt");
    f << "Final state of guest queue:" << endl;
    while(!vaseQ.empty())
    {
        f << vaseQ.front() << " ";
        vaseQ.pop();
    }
    f << "\n\n";
    f << "GUEST #\t| # OF TIMES SEEING VASE\n-------------------\n" << endl;
    for (int i = 0; i < numGuests; i++)
        f << i << "\t" << v.getGuest(i) << endl;
    f.close();
    cout << "Final states written to \"results-problem2.txt\"" << endl;
}

// Simple menu driven program
int main()
{
    int choice = 0;
    do 
    {
        cout << "Enter 1 to simulate problem #1 or 2 to simulate problem #2, or enter 0 to quit" << endl;
        cin >> choice;
        
        switch (choice)
        {
            case 0:
                break;
            case 1:
                runProblem1();
                choice = 0;
                break;
            case 2:
                choice = 0;
                runProblem2();
                break;
            default:
                cout << "Invalid input!" << endl;
                break;
        }
    } while (choice != 0);
    return 0;
}
