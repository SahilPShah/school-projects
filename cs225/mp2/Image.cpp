#include "Image.h"
#include "cs225/HSLAPixel.h"


using namespace cs225;
Image::Image():PNG(){

}
Image::Image(unsigned w, unsigned h):PNG(w,h){

}

Image::Image(const Image & other):PNG(other){
}
void Image::lighten(){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();y++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.l>.9)
                pxl.l=1;
            else
                pxl.l=pxl.l+.1;
        }
    }
}
void Image::lighten(double amount){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();y++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.l>1-amount)
                pxl.l=1;
            else
                pxl.l=pxl.l+amount;
        }
    }
}
void Image::darken(){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();x++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.l<.1)
                pxl.l=0;
            else
                pxl.l=pxl.l-.1;
        }
    }
}
void Image::darken(double amount){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();y++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.l<amount)
                pxl.l=0;
            else
                pxl.l=pxl.l-amount;
        }
    }
}
void Image::saturate(){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();y++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.s>.9)
                pxl.s=1;
            else
                pxl.s=pxl.s+.1;
        }
    }
}
void Image::saturate(double amount){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();y++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.s>1-amount)
                pxl.s=1;
            else
                pxl.s=pxl.s+amount;
        }
    }
}
void Image::desaturate(){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();x++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.s<.1)
                pxl.s=0;
            else
                pxl.s=pxl.s-.1;
        }
    }
}
void Image::desaturate(double amount){
    for(unsigned x=0;x<width();x++){
        for(unsigned y=0;y<height();y++){
            HSLAPixel &pxl = getPixel(x,y);
            if(pxl.s<amount)
                pxl.s=0;
            else
                pxl.s=pxl.s-amount;
        }
    }
}

void Image::grayscale(){
    for (unsigned x = 0; x < width(); x++) {
      for (unsigned y = 0; y < height(); y++) {
        HSLAPixel &pxl = getPixel(x, y);
        pxl.s = 0;//set the satration of the pxl to zero
    }
    }
}

void Image::illinify(){
    for(unsigned x = 0;x<width(); x++){
      for(unsigned y = 0;y<height(); y++){
        HSLAPixel & pixel = getPixel(x,y);//get reference to the pixel
        double hue = pixel.h;
        if(hue>=11 && hue<=216){
          if((hue-11)<=(216-hue)){//orange
            pixel.h=11;
          }
          else{
            pixel.h=216;
          }
        }
        else{
          if((371-hue)<=(hue-216)){//orange
            pixel.h=11;
          }
          else{
            pixel.h=216;
          }
        }
      }
    }
}
void Image::rotateColor(double degrees){
    for(unsigned x = 0; x<width();x++){
        for(unsigned y=0;y<height();y++){
            HSLAPixel &pxl = getPixel(x,y);
            pxl.h+=degrees;
            while(pxl.h<0){
                pxl.h+=360;
            }
            while(pxl.h>=360){
                pxl.h-=360;
            }
        }
    }
}
void Image::scale(double factor){
    if(factor == 1)
        return;
    unsigned origW = width();
    unsigned origH = height();
    HSLAPixel *imageData = new HSLAPixel[origW*origH];
    unsigned newH = height()*factor;
    unsigned newW = width()*factor;
    for(unsigned x=0;x<origW;x++){
        for(unsigned y=-0;y<origH;y++){
            imageData[x + (origW*y)] = getPixel(x,y);
        }
    }
    resize(newW, newH);
    for (unsigned int x = 0; x < newW; x++) {
      for (unsigned int y = 0; y < newH; y++) {
          HSLAPixel & pxl = getPixel(x,y);
          unsigned scaledX = x/factor;
          unsigned scaledY = y/factor;
          HSLAPixel & temp = imageData[scaledX + scaledY*origW];
          pxl = temp;
    }
    }
    //delete old imageData
    delete [] imageData;
    imageData=NULL;
}
void Image::scale(unsigned w, unsigned h){
    //check to make sure new width and old width arent equal
    if(w == width() && h == height())
      return;
    double width = w;
    double height = h;
    double scaleW = width / this->width();
    double scaleH = height / this->height();
    Image scaledImage;
    scaledImage.resize(w,h);//resize the image
    //import former image data with scaling
    for (unsigned int x = 0; x < w; x++) {
      for (unsigned int y = 0; y < h; y++) {
          HSLAPixel & scaledPxl = scaledImage.getPixel(x,y);
          HSLAPixel & origPxl = this->getPixel(x/scaleW, y/scaleH);
          scaledPxl.h=origPxl.h;
          scaledPxl.s=origPxl.s;
          scaledPxl.l=origPxl.l;
          scaledPxl.a=origPxl.a;
      }
    }
    *this=scaledImage;
}
