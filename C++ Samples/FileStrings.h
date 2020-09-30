/*****************************************************************************/
/*!
\file   FileStrings.h
\author Rohit Saini
\par    email: rohitsaini429@gmail.com
\brief
    This file contains the definition of the following functions for a
    Spell Checker, with custom string functions to support them.
*/
/*****************************************************************************/
#ifndef FILESTRINGS_H
#define FILESTRINGS_H

#include <string> // string
#include <vector> // vector

//! Space value constant
const char SPACE = 32;

//!The custom string functions class
class StringUtils
{
  public:
    //!Converts a given string to upper case
    static std::string& UpperCase(std::string& string);

    //!Wraps words per line to a specific length
    static std::vector<std::string> WordWrap(const std::string& words,
                                             size_t line_length);
    //!Tokenizes a given string
    static std::vector<std::string> Tokenize(const std::string& words);
};

//!Dictionary information
class DictionaryInfo
{
  public:

    size_t shortest; // Shortest word in the dictionary

    size_t longest;  // Longest word in the dictionary

    size_t count;    // Number of words in the dictionary
};

//!Spell Checker class
class SpellChecker
{
  public:

    //!Constants for return values from functions
    enum SCResult {

       scrFILE_OK = -1,       // File was opened successfully

        scrFILE_ERR_OPEN = -2, // File was unable to be opened

        scrWORD_OK = 1,        // Word found in dictionary

        scrWORD_BAD = 2        // Word not found in dictionary
                  };
    //!Constructs a spell checker with the given dictionary of words
    SpellChecker(const std::string &dictionary);

    //!Get dictionary information
    SCResult GetInfo(DictionaryInfo &info) const;

    //!Check if a word is spelled correctly
    SCResult SpellCheck(const std::string& word) const;

    //!Check how many words are of a given length and their frequency
    SCResult WordLengths(std::vector<size_t>& lengths, size_t count) const;

    //!Checks if a word starts with a specific letter
    SCResult WordsStartingWith(char letter, int& count) const;

    //!Checks if words contain the given acronym
    SCResult AcronymToWord(const std::string& acronym,
                           std::vector<std::string>& words,
                           size_t maxlen = 0) const;

  private:
    //!Thed dictionary variable
    std::string dictionary_;
};

#endif
