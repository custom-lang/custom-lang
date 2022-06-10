#ifndef CONSONANT_H
#define CONSONANT_H

#include "phoneme.h"

enum class Airstream {
    pul_egr = 1,    // Pulmonic egressive (Pulmonic)
    glo_egr,        // Glottallic egressive (Ejective)
    glo_ing,        // Glottallic ingressive (Implosive)
    vel_ing         // Velaric ingressive (Click)
};

/*
 * Represents degree of closure,
 * Ordered from full closure to no closure.
 */
enum class Manner {
    plosive = 1,
    nasal,
    affricate,
    fricative,
    trill,
    flap,
    liquid,
    glide
};

/*
 * Represents places of articulation
 */
enum class Articulation {
    lips_round = 1,     // Bilabial
    lips_neutral,       // Bilabial
    lips_drawn,         // Labiodental
    blade_neutral,      // Dental
    blade_grooved,      // Alveolar
    blade_lateral,      // Alveolar
    blade_laminal,      // Postalveolar
    blade_retroflex,    // Retroflex
    body_front,         // Palatal
    body_central,       // Velar
    body_back,          // Uvular
    root_rtr,           // Pharyngeal (Retracted tongue root)
    larynx              // Glottal
};

enum class Release {
    aspirated = 1,
    nasal,
    lateral,
    nad             // No audible release
};

/* Represents a consonant */
class Consonant : public Phoneme {
private:
    // Consonant Features
    Airstream airstream;
    Articulation pri_art;       // Primary Articulation (Place)
    Articulation sec_art;       // Secondary Articulation (Double or secondary articulation)
    Manner manner;
    Release release;

    unsigned int calc_id() const;

    // Helpers for creating consonant description from features
    std::string get_air_as_str(Airstream) const;
    std::string get_art_as_str(Articulation) const;
    std::string get_man_as_str(Manner) const;
    std::string get_rel_as_str(Release) const;
    std::string update_desc() const;
public:
    Consonant(std::string symbol,
              Airstream airstream, Articulation pri_art, Articulation sec_art,
              Manner manner, Voicing voicing, Release release) {

        type = Type::consonant;
        this->symbol = symbol;
        this->airstream = airstream;
        this->pri_art = pri_art;
        this->sec_art = sec_art;
        this->manner = manner;
        this->voicing = voicing;
        this->release = release;

        id = calc_id();
        desc = update_desc();
    }

    Consonant() {}

    bool operator==(const Consonant& consonant) {
        return this->get_id() == consonant.get_id() ? true : false;
    }

    Airstream get_airstream() const { return airstream; }
    Articulation get_pri_art() const { return pri_art; }
    Articulation get_sec_art() const { return sec_art; }
    Manner get_manner() const { return manner; }
    Release get_release() const { return release; }

    void set_symbol(std::string symbol) { this->symbol = symbol; }
    void set_airstream(Airstream airstream) { this->airstream = airstream; id = calc_id(); desc = update_desc(); }
    void set_pri_art(Articulation pri_art) { this->pri_art = pri_art; id = calc_id(); desc = update_desc(); }
    void set_sec_art(Articulation sec_art) { this->sec_art = sec_art; id = calc_id(); desc = update_desc(); }
    void set_manner(Manner manner) { this->manner = manner; id = calc_id(); desc = update_desc(); }
    void set_voicing(Voicing voicing) { this->voicing = voicing; id = calc_id(); desc = update_desc(); }
    void set_release(Release release) { this->release = release; id = calc_id(); desc = update_desc(); }
};

#endif
