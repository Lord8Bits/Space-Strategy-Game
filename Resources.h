#ifndef RESOURCE.h
#define RESOURCE.h
#include "Updatable.hpp"

// Resource class manages different types of resources (gold, silver, sodium)
class Resource : public Updatable {
private:
    //ATTRIBUTES
    int gold;    
    int silver;  
    int sodium;  

public:
    //CONSTRUCTOR
    // Initializes the resource values
    Resource(int g, int s, int so);

    //GETTERS 
    int getGold();    
    // returns the current amount of gold
    int getSilver();  
    // returns the current amount of silver
    int getSodium();  
    // returns the current amount of sodium

    //ADD FUNCTIONS
    void addGold(int value);     
    // adds "value" to gold
    void addSilver(int value);   
    // adds "value" to silver
    void addSodium(int value);   
    // adds "value" to sodium

    //CONSUME FUNCTIONS 
    void consumeGold(int value);    
    //decreases the gold value
    

    void consumeSilver(int value);  
    // same logic as gold

    void consumeSodium(int value);  
    // same logic as gold
     void update() override;
};

#endif
