#include "SocialNetwork.h"
#include <algorithm>
#include <cmath>
#include <cctype>

// User class methods
User::User(int id, string uname, string f, string l, string s, string loc, int age, vector<string> h)
    : id(id), username(uname), firstName(f), lastName(l), schoolName(s), locality(loc), age(age), hobbies(h) {}

void User::displayProfile()
{
    cout << "\n--- User Profile ---\n";
    cout << "username: " << username << "\n";
    cout << "Name: " << firstName << " " << lastName << "\n";
    cout << "Age: " << age << "\n";
    cout << "School/College: " << schoolName << "\n";
    cout << "Locality: " << locality << "\n";
    cout << "Hobbies: ";

    for (int i = 0; i < hobbies.size(); ++i)
    {
        cout << hobbies[i];

        if (i != hobbies.size() - 1)
            cout << ", ";
    }

    cout << "\n--------------------\n";
}


// SocialNetwork class methods
SocialNetwork::SocialNetwork() {}


// ==============================
// LOAD USERS
// ==============================

void SocialNetwork::loadUsers()
{
    // IMPORTANT:
    // Program is executed from FriendBook root folder.
    ifstream file("data/users.csv");

    if (!file.is_open())
    {
        cout << "Error opening users.csv!" << endl;
        return;
    }

    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string idStr;
        string username;
        string password;
        string fName;
        string lName;
        string college;
        string locality;
        string ageStr;
        string hobbiesStr;

        getline(ss, idStr, ',');
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, fName, ',');
        getline(ss, lName, ',');
        getline(ss, college, ',');
        getline(ss, locality, ',');
        getline(ss, ageStr, ',');
        getline(ss, hobbiesStr);

        int id;
        int age;

        try
        {
            id = stoi(idStr);
            age = stoi(ageStr);
        }
        catch (...)
        {
            cout << "Invalid Line\n";
            continue;
        }

        // Split hobbies by comma
        vector<string> hobbies;

        string hobby;
        stringstream hss(hobbiesStr);

        while (getline(hss, hobby, ','))
        {
            if (!hobby.empty())
                hobbies.push_back(hobby);
        }

        User u(
            id,
            username,
            fName,
            lName,
            college,
            locality,
            age,
            hobbies
        );

        u.password = password;

        users[id] = u;
        usernameToId[username] = id;
    }

    file.close();

    cout << "Loaded "
         << users.size()
         << " users successfully.\n";
}


// ==============================
// LOAD FRIENDS
// ==============================

void SocialNetwork::loadFriends()
{
    // IMPORTANT:
    // Program is executed from FriendBook root folder.
    ifstream file("data/friends.csv");

    if (!file.is_open())
    {
        cout << "Couldn't open friends.csv";
        return;
    }

    string line;

    // Skip header
    getline(file, line);

    while (getline(file, line))
    {
        if (line.empty())
            continue;

        stringstream ss(line);

        string userIdStr;
        string fIdStr;
        string weightStr;

        getline(ss, userIdStr, ',');
        getline(ss, fIdStr, ',');
        getline(ss, weightStr);

        try
        {
            int userId = stoi(userIdStr);
            int fId = stoi(fIdStr);
            int weight = stoi(weightStr);

            // Add connection
            friends[userId].push_back({fId, weight});

            // Add reverse connection
            friends[fId].push_back({userId, weight});
        }
        catch (...)
        {
            cout << "Invalid line: "
                 << line
                 << endl;
        }
    }

    file.close();
}


// ==============================
// SAVE USERS
// ==============================

void SocialNetwork::saveUsers()
{
    // IMPORTANT:
    // Save inside FriendBook/data
    ofstream file("data/users.csv");

    if (!file.is_open())
    {
        cout << "Error saving users!\n";
        return;
    }

    file << "id,username,password,firstName,lastName,college,locality,age,hobbies\n";

    for (auto &entry : users)
    {
        const User &u = entry.second;

        file << u.id << ","
             << u.username << ","
             << u.password << ","
             << u.firstName << ","
             << u.lastName << ","
             << u.schoolName << ","
             << u.locality << ","
             << u.age << ",";

        for (size_t i = 0; i < u.hobbies.size(); i++)
        {
            file << u.hobbies[i];

            if (i != u.hobbies.size() - 1)
                file << ",";
        }

        file << "\n";
    }

    file.close();

    cout << "Users saved successfully\n";
}


// ==============================
// SAVE FRIENDS
// ==============================

