#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

typedef enum { ROLE_BATSMAN = 1, ROLE_BOWLER = 2, ROLE_ALLROUNDER = 3 } RoleType;

typedef struct {
    int playerId;
    char playerName[64];
    char teamName[64];
    RoleType roleType;
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wicketsTaken;
    float economyRate;
    double performanceIndex;
} PlayerInfo;

typedef struct {
    char teamName[64];
    int totalPlayers;
    double averageStrikeRate;
} TeamStats;

double computePerformance(const PlayerInfo *playerRecord);
RoleType getRoleType(const char *roleString);
const char* getRoleText(RoleType roleType);
int comparePerformance(const void *firstPlayerRecord, const void *secondPlayerRecord);
int getTeamIndex(TeamStats teamArray[], int totalTeams, const char *teamName);
void showPlayerList(PlayerInfo playerArray[], int totalPlayers);
void loadPlayerData(PlayerInfo playerArray[]);
void loadTeamStats(TeamStats teamArray[], PlayerInfo playerArray[], int totalPlayers);
void displayPlayersByTeam(PlayerInfo playerArray[], TeamStats teamArray[], int totalPlayers, int totalTeams);
void displayTeamsByStrikeRate(TeamStats teamArray[], int totalTeams);
void displayTopKByRole(PlayerInfo playerArray[], TeamStats teamArray[], int totalPlayers, int totalTeams);
void displayPlayersByRole(PlayerInfo playerArray[], int totalPlayers);


