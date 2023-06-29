lexer grammar VCDLexer;

tokens {
	DUMMY	
}

Return: 'return';
Continue: 'continue';

INT: Digit+;
Digit: [0-9];

ID: LETTER (LETTER | '0'..'9')*;
fragment LETTER : [a-zA-Z\u0080-\uFFFF];

LessThan: '<';
GreaterThan:  '>';
Equal: '=';
And: 'and';

Colon: ':';
Semicolon: ';';
Plus: '+';
Minus: '-';
Star: '*';
OpenPar: '(';
ClosePar: ')';
OpenCurly: '{';
CloseCurly: '}';
QuestionMark: '?';
Comma: ',';
Dollar: '$';
Ampersand: '&';
 
String: '"' .*? '"';
Foo: {canTestFoo()}? 'foo' {isItFoo()}? { myFooLexerAction(); };
Bar: 'bar' {isItBar()}? { myBarLexerAction(); };
Any: Foo Dot Bar? DotDot Baz;

Comment : '#' ~[\r\n]* '\r'? '\n';
WS: [ \t\r\n]+;

fragment Baz: 'Baz';

mode Mode1;
Dot: '.';

mode Mode2;
DotDot: '..';