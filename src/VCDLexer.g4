lexer grammar VCDLexer;

End:        '$end';
Comment:    '$comment';
Date:       '$date';
EndDef:     '$enddefinitions';
Scope:      '$scope';
Timescale:  '$timescale';
UpScope:    '$upscope';
Var:        '$var';
Version:    '$version';
Dumpall:    '$dumpall'; 
Dumpoff:    '$dumpoff'; 
Dumpon:     '$dumpon';  
Dumpvars:   '$dumpvars';  

Hash: '#';
DecimalNumber: [1-9][0-9]*;

Value: '0' | '1' | 'x' | 'X' | 'z' | 'Z';

BinaryId: 'b' | 'B';
RealId: 'r' | 'R';

BinaryNumber: ('0' | '1')+;
RealNumber: [0-9]+'.'[0-9]+ | [0-9]+'e'('+'|'-')[0-9]+;

Whitespace: [ \t\r\n]+ -> skip;
AsciiChar: [\u0000-\u00FF];