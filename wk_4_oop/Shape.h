#pragma once

class Shape {
    public:
        Shape(); //can be abstract and never implemented in Shape layer
        Shape(double width);
        double getWidth() const;

    private:
        double width_;
};