parser grammar VCDParser;

options {
	tokenVocab = VCDLexer;
}


value_change_dump_definitions: declaration_command+ simulation_command+;
declaration_command: declaration_keyword text End;
text: AsciiChar+; 
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
  | Comment text End
  | simulation_time
  | value_change;
    
simulation_keyword: Dumpall
                  | Dumpoff
                  | Dumpon
                  | Dumpvars;

simulation_time: Hash DecimalNumber;

value_change: scalar_value_change | vector_value_change; 	    

scalar_value_change: Value identifier_code;
vector_value_change:
    BinaryId BinaryNumber identifier_code|
    RealId RealNumber identifier_code;
identifier_code: AsciiChar+; 
