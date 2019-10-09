#include "EntityCore.h"
#include "EntityPropertyManager.h"
#include "EntityPropertyList.h"
#include "EntityMessage.h"
#include "GraphicsMessage.h"
#include "ScriptingCore\ScriptMessages.h"
#include "CoreManager.h"
#include "Util.h"
#include "EPhysics.h"
#include "UIMessage.h"
#include "GameMessage.h"
#include "AI_Message.h"

EntityCore::EntityCore()
{
	WorldInfo winfo;
	CarInfo cinfo;
	m_Physics.CreateWorld(winfo);
	m_DrivingPhysics.CreateCar(cinfo);
}

void EntityCore::Update(float dt)
{
	CopyMessage();
	EntityMgrMsg();
	PhysicsMsg();
	m_Physics.UpdateWorld(dt, 1);
	PollPhysics();
	Export();
}

void EntityCore::OnLaunch()
{
	//Register graphics export
	gCoreMgr->HandleMessage(new SMessageRegisterSceneList(0, SCENE_STATICMESH, &m_StaticMeshExport));
	//Register AI export
	gCoreMgr->HandleMessage(new SMessageRegisterScriptObject(&m_ScriptExport));
	//Register scripting export
	//Register UI export
	gCoreMgr->HandleMessage(new SMessageRegisterPicking(&m_PickingExport));

	//Register AI Steering
	gCoreMgr->HandleMessage(new SMessageRegisterSteering(&m_SteeringExport,&m_ObstacleExport));

	//Register Game export
	gCoreMgr->HandleMessage(new SMessageGameRegisterPosition(&m_PositionExport));
	gCoreMgr->HandleMessage(new SMessageRegisterAIPosition(&m_PositionExport));
}

