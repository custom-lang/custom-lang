#include <map>

#include "phoneme.h"

std::string Phoneme::getStrVoicing(Voicing voic) const{
    static std::map<Voicing , std::string> voics {
        {Voicing::voiceless, "Voiceless"},
        {Voicing::voiced, "Voiced"}
    };

    std::map<Voicing, std::string>::iterator it;

    it = voics.find(voic);
    return it != voics.end() ? it->second : "";
}

std::string Phoneme::getStrCoart(Coarticulation coart) const {
    static std::map<Coarticulation, std::string> coarts {
        {Coarticulation::labialized, "Labialized"},
        {Coarticulation::palatalized, "Palatalized"},
        {Coarticulation::velarized, "Velarized"},
        {Coarticulation::pharyngealized, "Pharyngealized"},
        {Coarticulation::velPha, "Velar. or Phary."},
        {Coarticulation::rhoticized, "Rhoticized"},
        {Coarticulation::nasalized, "Nasalized"}
    };

    std::map<Coarticulation, std::string>::iterator it;

    it = coarts.find(coart);
    return it != coarts.end() ? it->second : "";
}
