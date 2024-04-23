#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;

struct requestStructure {
    int request_id; 
    string ip_address;
    string page_requested;
};

mutex mtx;
condition_variable cv;
queue<requestStructure> msg_queue;
int counter;

void listen(const string webPages[]) {
    for (int i = 1; i <= 10; ++i) {
        this_thread::sleep_for(chrono::seconds(1)); // sleep for random number between 1-3 seconds using sleep_for instruction
        {
            lock_guard< mutex> lock(mtx);
            
            requestStructure request; // generate a request with

            request.request_id = counter; // request_id from a counter shared
            request.ip_address = ""; // ip_address as an empty string
            request.page_requested = webPages[i]; // page_request as a random web page name from the array webPages defined above

            msg_queue.push(request); // save the request in msg_queue
        }
        cv.notify_one(); // notify one thread to consume it
    }
}

void do_request(int thread_id) {
    while (true) {
        unique_lock< mutex> lock(mtx);
        cv.wait(lock, []{ // if the msg_queue is empty, then wait using conditional variable
            return !msg_queue.empty();
        });  

        // int message = msg_queue.front();
        // msg_queue.pop();
        // lock.unlock();

        // else get the request from the msg_queue
        requestStructure request = msg_queue.front();
        msg_queue.pop();
        lock.unlock();

        cout << "Thread " << thread_id << " completed request " << request.request_id << " requesting webpage " << request.page_requested << endl;
    }
}

int main() {
    thread t1(listen);
    thread t2(do_request);
    t1.join();
    t2.join();
    cout << "Main thread finished" << endl;

    string webPages[10] = {"google.com", "yahoo.com", "gmail.com", "canvas.com", "submittable.com", "instagram.com", "twitter.com", "paliroots.com", "neworleansreview.com", "massreview.com",};

    return 0;
}

/*

In this question, we are going to pretend that we have a web server. Write a program in C++ to simulate a web server. 
The web server should receive requests from clients about web pages and the web server returns the request web pages. 
Your C++ program should create one thread that receive the requests from the clients and n threads that will search for the 
requested pages and send them to the client. This problem can be implemented using the producer-consumer problem. 
The requests from the client are going to be a structure of the following fields:

struct reqestStructure {
    int request_id; 
    string ip_address;
   string page_requested;
};

The msg_queue is defined as: queue<requestStructure> msg_queue;

Create a list of 10 web pages names as: string webPages[10] = {"google.com", "yahoo.com", ...};

The program consists of two functions:

1) first function is called listen: that will do the following steps:

sleep for random number between 1-3 seconds using sleep_for instruction.
generate a request with request_id from a counter shared, ip_address as an empty string and page_request as a random web page name from the array webPages defined above; then save the request in msg_queue.
notify one thread to consume it.
2) second function is called do_request that will do the following steps:

if the msg_queue is empty, then wait using conditional variable
else get the request from the msg_queue
display a message; "thread thread_id completed request request_id requesting webpage page_requested"; where thread_id is the id number of the thread, the request_id is the id number of the request and page_requested as the web page requested.

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