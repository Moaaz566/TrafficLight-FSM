#ifndef TRAFFICSTATE_H
#define TRAFFICSTATE_H

#include <string>

/* Abstract base class for traffic light states.
 * Defines the interface for polymorphic state behavior.
 * Uses Strategy pattern to encapsulate state-specific behavior and duration.
 * Virtual destructor enables safe cleanup through base class pointers. */
class TrafficState {
public:
	// Pure virtual: subclasses implement their specific state name and duration
	virtual std::string getName() const = 0;

	virtual int getDuration() const = 0;

	// Virtual destructor: required for proper cleanup in polymorphic hierarchies
	virtual ~TrafficState() {}
};

// GREEN state: 30 seconds. Vehicles proceed through the intersection.
class GreenState : public TrafficState {
public:

    std::string getName() const override {
        return "GREEN - Go!";
    }

    int getDuration() const override {
        return 30;
    }
};

// YELLOW state: 5 seconds. Warning phase for vehicles to prepare to stop.
class YellowState : public TrafficState {
public:

    std::string getName() const override {
        return "YELLOW - Slow down!";
    }

    int getDuration() const override {
        return 5;
    }
};

// RED state: 30 seconds. Vehicles must stop at the intersection.
class RedState : public TrafficState {
public:

    std::string getName() const override {
        return "RED - Stop!";
    }

    int getDuration() const override {
        return 30;
    }
};

#endif