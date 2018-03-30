#include <LED_signBoard.h>
#include <Timer.h>

#define buttonPin 7
#define greenPin 9
#define redPin 10
#define allAction 5
#define TimerSmalleastUnity 5
LED_signBoard LED_control;

Timer t1;
int timerCounter = 0;
int timerCounterGoal= 0;
bool timeArrive = 0;
int breatheCounter = 0;
byte  modeSelect;
byte horrorCounter = 0;
enum mode{
  timerStop = 0,
  timerBlink,
  timerBreathe,
  timerHorror
};

void setup() {
  Serial.begin(9600);
  modeSelect = timerStop;
  LED_control.LED_Mode_initial_set(greenPin, redPin, buttonPin);
   LED_control.LED_ON_all();
  t1.every(TimerSmalleastUnity,timerEvent);   
}

void loop() {
   t1.update();
   boolean BTN_check = digitalRead(buttonPin);     //=0 button push down,=1 button no push down
   if(!BTN_check) {       
        //deBounce
        delay(30);
        while (!digitalRead(buttonPin));
        if(LED_control.buttonCounter == allAction ) {
            LED_control.buttonCounter = 1;
        } else {
           LED_control.buttonCounter++;
        }
        delay(30);
       // Serial.print("Counter:" );
       // Serial.println(LED_control.buttonCounter);
        LED_mode_by_button();
   }  
}



void LED_mode_by_button() {
    uint8_t counter = LED_control.buttonCounter;
    switch(counter) {
      case 1:     
          blinkFunctionSet_all(500);   //ms
      break;     
     case 2:
          breatheFunctionSet_all(10);  //ms
      break; 
      case 3:
           horrorFunctionSet_all(400); //ms
      break;
      case 4:
          horrorCounter = 0;
          modeSelect = timerStop;
          LED_control.LED_ON_all();  
      break;      
      case 5:
          modeSelect = timerStop;
          LED_control.LED_OFF_all();  
      break;     
    };  
}

 void breatheFunctionSet_all(int timeSet) {      
  functionSet(timeSet);
  modeSelect = timerBreathe;
 }

 void blinkFunctionSet_all(int timeSet) { 
  functionSet(timeSet);
  modeSelect = timerBlink;
 }

 void horrorFunctionSet_all(int timeSet) {
  functionSet(timeSet);
  modeSelect = timerHorror;
 }

 void functionSet(int timeSet) {
   modeSelect = timerStop;
  timerCounter = 0;
  timerCounterGoal = timeSet;    //timeSet ms,smallest unity is TimerSmalleastUnity ms 
 }

 void doBlinkLED() {
       timerCounter = 0;
       LED_control.LED_Blink_all(timeArrive);    
       timeArrive = !timeArrive;                     
 }


void doHorrorLED() { 
      timerCounter++;      
          if(timerCounter*TimerSmalleastUnity >= timerCounterGoal && horrorCounter < 3) {
             horrorCounter++;
             doBlinkLED();          
              if(horrorCounter == 3){
                timerCounterGoal = 200;
              }
          } else if (timerCounter*TimerSmalleastUnity >= timerCounterGoal && horrorCounter < 6){
             horrorCounter++;
             doBlinkLED();   
               if(horrorCounter == 6){
                timerCounterGoal = 100;
              }
          } else if (timerCounter*TimerSmalleastUnity >= timerCounterGoal && horrorCounter < 12){
             horrorCounter++;
             doBlinkLED();   
               if(horrorCounter == 12){
                timerCounterGoal = 5;
              }
          } else if (timerCounter*TimerSmalleastUnity >= timerCounterGoal && horrorCounter == 12){     
               timerCounter = 0;
               breatheCounter++;        
                LED_control.LED_Breathe_all(breatheCounter);     
               if(breatheCounter == 256) {
                   timerCounter = 0;
                    breatheCounter = 0;
                    horrorCounter++;  
                    timerCounterGoal = 1000;
                    LED_control.LED_OFF_all();  
                 }                     
          } else if (timerCounter*TimerSmalleastUnity >= timerCounterGoal && horrorCounter == 13 ) {
            timerCounter = 0;
             horrorCounter = 0;  
             timerCounterGoal = 400;             
          } 
 }
 
 void timerEvent() {
    if (modeSelect == timerBlink) {
          timerCounter++;
          if (timerCounter*TimerSmalleastUnity >= timerCounterGoal){
             doBlinkLED();                                
          } 
     } else if(modeSelect == timerBreathe) {
          timerCounter++;
          if(timerCounter*TimerSmalleastUnity >= timerCounterGoal) {
                 timerCounter = 0;
                 breatheCounter++;
                 if(breatheCounter == 256) {
                    breatheCounter = 0;
                 }
              LED_control.LED_Breathe_all(breatheCounter);
          }      
     } else if(modeSelect == timerHorror) {
        doHorrorLED();
     }     
 }




