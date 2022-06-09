#include <iostream>
#include <iomanip>

#include "../ling/units/soundsystem.h"

int main() {

    SoundSystem soundSystem("preset01");
    soundSystem.load();

    std::map<unsigned int, Consonant> consonants = soundSystem.get_consonants();
    std::map<unsigned int, Vowel> vowels = soundSystem.get_vowels();

    std::cout << "Consonants: " << consonants.size() << "\n----\n";
    for (auto const& sym: consonants) {
        std::cout << "ID: 0x" << std::setfill('0') << std::setw(7) << std::hex << sym.first
                  << " [" << sym.second.get_symbol() << "] " << sym.second.get_desc() << "\n";
    }

    std::cout << "\nVowels: " << std::dec << vowels.size() << "\n----\n";
    for (auto const& sym: vowels) {
        std::cout << "ID: 0x" <<  std::hex << sym.first
                  << " [" << sym.second.get_symbol() << "] " << sym.second.get_desc() << "\n";
    }

    // Saving will delete any invalid phonemes from the file
    soundSystem.save();
    return 0;
}
