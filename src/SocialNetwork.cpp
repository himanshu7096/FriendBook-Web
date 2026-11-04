#include "SocialNetwork.h"
#include <algorithm>
#include <cmath>

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

void SocialNetwork::loadUsers(){
    ifstream file("../data/users.csv");
    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return ;
    }
    string line;
    getline(file, line);
    while(getline(file,line)){
        if(line.empty())    continue;

        stringstream ss(line);
        string idStr, username, password, fName, lName, college, locality, ageStr, hobbiesStr;
        getline(ss, idStr, ',');
        getline(ss,username,',');
        getline(ss,password,',');
        getline(ss,fName,',');
        getline(ss,lName,',');
        getline(ss,college,',');
        getline(ss,locality,',');
        getline(ss,ageStr,',');
        getline(ss, hobbiesStr);
        int id,age;
        try{
            id = stoi(idStr);
            age = stoi(ageStr);
        }
        catch (...) {
            cout<< "Invalid Line\n";
            continue; 
        } 

        //split hobbies by comma
        vector<string>hobbies;
        string hobby;
        stringstream hss(hobbiesStr);
        while(getline(hss,hobby,',')){
            if (!hobby.empty())
            hobbies.push_back(hobby);
        }
        User u(id,username,fName,lName,college,locality,age,hobbies);
        u.password = password;
        users[id] = u;
        usernameToId[username] = id;
    }
    file.close();
    cout << " Loaded " << users.size() << " users successfully.\n";
}

void SocialNetwork::loadFriends(){
   ifstream file("../data/friends.csv"); 
   if(!file.is_open()){
        cout<<"couldn't open friends.csv";
        return;
   }
   string line;
   getline(file,line);  // skip first line

   while(getline(file,line)){
        if(line.empty())    continue;

        stringstream ss(line);
        string userIdStr, fIdStr, weightStr;
        getline(ss,userIdStr,',');
        getline(ss,fIdStr,',');
        getline(ss,weightStr);

        try {
            int userId = stoi(userIdStr);
            int fId = stoi(fIdStr);
            int weight = stoi(weightStr);

            // add connection
            friends[userId].push_back({fId, weight});
            friends[fId].push_back({userId, weight}); // only if CSV is one-directional
        }
        catch (...) {
            cout << " invalid line: " << line << endl;
        }
    }
   file.close();
}

void SocialNetwork::saveUsers(){
    ofstream file("../data/users.csv");
    if(!file.is_open()){
        cout<<"Error saving users!\n";
        return;
    }
    file<<"id,username,password,firstName,lastName,college,locality,age,hobbies\n";

    for(auto &entry : users){
        const User &u = entry.second;

        file<<u.id<<","
            <<u.username<<","
            <<u.password<<","
            <<u.firstName<<","
            <<u.lastName<<","
            <<u.schoolName<<","
            <<u.locality<<","
            <<u.age<<",";
            
        for(size_t i=0;i<u.hobbies.size();i++){
            file<<u.hobbies[i];
            if(i!=u.hobbies.size()-1)   file<<",";
        }
        file<<"\n";
    }
    file.close();
    cout<<"Users saved succesfully"<<"\n";
}

void SocialNetwork::saveFriends(){
   ofstream file("../data/friends.csv");
    if(!file.is_open()){
        cout<<"Error saving friends!\n";
        return;
    }
    file<<"user_id,friend_id,weight\n";
    
    //to avoid duplicates
    set<pair<int,int>> writtenPairs;
    for(auto &entry : friends){
        int userId = entry.first;
        for(auto &pair : entry.second){
            int friendId = pair.first;
            int weight = pair.second;

            if(writtenPairs.count({friendId,userId})==0){
                file<<userId<<","<<friendId<<","<<weight<<"\n";
                writtenPairs.insert({userId,friendId});
            }
        }
    }
    file.close();
    cout<<"Friends saved successfully.\n";
}

