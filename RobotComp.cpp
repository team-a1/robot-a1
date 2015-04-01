#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>
#include <math.h>
#include <time.h>
#include <diagnostics.h>
#include <uimenu.h>
#include <nav.h>

#define PI 3.14159265

#define LC 0 //state where left and center optosensors see the line
#define L 1 //only left optosensor sees the line
#define C 2 //only center optosensor sees the line
#define RC 3 //right and center optosensors see line
#define R 5 //only right optosensor sees line
#define N 6 //none of the optosensors see the line

void GrabSaltBag()
{
    move_forward(50,30);
    SetHeading(45);
    servo2.SetDegree(0);
    Sleep(1.0);
    servo2.SetDegree(60);
    move_time(-50,-50,4);
    servo2.SetDegree(0);
    move_forward(50,5);
}

void TurnCrank()
{
    SetHeading(315);
    move_forward(50,15);
    SetHeading(357);
    servo2.SetDegree(15);
    move_forward(80,55);
    servo2.SetDegree(0);
    check_heading(357);
    check_y_plus(52);

    servo2.SetMin(694);
    servo2.SetMax(2350);

    servo2.SetDegree(0);

    //check where you are and pulse forward repeatedly
    adjust_heading_x(30);
    Sleep(100);
    move_forward(50,1);

    adjust_heading_x(30);
    Sleep(100);
    move_forward(50,1);
    Sleep(100);

    adjust_heading_x(30);
    Sleep(100);
    move_forward(50,1);
    Sleep(100);

    //if we happen to find the crank before the final charge
    if(switch1.Value()==1)
    {
        check_heading(357);
        move_time(60,60,1);
    }

    //error trapping if you are off target
    while(switch1.Value()==1)
    {
        move_forward(-50,8);

        adjust_heading_x(30);
        Sleep(100);
        move_forward(50,1);
        Sleep(100);
        check_heading(357);
        move_time(50,50,1);
        Sleep(100);
    }
    int dir;
    if(cds.Value()<.5)
    {
        //CW - Red
        dir=1;
        LCD.WriteLine("RED");
        LCD.WriteLine(cds.Value());

        //move out
        move_forward(-60,4);
        Sleep(250);

        //adjust servo
        servo.SetDegree(36);
        Sleep(250);

        //mopve back in
        move_forward(75,5);
        Sleep(250);

        //turn crank
        servo.SetDegree(180);
        Sleep(250);

        //repeat
        move_forward(-75,4);
        Sleep(250);
        servo.SetDegree(36);
        Sleep(250);
        move_forward(75,5);
        Sleep(250);
        servo.SetDegree(180);
        Sleep(250);

        move_forward(-75,4);
        Sleep(250);
        servo.SetDegree(36);
        Sleep(250);
        move_forward(75,5);
        Sleep(250);
        servo.SetDegree(135);
        Sleep(250);
    }
    else
    {
        //CCW - Blue
        dir=2;
        LCD.WriteLine("BLUE");
        LCD.WriteLine(cds.Value());

        //first turn
        servo.SetDegree(0);
        Sleep(250);

        //back up
        move_forward(-75,4);
        Sleep(250);

        //readjust servo
        servo.SetDegree(144);
        Sleep(250);

        //move in
        move_forward(75,5);
        Sleep(250);

        //turn again
        servo.SetDegree(0);
        Sleep(250);

        //reapeat
        move_forward(-75,4);
        Sleep(250);
        servo.SetDegree(144);
        Sleep(250);
        move_forward(75,5);
        Sleep(250);
        servo.SetDegree(45);
        Sleep(250);
    }
}

void DropSaltBag()
{
    move_forward(-50,5);
    SetHeading(45);
    move_forward(50,30);
    SetHeading(315);
    move_forward(-50,25);
    SetHeading(225);
    move_forward(-50,5);
    servo2.SetDegree(90);
    move_forward(50,15);
    servo2.SetDegree(0);
    move_forward(-50,20);
}

void PushButtons()
{
    move_forward(50,10);
    SetHeading(270);
    move_forward(50,10);
    SetHeading(315);
    move_forward(50,10);
    SetHeading(225);

    int red = RPS.RedButtonOrder();
    int blue = RPS.BlueButtonOrder();
    int white = RPS.WhiteButtonOrder();
    int a=1;
    while (a<=3)
    {
        Sleep(500);
        //adjust servo to desired angle
        if (red==a)
        {
            servo2.SetDegree(70);
        }
        else if (blue==a)
        {
            servo2.SetDegree(47);
        }
        else if(white==a)
        {
            servo2.SetDegree(58);
        }
        move_time(-60,-60,1);
        move_time(50,50,1);
        if(RPS.ButtonsPressed()==a)
        {
            a++;
        }
        if(RPS.ButtonsPressed()<a-1)
        {
            a=1;
        }
    }
}

void PumpSwitch()
{
    SetHeading(315);
    servo2.SetDegree(0);
    move_forward(-60,40);
    servo2.SetDegree(120);
    SetHeading(0);
    move_forward(-50,40);
    turn_left(50,7);
    move_forward(-50,5);
    SetHeading(0);
    int oil=RPS.OilDirec();
    if(oil==1)
    {
        servo2.SetDegree(70);
        move_time(-60,-60,2);
    }
    else
    {
        move_time(-50,-50,3);
        servo.SetDegree(50);
        move_time(60,60,2);
    }
}


int main(void)
{

  int choice;
  choice=ui(2);

  if (choice==1)
  {
        LCD.Clear();

        RPS.InitializeMenu();
        LCD.WriteLine("Press Middle Button to Start");
        while(!buttons.MiddlePressed());
        LCD.Clear();
        servo.SetMax(2284);
        servo.SetMin(533);
        servo.SetDegree(180);
        servo2.SetMin(662);
        servo2.SetMax(1980);
        right_encoder.SetThresholds(.5,1.5);
        left_encoder.SetThresholds(.5,1.5);

        float time=TimeNow();
        while(cds.Value()>1&&TimeNow()-time<3);

        GrabSaltBag();
        TurnCrank();
        DropSaltBag();
        PushButtons();
        PumpSwitch();
   } // end of ui if

} // end of main
