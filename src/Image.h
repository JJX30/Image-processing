//
// Created by Mauricio Del Castillo on 7/7/22.
//

#ifndef PROJECT2_IMAGE_H
#define PROJECT2_IMAGE_H
#include "Header.h"
#include "Pixel.h"
#include <vector>
using namespace std;

class Image {
    Header imageHeader;
    vector<Pixel> pixelList;

public:
    Image(Header imageHeader, vector<Pixel> pixelList);
    Header GetHeader();
    vector<Pixel> &GetPixelList();
};


#endif //PROJECT2_IMAGE_H
