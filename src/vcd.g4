grammar vcd;

value_change_dump_definitions: declaration_command+ simulation_command+ EOF;
declaration_command: comment | date | endDef | scope | timescale | upScope | var | version;

comment:    Comment;
date:       Date;
endDef:     EndDef;
scope:      Scope;
timescale:  Timescale;
upScope:    UpScope;
var:        Var;
version:    Version;

simulation_command:
    simulation_directive
  | comment
  | Simulation
  | value_change;

simulation_directive: simulation_keyword value_change* End;
simulation_keyword: Dumpall
                  | Dumpoff
                  | Dumpon
                  | Dumpvars;

End: '$end';

Comment:    '$comment' .*? End;
Date:       '$date' .*? End;
EndDef:     '$enddefinitions' .*? End;
Scope:      '$scope' .*? End;
Timescale:  '$timescale' .*? End;
UpScope:    '$upscope' .*? End;
Var:        '$var' .*? End;
Version:    '$version' .*? End;

Dumpall:    '$dumpall'; 
Dumpoff:    '$dumpoff'; 
Dumpon:     '$dumpon';  
Dumpvars:   '$dumpvars';  


value_change: scalar_change | vector_change; 	    

vector_change: VectorBinary | VectorReal;
scalar_change: ScalarChange;

ScalarChange: [01xXzZ] Identifier;
VectorBinary: [bB][01xXzR]+ ' ' Identifier;
VectorReal: [rR][0-9]+'.'[0-9]+ | [0-9]+'e'('+'|'-')[0-9]+ ' ' Identifier+;

Simulation: '#' DecimalNumber;
DecimalNumber: [1-9][0-9]* | '0';

Identifier: [\u0021-\u007e]+;
Whitespace: [ \t\r\n]+ -> skip;
