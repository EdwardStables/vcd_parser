#include "vcdBaseListener.h"
#include "antlr4-runtime.h"

class vcdListener : public antlrvcdp::vcdBaseListener {
  virtual void enterValue_change_dump_definitions(antlrvcdp::vcdParser::Value_change_dump_definitionsContext*) override;
  virtual void enterDeclaration_command(antlrvcdp::vcdParser::Declaration_commandContext*) override;
  virtual void enterSimulation_command(antlrvcdp::vcdParser::Simulation_commandContext*) override;
  virtual void enterSimulation_directive(antlrvcdp::vcdParser::Simulation_directiveContext*) override;
};