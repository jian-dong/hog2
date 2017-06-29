/*
 *  Map2DEnvironment.cpp
 *  hog2
 *
 *  Created by Nathan Sturtevant on 4/20/07.
 *  Copyright 2007 Nathan Sturtevant, University of Alberta. All rights reserved.
 *
 */
#include "Map2DEnvironment.h"
#include "FPUtil.h"
#include "SVGUtil.h"
#include <cstring>
#include "Graphics2D.h"
#include "PositionalUtils.h"

using namespace Graphics2D;

Grid3DEnvironment::Grid3DEnvironment(Map *_m, bool useOccupancy):h(nullptr),map(_m),oi(useOccupancy?new BaseMapOccupancyInterface(map):nullptr),DIAGONAL_COST(ROOT_TWO),connectedness(8){
  SQRT_5=sqrt(5.0);
  SQRT_10=sqrt(10.0);
  SQRT_13=sqrt(13);
}

Grid3DEnvironment::Grid3DEnvironment(Grid3DEnvironment *me)
{
	map = me->map->Clone();
	h = 0;
	if (me->oi)
		oi = new BaseMapOccupancyInterface(map);
	else oi = 0;
	DIAGONAL_COST = me->DIAGONAL_COST;
	connectedness = me->connectedness;
}

Grid3DEnvironment::~Grid3DEnvironment()
{
//	delete map;
	delete oi;
}

GraphHeuristic *Grid3DEnvironment::GetGraphHeuristic()
{
	return h;
}

void Grid3DEnvironment::SetGraphHeuristic(GraphHeuristic *gh)
{
	h = gh;
}

void Grid3DEnvironment::GetSuccessors(const xyzLoc &loc, std::vector<xyzLoc> &neighbors) const
{
        // Implement any-angle branching
        if(connectedness>49){
          if(solution.size()==0){
          }
        }
	neighbors.resize(0);
	bool u=false, d=false, l=false, r=false, u2=false, d2=false, l2=false, r2=false, ur=false, ul=false, dr=false, dl=false, u2l=false, d2l=false, u2r=false, d2r=false, ul2=false, ur2=false, dl2=false, dr2=false, u2r2=false, u2l2=false, d2r2=false, d2l2=false;
	// 
	if ((map->CanStep(loc.x, loc.y, loc.x, loc.y+1)))
	{
		d = true;
		neighbors.push_back(xyzLoc(loc.x, loc.y+1));
                if(connectedness>9 && map->IsTraversable(loc.x, loc.y+2)){
                  d2=true;
                  neighbors.push_back(xyzLoc(loc.x, loc.y+2));
                }
	}
	if ((map->CanStep(loc.x, loc.y, loc.x, loc.y-1)))
	{
		u = true;
		neighbors.push_back(xyzLoc(loc.x, loc.y-1));
                if(connectedness>9 && map->IsTraversable(loc.x, loc.y-2)){
                  u2=true;
                  neighbors.push_back(xyzLoc(loc.x, loc.y-2));
                }
	}
	if ((map->CanStep(loc.x, loc.y, loc.x-1, loc.y)))
        {
          l=true;
          neighbors.push_back(xyzLoc(loc.x-1, loc.y));
          if (connectedness>5){
            // Left is open ...
            if(connectedness>9 && map->IsTraversable(loc.x-2, loc.y)){ // left 2
              l2=true;
              neighbors.push_back(xyzLoc(loc.x-2, loc.y));
            }
            if(u && (map->CanStep(loc.x, loc.y, loc.x-1, loc.y-1))){
              ul=true;
              neighbors.push_back(xyzLoc(loc.x-1, loc.y-1));
              if(connectedness>9){
                // Left, Up, Left2 and UpLeft are open...
                if(l2 && map->IsTraversable(loc.x-2, loc.y-1)){
                  ul2=true;
                  neighbors.push_back(xyzLoc(loc.x-2, loc.y-1));
                }
                // Left, Up2, Up and UpLeft are open...
                if(u2 && map->IsTraversable(loc.x-1, loc.y-2)){
                  u2l=true;
                  neighbors.push_back(xyzLoc(loc.x-1, loc.y-2));
                }
                if(ul2 && u2l && map->IsTraversable(loc.x-2, loc.y-2)){
                  u2l2=true;
                  neighbors.push_back(xyzLoc(loc.x-2, loc.y-2));
                }
              }
            }

            if (d && (map->CanStep(loc.x, loc.y, loc.x-1, loc.y+1))){
              neighbors.push_back(xyzLoc(loc.x-1, loc.y+1));
              dl=true;
              if(connectedness>9){
                // Left, Down, Left2 and UpLeft are open...
                if(l2 && map->IsTraversable(loc.x-2, loc.y+1)){
                  dl2=true;
                  neighbors.push_back(xyzLoc(loc.x-2, loc.y+1));
                }
                // Left, Up2, Up and UpLeft are open...
                if(d2 && map->IsTraversable(loc.x-1, loc.y+2)){
                  d2l=true;
                  neighbors.push_back(xyzLoc(loc.x-1, loc.y+2));
                }
                if(dl2 && d2l && map->IsTraversable(loc.x-2, loc.y+2)){
                  d2l2=true;
                  neighbors.push_back(xyzLoc(loc.x-2, loc.y+2));
                }
              }
            } // down && downleft
          } // connectedness>5
        } // left

	if ((map->CanStep(loc.x, loc.y, loc.x+1, loc.y)))
        {
          r=true;
          neighbors.push_back(xyzLoc(loc.x+1, loc.y));
          if (connectedness>5){
            // Right is open ...
            if(connectedness>9 && map->IsTraversable(loc.x+2, loc.y)){ // right 2
              r2=true;
              neighbors.push_back(xyzLoc(loc.x+2, loc.y));
            }
            if(u && (map->CanStep(loc.x, loc.y, loc.x+1, loc.y-1))){
              ur=true;
              neighbors.push_back(xyzLoc(loc.x+1, loc.y-1));
              if(connectedness>9){
                // Right, Up, Right2 and UpRight are open...
                if(r2 && map->IsTraversable(loc.x+2, loc.y-1)){
                  ur2=true;
                  neighbors.push_back(xyzLoc(loc.x+2, loc.y-1));
                }
                // Right, Up2, Up and UpRight are open...
                if(u2 && map->IsTraversable(loc.x+1, loc.y-2)){
                  u2r=true;
                  neighbors.push_back(xyzLoc(loc.x+1, loc.y-2));
                }
                if(ur2 && u2r && map->IsTraversable(loc.x+2, loc.y-2)){
                  u2r2=true;
                  neighbors.push_back(xyzLoc(loc.x+2, loc.y-2));
                }
              }
            }

            if (d && (map->CanStep(loc.x, loc.y, loc.x+1, loc.y+1))){
              dr=true;
              neighbors.push_back(xyzLoc(loc.x+1, loc.y+1));
              if(connectedness>9){
                // Right, Down, Right2 and UpRight are open...
                if(r2 && map->IsTraversable(loc.x+2, loc.y+1)){
                  dr2=true;
                  neighbors.push_back(xyzLoc(loc.x+2, loc.y+1));
                }
                // Right, Up2, Up and UpRight are open...
                if(d2 && map->IsTraversable(loc.x+1, loc.y+2)){
                  d2r=true;
                  neighbors.push_back(xyzLoc(loc.x+1, loc.y+2));
                }
                if(dr2 && d2r && map->IsTraversable(loc.x+2, loc.y+2)){
                  d2r2=true;
                  neighbors.push_back(xyzLoc(loc.x+2, loc.y+2));
                }
              }
            } // down && downright
          } // connectedness>5
        } // right

        if(connectedness>25){
          if(d2 && map->IsTraversable(loc.x, loc.y+3))
            neighbors.push_back(xyzLoc(loc.x, loc.y+3));
          if(u2 && map->IsTraversable(loc.x, loc.y-3))
            neighbors.push_back(xyzLoc(loc.x, loc.y-3));
          if(r2 && map->IsTraversable(loc.x+3, loc.y))
            neighbors.push_back(xyzLoc(loc.x+3, loc.y));
          if(l2 && map->IsTraversable(loc.x-3, loc.y))
            neighbors.push_back(xyzLoc(loc.x-3, loc.y));

          // ul3
          //if(l2 && map->IsTraversable(loc.x-2, loc.y-1) && map->IsTraversable(loc.x-3, loc.y-1))
          if(l2 && ul2 && map->IsTraversable(loc.x-3, loc.y-1))
            neighbors.push_back(xyzLoc(loc.x-3, loc.y-1));
          // dl3
          //if(l2 && map->IsTraversable(loc.x-2, loc.y+1) && map->IsTraversable(loc.x-3, loc.y+1))
          if(l2 && dl2  && map->IsTraversable(loc.x-3, loc.y+1))
            neighbors.push_back(xyzLoc(loc.x-3, loc.y+1));
          // ur3
          //if(r2 && map->IsTraversable(loc.x+2, loc.y-1) && map->IsTraversable(loc.x+3, loc.y-1))
          if(r2 && ur2 && map->IsTraversable(loc.x+3, loc.y-1))
            neighbors.push_back(xyzLoc(loc.x+3, loc.y-1));
          // dr3
          //if(r2 && map->IsTraversable(loc.x+2, loc.y+1) && map->IsTraversable(loc.x+3, loc.y+1))
          if(r2 && dr2 && map->IsTraversable(loc.x+3, loc.y+1))
            neighbors.push_back(xyzLoc(loc.x+3, loc.y+1));
            
          // u3l
          //if(u2 && map->IsTraversable(loc.x-1, loc.y-2) && map->IsTraversable(loc.x-1, loc.y-3))
          if(u2 && u2l && map->IsTraversable(loc.x-1, loc.y-3))
            neighbors.push_back(xyzLoc(loc.x-1, loc.y-3));
          // d3l
          //if(d2 && map->IsTraversable(loc.x-1, loc.y+2) && map->IsTraversable(loc.x-1, loc.y+3))
          if(d2 && d2l && map->IsTraversable(loc.x-1, loc.y+3))
            neighbors.push_back(xyzLoc(loc.x-1, loc.y+3));
          // u3r
          //if(u2 && map->IsTraversable(loc.x+1, loc.y-2) && map->IsTraversable(loc.x+1, loc.y-3))
          if(u2 && u2r && map->IsTraversable(loc.x+1, loc.y-3))
            neighbors.push_back(xyzLoc(loc.x+1, loc.y-3));
          // d3r
          //if(d2 && map->IsTraversable(loc.x+1, loc.y+2) && map->IsTraversable(loc.x+1, loc.y+3))
          if(d2 && d2r && map->IsTraversable(loc.x+1, loc.y+3))
            neighbors.push_back(xyzLoc(loc.x+1, loc.y+3));
            
          // u2l3
          if(u2l2 && map->IsTraversable(loc.x-3,loc.y-1) && map->IsTraversable(loc.x-3, loc.y-2))
            neighbors.push_back(xyzLoc(loc.x-3, loc.y-2));
          // d2l3
          if(d2l2 && map->IsTraversable(loc.x-3,loc.y+1) && map->IsTraversable(loc.x-3, loc.y+2))
            neighbors.push_back(xyzLoc(loc.x-3, loc.y+2));
          // u2r3
          if(u2r2 && map->IsTraversable(loc.x+3,loc.y-1) && map->IsTraversable(loc.x+3, loc.y-2))
            neighbors.push_back(xyzLoc(loc.x+3, loc.y-2));
          // d2r3
          if(d2r2 && map->IsTraversable(loc.x+3,loc.y+1) && map->IsTraversable(loc.x+3, loc.y+2))
            neighbors.push_back(xyzLoc(loc.x+3, loc.y+2));
            
          // u3l2
          if(u2l2 && map->IsTraversable(loc.x-1,loc.y-3) && map->IsTraversable(loc.x-2, loc.y-3))
            neighbors.push_back(xyzLoc(loc.x-2, loc.y-3));
          // d3l2
          if(d2l2 && map->IsTraversable(loc.x-1,loc.y+3) && map->IsTraversable(loc.x-2, loc.y+3))
            neighbors.push_back(xyzLoc(loc.x-2, loc.y+3));
          // u3r2
          if(u2r2 && map->IsTraversable(loc.x+1,loc.y-3) && map->IsTraversable(loc.x+2, loc.y-3))
            neighbors.push_back(xyzLoc(loc.x+2, loc.y-3));
          // d3r2
          if(d2r2 && map->IsTraversable(loc.x+1,loc.y+3) && map->IsTraversable(loc.x+2, loc.y+3))
            neighbors.push_back(xyzLoc(loc.x+2, loc.y+3));
            
          // u3l3
          if(u2l2 && map->IsTraversable(loc.x-2,loc.y-3) && map->IsTraversable(loc.x-3,loc.y-2) && map->IsTraversable(loc.x-3, loc.y-3))
            neighbors.push_back(xyzLoc(loc.x-3, loc.y-3));
          // d3l3
          if(d2l2 && map->IsTraversable(loc.x-2,loc.y+3) && map->IsTraversable(loc.x-3,loc.y+2) && map->IsTraversable(loc.x-3, loc.y+3))
            neighbors.push_back(xyzLoc(loc.x-3, loc.y+3));
          // u3r3
          if(u2r2 && map->IsTraversable(loc.x+2,loc.y-3) && map->IsTraversable(loc.x+3,loc.y-2) && map->IsTraversable(loc.x+3, loc.y-3))
            neighbors.push_back(xyzLoc(loc.x+3, loc.y-3));
          // d3r3
          if(d2r2 && map->IsTraversable(loc.x+2,loc.y+3) && map->IsTraversable(loc.x+3,loc.y+2) && map->IsTraversable(loc.x+3, loc.y+3))
            neighbors.push_back(xyzLoc(loc.x+3, loc.y+3));
        }
        if(connectedness%2) // Is waiting allowed?
        {
		neighbors.push_back(loc);
        }
}

