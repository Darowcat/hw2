// R = 9.54k
// C = 47 * 10 ^ -9
// cut-off freq = 1 / 2 * PI * R * C = 355

#include "mbed.h"
#include "uLCD_4DGL.h"

using namespace std::chrono;

InterruptIn up_button(A2);
InterruptIn down_button(A0);
InterruptIn confirm_button(A1);
AnalogOut Aout(PA_4);
AnalogIn Ain(A5);
uLCD_4DGL uLCD(D1, D0, D2);

int freq = 355;
double T = 1000.0f / freq;
double i = 0.0f;
double ADCdata[100];
int j;
bool conf = false;
int idx = 0;

void info(){
    uLCD.locate(1,2);
    uLCD.printf("%d\n", freq);
    uLCD.printf("%.2lf\n", T);
}

int main()
{   
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.color(WHITE);
    info();
   
    while (1)
    {
        if (up_button == 1){
            freq += 10;
            info();
        }

        if (down_button == 1){
            freq -= 10;
            info();
        }

        if (confirm_button == 1){
            T = 1000.0f / freq;
            
            conf = true;
            info();
        }
        
        if (i >= T){
            i = 0;
            //idx = 0;
        }
        if (i <= 0.9f * T){
            Aout = i / (0.99f * T);
        } else {
            Aout = (T - i) / (0.11f * T);
        }

        wait_us(int(T * 20.0f - 40.0f));
        
        i += (T / 50.0f);

        if (conf){
            ADCdata[idx] = Ain;
            if (idx == 99){
                printf("%d\r\n", freq);
                for (j = 0; j < 100; j++){
                    printf("%lf\r\n", ADCdata[j]);
                }
                idx = 0;
                conf = 0;
            } else {
                //printf("%d\r\n", idx);
                idx++;
            }
        }
    }
    
}