#ifndef WORD_H
#define WORD_H

#include <initializer_list>
#include <morpheme.h>


class Word {
private:
  // string phonetic;
  // string spelling;
  // string meaning;
  // enum for some POS

  std::vector<Morpheme> morphemes;
  std::string phonemic;  
  int numMorphemes;
  
public:
  Word(initializer_list<Morpheme> lst) {
    for (const auto& morpheme : lst) {
        addMorpheme(morpheme);
    }

  }
  
  Morpheme() {
    numMorphemes = 0;
  }
  
  void addMorpheme(Morpheme m) {
    morphemes.push_back(m);
    phonemic += m.getPhonemic();
    numMorphemes++;
  }
  
  std::vector<Morpheme> getMorphemes() const {
    return morphemes;
  }
  
  std::string getPhonemic() const {
    return phonemic;
  }
  
  int getNumMorphemes() const {
    return numMorphemes;
  }

}

#endif