void SocialNetwork::saveFriends()
{
    // IMPORTANT:
    // Save inside FriendBook/data
    ofstream file("data/friends.csv");

    if (!file.is_open())
    {
        cout << "Error saving friends!\n";
        return;
    }

    file << "user_id,friend_id,weight\n";

    // Avoid duplicate connections
    set<pair<int, int>> writtenPairs;

    for (auto &entry : friends)
    {
        int userId = entry.first;

        for (auto &p : entry.second)
        {
            int friendId = p.first;
            int weight = p.second;

            if (writtenPairs.count({friendId, userId}) == 0)
            {
                file << userId << ","
                     << friendId << ","
                     << weight << "\n";

                writtenPairs.insert({userId, friendId});
            }
        }
    }

    file.close();

    cout << "Friends saved successfully.\n";
}


// ==============================
// REGISTER USER
// ==============================

void SocialNetwork::registerUser(User A)
{
    if (usernameToId.count(A.username))
    {
        cout << "Username '"
             << A.username
             << "' already exists. Try another.\n";
        return;
    }

    A.id = getNextUserId();

    users[A.id] = A;
    usernameToId[A.username] = A.id;

    cout << "User registered successfully with ID: "
         << A.id
         << "\n";
}


// ==============================
// GET NEXT USER ID
// ==============================

int SocialNetwork::getNextUserId()
{
    if (users.empty())
        return 1;

    int maxId = 0;

    for (const auto &entry : users)
    {
        if (entry.first > maxId)
            maxId = entry.first;
    }

    return maxId + 1;
}


// ==============================
// LOGIN USER
// ==============================

int SocialNetwork::loginUser(const string &usernme)
{
    if (!usernameToId.count(usernme))
    {
        cout << "Username not found\n";
        return -1;
    }

    int userId = usernameToId[usernme];

    User &u = users[userId];

    cin.ignore();

    string inputPassword;

    cout << "Enter password: ";

    getline(cin, inputPassword);

    if (inputPassword == u.password)
    {
        cout << "Login successful\n";
        return userId;
    }
    else
    {
        cout << "Incorrect Password\n";
        return -1;
    }
}


// ==============================
// DISPLAY CURRENT FRIENDS
// ==============================

void SocialNetwork::displayCurrentFriends(int userId)
{
    if (!friends.count(userId) ||
        friends[userId].empty())
    {
        cout << "You have no friends yet.\n";
        return;
    }

    cout << "\n--Friend List--\n";

    int count = 1;

    for (auto &p : friends[userId])
    {
        int friendId = p.first;
        int weight = p.second;

        if (!users.count(friendId))
            continue;

        User &f = users[friendId];

        cout << count
             << "."
             << f.username
             << " ("
             << f.firstName
             << " "
             << f.lastName
             << ") "
             << " | Age: "
             << f.age
             << " | City: "
             << f.locality
             << " | Friendship weight: "
             << weight
             << "\n";

        count++;
    }

    cout << "----------------\n";
}


// ==============================
// VIEW PROFILE
// ==============================

void SocialNetwork::viewProfile(int userID)
{
    if (!users.count(userID))
    {
        cout << "User not found.\n";
        return;
    }

    users[userID].displayProfile();
}


// ==============================
// ADD FRIEND
// ==============================

void SocialNetwork::addFriend(User &A, User &B)
{
    int userId = A.id;
    int friendId = B.id;

    for (auto &p : friends[userId])
    {
        if (p.first == friendId)
        {
            cout << "Already friends.\n";
            return;
        }
    }

    friends[userId].push_back({friendId, 1});
    friends[friendId].push_back({userId, 1});

    cout << "You and "
         << B.username
         << " are now friends!\n";
}


// ==============================
// INTERACT
// ==============================

void SocialNetwork::interact(User A, User B)
{
    int userId = A.id;
    int friendId = B.id;

    bool found = false;

    for (auto &p : friends[userId])
    {
        if (p.first == friendId)
        {
            p.second += 2;
            found = true;
            break;
        }
    }

    for (auto &p : friends[friendId])
    {
        if (p.first == userId)
        {
            p.second += 1;
            break;
        }
    }

    if (!found)
    {
        cout << "You and "
             << B.username
             << " are not friends yet. Add them first.\n";
    }
    else
    {
        cout << "You interacted with "
             << B.username
             << ". Friendship weight increased.\n";
    }
}


// ==============================
// GET USER ID BY USERNAME
// ==============================

int SocialNetwork::getUserIdByUsername(const string &uname)
{
    if (usernameToId.count(uname))
        return usernameToId[uname];

    return -1;
}


