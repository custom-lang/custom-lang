#ifndef CONSONANT_H
#define CONSONANT_H

#include "phoneme.h"

/* Where obstruction occurs during consonant production */
enum class Place {
    bilabial,
    labiodental,
    dental,
    alveolar,
    postAlveolar,
    retroflex,
    labPalatal,     // Labialized Palatal
    postalvVel,     // Postalveolar Velar
    alvPalatal,     // Alveolo Palatal
    palatal,
    labialVelar,
    velar,
    uvular,
    pharyngeal,
    epiglottal,
    glottal
};

/* How articulators are used during consonant production */
enum class Manner {
    plosive,
    implosive,
    ejective,
    nasal,
    trill,
    flap,
    latFlap,
    fricative,
    latFricative,
    ejecFricative,
    ejecLatFricative,
    affricate,
    approximant,
    latApproximant,
    click,
    latClick
};

enum class Articulation {
    linguolabial = 1,
    apical,
    laminal,
    advanced,
    retracted,
    centralized,
    midCentralized,
    raised,
    lowered
};

enum class Release {
    aspirated = 1,
    nasalRel,
    latRel,
    noAudRel
};

class Consonant : public Phoneme {
private:
    Place place;
    Manner manner;
    Voicing voicing;
    Coarticulation coart;
    Articulation art;
    Release release;
    bool syllabic;

    std::string getStrPlace(Place) const;
    std::string getStrManner(Manner) const;
    std::string getStrArt(Articulation) const;
    std::string getStrRel(Release) const;
public:
    Consonant(std::string symbol, float freq, Place place, Manner manner, Voicing voicing,
              Coarticulation coart, Articulation art, Release release, bool syllabic) {

        type = Type::consonant;
        this->symbol = symbol;
        this->freq = freq;
        this->place = place;
        this->manner = manner;
        this->voicing = voicing;
        this->coart = coart;
        this->art = art;
        this->release = release;
        this->syllabic = syllabic;

        // Syllabic|Release|Art|Coart|Voicing|Manner|Place|Type
        id = (syllabic * 0x10000000) + (static_cast<int>(release) * 0x1000000) + (static_cast<int>(art) * 0x100000)
             + (static_cast<int>(coart) * 0x10000) + (static_cast<int>(voicing) * 0x1000)
             + (static_cast<int>(manner) * 0x100) + (static_cast<int>(place) * 0x10)
             + static_cast<int>(Type::consonant);

        std::string srel = getStrRel(release);
        std::string sart = getStrArt(art);
        std::string scoart = getStrCoart(coart);
        std::string svoic = getStrVoicing(voicing);

        desc += (syllabic ? "Syllabic " : "") + std::string(srel != "" ? srel + " " : "") + (sart != "" ? sart + " " : "")
                + (scoart != "" ? scoart + " " : "") + (svoic != "" ? svoic + " " : "") + getStrPlace(place) + " " + getStrManner(manner);
    }

    Consonant() {}

    Place getPlace() { return place; }
    Manner getManner() { return manner; }
    Voicing getVoicing() { return voicing; }
    Coarticulation getCoart() { return coart; }
    Articulation getArt() { return art; }
    Release getRelease() { return release; }
    bool isSyllabic() { return syllabic; }
};

#endif
