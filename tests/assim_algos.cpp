/*
 * The purpose of this "test" is to try out
 * different algorithms that calculate the results of
 * any given phonological rule.
 *
 * Morphemes are generated randomly with each run.
 *
 * This differs with phon_rules, where predefined rules are
 * applied to predefined morphemes.
 *
 */

#include <iostream>
#include <sstream>
#include <iomanip>

#include "../ling/units/soundsystem.h"
#include "../ling/word/morpheme_gen.h"

static RootMorph assim1(std::map<unsigned int, Consonant>&,
                        std::string, std::string, RootMorph&);

static unsigned int convertID1(std::map<unsigned int, Consonant>&,
                               std::string, std::string, std::string);

int main() {

    int numMorphs = 10;

    // Load soundsystem
    SoundSystem soundSystem("preset01");
    soundSystem.load();
    std::map<unsigned int, Consonant> consonants = soundSystem.getConsonants();

    // Define range of syllable types
    std::array<int, 4> max = {1, 1, 1, 0};
    std::array<int, 4> min = {1, 1, 0, 0};

    // Create random morphemes
    std::cout << "Morphemes: \n";
    std::vector<RootMorph> morphs;
    for (int i = 0; i < numMorphs; i++) {
        morphs.push_back(makeRootMorph(soundSystem, max, min));
        std::cout << "/" << morphs[i].getPhonemic() << "/ ";
    }
    std::cout << "\n\nDefine a rule, /x/ --> [y] / _V or V_\n" // TODO: Allow selection for enviornment
              << "For example: \n if /voiceless consonant/ --> [voiced consonant] / _V or V_ "
              << "\n then x = xxx0xx1, y = xxx1xx1\n\n";


    // Prompt for values
    std::string x, y;
    while (x == "") {
        std::cout << "x: ";
        std::getline(std::cin, x);

        // String must be 7 chars long, like a consonant ID
        if(x.size() != 7) {
            x = "";
            std::cout << "Enter a 7 character string\n\n";
        }
    }

    while (y == "") {
        std::cout << "y: ";
        std::getline(std::cin, y);

        // String must be 7 chars long, like a consonant ID
        if(y.size() != 7) {
            y = "";
            std::cout << "Enter a 7 character string\n\n";
        }
    }


    // Apply rule and print new morphemes
    std::cout << "\n\n/" << x << "/ --> [" << y << "] / _V or V_\n";
    for (int i = 0; i < numMorphs; i++) {
        std::cout << "[" << assim1(consonants, x, y, morphs[i]).getPhonemic() << "] ";
    }

    std::cout << "\n";
    return 0;
 }

/**
 * Algorithm 1
 *
 * Uses strings to calculate the result of a
 * phonological rule.
 *
 * x represents the ID of the phoneme affected by the rule
 * y represents the ID of the phoneme that results from the rule
 * The 'x' characters in the ID are used to indicate that the digit at that
 * index should be ignored.
 * This allows for x and y to apply to natural classes of sounds, rather than specific phonemes.
 *
 * Assumes x and y follow the format of a valid conosonant ID.
 */
static RootMorph assim1(std::map<unsigned int, Consonant>& consonants,
                        std::string x, std::string y, RootMorph& input) {

    RootMorph output;
    int numSyl = input.getNumSyl();

    // For each syllable in morpheme
    for (int i = 0; i < numSyl; i++) {
        // Grab old onset and coda
        std::vector<Consonant> oldOnset = input.getSyllables()[i].getOnset();
        std::vector<Consonant> oldCoda = input.getSyllables()[i].getCoda();

        // Create new onset and coda
        std::vector<Consonant> onset, coda;

        // Apply rule on onset
        for (int j = 0; j < static_cast<int>(oldOnset.size()); j++) {

            // Convert id to string
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(7)
               << input.getSyllables()[i].getOnset()[j].getId();
            std::string curID = ss.str();

            onset.push_back(consonants[convertID1(consonants, curID, x, y)]);
        }

        // Apply rule on coda
        for (int j = 0; j < static_cast<int>(oldCoda.size()); j++) {

            // Convert id to string
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(7)
               << input.getSyllables()[i].getCoda()[j].getId();
            std::string curID = ss.str();

            coda.push_back(consonants[convertID1(consonants, curID, x, y)]);
        }

        output.addSyllable(Syllable(onset, input.getSyllables()[i].getNucleus(),
                                    coda, std::vector<Suprasegmental>()));
    }

    return output;
}

/*
 * Helper function for algorithm 1
 */
static unsigned int convertID1(std::map<unsigned int, Consonant>& consonants,
                               std::string curID, std::string x, std::string y) {

    std::string temp = curID;
    int index = curID.size() - 1;
    bool done = false;
    unsigned int id;

    while(!done && index > 0) {

        if (x[index] != 'x' && curID[index] == x[index]) {

            if (curID[index] == x[index]) {
                temp[index] = y[index];
            } else {
                temp = curID;   // Discard any changes
                done = true;
            }
        }
        index--;
    }

    // Check if new id exists in soundsystem
    id = std::stoi(temp, nullptr, 16);
    if (consonants.find(id) == consonants.end()) {
        id = std::stoi(curID, nullptr, 16);
    }

    return id;
}
