CXX = g++
CXXFLAGS = -g -D_GLIBCXX_USE_C99=1 -std=c++11 -Wall
PROGS = print_all phon_rules

.PHONY: all clean

all: $(PROGS)

clean:
	rm -f $(PROGS) tests/*.o ling/units/*.o ling/word/*.o

print_all: tests/print_all.o ling/units/phoneme.o ling/units/consonant.o ling/units/vowel.o ling/units/soundsystem.o
	$(CXX) $(CXXFLAGS) tests/print_all.cpp ling/units/*.cpp -o print_all

phon_rules: tests/phon_rules.o ling/units/phoneme.o ling/units/consonant.o ling/units/vowel.o ling/units/soundsystem.o
	$(CXX) $(CXXFLAGS) tests/phon_rules.cpp ling/units/*.cpp -o phon_rules
