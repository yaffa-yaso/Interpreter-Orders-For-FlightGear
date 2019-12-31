#include "General.h"
using namespace std;

#define PORT 8081

void resetCommandMap(unordered_map<string,Var> &varSim,
        unordered_map<string,Var> &varProgram, unordered_map<string, Command*> &commandMap){
    commandMap["openDataServer"] = new OpenServer(varProgram);
    commandMap["connectControlClient"] = new ConnectCommand;
    commandMap["var"] = new defineVarCommand(varSim, varProgram);
    commandMap["Print"] = new Print;
    commandMap["Sleep"] = new Sleep;
    commandMap["if"] = new ifCommand;
    commandMap["while"] = new loopCommands;

    // --- change zeros to the right value!
    commandMap["breaks"] = new SetVarCommand("breaks", 0,varSim, varProgram, 0);
    commandMap["throttle"] = new SetVarCommand("throttle", 0,varSim, varProgram, 0);
    commandMap["heading"] = new SetVarCommand("heading", 0,varSim, varProgram, 0);
    commandMap["airspeed"] = new SetVarCommand("airspeed", 0,varSim, varProgram, 0);
    commandMap["roll"] = new SetVarCommand("roll", 0,varSim, varProgram, 0);
    commandMap["pitch"] = new SetVarCommand("pitch", 0,varSim, varProgram, 0);
    commandMap["rudder"] = new SetVarCommand("rudder", 0,varSim, varProgram, 0);
    commandMap["aileron"] = new SetVarCommand("aileron", 0,varSim, varProgram, 0);
    commandMap["elevator"] = new SetVarCommand("elevator", 0,varSim, varProgram, 0);
    commandMap["alt"] = new SetVarCommand("alt", 0,varSim, varProgram, 0);
}

int main() {
  // reading from the file-> splitting to commands-> inserting them to an vector
  Lexer lex("fly.txt");
  lex.lexing();
  // creating maps of variables and defineVarCommand
  unordered_map<string,Var> varSim;
  unordered_map<string,Var> varProgram;
//  defineVarCommand VarCommand (varSim, varProgram); --- defined in commandMap

  // remember to insert VarCommand which created above, to the parser --- what that means?‬
  unordered_map<string, Command*> commandMap;
  resetCommandMap(varSim, varProgram, commandMap);
  Parser par(commandMap, lex.GetV1()); // --- something wrong with the receiving of lex.GetV1() ---- אהרון : הגטר שלי מחזיר קונסט - הוספתי לך לקונסטרטור של הפארסר קונסט פשוט לפני הארגומנט של הוקטור
  par.parsing();


////  ConnectCommand connect;
//  vector<string> v2;
//  v2.push_back("connectControlClient");
//  v2.push_back("127.0.0.1");
//  v2.push_back("5402");
////  connect.execute(v2);



  return 0;
}



