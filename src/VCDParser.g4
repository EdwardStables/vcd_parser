parser grammar VCDParser;

options {
	tokenVocab = VCDLexer;
}


value_change_dump_definitions: declaration_command* simulation_command*;
declaration_command: declaration_keyword Text End;
declaration_keyword: Comment
                    | Date
                    | EndDef
                    | Scope
                    | Timescale
                    | UpScope
                    | Var
                    | Version;

simulation_command:
    simulation_keyword value_change* End
  | Comment Text End
  | simulation_time
  | value_change;
    
simulation_keyword: Dumpall
                  | Dumpoff
                  | Dumpon
                  | Dumpvars;

simulation_time: Hash DecimalNumber;

value_change: scalar_value_change | vector_value_change; 	    

scalar_value_change: Value IdentifierCode;
vector_value_change:
    BinaryId BinaryNumber IdentifierCode |
    RealId RealNumber IdentifierCode;
