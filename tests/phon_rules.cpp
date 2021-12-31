#include "../ling/units/soundsystem.h"
#include "../ling/units/consonant.h"
#include "../ling/units/vowel.h"
#include "../ling/word/word.h"

static Word ruleVoicing(std::map<unsigned int, Consonant>&,
                        Word&);

static std::vector<Consonant> applyVRule(std::vector<Consonant>&,
                                         std::map<unsigned int, Consonant>&);

static Word rulePlosive(std::map<unsigned int, Consonant>&,
                        Word&);

int main() {
    SoundSystem soundSystem("preset00");
    soundSystem.load();

    std::map<unsigned int, Consonant> consonants = soundSystem.getConsonants();
    std::map<unsigned int, Vowel> vowels = soundSystem.getVowels();
    std::map<std::string, unsigned int> ids = soundSystem.getIds();

    /* Use ids map to retrieve phoneme given a symbol,
     * since ids may change in the future
     */

    Word word1 {
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
            }
        )
    }; // skæds

    Word word2 {
        Syllable(
            std::vector<Consonant> {
                consonants[ids["t"]],
            },
            std::vector<Vowel> {
                vowels[ids["æ"]],
            },
            std::vector<Consonant> {
                consonants[ids["t"]],
            }
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
            }
        )
    }; // tætsəs

    Word word3 {
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
            }
        ),
        Syllable(
            std::vector<Consonant> {
                consonants[ids["d"]],
            },
            std::vector<Vowel> {
                vowels[ids["i"]],
            }
        )
    }; // soʊgzdi

    Word word4 {
        Syllable(
            std::vector<Consonant> {
                consonants[ids["ŋ"]],
            },
            std::vector<Vowel> {
                vowels[ids["ɑ"]],
            },
            std::vector<Consonant> {
                consonants[ids["m"]],
            }
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
            }
        )
    }; // ŋɑmnəl

    Word word5 {
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
            }
        ),
        Syllable(
            std::vector<Consonant> {
                consonants[ids["n"]],
            },
            std::vector<Vowel> {
                vowels[ids["ɑ"]],
            }
        )
    }; // θoʊbnɑ

    // Apply voicing rule
    Word rep1 = ruleVoicing(consonants, word1); // skædz
    Word rep2 = ruleVoicing(consonants, word2); // tætsəs
    Word rep3 = ruleVoicing(consonants, word3); // soʊgzdi

    // Apply plosive rule
    Word rep4 = rulePlosive(consonants, word4); // gɑmdəl
    Word rep5 = rulePlosive(consonants, word5); // θoʊbdɑ

    std::cout << "/s/ --> [z] / C_\n"
              << "/" << word1.getPhonemic() << "/ --> ["
              << rep1.getPhonemic()  << "]\n"
              << "/" << word2.getPhonemic() << "/ --> ["
              << rep2.getPhonemic()  << "]\n"
              << "/" << word3.getPhonemic() << "/ --> ["
              << rep3.getPhonemic()  << "]\n";

    std::cout << "\n/nasal/ --> [plosive] / _V\n"
              << "/" << word4.getPhonemic() << "/ --> ["
              << rep4.getPhonemic() << "]\n"
              << "/" << word5.getPhonemic() << "/ --> ["
              << rep5.getPhonemic() << "]\n";

    return 0;
}

/* Voiceless alveolar fricative becomes voiced if it occurs after a voiced consonant
 * /s/ --> [z] / C_
 *
 * Consonants, at this point, can only occur in the onset and coda,
 * so ignore the nucleus
 */
static Word ruleVoicing(std::map<unsigned int, Consonant>& consonants,
                        Word& input) {

    Word output;
    int numSyl = input.getNumSyl();

    // Loop for each syllable
    for(int i = 0; i < numSyl; i++) {
        // First grab old onset and coda
        std::vector<Consonant> oldOnset = input.getSyllables().at(i).getOnset();
        std::vector<Consonant> oldCoda = input.getSyllables().at(i).getCoda();

        // Create new onset and coda with rule applied
        std::vector<Consonant> onset = applyVRule(oldOnset, consonants);
        std::vector<Consonant> coda = applyVRule(oldCoda, consonants);

        /**
         * Create new syllable using appropriate constructor:
         * V, VC, CV, else CVC
         * And add to output.
         *
         * How can this be simplified?
         */
        if (onset.empty() && coda.empty()) {
            output.addSyllable(Syllable(input.getSyllables().at(i).getNucleus()));
        } else if (onset.empty()) {
            output.addSyllable(Syllable(input.getSyllables().at(i).getNucleus(), coda));
        } else if (coda.empty()) {
            output.addSyllable(Syllable(onset, input.getSyllables().at(i).getNucleus()));
        } else {
            output.addSyllable(Syllable(onset, input.getSyllables().at(i).getNucleus(), coda));
        }
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
static Word rulePlosive(std::map<unsigned int, Consonant>& consonants,
                        Word& input) {

    Word output;
    int numSyl = input.getNumSyl();

    // Loop for each syllable
    for(int i = 0; i < numSyl; i++) {
        std::vector<Consonant> onset = input.getSyllables().at(i).getOnset();
        std::vector<Consonant> coda = input.getSyllables().at(i).getCoda();

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

        /**
         * Create new syllable using appropriate constructor:
         * V, VC, CV, else CVC
         * And add to output.
         *
         * How can this be simplified?
         */
        if (onset.empty()) {
            output.addSyllable(Syllable(input.getSyllables().at(i).getNucleus()));
        } else if (onset.empty()) {
            output.addSyllable(Syllable(input.getSyllables().at(i).getNucleus(), coda));
        } else if (coda.empty()) {
            output.addSyllable(Syllable(onset, input.getSyllables().at(i).getNucleus()));
        } else {
            output.addSyllable(Syllable(onset, input.getSyllables().at(i).getNucleus(), coda));
        }
    }
    return output;
}
