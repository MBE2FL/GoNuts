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

void Animator::calculateCurrentPose()
{
	getPrevNextFrames();
	float interValue = invLerp(_currentTime, _prevFrame->getStartTime(), _nextFrame->getStartTime());
	blendPoses(interValue);
}

void Animator::getPrevNextFrames()
{
	vector<KeyFrame*> prevNextFrames;
	vector<KeyFrame*> allFrames = _animation->getKeyFrames();
	_prevFrame = allFrames[0];
	_nextFrame = allFrames[0];

	// Find the frames before and after the current animation time. ##Maybe keey track of the current frame.
	for (size_t i = 1; i < allFrames.size(); ++i)
	{
		_nextFrame = allFrames[i];
		if (_nextFrame->getStartTime() > _currentTime)
			break;

		_prevFrame = allFrames[i];
	}
}

void Animator::blendPoses(float interValue)
{
	unordered_map<string, JointTransform*> prevFrameKeys = _prevFrame->getJointKeyFrames();
	unordered_map<string, JointTransform*> nextFrameKeys = _nextFrame->getJointKeyFrames();
	unordered_map<string, JointTransform*>::iterator it;

	// Go through each joint in the previous key frame, and blend it with the same joint in the next key frame.
	for (it = prevFrameKeys.begin(); it != prevFrameKeys.end(); ++it)
	{
		JointTransform* prevTransform = prevFrameKeys[it->first];
		JointTransform* nextTransform = nextFrameKeys[it->first];
		JointTransform currTransform = JointTransform::interpolate(*prevTransform, *nextTransform, interValue);
		_currentPose[it->first] = currTransform.getLocalTransform();
	}
}

void Animator::applyPoseToJoints(Joint * joint, const mat4 & parentLocalToWorldMatrix)
{
	mat4 localTransformMatrix = _currentPose[joint->getName()];
	mat4 localToWorldMatrix = parentLocalToWorldMatrix * localToWorldMatrix;

	for (Joint* child : joint->getChildren())
	{
		applyPoseToJoints(child, localToWorldMatrix);
	}

	// Similar to the camera view, calculate this joint's transfrom with the start transfrom acting as the origin.
	localToWorldMatrix * joint->getInverseBindTransfrom();
	joint->setAnimatedTransform(localToWorldMatrix);
}
