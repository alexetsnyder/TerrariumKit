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
				camera_ = camera;
				xRel_ = xRel;
				yRel_ = yRel;
			}

			void execute() override
			{
				camera_->transform().rotate(xRel_, yRel_);
			}

		private:
			float xRel_;
			float yRel_;
			ICamera* camera_;
	};
}