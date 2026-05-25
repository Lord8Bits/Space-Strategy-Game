#ifndef RESOURCE_H
#define RESOURCE_H
#include "Updatable.hpp"

// Resource class manages different types of resources (gold, silver, sodium)
class Resource : public Updatable {
private:
    //ATTRIBUTES
    int gold;    
    int silver;  
    int diamond;  

public:
    //CONSTRUCTOR
    // Initializes the resource values
    Resource(int g, int s, int di);

    //GETTERS 
    int getGold();    
    // returns the current amount of gold
    int getSilver();  
    // returns the current amount of silver
    int getdiamond();  
    // returns the current amount of sodium

    //ADD FUNCTIONS
    void addGold(int value);     
    // adds "value" to gold
    void addSilver(int value);   
    // adds "value" to silver
    void adddiamond(int value);   
    // adds "value" to sodium

    //CONSUME FUNCTIONS 
    void consumeGold(int value);    
    //decreases the gold value
    

    void consumeSilver(int value);  
    // same logic as gold

    void consumediamond(int value);  
    // same logic as gold
     void update() override;
};

#endif
