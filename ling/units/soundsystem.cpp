#include "soundsystem.h"

/* A phoneme symbol is valid if its length is between
 * 1 and the MAX_PHONEME_LENGTH (inclusive), is not the empty string
 * and does not contain spaces.
 */
bool SoundSystem::isValidSym(std::string str) const{
    if (str.length() > 0 && str.length() <= MAX_PHONEME_LENGTH
        && str.find(" ") == std::string::npos) {

        return true;
    } else {
        return false;
    }
}

/* Save all phonemes into a csv for the corresponding language
 *
 * Return true if file couldnt be opened, consonants is empty, or vowels is empty
 * Return false otherwise
 */
bool SoundSystem::save() {
    std::ofstream phonemes("langs/" + name + "/phonemes.csv");

    if (!phonemes.is_open()) {
        return true;
    }

    if (consonants.size() == 0 || vowels.size() == 0) {
        std::cerr << "Cannot save " << name << " phonemes, consonants and/or vowels is empty.\n";
        return true;
    }

    // Save consonants
    for (auto const& phon: consonants) {
        phonemes << phon.second.getSymbol() << ","
                 << std::hex << phon.second.getId() << ","
                 << std::dec << phon.second.getFreq() << "\n";
    }

    // Save vowels
    for (auto const& phon: vowels) {
        phonemes << phon.second.getSymbol() << ","
                 << std::hex << phon.second.getId() << ","
                 << std::dec << phon.second.getFreq() << "\n";
    }

    // Save suprasegmentals
    for (auto const& seg: suprasegmentals) {
        phonemes << seg.second.getSymbol() << ","
                 << std::hex << seg.second.getId() << ","
                 << std::dec << seg.second.getFreq() << "\n";
    }

    phonemes.close();
    return false;
}

/* Load phonemes from a csv from the corresponding language
 *
 * Return true if file couldnt be opened
 * Return false otherwise
 */
