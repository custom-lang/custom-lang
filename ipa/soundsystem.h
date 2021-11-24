#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>

#include "../ipa/consonant.h"
#include "../ipa/vowel.h"

#define MAX_PHONEME_LENGTH 10

/* Represents all possible phonemes in a language */
class SoundSystem {
private:
    std::string name;
    std::map<unsigned int, Consonant> consonants;
    std::map<unsigned int, Vowel> vowels;
    std::map<std::string, unsigned int> ids;

    bool isValidSym(std::string) const;
public:
    SoundSystem(std::string name) {
        this->name = name;
    }

    bool save();
    bool load();

    std::map<unsigned int, Consonant> getConsonants() const { return consonants; }
    std::map<unsigned int, Vowel> getVowels() const { return vowels; }
    std::map<std::string, unsigned int> getIds() const { return ids; }
};

#endif
