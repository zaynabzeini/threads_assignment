#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <ctime>
#include <cstdlib>
using namespace std;

struct requestStructure {
    int request_id; 
    string ip_address;
    string page_requested;
};

mutex mtx;
condition_variable cv;
queue<requestStructure> msg_queue;
int counter = 0;
int nListeners = 0;
// int mReceivers = 0;

void listen(string webPages[]) {
    srand(time(NULL));
    for (int i = 1; i <= 10; ++i) {
        int time = rand() % 3 + 1;
        int j = rand() % 10 + 0;
        this_thread::sleep_for(chrono::seconds(time)); // sleep for random number between 1-3 seconds using sleep_for instruction
        {
            lock_guard< mutex> lock(mtx);
            
            requestStructure request; // generate a request with

            counter++;
            request.request_id = counter; // request_id from a counter shared
            request.ip_address = ""; // ip_address as an empty string
            request.page_requested = webPages[j]; // page_request as a random web page name from the array webPages defined above

            msg_queue.push(request); // save the request in msg_queue
        }
        cv.notify_one(); // notify one thread to consume it
    }
}

void do_request(int thread_id) {
    while (true) {
        unique_lock< mutex> lock(mtx);

        // if the msg_queue is empty, then wait using conditional variable
        if (msg_queue.empty()) {
            cv.wait(lock);
        }
        // cv.wait(lock, []{ 
        //     return !msg_queue.empty();
        // });  

        // else get the request from the msg_queue
        else {
            requestStructure request = msg_queue.front();  // int message = msg_queue.front();
            msg_queue.pop(); // msg_queue.pop();
            lock.unlock(); // lock.unlock();
            cout << "Thread " << thread_id << " completed request " << request.request_id << " requesting webpage " << request.page_requested << endl;

        }
    }
}

int main() {
    string webPages[10] = {"google.com", "yahoo.com", "gmail.com", "canvas.com", "submittable.com", "instagram.com", "twitter.com", "paliroots.com", "neworleansreview.com", "massreview.com",};
    // thread recievers[mReceivers]; // array of threads that recieve requests
    thread listeners[nListeners]; // array of threads that search for the requested web pages
    int i;
    int k;
    
    // for (i = 0; i < mReceivers; i++) {
    //     recievers[i] = thread(listen, webPages); // thread that recieves the requests from the clients 
    //     for (k = 0; k < nListeners; k++) {
    //         listeners[k] = thread(do_request, k); // thread that search for the  requested pages and send them to the client 
    //     }
    //     recievers[i].join();
    //     listeners[k].join();
    // }    
    
    thread reciever(listen, webPages);
    for (k = 0; k < nListeners; k++) {
        listeners[k] = thread(do_request, k); // thread that search for the  requested pages and send them to the client 
        reciever.join();
        listeners[k].join();
    }

    cout << "Main thread finished" << endl;

    return 0;
}

/*

In this question, we are going to pretend that we have a web server. Write a program in C++ to simulate a web server. 
The web server should receive requests from clients about web pages and the web server returns the request web pages. 
Your C++ program should create one thread that receive the requests from the clients and n threads that will search for the 
requested pages and send them to the client. This problem can be implemented using the producer-consumer problem. 
The requests from the client are going to be a structure of the following fields:

struct requestStructure {
    int request_id; 
    string ip_address;
   string page_requested;
};

The msg_queue is defined as: queue<requestStructure> msg_queue;

Create a list of 10 web pages names as: string webPages[10] = {"google.com", "yahoo.com", ...};

The program consists of two functions:

1) first function is called listen: that will do the following steps:
sleep for random number between 1-3 seconds using sleep_for instruction.
generate a request with request_id from a counter shared, ip_address as an empty string and 
page_request as a random web page name from the array webPages defined above; then save the request in msg_queue.
notify one thread to consume it.

2) second function is called do_request that will do the following steps:
if the msg_queue is empty, then wait using conditional variable
else get the request from the msg_queue
display a message; "thread thread_id completed request request_id requesting webpage page_requested"; 
where thread_id is the id number of the thread, the request_id is the id number of the request and page_requested as the web page requested.

void producer() {
    cout << "Producer thread started" << endl;
    // simulate producing messages
    for (int i = 1; i <= 5; ++i) {
        this_thread::sleep_for( chrono::seconds(1));
        {
            lock_guard< mutex> lock(mtx);
            msg_queue.push(i);
            cout << "Produced message " << i << endl;
        }
        cv.notify_one();
    }
}

void consumer() {
    cout << "Consumer thread started" << endl;
    // simulate processing messages
    while (true) {
        unique_lock< mutex> lock(mtx);
        cv.wait(lock, []{return !msg_queue.empty();});
        int message = msg_queue.front();
        msg_queue.pop();
        cout << "Consumed message " << message << endl;
        lock.unlock();
        if (message == 5) {
            break;
        }
    }
}

*/