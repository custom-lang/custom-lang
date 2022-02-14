#ifndef MORPHEME_H
#define MORPHEME_H

#include <initializer_list>
#include "syllable.h"

enum class MorphType {
    root,
    function,
    derivationalAffix,
    inflectionalAffix,
};

// Content lexical category
enum class ConLexCat {
    noun,
    verb,
    adjective,
    adverb,
};

// Functional lexical category
enum class FuncLexCat {
    determiner,
    preposition,
    pronoun,
    conjunction
};

/*
 * A morpheme represents a sequence of syllables
 * and their transcription.
 *
 */
class Morpheme {
protected:
    MorphType type;
    bool free;

    std::vector<Syllable> syllables;
    std::string phonemic;               // Phonemic transcription of morpheme
    int numSyl;
public:
    void addSyllable(Syllable syl) {
        syllables.push_back(syl);
        // Add syllable breaks
        numSyl > 0 ? phonemic += "." : phonemic += "";
        phonemic += syl.getStrSyl();
        numSyl++;
    }

    bool isFree() const { return free; }
    bool isAffix() const { return type == MorphType::derivationalAffix
                            || type == MorphType::inflectionalAffix ? true : false; }
    MorphType getMorphType() const { return type; }
    std::vector<Syllable> getSyllables() const { return syllables; }
    std::string getPhonemic() const { return phonemic; }        // NOTE Should eventually be moved to the Word class
    int getNumSyl() const { return numSyl; }

    static std::string getStrMorphType(MorphType);
    static std::string getStrConLexCat(ConLexCat);
    static std::string getStrFuncLexCat(FuncLexCat);
};

#endif
