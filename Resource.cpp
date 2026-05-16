#include "Resource.h"
//Constructor
Resource::Resource(int g, int s, int so) {
    gold = g;
    silver = s;
    sodium = so;
}
//Getters

int Resource::getGold() {
    return gold;
}
int Resource::getSilver() {
    return silver;
}
int Resource::getSodium() {
    return sodium;
}
//Add functions

void Resource::addGold(int value) {
    gold += value;
}
void Resource::addSilver(int value) {
    silver += value;
}
void Resource::addSodium(int value) {
    sodium += value;
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
void Resource::consumeSodium(int value) {
  if (sodium >= value) {
    sodium -= value;
  }
}
// update function 
void Resource::update() {
}
