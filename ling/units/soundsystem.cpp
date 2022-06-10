#include "soundsystem.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool SoundSystem::is_valid_symbol(std::string str) const {
    if (str.length() > 0 && str.length() <= MAX_PHONEME_LENGTH
        && str.find(" ") == std::string::npos) {

        return true;
    } else {
        return false;
    }
}

bool SoundSystem::save() {

    // Do not overwrite file if the soundsystem no longer contains phonemes
    if (consonants.size() == 0 || vowels.size() == 0) {
        std::cerr << "Cannot save " << name << "'s phonemes, consonants and/or vowels is empty.\n";
        return true;
    }

    std::ofstream f_consonants("langs/" + name + "/units/consonants.csv");
    std::ofstream f_vowels("langs/" + name + "/units/vowels.csv");

    // Do not progress if any of the files could not be opened
    if (!f_consonants.is_open() || !f_vowels.is_open()) {
        return true;
    }

    f_vowels << "symbol,id\n";
    f_consonants << "symbol,id\n";

    // Save consonants
    for (auto const& phon: consonants) {
        f_consonants << phon.second.get_symbol() << ","
                    << std::hex << phon.second.get_id() << "\n";
    }

    // Save vowels
    for (auto const& phon: vowels) {
        f_vowels << phon.second.get_symbol() << ","
                << std::hex << phon.second.get_id() << "\n";
    }

    f_consonants.close();
    f_vowels.close();

    return false;
}

bool SoundSystem::load() {
    // Open all files
    std::ifstream f_consonants("langs/" + name + "/units/consonants.csv");
    std::ifstream f_vowels("langs/" + name + "/units/vowels.csv");

    // Do not progress if any of the files could not be opened
    if (!f_consonants.is_open() || !f_vowels.is_open()) {
        return true;
    }

    // Traverse Files
    read_file(f_consonants, 1);
    read_file(f_vowels, 2);

    f_consonants.close();
    f_vowels.close();

    return false;
}

void SoundSystem::read_file(std::ifstream& file, int type) {
    std::string line;
    bool isFirstLine = true;

    // Check if type passed in is valid (1-2 inclusive)
    if (type < 1 || type > 2) {
        std::cerr << "Failed to traverse file, given type: " << type << "\n";
        return;
    }

    while(getline(file, line)) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;

        // Tokenize line
        while(getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Ignore first line
        if (isFirstLine) {
            isFirstLine = false;
            continue; // Skip to next line
        }

        /*
         * Check if the line has the correct amount of values.
         * Each file has at least 2 arguments per line.
         * Extra values will be ignored and eventually deleted when writing over the file.
         */
        if (tokens.size() < 2) {
            std::cerr << "Invalid amount of values: " << tokens.size() << "\n";
            continue; // Skip to next line
        }

        // Check if the phoneme symbol is valid. If not, do not attempt to insert it.
        if (is_valid_symbol(tokens[0]) == false) {
            std::cerr << "The symbol [" << tokens[0] << "] is invalid\n";
            continue; // Skip to next line
        }

        // Try to convert id from string to unsigned int
        unsigned int id;
        try {
            id = std::stoi(tokens[1], nullptr, 16);
        } catch (...) {
            std::cerr << "Failed to convert '" << tokens[1] << "' into an integer id\n";
            continue; // Skip to next line
        }

        // Insert based on type
        if (type == 1) {
            if (insert_consonant(tokens[0], id)) {
                std::cerr << "Could not add the consonant [" << tokens[0]
                          << "] with id " << std::hex << id << "\n";
            }
        } else {
            if (insert_vowel(tokens[0], id)) {
                std::cerr << "Could not add the vowel [" << tokens[0]
                          << "] with id " << std::hex << id << "\n";
            }
        }
    }
}

bool SoundSystem::insert_consonant(std::string symbol, unsigned int id) {

    // Check if consonant id
    if (id % 0x10 != static_cast<int>(Type::consonant)) {
        return true;
    }

    // Get fields from id
    Airstream air = static_cast<Airstream>(id % 0x100 / 0x10);
    Articulation pri_art = static_cast<Articulation>(id % 0x1000 / 0x100);
    Articulation sec_art = static_cast<Articulation>(id % 0x10000 / 0x1000);
    Manner manner = static_cast<Manner>(id % 0x100000 / 0x10000);
    Voicing voicing = static_cast<Voicing>(id % 0x1000000 / 0x100000);
    Release release = static_cast<Release>(id % 0x10000000 / 0x1000000);

    // Insert only if id is valid
    if (air >= Airstream::pul_egr && air <= Airstream::vel_ing
        && pri_art >= Articulation::lips_round && pri_art <= Articulation::larynx
        && sec_art >= static_cast<Articulation>(0) && sec_art <= Articulation::larynx
        && manner >= static_cast<Manner>(0) && manner <= Manner::glide
        && voicing >= static_cast<Voicing>(0) && voicing <= Voicing::voiced
        && release >= static_cast<Release>(0) && release <= Release::nad) {

        // Insert Consonant
        Consonant consonant(symbol, air, pri_art, sec_art,
                            manner, voicing, release);

        consonants.insert(std::pair<unsigned int, Consonant>(id, consonant));
        ids.insert(std::pair<std::string, unsigned int>(symbol, id));

        return false;
    } else {
        return true;
    }
}

bool SoundSystem::insert_vowel(std::string symbol, unsigned int id) {

    // Check if vowel id
    if (id % 0x10 != static_cast<int>(Type::vowel)) {
        return true;
    }

    // Get fields from id
    Height height = static_cast<Height>(id % 0x100 / 0x10);
    Backness backness = static_cast<Backness>(id % 0x1000 / 0x100);
    int rounded = (id % 0x10000 / 0x1000) - 1;
    Voicing voicing = static_cast<Voicing>(id % 0x100000 / 0x10000);
    Length length = static_cast<Length>(id % 0x1000000 / 0x100000);
    int nasalized = (id % 0x10000000 / 0x1000000) - 1;
    int rhotic = (id % 0x100000000 / 0x10000000) - 1;

    // Insert only if id is valid
    if (height >= Height::close && height <= Height::open
        && backness >= Backness::front && backness <= Backness::back
        && rounded >= 0 && rounded <= 1
        && voicing >= Voicing::voiceless && voicing <= Voicing::voiced
        && length >= Length::extra_short && length <= Length::len_long
        && nasalized >= 0 && nasalized <= 1
        && rhotic >= 0 && rhotic <= 1) {

        // Insert vowel
        Vowel vowel(symbol, height, backness, (bool)rounded, voicing,
                    length, (bool)nasalized, (bool)rhotic);

        vowels.insert(std::pair<unsigned int, Vowel>(id, vowel));
        ids.insert(std::pair<std::string, unsigned int>(symbol, id));

        return false;
    } else {
        return true;
    }
}
