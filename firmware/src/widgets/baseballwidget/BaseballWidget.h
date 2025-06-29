#ifndef BASEBALL_WIDGET_H
#define BASEBALL_WIDGET_H

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <TFT_eSPI.h>

#include "BaseballDataModel.h"
#include "Widget.h"

#define MAX_BASEBALL_GAMES 5

class BaseballWidget : public Widget {
public:
    BaseballWidget(ScreenManager &manager);
    void setup() override;
    void update(bool force = false) override;
    void draw(bool force = false) override;
    void buttonPressed(uint8_t buttonId, ButtonState state) override;
    String getName() override;

    void changeMode();

private:
    void getBaseballData(BaseballDataModel &game);
    void displayGame(int8_t displayIndex, BaseballDataModel &game, uint32_t backgroundColor, uint32_t textColor);
    void fetchGamesForTeams();
    bool isTeamInFavorites(String teamName);

    unsigned long m_baseballDelay = 300000; // default to 5m between updates
    unsigned long m_baseballDelayPrev = 0;
    unsigned long m_rotationDelay = 5000; // 5 seconds between game rotations
    unsigned long m_rotationDelayPrev = 0;

    BaseballDataModel m_games[MAX_BASEBALL_GAMES];
    int8_t m_gameCount;
    int8_t m_currentGameIndex;

    String m_favoriteTeams[10]; // Array to store favorite team names
    int8_t m_favoriteTeamCount;
};

#endif // BASEBALL_WIDGET_H