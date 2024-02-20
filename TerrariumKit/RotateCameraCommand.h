#pragma once

#include "ICamera.h"
#include "ICommand.h"

namespace CmdTK
{
	class RotateCameraCommand : public ICommand
	{
		public:
			RotateCameraCommand(ICamera* camera, float xRel, float yRel)
			{
				_camera = camera;
				_xRel = xRel;
				_yRel = yRel;
			}

			void execute() override
			{
				_camera->transform().rotate(_xRel, _yRel);
			}

		private:
			float _xRel;
			float _yRel;
			ICamera* _camera;
	};
}