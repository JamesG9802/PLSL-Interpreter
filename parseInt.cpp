/* Implementation of Recursive-Descent Parser
 * parse.cpp
 * Programming Assignment 2
 * Spring 2022
*/
#include <stack>
#include <queue>
#include "parseInt.h"
#include "val.h"
map<string, bool> defVar;
map<string, Token> SymTable;

using namespace std;
namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
	}

	static void PushBackToken(LexItem & t) {
		if( pushed_back ) {
			abort();
		}
		pushed_back = true;
		pushed_token = t;
	}

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
	++error_count;
	cout << line << ": " << msg << endl;
}

queue<string> VarNameQue;
queue<Value>* ValQue;
stack<bool> ifbools;
map<string, Value> TempsResults;
bool IdentList(istream& in, int& line);


//Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream& in, int& line)
{
	bool f1, f2;
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == PROGRAM) {
		tok = Parser::GetNextToken(in, line);
		if (tok.GetToken() == IDENT) {

			tok = Parser::GetNextToken(in, line);
			if (tok.GetToken() == SEMICOL) {
				f1 = DeclBlock(in, line);

				if(f1) {
					f2 = ProgBody(in, line);
					if(!f2)
					{
						ParseError(line, "Incorrect Program Body.");
						return false;
					}

					return true;//Successful Parsing is completed
				}
				else
				{
					ParseError(line, "Incorrect Declaration Section.");
					return false;
				}
			}
			else
			{
				//Parser::PushBackToken(tok);
				ParseError(line-1, "Missing Semicolon.");
				return false;
			}
		}
		else
		{
			ParseError(line, "Missing Program Name.");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else if(tok.GetToken() == DONE && tok.GetLinenum() <= 1){
		ParseError(line, "Empty File");
		return true;
	}
	ParseError(line, "Missing PROGRAM.");
	return false;
}

bool ProgBody(istream& in, int& line){
	bool status;

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok.GetToken() == BEGIN) {

		status = Stmt(in, line);

		while(status)
		{
			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Statement.");
				return false;
			}

			status = Stmt(in, line);
		}

		tok = Parser::GetNextToken(in, line);
		if(tok == END )
		{
			return true;
		}
		else
		{
			ParseError(line, "Syntactic error in Program Body.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Program Body.");
		return false;
	}
}//End of ProgBody function


bool DeclBlock(istream& in, int& line) {
	bool status = false;
	LexItem tok;
	//cout << "in Decl" << endl;
	LexItem t = Parser::GetNextToken(in, line);

	if(t == VAR)
	{
		status = DeclStmt(in, line);
		while(status)
		{

			tok = Parser::GetNextToken(in, line);
			if(tok != SEMICOL)
			{
				line--;
				ParseError(line, "Missing semicolon in Declaration Statement.");
				return false;
			}
			status = DeclStmt(in, line);
		}

		tok = Parser::GetNextToken(in, line);
		if(tok == BEGIN )
		{
			Parser::PushBackToken(tok);
			return true;
		}
		else
		{
			ParseError(line, "Syntactic error in Declaration Block.");
			return false;
		}
	}
	else
	{
		ParseError(line, "Non-recognizable Declaration Block.");
		return false;
	}

}//end of DeclBlock function

