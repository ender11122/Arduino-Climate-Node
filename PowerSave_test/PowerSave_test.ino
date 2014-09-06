#include <Ports.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

//ADCSRA |= B10000000 // enable ADC (12 ticks to enable)
//ADCSRA &= ~(1<<ADEN); //Disable ADC
//ACSR = (1<<ACD); //Disable the analog comparator
//DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins.

int led = 13;
volatile int watchdog_counter = 0;

ISR(WDT_vect){
  watchdog_counter++;
}
  
void setup(){
  Serial.begin(4800);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  Serial.println("reset");
}

void loop(){
  //power_all_disable();
  go_to_sleep();
  if (watchdog_counter >= 1){
    watchdog_counter = 0;
    Serial.println("im awake");
    digitalWrite(led, HIGH);
    delay(2000);
    digitalWrite(led,LOW);
  }
}

void go_to_sleep() {
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);             // select the watchdog timer mode
  MCUSR &= ~(1 << WDRF);                           // reset status flag
  WDTCSR |= (1 << WDCE) | (1 << WDE);              // enable configuration changes
  WDTCSR = (1<< WDP0) | (1 << WDP1) | (1 << WDP2); // set the prescalar = 7
  WDTCSR |= (1 << WDIE);                           // enable interrupt mode
  sleep_enable();                                  // enable the sleep mode ready for use
  sleep_mode();                                    // trigger the sleep
  /* ...time passes ... */
}
