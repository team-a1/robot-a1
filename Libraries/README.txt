Both files must be in Library folder of the firmware folder.

Use uimenu.h to created a menu for choosing program.
ui(n) creates the menu where n is the number of programs including diagnostics.
Diagnostics is program 0 and automatically included as long as diagnostics.h is included

Example:

main()
{
//
//Menu of 3 programs- including diagnostics
int c;
c=ui(2);

//Diagnostics is included in uimenu and is choice 0 by default
// Therefore start the if-else at c=1

if (c=1)
{
// Main function

}

else if (c=2)
{
// Second function

}
}
