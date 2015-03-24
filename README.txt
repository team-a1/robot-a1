Both files must be in Library folder of the firmware folder.

Use uimenu.h to created a menu for choosing program.

Example:

main()
{
//Menu of 2 programs
int c;
c=ui(2);
if (c=0)
{
diagnostics();
}
else if (c=1)
{
// Main function

}


}
