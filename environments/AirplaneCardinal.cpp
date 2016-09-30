//
//  AirplaneCardinal.cpp
//  hog2 glut
//
//  Created by Thayne Walker on 5/4/16.
//  Copyright © 2016 University of Denver. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include "AirplaneCardinal.h"

AirplaneCardinalEnvironment::AirplaneCardinalEnvironment(
  unsigned width,
  unsigned length,
  unsigned height,
  double climbRate,
  double minSpeed,
  double maxSpeed,
  uint8_t numSpeeds,
  double cruiseBurnRate,
  double speedBurnDelta,
  double climbCost,
  double descendCost,
  double gridSize,
  std::string const& perimeterFile
): AirplaneEnvironment(
  width,
  length,
  height,
  climbRate,
  minSpeed,
  maxSpeed,
  numSpeeds,
  cruiseBurnRate,
  speedBurnDelta,
  climbCost,
  descendCost,
  gridSize,
  perimeterFile){
}

void AirplaneCardinalEnvironment::GetActions(const airplaneState &nodeID, std::vector<airplaneAction> &actions) const
{
  if(abs(nodeID.x-getGoal().x) <=2 && abs(nodeID.y-getGoal().y) <=2 &&abs(nodeID.height-getGoal().height) <=2)
  {
    AirplaneEnvironment::GetActions(nodeID,actions);
    return;
  }

  actions.resize(0);

  // Force a turn into a cardinal direction
  if(nodeID.heading%2)
  {
    actions.push_back(airplaneAction(-k45,0,0));
    actions.push_back(airplaneAction(k45,0,0));

    if(nodeID.height > 1)
    {
      actions.push_back(airplaneAction(-k45,0,-1));
      actions.push_back(airplaneAction(k45,0,-1));

      if(nodeID.speed > minSpeed)
      {
        actions.push_back(airplaneAction(k45,-1,-1));
        actions.push_back(airplaneAction(-k45,-1,-1));
      }
      if(nodeID.speed < numSpeeds+minSpeed)
      {
        actions.push_back(airplaneAction(-k45,1,-1));
        actions.push_back(airplaneAction(k45,1,-1));
      }
    }
    if(nodeID.height < height)
    {
      actions.push_back(airplaneAction(-k45,0,1));
      actions.push_back(airplaneAction(k45,0,1));

      if(nodeID.speed > minSpeed)
      {
        actions.push_back(airplaneAction(-k45,-1,1));
        actions.push_back(airplaneAction(k45,-1,1));
      }
      if(nodeID.speed < numSpeeds+minSpeed)
      {
        actions.push_back(airplaneAction(-k45,1,1));
        actions.push_back(airplaneAction(k45,1,1));
      }
    }
    if(nodeID.speed > minSpeed)
    {
      actions.push_back(airplaneAction(-k45,-1,0));
      actions.push_back(airplaneAction(k45,-1,0));
    }

    if(nodeID.speed < numSpeeds+minSpeed)
    {
      actions.push_back(airplaneAction(-k45,1,0));
      actions.push_back(airplaneAction(k45,1,0));
    }
    return;
  }
  // 45, 90, 0, shift
  // speed:
  // faster, slower
  // height:
  // up / down

  // no change
  actions.push_back(airplaneAction(0, 0, 0));
  actions.push_back(airplaneAction(k90, 0, 0));
  actions.push_back(airplaneAction(-k90, 0, 0));

  if(nodeID.height > 1)
  {
    // decrease height
    actions.push_back(airplaneAction(0, 0, -1));
    actions.push_back(airplaneAction(k90, 0, -1));
    actions.push_back(airplaneAction(-k90, 0, -1));

    // decrease height, decrease speed
    if(nodeID.speed > minSpeed)
    {
      actions.push_back(airplaneAction(0, -1, -1));
      actions.push_back(airplaneAction(k90, -1, -1));
      actions.push_back(airplaneAction(-k90, -1, -1));
    }
    // decrease height, increase speed
    if(nodeID.speed < numSpeeds+minSpeed)
    {
      actions.push_back(airplaneAction(0, +1, -1));
      actions.push_back(airplaneAction(k90, +1, -1));
      actions.push_back(airplaneAction(-k90, +1, -1));
    }
  }

  if(nodeID.height < height)
  {
    actions.push_back(airplaneAction(0, 0, +1));
    actions.push_back(airplaneAction(k90, 0, +1));
    actions.push_back(airplaneAction(-k90, 0, +1));

    if(nodeID.speed > minSpeed)
    {
      // increase height, decrease speed
      actions.push_back(airplaneAction(0, -1, +1));
      actions.push_back(airplaneAction(k90, -1, +1));
      actions.push_back(airplaneAction(-k90, -1, +1));
    }

    if(nodeID.speed < numSpeeds+minSpeed)
    {
      // increase height, increase speed
      actions.push_back(airplaneAction(0, +1, +1));
      actions.push_back(airplaneAction(k90, +1, +1));
      actions.push_back(airplaneAction(-k90, +1, +1));
    }
  }

  if (nodeID.speed > minSpeed)
  {
    // decrease speed
    actions.push_back(airplaneAction(0, -1, 0));
    actions.push_back(airplaneAction(k90, -1, 0));
    actions.push_back(airplaneAction(-k90, -1, 0));
  }

  if (nodeID.speed < numSpeeds+minSpeed)
  {
    // increase speed
    actions.push_back(airplaneAction(0, +1, 0));
    actions.push_back(airplaneAction(k90, +1, 0));
    actions.push_back(airplaneAction(-k90, +1, 0));
  }
}

