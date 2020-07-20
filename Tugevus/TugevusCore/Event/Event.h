#pragma once

namespace TUGEV {

	enum  class InputCategory {
		MOUSE_CLICKR,
		MOUSE_RELEASER,
		MOUSE_CLICKL,
		MOUSE_RELEASEL,
		KEY_PRESS_DOWN

	};


	enum  EventType{
		InputCategory ,
		OTHER

	};


	class Event {
	public:
		Event() {};
	}
	;
}
