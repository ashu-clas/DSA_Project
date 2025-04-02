#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

class TravelPlanner {
private:
    unordered_map<string, vector<pair<string, int>>> graph;

public:
    void addLocation(const string& location) {
        if (graph.find(location) == graph.end()) {
            graph[location] = vector<pair<string, int>>();
            cout << "Location '" << location << "' added successfully.\n";
        } else {
            cout << "Location '" << location << "' already exists.\n";
        }
    }

    void removeLocation(const string& location) {
        if (graph.find(location) == graph.end()) {
            cout << "Location '" << location << "' not found.\n";
            return;
        }

        for (auto& [src, edges] : graph) {
            edges.erase(
                remove_if(edges.begin(), edges.end(), 
                    [&location](const pair<string, int>& edge) {
                        return edge.first == location;
                    }),
                edges.end()
            );
        }

        graph.erase(location);
        cout << "Location '" << location << "' removed successfully.\n";
    }

    void addRoute(const string& src, const string& dest, int weight) {
        if (graph.find(src) == graph.end() || graph.find(dest) == graph.end()) {
            cout << "One or both locations not found.\n";
            return;
        }

        for (auto& edge : graph[src]) {
            if (edge.first == dest) {
                cout << "Route already exists. Use updateRoute to change weight.\n";
                return;
            }
        }

        graph[src].emplace_back(dest, weight);
        graph[dest].emplace_back(src, weight);
        cout << "Route added between '" << src << "' and '" << dest 
             << "' with weight " << weight << ".\n";
    }

    void updateRoute(const string& src, const string& dest, int newWeight) {
        if (graph.find(src) == graph.end() || graph.find(dest) == graph.end()) {
            cout << "One or both locations not found.\n";
            return;
        }

        bool found = false;
        for (auto& edge : graph[src]) {
            if (edge.first == dest) {
                edge.second = newWeight;
                found = true;
                break;
            }
        }

        if (found) {
            for (auto& edge : graph[dest]) {
                if (edge.first == src) {
                    edge.second = newWeight;
                    break;
                }
            }
            cout << "Route weight updated between '" << src << "' and '" 
                 << dest << "' to " << newWeight << ".\n";
        } else {
            cout << "Route not found between '" << src << "' and '" << dest << "'.\n";
        }
    }

    void removeRoute(const string& src, const string& dest) {
        if (graph.find(src) == graph.end() || graph.find(dest) == graph.end()) {
            cout << "One or both locations not found.\n";
            return;
        }

        bool found = false;
        auto& srcEdges = graph[src];
        srcEdges.erase(
            remove_if(srcEdges.begin(), srcEdges.end(), 
                [&dest](const pair<string, int>& edge) {
                    return edge.first == dest;
                }),
            srcEdges.end()
        );

        auto& destEdges = graph[dest];
        destEdges.erase(
            remove_if(destEdges.begin(), destEdges.end(), 
                [&src](const pair<string, int>& edge) {
                    return edge.first == src;
                }),
            destEdges.end()
        );

        cout << "Route removed between '" << src << "' and '" << dest << "'.\n";
    }

    void bfsTraversal(const string& startLocation) {
        if (graph.find(startLocation) == graph.end()) {
            cout << "Location '" << startLocation << "' not found.\n";
            return;
        }

        unordered_map<string, bool> visited;
        queue<string> q;
        cout << "BFS Traversal starting from '" << startLocation << "': ";
        visited[startLocation] = true;
        q.push(startLocation);

        while (!q.empty()) {
            string current = q.front();
            q.pop();
            cout << current << " ";
            
            for (const auto& neighbor : graph[current]) {
                if (!visited[neighbor.first]) {
                    visited[neighbor.first] = true;
                    q.push(neighbor.first);
                }
            }
        }
        cout << endl;
    }

    void dfsTraversal(const string& startLocation) {
        if (graph.find(startLocation) == graph.end()) {
            cout << "Location '" << startLocation << "' not found.\n";
            return;
        }

        unordered_map<string, bool> visited;
        stack<string> s;
        cout << "DFS Traversal starting from '" << startLocation << "': ";
        visited[startLocation] = true;
        s.push(startLocation);

        while (!s.empty()) {
            string current = s.top();
            s.pop();
            cout << current << " ";

            for (auto it = graph[current].rbegin(); it != graph[current].rend(); ++it) {
                if (!visited[it->first]) {
                    visited[it->first] = true;
                    s.push(it->first);
                }
            }
        }
        cout << endl;
    }

