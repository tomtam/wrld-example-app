// Copyright eeGeo Ltd (2012-2015), All Rights Reserved

#include "MyPinsFileIO.h"
#include "LatLongAltitude.h"
#include "IPersistentSettingsModel.h"
#include "MyPinModel.h"
#include "MyPinsSemanticPinType.h"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include <sstream>
#include <vector>


namespace ExampleApp
{
    namespace MyPins
    {
        namespace SdkModel
        {
            const std::string MyPinsDataFilename = "pin_data.json";
            const std::string MyPinImagePrefix = "my_pin_image_";
            const std::string MyPins_LastMyPinModelIdKey = "MyPins_LastMyPinModelIdKey";
            const std::string MyPinsJsonArrayName = "myPins";

            void CreateEmptyJsonFile(Eegeo::Helpers::IFileIO& fileIO)
            {
                rapidjson::Document jsonDoc;
                jsonDoc.SetObject();

                rapidjson::Value myArray(rapidjson::kArrayType);

                jsonDoc.AddMember(MyPinsJsonArrayName.c_str(), myArray, jsonDoc.GetAllocator());
                rapidjson::StringBuffer strbuf;
                rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
                jsonDoc.Accept(writer);

                std::string jsonString(strbuf.GetString());
                fileIO.WriteFile((Byte*)jsonString.c_str(), jsonString.size(), MyPinsDataFilename);
            }

            MyPinsFileIO::MyPinsFileIO(Eegeo::Helpers::IFileIO& fileIO,
                                       PersistentSettings::IPersistentSettingsModel& persistentSettings)
                : m_fileIO(fileIO)
                , m_persistentSettings(persistentSettings)
            {
                if (!m_fileIO.Exists(MyPinsDataFilename))
                {
                    CreateEmptyJsonFile(m_fileIO);
                }

                int lastId = 0;
                if (!m_persistentSettings.TryGetValue(MyPins_LastMyPinModelIdKey, lastId))
                {
                    m_persistentSettings.SetValue(MyPins_LastMyPinModelIdKey, lastId);
                }

            }

            bool MyPinsFileIO::TryCacheImageToDisk(Byte* imageData,
                                                   size_t imageSize,
                                                   int myPinId,
                                                   std::string& out_filename)
            {
                out_filename = "";

                std::stringstream ss;
                ss << MyPinImagePrefix << myPinId << ".jpg";

                std::string imagePath = ss.str();

                if (m_fileIO.Exists(imagePath))
                {
                    Eegeo_TTY("%s already exists\n", imagePath.c_str());
                    return false;
                }

                if (m_fileIO.WriteFile(imageData, imageSize, imagePath))
                {
                    out_filename = imagePath;
                    return true;
                }

                Eegeo_TTY("Couldn't write %s to file\n", imagePath.c_str());

                return false;
            }

            void MyPinsFileIO::DeleteImageFromDisk(const std::string& imagePath)
            {
                if (m_fileIO.Exists(imagePath))
                {
                    m_fileIO.DeleteFile(imagePath);
                }
            }

            void MyPinsFileIO::SavePinModelToDisk(const MyPinModel& pinModel)
            {
                std::fstream stream;
                size_t size;

                if (m_fileIO.OpenFile(stream, size, MyPinsDataFilename))
                {
                    std::string json((std::istreambuf_iterator<char>(stream)),
                                     (std::istreambuf_iterator<char>()));

                    rapidjson::Document jsonDoc;
                    if (jsonDoc.Parse<0>(json.c_str()).HasParseError())
                    {
                        Eegeo_TTY("Parse error in MyPins JSON.\n");
                        return;
                    }

                    Eegeo_ASSERT(jsonDoc.IsObject(), "JSON document is not of object type");

                    rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();
                    rapidjson::Value& myPinsArray = jsonDoc[MyPinsJsonArrayName.c_str()];

                    const Eegeo::Space::LatLong& latLong = pinModel.GetLatLong();

                    rapidjson::Value valueObject(rapidjson::kObjectType);
                    valueObject.AddMember("id", pinModel.Identifier(), allocator);
                    valueObject.AddMember("title", pinModel.GetTitle().c_str(), allocator);
                    valueObject.AddMember("description", pinModel.GetDescription().c_str(), allocator);
                    valueObject.AddMember("icon", pinModel.GetSdkMapPinIconIndexIcon(), allocator);
                    valueObject.AddMember("type", static_cast<int>(pinModel.GetSemanticPinType()), allocator);
                    valueObject.AddMember("metadata", pinModel.GetTypeMetadata().c_str(), allocator);
                    valueObject.AddMember("latitude", latLong.GetLatitudeInDegrees(), allocator);
                    valueObject.AddMember("longitude", latLong.GetLongitudeInDegrees(), allocator);

                    myPinsArray.PushBack(valueObject, allocator);

                    rapidjson::StringBuffer strbuf;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
                    jsonDoc.Accept(writer);
                    std::string jsonString = strbuf.GetString();

                    if (WriteJsonToDisk(jsonString))
                    {
                        m_persistentSettings.SetValue(MyPins_LastMyPinModelIdKey, pinModel.Identifier());
                    }
                }
                else
                {
                    Eegeo_TTY("Couldn't open file:%s\n", MyPinsDataFilename.c_str());
                }
            }


