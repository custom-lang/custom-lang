#include "soundsystem.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool SoundSystem::isValidSym(std::string str) const {
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

    std::ofstream fSupras("langs/" + name + "/units/suprasegmentals.csv");
    std::ofstream fConsonants("langs/" + name + "/units/consonants.csv");
    std::ofstream fVowels("langs/" + name + "/units/vowels.csv");

    // Do not progress if any of the files could not be opened
    if (!fSupras.is_open() || !fConsonants.is_open() || !fVowels.is_open() ) {
        return true;
    }

    fVowels << "symbol,id,nucleus weight\n";
    fConsonants << "symbol,id,onset weight,nucleus weight,coda weight\n";
    fSupras << "symbol,id,suprasegmental weight\n";

    // Save consonants
    for (auto const& phon: consonants) {
        fConsonants << phon.second.getSymbol() << ","
                    << std::hex << phon.second.getId() << ","
                    << std::dec << phon.second.getProbOnset() << ","
                    << phon.second.getProbNucleus() << ","
                    << phon.second.getProbCoda() << "\n";
    }

    // Save vowels
    for (auto const& phon: vowels) {
        fVowels << phon.second.getSymbol() << ","
                << std::hex << phon.second.getId() << ","
                << std::dec << phon.second.getProbNucleus() << "\n";
    }

    // Save suprasegmentals
    for (auto const& seg: suprasegmentals) {
        fSupras << seg.second.getSymbol() << ","
                 << std::hex << seg.second.getId() << ","
                 << std::dec << seg.second.getProbSupra() << "\n";
    }

    fSupras.close();
    fConsonants.close();
    fVowels.close();

    return false;
}

bool SoundSystem::load() {
    // Open all files
    std::ifstream fSupras("langs/" + name + "/units/suprasegmentals.csv");
    std::ifstream fConsonants("langs/" + name + "/units/consonants.csv");
    std::ifstream fVowels("langs/" + name + "/units/vowels.csv");

    // Do not progress if any of the files could not be opened
    if (!fSupras.is_open() || !fConsonants.is_open() || !fVowels.is_open() ) {
        return true;
    }

    // Traverse Files
    traverseFile(fSupras, 0);
    traverseFile(fConsonants, 1);
    traverseFile(fVowels, 2);

    fSupras.close();
    fConsonants.close();
    fVowels.close();

    return false;
}

void SoundSystem::traverseFile(std::ifstream& file, int type) {
    std::string line;
    bool isFirstLine = true;

    // Check if type passed in is valid (0-2 inclusive)
    if (type < 0 || type > 2) {
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
         * Each file has at least 3 arguments per line.
         * Extra values will be ignored and eventually deleted when writing over the file.
         */
        if (tokens.size() < 3) {
            std::cerr << "Invalid amount of values: " << tokens.size() << "\n";
            continue; // Skip to next line
        }

        // Check if the phoneme symbol is valid. If not, do not attempt to insert it.
        if (isValidSym(tokens[0]) == false) {
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

        /*
         * Try to convert weights from string to float
         *
         * For vowels:
         *  weight1 = nucleus weight
         * For consonants:
         *  weight1 = onset weight
         *  weight2 = nucleus weight
         *  weight3 = coda weight
         * For suprasegmentals:
         *  weight1 = suprasegmental weight
         */
        float weight1, weight2, weight3;
        try {
            if (type == 1) {
                weight1 = std::stof(tokens[2], nullptr);
                weight2 = std::stof(tokens[3], nullptr); // NOTE: Will not be used until syllabic consonants are supported
                weight3 = std::stof(tokens[4], nullptr);
            } else {
                weight1 = std::stof(tokens[2], nullptr);
            }
        } catch (...) {
            std::cerr << "Failed to convert weight(s) for " + tokens[0] + " into float(s)\n";
            continue; // Skip to next line
        }

        // Insert based on type
        if (type == 0) {
            if (insertSupra(tokens[0], id, weight1)) {
                std::cerr << "Could not add the suprasegmental [" << tokens[0]
                          << "] with id " << std::hex << id << "\n";
            }
        } else if (type == 1) {
            if (insertConsonant(tokens[0], id, weight1, weight2, weight3)) {
                std::cerr << "Could not add the consonant [" << tokens[0]
                          << "] with id " << std::hex << id << "\n";
            }
        } else {
            if (insertVowel(tokens[0], id, weight1)) {
                std::cerr << "Could not add the vowel [" << tokens[0]
                          << "] with id " << std::hex << id << "\n";
            }
        }
    }
}

bool SoundSystem::insertConsonant(std::string symbol, unsigned int id, float probOnset, float probNucleus, float probCoda) {

    // Check if consonant id
    if (id % 0x10 != static_cast<int>(Type::consonant)) {
        return true;
    }

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
        || isSyllabic < 0 || isSyllabic > 1) {

        return true;
    } else {
        // Insert Consonant
        Consonant consonant(symbol, probOnset, probNucleus, probCoda,
                            place, manner, voicing, coarticulation, articulation,
                            release, (bool)isSyllabic);

        probOnsets.push_back(probOnset);
        probCodas.push_back(probCoda);
        onsetIds.push_back(id);
        codaIds.push_back(id);

        consonants.insert(std::pair<unsigned int, Consonant>(id, consonant));
        ids.insert(std::pair<std::string, unsigned int>(symbol, id));

        return false;
    }
}

bool SoundSystem::insertVowel(std::string symbol, unsigned int id, float probNucleus) {

    // Check if vowel id
    if (id % 0x10 != static_cast<int>(Type::vowel)) {
        return true;
    }

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

        return true;
    } else {
        // Insert vowel
        Vowel vowel(symbol, probNucleus, height, back, voicing, rounding, coarticulation, root);
        probNuclei.push_back(probNucleus);
        nucleusIds.push_back(id);

        vowels.insert(std::pair<unsigned int, Vowel>(id, vowel));
        ids.insert(std::pair<std::string, unsigned int>(symbol, id));

        return false;
    }
}

bool SoundSystem::insertSupra(std::string symbol, unsigned int id, float probSupra) {

    // Check if suprasegmental id
    if (id % 0x10 != 0) {
        return true;
    }

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

        return true;
    } else {
        // Insert suprasegmental
        Suprasegmental supraseg(symbol, probSupra, feature, supraType);

        probSupras.push_back(probSupra);
        supraIds.push_back(id);

        suprasegmentals.insert(std::pair<unsigned int, Suprasegmental>(id, supraseg));
        ids.insert(std::pair<std::string, unsigned int>(symbol, id));

        return false;
    }
}
