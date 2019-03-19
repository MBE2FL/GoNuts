#include "Animator.h"
#include "SkeletalMesh.h"

Animator::Animator(SkeletalMesh * mesh)
{
	_mesh = mesh;
}

Animator::~Animator()
{
}

void Animator::setAnimation(SAnimation * animation)
{
	_currentTime = 0.0f;
	_animation = animation;
}

void Animator::update(float deltaTime)
{
	// Only animate if there is an animation loaded in.
	if (!_animation)
		return;

	// Adjust time if it goes beyond the duration of the current animation.
	_currentTime += deltaTime;
	if (_currentTime > _animation->getDuration())
	{
		_currentTime -= _animation->getDuration();
	}

	// Calculate the current pose of the animation.
	calculateCurrentPose();
	// Apply the current pose to the joints.
	applyPoseToJoints(_mesh->getRootJoint(), mat4::Identity);

}

void Animator::nextFrame()
{
	++frameNum;
	if (frameNum >= _animation->getJointAnims().front()->getKeyFrames().size())
		frameNum = 0;
}

void Animator::calculateCurrentPose()
{
	getPrevNextFrames();
	//float interValue = invLerp(_currentTime, _prevFrame->getStartTime(), _nextFrame->getStartTime());
	//blendPoses(interValue);
}

void Animator::getPrevNextFrames()
{
	// Find the previous and next frame on animation for each joint.
	vector<JointAnimation*> jointAnims = _animation->getJointAnims();

	for (JointAnimation* jointAnim : jointAnims)
	{
		vector<KeyFrame*> allFrames = jointAnim->getKeyFrames();
		KeyFrame* prevFrame = allFrames[0];
		KeyFrame* nextFrame = allFrames[0];
		string jointName = jointAnim->getName();

		// Find the frames before and after the current animation time. ##Maybe keey track of the current frame.
		for (size_t i = 1; i < allFrames.size(); ++i)
		{
			nextFrame = allFrames[i];
			if (nextFrame->getStartTime() > _currentTime)
				break;

			prevFrame = allFrames[i];
		}

		// Blend between the previous and next frame of animation.
		float interValue = invLerp(_currentTime, prevFrame->getStartTime(), nextFrame->getStartTime());
		prevFrame = allFrames[frameNum];
		nextFrame = allFrames[frameNum];
		interValue = 0.0f;
		blendPoses(prevFrame, nextFrame, interValue, jointName);
	}
}

void Animator::blendPoses(KeyFrame * prevFrame, KeyFrame * nextFrame, float interValue, const string & jointName)
{
	// Blend joint's previous frame's transform with it's next frame's transform.
	JointTransform* prevJointTransform = prevFrame->getJointTransform();
	JointTransform* nextJointTransform = nextFrame->getJointTransform();

	JointTransform currJointTransform = JointTransform::interpolate(*prevJointTransform, *nextJointTransform, interValue);
	//JointTransform currJointTransform = *prevJointTransform;
	_currentPose[jointName] = currJointTransform.getLocalTransform();
}


void Animator::applyPoseToJoints(Joint * joint, const mat4 & parentLocalToWorldMatrix)
{
	mat4 localTransformMatrix = _currentPose[joint->getName()];
	//mat4 localTransformMatrix = joint->getLocalBindTransform();
	mat4 localToWorldMatrix = parentLocalToWorldMatrix * localTransformMatrix;

	for (Joint* child : joint->getChildren())
	{
		applyPoseToJoints(child, localToWorldMatrix);
	}

	// Similar to the camera view, calculate this joint's transfrom with the start transform acting as the origin.
	//localToWorldMatrix = joint->getLocalBindTransform();
	localToWorldMatrix = localToWorldMatrix * joint->getInverseBindTransfrom();
	joint->setAnimatedTransform(localToWorldMatrix);
}
