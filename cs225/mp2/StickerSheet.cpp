#include "StickerSheet.h"
#include "Image.h"
#include <array>
#include <algorithm>
using namespace cs225;
StickerSheet::StickerSheet(const Image &picture, unsigned max){
    max_=max;
    this->stickers_ = new Image*[max];
    this->x_ = new unsigned[max_];
    this->y_ = new unsigned[max_];
    for(unsigned i=0;i<max_;i++){
        stickers_[i] = NULL;
        x_[i] =0;
        y_[i] = 0;
    }
    base_= new Image();
    *base_ = picture;
}

StickerSheet::~StickerSheet(){
    if(this->stickers_!=NULL)
      delete [] this->stickers_;
    if(this->base_!=NULL)
      delete base_;
    if(this->x_!=NULL)
      delete [] x_;
    if(this->y_!=NULL)
      delete [] y_;
    stickers_=NULL;
    base_=NULL;
    x_=NULL;
    y_=NULL;
}
StickerSheet::StickerSheet(const StickerSheet &other){
  base_=other.base_;
  max_=other.max_;
  stickers_ = other.stickers_;
  x_=other.x_;
  y_=other.y_;
  for(unsigned i = 0; i < max_; i++){
    x_[i] = other.x_[i];
    y_[i] = other.y_[i];
    if(other.stickers_[i]!=NULL)
      stickers_[i] = new Image(*(other.stickers_[i]));
    else
      stickers_[i] = NULL;
  }

}
const StickerSheet & StickerSheet::operator= (const StickerSheet &other){
    //if they are the same
    if(this == &other){
        return *this;
    }
     for(unsigned i= 0; i<max_;i++){
          stickers_[i]=NULL;
    }
    //begin copy
    max_=other.max_;
    base_=other.base_;
    stickers_ = new Image*[max_];
    x_=new unsigned[max_];
    y_=new unsigned[max_];
    for(unsigned i = 0; i < max_; i++){
      stickers_[i] = other.stickers_[i];
      x_[i] = other.x_[i];
      y_[i] = other.y_[i];
    }

    return *this;
}
void StickerSheet::changeMaxStickers(unsigned max){
    Image **stickerData = new Image*[max_];
    unsigned *oldX = new unsigned[max_];
    unsigned *oldY = new unsigned[max_];
    //copy stickers_ into stickerData;
    for(unsigned i = 0; i<max_;i++){
        stickerData[i] = stickers_[i];
        oldX[i] = x_[i];
        oldY[i] = y_[i];
    }
    //delete old stickers
    if(this->stickers_!=NULL)
      delete[] this->stickers_;
    //allocate a new array with the new size
    stickers_ = new Image*[max];
    //put old data back in
    if(max_<max){ //if the array is larger than the old array
        for(unsigned i = 0; i<max_;i++){
            stickers_[i] = stickerData[i];
            x_[i] = oldX[i];
            y_[i] = oldY[i];
        }
        for(unsigned i = max_; i<max;i++){//set empty images in array to NULL
            stickers_[i] = NULL;
        }
    }
    else{//case where new stickersheet is smaller
        for(unsigned i = 0; i <max;i++){
            stickers_[i] = stickerData[i];
            x_[i] = oldX[i];
            y_[i] = oldY[i];
        }
        for(unsigned i=max;i<max_;i++){
            delete stickerData[i];
        }
    }
    //change the old max to new max
    max_=max;
    delete [] stickerData;//delete reference to array
    delete [] oldX;
    delete [] oldY;
}
int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y){
    if(x>(base_->width()) || y>(base_->height()))
        return -1;
     for(unsigned i=0; i<max_;i++){
       if(stickers_[i] == NULL){
           stickers_[i]=new Image();
           *(stickers_[i]) = sticker;
           x_[i] = x;
           y_[i] = y;
           return i;
       }
   }
   return -1;
 }

 bool StickerSheet::translate(unsigned index, unsigned x, unsigned y){
     if(stickers_[index] == NULL || x > base_->width() || y > base_->height() || index > max_)
        return false;
    else{
        x_[index] = x;
        y_[index] = y;
        return true;
    }
 }
 void StickerSheet::removeSticker(unsigned index){
     if(index<max_ && stickers_[index] != NULL){
         delete stickers_[index];
         stickers_[index] = NULL;
         x_[index]=0;
         y_[index]=0;
     }
 }

 Image* StickerSheet::getSticker(unsigned index){
     if(stickers_[index] == NULL)
        return NULL;
    else
        return stickers_[index];
 }

 Image StickerSheet::render() const{
    //find the maximum x and y coordninate
    Image render = *base_;
    unsigned maxX = base_->width();
    unsigned maxY = base_->height();
    for(unsigned i=0;i<max_;i++){
        if(stickers_[i] == NULL)
            continue;
        if(getX(i) + stickers_[i]->width() > maxX)
            maxX=getX(i) + stickers_[i]->width();
        if(getY(i) + stickers_[i]->height() > maxY)
            maxY=getY(i) + stickers_[i]->height();
    }
    render.scale(maxX,maxY);
    for(unsigned i=0;i<max_;i++){
        if(stickers_[i] == NULL)
            continue;
        unsigned xbound = getX(i) + stickers_[i]->width();
        unsigned ybound = getY(i) + stickers_[i]->height();
        for(unsigned x = getX(i); x<xbound ; x++ ){
            for(unsigned y= getY(i);y<ybound;y++){
                HSLAPixel &pixl = (stickers_[i])->getPixel(x-getX(i),y-getY(i));
                HSLAPixel &base_pxl = render.getPixel(x,y);
                if(pixl.a == 0)
                    continue;
                else{
                    base_pxl.h=pixl.h;
                    base_pxl.s=pixl.s;
                    base_pxl.l=pixl.l;
                    base_pxl.a=pixl.a;
                }
            }
            }
        }
    return render;
}
unsigned StickerSheet::getX(unsigned index) const{
  if(index >= max_)
    return x_[max_];
  return x_[index];
}
unsigned StickerSheet::getY(unsigned index) const{
  if(index >= max_)
    return y_[max_];
  return y_[index];
}
