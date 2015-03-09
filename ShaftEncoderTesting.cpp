#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <math.h>
#include <FEHServo.h>

#define PI 3.14159265

ButtonBoard buttons(FEHIO::Bank3);
FEHEncoder right_encoder(FEHIO::P0_0);
FEHEncoder left_encoder(FEHIO::P0_4);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);
AnalogInputPin cds(FEHIO::P1_3);
DigitalInputPin switch1(FEHIO::P1_5);
DigitalInputPin switch2(FEHIO::P1_0);
FEHServo servo(FEHServo::Servo0);


void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    //right_encoder.ResetCounts();
    left_encoder.ResetCounts();
    right_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() +right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

int main(void)
{
    //ENCODER TESTING

    /*while(true)
    {
        LCD.WriteLine(right_encoder.Value());
        LCD.WriteLine(left_encoder.Value());
        Sleep(250);
        LCD.Clear();
    }*/
    right_encoder.SetThresholds(2.6,3);
    left_encoder.SetThresholds(2.6,3);
    move_forward(50,10);
    Sleep(500);
    LCD.Write("LE   ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("RE   ");
    LCD.WriteLine(right_encoder.Counts());
    move_forward(-50,20);
    Sleep(500);
    LCD.Write("LE   ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("RE   ");
    LCD.WriteLine(right_encoder.Counts());
    move_forward(50,30);
    LCD.Write("LE   ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("RE   ");
    LCD.WriteLine(right_encoder.Counts());
}

