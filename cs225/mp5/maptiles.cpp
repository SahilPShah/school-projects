/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */

#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

Point<3> convertToLAB(HSLAPixel pixel) {
    Point<3> result(pixel.h/360, pixel.s, pixel.l);
    return result;
}

/*
*This function takes the vector of tile images and puts them into a map. The average colors of each tileImage is also pushed into
*a vector of points. The points vector is then used to create a KDtree. For each section in the MosaicCanvas picture, the nearest node in the tree
*to the color in the corresponding region in the SourceImage is set that region in the mosaiccanvas.
*/
MosaicCanvas* mapTiles(SourceImage const& theSource,
                       vector<TileImage>& theTiles)
{
    MosaicCanvas* result = new MosaicCanvas(theSource.getRows(), theSource.getColumns());
    vector<Point<3>> points; //vector to create the KD tree
    map<Point<3>, TileImage*> tiles; // map average colors to tiles
    //convert all the tiles in theTiles to from HSLA to Point<3> and put them in a map
    for(vector<TileImage>::iterator it = theTiles.begin(); it != theTiles.end(); ++it){
        points.push_back(convertToLAB((*it).getAverageColor()));
        tiles[convertToLAB((*it).getAverageColor())] = &(*it);
    }
    KDTree<3> HSLTree(points);
    for(int i = 0; i < result->getRows(); i++){
        for(int j = 0; j < result->getColumns(); j++){
            Point<3> nearest = HSLTree.findNearestNeighbor(convertToLAB(theSource.getRegionColor(i,j)));
            result->setTile(i,j, tiles[nearest]);
        }
    }
    return result;
}

TileImage* get_match_at_idx(const KDTree<3>& tree,
                                  map<Point<3>, int> tile_avg_map,
                                  vector<TileImage>& theTiles,
                                  const SourceImage& theSource, int row,
                                  int col)
{
    // Create a tile which accurately represents the source region we'll be
    // using
    HSLAPixel avg = theSource.getRegionColor(row, col);
    Point<3> avgPoint = convertToLAB(avg);
    Point<3> nearestPoint = tree.findNearestNeighbor(avgPoint);

    // Check to ensure the point exists in the map
    map< Point<3>, int >::iterator it = tile_avg_map.find(nearestPoint);
    if (it == tile_avg_map.end())
        cerr << "Didn't find " << avgPoint << " / " << nearestPoint << endl;

    // Find the index
    int index = tile_avg_map[nearestPoint];
    return &theTiles[index];

}
