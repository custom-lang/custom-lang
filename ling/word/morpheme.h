/*
 * A morpheme represents a sequence of syllables
 * and their transcription.
 *
 */

#ifndef MORPHEME_H
#define MORPHEME_H

#include <initializer_list>
#include "syllable.h"

class Morpheme {
private:
    std::vector<Syllable> syllables;
    std::string phonemic;               // Phonemic transcription of morpheme
    int numSyl;
public:
    Morpheme(std::initializer_list<Syllable> list) {
        numSyl = 0;
        for (const auto& syllable : list) {
            addSyllable(syllable);
        }
    }

    Morpheme() { numSyl = 0; }

    void addSyllable(Syllable syl) {
        syllables.push_back(syl);
        phonemic += syl.getStrSyl();
        numSyl++;
    }

    std::vector<Syllable> getSyllables() const { return syllables; }
    std::string getPhonemic() const { return phonemic; }
    int getNumSyl() const { return numSyl; }
};

#endif
