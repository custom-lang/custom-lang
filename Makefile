CXX = g++
CXXFLAGS = -g -D_GLIBCXX_USE_C99=1 -std=c++11 -Wall
PROGS = print_all phon_rules syl_gen gen_morph assim_algos

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f $(PROGS) tests/*.o ling/units/*.o ling/word/*.o

print_all: tests/print_all.o ling/units/consonant.o ling/units/phoneme.o ling/units/vowel.o ling/units/soundsystem.o ling/word/morpheme.o ling/word/morpheme_gen.o ling/word/syllable_gen.o
	$(CXX) $(CXXFLAGS) tests/print_all.cpp ling/word/*.cpp ling/units/*.cpp -o print_all

phon_rules: tests/phon_rules.o ling/units/consonant.o ling/units/phoneme.o ling/units/vowel.o ling/units/soundsystem.o ling/word/morpheme.o ling/word/morpheme_gen.o ling/word/syllable_gen.o
	$(CXX) $(CXXFLAGS) tests/phon_rules.cpp ling/word/*.cpp ling/units/*.cpp -o phon_rules

syl_gen: tests/syl_gen.o ling/units/consonant.o ling/units/phoneme.o ling/units/vowel.o ling/units/soundsystem.o ling/word/morpheme.o ling/word/morpheme_gen.o ling/word/syllable_gen.o
	$(CXX) $(CXXFLAGS) tests/syl_gen.cpp ling/word/*.cpp ling/units/*.cpp -o syl_gen

gen_morph: tests/gen_morph.o ling/units/consonant.o ling/units/phoneme.o ling/units/vowel.o ling/units/soundsystem.o ling/word/morpheme.o ling/word/morpheme_gen.o ling/word/syllable_gen.o
	$(CXX) $(CXXFLAGS) tests/gen_morph.cpp ling/word/*.cpp ling/units/*.cpp -o gen_morph

assim_algos: tests/assim_algos.o ling/units/consonant.o ling/units/phoneme.o ling/units/vowel.o ling/units/soundsystem.o ling/word/morpheme.o ling/word/morpheme_gen.o ling/word/syllable_gen.o
	$(CXX) $(CXXFLAGS) tests/assim_algos.cpp ling/word/*.cpp ling/units/*.cpp -o assim_algos
