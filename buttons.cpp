#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHServo.h>
#include <nav.h>
#include <FEHMotor.h>
#include <FEHRPS.h>

void PressButtons()
{
    ButtonBoard buttons( FEHIO::Bank3 );

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    FEHServo servo2(FEHServo::Servo2);
    //now we are in front of the buttons

    int min=694,max=2350;

    servo2.SetMin(min);
    servo2.SetMax(max);

    RPS.InitializeMenu();
    Sleep(50);

            int red = RPS.RedButtonOrder();
            int blue = RPS.BlueButtonOrder();
            int white = RPS.WhiteButtonOrder();
            int red_angle = 60;
            int blue_angle = 35;
            int white_angle = 50;
            int a=1;


            while (RPS.BlueButtonPressed()!=1 || RPS.RedButtonPressed()!=1 || RPS.WhiteButtonPressed()!=1)
            {
                LCD.WriteLine(a);

                    if (red==a)
                    {
                        servo2.SetDegree(red_angle);
                        Sleep(250);

                        while (RPS.RedButtonPressed()!=1)
                        {
                        move_time(-40,-40,1.3);
                        Sleep(50);
                        }
                        a++;

                        }




                   else if (blue==a)
                    {
                       servo2.SetDegree(blue_angle);
                       Sleep(250);


                       while (RPS.BlueButtonPressed()!=1)
                       {
                       move_time(-40,-40,1.3);
                       Sleep(50);
                       }
                       a++;


                    }


                     else if(white==a)
                    {
                        servo2.SetDegree(white_angle);
                        Sleep(250);


                        while (RPS.WhiteButtonPressed()!=1)
                        {
                        move_time(-40,-40,1.3);
                        Sleep(50);
                        }
                        a++;
                    }

                    move_time(35,35,.5);


           }
}

int main(void)
{
  PressButtons();

return 0;  
}
