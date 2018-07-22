#pragma once
//
//

#include "Action.h"
#include "common.h"
#include "globalincs/pstypes.h"
#include <memory>

namespace actions {

class Program;
class ProgramSet;

enum class ProgramState {
	Running,
	Done,
	Errored
};

class ProgramInstance {
	size_t _currentInstruction = 0;
	ProgramLocals _locals;
	const Program* _program = nullptr;

  public:
	explicit ProgramInstance(const Program* program);

	ProgramState step();

	ProgramLocals& locals();
	const ProgramLocals& locals() const;
};

class Program {
	SCP_vector<std::unique_ptr<Action>> _actions;
  public:
	Program();

	Program(const Program& other);
	Program& operator=(const Program& other);

	Program(Program&& other) noexcept;
	Program& operator=(Program&& other) noexcept;

	const SCP_vector<std::unique_ptr<actions::Action>>& getActions() const;

	ProgramInstance newInstance() const;

	void parseActions(const flagset<ProgramParseFlags>& parse_flags);
};

class ProgramSet {
	SCP_vector<Program> _programs;
  public:
	const SCP_vector<actions::Program>& getPrograms() const;

	void start(object* objp, ship_subsys* subsys = nullptr) const;

	static ProgramSet parseProgramSet(const char* tag, const flagset<ProgramParseFlags>& parse_flags = {});
};

} // namespace actions
