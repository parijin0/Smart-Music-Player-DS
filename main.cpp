#include <bits/stdc++.h>
using namespace std;
/*
 =====================================================
 SMART MUSIC PLAYER (DS PROJECT)
 =====================================================
 DATA STRUCTURES USED:
 ---------------------
 1. Doubly Linked List -> Playlist (next/previous songs)
 2. Queue -> Upcoming songs ("Play Next")
 3. Stack -> History of played songs
 4. HashMap -> Music Library (fast song lookup by id/title)
 5. Priority Queue -> Most played songs (recommendation)
 6. Graph (Adjacency) -> Song similarity graph
 7. Custom Max Heap -> Top-rated songs
*/
void clearInputBuffer() {
 cin.clear();
 cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void pauseScreen() {
 cout << "\nPress ENTER to continue...";
 cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
class Song {
private:
 int id;
 string title;
 string artist;
 int durationSeconds;
 int rating;
 int playCount;
public:
 Song() : id(-1), durationSeconds(0), rating(0), playCount(0) {}
 Song(int _id, const string& _title, const string& _artist,
 int _durationSeconds, int _rating)
 : id(_id),
 title(_title),
 artist(_artist),
 durationSeconds(_durationSeconds),
 rating(_rating),
 playCount(0) {}
 int getId() const {
 return id;
 }
 const string& getTitle() const {
 return title;
 }
 const string& getArtist() const {
 return artist;
 }
 int getDuration() const {
 return durationSeconds;
 }
 int getRating() const {
 return rating;
 }
 int getPlayCount() const {
 return playCount;
 }
 void incrementPlayCount() {
 playCount++;
 }
 void setRating(int newRating) {
 if (newRating < 1) newRating = 1;
 if (newRating > 5) newRating = 5;
 rating = newRating;
 }
 void printBrief() const {cout << "[" << id << "] " << title << " - " << artist
 << " | Duration: " << durationSeconds << "s"
 << " | Rating: " << rating
 << " | Played: " << playCount << " times\n";
 }
 void printDetails() const {
 cout << "\n-------------------------------------\n";
 cout << " Song ID : " << id << "\n";
 cout << " Title : " << title << "\n";
 cout << " Artist : " << artist << "\n";
 cout << " Duration (s) : " << durationSeconds << "\n";
 cout << " Rating : " << rating << "/5\n";
 cout << " Play Count : " << playCount << "\n";
 cout << "-------------------------------------\n";
 }
};
struct PlaylistNode {
 Song* song;
 PlaylistNode* prev;
 PlaylistNode* next;
 PlaylistNode(Song* s) : song(s), prev(nullptr), next(nullptr) {}
};
class Playlist {
private:
 PlaylistNode* head;
 PlaylistNode* tail;
 PlaylistNode* current;
 int size;
public:
 Playlist() : head(nullptr), tail(nullptr), current(nullptr), size(0) {}
 ~Playlist() {
 clear();
 }
 bool isEmpty() const {
 return head == nullptr;
 }
 int getSize() const {
 return size;
 }
 void addSongAtEnd(Song* s) {
 PlaylistNode* node = new PlaylistNode(s);
 if (!head) {
 head = tail = current = node;
 } else {
 tail->next = node;
 node->prev = tail;
 tail = node;
 }
 size++;
 }
 void addSongAtStart(Song* s) {
 PlaylistNode* node = new PlaylistNode(s);
 if (!head) {
 head = tail = current = node;
 } else {
 node->next = head;
 head->prev = node;
 head = node;
 }
 size++;
 }
 void addSongAtPosition(Song* s, int pos) {
 if (pos <= 1) {
 addSongAtStart(s);
 return;
 }
 if (pos > size) {
 addSongAtEnd(s);
 return;
 }
 PlaylistNode* node = new PlaylistNode(s);
 PlaylistNode* temp = head;
 for (int i = 1; i < pos - 1 && temp; i++) {
 temp = temp->next;
 }
 if (!temp) {addSongAtEnd(s);
 return;
 }
 node->next = temp->next;
 node->prev = temp;
 if (temp->next) temp->next->prev = node;
 temp->next = node;
 if (node->next == nullptr) tail = node;
 size++;
 }
 void removeSongById(int id) {
 if (!head) return;
 PlaylistNode* temp = head;
 while (temp && temp->song->getId() != id) {
 temp = temp->next;
 }
 if (!temp) {
 cout << "Song with ID " << id << " not found in playlist.\n";
 return;
 }
 if (temp == head) {
 head = head->next;
 if (head) head->prev = nullptr;
 } else {
 temp->prev->next = temp->next;
 }
 if (temp == tail) {
 tail = tail->prev;
 if (tail) tail->next = nullptr;
 } else {
 if (temp->next) temp->next->prev = temp->prev;
 }
 if (current == temp) {
 current = temp->next ? temp->next : temp->prev;
 }
 delete temp;
 size--;
 if (size == 0) {
 head = tail = current = nullptr;
 }
 }
 Song* getCurrentSong() const {
 if (!current) return nullptr;
 return current->song;
 }
 Song* moveNext() {
 if (!current) return nullptr;
 if (current->next) {
 current = current->next;
 } else {
 cout << "Already at the end of playlist.\n";
 }
 return getCurrentSong();
 }
 Song* movePrev() {
 if (!current) return nullptr;
 if (current->prev) {
 current = current->prev;
 } else {
 cout << "Already at the start of playlist.\n";
 }
 return getCurrentSong();
 }
 void moveToStart() {
 current = head;
 }
 void moveToEnd() {
 current = tail;
 }
 void displayPlaylist() const {
 cout << "\n===== PLAYLIST (" << size << " songs) =====\n";
 PlaylistNode* temp = head;
 while (temp) {
 if (temp == current) cout << ">> ";
 else cout << " ";temp->song->printBrief();
 temp = temp->next;
 }
 if (!head) cout << "Playlist is empty.\n";
 }
 void clear() {
 PlaylistNode* temp = head;
 while (temp) {
 PlaylistNode* nextNode = temp->next;
 delete temp;
 temp = nextNode;
 }
 head = tail = current = nullptr;
 size = 0;
 }
};
class MusicLibrary {
private:
 unordered_map<int, Song*> songsById;
 unordered_map<string, vector<Song*>> songsByTitle;
public:
 MusicLibrary() {}
 ~MusicLibrary() {
 for (auto& p : songsById) {
 delete p.second;
 }
 songsById.clear();
 songsByTitle.clear();
 }
 bool exists(int id) const {
 return songsById.find(id) != songsById.end();
 }
 Song* addSong(int id, const string& title, const string& artist,
 int duration, int rating) {
 if (exists(id)) {
 cout << "Song with this ID already exists in library.\n";
 return songsById[id];
 }
 Song* s = new Song(id, title, artist, duration, rating);
 songsById[id] = s;
 songsByTitle[title].push_back(s);
 return s;
 }
 Song* getSongById(int id) const {
 auto it = songsById.find(id);
 if (it == songsById.end()) return nullptr;
 return it->second;
 }
 vector<Song*> searchByTitle(const string& title) const {
 auto it = songsByTitle.find(title);
 if (it == songsByTitle.end()) return {};
 return it->second;
 }
 void printLibrary() const {
 cout << "\n===== MUSIC LIBRARY (" << songsById.size() << " songs) =====\n";
 for (auto& p : songsById) {
 p.second->printBrief();
 }
 if (songsById.empty()) {
 cout << "No songs in library.\n";
 }
 }
};
class HistoryManager {
private:
 stack<Song*> historyStack;
public:
 void recordPlay(Song* s) {
 if (s) {
 historyStack.push(s);
 }
 }
 void showHistory() {
 cout << "\n===== PLAY HISTORY (Top is latest) =====\n";
 if (historyStack.empty()) {cout << "No history yet.\n";
 return;
 }
 stack<Song*> temp = historyStack;
 int index = 1;
 while (!temp.empty()) {
 cout << index++ << ". ";
 temp.top()->printBrief();
 temp.pop();
 }
 }
 bool isEmpty() const {
 return historyStack.empty();
 }
 Song* getLastPlayed() {
 if (historyStack.empty()) return nullptr;
 return historyStack.top();
 }
};
class UpcomingManager {
private:
 queue<Song*> upcomingQueue;
public:
 void addUpcoming(Song* s) {
 if (s) {
 upcomingQueue.push(s);
 }
 }
 Song* getNextFromQueue() {
 if (upcomingQueue.empty()) return nullptr;
 Song* s = upcomingQueue.front();
 upcomingQueue.pop();
 return s;
 }
 void showUpcoming() {
 cout << "\n===== UPCOMING SONGS QUEUE =====\n";
 if (upcomingQueue.empty()) {
 cout << "No songs in upcoming queue.\n";
 return;
 }
 queue<Song*> temp = upcomingQueue;
 int index = 1;
 while (!temp.empty()) {
 cout << index++ << ". ";
 temp.front()->printBrief();
 temp.pop();
 }
 }
 bool isEmpty() const {
 return upcomingQueue.empty();
 }
};
class RecommendationEngine {
private:
 priority_queue<pair<int, Song*>> maxHeap;
public:
 void updateOnPlay(Song* s) {
 if (!s) return;
 int count = s->getPlayCount();
 maxHeap.push({count, s});
 }
 void showTopRecommendations(int n = 5) {
 cout << "\n===== TOP RECOMMENDED SONGS =====\n";
 if (maxHeap.empty()) {
 cout << "No play data yet for recommendations.\n";
 return;
 }
 priority_queue<pair<int, Song*>> temp = maxHeap;
 int shown = 0;
 while (!temp.empty() && shown < n) {
 auto top = temp.top();
 temp.pop();
 cout << "Played " << top.first << " times: ";
 top.second->printBrief();shown++;
 }
 }
};
class SongGraph {
private:
 unordered_map<int, vector<int>> adjList;
public:
 void addSimilarity(int id1, int id2) {
 adjList[id1].push_back(id2);
 adjList[id2].push_back(id1);
 }
 void showSimilarSongs(int id) const {
 cout << "\n===== SIMILAR SONGS FOR ID " << id << " =====\n";
 auto it = adjList.find(id);
 if (it == adjList.end() || it->second.empty()) {
 cout << "No similarity data available.\n";
 return;
 }
 for (int neighbor : it->second) {
 cout << "Similar to -> Song ID: " << neighbor << "\n";
 }
 }
 void dfsUtil(int id, unordered_map<int, bool>& visited) const {
 visited[id] = true;
 cout << id << " ";
 auto it = adjList.find(id);
 if (it == adjList.end()) return;
 for (int neighbor : it->second) {
 if (!visited[neighbor]) {
 dfsUtil(neighbor, visited);
 }
 }
 }
 void depthFirstTraversal(int startId) const {
 cout << "\nDFS Traversal from Song ID " << startId << ": ";
 unordered_map<int, bool> visited;
 dfsUtil(startId, visited);
 cout << "\n";
 }
};
class RatingMaxHeap {
private:
 vector<Song*> heapArr;
 bool isHigherPriority(Song* a, Song* b) {
 if (a->getRating() == b->getRating()) {
 return a->getId() < b->getId();
 }
 return a->getRating() > b->getRating();
 }
 void heapifyUp(int index) {
 while (index > 0) {
 int parent = (index - 1) / 2;
 if (isHigherPriority(heapArr[index], heapArr[parent])) {
 swap(heapArr[index], heapArr[parent]);
 index = parent;
 } else {
 break;
 }
 }
 }
 void heapifyDown(int index) {
 int n = (int)heapArr.size();
 while (true) {
 int left = 2 * index + 1;
 int right = 2 * index + 2;
 int largest = index;
 if (left < n && isHigherPriority(heapArr[left], heapArr[largest])) {
 largest = left;
 }
 if (right < n && isHigherPriority(heapArr[right], heapArr[largest])) {
 largest = right;
 }
 if (largest != index) {
 swap(heapArr[index], heapArr[largest]);
 index = largest;
 } else {
 break;}
 }
 }
public:
 void clear() {
 heapArr.clear();
 }
 void buildFromLibrary(const MusicLibrary& lib,
 const vector<int>& allIds) {
 clear();
 for (int id : allIds) {
 (void)id;
 }
 }
 void push(Song* s) {
 heapArr.push_back(s);
 heapifyUp((int)heapArr.size() - 1);
 }
 Song* top() const {
 if (heapArr.empty()) return nullptr;
 return heapArr[0];
 }
 void pop() {
 if (heapArr.empty()) return;
 heapArr[0] = heapArr.back();
 heapArr.pop_back();
 if (!heapArr.empty()) {
 heapifyDown(0);
 }
 }
 bool empty() const {
 return heapArr.empty();
 }
 int size() const {
 return (int)heapArr.size();
 }
 void showTopRated(int n = 5) {
 cout << "\n===== TOP RATED SONGS (Using Custom Max Heap) =====\n";
 if (heapArr.empty()) {
 cout << "Heap is empty.\n";
 return;
 }
 RatingMaxHeap temp = *this;
 int count = 0;
 while (!temp.empty() && count < n) {
 Song* s = temp.top();
 temp.pop();
 cout << "Rating " << s->getRating() << " -> ";
 s->printBrief();
 count++;
 }
 }
};
class MusicPlayer {
private:
 MusicLibrary library;
 Playlist playlist;
 HistoryManager history;
 UpcomingManager upcoming;
 RecommendationEngine recommender;
 SongGraph graph;
 RatingMaxHeap ratingHeap;
 int nextSongId;
public:
 MusicPlayer() : nextSongId(1) {}
 void addSongInteractive() {
 string title, artist;
 int duration, rating;
 cout << "\nEnter song title: ";
 clearInputBuffer();
 getline(cin, title);
 cout << "Enter artist name: ";
 getline(cin, artist);cout << "Enter duration in seconds: ";
 cin >> duration;
 cout << "Enter rating (1 to 5): ";
 cin >> rating;
 Song* s = library.addSong(nextSongId, title, artist, duration, rating);
 if (s) {
 playlist.addSongAtEnd(s);
 ratingHeap.push(s);
 cout << "Song added with ID: " << nextSongId << "\n";
 nextSongId++;
 }
 }
 void displayLibrary() const {
 library.printLibrary();
 }
 void displayPlaylist() const {
 playlist.displayPlaylist();
 }
 void searchSongById() const {
 int id;
 cout << "\nEnter song ID to search: ";
 cin >> id;
 Song* s = library.getSongById(id);
 if (!s) {
 cout << "Song not found.\n";
 } else {
 s->printDetails();
 }
 }
 void searchSongByTitle() const {
 string title;
 cout << "\nEnter song title to search: ";
 clearInputBuffer();
 getline(cin, title);
 auto results = library.searchByTitle(title);
 if (results.empty()) {
 cout << "No matching songs found.\n";
 } else {
 cout << "\nFound " << results.size() << " matching song(s):\n";
 for (Song* s : results) {
 s->printBrief();
 }
 }
 }
 void playCurrentSong() {
 Song* current = playlist.getCurrentSong();
 if (!current) {
 cout << "No song selected in playlist.\n";
 return;
 }
 current->incrementPlayCount();
 history.recordPlay(current);
 recommender.updateOnPlay(current);
 cout << "\n===== NOW PLAYING =====\n";
 current->printDetails();
 }
 void nextSong() {
 Song* fromQueue = upcoming.getNextFromQueue();
 if (fromQueue) {
 cout << "\nPlaying from upcoming queue:\n";
 fromQueue->incrementPlayCount();
 history.recordPlay(fromQueue);
 recommender.updateOnPlay(fromQueue);
 fromQueue->printDetails();
 return;
 }
 Song* next = playlist.moveNext();
 if (!next) {
 cout << "Reached end of playlist.\n";
 return;
 }
 next->incrementPlayCount();
 history.recordPlay(next);
 recommender.updateOnPlay(next);cout << "\n===== NOW PLAYING (Next) =====\n";
 next->printDetails();
 }
 void previousSong() {
 Song* prev = playlist.movePrev();
 if (!prev) {
 cout << "At the start of playlist.\n";
 return;
 }
 prev->incrementPlayCount();
 history.recordPlay(prev);
 recommender.updateOnPlay(prev);
 cout << "\n===== NOW PLAYING (Previous) =====\n";
 prev->printDetails();
 }
 void goToStartOfPlaylist() {
 playlist.moveToStart();
 cout << "Moved to start of playlist.\n";
 playCurrentSong();
 }
 void goToEndOfPlaylist() {
 playlist.moveToEnd();
 cout << "Moved to end of playlist.\n";
 playCurrentSong();
 }
 void addToUpcomingById() {
 int id;
 cout << "\nEnter song ID to add into upcoming queue: ";
 cin >> id;
 Song* s = library.getSongById(id);
 if (!s) {
 cout << "Song not found in library.\n";
 return;
 }
 upcoming.addUpcoming(s);
 cout << "Song added to upcoming queue.\n";
 }
 void showUpcomingQueue() {
 upcoming.showUpcoming();
 }
 void showHistory() {
 history.showHistory();
 }
 void showRecommendations() {
 recommender.showTopRecommendations();
 }
 void addSongSimilarity() {
 int id1, id2;
 cout << "\nEnter Song ID 1: ";
 cin >> id1;
 cout << "Enter Song ID 2: ";
 cin >> id2;
 Song* s1 = library.getSongById(id1);
 Song* s2 = library.getSongById(id2);
 if (!s1 || !s2) {
 cout << "One or both songs not found in library.\n";
 return;
 }
 graph.addSimilarity(id1, id2);
 cout << "Similarity added between " << id1 << " and " << id2 << ".\n";
 }
 void showSimilarSongs() const {
 int id;
 cout << "\nEnter Song ID to view similar songs: ";
 cin >> id;
 graph.showSimilarSongs(id);
 }
 void dfsFromSong() const {
 int id;
 cout << "\nEnter start Song ID for DFS traversal: ";
 cin >> id;
 graph.depthFirstTraversal(id);
 }
 void showTopRatedSongs() {
 ratingHeap.showTopRated();}
 void removeSongFromPlaylist() {
 int id;
 cout << "\nEnter song ID to remove from playlist: ";
 cin >> id;
 playlist.removeSongById(id);
 }
 void showMainMenu() {
 cout << "\n============================================\n";
 cout << " SMART MUSIC PLAYER - MAIN MENU \n";
 cout << "============================================\n";
 cout << " 1. Add Song (Library + Playlist)\n";
 cout << " 2. Show Music Library\n";
 cout << " 3. Show Playlist\n";
 cout << " 4. Search Song by ID\n";
 cout << " 5. Search Song by Title\n";
 cout << " 6. Play Current Song\n";
 cout << " 7. Next Song\n";
 cout << " 8. Previous Song\n";
 cout << " 9. Go to Start of Playlist\n";
 cout << "10. Go to End of Playlist\n";
 cout << "11. Add Song to Upcoming Queue\n";
 cout << "12. Show Upcoming Queue\n";
 cout << "13. Show Play History\n";
 cout << "14. Show Recommendations (Most Played)\n";
 cout << "15. Add Song Similarity (Graph Edge)\n";
 cout << "16. Show Similar Songs for an ID\n";
 cout << "17. DFS Traversal in Song Graph\n";
 cout << "18. Show Top Rated Songs (Custom Heap)\n";
 cout << "19. Remove Song from Playlist\n";
 cout << " 0. Exit\n";
 cout << "============================================\n";
 cout << "Enter your choice: ";
 cout.flush();
 }
 void run() {
 int choice;
 bool running = true;
 while (running) {
 showMainMenu();
 cin >> choice;
 switch (choice) {
 case 1:
 addSongInteractive();
 break;
 case 2:
 displayLibrary();
 break;
 case 3:
 displayPlaylist();
 break;
 case 4:
 searchSongById();
 break;
 case 5:
 searchSongByTitle();
 break;
 case 6:
 playCurrentSong();
 break;
 case 7:
 nextSong();
 break;
 case 8:
 previousSong();
 break;
 case 9:
 goToStartOfPlaylist();
 break;
 case 10:
 goToEndOfPlaylist();
 break;
 case 11:
 addToUpcomingById();
 break;
 case 12:
 showUpcomingQueue();
 break;
 case 13:
 showHistory();
 break;
 case 14:
 showRecommendations();
 break;case 15:
 addSongSimilarity();
 break;
 case 16:
 showSimilarSongs();
 break;
 case 17:
 dfsFromSong();
 break;
 case 18:
 showTopRatedSongs();
 break;
 case 19:
 removeSongFromPlaylist();
 break;
 case 0:
 running = false;
 cout << "Exiting Music Player. Goodbye!\n";
 break;
 default:
 cout << "Invalid choice. Try again.\n";
 break;
 }
 if (running) {
 clearInputBuffer();
 pauseScreen();
 }
 }
 }
};
int main() {
 MusicPlayer player;
 player.run();
 return 0;
}