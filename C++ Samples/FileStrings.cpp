/*****************************************************************************/
/*!
\file   FileStrings.cpp
\author Rohit Saini
\par    email: rohitsaini429@gmail.com
\brief
    This file contains the implementation of the following functions for the
    Spell Checker.
    Functions include:

      + UpperCase
      + WordWrap
      + Tokenize
      + SpellChecker()
      + GetInfo
      + SpellCheck
      + WordLengths
      + WordsStartingWith
      + AcronymToWord
*/
/*****************************************************************************/
#include < fstream>        // ifstream
#include "FileStrings.h"  // SpellChecker, StringUtils, DictionaryInfo

/**************************************************************************/
/*!
  \brief
    Converts all lower case characters of a string to upper case.

  \param string
    The string to convert to uppercase.

  \return
    Returns the string passed in converted to upper case.

*/
/**************************************************************************/
std::string& StringUtils::UpperCase(std::string& string)
{
  //Iterator to go through the string characters
  std::string::iterator it;

  //Loop through characters of the string
  for (it = string.begin(); it != string.end(); ++it)
    //If the letter is lowercase
    if (*it >= 'a' && *it <= 'z')
      //Convert it to upper case
      *it = static_cast<char>(*it - SPACE);

  //Return the modified string
  return string;
}

/**************************************************************************/
/*!
  \brief
    Wraps a string into seperate lines based on a given line length.

  \param words
    The variable that stores the entire string to wrap.

  \param line_length
    The maximum length of a single line.

  \return
    Returns a vector containing the list of individual lines in the given
    string.

*/
/**************************************************************************/
std::vector<std::string> StringUtils::WordWrap(const std::string& words,
                                               size_t line_length)
{
  //Vector of each line of wrapped text
  std::vector<std::string> lines;

  //String to store the characters of each line
  std::string lineText;

  //Stores the number of characters in the current line
  size_t currentLength = 0;

  //Loop through the entire word string
  for (std::string::size_type index = 0; index < words.size(); index++)
  {

    //Append the current character to the line text
    lineText += words[index];

    /*
    If a space is encountered, perform checks to see if the next word will
    fit on the same line. Otherwise, terminate the current line
    */
    if (words[index] == ' ')
    {
      //Find the next space (discluding the one just encountered)
      size_t nextSpaceIndex = words.find(" ", index + 1);

      //If a space was found, it means there is another word
      if (nextSpaceIndex != std::string::npos)
        {
          /*
            This if means that if the next word is put onto the current line,
            it will be partially on one line and partially on the next line,
            so the current line needs to be terminated to start the new word
            on a new line.
          */
          if (currentLength + (nextSpaceIndex - index) > line_length)
          {

            /*
            REMOVE the space at the end since the next word will start on
            a new line. If this is not done, a space will appear at the end
            of each wrapped line.
            */
            lineText.erase(lineText.size() - 1);

            //Push the entire line onto the vector array
            lines.push_back(lineText);

            //Reset the text to use and appear for the new line
            lineText = "";

            //Reset current line length character count
            currentLength = 0;

            //Done adding a line, so skip iteration to move to next word
            continue;
          }
        }

      /*
      This if means that the last word has been encountered, since there are
      no spaces left in text, it must be added to the next line if it can not
      fit.
      (words.size() - index) gets the length of the last word. If the word
      cannot fit on the current line, the current line will be terminated and
      a new one will store the last word.

      */
      else if (currentLength + (words.size() - index) > line_length)
      {
        /*
        REMOVE the space at the end since the next (last) word will start on
        a new line. If this is not done, a space will appear at the end
        of each wrapped line.
        */
        lineText.erase(lineText.size() - 1);

        //Push the entire line onto the vector array
        lines.push_back(lineText);

        //Reset the text to use and appear for the new line
        lineText = "";

        //Reset current line length character count
        currentLength = 0;
      }

    }

    //Increase the current character count on the current line
    currentLength++;

    /*
    If the length of the characters has reached the line length, it is time
    to wrap the text onto the next line.
    */
    if (currentLength >= line_length)
    {

      /*
      First make sure that if the wrap length is less than a word's length,
      the entire word is shown on a line. This is to make sure that a single
      word is not cut halfway through to a new line. This is done below.
      */

      /*
      Get the index of the next space (to determine where this word ends)
      index + 1 is used so that if the current character is skipped (in case
      the current character is also a space).
      */
      size_t spaceIndex = words.find(" ", index + 1);

      //If a space is found, that means an entire word exists
      if (spaceIndex != std::string::npos)
      {
        /*
        Get the length of the line if it were to include the word which is
        trying to be wrapped halfway through.
        (spaceIndex - index) gets the remaining word length which would be
        moved to a different line if there is not enough space, and that
        being added to the line length gets the projected line length.
        */
        size_t wordLength = spaceIndex - index + currentLength;

        /*
        If the line length with the new word is more than the line wrap
        length, put the entire word onto this line (so it doesn't get cut /
        split onto a new line).
        */
        if (wordLength > line_length)
        {
          /*
          Loop to add the entire word to this line.
          (spaceIndex - 1) is the last index of the current word, since
          spaceIndex is the index of the space right after the word.
          */
          for (; index < spaceIndex - 1; )
            lineText += words[++index];
        }

        /*
        Start the next character check at the next word since this entire
        word has been added manually.
        */
        index = spaceIndex;
      }

      /*
      No space is found, so index is at the end of text. This means that all
      the remaining characters must be added to this line.
      */
      else
      {
        /*
        Add the entire string's characters till the end of the text. Prefix
        is used since index would be the character already stored in the
        line.
        */
        for (; index < words.size(); )
          lineText += words[++index];

        /*
        Remove last character because ++index will read in 1 garbage value
        at the end.
        */
        lineText.erase(lineText.size() - 1);
      }

      //Add the line to the end of vector
      lines.push_back(lineText);

      //Reset the current word
      lineText = "";

      //Reset current line length character count
      currentLength = 0;
    }

  }

  /*
  Add the last line to the vector array of lines if it contains any text.
  This needs to be done to ensure that the last line is added in case it
  did not reach the line length.
  */
  if (lineText != "")
    lines.push_back(lineText);

  //Return the vector of wrapped lines
  return lines;
}

