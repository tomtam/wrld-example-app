// Copyright eeGeo Ltd (2012-2016), All Rights Reserved

#pragma once

#include "ReflectionHelpers.h"
#include "WindowsNativeState.h"

namespace ExampleApp
{
    namespace AttractModeOverlay
    {
        namespace View
        {
            class AttractModeOverlayView
            {
            public:
                AttractModeOverlayView(WindowsNativeState& nativeState, AppModes::SdkModel::IAppModeModel& appModeModel);
                ~AttractModeOverlayView();

            private:
                void OnAppModeChanged();

                Helpers::ReflectionHelpers::Method<void> mDestroy;
                Helpers::ReflectionHelpers::Method<void> mOnAttractModeStart;
                Helpers::ReflectionHelpers::Method<void> mOnAttractModeStop;

                AppModes::SdkModel::IAppModeModel& m_appModeModel;
                Eegeo::Helpers::TCallback0<AttractModeOverlayView> m_appModeChangedCallback;

                WindowsNativeState& m_nativeState;

                gcroot<System::Type^> m_uiViewClass;
                gcroot<System::Object^> m_uiView;
            };
        }
    }
}