int main() {
    int totalPlayers = playerCount;
    PlayerInfo *playerArray = malloc(sizeof(PlayerInfo) * totalPlayers);
    loadPlayerData(playerArray);

    int totalTeams = teamCount;
    TeamStats *teamArray = malloc(sizeof(TeamStats) * totalTeams);
    loadTeamStats(teamArray, playerArray, totalPlayers);

    while(1) {
        printf("\n==============================================================\n");
        printf("ICC ODI Player Performance Analyzer\n");
        printf("==============================================================\n");
        printf("1. Add Player (Feature Not Included)\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams Sorted by Average Strike Rate\n");
        printf("4. Display Top K Players of a Specific Role in a Team\n");
        printf("5. Display All Players of a Specific Role Across All Teams\n");
        printf("6. Exit\n");
        printf("==============================================================\n");
        printf("Enter your choice: ");

        int userChoice;
        scanf("%d", &userChoice);

        if(userChoice == 1) {
            printf("Add Player feature not implemented.\n");
        }
        else if(userChoice == 2) {
            displayPlayersByTeam(playerArray, teamArray, totalPlayers, totalTeams);
        }
        else if(userChoice == 3) {
            displayTeamsByStrikeRate(teamArray, totalTeams);
        }
        else if(userChoice == 4) {
            displayTopKByRole(playerArray, teamArray, totalPlayers, totalTeams);
        }
        else if(userChoice == 5) {
            displayPlayersByRole(playerArray, totalPlayers);
        }
        else if(userChoice == 6) {
            break;
        }
        else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    free(playerArray);
    free(teamArray);
    return 0;
}


double computePerformance(const PlayerInfo *playerRecord) {
    if(playerRecord->roleType == ROLE_BATSMAN)
        return (playerRecord->battingAverage * playerRecord->strikeRate) / 100.0;

    if(playerRecord->roleType == ROLE_BOWLER)
        return (playerRecord->wicketsTaken * 2.0) + (100.0 - playerRecord->economyRate);

    return ((playerRecord->battingAverage * playerRecord->strikeRate) / 100.0) + 
            (playerRecord->wicketsTaken * 2.0);
}

RoleType getRoleType(const char *roleString) {
    if(strcmp(roleString, "Batsman") == 0) return ROLE_BATSMAN;
    if(strcmp(roleString, "Bowler") == 0) return ROLE_BOWLER;
    return ROLE_ALLROUNDER;
}

const char* getRoleText(RoleType roleType) {
    if(roleType == ROLE_BATSMAN) return "Batsman";
    if(roleType == ROLE_BOWLER) return "Bowler";
    return "All-rounder";
}

int comparePerformance(const void *firstPlayerRecord, const void *secondPlayerRecord) {
    const PlayerInfo *firstPlayer = firstPlayerRecord;
    const PlayerInfo *secondPlayer = secondPlayerRecord;

    if(secondPlayer->performanceIndex > firstPlayer->performanceIndex) return 1;
    if(secondPlayer->performanceIndex < firstPlayer->performanceIndex) return -1;
    return 0;
}

int getTeamIndex(TeamStats teamArray[], int totalTeams, const char *teamName) {
    for(int teamIndex = 0; teamIndex < totalTeams; teamIndex++)
        if(strcasecmp(teamArray[teamIndex].teamName, teamName) == 0)
            return teamIndex;
    return -1;
}

void showPlayerList(PlayerInfo playerArray[], int totalPlayers) {
    printf("==========================================================================\n");
    printf("ID Name Role Runs Avg SR Wkts ER PerfIndex\n");
    printf("==========================================================================\n");

    for(int index = 0; index < totalPlayers; index++)
        printf("%d %s %s %d %.1f %.1f %d %.1f %.2f\n",
               playerArray[index].playerId,
               playerArray[index].playerName,
               getRoleText(playerArray[index].roleType),
               playerArray[index].totalRuns,
               playerArray[index].battingAverage,
               playerArray[index].strikeRate,
               playerArray[index].wicketsTaken,
               playerArray[index].economyRate,
               playerArray[index].performanceIndex);

    printf("==========================================================================\n");
}

void loadPlayerData(PlayerInfo playerArray[]) {
    for(int index = 0; index < playerCount; index++) {
        playerArray[index].playerId = players[index].id;
        strcpy(playerArray[index].playerName, players[index].name);
        strcpy(playerArray[index].teamName, players[index].team);
        playerArray[index].roleType = getRoleType(players[index].role);
        playerArray[index].totalRuns = players[index].totalRuns;
        playerArray[index].battingAverage = players[index].battingAverage;
        playerArray[index].strikeRate = players[index].strikeRate;
        playerArray[index].wicketsTaken = players[index].wickets;
        playerArray[index].economyRate = players[index].economyRate;
        playerArray[index].performanceIndex = computePerformance(&playerArray[index]);
    }
}

void loadTeamStats(TeamStats teamArray[], PlayerInfo playerArray[], int totalPlayers) {
    for(int index = 0; index < teamCount; index++) {
        strcpy(teamArray[index].teamName, teams[index]);
        teamArray[index].totalPlayers = 0;
        teamArray[index].averageStrikeRate = 0;
    }

    for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++)
        for(int teamIndex = 0; teamIndex < teamCount; teamIndex++)
            if(strcmp(playerArray[playerIndex].teamName, teamArray[teamIndex].teamName) == 0)
                teamArray[teamIndex].totalPlayers++;

    for(int teamIndex = 0; teamIndex < teamCount; teamIndex++) {
        double strikeRateSum = 0;
        int validStrikerCount = 0;

        for(int playerIndex = 0; playerIndex < totalPlayers; playerIndex++)
            if(strcmp(playerArray[playerIndex].teamName, teamArray[teamIndex].teamName) == 0 &&
               (playerArray[playerIndex].roleType == ROLE_BATSMAN || playerArray[playerIndex].roleType == ROLE_ALLROUNDER)) {
                strikeRateSum += playerArray[playerIndex].strikeRate;
                validStrikerCount++;
            }

        teamArray[teamIndex].averageStrikeRate = (validStrikerCount == 0 ? 0 : strikeRateSum / validStrikerCount);
    }
}

