/*
Write a program to create two threads. One thread will run the function called runMeFirst, 
and the other thread runs the function runMeSecond. 
The function runMeFirst displays the sentence "Run me first" and the runMeSecond function displays "I run second". 
Use semaphores to make the function runMeFirst runs before the function runMeSecond.
*/ 
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <semaphore.h>

using namespace std;

sem_t sem_1, sem_2, sem_3;

void runMeFirst() {
    sem_wait(&sem_1); // blocking call
    cout << "Run me first" << endl;
    // this_thread::sleep_for(chrono::seconds(2));
    sem_post(&sem_2);
}

void runMeSecond() {
    sem_wait(&sem_2); // blocking call
    cout << "I run second" << endl;
    // this_thread::sleep_for(chrono::seconds(2));
    sem_post(&sem_3);
}

void runMeThird() {
    sem_wait(&sem_3); // blocking call
    cout << "I run second" << endl;
    // this_thread::sleep_for(chrono::seconds(2));
    sem_post(&sem_3);
}

int main() {
    sem_init(&sem_1, 0, 1);
    sem_init(&sem_2, 0, 0);
    sem_init(&sem_3, 0, 0);

    thread t1(runMeFirst);
    thread t2(runMeSecond);
    thread t3(runMeThird);

    t1.join();
    t2.join();
    t3.join();

    sem_destroy(&sem_1);
    sem_destroy(&sem_2);
    sem_destroy(&sem_3);


    return 0;
}

/*
sem_t sem;

void runMeFirst() {
    sem_wait(&sem); // blocking call
    cout << "Run me first" << endl;
    // this_thread::sleep_for(chrono::seconds(2));
    sem_post(&sem);
}

void runMeSecond() {
    sem_wait(&sem); // blocking call
    cout << "I run second" << endl;
    // this_thread::sleep_for(chrono::seconds(2));
    sem_post(&sem);
}

void runMeThird() {
    sem_wait(&sem); // blocking call
    cout << "I run third" << endl;
    // this_thread::sleep_for(chrono::seconds(2));
    sem_post(&sem);
}

int main() {
    sem_init(&sem, 0, 1);

    thread t1(runMeFirst);
    thread t2(runMeSecond);
    thread t3(runMeThird);

    t1.join();
    t2.join();
    t3.join();

    sem_destroy(&sem); // destroy the semaphore

    return 0;
}

*/