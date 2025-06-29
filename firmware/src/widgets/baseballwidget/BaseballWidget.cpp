#include "BaseballWidget.h"

#include "config_helper.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>

BaseballWidget::BaseballWidget(ScreenManager &manager) : Widget(manager), m_gameCount(0), m_currentGameIndex(0), m_favoriteTeamCount(0) {
#ifdef BASEBALL_TEAM_LIST
    char teamList[strlen(BASEBALL_TEAM_LIST) + 1];
    strcpy(teamList, BASEBALL_TEAM_LIST);

    char *team = strtok(teamList, ",");
    m_favoriteTeamCount = 0;
    do {
        if (team != nullptr) {
            m_favoriteTeams[m_favoriteTeamCount] = String(team);
            m_favoriteTeamCount++;
            if (m_favoriteTeamCount >= 10) {
                Serial.println("MAX TEAMS REACHED");
                break;
            }
        }
    } while (team = strtok(nullptr, ","));
#endif
}

void BaseballWidget::setup() {
    if (m_favoriteTeamCount == 0) {
        Serial.println("No baseball teams configured");
        return;
    }
    
    Serial.println("Baseball widget setup complete with " + String(m_favoriteTeamCount) + " teams");
}

void BaseballWidget::draw(bool force) {
    if (m_gameCount == 0) {
        // Display "No Games" message on all screens
        for (int8_t i = 0; i < 5; i++) {
            m_manager.selectScreen(i);
            m_manager.fillScreen(TFT_BLACK);
            m_manager.setFontColor(TFT_WHITE, TFT_BLACK);
            m_manager.drawCentreString("No Games", 120, 100, 20);
            m_manager.drawCentreString("Today", 120, 130, 16);
        }
        return;
    }

    // Display current game on all screens
    BaseballDataModel &currentGame = m_games[m_currentGameIndex];
    if (currentGame.isChanged() || force) {
        for (int8_t i = 0; i < 5; i++) {
            displayGame(i, currentGame, TFT_BLACK, TFT_WHITE);
        }
        currentGame.setChangedStatus(false);
    }
}

void BaseballWidget::update(bool force) {
    if (force || m_baseballDelayPrev == 0 || (millis() - m_baseballDelayPrev) >= m_baseballDelay) {
        setBusy(true);
        fetchGamesForTeams();
        setBusy(false);
        m_baseballDelayPrev = millis();
    }
    
    // Rotate through games if we have multiple
    if (m_gameCount > 1 && (m_rotationDelayPrev == 0 || (millis() - m_rotationDelayPrev) >= m_rotationDelay)) {
        m_currentGameIndex = (m_currentGameIndex + 1) % m_gameCount;
        draw(true);
        m_rotationDelayPrev = millis();
    }
}

void BaseballWidget::changeMode() {
    // Switch to next game immediately
    if (m_gameCount > 1) {
        m_currentGameIndex = (m_currentGameIndex + 1) % m_gameCount;
        draw(true);
        m_rotationDelayPrev = millis();
    }
}

void BaseballWidget::buttonPressed(uint8_t buttonId, ButtonState state) {
    if (buttonId == BUTTON_OK && state == BTN_SHORT)
        changeMode();
}

