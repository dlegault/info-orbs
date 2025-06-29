# Baseball Widget

The Baseball Widget displays Major League Baseball (MLB) game scores and information for your favorite teams on the Info Orbs display.

## Features

- **Real-time scores**: Displays live scores for games involving your favorite teams
- **Game status**: Shows whether games are live, final, or scheduled
- **Multiple games**: Automatically rotates through multiple games when your teams are playing
- **Configurable teams**: Add up to 10 favorite teams via configuration
- **Automatic updates**: Refreshes data every 5 minutes
- **Game rotation**: Automatically cycles through games every 5 seconds, or manually with button press

## Configuration

To enable the Baseball Widget, add the following line to your `firmware/config/config.h` file:

```cpp
#define BASEBALL_TEAM_LIST "Seattle Mariners,New York Yankees,Boston Red Sox"
```

### Team Names

Use the full team names as they appear in the MLB API. Here are the correct team names for all MLB teams:

**American League:**

- Baltimore Orioles
- Boston Red Sox
- Chicago White Sox
- Cleveland Guardians
- Detroit Tigers
- Houston Astros
- Kansas City Royals
- Los Angeles Angels
- Minnesota Twins
- New York Yankees
- Oakland Athletics
- Seattle Mariners
- Tampa Bay Rays
- Texas Rangers
- Toronto Blue Jays

**National League:**

- Arizona Diamondbacks
- Atlanta Braves
- Chicago Cubs
- Cincinnati Reds
- Colorado Rockies
- Los Angeles Dodgers
- Miami Marlins
- Milwaukee Brewers
- New York Mets
- Philadelphia Phillies
- Pittsburgh Pirates
- San Diego Padres
- San Francisco Giants
- St. Louis Cardinals
- Washington Nationals

## Usage

1. **Widget Selection**: Use the left/right buttons to cycle to the Baseball widget
2. **Game Rotation**:
   - Games automatically rotate every 5 seconds
   - Press the center button to manually advance to the next game
3. **Display Information**: Each orb shows the same game information:
   - Team names at the top
   - Current score in the center
   - Game status (Live/Final/Scheduled) below
   - Additional game info at the bottom

## Display Layout

The widget displays:

- **Header**: Away team vs Home team
- **Main Area**: Current score (e.g., "3 - 2")
- **Status Area**: Game status (e.g., "3rd Top", "FINAL", "7:05 PM")
- **Info Area**: Live indicator and game counter

## Data Source

The widget uses the official MLB Stats API to fetch game data:

- **API Endpoint**: `https://statsapi.mlb.com/api/v1/schedule`
- **Update Frequency**: Every 5 minutes
- **Data**: Real-time scores, game status, and scheduling information

## Troubleshooting

- **No games showing**: Check that your team names are spelled correctly in the configuration
- **No data**: Ensure your device has internet connectivity
- **API errors**: The MLB API is free and doesn't require authentication, but may have rate limits

## Example Configuration

```cpp
// Track multiple teams from different divisions
#define BASEBALL_TEAM_LIST "Seattle Mariners,New York Yankees,Los Angeles Dodgers,Chicago Cubs"

// Track teams from your favorite division
#define BASEBALL_TEAM_LIST "Boston Red Sox,New York Yankees,Tampa Bay Rays,Toronto Blue Jays,Baltimore Orioles"
```
