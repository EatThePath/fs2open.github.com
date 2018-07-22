#pragma once

#include "actions/Action.h"

namespace actions {
namespace types {

class WaitAction : public Action {
	int _waitTime = -1;

  public:
	~WaitAction() override;

	ActionResult execute(ProgramLocals& locals) const override;

	void parseValues(const flagset<ProgramParseFlags>& parse_flags) override;

	WaitAction* clone() const override;
};

} // namespace types
} // namespace actions