void EntityCore::EntityMgrMsg()
{
	queue<SMessage*> tempQueue;
	while(!m_QueueBuffer.empty())
	{
		SMessage* msg = m_QueueBuffer.front();
		switch(msg->Message)
		{
		case MSG_ADDENTITY:
			{
				SMessageAddEntity* msgx = (SMessageAddEntity*)msg;
				EntityPropertyData* newEPD = gEPMgr->GetItem(msgx->Type);
				//Add entity to manager
				m_EntityMgr.AddItem(msgx->Type, msgx->ID);
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_POSITION, &(msgx->Position));
				
				//Add object to physics
				switch(newEPD->BodyType)
				{
				case BODYTYPE_SPHERE:
					{
						SphereInfo info;
						info.orgin = m_Physics.ConvertToBtVec(msgx->Position);
						info.friction = newEPD->Friction;
						info.restitution = newEPD->Restitution;
						info.mass = newEPD->Mass;
						info.angularDamping = newEPD->AngularDampen;
						info.radius = newEPD->Radius;
						int temp = m_Physics.CreateCollisionSphere(info);
						m_EntityMgr.SetValue(msgx->ID, EDTYPE_PHYSICSID, &temp);
						if(newEPD->PhysicsType == PHYSICSTYPE_KINEMATIC)
							m_Physics.SetAsKinematic(temp);
						break;
					}
				case BODYTYPE_CYLINDER:
					{
						BoxInfo info;
						info.orgin = m_Physics.ConvertToBtVec(msgx->Position);
						info.friction = newEPD->Friction;
						info.restitution = newEPD->Restitution;
						info.mass = newEPD->Mass;
						info.halfscale = m_Physics.ConvertToBtVec(newEPD->HalfScale);
						int temp = m_Physics.CreateCollisionBox(info);
						m_EntityMgr.SetValue(msgx->ID, EDTYPE_PHYSICSID, &temp);
						if(newEPD->PhysicsType == PHYSICSTYPE_KINEMATIC)
							m_Physics.SetAsKinematic(temp);
						break;
					}
				case BODYTYPE_BOX:
					{
						BoxInfo info;
						info.orgin = m_Physics.ConvertToBtVec(msgx->Position);
						info.friction = newEPD->Friction;
						info.restitution = newEPD->Restitution;
						info.mass = newEPD->Mass;
						info.halfscale = m_Physics.ConvertToBtVec(newEPD->HalfScale);
						int temp = m_Physics.CreateCollisionBox(info);
						m_EntityMgr.SetValue(msgx->ID, EDTYPE_PHYSICSID,&temp);
						if(newEPD->PhysicsType == PHYSICSTYPE_KINEMATIC)
							m_Physics.SetAsKinematic(temp);
						break;
					}
				case BODYTYPE_CAR:
					{
						CarInfo info;
						info.orgin = m_Physics.ConvertToBtVec(msgx->Position);
						info.friction = newEPD->Friction;
						info.restitution = newEPD->Restitution;
						info.mass = newEPD->Mass;
						info.halfscale = m_Physics.ConvertToBtVec(newEPD->HalfScale);
						info.acceleration = newEPD->Acceleration;
						info.maxVelocity = newEPD->MaxVelocity;
						info.steeringAngle = newEPD->SteeringAngle;
						info.brakingPower = newEPD->BrakingPower;
						int temp = m_DrivingPhysics.CreateCollisionCar(info);
						m_EntityMgr.SetValue(msgx->ID, EDTYPE_PHYSICSID,&temp);
						if(newEPD->PhysicsType == PHYSICSTYPE_KINEMATIC)
							m_Physics.SetAsKinematic(temp);
						break;
					}
				}
				delete msg;
				break;
			}
		case MSG_REMOVEENTITY:
			{
				SMessageRemoveEntity* msgx = (SMessageRemoveEntity*)msg;
				//Remove object from physics
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID,temp);
				//if(temp.PhysicsID > -1)
				//	m_Physics.DeletePhysicsObject(temp.PhysicsID);
				//Remove entity from manager
				m_EntityMgr.RemoveItem(*(msgx->ID));
				delete msg;
				break;
			}
		case MSG_SETVELOCITY:
			{
				SMessageSetVelocity* msgx = (SMessageSetVelocity*)msg;
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_VELOCITY, msgx->Velocity);
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				if(temp.PhysicsID > -1)
					m_Physics.SetLinearVelocity(temp.PhysicsID, m_Physics.ConvertToBtVec(msgx->Velocity));
				delete msg;
				break;
			}
		case MSG_SETFORCE:
			{
				SMessageSetForce* msgx = (SMessageSetForce*)msg;
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_FORCE, msgx->Force);
				delete msg;
				break;
			}
		case MSG_INCSCRIPTFLAG1:
			{
				SMessageIncScriptFlag1* msgx = (SMessageIncScriptFlag1*)msg;
				EntityData temp;
				m_EntityMgr.GetItem(&(msgx->ID), temp);
				++temp.ScriptFlag1;
				m_EntityMgr.SetValue(&(msgx->ID), EDTYPE_SCRIPTFLAG1, &temp.ScriptFlag1);
				delete msg;
				break;
			}
		case MSG_SETPOSITION:
			{
				SMessageSetPosition* msgx = (SMessageSetPosition*)msg;
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_POSITION, msgx->Position);
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				if(temp.PhysicsID > -1)
					m_Physics.SetShapePosition(temp.PhysicsID, m_Physics.ConvertToBtVec(msgx->Position));
				delete msg;
				break;
			}
		case MSG_SETROTATION:
			{
				SMessageSetRotation* msgx = (SMessageSetRotation*)msg;
			//	m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, msgx->);
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				if(temp.PhysicsID > -1)
					m_Physics.SetShapeRotation(temp.PhysicsID, m_Physics.ConvertToBtVec(msgx->Axis),msgx->Degree);
				D3DXQUATERNION Rot=m_Physics.ConvertToDxRot(m_Physics.GetShapeRotation(temp.PhysicsID));
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, Rot);
				delete msg;
				break;
			}
		case MSG_ROTATEONX:
			{
				SMessageAddRotationOnX* msgx = (SMessageAddRotationOnX*)msg;
			//	m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, msgx->);
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				if(temp.PhysicsID > -1)
					m_Physics.RotateShapeOnX(temp.PhysicsID, msgx->Degree);
				D3DXQUATERNION Rot=m_Physics.ConvertToDxRot(m_Physics.GetShapeRotation(temp.PhysicsID));
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, Rot);
				delete msg;
				break;
			}
			case MSG_ROTATEONY:
			{
				SMessageAddRotationOnY* msgx = (SMessageAddRotationOnY*)msg;
			//	m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, msgx->);
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				if(temp.PhysicsID > -1)
					m_Physics.RotateShapeOnY(temp.PhysicsID, msgx->Degree);
				D3DXQUATERNION Rot=m_Physics.ConvertToDxRot(m_Physics.GetShapeRotation(temp.PhysicsID));
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, Rot);
				delete msg;
				break;
			}
			case MSG_ROTATEONZ:
			{
				SMessageAddRotationOnZ* msgx = (SMessageAddRotationOnZ*)msg;
			//	m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, msgx->);
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				if(temp.PhysicsID > -1)
					m_Physics.RotateShapeOnZ(temp.PhysicsID, msgx->Degree);
				D3DXQUATERNION Rot=m_Physics.ConvertToDxRot(m_Physics.GetShapeRotation(temp.PhysicsID));
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_ROTATION, Rot);
				delete msg;
				break;
			}
			case MSG_SETSTEERING:
			{
				SMessageSetSteering* msgx = (SMessageSetSteering*)msg;
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_STEERINGTYPE, &(msgx->SteeringType));
				break;
			}
			case MSG_ACCELERATION:
			{
				SMessageAcceleration* msgx = (SMessageAcceleration*)msg;
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_ACCELERATION, &(msgx->Acceleration));
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				btVector3 vel = m_Physics.ConvertToBtVec(temp.Velocity);
				if(temp.PhysicsID > -1)
					m_DrivingPhysics.Acceleration(temp.PhysicsID, temp.MaxVelocity, temp.Acceleration, temp.SteeringAngle, vel);
				delete msg;
				break;
			}
			case MSG_DECELERATION:
			{
				SMessageAcceleration* msgx = (SMessageAcceleration*)msg;
				m_EntityMgr.SetValue(msgx->ID, EDTYPE_ACCELERATION, &(msgx->Acceleration));
				EntityData temp;
				m_EntityMgr.GetItem(msgx->ID, temp);
				btVector3 vel = m_Physics.ConvertToBtVec(temp.Velocity);
				if(temp.PhysicsID > -1)
					m_DrivingPhysics.DecelerateAndReverse(temp.PhysicsID, temp.MaxVelocity, temp.BrakingPower, temp.SteeringAngle, vel);
				delete msg;
				break;
			}
		default:
			{
				tempQueue.push(msg);
				break;
			}
		}
		m_QueueBuffer.pop();
	}
	m_QueueBuffer = tempQueue;
}

