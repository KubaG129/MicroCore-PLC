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

	PlcEngine engine(plcMemory);

	engine.start();

	for (int i = 0; i < 5; i++) {
		std::this_thread::sleep_for(std::chrono::milliseconds(250));
		std::cout << "[HMI Alert] Current temperature: " <<
			plcMemory.getAnalog("AI_Temperature") << " C" << std::endl;

		if (engine.getState() == PlcState::FAULT) {
			std::cout << "[HMI Alert] Error state!" << std::endl;
			break;
		}
	}

	engine.stop();
	return 0;
}