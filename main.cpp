// Computer Networks and Control Systems Assignmemt //
// Game Controller //

#include <mbed.h>
#include <EthernetInterface.h>
#include <rtos.h>
#include <mbed_events.h>
#include <FXOS8700Q.h>
#include <C12832.h>
#include <math.h>

// display 
C12832 lcd(D11, D13, D12, D7, D10);

// LEDS on on/off 
enum { Red, Green, Blue};
DigitalOut LED[] = {
  DigitalOut(PTB22,1),
  DigitalOut(PTE26,1),
  DigitalOut(PTB21,1)
};

void LEDon(int n) {
  LED[n].write(0);
}
void LEDoff(int n) {
  LED[n].write(1);
}

// speaker
PwmOut speaker(D6);

Thread dispatch;
EventQueue periodic;

// Accelerometer 
I2C i2c(PTE25, PTE24);
FXOS8700QAccelerometer acc(i2c, FXOS8700CQ_SLAVE_ADDR1);

// Potentiometer input
AnalogIn  left(A0);

//is button pressed
enum { Btn1, Btn2, sw_down, sw_left, sw_right};
struct pushbutton {
    DigitalIn sw;
    bool invert;
} buttons[] = {
  {DigitalIn(SW2),true},
  {DigitalIn(SW3),true},
  {DigitalIn(A3),false},
  {DigitalIn(A4),false},
  {DigitalIn(A5),false}
};

// interrupt 
InterruptIn sw_up(A2);

bool ispressed(int b) {
  return (buttons[b].sw.read())^buttons[b].invert;
}


bool automatic = false;
// switch modes
void swapMode(void){
  automatic = !automatic;
}

//Input states 
float rollRate;
float throttle = 0.0f;

//Task for polling sensors 
void user_input(void){
    if (automatic){
    motion_data_units_t a;
    acc.getAxis(a);
     if (asin(a.x) < 0.1f && asin(a.x) > -0.1f){
      rollRate = 0.0f;
    }
    // acquring reading if the Accelerometer is acceptable 
    else{
    float magnitude = sqrt( a.x*a.x + a.x*a.x + a.x*a.x );
    a.x = a.x/magnitude;
    rollRate = (asin(a.x) * -1.0f);
    }
    // failsafe if no input, if not a number set roll rate 0
    if (isnan(rollRate)){
      rollRate = 0.0f;
    }
    // Potentiometer reading
    if (left >= 0.0f){
      throttle = (left * 100.0f);
    }
    }
    // 	Inputs for manual mode
    else if(!automatic){
      rollRate = 0.0f;
      if(ispressed(3)){
        rollRate -= 1.0f;
      }
      else if(ispressed(4)){
        rollRate += 1.0f;
      }
      else if(ispressed(0)){
        throttle++;
      }
      else if(ispressed(1)){
        throttle--;
      }
    }
    // Stops throttle
    if (ispressed(2)){
      throttle = 0.0f;
    }
    sw_up.rise(swapMode);
}


float altitude = 0.0, fuel= 100.00;
int orientation, Vx, Vy;
bool isFlying, isCrashed;

// Addresses of  lander 
SocketAddress lander("192.168.0.16",65200);
SocketAddress dash("192.168.0.16",65250);

EthernetInterface eth;
UDPSocket udp;

