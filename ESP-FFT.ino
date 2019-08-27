#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "arduinoFFT.h"
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 


#define OLED_RESET     -1 //
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
 
#define SAMPLES 256          
#define SAMPLING_FREQUENCY 5000 
void reDrawDisplay(double * vReal, double peak);
arduinoFFT FFT = arduinoFFT();
 
unsigned int sampling_period_us;
unsigned long microseconds;
 
double vReal[SAMPLES];
double vImag[SAMPLES];
 
void setup() {
    Serial.begin(115200);
     display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // display.clearDispay();
    // display.display();
    sampling_period_us = round(1000000*(1.0/SAMPLING_FREQUENCY));
}
 
void loop() {
   while(1){

    for(int i=0; i<SAMPLES; i++)
    {
        microseconds = micros();   
     
        vReal[i] = analogRead(A0);
        vImag[i] = 0;
     
        while(micros() < (microseconds + sampling_period_us)){
        }
    }
 

    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    reDrawDisplay(vReal, peak);
 
    //Serial.println(peak);     //Print out what frequency is the most dominant.
 
    //for(int i=0; i<(SAMPLES/2); i++)
    // {
 
         
       // Serial.print((i * 1.0 * SAMPLING_FREQUENCY) / SAMPLES, 1);
       // Serial.print(" ");
       // Serial.println(vReal[i], 1);   
  //  }
 
    //delay(30);  
    }
    
    //while(1);     
}

void reDrawDisplay(double * vReal , double peak){
        display.clearDisplay();
        for(int i=0; i<(SAMPLES/2); i++){
          vReal[i] = (double) vReal[i]/2500.0 * 64;
          if(vReal[i]>63){
            vReal[i] = 63;
            }
           display.drawLine( (SAMPLES/2)-i, 0,(SAMPLES/2)-i ,vReal[i] , WHITE);
          }
        display.display();
    

}
