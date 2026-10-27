// src/tools/calculator_eval.cpp — YOU IMPLEMENT (spec §4.6).
//
// calc_evaluate: evaluate a well-formed token list with two stacks
// (one for numbers, one for operators and '('). See spec §4.6 and the
// worked trace in Appendix C. std::stack or std::vector are both fine.

#include "tools/calculator.h"
#include <stack>

static int precedence(CalcToken::Kind kind){
    if (kind == CalcToken::Kind::Times || kind == CalcToken::Kind::Divide){
        return 2;
    }
    else if (kind == CalcToken::Kind::Plus || kind == CalcToken::Kind::Minus){
        return 1;
    }
    else{
        return 0;
    }
}

static bool operation(std::stack<double>& values , std::stack<CalcToken::Kind>& ops, std::string* error){ //meed refs for pop 
    if (ops.empty() || values.size() < 2){
        *error = "no stack";
        return false;
    }
    else{
        double right_op = values.top();
        values.pop();

        double left_op = values.top();
        values.pop();

        CalcToken::Kind op_top = ops.top();
        ops.pop();

        double total {};

        if (op_top == CalcToken::Kind::Times){
            total = left_op * right_op;
        }
        else if (op_top == CalcToken::Kind::Divide){
            if (right_op == 0.0){
                *error = "division by zero";
                return false;
            }
            else{
                total = static_cast<double>(left_op)/static_cast<double>(right_op);
            }
        }
        else if (op_top == CalcToken::Kind::Plus){
            total = right_op + left_op;
        }
        else if (op_top == CalcToken::Kind::Minus){
            total = left_op - right_op;
        }

        values.push(total);
        return true;
    }
}
bool calc_evaluate(const std::vector<CalcToken>& tokens, double* out, std::string* error) {

    std::stack<double> values;
    std::stack<CalcToken::Kind> ops;

    for (const auto& token: tokens){
        if (token.kind == CalcToken::Kind::Number){ //Kind is number, need a case for each
            values.push(token.value);
        }
        else if (token.kind == CalcToken::Kind::LParen){
            ops.push(token.kind);
        }
        else if (token.kind == CalcToken::Kind::RParen){
            while (!ops.empty() && ops.top() != CalcToken::Kind::LParen){
                if (!operation(values,ops,error)){
                    return false;
                }    
            }

            if (!ops.empty()){
                ops.pop();
            }
        }
        else{
            while (!ops.empty() && precedence(ops.top()) >= precedence(token.kind)) {
                if (!operation(values, ops, error)){
                    return false;
                }
            }
            // Finally, push the current operator
            ops.push(token.kind);
        }
    }

    //Must clear out when tokens run out
    while (!ops.empty()){
        if (!operation(values,ops,error)){
            return false;
        }
    }

    if (values.size() != 1){
        *error = "bad expression provided by user";
        return false;
    }
    else{ //size is only 1
        *out = values.top();
        return true;
    }
}
