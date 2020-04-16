#include <iostream>
#include <queue>
#include <algorithm>

class Client {
  public:
   enum Type {
     CASUAL,
     MID,
     VIP
   };
   Client(int payload, Type type) : payload_ {payload}, type_{type}{}
   int getPayload() const {return payload_;}
   Type getType() const {return type_;}
  private:
   int payload_{0};
   Type type_{Type::MID};
};


class CompClient {
  public:
   bool operator()(const Client& lhs, const Client& rhs) {
    if (lhs.getType() == rhs.getType()) {
      return lhs.getPayload() < rhs.getPayload();
    }
    return lhs.getType() < rhs.getType();
   }
};

using PQ = std::priority_queue<Client, std::deque<Client>, CompClient>;

int main(int argc, char const *argv[]) {
    PQ pq;
    pq.push(Client{10, Client::Type::VIP});
    pq.push(Client{20, Client::Type::VIP});
    pq.push(Client{10, Client::Type::MID});

    auto elem = pq.top();

    std::cout << elem.getPayload() << " : " << static_cast<int>(elem.getType()) << std::endl;

    pq.pop();
    elem = pq.top();
    std::cout << elem.getPayload() << " : " << static_cast<int>(elem.getType()) << std::endl;

    pq.pop();
    elem = pq.top();
    std::cout << elem.getPayload() << " : " << static_cast<int>(elem.getType()) << std::endl;

    return 0;
}
