#pragma once

#include "Enums.h"
#include "ICommand.h"
#include "IGameActor.h"

namespace CmdTK
{
	class MoveCommand : public ICommand
	{
		public:
			MoveCommand(IGameActor* gameActor, InputDirection direction)
			{
				_gameActor = gameActor;
				_inputDirection = direction;
			}

			void execute() override
			{
				_gameActor->move(_inputDirection);
			}

		private:
			IGameActor* _gameActor;
			InputDirection _inputDirection;
	};
}