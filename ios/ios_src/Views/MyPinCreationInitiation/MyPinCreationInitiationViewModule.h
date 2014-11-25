// Copyright eeGeo Ltd (2012-2014), All Rights Reserved

#pragma once

#include "Types.h"
#include "MyPinCreationInitiationViewIncludes.h"
#include "Rendering.h"
#include "MyPinCreation.h"
#include "IMyPinCreationInitiationViewModule.h"
#include "UiToNativeMessageBus.h"

namespace ExampleApp
{
    namespace MyPinCreation
    {
        class MyPinCreationInitiationViewModule: public IMyPinCreationInitiationViewModule, private Eegeo::NonCopyable
        {
        private:
            MyPinCreationInitiationViewController* m_pController;
            
        public:
            MyPinCreationInitiationViewModule(ExampleApp::ExampleAppMessaging::UiToNativeMessageBus& uiToNativeMessageBus,
                                              IMyPinCreationInitiationViewModel& viewModel,
                                              IMyPinCreationConfirmationViewModel& confirmationViewModel,
                                              const Eegeo::Rendering::ScreenProperties& screenProperties);
            
            ~MyPinCreationInitiationViewModule();
            
            MyPinCreationInitiationViewController& GetMyPinCreationInitiationViewController() const;
            
            MyPinCreationInitiationView& GetMyPinCreationInitiationView() const;
        };
    }
}