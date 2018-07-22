#pragma once

#include "actions/Action.h"
#include "particle/ParticleManager.h"

namespace actions {
namespace types {

class ParticleEffectAction : public Action {
	particle::ParticleEffectHandle _effectHandle;

  public:
	~ParticleEffectAction() override;

	ActionResult execute(ProgramLocals& locals) const override;

	void parseValues(const flagset<ProgramParseFlags>& parse_flags) override;

	ParticleEffectAction* clone() const override;
};

} // namespace types
} // namespace actions
