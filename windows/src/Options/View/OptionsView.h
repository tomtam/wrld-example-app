// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#pragma once

#include "Types.h"
#include "IOptionsView.h"
#include "WindowsNativeState.h"
#include "ICallback.h"
#include "CallbackCollection.h"

namespace ExampleApp
{
    namespace Options
    {
        namespace View
        {
            class OptionsViewImpl;

            class OptionsView : public IOptionsView, private Eegeo::NonCopyable
            {
            private:
                OptionsViewImpl* m_pImpl;

            public:
                OptionsView(const std::shared_ptr<WindowsNativeState>& nativeState);

                ~OptionsView();

                bool IsStreamOverWifiOnlySelected() const;

                bool IsCacheEnabledSelected() const;

                void SetStreamOverWifiOnlySelected(bool isStreamOverWifiOnlySelected);

                void SetCacheEnabledSelected(bool isCacheEnabledSelected);

                void Open();

                void Close();

                void ConcludeCacheClearCeremony();

                void HandleCloseSelected();

                void HandleStreamOverWifiOnlySelectionStateChanged();

                void HandleCacheEnabledSelectionStateChanged();

                void HandleClearCacheSelected();

                void InsertCloseSelectedCallback(Eegeo::Helpers::ICallback0& callback);
                void RemoveCloseSelectedCallback(Eegeo::Helpers::ICallback0& callback);

                void InsertStreamOverWifiOnlySelectionChangedCallback(Eegeo::Helpers::ICallback0& callback);
                void RemoveStreamOverWifiOnlySelectionChangedCallback(Eegeo::Helpers::ICallback0& callback);

                void InsertCacheEnabledSelectionCallback(Eegeo::Helpers::ICallback0& callback);
                void RemoveCacheEnabledSelectionCallback(Eegeo::Helpers::ICallback0& callback);

                void InsertClearCacheSelectedCallback(Eegeo::Helpers::ICallback0& callback);
                void RemoveClearCacheSelectedCallback(Eegeo::Helpers::ICallback0& callback);
            };
        }
    }
}