/**************************************************************************/
/*!
  \brief
    Splits a string into a vector of individual tokens (space - seperated)

  \param words
    The variable that stores the entire string to tokenize.

  \return
    Returns a vector containing the list of individual tokens in the given
    string.

*/
/**************************************************************************/
std::vector<std::string> StringUtils::Tokenize(const std::string& words)
{
  //Variable to store word to append to tokenized list.
  std::string word;

  //The tokenized list of strings
  std::vector<std::string> wordsList;

  //Loop through the entire word string
  for (std::string::size_type index = 0; index < words.size(); index++)
  {
    //If the character is not a space, update the current word
    if (words[index] != ' ')
      word += words[index];

    //Otherwise add the word since a space was enocuntered
    else if (word != "")
    {
      //Add to end of vector
      wordsList.push_back(word);

      //Reset the current wordd
      word = "";
    }
  }

  //If the last character was a word character, add it to the back
  if (word != "")
    wordsList.push_back(word);

  //Return the list of tokenized strings.
  return wordsList;
}

/**************************************************************************/
/*!
  \brief
    Initializes a spell checker object with legal initial values.

  \param dictionary
    The variable that stores the dictionary file name

*/
/**************************************************************************/
SpellChecker::SpellChecker(const std::string &dictionary)
{
  dictionary_ = dictionary;
}

