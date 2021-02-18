//
//      Sample IDC program to automate IDA Pro.
//
//      IDA Pro can be run from the command line in the
//      batch (non-interactive) mode.
//
//      If IDA is started with
//
//              idag -A -Sanalysis.idc file
//
//      then this IDC file will be executed. It performs the following:
//
//        - the code segment is analyzed
//        - the output file is created
//        - IDA exits to OS
//
//      Feel free to modify this file as you wish
//      (or write your own script/plugin to automate IDA)
//
//      Since the script calls the Exit() function at the end,
//      it can be used in the batch files (use text mode idaw.exe)
//
//      NB: "idag -B file" is equivalent to the command line above
//

#include <idc.idc>

static main()
{
  auto file;

  // turn on coagulation of data in the final pass of analysis
  SetShortPrm(INF_AF2, GetShortPrm(INF_AF2) | AF2_DODATA);

  Message("Waiting for the end of the auto analysis...\n");
  Wait();

  Message("\n\n------ Creating the output file.... --------\n");
  file = GetIdbPath();
  file = substr(file,0,strstr(file,".")) + ".asm";
  WriteTxt(file, 0, BADADDR);           // create the assembler file
  Message("All done, exiting...\n");
  Exit(0);                              // exit to OS, error code 0 - success
}