void EntityCore::PhysicsMsg()
{
}

void EntityCore::Export()
{
	vector<pair<int, EntityData>> vEntity;
	m_EntityMgr.GetCopy(vEntity);

	//GraphicsCore export
	vector<DD_StaticMeshData> staticMeshExport;
	//ScriptingCore export
	vector<ScriptData> scriptExport;
	//Picking export
	DDPickingData Pickingtemp;
	//Position export
	vector<DDGameData> posExport;
	//Steering export
	vector<DDSteeringData> steerExport;
	vector<SteeringObstacles> obstacleExport;

	for(int i = 0; i < vEntity.size(); ++i)
	{
		//Export to Graphics
		switch(gEPMgr->GetItem(vEntity[i].second.Type)->RenderType)
		{
		case SCENE_STATICMESH:
			{
				//Export Graphics
				DD_StaticMeshData SMDtemp;
				TranslateToStaticMesh(vEntity[i], SMDtemp);
				staticMeshExport.push_back(SMDtemp);
				//Export scripting
				ScriptData SDtemp;
				TranslateToScriptData(vEntity[i], SDtemp);
				scriptExport.push_back(SDtemp);
				//Export picking
				Pickingtemp.ID.push_back(vEntity[i].first);
				Pickingtemp.Position.push_back(vEntity[i].second.Position);
				Pickingtemp.Halfscale.push_back(gEPMgr->GetItem(vEntity[i].second.Type)->HalfScale);
				break;
			}
		}
		//Export to Physics
		if(vEntity[i].second.PhysicsID > -1)
		{
			m_Physics.ApplyCentralForce(vEntity[i].second.PhysicsID, 
				m_Physics.ConvertToBtVec(vEntity[i].second.Force));
		}
		//if(vEntity[i].second.Type == EP_SMALLMARBLE || vEntity[i].second.Type == EP_MEDIUMMARBLE ||
		//	vEntity[i].second.Type == EP_LARGEMARBLE || vEntity[i].second.Type == EP_PLAYERMARBLE)
		{
			DDGameData temp;
			temp.ID = vEntity[i].first;
			temp.Type = vEntity[i].second.Type;
			temp.Position = vEntity[i].second.Position;
			posExport.push_back(temp);
		}

		//Export to AI
		if(vEntity[i].second.SteeringType &&
			(vEntity[i].second.Type == EP_SMALLMARBLE || vEntity[i].second.Type == EP_MEDIUMMARBLE ||
			vEntity[i].second.Type == EP_LARGEMARBLE))
		{
			DDSteeringData temp;
			temp.ID = vEntity[i].first;
			temp.m_dSpeed = D3DXVec3Length(&(vEntity[i].second.Velocity));
			temp.m_iEvadeID = 0;
			temp.m_iPursuitID = 0;
			temp.SHeading = SVector3D(0, 0);
			temp.SPosition = SVector3D(vEntity[i].second.Position.x, vEntity[i].second.Position.z);
			temp.SteeringType = vEntity[i].second.SteeringType;
			temp.SVelocity = SVector3D(vEntity[i].second.Velocity.x, vEntity[i].second.Velocity.z);

			steerExport.push_back(temp);
		}
		if(vEntity[i].second.Type == EP_WALL || vEntity[i].second.Type == EP_BOX)
		{
			SteeringObstacles temp;
			temp.m_iID = vEntity[i].first;
			temp.m_dRadius = gEPMgr->GetItem(vEntity[i].second.Type)->HalfScale.x;
			temp.m_dScale = 1;
			temp.m_dTagged = false;
			temp.SPosition = SVector3D(vEntity[i].second.Position.x, vEntity[i].second.Position.z);
			obstacleExport.push_back(temp);
		}
	}
	m_PositionExport.Overwrite(posExport);
	m_StaticMeshExport.Overwrite(staticMeshExport);
	m_ScriptExport.Overwrite(scriptExport);
	m_PickingExport.Overwrite(Pickingtemp);
	m_SteeringExport.Overwrite(steerExport);
	m_ObstacleExport.Overwrite(obstacleExport);
}