/**************************************************************************/
/*!
  \brief
    Finds the shortest, longest and number of words in a dictionary.

  \param info
    The structure to store the information in.

  \return
    Returns scrFILE_OK if the information was obtained successfully.
    Otherwise, returns scrFILE_ERR_OPEN if the dictionary was not opened
    successfully.
*/
/**************************************************************************/
SpellChecker::SCResult SpellChecker::GetInfo(DictionaryInfo &info) const
{
  //Attempt to open the dictionary file
  std::ifstream dictionary(dictionary_.c_str());

  //Return if the dictionary could not be opened
  if (!dictionary.is_open())
    return scrFILE_ERR_OPEN;

  //Variable to store a single word from the dictionary
  std::string word;

  //Variables to store the dictionary information to obtain
  size_t longest = 0;
  size_t shortest = 10000;
  size_t count = 0;

  //Loop while not at the end of the dictionary file
  while (!dictionary.eof())
  {
    //Read in a line from the dictionary into the word variable
    if (std::getline(dictionary, word).eof())
      //Break if eof is encounteredd
      break;

    //Update longest word
    if (word.size() > longest)
      longest = word.size();

    //Update shortest word
    if (word.size() < shortest)
      shortest = word.size();

    //Count total words
    count++;
  }

  //Assign and update structure values
  info.shortest = shortest;
  info.longest = longest;
  info.count = count;

  //Return successful operation
  return scrFILE_OK;
}

/**************************************************************************/
/*!
  \brief
    Checks a word for correct spelling against a dictionary.

  \param word
    The word to check the spelling of

  \return
    Returns scrFILE_ERR_OPEN if the dictionary file was not opened. If the
    dictionary is opened, it returns scrWORD_OK if the word is spelled
    correctly. Otherwise returns scrWORD_BAD.
*/
/**************************************************************************/
SpellChecker::SCResult SpellChecker::SpellCheck(const std::string& word) const
{
  //Attempt to open the dictionary file
  std::ifstream dictionary(dictionary_.c_str());

  //Return if the dictionary could not be opened
  if (!dictionary.is_open())
    return scrFILE_ERR_OPEN;

  //Variable to store a single word from the dictionary
  std::string dictionaryWord;

  //Variable to store the uppercase word for comparison
  std::string wordUpper(word);

  //Convert word to upper case
  StringUtils::UpperCase(wordUpper);

  //Loop while not at the end of the dictionary file
  while (!dictionary.eof())
  {

    //Read a single word from the dictionary
    dictionary >> dictionaryWord;

    //Convert it to uppercase for comparison
    StringUtils::UpperCase(dictionaryWord);

    //If the word is spelled correctly, return successful.
    if (dictionaryWord == wordUpper)
      return scrWORD_OK;

    //break and return early because the words are in alphabetical order
    else if (dictionaryWord[0] > wordUpper[0])
      return scrWORD_BAD;
  }

  //Return unsuccessful since the word was misspelled
  return scrWORD_BAD;
}

/**************************************************************************/
/*!
  \brief
    Stores the number of words of specific length in a given range.

  \param lengths
    The vector to store the number of length occurrences of words in.

  \param count
    The maximum length of a word to count.

  \return
    Returns scrFILE_OK if the lengths were obtained successfully.
    Otherwise, returns scrFILE_ERR_OPEN if the dictionary was not opened
    successfully.
*/
/**************************************************************************/
SpellChecker::SCResult SpellChecker::WordLengths(std::vector<size_t>& lengths,
                                                 size_t count) const
{
  //Attempt to open the dictionary file
  std::ifstream dictionary(dictionary_.c_str());

  //Return if the dictionary could not be opened
  if (!dictionary.is_open())
    return scrFILE_ERR_OPEN;

  //Variable to store a single word from the dictionary
  std::string word;

  //Loop while not at the end of the dictionary file
  while (!dictionary.eof())
  {

    //Read a dictionary word into the variable (if not at eof)
    if (std::getline(dictionary, word).eof())
      break;

    //If the variable has a valid length, count it
    if (word.size() > 0 && word.size() <= count)
    {
      //Increase specific array value to count length
      lengths[word.size()]++;
    }

  }

  //Return successful operation
  return scrFILE_OK;
}