void SocialNetwork::registerUser(User A){
    if(usernameToId.count(A.username)){
        cout<<"Username '"<<A.username<<"' already exists. Try another.\n ";
        return;
    }

    A.id = getNextUserId();

    users[A.id] = A;

    usernameToId[A.username] = A.id;

    cout<<"User registered succesfully with ID: "<<A.id<<"\n";
}

int SocialNetwork::getNextUserId(){
    if(users.empty()) return 1;
    int maxId = 0;
    for(const auto &entry : users){
        if(entry.first>maxId)   maxId = entry.first;
    }
    return maxId + 1;
}

int SocialNetwork::loginUser(const string &usernme){
    if(!usernameToId.count(usernme)){
        cout<<"Username not found\n";
        return -1;
    }

    int userId = usernameToId[usernme];
    User &u = users[userId];

    //ask for password
    cin.ignore();
    string inputPassword;

    cout<<"Enter password: ";

    getline(cin,inputPassword);
    
    if(inputPassword==u.password){
        cout<<"Login successful\n";
        return userId;
    }
    else{
        cout<<"Incorrect Password\n";
        return -1;
    }
}

void SocialNetwork::displayCurrentFriends(int userId){

    if(!friends.count(userId)||friends[userId].empty()){
        cout<<"You have no friends yet.\n";
        return;
    }

    cout<<"\n--Friend List--\n";
    int count = 1;
    for(auto &pair : friends[userId]){
        int friendId = pair.first;
        int weight = pair.second;
        if(!users.count(friendId))  continue;

        User &f = users[friendId];
        cout<<count<<"."<<f.username    
            <<" (" << f.firstName << " " << f.lastName << ") "
            << " | Age: " << f.age
            << " | City: " << f.locality
            << " | Friendship weight: " << weight<< "\n";
            
        count++;
    }
    cout<<"----------------\n";
}

void SocialNetwork::viewProfile(int userID){
    users[userID].displayProfile();
}

void SocialNetwork::addFriend(User& A, User& B){
    int userId = A.id;
    int friendId = B.id;

    for(auto &pair : friends[userId]){
        if(pair.first == friendId){
            cout<<"Already friends.\n";
            return;
        }
    }

    friends[userId].push_back({friendId,1});
    friends[friendId].push_back({userId,1});
    
    cout<<"You and "<<B.username<<" are now friends!\n";
}

void SocialNetwork::interact(User A, User B){
    int userId = A.id;
    int friendId = B.id;

    bool found = false;
    
    for(auto &pair: friends[userId]){
        if(pair.first == friendId){
            pair.second += 2;
            found  = true;
            break;
        }
    }

    for(auto &pair : friends[friendId]){
        if(pair.first == userId){
            pair.second += 1;
            break;
        }
    }

    if(!found){
        cout << "You and " << B.username << " are not friends yet. Add them first.\n";
    }
    else {
        cout <<"You interacted with " << B.username << ". Friendship weight increased.\n";
    }
}

int SocialNetwork::getUserIdByUsername(const string &uname) {
    if (usernameToId.count(uname)) return usernameToId[uname];
    return -1;
}

User& SocialNetwork::getUser(int id) {
    return users[id];
}

// Jaccard Similarity for hobbies: intersection/union
double SocialNetwork::jaccardSimilarity(const vector<string> &h1, const vector<string> &h2){
    if(h1.empty() && h2.empty()) return 0.0;
    if(h1.empty() || h2.empty()) return 0.0;
    
    unordered_set<string> set1(h1.begin(), h1.end());
    unordered_set<string> set2(h2.begin(), h2.end());
    
    int intersection = 0;
    for(const string &h : set1){
        if(set2.count(h)) intersection++;
    }
    
    int unionSize = set1.size() + set2.size() - intersection;
    if(unionSize == 0) return 0.0;
    
    return (double)intersection / unionSize;
}

