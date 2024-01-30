#pragma once

#include "Enums.h"
#include "ICommand.h"
#include "IGameActor.h"

namespace CmdTK
{
	class moveCommand : public ICommand<IGameActor>
	{
		public:
			moveCommand(InputDirection direction, double deltaTime)
			{
				_inputDirection = direction;
				_deltaTime = deltaTime;
			}

			void execute(IGameActor& gameActor) override
			{
				gameActor.move(_inputDirection, _deltaTime);
			}

		private:
			double _deltaTime;
			InputDirection _inputDirection;
	};
}