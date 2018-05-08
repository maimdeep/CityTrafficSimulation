#include "Street.h"
#include<iostream>
using namespace std;

class Simulator;

float Road::freeSpace(bool dir)
{
    //if ()

    if (dir)
    {
        //cout<<"freespace1"<<endl;
        if (vehiclesBeg.size() == 0) return length - reservedSpaceBeg;

        //cout<<"freespace2"<<endl;
        //cout<<"freespace2 :"<<vehiclesBeg.back()->idnumber<<"  "<<vehiclesBeg.back()->xPos<<"  "<<vehiclesBeg.back()->pos<<endl;

        return vehiclesBeg.back()->xPos - reservedSpaceBeg;
    }

    //cout<<"freespace3"<<endl;

    if (vehiclesEnd.size() == 0) return length - reservedSpaceEnd;

    //cout<<"freespace4"<<endl;
    //cout<<"freespace4: "<<vehiclesEnd.back()->id<<"  "<<vehiclesEnd.back()->xPos<< "  "<<vehiclesEnd.back()->pos<<endl;

    //cout<<"debug: "<<id<<"  "<<vehiclesEnd.back()->id<<"  "<<vehiclesEnd.back()->xPos <<endl;

    return vehiclesEnd.back()->xPos - reservedSpaceEnd;
}

Vec3 Road::getBegJoint(bool dir)
{
    if (dir)
    {
        return begJoint + normal * 0.1;
    }

    return begJoint - normal * 0.1;
}

Vec3 Road::getEndJoint(bool dir)
{
    if (dir)
    {
        return endJoint + normal * 0.1;
    }

    return endJoint - normal * 0.1;
}

Cross::Cross(Vec3 position)// : GameObject(engine)
{
    pos = position;
    allowedVeh = 0;

    isSet = false;

    //cout<<(streets[0]==NULL)<<endl<<(streets[1]==NULL)<<endl;
    //cout<<streets[0].street<<endl;

}

