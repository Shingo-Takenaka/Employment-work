#include "DrawManager.h"

#include <algorithm>

DrawManager::DrawManager()
{
}

DrawManager::~DrawManager()
{
}

void DrawManager::Add(
    VECTOR position,
    float sortValue,
    std::function<void()> draw)
{
    DrawObject object;

    object.position = position;
    object.sortValue = sortValue;
    object.draw = draw;

    m_drawList.push_back(object);
}

void DrawManager::Clear()
{
    m_drawList.clear();
}

void DrawManager::Draw()
{
    // Žè‘O‚É‚È‚é‚à‚Ì‚Ù‚ÇŒã‚Å•`‰æ‚·‚é
    std::sort(
        m_drawList.begin(),
        m_drawList.end(),
        [](const DrawObject& a, const DrawObject& b)
        {
            return a.sortValue > b.sortValue;
        });

    for (auto& object : m_drawList)
    {
        object.draw();
    }
}