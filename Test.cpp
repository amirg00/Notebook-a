/**
 * @author Amir Gillette
 * @date 2021-03-09
 */

#include "doctest.h"
#include "Notebook.hpp"
using namespace ariel;

#include <string>
#include <algorithm>
using namespace std;




/**
 * Test checks for invalid chars inside an inserted word.
 */
TEST_CASE("Invalid Chars"){
    ariel::Notebook notebook;
    CHECK_THROWS(notebook.write(0, 34, 75, Direction::Vertical, "cpp~is~hard!"));
    CHECK_THROWS(notebook.write(0, 34, 74, Direction::Vertical, "cpp\nis\nhard!"));
}

/**
 * Test for invalid operations.
 */
TEST_CASE("Invalid Operations Order"){
    ariel::Notebook notebook;
    notebook.write(0, 0, 0, Direction::Horizontal, "blabla");
    notebook.erase(0,3,0,Direction::Vertical,6);
    // Writing onto deleted text.
    CHECK_THROWS(notebook.write(0, 3, 0, Direction::Vertical, "a"));
    // Writing onto existed text.
    CHECK_THROWS(notebook.write(0, 0, 0, Direction::Vertical, "a"));


}

/**
 * Test checks for word which exceed the constant row length (ROW = 100).
 */
TEST_CASE("Exceeding Row"){
    ariel::Notebook notebook;
    // row starts at index 0 to 99 included.
    CHECK_NOTHROW(notebook.write(0, 0, 95, Direction::Horizontal, "hello"));
    CHECK_THROWS(notebook.write(0, 1, 95, Direction::Horizontal, "hello!"));

    // Generates a random alphabetic word in length 100
    string randWord;
    int len = 100, charsInAlphabet = 26;
    for (int i = 0; i < len; ++i) {
        char randChar = 'a' + (rand() % charsInAlphabet);
        randWord += randChar;
    }
    cout << randWord.length() << endl;
    CHECK_NOTHROW(notebook.write(1, 0, 0, Direction::Horizontal, randWord));
    CHECK_THROWS(notebook.write(1, 1, 0, Direction::Horizontal, randWord + "D"));



}


/**
 * This test regular actions combine write to page, read from page, and erase a certain word/sentence.
 */
TEST_CASE("Write-Read-Erase"){
    ariel::Notebook notebook;
    CHECK_NOTHROW(notebook.write(0, 0, 0, Direction::Horizontal, "hey_there"));
    notebook.show(0);
    CHECK_NOTHROW(notebook.write(0, 0, 3, Direction::Vertical, "Nope"));

    // heyNthere
    string read = notebook.read(0, 0, 1, Direction::Horizontal, 3);
    CHECK(read == "eyN");
    read = notebook.read(0, 0, 3, Direction::Vertical, 4);
    CHECK(read == "Nope");

    CHECK_NOTHROW(notebook.erase(0, 0, 3, Direction::Horizontal, 6));
    read = notebook.read(0, 0, 0, Direction::Horizontal, 9);
    CHECK(read == "hey~~~~~~");


    CHECK_NOTHROW(notebook.erase(0, 1, 3, Direction::Vertical, 3));
    read = notebook.read(0, 0, 3, Direction::Vertical, 4);
    CHECK(read == "~~~e");

    CHECK_NOTHROW(notebook.show(0));
    CHECK_THROWS(notebook.show(1));

    CHECK_NOTHROW(notebook.write(1, 0, 0, Direction::Horizontal, "THIS IS PAGE 1!"));
    CHECK_NOTHROW(notebook.show(1));

}