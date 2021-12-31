#include <iomanip>

#include "../ling/units/soundsystem.h"
#include "../ling/units/consonant.h"
#include "../ling/units/vowel.h"

int main() {

    SoundSystem soundSystem("preset00");
    soundSystem.load();

    std::map<unsigned int, Consonant> consonants = soundSystem.getConsonants();
    std::map<unsigned int, Vowel> vowels = soundSystem.getVowels();
    std::map<unsigned int, Suprasegmental> suprasegmentals = soundSystem.getSupraSegs();

    std::cout << "Consonants: " << consonants.size() << "\n----\n";
    for (auto const& sym: consonants) {
        std::cout << "ID: 0x" << std::setfill('0') << std::setw(8) << std::hex << sym.first
                  << " %: " << sym.second.getFreq() << " [" << sym.second.getSymbol()
                  << "] " << sym.second.getDesc() << std::endl;
    }

    std::cout << "\nVowels: " << std::dec << vowels.size() << "\n----\n";
    for (auto const& sym: vowels) {
        std::cout << "ID: 0x" << std::setfill('0') << std::setw(8) << std::hex << sym.first
                  << " %: " << sym.second.getFreq() << " [" << sym.second.getSymbol()
                  << "] " << sym.second.getDesc() << std::endl;
    }

    // Having suprasegmentals is optional
    if (suprasegmentals.size() > 0) {
        std::cout << "\nSuprasegmentals: " << std::dec << suprasegmentals.size() << "\n----\n";
        for (auto const& sym: suprasegmentals) {
            std::cout << "ID: 0x" << std::setfill('0') << std::setw(3) << std::hex << sym.first
                    << " %: " << sym.second.getFreq() << " " << sym.second.getSymbol()
                    << " " << sym.second.getDesc() << std::endl;
        }
    }

    // Saving will delete any invalid phonemes from the file
    soundSystem.save();
    return 0;
}
