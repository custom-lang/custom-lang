#include <map>

#include "phoneme.h"

std::string Phoneme::get_voi_as_str(Voicing voicing) const {
    std::string str;
    switch (voicing) {
        case Voicing::voiceless:
            str = "Voiceless";
            break;
        case Voicing::voiced:
            str = "Voiced";
            break;
        default:
            str = "";
    }

    return str;
}
