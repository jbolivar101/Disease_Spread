#include <iostream>
#include<string>
#include<vector>
#include<iomanip>
#include<memory>
#include<random>
using namespace std;  

class Person{
private:
  int state = 0;
  int n;
  string condition;
public:
  string status_string(){
    if(state==-1){
      condition= "recovered";}
    else if(state==-2){
      condition= "vaccinated";}
    else if(state>0){
      cout << "sick (" << to_string(state) << " to go)" << endl;
      condition= false;}
    else if(state==0){
      condition= "susceptible";}
    return condition;}
    
  int update(){
    if(state>1){
      state = state -1;
    }
    else if(state==1){
      state = state -2;
    }
    return state;}
    
  void infect(int n) {
    if (state==0){
      state = n;}
  }
    
  void vacc() {
    if (state==0){
      state = -2;}
  }
    
  bool is_stable(){
    if(state==-1){
      return true;}
    else {
      return false;}
  }
    
  int new_state(){
    return state;}
};

class Population: Person {
private:
  vector<Person> people;
  int Icount;
  int Vcount;
  int npeople = 0;
  int totdays;
  int randn;
public:
  Population() { };
  Population (int npeople) : npeople(npeople){
    people = vector<Person>(npeople);
  };
    
  int num_days(int i) {
    totdays = i;
    return totdays;}
    
  void random_infection() {
    randn = rand() % people.size();
    return people[randn].infect(totdays);
  };
    
  int inf_count(){
    Icount=0;
    for(int i=0;i<people.size();i++){
      if(people[i].new_state()>0){
	Icount++;}
    }
    return Icount;};
    
  int vacc_count(){
    Vcount=0;
    for(int i=0;i<people.size();i++){
      if(people[i].new_state()==-2){
	Vcount++;}
    }
    return Vcount;};
    
  void updatePop(){
    for(int i=0;people.size()>i;i++){
      people[i].update();}
  }
    
    
  void pop_status() {
    for (int i = 0; i < people.size(); i++) {
      if (people[i].new_state() > 0) {
	cout << "+";
      }
      else if (people[i].new_state() == 0) {
	cout << "?";
      }
      else if (people[i].new_state() == -1) {
	cout << "-";
      }
      else if (people[i].new_state() == -2) {
	cout << "*";
      }
    }
  };
    
  void interaction(float prob) {
    for (int i = 0; i < people.size(); i++) {
      float bad_luck = ((float)rand() / (float)RAND_MAX);
      if (bad_luck > prob && (i - 1) > 0 && (i + 1) < people.size()) {
	people[i - 1].infect(totdays);
	people[i + 1].infect(totdays);}
    }
  }
  void Vaccine_talk(float Vprob) {
    for (int i = 0; i < people.size(); i++) {
      float good_luck = ((float)rand() / (float)RAND_MAX);
      if (good_luck > Vprob && (i - 1) > 0 && (i + 1) < people.size()) {
	people[i].vacc();}
    }
  }
};


int main() {
  srand(static_cast <unsigned> (time(0)));
  Person jose;
  int st = 1, amount= 10;
  for ( ; ; st++) {
    jose.update();
    float bad_luck = (float) rand()/(float)RAND_MAX;
    if (bad_luck>.95)
      jose.infect(5);
    cout << "On day " << st << ", Jose is "
	 << jose.status_string() << endl;
    if (jose.is_stable())
      break;
  }

    
  int step= 0;
  int days;
  int population;
  float prob; 
  float Vprob;
        
  cout << "Population of Austin: ";
  cin >> population;
  cout << "Days of infection: ";
  cin >> days;
  cout <<"Threshold of infection 0 < i < 1: " ;
  cin >> prob;
  cout <<"Threshold of vaccination 0 < v < 1: ";
  cin >> Vprob;
        
  Population Austin(population);
  Austin.num_days(days);
  Austin.random_infection();
  while (Austin.inf_count() > 0) {
    Austin.Vaccine_talk(Vprob);
    Austin.updatePop();
    int Icount = Austin.inf_count();
    int Vcount = Austin.vacc_count();
    Austin.pop_status();
    cout << "  |  " << Icount <<", " << Vcount << " people are infected, vaccinated on day " << step << endl;
    Austin.interaction(prob);
    step++;}
  return 0;
}
