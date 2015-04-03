#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#include <FEHServo.h>
#include <math.h>
#include <time.h>


#define PI 3.14159265

ButtonBoard buttons(FEHIO::Bank3);

FEHEncoder right_encoder(FEHIO::P1_6);
FEHEncoder left_encoder(FEHIO::P0_0);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);

AnalogInputPin cds(FEHIO::P2_5);

DigitalInputPin switch1(FEHIO::P1_3);

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
            right_motor.SetPercent(-40);
            left_motor.SetPercent(-40);
        }
        //You have not gone far enough, must pulse forward
        else if(RPS.Y() < y_coordinate)
        {
            right_motor.SetPercent(40);
            left_motor.SetPercent(40);
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
                turn_left_time(40,.1);
                //Slows down program to check for correct heading
                Sleep(100);
            }

            //otherwise you should turn right to get to target heading
            else
            {
                //turn right 1 count
                turn_right_time(40,.1);
                Sleep(100);
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
                turn_right_time(40,.1);
                Sleep(100);
            }

            //otherwise, turn left
            else
            {
                //turn left 1 count
                turn_left_time(40,.1);
                Sleep(100);
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
            counts=dist*(11.0/90);
            turn_right(50,counts);
        }
    }
    //fine-tuning
    check_heading(heading);
}

