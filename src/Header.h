//
// Created by Mauricio Del Castillo on 7/7/22.
//

#ifndef PROJECT2_HEADER_H
#define PROJECT2_HEADER_H


struct Header {
    char idLength;
    char colorMapType;
    char dataTypeCode;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
};


#endif //PROJECT2_HEADER_H
