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
  virtual void enterTime_update(antlrvcdp::vcdParser::Time_updateContext*) override;
  virtual void enterScalar_change(antlrvcdp::vcdParser::Scalar_changeContext*) override;
  virtual void enterVector_change(antlrvcdp::vcdParser::Vector_changeContext*) override;
  virtual void enterVar(antlrvcdp::vcdParser::VarContext*) override;
  virtual void enterScope(antlrvcdp::vcdParser::ScopeContext*) override;
  virtual void enterUpScope(antlrvcdp::vcdParser::UpScopeContext*) override;
public:
  vcdListener(Store* store);
};