    void findShortestPath(const string& src, const string& dest) {
        if (graph.find(src) == graph.end() || graph.find(dest) == graph.end()) {
            cout << "One or both locations not found.\n";
            return;
        }

        priority_queue<pair<int, string>, vector<pair<int, string>>, greater<pair<int, string>>> pq;
        unordered_map<string, int> distances;
        unordered_map<string, string> previous;

        for (const auto& [location, _] : graph)
            distances[location] = numeric_limits<int>::max();
        distances[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            string current = pq.top().second;
            int currentDist = pq.top().first;
            pq.pop();

            if (currentDist > distances[current]) continue;

            for (const auto& neighbor : graph[current]) {
                int distance = currentDist + neighbor.second;
                if (distance < distances[neighbor.first]) {
                    distances[neighbor.first] = distance;
                    previous[neighbor.first] = current;
                    pq.push({distance, neighbor.first});
                }
            }
        }

        if (distances[dest] == numeric_limits<int>::max()) {
            cout << "No path exists between '" << src << "' and '" << dest << "'.\n";
            return;
        }

        vector<string> path;
        string current = dest;
        while (current != src) {
            path.push_back(current);
            current = previous[current];
        }
        path.push_back(src);
        reverse(path.begin(), path.end());

        cout << "Shortest path from '" << src << "' to '" << dest << "':\n";
        cout << "Total distance: " << distances[dest] << "\nPath: ";
        for (size_t i = 0; i < path.size(); ++i) {
            if (i != 0) cout << " -> ";
            cout << path[i];
        }
        cout << endl;
    }

    void displayGraph() {
        cout << "\nCurrent Travel Network:\n----------------------\n";
        for (const auto& [location, edges] : graph) {
            cout << location << " connects to:\n";
            if (edges.empty()) cout << "  (no connections)\n";
            else for (const auto& edge : edges)
                cout << "  - " << edge.first << " (weight: " << edge.second << ")\n";
            cout << endl;
        }
    }
};

void displayMenu() {
    cout << "\nTravel Planner Menu:\n"
         << "1. Add a location\n2. Remove a location\n3. Add a route\n"
         << "4. Update route weight\n5. Remove a route\n6. BFS Traversal\n"
         << "7. DFS Traversal\n8. Find shortest path\n9. Display all\n10. Exit\n"
         << "Enter your choice: ";
}

int main() {
    TravelPlanner planner;
    int choice;
    string location1, location2;
    int weight;

    cout << "Welcome to the Travel Planner System!\n";
    do {
        displayMenu();
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number (1-10).\n";
            continue;
        }

        switch (choice) {
            case 1:
                cout << "Enter location name to add: ";
                cin >> location1;
                planner.addLocation(location1);
                break;
            case 2:
                cout << "Enter location name to remove: ";
                cin >> location1;
                planner.removeLocation(location1);
                break;
            case 3:
                cout << "Enter source location: ";
                cin >> location1;
                cout << "Enter destination location: ";
                cin >> location2;
                while (true) {
                    cout << "Enter route weight: ";
                    cin >> weight;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid weight. ";
                    } else break;
                }
                planner.addRoute(location1, location2, weight);
                break;
            case 4:
                cout << "Enter source location: ";
                cin >> location1;
                cout << "Enter destination location: ";
                cin >> location2;
                while (true) {
                    cout << "Enter new weight: ";
                    cin >> weight;
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid weight. ";
                    } else break;
                }
                planner.updateRoute(location1, location2, weight);
                break;
            case 5:
                cout << "Enter source location: ";
                cin >> location1;
                cout << "Enter destination location: ";
                cin >> location2;
                planner.removeRoute(location1, location2);
                break;
            case 6:
                cout << "Enter starting location for BFS: ";
                cin >> location1;
                planner.bfsTraversal(location1);
                break;
            case 7:
                cout << "Enter starting location for DFS: ";
                cin >> location1;
                planner.dfsTraversal(location1);
                break;
            case 8:
                cout << "Enter source location: ";
                cin >> location1;
                cout << "Enter destination location: ";
                cin >> location2;
                planner.findShortestPath(location1, location2);
                break;
            case 9:
                planner.displayGraph();
                break;
            case 10:
                cout << "Exiting Travel Planner. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 10.\n";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    } while (choice != 10);

    return 0;
}