#ifndef ALLMOTOR_H
#define ALLMOTOR_H

#include <wiringPi.h>

#define OUT1 22
#define OUT2 23
#define OUT3 24
#define OUT4 25

#define OUT5 5
#define OUT6 6
#define OUT7 13
#define OUT8 19

#define OUT9 16
#define OUT10 20

void setSteps2(int x1, int x2, int x3, int x4)
{
  pinMode(OUT5, OUTPUT);
  digitalWrite(OUT5, y1);
  pinMode(OUT6, OUTPUT);
  digitalWrite(OUT6, y2);
  pinMode(OUT7, OUTPUT);
  digitalWrite(OUT7, y3);
  pinMode(OUT8, OUTPUT);
  digitalWrite(OUT8, y4);
}

void setSteps3(int x1, int x2, int x3, int x4)
{
  pinMode(OUT1, OUTPUT);
  digitalWrite(OUT1, x1);
  pinMode(OUT2, OUTPUT);
  digitalWrite(OUT2, x2);
  pinMode(OUT3, OUTPUT);
  digitalWrite(OUT3, x3);
  pinMode(OUT4, OUTPUT);
  digitalWrite(OUT4, x4);
}

void forward1(int time)
{
  for (int i = 0; i <= time; i++)
  {
    pinMode(OUT9, OUTPUT);
    pinMode(OUT10, OUTPUT);
    digitalWrite(OUT9, LOW);
    digitalWrite(OUT10, HIGH);
    delay(time * 17);
    digitalWrite(OUT9, LOW);
    digitalWrite(OUT10, LOW);
  }
}

void forward2(int steps)
{
  for (int i = 0; i <= steps; i++)
  {
    setSteps2(1, 1, 0, 0); // setSteps(w5,w6,w7,w8)
    delay(10);
    setSteps2(0, 1, 1, 0);
    delay(10);
    setSteps2(0, 0, 1, 1);
    delay(10);
    setSteps2(1, 0, 0, 1);
    delay(10);
  }
}

void forward3(int steps)
{
  for (int i = 0; i <= steps; i++)
  {
    setSteps3(1, 1, 0, 0);
    delay(10);
    setSteps3(0, 1, 1, 0);
    delay(10);
    setSteps3(0, 0, 1, 1);
    delay(10);
    setSteps3(1, 0, 0, 1);
    delay(10);
  }
}

#endif // ALLMOTOR_H
