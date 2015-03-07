#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
FEHEncoder right_encoder(FEHIO::P0_0);
FEHEncoder left_encoder(FEHIO::P0_4);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);
AnalogInputPin cds(FEHIO::P1_3);
DigitalInputPin switch1(FEHIO::P1_5);
DigitalInputPin switch2(FEHIO::P1_0);
FEHServo servo(FEHServo::Servo0);

//if you want to move relative to an amount of time
void move_time(int Rpercent, int Lpercent, float sec)
{
    right_motor.SetPercent(Rpercent);
    left_motor.SetPercent(Lpercent);
    //uses time library
    float time = TimeNow();
    while(TimeNow()-time<sec);
    //stops after time has passed
    right_motor.Stop();
    left_motor.Stop();
}

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_right(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //To turn right, make left motor go forward while right goes backward

    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_left(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //To turn left, make right motor go forward while left goes backward

    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 2 || RPS.X() > x_coordinate + 2)
    {
        //If RPS coordinate is greater than the target coordinate,
        //you have gone too far and must pulse backward
        if(RPS.X() > x_coordinate)
        {
            right_motor.SetPercent(-30);
            left_motor.SetPercent(-30);
        }
        //in this case you have not gone far enough and must pulse forward
        else if(RPS.X() < x_coordinate)
        {
            right_motor.SetPercent(30);
            left_motor.SetPercent(30);
        }
    }
    right_motor.Stop();
    left_motor.Stop();
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 2 || RPS.Y() > y_coordinate + 2)
    {
        //You have not gone far enough, must pulse forward
        if(RPS.Y() > y_coordinate)
        {
            right_motor.SetPercent(30);
            left_motor.SetPercent(30);
        }
        //You have gone too far, must pulse backward
        else if(RPS.Y() < y_coordinate)
        {
            right_motor.SetPercent(-30);
            left_motor.SetPercent(-30);
        }
    }
    right_motor.Stop();
    left_motor.Stop();
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 2 || RPS.Y() > y_coordinate + 2)
    {
        //You have gone too far, must pulse backward
        if(RPS.Y() > y_coordinate)
        {
            right_motor.SetPercent(-30);
            left_motor.SetPercent(-30);
        }
        //You have not gone far enough, must pulse forward
        else if(RPS.Y() < y_coordinate)
        {
            right_motor.SetPercent(30);
            left_motor.SetPercent(30);
        }
    }
    right_motor.Stop();
    left_motor.Stop();
}

void check_heading(float heading) //using RPS
{
    while(RPS.Heading() < heading - 2 || RPS.Heading() > heading + 2)
    {
        //If RPS heading is greater than target heading
        if(RPS.Heading() > heading)
        {
            // Ex. at 357 and trying to get to 0, turn left
            //If the difference is greater than 180, then the robot
            //should be turning left to get to target heading
            if (RPS.Heading()-heading>180)
            {
                //turn left 1 count
                turn_left(40,1);
                //Slows down program to check for correct heading
                Sleep(250);
            }

            //otherwise you should turn right to get to target heading
            else
            {
                //turn right 1 count
                turn_right(40,1);
                Sleep(250);
            }
        }
        //If RPS heading is less than target heading
        else if(RPS.Heading() < heading)
        {
            // Ex. At 0 and trying to get to 357, turn right
            //if difference is greater than 180, turn right
            if (heading-RPS.Heading()>180)
            {
                //turn right 1 count
                turn_right(40,1);
                Sleep(250);
            }

            //otherwise, turn left
            else
            {
                //turn left 1 count
                turn_left(40,1);
                Sleep(250);
            }
        }
    }
}

//function when you want to turn relative to RPS location
//takes desired new location coordinates as parameters
//should turn robot so that it is facing this location
float turnRPS(float xc, float yc)
{
    //stores current location of robot
    float x=RPS.X();
    float y=RPS.Y();
    //stores change in x and y
    float dx=xc-x;
    float dy=yc-y;
    //calculates heading (Note: must be converted from radians to degrees)
    float heading=atan(dy/dx)*(180/PI);
    //accounts for 0 degrees being north instead of east
    if (dx>0)
    { //add 270 to rotate one quadrant CCW
        heading+=270;
    }
    else
    { //range of atan is only (-PI/2, PI/2), so if you want to be in
        //quadrants 2 or 3 you add 180 then 270, or just add 90
        heading+=90;
    }
    //stores actual rotational distance you want to travel
    float dist;
    //counts to travel that distance
    int counts;
    //Stores current RPS heading
    //(RPS heading fluctuates so it's easier to store it in a variable at the beginning)
    float Rhead=RPS.Heading();
    if (heading-Rhead>0)
    {
        //it would be faster to turn right
        if (heading-Rhead>180)
        {
            dist=(360-heading)+Rhead;
            //calculates counts based on the ratio of 11 counts per 90 degrees
            counts=dist*(11.0/90);
            turn_right(50,counts);
        }
        //it would be faster to turn left
        else
        {
            dist=heading-Rhead;
            counts=dist*(11.0/90);
            turn_left(50,counts);
        }
    }
    else
    {
        //it would be faster to turn left
        if(Rhead-heading>180)
        {
            dist=(360-Rhead)+heading;
            counts=dist*(11.0/90);
            turn_left(50,counts);
        }
        //it would be faster to turn right
        else
        {
            dist=Rhead-heading;
            counts=dist*(11.0/90);
            turn_right(50,counts);
        }
    }
    //fine-tuning
    check_heading(heading);
    return heading;
}