bool Grid3DEnvironment::GetNextSuccessor(const xyzLoc &currOpenNode, const xyzLoc &goal,
									  xyzLoc &next, double &currHCost, uint64_t &special,
									  bool &validMove)
{
  // In the case of 5-connectedness; next successor does not look at waiting in place
  if (connectedness<6)
    return GetNext4Successor(currOpenNode, goal, next, currHCost, special, validMove);

  // In the case of 9-connectedness; next successor does not look at waiting in place
  return GetNext8Successor(currOpenNode, goal, next, currHCost, special, validMove);

}

bool Grid3DEnvironment::GetNext4Successor(const xyzLoc &currOpenNode, const xyzLoc &goal,
									   xyzLoc &next, double &currHCost, uint64_t &special,
									  bool &validMove)
{
	validMove = false;
	if (special > 3)
		return false;
	// pass back next h-cost?
	// 4 connected:
	// case 2: above and right: Up, Right, Left, Down
	// case 3: directly right: Right, Down, Up, Left
	// case 4: below and right: Right, Down, Up, Left
	// case 5: directly below: Down, Left, Right, Up
	// case 6: below and left: Down, Left, Right, Up
	// case 7: directly left: Left, Up, Down, Right
	// case 8: above and left: Left, Up, Down, Right
	
	// 1,2. same y and different x (+/-)
	// 3,4. same x and different y (+/-)
	// 5,6,7,8. same x/y difference (+/-) combinations
	int theEntry = 0;
	const tDirection order[8][8] =
	{
		{kN, kE, kW, kS},
		{kS, kE, kW, kN},
		{kW, kN, kS, kE},
		{kE, kN, kS, kW},

		{kN, kW, kE, kS},
		{kW, kS, kN, kE},
		{kN, kE, kW, kS},
		{kS, kE, kW, kN},
	};
	const double hIncrease[8][8] = 
	{
		{1.0, 0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0, 0.0},
		{1.0, 0.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
		{0.0, 1.0, 0.0, 0.0},
	};
	
	if      (currOpenNode.x == goal.x && currOpenNode.y > goal.y)
	{ theEntry = 0; }
	else if (currOpenNode.x == goal.x && currOpenNode.y < goal.y)
	{ theEntry = 1; }
	else if (currOpenNode.x > goal.x && currOpenNode.y == goal.y)
	{ theEntry = 2; }
	else if (currOpenNode.x < goal.x && currOpenNode.y == goal.y)
	{ theEntry = 3; }
	else if (currOpenNode.x > goal.x && currOpenNode.y > goal.y)
	{ theEntry = 4; }
	else if (currOpenNode.x > goal.x && currOpenNode.y < goal.y)
	{ theEntry = 5; }
	else if (currOpenNode.x < goal.x && currOpenNode.y > goal.y)
	{ theEntry = 6; }
	else if (currOpenNode.x < goal.x && currOpenNode.y < goal.y)
	{ theEntry = 7; }

//	std::cout << special << " h from " << currHCost << " to "
//	<< currHCost + hIncrease[theEntry][special] << std::endl;
	switch (special) {
		case 0:
			next = currOpenNode;
			currHCost += hIncrease[theEntry][special];
			ApplyAction(next, order[theEntry][special]);
			special++;
			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
			{
				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
				validMove = true;
				return true;
			}
		case 1:
			next = currOpenNode;
			currHCost += hIncrease[theEntry][special];
			ApplyAction(next, order[theEntry][special]);
			special++;
			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
			{
				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
				validMove = true;
				return true;
			}
		case 2:
			next = currOpenNode;
			currHCost += 1;
			currHCost += hIncrease[theEntry][special];
			ApplyAction(next, order[theEntry][special]);
			special++;
			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
			{
				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
				validMove = true;
				return true;
			}
		case 3:
			next = currOpenNode;
			currHCost += hIncrease[theEntry][special];
			ApplyAction(next, order[theEntry][special]);
			special++;
			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
			{
				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
				validMove = true;
				return false;
			}
		default:
			return false;
	}

	return false;
}

bool Grid3DEnvironment::GetNext8Successor(const xyzLoc &currOpenNode, const xyzLoc &goal,
									   xyzLoc &next, double &currHCost, uint64_t &special,
									   bool &validMove)
{
	// in addition to the 16 obvious cases, when diagonal movess cross the 
	// diagonals we have separate cases. Thus, we don't implement this for now.
	
	// Diagonal movement when next to the goal could also be problematic, depending on the
	// map representation
	assert(false);
	
//	// it must be 1.5 for this code to be correct...
//	assert(DIAGONAL_COST == 1.5);
//	validMove = false;
//	if (special > 7) // moves
//		return false;
//	// pass back next h-cost?
//	// 4 connected:
//	// case 2: above and right: Up, Right, Left, Down
//	// case 3: directly right: Right, Down, Up, Left
//	// case 4: below and right: Right, Down, Up, Left
//	// case 5: directly below: Down, Left, Right, Up
//	// case 6: below and left: Down, Left, Right, Up
//	// case 7: directly left: Left, Up, Down, Right
//	// case 8: above and left: Left, Up, Down, Right
//	
//	// 1,2. same y and different x (+/-)
//	// 3,4. same x and different y (+/-)
//	// 5,6,7,8. same x/y difference (+/-) combinations
//	int theEntry = 0;
//	const tDirection order[8][8] =
//	{
//		// directly above
//		{kN, kNW, kNE, kE, kW, kS, kSE, kSW},
//		// more above to the right
//		{kN, kNE, kE, kNW, kW, kS, kSE, kSW},
//		// diagonally right
//		{kNE, kN, kE, kNW, kSE, kS, kW, kSW},
//		// more right and above
//		{kE, kNE, kN, kS, kSE, kW, kNW, kSW},
//
//		{kE, kN, kS, kW},
//		
//		{kN, kW, kE, kS},
//		{kW, kS, kN, kE},
//		{kN, kE, kW, kS},
//		{kS, kE, kW, kN},
//	};
//	const double hIncrease[8][8] = 
//	{
//		// directly above
//		{1.0, 0.0, 0.5, 0.0, 0.5, 1.0, 0.0, 0.0},
//		// more above to the right
//		{0.0, 0.5, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0},
//		// diagonally right
//		{0.5, 0.0, 1.5, 0.0, 0.0, 0.0, 1.0, 0.0},
//		// more right and above
//		{0.0, 0.5, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0},
//		{0.0, 1.0, 0.0, 0.0},
//		{0.0, 1.0, 0.0, 0.0},
//		{0.0, 1.0, 0.0, 0.0},
//		{0.0, 1.0, 0.0, 0.0},
//	};
//	
//	if      (currOpenNode.x == goal.x && currOpenNode.y > goal.y)
//	{ theEntry = 0; }
//	else if (currOpenNode.x == goal.x && currOpenNode.y < goal.y)
//	{ theEntry = 1; }
//	else if (currOpenNode.x > goal.x && currOpenNode.y == goal.y)
//	{ theEntry = 2; }
//	else if (currOpenNode.x < goal.x && currOpenNode.y == goal.y)
//	{ theEntry = 3; }
//	else if (currOpenNode.x > goal.x && currOpenNode.y > goal.y)
//	{ theEntry = 4; }
//	else if (currOpenNode.x > goal.x && currOpenNode.y < goal.y)
//	{ theEntry = 5; }
//	else if (currOpenNode.x < goal.x && currOpenNode.y > goal.y)
//	{ theEntry = 6; }
//	else if (currOpenNode.x < goal.x && currOpenNode.y < goal.y)
//	{ theEntry = 7; }
//	
//	//	std::cout << special << " h from " << currHCost << " to "
//	//	<< currHCost + hIncrease[theEntry][special] << std::endl;
//	switch (special) {
//		case 0:
//			next = currOpenNode;
//			currHCost += hIncrease[theEntry][special];
//			ApplyAction(next, order[theEntry][special]);
//			special++;
//			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
//			{
//				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
//				validMove = true;
//				return true;
//			}
//		case 1:
//			next = currOpenNode;
//			currHCost += hIncrease[theEntry][special];
//			ApplyAction(next, order[theEntry][special]);
//			special++;
//			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
//			{
//				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
//				validMove = true;
//				return true;
//			}
//		case 2:
//			next = currOpenNode;
//			currHCost += 1;
//			currHCost += hIncrease[theEntry][special];
//			ApplyAction(next, order[theEntry][special]);
//			special++;
//			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
//			{
//				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
//				validMove = true;
//				return true;
//			}
//		case 3:
//			next = currOpenNode;
//			currHCost += hIncrease[theEntry][special];
//			ApplyAction(next, order[theEntry][special]);
//			special++;
//			if (map->CanStep(currOpenNode.x, currOpenNode.y, next.x, next.y))
//			{
//				//std::cout << "Next successor of " << currOpenNode << " is " << next << std::endl;
//				validMove = true;
//				return false;
//			}
//		default:
//			return false;
//	}
//	
//	return false;
}

void Grid3DEnvironment::GetActions(const xyzLoc &loc, std::vector<tDirection> &actions) const
{
	bool up=false, down=false;
	if ((map->CanStep(loc.x, loc.y, loc.x, loc.y+1)))
	{
		down = true;
		actions.push_back(kS);
	}
	if ((map->CanStep(loc.x, loc.y, loc.x, loc.y-1)))
	{
		up = true;
		actions.push_back(kN);
	}
	if ((map->CanStep(loc.x, loc.y, loc.x-1, loc.y)))
	{
		if (connectedness>5)
		{
			if ((up && (map->CanStep(loc.x, loc.y, loc.x-1, loc.y-1))))
				actions.push_back(kNW);
			if ((down && (map->CanStep(loc.x, loc.y, loc.x-1, loc.y+1))))
				actions.push_back(kSW);
		}
		actions.push_back(kW);
	}
	if ((map->CanStep(loc.x, loc.y, loc.x+1, loc.y)))
	{
		if (connectedness>5)
		{
			if ((up && (map->CanStep(loc.x, loc.y, loc.x+1, loc.y-1))))
				actions.push_back(kNE);
			if ((down && (map->CanStep(loc.x, loc.y, loc.x+1, loc.y+1))))
				actions.push_back(kSE);
		}
		actions.push_back(kE);
	}
        if(connectedness%2)
          actions.push_back(kStay);
// TODO: add 24 and 48 cases.
}

tDirection Grid3DEnvironment::GetAction(const xyzLoc &s1, const xyzLoc &s2) const
{
	int result = kStay;
	switch (s1.x-s2.x)
	{
		case -1: result = kE; break;
		case 0: break;
		case 1: result = kW; break;
		default: return kTeleport;
	}
	
	// Tack the vertical move onto it
	// Notice the exploit of particular encoding of kStay, kE, etc. labels
	switch (s1.y-s2.y)
	{
		case -1: result = result|kS; break;
		case 0: break;
		case 1: result = result|kN; break;
		default: return kTeleport;
	}
	return (tDirection)result;
}

bool Grid3DEnvironment::InvertAction(tDirection &a) const
{
	switch (a)
	{
		case kN: a = kS; break;
		case kNE: a = kSW; break;
		case kE: a = kW; break;
		case kSE: a = kNW; break;
		case kS: a = kN; break;
		case kSW: a = kNE; break;
		case kW: a = kE; break;
		case kNW: a = kSE; break;

		case kNN: a = kSS; break;
		case kNNE: a = kSSW; break;
		case kNEE: a = kSWW; break;
		case kNNEE: a = kSSWW; break;
		case kEE: a = kWW; break;
		case kSSE: a = kNNW; break;
		case kSEE: a = kNWW; break;
		case kSSEE: a = kNNWW; break;
		case kSS: a = kNN; break;
		case kSSW: a = kNNE; break;
		case kSWW: a = kNEE; break;
		case kSSWW: a = kNNEE; break;
		case kWW: a = kEE; break;
		case kNNW: a = kSSE; break;
		case kNWW: a = kSEE; break;
		case kNNWW: a = kSSEE; break;

		case kNNN: a = kSSS; break;
		case kNNNE: a = kSSSW; break;
		case kNEEE: a = kSWWW; break;
		case kNNNEE: a = kSSSWW; break;
		case kNNEEE: a = kSSWWW; break;
		case kNNNEEE: a = kSSSWWW; break;
		case kEEE: a = kWWW; break;
		case kSSSE: a = kNNNW; break;
		case kSEEE: a = kNWWW; break;
		case kSSEEE: a = kNNWWW; break;
		case kSSSEE: a = kNNNWW; break;
		case kSSSEEE: a = kNNNWWW; break;
		case kSSS: a = kNNN; break;
		case kSSSW: a = kNNNE; break;
		case kSWWW: a = kNEEE; break;
		case kSSWWW: a = kNNEEE; break;
		case kSSSWW: a = kNNNEE; break;
		case kSSSWWW: a = kNNNEEE; break;
		case kWWW: a = kEEE; break;
		case kNNNW: a = kSSSE; break;
		case kNWWW: a = kSEEE; break;
		case kNNNWW: a = kSSSEE; break;
		case kNNWWW: a = kSSEEE; break;
		case kNNNWWW: a = kSSSEEE; break;
		default: break;
	}
	return true;
}

void Grid3DEnvironment::ApplyAction(xyzLoc &s, tDirection dir) const
{
	//xyzLoc old = s;
	switch (dir)
	{
		case kN: s.y-=1; break;
		case kS: s.y+=1; break;
		case kE: s.x+=1; break;
		case kW: s.x-=1; break;
		case kNW: s.y-=1; s.x-=1; break;
		case kSW: s.y+=1; s.x-=1; break;
		case kNE: s.y-=1; s.x+=1; break;
		case kSE: s.y+=1; s.x+=1; break;

		case kNN: s.y-=2; break;
		case kNNE: s.y-=2; s.x+=1; break;
		case kNEE: s.y-=1; s.x+=2; break;
		case kNNEE: s.y-=2; s.x+=2; break;
		case kEE: s.x+=2; break;
		case kSSE: s.y+=2; s.x+=1; break;
		case kSEE: s.y+=1; s.x+=2; break;
		case kSSEE: s.y+=2; s.x+=2; break;
		case kSS: s.y+=2; break;
		case kSSW: s.y+=2; s.x-=1; break;
		case kSWW: s.y+=1; s.x-=2; break;
		case kSSWW: s.y+=2; s.x-=2; break;
		case kWW: s.x-=2; break;
		case kNNW: s.y-=2; s.x-=1; break;
		case kNWW: s.y-=1; s.x-=2; break;
		case kNNWW: s.y-=2; s.x-=2; break;

		case kNNN: s.y-=3; break;
		case kNNNE: s.y-=3; s.x+=1; break;
		case kNEEE: s.y-=1; s.x+=3; break;
		case kNNNEE: s.y-=3; s.x+=2; break;
		case kNNEEE: s.y-=2; s.x+=3; break;
		case kNNNEEE: s.y-=3; s.x+=3; break;
		case kEEE: s.x+=3; break;
		case kSSSE: s.y+=3; s.x+=1; break;
		case kSEEE: s.y+=1; s.x+=3; break;
		case kSSEEE: s.y+=2; s.x+=3; break;
		case kSSSEE: s.y+=3; s.x+=2; break;
		case kSSSEEE: s.y+=3; s.x+=3; break;
		case kSSS: s.y+=3; break;
		case kSSSW: s.y+=3; s.x-=1; break;
		case kSWWW: s.y+=1; s.x-=3; break;
		case kSSWWW: s.y+=2; s.x-=3; break;
		case kSSSWW: s.y+=3; s.x-=2; break;
		case kSSSWWW: s.y+=3; s.x-=3; break;
		case kWWW: s.x-=3; break;
		case kNNNW: s.y-=3; s.x-=1; break;
		case kNWWW: s.y-=1; s.x-=3; break;
		case kNNNWW: s.y-=3; s.x-=2; break;
		case kNNWWW: s.y-=2; s.x-=3; break;
		case kNNNWWW: s.y-=3; s.x-=3; break;

		default: break;
	}
//	if (map->CanStep(s.x, s.y, old.x, old.y) &&
//		((!oi) || (oi && !(oi->GetStateOccupied(s)))))
//	{
//		return;
//	}
//	s = old;
}

double Grid3DEnvironment::HCost(const xyzLoc &l1, const xyzLoc &l2) const
{
        if(connectedness>49){
          // Straight line distance
          return Util::distance(l1.x,l1.y,l2.x,l2.y);
        }
	double h1, h2;
	if (connectedness<6)
	{
		h1 = abs(l1.x-l2.x)+abs(l1.y-l2.y);
	}
	else {
		double a = ((l1.x>l2.x)?(l1.x-l2.x):(l2.x-l1.x));
		double b = ((l1.y>l2.y)?(l1.y-l2.y):(l2.y-l1.y));
		//return sqrt(a*a+b*b);
		h1 = (a>b)?(b*DIAGONAL_COST+a-b):(a*DIAGONAL_COST+b-a);
	}

	if (h == 0)
		return h1;
	
	int n1 = map->GetNodeNum(l1.x, l1.y);
	int n2 = map->GetNodeNum(l2.x, l2.y);
	if ((n1 != -1) && (n2 != -1))
	{
		graphState nn1 = n1;
		graphState nn2 = n2;
		h2 = h->HCost(nn1, nn2);
	}
	else
		h2 = 0;
	return std::max(h1, h2);
}

double Grid3DEnvironment::GCost(const xyzLoc &l, const tDirection &act) const
{
	double multiplier = 1.0;
//	if (map->GetTerrainType(l.x, l.y) == kSwamp)
//	{
//		multiplier = 3.0;
//	}
	switch (act)
	{
		case kN: return 1.0*multiplier;
		case kS: return 1.0*multiplier;
		case kE: return 1.0*multiplier;
		case kW: return 1.0*multiplier;
		case kNW: return DIAGONAL_COST*multiplier;
		case kSW: return DIAGONAL_COST*multiplier;
		case kNE: return DIAGONAL_COST*multiplier;
		case kSE: return DIAGONAL_COST*multiplier;

		case kNN: 2.0*multiplier;
		case kNNE: SQRT_5*multiplier;
		case kNEE: SQRT_5*multiplier;
		case kNNEE: 2.0*DIAGONAL_COST*multiplier;
		case kEE: 2.0*multiplier;
		case kSSE: SQRT_5*multiplier;
		case kSEE: SQRT_5*multiplier;
		case kSSEE: 2.0*DIAGONAL_COST*multiplier;
		case kSS: 2.0*multiplier;
		case kSSW: SQRT_5*multiplier;
		case kSWW: SQRT_5*multiplier;
		case kSSWW: 2.0*DIAGONAL_COST*multiplier;
		case kWW: 2.0*multiplier;
		case kNNW: SQRT_5*multiplier;
		case kNWW: SQRT_5*multiplier;
		case kNNWW: 2.0*DIAGONAL_COST*multiplier;

		case kNNN: 3.0*multiplier;
		case kNNNE: SQRT_10*multiplier;
		case kNEEE: SQRT_10*multiplier;
		case kNNNEE: SQRT_13*multiplier;
		case kNNEEE: SQRT_13*multiplier;
		case kNNNEEE: 3.0*DIAGONAL_COST*multiplier;
		case kEEE: 3.0*multiplier;
		case kSSSE: SQRT_10*multiplier;
		case kSEEE: SQRT_10*multiplier;
		case kSSEEE: SQRT_13*multiplier;
		case kSSSEE: SQRT_13*multiplier;
		case kSSSEEE: 3.0*DIAGONAL_COST*multiplier;
		case kSSS: 3.0*multiplier;
		case kSSSW: SQRT_10*multiplier;
		case kSWWW: SQRT_10*multiplier;
		case kSSWWW: SQRT_13*multiplier;
		case kSSSWW: SQRT_13*multiplier;
		case kSSSWWW: 3.0*DIAGONAL_COST*multiplier;
		case kWWW: 3.0*multiplier;
		case kNNNW: SQRT_10*multiplier;
		case kNWWW: SQRT_10*multiplier;
		case kNNNWW: SQRT_13*multiplier;
		case kNNWWW: SQRT_13*multiplier;
		case kNNNWWW: 3.0*DIAGONAL_COST*multiplier;
		default: return 0;
	}
	return 0;
}

double Grid3DEnvironment::GCost(const xyzLoc &l1, const xyzLoc &l2) const
{
	double multiplier = 1.0;
//	if (map->GetTerrainType(l1.x, l1.y) == kSwamp)
//	{
//		multiplier = 3.0;
//	}
	if (l1.x == l2.x) return abs(l1.y-l2.y)*multiplier;
	if (l1.y == l2.y) return abs(l1.x-l2.x)*multiplier;
	if (l1 == l2) return 0.0;
	if (abs(l1.x-l2.x)==1) return DIAGONAL_COST*multiplier;
        return Util::distance(l1.x,l1.y,l2.x,l2.y);
//	double h = HCost(l1, l2);
//	if (fgreater(h, DIAGONAL_COST))
//		return DBL_MAX;
//	return h;
}

bool Grid3DEnvironment::LineOfSight(const xyzLoc &node, const xyzLoc &goal) const{
  return map->LineOfSight(node.x,node.y,goal.x,goal.y);
}

bool Grid3DEnvironment::GoalTest(const xyzLoc &node, const xyzLoc &goal) const
{
	return ((node.x == goal.x) && (node.y == goal.y));
}

uint64_t Grid3DEnvironment::GetMaxHash() const
{
	return map->GetMapWidth()*map->GetMapHeight();
}

uint64_t Grid3DEnvironment::GetStateHash(const xyzLoc &node) const
{
	//return (((uint64_t)node.x)<<16)|node.y;
	return node.y*map->GetMapWidth()+node.x;
	//	return (node.x<<16)|node.y;
}

uint64_t Grid3DEnvironment::GetActionHash(tDirection act) const
{
	return (uint32_t) act;
}

void Grid3DEnvironment::OpenGLDraw() const
{
	//std::cout<<"drawing\n";
	map->OpenGLDraw();
	// Draw occupancy interface - occupied = white
//	for (int i=0; i<map->GetMapWidth(); i++)
//		for (int j=0; j<map->GetMapHeight(); j++)
//		{
//			xyzLoc l;
//			l.x = i;
//			l.y = j;
//			if (oi && oi->GetStateOccupied(l))
//			{
//				SetColor(1.0, 1.0, 1.0, 1.0);
//				OpenGLDraw(l);//, 1.0, 1.0, 1.0);
//			}
//		}
}
	


void Grid3DEnvironment::OpenGLDraw(const xyzLoc &l) const
{
	GLdouble xx, yy, zz, rad;
	map->GetOpenGLCoord(l.x, l.y, xx, yy, zz, rad);
	GLfloat r, g, b, t;
	GetColor(r, g, b, t);
	glColor4f(r, g, b, t);
	//glColor3f(0.5, 0.5, 0.5);
	DrawSphere(xx, yy, zz, rad);
}

void Grid3DEnvironment::OpenGLDraw(const xyzLoc &l1, const xyzLoc &l2, float v) const
{
	GLdouble xx, yy, zz, rad;
	GLdouble xx2, yy2, zz2;
//	map->GetOpenGLCoord((float)((1-v)*l1.x+v*l2.x),
//						(float)((1-v)*l1.y+v*l2.y), xx, yy, zz, rad);
//	printf("%f between (%d, %d) and (%d, %d)\n", v, l1.x, l1.y, l2.x, l2.y);
	map->GetOpenGLCoord(l1.x, l1.y, xx, yy, zz, rad);
	map->GetOpenGLCoord(l2.x, l2.y, xx2, yy2, zz2, rad);
	//	map->GetOpenGLCoord(perc*newState.x + (1-perc)*oldState.x, perc*newState.y + (1-perc)*oldState.y, xx, yy, zz, rad);
	xx = (1-v)*xx+v*xx2;
	yy = (1-v)*yy+v*yy2;
	zz = (1-v)*zz+v*zz2;
	GLfloat r, g, b, t;
	GetColor(r, g, b, t);
	glColor4f(r, g, b, t);
	DrawSphere(xx, yy, zz, rad);
}

//void Grid3DEnvironment::OpenGLDraw(const xyzLoc &l, GLfloat r, GLfloat g, GLfloat b) const
//{
//	GLdouble xx, yy, zz, rad;
//	map->GetOpenGLCoord(l.x, l.y, xx, yy, zz, rad);
//	glColor3f(r,g,b);
//	DrawSphere(xx, yy, zz, rad);
//}


void Grid3DEnvironment::OpenGLDraw(const xyzLoc& initial, const tDirection &dir) const
{
	
	xyzLoc s = initial;
	GLdouble xx, yy, zz, rad;
	map->GetOpenGLCoord(s.x, s.y, xx, yy, zz, rad);
	
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_LINE_STRIP);
	glVertex3f(xx, yy, zz-rad/2);
		
	switch (dir)
	{
		case kN: s.y-=1; break;
		case kS: s.y+=1; break;
		case kE: s.x+=1; break;
		case kW: s.x-=1; break;
		case kNW: s.y-=1; s.x-=1; break;
		case kSW: s.y+=1; s.x-=1; break;
		case kNE: s.y-=1; s.x+=1; break;
		case kSE: s.y+=1; s.x+=1; break;

		case kNN: s.y-=2; break;
		case kNNE: s.y-=2; s.x+=1; break;
		case kNEE: s.y-=1; s.x+=2; break;
		case kNNEE: s.y-=2; s.x+=2; break;
		case kEE: s.x+=2; break;
		case kSSE: s.y+=2; s.x+=1; break;
		case kSEE: s.y+=1; s.x+=2; break;
		case kSSEE: s.y+=2; s.x+=2; break;
		case kSS: s.y+=2; break;
		case kSSW: s.y+=2; s.x-=1; break;
		case kSWW: s.y+=1; s.x-=2; break;
		case kSSWW: s.y+=2; s.x-=2; break;
		case kWW: s.x-=2; break;
		case kNNW: s.y-=2; s.x-=1; break;
		case kNWW: s.y-=1; s.x-=2; break;
		case kNNWW: s.y-=2; s.x-=2; break;

		case kNNN: s.y-=3; break;
		case kNNNE: s.y-=3; s.x+=1; break;
		case kNEEE: s.y-=1; s.x+=3; break;
		case kNNNEE: s.y-=3; s.x+=2; break;
		case kNNEEE: s.y-=2; s.x+=3; break;
		case kNNNEEE: s.y-=3; s.x+=3; break;
		case kEEE: s.x+=3; break;
		case kSSSE: s.y+=3; s.x+=1; break;
		case kSEEE: s.y+=1; s.x+=3; break;
		case kSSEEE: s.y+=2; s.x+=3; break;
		case kSSSEE: s.y+=3; s.x+=2; break;
		case kSSSEEE: s.y+=3; s.x+=3; break;
		case kSSS: s.y+=3; break;
		case kSSSW: s.y+=3; s.x-=1; break;
		case kSWWW: s.y+=1; s.x-=3; break;
		case kSSWWW: s.y+=2; s.x-=3; break;
		case kSSSWW: s.y+=3; s.x-=2; break;
		case kSSSWWW: s.y+=3; s.x-=3; break;
		case kWWW: s.x-=3; break;
		case kNNNW: s.y-=3; s.x-=1; break;
		case kNWWW: s.y-=1; s.x-=3; break;
		case kNNNWW: s.y-=3; s.x-=2; break;
		case kNNWWW: s.y-=2; s.x-=3; break;
		case kNNNWWW: s.y-=3; s.x-=3; break;
		default: break;
	}

	
	map->GetOpenGLCoord(s.x, s.y, xx, yy, zz, rad);
	glVertex3f(xx, yy, zz-rad/2);
	glEnd();
	
}

