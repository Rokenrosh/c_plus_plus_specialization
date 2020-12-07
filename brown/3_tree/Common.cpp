#include "Common.h"
#include <sstream>

using namespace std;

class Node: public Expression {
public:
  Node(int value): value_(value){}

  int Evaluate() const override;

  std::string ToString() const override;
private:
  int value_;
};

class Sum_op: public Expression {
public:
  Sum_op(ExpressionPtr l, ExpressionPtr r): left(move(l)), right(move(r)){
  }

  int Evaluate() const override;

  std::string ToString() const override;

private:
  ExpressionPtr left, right;
};

class Mul_op: public Expression{
public:
  Mul_op(ExpressionPtr l, ExpressionPtr r): left(move(l)), right(move(r)){
  }

  int Evaluate() const override;

  std::string ToString() const override;

private:
  ExpressionPtr left, right;
};

int Node::Evaluate() const{
    return value_;
}

string Node::ToString() const{
    string result = to_string(value_);
    return result;
}

int Sum_op::Evaluate() const{
    return left->Evaluate() + right->Evaluate();
}

string Sum_op::ToString() const{
    stringstream result;
    result << '(' << left->ToString() << ')' << '+' << '(' << right->ToString() << ')';
    return result.str();
}

int Mul_op::Evaluate() const{
    return left->Evaluate() * right->Evaluate();
}

string Mul_op::ToString() const{
    stringstream result;
    result << '(' << left->ToString() << ')' << '*' << '(' << right->ToString() << ')';
    return result.str();
}

ExpressionPtr Value(int value){
    return ExpressionPtr(make_unique<Node>(value));
}

ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right){
    return ExpressionPtr(make_unique<Sum_op>(move(left), move(right)));
}

ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right){
    return ExpressionPtr(make_unique<Mul_op>(move(left), move(right)));
}
