#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <map>
#include <vector>

#include "consonant.h"
#include "vowel.h"

#define MAX_PHONEME_LENGTH 10

/* Represents all possible phonemes and suprasegmentals in a language */
class SoundSystem {
private:
    std::string name;
    std::map<unsigned int, Consonant> consonants;
    std::map<unsigned int, Vowel> vowels;
    std::map<std::string, unsigned int> ids;    // NOTE: Temporary, meant for testing

    /*
     * A phoneme symbol is valid if its length is between
     * 1 and the MAX_PHONEME_LENGTH (inclusive), is not the empty string
     * and does not contain spaces.
     */
    bool is_valid_symbol(std::string) const;

    /*
     * Traverse vowels.csv
     * Type: Represents the contents of the file passed in (consonant, vowel, or suprasegmental)
     *  1: Consonant
     *  2: Vowel
     */
    void read_file(std::ifstream& file, int type);

    /*
     * Adds consonant to the soundSystem
     * Returns true if the consonant could not be added
     * Returns false if the consonant was successfully added
     */
    bool insert_consonant(std::string, unsigned int);

    /*
     * Adds vowel to the soundSystem
     * Returns true if the vowel could not be added
     * Returns false if vowel was successfully added
     */
    bool insert_vowel(std::string, unsigned int);
public:
    SoundSystem(std::string name) {
        this->name = name;
    }

    /*
     * Save all phonemes into a csv for the corresponding language
     *
     * Returns true if file couldnt be opened, consonants is empty, or vowels is empty
     * Returns false otherwise
     */
    bool save();

    /*
     * Load phonemes from a csv from the corresponding language
     *
     * Returns true if file couldnt be opened
     * Returns false otherwise
     */
    bool load();

    std::map<unsigned int, Consonant> get_consonants() const { return consonants; }
    std::map<unsigned int, Vowel> get_vowels() const { return vowels; }
    std::map<std::string, unsigned int> get_ids() const { return ids; }
};

#endif
