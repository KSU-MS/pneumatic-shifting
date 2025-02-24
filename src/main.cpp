#include <Arduino.h>
#include "main.hpp"
uint16_t Paddle_Value;
uint16_t Launch_Value;

// This should also be a child function of the class ~Chance
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

// BTW this statement defining the class is something you would put in the main.hpp file ~Chance
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
        else{ // You shouldn't have to state that it will stay in the same state but it's not a terrible habit ig ~Chance
            current_state = rest;
        }
        break;
    case launch:
        digitalWrite(CLUTCH_PIN, HIGH);
        while (Launch_Value == AcceptedLaunchValue){
            serial_print("Waiting for button to be let go of");
            Launch_Value = analogRead(LAUNCH_BUTTON_SIGNAL);
        }
        Paddle_Value = rest; // Are you sure you meant to set the Paddle Value here??? ~Chance
        break;
    case up_shift:
        digitalWrite(NO_LIFT_SHIFT, HIGH);
        digitalWrite(UP_PIN, HIGH);
        //ADD WAIT FUNCTION DOWN TO UP PIN HEIGHT TIME (WILL WORRY ABOUT LATER)
        while (Paddle_Value == AcceptedPaddleValueUp){
            serial_print("Waiting for up paddle to be let go of");
            Paddle_Value == analogRead(PADDLE_SIGNAL);
        }
        Paddle_Value = rest; // Think about how this evaluates line by line, what points the state machine back to the rest state? ~Chance
        break;
    case down_shift:
        digitalWrite(CLUTCH_PIN, HIGH);
        digitalWrite(DOWN_PIN, HIGH);
        //ADD WAIT FUNCTION DOWN TO DOWN PIN HEIGHT TIME (WILL WORRY ABOUT LATER)
        while (Paddle_Value == AcceptedPaddleValueDown){
            serial_print("Waiting for down paddle to be let go of");
            Paddle_Value == analogRead(PADDLE_SIGNAL);
        }
        Paddle_Value = rest; // Same thing here
        break;
    default: 
        Paddle_Value = rest; // If this was the right variable, this would be a good catch, tho it should never happen ~Chance
        break;
    }
};

void setup(){
    pinMode(UP_PIN, OUTPUT);
    pinMode(NO_LIFT_SHIFT, OUTPUT);
    pinMode(LAUNCH_CONTROL, OUTPUT);
    pinMode(DOWN_PIN, OUTPUT);
    pinMode(CLUTCH_PIN, OUTPUT);
}   

void loop(){
    state_machine(); // This is not how you define and refrence classes ~Chance
    PinReset(); //end of loop
};

// So how do you want to go about this next? I can show you how I would model the system, think about
// structure, and actually write all the code in front of you in one sitting or I can continue to try 
// and give you hints about how state machines are used and why. Up to whatever you think would be 
// better for you to learn. Its my first time trying to teach to this extent so feedback would also
// be cool if there is something you think I am doing wrong. ~Chance
