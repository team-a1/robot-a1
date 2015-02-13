#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

// declare motor
FEHMotor motor1(FEHMotor::Motor0);

int main(void)
{
    ButtonBoard buttons( FEHIO::Bank3 );

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );
    LCD.WriteLine("DC Motor Test");



    AnalogInputPin cds(FEHIO::P1_0);


    float x;

    while(1)
    {
        LCD.Clear( FEHLCD::Black );
        x=cds.Value();
        LCD.WriteLine(x);
        Sleep(500);

    }





/*
    while(1)
    {
        LCD.Clear( FEHLCD::Black );
    if (switch1.Value())
    {
       motor1.SetPercent(75);
       LCD.WriteLine("Motor Running at 75%");
       Sleep(1000);
    }
    else
    {
    motor1.SetPercent(0);
     LCD.Clear( FEHLCD::Black );
      LCD.WriteLine("Motor Running at 0%");
    Sleep(100);
    }
        }



    motor1.SetPercent(75);
    Sleep(1000);

    motor1.SetPercent(0);
    Sleep(500);

    motor1.SetPercent(-75);
    Sleep(1000);
     motor1.SetPercent(0);

  LCD.Clear( FEHLCD::Black );
  */

    }



