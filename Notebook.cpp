/**
 * @author Amir Gillette
 * @date 2022-03-18
 */

#include "Notebook.hpp"
#include "Direction.hpp"
#include <iostream>
#include <stdexcept>
#include <valarray>

using ariel::Direction;
using namespace std;

string createNewLine(string page);
string addLines(string str, unsigned int amount);


/**
 * The method writes a given word in a given direction (horizontal/vertical) and position (row, col)
 * @param page a given page index.
 * @param row row location.
 * @param column column location.
 * @param direction the direction in which the word will be read.
 * @param word word to write.
 * @note Exception is thrown whenever the writing position is already written or deleted (everything except '_').
 */
void ariel::Notebook::write(unsigned int page, unsigned int row, unsigned int column, Direction direction, string const& word) {
    string currPage;
    try {
        currPage =_pages.at(page);
    }
    catch (exception) { // Page doesn't exist
        string newPage = createNewLine("");
        _pages.insert(_pages.begin() + page, newPage);
        currPage = _pages.at(page);
    }
    unsigned int pos = row > 1 ? (ROW + 1) * (row - 1) + column : column;
    if (direction == Direction::Horizontal){
        if (column + word.length() - 1 > ROW - 1){
            cout << column << ", " << word.length() << '\n';
            throw errc::result_out_of_range;
        }

        unsigned int MaxLineIndex = currPage.length() / (ROW + 1);
        if (row > MaxLineIndex){
            unsigned int linesToCreate = row - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }
        for (size_t i = pos, k = 0; i < pos + word.length(); ++i, k++) {
            if (currPage[i] != '_'){
                throw invalid_argument("Char has written/deleted already in this position.");
            }
            currPage[i] = word[k];
        }
    }
    else{  // direction is Vertical

        unsigned int MaxLineIndex = currPage.length() / (ROW + 1);

        if (row + word.length() > MaxLineIndex){
            unsigned int linesToCreate = (row + word.length()) - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }

        for (size_t i = 0; i < word.length(); ++i) {
            if (currPage[pos + (ROW + 1) * i] != '_'){
                throw invalid_argument("Char has written/deleted already in this position.");
            }
            currPage[pos + (ROW + 1) * i] = word[i];
        }
    }
    _pages.at(page) = currPage;
}

/**
 * Method erases a word in a given length, direction (horizontal/vertical), and starting position (row, column)/
 * @param page a given page index.
 * @param row row location.
 * @param column column location.
 * @param direction the direction in which the word will be read.
 * @param length the length of the word to be read.
 */
void ariel::Notebook::erase(unsigned int page, unsigned int row, unsigned int column, Direction direction, unsigned int length) {
    string currPage = _pages.at(page);
    unsigned int pos = row > 1 ? (ROW + 1) * (row - 1) + column : column;
    if (direction == Direction::Horizontal){
        if (column + length > ROW - 1){
            throw errc::result_out_of_range;
        }

        for (size_t i = pos; i < pos + length; ++i) {
            currPage[i] = '~';
        }
    }
    else{  // direction is Vertical

        unsigned int MaxLineIndex = currPage.length() / (ROW + 1);
        if (row + length > MaxLineIndex){
            unsigned int linesToCreate = (row + length) - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }

        for (size_t i = 0; i < length; ++i) {
            currPage[pos + (ROW + 1) * i] = '~';
        }
    }
    _pages.at(page) = currPage;

}

/**
 * The method reads the word at the position (row, col), a word which its length is given.
 *
 * @param page a given page index.
 * @param row row location.
 * @param column column location.
 * @param direction the direction in which the word will be read.
 * @param length the length of the word to be read.
 * @return the read word.
 * @note Exception is thrown whenever the word to read exceed row length, which is an integer constant (ROW = 100).
 */
string ariel::Notebook::read(unsigned int page, unsigned int row, unsigned int column, Direction direction, unsigned int length) {
    string currPage = _pages.at(page);

    unsigned int pos = row > 1 ? (ROW + 1) * (row - 1) + column : column;
    string readStr;

    if (direction == Direction::Horizontal){
        if (column + length > ROW - 1){
            throw errc::result_out_of_range;
        }

        for (size_t i = pos; i < pos + length; ++i) {
            readStr += currPage[i];
        }
    }
    else{  // direction is Vertical

        unsigned int MaxLineIndex = currPage.length() / (ROW + 1);

        if (row + length > MaxLineIndex){
            unsigned int linesToCreate = (row + length) - MaxLineIndex;
            currPage = addLines(currPage, linesToCreate);
        }

        for (size_t i = 0; i < length; ++i) {
            readStr += currPage[pos + (ROW + 1) * i];
        }
    }
    return readStr;
}

/**
 * Method shows the page by a given index.
 * The representation of the page determined according to the example representation in Demo.cpp file.

   E.G.:      1:   _________
		      2:   ___~_____
		      3:   __a~cd___
		      4:   ___~_____
		      .
		      .
		      .
		      100: _________

 * @param page a page index to the page string.
 */
void ariel::Notebook::show(unsigned int page) {
    string currPage = _pages.at(page);  // throws exception if page doesn't exist.
    unsigned int MaxLineIndex = currPage.length() / (ROW + 1);
    unsigned int MaxLineLen = ((unsigned int)(log10(MaxLineIndex))) + 1;

    for (size_t i = 0, line = 1; i < currPage.length(); ++i) {
        unsigned int lineNumLen = ((unsigned int)(log10(line)));
        string space = string(MaxLineLen - lineNumLen, ' ');
        if (i % (ROW + 1) == 0){
            cout << line << ':' << space;
            line++;
        }
        else{
            cout << currPage[i];
        }
    }
}

/**
 * Function creates new line to the given page.
 * @param page a given page
 * @return the page after adding new line to it.
 */
string createNewLine(string page){
    for (int i = 0; i < ROW; ++i) {
        page += '_';
    }
    page += '\n';
    return page;
}

/**
 * Function add lines to the page string by a given amount.
 * @param str a given string page.
 * @param amount the amount of lines to add.
 * @return the new page string.
 */
string addLines(string str, unsigned int amount){
    for (int i = 0; i < amount; ++i) {
        str = createNewLine(str);
    }
    return str;
}