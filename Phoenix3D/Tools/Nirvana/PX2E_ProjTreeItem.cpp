// PX2E_ProjTreeItem.cpp

#include "PX2E_ProjTreeItem.hpp"
#include "PX2E_ProjTree.hpp"
#include "PX2Actor.hpp"
#include "PX2Project.hpp"
#include "PX2Edit.hpp"
#include "PX2E_Define.hpp"
using namespace PX2Editor;
using namespace PX2;

//-----------------------------------------------------------------------------
ProjTreeItem::ProjTreeItem(ProjTree *tree, ProjTreeItem *parent,
	ItemType type, int iconID, PX2::Object *obj,
	ProjTreeLevel showLevel, const std::string &name,
	bool doInsert, wxTreeItemId insertUpID) :
	mProjTree(tree),
	mParentItem(parent),
	mItemType(type),
	mIconID(iconID),
	mObject(obj)
{
	assertion(0 != parent, "parent must not be 0.\n");

	std::string useName = name;
	if (obj && name.empty())
	{
		useName = obj->GetName();
	}
	mName = useName;

	if (!doInsert)
		mItemID = mProjTree->AppendItem(parent->GetItemID(), useName, mIconID);
	else
		mItemID = mProjTree->InsertItem(parent->GetItemID(), insertUpID, useName, mIconID);

	SetTreeLevel(showLevel);
}
//-----------------------------------------------------------------------------
ProjTreeItem::ProjTreeItem(ProjTree *tree, wxTreeItemId id,
	ItemType type, int iconID, const std::string &name) :
mProjTree(tree),
mItemID(id),
mItemType(type),
mIconID(iconID),
mParentItem(0),
mName(name)
{
}
//-----------------------------------------------------------------------------
ProjTreeItem::~ProjTreeItem()
{
	ClearChildren();
	mProjTree->Delete(mItemID);
}
//-----------------------------------------------------------------------------
void ProjTreeItem::SetTreeLevel(ProjTreeLevel level)
{
	mTreeLevel = level;

	Project *proj = DynamicCast<Project>(mObject);
	Scene *scene = DynamicCast<Scene>(mObject);
	if (proj || scene)
		return;

	Actor *actor = DynamicCast<Actor>(mObject);
	Movable *mov = DynamicCast<Movable>(mObject);
	Node *node = DynamicCast<Node>(mObject);

	if (!mObject)
	{ // IT_CATALOG
		for (int i = 0; i < (int)mChildItems.size(); i++)
		{
			ProjTreeItem *item = mChildItems[i];
			if (item) item->SetTreeLevel(level);
		}
		return;
	}
	else
	{
		ClearChildren();
	}

	bool addCtrl = false;
	bool addNode = false;

	if (PTL_GENERAL == mTreeLevel)
	{
	}
	else if (PTL_CHILDREN == mTreeLevel)
	{
		if (!scene && !proj && node)
		{
			addNode = true;
		}
	}
	else if (PTL_CONTROLS == mTreeLevel)
	{
		if (!scene && !proj && mov)
		{
			addCtrl = true;
		}

		if (!scene && !proj && node)
		{
			addNode = true;
		}
	}

	if (addCtrl)
	{
		int numCtrls = mov->GetNumControllers();
		for (int i = 0; i < numCtrls; i++)
		{
			Controller *ctrl = mov->GetController(i);

			AddChild(ctrl, mIconID, mTreeLevel);
		}
	}

	if (addNode)
	{
		int numChildren = node->GetNumChildren();
		for (int i = 0; i < numChildren; i++)
		{
			Movable *child = node->GetChild(i);
			if (child)
			{
				AddChild(child, mIconID, mTreeLevel);
			}
		}
	}
}
//-----------------------------------------------------------------------------
void ProjTreeItem::SetName(const std::string &name)
{
	mName = name;

	mProjTree->SetItemText(mItemID, name);

	if (mObject) mObject->SetName(name);
}
//-----------------------------------------------------------------------------
ProjTreeItem *ProjTreeItem::AddChild(PX2::Object *obj, int iconID, 
	ProjTreeLevel showLevel)
{
	ProjTreeItem *item = new ProjTreeItem(mProjTree, this, IT_OBJECT, iconID,
		obj, showLevel);
	mChildItems.push_back(item);

	return item;
}
//-----------------------------------------------------------------------------
ProjTreeItem *ProjTreeItem::InsertChild(wxTreeItemId upID, PX2::Object *obj,
	int iconID, ProjTreeLevel showLevel)
{
	ProjTreeItem *item = new ProjTreeItem(mProjTree, this, IT_OBJECT, iconID,
		obj, showLevel, "", true, upID);
	mChildItems.push_back(item);

	return item;
}
//-----------------------------------------------------------------------------
void ProjTreeItem::RemoveChild(PX2::Object *obj)
{
	std::vector<ProjTreeItem*>::iterator it = mChildItems.begin();
	for (; it != mChildItems.end(); it++)
	{
		if ((*it)->GetObject() == obj)
		{
			delete (*it);
			mChildItems.erase(it);
			return;
		}
	}
}
//-----------------------------------------------------------------------------
void ProjTreeItem::ClearChildren()
{
	for (int i = 0; i < (int)mChildItems.size(); i++)
	{
		delete mChildItems[i];
	}
	mChildItems.clear();
}
//-----------------------------------------------------------------------------
ProjTreeItem *ProjTreeItem::GetItem(PX2::Object *obj)
{
	if (mObject == obj)
		return this;

	for (int i = 0; i < (int)mChildItems.size(); i++)
	{
		ProjTreeItem *item = mChildItems[i]->GetItem(obj);
		if (item) return item;
	}

	return 0;
}
//-----------------------------------------------------------------------------
ProjTreeItem *ProjTreeItem::GetItem(wxTreeItemId id)
{
	
	if (mItemID == id)
		return this;

	for (int i = 0; i < (int)mChildItems.size(); i++) 
	{
		if (mChildItems[i]) 
		{
			ProjTreeItem *item = mChildItems[i]->GetItem(id);
			if (item) return item;
		}
	}

	return 0;
}
//-----------------------------------------------------------------------------