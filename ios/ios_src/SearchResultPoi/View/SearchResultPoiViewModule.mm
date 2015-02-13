// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "SearchResultPoiViewModule.h"
#include "ISearchResultPoiViewModel.h"
#include "SearchResultPoiView.h"
#include "SearchResultPoiController.h"

namespace ExampleApp
{
    namespace SearchResultPoi
    {
        namespace View
        {
            SearchResultPoiViewModule::SearchResultPoiViewModule(ISearchResultPoiViewModel& searchResultPoiViewModel,
                                                                 ExampleAppMessaging::TMessageBus& messageBus)
            {
                m_pView = [[SearchResultPoiView alloc] initWithoutParams];
                
                m_pController = Eegeo_NEW(SearchResultPoiController)(*[m_pView getInterop],
                                                                     searchResultPoiViewModel,
                                                                     messageBus);
            }

            SearchResultPoiViewModule::~SearchResultPoiViewModule()
            {
                Eegeo_DELETE m_pController;
                
                [m_pView release];
            }

            SearchResultPoiController& SearchResultPoiViewModule::GetController() const
            {
                return *m_pController;
            }

            SearchResultPoiView& SearchResultPoiViewModule::GetView() const
            {
                return *m_pView;
            }
        }
    }
}
