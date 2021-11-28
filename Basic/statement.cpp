/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
// add some headfiles
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

void parseTheToken() {}

Statement *StatementForParser(bool whetherlineNum, string tag) {
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(tag);
    string token;
    if (whetherlineNum == 1) {
        token = scanner.nextToken();//有行号，继续获取下一个字段
    }
    token = scanner.nextToken();//没进if说明有行号，跳过行号，获取下一个字段，及时statement.h里边写的那些basic语法


    if (token == "REM") {
        Expression *exp = parseExp(scanner);
        return new REM(exp);
    } else if (token == "LET") {
        Expression *exp = parseExp(scanner);
        string tmp1 = scanner.nextToken();
        if (tmp1 == "LET" || tmp1 == "INPUT" || tmp1 == "QUIT" || tmp1 == "PRINT")
            error("SYNTAX ERROR");
        return new LET(exp);
    } else if (token == "PRINT") {
        Expression *exp = parseExp((scanner));
        return new PRINT(exp);
    } else if (token == "INPUT") {
        Expression *tmp = nullptr;
        tmp = parseExp(scanner);
        if (tmp->getType() != IDENTIFIER) { error("SYNTAX ERROR"); }
        return new INPUT(tmp);
    } else if (token == "END") {
        return new END();
    } else if (token == "GOTO") {
        Expression *exp = parseExp(scanner);
        return new GOTO(exp);
    } else if (token == "IF") {
        Expression *exp = parseExp(scanner);
        Expression *tmp1 = readT(scanner);
        if (tmp1->getType() != IDENTIFIER) { error("SYNTAX ERROR"); }
        Expression *tmp2 = readT(scanner);
        if (tmp2->toString() != "=" && tmp2->toString() != "<" && tmp2->toString() != ">") { error("SYNTAX ERROR"); }
        Expression *tmp3 = readT(scanner);
        if (tmp3->getType() != IDENTIFIER) { error("SYNTAX ERROR"); }
        Expression *tmp4 = parseExp(scanner);
        if (tmp4->toString() != "THEN") { error("SYNTAX ERROR"); }
        Expression *tmp5 = parseExp(scanner);
        if (tmp5->getType() != CONSTANT) { error("SYNTAX ERROR"); }
        return new IF(tmp1, tmp2, tmp3, tmp4, tmp5);
    } else { error("SYNTAX ERROR"); }//一开始就是错的
}

INPUT::INPUT(Expression *tmp) { ptr = tmp; }

INPUT::~INPUT() noexcept {};//bool右值，判断所给函数是否异常

void INPUT::execute(EvalState &state) {
    int value;
    while (true) {
        try {
            string tmp;
            cout << " ? ";
            cin >> tmp;
            value = stringToInteger(tmp);
        } catch (...) {
            cout << "INVALID NUMBER" << endl;
            continue;
        }
        break;
    }
    state.setValue(((IdentifierExp *) ptr)->getName(), value);
}



