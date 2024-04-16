#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <iterator>
#include "doublelinklist.h"
#include "objects.h"

using namespace std;

struct Data //I store effective actions&objects in this structure
{
    public:
    int action;
    int object;
    int returns;
};

struct Stack //This structure allows me to store more than 1 pair of action and object.
{
    private:


    public:
    Data data[10000];
    int step = 0;
    void push(int, int, int);
    void print();
};

void Stack :: push(int next_action,int next_object, int next_return) //Whenever advance() returns positive integer I will push the inputs.
{
    this->data[step].action = next_action;
    this->data[step].object = next_object;
    this->data[step].returns = next_return;
    this->step++;
};


void Stack :: print() //This function prints the path to win at the end.
{
    int i = 0;
    while(i < step)
    {
        cout<<"Action: " << data[i].action << "   " << "Object: " << data[i].object <<endl;
        i++;
    }
}


int main()
{
    Stack mystack;
    GameState first_state;
    first_state.create_init_state();
    int actions = 1; //Action is initialized at 1 because I will cycle all actions for the first object and only then we will pass that object
    int objects = 0;//Object is initialized at 0 because then we will not miss any object because first objects' index is 0 in each room
    int effectID;


    while(true)//This loop will cycle until first.state.win becomes 1
    {

        if(mystack.step > 2 && mystack.data[mystack.step - 1].returns == mystack.data[mystack.step - 3].returns)//This conditional block is for prevent the loop between rooms. Because going between rooms produce positive return value, we can get stuck between rooms always going one from another... So if there is a "returnA, returnB, returnA" kind of values in our stack we skip an action after another returnB...
        {
            mystack.data[mystack.step - 1].returns = -1;
            if(mystack.data[mystack.step - 2].action >= 5)
            {
                objects = 0;
                actions = 1;
            }
            else
            {
                actions =  mystack.data[mystack.step - 2].action + 1;
                objects = mystack.data[mystack.step - 2].object;
            }
        }
        else if(first_state.room_id == 2 & actions == 1)//This condition only prevents us from exiting from the last room before doing anything purposeful...
        {
            actions += 1;
        }

        effectID = first_state.advance(actions, objects);//We advance in each iteration

        if(first_state.win)//We found winning path so we break the loop
        {
            mystack.push(actions, objects, effectID);
            cout << "The winning sequeance: " <<endl;
            break;
        }
        
        else if(effectID == -1)//We died (in-game) so we skip that action
        {
            if(actions >= 5)
            {
                objects++;
                actions = 1;
            }
            else
                actions++;
        }
        else if(effectID == 0)//If this action does not change anything, then we skip also
        {
            if(actions >= 5)
            {
                objects++;
                actions = 1;
            }
            else
                actions++;
        }
        else if(effectID != 0 && effectID != -1)//This action changes something but does not kill us so we push this input pair into our stack.
        {
            mystack.push(actions, objects, effectID);
            actions = 1;
            objects = 0;
        }
    }
        mystack.print();//We print the winning sequance at the end
        return 0;
}
