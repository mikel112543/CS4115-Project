#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

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

int main() {

    vector<coordinates> coordinatesList;

    float x;
    float y;
    float z;
    ifstream indata;
    indata.open(R"(C:\Users\mikel\OneDrive - University of Limerick\2nd Year\Semester 2\CS4115 (Data Structures and Algorithms)\Project\belgium.osm.xyz)");
    if(!indata) {
        cout << "Error: file could not be opened" << endl;
        exit(1);
    }else {
        cout << "File read successfully" << endl;
    }
    while(!indata.eof()) {
            indata >> x;
            indata >> y;
            indata >> z;
            coordinates node(x, y, z);
            coordinatesList.push_back(node);
    }
    indata.close();
    cout << coordinatesList[1441294].getY() << endl;

}
