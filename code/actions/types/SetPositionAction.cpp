
#include "SetPositionAction.h"
#include "parse/parselo.h"

namespace actions {
namespace types {
SetPositionAction::~SetPositionAction() = default;

ActionResult SetPositionAction::execute(ProgramLocals& locals) const
{
	locals.position = _newPos;
	return ActionResult::Finished;
}

void SetPositionAction::parseValues(const flagset<ProgramParseFlags>& /*parse_flags*/)
{
	stuff_parenthesized_vec3d(&_newPos);
}

SetPositionAction* SetPositionAction::clone() const { return new SetPositionAction(*this); }

} // namespace types
} // namespace actions