// Lastname similarity using character-based matching
double SocialNetwork::lastNameSimilarity(const string &name1, const string &name2){
    if(name1.empty() || name2.empty()) return 0.0;
    
    string n1 = name1, n2 = name2;
    // Convert to lowercase for comparison
    transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
    transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
    
    // Exact match
    if(n1 == n2) return 1.0;
    
    // Check if one is substring of another (partial match)
    if(n1.find(n2) != string::npos || n2.find(n1) != string::npos){
        return 0.6;
    }
    
    // Character-based similarity (simple Levenshtein-like)
    int commonChars = 0;
    int minLen = min(n1.length(), n2.length());
    int maxLen = max(n1.length(), n2.length());
    
    for(int i = 0; i < minLen; i++){
        if(n1[i] == n2[i]) commonChars++;
    }
    
    if(maxLen == 0) return 0.0;
    return (double)commonChars / maxLen;
}

double SocialNetwork::computeSimilarity(const User &A, const User &B){
    double score = 0.0;

    //  Same College - weight: 45 (HIGHEST PRIORITY)
    if(A.schoolName == B.schoolName && !A.schoolName.empty()){
        score += 45.0;
    }

    //  Same Locality - weight: 40 (SECOND PRIORITY, comparable to college)
    if(A.locality == B.locality && !A.locality.empty()){
        score += 40.0;
    }

    //  Hobbies similarity  weight: up to 20.0 
    if(!A.hobbies.empty() && !B.hobbies.empty()){
        double hobbySim = jaccardSimilarity(A.hobbies, B.hobbies);
        score += hobbySim * 20.0;  
    }
    
    //  Age similarity - weight: up to 10.0 
    int ageDiff = abs(A.age - B.age);
    if(ageDiff == 0)      score += 10.0;
    else if(ageDiff <= 2) score += 7.0;
    else if(ageDiff <= 5) score += 4.0;
    else if(ageDiff <= 10) score += 2.0;
    else if(ageDiff <= 15) score += 1.0;
    
    // Lastname similarity- weight: up to 5.0
    if(!A.lastName.empty() && !B.lastName.empty()){
        double nameSim = lastNameSimilarity(A.lastName, B.lastName);
        score += nameSim * 5.0; 
    }

    return score;
}

// Dijkstra's algorithm for weighted shortest paths in the friendship graph
unordered_map<int, double> SocialNetwork::dijkstraShortestPaths(int source, int maxDepth){
    unordered_map<int, double> distances;
    set<pair<double, int>> pq;  // {distance, node}
    
    distances[source] = 0.0;
    pq.insert({0.0, source});
    
    while(!pq.empty() && distances.size() < users.size() * 2){
        auto it = pq.begin();
        double dist = it->first;
        int curr = it->second;
        pq.erase(it);
        
        // Limit depth to avoid exploring too far
        if(dist > maxDepth) continue;
        
        if(!friends.count(curr)) continue;
        
        for(auto &neighbor : friends[curr]){
            int neighborId = neighbor.first;
            int edgeWeight = neighbor.second;
            
            //  inverse: distance = 1 / (1 + weight)
            double edgeDistance = 1.0 / (1.0 + edgeWeight);
            double newDist = dist + edgeDistance;
            
            if(!distances.count(neighborId) || distances[neighborId] > newDist){
                // Remove old entry if exists
                if(distances.count(neighborId)){
                    pq.erase({distances[neighborId], neighborId});
                }
                distances[neighborId] = newDist;
                pq.insert({newDist, neighborId});
            }
        }
    }
    
    return distances;
}

// Calculate weighted path score between two users
double SocialNetwork::calculateWeightedPathScore(int userA, int userB){
    if(userA == userB) return 0.0;
    
    // Check if they are direct friends
    if(friends.count(userA)){
        for(auto &p : friends[userA]){
            if(p.first == userB){
                // Direct friend: return inverse of weight (higher weight = better)
                return 10.0 / (1.0 + p.second);
            }
        }
    }
    
    // Use Dijkstra to find shortest weighted path
    auto distances = dijkstraShortestPaths(userA, 3);  // Max depth 3
    
    if(!distances.count(userB)){
        return 0.0;  // No path found
    }
    
    double pathDistance = distances[userB];
   
    return 10.0 / (1.0 + pathDistance);
}

