#pragma once

namespace CmdTK
{
	template<class T>
	class ICommand
	{
		public:
			virtual void execute(T& gameActor) = 0;
			virtual ~ICommand() {};
	};
}
