#include "Game.h"
#include <iostream>
#include <unistd.h>
#include <limits>
#include <cstdlib>
using namespace std;

Game::Game() {
    teamA = new Team("Team A");
    teamB = new Team("Team B");

    string names[11] = {
        "Virat", "Rohit", "Dhawan", "Rahul", "Hardik",
        "Pant", "Jadeja", "Ashwin", "Shami", "Bumrah", "Surya"
    };

    for (int i = 0; i < totalPlayers; i++) {
        allPlayers.push_back(Player(i, names[i]));
    }
}

void Game::showWelcome() {
    cout << "==============================\n";
    cout << "🏏  GULLY CRICKET GAME - C++  🏏\n";
    cout << "==============================\n\n";
    usleep(500000);
}

void Game::showAllPlayers() {
    cout << "Available Players:\n";
    for (int i = 0; i < totalPlayers; i++) {
        cout << "\t" << i << " - " << allPlayers[i].name << endl;
    }
    cout << endl;
}

bool Game::isPlayerAvailable(int id) {
    for (auto p : teamA->players)
        if (p->id == id) return false;
    for (auto p : teamB->players)
        if (p->id == id) return false;
    return true;
}

void Game::selectPlayers() {
    int id;

    for (int i = 0; i < playersPerTeam * 2; i++) {
        if (i % 2 == 0)
            cout << "Select player " << (i / 2 + 1) << " for Team A: ";
        else
            cout << "Select player " << (i / 2 + 1) << " for Team B: ";

        while (true) {
            cin >> id;
            if (cin.fail() || id < 0 || id >= totalPlayers || !isPlayerAvailable(id)) {
                cout << "❌ Invalid selection! Try again: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            } else break;
        }

        if (i % 2 == 0)
            teamA->addPlayer(&allPlayers[id]);
        else
            teamB->addPlayer(&allPlayers[id]);
    }

    teamA->displayTeam();
    teamB->displayTeam();
}

void Game::toss() {
    cout << "\n🎲 Tossing...\n";
    usleep(1000000);
    int tossResult = rand() % 2;
    string tossWinner = (tossResult == 0) ? "Team A" : "Team B";

    cout << tossWinner << " won the toss.\n";
    cout << "Choose:\n1 - Bat\n2 - Bowl: ";
    int choice;
    cin >> choice;

    if ((tossResult == 0 && choice == 1) || (tossResult == 1 && choice == 2)) {
        battingTeam = teamA;
        bowlingTeam = teamB;
    } else {
        battingTeam = teamB;
        bowlingTeam = teamA;
    }
}

void Game::startFirstInnings() {
    currentInnings++;
    swap(battingTeam, bowlingTeam);
    battingTeam->totalRuns = 0;
    battingTeam->wicketsLost = 0;
    battingTeam->ballsBowled = 0;
    currentBatsman = battingTeam->players[0];
    currentBowler = bowlingTeam->players[0];

    cout << "\n🏏 Starting Innings " << (currentInnings == 2 ? "2" : "1") << ": " << battingTeam->teamName << " is batting\n";
}

void Game::playInnings() {
    int batsmanIndex = 0;

    for (int ball = 1; ball <= maxBalls; ++ball) {
        usleep(700000);
        int runs = rand() % 7;

        cout << "\nBall " << ball << ": ";
        cout << currentBatsman->name << " scores " << runs;

        if (runs == 0) {
            cout << " ➡ OUT!";
            currentBowler->wicketsTaken++;
            battingTeam->wicketsLost++;
            batsmanIndex++;
            if (batsmanIndex >= playersPerTeam) break;
            currentBatsman = battingTeam->players[batsmanIndex];
        } else {
            currentBatsman->runsScored += runs;
            currentBatsman->ballsPlayed++;
            battingTeam->totalRuns += runs;
        }

        currentBowler->ballsBowled++;
        currentBowler->runsGiven += runs;
        battingTeam->ballsBowled++;

        showScoreCard();

        if (currentInnings == 2 && battingTeam->totalRuns > bowlingTeam->totalRuns) {
            cout << "\n💥 " << battingTeam->teamName << " chased the target successfully!\n";
            break;
        }
    }
}

void Game::showScoreCard() {
    cout << "\nScorecard - " << battingTeam->teamName << ": "
         << battingTeam->totalRuns << "/" << battingTeam->wicketsLost
         << " in " << battingTeam->ballsBowled << " balls.\n";
}

void Game::showMatchSummary() {
    cout << "\n====================\n🏁 MATCH SUMMARY 🏁\n====================\n";
    cout << teamA->teamName << ": " << teamA->totalRuns << "/" << teamA->wicketsLost << endl;
    cout << teamB->teamName << ": " << teamB->totalRuns << "/" << teamB->wicketsLost << endl;

    if (teamA->totalRuns > teamB->totalRuns)
        cout << "🎉 " << teamA->teamName << " Wins!\n";
    else if (teamB->totalRuns > teamA->totalRuns)
        cout << "🎉 " << teamB->teamName << " Wins!\n";
    else
        cout << "🤝 Match Drawn!\n";
}
