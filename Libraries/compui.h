// UI Menu library for testing
// HIGH SPEED COMPETITION VERSION
// Creates a UI menu with choices to select different functions
// NO DIAGNOSTICS
// Supports up to 10 programs
// USE choice=ui() to create a ui fast.
// Returns choice as an integer
// 0 is first program
// Vinh Peters

#ifndef compui_H
#define compui_H

#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHLCD.h"
#include "FEHIO.h"
#include "FEHBattery.h"

LCD.Clear(FEHLCD::Black);
LCD.SetFontColor(FEHLCD::White);
}


Main menu function
// Creates menu with instructions and options

void menu(void)
{

    LCD.Clear(FEHLCD::Black);
    LCD.WriteLine("Select an option using");
    LCD.WriteLine("the right and left buttons");
    LCD.WriteLine(" ");
    LCD.WriteLine("Press the middle button");
    LCD.WriteLine("to select an option.");
    LCD.WriteLine(" ");
    LCD.Write("Battery Voltage ");
    LCD.WriteLine(Battery.Voltage());
    LCD.Write(" ");
    LCD.WriteLine(" ");
}



// Option counter
int options(int k,int n)
{
 ButtonBoard buttons( FEHIO::Bank3 );
    // Wait for a button press
    while (!buttons.RightPressed() && !buttons.LeftPressed() && !buttons.MiddlePressed())
    {
    }
// Change counters accordingly
    if (buttons.RightPressed())
    {
     k++;

    }
    else if (buttons.LeftPressed())
    {

        k--;
    }
    // Prevents pressing too fast
    Sleep(.15);

if (k==n)
{
k=0;
}

else if (k==-1)
{
    k=n-1;

}
Sleep(.1);

    // Return k, final choice
    return k;

}



//Display options
// These can be modifed to include more descriptive titles,
//but save to a new header file. More options can also be added
// Could be made to update from a string array, but the FEH package
// does not support string operations apparently

void dispopt(int n)
{

    if (n==0)
    {
        LCD.Write("<Program 1>");
    }
    else if (n==1)
    {
        LCD.Write("<Program 2>");
    }
    else if (n==2)
    {
        LCD.Write("<Program 3>");
    }
    else if (n==3)
    {
        LCD.Write("<Program 4>");
    }
    else if (n==4)
    {
        LCD.Write("<Program 5>");
    }
    else if (n==5)
    {
        LCD.Write("<Program 6>");
    }
    else if (n==6)
    {
        LCD.Write("<Program 7>");
    }
    else if (n==7)
    {
        LCD.Write("<Program 8>");
    }
    else if (n==8)
    {
        LCD.Write("<Program 9>");
    }
    else if (n==9)
    {
        LCD.Write("<Program 10>");
    }
}


//UI Function
// Uses all above functions
// Use this function
// Remember that this returns the selected choice number
// 0 is diagnotiscs
// 1 is program 1
// Takes the number of options
// n is number of programs including diagnostics

int ui(int n) // c=ui(number of options);
{

    splash();
    int c=0,j,temp;



    //Master while loop
    // Returns here after selected program runs
    while (1)
    {
        j=0;
    // Menu while loop
    while (j==0)
    {

    menu();
    dispopt(c);

    temp = c;
    c = options(c,n);

    /* Determine if a choice was made */
    if (c == temp)
    {
        /* Set counter to exit main menu loop */
       j=1;
    }
    }

// Option executes
// Returns c to be used in another if-else to determine which
// program to run

LCD.WriteLine("");
LCD.WriteLine("Running...");
Sleep(50
LCD.Clear(FEHLCD::Black);
break;
    }
	if (c==0)
	{
	diagnostics();
	}
 return c;
}



#endif
