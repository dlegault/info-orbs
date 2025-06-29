#include "BaseballDataModel.h"

BaseballDataModel::BaseballDataModel() {
}

// Team information
BaseballDataModel &BaseballDataModel::setHomeTeam(String homeTeam) {
    m_homeTeam = homeTeam;
    return *this;
}

String BaseballDataModel::getHomeTeam() {
    return m_homeTeam;
}

BaseballDataModel &BaseballDataModel::setAwayTeam(String awayTeam) {
    m_awayTeam = awayTeam;
    return *this;
}

String BaseballDataModel::getAwayTeam() {
    return m_awayTeam;
}

// Scores
BaseballDataModel &BaseballDataModel::setHomeScore(int homeScore) {
    m_homeScore = homeScore;
    return *this;
}

int BaseballDataModel::getHomeScore() {
    return m_homeScore;
}

BaseballDataModel &BaseballDataModel::setAwayScore(int awayScore) {
    m_awayScore = awayScore;
    return *this;
}

int BaseballDataModel::getAwayScore() {
    return m_awayScore;
}

// Game status
BaseballDataModel &BaseballDataModel::setGameStatus(String status) {
    m_gameStatus = status;
    return *this;
}

String BaseballDataModel::getGameStatus() {
    return m_gameStatus;
}

BaseballDataModel &BaseballDataModel::setInning(String inning) {
    m_inning = inning;
    return *this;
}

String BaseballDataModel::getInning() {
    return m_inning;
}

BaseballDataModel &BaseballDataModel::setInningState(String inningState) {
    m_inningState = inningState;
    return *this;
}

String BaseballDataModel::getInningState() {
    return m_inningState;
}

// Game time
BaseballDataModel &BaseballDataModel::setGameTime(String gameTime) {
    m_gameTime = gameTime;
    return *this;
}

String BaseballDataModel::getGameTime() {
    return m_gameTime;
}

BaseballDataModel &BaseballDataModel::setGameDate(String gameDate) {
    m_gameDate = gameDate;
    return *this;
}

String BaseballDataModel::getGameDate() {
    return m_gameDate;
}

// Game ID
BaseballDataModel &BaseballDataModel::setGameId(String gameId) {
    m_gameId = gameId;
    return *this;
}

String BaseballDataModel::getGameId() {
    return m_gameId;
}

// Change tracking
bool BaseballDataModel::isChanged() {
    return m_changed;
}

BaseballDataModel &BaseballDataModel::setChangedStatus(bool changed) {
    m_changed = changed;
    return *this;
}

// Utility methods
String BaseballDataModel::getScoreDisplay() {
    return String(m_awayScore) + " - " + String(m_homeScore);
}

String BaseballDataModel::getGameStatusDisplay() {
    if (isGameLive()) {
        return m_inning + " " + m_inningState;
    } else if (isGameFinal()) {
        return "FINAL";
    } else if (isGameScheduled()) {
        return m_gameTime;
    } else {
        return m_gameStatus;
    }
}

bool BaseballDataModel::isGameLive() {
    return m_gameStatus == "Live" || m_gameStatus == "In Progress";
}

bool BaseballDataModel::isGameFinal() {
    return m_gameStatus == "Final" || m_gameStatus == "Game Over";
}

bool BaseballDataModel::isGameScheduled() {
    return m_gameStatus == "Scheduled" || m_gameStatus == "Pre-Game";
} 