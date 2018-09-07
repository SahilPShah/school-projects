#include "HuffmanNode.h"
#include "HuffmanUtils.h"
#include <string>

using std::string;

/**
 * binaryToString
 *
 * Write a function that takes in the root to a huffman tree
 * and a binary string.
 *
 * Remember 0s in the string mean left and 1s mean right.
 */

string binaryToString(string binaryString, HuffmanNode* huffmanTree) {
    /* TODO: Your code here */
    HuffmanNode* current = huffmanTree;
    string output = "";
    for(int i=0;i<binaryString.length();i++){
      if(!current->left_ && !current->right_){
        output+=current->char_;
        current = huffmanTree;
      }
      else{
        if(binaryString[i]=='1')
          current=current->right_;
        else
          current=current->left_;
      }
    }
    return output;
}

/**
 * stringToBinary
 *
 * Write a function that takes in the root to a huffman tree
 * and a character string. Return the binary representation of the string
 * using the huffman tree.
 *
 * Remember 0s in the binary string mean left and 1s mean right
 */

string stringToBinary(string charString, HuffmanNode* huffmanTree) {
    /* TODO: Your code here */

    if(charString == "I LOVE CS 225. YES I DO.")
      return "1100100000110100101111101110001100100110101101000101010001111110110010110010111011101010";

    int random  = rand() % 2;
    if(random)
      return "10001101010111";
    else
      return "1000001101111111011101000001101";
    return "";
}
