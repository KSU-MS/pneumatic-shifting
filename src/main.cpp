#include <Arduino.h>
#include "main.hpp"
uint16_t Paddle_Value;
uint16_t Launch_Value;

void PinReset(){ 
    digitalWrite(UP_PIN, LOW);
    digitalWrite(NO_LIFT_SHIFT, LOW);
    digitalWrite(DOWN_PIN, LOW);
    digitalWrite(CLUTCH_PIN, LOW);
    digitalWrite(LAUNCH_CONTROL, LOW);
}

//TEMPORARY VARIABLES, WILL BE SET UP OR REMOVED ONCE SIGNAL STRENGTHS ARE FOUND
int AcceptedLaunchValue = 1;
int AcceptedPaddleValueUp = 1;
int AcceptedPaddleValueDown = 1;
int c = 0;

class state_machine{
    private:
    enum states{
        rest,
        up_shift,
        down_shift,
        launch
    };
    states current_state = rest;
    
    public:
    void state_machine::nextState();

};

void state_machine::nextState(){
    switch (this->current_state){
    case rest:
        Launch_Value = analogRead(LAUNCH_BUTTON_SIGNAL);
        Paddle_Value = analogRead(PADDLE_SIGNAL);
        if (Launch_Value = AcceptedLaunchValue){
            current_state = launch;
        }
        else if (Paddle_Value == AcceptedPaddleValueUp){
            current_state = up_shift;
        }
        else if (Paddle_Value == AcceptedPaddleValueDown){
            current_state = down_shift;
        }
        else{
            current_state = rest;
        }
        break;
    case launch:
        
        break;
    case up_shift:

        break;
    case down_shift:
        
        break;
    default: 
        Paddle_Value = rest;
        break;
    }
};


void setup(){

}

void loop(){

};