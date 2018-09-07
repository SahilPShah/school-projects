/**
 * @file cartalk_puzzle.cpp
 * Holds the function which solves a CarTalk puzzler.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include <fstream>

#include "cartalk_puzzle.h"

using namespace std;

/**
 * Solves the CarTalk puzzler described here:
 * http://www.cartalk.com/content/wordplay-anyone.
 * @return A vector of (string, string, string) tuples
 * Returns an empty vector if no solutions are found.
 * @param d The PronounceDict to be used to solve the puzzle.
 * @param word_list_fname The filename of the word list to be used.
 */
vector<std::tuple<std::string, std::string, std::string>> cartalk_puzzle(PronounceDict d,
                                    const string& word_list_fname)
{
    vector<std::tuple<std::string, std::string, std::string>> ret;

    ifstream words(word_list_fname);
    vector<string> wordFile;

    if (words.is_open()) {
        std::istream_iterator<string> it(words);
        while (!words.eof()) {
            wordFile.push_back(*it);
            ++it;
        }
    }
    words.close();

    for(vector<string>::iterator it = wordFile.begin(); it != wordFile.end(); ++it) {
      string first_letter = (*it).substr(1, (*it).size() - 1);
      string second_letter = *it;
      second_letter.erase(1, 1);
      if(d.homophones(*it, first_letter) && d.homophones(*it, second_letter))
        ret.push_back(make_tuple(*it, first_letter, second_letter));
    }
    return ret;
}
