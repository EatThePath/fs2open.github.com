#pragma once
//
//

#include "common.h"

namespace actions {

enum class ActionResult { Finished, Errored, Wait };

class Action {
  public:
	virtual ~Action() = 0;

	virtual ActionResult execute(ProgramLocals& locals) const = 0;

	virtual void parseValues(const flagset<ProgramParseFlags>& parse_flags) = 0;

	virtual Action* clone() const = 0;
};

} // namespace actions
