//
// Created by aharon on 18/12/2019.
//
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
#include <arpa/inet.h>
#include <fstream>
#include <algorithm>
#include <cstring>
#include "Command.h"
#include <chrono>
#include <thread>
#include <mutex>
#include <cstddef>


using namespace std;
using namespace std::literals::chrono_literals;

extern bool isParsing;
extern bool isServerOpen;
extern bool serverReady;
extern bool loop;
extern int stepsLoop;
extern vector<string> loopLex;
extern std::mutex mutex1;
extern std::mutex mutex2;
extern condition_variable isThreadEnd;
extern condition_variable isThreadEnd2;


extern condition_variable isThreadEnd2;



class Lexer {
 private:
  string fileName;
  vector<string> v1;
 public:
  Lexer(string fileName);
  void lexing();
  void fixLine(string &line);
  void removeWhite(string line);
  const vector<string> &GetV1() const;
  bool isprefix(string &str, string &prefix);
  virtual ~Lexer();
};

class Var {
 private:

  double value =  0.0;
  string name = "";
  string BoundWay = "-1";
  string sim = "";
 public:
  void SetValue(double value);
  void SetBoundWay(const string &bound_way);
  void SetSim(const string &sim);
  double GetValue() const;
  const string &GetBoundWay() const;
  const string &GetSim() const;
  const string &GetName() const;
  void SetName(const string &name);
 public:
  Var();

};

//gloabl variable
extern  queue<Var> setQueue ;
//

class DefineVarCommand : public Command {
 private:
  unordered_map<string, Var*> *varSim;
  unordered_map<string, Var> *varProgram;
 public:
  DefineVarCommand(unordered_map<string, Var*> &varSim, unordered_map<string, Var> &varProgram);
  void insertToMap(unordered_map<string, Var> &sourceMap, map<string,string> &destMap);
  int execute(vector<string> &arguments) override ;
  ~DefineVarCommand()override ;
};

class SetVarCommand : public Command {
 private:
  int sockfd;
  unordered_map<string, Var*> *varSim;
  unordered_map<string, Var> *varProgram;
  string name;
  double value;
 public:
  SetVarCommand(unordered_map<string, Var*> &varSim,unordered_map<string, Var> &varProgram);
  int execute(vector<string> &arguments) override;
  ssize_t sendMessage(string path);
  void insertToMap(unordered_map<string, Var> &sourceMap, map<string,string> &destMap);
  ~SetVarCommand()override ;
};

class ConnectCommand : public Command {

 public:
  ConnectCommand();
  int clientConnect(int client_socket);
  int execute(vector<string> &arguments) override;
  int newSocket(string adress2, int port);
  ~ConnectCommand()override ;
};

class ConditionParser : public Command {
 protected:
  bool condition = false;
  unordered_map<string, Var*> *varSim;
  unordered_map<string, Var> *varProgram;
 public:
  ConditionParser(unordered_map<string, Var*> &varSim,unordered_map<string, Var> &varProgram);
  int execute(vector<string> &arguments) override;
  int commandsCounter(vector<string> &arguments);
  bool checkCondition (vector<string> &arguments);
 void insertToMap(unordered_map<string, Var> &sourceMap, map<string,string> &destMap);
};

class loopCommands : public ConditionParser {

 public:
  loopCommands(unordered_map<string, Var*> &var_sim,
               unordered_map<string, Var> &var_program);
  int execute(vector<string> &arguments) override;
};

class Expression {

 public:
  virtual double calculate() = 0;
  virtual ~Expression() {}
};


class ifCommand: public ConditionParser {
 public:
  ifCommand(unordered_map<string, Var*> &var_sim,
            unordered_map<string, Var> &var_program);
  int execute(vector<string> &v) override;
  vector<string> cut(vector<string> &v, int m);
  ~ifCommand() override;
};

