#pragma once

#include "actions/Action.h"

namespace actions {
namespace types {

class SetDirectionAction : public Action {
	vec3d _newPos = vmd_zero_vector;

  public:
	~SetDirectionAction() override;

	ActionResult execute(ProgramLocals& locals) const override;

	void parseValues(const flagset<ProgramParseFlags>& parse_flags) override;

	SetDirectionAction* clone() const override;
};

} // namespace types
} // namespace actions
