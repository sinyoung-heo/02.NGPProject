#pragma once

namespace ObjID
{
	enum ID 
	{
		SKILL, PLAYER, SKILL_COLBOX , 
		ITEM,OTHERS,
		MONSTER_COW, MONSTER_NINJA, MONSTER_BORIS,
		MONSTER, MON_ATT_COLBOX,
		NPC, SELL_LST,
		INVENTORY, EQUIPMENT,
		EFFECT, 
		BUTTON, UI, UI_BUTTON, 
		COMBO_BOX,
		END 
	};
}


namespace ObjDir
{
	enum DIR
	{
		DOWN, LEFT, LEFT_DOWN, LEFT_UP,
		RIGHT, RIGHT_DOWN, RIGHT_UP, UP
	};
}

namespace ItemType
{
	enum TYPE
	{
		HELMET, ARMOR, WEAPON, SHOES, TYPE_END
	};
}

namespace Layer
{
	enum ID { BACKGROUND, GAMEOBJECT, EFFECT, UI, END };
}