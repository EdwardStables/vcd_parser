parser grammar VCDParser;

options {
	tokenVocab = VCDLexer;
}

stat: expr Equal expr Semicolon
    | expr Semicolon
;

expr: expr Star expr
    | expr Plus expr
    | OpenPar expr ClosePar
    | <assoc = right> expr QuestionMark expr Colon expr
    | <assoc = right> expr Equal expr
    | identifier = id
    | flowControl
    | INT
    | String
;

flowControl:
	Return expr # Return
	| Continue # Continue
;

id: ID;
array : OpenCurly el += INT (Comma el += INT)* CloseCurly;
idarray : OpenCurly element += id (Comma element += id)* CloseCurly;
any: t = .;