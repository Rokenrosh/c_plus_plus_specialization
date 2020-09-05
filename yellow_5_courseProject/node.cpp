#include "node.h"
#include "date.h"
#include <string>

using namespace std;

template <class T1, class T2>
bool Cmp(Comparison cmp, const T1& left, const T2& right){
    switch(cmp){
        case Comparison::Less:
            return left < right;
        case Comparison::LessOrEqual:
            return left <= right;
        case Comparison::Greater:
            return left > right;
        case Comparison::GreaterOrEqual:
            return left >= right;
        case Comparison::Equal:
            return left == right;
        case Comparison::NotEqual:
            return left != right;
    }
    return false;
}

bool EmptyNode::Evaluate(const Date& date, const string& event) const{
    return true;
}

LogicalOperationNode::LogicalOperationNode(const LogicalOperation& lo, shared_ptr<Node> left, shared_ptr<Node> right): _lo(lo), _left(left), _right(right){}

bool LogicalOperationNode::Evaluate(const Date& date, const string& event) const{
    if(_lo == LogicalOperation::And){
        return _left->Evaluate(date, event) && _right->Evaluate(date, event);
    }
    else {
        return _left->Evaluate(date, event) || _right->Evaluate(date, event);
    }
}

DateComparisonNode::DateComparisonNode(const Comparison& cmp, const Date& date): _cmp(cmp), _date(date){}

bool DateComparisonNode::Evaluate(const Date& date, const string& event) const{
    return Cmp(_cmp, date, _date);
}

EventComparisonNode::EventComparisonNode(const Comparison& cmp, const string& event): _cmp(cmp), _event(event){}

bool EventComparisonNode::Evaluate(const Date& date, const string& event) const{
    return Cmp(_cmp, event, _event);
}