//adjusts heading relative to x coordinate
void adjust_heading_x(float xc)
{

     if(RPS.X()<xc)
     {
         turn_right_time(50,.2);
     }
     else
     {
         turn_left_time(50,.2);
     }
     Sleep(100);
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

void GrabSaltBag()
{
    LCD.WriteLine("Grab Salt Bag");
    move_forward(-50,20);
    SetHeading(45);

    servo2.SetDegree(0);

    Sleep(500);

    servo2.SetDegree(48);
    move_time(-60,-60,3);
    servo2.SetDegree(0);
    move_forward(50,10);
}

void Ramp()
{
    LCD.WriteLine("Ramp");
    SetHeading(315);
    move_forward(50,20);
    SetHeading(356);

    //Going up ramp
    servo2.SetDegree(14);
    Sleep(100);
    move_forward(80,45);

    while(RPS.Y()<35)
    {
        check_heading(356);
        move_forward(80,30);
    }

    // At the top
    servo2.SetDegree(0);
}

void TurnCrank2()
{
    int dir;
    LCD.WriteLine("Turn Crank");
    check_heading(355);
    check_y_plus(53);

        adjust_heading_x(30);
        Sleep(100);
        move_forward(50,1);
        Sleep(100);
        check_heading(357);

        adjust_heading_x(30);
        Sleep(100);
        move_forward(50,1);
        Sleep(100);
        check_heading(357);

        adjust_heading_x(30);
        Sleep(100);
        move_forward(50,1);
        Sleep(100);
        check_heading(357);


        //if we happen to find the crank before the final charge
        if(switch1.Value()==1)
        {
            check_heading(357);
            move_time(60,60,1);
        }

        float time=TimeNow();
        //error trapping if you are off target
        while(switch1.Value()==1&&TimeNow()-time<20)
        {
            move_forward(-50,8);

            adjust_heading_x(30);
            Sleep(100);
            move_forward(50,1);
            Sleep(100);
            check_heading(357);

            adjust_heading_x(30);
            Sleep(100);
            move_forward(50,1);
            Sleep(100);
            check_heading(357);

            move_time(50,50,1);
            Sleep(100);
            LCD.WriteLine(cds.Value());
            if(cds.Value()<.5)
            {
                dir=1;
            }
            else if(cds.Value()<1)
            {
                dir=2;
            }
        }
        move_time(50,50,.5);

        if(dir!=1&&dir!=2)
        {
            LCD.WriteLine(cds.Value());
            if(cds.Value()<.5)
            {
                dir=1;
            }
            else
            {
                dir=2;
            }
        }
    if(dir==1)
    {
        //CW - Red
        LCD.WriteLine("RED");
        LCD.WriteLine(cds.Value());

        //move out
        move_forward(-60,4);
        Sleep(500);

        //adjust servo
        servo.SetDegree(36);
        Sleep(400);

        //move back in
        move_forward(75,5);
        Sleep(500);

        //turn crank
        servo.SetDegree(180);
        Sleep(500);

        //repeat
        move_forward(-60,3);
        Sleep(500);
        servo.SetDegree(36);
        Sleep(500);
        move_forward(75,3);
        Sleep(500);
        servo.SetDegree(180);
        Sleep(500);

        move_forward(-60,3);
        Sleep(500);
        servo.SetDegree(36);
        Sleep(500);
        move_forward(75,3);
        Sleep(500);
        servo.SetDegree(180);
        Sleep(500);
    }
    else
    {
        //CCW - Blue
        LCD.WriteLine("BLUE");
        LCD.WriteLine(cds.Value());

        //first turn
        servo.SetDegree(0);
        Sleep(400);

        //back up
        move_forward(-75,4);
        Sleep(400);

        //readjust servo
        servo.SetDegree(144);
        Sleep(400);

        //move in
        move_forward(75,5);
        Sleep(400);

        //turn again
        servo.SetDegree(0);
        Sleep(400);

        //reapeat

        move_forward(-60,3);
        Sleep(500);
        servo.SetDegree(144);
        Sleep(500);
        move_forward(75,3);
        Sleep(500);
        servo.SetDegree(0);
        Sleep(500);
    }
}

void FindCrank()
{
    LCD.WriteLine("Turn Crank");
    check_heading(355);
    check_y_plus(50);

    //check where you are and pulse forward repeatedly
    while((RPS.X()<29.5||RPS.X()>30.5)||(RPS.Heading()<355||RPS.Heading()>359))
    {
        if(switch1.Value()!=1)
        {
            break;
        }

        else if (RPS.X()<29.5)
        {
            left_motor.SetPercent(50);
            Sleep(500);
            left_motor.Stop();
        }
        else if(RPS.X()>30.5)
        {
            right_motor.SetPercent(50);
            Sleep(500);
            right_motor.Stop();
        }
        check_heading(357);
    }// end of while

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

}// end of function

void TurnCrank()
{
    int dir;
    if(cds.Value()<.5)
    {
        //CW - Red
        dir=1;
        LCD.WriteLine("RED");
        LCD.WriteLine(cds.Value());

        //move out
        move_forward(-60,4);
        Sleep(400);

        //adjust servo
        servo.SetDegree(36);
        Sleep(400);

        //mopve back in
        move_forward(75,5);
        Sleep(400);

        //turn crank
        servo.SetDegree(180);
        Sleep(400);

        //repeat
        move_forward(-75,4);
        Sleep(400);
        servo.SetDegree(36);
        Sleep(400);
        move_forward(75,5);
        Sleep(400);
        servo.SetDegree(180);
        Sleep(400);

        move_forward(-75,4);
        Sleep(400);
        servo.SetDegree(36);
        Sleep(400);
        move_forward(75,5);
        Sleep(400);
        servo.SetDegree(135);
        Sleep(400);
    }
    else
    {
        //CCW - Blue
        dir=2;
        LCD.WriteLine("BLUE");
        LCD.WriteLine(cds.Value());

        //first turn
        servo.SetDegree(0);
        Sleep(400);

        //back up
        move_forward(-75,4);
        Sleep(400);

        //readjust servo
        servo.SetDegree(144);
        Sleep(400);

        //move in
        move_forward(75,5);
        Sleep(400);

        //turn again
        servo.SetDegree(0);
        Sleep(400);

        //reapeat
        move_forward(-75,4);
        Sleep(400);
        servo.SetDegree(144);
        Sleep(400);
        move_forward(75,5);
        Sleep(400);
        servo.SetDegree(45);
        Sleep(400);
    }
}



void DropSaltBag()
{
    //Navigate around snow pile
    LCD.WriteLine("Drop Salt Bag");
    move_forward(-50,5);
    SetHeading(45);
    move_forward(50,28);
    SetHeading(305);
    move_forward(-50,25);
    SetHeading(225);

    //Drop bag
    move_forward(-50,5);
    servo2.SetDegree(90);

    //Push into garage
    move_forward(50,15);
    servo2.SetDegree(0);
    move_forward(-50,20);


}

void PushButtons()
{
    LCD.WriteLine("Push Buttons");
    LCD.WriteLine("");
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
            servo2.SetDegree(75);
        }
        else if (blue==a)
        {
            servo2.SetDegree(48);
        }
        else if(white==a)
        {
            servo2.SetDegree(62);
        }
        move_time(-60,-60,1);
        move_time(50,50,1);
        if(RPS.ButtonsPressed()==a)
        {
            a++;
        }
        else
        {
            a=1;
        }
        LCD.Write("a: ");
        LCD.WriteLine(a);
        LCD.Write("buttons pressed: ");
        LCD.WriteLine(RPS.ButtonsPressed());
    }
}

