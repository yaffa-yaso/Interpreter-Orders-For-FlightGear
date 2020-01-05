#include "General.h"

Parser:: Parser(unordered_map<string, Command *> &map, const vector<string> &vec,
        unordered_map<string, Var*> &sim,unordered_map<string, Var> &program){
    this -> commandMap = map;
    this -> v = vec;
    this->varSim = &sim;
    this->varProgram = &program;
}

void Parser:: parsing(){
   isParsing = true;
    int i = 0;
    Command* c = NULL;
    //activate all commands in the lexer
    while(i < v.size()){
        while(v[i] == "}"){
          if(loop) {
           i=i-stepsLoop;
           loop=false;
          }else {
            i++;
          }
        }
        if(v[i] == "var"){
            if(commandMap.find(v[i+1]) == commandMap.end()){
                commandMap[v[i+1]] = new SetVarCommand(*varSim, *varProgram);
            }
        }
        c = commandMap[v[i]];
        if(c != NULL){
            vector<string> vec = cut(i);
            i += c->execute(vec);
        }
    }
}
vector<string> Parser:: cut( int m){
    auto first = v.cbegin() + m;
    auto last = v.cend();

    vector<string> vec(first, last);
    return vec;
}

