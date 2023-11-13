//
// Created by Mauricio Del Castillo on 7/7/22.
//
#include <iostream>
#include <fstream>
#include <vector>
#include "Header.h"
#include "Pixel.h"
#include "Image.h"

using namespace std;

Image loadImage(const string &fileName){
    ifstream inFile(fileName, ios_base::binary);
    Header headerObject;

    // Getting the Image Description
    inFile.read(&headerObject.idLength, sizeof(headerObject.idLength));
    inFile.read(&headerObject.colorMapType, sizeof(headerObject.colorMapType));
    inFile.read(&headerObject.dataTypeCode, sizeof (headerObject.dataTypeCode));
    inFile.read((char *) &headerObject.colorMapOrigin, sizeof (headerObject.colorMapOrigin));
    inFile.read((char *) &headerObject.colorMapLength, sizeof (headerObject.colorMapLength));
    inFile.read(&headerObject.colorMapDepth, sizeof (headerObject.colorMapDepth));
    inFile.read((char *) &headerObject.xOrigin, sizeof (headerObject.xOrigin));
    inFile.read((char *) &headerObject.yOrigin, sizeof (headerObject.yOrigin));
    inFile.read((char *) &headerObject.width, sizeof (headerObject.width));
    inFile.read((char *) &headerObject.height, sizeof (headerObject.height));
    inFile.read(&headerObject.bitsPerPixel, sizeof (headerObject.bitsPerPixel));
    inFile.read(&headerObject.imageDescriptor, sizeof (headerObject.imageDescriptor));

    // Getting the Pixel Values
    vector<Pixel> pixelList;
    unsigned int pixelListLength = (int)headerObject.width * (int)headerObject.height;
    for(int i = 0; i < pixelListLength; i++){
        Pixel pixel;
        inFile.read((char *) &pixel.blue, sizeof(pixel.blue));

        inFile.read((char *) &pixel.green, sizeof(pixel.green));

        inFile.read((char *) &pixel.red, sizeof(pixel.red));

        pixelList.push_back(pixel);
    }
    Image image(headerObject, pixelList);
    return image;
}

bool comparePixelList(Image &image1, Image &image2){
    for(int i = 0; i < image1.GetPixelList().size(); i++){
        if((image1.GetPixelList()[i].red != image2.GetPixelList()[i].red) ||
           (image1.GetPixelList()[i].green != image2.GetPixelList()[i].green) ||
           (image1.GetPixelList()[i].blue != image2.GetPixelList()[i].blue)){
            return false;
        }
    }
    return true;
}

bool compareHeader(Image &image1, Image &image2){
    if(((int)image1.GetHeader().idLength == (int)image2.GetHeader().idLength) &&
    ((int)image1.GetHeader().colorMapType == (int)image2.GetHeader().colorMapType) &&
    ((int)image1.GetHeader().dataTypeCode == (int)image2.GetHeader().dataTypeCode) &&
    (image1.GetHeader().colorMapOrigin == image2.GetHeader().colorMapOrigin) &&
    (image1.GetHeader().colorMapLength == image2.GetHeader().colorMapLength) &&
    ((int)image1.GetHeader().colorMapDepth == (int)image2.GetHeader().colorMapDepth) &&
    (image1.GetHeader().xOrigin == image2.GetHeader().xOrigin) &&
    (image1.GetHeader().yOrigin == image2.GetHeader().yOrigin) &&
    (image1.GetHeader().width == image2.GetHeader().width) &&
    (image1.GetHeader().height == image2.GetHeader().height) &&
    ((int)image1.GetHeader().bitsPerPixel == (int)image2.GetHeader().bitsPerPixel) &&
    ((int)image1.GetHeader().imageDescriptor == (int)image2.GetHeader().imageDescriptor)){
        return true;
    }
    return false;
}