/**************************************************************************/
/*!
  \brief
    Counts the number of words starting with a specific letter.

  \param letter
    The starting letter of each word to check.

  \param count
    The number of words starting with the letter (to be modified)

  \return
    Returns scrFILE_OK if the count was obtained successfully.
    Otherwise, returns scrFILE_ERR_OPEN if the dictionary was not opened
    successfully.
*/
/**************************************************************************/
SpellChecker::SCResult SpellChecker::WordsStartingWith(char letter,
                                                       int& count) const
{
  //Attempt to open the dictionary file
  std::ifstream dictionary(dictionary_.c_str());

  //Return if the dictionary could not be opened
  if (!dictionary.is_open())
    return scrFILE_ERR_OPEN;

  //Convert to uppercase letter if it is lowercase
  if (letter >= 'a' && letter <= 'z')
    letter = static_cast<char>(letter - SPACE);

  //Variable to store dictionary word
  std::string word;

  //Loop while not at the end of the dictionary file
  while (!dictionary.eof())
  {

    //Read in a dictionary word into the variable
    if (std::getline(dictionary, word).eof())
      //break if eof is encountered
      break;

    //Convert word to uppercase for comparison
    StringUtils::UpperCase(word);

    //Count a word starting with the letter
    if (word[0] == letter)
      count++;

    //Otherwise break early because the words are in alphabetical order
    else if (word[0] > letter)
      //return successful operation attempt since file was opened
      return scrFILE_OK;
  }

  //return successful operation attempt since file was opened
  return scrFILE_OK;
}

/**************************************************************************/
/*!
  \brief
    Stores all words which are acronyms of a given abbrevation.

  \param acronym
    The acronym to check for in the dictionary

  \param words
    The vector to store all the words which are compatible acronyms.

  \param maxlen
    The maximum length of a word to check for.

  \return
    Returns scrFILE_OK if the acronyms were tested successfully.
    Otherwise, returns scrFILE_ERR_OPEN if the dictionary was not opened
    successfully.
*/
/**************************************************************************/
SpellChecker::SCResult SpellChecker::AcronymToWord(const std::string& acronym,
                                     std::vector<std::string>& words,
                                     size_t maxlen) const
{
  //Attempt to open the dictionary file
  std::ifstream dictionary(dictionary_.c_str());

  //Return if the dictionary could not be opened
  if (!dictionary.is_open())
    return scrFILE_ERR_OPEN;

  //Variable to store a single word from the dictionary
  std::string word;

  //Variable to store the acronym in uppercase (non-const)
  std::string acronymUpper = acronym;

  //Convert acronym to upper case
  StringUtils::UpperCase(acronymUpper);

  //Loop while not at the end of the dictionary file
  while (!dictionary.eof())
  {
    //Read in a dictionary word
    dictionary >> word;

    //If the word is not a valid length, then skip it
    if (word.size() > maxlen && maxlen > 0)
      continue;

    //Variable to store the uppercase word
    std::string wordUpper = word;

    //Convert word to uppercase
    StringUtils::UpperCase(wordUpper);

    //To track whether the word is a valid acronym
    bool isAcronym = true;

    //If the first letter is not the acronym letter, mark as not acronym
    if (wordUpper[0] != acronymUpper[0])
        isAcronym = false;

    else
    {
      //Index of acronym letter to check (1 since first letter was checked)
      std::string::size_type acrLetter = 1;

      //Loop through the corresponding characters in the word
      for (std::string::size_type i = 1; i < wordUpper.size(); i++)
        //If the acronym letter being checked is in the word
        if (acronymUpper[acrLetter] == wordUpper[i])
            //Move to the next acronym letter
            acrLetter++;

      //If an incorrect number of acronym letters were counted, mark as false
      if (acrLetter != acronym.size())
        isAcronym = false;
    }

    //if the word is an acronym, add it to the vector
    if (isAcronym)
      words.push_back(word);
  }

  //return successful operation
  return scrFILE_OK;
}