void Cross::update(float delta)
{
    if(!isSet && /*id.compare("SK2") == 0*/ streets.size() == 4)
    {
        //isSet = true;
        setDefaultPriority(streets[0].street, streets[1].street, streets[2].street, streets[3].street);
        //cout<<id<<"   did happen?"<<endl;
    }

    if (!isSet && streets.size() == 2)
    {
        //isSet = true;
        setDefaultPriority();
        //cout<<"should happen"<<endl;
    }

    if (!isSet && streets.size() == 3)
    {
        //isSet = true;
        setDefaultPriority(streets[0].street, streets[1].street, streets[2].street);
    }

    if (!isSet)
    {
        //cout<<id<<"   "<<streets.size()<<endl;
    }

    /*cout<<"allowed vehs "<<id<<": " << allowedVeh<<endl;
    if (allowedVeh == 0)
    {
        int priority = -1;
        for(int i=0;i<streets.size();i++)
        {
            if (streets[i].vehicles.size() == 0)
            {
                int j = (i+1) % streets.size();
                while (streets[j].vehicles.size() == 0)
                {
                    if (i == j)
                    {
                        priority = -1;
                        break;
                    }
                    j = (j+1)%streets.size();
                }
                if (i == j) break;
                priority = (j) % streets.size();
                break;
            }
        }

        if (priority > 0)
        {
            //cout<<streets[priority].vehicles.size()<<endl;
            streets[priority].vehicles[0]->allowedToCross = true;
            streets[priority].vehicles.erase(streets[priority].vehicles.begin());
            allowedVeh=1;

            for(int i=0;i<streets[priority].vehicles.size();i++)
            {
                if (streets[priority].vehicles[i]->desiredTurn == streets[priority].vehicles[0]->desiredTurn)
                {
                    streets[priority].vehicles[i]->allowedToCross = true;
                    allowedVeh++;
                    //cout<<streets[priority].vehicles[i]->id<<"################################"<<streets[priority].vehicles[0]->id<<endl;
                    streets[priority].vehicles.erase(streets[priority].vehicles.begin()+i);
                    i--;
                }
                else break;
            }
        }
    }*/

    bool didAllow = false;
    int smallestDstIndex = -1;
    float smallestDstValue = -1;

    //if(id.compare("SK4")==0) cout << id<< "    "<<allowedVeh<<endl;

    if ((allowedVeh == 0 || streets.size() <= 2) && isSet)
    {
        vector<int> indexesToPass;

        for (int i=0;i<streets.size();i++)
        {
            if (streets[i].vehicles.size() > 0)
            {
                //if (streets[i].vehicles[0]->dstToCross > 0.7) continue;
                //if (streets[i].vehicles[0]->curCross == NULL) continue;
                //cout<<streets[i].vehicles[0]->id << "    "<<streets[i].vehicles[0]->allowedToCross<<endl;
                if (streets[i].vehicles[0]->dstToCross < smallestDstValue || smallestDstIndex < 0)
                {
                    smallestDstIndex = i;
                    smallestDstValue = streets[i].vehicles[0]->dstToCross;
                }

                int which = streets[i].vehicles[0]->desiredTurn;
                //int which2 = streets[i].yield[which];
                bool isOK = true;

                //if (id.compare("SK4")==0) cout<<"test "<<allowedVeh<<"  "<<id<<"   "<<streets[i].vehicles[0]->id<<": "<<which<< "    " << streets[i].yield.size()<<endl;

                //if (which > 0)
                for (int j=0;j<streets[i].yield[which].size();j++)
                {
                    //cout<<"YIELD:  "<<id<<"   "<<streets[i].vehicles[0]->id<<"   "<<j<<"    "<<streets[streets[i].yield[which][j]].vehicles.size()<<endl;

                    if (streets[streets[i].yield[which][j]].vehicles.size() > 0)
                    {
                        //cout<<id<<"  "<<streets[i].vehicles[0]->id<<"  STOP  "<<streets[streets[i].yield[which][j]].vehicles[0]->id<<endl;//int i;cin>>i;
                        isOK = false;
                        break;
                    }
                }
                //cout<<endl;

                if (isOK)
                {
                    //cout<<"ALLOWED:  "<<streets[i].vehicles[0]->id<<endl;


                    didAllow = true;

                    if (streets[i].vehicles[0]->dstToCross > 0.7) continue;

                    //if (streets[i].vehicles[0]->isEnoughSpace())

                    indexesToPass.push_back(i);


                    /*streets[i].vehicles[0]->allowedToCross = true;
                    streets[i].vehicles.erase(streets[i].vehicles.begin());
                    allowedVeh++;

                    for (int j=1;j<streets[i].vehicles.size();j++)
                    {
                        if (streets[i].vehicles[j]->desiredTurn == streets[i].vehicles[0]->desiredTurn)
                        {
                            streets[i].vehicles[j]->allowedToCross = true;
                            streets[i].vehicles.erase(streets[i].vehicles.begin());
                            allowedVeh++;
                        }
                        else break;
                    }*/
                }
            }
        }

        for (int i=0;i<indexesToPass.size();i++)
        {
            if (streets[indexesToPass[i]].vehicles[0]->isEnoughSpace())
            {
                streets[indexesToPass[i]].vehicles[0]->allowedToCross = true;
                streets[indexesToPass[i]].vehicles.erase(streets[indexesToPass[i]].vehicles.begin());
                allowedVeh++;
            }

            /*for (int j=1;j<streets[indexesToPass[i]].vehicles.size();j++)
            {
                if (streets[indexesToPass[i]].vehicles[j]->desiredTurn == streets[indexesToPass[i]].vehicles[0]->desiredTurn && streets[indexesToPass[i]].vehicles[j]->desiredTurn < 1)
                {
                    streets[indexesToPass[i]].vehicles[j]->allowedToCross = true;
                    streets[indexesToPass[i]].vehicles.erase(streets[indexesToPass[i]].vehicles.begin());
                    allowedVeh++;
                }
                else break;
            }*/
        }

        if (allowedVeh == 0)
        {
            /*if (smallestDstIndex >= 0 && streets[smallestDstIndex].vehicles[0]->isEnoughSpace())
            {
                streets[smallestDstIndex].vehicles[0]->allowedToCross = true;
                streets[smallestDstIndex].vehicles.erase(streets[smallestDstIndex].vehicles.begin());
                allowedVeh++;
                didAllow = true;
            }*/

            if (allowedVeh == 0)
            {
                for(int i=0;i<streets.size();i++)
                {
                    if (streets[i].vehicles.size() > 0)
                    {
                        if (streets[i].vehicles[0]->isEnoughSpace())
                        {
                            streets[i].vehicles[0]->allowedToCross = true;
                            streets[i].vehicles.erase(streets[i].vehicles.begin());
                            allowedVeh++;
                            didAllow = true;
                            break;
                        }
                    }
                }
            }
        }

        /*if (allowedVeh == 0 && smallestDstIndex >= 0)
        {
            streets[smallestDstIndex].vehicles[0]->allowedToCross = true;
            streets[smallestDstIndex].vehicles.erase(streets[smallestDstIndex].vehicles.begin());
            allowedVeh++;

            /*for (int j=1;j<streets[smallestDstIndex].vehicles.size();j++)
            {
                if (streets[smallestDstIndex].vehicles[j]->desiredTurn == streets[smallestDstIndex].vehicles[0]->desiredTurn)
                {
                    streets[smallestDstIndex].vehicles[j]->allowedToCross = true;
                    streets[smallestDstIndex].vehicles.erase(streets[smallestDstIndex].vehicles.begin());
                    allowedVeh++;
                }
                else break;
            }*/
        //}
    }
}

