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
				camera_ = camera;
				yOffset_ = yOffset;
			}

			void execute() override
			{
				camera_->zoom(yOffset_);
			}

		private:
			float yOffset_;
			ICamera* camera_;
	};
}