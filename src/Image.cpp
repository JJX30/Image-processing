//
// Created by Mauricio Del Castillo on 7/7/22.
//

#include "Image.h"

Image::Image(Header imageHeader, vector<Pixel> pixelList){
    this->imageHeader = imageHeader;
    this->pixelList = pixelList;
}

Header Image::GetHeader() {
    return this->imageHeader;
}

vector<Pixel> &Image::GetPixelList() {
    return this->pixelList;
}
