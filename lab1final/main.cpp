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

    while( true )
    {
        Sleep(500);
        if( buttons.MiddlePressed() )
        {
            break;
        }

    }
DigitalInputPin switch1(FEHIO::P0_0);
AnalogInputPin cds(FEHIO::P1_0);

int spd=100;
motor1.SetPercent(spd);
        Sleep(1000);

while(1)
{

if (!switch1.Value())
{
motor1.SetPercent(0);
 LCD.Clear( FEHLCD::Black );
Sleep(100);

}

else
{
    spd=200/3.3*cds.Value();
    if (spd>100 || spd==0)
    {
        spd=spd-100;
    }

    else if (spd<100)
    {
        spd=-spd;
    }

}
}

    return 0;
}

