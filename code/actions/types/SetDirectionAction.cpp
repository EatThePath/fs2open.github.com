
#include "SetDirectionAction.h"
#include "math/vecmat.h"
#include "parse/parselo.h"

namespace actions {
namespace types {
SetDirectionAction::~SetDirectionAction() = default;

ActionResult SetDirectionAction::execute(ProgramLocals& locals) const
{
	locals.direction = _newPos;
	return ActionResult::Finished;
}

void SetDirectionAction::parseValues(const flagset<ProgramParseFlags>& /*parse_flags*/)
{
	stuff_parenthesized_vec3d(&_newPos);

	if (vm_vec_mag(&_newPos) < 0.1f) {
		error_display(
		    0, "Zero vector is not valid here! The vector will be normalized so the actual length does not matter.");
		_newPos       = vmd_zero_vector;
		_newPos.xyz.z = 1.0f;
	}

	vm_vec_normalize(&_newPos);
}

SetDirectionAction* SetDirectionAction::clone() const { return new SetDirectionAction(*this); }

} // namespace types
} // namespace actions
