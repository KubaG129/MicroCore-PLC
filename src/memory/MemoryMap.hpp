#pragma once
#include <string>
#include <unordered_map>
#include <stdexcept>

class MemoryMap{
private:
    std::unordered_map<std::string, bool> digitalTags;
    std::unordered_map<std::string, float> analogTags;
public:
    MemoryMap() = default;

    void setDigital(const std::string& name, bool value) {
        digitalTags[name] = value;
    }
    bool getDigital(const std::string& name) const {
        auto it = digitalTags.find(name);
        if (it == digitalTags.end()) {
            throw std::runtime_error("Digital tag not found " + name);
        }
        return it->second;
    }
    void setAnalog(const std::string& name, float value) {
        analogTags[name] = value;
    }
    float getAnalog(const std::string& name) const {
        auto it = analogTags.find(name);
        if (it == analogTags.end()) {
            throw std::runtime_error("Digital tag not found " + name);
        }
        return it->second;
    }
    bool hasTag(const std::string& name) const {
        return digitalTags.count(name) > 0 || analogTags.count(name) > 0;
    }
};