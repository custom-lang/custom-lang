#ifndef VOWEL_H
#define VOWEL_H

#include "phoneme.h"

/* Tongue height during vowel production */
enum class Height {
    close,
    nearClose,
    closeMid,
    mid,
    openMid,
    nearOpen,
    open
};

/* Tongue position during vowel production */
enum class Backness {
    front,
    central,
    back
};

// Vowel lip rounding
enum class Rounding {
    unrounded,
    lessRounded,
    rounded,
    moreRounded
};

enum class TongueRoot {
    retracted = 1,
    advanced
};

class Vowel : public Phoneme {
private:
    Height height;
    Backness backness;
    Rounding rounding;
    Coarticulation coart;
    TongueRoot root;

    std::string getStrHeight(Height) const;
    std::string getStrBackness(Backness) const;
    std::string getStrRounding(Rounding) const;
    std::string getStrTngRoot(TongueRoot) const;
public:
    Vowel(std::string symbol, float freq, Height height, Backness backness, Voicing voicing,
          Rounding rounding, Coarticulation coart, TongueRoot root) {

        type = Type::vowel;
        this->symbol = symbol;
        this->freq = freq;
        this->height = height;
        this->backness = backness;
        this->rounding = rounding;
        this->coart = coart;
        this->root = root;

        // |Tongue Root|Coarticulation|Rounding|Voicing|Part|Height|Type
        id = (static_cast<int>(root) * 0x1000000) + (static_cast<int>(coart) * 0x100000)
            + (static_cast<int>(rounding) * 0x10000) + (static_cast<int>(voicing) * 0x1000) + (static_cast<int>(backness) * 0x100)
            + (static_cast<int>(height) * 0x10) + static_cast<int>(Type::vowel);

        std::string svoic = getStrVoicing(voicing);
        std::string scoart = getStrCoart(coart);
        std::string stng = getStrTngRoot(root);

        desc += (stng != "" ? stng + " " : "" ) + (scoart != "" ? scoart + " " : "" ) + (svoic != "Voiced" ? svoic + " " : "" )
             + getStrHeight(height) + " " + getStrBackness(backness) + " " + getStrRounding(rounding) + " Vowel";
    }

    Vowel() {}

    Height getHeight() const { return height; }
    Backness getBackness() const { return backness; }
    Rounding getRounding() const { return rounding; }
    Coarticulation getCoart() const { return coart; }
    TongueRoot getTngRoot() const { return root; }
};

#endif
