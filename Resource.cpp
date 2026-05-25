#include "Resource.h"
//Constructor
Resource::Resource(int g, int s, int di) {
    gold = g;
    silver = s;
    diamond = di;
}
//Getters

int Resource::getGold() {
    return gold;
}
int Resource::getSilver() {
    return silver;
}
int Resource::getdiamond() {
    return diamond;
}
//Add functions

void Resource::addGold(int value) {
    gold += value;
}
void Resource::addSilver(int value) {
    silver += value;
}
void Resource::adddiamond(int value) {
    diamond += value;
}
// Consume functions

void Resource::consumeGold(int value) {
   if (gold >= value) {  // So that the value doesnt become negative
    gold -= value;
   }
}
void Resource::consumeSilver(int value) {
   if (silver >= value) {
    silver -= value;
   }  
}
void Resource::consumediamond(int value) {
  if (diamond >= value) {
    diamond -= value;
  }
}
// update function 
void Resource::update() {
}