Image multiply(Image &topLayer, Image &bottomLayer){
    unsigned int length = topLayer.GetHeader().width * topLayer.GetHeader().height;
    vector<Pixel> newPixelList;

    Pixel pixel;
    for(int i = 0; i < length; i++){

        // Red value
        int redVal = (int)topLayer.GetPixelList()[i].red * (int)bottomLayer.GetPixelList()[i].red;
        float normRedVal = (float) redVal / 255.0f;
        int roundedNormRedVal = normRedVal + 0.5f;
        pixel.red = (unsigned char) roundedNormRedVal;

        // Green value
        int greenVal = (int)topLayer.GetPixelList()[i].green * (int)bottomLayer.GetPixelList()[i].green;
        float normGreenVal = (float) greenVal / 255.0f;
        int roundedNormGreenVal = normGreenVal + 0.5f;
        pixel.green = (unsigned char) roundedNormGreenVal;

        // Blue value
        int blueVal = (int)topLayer.GetPixelList()[i].blue * (int)bottomLayer.GetPixelList()[i].blue;
        float normBlueVal = (float) blueVal / 255.0f;
        int roundedNormBlueVal = normBlueVal + 0.5f;
        pixel.blue = (unsigned char) roundedNormBlueVal;

        newPixelList.push_back(pixel);
    }
    Image image(topLayer.GetHeader(), newPixelList);
    return image;
}

Image subtract(Image &topLayer, Image &bottomLayer){

    unsigned int length = topLayer.GetHeader().width * topLayer.GetHeader().height;
    vector<Pixel> newPixelList;

    Pixel pixel;
    for(int i = 0; i < length; i++){

        // Red Value
        int subtractedRedValue = (int) bottomLayer.GetPixelList()[i].red - (int) topLayer.GetPixelList()[i].red;
        if(subtractedRedValue < 0){
            subtractedRedValue = 0;
        }
        pixel.red = (unsigned char) subtractedRedValue;

        // Green Value
        int subtractedGreenValue = (int) bottomLayer.GetPixelList()[i].green - (int) topLayer.GetPixelList()[i].green;
        if(subtractedGreenValue < 0){
            subtractedGreenValue = 0;
        }
        pixel.green = (unsigned char) subtractedGreenValue;

        // Blue Value
        int subtractedBlueValue = (int) bottomLayer.GetPixelList()[i].blue - (int) topLayer.GetPixelList()[i].blue;
        if(subtractedBlueValue < 0){
            subtractedBlueValue = 0;
        }
        pixel.blue = (unsigned char) subtractedBlueValue;

        newPixelList.push_back(pixel);
    }
    Image image(topLayer.GetHeader(), newPixelList);
    return image;
}

Image screen(Image &topLayer, Image &bottomLayer){

    unsigned int length = topLayer.GetHeader().width * topLayer.GetHeader().height;
    vector<Pixel> newPixelList;

    Pixel pixel;
    for(int i = 0; i < length; i++){
        // Red value
        float topRedVal = (float) topLayer.GetPixelList()[i].red / 255.0f;
        float bottomRedVal = (float) bottomLayer.GetPixelList()[i].red / 255.0f;
        float redVal = (1 - ((1 - topRedVal) * (1 - bottomRedVal))) * 255 + 0.5f;
        int roundedNormRedVal = (int) redVal;
        pixel.red = (unsigned char) roundedNormRedVal;

        // Green value
        float topGreenVal = (float) topLayer.GetPixelList()[i].green / 255.0f;
        float bottomGreenVal = (float) bottomLayer.GetPixelList()[i].green/ 255.0f;
        float greenVal = (1 - ((1 - topGreenVal) * (1 - bottomGreenVal))) * 255 + 0.5f;
        int roundedNormGreenVal = (int) greenVal;
        pixel.green = (unsigned char) roundedNormGreenVal;

        // Blue value
        float topBlueVal = (float) topLayer.GetPixelList()[i].blue / 255.0f;
        float bottomBlueVal = (float) bottomLayer.GetPixelList()[i].blue / 255.0f;
        float blueVal = (1 - ((1 - topBlueVal) * (1 - bottomBlueVal))) * 255 + 0.5f;
        int roundedNormBlueVal = (int) blueVal;
        pixel.blue = (unsigned char) roundedNormBlueVal;

        newPixelList.push_back(pixel);
    }
    Image image(topLayer.GetHeader(), newPixelList);
    return image;
}

Image greenTint(Image &image){
    vector<Pixel> pixelList;
    Pixel pixel;

    for(int i = 0; i < image.GetPixelList().size(); i++){
        // Red value
        pixel.red = image.GetPixelList()[i].red;

        // Green value
        int greenVal = (int)image.GetPixelList()[i].green + 200;
        if(greenVal > 255){
            greenVal = 255;
        }
        pixel.green = (unsigned char) greenVal;

        // Blue value
        pixel.blue = image.GetPixelList()[i].blue;
        pixelList.push_back(pixel);
    }
    Image modifiedImage(image.GetHeader(), pixelList);
    return modifiedImage;
}

