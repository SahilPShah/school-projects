#include "Image.h"
#include "StickerSheet.h"
using namespace cs225;
int main() {
    //still need to make actual sticker sheet
    Image chi,sox,bulls,bears,blackhawks,render;
    //read in images
    //image from google images: https://graphicsmesh.com/products/chicago-night-skyline-wall-mural
    chi.readFromFile("ChicagoSkyline_Mural_Square_1024x1024.png");
    chi.scale(5000,5000);
    //pic from http://www.stickpng.com/img/sports/baseball/major-league-baseball-mlb/chicago-white-sox/chicago-white-sox-logo
    sox.readFromFile("Sox.png");
    sox.scale(2250,2250);
    //pic from http://www.stickpng.com/img/sports/basketball/nba-teams/chicago-bulls-logo
    bulls.readFromFile("bulls.png");
    bulls.scale(1500,1500);
    //pic from http://www.stickpng.com/img/sports/nfl-football/chicago-bears/chicago-bears-logo
    bears.readFromFile("bears.png");
    bears.scale(1400,1400);
    //pic from https://en.wikipedia.org/wiki/Chicago_Blackhawks
    blackhawks.readFromFile("blackhawks.png");
    blackhawks.scale(1500,1500);
    StickerSheet mySheet(chi,4);//base image of alma with 3 max stickers;
    //use guess and check for the locations
    mySheet.addSticker(sox, 1200, 25);
    mySheet.addSticker(bears, 100,3350);
    mySheet.addSticker(bulls, 1750, 3250);
    mySheet.addSticker(blackhawks, 3500, 3250);
    render=mySheet.render();
    //final image effects
    render.saturate();
    render.lighten();
    render.writeToFile("myImage.png");

  return 0;
}
