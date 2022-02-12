#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include <map>
#include <vector>

#include "consonant.h"
#include "vowel.h"
#include "suprasegmental.h"

#define MAX_PHONEME_LENGTH 10

/* Represents all possible phonemes and suprasegmentals in a language */
class SoundSystem {
private:
    std::string name;
    std::map<unsigned int, Consonant> consonants;
    std::map<unsigned int, Vowel> vowels;
    std::map<unsigned int, Suprasegmental> suprasegmentals;
    std::vector<float> probOnsets, probNuclei, probCodas, probSupras;
    std::vector<unsigned int> onsetIds, nucleusIds, codaIds, supraIds;
    std::map<std::string, unsigned int> ids;    // Temporary, meant for testing

    /*
     * A phoneme symbol is valid if its length is between
     * 1 and the MAX_PHONEME_LENGTH (inclusive), is not the empty string
     * and does not contain spaces.
     */
    bool isValidSym(std::string) const;

    /*
     * Traverse vowels.csv
     * Type: Represents the contents of the file passed in (consonant, vowel, or suprasegmental)
     *  0: Suprasegmental
     *  1: Consonant
     *  2: Vowel
     */
    void traverseFile(std::ifstream& file, int type);

    /*
     * Adds consonant to the soundSystem
     * Returns true if the consonant could not be added
     * Returns false if the consonant was successfully added
     */
    bool insertConsonant(std::string, unsigned int, float, float, float);

    /*
     * Adds vowel to the soundSystem
     * Returns true if the vowel could not be added
     * Returns false if vowel was successfully added
     */
    bool insertVowel(std::string, unsigned int, float);

    /*
     * Adds suprasegmental to the soundSystem
     * Returns true if the suprasegmental could not be added
     * Returns false if suprasegmental was successfully added
     */
    bool insertSupra(std::string, unsigned int, float);
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

    std::map<unsigned int, Consonant> getConsonants() const { return consonants; }
    std::map<unsigned int, Vowel> getVowels() const { return vowels; }
    std::map<unsigned int, Suprasegmental> getSupraSegs() const { return suprasegmentals; }
    std::map<std::string, unsigned int> getIds() const { return ids; }

    std::vector<float> getProbOnsets() const { return probOnsets; }
    std::vector<float> getProbNuclei() const { return probNuclei; }
    std::vector<float> getProbCodas() const { return probCodas; }
    std::vector<float> getProbSupras() const { return probSupras; }

    std::vector<unsigned int> getOnsetIds() const { return onsetIds; }
    std::vector<unsigned int> getNucleusIds() const { return nucleusIds; }
    std::vector<unsigned int> getCodaIds() const { return codaIds; }
    std::vector<unsigned int> getSupraIds() const { return supraIds; }
};

#endif
