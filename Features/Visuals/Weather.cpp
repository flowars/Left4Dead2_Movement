#include "Visuals.h"

static vcollide_t precipitation_collideable{};
int m_timer = -1;

void Visuals::Reset_Weather(const bool cleanup) {
    created_rain = false;

    if (rain_entity && cleanup) {
        rain_entity = reinterpret_cast<CPrecipitation*>(interfaces::entity_list->GetClientEntity(MAX_EDICTS - 1));
        if (!rain_entity) {
            return;
        }

        rain_entity->PreDataUpdate(DATA_UPDATE_CREATED);
        rain_entity->OnPreDataChanged(DATA_UPDATE_CREATED);

        rain_entity->m_nPrecipType() = (precipitation_type_t)-1;

        rain_entity->Collideable()->OBBMins() = { 0.f, 0.f, 0.f };
        rain_entity->Collideable()->OBBMaxs() = { 0.f, 0.f, 0.f };

        interfaces::physics_collision->VCollideUnload(&precipitation_collideable);

        rain_entity->OnDataChanged(DATA_UPDATE_CREATED);
        rain_entity->PostDataUpdate(DATA_UPDATE_CREATED);

        if (rain_entity->Networkable())
            rain_entity->Release(1);

    }

    rain_entity = nullptr;
    m_timer = -1;
}

void* Visuals::GetVCollideble()
{
    return &precipitation_collideable;
}

void Visuals::Update_Weather() {

    if (!precipitation) {
        for (auto pclass = interfaces::client->GetAllClasses(); pclass && !precipitation; pclass = pclass->m_pNext)
            if (strstr(pclass->m_pNetworkName, "CPrecipitation"))
            {
                precipitation = pclass;
            }
    }

    static int weather_type = 0;
    switch (Config::Visuals::iWeather) {
    case 0: {
        weather_type = 3;
        break;
    }
    case 1: {
        weather_type = 4;
        break;
    }
    case 2: {
        weather_type = 5;
        break;
    }
    case 3: {
        weather_type = 6;
        break;
    }
    case 4: {
        weather_type = 7;
        break;
    }
    case 5: {
        weather_type = 8;
        break;
    }
    }
    if (m_timer > -1) {
        --m_timer;
        if (m_timer == 0) {
            Reset_Weather();
        }
    }

    if (!precipitation)
        return;

    static std::optional< int > last_type{};

    if (last_type.has_value() && last_type.value() != weather_type)
        Reset_Weather();

    last_type = weather_type;

    memset(&precipitation_collideable, 0, sizeof(precipitation_collideable));

    if (!created_rain && Config::Visuals::bWeather || Config::Visuals::bWeather && !last_state || interfaces::engine->IsConnected() && !reinterpret_cast<CPrecipitation*>(interfaces::entity_list->GetClientEntity(MAX_EDICTS - 1)) && rain_entity && Config::Visuals::bWeather) {
        if (created_rain && rain_entity)
            Reset_Weather();

        if (precipitation->m_pCreateFn && precipitation->m_pCreateFn(MAX_EDICTS - 1, 0)) {

            rain_entity = reinterpret_cast<CPrecipitation*>(interfaces::entity_list->GetClientEntity(MAX_EDICTS - 1));

            rain_entity->PreDataUpdate(DATA_UPDATE_CREATED);
            rain_entity->OnPreDataChanged(DATA_UPDATE_CREATED);

            printf("Created Precip: %p \n", rain_entity);

            rain_entity->GetIndex() = -1;

            rain_entity->m_nPrecipType() = (precipitation_type_t) * &weather_type;

            rain_entity->Collideable()->OBBMins() = { -32768.0f, -32768.0f, -32768.0f };
            rain_entity->Collideable()->OBBMaxs() = { 32768.0f, 32768.0f, 32768.0f };

            interfaces::physics_collision->VCollideLoad(&precipitation_collideable, 1, (const char*)collide_data, sizeof(collide_data));

            rain_entity->m_nModelIndex() = -1;

            rain_entity->OnDataChanged(DATA_UPDATE_CREATED);
            rain_entity->PostDataUpdate(DATA_UPDATE_CREATED);


            created_rain = true;
        }
    }
    else if (created_rain && !Config::Visuals::bWeather && last_state) {
        Reset_Weather();
    }

    last_state = Config::Visuals::bWeather;
}

