#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHRPS.h>
#define LINE_ON_RIGHT 0
#define ON_LINE_FIRST 1
#define LINE_ON_LEFT 2
#define ON_LINE_SECOND 3

#define LC 0 //state where left and center optosensors see the line
#define L 1 //only left optosensor sees the line
#define C 2 //only center optosensor sees the line
#define RC 3 //right and center optosensors see line
#define R 5 //only right optosensor sees line
#define N 6 //none of the optosensors see the line

//Declarations for encoders & motors
ButtonBoard buttons(FEHIO::Bank3);
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor0);
FEHMotor left_motor(FEHMotor::Motor1);
AnalogInputPin Center_Optosensor(FEHIO::P0_5);
AnalogInputPin Left_Optosensor(FEHIO::P0_3);
AnalogInputPin Right_Optosensor(FEHIO::P0_7);

int main(void)
{
    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    int prestate = C; //keeps track of previous state the robot was in
    int state = C; // Set the initial state

    //3 optosensor

    while (true) { // I will follow this line forever!
        switch(state) {
            // If the line is on my right...
            case C:

                //robot is centered so drive straight
                right_motor.SetPercent(50);
                left_motor.SetPercent(50);
                //if left optosensor finds the line
                if (Left_Optosensor.Value()>2)
                {
                    //if center optosensor also loses the line, set state to L
                    if(Center_Optosensor.Value()<2)
                    {
                        prestate=C;
                        state=L;
                    }
                    //otherwise the state is LC
                    else
                    {
                        prestate=C;
                        state=LC;
                    }
                }
                //if right optosensor finds the line
                if (Right_Optosensor.Value()>2)
                {
                    //if center optosensor also loses the line, set state to R
                    if(Center_Optosensor.Value()<2)
                    {
                        prestate=C;
                        state=R;
                    }
                    //otherwise the state is RC
                    else
                    {
                        prestate=C;
                        state=RC;
                    }
                }
                break;

            case L:
                //only left sees line so you want to turn left to get back
                right_motor.SetPercent(50);
                left_motor.SetPercent(0);
                //if center optosensor finds line
                //Note: no need to check for right optosensor, center will always
                //find line first and state will change
                if(Center_Optosensor.Value()>2)
                {
                    state=LC;
                    prestate=L;
                }
                //if left somehow loses line
                if (Left_Optosensor.Value()<2)
                {
                    //set state to N
                    state=N;
                    //store prestate as L so you know how to get back to the line
                    prestate=L;
                }
                break;

            case LC:
                //if the previous state was L
                //you are zigging in from the right and should turn
                //right to center yourself (though not as sharply)
                if (prestate=L)
                {
                    right_motor.SetPercent(25);
                    left_motor.SetPercent(50);
                }
                //otherwise (if prestate was C), then you would want
                //to turn left to get back on track (though not as sharply)
                else
                {
                    right_motor.SetPercent(50);
                    left_motor.SetPercent(25);
                }
                //if left optosensor loses line
                if(Left_Optosensor.Value()<2)
                {
                    //if right optosensor has also found line, state should be RC
                    if(Right_Optosensor.Value()>2)
                    {
                        state=RC;
                        prestate=LC;
                    }
                    //otherwise, state is C
                    else
                    {
                        state=C;
                        prestate=LC;
                    }
                }
                //if center optosensor loses line, state becomes L
                if(Center_Optosensor.Value()<2)
                {
                    state=L;
                    prestate=LC;
                }
                break;
            case R:
                //pretty much mirrors previous two cases
                //only right sees line so you want to turn right to get back
                right_motor.SetPercent(0);
                left_motor.SetPercent(50);
                //if center optosensor finds line
                //Note: no need to check for left optosensor, center will always
                //find line first and state will change
                if(Center_Optosensor.Value()>2)
                {
                    state=RC;
                    prestate=R;
                }
                //if left somehow loses line
                if (Right_Optosensor.Value()<2)
                {
                    //set state to N
                    state=N;
                    //store prestate as L so you know how to get back to the line
                    prestate=L;
                }
                break;

            case RC:
                //if the previous state was R
                //you are zigging in from the left and should turn
                //left to center yourself (though not as sharply)
                if (prestate=R)
                {
                    right_motor.SetPercent(50);
                    left_motor.SetPercent(25);
                }
                //otherwise (if prestate was C), then you would want
                //to turn left to get back on track (though not as sharply)
                else
                {
                    right_motor.SetPercent(25);
                    left_motor.SetPercent(50);
                }
                //if right optosensor loses line
                if(Right_Optosensor.Value()<2)
                {
                    //if left optosensor has also found line, state should be LC
                    if(Left_Optosensor.Value()>2)
                    {
                        state=LC;
                        prestate=RC;
                    }
                    //otherwise, state is C
                    else
                    {
                        state=C;
                        prestate=RC;
                    }
                }
                //if center optosensor loses line, state becomes R
                if(Center_Optosensor.Value()<2)
                {
                    state=R;
                    prestate=RC;
                }
                break;
            case N:
                //you have somehow lost the line. Que Lastima!
                //if the previous state was L,
                //the line is to your left, and you should turn right
                if (prestate==L)
                {
                    right_motor.SetPercent(50);
                    left_motor.SetPercent(0);
                }
                //otherwise, you should turn left
                if (prestate==R)
                {
                    right_motor.SetPercent(0);
                    left_motor.SetPercent(50);
                }
                if(Right_Optosensor.Value()>2)
                {
                    //if right finds line first
                    state=R;
                    prestate=N;
                }
                else if (Left_Optosensor.Value()>2)
                {
                    state=L;
                    prestate=N;
                }
                break;
                default: // Error. Something is very wrong.
                break;
            }
        LCD.Write("right: ");
        LCD.WriteLine(Right_Optosensor.Value());
        LCD.Write("center: ");
        LCD.WriteLine(Center_Optosensor.Value());
        LCD.Write("left: ");
        LCD.WriteLine(Left_Optosensor.Value());
        LCD.WriteLine(state);
        LCD.WriteLine(prestate);
        Sleep(100);
        LCD.Clear();
    }

        //1 optosensor

    /*while (true) { // I will follow this line forever!
        switch(state) {
            // If the line is on my right...
            case LINE_ON_RIGHT:

            // Set motor powers for right turn
            right_motor.SetPercent(25);
            left_motor.SetPercent(75);
            if (Center_Optosensor.Value()>2.1&&Center_Optosensor.Value()<2.2) {
            state = ON_LINE_FIRST; // update a new state
            }
            break;
             // If I am on the line, but zigging to the right...
            case ON_LINE_FIRST:
            // Set motor powers for right turn
            if(Center_Optosensor.Value()>.9&&Center_Optosensor.Value()<1.1) {
            state = LINE_ON_LEFT; // update a new state
            }
            break;
             // If the line is on my left...
            case LINE_ON_LEFT:
            // Mirror operation of LINE_ON_RIGHT state
            right_motor.SetPercent(75);
            left_motor.SetPercent(25);
            if (Center_Optosensor.Value()>2.1&&Center_Optosensor.Value()<2.2) {
            state = ON_LINE_SECOND; // update a new state
            }
            break;

            // If I am on the line, but zagging to the left...
            case ON_LINE_SECOND:
            // Mirror operation of ON_LINE_FIRST state
            if(Center_Optosensor.Value()>.9&&Center_Optosensor.Value()<1.1) {
                state = LINE_ON_RIGHT; // update a new state
            }
            break;
            default: // Error. Something is very wrong.
            break;
        }
        // Sleep a bit
        LCD.WriteLine(Center_Optosensor.Value());*/
}
