#ifndef VOWEL_H
#define VOWEL_H

#include "phoneme.h"

/* Tongue height during vowel production */
enum class Height {
    close = 1,
    near_close,
    close_mid,
    mid,
    open_mid,
    near_open,
    open
};

/* Tongue position during vowel production */
enum class Backness {
    front = 1,
    central,
    back
};

/* Represents vowel length as a contrastive feature */
enum class Length {
    extra_short = 1,
    len_short,
    half_long,
    len_long
};

/* Represents a vowel */
class Vowel : public Phoneme {
private:
    Height height;
    Backness backness;
    Length length;
    bool rounded;
    bool nasalized;
    bool rhotic;

    float probNucleus;      // Probabilty of phoneme occuring in nucleus

    unsigned int calc_id() const;

    std::string get_height_as_str(Height) const;
    std::string get_backness_as_str(Backness) const;
    std::string get_length_as_str(Length) const;
    std::string update_desc() const;
public:
    Vowel(std::string symbol, Height height, Backness backness, bool rounded,
          Voicing voicing, Length length, bool nasalized, bool rhotic,
          float probNucleus) {

        type = Type::vowel;
        this->symbol = symbol;
        this->height = height;
        this->backness = backness;
        this->rounded = rounded;
        this->voicing = voicing;
        this->length = length;
        this->nasalized = nasalized,
        this->rhotic = rhotic;
        this->probNucleus = probNucleus;

        id = calc_id();

        desc = update_desc();
    }

    Vowel() {}

    bool operator==(const Vowel& vowel) {
        return this->get_id() == vowel.get_id() ? true : false;
    }

    Height get_height() const { return height; }
    Backness get_backness() const { return backness; }
    bool is_rounded() const { return rounded; }
    Length is_length() const { return length; }
    bool is_nasalized() const { return nasalized; }
    bool is_rhotic() const { return rhotic; }

    float getProbNucleus() const { return probNucleus; }

    void set_height(Height height) { this->height = height; id = calc_id(); desc = update_desc(); }
    void set_backness(Backness backness) { this->backness = backness; id = calc_id(); desc = update_desc(); }
    void set_rounded(bool rounded) { this->rounded = rounded; id = calc_id(); desc = update_desc(); }
    void set_length(Length length) { this->length = length; id = calc_id(); desc = update_desc(); }
    void set_nasalized(bool nasalized) { this->nasalized = nasalized; id = calc_id(); desc = update_desc(); }
    void set_rhotic(bool rhotic) { this->rhotic = rhotic; id = calc_id(); desc = update_desc(); }
};

#endif
