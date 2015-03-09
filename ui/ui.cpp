/* Proteus UI
 *Creates as basic menu with options to test various functions
 */

#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>


//Set Button Board
ButtonBoard buttons (FEHIO::Bank3);

//Main menu function

void menu(void)
{

    LCD.Clear(FEHLCD::Black);
    LCD.WriteLine("Select an option using");
    LCD.WriteLine("the right and left buttons");
    LCD.WriteLine(" ");
    LCD.WriteLine("Press the middle button");
    LCD.WriteLine("to select an option.");
    LCD.WriteLine(" ");
    LCD.WriteLine(" ");
}



// Option counter
int options(int k,int n)
{
    // Wait for button press
    while (!buttons.RightPressed() && !buttons.LeftPressed() && !buttons.MiddlePressed())
    {
    }

    if (buttons.RightPressed())
    {
     k++;

    }
    else if (buttons.LeftPressed())
    {
        /* Decrement c */
        k--;
    }
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

    // Return k
    return k;

}

// Team A1 splash screen
// I have no life
void splash(void)
{
    LCD.Clear(FEHLCD::Black);
    LCD.SetFontColor(FEHLCD::White);

    LCD.WriteLine(" ");
    LCD.WriteLine(" ");

LCD.WriteLine(" ##### #####   #   #   #");
LCD.WriteLine("   #   #      # #  ## ##");
LCD.WriteLine("   #   ####  ##### # # #");
LCD.WriteLine("   #   #     #   # #   #");
LCD.WriteLine("   #   ##### #   # #   #");
LCD.WriteLine("          #     #     ");
LCD.WriteLine("         # #   ##     ");
LCD.WriteLine("        #####   #     ");
LCD.WriteLine("        #   #   #     ");
LCD.WriteLine("        #   #  ###    ");



Sleep(750);
LCD.Clear(FEHLCD::Black);
LCD.SetFontColor(FEHLCD::White);
}

//Display options
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
}
    void midrelease()
    {

        while (!buttons.MiddlePressed())
        {

        }
        while (buttons.MiddlePressed())
        {

        }
    }


//UI Function
int ui(void)
{

    splash();
    int c=0,j,temp, c2=0, n=0;



    //Master while loop
    while (1)
    {
        j=0;
    // Menu while loop
    while (j==0)
    {

    menu();

    dispopt(c);


    /* Allow change in c to select test */
    temp = c;
    c = options(c,4);

    /* Determine if a choice was made */
    if (c == temp)
    {
        /* Set counter to exit main menu loop */
       j=1;
    }


    }


// Option executes
    if (c == 0)
    {
        LCD.WriteLine("");
LCD.WriteLine("Running...");
Sleep(500);
    }
    else if (c == 1)
    {
        LCD.WriteLine("");
LCD.WriteLine("Running...");
Sleep(500);
    }
    else if (c == 2)
    {
        LCD.WriteLine("");
LCD.WriteLine("Running...");
Sleep(500);
    }
    else if (c == 3)
    {
        LCD.WriteLine("");
LCD.WriteLine("Running...");
Sleep(500);
    }

    }

 return c;
}


// Runs ui function which contains all menu related functions
int main(void)
{
    ui();
    return 0;
}