int main(void)
{
        LCD.Clear();

        RPS.InitializeMenu();
        //values from calibration
        servo.SetMax(2289);
        servo.SetMin(500);
        servo.SetDegree(90);

        //sets thresholds for encoders
        left_encoder.SetThresholds(.5,2.5);
        right_encoder.SetThresholds(.5,2.5);
        /*move_time(50,50,2);
        turn_left(50,11);
        check_heading(270);
        move_time(50,50,2);
        turn_left(50,11);
        check_heading(0);
        move_time(60,30,1);
        move_time(30,60,1);*/
        move_time(65,65,3);
        check_y_plus(51);

        //(13,63) = coordinates of area of course directly in front of buttons
        float button_heading=turnRPS(15.5,59);
        right_motor.SetPercent(50);
        left_motor.SetPercent(50);

        //use white button as bump switch
        float time = TimeNow();
        while(RPS.ButtonsPressed()<1&&TimeNow()-time<4);
        right_motor.Stop();
        left_motor.Stop();
        if(RPS.ButtonsPressed()<1)
        {
            while(RPS.ButtonsPressed()<1)
            {
                float y = RPS.Y();
                //too far left
                if(y<61.5)
                {
                    move_time(-30,-30,1);
                    turn_right(30,2);
                    move_time(40,40,1);
                    right_motor.SetPercent(50);
                    Sleep(500);
                    right_motor.Stop();
                }
                //too far right
                else
                {
                    move_time(-30,-30,1);
                    turn_left(30,2);
                    move_time(40,40,1);
                    left_motor.SetPercent(50);
                    Sleep(500);
                    left_motor.Stop();
                }
            }
        }
        //now we are in front of the buttons
        int red = RPS.RedButtonOrder();
        int blue = RPS.BlueButtonOrder();
        int white = RPS.WhiteButtonOrder();
        int red_angle = 30;
        int blue_angle = 150;
        int white_angle = 90;
        int a=1;
        float b=button_heading;
        while (a<=3)
        {
            //move backwards from wall
            Sleep(500);
            move_time(-40,-40,1);
            //adjust servo to desired angle
            if (red==a && RPS.RedButtonPressed()!=1)
            {
                servo.SetDegree(red_angle);
                check_heading(b);
                Sleep(500);
                move_time(40,40,1);
                if (RPS.RedButtonPressed()==1)
                {
                    a++;
                }
                else
                {
                    red_angle=red_angle+5;
                    if(red_angle==40)
                    {
                        red_angle=0;
                    }
                    if(RPS.Y()>61.5)
                    {
                        b+=3;
                    }
                    else
                    {
                        b-=3;
                    }
                    if(b>button_heading+20)
                    {
                        b=button_heading-20;
                    }
                    else if(b<button_heading-20)
                    {
                        b=button_heading+20;
                    }
                }
            }
            else if (blue==a && RPS.BlueButtonPressed()!=1)
            {
                servo.SetDegree(blue_angle);
                check_heading(b);
                Sleep(500);
                move_time(40,40,1);
                if(RPS.BlueButtonPressed()==1)
                {
                    a++;
                }
                else
                {
                    blue_angle=blue_angle-5;
                    if(blue_angle==140)
                    {
                        blue_angle=180;
                    }
                    if(RPS.Y()>61.5)
                    {
                        b+=3;
                    }
                    else
                    {
                        b-=3;
                    }
                    if(b>button_heading+20)
                    {
                        b=button_heading-20;
                    }
                    else if(b<button_heading-20)
                    {
                        b=button_heading+20;
                    }
                }
            }
            else if(white==a && RPS.BlueButtonPressed()!=1)
            {
                servo.SetDegree(white_angle);
                check_heading(b);
                Sleep(500);
                move_time(40,40,1);
                if (RPS.WhiteButtonPressed()==1)
                {
                    a++;
                }
                else
                {
                    white_angle=white_angle+5;
                    if(white_angle==65)
                    {
                        white_angle=25;
                    }
                    if(RPS.Y()>61.5)
                    {
                        b+=3;
                    }
                    else
                    {
                        b-=3;
                    }
                    if(b>button_heading+20)
                    {
                        b=button_heading-20;
                    }
                    else if(b<button_heading-20)
                    {
                        b=button_heading+20;
                    }
                }
            }
            if(RPS.ButtonsPressed()==a)
            {
                a++;
            }
            if(RPS.ButtonsPressed()<a-1)
            {
                a=1;
            }
        }

    return 0;
}
