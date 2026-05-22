#include <iostream>
#include <thread>
#include <chrono>
#include "memory/MemoryMap.hpp"
#include "core/PlcEngine.hpp"
int main() {
	std::cout << "Plc is starting " << std::endl;

	MemoryMap plcMemory;
	plcMemory.setDigital("I_StartButton", true);
	plcMemory.setAnalog("AI_Temperature", 23.5f);
	std::string csvPath = "C:/Users/kubag/Desktop/MicroCore-PLC/telemtry.csv";
	try {
		PlcEngine engine(plcMemory, csvPath);
		if (engine.getState() == PlcState::FAULT) {
			std::cout << "[HMI ERROR] Init error";
			return 1;
		}
		engine.start();

		while (engine.getState() == PlcState::RUN) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50));
			std::cout << "[HMI Podglad] Current value: "
				<< plcMemory.getAnalog("AI_Temperature") << " C" << std::endl;
		}

		if (engine.getState() == PlcState::FAULT) {
			std::cout << "[HMI]SYSTEM HALTED." << std::endl;
		}
		else {
			std::cout << "[HMI] No errors." << std::endl;
		}

		engine.stop();
	}
	catch (const std::exception& e) {
		std::cerr << "[CRITICAL ERROR] Fatal aplication crash: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}