            void MyPinsFileIO::SaveAllRepositoryPinsToDisk(const std::vector<MyPinModel*>& pinModels)
            {
                if (m_fileIO.Exists(MyPinsDataFilename))
                {
                    m_fileIO.DeleteFile(MyPinsDataFilename);
                }

                rapidjson::Document jsonDoc;
                jsonDoc.SetObject();

                rapidjson::Value pinsArray(rapidjson::kArrayType);
                pinsArray.SetArray();
                rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

                jsonDoc.AddMember(MyPinsJsonArrayName.c_str(), pinsArray, allocator);
                rapidjson::Value& myPinsArray = jsonDoc[MyPinsJsonArrayName.c_str()];

                for (std::vector<MyPinModel*>::const_iterator it = pinModels.begin(); it != pinModels.end(); ++it)
                {
                    const MyPinModel* pinModel = *it;
                    const Eegeo::Space::LatLong& latLong = pinModel->GetLatLong();

                    rapidjson::Value valueObject(rapidjson::kObjectType);
                    valueObject.AddMember("id", pinModel->Identifier(), allocator);
                    valueObject.AddMember("title", pinModel->GetTitle().c_str(), allocator);
                    valueObject.AddMember("description", pinModel->GetDescription().c_str(), allocator);
                    valueObject.AddMember("icon", pinModel->GetSdkMapPinIconIndexIcon(), allocator);
                    valueObject.AddMember("type", static_cast<int>(pinModel->GetSemanticPinType()), allocator);
                    valueObject.AddMember("metadata", pinModel->GetTypeMetadata().c_str(), allocator);
                    valueObject.AddMember("latitude", latLong.GetLatitudeInDegrees(), allocator);
                    valueObject.AddMember("longitude", latLong.GetLongitudeInDegrees(), allocator);

                    myPinsArray.PushBack(valueObject, allocator);
                }

                rapidjson::StringBuffer strbuf;
                rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
                jsonDoc.Accept(writer);

                std::string jsonString(strbuf.GetString());
                m_fileIO.WriteFile((Byte*)jsonString.c_str(), jsonString.size(), MyPinsDataFilename);
            }


            void MyPinsFileIO::LoadPinModelsFromDisk(std::vector<MyPinModel*>& out_pinModels)
            {
                out_pinModels.clear();

                std::fstream stream;
                size_t size;

                if (m_fileIO.OpenFile(stream, size, MyPinsDataFilename))
                {
                    std::string json((std::istreambuf_iterator<char>(stream)),
                                     (std::istreambuf_iterator<char>()));

                    rapidjson::Document jsonDoc;
                    if (jsonDoc.Parse<0>(json.c_str()).HasParseError())
                    {
                        Eegeo_TTY("Parse error in MyPins JSON.\n");
                        return;
                    }

                    Eegeo_ASSERT(jsonDoc.IsObject(), "JSON document is not of object type");

                    const rapidjson::Value& myPinsArray = jsonDoc[MyPinsJsonArrayName.c_str()];
                    size_t numEntries = myPinsArray.Size();

                    for(int i = 0; i < numEntries; ++i)
                    {
                        const rapidjson::Value& entry = myPinsArray[i];

                        int pinId = entry["id"].GetInt();
                        std::string title = entry["title"].GetString();
                        std::string description = entry["description"].GetString();
                        int sdkMapPinIconIndex = entry["icon"].GetInt();
                        double latitude = entry["latitude"].GetDouble();
                        double longitude = entry["longitude"].GetDouble();
                        MyPinsSemanticPinType semanticPinType = static_cast<MyPinsSemanticPinType>(entry["type"].GetInt());
                        std::string pinTypeMetadata = entry["metadata"].GetString();
                        
                        out_pinModels.push_back(Eegeo_NEW(MyPinModel)(pinId,
                                                                      title,
                                                                      description,
                                                                      sdkMapPinIconIndex,
                                                                      Eegeo::Space::LatLong::FromDegrees(latitude, longitude),
                                                                      semanticPinType,
                                                                      pinTypeMetadata));
                    }
                }
            }
            
            bool MyPinsFileIO::WriteJsonToDisk(const std::string &jsonString)
            {
                m_fileIO.DeleteFile(MyPinsDataFilename);
                return m_fileIO.WriteFile((Byte*)jsonString.c_str(), jsonString.size(), MyPinsDataFilename);
            }

            int MyPinsFileIO::GetLastIdWrittenToDisk() const
            {
                int lastId = -1;
                m_persistentSettings.TryGetValue(MyPins_LastMyPinModelIdKey, lastId);

                return lastId;
            }
        }
    }
}
