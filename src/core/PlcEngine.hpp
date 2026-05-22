#pragma once
#include "memory/MemoryMap.hpp"
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

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

	std::ifstream telemetryFile;

	void runEngine() {
		auto nextCycle = std::chrono::steady_clock::now();
		std::string line;

		if (telemetryFile.is_open()) {
			std::getline(telemetryFile, line);
		}

		while (isRunning) {
			nextCycle += cycleTime;
			if (currentState == PlcState::RUN) {
				readInputsFromCsv();
				executeLogic();
			}

			std::this_thread::sleep_until(nextCycle);
		}
	}
	void readInputsFromCsv() {
		if (!telemetryFile.is_open() || telemetryFile.eof()) {
			std::cout << "[PLC] End of csv data" << std::endl;
			currentState = PlcState::STOP;
			isRunning = false;
			return;
		}
		std::string line;
		if (std::getline(telemetryFile, line)) {
			std::stringstream ss(line);
			std::string stepStr, tempStr;

			if (std::getline(ss, stepStr, ';') && std::getline(ss, tempStr, ';')) {				
				try {
					float currentTemp = std::stof(tempStr);
					memory.setAnalog("AI_Temperature", currentTemp);
				}
				catch (const std::exception& e) {

				}
				
				

			}
		}
	}
	void executeLogic() {
		try {
			float currentTemp = memory.getAnalog("AI_Temperature");

			if (currentTemp > 30.0f) {
				std::cout << "[PLC ALARM] Temperature is to high!" << currentTemp << " C. DEVICE IS STOPPING" << std::endl;
				currentState = PlcState::FAULT;
				isRunning = false;
			}
		}
		catch (const::std::exception& e) {
			std::cout << "[PLC EXCEPTION]" << e.what() << std::endl;
			currentState = PlcState::FAULT;
		}
	}
public:
	PlcEngine(MemoryMap& mem, const std::string& csvPath)
		: memory(mem), currentState(PlcState::STOP), isRunning(false){
		telemetryFile.open(csvPath);
		if (!telemetryFile.is_open()) {
			std::cout << "[PLC ERROR] Can't open file:  " << csvPath << std::endl;
			currentState = PlcState::FAULT;
		}
		
	}
	~PlcEngine() {
		stop();
		if (telemetryFile.is_open()) {
			telemetryFile.close();
		}
	}
	void start() {
		if (isRunning) return;
		isRunning = true;
		currentState = PlcState::RUN;

		workerThread = std::thread(&PlcEngine::runEngine, this);
		std::cout << "[PLC] PLC is running" << std::endl;
	}
	void stop() {
		isRunning = false;
		if (workerThread.joinable()) {
			workerThread.join();
			std::cout << "[PLC] PLC thread joined successfully." << std::endl;
		}
		if (currentState != PlcState::FAULT) {
			currentState = PlcState::STOP;
		}

		std::cout << "[PLC] PLC Stopped" << std::endl;
	}

	PlcState getState() const {
		return currentState;
	}
};