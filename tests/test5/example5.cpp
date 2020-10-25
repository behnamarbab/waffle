#include<iostream>
#include<fstream>
#include<cstring>
#include<queue>
using namespace std;

#define pii pair<int, int> 

const unsigned int MAP_SIZE = 8;
const unsigned int BUFFER_SIZE = MAP_SIZE * MAP_SIZE;

bool  MP[MAP_SIZE][MAP_SIZE] = {
    {0, 1, 1, 0, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 0, 1, 0},
};

bool  vis[MAP_SIZE][MAP_SIZE];

char buffer[BUFFER_SIZE];

pii dirs[] = {
    make_pair(-1, 0),   // up
    make_pair( 1, 0),   // dw
    make_pair( 0, 1),   // ri
    make_pair(0, -1)    // le
};

inline bool isOK(pii p) {
    return p.first<MAP_SIZE && p.second<MAP_SIZE && p.first>=0 && p.second>=0;
}

int bfs(pii s, pii d) {
    if(MP[s.first][s.second]) return 1;
    queue<pii> q;
    memset(vis, 0, sizeof vis);

    q.push(s);
    vis[s.first][s.second] = 1;

    while(q.size()) {
        pii cur = q.front();
        if(cur==d){
            cout << "Found one!" << endl;
            return 0;
        }
        q.pop();
        for(auto d: dirs) {
            pii tmp = make_pair(d.first+cur.first, d.second+cur.second);
            if(isOK(tmp) && !MP[tmp.first][tmp.second] && !vis[tmp.first][tmp.second]) {
                vis[tmp.first][tmp.second] = 1;
                q.push(tmp);
            }
        }
    }

    return 1;
}

void parseInput() {
    for(int i=0; i<MAP_SIZE; i++) {
        for(int j=0; j<MAP_SIZE; j++) {
            MP[i][j] = (buffer[i]>>(MAP_SIZE - j)) & 1;
        }
    }
}

void printMap() {
    for(int i = 0; i<MAP_SIZE; i++) {
        for(int j = 0; j<MAP_SIZE; j++) {
            cout << ('#'?MP[i][j]:' ');
        }
        cout << endl;
    }
}

int main(int argc, char *argv[]) {
    if(argc<2) {
        std::cerr << "You must specify the input file" << std::endl;
        return 101;
    }

    std::ifstream input_file;
    input_file.open(argv[1]);
    if(!input_file.is_open()) {
        std::cerr << "Could not open the input file." << std::endl;
        return 102;
    }

    char c, i=0;

    while(input_file.get(c) && i<BUFFER_SIZE) {
        
        buffer[i++] = c;
    }
    if(i<BUFFER_SIZE) return 1;

    parseInput();

    printMap();

    bool res = bfs(pii(0, 0), pii(MAP_SIZE-1, MAP_SIZE-1));

    if(!res) {
        throw "Bad is bad";
        return 2;
    }

    return 0;
}