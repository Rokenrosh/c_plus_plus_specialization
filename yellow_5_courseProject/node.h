#pragma once

#include <memory>
#include "date.h"

using namespace std;

enum class Comparison{
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual
};

enum class LogicalOperation{
    Or,
    And
};

template <class T1, class T2>
bool Cmp(Comparison cmp, const T1& left, const T2& right);


class Node{
public:
    virtual bool Evaluate(const Date& date, const string& event) const = 0;
};

class EmptyNode: public Node{
    bool Evaluate(const Date& date, const string& event) const override;
};

class LogicalOperationNode: public Node{
public:
    LogicalOperationNode(const LogicalOperation& lo, shared_ptr<Node> left, shared_ptr<Node> right);
    bool Evaluate(const Date& date, const string& event) const override;
private:
    const LogicalOperation _lo;
    shared_ptr<const Node> _left;
    shared_ptr<const Node> _right;
};

class DateComparisonNode: public Node{
public:
    DateComparisonNode(const Comparison& cmp, const Date& date);
    bool Evaluate(const Date& date, const string& event) const override;
private:
    const Comparison _cmp;
    const Date _date;
};

class EventComparisonNode: public Node{
public:
    EventComparisonNode(const Comparison& cmp, const string& event);
    bool Evaluate(const Date& date, const string& event) const override;
private:
    const Comparison _cmp;
    const string _event;
};

