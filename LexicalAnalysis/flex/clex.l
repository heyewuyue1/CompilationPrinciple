%{
#include <stdio.h>
#include <string.h>
enum TokenType
{
    Identifier,  // 标志符
    ConstInt,    // 整数常量
    ConstFloat,  // 浮点常量
    ConstChar,   // 字符常量
    ConstString, // 字符串常量
    KeyWord,     // 关键字
    Operator,    // 运算符
    Seperator,   // 分隔符
    Error        // 错误标记
};

const char * tokenTypeStr[9] = {
    "Identifier",
    "Int",
    "Float",
    "Char",
    "String",
    "KeyWord",
    "Operator",
    "Seperator",
    "Error"
};

unsigned int line = 1;
unsigned int tokenNum[9]={0};
unsigned int charNum = 0;
unsigned int col = 1;
%}

keyword char|short|int|long|signed|unsigned|float|double|struct|union|enum|void|for|do|while|break|continue|if|else|goto|switch|case|default|return|auto|extern|register|static|typedef|const|sizeof|volatile

digit [0-9]
letter [a-zA-Z]

identifier (_|{letter})(_|{letter}|{digit})*

lParent [\()]
rParent [\)]]
lBrace [\{]
rBrace [\}]
comma [,]
colon [:]
semicolon [;]
lBracket [\[]
rBracket [\]]
equal (==)
assign [=]
decrease (--)
structPointer (->)
minusAssign (-=)
minus [-]

increase (\+\+)
plusAssign (\+=)
plus [\+]

multiplyAssign (\*=)
star [\*]

comment (\/\/)
divisionAssign (\/=)
division [\/]
multiComment (\/\*)

modAssign (%=)
mod [%]

notEqual (!=)
logicNot [!]

digitNot [~]

logicAnd (&&)
andAssign (&=)
digitAnd [&]

logicOr (\|\|)
orAssign (\|=)
digitOr [\|]

xorAssign (\^=)
xor [\^]

question [\?]

leftShiftAssign (<<=)
leftShift (<<)
lessEqual (<=)
less [<]

rightShiftAssign (>>=)
rightShift (>>)
greaterEqual (>=)
greater [>]

dot [\.]

xDigit [0-9a-fA-f]
oDigit [0-7]
bDigit [01]
xPreffix (0x|0X)


intSuffix u|l|ul|uL|Ul|UL|lu|lU|Lu|LU|LL|ll|ull|uLL|Ull|ULL|llu|llU|LLu|LLU
oct 0[0-7]*{intSuffix}?
hexFloat 0(x|X){xDigit}+(\.{xDigit}+)?(p|P)[-,+]?[0-9]+(f|F|l|L)?
hexInt 0(x|X){xDigit}+{intSuffix}?
bin 0(b|B)(0|1)+{intSuffix}?
decInt [1-9][0-9]*{intSuffix}?
decFloat [0-9]*((\.[0-9]*)|((e|E)[-,+]?[0-9]*)|(\.[0-9]*)((e|E)[-,+]?[0-9]*))(f|F|l|L)?

errorNum (.|{digit})(^({oct}|{hexFloat}|{hexInt}|{bin}|{decInt}|{decFloat}))

constChar '[^']*'
constString \"(.|\ |\t)*\"

newLine [\n]
blank [ \n\t]


%%
{keyword} {
    if (!strcmp(yytext, "sizeof")) {
        tokenNum[Operator]++;
        printf("<Operator, %s>:%d:%d\n", yytext, line, col);
    } else {
        tokenNum[KeyWord]++;
        printf("<KeyWord, %s>:%d:%d\n", yytext, line, col);
    }
    charNum += yyleng;
    col += yyleng;
}

{identifier} {
    tokenNum[Identifier]++;
    printf("<Identifier, %s>:%d:%d\n", yytext, line, col);
    charNum += yyleng;
    col += yyleng;
}

({dot}|{lBrace}|{rBrace}|{lParent}|{rParent}|{comma}|{colon}|{semicolon}) {
    tokenNum[Seperator]++;
    printf("<Seperator, %s>:%d:%d\n", yytext, line, col);
    charNum += yyleng;
    col += yyleng;
}

({question}|{lBracket}|{rBracket}|{equal}|{assign}|{decrease}|{structPointer}|{minusAssign}|{minus}|{increase}|{plusAssign}|{plus}|{multiplyAssign}|{star}|{divisionAssign}|{division}|{modAssign}|{mod}|{logicNot}|{notEqual}|{digitNot}|{logicAnd}|{andAssign}|{digitAnd}|{logicOr}|{orAssign}|{digitOr}|{xor}|{xorAssign}|{leftShiftAssign}|{leftShift}|{lessEqual}|{less}|{rightShiftAssign}|{rightShift}|{greaterEqual}|{greater}) {
    tokenNum[Operator]++;
    printf("<Operator, %s>:%d:%d\n", yytext, line, col);
    charNum += yyleng;
    col += yyleng;
}

({hexFloat}|{decFloat}) {
    tokenNum[ConstFloat]++;
    printf("<Float, %s>:%d:%d\n", yytext, line, col);
    charNum += yyleng;
    col += yyleng;
}

({oct}|{hexInt}|{bin}|{decInt}) {
    tokenNum[ConstInt]++;
    printf("<Int, %s>:%d:%d\n", yytext, line, col);
    charNum += yyleng;
    col += yyleng;
}

{errorNum} {
    tokenNum[Error]++;
    printf("<Error, invalid number: %s>:%d:%d\n", yytext, line, col);
    charNum += yyleng;
    col += yyleng;
}

{constChar} {
    if (yyleng > 4) {
        printf("<Error, multi-character character constant: %s>:%d:%d\n", yytext, line, col);
        tokenNum[Error]++;
    } else {
        printf("<Char, %s>:%d:%d\n", yytext, line, col);
        tokenNum[ConstChar]++;
    }
    charNum += yyleng;
    col += yyleng;
}

{constString} {
    printf("<String, %s>:%d:%d\n", yytext, line, col);
    tokenNum[ConstString]++;
    charNum += yyleng;
    col += yyleng;
}

{comment} {
    charNum += 2;
    char c = input();
    charNum++;
    while (c != '\n') {
        c = input();
        charNum++;
    }
    line++;
}

{multiComment} {
    charNum+=2;
    char c = input();
    charNum++;
    if (c == '\n'){
        col = 1;
        line++;
    }
        
    while (1) {
        while (c != '*'){
            c = input();
            if (c == EOF)
                break;
            charNum++;
            if (c == '\n'){
                col = 1;
                line++;
            }
        }
        c = input();
        if(c == EOF)
            break;
        charNum++;
        if (c == '\n'){
            col = 1;
            line++;
        }
        if (c == '/')
            break;
    }
}

{newLine} {
    col = 1;
    line++;
    charNum++;
}

{blank} {
    charNum += yyleng;
    col += yyleng;
}

%%
int main(int argc, char *argv[])
{
    yyin = fopen(argv[1], "r");
    yylex();
    int sum = 0;
    for(int i = 0; i < 9; i++) {
        sum += tokenNum[i];
    }
    printf("[Statistics]\n");
    printf("Total characters: %d\n", charNum);
    printf("Total lines: %d\n", line);
    printf("Total tokens: %d\n", sum);
    for(int i = 0; i < 9; i++)
    {
        printf("%s: %d\n", tokenTypeStr[i], tokenNum[i]);
    }
    return 0;
}


int yywrap() {

    return 1;
}