Image moreRedLessBlue(Image &image){
    vector<Pixel> pixelList;
    Pixel pixel;

    for(int i = 0; i < image.GetPixelList().size(); i++){
        // Red value
        int redVal = (int)image.GetPixelList()[i].red * 4;
        if(redVal > 255){
            redVal = 255;
        }
        pixel.red = (unsigned char) redVal;

        // Green value
        pixel.green = image.GetPixelList()[i].green;

        // Blue value
        int blueVal = (int)image.GetPixelList()[i].blue * 0;
        pixel.blue = (unsigned char) blueVal;

        pixelList.push_back(pixel);
    }
    Image modifiedImage(image.GetHeader(), pixelList);
    return modifiedImage;
}

Image overlay(Image &topLayer, Image &bottomLayer){
    unsigned int length = topLayer.GetHeader().width * topLayer.GetHeader().height;
    vector<Pixel> newPixelList;

    Pixel pixel;
    for(int i = 0; i < length; i++){

        // Red value
        float bottomRedVal = (float) bottomLayer.GetPixelList()[i].red / 255.0f;
        if(bottomRedVal <= 0.5f){
            float topRedVal = (float) topLayer.GetPixelList()[i].red / 255.0f;
            float redVal = (2 * topRedVal * bottomRedVal) * 255 + 0.5f;
            int roundedNormRedVal = (int) redVal;
            pixel.red = (unsigned char) roundedNormRedVal;
        } else {
            float topRedVal = 1 - ((float) topLayer.GetPixelList()[i].red / 255.0f);
            bottomRedVal = 1 - ((float) bottomLayer.GetPixelList()[i].red / 255.0f);
            float redVal = (1 - 2 * (topRedVal * bottomRedVal)) * 255 + 0.5f;
            int roundedNormRedVal = (int) redVal;
            pixel.red = (unsigned char) roundedNormRedVal;
        }


        // Green value
        float bottomGreenVal = (float) bottomLayer.GetPixelList()[i].green / 255.0f;
        if(bottomGreenVal <= 0.5f){
            float topGreenVal = (float) topLayer.GetPixelList()[i].green / 255.0f;
            float greenVal = (2 * topGreenVal * bottomGreenVal) * 255 + 0.5f;
            int roundedNormGreenVal = (int) greenVal;
            pixel.green = (unsigned char) roundedNormGreenVal;
        } else {
            float topGreenVal = 1 - ((float) topLayer.GetPixelList()[i].green/ 255.0f);
            bottomGreenVal = 1 - ((float) bottomLayer.GetPixelList()[i].green / 255.0f);
            float greenVal = (1 - 2 * (topGreenVal * bottomGreenVal)) * 255 + 0.5f;
            int roundedNormGreenVal = (int) greenVal;
            pixel.green = (unsigned char) roundedNormGreenVal;
        }

        // Blue value
        float bottomBlueVal = (float) bottomLayer.GetPixelList()[i].blue / 255.0f;
        if(bottomBlueVal <= 0.5f){
            float topBlueVal = (float) topLayer.GetPixelList()[i].blue / 255.0f;
            float blueVal = (2 * topBlueVal * bottomBlueVal) * 255 + 0.5f;
            int roundedNormBlueVal = (int) blueVal;
            pixel.blue = (unsigned char) roundedNormBlueVal;
        } else {
            float topBlueVal = 1 - ((float) topLayer.GetPixelList()[i].blue / 255.0f);
            bottomBlueVal = 1 - ((float) bottomLayer.GetPixelList()[i].blue / 255.0f);
            float blueVal = (1 - 2 * (topBlueVal * bottomBlueVal)) * 255 + 0.5f;
            int roundedNormBlueVal = (int) blueVal;
            pixel.blue = (unsigned char) roundedNormBlueVal;
        }

        newPixelList.push_back(pixel);
    }
    Image image(topLayer.GetHeader(), newPixelList);
    return image;
}

