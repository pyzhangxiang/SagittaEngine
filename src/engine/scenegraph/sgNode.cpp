//////////////////////////////////////////////////////
// file: sgNode.cpp @ 2008-1-31 by Zhang Xiang
// defines of the class sgNode
// sgNode is a class ...
//////////////////////////////////////////////////////

// INCLUDES //////////////////////////////////////////
#include "sgNode.h"
#include "engine/common/sgException.h"

// DECLARES //////////////////////////////////////////

// DEFINES ///////////////////////////////////////////
namespace Sagitta{

	SG_META_DEFINE_ABSTRACT(sgNode, sgObject)

	//  [7/30/2008 zhangxiang]
	sgNode::sgNode(void) :
	sgObject(),
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
		needUpdate();
	}


	//  [7/30/2008 zhangxiang]
    //  [10/26/2012 zhangxiang]
	sgNode::~sgNode(){

        if(m_pParent)
        {
            // detached me from my parent
            m_pParent->removeChild(this);
            m_pParent = 0;
        }
        
        // destroy all children
        ChildNodeMap to_rm = m_Children;
        // first clear map
        // so that while in the destruction of children
        // detaching from me would be faster
        m_Children.clear();
        
		ChildNodeMap::iterator it = to_rm.begin();
		ChildNodeMap::iterator eit = to_rm.end();
		for(; it!=eit; ++it)
		{
			it->second->__markDestroying();
		}

		it = to_rm.begin();
		for(; it!=eit; ++it)
		{
            sgObject::destroyObject(it->second, false);
		}
		
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


	//  [7/30/2008 zhangxiang]
	sgNode *sgNode::parent(void) const{
		return m_pParent;
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::setParent(sgNode *apParent){

        if(m_pParent)
        {
            m_pParent->removeChild(this);
        }
		m_pParent = apParent;
        
        if(m_pParent)
        {
            m_pParent->addChild(this);
            
            // Request update from parent
            needUpdate();

            // after set parent processed, set active
            if( !(m_pParent->isActive()) )
                m_bActive = false;
        }
        
        this->onSetParent(apParent);
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
		if(!aChild)
        {
			return ;
		}

        ChildNodeMap::iterator it = m_Children.find(aChild->getId());
        if(it != m_Children.end())
            return ;
        
		m_Children.insert(std::make_pair(aChild->getId(), aChild));

	}

	//  [7/30/2008 zhangxiang]
	size_t sgNode::childNum(void) const{
		return m_Children.size();
	}

	//  [7/30/2008 zhangxiang]
	void sgNode::removeChild(sgNode *apChild){
        if(!apChild)
            return ;

        ChildNodeMap::iterator it = m_Children.find(apChild->getId());
        if(it == m_Children.end())
            return ;

        m_Children.erase(it);

	}

	//  [7/30/2008 zhangxiang]
	void sgNode::removeAllChildren(void){
        ChildNodeMap torm = m_Children;
        
		ChildNodeMap::iterator i, iend;
		iend = torm.end();
		for (i = torm.begin(); i != iend; ++i){
			i->second->setParent(0);
		}
		m_Children.clear();
	}

	//  [7/30/2008 zhangxiang]
	const Matrix4 &sgNode::getFullTransform(void) const{
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
		else{
			// Root node, no parent
			m_DerivedOrientation = m_RelativeOrientation;
			m_DerivedPosition = m_RelativePosition;
			m_DerivedScale = m_RelativeScale;
		
		}
		
		

		m_bCachedTransformOutOfDate = true;
		m_bNeedUpdateFromParent = false;

	}
    
    //  [22/9/2012 zhangxiang]
    sgNode *sgNode::getChild(id_type aId) const
    {
        ChildNodeMap::const_iterator it = m_Children.find(aId);
        if(it == m_Children.end())
            return NULL;
        return it->second;
    }
    
    sgNode *sgNode::getFirstChild(void) const
    {
        if(m_Children.empty())
            return NULL;
        return m_Children.begin()->second;
    }

	//  [1/6/2009 zhangxiang]
	sgNode::ChildIterator sgNode::getChildIterator(void){
		return sgNode::ChildIterator(m_Children);
	}

	//  [1/6/2009 zhangxiang]
	sgNode::ConstChildIterator sgNode::getConstChildIterator(void) const{
		return sgNode::ConstChildIterator(m_Children);
	}

	sgNode * sgNode::root( void )
	{
		sgNode *node = this;
		while(node->parent())
		{
			node = node->parent();
		}

		return node;
	}

	void sgNode::update( Float32 deltaTime )
	{
		if(m_bNeedUpdateFromParent)
			_updateFromParent();

		// update children
		ChildNodeMap::iterator it = m_Children.begin();
		ChildNodeMap::iterator eit = m_Children.end();
		for(; it!=eit; ++it)
		{
			it->second->update(deltaTime);
		}

	}

    void sgNode::onSetParent(sgNode *aParent)
    {
        
    }

	void sgNode::setAbsoluteOrientation( const Quaternion &aq )
	{
		Quaternion q = Quaternion::IDENTITY;
		if(parent())
		{
			q = parent()->absoluteOrientation().inverse();
		}
		setRelativeOrientation(q * aq);
	}

	void sgNode::getInheritsNodes( NodeList &outlist, size_t count /*= 0*/, const StringHandleSet &classTypefilter /*= StringHandleSet()*/ )
	{
		outlist.clear();
		if(count > 0)
			outlist.reserve(count);
		else
			count = size_t(-1) - 1;

		bool doFilter = true;
		if(classTypefilter.empty())
			doFilter = false;

		size_t mycount = 0;

		if(mycount == count)
			return ;

		sg_list(sgNode*) nodeQueue;
		nodeQueue.push_back(this);

		ChildNodeMap *childMap = &m_Children;
		ChildNodeMap::const_iterator it = childMap->begin();
		while(mycount < count && !nodeQueue.empty())
		{
			sgNode *node = nodeQueue.front();
			nodeQueue.pop_front();
			// do sth.
			if(!doFilter)
			{
				outlist.push_back(node);
				++mycount;
			}
			else if(classTypefilter.find(node->GetMyClassName()) != classTypefilter.end())
			{
				outlist.push_back(node);
				++mycount;
			}

			childMap = &(node->m_Children);
			it = childMap->begin();
			for(; it!=childMap->end(); ++it)
			{
				nodeQueue.push_back(it->second);
			}

		}
	}

} // namespace Sagitta
