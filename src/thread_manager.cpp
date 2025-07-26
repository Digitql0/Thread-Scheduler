#include "thread_manager.hpp"
#include <thread>
#include <iostream>

worker::worker() {
	t = std::thread(&worker::idle, this);
}

worker::~worker() {
	if (t.joinable()) t.join();
}

void worker::idle() {
	std::cout << "this is a running thread" << std::endl;
}

void assignJob(task t) {

}

void isBusy() {

}