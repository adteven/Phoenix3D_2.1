// PX2AmbientActor.cpp

#include "PX2AmbientRegionActor.hpp"
#include "PX2GraphicsRoot.hpp"
#include "PX2AVector.hpp"
#include "PX2RenderStepScene.hpp"
#include "PX2Project.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TriggerActor, AmbientRegionActor);
PX2_IMPLEMENT_STREAM(AmbientRegionActor);
PX2_IMPLEMENT_FACTORY(AmbientRegionActor);

//----------------------------------------------------------------------------
AmbientRegionActor::AmbientRegionActor() :
mHorAngle(30.0f),
mVerAngle(15.0f),
mLightCameraExtent(10.0f),
mSpecularPow(10.0f),
mIntensity(1.0f),
mLightCameraLookDistance(100.0f),
mIsNeedUpdated(false)
{
	SetName("AmbRegAtr");

	mAmbientColor = Float3(0.4f, 0.4f, 0.4f);
	mDiffuseColor = Float3(1.0f, 1.0f, 1.0f);
	mSpecularColor = Float3(1.0f, 1.0f, 1.0f);

	CreateHelpNode()->Show(false);

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
AmbientRegionActor::~AmbientRegionActor()
{
}
//----------------------------------------------------------------------------
void AmbientRegionActor::UpdateWorldData(double applicationTime, 
	double elapsedTime)
{
	TriggerActor::UpdateWorldData(applicationTime, elapsedTime);

	if (mIsNeedUpdated)
	{
		mIsNeedUpdated = false;
	}
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetLightCameraLookPostion(const APoint &pos)
{
	mLightCameraLookPosition = pos;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetLightCameraLookDistance(float dist)
{
	mLightCameraLookDistance = dist;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetLightCameraExtent(float extent)
{
	mLightCameraExtent = extent;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetParent(Movable* parent)
{
	TriggerActor::SetParent(parent);
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetHorAngle(float angle)
{
	mHorAngle = angle;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetVerAngle(float angle)
{
	mVerAngle = angle;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetAmbientColor(const Float3 &color)
{
	mAmbientColor = color;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetDiffuseColor(const Float3 &color)
{
	mDiffuseColor = color;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetSpecularColor(const Float3 &color)
{
	mSpecularColor = color;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetSpecularPow(float pow)
{
	mSpecularPow = pow;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::SetIntensity(float intensity)
{
	mIntensity = intensity;

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
void AmbientRegionActor::_UpdateDirLightCamera()
{
	AVector dir = AVector::AnglesToDirection(Mathf::DEG_TO_RAD*mHorAngle,
		Mathf::DEG_TO_RAD*mVerAngle);
	dir.Normalize();

	Light *lightDir = PX2_GR.GetLight_Dir();
	Projector *projector = PX2_GR.GetLight_Dir_Projector();

	lightDir->Ambient = Float4(mAmbientColor[0], mAmbientColor[1],
		mAmbientColor[2], mIntensity);
	lightDir->Intensity = mIntensity;
	lightDir->Diffuse = Float4(mDiffuseColor[0], mDiffuseColor[1],
		mDiffuseColor[2], 1.0f);
	lightDir->Specular = Float4(mSpecularColor[0], mSpecularColor[1],
		mSpecularColor[2], mSpecularPow);

	float upDot = dir.Dot(-AVector::UNIT_Z);
	if (upDot >= 0.99f)
	{
	}
	else
	{
		AVector upTemp = AVector::UNIT_Z;
		AVector right = dir.UnitCross(upTemp);
		AVector up = right.UnitCross(dir);

		lightDir->DVector = dir;
		lightDir->UVector = up;
		lightDir->RVector = right;

		APoint camPos = mLightCameraLookPosition - dir*mLightCameraLookDistance;
		projector->SetFrame(camPos, lightDir->DVector,
			lightDir->UVector, lightDir->RVector);
	}

	projector->SetFrustum(0.1f, Mathf::FAbs(50.0f),
		-mLightCameraExtent, mLightCameraExtent, -mLightCameraExtent,
		mLightCameraExtent);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void AmbientRegionActor::RegistProperties()
{
	TriggerActor::RegistProperties();

	AddPropertyClass("AmbientRegionActor");

	AddProperty("HorAngle", Object::PT_FLOAT, mHorAngle);
	AddProperty("VerAngle", Object::PT_FLOAT, mVerAngle);
	AddProperty("LightCameraLookPosition", Object::PT_APOINT3, mLightCameraLookPosition);
	AddProperty("LightCameraLookDistance", Object::PT_FLOAT, mLightCameraLookDistance);
	AddProperty("LightCameraExtent", Object::PT_FLOAT, mLightCameraExtent);

	AddProperty("AmbientColor", Object::PT_COLOR3FLOAT3, mAmbientColor);
	AddProperty("DiffuseColor", Object::PT_COLOR3FLOAT3, mDiffuseColor);
	AddProperty("SpecularColor", Object::PT_COLOR3FLOAT3, mSpecularColor);
	AddProperty("SpecularPow", Object::PT_FLOAT, mSpecularPow);
	AddProperty("Intensity", Object::PT_FLOAT, mIntensity);
}
//----------------------------------------------------------------------------
void AmbientRegionActor::OnPropertyChanged(const PropertyObject &obj)
{
	TriggerActor::OnPropertyChanged(obj);

	if ("HorAngle" == obj.Name)
	{
		SetHorAngle(PX2_ANY_AS(obj.Data, float));
	}
	else if ("VerAngle" == obj.Name)
	{
		SetVerAngle(PX2_ANY_AS(obj.Data, float));
	}
	else if ("LightCameraLookPosition" == obj.Name)
	{
		SetLightCameraLookPostion(PX2_ANY_AS(obj.Data, APoint));
	}
	else if ("LightCameraLookDistance" == obj.Name)
	{
		SetLightCameraLookDistance(PX2_ANY_AS(obj.Data, float));
	}
	else if ("LightCameraExtent" == obj.Name)
	{
		SetLightCameraExtent(PX2_ANY_AS(obj.Data, float));
	}
	else if ("AmbientColor" == obj.Name)
	{
		SetAmbientColor(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("DiffuseColor" == obj.Name)
	{
		SetDiffuseColor(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("SpecularColor" == obj.Name)
	{
		SetSpecularColor(PX2_ANY_AS(obj.Data, Float3));
	}
	else if ("SpecularPow" == obj.Name)
	{
		SetSpecularPow(PX2_ANY_AS(obj.Data, float));
	}
	else if ("Intensity" == obj.Name)
	{
		SetIntensity(PX2_ANY_AS(obj.Data, float));
	}
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 名称支持
//----------------------------------------------------------------------------
Object* AmbientRegionActor::GetObjectByName(const std::string& name)
{
	Object* found = TriggerActor::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	return 0;
}
//----------------------------------------------------------------------------
void AmbientRegionActor::GetAllObjectsByName(const std::string& name,
	std::vector<Object*>& objects)
{
	TriggerActor::GetAllObjectsByName(name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// 持久化支持
//----------------------------------------------------------------------------
AmbientRegionActor::AmbientRegionActor(LoadConstructor value) :
TriggerActor(value),
mIsNeedUpdated(true)
{
}
//----------------------------------------------------------------------------
void AmbientRegionActor::Load(InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TriggerActor::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mHorAngle);
	source.Read(mVerAngle);

	source.ReadAggregate(mAmbientColor);
	source.ReadAggregate(mDiffuseColor);
	source.ReadAggregate(mSpecularColor);
	source.Read(mSpecularPow);
	source.Read(mIntensity);

	source.ReadAggregate(mLightCameraLookPosition);
	source.Read(mLightCameraLookDistance);
	source.Read(mLightCameraExtent);

	PX2_END_DEBUG_STREAM_LOAD(AmbientRegionActor, source);
}
//----------------------------------------------------------------------------
void AmbientRegionActor::Link(InStream& source)
{
	TriggerActor::Link(source);
}
//----------------------------------------------------------------------------
void AmbientRegionActor::PostLink()
{
	TriggerActor::PostLink();

	SetHorAngle(mHorAngle);
	SetVerAngle(mVerAngle);
	SetAmbientColor(mAmbientColor);
	SetDiffuseColor(mDiffuseColor);
	SetSpecularColor(mSpecularColor);
	SetSpecularPow(mSpecularPow);

	_UpdateDirLightCamera();
}
//----------------------------------------------------------------------------
bool AmbientRegionActor::Register(OutStream& target) const
{
	if (TriggerActor::Register(target))
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void AmbientRegionActor::Save(OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TriggerActor::Save(target);
	PX2_VERSION_SAVE(target);
	
	target.Write(mHorAngle);
	target.Write(mVerAngle);

	target.WriteAggregate(mAmbientColor);
	target.WriteAggregate(mDiffuseColor);
	target.WriteAggregate(mSpecularColor);
	target.Write(mSpecularPow);
	target.Write(mIntensity);

	target.WriteAggregate(mLightCameraLookPosition);
	target.Write(mLightCameraLookDistance);
	target.Write(mLightCameraExtent);

	PX2_END_DEBUG_STREAM_SAVE(AmbientRegionActor, target);
}
//----------------------------------------------------------------------------
int AmbientRegionActor::GetStreamingSize(Stream &stream) const
{
	int size = TriggerActor::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);

	size += sizeof(mHorAngle);
	size += sizeof(mVerAngle);

	size += sizeof(mAmbientColor);
	size += sizeof(mDiffuseColor);
	size += sizeof(mSpecularColor);
	size += sizeof(mSpecularPow);
	size += sizeof(mIntensity);

	size += sizeof(mLightCameraLookPosition);
	size += sizeof(mLightCameraLookDistance);
	size += sizeof(mLightCameraExtent);

	return size;
}
//----------------------------------------------------------------------------