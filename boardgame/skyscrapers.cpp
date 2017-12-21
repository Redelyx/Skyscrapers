#include "skyscrapers.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <QMessageBox>

using namespace std;

vector<float> split(string txt, char sep){
    vector<float> values;
    istringstream sstr{txt};
    string item;
    while (getline(sstr, item, sep)){
        values.push_back(stof(item));
    }
    return values;
}

vector<vector<int>> m0;
int n0 = 0;

void read_file(string filename, vector<vector<int>> &m0, int &n0){
    n0 = 0;
    vector<int> matrix = {};
    int i = 0;
    string line;
    ifstream f1{filename};
    while (getline(f1, line)) {
        auto values = split(line, ',');
        if (n0 == 0) n0 = values.size();
        for (auto v : values) {
            matrix.push_back(v);
        }
    }
    f1.close();

    m0.assign(n0, vector<int>(n0, 0));

    for (auto j = 0; j < n0; j++){
         for (auto k = 0; k < n0; k++){
             m0[j][k] = matrix[i];
             i++;
         }
    }
}

bool n_roof(vector<int> v, int vincolo){
    int roof = 0, max_line = 0;
    if(vincolo != 0){
        for (auto y = 0; y < v.size(); y++){
            if (max_line < v[y]){
                max_line = v[y];
                roof++;
            }
        }
    }
    if (vincolo == roof) return true;
    else return false;
}



Skyscrapers::Skyscrapers(string filename)
{
    read_file(filename, m0, n0);
    matrix_ = m0;
    side_ = n0;
    filename_ = filename;
    separator_ = "-----------------------";

    cout << "you are playing on " << side_-2 << "x" << side_-2 << endl;

}

void Skyscrapers::play_at(int x0, int y0)
{
    if (x0 > 0 && x0 < side_-1 && y0 > 0 && y0 < side_-1){
        if (matrix_[x0][y0] == side_-2) matrix_[x0][y0]= 0;
        else matrix_[x0][y0]++;
    }
    else cout << "Error!" << endl;

    for (auto a = 0; a < side_; ++a) {
        cout << "\t\t";
        for (auto b = 0; b < side_; ++b) {
            cout << matrix_[a][b] << ' ';
        }
        cout << endl;
    }
    cout << endl << "\t" << separator_ << endl << endl;
}

void Skyscrapers::auto_hint()
{
    for (auto a = 0; a < side_; ++a){
        // colonna 0
        if (matrix_[a][0]!=0){
            for (auto b = 1; b < side_-1; b++){
                if (matrix_[a][0] == side_-2) matrix_[a][b] = b;
            }
            if (matrix_[a][0] == 1) matrix_[a][1] = side_-2;
        }
        // riga 0
        if (matrix_[0][a]!=0){
            for (auto b = 1; b < side_-1; b++){
                if (matrix_[0][a] == side_-2) matrix_[b][a] = b;
            }
            if (matrix_[0][a] == 1) matrix_[1][a] = side_-2;
        }
        //colonna n
        if (matrix_[a][side_-1]!=0){
            for (auto b = 1; b < side_-1; b++){
                 if (matrix_[a][side_-1] == side_-2) matrix_[a][b] = (side_-2) - (b-1);
            }
            if (matrix_[a][side_-1] == 1) matrix_[a][side_-2] = side_-2;
        }
        // riga n
        if (matrix_[side_-1][a]!=0){
            for (auto b = 1; b < side_-1; b++){
                 if (matrix_[side_-1][a] == side_-2) matrix_[b][a] = (side_-2) - (b-1);
            }
            if (matrix_[side_-1][a] == 1) matrix_[side_-2][a] = side_-2;
        }
    }
}

void Skyscrapers::print_matrix(string filename)
{
    ofstream fout{filename};
    if (fout){
        for (auto a = 0; a < side_; ++a) {
            for (auto b = 0; b < side_; ++b) {
                fout << matrix_[a][b] << ',';
            }
            fout << "\n";
        }
    }
}

string Skyscrapers::get_val(int x, int y)
{
    return to_string(matrix_[x][y]);
}

bool Skyscrapers::finished()
{
    int i = 0, k_1 = 0, k_2 = 0;
    bool flag_full = false, flag_roof = false, flag_diff = false;
    int col_0 = 0, row_0 = 0, col_n = 0, row_n = 0;
    int r_0 = 0, c_0 = 0, r_n = 0, c_n = 0;
    int counter_col0 = 0, counter_row0 = 0, counter_coln = 0, counter_rown = 0;
    vector<int> v1, v2, v3, v4;

    //flag_full
    for (auto a = 1; a < side_-1; ++a) {
        for (auto b = 1; b < side_-1; ++b) {
            if(matrix_[a][b] != 0) i++;
        }
    }
    if (i == (side_-2)*(side_-2)) flag_full = true;

    //flag_roof
    for (auto a = 0; a < side_; ++a){
        // colonna 0
        if (matrix_[a][0]!=0){
            counter_col0++;
            col_0 = matrix_[a][0];
            r_0 = a;
            v1 = {};
            for (auto b = 1; b < side_-1; b++){
                v1.push_back(matrix_[r_0][b]);
            }
            if (n_roof(v1, col_0)) counter_col0--;
        }
        // riga 0
        if (matrix_[0][a]!=0){
            counter_row0++;
            row_0 = matrix_[0][a];
            c_0 = a;
            v2 = {};
            for (auto b = 1; b < side_-1; b++){
                v2.push_back(matrix_[b][c_0]);
            }
            if (n_roof(v2, row_0)) counter_row0--;
        }
        //colonna n
        if (matrix_[a][side_-1]!=0){
            counter_coln++;
            col_n = matrix_[a][side_-1];
            r_n = a;
            v3 = {};
            for (auto b = 1; b < side_-1; b++){
                v3.push_back(matrix_[r_n][b]);
            }
            reverse(v3.begin(), v3.end());
            if (n_roof(v3, col_n)) counter_coln--;

        }
        // riga n
        if (matrix_[side_-1][a]!=0){
            counter_rown++;
            row_n = matrix_[side_-1][a];
            c_n = a;
            v4 = {};
            for (auto b = 1; b < side_-1; b++){
                v4.push_back(matrix_[b][c_n]);
            }
            reverse(v4.begin(), v4.end());
            if(n_roof(v4, row_n)) counter_rown--;
        }
    }

    if(counter_col0 + counter_row0 + counter_coln + counter_rown == 0) flag_roof = true;

    //flag_diff
    for (auto a = 1; a < side_-2; a++){
        for (auto b = 1; b < side_-1; b++){
            for (auto c = a+1; c < side_-1; c++){
                if (matrix_[a][b] == matrix_[c][b])k_1++;
            }
            for (auto c = b+1; c < side_-1; c++){
                if (matrix_[a][b] == matrix_[a][c])k_2++;
            }
        }
    }

    if (k_1 == 0 && k_2 == 0) flag_diff = true;

    if (flag_full == true && flag_roof == true && flag_diff) return true;
    else return false;
}

