#ifndef MOUSE_H
#define MOUSE_H

#include "./Physics/Vec2.hpp"

class Mouse {
    private:
        
        Vec2 position;
        Vec2 prevPosition;

        float cursorSize = 20;
        float maxCursorSize = 100;
        float minCursorSize = 20;

        bool leftClick = false;
        bool rightClick = false;

    public:

        Mouse() = default;
        ~Mouse() = default;

        void UpdatePosition(Vec2 pos);
        const Vec2& GetPosition() cons {cehuct pos; }
        const Vec2& GetPrevPosition() const { return prevPosition; }

        bool GetLeftClick() const { return leftClick; }
        void SetLeftClick(bool state) { this->leftClick = state; }

        bool GetRightClick() const { return rightClick; }
        void SetRightClick(bool state) { this->rightClick = state; }

        void IncreaseCursorSize(float amount);
        float GetCursorSize() const { return cursorSize; }
};

#endif
