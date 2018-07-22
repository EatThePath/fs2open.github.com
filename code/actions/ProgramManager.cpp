//
//

#include "ProgramManager.h"
#include "tracing/tracing.h"

namespace actions {

ProgramManager::ProgramManager() = default;

ProgramManager* actions::ProgramManager::instance()
{
	static ProgramManager manager;
	return &manager;
}
void ProgramManager::stepInstances()
{
	TRACE_SCOPE(tracing::ProgramsStepAll);

	for (auto prog_iter = std::begin(_runningPrograms); prog_iter != std::end(_runningPrograms);) {
		TRACE_SCOPE(tracing::ProgramStepOne);

		auto res = prog_iter->step();

		if (res == ProgramState::Done || res == ProgramState::Errored) {
			// if we're sitting on the very last source, popping-back will invalidate the iterator!
			if (std::next(prog_iter) == std::end(_runningPrograms)) {
				_runningPrograms.pop_back();
				break;
			}

			*prog_iter = _runningPrograms.back();
			_runningPrograms.pop_back();
			continue;
		}

		// prog_iter is only incremented here as elements would be skipped in the case that a source needs to be removed
		++prog_iter;
	}
}
void ProgramManager::addInstance(ProgramInstance instance) { _runningPrograms.push_back(instance); }

} // namespace actions
