// PX2SkillActor.hpp

#ifndef PX2SKILLACTOR_HPP
#define PX2SKILLACTOR_HPP

#include "PX2Actor.hpp"
#include "PX2InterpCurveFloat3Controller.hpp"

namespace PX2
{

	class SkillActor : public Actor
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(SkillActor);

	public:
		SkillActor();
		virtual ~SkillActor();

		virtual void Update(double appSeconds);

		enum TargetType
		{
			TT_ACTOR,
			TT_POSITION,
			TT_MAX_TYPE
		};
		void SetTargetType(TargetType type);
		TargetType GetTargetType() const;

		void SetTrackSpeed(float speed);
		float GetTrackSpeed() const;

		void SetBeginPos(const APoint &pos);
		const APoint &GetBeginPos() const;

		void SetCharacterGroup(int group);
		int GetCharacterGroup() const;

		void SetSkillTypeID(int ID);
		int GetSkillITypeID() const;

		void SetTargetID(int ID);
		int GetTargetID() const;
		void SetTargetPos(const APoint &pos);
		const APoint &GetTargetPos() const;

		void SetDegree(float degree);
		float GetDegree() const;

		void SetAlignTrace(bool alignTrace);
		bool IsAlignTrace() const;

		void SetDoDamageOnTrack(bool damage);
		bool IsDoDamageOnTrack() const;

		void Start();
		virtual void OnStart();

		void SetOver(bool over);
		bool IsOver() const;

	protected:
		TargetType mTargetType;
		float mTrackSpeed;
		APoint mBeginPos;
		int mCharacterGroup;
		int mSkillTypeID;
		int mTargetID;
		APoint mTargetPos;
		float mDegree;
		bool mIsAlignTrace;
		bool mIsDoDamageOnTrack;

		float mFlyToNeedTime;
		APoint mLastPos;
		float mCurRunTime;
		InterpCurveFloat3 mCurve;
		bool mIsOver;
	};

	PX2_REGISTER_STREAM(SkillActor);
	typedef Pointer0<SkillActor> SkillActorPtr;
#include "PX2SkillActor.inl"

}

#endif