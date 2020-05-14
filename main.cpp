#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

class coordinates {
public:

    coordinates(float x, float y, float z) : x(x), y(y), z(z) {}

    float getX() { return x; }

    float getY() { return y; }

    float getDistance(vector<coordinates *> vec, int parent, int neighbour) {
        float x1 = vec[parent - 1]->getX();
        float y1 = vec[parent - 1]->getY();
        float x2 = vec[neighbour - 1]->getX();
        float y2 = vec[neighbour - 1]->getY();
        float distance = sqrt(pow(x2 - x1, 2) +
                              pow(y2 - y1, 2) * 1.0);

        return distance;
    }

private:
    float x;
    float y;
    float z;
};

class Node {
    int neighbour;
    float distance;
public:

    Node(int _neighbour, float _distance) : neighbour(_neighbour), distance(_distance) {}

    int getNeighbour() const { return neighbour; }

private:

};

typedef vector<int> numVec;
typedef vector<numVec> adjMat;
typedef list<Node *> neighbours;
vector<coordinates *> coordinatesList;
typedef vector<neighbours> collection;

void createAdjMat(adjMat &m1, const collection &collection1) {
    for (auto &i : collection1) {
        numVec numVec1(collection1.size());
        for (Node *node : i) {
            int num = node->getNeighbour();
            if (num != 0) {
                numVec1[num - 1] = 1;
            }
        }
        m1.push_back(numVec1);
    }
}

void matPow(const adjMat m1, adjMat &m2, adjMat &temp, adjMat &mult, int pow) {
    int i = 1;
    while (i < pow) {
        for (unsigned int c = 0; c < m1.size(); c++) {
            numVec vec(m1.size(), 0);
            temp.push_back(vec);
            mult.push_back(vec);
        }
        for (int r = 0; r < m1.size(); r++) {
            for (int c = 0; c < m1[r].size(); c++) {
                for (int k = 0; k < m1[r].size(); k++) {
                    temp[r][c] += m1[r][k] * m2[k][c];
                }
            }
        }
        mult = temp;
        m2 = temp;
        temp.clear();
        i++;
    }
    if (pow == 1) {
        mult = m1;
    }
}

int main(int argc, char *argv[]) {
    int n;
    int m;
    string xyzPath;
    string graphPath;
    collection collection1;

    string file;
    string graph = ".osm.graph";
    string xyz = ".osm.xyz";
    cin >> file;
    string temp = file;
    if (file.rfind(".osm.graph") != true) {
        graphPath = file.append(graph);
    }
    if (file.rfind(".osm.xyz") != true) {
        xyzPath = temp.append(xyz);
    }
    //Read in .xyz file
    ifstream xyzFile;
    xyzFile.open(xyzPath.c_str());
    if (!xyzFile) {
        cout << "Error: file could not be opened" << endl;
        exit(1);
    } /*else {
        cout << "File read successfully" << endl;
    }*/
    while (!xyzFile.eof()) {
        float x, y, z;
        xyzFile >> x;
        xyzFile >> y;
        xyzFile >> z;
        auto *coordinate = new coordinates(x, y, z);
        coordinatesList.push_back(coordinate);
    }
    xyzFile.close();
    //cout << "Finished reading file" << endl;

    //Read in .graph file
    ifstream graphFile;
    string line;
    int lineNum = 0;
    graphFile.open(graphPath.c_str());
    if (!graphFile) {
        cout << "Error: file could not be opened" << endl;
        exit(1);
    } /*else {
        cout << "File read successfully" << endl;
    }*/
    while (!graphFile.eof()) {
        while (getline(graphFile, line)) {
            std::string::size_type t = line.find('%');
            if (t != std::string::npos) {
                continue;
            }
            istringstream num(line);
            if (lineNum == 0) {
                num >> n;                                           //Record number of vertices and edges
                num >> m;
                lineNum++;
            } else if (line.empty() && lineNum <= n) {
                Node *node;
                node = new Node(0, 0);          //Record nodes with zero neighbours
                neighbours neighbours1;
                neighbours1.push_back(node);
                collection1.push_back(neighbours1);
                lineNum++;
            } else {
                neighbours neighbours1;                              //Create List of neighbours for line number
                while (!num.eof() && lineNum <= n) {
                    int neighbour;
                    num >> neighbour;
                    Node *node;
                    coordinates *coordinate = coordinatesList[lineNum - 1];
                    node = new Node(neighbour, coordinate->getDistance(coordinatesList, lineNum, neighbour));
                    neighbours1.push_back(node);                    //Add neighbours to list
                }
                collection1.push_back(neighbours1);                  //Add List to vector
                lineNum++;
            }
        }
    }
    graphFile.close();
    //cout << "Finished reading file" << endl;

    int query;
    cin >> query;
    switch (query) {
        case 1: {
            cout << "n= " << n << " m= " << m << endl;
            exit(1);
        }
        case 2: {
            int size = (collection1[0].size());
            int index = 1;
            for (int i = 0; i < collection1.size(); i++) {
                if (size < collection1[i].size()) {
                    index = i + 1;
                    size = collection1[i].size();
                }
            }
            cout << "v= " << index << " |N(v)|= " << size;
            exit(1);
        }
        case 3: {
            float totalNeighbours = 0.0;
            for (auto &i : collection1) {
                totalNeighbours += i.size();
            }
            float average = totalNeighbours / collection1.size();
            cout << "avg |N(v)|= " << fixed << average << setprecision(6);
            exit(1);
        }
        case 4: {
            int node;
            cin >> node;
            cout << "N(" << node << ")= ";
            for (const auto &neighbour : collection1[node - 1]) {
                cout << neighbour->getNeighbour() << " ";
            }
            exit(1);
        }
        case 5: {
            int parent;
            int distance;
            cin >> parent;
            cin >> distance;
            adjMat adjMat1;
            adjMat adjMat2;
            adjMat tempMat;
            adjMat res;
            createAdjMat(adjMat1, collection1);
            createAdjMat(adjMat2, collection1);
            cout << "N(" << parent << "," << distance << ")=";
            matPow(adjMat1, adjMat2, tempMat, res, distance);
            for (int i = 0; i < res.size(); i++) {
                if (res.at(parent - 1).at(i) != 0) {
                    cout << i + 1 << " ";
                }
            }
            exit(0);
        }
        case 6: {
            int start;
            int finish;
            cin >> start;
            cin >> finish;
            cout << "Path: " << start;
            cout << "d(" << start << "," << finish << ")=";

            exit(0);
        }
        case 7: {
            int start;
            int finish;
            cin >> start;
            cin >> finish;
            cout << "Path: " << start;
            cout << "ed(" << start << "," << finish << ")=";

            exit(0);
        }
    }
}