// ==============================
// GET USER
// ==============================

User &SocialNetwork::getUser(int id)
{
    return users[id];
}


// ==============================
// JACCARD SIMILARITY
// ==============================

double SocialNetwork::jaccardSimilarity(
    const vector<string> &h1,
    const vector<string> &h2)
{
    if (h1.empty() && h2.empty())
        return 0.0;

    if (h1.empty() || h2.empty())
        return 0.0;

    unordered_set<string> set1(
        h1.begin(),
        h1.end()
    );

    unordered_set<string> set2(
        h2.begin(),
        h2.end()
    );

    int intersection = 0;

    for (const string &h : set1)
    {
        if (set2.count(h))
            intersection++;
    }

    int unionSize =
        set1.size()
        + set2.size()
        - intersection;

    if (unionSize == 0)
        return 0.0;

    return (double)intersection / unionSize;
}


// ==============================
// LAST NAME SIMILARITY
// ==============================

double SocialNetwork::lastNameSimilarity(
    const string &name1,
    const string &name2)
{
    if (name1.empty() || name2.empty())
        return 0.0;

    string n1 = name1;
    string n2 = name2;

    transform(
        n1.begin(),
        n1.end(),
        n1.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(
                std::tolower(c)
            );
        }
    );

    transform(
        n2.begin(),
        n2.end(),
        n2.begin(),
        [](unsigned char c)
        {
            return static_cast<char>(
                std::tolower(c)
            );
        }
    );

    // Exact match
    if (n1 == n2)
        return 1.0;

    // Partial match
    if (n1.find(n2) != string::npos ||
        n2.find(n1) != string::npos)
    {
        return 0.6;
    }

    // Character-based similarity
    int commonChars = 0;

    int minLen =
        static_cast<int>(
            min(n1.length(), n2.length())
        );

    int maxLen =
        static_cast<int>(
            max(n1.length(), n2.length())
        );

    for (int i = 0; i < minLen; i++)
    {
        if (n1[i] == n2[i])
            commonChars++;
    }

    if (maxLen == 0)
        return 0.0;

    return (double)commonChars / maxLen;
}


// ==============================
// COMPUTE SIMILARITY
// ==============================

double SocialNetwork::computeSimilarity(
    const User &A,
    const User &B)
{
    double score = 0.0;

    // Same College - weight: 45
    if (A.schoolName == B.schoolName &&
        !A.schoolName.empty())
    {
        score += 45.0;
    }

    // Same Locality - weight: 40
    if (A.locality == B.locality &&
        !A.locality.empty())
    {
        score += 40.0;
    }

    // Hobbies similarity - weight up to 20
    if (!A.hobbies.empty() &&
        !B.hobbies.empty())
    {
        double hobbySim =
            jaccardSimilarity(
                A.hobbies,
                B.hobbies
            );

        score += hobbySim * 20.0;
    }

    // Age similarity - weight up to 10
    int ageDiff =
        abs(A.age - B.age);

    if (ageDiff == 0)
        score += 10.0;
    else if (ageDiff <= 2)
        score += 7.0;
    else if (ageDiff <= 5)
        score += 4.0;
    else if (ageDiff <= 10)
        score += 2.0;
    else if (ageDiff <= 15)
        score += 1.0;

    // Last name similarity - weight up to 5
    if (!A.lastName.empty() &&
        !B.lastName.empty())
    {
        double nameSim =
            lastNameSimilarity(
                A.lastName,
                B.lastName
            );

        score += nameSim * 5.0;
    }

    return score;
}


// ==============================
// DIJKSTRA SHORTEST PATHS
// ==============================

unordered_map<int, double>
SocialNetwork::dijkstraShortestPaths(
    int source,
    int maxDepth)
{
    unordered_map<int, double> distances;

    set<pair<double, int>> pq;

    distances[source] = 0.0;

    pq.insert({0.0, source});

    while (!pq.empty() &&
           distances.size() < users.size() * 2)
    {
        auto it = pq.begin();

        double dist = it->first;
        int curr = it->second;

        pq.erase(it);

        if (dist > maxDepth)
            continue;

        if (!friends.count(curr))
            continue;

        for (auto &neighbor : friends[curr])
        {
            int neighborId = neighbor.first;
            int edgeWeight = neighbor.second;

            double edgeDistance =
                1.0 / (1.0 + edgeWeight);

            double newDist =
                dist + edgeDistance;

            if (!distances.count(neighborId) ||
                distances[neighborId] > newDist)
            {
                if (distances.count(neighborId))
                {
                    pq.erase({
                        distances[neighborId],
                        neighborId
                    });
                }

                distances[neighborId] = newDist;

                pq.insert({
                    newDist,
                    neighborId
                });
            }
        }
    }

    return distances;
}


