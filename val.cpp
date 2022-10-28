using namespace std;

#include "val.h"

Value Value::operator+(const Value& op) const
{
    if((this->GetType() == ValType::VINT ||
       this->GetType() == ValType::VREAL) &&
       (op.GetType() == ValType::VINT ||
       op.GetType() == ValType::VREAL))
    {
        float val1 = 0;
        if(this->GetType() == ValType::VINT)
            val1 += this->GetInt();
        if(this->GetType() == ValType::VREAL)
            val1 += this->GetReal();
        float val2 = 0;
        if(op.GetType() == ValType::VINT)
            val2 += op.GetInt();
        if(op.GetType() == ValType::VREAL)
            val2 += op.GetReal();
        return Value(val1+val2);
    }
    if(this->GetType() == ValType::VSTRING && this->GetType() == op.GetType())
    {
        return Value(this->GetString() + op.GetString());
    }
    return Value();
}
Value Value::operator-(const Value& op) const
{
    if((this->GetType() == ValType::VINT ||
       this->GetType() == ValType::VREAL) &&
       (op.GetType() == ValType::VINT ||
       op.GetType() == ValType::VREAL))
    {
        float val1 = 0;
        if(this->GetType() == ValType::VINT)
            val1 += this->GetInt();
        if(this->GetType() == ValType::VREAL)
            val1 += this->GetReal();
        float val2 = 0;
        if(op.GetType() == ValType::VINT)
            val2 += op.GetInt();
        if(op.GetType() == ValType::VREAL)
            val2 += op.GetReal();
        return Value(val1-val2);
    }
    if(this->GetType() == ValType::VSTRING && this->GetType() == op.GetType())
    {
        return Value(this->GetString() + op.GetString());
    }
    return Value();
}
Value Value::operator*(const Value& op) const
{
    if((this->GetType() == ValType::VINT ||
       this->GetType() == ValType::VREAL) &&
       (op.GetType() == ValType::VINT ||
       op.GetType() == ValType::VREAL))
    {
        float val1 = 0;
        if(this->GetType() == ValType::VINT)
            val1 += this->GetInt();
        if(this->GetType() == ValType::VREAL)
            val1 += this->GetReal();
        float val2 = 0;
        if(op.GetType() == ValType::VINT)
            val2 += op.GetInt();
        if(op.GetType() == ValType::VREAL)
            val2 += op.GetReal();
        return Value(val1*val2);
    }
    return Value();
}
Value Value::operator/(const Value& op) const
{
    if((this->GetType() == ValType::VINT ||
       this->GetType() == ValType::VREAL) &&
       (op.GetType() == ValType::VINT ||
       op.GetType() == ValType::VREAL))
    {
        float val1 = 0;
        if(this->GetType() == ValType::VINT)
            val1 += this->GetInt();
        if(this->GetType() == ValType::VREAL)
            val1 += this->GetReal();
        float val2 = 0;
        if(op.GetType() == ValType::VINT)
            val2 += op.GetInt();
        if(op.GetType() == ValType::VREAL)
            val2 += op.GetReal();
        if(val2 == 0)
            return Value();
        return Value(val1/val2);
    }
    return Value();
}
Value Value::operator==(const Value& op) const
{

    if((this->GetType() == ValType::VINT ||
       this->GetType() == ValType::VREAL) &&
       (op.GetType() == ValType::VINT ||
       op.GetType() == ValType::VREAL))
    {
        float val1 = 0;
        if(this->GetType() == ValType::VINT)
            val1 += this->GetInt();
        if(this->GetType() == ValType::VREAL)
            val1 += this->GetReal();
        float val2 = 0;
        if(op.GetType() == ValType::VINT)
            val2 += op.GetInt();
        if(op.GetType() == ValType::VREAL)
            val2 += op.GetReal();
        return Value(val1 == val2);
    }
    if(this->GetType() == op.GetType())
    {
        if(this->GetType() == ValType::VBOOL)
            return Value(this->GetBool() == op.GetBool());
        if(this->GetType() == ValType::VSTRING)
            return Value(this->GetString() == op.GetString());
    }
    return Value();
}

Value Value::operator>(const Value& op) const
{
    if((this->GetType() == ValType::VINT ||
       this->GetType() == ValType::VREAL) &&
       (op.GetType() == ValType::VINT ||
       op.GetType() == ValType::VREAL))
    {
        float val1 = 0;
        if(this->GetType() == ValType::VINT)
            val1 += this->GetInt();
        if(this->GetType() == ValType::VREAL)
            val1 += this->GetReal();
        float val2 = 0;
        if(op.GetType() == ValType::VINT)
            val2 += op.GetInt();
        if(op.GetType() == ValType::VREAL)
            val2 += op.GetReal();
        return Value(val1>val2);
    }
    return Value();
}
Value Value::operator<(const Value& op) const
{
    if((this->GetType() == ValType::VINT ||
       this->GetType() == ValType::VREAL) &&
       (op.GetType() == ValType::VINT ||
       op.GetType() == ValType::VREAL))
    {
        float val1 = 0;
        if(this->GetType() == ValType::VINT)
            val1 += this->GetInt();
        if(this->GetType() == ValType::VREAL)
            val1 += this->GetReal();
        float val2 = 0;
        if(op.GetType() == ValType::VINT)
            val2 += op.GetInt();
        if(op.GetType() == ValType::VREAL)
            val2 += op.GetReal();
        return Value(val1<val2);
    }
    return Value();
}
