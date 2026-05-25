#include <iostream>
#include <string>
#include <stdexcept>
#include "TrafficLight.hpp"

using namespace std;

/* Traffic Light FSM Simulator - Main Entry Point
 * Implements an interactive shell for simulating a traffic light state machine.
 * Demonstrates FSM design patterns, polymorphism, memory management, and event logging. */
int main() {

	// Create traffic light FSM controller
	TrafficLight light;

	// Interactive event loop: process user commands until exit
	while (true) {

		light.display();

		cout << "Enter command (tick, skip10, next, emergency, exit): ";

		string input;
		cin >> input;

		// Exception handling: catch invalid user input
		try {
			if (input == "tick") {
				// Advance 1 second
				light.tick();
			}
			else if (input == "skip10") {
				// Advance 10 seconds
				light.skip10();
			}
			else if (input == "next") {
				// Force manual transition
				light.nextState();
			}
			else if (input == "emergency") {
				// Trigger emergency override to RED
				light.emergency();
			}
			else if (input == "exit") {
				// Persist logs and shut down
				light.saveLogs();
				break;
			}
			// Invalid command
			else { throw invalid_argument("Invalid command. Please try again."); }
		}
		catch (const invalid_argument& e) {
			cout << e.what() << endl;
		}
	}
	return 0;
}