// ==============================
// WEIGHTED PATH SCORE
// ==============================

double SocialNetwork::calculateWeightedPathScore(
    int userA,
    int userB)
{
    if (userA == userB)
        return 0.0;

    // Check direct friendship
    if (friends.count(userA))
    {
        for (auto &p : friends[userA])
        {
            if (p.first == userB)
            {
                return 10.0 /
                       (1.0 + p.second);
            }
        }
    }

    // Dijkstra
    auto distances =
        dijkstraShortestPaths(
            userA,
            3
        );

    if (!distances.count(userB))
        return 0.0;

    double pathDistance =
        distances[userB];

    return 10.0 /
           (1.0 + pathDistance);
}


// ==============================
// COUNT MUTUAL FRIENDS
// ==============================

int SocialNetwork::countMutualFriends(
    int userA,
    int userB)
{
    if (!friends.count(userA) ||
        !friends.count(userB))
    {
        return 0;
    }

    unordered_set<int> friendsOfA;

    for (auto &p : friends[userA])
    {
        friendsOfA.insert(p.first);
    }

    int mutualCount = 0;

    for (auto &p : friends[userB])
    {
        if (friendsOfA.count(p.first))
        {
            mutualCount++;
        }
    }

    return mutualCount;
}


// ==============================
// MUTUAL FRIENDS SCORE
// ==============================

double SocialNetwork::calculateMutualFriendsScore(
    int userId,
    int candidateId,
    int mutualCount)
{
    if (mutualCount == 0)
        return 0.0;

    double avgMutualWeight = 0.0;

    int mutualWithWeight = 0;

    if (friends.count(userId) &&
        friends.count(candidateId))
    {
        unordered_set<int> friendsOfUser;

        for (auto &p : friends[userId])
        {
            friendsOfUser.insert(p.first);
        }

        for (auto &p : friends[candidateId])
        {
            if (friendsOfUser.count(p.first))
            {
                for (auto &p2 : friends[userId])
                {
                    if (p2.first == p.first)
                    {
                        avgMutualWeight +=
                            (p.second + p2.second)
                            / 2.0;

                        mutualWithWeight++;

                        break;
                    }
                }
            }
        }
    }

    if (mutualWithWeight > 0)
    {
        avgMutualWeight /=
            mutualWithWeight;
    }

    return mutualCount * 4.0
           + avgMutualWeight * 0.3;
}


// ==============================
// TRIADIC CLOSURE SCORE
// ==============================

double SocialNetwork::calculateTriadicClosureScore(
    int candidateId,
    const unordered_map<int, double>
        &weightedPaths)
{
    if (!weightedPaths.count(candidateId))
        return 0.0;

    double pathDist =
        weightedPaths.at(candidateId);

    if (pathDist <= 2.0)
    {
        return 5.0 /
               (1.0 + pathDist);
    }

    return 0.0;
}


// ==============================
// DIRECT CONNECTION SCORE
// ==============================

double SocialNetwork::calculateDirectConnectionScore(
    int userId,
    int candidateId)
{
    if (!friends.count(userId))
        return 0.0;

    for (auto &p : friends[userId])
    {
        if (friends.count(p.first))
        {
            for (auto &p2 : friends[p.first])
            {
                if (p2.first == candidateId)
                {
                    double connectionStrength =
                        (p.second + p2.second)
                        / 2.0;

                    return connectionStrength * 2.0;
                }
            }
        }
    }

    return 0.0;
}


// ==============================
// NEW USER SCORE
// ==============================

double SocialNetwork::calculateNewUserScore(
    const User &currentUser,
    const User &candidate)
{
    double totalScore = 0.0;

    double profileScore =
        computeSimilarity(
            currentUser,
            candidate
        );

    totalScore += profileScore;

    // Same college boost
    if (currentUser.schoolName ==
            candidate.schoolName &&
        !currentUser.schoolName.empty())
    {
        totalScore += 18.0;
    }

    // Same locality boost
    if (currentUser.locality ==
            candidate.locality &&
        !currentUser.locality.empty())
    {
        totalScore += 17.0;
    }

    return totalScore;
}


// ==============================
// EXISTING USER SCORE
// ==============================

