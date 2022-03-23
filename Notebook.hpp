#include <iostream>
#include <vector>
#include "Direction.hpp"

using namespace std;

#define ROW 100

namespace ariel{
    class Notebook{
    vector<string> _pages;
    public:
        void write(unsigned int page, unsigned int row, unsigned int column, Direction direction, string const& word);
        string read(unsigned int page, unsigned int row, unsigned int column, Direction direction, unsigned int length);
        void erase(unsigned int page, unsigned int row, unsigned int column, Direction direction, unsigned int length);
        void show(unsigned int page);
    };
}
