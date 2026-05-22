#include <catch2/catch_test_macros.hpp>
#include "memory/MemoryMap.hpp"

TEST_CASE("MemoryMap basic operations", "[memory]") {
	MemoryMap mem;
    SECTION("Digital tags read and write") {
        mem.setDigital("I_EmergencyStop", false);
        REQUIRE(mem.getDigital("I_EmergencyStop") == false);

        mem.setDigital("I_EmergencyStop", true);
        REQUIRE(mem.getDigital("I_EmergencyStop") == true);
    }

    SECTION("Analog tags read and write") {
        mem.setAnalog("AI_Pressure", 4.2f);
        REQUIRE(mem.getAnalog("AI_Pressure") == 4.2f);
    }

    SECTION("Handling non-existent tags") {

        REQUIRE_THROWS_AS(mem.getDigital("NonExistentTag"), std::runtime_error);
        REQUIRE_THROWS_AS(mem.getAnalog("NonExistentTag"), std::runtime_error);
    }

    SECTION("Checking tag existence via hasTag") {
        REQUIRE_FALSE(mem.hasTag("O_Valve"));

        mem.setDigital("O_Valve", true);
        REQUIRE(mem.hasTag("O_Valve") == true);
    }
}