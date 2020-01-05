#include "General.h"
using namespace std;

#define PORT 8081
bool isParsing = false;
bool isServerOpen= false;
bool serverReady = false;
queue<Var> setQueue ;
bool loop = false;
int stepsLoop = 0;
vector<string> loopLex;


void resetCommandMap(unordered_map<string,Var*> &varSim,
    unordered_map<string,Var> &varProgram, unordered_map<string, Command*> &commandMap){
    commandMap["openDataServer"] = new OpenServer(varSim);
    commandMap["connectControlClient"] = new ConnectCommand;
    commandMap["var"] = new DefineVarCommand(varSim, varProgram);
    commandMap["Print"] = new Print(varProgram);
    commandMap["Sleep"] = new Sleep;
    commandMap["if"] = new ifCommand(varSim, varProgram);
    commandMap["while"] = new loopCommands(varSim, varProgram);
}

int main() {
  // reading from the file-> splitting to commands-> inserting them to an vector
  Lexer lex("fly.txt");
  lex.lexing();
  // creating maps of variables and defineVarCommand
  unordered_map<string,Var*> varSim;
  unordered_map<string,Var> varProgram;
//  defineVarCommand VarCommand (varSim, varProgram); --- defined in commandMap

  // remember to insert VarCommand which created above, to the parser --- what that means?‬
  unordered_map<string, Command*> commandMap;
  resetCommandMap(varSim, varProgram, commandMap);
  Parser par(commandMap, lex.GetV1(), varSim, varProgram); // --- something wrong with the receiving of lex.GetV1() ---- אהרון : הגטר שלי מחזיר קונסט - הוספתי לך לקונסטרטור של הפארסר קונסט פשוט לפני הארגומנט של הוקטור
  par.parsing();
  isParsing = false;



////  ConnectCommand connect;
//  vector<string> v2;
//  v2.push_back("connectControlClient");
//  v2.push_back("127.0.0.1");
//  v2.push_back("5402");
////  connect.execute(v2);



  return 0;
}