class OpenServer: public Command{
 protected:
  unordered_map<string,Var*> *pathMap;
  unordered_map<string,Var> *varProgram;
  string ip = "";
  int port;
  string table[36];
 public:
//  OpenServer(unordered_map<string, Var> &varProgram_)  :    varProgram(varProgram_) {};
  OpenServer(unordered_map<string,Var*> &varSim) ;
  int execute(vector<string> &v) override;
  void Server(int client_socket);
  void tableUpdate();
  int newSocket();
  virtual ~OpenServer()override ;
};

class Print: public Command {
 private:
  unordered_map<string, Var> *varProgram;
 public:
//  Print(unordered_map<string, Var> &varSim,unordered_map<string, Var> &varProgram);
  Print(unordered_map<string, Var> &varProgram) ;
  int execute(vector<string> &v) override;
  void insertToMap(unordered_map<string, Var> &sourceMap, map<string,string> &destMap);
  virtual ~Print()override ;
};

class Sleep: public Command{
 public:
  int execute(vector<string> &v) override;
  void sleep(int milli);
  virtual ~Sleep()override ;
};

class Parser {
 private:
  unordered_map<string, Command*> commandMap;
  vector<string> v;
  unordered_map<string, Var*> *varSim;
  unordered_map<string, Var> *varProgram;

 public:
  Parser(unordered_map<string, Command *> &map, const vector<string> &vec,
         unordered_map<string, Var*> &sim,unordered_map<string, Var> &program);
  void parsing();
  vector<string> cut(int m);
};



/////////////////// ex1 files

// UNARY
class UnaryOperator : public Expression {
 public:
  double calculate()override;
  UnaryOperator(Expression *exp);
  virtual ~UnaryOperator();
 protected:
  Expression *exp = nullptr;
};

//Variable
class Variable : public Expression {
 private:
  string name;
  double value;
 public:
  void setName(const string &name);
  void setValue(double value);
  virtual ~Variable();
  Variable(string name, double value);
  double calculate()override ;
  Variable &operator++();
  Variable &operator--();
  Variable &operator++(int);
  Variable &operator--(int);
  Variable &operator+=(double v1);
  Variable &operator-=(double v1);

  const string &getName() const;

  double getValue() const;

};

//binary

class BinaryOperator : public Expression {
 public:
  double calculate()override ;
  BinaryOperator(Expression *right1, Expression *left1);
  virtual ~BinaryOperator();
 protected:
  Expression *right = nullptr;
  Expression *left = nullptr;
};

// plus

class Plus : public BinaryOperator {
 public:
  double calculate();
  Plus(Expression *right1, Expression *left1);
  virtual ~Plus();
};
//
// Minus

class Minus : public BinaryOperator {
 public:
  Minus(Expression *right, Expression *left);
  double calculate();
  virtual ~Minus();
};

// Div

class Div : public BinaryOperator {
 public:
  double calculate() ;
  Div(Expression *right_1, Expression *left_1);
  virtual ~Div();
};

//Mult

class Mul : public BinaryOperator {
 public:
  double calculate();
  Mul(Expression *right_1, Expression *left_1);
  virtual ~Mul();
};

//Value

class Value : public Expression {
 private:
  double value;
 public:
  double calculate() ;
  Value(double value);
  virtual ~Value();
};

//Uplus

class UPlus : public UnaryOperator {
 public:
  double calculate();
  UPlus(Expression *exp);
  virtual ~UPlus();
};

//UMinus

class UMinus : public UnaryOperator {
 public:
  UMinus(Expression *exp);
  double calculate();
  virtual ~UMinus();
};
// interpreter
class Interpreter {
 private:
  string inPutString;
  map<string,string > variablesMap;
  map<string,int> precedence;
  stack<string>operatorStack;
  stack<Expression*>finalExp;
  queue<string>numQueue;
  string postFixExp;
  vector< string> realNumbers;

 public:
  Interpreter ();
  Expression* evaluate ();
  Expression* interpret(string str);
  void setVariables(string exp);
  void splitInput(string str);
  bool unaryCheck(string opr, string last,int roundNumber );
  virtual ~Interpreter();
  map<string,string> &GetVariablesMap() ;
};