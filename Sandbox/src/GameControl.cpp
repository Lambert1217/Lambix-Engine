#include "GameControl.h"

GameControl::GameControl()
{
    std::srand(time(NULL));
    reset();
}

void GameControl::OnEvent(Lambix::Event& event)
{
    Lambix::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<Lambix::KeyReleasedEvent>(LB_BIND_EVENT_FN(GameControl::OnKeyTyped));
}

void GameControl::reset()
{
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            m_Data[i][j] = 0;
        }
    }
    m_EmptyBlock = 16;
    CreateBlock();
    CreateBlock();
    m_GameStatus = Running;
}
void GameControl::CreateBlock()
{
    if (m_EmptyBlock == 0) return;
    int emptyIndex = std::rand() % m_EmptyBlock;
    // 遍历数组，找到第 emptyIndex 个空白格
    int count = 0;
    for (int x = 0; x < 4; ++x)
    {
        for (int y = 0; y < 4; ++y)
        {
            if (m_Data[x][y] == 0)
            {
                if (count == emptyIndex)
                {
                    // 生成新的2或4的数字方块
                    m_Data[x][y] = (std::rand() % 3 == 0) ? 4 : 2;
                    m_EmptyBlock--;
                }
                count++;
            }
        }
    }
}

bool GameControl::isDefeat()
{
    UpdateEmptyBlock();
    if (m_EmptyBlock != 0)
    {
        return false;
    }
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (m_Data[i][j] == m_Data[i][j + 1])
            {
                return false;
            }
        }
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (m_Data[i][j] == m_Data[i + 1][j])
            {
                return false;
            }
        }
    }

    return true;
}

void GameControl::Move(MoveDirection direction)
{
    for (int i = 0;i < 4;i++)
    {
        switch (direction)
        {
        case Up: Merge({ i,3 }, direction); break;
        case Down: Merge({ i,0 }, direction); break;
        case Left: Merge({ 0,i }, direction); break;
        case Right: Merge({ 3,i }, direction); break;
        };
    }
    if (isDefeat())
    {
        m_GameStatus = Defeat;
    }
    else
    {
        CreateBlock();
    }
}

bool GameControl::OnKeyTyped(Lambix::KeyReleasedEvent& e)
{
    if (m_GameStatus == Running)
    {
        switch (e.GetKeyCode())
        {
        case LB_KEY_UP:
            Move(Up);
            return true;
        case LB_KEY_DOWN:
            Move(Down);
            return true;
        case LB_KEY_LEFT:
            Move(Left);
            return true;
        case LB_KEY_RIGHT:
            Move(Right);
            return true;
        }
    }
    return false;
}

void GameControl::Merge(IndexPair start, MoveDirection direction)
{
    IndexPair current = start;
    IndexPair cmp = start;
    cmp.next(direction);

    while (1)
    {
        if (m_Data[cmp.x][cmp.y] == 0)
        {
            if (cmp.next(direction))
                continue;
            else break;
        }

        if (m_Data[current.x][current.y] == 0)
        {
            std::swap(m_Data[current.x][current.y], m_Data[cmp.x][cmp.y]);
        }
        else
        {
            if (m_Data[current.x][current.y] == m_Data[cmp.x][cmp.y])
            {
                m_Data[current.x][current.y] += m_Data[cmp.x][cmp.y];
                m_Data[cmp.x][cmp.y] = 0;
                if (m_Data[current.x][current.y] == 2048)
                {
                    m_GameStatus = Victory;
                }
            }
            if (current.next(direction))
            {
                cmp = current;
                if (cmp.next(direction))
                    continue;
                else break;
            }
        }
    }
}

void GameControl::UpdateEmptyBlock()
{
    m_EmptyBlock = 0;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            if (m_Data[i][j] == 0)
                m_EmptyBlock++;
        }
    }
}

bool GameControl::IndexPair::next(MoveDirection direction)
{
    switch (direction)
    {
    case Down: if (y + 1 < 4) { y++; return true; }
             break;
    case Up: if (y - 1 >= 0) { y--; return true; }
           break;
    case Left: if (x + 1 < 4) { x++; return true; }
             break;
    case Right: if (x - 1 >= 0) { x--; return true; }
              break;
    };
    return false;
}
