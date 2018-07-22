
#include "ParticleEffectAction.h"
#include "math/vecmat.h"
#include "parse/parselo.h"
#include "ship/ship.h"

namespace actions {
namespace types {
ParticleEffectAction::~ParticleEffectAction() = default;

ActionResult ParticleEffectAction::execute(ProgramLocals& locals) const
{
	if (!_effectHandle.isValid()) {
		// In case the parsing code failed
		return ActionResult::Finished;
	}

	using namespace particle;

	auto source = ParticleManager::get()->createSource(_effectHandle);

	vec3d local_pos;
	if (locals.hostSubsystem != nullptr) {
		get_local_subsystem_pos(&local_pos, locals.host.objp, locals.hostSubsystem);
	} else {
		local_pos = vmd_zero_vector;
	}

	vm_vec_add2(&local_pos, &locals.position);

	source.moveToObject(locals.host.objp, &local_pos);
	source.setOrientationFromNormalizedVec(&locals.direction, true);

	source.finish();

	return ActionResult::Finished;
}

void ParticleEffectAction::parseValues(const flagset<ProgramParseFlags>& parse_flags)
{
	if (!parse_flags[ProgramParseFlags::HasObject]) {
		error_display(1, "The particle effect action requires a host object but in this context none is available!");
	}
	_effectHandle = particle::util::parseEffect();
}

ParticleEffectAction* ParticleEffectAction::clone() const { return new ParticleEffectAction(*this); }

} // namespace types
} // namespace actions
