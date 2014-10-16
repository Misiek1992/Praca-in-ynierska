#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <signal.h>
#include <softPwm.h>
#include <math.h>

int i=0;
int isr=0;
int pomiar1=0;
int pomiar2=0;
int wynik=0;
double predkosc=0;
void updateEncoder() {
  isr++;
 // printf("Got Interrupted %i!\n", isr); //see how often we get interrupts.
}

int main()
{
	wiringPiSetup();
	pinMode(0,OUTPUT);
	pinMode(2,INPUT);
	pinMode(3,INPUT);
	softPwmCreate(0,0,100);
	wiringPiISR(2,INT_EDGE_BOTH,updateEncoder);
	wiringPiISR(3,INT_EDGE_BOTH,updateEncoder);
	softPwmWrite(0,100);
	delay(2000);
	isr=0;
	pomiar1=millis();
	softPwmWrite(0,100);
	delay(12000);
	pomiar2=millis();
	softPwmWrite(0,0);
	softPwmWrite(0,100);
	delay(3000);
	wynik=pomiar2-pomiar1;
	printf("Pomiar czasu: %i\n",wynik);
	delay(1000);
	predkosc=(isr*0.000119047)*(60000/wynik);
	printf("Predkosc: %f\n",predkosc);
	return;
}


