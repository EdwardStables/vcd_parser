#include "VCDParserBaseListener.h"
#include "antlr4-runtime.h"

class VCDListener : public antlrvcdp::VCDParserBaseListener {
  virtual void enterValue_change_dump_definitions(antlrvcdp::VCDParser::Value_change_dump_definitionsContext*) override;
  virtual void enterDeclaration_command(antlrvcdp::VCDParser::Declaration_commandContext*) override;
};