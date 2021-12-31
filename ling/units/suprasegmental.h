#ifndef SUPRASEG_H
#define SUPRASEG_H

#include <string>
#include <map>

enum class Feature {
    tone,
    length,
    stress,
    intonation
};

enum class SupraType {
    extraLow,   // Tone
    low,
    mid,
    high,
    extraHigh,
    downstep,
    upstep,
    extraShort, // Length
    halfLong,
    llong,
    extraLong,
    priStress,  // Stress
    secStress,
    minor,      // Intonation
    major,
    globalFall,
    globalRise

};

class Suprasegmental {
private:
    unsigned int id;
    float prob;
    Feature feature;
    SupraType supraType;
    std::string symbol;
    std::string desc;

    std::string getStrSupraType(SupraType type) const {
        static std::map<SupraType, std::string> types {
            {SupraType::extraLow, "Extra Low"},
            {SupraType::low, "Low"},
            {SupraType::mid, "Mid"},
            {SupraType::high, "High"},
            {SupraType::extraHigh, "Extra High"},
            {SupraType::downstep, "Downstep"},
            {SupraType::upstep, "Upstep"},
            {SupraType::extraShort, "Extra Short"},
            {SupraType::halfLong, "Half Long"},
            {SupraType::llong, "Long"},
            {SupraType::extraLong, "Extra Long"},
            {SupraType::priStress, "Primary Stress"},
            {SupraType::secStress, "Secondary Stress"},
            {SupraType::minor, "Minor"},
            {SupraType::major, "Major"},
            {SupraType::globalFall, "Global Fall"},
            {SupraType::globalRise, "Global Rise"}
        };

        std::map<SupraType, std::string>::iterator it;

        it = types.find(type);
        if (it != types.end()) {
            return it->second;
        } else {
            return "NA";
        }
    }
public:
    Suprasegmental(std::string symbol, float prob, Feature feature, SupraType supraType) {

        this->symbol = symbol;
        this->prob = prob;
        this->feature = feature;
        this->supraType = supraType;

        // Format TTS, where S = 0 and the next two digits are for type (extra low, etc)
        id = (static_cast<int>(supraType) * 0x10);

        desc = getStrSupraType(supraType);
    }

    Suprasegmental() {}

    float getFreq() const { return prob; }
    unsigned int getId() const { return id; }
    Feature getFeature() const { return feature; }
    SupraType getSupraType() const { return supraType; }
    std::string getSymbol() const { return symbol; }
    std::string getDesc() const { return desc; }
};

#endif
