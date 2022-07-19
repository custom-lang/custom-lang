#include "consonant.h"

#include <map>

/*
 * Calculate Consonant ID
 *
 * FORMAT
 *  Index:   |0      |1      |2     |3      |4      |5        |6   |
 *  Feature: |Release|Voicing|Manner|Sec Art|Pri Art|Airstream|Type|
 */

unsigned int Consonant::calc_id() const {
    return (static_cast<int>(release) * 0x1000000) + (static_cast<int>(voicing) * 0x100000)
            + (static_cast<int>(manner) * 0x10000) + (static_cast<int>(sec_art) * 0x1000)
            + (static_cast<int>(pri_art) * 0x100) + (static_cast<int>(airstream) * 0x10)
            + static_cast<int>(Type::consonant);
}

std::string Consonant::update_desc() const {
    std::string desc;
    std::string art;

    // Determine name of articulation
    if (pri_art == Articulation::body_central && sec_art == Articulation::lips_round) {
        art = "Labial-Velar ";
    } else if (pri_art == Articulation::body_front && sec_art == Articulation::lips_round) {
        art = "Labial-Palatal ";
    } else if (pri_art == Articulation::blade_grooved && sec_art == Articulation::body_front) {
        art = "Alveolo-Palatal ";
    } else if (pri_art == Articulation::blade_laminal && sec_art == Articulation::body_front) {
        art = "Palato-Alveolar ";
    } else if (sec_art != static_cast<Articulation>(0) && sec_art != Articulation::blade_lateral) {
        art = get_art_as_str(pri_art) + " and " + (get_art_as_str(sec_art) + " ");
    } else {
        art = get_art_as_str(pri_art) + " " + (sec_art != static_cast<Articulation>(0) ? get_art_as_str(sec_art) + " " : "");
    }

    std::string str_air = airstream == Airstream::pul_egr ? "" : (manner != static_cast<Manner>(0) ? " " : "") + get_air_as_str(airstream);
    std::string str_man = get_man_as_str(manner);
    std::string str_voi = airstream != Airstream::pul_egr ? "" : get_voi_as_str(voicing) + " ";
    std::string str_rel = release != static_cast<Release>(0) ? get_rel_as_str(release) + " " : "";

    desc = str_rel + str_voi + art + str_man + str_air;

    return desc;
}

std::string Consonant::get_air_as_str(Airstream airstream) const {
    std::string str;
    switch (airstream) {
        case Airstream::pul_egr:
            str = "Pulmonic";
            break;
        case Airstream::glo_egr:
            str = "Ejective";
            break;
        case Airstream::glo_ing:
            str = "Implosive";
            break;
        case Airstream::vel_ing:
            str = "Click";
            break;
        default:
            str = "NA";
    }

    return str;
}

std::string Consonant::get_art_as_str(Articulation articulation) const {
    std::string str;
    switch (articulation) {
        case Articulation::lips_round:
            str = "Bilabial";
            break;
        case Articulation::lips_neutral:
            str = "Bilabial";
            break;
        case Articulation::lips_drawn:
            str = "Labiodental";
            break;
        case Articulation::blade_neutral:
            str = "Dental";
            break;
        case Articulation::blade_grooved:
            str = "Alveolar";
            break;
        case Articulation::blade_lateral:
            str = "Lateral";
            break;
        case Articulation::blade_laminal:
            str = "Postalveolar";
            break;
        case Articulation::blade_retroflex:
            str = "Retroflex";
            break;
        case Articulation::body_front:
            str = "Palatal";
            break;
        case Articulation::body_central:
            str = "Velar";
            break;
        case Articulation::body_back:
            str = "Uvular";
            break;
        case Articulation::root_rtr:
            str = "Pharyngeal";
            break;
        case Articulation::larynx:
            str = "Glottal";
            break;
        default:
            str = "";
    }

    return str;
}

std::string Consonant::get_man_as_str(Manner manner) const {
    std::string str;
    switch (manner) {
        case Manner::plosive:
            str = "Plosive";
            break;
        case Manner::nasal:
            str = "Nasal";
            break;
        case Manner::affricate:
            str = "Affricate";
            break;
        case Manner::fricative:
            str = "Fricative";
            break;
        case Manner::trill:
            str = "Trill";
            break;
        case Manner::flap:
            str = "Flap";
            break;
        case Manner::liquid:
            str = "Liquid";
            break;
        case Manner::glide:
            str = "Glide";
            break;
        default:
            str = "";
    }

    return str;
}

std::string Consonant::get_rel_as_str(Release rel) const {
    std::string str;
    switch (rel) {
        case Release::aspirated:
            str = "Aspirated";
            break;
        case Release::nasal:
            str = "Nasal-release";
            break;
        case Release::lateral:
            str = "Lateral-release";
            break;
        case Release::nad:
            str = "Not-audibly-released";
            break;
        default:
            str = "";
    }

    return str;
}
