#ifndef PHONEME_H
#define PHONEME_H

#include <string>

// Start at 1, since 0 is reserved for suprasegmentals
enum class Type {
    consonant = 1,
    vowel
};

enum class Voicing {
    voiceless,
    voiced
};

// Secondary Articulation
enum class Coarticulation {
    labialized = 1,
    palatalized,
    velarized,
    pharyngealized,
    velPha,            //Velarized or Pharyngealized
    rhoticized,
    nasalized
};

/* Represents a discrete unit of sound */
class Phoneme {
protected:
    Type type;
    unsigned int id;
    float freq;
    std::string symbol;
    std::string desc;
    Voicing voicing;

    std::string getStrVoicing (Voicing) const;
    std::string getStrCoart (Coarticulation) const;
public:
    Type getType() const { return type; }
    unsigned int getId() const { return id; }
    float getFreq() const { return freq; }
    std::string getSymbol() const { return symbol; }
    std::string getDesc() const { return desc; }
    Voicing getVoicing() const { return voicing; }
};

#endif
