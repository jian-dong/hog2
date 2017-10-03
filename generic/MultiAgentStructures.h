#ifndef MultiAgentStructures_H
#define MultiAgentStructures_H

#include "GenericSearchAlgorithm.h"
#include "VelocityObstacle.h"
#include "NonUnitTimeCAT.h"
#include <set>

template<typename state>
using MultiAgentState = std::vector<state>;

template<typename state>
using Solution = std::vector<std::vector<state>>;

typedef std::set<IntervalData> ConflictSet;

template<typename state, typename action>
struct EnvironmentContainer {
  EnvironmentContainer() : name("NULL ENV"), environment(nullptr), heuristic(nullptr), threshold(0), astar_weight(1.0f) {
    std::cout << "stuff\n";
  }
  EnvironmentContainer(std::string n, ConstrainedEnvironment<state,action>* e, Heuristic<state>* h, uint32_t conf, float a) : name(n), environment(e), heuristic(h), threshold(conf), astar_weight(a) {}
  void SetupSearch(GenericSearchAlgorithm<state,action,ConstrainedEnvironment<state,action>>& srch){if(heuristic)srch.SetHeuristic(heuristic); srch.SetWeight(astar_weight);}
  std::string name;
  ConstrainedEnvironment<state,action>* environment;
  Heuristic<state>* heuristic; // This could be an array if we are planning for multiple legs
  unsigned threshold;
  float astar_weight;
};

// Struct for parsing config files
struct EnvData{
  EnvData():name(),threshold(0),weight(1.0){}
  EnvData(std::string const& n, unsigned t, double w):name(n),threshold(t),weight(w){}
  std::string name;
  unsigned threshold;
  double weight;
};

template<typename state, typename action>
class MAPFAlgorithm{
  public:
  virtual void GetSolution(std::vector<EnvironmentContainer<state,action>*> const& env, MultiAgentState<state> const& start, MultiAgentState<state> const& goal, Solution<state>& solution)=0;
  virtual unsigned GetNodesExpanded()const=0;
};

template<typename state>
bool checkCollision(std::vector<state> const& p1, std::vector<state> const& p2,float radius=.5,bool loud=false){
  auto ap(p1.begin());
  auto a(ap+1);
  auto bp(p2.begin());
  auto b(bp+1);
  while(a!=p1.end() && b!=p2.end()){
    Vector2D A(*ap);
    Vector2D B(*bp);
    Vector2D VA(*a);
    VA-=A;
    VA.Normalize();
    Vector2D VB(*b);
    VB-=B;
    VB.Normalize();
    if(collisionImminent(A,VA,radius,ap->t,a->t,B,VB,radius,bp->t,b->t)){
      if(loud)std::cout << "Collision: " << *ap << "-->" << *a << "," << *bp << "-->" << *b << "\n";
      return false;
    }
    if(fless(a->t,b->t)){
      ++a;
      ++ap;
    }else if(fgreater(a->t,b->t)){
      ++b;
      ++bp;
    }else{
      ++a;++b;
      ++ap;++bp;
    }
  }
  return true;
}

template<typename state>
bool validateSolution(Solution<state> const& sol, bool bruteForce=true){
  if(bruteForce){
    for(auto a(sol.begin()); a!=sol.end(); ++a){
      for(auto b(a+1); b!=sol.end(); ++b){
        if(!checkCollision<state>(*a,*b)) return false;
      }
    }
  }else{
  }
  return true;
}

// Check if an openlist node conflicts with a node from an existing path
template<typename state>
unsigned checkForConflict(state const*const parent, state const*const node, state const*const pathParent, state const*const pathNode){
  Constraint<state> v(*node);
  if(v.ConflictsWith(*pathNode)){return 1;}
  if(parent && pathParent){
    Constraint<state> e1(*parent,*node);
    if(e1.ConflictsWith(*pathParent,*pathNode)){return 1;}
  }
  return 0; 
}

#endif