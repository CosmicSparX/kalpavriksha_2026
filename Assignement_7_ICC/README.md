# ICC ODI Player Performance Analyzer

## Objective

- Design and implement a data management and analysis system for ICC ODI player statistics that organizes and processes data using Searching, Sorting and Linked list concepts.

- The system should allow storing, searching, sorting, and merging team and player data for analytical insights.

## Players_data.h

- This header file provides predefined data of 200 players of 10 teams.

- It also includes an array of all 10 teams names.

- This header file can be included in our program to load initial data of these players and teams.

- Please note that, A separate Player object must be defined according to the data model provided below, independent of the Player structure defined in this header file.

## Data Model

- Each player will have `PlayerId`, `Name`, `TeamName`, `Role` (Batsman, Bowler, All-rounder), `TotalRuns`, `BattingAverage`, `StrikeRate`, `Wickets`, `EconomyRate`, `PerformanceIndex`.

- Each team will have `TeamId`, `Name`, `TotalPlayers`, `AverageBattingStrikerate`.
  (Note: To calculate AverageBattingStrikerate, consider strike rate of batters and all-rounders only)

## PerformanceIndex

Each player will have a performance index according to the Role. And formula for the same are given below:

- **Batsman:**
  `(BattingAverage × StrikeRate) / 100`

- **Bowlers:**
  `(Wickets × 2) + (100 − EconomyRate)`

- **All-Rounders:**
  `[(BattingAverage × StrikeRate) / 100] + (Wickets × 2)`

**Note:** Performance indexes for different roles are not directly comparable. For example, if Batsman X has a higher performance index than Bowler Y, it does not necessarily mean X’s performance is better than Y.

## Functional Requirements

- Create a menu-driven program to simulate ICC’s internal system with the following features:

- Once the program starts, initialize players and teams using the given header file. After that:
  1. Allow user to add new players to teams using team id.

  2. Allow user to display all players of a specific team with a total number of players and average batting strike rate.

  3. Allow user to display teams according to average batting strike rate of batters + all-rounders of that team in descending order.

  4. Allow user to display top K players of a specific team according to their performance index and given Role. (For example, if the input role is Batsman, then the top K batsmen of that specific team will be displayed according to their performance index in descending order, and the same applies for Bowlers and All-rounders.).

  5. Allow user to display players of all teams of specific role according to their performance index in descending order. (For example, if the input role is Bowler, then all Bowlers of all team will be displayed according to their performance index in descending order, and the same applies for Batsmen and All-rounders.).

## Performance Requirements

- After initialization, team searching should be done in `O(logn)` time when required.

- Requirement mentioned in 4th point should be done in `O(K)` time.

- Requirement mentioned in 5th point should be done in `O(N logt)` time.
  Where `t` is the number of teams and `N` is the total number of players of that role (An extra list can be created.)

## Constraints

- Number of teams = 10

- 1 <= PlayerId/TeamId <= 1000

- 11 <= Players per team <= 50

- 1 <= Name length <= 50

## Example

Note: Purpose of data displayed in the example is only to explain each requirement. It may not align with data given in the header file.
