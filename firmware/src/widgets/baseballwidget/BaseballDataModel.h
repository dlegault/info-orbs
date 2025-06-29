#ifndef BASEBALL_DATA_MODEL_H
#define BASEBALL_DATA_MODEL_H

#include <Arduino.h>

class BaseballDataModel {
public:
    BaseballDataModel();
    
    // Team information
    BaseballDataModel &setHomeTeam(String homeTeam);
    String getHomeTeam();
    BaseballDataModel &setAwayTeam(String awayTeam);
    String getAwayTeam();
    
    // Scores
    BaseballDataModel &setHomeScore(int homeScore);
    int getHomeScore();
    BaseballDataModel &setAwayScore(int awayScore);
    int getAwayScore();
    
    // Game status
    BaseballDataModel &setGameStatus(String status);
    String getGameStatus();
    BaseballDataModel &setInning(String inning);
    String getInning();
    BaseballDataModel &setInningState(String inningState);
    String getInningState();
    
    // Game time
    BaseballDataModel &setGameTime(String gameTime);
    String getGameTime();
    BaseballDataModel &setGameDate(String gameDate);
    String getGameDate();
    
    // Game ID for tracking
    BaseballDataModel &setGameId(String gameId);
    String getGameId();
    
    // Change tracking
    bool isChanged();
    BaseballDataModel &setChangedStatus(bool changed);
    
    // Utility methods
    String getScoreDisplay();
    String getGameStatusDisplay();
    bool isGameLive();
    bool isGameFinal();
    bool isGameScheduled();

private:
    String m_homeTeam = "";
    String m_awayTeam = "";
    int m_homeScore = 0;
    int m_awayScore = 0;
    String m_gameStatus = "";
    String m_inning = "";
    String m_inningState = "";
    String m_gameTime = "";
    String m_gameDate = "";
    String m_gameId = "";
    bool m_changed = false;
};

#endif // BASEBALL_DATA_MODEL_H 