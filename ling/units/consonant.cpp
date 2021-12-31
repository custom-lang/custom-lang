#include "consonant.h"
#include <map>

std::string Consonant::getStrPlace (Place place) const {
    static std::map<Place, std::string> places {
        {Place::bilabial, "Bilabial"},
        {Place::labiodental, "Labiodental"},
        {Place::dental, "Dental"},
        {Place::alveolar, "Alveolar"},
        {Place::postAlveolar, "Post-Alveolar"},
        {Place::retroflex, "Retroflex"},
        {Place::labPalatal, "Labialized Palatal"},
        {Place::postalvVel, "Postalveolar-Velar"},
        {Place::alvPalatal, "Alveolo-Palatal"},
        {Place::palatal, "Palatal"},
        {Place::labialVelar, "Labial-Velar"},
        {Place::velar, "Velar"},
        {Place::uvular, "Uvular"},
        {Place::pharyngeal, "Pharyngeal"},
        {Place::epiglottal, "Epiglottal"},
        {Place::glottal, "Glottal"}
    };

    std::map<Place, std::string>::iterator it;

    it = places.find(place);
    return it != places.end() ? it->second : "NA";
}

std::string Consonant::getStrManner(Manner manner) const {
    static std::map<Manner, std::string> manners {
        {Manner::plosive, "Plosive"},
        {Manner::implosive, "Implosive"},
        {Manner::ejective, "Ejective"},
        {Manner::nasal, "Nasal"},
        {Manner::trill, "Trill"},
        {Manner::flap, "Flap"},
        {Manner::latFlap, "Lateral Flap"},
        {Manner::fricative, "Fricative"},
        {Manner::latFricative, "Lateral Fricative"},
        {Manner::ejecFricative, "Ejective Fricative"},
        {Manner::ejecLatFricative, "Ejective Lateral Fricative"},
        {Manner::affricate, "Affricate"},
        {Manner::approximant, "Approximant"},
        {Manner::latApproximant, "Lateral Approximant"},
        {Manner::click, "Click"},
        {Manner::latClick, "Lateral Click"}
    };

    std::map<Manner, std::string>::iterator it;

    it = manners.find(manner);
    return it != manners.end() ? it->second : "NA";
}

std::string Consonant::getStrArt(Articulation art) const {
    static std::map<Articulation, std::string> arts {
        {Articulation::linguolabial, "Linguolabial"},
        {Articulation::apical, "Apical"},
        {Articulation::laminal, "Laminal"},
        {Articulation::advanced, "Advanced"},
        {Articulation::retracted, "Retracted"},
        {Articulation::centralized, "Centralized"},
        {Articulation::midCentralized, "Mid-centralized"},
        {Articulation::raised, "Raised"},
        {Articulation::lowered, "Lowered"}
    };

    std::map<Articulation, std::string>::iterator it;

    it = arts.find(art);
    return it != arts.end() ? it->second : "";
}

std::string Consonant::getStrRel(Release rel) const {
    static std::map<Release, std::string> rels {
        {Release::aspirated, "Aspirated"},
        {Release::nasalRel, "Nasal-release"},
        {Release::latRel, "Lateral-release"},
        {Release::noAudRel, "No-audible-release"}
    };

    std::map<Release, std::string>::iterator it;

    it = rels.find(rel);
    return it != rels.end() ? it->second : "";
}
