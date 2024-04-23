/*
Write a program to create two threads. One thread will run the function called runMeFirst, 
and the other thread runs the function runMeSecond. 
The function runMeFirst displays the sentence "Run me first" and the runMeSecond function displays "I run second". 
Use semaphores to make the function runMeFirst runs before the function runMeSecond.
*/ 
#include <iostream>
#include <thread>
#include <semaphore.h>

using namespace std;

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

int main() {
    sem_init(&sem, 0, 1);

    thread t1(runMeFirst);
    thread t2(runMeSecond);

    t1.join();
    t2.join();

    sem_destroy(&sem); // destroy the semaphore

    return 0;
}