double SocialNetwork::calculateExistingUserScore(
    int userId,
    int candidateId,
    const User &currentUser,
    const User &candidate,
    const unordered_map<int, double>
        &weightedPaths)
{
    double totalScore = 0.0;

    // Profile similarity
    double profileScore =
        computeSimilarity(
            currentUser,
            candidate
        );

    totalScore +=
        profileScore * 2.0;

    // Weighted graph path
    double pathScore =
        calculateWeightedPathScore(
            userId,
            candidateId
        );

    totalScore +=
        pathScore * 3.0;

    // Mutual friends
    int mutualCount =
        countMutualFriends(
            userId,
            candidateId
        );

    if (mutualCount > 0)
    {
        totalScore +=
            calculateMutualFriendsScore(
                userId,
                candidateId,
                mutualCount
            );
    }

    // Triadic closure
    totalScore +=
        calculateTriadicClosureScore(
            candidateId,
            weightedPaths
        );

    // Direct connection
    totalScore +=
        calculateDirectConnectionScore(
            userId,
            candidateId
        );

    return totalScore;
}


// ==============================
// RANK CANDIDATES
// ==============================

vector<int>
SocialNetwork::rankAndReturnTopCandidates(
    const unordered_map<int, double>
        &scores,
    int limit)
{
    vector<pair<double, int>> ranked;

    for (auto &entry : scores)
    {
        ranked.push_back({
            entry.second,
            entry.first
        });
    }

    sort(
        ranked.begin(),
        ranked.end(),
        [](const pair<double, int> &a,
           const pair<double, int> &b)
        {
            if (a.first != b.first)
                return a.first > b.first;

            return a.second < b.second;
        }
    );

    vector<int> result;

    int actualLimit =
        min(
            limit,
            static_cast<int>(ranked.size())
        );

    for (int i = 0;
         i < actualLimit;
         i++)
    {
        result.push_back(
            ranked[i].second
        );
    }

    return result;
}


// ==============================
// SUGGESTED FRIENDS
// ==============================

vector<int>
SocialNetwork::suggestedFriends(int userId)
{
    vector<int> result;

    if (!users.count(userId))
    {
        cout << "Invalid userID\n";
        return result;
    }

    const User &currentUser =
        users[userId];

    // Check if user is new
    bool isNewUser =
        !friends.count(userId) ||
        friends[userId].empty();

    // Exclude current user
    // and existing friends
    unordered_set<int> excluded;

    excluded.insert(userId);

    if (friends.count(userId))
    {
        for (auto &p : friends[userId])
        {
            excluded.insert(p.first);
        }
    }

    unordered_map<int, double>
        comprehensiveScore;

    // ==========================
    // NEW USER
    // ==========================

    if (isNewUser)
    {
        for (auto &entry : users)
        {
            int candidateId =
                entry.first;

            if (excluded.count(candidateId))
                continue;

            const User &candidate =
                entry.second;

            comprehensiveScore[candidateId] =
                calculateNewUserScore(
                    currentUser,
                    candidate
                );
        }
    }

    // ==========================
    // EXISTING USER
    // ==========================

    else
    {
        auto weightedPaths =
            dijkstraShortestPaths(
                userId,
                3
            );

        for (auto &entry : users)
        {
            int candidateId =
                entry.first;

            if (excluded.count(candidateId))
                continue;

            const User &candidate =
                entry.second;

            comprehensiveScore[candidateId] =
                calculateExistingUserScore(
                    userId,
                    candidateId,
                    currentUser,
                    candidate,
                    weightedPaths
                );
        }
    }

    // Top 10 suggestions
    return rankAndReturnTopCandidates(
        comprehensiveScore,
        10
    );
}


// ==============================
// DELETE USER
// ==============================

bool SocialNetwork::deleteUser(int userId)
{
    if (!users.count(userId))
    {
        return false;
    }

    // Save username before erasing user
    string username =
        users[userId].username;

    // Remove user from users map
    users.erase(userId);

    // Remove username mapping
    usernameToId.erase(username);

    // Remove user from their own
    // friendship list
    friends.erase(userId);

    // Remove deleted user from
    // every remaining user's list
    for (auto &entry : friends)
    {
        auto &friendList =
            entry.second;

        friendList.erase(
            std::remove_if(
                friendList.begin(),
                friendList.end(),
                [userId](
                    const std::pair<int, int> &p)
                {
                    return p.first == userId;
                }
            ),
            friendList.end()
        );
    }

    return true;
}