Image singleColor (Image &image, const string &color){
    vector<Pixel> pixelList;
    Pixel pixel;

    if(color == "red"){
        for(int i = 0; i < image.GetPixelList().size(); i++){
            // Red value
            pixel.red = image.GetPixelList()[i].red;

            // Green value
            pixel.green = image.GetPixelList()[i].red;

            // Blue value
            pixel.blue = image.GetPixelList()[i].red;

            pixelList.push_back(pixel);
        }
    } else if(color == "green"){
        for(int i = 0; i < image.GetPixelList().size(); i++){
            // Red value
            pixel.red = image.GetPixelList()[i].green;

            // Green value
            pixel.green = image.GetPixelList()[i].green;

            // Blue value
            pixel.blue = image.GetPixelList()[i].green;

            pixelList.push_back(pixel);
        }
    } else {
        for(int i = 0; i < image.GetPixelList().size(); i++){
            // Red value
            pixel.red = image.GetPixelList()[i].blue;

            // Green value
            pixel.green = image.GetPixelList()[i].blue;

            // Blue value
            pixel.blue = image.GetPixelList()[i].blue;

            pixelList.push_back(pixel);
        }
    }
    Image modifiedImage(image.GetHeader(), pixelList);
    return modifiedImage;
}

Image allThreeColors(Image &redImage, Image &greenImage, Image &blueImage){
    vector<Pixel> pixelList;
    Pixel pixel;

    for(int i = 0; i < redImage.GetPixelList().size(); i++){
        // Red value
        pixel.red = redImage.GetPixelList()[i].red;

        // Green value
        pixel.green = greenImage.GetPixelList()[i].green;

        // Blue value
        pixel.blue = blueImage.GetPixelList()[i].blue;

        pixelList.push_back(pixel);
    }
    Image modifiedImage(redImage.GetHeader(), pixelList);
    return modifiedImage;
}

Image flipImage(Image &image){
    vector<Pixel> pixelList;
    Pixel pixel;
    int length = (int) image.GetPixelList().size();
    for(int i = length - 1; i >= 0; i--){
        // Red value
        pixel.red = image.GetPixelList()[i].red;

        // Green value
        pixel.green = image.GetPixelList()[i].green;

        // Blue value
        pixel.blue = image.GetPixelList()[i].blue;
        pixelList.push_back(pixel);
    }
    Image modifiedImage(image.GetHeader(), pixelList);
    return modifiedImage;
}

void outputImage(Image &image, const string &fileName){
    ofstream onFile(fileName, ios_base::binary);

    char idLength = image.GetHeader().idLength;
    onFile.write(reinterpret_cast<char*>(&idLength), sizeof(idLength));

    char colorMapType = image.GetHeader().colorMapType;
    onFile.write(reinterpret_cast<char*>(&colorMapType), sizeof(colorMapType));

    char dataTypeCode = image.GetHeader().dataTypeCode;
    onFile.write(reinterpret_cast<char*>(&dataTypeCode), sizeof(dataTypeCode));

    short colorMapOrigin = image.GetHeader().colorMapOrigin;
    onFile.write(reinterpret_cast<char*>(&colorMapOrigin), sizeof(colorMapOrigin));

    short colorMapLength = image.GetHeader().colorMapLength;
    onFile.write(reinterpret_cast<char*>(&colorMapLength), sizeof(colorMapLength));

    char colorMapDepth = image.GetHeader().colorMapDepth;
    onFile.write(reinterpret_cast<char*>(&colorMapDepth), sizeof(colorMapDepth));

    short xOrigin = image.GetHeader().xOrigin;
    onFile.write(reinterpret_cast<char*>(&xOrigin), sizeof(xOrigin));

    short yOrigin = image.GetHeader().yOrigin;
    onFile.write(reinterpret_cast<char*>(&yOrigin), sizeof(yOrigin));

    short width = image.GetHeader().width;
    onFile.write(reinterpret_cast<char*>(&width), sizeof(width));

    short height = image.GetHeader().height;
    onFile.write(reinterpret_cast<char*>(&height), sizeof(height));

    char bitsPerPixel = image.GetHeader().bitsPerPixel;
    onFile.write(reinterpret_cast<char*>(&bitsPerPixel), sizeof(bitsPerPixel));

    char imageDescriptor = image.GetHeader().imageDescriptor;
    onFile.write(reinterpret_cast<char*>(&imageDescriptor), sizeof(imageDescriptor));

    for(int i = 0; i < image.GetPixelList().size(); i++){
        unsigned char blueVal = image.GetPixelList()[i].blue;
        onFile.write(reinterpret_cast<char*>(&blueVal), sizeof(blueVal));

        unsigned char greenVal = image.GetPixelList()[i].green;
        onFile.write(reinterpret_cast<char*>(&greenVal), sizeof(greenVal));

        unsigned char redVal = image.GetPixelList()[i].red;
        onFile.write(reinterpret_cast<char*>(&redVal), sizeof(redVal));
    }
    onFile.close();
}

