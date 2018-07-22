//
//

#include "Program.h"
#include "Action.h"
#include "ProgramManager.h"
#include "ship/ship.h"

#include "actions/types/ParticleEffectAction.h"
#include "actions/types/SetDirectionAction.h"
#include "actions/types/SetPositionAction.h"
#include "actions/types/WaitAction.h"

namespace {
using namespace actions;

std::unique_ptr<Action> parse_action(const flagset<ProgramParseFlags>& parse_flags)
{
	std::unique_ptr<Action> act;

	if (optional_string("+Wait:")) {
		act.reset(new types::WaitAction());
	} else if (optional_string("+Set Position:")) {
		act.reset(new types::SetPositionAction());
	} else if (optional_string("+Set Direction:")) {
		act.reset(new types::SetDirectionAction());
	} else if (optional_string("+Start Particle Effect:")) {
		act.reset(new types::ParticleEffectAction());
	}

	if (act) {
		act->parseValues(parse_flags);
	}

	return act;
}
} // namespace

namespace actions {

ProgramInstance::ProgramInstance(const actions::Program* program) : _program(program) {}

ProgramState ProgramInstance::step()
{
	if (!_locals.host.IsValid()) {
		return ProgramState::Done;
	}

	auto& actions = _program->getActions();

	if (_currentInstruction >= actions.size()) {
		// End of program reached
		return ProgramState::Done;
	}

	for (; _currentInstruction < actions.size(); ++_currentInstruction) {
		auto& action = actions[_currentInstruction];

		auto result = action->execute(_locals);

		if (result == ActionResult::Errored) {
			return ProgramState::Errored;
		}

		if (result == ActionResult::Wait) {
			// Action is not done yet, call again next frame
			return ProgramState::Running;
		}

		// Action finished, run next action in program
	}

	// End of program reached, we are done now
	return ProgramState::Done;
}
ProgramLocals& ProgramInstance::locals() { return _locals; }
const ProgramLocals& ProgramInstance::locals() const { return _locals; }

const SCP_vector<std::unique_ptr<actions::Action>>& Program::getActions() const { return _actions; }

Program::Program() = default;

ProgramInstance Program::newInstance() const { return ProgramInstance(this); }

void Program::parseActions(const flagset<ProgramParseFlags>& parse_flags)
{
	while (true) {
		auto action = parse_action(parse_flags);

		if (!action) {
			break;
		}

		_actions.push_back(std::move(action));
	}
}

Program::Program(const Program& other) { *this = other; }
Program& Program::operator=(const Program& other)
{
	_actions.clear();
	_actions.reserve(other._actions.size());

	for (auto& action : other._actions) {
		_actions.emplace_back(action->clone());
	}

	return *this;
}

Program& Program::operator=(Program&& other) noexcept = default;
Program::Program(Program&& other) noexcept            = default;

const SCP_vector<actions::Program>& ProgramSet::getPrograms() const {
	return _programs;
}

void ProgramSet::start(object* objp, ship_subsys* subsys) const
{
	for (auto& prog : _programs) {
		auto instance = prog.newInstance();

		instance.locals().host          = object_h(objp);
		instance.locals().hostSubsystem = subsys;

		ProgramManager::instance()->addInstance(instance);
	}
}

ProgramSet ProgramSet::parseProgramSet(const char* tag, const flagset<ProgramParseFlags>& parse_flags)
{
	if (!check_for_string(tag)) {
		return ProgramSet();
	}

	ProgramSet set;

	while (optional_string(tag)) {
		Program p;
		p.parseActions(parse_flags);

		set._programs.push_back(std::move(p));
	}

	return set;
}
} // namespace actions
