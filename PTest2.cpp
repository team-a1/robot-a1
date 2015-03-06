#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_4);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor2);
AnalogInputPin cds(FEHIO::P1_3);
DigitalInputPin switch1(FEHIO::P1_5);
DigitalInputPin switch2(FEHIO::P1_0);

void move_forward(int percent, int counts) //using encoders
{
    LCD.WriteLine("MOVE FUNCTION");
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts)
    {
        LCD.WriteLine(left_encoder.Counts());
        LCD.WriteLine(right_encoder.Counts());
    }

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
    LCD.WriteLine("TURN LEFT");
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //To turn left, make right motor go forward while left goes backward

    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    //While the average of the left and right encoder are less than counts,
    //keep running motors

    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts)
    {
        LCD.WriteLine(left_encoder.Counts());
        LCD.WriteLine(right_encoder.Counts());
    }

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void check_x_plus(float x_coordinate) //using RPS while robot is in the +x direction
{
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1)
    {
        //If RPS coordinate is greater than the target coordinate,
        //you have gone too far and must pulse backward
        if(RPS.X() > x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            //To pulse backward, set motor to negative percent for 1 count
            move_forward(-50,1);
            //To slow program down so it has time to check if it is in the
            //correct location
            Sleep(100);
        }
        //in this case you have not gone far enough and must pulse forward
        else if(RPS.X() < x_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            //pulse forward by setting motor to positive percent for one count
            move_forward(50,1);
            Sleep(100);
        }
    }
}

void check_y_minus(float y_coordinate) //using RPS while robot is in the -y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        //You have not gone far enough, must pulse forward
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            //Set motor to positive percent for 1 count
            move_forward(50,1);
            Sleep(100);
        }
        //You have gone too far, must pulse backward
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            //Set motor to negative percent for 1 count
            move_forward(-50,1);
            Sleep(100);
        }
    }
}

void check_y_plus(float y_coordinate) //using RPS while robot is in the +y direction
{
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        //You have gone too far, must pulse backward
        if(RPS.Y() > y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            //set motor to negative percent for 1 count
            move_forward(-50,1);
            Sleep(100);
        }
        //You have not gone far enough, must pulse forward
        else if(RPS.Y() < y_coordinate)
        {
            //pulse the motors for a short duration in the correct direction
            //set motor to positive percent for 1 count
            move_forward(50,1);
            Sleep(100);
        }
    }
}

void check_heading(float heading) //using RPS
{
    while(RPS.Heading() < heading - 1 || RPS.Heading() > heading + 1)
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
                turn_left(30,1);
                //Slows down program to check for correct heading
                Sleep(250);
            }

            //otherwise you should turn right to get to target heading
            else
            {
                //turn right 1 count
                turn_right(30,1);
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
                turn_right(30,1);
                Sleep(250);
            }

            //otherwise, turn left
            else
            {
                //turn left 1 count
                turn_left(30,1);
                Sleep(250);
            }
        }
    }
}

int main(void)
{
    RPS.InitializeMenu();

    while(cds.Value()>.4);
    move_forward(50,15);
    turn_left(50, 11);
    check_heading(270);
    move_forward(50,20);
    turn_left(50,11);
    check_heading(0);
    move_forward(75,100);

    return 0;
}
