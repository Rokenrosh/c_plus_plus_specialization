#include "test_runner.h"
#include <functional>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;


struct Email {
  string from;
  string to;
  string body;
};


class Worker {
public:
  virtual ~Worker() = default;
  virtual void Process(unique_ptr<Email> email) = 0;
  virtual void Run() {
    // только первому worker-у в пайплайне нужно это имплементировать
    throw logic_error("Unimplemented");
  }

protected:
  // реализации должны вызывать PassOn, чтобы передать объект дальше
  // по цепочке обработчиков
  void PassOn(unique_ptr<Email> email) const{
    if(next_){
      next_->Process(move(email));
    }
  }

public:
  void SetNext(unique_ptr<Worker> next){
    next_ = move(next);
  }
private:
  unique_ptr<Worker> next_ = 0;
};


class Reader : public Worker {
public:
  Reader(istream& is): is_(is){}
  void Run() override{
    while(is_){
      string from, to, body;
      getline(is_, from);
      if(!is_) break;
      getline(is_, to);
      if(!is_) break;
      getline(is_, body);
      unique_ptr<Email> ptr = make_unique<Email>(Email{move(from), move(to), move(body)});
      PassOn(move(ptr));
    }
  }
  void Process(unique_ptr<Email> email) override{}
private:
  istream& is_;
};


class Filter : public Worker {
public:
  using Function = function<bool(const Email&)>;

public:
  Filter(Function func): func_(func){}

  void Process(unique_ptr<Email> email) override{
    if(func_(*email)){
      PassOn(move(email));
    }
  }
private:
  Function func_;
};


class Copier : public Worker {
public:
  Copier(const string& to): to_(to){}

  void Process(unique_ptr<Email> email) override{
    if(to_ != email->to){
      unique_ptr<Email> ptr = make_unique<Email>(Email{email->from, to_, email->body});
      PassOn(move(email));
      PassOn(move(ptr));
    }
    else{
      PassOn(move(email));
    }
  }
private:
  const string to_;
};


class Sender : public Worker {
public:
  Sender(ostream& os): os_(os){}

  void Process(unique_ptr<Email> email) override{
    os_ << email->from << '\n' << email->to << '\n' << email->body << "\n";
    PassOn(move(email));
  }
private:
  ostream& os_;
};


// реализуйте класс
class PipelineBuilder {
public:
  // добавляет в качестве первого обработчика Reader
  explicit PipelineBuilder(istream& in){
    handlers.push_back(make_unique<Reader>(in));
  }

  // добавляет новый обработчик Filter
  PipelineBuilder& FilterBy(Filter::Function filter){
    handlers.push_back(make_unique<Filter>(filter));
    return *this;
  }

  // добавляет новый обработчик Copier
  PipelineBuilder& CopyTo(string recipient){
    handlers.push_back(make_unique<Copier>(recipient));
    return *this;
  }

  // добавляет новый обработчик Sender
  PipelineBuilder& Send(ostream& out){
    handlers.push_back(make_unique<Sender>(out));
    return *this;
  }

  // возвращает готовую цепочку обработчиков
  unique_ptr<Worker> Build(){
    for(int i = handlers.size() - 1; i > 0; i--){
      handlers[i - 1]->SetNext(move(handlers[i]));
    }
    return move(handlers[0]);
  }
private:
  vector<unique_ptr<Worker>> handlers;
};


void TestSanity() {
  string input = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "ralph@example.com\n"
    "erich@example.com\n"
    "I do not make mistakes of that kind\n"
  );
  istringstream inStream(input);
  ostringstream outStream;

  PipelineBuilder builder(inStream);
  builder.FilterBy([](const Email& email) {
    return email.from == "erich@example.com";
  });
  builder.CopyTo("richard@example.com");
  builder.Send(outStream);
  auto pipeline = builder.Build();

  pipeline->Run();

  string expectedOutput = (
    "erich@example.com\n"
    "richard@example.com\n"
    "Hello there\n"

    "erich@example.com\n"
    "ralph@example.com\n"
    "Are you sure you pressed the right button?\n"

    "erich@example.com\n"
    "richard@example.com\n"
    "Are you sure you pressed the right button?\n"
  );

  ASSERT_EQUAL(expectedOutput, outStream.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSanity);
  return 0;
}