void Cross::setDefaultPriority(Road *s0, Road *s1, Road *s2, Road *s3)
{
    isSet = true;

    for(int i=0;i<streets.size();i++)
    {
        streets[i].yield.clear();
    }

    cout<<id<<"   number of streets: "<<streets.size()<<endl;

    if (streets.size() == 2)
    {
        vector<int> yield;
        vector<vector<int> > finalVec;
        finalVec.push_back(yield);
        finalVec.push_back(yield);

        streets[0].yield = finalVec;
        streets[1].yield = finalVec;

        cout<<id<<"   "<<streets[0].yield.size();
    }

    if (streets.size() == 3)
    {
        vector<int> yield0;
        yield0.push_back(1);
        vector<vector<int> > finalVec0;
        finalVec0.push_back(yield0);
        finalVec0.push_back(yield0);
        finalVec0.push_back(yield0);

        vector<int> yield1;
        vector<int> yield1empty;
        yield1.push_back(2);
        vector<vector<int> > finalVec1;
        finalVec1.push_back(yield1);
        finalVec1.push_back(yield1);
        finalVec1.push_back(yield1empty);

        vector<int> yield2;
        yield2.push_back(0);
        vector<vector<int> >finalVec2;
        finalVec2.push_back(yield2);
        finalVec2.push_back(yield2);
        yield2.push_back(1);
        finalVec2.push_back(yield2);

        for(int i=0;i<3;i++)
        {
            if (streets[i].street == s0) streets[i].yield = finalVec0;
            if (streets[i].street == s1) streets[i].yield = finalVec1;
            if (streets[i].street == s2) streets[i].yield = finalVec2;
        }
    }

    if (streets.size() == 4)
    {
        vector<int> yield[4];

        yield[0].push_back(1);
        yield[0].push_back(2);

        yield[1].push_back(1);
        yield[2].push_back(1);

        yield[3].push_back(1);
        yield[3].push_back(2);

        vector<vector<int> > finalVec;
        //finalVec.push_back(yield[0]);
        //finalVec.push_back(yield[1]);
        //finalVec.push_back(yield[2]);
        //finalVec.push_back(yield[3]);

        for (int i=0;i<4;i++)
        {
            if (streets[i].street == s0) finalVec.push_back(yield[0]);
            if (streets[i].street == s1) finalVec.push_back(yield[1]);
            if (streets[i].street == s2) finalVec.push_back(yield[2]);
            if (streets[i].street == s3) finalVec.push_back(yield[3]);
        }

        for (int i=0;i<4;i++)
        {
            //streets[i].yield.push_back()

            streets[i].yield = finalVec;

            for(int j=0;j<4;j++)
            {
                for(int k=0;k<finalVec[j].size();k++)
                {
                    //cout<<"DEbug: "<<finalVec[j][k]<<endl;
                    finalVec[j][k] = (finalVec[j][k]+1) % 4;
                }
            }
        }

        //debug
        /*for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                cout<<i<<" -> "<<j<<": ";
                for(int k=0;k<streets[i].yield[j].size();k++)
                {
                    cout<<streets[i].yield[j][k]<<" ";

                }
                cout<<endl;
            }
        }*/
    }


}

void glP(Vec3 x)
{
    glVertex3f(x.x,x.y,x.z);
}

void Cross::draw()
{
    setColor(0.1,0.1,0.1);
    drawCube(0.6, 0.1, 0.6);

}