void EntityCore::PollPhysics()
{
	vector<pair<int, EntityData>> vEntity;
	m_EntityMgr.GetCopy(vEntity);
	for(int i = 0; i < vEntity.size(); ++i)
	{
		if(vEntity[i].second.PhysicsID > -1)
		{
			btVector3 temp;
			D3DXVECTOR3 newtemp;
			newtemp = m_Physics.convertToDxVec(m_Physics.GetShapePosition(vEntity[i].second.PhysicsID));
			m_EntityMgr.SetValue(&vEntity[i].first, EDTYPE_POSITION, newtemp);
			newtemp = m_Physics.convertToDxVec(m_Physics.GetLinearVelocity(vEntity[i].second.PhysicsID));
			m_EntityMgr.SetValue(&vEntity[i].first, EDTYPE_VELOCITY, newtemp);
			D3DXQUATERNION temprot;
			temprot = m_Physics.ConvertToDxRot(m_Physics.GetShapeRotation(vEntity[i].second.PhysicsID));
			m_EntityMgr.SetValue(&vEntity[i].first, EDTYPE_ROTATION, temprot);
		}
	}
}

/*
This function translates the entity into static mesh
takes in and entity + id and DD_StaticMeshData, both reference
outputs onto the second parameter
*/
void EntityCore::TranslateToStaticMesh(std::pair<int,EntityData> &entity, DD_StaticMeshData &out)
{
	out.MeshID = gEPMgr->GetItem(entity.second.Type)->GResourceID;
	out.TextureID = 0;
	D3DXMATRIX m;
	D3DXMatrixTransformation(&m, &D3DXVECTOR3(0,0,0), 0, 
		&(gEPMgr->GetItem(entity.second.Type)->GraphicsScale), &D3DXVECTOR3(0,0,0), 
		&(entity.second.Rotation), &(entity.second.Position));
	out.Transform = m;
}

/*
This function translates the entity into scripting data
takes in an entity + id and ScriptData, both reference as inputs
outputs onto the second parameter
*/
void EntityCore::TranslateToScriptData(std::pair<int,EntityData> &entity, ScriptData &out)
{
	out.objID = entity.first;
	out.objType = gEPMgr->GetItem(entity.second.Type)->ScriptName;
	out.LMouseClick = entity.second.ScriptFlag1;
	out.RMouseClick = entity.second.ScriptFlag2;
	out.Collision = 0;
}
