#include <iostream>
#include <functional>

#include "../ling/units/soundsystem.h"

/*
 * Given a vector of phonemes, print out their symbols
 */
static std::string get_representation(std::map<unsigned int, Consonant>&,
                                      std::map<unsigned int, Vowel>&,
                                      std::vector<unsigned int>&);

/*
 * Applies an assimilation rule to a sequence of phonemes
 *
 * cur_class -> res_class / prev_class _ next_class
 *
 */
static std::vector<unsigned int> assim_rule(std::map<unsigned int, Consonant>&,
                                            std::map<unsigned int, Vowel>&,
                                            unsigned int,
                                            unsigned int,
                                            unsigned int,
                                            unsigned int,
                                            std::vector<unsigned int>&);

int main() {
    SoundSystem soundSystem("preset01");
    soundSystem.load();

    std::map<unsigned int, Consonant> consonants = soundSystem.get_consonants();
    std::map<unsigned int, Vowel> vowels = soundSystem.get_vowels();
    std::map<std::string, unsigned int> ids = soundSystem.get_ids();

    // high vowel -> voiceless / voiceless consonant _ voiceless consonant
    auto voicing_rule = std::bind(assim_rule, consonants, vowels,
                                  0x20012, 0x10012, 0x100001, 0x100001, std::placeholders::_1);

    // plosive -> nasal / _ nasal consonant
    auto plosive_rule = std::bind(assim_rule, consonants, vowels,
                                  0x10011, 0x20011, 0x0, 0x20011, std::placeholders::_1);

    /*
     * Use ids map to retrieve phoneme given a symbol,
     * since ids may change in the future
     */
    std::vector<unsigned int> word1 = {
        ids["s"],
        ids["u"],
        ids["t"],
        ids["u"],
        ids["j"],
        ids["i"]
    }; // sutuji

    std::vector<unsigned int> word2 = {
        ids["k"],
        ids["u"],
        ids["ʒ"],
        ids["i"],
        ids["n"]
    }; // kuʒin

    std::vector<unsigned int> word3 = {
        ids["z"],
        ids["o"],
        ids["s"],
        ids["i"],
        ids["k"],
        ids["ɑ"]
    }; // zosikɑ

    std::vector<unsigned int> word4 = {
        ids["v"],
        ids["ɛ"],
        ids["d"],
        ids["n"],
        ids["ɛ"],
        ids["l"]
    }; // vednɛl

    std::vector<unsigned int> word5 = {
        ids["s"],
        ids["o"],
        ids["b"],
        ids["n"],
        ids["i"]
    }; // sobni

    // Apply voicing rule
    std::vector<unsigned int> rep1 = voicing_rule(word1); // su̥tuji
    std::vector<unsigned int> rep2 = voicing_rule(word2); // kuʒin
    std::vector<unsigned int> rep3 = voicing_rule(word3); // zosi̥kɑ

    // Apply plosive rule
    std::vector<unsigned int> rep4 = plosive_rule(word4); // vɛnnɛl
    std::vector<unsigned int> rep5 = plosive_rule(word5); // somni

    std::cout << "high vowel -> voiceless / voiceless consonant _ voiceless consonant\n"
              << "/" << get_representation(consonants, vowels, word1) << "/ --> ["
              << get_representation(consonants, vowels, rep1)  << "]\n"
              << "/" << get_representation(consonants, vowels, word2) << "/ --> ["
              << get_representation(consonants, vowels, rep2)  << "]\n"
              << "/" << get_representation(consonants, vowels, word3) << "/ --> ["
              << get_representation(consonants, vowels, rep3)  << "]\n";

    std::cout << "\nplosive -> nasal / _ nasal consonant\n"
              << "/" << get_representation(consonants, vowels, word4) << "/ --> ["
              << get_representation(consonants, vowels, rep4)  << "]\n"
              << "/" << get_representation(consonants, vowels, word5) << "/ --> ["
              << get_representation(consonants, vowels, rep5)  << "]\n";

    return 0;
}

static std::string get_representation(std::map<unsigned int, Consonant>& consonants,
                                      std::map<unsigned int, Vowel>& vowels,
                                      std::vector<unsigned int>& word) {

    std::string output = "";

    for(const auto& phoneme: word) {

        // Calculate if phoneme is consonant or vowel
        if(phoneme % 0x10 == 1) {
            output += consonants[phoneme].get_symbol();
        } else {
            output += vowels[phoneme].get_symbol();
        }
    }

    return output;
}

static std::vector<unsigned int> assim_rule(std::map<unsigned int, Consonant>& consonants,
                                            std::map<unsigned int, Vowel>& vowels,
                                            unsigned int cur_class,
                                            unsigned int res_class,
                                            unsigned int prev_class,
                                            unsigned int next_class,
                                            std::vector<unsigned int>& word) {

    std::vector<unsigned int> output;
    int len = word.size();

    for(int i = 0; i < len; i ++) {
        unsigned int cur_id = word[i], new_id = 0x0;

        // Apply rule
        if (prev_class != 0 && next_class != 0) { // Environment: prev_class _ next_class
            if (i - 1 >= 0
                && i + 1 < len
                && (word[i - 1] & prev_class) == prev_class
                && (word[i + 1] & next_class) == next_class
                && (cur_id & cur_class) == cur_class) {

                    new_id = cur_id + (res_class - cur_class);
            }
        } else if (prev_class != 0) { // Environment: prev_class _
            if (i - 1 >= 0
                && (word[i - 1] & prev_class) == prev_class
                && (cur_id & cur_class) == cur_class) {

                    new_id = cur_id + (res_class - cur_class);
            }
        } else { // Environment: _ next_class
            if (i + 1 < len
                && (word[i + 1] & next_class) == next_class
                && (cur_id & cur_class) == cur_class) {

                    new_id = cur_id + (res_class - cur_class);
            }
        }

        // Only push id to output if id exists in the current sound system
        if(consonants.find(new_id) != consonants.end() || vowels.find(new_id) != vowels.end()) {
            output.push_back(new_id);
        } else {
            output.push_back(cur_id);
        }
    }

    return output;
}
