//
//

#include "WaitAction.h"
#include "io/timer.h"
#include "parse/parselo.h"

namespace actions {
namespace types {

WaitAction::~WaitAction() = default;
ActionResult WaitAction::execute(actions::ProgramLocals& locals) const
{
	if (!timestamp_valid(locals.waitTimestamp)) {
		locals.waitTimestamp = timestamp(_waitTime);
	}
	if (!timestamp_elapsed(locals.waitTimestamp)) {
		// Wait until the timestamp is elapsed
		return ActionResult::Wait;
	}
	// Timestamp is elapsed. The timestamp is elapsed so that timestamp_valid above returns false for the next wait
	// action
	locals.waitTimestamp = 0;
	return ActionResult::Finished;
}
void WaitAction::parseValues(const flagset<ProgramParseFlags>&)
{
	float wait_seconds;
	stuff_float(&wait_seconds);

	if (wait_seconds <= 0.0f) {
		error_display(0, "Invalid wait time %f! Must be greater than zero.", wait_seconds);
		wait_seconds = 0.001f; // Set it to some small value
	}

	_waitTime = (int)(wait_seconds * TIMESTAMP_FREQUENCY);
}
WaitAction* WaitAction::clone() const { return new WaitAction(*this); }

} // namespace types
} // namespace actions
