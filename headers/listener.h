#include "VCDParserBaseListener.h"
#include "antlr4-runtime.h"

class VCDListener : public antlrvcdp::VCDParserBaseListener {

  virtual void enterDeclaration_command(antlrvcdp::VCDParser::Declaration_commandContext*) override;
};