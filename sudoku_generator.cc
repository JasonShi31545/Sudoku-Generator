#include <bits/stdc++.h>
#include <random>
using namespace std;

#define ul unsigned long

#define x first
#define y second

// ####### DEBUGGING #############
void print_pair(pair<int, int> p) {
    cout << p.x << ", " << p.y << endl;
}
void print_coordinates(vector<pair<int,int>> coordinates) {
    for (int i = 0; i < coordinates.size(); i++) {
        print_pair(coordinates[i]);
    }
}

// ###### END DEBUGGING SECTION ##########

int random_number() {
    /*
      Generates a random number between 1...9
    */
    const int range_from = 1;
    const int range_to = 9;
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<int> distr(range_from, range_to);
    return distr(generator);
}

vector<vector<int>> grid;

map<pair<int, int>, int> block_mapping;
map<int, vector<pair<int,int>>> reverse_block_mapping;

void init_block_mapping() {
    int k;
    k = 1; // reverse block mapping setup
    for (k = 1; k <= 9; k++) {
        vector<pair<int,int>> coordinates;
        int startx = 3 * ((k-1) / 3);
        int starty = (k % 3 == 0) ? 6 : (k % 3 - 1) * 3;
        // initialise a block
        for (int l = 0; l < 3; l++) {
            for (int m = 0; m < 3; m++) {
                coordinates.push_back(make_pair(startx + l, starty + m));
            }
        }
        // print_coordinates(coordinates);
        assert((int)coordinates.size() == 9);
        reverse_block_mapping[k] = coordinates;
    }

    k = 1; // block mapping set
    for (k = 1; k <= 9; k++) {
        vector<pair<int, int>> coordinates = reverse_block_mapping[k];
        for (auto coord : coordinates) {
            block_mapping[coord] = k;
        }
    }

}

void check_row(pair<int, int> coord, bitset<10> &availability) {
    int row_id = coord.x;
    for (int i = 0; i < 9; i++) {
        if (grid[row_id][i] == 0) continue;
        availability.set(grid[row_id][i], 0);
    }
    // cerr << availability << endl;
}

void check_col(pair<int, int> coord, bitset<10> &availability) {
    int col_id = coord.y;
    for (int i = 0; i < 9; i++) {
        if (grid[i][col_id] == 0) continue;
        availability.set(grid[i][col_id], 0);
    }
    // cerr << availability << endl;
}


void check_block(pair<int, int> coord, bitset<10> &availability) {
    int block = block_mapping[coord];
    vector<pair<int,int>> coordinates_to_check = reverse_block_mapping[block];
    for (auto coord : coordinates_to_check) {
        int _x = coord.x;
        int _y = coord.y;
        int element = grid[_x][_y];
        if (element != 0) {
            availability.set(grid[_x][_y], 0);
        }
    }
    // cerr << availability << endl;
}

int s;

void print_grid() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}
void print_grid_unsolved() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (grid[i][j] != 0) {
                cout << grid[i][j] << " ";
            } else {
                cout << 'X' << " ";
            }
        }
        cout << endl;
    }
}
// void generate_grid(int row, int col, bitset<10> availability = 0x3FF) {
//     const pair<int, int> coord = make_pair(row, col);
//     if (availability.to_ulong() == (ul)0x1) { // we are out of numbers
//         if (col == 0) { // col = 8, row--;
//             generate_grid(row - 1, 8); // TODO: check again
//         } else { // col--
//             generate_grid(row, col - 1); // TODO: same here
//         }
//     } else { // we still have numbers left
//         int rnumber = random_number();
//         check_block(coord, availability);
//         check_col(coord, availability);
//         check_row(coord, availability);
//         if (availability[rnumber] == 0) { // conflict
//             availability[rnumber] = 0;
//             generate_grid(row, col, availability);
//         } else { // no conflict
//             grid[row][col] = rnumber;
//             if (row == 8 && col == 8) {
//                 // we're done
//                 return;
//             } else {
//                 if (col == 8) {
//                     generate_grid(row + 1, 0);
//                 } else {
//                     generate_grid(row, col + 1);
//                 }
//             }
//         }
//     }
// }

bool generate_grid(int row = 0, int col = 0, bitset<10> availability = 0x3FF) {
    // false for failed
    assert(row <= 9 && col < 9 && (row >= 0 && col >= 0));
    const pair<int, int> coord = make_pair(row, col);
    check_block(coord, availability);
    check_row(coord, availability);
    check_col(coord, availability);

    if (availability.to_ulong() == (ul)0x1) {
        // out of numbers
        // go back one square
        // if (col == 0) {
        //     return generate_grid(row - 1, 8, 0x3FF); // regenerate 
        // } else {
        //     return generate_grid(row, col - 1, 0x3FF);
        // }
        grid[row][col] = 0;
        return false;
    } else { // not out of numbers
        int rnumber = random_number();
        while (availability[rnumber] != 1) {
            rnumber = random_number();
        }
        // USE IT!
        grid[row][col] = rnumber;
        if (row == 8 && col == 8) { return true; }
        bool result;
        if (col == 8) {
            result = generate_grid(row + 1, 0, 0x3FF);
        } else {
            result = generate_grid(row, col + 1, 0x3FF);
        }
        if (result == false) {
            // fail
            availability[rnumber] = 0;
            return generate_grid(row, col, availability);
        } else {
            return true;
        }
    }
}


int main(int argc, const char *argv[]) {

    init_block_mapping();

    assert(block_mapping.size() == 81);
    assert(reverse_block_mapping.size() == 9);

    // initialise grid
    grid.resize(9);
    for (int i = 0; i < 9; i++) {
        grid[i].resize(9);
        for (int j = 0; j < 9; j++) {
            grid[i][j] = 0;
        }
    }

    s = 0;

    bool a = generate_grid();
    assert(a);

    double percentage = 0.80;
    if (argc > 1) {
        percentage = atoi(argv[1]);
        if (percentage < 0 || percentage > 100) {
            cerr << "Personalised difficulty must be between 0 and 100 (inclusive)" << endl;
            return -1;
        }
        percentage /= 100;
    }
    int gone_number = ceil(percentage * 81);
    
    for (int i = 0; i < gone_number; i++) {
        // remove a random coordinate;
        while (true) {
            int randx = random_number() - 1;
            int randy = random_number() - 1;
            if (grid[randx][randy] != 0) {
                grid[randx][randy] = 0;
                break;
            }
        }
    }


    
    print_grid_unsolved();
    
    return 0;
}
