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
    Node();

    Node(int _neighbour, float _distance) : neighbour(_neighbour), distance(_distance) {}

    int getNeighbour() { return neighbour; }

    float getDistance(vector<coordinates *> vec, int parent, int neighbour) {
        float x1 = vec[parent - 1]->getX();
        float y1 = vec[parent - 1]->getY();
        float x2 = vec[neighbour - 1]->getX();
        float y2 = vec[neighbour - 1]->getY();
        distance = sqrt(pow(x2 - x1, 2) +
                        pow(y2 - y1, 2) * 1.0);

        return distance;
    }

    void setDistance(float _distance) { distance = _distance; }

private:

};

typedef list<Node *> neighbours;
vector<coordinates *> coordinatesList;

vector<vector<int>> matPow(vector<vector<int>> mat1, vector<vector<int>> &mat2, int pow) {
    vector<vector<int>> mult(mat1.size(), vector<int>(mat1.size(), 0));
    int i = 1;
    while (i < pow) {
        for (int r = 0; r < mat1.size(); r++) {
            for (int c = 0; c < mat1[r].size(); c++) {
                for (int k = 0; k < mat1[r].size(); k++) {
                    mult[r][c] += mat1[r][k] * mat2[k][c];
                }
            }
        }
        mat2 = mult;
        mult = vector<vector<int>>(mat2.size(), vector<int>(mat2.size(), 0));
        i++;
    }
    /*for (int r = 0; r < mult.size(); r++) {
        for (int c = 0; c < mult.size(); c++) {
            std::cout << mat2[r][c] << " ";
        }
    }*/
    return mat2;
}

int main(int argc, char *argv[]) {
    int n;
    int m;
    string xyzPath;
    string graphPath;
    vector<neighbours> collection;

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
    } else {
        cout << "File read successfully" << endl;
    }
    while (!xyzFile.eof()) {
        float x, y, z;
        xyzFile >> x;
        xyzFile >> y;
        xyzFile >> z;
        auto *coordinate = new coordinates(x, y, z);
        coordinatesList.push_back(coordinate);
    }
    xyzFile.close();
    cout << "Finished reading file" << endl;

    //Read in .graph file
    ifstream graphFile;
    string line;
    int lineNum = 0;
    graphFile.open(graphPath.c_str());
    if (!graphFile) {
        cout << "Error: file could not be opened" << endl;
        exit(1);
    } else {
        cout << "File read successfully" << endl;
    }
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
                collection.push_back(neighbours1);
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
                collection.push_back(neighbours1);                  //Add List to vector
                lineNum++;
            }
        }
    }
    graphFile.close();
    cout << "Finished reading file" << endl;

    int query;
    cin >> query;
    switch (query) {
        case 1: {
            cout << "n= " << n << " m= " << m << endl;
            break;
        }
        case 2: {
            int size = (collection[0].size());
            int index = 1;
            for (int i = 0; i < collection.size(); i++) {
                if (size < collection[i].size()) {
                    index = i + 1;
                    size = collection[i].size();
                }
            }
            cout << "v= " << index << " |N(v)|= " << size;
            break;
        }
        case 3: {
            float totalNeighbours = 0.0;
            for (auto &i : collection) {
                totalNeighbours += i.size();
            }
            float average = totalNeighbours / collection.size();
            cout << "avg |N(v)|= " << average << setprecision(6);
            break;
        }
        case 4: {
            int node;
            cin >> node;
            cout << "N(" << node << ")= ";
            for (const auto &neighbour : collection[node - 1]) {
                cout << neighbour->getNeighbour() << " ";
            }
            break;
        }
        case 5: {
            int parent;
            int distance;
            cin >> parent;
            cin >> distance;
            vector<vector<int>> adjMat(n, vector<int>(n, 0));
            for (int i = 0; i < collection.size() / 2; i++) {
                for (Node *node : collection[i]) {
                    int num = node->getNeighbour();
                    if (num != 0) {
                        adjMat.at(i).at(num - 1) = 1;
                        adjMat.at(num - 1).at(i) = 1;
                    }
                }
            }
            for (auto &row : adjMat) {
                for (int &col : row) {
                    std::cout << col << ' ';
                }
            }
            cout << "N(" << parent << "," << distance << ")=";
            for(int i = 0; i < matPow(adjMat, adjMat, distance).size(); i++) {
                if(matPow(adjMat, adjMat, 4).at(parent-1).at(i) != 0) {
                    cout << i + 1 << " ";
                }
            }

            /* string nodes;
             getline(std::cin,nodes);
             getline(std::cin,nodes);
             istringstream num(nodes);
             string userInputNodes;
             vector<int> vec;
             int node;
             while (!(num.eof())) {
                 num >> node;
                 //while(cin >> node){
                 userInputNodes += to_string(node) + ",";
                 //list<Node*>::const_iterator rit;
                 for (const auto &neighbour : collection[node - 1]) {
                     //for(rit = collection[node-1].begin(); rit != collection[node-1].end(); rit++) {
                     //Node *node1 = *rit;
                     vec.push_back(neighbour->getNeighbour());
                 }
             }
             sort(vec.begin(), vec.end());
             for (int i = 0; i < vec.size()-1; i++) {
                 if (vec[i] == vec[i + 1]) {
                     vec.erase(vec.begin() + i);
                 }
             }
             cout << "N(" << userInputNodes << ")= ";
             for (int neighbour : vec) {
                 cout << neighbour << " ";
             }
             break;*/
            break;
        }
    }
}