bool DeclStmt(istream& in, int& line)
{
	LexItem t;
	bool status = IdentList(in, line);

	if (!status)
	{
		ParseError(line, "Incorrect variable in Declaration Statement.");
		return status;
	}
	t = Parser::GetNextToken(in, line);
	if(t == COLON)
	{
		t = Parser::GetNextToken(in, line);
		if(t == INTEGER || t == REAL || t == STRING)
		{
		    while (!VarNameQue.empty()){
                SymTable[VarNameQue.front()] = t.GetToken();
                VarNameQue.pop();
            }
			return true;
		}
		else
		{
			ParseError(line, "Incorrect Declaration Type.");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(t);
		return false;
	}

}//End of DeclStmt

//IdList:= IDENT {,IDENT}
bool IdentList(istream& in, int& line) {
	bool status = false;
	string identstr;

	LexItem tok = Parser::GetNextToken(in, line);

	if(tok == IDENT)
	{
		//set IDENT lexeme to the type tok value

		identstr = tok.GetLexeme();
		VarNameQue.push(identstr);
		if (!(defVar.find(identstr)->second))
		{
			defVar[identstr] = true;
		}
		else
		{
			ParseError(line, "Variable Redefinition");
			return false;
		}
	}
	else
	{
		Parser::PushBackToken(tok);
		return true;
	}

	tok = Parser::GetNextToken(in, line);

	if (tok == COMMA) {
		status = IdentList(in, line);
	}
	else if(tok == COLON)
	{
		Parser::PushBackToken(tok);
		return true;
	}
	else {
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return status;

}//End of IdentList


//Stmt is either a WriteLnStmt, ForepeatStmt, IfStmt, or AssigStmt
//Stmt = AssigStmt | IfStmt | WriteStmt | ForStmt
bool Stmt(istream& in, int& line) {
	bool status;

	LexItem t = Parser::GetNextToken(in, line);

	switch( t.GetToken() ) {

	case WRITELN:
		status = WriteLnStmt(in, line);

		break;

	case IF:
		status = IfStmt(in, line);
		break;

	case IDENT:
		Parser::PushBackToken(t);
        status = AssignStmt(in, line);

		break;
/*
doesnt exist anymore
	case FOR:
		status = ForStmt(in, line);

		break;
*/

	default:
		Parser::PushBackToken(t);
		return false;
	}

	return status;
}//End of Stmt


//WriteStmt:= wi, ExpreList
bool WriteLnStmt(istream& in, int& line) {
	LexItem t;

    /*create an empty queue of Value objects.*/
    ValQue = new queue<Value>;
	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

	bool ex = ExprList(in, line);

	if( !ex ) {
        queue<Value> empty;
        swap( *ValQue, empty );
        ParseError(line, "Missing expression after WriteLn");
        return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}
	if(ifbools.empty() || ifbools.top() == true)
    {
        while (!(*ValQue).empty()){
            cout << (*ValQue).front();
            ValQue->pop();
        }
        cout << endl;
    }
    else
    {
        queue<Value> empty;
        swap( *ValQue, empty );
    }
	return ex;
}

//IfStmt:= if (Expr) then Stm} [Else Stmt]
bool IfStmt(istream& in, int& line) {
	bool ex=false, status ;
	LexItem t;

	t = Parser::GetNextToken(in, line);
	if( t != LPAREN ) {

		ParseError(line, "Missing Left Parenthesis");
		return false;
	}

    Value val = Value();
	ex = LogicExpr(in, line, val);
	if( !ex )
    {
		ParseError(line, "Missing if statement Logic Expression");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if(t != RPAREN ) {

		ParseError(line, "Missing Right Parenthesis");
		return false;
	}

	t = Parser::GetNextToken(in, line);
	if(t != THEN)
	{
		ParseError(line, "If-Stmt Syntax Error");
		return false;
	}
	bool ontop = ifbools.empty() || (ifbools.top() == true);
	if(ifbools.empty() || ontop)
    {
        ifbools.push(val.GetBool());
    }
	status = Stmt(in, line);
    if(ontop)
    {
        ifbools.pop();
    }
	if(!status)
	{
		ParseError(line, "Missing Statement for If-Stmt Then-Part");
		return false;
	}
	t = Parser::GetNextToken(in, line);
	if( t == ELSE ) {
        if(ifbools.empty() || ontop)
            ifbools.push(!val.GetBool());
        status = Stmt(in, line);
        if(ontop)
            ifbools.pop();
		if(!status)
		{
			ParseError(line, "Missing Statement for If-Stmt Else-Part");
			return false;
		}
		//cout << "in IFStmt status of Stmt true" << endl;
		return true;
	}
	Parser::PushBackToken(t);
	return true;
}//End of IfStmt function

/*  For stmt - deprecated
bool ForStmt(istream& in, int& line)
}*/
//Var:= ident
bool Var(istream& in, int& line, LexItem & idtok)
{
	//called only from the AssignStmt function
	string identstr;

	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == IDENT){
		identstr = tok.GetLexeme();

		if (!(defVar.find(identstr)->second))
		{
			ParseError(line, "Undeclared Variable");
			return false;
		}
		idtok = tok;
		return true;
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	return false;
}//End of Var

//AssignStmt:= Var = Expr
bool AssignStmt(istream& in, int& line) {

	bool varstatus = false, status = false;
	LexItem t;

	varstatus = Var( in, line, t);

    string varname = t.GetLexeme();

	if (varstatus){
		t = Parser::GetNextToken(in, line);

        Value v = Value();
		if (t == ASSOP){
			status = Expr(in, line, v );
			if(!status) {
				ParseError(line, "Missing Expression in Assignment Statment");
				return status;
			}

			if(v.IsString() && SymTable[varname] != Token::STRING)
            {
                ParseError(line,"Illegal Assignment Operation");
                return false;
            }

			if(v.GetType() == ValType::VINT && SymTable[varname] == Token::REAL)
            {
                v = Value(static_cast<float>(v.GetInt()));
            }
            else if(v.IsReal() && SymTable[varname] == Token::INTEGER)
            {
                v = Value(static_cast<int>(v.GetReal()));
            }
            if(ifbools.empty() || ifbools.top() == true)
                TempsResults[varname] = v;
		}
		else if(t.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			return false;
		}
		else {
			ParseError(line, "Missing Assignment Operator");
			return false;
		}
	}
	else {
		ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
		return false;
	}
	return status;
}

//ExprList:= Expr {,Expr}
bool ExprList(istream& in, int& line) {
	bool status = false;

	Value value = Value();
	status = Expr(in, line, value);
	if(!status){
		ParseError(line, "Missing Expression");
		return false;
	}
	ValQue->push(value);
	LexItem tok = Parser::GetNextToken(in, line);

	if (tok == COMMA) {

		status = ExprList(in, line);
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	else{
		Parser::PushBackToken(tok);
		return true;
	}
	return status;
}

//Expr:= Term {(+|-) Term}
bool Expr(istream& in, int& line, Value& retVal) {
    Value val1, val2;
	bool t1 = Term(in, line, val1);
	LexItem tok;

	if( !t1 ) {
		return false;
	}
    retVal = val1;
    if(retVal.IsErr())
    {
        ParseError(line,"Undefined Variable");
        return false;
    }
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS )
	{
		t1 = Term(in, line, val2);
		if( !t1 )
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        if(tok == Token::PLUS)
        {
            Value debug1 = retVal;
            Value debug2 = val2;
            retVal = retVal + val2;
            if(retVal.IsErr())
            {
                ParseError(line,"Uh var wrong for Expr + ");
                return false;
            }
        }
        else if(tok == Token::MINUS)
        {
            retVal = retVal - val2;
            if(retVal.IsErr())
            {
                ParseError(line,"Uh var wrong for Expr - ");
                return false;
            }
        }
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

//Term:= SFactor {(*|/) SFactor}
bool Term(istream& in, int& line, Value& retVal) {
    Value val1, val2;
	bool t1 = SFactor(in, line, val1);
	LexItem tok;

	if( !t1 ) {
		return false;
	}
    retVal = val1;
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		t1 = SFactor(in, line, val2);

		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

        if(tok == Token::MULT)
        {
            retVal = retVal * val2;
            if(retVal.IsErr())
            {

                ParseError(line,"Uh var wrong for Term");
                return false;
            }
        }
        else if(tok == Token::DIV)
        {
            retVal = retVal / val2;
            if(retVal.IsErr())
            {
                ParseError(line,"Run-TIme Error-Illegal Division by Zero");
                return false;
            }
        }
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
	}
	Parser::PushBackToken(tok);
	return true;
}

//SFactor = Sign Factor | Factor
bool SFactor(istream& in, int& line, Value& retVal)
{
	LexItem t = Parser::GetNextToken(in, line);
	bool status;
	int sign = 0;
	if(t == MINUS )
	{
		sign = -1;
	}
	else if(t == PLUS)
	{
		sign = 1;
	}
	else
		Parser::PushBackToken(t);
	status = Factor(in, line, sign, retVal);
	return status;
}
//  and below
//LogicExpr = Expr (== | <) Expr
bool LogicExpr(istream& in, int& line, Value& retVal)
{

    Value val1,val2;
	bool t1 = Expr(in, line, val1);
	LexItem tok;

	if( !t1 ) {
		return false;
	}

	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	if ( tok == GTHAN  || tok == EQUAL  || tok == LTHAN)
	{
		t1 = Expr(in, line, val2);
		if( !t1 )
		{
			ParseError(line, "Missing expression after relational operator");
			return false;
		}
		if(tok == GTHAN)
        {
            retVal = val1 > val2;
        }
        else if(tok == EQUAL)
            retVal = val1 == val2;
        else if(tok == LTHAN)
            retVal = val1 < val2;
        if(retVal.IsErr())
        {
            ParseError(line,"Run-Time Error-Illegal Mixed Type Operands for a Logic Expression");
            return false;
        }
		return true;
	}
	Parser::PushBackToken(tok);
	return true;
}

//Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream& in, int& line, int sign, Value& retVal) {
	LexItem tok = Parser::GetNextToken(in, line);

	if( tok == IDENT ) {
		string lexeme = tok.GetLexeme();
		if (!(defVar.find(lexeme)->second))
		{
			ParseError(line, "Using Undefined Variable");
			return false;
		}
		retVal = TempsResults[lexeme];
		if(sign == -1)
        {
            if(retVal.IsReal())
            {
                retVal = Value(-retVal.GetReal());
            }
            if(retVal.IsInt())
            {
                retVal = Value(-retVal.GetInt());
            }
        }
		return true;
	}
	else if( tok == ICONST ) {
            int num = stoi(tok.GetLexeme());
            if(sign == -1)
            {
                num *= -1;
            }
            retVal = Value(num);
		return true;
	}
	else if( tok == SCONST ) {
        if(sign == 1)
        {
            ParseError(line, "Illegal Operand Type for Sign Operator");
            return false;
        }
        retVal = Value(tok.GetLexeme());
		return true;
	}
	else if( tok == RCONST ) {
	    float num = stof(tok.GetLexeme());
            if(sign == -1)
            {
               num *= -1;
            }
            retVal = Value(num);
		return true;
	}
	else if( tok == LPAREN ) {
		bool ex = Expr(in, line, retVal);
		if( !ex ) {
			ParseError(line, "Missing expression after (");
			return false;
		}
		if( Parser::GetNextToken(in, line) == RPAREN )
			return ex;
		else
		{
			Parser::PushBackToken(tok);
			ParseError(line, "Missing ) after expression");
			return false;
		}
	}
	else if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}

	return false;
}
