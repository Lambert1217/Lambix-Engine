#pragma once

#include "Lambix.h"

class GameControl
{
public:
    GameControl();
    ~GameControl() = default;

    enum GameStatus
    {
        Defeat = 0, Victory, Running
    };
    inline const int& GetData(int x, int y) const { return m_Data[x][y]; }
    inline const GameStatus GetGameStatus() const { return m_GameStatus; }
    void OnEvent(Lambix::Event& event);

private:
    enum MoveDirection
    {
        Up = 0, Down, Left, Right
    };

    struct IndexPair
    {
        int x, y;

        bool next(MoveDirection direction);
    };

    int m_Data[4][4];
    int m_EmptyBlock;
    GameStatus m_GameStatus;

private:
    void reset();
    void CreateBlock();
    bool isDefeat();
    void Merge(IndexPair start, MoveDirection direction);
    void UpdateEmptyBlock();
    void Move(MoveDirection direction);

    bool OnKeyTyped(Lambix::KeyReleasedEvent& e);
};