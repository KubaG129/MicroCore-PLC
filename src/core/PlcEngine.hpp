#pragma once
#include "memory/MemoryMap.hpp"
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>


enum class PlcState {
	STOP,
	RUN,
	FAULT
};

class PlcEngine {
private:
	MemoryMap& memory;
	std::atomic<PlcState> currentState; //atomic variable is prepared for working on thread
	std::thread workerThread;	//Main thread
	std::atomic<bool> isRunning;

	const std::chrono::milliseconds cycleTime{ 10 }; // 1 cycle

	void runEngine() {
		auto nextCycle = std::chrono::steady_clock::now();

		while (isRunning) {
			nextCycle += cycleTime;
			if (currentState == PlcState::RUN) {
				executeLogic();
			}

			std::this_thread::sleep_until(nextCycle);
		}
	}
	void executeLogic() {
		try {
			if (memory.getDigital("I_StartButton")) {
				float currentTemp = memory.getAnalog("AI_Temperature");

				memory.setAnalog("AI_Temperature", currentTemp + 0.1f);

				if (memory.getAnalog("AI_Temperature") > 30.0f) {
					std::cout << "[PLC ERROR] Temperature over the limit" << std::endl;
					currentState = PlcState::FAULT;
				}
			}
		}
		catch (const::std::exception& e) {
			std::cout << "[PLC EXCEPTION]" << e.what() << std::endl;
			currentState = PlcState::FAULT;
		}
	}
public:
	PlcEngine(MemoryMap& mem)
		: memory(mem), currentState(PlcState::STOP), isRunning(false){}
	~PlcEngine() {
		stop();
	}
	void start() {
		if (isRunning) return;
		isRunning = true;
		currentState = PlcState::RUN;

		workerThread = std::thread(&PlcEngine::runEngine, this);
		std::cout << "[PLC] PLC is running" << std::endl;
	}
	void stop() {
		if (!isRunning) return;

		isRunning = false;
		currentState = PlcState::STOP;
		if (workerThread.joinable()) {
			workerThread.join();
		}
		std::cout << "[PLC] PLC Stopped" << std::endl;
	}

	PlcState getState() const {
		return currentState;
	}
};