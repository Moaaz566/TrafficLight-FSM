#include "TrafficLight.hpp"
#include <iostream>
#include <fstream>

using namespace std;

/* Constructor: initializes FSM and dynamic resources.
 * GREEN is the starting state by design (safe default).
 * Allocates log array with fixed capacity to prevent memory fragmentation. */
TrafficLight::TrafficLight() {
	// Create initial GREEN state object on heap to enable polymorphic behavior
	currentState = new GreenState();

	// Initialize timer to state's duration (30 seconds for GREEN)
	timer = currentState->getDuration();

	// Start simulation clock at 0 seconds
	simulationTime = 0;

	// Allocate fixed-size log buffer; prevents unbounded memory growth
	logs = new string[MAX_LOGS];

	// Initialize log index to track number of entries
	logIndex = 0;
}

// Accessor: retrieves state name through virtual function (polymorphic).
std::string TrafficLight::getState() {
	return currentState->getName();
}

// UI: displays current state and remaining time before next transition
void TrafficLight::display() {
	cout << "==============================" << endl;
	cout << "Current State : " << getState() << endl;
	cout << "Time Remaining: " << timer << " seconds" << endl;
}

/* Event handler: advances simulation by 1 second and logs the action.
 * Delegates time progression to advanceTime() which handles state transitions. */
void TrafficLight::tick() {
	advanceTime(1);
	// Buffer overflow protection: only log if space available
	if (logIndex < MAX_LOGS) {
		logs[logIndex++] = "[" + to_string(simulationTime) + "s] Tick -> " +
			getState() + " (" + to_string(timer) + "s left)";
	}
}

/* Event handler: advances simulation by 10 seconds and logs the action.
 * May trigger multiple state transitions if 10 seconds spans state boundaries. */
void TrafficLight::skip10() {
	advanceTime(10);
	if (logIndex < MAX_LOGS) {
		logs[logIndex++] = "[" + to_string(simulationTime) + "s] Skip 10s -> " +
			getState() + " (" + to_string(timer) + "s left)";
	}
}

/* Event handler: manual state transition (overrides timer).
 * Resets timer to 0, triggering transition() on next logic cycle. */
void TrafficLight::nextState() {

	timer = 0;

	transition();

	cout << "Manual transition executed." << endl;

	if (logIndex < MAX_LOGS) {

		logs[logIndex++] =
			"[" + to_string(simulationTime) +
			"s] Manual state transition";
	}
}

/* Emergency override: bypasses normal FSM logic and forces RED state.
 * Deletes current state object and allocates new RedState.
 * Represents a hardware interrupt-like event in the simulation. */
void TrafficLight::emergency() {
	string oldState = currentState->getName();

	// Release old state object before reassigning pointer
	delete currentState;

	// Allocate new state object on heap
	currentState = new RedState();
	timer = currentState->getDuration();

	cout << "==============================" << endl;
	cout << "EMERGENCY!!! Switching to RED." << endl;

	if (logIndex < MAX_LOGS) {
		logs[logIndex++] = "[" + to_string(simulationTime) + "s] Emergency: " + oldState + " -> RED";

	}

}

/* FSM core transition logic: implements state machine state changes.
 * Triggered when timer expires (timer == 0).
 * Uses polymorphic interface to retrieve state names and allocate new state objects.
 * State transitions: GREEN -> YELLOW -> RED -> GREEN (cyclic). */
void TrafficLight::transition() {
	// Guard: only transition when timer is depleted
	if (timer > 0) return;

	// Retrieve current state name before deletion (needed for logging)
	string oldState = currentState->getName();

	// Release old state object before reassigning pointer
	delete currentState;

	// GREEN -> YELLOW transition
	if (oldState == "GREEN - Go!") {
		currentState = new YellowState();

		if(logIndex<MAX_LOGS) {
			logs[logIndex++] = "[" + to_string(simulationTime) + "s] Transition: GREEN -> YELLOW";
		}
	}

	// YELLOW -> RED transition
	if (oldState == "YELLOW - Slow down!") {
		currentState = new RedState();

		if(logIndex<MAX_LOGS) {
			logs[logIndex++] = "[" + to_string(simulationTime) + "s] Transition: YELLOW -> RED";
		}
	}

	// RED -> GREEN transition
	if (oldState == "RED - Stop!") {
		currentState = new GreenState();

		if(logIndex<MAX_LOGS) {
			logs[logIndex++] = "[" + to_string(simulationTime) + "s] Transition: RED -> GREEN";
		}
	}

	// Initialize timer for newly allocated state object
	timer = currentState->getDuration();
}

/* Time progression with automatic state transitions.
 * Processes time in a loop, triggering transitions when states expire.
 * Example: advanceTime(35) from GREEN may result in: GREEN -> YELLOW -> RED.
 * This implements the core temporal behavior of the FSM. */
void TrafficLight::advanceTime(int seconds) {
	// Increment total simulation time
	simulationTime += seconds;

	// Process all time units, allowing multiple state transitions if needed
	while (seconds > 0) {
		// Case 1: Enough time remains to complete current state
		if (seconds >= timer) {
			// Consume time equal to remaining duration of current state
			seconds -= timer;

			// Trigger state transition by setting timer to 0
			timer = 0;

			// Execute transition logic (state change and timer reset)
			transition();
		}
		// Case 2: Time runs out before state completes
		else {
			// Deduct remaining time from current state's timer
			timer -= seconds;
			seconds = 0;
		}
	}
}

/* File I/O: persists all logged events to disk.
 * Implements exception handling for file open failures.
 * Gracefully handles log buffer overflow (logs are silently discarded if full). */
void TrafficLight::saveLogs() {
	// Open output file stream
	ofstream file("traffic_light_logs.txt");

	// Exception handling: validate file open succeeded
	if (!file) {
		cerr << "Error: Could not open traffic_light_logs.txt" << endl;
		return;
	}

	// Write all logged events to file
	for (int i = 0;i < logIndex;i++) {
		file << logs[i] << endl;
	}

	cout << "Logs saved to traffic_light_logs.txt" << endl;
}

/* Destructor: releases heap-allocated resources.
 * Deletes current state object (cannot use delete[] since it's a single object,
 * not an array). Deallocates log array.
 * Critical for preventing memory leaks in a long-running simulator. */
TrafficLight::~TrafficLight() {
	// Release current state object (single object, not array)
	delete currentState;

	// Release log array
	delete[] logs;
}
