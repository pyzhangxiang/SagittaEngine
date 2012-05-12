//////////////////////////////////////////////////////
// file: sgNode.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgNode
// sgNode is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgNode.h"
#include "../../common/utils/sgException.h"
#include "../../common/utils/sgStringUtil.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	unsigned long sgNode::ms_iNodeCount = 1;

	//  [7/30/2008 zhangxiang]
	sgNode::sgNode(void) :
	sgObject(),
	mc_iNodeID(ms_iNodeCount),
	m_bActive(true),
	m_pParent(0),
	m_bNeedUpdateFromParent(true),
	m_RelativeOrientation(Quaternion::IDENTITY),
	m_RelativePosition(Vector3::ZERO),
	m_RelativeScale(Vector3::UNIT_SCALE),
	m_bInheritOrientation(true),
	m_bInheritScale(true),
	m_DerivedOrientation(Quaternion::IDENTITY),
	m_DerivedPosition(Vector3::ZERO),
	m_DerivedScale(Vector3::UNIT_SCALE),
	m_bCachedTransformOutOfDate(true){
		m_sName = "Node" + sgStringUtil::to_string(sgNode::ms_iNodeCount++);
		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	sgNode::sgNode(const StdString &aName) :
	sgObject(),
	mc_iNodeID(ms_iNodeCount),
	m_sName(aName),
	m_bActive(true),
	m_pParent(0),
	m_bNeedUpdateFromParent(true),
	m_RelativeOrientation(Quaternion::IDENTITY),
	m_RelativePosition(Vector3::ZERO),
	m_RelativeScale(Vector3::UNIT_SCALE),
	m_bInheritOrientation(true),
	m_bInheritScale(true),
	m_DerivedOrientation(Quaternion::IDENTITY),
	m_DerivedPosition(Vector3::ZERO),
	m_DerivedScale(Vector3::UNIT_SCALE),
	m_bCachedTransformOutOfDate(true){
		++ms_iNodeCount;
		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	sgNode::~sgNode(){
		removeAllChildren();
		if(m_pParent){
			m_pParent->removeChild(this);
		}
	}

	//  [1/9/2009 zhangxiang]
	uLong sgNode::getNodeID(void) const{
		return mc_iNodeID;
	}

	//  [1/9/2009 zhangxiang]
	const StdString &sgNode::name(void) const{
		return m_sName;
	}

	//  [1/9/2009 zhangxiang]
	void sgNode::setName(const StdString &aName){
		m_sName = aName;
	}

	//  [1/9/2009 zhangxiang]
	bool sgNode::isActive(void) const{
		return m_bActive;
	}

	//  [1/2/2009 zhangxiang] -- in sgNode
	//  [1/9/2009 zhangxiang]
	void sgNode::setActive(bool aActive){
		if(m_bActive != aActive){
			m_bActive = aActive;

			// notify children to set active
			ChildNodeMap::iterator it = m_Children.begin();
			ChildNodeMap::iterator eit = m_Children.end();
			for(; it!=eit; ++it){
				it->second->setActive(aActive);
			}
		}
	}

	//  [1/6/2009 zhangxiang]
	void sgNode::stepToParent(sgNode *aNewParent){
		if(!aNewParent){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Null new parent pointer.", "sgNode::stepToParent");
		}

		if(m_pParent){
			// if i have a parent, ask him to remove me first.
			m_pParent->removeChild(this);
		}

		// at last, ask the new parent to add me.
		aNewParent->addChild(this);
	}

	//  [7/30/2008 zhangxiang]
	sgNode *sgNode::parent(void) const{
		return m_pParent;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::setParent(sgNode *apParent){
		m_pParent = apParent;
		
		// Request update from parent
		needUpdate();

		// after set parent processed, set active
		if(!apParent || !(apParent->isActive())){
			// no parent now, so set inactive
			m_bActive = false;
		}else{
			// new parent is active, i should be active too
			m_bActive = true;
		}
	}

	//  [8/21/2008 zhangxiang]
	bool sgNode::isNeedUpdate(void) const{
		return m_bNeedUpdateFromParent;
	}

	//  [7/30/2008 zhangxiang]
	const Quaternion &sgNode::relativeOrentation(void) const{
		return m_RelativeOrientation;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::setRelativeOrientation(const Quaternion &aq){
		m_RelativeOrientation = aq;
		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::resetRelativeOrientation(void){
		m_RelativeOrientation = Quaternion::IDENTITY;
		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	const Quaternion &sgNode::absoluteOrientation(void) const{
		return _getDerivedOrientation();
	}

	//  [7/30/2008 zhangxiang]
	const Quaternion &sgNode::orientation(void) const{
		if(m_bInheritOrientation)
			return m_DerivedOrientation;
		else
			return m_RelativeOrientation;
	}

	//  [7/30/2008 zhangxiang]
	const Vector3 &sgNode::relativePosition(void) const{
		return m_RelativePosition;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::setRelativePosition(const Vector3 &aPos){
		m_RelativePosition = aPos;
		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	const Vector3 &sgNode::absolutePosition(void) const{
		return _getDerivedPosition();
	}

	//  [8/2/2008 zhangxiang]
	Vector3 sgNode::position(void) const{
		return _getDerivedPosition();
	}

	//  [7/30/2008 zhangxiang]	mabe a bug...
	void sgNode::setAbsolutePosition(const Vector3 &aPos){
		Vector3 aTranslate = aPos - _getDerivedPosition();
		translate(aTranslate, TS_PARENT);
	}

	//  [7/30/2008 zhangxiang]
	const Vector3 &sgNode::relativeScale(void) const{
		return m_RelativeScale;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::setRelativeScale(const Vector3 &aScale){
		m_RelativeScale = aScale;
		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	const Vector3 &sgNode::absoluteScale(void) const{
		return _getDerivedScale();
	}

	//  [7/30/2008 zhangxiang]
	const Vector3 &sgNode::scale(void) const{
		if(m_bInheritScale)
			return m_DerivedScale;
		else
			return m_RelativeScale;
	}

	//  [7/30/2008 zhangxiang]
	const Quaternion &sgNode::_getDerivedOrientation(void) const{
		if(m_bNeedUpdateFromParent){
			_updateFromParent();
		}
		return m_DerivedOrientation;
	}

	//  [7/30/2008 zhangxiang]
	const Vector3 &sgNode::_getDerivedPosition(void) const{
		if(m_bNeedUpdateFromParent){
			_updateFromParent();
		}
		return m_DerivedPosition;
	}

	//  [7/30/2008 zhangxiang]
	const Vector3 &sgNode::_getDerivedScale(void) const{
		if(m_bNeedUpdateFromParent){
			_updateFromParent();
		}
		return m_DerivedScale;
	}

	//  [7/30/2008 zhangxiang]
	bool sgNode::inheritOrientation(void) const{
		return m_bInheritOrientation;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::setInheritOrientation(bool abInherit){
		if(m_bInheritOrientation != abInherit){
			m_bInheritOrientation = abInherit;
			needUpdate();
		}
	}

	bool sgNode::inheritScale(void) const{
		return m_bInheritScale;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::setInheritScale(bool abInherit){
		if(m_bInheritScale != abInherit){
			m_bInheritScale = abInherit;
			needUpdate();
		}
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::scale(const Vector3 &aScale){
		m_RelativeScale *= aScale;
		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::translate(const Vector3 &aTranslate, int aRelativeTo /* = TS_PARENT */){
		Vector3 adjusted;
		switch(aRelativeTo){
		case TS_LOCAL:
			// position is relative to parent so transform downwards
			m_RelativePosition += m_RelativeOrientation * aTranslate;
			break;

		case TS_WORLD:
			// position is relative to parent so transform upwards
			if (m_pParent){
				m_RelativePosition += (m_pParent->_getDerivedOrientation().inverse() * aTranslate)
					/ m_pParent->_getDerivedScale();
			}else{
				m_RelativePosition += aTranslate;
			}
			break;

		case TS_PARENT:
			m_RelativePosition += aTranslate;
			break;
		}

		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::translate(const Matrix3 &aAxes, const Vector3 &aTranslate, int aRelativeTo /* = TS_PARENT */){
		Vector3 derived = aAxes * aTranslate;
		translate(derived, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::roll(const Radian &aAngle, int aRelativeTo /* = TS_LOCAL */){
		rotationZ(aAngle, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::rotationZ(const Radian &aAngle, int aRelativeTo /* = TS_LOCAL */){
		rotate(Vector3::UNIT_Z, aAngle, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::pitch(const Radian &aAngle, int aRelativeTo /* = TS_LOCAL */){
		rotationX(aAngle, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::rotationX(const Radian &aAngle, int aRelativeTo /* = TS_LOCAL */){
		rotate(Vector3::UNIT_X, aAngle, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::yaw(const Radian &aAngle, int aRelativeTo /* = TS_LOCAL */){
		rotationY(aAngle, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::rotationY(const Radian &aAngle, int aRelativeTo /* = TS_LOCAL */){
		rotate(Vector3::UNIT_Y, aAngle, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::rotate(const Vector3 &aAxis, const Radian& aAngle, int aRelativeTo /* = TS_LOCAL */){
		Quaternion q;
		q.fromAngleAxis(aAngle, aAxis);
		rotate(q, aRelativeTo);
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::rotate(const Quaternion &aq, int aRelativeTo /* = TS_LOCAL */){
		switch(aRelativeTo){
		case TS_PARENT:
			// Rotations are normally relative to local axes, transform up
			m_RelativeOrientation = aq * m_RelativeOrientation;
			break;

		case TS_WORLD:
			// Rotations are normally relative to local axes, transform up
			m_RelativeOrientation = m_RelativeOrientation * _getDerivedOrientation().inverse()
				* aq * _getDerivedOrientation();
			break;

		case TS_LOCAL:
			// Note the order of the mult, i.e. q comes after
			m_RelativeOrientation = m_RelativeOrientation * aq;
			break;
		}

		needUpdate();
	}

	//  [7/30/2008 zhangxiang]
	Matrix3 sgNode::getLocalAxes(void) const{
		Vector3 axisX = Vector3::UNIT_X;
		Vector3 axisY = Vector3::UNIT_Y;
		Vector3 axisZ = Vector3::UNIT_Z;

		axisX = m_RelativeOrientation * axisX;
		axisY = m_RelativeOrientation * axisY;
		axisZ = m_RelativeOrientation * axisZ;

		return Matrix3(axisX.x(), axisY.x(), axisZ.x(),
					axisX.y(), axisY.y(), axisZ.y(),
					axisX.z(), axisY.z(), axisZ.z());
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::addChild(sgNode* aChild){
		if(!aChild){
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL child node pointer.", "sgNode::addChild");
		}

		// Continue when the new child has no parent or
		if(aChild->parent()){
			if(aChild->parent() != this){
				THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Node \"" + aChild->name() + "\" already was a child of \"" + aChild->parent()->name() + "\".", "sgNode::addChild");
			}else{
				return ;
			}
		}

		m_Children.insert(std::make_pair(aChild->getNodeID(), aChild));
		aChild->setParent(this);
	}

	//  [7/30/2008 zhangxiang]
	size_t sgNode::childNum(void) const{
		return m_Children.size();
	}

	//  [7/30/2008 zhangxiang]
	sgNode *sgNode::getChild(size_t aIndex) const{
		if(aIndex < m_Children.size()){
			ChildNodeMap::const_iterator i = m_Children.begin();
			while(aIndex--) ++i;
			return i->second;
		}else{
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Child index out of range.", "sgNode::getChild");
		}
	}

	//  [7/30/2008 zhangxiang]
	sgNode *sgNode::getChild(const StdString &aName) const{
		ChildNodeMap::const_iterator i = m_Children.begin();
		for(; i!=m_Children.end(); ++i){
			if(i->second->name() == aName)
				return i->second;
		}

		THROW_SAGI_EXCEPT(sgException::ERR_ITEM_NOT_FOUND, "Child node named " + aName + "does not exist.", "sgNode::getChild");
	}

	//  [7/30/2008 zhangxiang]
	sgNode *sgNode::removeChild(size_t aIndex){
		sgNode* ret;
		if(aIndex < m_Children.size()){
			ChildNodeMap::iterator i = m_Children.begin();
			while (aIndex--) ++i;
			ret = i->second;
			
			m_Children.erase(i);
			ret->setParent(0);
			return ret;
		}else{
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "Child index out of range.", "sgNode::removeChild");
		}
	}

	//  [7/30/2008 zhangxiang]
	sgNode *sgNode::removeChild(sgNode *apChild){
		if(apChild){
			ChildNodeMap::iterator i = m_Children.find(apChild->getNodeID());

			if(i == m_Children.end()){
				THROW_SAGI_EXCEPT(sgException::ERR_ITEM_NOT_FOUND, "Child Node" + sgStringUtil::to_string(apChild->getNodeID()) + " does not exist.", "sgNode::removeChild");
			}

			sgNode* ret = i->second;
			m_Children.erase(i);
			ret->setParent(0);
			return ret;
		}else{
			THROW_SAGI_EXCEPT(sgException::ERR_INVALIDPARAMS, "NULL child for remove.", "sgNode::removeChild");
		}
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::removeAllChildren(void){
		ChildNodeMap::iterator i, iend;
		iend = m_Children.end();
		for (i = m_Children.begin(); i != iend; ++i){
			i->second->setParent(0);
		}
		m_Children.clear();
	}

	//  [7/30/2008 zhangxiang]
	const Matrix4 &sgNode::_getFullTransform(void) const{
		if(m_bCachedTransformOutOfDate){
			// Use derived values
			m_CachedTransform.makeTransform(
				_getDerivedPosition(),
				_getDerivedScale(),
				_getDerivedOrientation());
			m_bCachedTransformOutOfDate = false;
		}
		return m_CachedTransform;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::needUpdate(void){
		m_bNeedUpdateFromParent = true;
		m_bCachedTransformOutOfDate = true;

		// notify children to update
		ChildNodeMap::iterator it = m_Children.begin();
		ChildNodeMap::iterator eit = m_Children.end();
		for(; it!=eit; ++it){
			it->second->needUpdate();
		}
	}

	void sgNode::_updateFromParent(void) const{
		// no parent, cannot transform
		if(m_pParent != NULL){
			// Update orientation
			const Quaternion& parentOrientation = m_pParent->_getDerivedOrientation();
			// Combine orientation with that of parent
			m_DerivedOrientation = parentOrientation * m_RelativeOrientation;

			// Update scale
			const Vector3& parentScale = m_pParent->_getDerivedScale();
			// Scale own position by parent scale, NB just combine
			// as equivalent axes, no shearing
			m_DerivedScale = parentScale * m_RelativeScale;

			// Change position vector based on parent's orientation & scale
			m_DerivedPosition = parentOrientation * (parentScale * m_RelativePosition);

			// Add altered position vector to parents
			m_DerivedPosition += m_pParent->_getDerivedPosition();

		}
		/*else{
			// Root node, no parent
			m_DerivedOrientation = m_RelativeOrientation;
			m_DerivedPosition = m_RelativePosition;
			m_DerivedScale = m_RelativeScale;
		
		}
		*/
		

		m_bCachedTransformOutOfDate = true;
		m_bNeedUpdateFromParent = false;

	}

	//  [1/6/2009 zhangxiang]
	sgNode::ChildIterator sgNode::getChildIterator(void){
		return sgNode::ChildIterator(m_Children);
	}

	//  [1/6/2009 zhangxiang]
	sgNode::ConstChildIterator sgNode::getConstChildIterator(void) const{
		return sgNode::ConstChildIterator(m_Children);
	}

} // namespace Sagitta