bool SoundSystem::load() {
    std::ifstream phonemes("langs/" + name + "/phonemes.csv");

    if (!phonemes.is_open()) {
        return true;
    }

    // Keep track of frequency, both cannot be over 1
    float totConFreq = 0;
    float totVowFreq = 0;

    // Read each line in file
    std::string line;
    while (getline(phonemes, line)) {
        std::vector<std::string> tokens;
        std::stringstream ss(line);
        std::string token;

        // Tokenize line
        while(getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        // Check if line has correct amount of values
        if (tokens.size() != 3) {
            std::cerr << "Invalid amount of values: " << tokens.size() << "\n";
            continue; // Skip to next line
        }

        // Check if phoneme symbol is valid. If not, do not attempt to insert it.
        if (isValidSym(tokens[0]) == false) {
            std::cerr << "The symbol [" << tokens[0] << "] is invalid\n";
            continue; // Skip to next line
        }

        // Try to convert id from string to unsigned int
        unsigned int id;
        try {
            id = std::stoi(tokens[1], nullptr, 16);
        } catch (...) {
            std::cerr << "Could not convert '" << tokens[1] << "' into an integer id\n";
            continue; // Skip to next line
        }

        // Try to convert frequency from string to float
        float freq;
        try {
            freq = std::stof(tokens[2], nullptr);
        } catch (...) {
            std::cerr << "Could not convert '" << tokens[2] << "' into a float\n";
            continue; // Skip to next line
        }

        int type = id % 0x10;

        // Add frequency %, do not add phoneme if % exceeds 1
        if (type == static_cast<int>(Type::consonant)) {

            // Check if adding causes total % to exceed 1
            if (totConFreq + freq <= 1) {
                totConFreq += freq;
            } else {
                std::cerr << "Cannot add any more consonants, total consonant relative frequency > 1\n";
            }
        } else if (type == static_cast<int>(Type::vowel)) {

            // Check if adding causes total % to exceed 1
            if (totVowFreq + freq <= 1) {
                totVowFreq += freq;
            } else {
                std::cerr << "Cannot add any more vowels, total vowel relative frequency > 1\n";
            }
        } else if (type != 0) {
            // ID does not denote a valid type, skip to next line
            std::cerr << std::hex << id % 0x10 << " is not a valid type.\n";
            continue;
        }

        // Insert based on type
        if (id % 0x10 == static_cast<int>(Type::consonant)) {
            // Get fields from id
            Place place = (Place)(id % 0x100 / 0x10);
            Manner manner = (Manner)(id % 0x1000 / 0x100);
            Voicing voicing = (Voicing)(id % 0x10000 / 0x1000);
            Coarticulation coarticulation = (Coarticulation)(id % 0x100000 / 0x10000);
            Articulation articulation = (Articulation)(id % 0x1000000 / 0x100000);
            Release release = (Release)(id % 0x10000000 / 0x1000000);
            unsigned int isSyllabic = (id % 0x100000000 / 0x10000000);

            // Insert only if id is valid
            if (voicing < Voicing::voiceless || voicing > Voicing::voiced
                || place < Place::bilabial || place > Place::glottal
                || manner < Manner::plosive || manner > Manner::latClick
                || articulation > Articulation::lowered
                || coarticulation > Coarticulation::nasalized
                || release > Release::noAudRel
                || isSyllabic < 0   || isSyllabic > 1) {

                std::cerr << "Could not add the consonant [" << tokens[0] << "] with id " << std::hex << id << "\n";
            } else {
                // Insert Consonant
                Consonant consonant(tokens[0], freq, place, manner, voicing, coarticulation, articulation, release, (bool)isSyllabic);

                consonants.insert(std::pair<unsigned int, Consonant>(id, consonant));
                ids.insert(std::pair<std::string, unsigned int>(tokens[0], id));
            }
        } else if (id % 0x10 == static_cast<int>(Type::vowel)) {
            // Get fields from id
            Height height = (Height)(id % 0x100 / 0x10);
            Backness back = (Backness)(id % 0x1000 / 0x100);
            Voicing voicing = (Voicing)(id % 0x10000 / 0x1000);
            Rounding rounding = (Rounding)(id % 0x100000 / 0x10000);
            Coarticulation coarticulation = (Coarticulation)(id % 0x1000000 / 0x100000);
            TongueRoot root = (TongueRoot)(id % 0x10000000 / 0x1000000);

            // Insert only if id is valid
            if (voicing < Voicing::voiceless || voicing > Voicing::voiced
                || height < Height::close || height > Height::open
                || back < Backness::front || back > Backness::back
                || rounding < Rounding::unrounded || rounding > Rounding::moreRounded
                || coarticulation > Coarticulation::nasalized
                || root > TongueRoot::advanced) {

                std::cerr << "Could not add the vowel [" << tokens[0] << "] with id " << std::hex << id << "\n";
            } else {
                // Insert vowel
                Vowel vowel(tokens[0], freq, height, back, voicing, rounding, coarticulation, root);

                vowels.insert(std::pair<unsigned int, Vowel>(id, vowel));
                ids.insert(std::pair<std::string, unsigned int>(tokens[0], id));
            }
        } else {
            // Get fields from id
            SupraType supraType = (SupraType)(id % 0x1000 / 0x10);
            Feature feature = Feature::intonation;

            // Get feature from suprasegmental range
            switch (static_cast<int>(supraType)) {
                case 0:
                    feature = Feature::tone;
                case 6:
                    break;
                case 7:
                    feature = Feature::length;
                case 10:
                    break;
                case 11:
                    feature = Feature::stress;
                case 12:
                    break;
            }

            if (supraType < SupraType::extraLow
                || supraType > SupraType::globalRise) {

                std::cerr << "Could not add the suprasegmental " << tokens[0] << " with id " << std::hex << id << "\n";
            } else {
                // Insert suprasegmental
                Suprasegmental supraseg(tokens[0], freq, feature, supraType);

                suprasegmentals.insert(std::pair<unsigned int, Suprasegmental>(id, supraseg));
                ids.insert(std::pair<std::string, unsigned int>(tokens[0], id));
            }
        }
    }
    phonemes.close();

    if (totConFreq < 1) {
        std::cerr << "WARNING: The sum of all consonant relative frequencies is less than 1, at "
                  << totConFreq << "\n";
    }

    if (totVowFreq < 1) {
        std::cerr << "WARNING: The sum of all vowel relative frequencies is less than 1, at "
                  << totVowFreq << "\n";
    }

    return false;
}