int SocialNetwork::countMutualFriends(int userA, int userB){
    if(!friends.count(userA)||!friends.count(userB)){
        return 0;
    }
    
    unordered_set<int> friendsOfA;
    for(auto &p:friends[userA]){
        friendsOfA.insert(p.first);
    }

    int mutualCount = 0;

    for(auto &p : friends[userB]){
        if(friendsOfA.count(p.first)){
            mutualCount++;
        }
    }
    return mutualCount;
}

double SocialNetwork::calculateMutualFriendsScore(int userId, int candidateId, int mutualCount){
    if(mutualCount == 0) return 0.0;
    
    // Calculate average weight of mutual connections
    double avgMutualWeight = 0.0;
    int mutualWithWeight = 0;
    
    if(friends.count(userId) && friends.count(candidateId)){
        unordered_set<int> friendsOfUser;
        for(auto &p : friends[userId]){
            friendsOfUser.insert(p.first);
        }
        
        for(auto &p : friends[candidateId]){
            if(friendsOfUser.count(p.first)){
                // Find weight from user to mutual friend
                for(auto &p2 : friends[userId]){
                    if(p2.first == p.first){
                        avgMutualWeight += (p.second + p2.second) / 2.0;
                        mutualWithWeight++;
                        break;
                    }
                }
            }
        }
    }
    
    if(mutualWithWeight > 0){
        avgMutualWeight /= mutualWithWeight;
    }
    
    // Mutual friends score: count * 4.0 + average weight bonus
    return mutualCount * 4.0 + avgMutualWeight * 0.3;
}

// Calculate triadic closure score (friends of friends principle)
double SocialNetwork::calculateTriadicClosureScore(int candidateId, const unordered_map<int, double> &weightedPaths){
    if(!weightedPaths.count(candidateId)) return 0.0;
    
    double pathDist = weightedPaths.at(candidateId);
    if(pathDist <= 2.0){  // Within 2 hops
        return 5.0 / (1.0 + pathDist);
    }
    
    return 0.0;
}

double SocialNetwork::calculateDirectConnectionScore(int userId, int candidateId){
    if(!friends.count(userId)) return 0.0;
    
    for(auto &p : friends[userId]){
        if(friends.count(p.first)){
            for(auto &p2 : friends[p.first]){
                if(p2.first == candidateId){
                    // Friend of friend: boost based on connection strength
                    double connectionStrength = (p.second + p2.second) / 2.0;
                    return connectionStrength * 2.0;
                }
            }
        }
    }
    
    return 0.0;
}

// Calculate score for new users (profile-based only)
double SocialNetwork::calculateNewUserScore(const User &currentUser, const User &candidate){
    double totalScore = 0.0;
    
    // Profile Similarity gets full weight for new users
    // Priority: College > Locality > Hobbies > Age > LastName
    double profileScore = computeSimilarity(currentUser, candidate);
    totalScore += profileScore;  // Full weight for profile match (no multiplier needed)
    
    // Additional boost for new users with same college or locality
    if(currentUser.schoolName == candidate.schoolName && !currentUser.schoolName.empty()){
        totalScore += 18.0;  // Extra boost for same college
    }
    if(currentUser.locality == candidate.locality && !currentUser.locality.empty()){
        totalScore += 17.0;  // Extra boost for same locality (comparable to college)
    }
    
    return totalScore;
}

