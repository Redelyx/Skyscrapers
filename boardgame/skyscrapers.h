#ifndef SKYSCRAPERS_H
#define SKYSCRAPERS_H
#include <vector>
#include <iostream>
#include "boardgame.h"

using namespace std;

class Skyscrapers : public BoardGame
{
public:
    Skyscrapers(string filename);
    void play_at(int x, int y);
    void auto_hint();
    void print_matrix(string filename);
    int cols() { return side_; }
    int rows() { return side_; }
    string get_val(int x , int y);
    bool finished();
    string message() { return "Solved!"; }
    string message_1() { return "You hint will be showed after the next move."; }
    ~Skyscrapers() {}

private:
    vector<vector<int>> matrix_;
    int side_;
    string filename_;
    string separator_;
};

#endif // SKYSCRAPERS_H
