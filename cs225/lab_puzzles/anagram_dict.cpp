/**
 * @file anagram_dict.cpp
 * Implementation of the AnagramDict class.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "anagram_dict.h"

#include <algorithm> /* I wonder why this is included... */
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

/**
 * Constructs an AnagramDict from a filename with newline-separated
 * words.
 * @param filename The name of the word list file.
 */
AnagramDict::AnagramDict(const string& filename)
{
    ifstream wordsFile(filename);
    string word, order;
    if (wordsFile.is_open()) {
        while (getline(wordsFile, word)) {
                order = word;
                sort(order.begin(), order.end());
                dict[order].push_back(word);
        }
    }
}

/**
 * Constructs an AnagramDict from a vector of words.
 * @param words The vector of strings to be used as source words.
 */
AnagramDict::AnagramDict(const vector<string>& words)
{
    string order;
    for(vector<string>::const_iterator it = words.begin(); it!=words.end(); ++it){
        order = *it;
        sort(order.begin(), order.end());
        dict[order].push_back(*it);
    }
}

/**
 * @param word The word to find anagrams of.
 * @return A vector of strings of anagrams of the given word. Empty
 * vector returned if no anagrams are found or the word is not in the
 * word list.
 */
vector<string> AnagramDict::get_anagrams(const string& word) const
{
    string order = word;
    sort(order.begin(), order.end());
    if(!dict.count(order)) //this key does not exist
        return vector<string>();
    return dict.find(order)->second;
}

/**
 * @return A vector of vectors of strings. Each inner vector contains
 * the "anagram siblings", i.e. words that are anagrams of one another.
 * NOTE: It is impossible to have one of these vectors have less than
 * two elements, i.e. words with no anagrams are ommitted.
 */
vector<vector<string>> AnagramDict::get_all_anagrams() const
{
    vector<vector<string>> out;
    for(std::map<string,vector<string>>::const_iterator it = dict.begin(); it != dict.end(); ++it){
        if((it->second).size() > 1)
            out.push_back(it->second);
    }
    return out;
}
