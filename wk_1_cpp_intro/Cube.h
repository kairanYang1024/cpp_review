//encapsulation: enclose data and functionality into a single unit (class)

//public members can be accessed by client code (outside code that imports this class)
//private members cannot be accessed by client code, only inside the class itself
//interface: .h file (header): declare all mem vars and mem funcs, no function (like api)
//implementation: .cpp file (source)

#pragma once //present in .h file, non-standard but widely used, reduce build time and preventing double-inclusion
/**If certain C or C++ language constructs are defined twice, the resulting translation unit is invalid.
 *  #include guards prevent this erroneous construct from arising by the double inclusion mechanism.
**/

//specify the impled cube in the uiuc namespace
namespace uiuc {
    class Cube { //scope of a class
        public:
            double getVolume();
            double getSurfaceArea();
            double getLength();
            void setLength(double length);
        
        private:
            double length_; //google-style specification on var name underscore=private member
    }; //always end w/ semicolon
}