void Grid3DEnvironment::GLDrawLine(const xyzLoc &a, const xyzLoc &b) const
{
	GLdouble xx1, yy1, zz1, rad;
	GLdouble xx2, yy2, zz2;
	map->GetOpenGLCoord(a.x, a.y, xx1, yy1, zz1, rad);
	map->GetOpenGLCoord(b.x, b.y, xx2, yy2, zz2, rad);
	
	double angle = atan2(yy1-yy2, xx1-xx2);
	double xoff = sin(2*PI-angle)*rad*0.1;
	double yoff = cos(2*PI-angle)*rad*0.1;

	
	
	GLfloat rr, gg, bb, t;
	GetColor(rr, gg, bb, t);
	glColor4f(rr, gg, bb, t);

	
	glBegin(GL_LINES);
	glVertex3f(xx1, yy1, zz1-rad/2);
	glVertex3f(xx2, yy2, zz2-rad/2);
	glEnd();

//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
	//glEnable(GL_POLYGON_SMOOTH);
//	glBegin(GL_TRIANGLE_STRIP);
//	//glBegin(GL_QUADS);
//	glVertex3f(xx1+xoff, yy1+yoff, zz1-rad/2);
//	glVertex3f(xx2+xoff, yy2+yoff, zz2-rad/2);
//	glVertex3f(xx1-xoff, yy1-yoff, zz1-rad/2);
//	glVertex3f(xx2-xoff, yy2-yoff, zz2-rad/2);
//	glEnd();

	//	glDisable(GL_POLYGON_SMOOTH);
	//
//	glBegin(GL_LINES);
//	glVertex3f(xx, yy, zz-rad/2);
//	map->GetOpenGLCoord(b.x, b.y, xx, yy, zz, rad);
//	glVertex3f(xx, yy, zz-rad/2);
//	glEnd();
}

