#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"
#include <string>

using namespace cs225;


void rotate(std::string inputFile, std::string outputFile) {
  // Part 2
  PNG image;
  image.readFromFile(inputFile);//read in file
  PNG output = image;//temporary.
  for(int x=0;x<image.width();x++){//traverse input photo
      for(int y=0;y<image.height();y++){
          HSLAPixel *pixl = image.getPixel(x,y);
          HSLAPixel *pixl_out = output.getPixel(image.width()-1-x,image.height()-1-y);//rotate
          *pixl_out = *pixl;
      }
  }
  output.writeToFile(outputFile);//write output to file
}

PNG myArt(unsigned int width, unsigned int height) {
  PNG png(width, height);
  // Part 3
  //create an image with one color
 int init_h= 200;
 int hue = init_h;
 int hchanges = width/150;
 int count = hchanges;
 //COLOR GRADIENT
  for(int x=1;x<=width;x++){
     for(int y=1;y<=height;y++){
         HSLAPixel *pxl = png.getPixel(x-1,y-1);
         //****CHANGE THE HUE EVERY hchanges number of times****
         if(count==0){
             hue--;
             count = hchanges;
         }
         pxl->h = hue;
         pxl->s=1;
         pxl->l=.5;
         count--;
     }
     count=hchanges;
     hue = init_h;
 }
  return png;
}
