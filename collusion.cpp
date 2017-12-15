#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <random>


using namespace std;

const int NUM_VOTERS = 1000;
const int NUM_CANDIDATES = 10;
const int NUM_COLLUDERS = 2;

struct voter {
    int p[NUM_CANDIDATES] = {0};
    int c[NUM_CANDIDATES] = {0};
    int cv;
    voter(int* a) {
        for (int i = 0; i < NUM_CANDIDATES; i++) {
            p[i] = a[i];
            cv = a[0];
            c[a[i]] = i;
        }
    }
    voter() {}
};

struct voteChange {
    int from, to, who;
    voteChange(int f, int t, int x) {
        from = f;
        to = t;
        who = x;
    }
    voteChange() {
        from = -1;
        to = -1;
        who = -1;
    }
    bool operator==(const voteChange & o) {
        return o.from == from && o.to == to;
    }
    bool operator!=(const voteChange & o) {
        return o.from != from || o.to != to;
    }
    void print() const {
        cout << "from: " << from << " to: " << to << endl;
    }
};

const voteChange noChange = voteChange(-1, -1, -1);



voter createUniformlyRandomVoter() {
    int a[100];
    for (int i = 0; i < NUM_CANDIDATES; i++) a[i] = i;
    random_device rand;
    mt19937 generator(rand());
    shuffle(a, a + NUM_CANDIDATES, generator);
    return voter(a);
}

void generateUniformlyRandomVoters(voter* v) {
    for (int i = 0; i < NUM_VOTERS; i++) {
        v[i] = createUniformlyRandomVoter();
    }
}

vector<pair<int, int> > rankVoters(voter* v) {
    vector<pair<int, int> > rankings(NUM_CANDIDATES);
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        rankings[i].first = 0;
        rankings[i].second = i;
    }
    for (int i = 0; i < NUM_VOTERS; i++) {
        rankings[v[i].cv].first++;
    }
    sort(rankings.begin(), rankings.end());
    reverse(rankings.begin(), rankings.end());
    return rankings;
}

int main() {

    voter voters[NUM_VOTERS];
    generateUniformlyRandomVoters(voters);
    vector<pair<int, int> > trueRankings = rankVoters(voters);
    vector<vector<int> > friendGraph(NUM_VOTERS);
    for (int i = 0; i < NUM_VOTERS; i++) {
        friendGraph[i].resize(NUM_VOTERS);
        for (int j = 0; j < NUM_VOTERS; j++) friendGraph[i][j] = 1;
    }
    bool notFinished = true;
    int cnt = 0;
    while (notFinished) {
        for (int i = 0; i < NUM_CANDIDATES; i++) {
            cout << trueRankings[i].second << " " << trueRankings[i].first << endl;
        }
        cout << "----------------------" << endl;
        cnt++;
        notFinished = false;
        bool hasColluded[NUM_VOTERS] = {0};
        for (int i = 0; i < NUM_VOTERS; i++) {
            if (hasColluded[i]) continue;
            vector<pair<int, int> > currRankings = trueRankings;
            int best = voters[i].c[currRankings[0].second];
            voteChange a = noChange;
            voteChange b = noChange;
            for (int j = i + 1; j < NUM_VOTERS; j++) {
                if (hasColluded[j]) continue;
                if (i == j) continue;
                if (!friendGraph[i][j]) continue;
                for (int k = 1; k < NUM_CANDIDATES; k++) {
                    currRankings = trueRankings;
                    if (currRankings[0].first - currRankings[k].first > 4) break;
                    if (best <= voters[i].c[currRankings[k].second]) continue;
                    currRankings[voters[i].cv].first--;
                    currRankings[k].first++;
                    if (currRankings[0].first < currRankings[k].first || (currRankings[0].first == currRankings[k].first && currRankings[k].second < currRankings[0].second)) {
                        a = voteChange(voters[i].cv, currRankings[k].second, i);
                        b = voteChange(-1, -1, -1);
                        best = voters[i].c[currRankings[k].second];
                        continue;
                    }
                    if (voters[j].c[currRankings[0].second] < voters[j].c[currRankings[k].second]) continue;
                    currRankings[voters[j].cv].first--;
                    currRankings[k].first++;
                    if (currRankings[0].first < currRankings[k].first || (currRankings[0].first == currRankings[k].first && currRankings[k].second < currRankings[0].second)) {
                        a = voteChange(voters[i].cv, currRankings[k].second, j);
                        b = voteChange(voters[j].cv, currRankings[k].second, j);
                        best = voters[i].c[currRankings[k].second];
                        continue;
                    }
                }
            }
            if (a != noChange) {
                notFinished = true;
                voters[i].cv = a.to;
                trueRankings = rankVoters(voters);
            }
            if (b != noChange) {
                voters[b.who].cv = b.to;
            }
        }
    }
    
}