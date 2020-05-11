#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <cmath>

using namespace std;

class coordinates {
public:

    coordinates(float x, float y, float z) : x(x), y(y), z(z) {}

    float getX() { return x; }

    float getY() { return y; }

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

    int getNeighbour() { return neighbour; }

    int getDistance(vector<coordinates*> vec, int parent, int neighbour) {
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


int main() {
    int n;
    int m;
    string xyzPath;
    string graphPath;
    vector<coordinates*> coordinatesList;
    typedef list<Node*> neighbours;
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
                num >> n;                   //Record number of vertices and edges
                num >> m;
                lineNum++;
            } else if (line.empty() && lineNum != n) {
                    Node *node;
                    node = new Node(0, 0);          //Record nodes with zero neighbours
                    neighbours neighbours1;
                    neighbours1.push_back(node);
                    collection.push_back(neighbours1);
                    lineNum++;
                } else {
                    neighbours neighbours1;             //Create List of neighbours for line number
                    while (!num.eof() && lineNum != n) {
                        int neighbour;
                        num >> neighbour;
                        Node *node;
                        node = new Node(neighbour, node->getDistance(coordinatesList, lineNum, neighbour));
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
        case 1:
            cout << "n= " << n << " m= " << m << endl;
            break;
        case 2:;
    }
}