// Calculate score for existing users (graph-based + profile-based)
double SocialNetwork::calculateExistingUserScore(int userId, int candidateId, const User &currentUser, const User &candidate, const unordered_map<int, double> &weightedPaths){
    double totalScore = 0.0;
    
    // Profile Similarity (college, locality, age, hobbies, lastname)
    double profileScore = computeSimilarity(currentUser, candidate);
    totalScore += profileScore * 2.0;  
    
    // Weighted Graph Path Score using Dijkstra's algorithm
    double pathScore = calculateWeightedPathScore(userId, candidateId);
    totalScore += pathScore * 3.0;  // Very high weight for graph connections
    
    // Mutual Friends with weighted consideration
    int mutualCount = countMutualFriends(userId, candidateId);
    if(mutualCount > 0){
        totalScore += calculateMutualFriendsScore(userId, candidateId, mutualCount);
    }
    
    // Triadic Closure: If candidate is a friend of a friend, boost the score
    totalScore += calculateTriadicClosureScore(candidateId, weightedPaths);
    
    // Direct connection strength (friend of friend)
    totalScore += calculateDirectConnectionScore(userId, candidateId);
    
    return totalScore;
}

vector<int> SocialNetwork::rankAndReturnTopCandidates(const unordered_map<int, double> &scores, int limit){
    vector<pair<double, int>> ranked;
    for(auto& entry : scores){
        ranked.push_back({entry.second, entry.first});
    }
    
    sort(ranked.begin(), ranked.end(), [&](auto &a, auto &b){
        return a.first > b.first;
    });
    
    vector<int> result;
    int actualLimit = min(limit, (int)ranked.size());
    for(int i = 0; i < actualLimit; i++){
        result.push_back(ranked[i].second);
    }
    
    return result;
}

vector<int> SocialNetwork::suggestedFriends(int userId){
    vector<int> result;
    if(!users.count(userId)){
        cout<<"Invalid userID\n";
        return result;  
    }
    
    const User &currentUser = users[userId];
    
    // Check if user is new (has no friends)
    bool isNewUser = !friends.count(userId) || friends[userId].empty();
    
    // Exclude yourself and current friends
    unordered_set<int> excluded;
    excluded.insert(userId);
    if(friends.count(userId)){
        for(auto& p : friends[userId]){
            excluded.insert(p.first);
        }
    }
    
    unordered_map<int, double> comprehensiveScore;
    
    if(isNewUser){
        // For new users: Focus primarily on profile-based similarity
        // No graph-based metrics available, so rely on profile matching
        for(auto& entry : users){
            int candidateId = entry.first;
            if(excluded.count(candidateId)) continue;
            
            const User &candidate = entry.second;
            comprehensiveScore[candidateId] = calculateNewUserScore(currentUser, candidate);
        }
    }
    else{
        // For existing users: Use graph-based + profile-based approach
        // Calculate weighted shortest paths from current user using Dijkstra
        auto weightedPaths = dijkstraShortestPaths(userId, 3);
        
        // For each candidate user
        for(auto& entry : users){
            int candidateId = entry.first;
            if(excluded.count(candidateId)) continue;
            
            const User &candidate = entry.second;
            comprehensiveScore[candidateId] = calculateExistingUserScore(userId, candidateId, currentUser, candidate, weightedPaths);
        }
    }
    
    // Rank and return top 10 suggestions
    return rankAndReturnTopCandidates(comprehensiveScore, 10);
}


bool SocialNetwork::deleteUser(int userId){
    if(!users.count(userId)){
        return false;
    }
    
    User &u = users[userId];
    string username = u.username;
    
    // Remove user from users map
    users.erase(userId);
    
    // Remove username from usernameToId map
    usernameToId.erase(username);
    
    //  remove user from all their friends' friend lists
    if(friends.count(userId)){
        for(auto &pair : friends[userId]){
            int friendId = pair.first;
            // Remove this user from friend's friend list
            if(friends.count(friendId)){
                auto &friendList = friends[friendId];
                friendList.erase(
                    remove_if(friendList.begin(), friendList.end(),
                    [userId](const std::pair<int, int> &p) { return p.first == userId; }),
                    friendList.end()
                );
            }
        }
        friends.erase(userId);
    }
    
    // Also check all other users' friend lists and remove this user if present
    for(auto &entry : friends){
        auto &friendList = entry.second;
        friendList.erase(
            remove_if(friendList.begin(), friendList.end(),
            [userId](const std::pair<int, int> &p) { return p.first == userId; }),
            friendList.end()
        );
    }
    
    return true;
}