void displayPlayersByTeam(PlayerInfo playerArray[], TeamStats teamArray[], int totalPlayers, int totalTeams) {
    char inputTeamName[64];
    printf("Enter Team Name: ");
    scanf(" %[^\n]", inputTeamName);

    int teamIndex = getTeamIndex(teamArray, totalTeams, inputTeamName);
    if(teamIndex == -1) {
        printf("Team not found.\n");
        return;
    }

    PlayerInfo *teamPlayerList = malloc(sizeof(PlayerInfo) * teamArray[teamIndex].totalPlayers);
    int filteredCount = 0;

    for(int i = 0; i < totalPlayers; i++)
        if(strcasecmp(playerArray[i].teamName, inputTeamName) == 0)
            teamPlayerList[filteredCount++] = playerArray[i];

    printf("Players of %s:\n", inputTeamName);
    showPlayerList(teamPlayerList, filteredCount);
    printf("Total Players: %d\nAverage Strike Rate: %.2f\n", filteredCount, teamArray[teamIndex].averageStrikeRate);

    free(teamPlayerList);
}

void displayTeamsByStrikeRate(TeamStats teamArray[], int totalTeams) {
    TeamStats sortedTeamArray[10];
    for(int i = 0; i < totalTeams; i++)
        sortedTeamArray[i] = teamArray[i];

    for(int i = 0; i < totalTeams; i++)
        for(int j = i + 1; j < totalTeams; j++)
            if(sortedTeamArray[j].averageStrikeRate > sortedTeamArray[i].averageStrikeRate) {
                TeamStats temp = sortedTeamArray[i];
                sortedTeamArray[i] = sortedTeamArray[j];
                sortedTeamArray[j] = temp;
            }

    printf("Team | Avg Strike Rate | Total Players\n");
    for(int i = 0; i < totalTeams; i++)
        printf("%s | %.2f | %d\n", sortedTeamArray[i].teamName, sortedTeamArray[i].averageStrikeRate, sortedTeamArray[i].totalPlayers);
}

void displayTopKByRole(PlayerInfo playerArray[], TeamStats teamArray[], int totalPlayers, int totalTeams) {
    char inputTeamName[64];
    printf("Enter Team Name: ");
    scanf(" %[^\n]", inputTeamName);

    int teamIndex = getTeamIndex(teamArray, totalTeams, inputTeamName);
    if(teamIndex == -1) {
        printf("Team not found.\n");
        return;
    }

    int roleChoice, topK;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-Allrounder): ");
    scanf("%d", &roleChoice);
    printf("Enter K Value: ");
    scanf("%d", &topK);

    PlayerInfo *rolePlayerList = malloc(sizeof(PlayerInfo) * teamArray[teamIndex].totalPlayers);
    int filteredCount = 0;

    for(int i = 0; i < totalPlayers; i++)
        if(strcasecmp(playerArray[i].teamName, inputTeamName) == 0 && playerArray[i].roleType == roleChoice)
            rolePlayerList[filteredCount++] = playerArray[i];

    if(filteredCount == 0) {
        printf("No players found.\n");
        free(rolePlayerList);
        return;
    }

    qsort(rolePlayerList, filteredCount, sizeof(PlayerInfo), comparePerformance);
    if(topK > filteredCount) topK = filteredCount;

    printf("Top %d %s of %s:\n", topK, getRoleText(roleChoice), inputTeamName);
    showPlayerList(rolePlayerList, topK);

    free(rolePlayerList);
}

void displayPlayersByRole(PlayerInfo playerArray[], int totalPlayers) {
    int roleChoice;
    printf("Enter Role (1-Batsman, 2-Bowler, 3-Allrounder): ");
    scanf("%d", &roleChoice);

    PlayerInfo *rolePlayerList = malloc(sizeof(PlayerInfo) * totalPlayers);
    int filteredCount = 0;

    for(int i = 0; i < totalPlayers; i++)
        if(playerArray[i].roleType == roleChoice)
            rolePlayerList[filteredCount++] = playerArray[i];

    qsort(rolePlayerList, filteredCount, sizeof(PlayerInfo), comparePerformance);
    showPlayerList(rolePlayerList, filteredCount);

    free(rolePlayerList);
}