void AirplaneCardinalEnvironment::GetReverseActions(const airplaneState &nodeID, std::vector<airplaneAction> &actions) const
{
  assert(false && "This should never be used... It's not ready.");
  actions.resize(0);
  uint8_t hmod(nodeID.height%8);

  // no change
  if(hmod==nodeID.heading) actions.push_back(airplaneAction(0, 0, 0));

  if (nodeID.height < height)
  {
    uint8_t hmodd((nodeID.height+1)%8);
    uint8_t hdg((nodeID.heading+1)%8);
    // decrease height
    //actions.push_back(airplaneAction(0, 0, -1));
    if(hmodd==hdg)actions.push_back(airplaneAction(k90, 0, -1));

    // decrease height, decrease speed
    if (nodeID.speed > minSpeed)
    {
      if(hmodd==hdg)actions.push_back(airplaneAction(k90, 1, -1));
    }

    // increase height, decrease speed
    if (nodeID.speed < numSpeeds+minSpeed)
    {
      if(hmodd==hdg)actions.push_back(airplaneAction(k90, -1, -1));
    }
  }

  if (nodeID.height > 0)
  {
    uint8_t hmodd((nodeID.height+7)%8);
    uint8_t hdg((nodeID.heading+7)%8);
    // increase height
    if(hmodd==hdg)actions.push_back(airplaneAction(k90, 0, +1));

    if (nodeID.speed > minSpeed)
    {
      // increase height, decrease speed
      if(hmodd==hdg)actions.push_back(airplaneAction(k90, 1, +1));
    }

    if (nodeID.speed < numSpeeds+minSpeed)
    {
      // increase height, increase speed
      if(hmodd==hdg)actions.push_back(airplaneAction(k90, -1, +1));
    }
  }

  if (nodeID.speed > minSpeed)
  {
    // decrease speed
    if(hmod==nodeID.heading)actions.push_back(airplaneAction(k90, 1, 0));
  }

  if (nodeID.speed < numSpeeds+minSpeed)
  {
    // increase speed
    if(hmod==nodeID.heading)actions.push_back(airplaneAction(k90, -1, 0));
  }
}

double AirplaneCardinalEnvironment::myHCost(const airplaneState &node1, const airplaneState &node2) const
{
  // Estimate fuel cost...
  static const int cruise(3);
  int diffx(abs(node1.x-node2.x));
  int diffy(abs(node1.y-node2.y));
  int hdiff(hdgDiff<8>(node1.heading,node1.headingTo(node2))/2);
  hdiff = hdiff>0? hdiff-1:0; // Subtract 1
  int diff(diffx+diffy+hdiff);
  int vertDiff(node2.height-node1.height);
  int speedDiff1(std::max(0,abs(cruise-node1.speed)-1));
  int speedDiff2(abs(cruise-node2.speed));
  int speedDiff(abs(node2.speed-node1.speed));
  double vcost(vertDiff>0?climbCost:descendCost);
  //vertDiff=std::max(hdgDiff<4>(node1.heading,node1.cardinalHeadingTo(node2))/2,(unsigned)abs(vertDiff));
  int maxMove(speedDiff1+speedDiff2+vertDiff<=diff?(speedDiff1+speedDiff2+vertDiff):speedDiff+vertDiff);

  int speedChanges(((diff-vertDiff)>=speedDiff1+speedDiff2)?(speedDiff1+speedDiff2):speedDiff);

  if(diff<vertDiff+speedChanges){ diff=(vertDiff+speedChanges); }
  
  return diff*cruiseBurnRate+std::min(speedChanges,diff)*speedBurnDelta+std::min(vertDiff,diff)*vcost;
}
