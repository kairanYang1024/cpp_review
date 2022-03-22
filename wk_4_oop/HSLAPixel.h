#pragma once

class HSLAPixel {
    public:
        HSLAPixel() {
            this->h = 0;
            this->s = 0;
            this->l = 0;
            this->a = 0;
        }
        
        HSLAPixel(double h, double s, double l, double a) {
            this->h = h;
            this->s = s;
            this->l = l;
            this->a = a;
        }

    private:
        double h; //0~360
        double s; //0~1
        double l; //0~1
        double a; //0~1
};