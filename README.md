
---

## Overview  
This is a **C++ console-based Social Network System** where users can register, log in, manage friends, and receive intelligent friend recommendations using **graph algorithms + similarity scoring**.

This project demonstrates real-world usage of:
- Graphs  
- Dijkstra’s Algorithm  
- Hash Maps  
- Sorting & Ranking  
- String Parsing  
- File handling (CSV persistence)  

---

##  Project Structure

```
DSA-Project-Friend-Suggestion/
│
├── src/
│   ├── main.cpp
│   ├── SocialNetwork.cpp
│   └── SocialNetwork.h
│
├── data/
│   ├── users.csv
│   └── friends.csv
│
└── README.md
```

---

##  Features

### 🔐 User System
- Register new user  
- Login with password  
- Delete user account  
- Auto-assigned user IDs  

### 👤 Profile & Friend Management
- View user profile  
- Display friend list  
- Add friends  
- Interact with friends → increases friendship weight  

### 🤖 Friend Recommendation Engine
Uses **Profile-Based Similarity**:
- Same school/college  
- Same locality  
- Age similarity  
- Jaccard similarity of hobbies  
- Last name similarity  

Plus **Graph-Based Metrics**:
- Weighted paths via Dijkstra  
- Mutual friends score  
- Triadic closure  
- Connection strength (friend-of-friend)  

### 💾 Persistence (CSV)
All data saved in:
- `users.csv`  
- `friends.csv`  

---

## 🧠 Algorithms & Data Structures Used

| Concept | Usage |
|--------|-------|
| Graph (Adjacency List) | Storing friendships with weights |
| Dijkstra Algorithm | Weighted shortest path between users |
| Jaccard Similarity | Comparing hobbies |
| Sorting | Ranking final recommendations |
| Unordered Maps | Fast lookup for users & graph |
| Sets / HashSets | Mutual friend detection |
| Vectors | Dynamic data structures for hobbies, friend lists |

---

##  How to Compile & Run

### **Step 1 – Navigate to src**
```bash
cd src
```

### **Step 2 – Compile**
```bash
g++ main.cpp SocialNetwork.cpp -o friendbook
```

### **Step 3 – Run**
```bash
./friendbook
```

---


##  Future Enhancements (Optional)
- GUI (Raylib / Qt)  
- Password encryption  
- User chat logs  
- Recommended groups/pages  
- More ML‑based friend prediction  

---

 
DSA Project — Social Graph Algorithms  

---
