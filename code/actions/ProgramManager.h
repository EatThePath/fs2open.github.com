#pragma once
//
//

#include "Program.h"
#include "globalincs/pstypes.h"

namespace actions {

class ProgramManager {
	SCP_vector<ProgramInstance> _runningPrograms;

	ProgramManager();
  public:
	void stepInstances();

	void addInstance(ProgramInstance instance);

	static ProgramManager* instance();
};

} // namespace actions
