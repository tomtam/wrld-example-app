//  Copyright (c) 2014 eeGeo. All rights reserved.

#pragma once

#include "Types.h"
#include "NativeToUiMessageBus.h"
#include "ICompassModel.h"
#include "ICallback.h"
#include "CompassModeChangedMessage.h"

namespace ExampleApp
{
	namespace Compass
	{
		class CompassModeObserver : private Eegeo::NonCopyable
		{
		private:
			ICompassModel& m_model;
			ExampleApp::ExampleAppMessaging::NativeToUiMessageBus& m_nativeToUiMessageBus;
			Eegeo::Helpers::TCallback0<CompassModeObserver> m_callback;

			void OnGpsModeChanged();

		public:
			CompassModeObserver(ICompassModel& model,
			                    ExampleApp::ExampleAppMessaging::NativeToUiMessageBus& nativeToUiMessageBus);

			~CompassModeObserver();
		};
	}
}