Street::Street(Cross *begCross, Cross *endCross)
{
    crossBeg = begCross;
    crossEnd = endCross;

    length = Vec3::dst(crossBeg->getPos(), crossEnd->getPos());
    direction = crossEnd->getPos() - crossBeg->getPos();
    direction.normalize();

    Cross::OneStreet temp;
    temp.street = this;
    temp.enabled = true;
    temp.direction = true;
    //temp.jointPos = crossBeg->getPos() + direction * 0.3;
    begJoint = crossBeg->getPos() + direction * 0.3;;

    crossBeg->streets.push_back(temp);

    temp.direction = false;
    //temp.jointPos = crossEnd->getPos() - direction * 0.3;
    endJoint = crossEnd->getPos() - direction * 0.3;
    crossEnd->streets.push_back(temp);

    begPos = crossBeg->getPos();
    endPos = crossEnd->getPos();

    reservedSpaceBeg = 0;
    reservedSpaceEnd = 0;

    normal = Vec3::cross(Vec3(0,1,0), direction);
    normal.normalize();
}

void Street::draw()
{
    setColor(0.3,0.3,0.3);
    drawLine(crossBeg->getPos(), crossEnd->getPos());

    Vec3 szer = Vec3::cross(Vec3(0,1,0), direction);
    szer.normalize();
    szer *= 0.3;

    Vec3 a = endPos + szer;
    Vec3 b = endPos - szer;
    Vec3 c = begPos + szer;
    Vec3 d = begPos - szer;

    //cout<<direction<<endl;

    glBegin(GL_POLYGON);
    glP(a);
    glP(b);
    glP(d);
    glP(c);
    //glP(a);
    glEnd();
}

Garage::Garage(Simulator *engine, Vec3 p, Cross *c)
{
    pos = p;
    crossEnd = c;
    crossBeg = NULL;

    begPos = pos;
    endPos = crossEnd->getPos();

    direction = endPos - begPos;
    direction.normalize();

    length = Vec3::dst(begPos, endPos);

    Cross::OneStreet temp;
    temp.street = this;
    temp.enabled = false;
    temp.direction = false;
    //temp.jointPos = crossEnd->getPos() - direction * 0.3;

    begJoint = begPos;
    endJoint = crossEnd->getPos() - direction * 0.3;

    //cout<<"garage joint: "<<temp.jointPos<<endl;

    gameEngine = engine;

    crossEnd->streets.push_back(temp);

    reservedSpaceBeg = 0;
    reservedSpaceEnd = 0;

    normal = Vec3::cross(Vec3(0,1,0), direction);
    normal.normalize();

    curTimeSpot = 0;
    frecSpot = 5;

    curTimeDelete = 0;
    frecDelete = 5;
}

void Garage::draw()
{
    setColor(0,0,1);
    drawCube(0.3);
}

void Garage::update(float delta)
{
    //cout<<id<<"   "<<curTime<<" "<<delta<<endl;

    //if (vehiclesBeg.size() > 0)cout<< vehiclesBeg.back()->xPos <<endl;
    //8cout<<"veh size: "<<vehiclesBeg.size()<<endl;
    if (vehiclesBeg.size() == 0 || (vehiclesBeg.size() > 0 && vehiclesBeg.back()->xPos > 1))
        curTimeSpot += delta;

    if (curTimeSpot > frecSpot)
    {
        //cout<<"%%%%%%%"<<endl;
        curTimeSpot = 0;
        spotCar();
    }

    if (vehiclesEnd.size() > 0)
        curTimeDelete += delta;

    if (curTimeDelete > frecDelete)
    {
        curTimeDelete = 0;
        deleteCar();
    }
}

void registerNewObject(Simulator *engine, GameObject *go);
void destroyNextObject(Simulator *engine, GameObject *go);

string Garage::itos(int x)
{
    ostringstream ss;
    ss << x;
    return ss.str();
}

//int number = 0;
void Garage::spotCar()
{
    //number ++;
    //if (number > 70) return;
    Vehicle *temp;
    temp = new Car(this);
    temp->curRoad = this;
    temp->direction = true;
    temp->id = "CAR_" + itos(Vehicle::numVeh) + id;
    temp->isFirstVeh = vehiclesBeg.size() == 0;
    temp->idnumber = Vehicle::numVeh;
    vehiclesBeg.push(temp);
//    gameEngine->registerObject(temp);
    registerNewObject(gameEngine, temp);

    cout<<"spotted " << temp->id << " by "<<id<<endl;
}

void Garage::deleteCar()
{
    if (vehiclesEnd.size() > 0)
    {
        Vehicle *temp = vehiclesEnd.front();
        vehiclesEnd.pop();

        if(temp->backVeh != NULL)
        {
            temp->backVeh->isFirstVeh = true;
            temp->backVeh->frontVeh = NULL;
        }

        destroyNextObject(gameEngine, temp);
    }
}
