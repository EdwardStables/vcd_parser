lexer grammar VCDLexer;

Text: [0-9a-zA-Z_ ];
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
IdentifierCode: [\u0000-\u00FF]+;

BinaryId: 'b' | 'B';
RealId: 'r' | 'R';

BinaryNumber: ('0' | '1')+;
RealNumber: [0-9]+'.'[0-9]+ | [0-9]+'e'('+'|'-')[0-9]+;

Whitespace: [ \t\r\n]+ -> skip;
