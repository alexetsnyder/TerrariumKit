#pragma once

#include "ICamera.h"
#include "ICommand.h"

namespace CmdTK
{
	class ZoomCameraCommand : public ICommand
	{
		public:
			ZoomCameraCommand(ICamera* camera, float yOffset)
			{
				_camera = camera;
				_yOffset = yOffset;
			}

			void execute() override
			{
				_camera->zoom(_yOffset);
			}

		private:
			float _yOffset;
			ICamera* _camera;
	};
}