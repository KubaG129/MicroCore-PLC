#include <iostream>
#include "memory/MemoryMap.hpp"

int main() {
	std::cout << "Uruchamianie sterownika MicroCore-PLC " << std::endl;

	MemoryMap plcMemory;
	plcMemory.setDigital("I_StartButton", true);
	plcMemory.setAnalog("AI_Temperature", 23.5f);

	std::cout << "Stan przycisku: " << plcMemory.getDigital("I_StartButton") << std::endl;
	std::cout << "Temperatura: " << plcMemory.getAnalog("AI_Temperature") << " C" << std::endl;
	return 0;
}