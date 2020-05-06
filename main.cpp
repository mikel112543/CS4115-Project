#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>

using namespace std;

class coordinates {
public:
    coordinates(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float getX() { return x;}

    float getY() { return y;}

    float getZ() { return z;}

    void setX(float _x) {
        x = _x;
    }

    void setY(float _y) {
        y = _y;
    }

    void setZ(float _z) {
        z = _z;
    }


private:
    float x;
    float y;
    float z;
};

class Node {
public:
    Node(int _parent, int _neighbour) {
        this->parent = _parent;
        this->neighbour = _neighbour;
    }
    int getParent() { return parent; }
    int getNeighbour() { return neighbour; }


private:
    int parent;
    int neighbour;


};


int main() {
    string xyzPath;
    string graphPath;
    vector<coordinates> coordinatesList;
    typedef list<Node> neighbours;
    vector<neighbours> collection;

    string file;
    string graph = ".osm.graph";
    string xyz = ".osm.xyz";
    cin >> file;
    string temp = file;
    if(file.rfind(".osm.graph") != true) {
        graphPath = file.append(graph);
    }if(file.rfind(".osm.xyz") != true) {
        xyzPath = temp.append(xyz);
    }
        ifstream xyzFile;
        xyzFile.open(xyzPath.c_str());
        if(!xyzFile) {
            cout << "Error: file could not be opened" << endl;
            exit(1);
        }else {
            cout << "File read successfully" << endl;
        }
        while(!xyzFile.eof()) {
            float x, y, z;
            xyzFile >> x;
            xyzFile >> y;
            xyzFile >> z;
            coordinates node(x, y, z);
            coordinatesList.push_back(node);
        }
        xyzFile.close();
        cout << "Finished reading file" << endl;



    ifstream graphFile;
    string line;
    int numberOfNeighbours = 0;
    int lineNum = 0;
    graphFile.open(graphPath.c_str());
    if(!graphFile) {
        cout << "Error: file could not be opened" << endl;
        exit(1);
    }else{
        cout << "File read successfully" << endl;
    }
    while(!graphFile.eof()) {
        while(getline(graphFile,line)) {
            std::string::size_type n = line.find('%');
            if (n != std::string::npos) {
                continue;
            }
            istringstream num(line);
            for(int i = 0; line[i] != '\0'; i++) {
                if(line[i] == ' ') {
                    numberOfNeighbours++;
                }
            }
            numberOfNeighbours+=1;
            for(int i = 0; i < numberOfNeighbours; i++) {
                int temp;
                num >> temp;
                //Node node(lineNum, temp);
                neighbours neighbours1;
                neighbours1.push_back(Node(lineNum, temp));
                collection.push_back(neighbours1);
                //Node inverse(temp, lineNum);
                //neighbours.push_back(inverse);
            }
            lineNum++;
            numberOfNeighbours = 0;
        }
    }
    cout << "Finished reading file" << endl;
}
