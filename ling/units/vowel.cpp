#include "vowel.h"

#include <map>

/*
 * Calculate Vowel ID
 *
 * FORMAT
 *  Index:   |0     |1        |2     |3      |4      |5       |6     |7
 *  Feature: |Rhotic|Nasalized|Length|Voicing|Rounded|Backness|Height|Type
 */

unsigned int Vowel::calc_id() const {
    return ((static_cast<int>(rhotic) + 1) * 0x10000000) + ((static_cast<int>(nasalized) + 1) * 0x1000000)
            + (static_cast<int>(length) * 0x100000) + (static_cast<int>(voicing) * 0x10000)
            + ((static_cast<int>(rounded) + 1 )* 0x1000) + (static_cast<int>(backness) * 0x100)
            + (static_cast<int>(height) * 0x10) + static_cast<int>(Type::vowel);
}

std::string Vowel::update_desc() const {
    std::string desc = std::string(voicing == Voicing::voiceless ? "Voiceless " : "") +
            + (nasalized ? "Nasalized " : "") + (rhotic ? "Rhotic " : "")
            + (length != Length::len_short ? get_length_as_str(length) + " " : "")
            + get_height_as_str(height) + " " + get_backness_as_str(backness) + " "
            + (rounded ? "Rounded" : "Unrounded") + " Vowel";

    return desc;
}

std::string Vowel::get_height_as_str(Height height) const {
    std::string str;
    switch (height) {
        case Height::close:
            str = "Close";
            break;
        case Height::near_close:
            str = "Near-Close";
            break;
        case Height::close_mid:
            str = "Close-Mid";
            break;
        case Height::mid:
            str = "Mid";
            break;
        case Height::open_mid:
            str = "Open-Mid";
            break;
        case Height::near_open:
            str = "Near-Open";
            break;
        case Height::open:
            str = "Open";
            break;
        default:
            str = "NA";
    }

    return str;
}

std::string Vowel::get_backness_as_str(Backness backness) const {
    std::string str;
    switch (backness) {
        case Backness::front:
            str = "Front";
            break;
        case Backness::central:
            str = "Central";
            break;
        case Backness::back:
            str = "Back";
            break;
        default:
            str = "NA";
    }

    return str;
}

std::string Vowel::get_length_as_str(Length length) const {
    std::string str;
    switch (length) {
        case Length::extra_short:
            str = "Extra-Short";
            break;
        case Length::half_long:
            str = "Half-Long";
            break;
        case Length::len_long:
            str = "Long";
            break;
        default:
            str = "";
    }

    return str;
}
