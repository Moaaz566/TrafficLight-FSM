#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <string>
#include "TrafficState.hpp"

/* FSM controller class for the traffic light simulator.
 * Manages state objects, timers, transitions, and event logging.
 * Implements state machine pattern using polymorphic TrafficState hierarchy.
 * Uses dynamic memory for state objects to enable runtime polymorphism. */
class TrafficLight {
private:

	// Pointer to current state object. Dynamically allocated to support polymorphism.
	// Reset on each state transition to instantiate the new state type.
	TrafficState* currentState;

	// Countdown timer in seconds. When it reaches 0, automatic transition occurs.
	int timer;

	// Cumulative simulation time in seconds.
	int simulationTime;

	// FSM state machine core: evaluates timer and transitions to next state.
	void transition();

	// Advances simulation time and handles intermediate state transitions.
	// Processes multiple seconds by chaining transitions if needed.
	void advanceTime(int seconds);

	// Dynamically allocated array for storing event logs.
	// Fixed capacity design to avoid unbounded memory growth.
	static const int MAX_LOGS = 100;
	std::string* logs;

	// Tracks number of logged events; prevents buffer overflow.
	int logIndex;

public:

	// Constructor: initializes FSM starting in GREEN state with initial timer and empty log.
	TrafficLight();

	// Delete copy/move operations to prevent unsafe duplication of dynamic resources.
	TrafficLight(const TrafficLight&) = delete;
	TrafficLight& operator=(const TrafficLight&) = delete;

	// Simulation event: advances time by 1 second, logs action.
	void tick();

	// Simulation event: advances time by 10 seconds, logs action.
	void skip10();

	// Simulation event: manual transition trigger (bypasses timer).
	void nextState();

	// Simulation event: overrides FSM to force RED state (emergency override).
	void emergency();

	// Displays current state and remaining timer to console.
	void display();

	// File I/O: persists logs to disk before shutdown.
	void saveLogs();

	// Accessor: returns current state name via polymorphic interface.
	std::string getState();

	// Destructor: releases dynamically allocated state and log array.
	~TrafficLight();
};

#endif