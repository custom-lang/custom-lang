#include <iostream>
#include <iomanip>

#include "../ling/units/soundsystem.h"
#include "../ling/word/syllable_gen.h"

int main() {
    SoundSystem soundSystem("preset01");
    soundSystem.load();

    std::map<unsigned int, Consonant> consonants = soundSystem.getConsonants();
    std::map<unsigned int, Vowel> vowels = soundSystem.getVowels();

    // Define range of syllable types
    std::array<int, 4> max = {1, 1, 1, 0};
    std::array<int, 4> min = {0, 1, 0, 0};

    for (int i = 1; i <= 100; i++) {
        std::cout << "/" << makeSyllable(soundSystem, max, min).getStrSyl() << "/\n";
    }

    return 0;
}