void FindButtons()
{
    // Go to buttons
    LCD.WriteLine("Find Buttons");
    move_forward(50,10);
    SetHeading(315);
    move_forward(50,20);
    SetHeading(225);
    move_forward(50,5);
}

void PressButtons()
{

    LCD.WriteLine("Press Buttons");
    int red = RPS.RedButtonOrder();
    int blue = RPS.BlueButtonOrder();
    int white = RPS.WhiteButtonOrder();
    int red_angle = 80;
    int blue_angle = 49;
    int white_angle = 65;
    int a=1;
    float time;

    while (RPS.BlueButtonPressed()!=1 || RPS.RedButtonPressed()!=1 || RPS.WhiteButtonPressed()!=1)
    {
    LCD.WriteLine(a);
        if (red==a)
        {
            servo2.SetDegree(red_angle);
            Sleep(250);
            time=TimeNow();
            while (RPS.RedButtonPressed()!=1&&TimeNow()-time<2)
            {
                move_time(-50,-50,.5);;
                Sleep(50);
            }
            if(RPS.RedButtonPressed()==1)
            {
                a++;
            }
            else if (RPS.BlueButtonPressed()==0&&RPS.RedButtonPressed()==0&&RPS.WhiteButtonPressed()==0)
            {
                a=1;
            }
        }
        else if (blue==a)
        {
            servo2.SetDegree(blue_angle);
            Sleep(250);
            time=TimeNow();
            while (RPS.BlueButtonPressed()!=1&&TimeNow()-time<2)
            {
                move_time(-50,-50,.5);
                Sleep(50);
            }
            if(RPS.BlueButtonPressed()==1)
            {
                a++;
            }
            else if (RPS.BlueButtonPressed()==0&&RPS.RedButtonPressed()==0&&RPS.WhiteButtonPressed()==0)
            {
                a=1;
            }
        }
        else if(white==a)
        {
            servo2.SetDegree(white_angle);
            Sleep(250);
            time=TimeNow();
            while (RPS.WhiteButtonPressed()!=1&&TimeNow()-time<2)
            {
                move_time(-50,-50,.5);
                Sleep(50);
            }
            if(RPS.WhiteButtonPressed()==1)
            {
                a++;
            }
            else if (RPS.BlueButtonPressed()==0&&RPS.RedButtonPressed()==0&&RPS.WhiteButtonPressed()==0)
            {
                a=1;
            }
        }
        move_time(50,50,.5);
    }
}

void PumpSwitch()
{
    LCD.WriteLine("Pump Switch");
    SetHeading(305);
    servo2.SetDegree(90);
    move_forward(-60,35);
    SetHeading(357);
    move_forward(-60,60);
    Sleep(1.0);
    check_heading(357);
    turn_left(80,8);
    SetHeading(90);
    move_forward(-50,10);
    SetHeading(353);
    int oil=RPS.OilDirec();
    if(oil==1)
    {
        servo2.SetDegree(70);
        move_time(-60,-60,2);
        while(RPS.OilPress()==0)
        {
            move_forward(50,10);
            move_time(-75,-75,2);
        }
    }
    else
    {
        move_time(-50,-50,3);
        servo2.SetDegree(50);
        move_time(60,60,2);
        while(RPS.OilPress()==0)
        {
            servo2.SetDegree(90);
            move_forward(50,10);
            move_time(-50,-50,2);
            servo2.SetDegree(50);
            move_time(75,75,2);
        }
    }
}


int main(void)
{

// Course run
     LCD.Clear();

        // Initialize RPS
        RPS.InitializeMenu();
        LCD.WriteLine("Press Middle Button to Start");

        while(!buttons.MiddlePressed())
        {

        }

        LCD.Clear();
        servo.SetMax(2284);
        servo.SetMin(533);
        servo.SetDegree(180);
        servo2.SetMin(562);
        servo2.SetMax(2243);
        right_encoder.SetThresholds(.5,1.5);
        left_encoder.SetThresholds(.5,1.5);

        float time=TimeNow();
        while(cds.Value()>1&&TimeNow()-time<30)
        {

        }


        GrabSaltBag();
        Ramp();
        TurnCrank2();
        DropSaltBag();
        FindButtons();
        PressButtons();
        PumpSwitch();

}