void BaseballWidget::fetchGamesForTeams() {
    // Get today's date in YYYY-MM-DD format
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", timeinfo);
    
    String url = "https://statsapi.mlb.com/api/v1/schedule?date=" + String(dateStr) + "&sportId=1";
    
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
        String payload = http.getString();
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (!error) {
            m_gameCount = 0;
            
            if (doc.containsKey("dates") && doc["dates"].size() > 0) {
                JsonArray games = doc["dates"][0]["games"];
                
                for (JsonObject game : games) {
                    if (m_gameCount >= MAX_BASEBALL_GAMES) break;
                    
                    String awayTeam = game["teams"]["away"]["team"]["name"].as<String>();
                    String homeTeam = game["teams"]["home"]["team"]["name"].as<String>();
                    
                    // Check if either team is in our favorites
                    if (isTeamInFavorites(awayTeam) || isTeamInFavorites(homeTeam)) {
                        BaseballDataModel &gameModel = m_games[m_gameCount];
                        
                        gameModel.setAwayTeam(awayTeam);
                        gameModel.setHomeTeam(homeTeam);
                        gameModel.setAwayScore(game["teams"]["away"]["score"].as<int>());
                        gameModel.setHomeScore(game["teams"]["home"]["score"].as<int>());
                        gameModel.setGameStatus(game["status"]["detailedState"].as<String>());
                        gameModel.setGameId(game["gamePk"].as<String>());
                        
                        // Handle inning information for live games
                        if (gameModel.isGameLive()) {
                            JsonObject linescore = game["linescore"];
                            if (!linescore.isNull()) {
                                String inning = linescore["currentInningOrdinal"].as<String>();
                                String inningState = linescore["inningState"].as<String>();
                                gameModel.setInning(inning);
                                gameModel.setInningState(inningState);
                            }
                        }
                        
                        // Handle game time for scheduled games
                        if (gameModel.isGameScheduled()) {
                            String gameTime = game["gameDate"].as<String>();
                            // Convert UTC to local time (simplified)
                            gameModel.setGameTime(gameTime.substring(11, 16)); // Extract HH:MM
                        }
                        
                        gameModel.setChangedStatus(true);
                        m_gameCount++;
                    }
                }
            }
            
            Serial.println("Found " + String(m_gameCount) + " games for favorite teams");
        } else {
            Serial.println("deserializeJson() failed for baseball data");
        }
    } else {
        Serial.printf("HTTP request failed for baseball data, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}

bool BaseballWidget::isTeamInFavorites(String teamName) {
    for (int i = 0; i < m_favoriteTeamCount; i++) {
        if (m_favoriteTeams[i] == teamName) {
            return true;
        }
    }
    return false;
}

void BaseballWidget::displayGame(int8_t displayIndex, BaseballDataModel &game, uint32_t backgroundColor, uint32_t textColor) {
    m_manager.selectScreen(displayIndex);
    m_manager.fillScreen(backgroundColor);

    // Calculate center positions
    int centre = 120;
    int bigFontSize = 20;
    int smallFontSize = 14;
    int tinyFontSize = 12;

    // Header with teams
    m_manager.fillRect(0, 0, 240, 50, TFT_NAVY);
    m_manager.setFontColor(TFT_WHITE, TFT_NAVY);
    m_manager.drawCentreString(game.getAwayTeam(), centre, 15, smallFontSize);
    m_manager.drawCentreString("vs", centre, 30, tinyFontSize);
    m_manager.drawCentreString(game.getHomeTeam(), centre, 45, smallFontSize);

    // Main score area
    m_manager.fillRect(0, 50, 240, 80, TFT_WHITE);
    m_manager.setFontColor(TFT_BLACK, TFT_WHITE);
    m_manager.drawCentreString(game.getScoreDisplay(), centre, 90, bigFontSize);

    // Game status area
    m_manager.fillRect(0, 130, 240, 40, TFT_LIGHTGREY);
    m_manager.setFontColor(TFT_BLACK, TFT_LIGHTGREY);
    m_manager.drawCentreString(game.getGameStatusDisplay(), centre, 150, smallFontSize);

    // Bottom info area
    m_manager.fillRect(0, 170, 240, 70, TFT_BLACK);
    m_manager.setFontColor(TFT_WHITE, TFT_BLACK);
    
    // Show different info based on game status
    if (game.isGameLive()) {
        m_manager.drawCentreString("LIVE", centre, 185, smallFontSize);
        m_manager.drawCentreString("Game " + String(m_currentGameIndex + 1) + " of " + String(m_gameCount), centre, 205, tinyFontSize);
    } else if (game.isGameFinal()) {
        m_manager.drawCentreString("FINAL", centre, 185, smallFontSize);
        m_manager.drawCentreString("Game " + String(m_currentGameIndex + 1) + " of " + String(m_gameCount), centre, 205, tinyFontSize);
    } else {
        m_manager.drawCentreString("SCHEDULED", centre, 185, smallFontSize);
        m_manager.drawCentreString("Game " + String(m_currentGameIndex + 1) + " of " + String(m_gameCount), centre, 205, tinyFontSize);
    }
}

String BaseballWidget::getName() {
    return "Baseball";
} 