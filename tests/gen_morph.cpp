#include <iostream>
#include <iomanip>
#include <random>

#include "../ling/units/soundsystem.h"
#include "../ling/word/morpheme_gen.h"

int main() {
    SoundSystem soundSystem("preset01");
    soundSystem.load();

    // Define range of syllable types
    std::array<int, 4> max = {1, 1, 1, 0};
    std::array<int, 4> min = {0, 1, 0, 0};

    // Root morpheme
    for (int i = 0; i < 5; i++) {
        RootMorph m = makeRootMorph(soundSystem, max, min);

        std::cout << std::string(m.isFree() ? "Free " : "Bound ")
                  << Morpheme::getStrMorphType(m.getMorphType())
                  << " : /" << m.getPhonemic() << "/ "
                  << Morpheme::getStrConLexCat(m.getLexCat()) << "\n";
    }

    std::cout << "\n";

    // Function morpheme
    for (int i = 0; i < 5; i++) {
        FunctionMorph m = makeFuncMorph(soundSystem, max, min);

        std::cout << Morpheme::getStrMorphType(m.getMorphType()) << " : /" << m.getPhonemic() << "/ "
                  << Morpheme::getStrFuncLexCat(m.getLexCat()) << "\n";
    }

    std::cout << "\n";

    // Derviational Affixes
    for (int i = 0; i < 5; i++) {
        DerivationalAffix m = makeDerivAffix(soundSystem, max, min);

        std::cout << Morpheme::getStrMorphType(m.getMorphType()) << " : /" << m.getPhonemic() << "/ "
                  << Morpheme::getStrConLexCat(m.getLexAttach()) << " | "
                  << Morpheme::getStrConLexCat(m.getLexChange())
                  << "\n";
    }

    std::cout << "\n";

    // Inflectional Affixes
    for (int i = 0; i < 5; i++) {
        InflectionalAffix m = makeInflecAffix(soundSystem, max, min);

        std::cout << Morpheme::getStrMorphType(m.getMorphType()) << " : /" << m.getPhonemic() << "/ "
                  << Morpheme::getStrConLexCat(m.getLexAttach()) << "\n";
    }

    return 0;
}
