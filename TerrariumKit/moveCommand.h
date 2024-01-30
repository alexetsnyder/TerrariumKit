#pragma once

#include "Enums.h"
#include "ICommand.h"
#include "IGameActor.h"

namespace CmdTK
{
	class moveCommand : public ICommand
	{
		public:
			moveCommand(IGameActor* gameActor, InputDirection direction, double deltaTime)
			{
				_gameActor = gameActor;
				_inputDirection = direction;
				_deltaTime = deltaTime;
			}

			void execute() override
			{
				_gameActor->move(_inputDirection, _deltaTime);
			}

		private:
			double _deltaTime;
			IGameActor* _gameActor;
			InputDirection _inputDirection;
	};
}