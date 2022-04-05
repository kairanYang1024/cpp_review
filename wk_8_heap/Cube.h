#pragma once 
#include <iostream>

class Cube {
    public:
        Cube();
        Cube(double length);
        Cube(const Cube& another);
        ~Cube();

        //operator overloading: explicit keyword operator follow by the operator itself
        //also let chaining '=' be supported
        Cube& operator=(const Cube& another);
        bool operator<(const Cube& another); //supporting comparing
        friend std::ostream& operator<<(std::ostream &os, const Cube& cube); //supporting printing

        double getVolume() const;
        double getSurfaceArea() const;
        double getLength() const;
        void setLength(double length);
        
    private:
        double length_; //default value initialisation: 0
};
