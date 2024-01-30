#pragma once

namespace CmdTK
{
	class ICommand
	{
		public:
			virtual void execute() = 0;
			virtual ~ICommand() {};
	};
}
