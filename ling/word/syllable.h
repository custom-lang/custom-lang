/*
 * Represents a single syllable.
 * A syllable is of the format "onset nucleus coda", where onset and
 * coda can be ommited, but nucleus is mandatory.
 *
 * A syllable can also carry a suprasegmental, of which is associated with
 * the nucleus.
 *
 */

#ifndef SYLLABLE_H
#define SYLLABLE_H

#include <vector>
#include <initializer_list>

#include "../units/consonant.h"
#include "../units/vowel.h"
#include "../units/suprasegmental.h"

class Syllable {
private:
    std::vector<Consonant> onset;
    std::vector<Vowel> nucleus;
    std::vector<Consonant> coda;
    std::vector<Suprasegmental> suprasegs;
public:
    // CVC
    Syllable(std::vector<Consonant> onset, std::vector<Vowel> nucleus, std::vector<Consonant> coda) {
        this->onset = onset;
        this->nucleus = nucleus;
        this->coda = coda;
    }

    // VC
    Syllable(std::vector<Vowel> nucleus, std::vector<Consonant> coda) {
        this->nucleus = nucleus;
        this->coda = coda;
    }

    // CV
    Syllable(std::vector<Consonant> onset, std::vector<Vowel> nucleus) {
        this->onset = onset;
        this->nucleus = nucleus;
    }

    // V
    Syllable(std::vector<Vowel> nucleus) {
        this->nucleus = nucleus;
    }

    // With Suprasegmentals

    // CVSC
    Syllable(std::vector<Consonant> onset, std::vector<Vowel> nucleus,
             std::vector<Suprasegmental> suprasegs, std::vector<Consonant> coda) {

        this->onset = onset;
        this->nucleus = nucleus;
        this->suprasegs = suprasegs;
        this->coda = coda;
    }

    // VSC
    Syllable(std::vector<Vowel> nucleus, std::vector<Suprasegmental> suprasegs,
             std::vector<Consonant> coda) {

        this->nucleus = nucleus;
        this->coda = coda;
        this->suprasegs = suprasegs;
    }

    // CVS
    Syllable(std::vector<Consonant> onset, std::vector<Vowel> nucleus,
             std::vector<Suprasegmental> suprasegs) {

        this->onset = onset;
        this->nucleus = nucleus;
        this->suprasegs = suprasegs;
    }

    // VS
    Syllable(std::vector<Vowel> nucleus, std::vector<Suprasegmental> suprasegs) {
        this->nucleus = nucleus;
        this->suprasegs = suprasegs;
    }

    std::vector<Consonant> getOnset() const { return onset; }
    std::vector<Vowel> getNucleus() const { return nucleus; }
    std::vector<Consonant> getCoda() const { return coda; }
    std::vector<Suprasegmental> getSuprasegs() const { return suprasegs; }

    bool hasOnset() const { return onset.size() > 0 ? true : false; }
    bool hasSupraseg() const { return suprasegs.size() > 0 ? true : false; }
    bool hasCoda() const { return coda.size() > 0 ? true : false; }

    std::string getStrSyl() const {
        std::string str = "";

        // Onset
        for (const auto& con: onset) {
            str += con.getSymbol();
        }

        // Nucleus
        for (const auto& vow: nucleus) {
            str += vow.getSymbol();
        }

        // Suprasegmental
        for (const auto& sup: suprasegs) {
                str += sup.getSymbol();
        }

        // Coda
        for (const auto& con: coda) {
            str += con.getSymbol();
        }

        return str;
    }
};

#endif
