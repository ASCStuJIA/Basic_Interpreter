/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include "program.h"
#include "statement.h"
#include<map>

using namespace std;

Program::Program() {
    // Replace this stub with your own code
}

Program::~Program() {
    // Replace this stub with your own code
}

void Program::clear() {
    if (!Lines.empty()) {
        auto it = Lines.begin();
        while (it != Lines.end()) {
            delete it->second.lineMeaning;
            it++;
        }
    }
    Lines.clear();
}

void Program::addSourceLine(int lineNumber, string line) {
    auto it = Lines.find(lineNumber);//找到第lineNumber行对应的容器，很好用
    if (it == Lines.end()) {
        Line tmp;
        tmp.lineContent = line;
        tmp.lineMeaning = StatementForParser(1, line);
        Lines.insert(make_pair(lineNumber, tmp));
    } else {
        removeSourceLine(lineNumber);
        addSourceLine(lineNumber, line);
    }
}

void Program::removeSourceLine(int lineNumber) {
    auto it = Lines.find(lineNumber);
    if (it == Lines.end()) { return; }
    else {
        delete it->second.lineMeaning;
        Lines.erase(it);//把现声明的指针删掉
    }
}

string Program::getSourceLine(int lineNumber) {
    auto it = Lines.find(lineNumber);
    if (it == Lines.end()) return "";
    else { return it->second.lineContent; }
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
    //auto it = Lines.find(lineNumber);
    //it->lineMeaning = stmt;
    Lines[lineNumber].lineMeaning = stmt;
}

Statement *Program::getParsedStatement(int lineNumber) {
    auto it = Lines.find(lineNumber);
    if (it == Lines.end()) { return NULL; }
    else { it->second.lineMeaning; }

}

int Program::getFirstLineNumber() {//要寻找的是行号，但不一定从零开始
    if (!Lines.empty()) { return Lines.begin()->first; }
    else return 0;
}

int Program::getNextLineNumber(int lineNumber) {//要寻找的是下一个行号
    auto it = Lines.find(lineNumber);
    //auto end = Lines.end();
    //end--;
    if (it == Lines.end()) { return 0; }
    //else if (it == end) { return 0; }
    it++;
    return it->first;
}

void Program::PRINT() {
    if (!Lines.empty()) {
        auto it = Lines.begin();
        for (; it != Lines.end(); it++) {
            cout << it->second.lineContent;
        }
    }
}

void Program::RUN(EvalState &state) {
    if (!Lines.empty()) {
        auto it = Lines.begin();
        for (; it != Lines.end(); it++) {
            Statement *tmp;
            tmp = StatementForParser(1, it->second.lineContent);
            try {
                tmp->execute(state);
            } catch (jumpOverTheLine tem) {
                if (tem.action == 0) { break; }     //0 end
                if (tem.action == 1) {              //1 change
                    auto iter = Lines.find(tem.lineNum);
                    if (iter == Lines.end()) cout << "LINE NUMBER ERROR" << endl;
                    else {
                        it = iter;
                        continue;
                    }
                }
            }
            delete tmp;
        }
    }
}
