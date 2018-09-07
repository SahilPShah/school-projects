/**
 * @file StickerSheet.h
 * Contains your declaration of the interface for the StickerSheet class.
 */
#ifndef STICKERSHEET_H_
#define STICKERSHEET_H_

#include "Image.h"
class StickerSheet{
public:
    //initialize the StickerSheet
    //picture - base picture of the sticker SticerSheet
    //max - maximum number of stickers allowed
    StickerSheet(const Image &picture, unsigned max);
    //frees any dynamically allocated memory
    ~StickerSheet();
    //copy constructor
    StickerSheet(const StickerSheet &other);
    //overloaded operator =
    const StickerSheet & operator= (const StickerSheet &other);
    //max - new max number of stickers
    void changeMaxStickers(unsigned max);
    //sticker - image to be added
    //x,y - coordinates of the image to be added
    int addSticker(Image &sticker, unsigned x, unsigned y);
    //change the coordinates of the sticker in a given layer
    //index - zero based layer index of the stickers
    //x,y - new coordinates
    bool translate(unsigned index, unsigned x, unsigned y);
    //remove the sticker at a given index
    void removeSticker(unsigned index);
    //get the sticker at the given index
    Image *getSticker(unsigned index);
    //return the stickersheet with stickers on it
    Image render() const;
private:
    //pointer to the array of stickers - initialize in constructor
    Image **stickers_;
    //base image
    Image *base_;
    //max number of stickers
    unsigned max_;
    // array to hold the x and y coordinates of each image
    unsigned *x_;
    unsigned *y_;

    unsigned getX(unsigned index) const;
    unsigned getY(unsigned index) const;
};
#endif
