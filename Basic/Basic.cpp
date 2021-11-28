/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../../../Desktop/BASIC-Interpreter-main/StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../../../Desktop/BASIC-Interpreter-main/StanfordCPPLib/strlib.h"
//add some headfiles
#include"statement.h"

using namespace std;

/* Function prototypes */

bool isNum(string str) {
    stringstream sin(str);
    int ii;
    double dd;
    char cc;
    if (sin >> dd) { return 0; }
    if (sin >> cc) { return 0; }
    if (sin >> ii) { return 1; }
    return 1;
}

void processLine(string line, Program &program, EvalState &state);

/* Main program */

int main() {
    EvalState state;
    Program program;
    cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {//检查错误，有错误就扔给catch执行
            string input = getLine();
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program, EvalState &state) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);

    if (line == "QUIT") exit(0);//在main函数以外的地方直接终止运行
    if (scanner.hasMoreTokens()) {
        string token = scanner.nextToken();
        if (isNum(token)) { //有行号
            if (scanner.hasMoreTokens()) { //maybe need to transfer

                const char *tokenn = token.c_str();
                program.addSourceLine(atoi(tokenn), line);//用atoi字符串强行转数字

            } else {
                const char *token_ = token.c_str();
                program.removeSourceLine(atoi(token_));
            }
        } else if (scanner.hasMoreTokens()) {// PRINT & LET & INPUT
            if (token != "PRINT" && token != "LET" && token != "INPUT") error("SYNTAX ERROR");
            else {
                if (token == "PRINT") {
                    Expression *exp = parseExp(scanner);
                    int value = exp->eval(state);
                    cout << value << endl;
                    delete exp;
                }
                if (token == "LET") {
                    string tmp1 = scanner.nextToken();
                    if (tmp1 == "LET" || tmp1 == "INPUT" || tmp1 == "QUIT" || tmp1 == "PRINT")
                        error("SYNTAX ERROR");
                    if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
                    string tmp2 = scanner.nextToken();
                    if (!scanner.hasMoreTokens()) error("SYNTAX ERROR");
                    string tmp3 = scanner.nextToken();
                    if (tmp2 != "=" || !isNum(tmp2)) error("SYNTAX ERROR");
                    const char *num_ = tmp2.c_str();
                    state.setValue(tmp1, atoi(num_));
                }
                if (token == "INPUT") {
                    Statement *temp = nullptr;
                    temp = StatementForParser(0, line);
                    temp->execute(state);
                    delete temp;
                }
            }
        } else {// list & quit
            if (token == "LIST") {
                program.PRINT();
            } else if (token == "HELP") {
                cout << "Yet another basic interpreter" << endl;
            } else if (token == "CLEAR") {
                program.clear();
            } else if (token == "RUN") {
                program.RUN(state);
            } else {
                error("SYNTAX ERROR");
            }
        }

    } else return;
}
