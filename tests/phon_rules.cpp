#include <iostream>

#include "../ling/units/soundsystem.h"
#include "../ling/word/root_morph.h"

static RootMorph ruleVoicing(std::map<unsigned int, Consonant>&,
                             RootMorph&);

static std::vector<Consonant> applyVRule(std::vector<Consonant>&,
                                         std::map<unsigned int, Consonant>&);

static RootMorph rulePlosive(std::map<unsigned int, Consonant>&,
                             RootMorph&);

int main() {
    SoundSystem soundSystem("preset00");
    soundSystem.load();

    std::map<unsigned int, Consonant> consonants = soundSystem.getConsonants();
    std::map<unsigned int, Vowel> vowels = soundSystem.getVowels();
    std::map<std::string, unsigned int> ids = soundSystem.getIds();

    /*
     * Use ids map to retrieve phoneme given a symbol,
     * since ids may change in the future
     */

    std::vector<Syllable> syl1 {
        Syllable(
            std::vector<Consonant> {
                consonants[ids["s"]],
                consonants[ids["k"]]
            },
            std::vector<Vowel> {
                vowels[ids["æ"]],
            },
            std::vector<Consonant> {
                consonants[ids["d"]],
                consonants[ids["s"]]
            },
            std::vector<Suprasegmental>()
        )
    };

    std::vector<Syllable> syl2 {
        Syllable(
            std::vector<Consonant> {
                consonants[ids["t"]],
            },
            std::vector<Vowel> {
                vowels[ids["æ"]],
            },
            std::vector<Consonant> {
                consonants[ids["t"]],
            },
            std::vector<Suprasegmental>()
        ),
        Syllable(
            std::vector<Consonant> {
                consonants[ids["s"]],
            },
            std::vector<Vowel> {
                vowels[ids["ə"]],
            },
            std::vector<Consonant> {
                consonants[ids["s"]],
            },
            std::vector<Suprasegmental>()
        )
    };

    std::vector<Syllable> syl3 {
        Syllable(
            std::vector<Consonant> {
                consonants[ids["s"]],
            },
            std::vector<Vowel> {
                vowels[ids["o"]],
                vowels[ids["ʊ"]]
            },
            std::vector<Consonant> {
                consonants[ids["g"]],
                consonants[ids["s"]]
            },
            std::vector<Suprasegmental>()
        ),
        Syllable(
            std::vector<Consonant> {
                consonants[ids["d"]],
            },
            std::vector<Vowel> {
                vowels[ids["i"]],
            },
            std::vector<Consonant>(),
            std::vector<Suprasegmental>()
        )
    };

    std::vector<Syllable> syl4 {
        Syllable(
            std::vector<Consonant> {
                consonants[ids["ŋ"]],
            },
            std::vector<Vowel> {
                vowels[ids["ɑ"]],
            },
            std::vector<Consonant> {
                consonants[ids["m"]],
            },
            std::vector<Suprasegmental>()
        ),
        Syllable(
            std::vector<Consonant> {
                consonants[ids["n"]],
            },
            std::vector<Vowel> {
                vowels[ids["ə"]],
            },
            std::vector<Consonant> {
                consonants[ids["l"]],
            },
            std::vector<Suprasegmental>()
        )
    };

    std::vector<Syllable> syl5 {
        Syllable(
            std::vector<Consonant> {
                consonants[ids["θ"]],
            },
            std::vector<Vowel> {
                vowels[ids["o"]],
                vowels[ids["ʊ"]]
            },
            std::vector<Consonant> {
                consonants[ids["b"]],
            },
            std::vector<Suprasegmental>()
        ),
        Syllable(
            std::vector<Consonant> {
                consonants[ids["n"]],
            },
            std::vector<Vowel> {
                vowels[ids["ɑ"]],
            },
            std::vector<Consonant>(),
            std::vector<Suprasegmental>()
        )
    };

    RootMorph morpheme1 (syl1, ConLexCat::noun, true); // skæds
    RootMorph morpheme2 (syl2, ConLexCat::noun, true); // tætsəs
    RootMorph morpheme3 (syl3, ConLexCat::noun, true); // soʊgzdi
    RootMorph morpheme4 (syl4, ConLexCat::noun, true); // ŋɑmnəl
    RootMorph morpheme5 (syl5, ConLexCat::noun, true); // θoʊbnɑ

    // Apply voicing rule
    RootMorph rep1 = ruleVoicing(consonants, morpheme1); // skædz
    RootMorph rep2 = ruleVoicing(consonants, morpheme2); // tætsəs
    RootMorph rep3 = ruleVoicing(consonants, morpheme3); // soʊgzdi

    // Apply plosive rule
    RootMorph rep4 = rulePlosive(consonants, morpheme4); // gɑmdəl
    RootMorph rep5 = rulePlosive(consonants, morpheme5); // θoʊbdɑ

    std::cout << "/s/ --> [z] / C_\n"
              << "/" << morpheme1.getPhonemic() << "/ --> ["
              << rep1.getPhonemic()  << "]\n"
              << "/" << morpheme2.getPhonemic() << "/ --> ["
              << rep2.getPhonemic()  << "]\n"
              << "/" << morpheme3.getPhonemic() << "/ --> ["
              << rep3.getPhonemic()  << "]\n";

    std::cout << "\n/nasal/ --> [plosive] / _V\n"
              << "/" << morpheme4.getPhonemic() << "/ --> ["
              << rep4.getPhonemic() << "]\n"
              << "/" << morpheme5.getPhonemic() << "/ --> ["
              << rep5.getPhonemic() << "]\n";

    return 0;
}

