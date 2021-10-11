#include "Object.h"
size_t Object::m_AmountOfObjects = 0;
Object::Object()
	: m_ID{ m_AmountOfObjects++ }
{
}