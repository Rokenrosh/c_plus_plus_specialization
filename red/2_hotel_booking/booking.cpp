#include <iomanip>
#include <iostream>
#include <queue>
#include <utility>
#include <unordered_map>

using namespace std;

struct Event{
    int64_t time;
    int client_id;
    int room_count;
    string hotel_name;
};

class Booking{
public:
    void Book(int64_t time, string hotel_name, int client_id, int room_count){
        requests.push({time, client_id, room_count, hotel_name});
        rooms[hotel_name] += room_count;
        clients[hotel_name][client_id] += 1;
    }

    int Clients(string hotel_name){
        Update();
        if(clients.find(hotel_name) != clients.end()){
            return clients[hotel_name].size();
        }
        return 0;
    }

    int Rooms(string hotel_name){
        Update();
        if(rooms.find(hotel_name) != rooms.end()){
            return rooms.at(hotel_name);
        }
        return 0;
    }
private:
    queue<Event> requests;
    unordered_map<string, int> rooms;
    unordered_map<string, unordered_map<int, int>> clients;
    void Update(){
        if(!requests.empty()){
            int64_t current_time = requests.back().time;
            while(requests.front().time <= current_time - 86400){
                rooms[requests.front().hotel_name] -= requests.front().room_count;
                clients[requests.front().hotel_name][requests.front().client_id]--;
                if(clients[requests.front().hotel_name][requests.front().client_id] < 1){
                    clients[requests.front().hotel_name].erase(requests.front().client_id);
                }
                requests.pop();
            }
        }
    }
};

int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Booking manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int64_t time;
      string hotel_name;
      int client_id;
      int room_count;
      cin >> time >> hotel_name >> client_id >> room_count;
      manager.Book(time, hotel_name, client_id, room_count);
    } else if (query_type == "CLIENTS") {
        string hotel_name;
        cin >> hotel_name;
      cout << manager.Clients(hotel_name) << "\n";
    } else if (query_type == "ROOMS"){
        string hotel_name;
        cin >> hotel_name;
        cout << manager.Rooms(hotel_name) << "\n";
    }
  }

  return 0;
}