// communications with lander 
void communications(void){
    SocketAddress source;
    char buffer[512];

    // Formatting message 
    sprintf(buffer,"command:!\nthrottle:%f\nroll:%f\n", throttle, rollRate);

    // Send message 
    udp.sendto( lander, buffer, strlen(buffer));

    // Recieve message 
    nsapi_size_or_error_t  n =
     udp.recvfrom(&source, buffer, sizeof(buffer));
    buffer[n] = '\0';

    // Unpack message 
    char *nextline, *line;
    for(
        line = strtok_r(buffer, "\r\n", &nextline);
        line != NULL;
        line = (strtok_r(NULL, "\r\n", &nextline))
    ){
      char *key, *value;
      key = strtok(line, ":");
      value = strtok(NULL, ":");
      if(strcmp(key,"altitude")==0 ) {
        altitude = atof(value);
      }
      else if (strcmp(key,"fuel")==0){
        fuel = atof(value);
      }
      else if (strcmp(key, "flying") == 0){
        if (atoi(value) == 1){
          isFlying = true;
        }
        if (atoi(value) == 0){
          isFlying = false;
        }
      }
      else if (strcmp(key,"crashed")==0){
        if (atoi(value) == 1){
          isCrashed = true;
        }
        else if(atoi(value) == 0){
          isCrashed = false;
        }
      }
      else if(strcmp(key,"orientation")==0){
          orientation = atoi(value);
      }
      else if(strcmp(key,"Vx")==0){
          Vx = atoi(value);
      }
      else if(strcmp(key,"Vy")==0){
        Vy = atoi(value);
      }
    }
}


void dashboard(void){
    //formatting and sending message to dashboard 
    char buffer[512];
    sprintf(buffer, "altitude:%f\nfuel:%f\nflying:%d\noVx:%d\noVy:%d\nautomatic:%d", altitude, fuel, isFlying,Vx,Vy,automatic);
    udp.sendto( dash, buffer, strlen(buffer));
}

int main() {
    acc.enable();
    throttle = 0.00f;
    isFlying = false;
    isCrashed = false;

    // ethernet connection  
    printf("Conecting \n");
    eth.connect();
    // obtained IP 
    const char *ip = eth.get_ip_address();
    printf("IP address is: %s\n", ip ? ip : "No IP");

    // open udp communications 
    udp.open( &eth);

    printf("lander is on %s/%d\n",lander.get_ip_address(),lander.get_port() );
    printf("dash   is on %s/%d\n",dash.get_ip_address(),dash.get_port() );

    dispatch.start( callback(&periodic, &EventQueue::dispatch_forever) );

    // periodic tasks
    periodic.call_every(50, user_input);
    periodic.call_every(50, communications);
    periodic.call_every(50, dashboard);

    // start dispatching thread 
    dispatch.start( callback(&periodic, &EventQueue::dispatch_forever) );

    while(1) {
        // Speakers and LEDs on/off
        speaker.write(0);
        lcd.locate(0,0);
        LEDoff(0); 
        LEDoff(1); 
        LEDoff(1); 
        // Lander flying
        if(isFlying){
          lcd.printf("Velocity X: %i\nVelocity Y: %i", Vx, Vy);
          lcd.locate(0,20);
          lcd.printf("orientation:%d", orientation);
          // Change to manual mode
          if (automatic == false){
            LEDoff(0);
            LEDoff(2);
            wait(0.5);
            LEDon(2);
            wait(0.5);
          }
          else if (automatic == true){
          LEDoff(0);
          LEDoff(2);
          wait(0.5);
          LEDon(1);
          wait(0.5);
          LEDoff(1);
          }
        }
        // Lander crashed 
        if(isCrashed){
          lcd.cls();
          lcd.locate(0,20);
          lcd.printf("Oh no you've crashed!\n");
          LEDoff(1);
          LEDoff(2);
          wait(0.5);
          LEDon(0);
          wait(0.5);
          for (int f=20.0; f<100; f+=10) {
            speaker.period(1.0/f);
            speaker.write(0.5);
            wait(0.1);
          }
          // Lander landed 
          if (!isCrashed && !isFlying){
            lcd.cls();
            lcd.locate(0,20);
            lcd.printf("You've landed!\n");
            LEDoff(2);
            LEDoff(0);
            LEDoff(1);
            wait(0.5);
            LEDon(1);
            wait(0.5);
          }
          lcd.cls();
        }
        wait(0.5);
    }
}

//End of program