void Grid3DEnvironment::GLLabelState(const xyzLoc &s, const char *str, double scale) const
{
	glPushMatrix();
	
	GLdouble xx, yy, zz, rad;
	map->GetOpenGLCoord(s.x, s.y, xx, yy, zz, rad);
	GLfloat r, g, b, t;
	GetColor(r, g, b, t);
	glColor4f(r, g, b, t);
	
	glTranslatef(xx-rad, yy+rad/2, zz-2*rad);
	glScalef(scale*rad/(300.0), scale*rad/300.0, 1);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	//glTranslatef((float)x/width-0.5, (float)y/height-0.5, 0);
	glDisable(GL_LIGHTING);
	for (int which = 0; which < strlen(str); which++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[which]);
	glEnable(GL_LIGHTING);
	//glTranslatef(-x/width+0.5, -y/height+0.5, 0);
	glPopMatrix();
}

void Grid3DEnvironment::GLLabelState(const xyzLoc &s, const char *str) const
{
	glPushMatrix();

	GLdouble xx, yy, zz, rad;
	map->GetOpenGLCoord(s.x, s.y, xx, yy, zz, rad);
	GLfloat r, g, b, t;
	GetColor(r, g, b, t);
	glColor4f(r, g, b, t);
	
	glTranslatef(xx-rad, yy+rad/2, zz-rad);
	glScalef(rad/(300.0), rad/300.0, 1);
	glRotatef(180, 0.0, 0.0, 1.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	//glTranslatef((float)x/width-0.5, (float)y/height-0.5, 0);
	glDisable(GL_LIGHTING);
	for (int which = 0; which < strlen(str); which++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[which]);
	glEnable(GL_LIGHTING);
	//glTranslatef(-x/width+0.5, -y/height+0.5, 0);
	glPopMatrix();
}

std::string Grid3DEnvironment::SVGHeader()
{
	std::string s;
	// 10% margin on all sides of image
	s = "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width = \""+std::to_string(10*map->GetMapWidth())+"\" height = \""+std::to_string(10*map->GetMapHeight())+"\" ";
	s += "viewBox=\""+std::to_string(-map->GetMapWidth())+" "+std::to_string(-map->GetMapHeight())+" ";
	s += std::to_string(12*map->GetMapWidth())+" "+std::to_string(12*map->GetMapHeight())+"\" ";
	s += "preserveAspectRatio = \"none\" ";
	s += ">\n";
	return s;
}

std::string Grid3DEnvironment::SVGDraw()
{
	std::string s;
	recColor black = {0.0, 0.0, 0.0};
	
	// draw tiles
	for (int y = 0; y < map->GetMapHeight(); y++)
	{
		for (int x = 0; x < map->GetMapWidth(); x++)
		{
			bool draw = true;
			if (map->GetTerrainType(x, y) == kGround)
			{
				recColor c = {0.9, 0.9, 0.9};
				s += SVGDrawRect(x+1, y+1, 1, 1, c);
				s += "\n";
			}
			else if (map->GetTerrainType(x, y) == kTrees)
			{
				recColor c = {0.0, 0.5, 0.0};
				s += SVGDrawRect(x+1, y+1, 1, 1, c);
				s += "\n";
			}
			else if (map->GetTerrainType(x, y) == kWater)
			{
				recColor c = {0.0, 0.0, 1.0};
				s += SVGDrawRect(x+1, y+1, 1, 1, c);
				s += "\n";
			}
			else if (map->GetTerrainType(x, y) == kSwamp)
			{
				recColor c = {0.0, 0.3, 1.0};
				s += SVGDrawRect(x+1, y+1, 1, 1, c);
				s += "\n";
			}
			else {
				draw = false;
			}
		}
	}
	
	// draw cell boundaries for open terrain
	if (0)
	for (int y = 0; y < map->GetMapHeight(); y++)
	{
		for (int x = 0; x < map->GetMapWidth(); x++)
		{
			// mark cells on map
			if ((map->GetTerrainType(x, y)>>terrainBits) == (kGround>>terrainBits))
			{
				recColor c = {0.75, 0.75, 0.75};
				s += ::SVGFrameRect(x+1, y+1, 1, 1, 1, c);
				s += "\n";
			}
		}
	}

	// draw lines between different terrain types
	for (int y = 0; y < map->GetMapHeight(); y++)
	{
		for (int x = 0; x < map->GetMapWidth(); x++)
		{
			bool draw = true;
			if (map->GetTerrainType(x, y) == kGround)
			{
				if (x == map->GetMapWidth()-1)
					s += ::SVGDrawLine(x+1+1, y+1, x+1+1, y+1+1, 1, black, false);
				if (y == map->GetMapHeight()-1)
					s += ::SVGDrawLine(x+1, y+1+1, x+1+1, y+1+1, 1, black, false);
			}
			else if (map->GetTerrainType(x, y) == kTrees)
			{
				if (x == map->GetMapWidth()-1)
					s += ::SVGDrawLine(x+1+1, y+1, x+1+1, y+1+1, 1, black, false);
				if (y == map->GetMapHeight()-1)
					s += ::SVGDrawLine(x+1, y+1+1, x+1+1, y+1+1, 1, black, false);
			}
			else if (map->GetTerrainType(x, y) == kWater)
			{
				if (x == map->GetMapWidth()-1)
					s += ::SVGDrawLine(x+1+1, y+1, x+1+1, y+1+1, 1, black, false);
				if (y == map->GetMapHeight()-1)
					s += ::SVGDrawLine(x+1, y+1+1, x+1+1, y+1+1, 1, black, false);
			}
			else if (map->GetTerrainType(x, y) == kSwamp)
			{
			}
			else {
				draw = false;
			}

			if (draw)
			{
				SetColor(0.0, 0.0, 0.0);

				// Code does error checking, so this works with x == 0
				if (map->GetTerrainType(x, y) != map->GetTerrainType(x-1, y))
				{
					SetColor(0.0, 0.0, 0.0);
					s += ::SVGDrawLine(x+1, y+1, x+1, y+1+1, 1, black, false);
					s += "\n";
				}

				if (map->GetTerrainType(x, y) != map->GetTerrainType(x, y-1))
				{
					s += ::SVGDrawLine(x+1, y+1, x+1+1, y+1, 1, black, false);
					s += "\n";
				}
				
				if (map->GetTerrainType(x, y) != map->GetTerrainType(x+1, y))
				{
					s += ::SVGDrawLine(x+1+1, y+1, x+1+1, y+1+1, 1, black, false);
					s += "\n";
				}
				
				if (map->GetTerrainType(x, y) != map->GetTerrainType(x, y+1))
				{
					s += ::SVGDrawLine(x+1, y+1+1, x+1+1, y+1+1, 1, black, false);
					s += "\n";
				}
			}

		}
	}
	s += "\n";

	return s;
}

std::string Grid3DEnvironment::SVGDraw(const xyzLoc &l)
{
	std::string s;
	if (map->GetTerrainType(l.x, l.y) == kGround)
	{
		recColor c;// = {0.5, 0.5, 0};
		GLfloat t;
		GetColor(c.r, c.g, c.b, t);
		s += SVGDrawCircle(l.x+0.5+1, l.y+0.5+1, 0.5, c);
		//stroke-width="1" stroke="pink" />
	}
	return s;
}

std::string Grid3DEnvironment::SVGFrameRect(int left, int top, int right, int bottom, int width)
{
	std::string s;

	recColor c;// = {0.5, 0.5, 0};
	GLfloat t;
	GetColor(c.r, c.g, c.b, t);
	s += ::SVGFrameRect(left+1, top+1, right-left+1, bottom-top+1, width, c);

	return s;
}

std::string Grid3DEnvironment::SVGLabelState(const xyzLoc &l, const char *str, double scale) const
{
	std::string s;
	recColor c;// = {0.5, 0.5, 0};
	GLfloat t;
	GetColor(c.r, c.g, c.b, t);
	s += SVGDrawText(l.x+0.5+1, l.y+0.5+1+1, str, c, scale);
	return s;
//	std::string s;
//	s =  "<text x=\"0\" y=\"15\" fill=\"black\">";
//	s += str;
//	s += "</text>";
//	return s;
}

std::string Grid3DEnvironment::SVGDrawLine(const xyzLoc &p1, const xyzLoc &p2, int width) const
{
	//<line x1="0" y1="0" x2="200" y2="200" style="stroke:rgb(255,255,255);stroke-width:1" />
	//std::string s;
	recColor c;// = {0.5, 0.5, 0};
	GLfloat t;
	GetColor(c.r, c.g, c.b, t);
	return ::SVGDrawLine(p1.x+1, p1.y+1, p2.x+1, p2.y+1, width, c);

//	s = "<line x1 = \"" + std::to_string(p1.x) + "\" ";
//	s +=      "y1 = \"" + std::to_string(p1.y) + "\" ";
//	s +=      "x2 = \"" + std::to_string(p2.x) + "\" ";
//	s +=      "y2 = \"" + std::to_string(p2.y) + "\" ";
//	s += "style=\"stroke:"+SVGGetRGB(c)+";stroke-width:"+std::to_string(width)+"\" />";
//	return s;
}


void Grid3DEnvironment::Draw() const
{
	recColor black = {0.0, 0.0, 0.0};
	
	// draw tiles
	for (int y = 0; y < map->GetMapHeight(); y++)
	{
		for (int x = 0; x < map->GetMapWidth(); x++)
		{
			bool draw = true;
			rect r;
			GLdouble px, py, t, rad;
			map->GetOpenGLCoord(x, y, px, py, t, rad);
			r.left = px-rad;
			r.top = py-rad;
			r.right = px+rad;
			r.bottom = py+rad;
			
			if (map->GetTerrainType(x, y) == kGround)
			{
				recColor c = {0.9, 0.9, 0.9};
				FillRect(r, c);
			}
			else if (map->GetTerrainType(x, y) == kTrees)
			{
				recColor c = {0.0, 0.5, 0.0};
				FillRect(r, c);
			}
			else if (map->GetTerrainType(x, y) == kWater)
			{
				recColor c = {0.0, 0.0, 1.0};
				FillRect(r, c);
			}
			else if (map->GetTerrainType(x, y) == kSwamp)
			{
				recColor c = {0.0, 0.3, 1.0};
				FillRect(r, c);
			}
			else {
				draw = false;
			}
		}
	}
	
	// draw cell boundaries for open terrain
	if (0)
		for (int y = 0; y < map->GetMapHeight(); y++)
		{
			for (int x = 0; x < map->GetMapWidth(); x++)
			{
				// mark cells on map
				if ((map->GetTerrainType(x, y)>>terrainBits) == (kGround>>terrainBits))
				{
					recColor c = {0.75, 0.75, 0.75};
					rect r;
					GLdouble px, py, t, rad;
					map->GetOpenGLCoord(x, y, px, py, t, rad);
					r.left = px-rad;
					r.top = py-rad;
					r.right = px+rad;
					r.bottom = py+rad;
					FrameRect(r, c);
				}
			}
		}
	
	// draw lines between different terrain types
//	if (0)
	for (int y = 0; y < map->GetMapHeight(); y++)
	{
		for (int x = 0; x < map->GetMapWidth(); x++)
		{
			GLdouble px, py, t, rad;
			map->GetOpenGLCoord(x, y, px, py, t, rad);
			
			bool draw = true;
			if ((map->GetTerrainType(x, y) == kGround) ||
				(map->GetTerrainType(x, y) == kTrees) ||
				(map->GetTerrainType(x, y) == kWater))
			{
				if (x == map->GetMapWidth()-1)
				{
					point2d s = {px+rad, py-rad};
					point2d g = {px+rad, py+rad};
					::DrawLine(s, g, 1, black);
				}
				if (y == map->GetMapHeight()-1)
				{
					point2d s = {px-rad, py+rad};
					point2d g = {px+rad, py+rad};
					::DrawLine(s, g, 1, black);
				}
			}
			else if (map->GetTerrainType(x, y) == kSwamp)
			{
			}
			else {
				draw = false;
			}
			
			if (draw)
			{
				// Code does error checking, so this works with x == 0
				if (map->GetTerrainType(x, y) != map->GetTerrainType(x-1, y))
				{
					point2d s = {px-rad, py-rad};
					point2d g = {px-rad, py+rad};
					::DrawLine(s, g, 1, black);
				}
				
				if (map->GetTerrainType(x, y) != map->GetTerrainType(x, y-1))
				{
					point2d s = {px-rad, py-rad};
					point2d g = {px+rad, py-rad};
					::DrawLine(s, g, 1, black);
				}
				
				if (map->GetTerrainType(x, y) != map->GetTerrainType(x+1, y))
				{
					point2d s = {px+rad, py-rad};
					point2d g = {px+rad, py+rad};
					::DrawLine(s, g, 1, black);
				}
				
				if (map->GetTerrainType(x, y) != map->GetTerrainType(x, y+1))
				{
					point2d s = {px-rad, py+rad};
					point2d g = {px+rad, py+rad};
					::DrawLine(s, g, 1, black);
				}
			}
			
		}
	}
}

void Grid3DEnvironment::Draw(const xyzLoc &l) const
{
	GLdouble px, py, t, rad;
	map->GetOpenGLCoord(l.x, l.y, px, py, t, rad);

	//if (map->GetTerrainType(l.x, l.y) == kGround)
	{
		recColor c;// = {0.5, 0.5, 0};
		GLfloat t;
		GetColor(c.r, c.g, c.b, t);

		rect r;
		r.left = px-rad;
		r.top = py-rad;
		r.right = px+rad;
		r.bottom = py+rad;

		//s += SVGDrawCircle(l.x+0.5+1, l.y+0.5+1, 0.5, c);
		::FillCircle(r, c);
		//stroke-width="1" stroke="pink" />
	}
}

void Grid3DEnvironment::DrawLine(const xyzLoc &a, const xyzLoc &b, double width) const
{
	GLdouble xx1, yy1, zz1, rad;
	GLdouble xx2, yy2, zz2;
	map->GetOpenGLCoord(a.x, a.y, xx1, yy1, zz1, rad);
	map->GetOpenGLCoord(b.x, b.y, xx2, yy2, zz2, rad);

	recColor c;// = {0.5, 0.5, 0};
	GLfloat t;
	GetColor(c.r, c.g, c.b, t);
	
	::DrawLine({xx1, yy1}, {xx2, yy2}, width, c);
}


//void Grid3DEnvironment::OpenGLDraw(const xyzLoc& initial, const tDirection &dir, GLfloat r, GLfloat g, GLfloat b) const
//{
//	xyzLoc s = initial;
//	GLdouble xx, yy, zz, rad;
//	map->GetOpenGLCoord(s.x, s.y, xx, yy, zz, rad);
//	
//	glColor3f(r,g,b);
//	glBegin(GL_LINE_STRIP);
//	glVertex3f(xx, yy, zz-rad/2);
//	
//	
//	switch (dir)
//	{
//		case kN: s.y-=1; break;
//		case kS: s.y+=1; break;
//		case kE: s.x+=1; break;
//		case kW: s.x-=1; break;
//		case kNW: s.y-=1; s.x-=1; break;
//		case kSW: s.y+=1; s.x-=1; break;
//		case kNE: s.y-=1; s.x+=1; break;
//		case kSE: s.y+=1; s.x+=1; break;
//		default: break;
//	}
//
//	
//	map->GetOpenGLCoord(s.x, s.y, xx, yy, zz, rad);
//	glVertex3f(xx, yy, zz-rad/2);
//	glEnd();
//}

void Grid3DEnvironment::GetNextState(const xyzLoc &currents, tDirection dir, xyzLoc &news) const
 {
	news = currents;
 	switch (dir)
	{
 		case kN: news.y-=1; break;
 		case kS: news.y+=1; break;
 		case kE: news.x+=1; break;
 		case kW: news.x-=1; break;
 		case kNW: news.y-=1; news.x-=1; break;
 		case kSW: news.y+=1; news.x-=1; break;
 		case kNE: news.y-=1; news.x+=1; break;
 		case kSE: news.y+=1; news.x+=1; break;

		case kNN: news.y-=2; break;
		case kNNE: news.y-=2; news.x+=1; break;
		case kNEE: news.y-=1; news.x+=2; break;
		case kNNEE: news.y-=2; news.x+=2; break;
		case kEE: news.x+=2; break;
		case kSSE: news.y+=2; news.x+=1; break;
		case kSEE: news.y+=1; news.x+=2; break;
		case kSSEE: news.y+=2; news.x+=2; break;
		case kSS: news.y+=2; break;
		case kSSW: news.y+=2; news.x-=1; break;
		case kSWW: news.y+=1; news.x-=2; break;
		case kSSWW: news.y+=2; news.x-=2; break;
		case kWW: news.x-=2; break;
		case kNNW: news.y-=2; news.x-=1; break;
		case kNWW: news.y-=1; news.x-=2; break;
		case kNNWW: news.y-=2; news.x-=2; break;

		case kNNN: news.y-=3; break;
		case kNNNE: news.y-=3; news.x+=1; break;
		case kNEEE: news.y-=1; news.x+=3; break;
		case kNNNEE: news.y-=3; news.x+=2; break;
		case kNNEEE: news.y-=2; news.x+=3; break;
		case kNNNEEE: news.y-=3; news.x+=3; break;
		case kEEE: news.x+=3; break;
		case kSSSE: news.y+=3; news.x+=1; break;
		case kSEEE: news.y+=1; news.x+=3; break;
		case kSSEEE: news.y+=2; news.x+=3; break;
		case kSSSEE: news.y+=3; news.x+=2; break;
		case kSSSEEE: news.y+=3; news.x+=3; break;
		case kSSS: news.y+=3; break;
		case kSSSW: news.y+=3; news.x-=1; break;
		case kSWWW: news.y+=1; news.x-=3; break;
		case kSSWWW: news.y+=2; news.x-=3; break;
		case kSSSWW: news.y+=3; news.x-=2; break;
		case kSSSWWW: news.y+=3; news.x-=3; break;
		case kWWW: news.x-=3; break;
		case kNNNW: news.y-=3; news.x-=1; break;
		case kNWWW: news.y-=1; news.x-=3; break;
		case kNNNWW: news.y-=3; news.x-=2; break;
		case kNNWWW: news.y-=2; news.x-=3; break;
		case kNNNWWW: news.y-=3; news.x-=3; break;
 		default: break;
	}	
}

double Grid3DEnvironment::GetPathLength(std::vector<xyzLoc> &neighbors)
{
	double length = 0;
	for (unsigned int x = 1; x < neighbors.size(); x++)
	{
		length += HCost(neighbors[x-1], neighbors[x]);
	}
	return length;
}

/************************************************************/

AbsGrid3DEnvironment::AbsGrid3DEnvironment(MapAbstraction *_ma)
:Grid3DEnvironment(_ma->GetMap())
{
	ma = _ma;
	
}

AbsGrid3DEnvironment::~AbsGrid3DEnvironment()
{
	map = 0;
	//delete ma;
}

/************************************************************/

/** Constructor for the BaseMapOccupancyInterface
* 
* @author Renee Jansen
* @date 08/22/2007
*
* @param m The map to which the occupancy interface applies
*/
BaseMapOccupancyInterface::BaseMapOccupancyInterface(Map* m)
{
 	mapWidth = m->GetMapWidth();
 	mapHeight = m->GetMapHeight();
	bitvec.resize(mapWidth*mapHeight);// = new BitVector(mapWidth * mapHeight);
	
	//initialize the bitvector
//	for (int i=0; i<m->GetMapWidth(); i++)
//		for (int j=0; j<m->GetMapHeight(); j++)
//			bitvec->Set(CalculateIndex(i,j), false);
}



/** Destructor for the BaseMapOccupancyInterface
* 
* @author Renee Jansen
* @date 08/22/2007
*/
BaseMapOccupancyInterface::~BaseMapOccupancyInterface()
{
//	delete bitvec;
//	bitvec = 0;
}

/** Sets the occupancy of a state.
* 
* @author Renee Jansen
* @date 08/22/2007
*
* @param s The state for which we want to set the occupancy
* @param occupied Whether or not the state is occupied
*/
void BaseMapOccupancyInterface::SetStateOccupied(const xyzLoc &s, bool occupied)
{
	// Make sure the location is valid
	// unsigned, so must be greater than 0
	assert(/*(s.x>=0) &&*/ (s.x<mapWidth)/* && (s.y>=0)*/ && (s.y<mapHeight));
//	bitvec->Set(CalculateIndex(s.x,s.y), occupied);
	bitvec[CalculateIndex(s.x,s.y)] = occupied;
}

/** Returns the occupancy of a state.
* 
* @author Renee Jansen
* @date 08/22/2007
*
* @param s The state for which we want to know the occupancy information
* @return True if the state is occupied, false otherwise. 
*/
bool BaseMapOccupancyInterface::GetStateOccupied(const xyzLoc &s)
{
	// unsigned, so must be greater than 0
	assert(/*s.x>=0 &&*/ s.x<=mapWidth && /*s.y>=0 && */s.y<=mapHeight);
	//return bitvec->Get(CalculateIndex(s.x,s.y));
	return bitvec[CalculateIndex(s.x,s.y)];
}

/** Gets the index into the bitvector. 
*
* Converts (x,y) locations to a position in the bitvector. 
*
* @author Renee Jansen
* @date 08/22/2007
*
* @param x The x-coordinate of the location
* @param y The y-coordinate of the location
* @return The index into the bit vector
*/
//template <class state, class action>
long BaseMapOccupancyInterface::CalculateIndex(uint16_t x, uint16_t y)
{
	return (y * mapWidth) + x;
}

/** Updates the occupancy interface when a unit moves
*
* Sets the old location to be not occupied, and the new location
* to be occupied.
* 
* @author Renee Jansen
* @date 09/17/2007
*
* @param oldState The unit's previous state
* @param newState The unit's new state
*/
void BaseMapOccupancyInterface::MoveUnitOccupancy(const xyzLoc &oldState, const xyzLoc &newState)
{
	SetStateOccupied(oldState, false);
	SetStateOccupied(newState, true);
}

bool BaseMapOccupancyInterface::CanMove(const xyzLoc &, const xyzLoc &l2)
{
	if (!(GetStateOccupied(l2)))
	{
		return true;
	}
	else
	{		
		return false;
	}
	
}