int main() {

    string clionPath = "";

    // Task 1
    cout << "Task #1...... ";
    Image topLayer = loadImage(clionPath + "input/layer1.tga");
    Image bottomLayer = loadImage(clionPath + "input/pattern1.tga");
    Image exampleImage = loadImage(clionPath + "examples/EXAMPLE_part1.tga");
    Image modifiedImage = multiply(topLayer, bottomLayer);

    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part1.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 2
    cout << "Task #2...... ";
    topLayer = loadImage(clionPath + "input/layer2.tga");
    bottomLayer = loadImage(clionPath + "input/car.tga");
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part2.tga");
    modifiedImage = subtract(topLayer, bottomLayer);
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part2.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 3
    cout << "Task #3...... ";
    Image topLayerMultiplied = loadImage(clionPath + "input/layer1.tga");
    Image bottomLayerMultiplied = loadImage(clionPath + "input/pattern2.tga");
    Image multipliedImage = multiply(topLayerMultiplied, bottomLayerMultiplied);
    topLayer = loadImage(clionPath + "input/text.tga");
    modifiedImage = screen(topLayer, multipliedImage);
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part3.tga");
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part3.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 4
    cout << "Task #4...... ";
    topLayerMultiplied = loadImage(clionPath + "input/layer2.tga");
    bottomLayerMultiplied = loadImage(clionPath + "input/circles.tga");
    multipliedImage = multiply(topLayerMultiplied, bottomLayerMultiplied);
    topLayer = loadImage(clionPath + "input/pattern2.tga");
    modifiedImage = subtract(topLayer, multipliedImage);
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part4.tga");
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part4.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 5
    cout << "Task #5...... ";
    topLayer = loadImage(clionPath + "input/layer1.tga");
    bottomLayer = loadImage(clionPath + "input/pattern1.tga");
    modifiedImage = overlay(topLayer, bottomLayer);
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part5.tga");
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part5.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 6
    cout << "Task #6...... ";
    Image carImage = loadImage(clionPath + "input/car.tga");
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part6.tga");
    modifiedImage = greenTint(carImage);
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part6.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 7
    cout << "Task #7...... ";
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part7.tga");
    modifiedImage = moreRedLessBlue(carImage);
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part7.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 8
    cout << "Task #8...... ";
    carImage = loadImage(clionPath + "input/car.tga");
    Image carImageRed = singleColor(carImage, "red");
    Image carImageGreen = singleColor(carImage, "green");
    Image carImageBlue = singleColor(carImage, "blue");
    Image exampleCarImageRed = loadImage(clionPath + "examples/EXAMPLE_part8_r.tga");
    Image exampleCarImageGreen = loadImage(clionPath + "examples/EXAMPLE_part8_g.tga");
    Image exampleCarImageBlue = loadImage(clionPath + "examples/EXAMPLE_part8_b.tga");
    if((comparePixelList(carImageRed, exampleCarImageRed) && compareHeader(carImageRed, exampleCarImageRed)) &&
    (comparePixelList(carImageGreen, exampleCarImageGreen) && compareHeader(carImageGreen, exampleCarImageGreen)) &&
    (comparePixelList(carImageBlue, exampleCarImageBlue) && compareHeader(carImageBlue, exampleCarImageBlue))){
        cout << "Passed!" << endl;
        outputImage(carImageRed, clionPath + "output/part8_r.tga");
        outputImage(carImageGreen, clionPath + "output/part8_g.tga");
        outputImage(carImageBlue, clionPath + "output/part8_b.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 9
    cout << "Task #9...... ";
    Image redLayer = loadImage(clionPath + "input/layer_red.tga");
    Image greenLayer = loadImage(clionPath + "input/layer_green.tga");
    Image blueLayer = loadImage(clionPath + "input/layer_blue.tga");
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part9.tga");
    modifiedImage = allThreeColors(redLayer, greenLayer, blueLayer);
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part9.tga");
    } else {
        cout << "Failed!" << endl;
    }

    // Task 10
    cout << "Task #10...... ";
    Image textImage = loadImage(clionPath + "input/text2.tga");
    exampleImage = loadImage(clionPath + "examples/EXAMPLE_part10.tga");
    modifiedImage = flipImage(textImage);
    if(comparePixelList(modifiedImage, exampleImage) && compareHeader(modifiedImage, exampleImage)){
        cout << "Passed!" << endl;
        outputImage(modifiedImage, clionPath + "output/part10.tga");
    } else {
        cout << "Failed!" << endl;
    }

}
