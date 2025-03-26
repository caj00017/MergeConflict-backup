// Include required header files
#include <mpx/comhand.h>
#include <mpx/comexec.h>
#include <mpx/device.h>
#include <mpx/serial.h>
#include <sys_req.h>
#include <string.h>


void comhand(void) {
    //Print Startup Screen
    rOver_Startup();
    print_color("@ ","blue");
    print("Type 'help' to see list of commands\n");

    while(1) {
        // process the command
        int status = comexec();
        
        // if the command is a shutdown, exit command loop
        if (status == -1) {
            println();
            print("Shutting down...");
            println();
            return;
        }
        if(status != 5) {
            println();
        }
    } 
}


//Get it the O and S are capitalized for 'OS' (I'm hilarious)
void rOver_Startup(void){
    print_color(" ______________________ ","blue");
    println();
    print_color("/ ~~ MERGE CONFLICT ~~ \\","blue");
    println();
    print_color("\\  ~~ STANDARD OS ~~   /","blue");
    println();
    print_color(" ----------------------- ","blue");
    println();
    print_color("          \\","blue");
    println();
    print_color("           \\","blue");
    println();
    print_color("            \\","blue" );
    print(" ______");
    println();
    print("              |   ");
    print_color("O","yellow");
    print("|");
    println();
    print("              8M==M8");
    println();
    print("                []");
    println();
    print("        _     <=[]                     T _,-\\");
    println();
    print("       (O)      []>      ,      , ___  #'  ,-\\");
    println();
    print("      //`\\\\ TT\\((O) mm   H  === H %%% ##.-'   \\");
    println();
    print("     //   \\\\ oI\"T\"\"\"\"\"\"\"\"\"\"\"\"|\"\"\"\"\"\"\"\"\"I  _,-'");
    println();
    print("    //     \\\\ [ |         A--+        D|-'");
    println();
    print("   //       \\\\[ |     _=--O--____     D|v");
    println();
    print("  (O)        aL_==##\"\"________==#o==,__J");
    println();
    print(" ACDMM      ==\"\"           =\"\"       ``\"==");
    println();
    print(" BCDV      /``\\          /``\\          /``\\");
    println();
    print("  |D|     |    |        |    |        |    |");
    println();
    print("   Y       \\__/          \\__/          \\__/");
    println();
    print_error("================================================");
    println();
}
