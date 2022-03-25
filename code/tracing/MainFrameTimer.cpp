
#include "MainFrameTimer.h"
#include "object/object.h"
#include "ship/ship.h"
#include "weapon/weapon.h"

namespace tracing {

MainFrameTimer::MainFrameTimer() : _out("profiling.csv") {
}
MainFrameTimer::~MainFrameTimer() {
	_out.close();
}
void MainFrameTimer::processEvent(const trace_event* event) {
	if (event->scope != &MainFrameScope || event->category != &MainFrame) {
		return;
	}

	switch(event->type) {
		case EventType::AsyncBegin:
			_begin_time = event->timestamp;
			break;
		case EventType::AsyncEnd:
		{
			auto end = event->timestamp;
			auto duration = event->timestamp - _begin_time;

			_out << end << ";" << duration << ";"<< Num_objects<<";"<<Num_weapons<<";" <<ship_get_num_ships()<<"\n"  ;
			break;
		}
		default:
			// Ignore everything else
			return;
	}
}

}
