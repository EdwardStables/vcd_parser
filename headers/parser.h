#pragma once

#include "vcdListener.h"
#include "vcdBaseListener.h"
#include "antlr4-runtime.h"
#include "store.h"

Store* build_store(std::string file);

class vcdListener : public antlrvcdp::vcdBaseListener {
  Store* store;  
  virtual void enterTime_update(antlrvcdp::vcdParser::Time_updateContext*) override;
  virtual void enterScalar_change(antlrvcdp::vcdParser::Scalar_changeContext*) override;
  virtual void enterVector_change(antlrvcdp::vcdParser::Vector_changeContext*) override;
  virtual void enterVar(antlrvcdp::vcdParser::VarContext*) override;
  virtual void enterScope(antlrvcdp::vcdParser::ScopeContext*) override;
  virtual void enterUpScope(antlrvcdp::vcdParser::UpScopeContext*) override;
public:
  vcdListener(Store* store);
};