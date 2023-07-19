#pragma once

#include "vcdListener.h"
#include "vcdBaseListener.h"
#include "antlr4-runtime.h"
#include "store.h"

class vcdListener : public antlrvcdp::vcdBaseListener {
  Store* store;  

  //virtual void enterValue_change_dump_definitions(antlrvcdp::vcdParser::Value_change_dump_definitionsContext*) override;
  //virtual void enterDeclaration_command(antlrvcdp::vcdParser::Declaration_commandContext*) override;
  //virtual void enterSimulation_command(antlrvcdp::vcdParser::Simulation_commandContext*) override;
  //virtual void enterSimulation_directive(antlrvcdp::vcdParser::Simulation_directiveContext*) override;
  virtual void enterScalar_change(antlrvcdp::vcdParser::Scalar_changeContext*) override;
  virtual void enterScope(antlrvcdp::vcdParser::ScopeContext*) override;
  virtual void enterUpScope(antlrvcdp::vcdParser::UpScopeContext*) override;
public:
  vcdListener(Store* store);
};