/* Voiceless alveolar fricative becomes voiced if it occurs after a voiced consonant
 * /s/ --> [z] / C_
 *
 * Consonants, at this point, can only occur in the onset and coda,
 * so ignore the nucleus
 */
static RootMorph ruleVoicing(std::map<unsigned int, Consonant>& consonants,
                            RootMorph& input) {

    RootMorph output;
    int numSyl = input.getNumSyl();

    // Loop for each syllable
    for(int i = 0; i < numSyl; i++) {
        // First grab old onset and coda
        std::vector<Consonant> oldOnset = input.getSyllables()[i].getOnset();
        std::vector<Consonant> oldCoda = input.getSyllables()[i].getCoda();

        // Create new onset and coda with rule applied
        std::vector<Consonant> onset = applyVRule(oldOnset, consonants);
        std::vector<Consonant> coda = applyVRule(oldCoda, consonants);

        output.addSyllable(Syllable(onset, input.getSyllables()[i].getNucleus(),
                                    coda, std::vector<Suprasegmental>()));
    }
    return output;
}

/**
 * Apply voicing rule on an onset or coda
 * The purpose of this function is to avoid code duplication
 */
static std::vector<Consonant> applyVRule(std::vector<Consonant>& input, std::map<unsigned int, Consonant>& consonants) {
    std::vector<Consonant> output;
    int size = input.size();

    for (int i = 0; i < size; i++) {
        unsigned int curId = input[i].getId();

        /* Check if there is a phoneme before current position,
        * if that phoneme is voiced, and
        * if current phoneme is a voiceless alveolar fricative.
        *
        * If true, add voiced alveolar fricative to new coda
        */
        if (i - 1 >= 0 && input[i - 1].getVoicing() == Voicing::voiced
            && curId % 0x10000 == 0x0731) {

            curId += 0x1000;
        }
        output.push_back(consonants[curId]);
    }

    return output;
}

/* Nasal becomes plosive if it occurs before a vowel
 * /nasal/ --> [plosive] / _V
 *
 * Here, only onsets can occur before a vowel, so ignore coda and nucleus
 */
static RootMorph rulePlosive(std::map<unsigned int, Consonant>& consonants,
                            RootMorph& input) {

    RootMorph output;
    int numSyl = input.getNumSyl();

    // Loop for each syllable
    for(int i = 0; i < numSyl; i++) {
        std::vector<Consonant> onset = input.getSyllables()[i].getOnset();
        std::vector<Consonant> coda = input.getSyllables()[i].getCoda();

        //Check if final consonant in onset is a nasal
        if (onset.back().getManner() == Manner::nasal) {

            // Turn plosive
            unsigned int id = onset.back().getId() + ((static_cast<int>(Manner::plosive)
                                                   - static_cast<int>(Manner::nasal)) * 0x100);

            // Change id only if corresponding plosive exists
            if (consonants.find(id) != consonants.end()) {
                onset.back() = consonants[id];
            }
        }

        output.addSyllable(Syllable(onset, input.getSyllables()[i].getNucleus(),
                                    coda, std::vector<Suprasegmental>()));
    }
    return output;
}
