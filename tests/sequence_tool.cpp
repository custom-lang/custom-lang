#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "../include/soundsystem.h"

template<typename value>
std::set<unsigned int> get_keys(std::map<unsigned int, value> const& map) {
    std::set<unsigned int> output;

    for (auto const& key : map) {
        output.insert(key.first);
    }

    return output;
}

bool insert_sequences(std::string, std::string, std::vector<std::vector<unsigned int>>);

std::vector<std::vector<unsigned int>> create_sequences(std::map<unsigned int, Consonant>&,
                                                        std::map<unsigned int, Vowel>&,
                                                        std::set<unsigned int>&,
                                                        std::vector<unsigned int>&);

void print_ids(std::map<unsigned int, Consonant>&,
            std::map<unsigned int, Vowel>&,
            std::vector<std::vector<unsigned int>>&);

bool in_class(unsigned int, unsigned int);

int main () {

    // Prompt for language
    bool done = false;
    std::string lang;

    std::cout << "Enter Language: \n";
    getline(std::cin, lang);
    std::transform(lang.begin(), lang.end(), lang.begin(), ::tolower);

    SoundSystem sound_system(lang);

    if (sound_system.load()) {
        std::cout << "Could not find language named " + lang + "\n";
        return 0;
    } else {
        std::cout << "\n";
    }

    std::map<unsigned int, Consonant> consonants = sound_system.get_consonants();
    std::map<unsigned int, Vowel> vowels = sound_system.get_vowels();

    std::set<unsigned int> ids = get_keys(consonants);
    std::set<unsigned int> temp = get_keys(vowels);

    ids.insert(temp.begin(), temp.end());

    std::cout << "Commands:\n\n[onset|nucleus|coda] [natural class as ID]+\n"
              << "\tonset 0000011\t\t allow single pulmonic consonants to appear in the onset\n"
              << "\tnucleus 11221172 12\t allow diphtongs of the form [ɑ][high vowel] to appear in the nucleus\n"
              << "\n----------\n";

    while (!done) {
        std::vector<std::string> tokens;
        std::string token;
        std::stringstream ss;
        std::string line;

        getline(std::cin, line);
        ss = std::stringstream(line);

        // Set all characters in line to lowercase
        std::transform(line.begin(), line.end(), line.begin(), ::tolower);

        // Tokenize line
        while(getline(ss, token, ' ')) {
            tokens.push_back(token);
        }

        if (!tokens.empty()) {
            if (tokens[0] == "quit" || tokens[0] == "q") {
                done = true;
            } else if (tokens[0] == "onset" || tokens[0] == "nucleus" || tokens[0] == "coda") {

                if (tokens.size() == 1) {
                    std::cerr << "onset|nucleus|coda requires at least one natural class\n\n";
                } else {
                    int len = tokens.size();
                    bool failed = false;
                    std::vector<unsigned int> classes;

                    // Convert natural class IDS from string to int
                    for (int i = 1; i < len; i++) {
                        try {
                            classes.push_back(std::stoi(tokens[i], nullptr, 16));
                        } catch (...) {
                            std::cerr << "Failed to convert '" << tokens[i] << "' into an integer\n\n";
                            failed = true;
                            break;
                        }
                    }

                    if (!failed) {
                        // Create sequences
                        std::vector<std::vector<unsigned int>> sequences = create_sequences(consonants, vowels, ids, classes);

                        if (sequences.size() != 0) {

                            std::cout << "\nAllow the following phonemes/clusters to occur in the " << tokens[0] << "? y\\n\n";
                            print_ids(consonants, vowels, sequences);

                            // Take input
                            getline(std::cin, line);
                            std::transform(line.begin(), line.end(), line.begin(), ::tolower);

                            if (line == "y") {
                                // Attempt to add sequences
                                int result = insert_sequences(tokens[0], lang, sequences);

                                if (result) {
                                    std::cout << "Added sequences to " << tokens[0] << "\n\n";
                                } else {
                                    std::cout << "There were no new sequences to add to " << tokens[0] << "\n\n";
                                }
                            } else {
                                std::cout << "\n";
                            }
                        } else {
                            std::cout << "Could not find phonemes/sequences of the given class(es)\n\n";
                        }
                    }
                }
            } else {
                std::cout << "Invalid command\n\n";
            }
        }
    }


    return 0;
}

bool insert_sequences(std::string part, std::string lang, std::vector<std::vector<unsigned int>> sequences) {

    int num_added = 0;

    // Read from file
    std::ifstream in_file("langs/" + lang + "/phonology/phonotactics.json");
    json data = json::parse(in_file);
    in_file.close();

    std::vector<std::vector<unsigned int>> old_sequences = data["inventory"][part];

    // Add new sequences to vector
    for (auto const& sequence: sequences) {
        if (std::find(old_sequences.begin(), old_sequences.end(), sequence) == old_sequences.end()) {
            old_sequences.push_back(sequence);
            num_added++;
        }
    }

    if (num_added == 0) {
        return false;
    }

    data["inventory"][part] = old_sequences;

    // Write to file
    std::ofstream out_file("langs/" + lang + "/phonology/phonotactics.json");
    out_file << data.dump(4) << "\n";
    out_file.close();
    return true;
}

std::vector<std::vector<unsigned int>> create_sequences(std::map<unsigned int, Consonant>& consonants,
                    std::map<unsigned int, Vowel>& vowels,
                    std::set<unsigned int>& ids,
                    std::vector<unsigned int>& classes) {

    std::vector<std::vector<unsigned int>> phonemes;

    // Search for phonemes
    for (auto const& phon_class: classes) {
        std::vector<unsigned int> temp;

        for (auto const& id: ids) {
            if (in_class(id, phon_class)) {
                temp.push_back(id);
            }
        }

        phonemes.push_back(temp);
    }

    // Create sequences
    std::vector<std::vector<unsigned int>> sequences(1);

    for (int i = 0; i < phonemes.size(); i++) {
        std::vector<std::vector<unsigned int>> temp;

        for (auto const& sequence: sequences) {
            for (auto const& phoneme: phonemes[i]) {

                std::vector<unsigned int> temp_seq = sequence;
                temp_seq.push_back(phoneme);
                temp.push_back(temp_seq);
            }
        }
        sequences = temp;
    }

    return sequences;
}

void print_ids(std::map<unsigned int, Consonant>& consonants,
            std::map<unsigned int, Vowel>& vowels,
            std::vector<std::vector<unsigned int>>& ids) {

    for (auto const& vec: ids) {
        for (auto const& id: vec) {
            if (id % 0x10 == 0x1) {
                std::cout << std::hex << "0x" << id << " [" << consonants[id].get_symbol() << "] ";
            } else {
                std::cout << std::hex << "0x" << id << " [" << vowels[id].get_symbol() << "] ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

bool in_class(unsigned int id, unsigned int phon_class) {

    for (int i = 4; i <= 32; i += 4) {
        unsigned long int place = 1 << i;
        unsigned int class_digit = (phon_class % (place * 0x10)) / place;

        if (class_digit != 0x0) {
            unsigned int id_digit = (id % (place * 0x10)) / place;

            if (id_digit != class_digit) {
                return false;
            }
        }
    }

    return true;
}
