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
FEHServo servo2(FEHServo::Servo2);

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
        float time=TimeNow();
        while(TimeNow()-time<.1);
        right_motor.Stop();
        left_motor.Stop();
        Sleep(250);
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

void turn_left_time(int percent, float time)
{
    left_motor.SetPercent(-percent);
    right_motor.SetPercent(percent);
    float t = TimeNow();
    while(TimeNow()-t<time);
    right_motor.Stop();
    left_motor.Stop();
}

void turn_right_time(int percent, float time)
{
    left_motor.SetPercent(percent);
    right_motor.SetPercent(-percent);
    float t = TimeNow();
    while(TimeNow()-t<time);
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
                turn_left_time(40,.2);
                //Slows down program to check for correct heading
                Sleep(250);
            }

            //otherwise you should turn right to get to target heading
            else
            {
                //turn right 1 count
                turn_right_time(40,.2);
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
                turn_right_time(40,.2);
                Sleep(250);
            }

            //otherwise, turn left
            else
            {
                //turn left 1 count
                turn_left_time(40,.2);
                Sleep(250);
            }
        }
    }
}

void SetHeading(float heading)
{
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
            LCD.WriteLine(dist);
            counts=dist*(11.0/90);
            LCD.WriteLine(counts);
            turn_right(50,counts);
            LCD.Write("LE  ");
            LCD.WriteLine(left_encoder.Counts());
            LCD.Write("RE  ");
            LCD.WriteLine(right_encoder.Counts());
        }
    }
    //fine-tuning
    check_heading(heading);
}

//function when you want to turn relative to RPS location
//takes desired new location coordinates as parameters
//and returns the heading towards that location
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
    return heading;
}

int main(void)
{
    LCD.Clear();

    RPS.InitializeMenu();

    while(cds.Value()>.4);

    //values from calibration
    servo.SetMax(2289);
    servo.SetMin(500);
    servo2.SetMin(662);
    servo2.SetMax(1980);

    //sets thresholds for encoders
    left_encoder.SetThresholds(.3,2.3);
    right_encoder.SetThresholds(.3,1.1);

    //move to rock salt bag
    move_forward(50,17);
    SetHeading(turnRPS(26,8)-180); //(26,8) is coordinate to "aim for"
    SetHeading(turnRPS(24,13)-180);
    move_forward(-45,15);
    check_heading(45);
    Sleep(1.0);

    //pick up salt bag
    servo2.SetDegree(0);
    Sleep(1.0);

    move_forward(-40,5);
    move_forward(40,5);

    turn_right_time(40,.5);
    turn_left_time(40,.5);
    turn_right_time(40,.5);
    turn_left_time(40,.5);

    turn_right(50,5);
    move_forward(50,13);
    check_heading(0);
    servo2.Off();
    move_forward(60,15);
    servo2.SetMin(662);
    servo2.SetMax(1980);
    servo2.SetDegree(0);
    move_forward(60,20);
    check_heading(0);
    check_y_plus(55);

    turn_right(50,11);
    check_heading(270);
    move_forward(-50,30);
    turn_right(50,5);
    check_heading(225);
    servo2.SetDegree(90);
    move_forward(50,10);
    servo2.SetDegree(0);
    move_forward(-50,20);

    move_forward(50,2);
    turn_right(50,5);
    check_heading(180);
    servo.SetDegree(90);
    move_forward(50,25);
    turn_left(50,7);
    move_forward(50,15);
    turn_left(50,5);
    check_heading(270);
    move_forward(-50,15);





    return 0;
}
