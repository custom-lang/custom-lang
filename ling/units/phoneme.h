#ifndef PHONEME_H
#define PHONEME_H

#include <string>

enum class Type {
    consonant = 1,
    vowel
};

enum class Voicing {
    voiceless = 1,
    voiced
};

/* Represents a discrete unit of sound */
class Phoneme {
protected:
    Type type;
    Voicing voicing;
    unsigned int id;
    std::string symbol;
    std::string desc;

    virtual unsigned int calc_id() const = 0;
    virtual std::string update_desc() const = 0;
    std::string get_voi_as_str(Voicing) const;
public:
    Type get_type() const { return type; }
    Voicing get_voicing() const { return voicing; }
    unsigned int get_id() const { return id; }
    std::string get_symbol() const { return symbol; }
    std::string get_desc() const { return desc; }
};

#endif
