#pragma once 
//specify the impled cube in the uiuc namespace
namespace uiuc {
    class Cube {
        /*
            when instance of a class is created, the class constructor set up the initial
            state of the object.

            auto default ctor: intialize all member vars to default values
            custom default ctor: specify state of the object when being constructed
            mem func w/ same name of the class, 0 param, no return type
            
            custom ctor: specify arguments to define the properties of the object
            function overloading args for ctors
            if any custom ctor is defined, an auto ctor is overwritten (not defined)
        
            copy ctor: specifial ctor that exists to make a copy of existing object
            auto copy ctor provided if not writing one: copy content of all mem vars
            const: the arg cannot be modified when copying from it
            ref: aliasing the 'another', so don't let the arg be a copy itself of the source obj
            
            copy ctor invokation: 
                automatically, passing obj as a param by val
                returning an obj from a function by val -- return by ref of stack var cause undef
                init a new obj
            
            copy assignment operator: behavior when an object is copied using '='
            replaces a value to an [existing object], copy ctor can't do anything to an already-created obj
            define only when need to alloc external memo/let multiple obj point to same thing
            returning an alias of '*this' to smooth the effort of return value copying
        
            dtor: has auto dtor if not write any dtor, calls dtor of all member objs
            clean-up memory, invoked by 'delete' on heap OR by going out of scope on stack 
            essential when obj allocates external rsrc that must be closed/freed when obj is destoryed
            e.g. heap memory, open files (fopen), shared memory (like in LinkedList class)
        */
        public:
            Cube();
            Cube(double length);
            Cube(const Cube& another);
            ~Cube();

            //operator overloading: explicit keyword operator follow by the operator itself
            //also let chaining '=' be supported
            Cube& operator=(const Cube& another);

            double getVolume();
            double getSurfaceArea();
            double getLength();
            void setLength(double length);
        
        private:
            double length_; //default value initialisation: 0
    };
}