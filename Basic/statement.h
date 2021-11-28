/*
 * File: statement.h
 * -----------------
 * This file defines the Statement abstract type.  In
 * the finished version, this file will also specify subclasses
 * for each of the statement types.  As you design your own
 * version of this class, you should pay careful attention to
 * the exp.h interface, which is an excellent model for
 * the Statement class hierarchy.
 */

#ifndef _statement_h
#define _statement_h

#include "evalstate.h"
#include "exp.h"
#include "parser.h"

/*
 * Class: Statement
 * ----------------
 * This class is used to represent a statement in a program.
 * The model for this class is Expression in the exp.h interface.
 * Like Expression, Statement is an abstract class with subclasses
 * for each of the statement and command types required for the
 * BASIC interpreter.
 */
class jumpOverTheLine {
public:
    int action, lineNum;

    jumpOverTheLine(int a, int b) {
        //0 END, 1 CHANGE
        action = a;
        lineNum = b;
    }
};

class Statement {

public:

/*
 * Constructor: Statement
 * ----------------------
 * The base class constructor is empty.  Each subclass must provide
 * its own constructor.
 */

    Statement();

/*
 * Destructor: ~Statement
 * Usage: delete stmt;
 * -------------------
 * The destructor deallocates the storage for this expression.
 * It must be declared virtual to ensure that the correct subclass
 * destructor is called when deleting a statement.
 */

    virtual ~Statement();

/*
 * Method: execute
 * Usage: stmt->execute(state);
 * ----------------------------
 * This method executes a BASIC statement.  Each of the subclasses
 * defines its own execute method that implements the necessary
 * operations.  As was true for the expression evaluator, this
 * method takes an EvalState object for looking up variables or
 * controlling the operation of the interpreter.
 */

    virtual void execute(EvalState &state) = 0;

};

Statement *StatementForParser(bool whetherlineNum, string ptr);

/*
 * The remainder of this file must consists of subclass
 * definitions for the individual statement forms.  Each of
 * those subclasses must define a constructor that parses a
 * statement from a scanner and a method called execute,
 * which executes that statement.  If the private data for
 * a subclass includes data allocated on the heap (such as
 * an Expression object), the class implementation must also
 * specify its own destructor method to free that memory.
 */

class REM : public Statement {
public:
    REM(Expression *tmp) {};

    virtual ~REM() {};

    void execute(EvalState &state) {};
};

class LET : public Statement {
private:
    Expression *exp;
public:
    LET(Expression *tmp) {
        exp = tmp;
        if (exp->getType() != COMPOUND) { error("SYTAX ERROR"); }
        if (((CompoundExp *) exp)->getOp() != "=") { error("SYNTAX ERROR"); }
    }

    virtual ~LET() {
        delete exp;
    };

    void execute(EvalState &state) {
        exp->eval(state);
    };
};

class PRINT : public Statement {
private:
    Expression *exp;
public:
    PRINT(Expression *tmp) {
        exp = tmp;
    }

    virtual ~PRINT() {
        delete exp;
    }

    void execute(EvalState &state) {
        cout << exp->eval(state) << endl;
    }
};

class INPUT : public Statement {
public:
    INPUT(Expression *temp);

    virtual ~INPUT();

    void execute(EvalState &state);

    Expression *ptr;
};

class END : public Statement {
public:
    END() {};

    virtual ~END() {};

    void execute(EvalState &state) {
        throw jumpOverTheLine(0, 0);
    }
};

class GOTO : public Statement {
private:
    int lineNum;
    Expression *exp;
public:
    GOTO(Expression *tmp) {
        exp = tmp;
        try {
            lineNum = stringToInteger(exp->toString());
        } catch (...) {//不进行检查类型
            error("SYNTAX ERROR");
        }
    }

    virtual ~GOTO() {
        delete exp;
    }

    void execute(EvalState &state) {
        throw jumpOverTheLine(1, lineNum);
    }

};

class IF : public Statement {       //expression : IF exp cmp exp THEN lineNum(n)
private:                            //   ptr     :     1   2   3    4      5
    Expression *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;
public:
    IF(Expression *tmp1, Expression *tmp2, Expression *tmp3, Expression *tmp4, Expression *tmp5) {
        tmp1 = ptr1;
        tmp2 = ptr2;
        tmp3 = ptr3;
        tmp4 = ptr4;
        tmp5 = ptr5;
    }

    virtual ~IF() {
        delete ptr1;
        delete ptr2;
        delete ptr3;
        delete ptr4;
        delete ptr5;
    }

    void execute(EvalState &state) {
        if (ptr2->toString() == "=") {
            if (ptr1->eval(state) == ptr3->eval(state)) throw jumpOverTheLine(1, stringToInteger(ptr5->toString()));
        } else if (ptr2->toString() == ">") {
            if (ptr1->eval(state) > ptr3->eval(state)) throw jumpOverTheLine(1, stringToInteger(ptr5->toString()));
        } else if (ptr2->toString() == "<") {
            if (ptr1->eval(state) < ptr3->eval(state)) throw jumpOverTheLine(1, stringToInteger(ptr5->toString()));
        }
    }
};

#endif
