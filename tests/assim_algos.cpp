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
#include <algorithm>
#include <iomanip>

#include "../ling/units/soundsystem.h"
#include "../ling/word/morpheme_gen.h"

static RootMorph assim1(std::map<unsigned int, Consonant>&,
                        std::string, std::string, RootMorph&);

static RootMorph assim2(std::map<unsigned int, Consonant>&,
                        unsigned int, unsigned int, RootMorph&);

static unsigned int convertID1(std::map<unsigned int, Consonant>&,
                               std::string, std::string, std::string);

int main() {

    int numMorphs = 10;
    unsigned int xI, yI;

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
    int choice = -1;
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

    while (choice == -1) {
        std::cout << "Enter an integer 1-2: ";
        std::cin >> choice;

        if(std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();

            choice = -1;
        } else if (choice != 1 && choice != 2) {
            std::cout << "\nSelection must be 1-2, inclusive\n\n";

            choice = -1;
        }
    }

    /* Convert x and y to unsigned integers for algorithm 2
     * Assumes conversion always works
     */
    std::replace(x.begin(), x.end(), 'x', '0');
    std::replace(y.begin(), y.end(), 'x', '0');
    xI = std::stoi(x, nullptr, 16);
    yI = std::stoi(y, nullptr, 16);


    // Apply rule and print new morphemes
    std::cout << "\n\n/" << x << "/ --> [" << y << "] / _V or V_\n";
    for (int i = 0; i < numMorphs; i++) {

        switch(choice) {
            case 1:
                std::cout << "[" << assim1(consonants, x, y, morphs[i]).getPhonemic() << "] ";
                break;
            case 2:
                std::cout << "[" << assim2(consonants, xI, yI, morphs[i]).getPhonemic() << "] ";
                break;

        }
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

        // Get sizes
        int onSize = oldOnset.size();
        int coSize = oldCoda.size();

        // Apply rule on onset
        for (int j = 0; j < onSize; j++) {

            // Convert id to string
            std::stringstream ss;
            ss << std::hex << std::setfill('0') << std::setw(7)
               << input.getSyllables()[i].getOnset()[j].getId();
            std::string curID = ss.str();

            onset.push_back(consonants[convertID1(consonants, curID, x, y)]);
        }

        // Apply rule on coda
        for (int j = 0; j < coSize; j++) {

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

/**
 * Algorithm 2
 *
 * Uses integers and bitwise operators to
 * calculate the result of a phonological rule.
 *
 * x represents the ID of the phoneme affected by the rule
 * y represents the ID of the phoneme that results from the rule
 * The 0s in the ID are used to indicate that the digit at that
 * index should be ignored.
 * This allows for x and y to apply to natural classes of sounds, rather than specific phonemes.
 *
 * Consequentally, this algorithm will not work on features that evaluate
 * to 0. For instance, voiceless = 0, bilabial = 0, plosive = 0.
 *
 * This cannot be avoided by adding 1 to certain indexes of an ID, since
 * the maximum value of Manner and Place is 0xf.
 *
 * A possible fix so that this algorithm can be used is
 * representing phoneme IDs with unsigned longs, allowing for
 * Manner and Place to be represented by two indexes.
 *
 * Assumes x and y follow the format of a valid conosonant ID.
 */
static RootMorph assim2(std::map<unsigned int, Consonant>& consonants,
                        unsigned int x, unsigned int y, RootMorph& input) {

    RootMorph output;
    int numSyl = input.getNumSyl();

    // For each syllable in morpheme
    for (int i = 0; i < numSyl; i++) {
        // Grab old onset and coda
        std::vector<Consonant> oldOnset = input.getSyllables()[i].getOnset();
        std::vector<Consonant> oldCoda = input.getSyllables()[i].getCoda();

        // Create new onset and coda
        std::vector<Consonant> onset, coda;

        // Get sizes
        int onSize = oldOnset.size();
        int coSize = oldCoda.size();

        // Apply rule on onset
        for (int j = 0; j < onSize; j++) {
            unsigned int curID = input.getSyllables()[i].getOnset()[j].getId(), newID = 0xfffffff;

            // Check if current ID matches the natural class denoted in x
            if ((curID & x) == x) {

                // Apply rule to current ID
                newID = curID + (y - x);
            }

            /*
             * Push phoneme to onset if resulting ID exists in the soundsystem
             * else, push the old one.
             */
            if(consonants.find(newID) != consonants.end()) {
                onset.push_back(consonants[newID]);
            } else {
                onset.push_back(consonants[curID]);
            }
        }

        // Apply rule on coda
        for (int j = 0; j < coSize; j++) {
            unsigned int curID = input.getSyllables()[i].getCoda()[j].getId(), newID;

            // Check if current ID matches the natural class denoted in x
            if ((curID & x) == x) {

                // Apply rule to current ID
                newID = curID + (y - x);
            }

            /*
             * Push phoneme to onset if resulting ID exists in the soundsystem
             * else, push the old one.
             */
            if(consonants.find(newID) != consonants.end()) {
                coda.push_back(consonants[newID]);
            } else {
                coda.push_back(consonants[curID]);
            }
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
